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


#define DIV_ROUND(dividend, divisor) (\
		((dividend - 1) > 0 || dividend > 0) ? \
		((dividend + (divisor / 2)) / divisor) : \
		((dividend - (divisor / 2)) / divisor) \
		)



#define LED_PORT                GPIOC
#define LED_PIN                 GPIO_PIN_13
#define LED_PORT_CLK_ENABLE     __HAL_RCC_GPIOC_CLK_ENABLE
#define RESPONSE_SIZE 16
#define NUM_FANS 6 // limited by the driver
#define NUM_TEMP_SENSORS 4


#define MAX_PWM 255
#define MAX_RPM 3000

typedef struct fan_handle_s {
	volatile int pwm_percent;
	volatile int rpm;
	int is_4pin;
	int fan_detected;

	volatile TIM_HandleTypeDef* pwm_handle;
	volatile uint32_t pwm_channel;

	TIM_HandleTypeDef* ic_handle;
	uint32_t ic_channel;
	uint32_t ic_channel_active;

	GPIO_TypeDef* power_switch_port;
	uint16_t power_switch_pin;

	// for input capture
	volatile uint32_t ic_overflow;
	volatile uint32_t last_val;

} fan_handle_t;

volatile static fan_handle_t fans[NUM_FANS];

int get_fan_type(int fan_id) {
	if(fans[fan_id].rpm == 0) {
		return 0x00;
	} else if(fans[fan_id].is_4pin) {
		return 0x02;
	} else {
		return 0x01;
	}
}

uint16_t get_fan_rpm(int fan_id) {
	return fans[fan_id].rpm;
}

uint16_t get_fan_pwm(int fan_id) {
	return fans[fan_id].pwm_percent;
}


void set_fan_pwm_percentage(uint8_t fan_id, uint8_t pwm_percent) {
	if(fan_id >= NUM_FANS) return;
	//current_pwm = DIV_ROUND(MAX_PWM * pwm_percent, 100);
	volatile TIM_HandleTypeDef* handle = fans[fan_id].pwm_handle;
	if(handle) {
		uint16_t pwm = 0;
		if(pwm_percent > 0) {
			pwm = handle->Init.Period * (pwm_percent/100.0f) + 0.5;
			__HAL_TIM_SET_COMPARE(handle, fans[fan_id].pwm_channel, pwm);
			HAL_GPIO_WritePin(fans[fan_id].power_switch_port, fans[fan_id].power_switch_pin, GPIO_PIN_SET);
		} else {
			// Shut down fan
			HAL_GPIO_WritePin(fans[fan_id].power_switch_port, fans[fan_id].power_switch_pin, GPIO_PIN_RESET);
		}
	}
	fans[fan_id].pwm_percent = pwm_percent;
}

