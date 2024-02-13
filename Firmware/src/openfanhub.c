#include "STM32F1/Drivers/STM32F1xx_HAL_Driver/Inc/stm32f1xx_hal_gpio.h"
#include "adc.h"
#include "stm32f1xx.h"
#include "STM32F1/Drivers/STM32F1xx_HAL_Driver/Inc/stm32f1xx_hal_flash.h"
#include <cmsis_gcc.h>
#include <stm32f103xb.h>
#include <stm32f1xx_hal.h>
#include <stm32f1xx_hal_conf.h>
#include <main.h>
#include <adc.h>
#include <dma.h>
#include <i2c.h>
#include <tim.h>
#include <usart.h>
#include <usb_device.h>
#include <gpio.h>

#include "STM32F1/Inc/usbd_custom_hid_if.h"
#include "stm32f1xx_hal_adc.h"
#include "stm32f1xx_hal_tim.h"

#include <gpio.h>

#include <stdint.h>
#include <string.h>

#include "config.h"

#include "lib/logf.h"

#include "fan.h"

volatile uint16_t temps[4] = {0};

/**
 * Notes Fan:
 * PWM
 * https://www.glkinst.com/cables/cable_pics/4_Wire_PWM_Spec.pdf
 * https://noctua.at/pub/media/wysiwyg/Noctua_PWM_specifications_white_paper.pdf
 * 	25kHz, acceptable range 21kHz to 28kHz
 * 		freq doesn't seem to matter at all. even worked with a 18hz signal
 * 	max: 5,25V 
 * 	low: 0.8V
 * 		3.3V PWM works fine
 * 	pulled high
 * 		max 5.25v. rec 3.3v
 * 	fan pulls tacho signal low on pass
 * 		pull up for desired voltage
 *
 *
 */

extern int8_t USBD_CUSTOM_HID_SendReport_FS ( uint8_t *report,uint16_t len);
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern ADC_HandleTypeDef hadc1;

enum CONTROL {
	CTL_GET_TMP_CNCT = 0x10,
	CTL_GET_TMP = 0x11,
	CTL_GET_FAN_CNCT = 0x20,
	CTL_GET_FAN_RPM = 0x21,
	CTL_GET_FAN_PWM = 0x22,
	CTL_SET_FAN_FPWM = 0x23,
	CTL_SET_FAN_TARGET = 0x24
};

#define RESPONSE_SIZE 16
#define NUM_FANS 6 // limited by the driver
#define NUM_TEMP_SENSORS 4


static fan_handle_t fans[NUM_FANS];


float steinhart(int res, float a0, float a1, float a2) {
	float res_log = logf(res);
	return 1. / (a0 + a1 * res_log + a2 * res_log*res_log);
}

uint16_t get_temp(int temp_id) {
	int voltage_mv = ((float)ADC_VREF/(1 << ADC_BITS)) * temps[temp_id]; 
	int resistance = TEMP_R1 * (1/(((float)ADC_VREF/voltage_mv) - 1));
	float temp_k = steinhart(resistance, TEMP1_COEFFA, TEMP1_COEFFB, TEMP1_COEFFC);
	return (temp_k - 275.15)*100;
}


void on_usb_rx(void* data) {
	uint8_t* data_8 = (uint8_t*) data;
	static uint8_t response[RESPONSE_SIZE];
	memset(response, 0, RESPONSE_SIZE);
	uint8_t command_id = data_8[0];
	switch(command_id) {
		case CTL_GET_TMP_CNCT:
			// fill byte 1-4 with 1 if a temp probe is present
			for(int i = 0; i < NUM_TEMP_SENSORS; ++i) {
				response[i+1] = 0x01;
			}
			break;
		case CTL_GET_FAN_CNCT:
			for(int i = 0; i < NUM_FANS; ++i) {
				response[i+1] = get_fan_type(&fans[i]);
			}
			break;
		case CTL_GET_FAN_RPM:
			{
				uint16_t rpm = get_fan_rpm(&fans[data_8[1]]);
				response[1] = rpm >> 8;
				response[2] = rpm;

			}
			break;
		case CTL_GET_FAN_PWM:
			{
				uint16_t pwm = get_fan_pwm(&fans[data_8[1]]);
				response[1] = pwm;
			}
			break;
		case CTL_SET_FAN_FPWM:
			{
				uint8_t fan_num = data_8[1];
				uint8_t percentage = data_8[2];
				set_fan_pwm_percentage(&fans[fan_num], percentage);
			}
			break;
		case CTL_GET_TMP:
			{
				uint16_t temp = get_temp(data_8[1]);
				response[1] = temp >> 8;
				response[2] = temp;
			}
			break;
		default:
			break;
	};
	USBD_CUSTOM_HID_SendReport_FS(response, 16);
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
}



