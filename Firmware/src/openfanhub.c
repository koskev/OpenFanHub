#include "stm32f1xx.h"
#include "STM32F1/Drivers/STM32F1xx_HAL_Driver/Inc/stm32f1xx_hal_flash.h"
#include <stm32f1xx_hal.h>
#include <stm32f1xx_hal_conf.h>
#include <main.h>

#include "STM32F1/Inc/usbd_custom_hid_if.h"
#include "stm32f1xx_hal_tim.h"

#include <gpio.h>

#include <stdint.h>
#include <string.h>

/**
 *
 *  TODO:
 *  	Actual PWM value differs from requested one, skipping. Requested: 1 Actual: 0
  DEBUG   Setting Fan PWM of 'hub_fan4' to 2 ...
  DEBUG   Actual PWM value differs from requested one, skipping. Requested: 2 Actual: 0
 */

/**
 * Notes Fan:
 * PWM
 * https://www.glkinst.com/cables/cable_pics/4_Wire_PWM_Spec.pdf
 * https://noctua.at/pub/media/wysiwyg/Noctua_PWM_specifications_white_paper.pdf
 * 	25kHz, acceptable range 21kHz to 28kHz
 * 	max: 5,25V 
 * 	low: 0.8V
 * 		3.3V PWM should be fine
 *
 *
 */

extern int8_t USBD_CUSTOM_HID_SendReport_FS ( uint8_t *report,uint16_t len);
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;

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
	uint8_t pwm_percent;
	volatile uint16_t rpm;

	TIM_HandleTypeDef* pwm_handle;
	uint32_t pwm_channel;

	TIM_HandleTypeDef* ic_handle;
	uint32_t ic_channel;
	uint32_t ic_channel_active;

	// for input capture
	volatile uint32_t ic_overflow;
	volatile uint32_t last_val;

} fan_handle_t;

volatile static fan_handle_t fans[NUM_FANS];

int get_fan_type(int fan_id) {
	return 0x02;
}

uint16_t get_fan_rpm(int fan_id) {
	return fans[fan_id].rpm;
}

uint16_t get_fan_pwm(int fan_id) {
	return fans[fan_id].pwm_percent;
}


void set_fan_pwm_percentage(uint8_t fan_id, uint8_t pwm_percent) {
	uint16_t pwm = MAX_PWM * (pwm_percent/100.0f) + 0.5;
	//current_pwm = DIV_ROUND(MAX_PWM * pwm_percent, 100);
	TIM_HandleTypeDef* handle = fans[fan_id].pwm_handle;
	if(handle) {
		__HAL_TIM_SET_COMPARE(handle, fans[fan_id].pwm_channel, pwm);
	}
}

uint16_t get_temp(int temp_id) {
	return 5200;
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

void init_fan(int id, TIM_HandleTypeDef* pwm_handle, uint32_t pwm_channel, TIM_HandleTypeDef* ic_handle, uint32_t ic_channel, uint32_t ic_active_channel) {
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

	fans[id].rpm = 42;
}

void init_fans() {
	init_fan(0, &htim2, TIM_CHANNEL_1, &htim4, TIM_CHANNEL_4, HAL_TIM_ACTIVE_CHANNEL_4);
	//TODO: other fans
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
	for(int i = 0; i < NUM_FANS; ++i) {
		volatile fan_handle_t *fan = &fans[i];
		// XXX: Not sure if we need to compare instance or htim directly. better be safe
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
		fan->rpm = 1;

	}

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
	for(int i = 0; i < NUM_FANS; ++i) {
		volatile fan_handle_t *fan = &fans[i];
		if(fan->ic_handle->Instance == htim->Instance) {
			fans[i].ic_overflow +=1;
		}
	}
}

int main() {
	for(int i = 0; i < sizeof(fan_handle_t) * NUM_FANS; ++i) {
		*((volatile uint8_t*)fans+i) = 0;
	}
	init_cubemx();
	
	init_fans();
	
	set_fan_pwm_percentage(0, 50);
	set_fan_pwm_percentage(1, 50);


	while(42) {
		__WFI();
	}

}