#define ADC_VREF 3300
#define ADC_BITS 12
#define TEMP_R1 10000
#define TEMP1_COEFFA 1.02440469e-03
#define TEMP1_COEFFB 2.06924335e-04
#define TEMP1_COEFFC 5.07042776e-06

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
				response[i+1] = get_fan_type(i);
			}
			break;
		case CTL_GET_FAN_RPM:
			{
				uint16_t rpm = get_fan_rpm(data_8[1]);
				response[1] = rpm >> 8;
				response[2] = rpm;

			}
			break;
		case CTL_GET_FAN_PWM:
			{
				uint16_t pwm = get_fan_pwm(data_8[1]);
				response[1] = pwm;
				//response[1] = (pwm*100.0f) / MAX_PWM + 0.5f;
			}
			break;
		case CTL_SET_FAN_FPWM:
			{
				uint8_t fan_num = data_8[1];
				uint8_t percentage = data_8[2];
				set_fan_pwm_percentage(fan_num, percentage);
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

/**
 * Returns 1 if a 4-pin fan is detected. 0 otherwise
 */
int fan_detect_4pin(GPIO_TypeDef* pwm_port, uint16_t pwm_pin) {
	int ret = 0;
	// enable input
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = pwm_pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(pwm_port, &GPIO_InitStruct);
	
	// read if is high
	GPIO_PinState state = HAL_GPIO_ReadPin(pwm_port, pwm_pin);

	if(state == GPIO_PIN_SET) {
		ret = 1;
	}
	
	// enable output again with prev stats
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(pwm_port, &GPIO_InitStruct);
	
	return ret;
}

void init_fan(int id, TIM_HandleTypeDef* pwm_handle, uint32_t pwm_channel, TIM_HandleTypeDef* ic_handle, uint32_t ic_channel, uint32_t ic_active_channel, GPIO_TypeDef* pwm_port, uint16_t pwm_pin, GPIO_TypeDef* power_switch_port, uint16_t power_switch_pin) {
	// Enable fan power
	HAL_GPIO_WritePin(power_switch_port, power_switch_pin, GPIO_PIN_SET);
	HAL_TIM_Base_Start_IT(pwm_handle);
	HAL_TIM_PWM_Start(pwm_handle, pwm_channel);
	__HAL_TIM_SET_COMPARE(pwm_handle, pwm_channel, 0);

	HAL_TIM_Base_Start_IT(ic_handle);
	HAL_TIM_IC_Start_IT(ic_handle, ic_channel);

	fans[id].pwm_handle = pwm_handle;
	fans[id].pwm_channel = pwm_channel;
	fans[id].ic_handle = ic_handle;
	fans[id].ic_channel = ic_channel;
	fans[id].ic_channel_active = ic_active_channel;
	fans[id].is_4pin = fan_detect_4pin(pwm_port, pwm_pin);
	fans[id].power_switch_port = power_switch_port;
	fans[id].power_switch_pin = power_switch_pin;


	set_fan_pwm_percentage(id, 25);
}

void init_fans() {
	//TODO: yeah I hate it
	init_fan(0, FAN1_PWM_TIMER, FAN1_PWM_CHANNEL, FAN1_IC_TIMER, FAN1_IC_CHANNEL, FAN1_IC_CHANNEL_ACTIVE, FAN1_PWM_PORT, FAN1_PWM_PIN, FAN1_POWER_SWITCH_PORT, FAN1_POWER_SWITCH_PIN);
	init_fan(1, FAN2_PWM_TIMER, FAN2_PWM_CHANNEL, FAN2_IC_TIMER, FAN2_IC_CHANNEL, FAN2_IC_CHANNEL_ACTIVE, FAN2_PWM_PORT, FAN2_PWM_PIN, FAN2_POWER_SWITCH_PORT, FAN2_POWER_SWITCH_PIN);
	init_fan(2, FAN3_PWM_TIMER, FAN3_PWM_CHANNEL, FAN3_IC_TIMER, FAN3_IC_CHANNEL, FAN3_IC_CHANNEL_ACTIVE, FAN3_PWM_PORT, FAN3_PWM_PIN, FAN3_POWER_SWITCH_PORT, FAN3_POWER_SWITCH_PIN);
	init_fan(3, FAN4_PWM_TIMER, FAN4_PWM_CHANNEL, FAN4_IC_TIMER, FAN4_IC_CHANNEL, FAN4_IC_CHANNEL_ACTIVE, FAN4_PWM_PORT, FAN4_PWM_PIN, FAN4_POWER_SWITCH_PORT, FAN4_POWER_SWITCH_PIN);
	init_fan(4, FAN5_PWM_TIMER, FAN5_PWM_CHANNEL, FAN5_IC_TIMER, FAN5_IC_CHANNEL, FAN5_IC_CHANNEL_ACTIVE, FAN5_PWM_PORT, FAN5_PWM_PIN, FAN5_POWER_SWITCH_PORT, FAN5_POWER_SWITCH_PIN);
	init_fan(5, FAN6_PWM_TIMER, FAN6_PWM_CHANNEL, FAN6_IC_TIMER, FAN6_IC_CHANNEL, FAN6_IC_CHANNEL_ACTIVE, FAN6_PWM_PORT, FAN6_PWM_PIN, FAN6_POWER_SWITCH_PORT, FAN6_POWER_SWITCH_PIN);
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