void init_fans() {
	//TODO: yeah I hate it
	init_fan(&fans[0], FAN1_PWM_TIMER, FAN1_PWM_CHANNEL, FAN1_IC_TIMER, FAN1_IC_CHANNEL, FAN1_IC_CHANNEL_ACTIVE, FAN1_PWM_PORT, FAN1_PWM_PIN, FAN1_POWER_SWITCH_PORT, FAN1_POWER_SWITCH_PIN);
	init_fan(&fans[1], FAN2_PWM_TIMER, FAN2_PWM_CHANNEL, FAN2_IC_TIMER, FAN2_IC_CHANNEL, FAN2_IC_CHANNEL_ACTIVE, FAN2_PWM_PORT, FAN2_PWM_PIN, FAN2_POWER_SWITCH_PORT, FAN2_POWER_SWITCH_PIN);
	init_fan(&fans[2], FAN3_PWM_TIMER, FAN3_PWM_CHANNEL, FAN3_IC_TIMER, FAN3_IC_CHANNEL, FAN3_IC_CHANNEL_ACTIVE, FAN3_PWM_PORT, FAN3_PWM_PIN, FAN3_POWER_SWITCH_PORT, FAN3_POWER_SWITCH_PIN);
	init_fan(&fans[3], FAN4_PWM_TIMER, FAN4_PWM_CHANNEL, FAN4_IC_TIMER, FAN4_IC_CHANNEL, FAN4_IC_CHANNEL_ACTIVE, FAN4_PWM_PORT, FAN4_PWM_PIN, FAN4_POWER_SWITCH_PORT, FAN4_POWER_SWITCH_PIN);
	init_fan(&fans[4], FAN5_PWM_TIMER, FAN5_PWM_CHANNEL, FAN5_IC_TIMER, FAN5_IC_CHANNEL, FAN5_IC_CHANNEL_ACTIVE, FAN5_PWM_PORT, FAN5_PWM_PIN, FAN5_POWER_SWITCH_PORT, FAN5_POWER_SWITCH_PIN);
	init_fan(&fans[5], FAN6_PWM_TIMER, FAN6_PWM_CHANNEL, FAN6_IC_TIMER, FAN6_IC_CHANNEL, FAN6_IC_CHANNEL_ACTIVE, FAN6_PWM_PORT, FAN6_PWM_PIN, FAN6_POWER_SWITCH_PORT, FAN6_POWER_SWITCH_PIN);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
	for(int i = 0; i < NUM_FANS; ++i) {
		volatile fan_handle_t *fan = &fans[i];
		if(fan->ic_handle->Instance == htim->Instance && fan->ic_channel_active == htim->Channel) {

			uint32_t curr_val = HAL_TIM_ReadCapturedValue(fan->ic_handle, fan->ic_channel);
			uint32_t diff = (curr_val+fan->ic_overflow*htim->Init.Period) - fan->last_val;

			if(diff != 0) {
				uint32_t freq = (72000000.0f/htim->Init.Prescaler)/diff;
				fan->rpm = freq * 30; // 2 pulses = one rev
			}

			fan->last_val = curr_val;
			fan->ic_overflow = 0;
			break;
		}
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
	for(int i = 0; i < NUM_FANS; ++i) {
		volatile fan_handle_t *fan = &fans[i];
		if(fan->ic_handle->Instance == htim->Instance) {
			fans[i].ic_overflow +=1;
			if(fans[i].ic_overflow > 5) {
				fans[i].rpm = 0;
			}
		}
	}
}

void read_adc() {
	//HAL_ADC_Start_IT(&hadc1);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&temps, 4);
}

void init_system_clock(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
	
	/** Initializes the RCC Oscillators according to the specified parameters
	* in the RCC_OscInitTypeDef structure.
	*/
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
	  Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	*/
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
	                            |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	
	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	{
	  Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC|RCC_PERIPHCLK_USB;
	PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
	PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
	{
	  Error_Handler();
	}
}

void init() {
	HAL_Init();

	init_system_clock();
	MX_GPIO_Init();
	MX_TIM1_Init();
	MX_TIM2_Init();
	MX_TIM3_Init();
	MX_TIM4_Init();
	MX_USB_DEVICE_Init();
	MX_DMA_Init();
	MX_ADC1_Init();
	MX_I2C2_Init();
	MX_USART2_UART_Init();
	
}

int main() {
	for(int i = 0; i < sizeof(fan_handle_t) * NUM_FANS; ++i) {
		*((volatile uint8_t*)fans+i) = 0;
	}
	init();
	HAL_ADCEx_Calibration_Start(&hadc1);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&temps, 4);
	
	
	/**
	 * TODO:
	 * 	push usb commands in a queue
	 * 		less volatile needed
	 * 		same possible for ic?
	 */
	init_fans();
	
	while(42) {
		__WFI();
	}
}

