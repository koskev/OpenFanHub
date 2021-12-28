#include "STM32F1/Drivers/STM32F1xx_HAL_Driver/Inc/stm32f1xx_hal_gpio.h"
#include "stm32f1xx.h"
#include "STM32F1/Drivers/STM32F1xx_HAL_Driver/Inc/stm32f1xx_hal_flash.h"
#include <stm32f103xb.h>
#include <stm32f1xx_hal.h>
#include <stm32f1xx_hal_conf.h>
#include <main.h>

#include "STM32F1/Inc/usbd_custom_hid_if.h"
#include "stm32f1xx_hal_tim.h"

#include <gpio.h>

#include <stdint.h>
#include <string.h>

#include "config.h"

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
	if(fan_id >= NUM_FANS) return;
	//current_pwm = DIV_ROUND(MAX_PWM * pwm_percent, 100);
	volatile TIM_HandleTypeDef* handle = fans[fan_id].pwm_handle;
	if(handle) {
		uint16_t pwm = 0;
		if(pwm_percent > 0) {
			pwm = handle->Init.Period * (pwm_percent/100.0f) + 0.5;
		}
		__HAL_TIM_SET_COMPARE(handle, fans[fan_id].pwm_channel, pwm);
	}
	fans[fan_id].pwm_percent = pwm_percent;
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

void init_fan(int id, TIM_HandleTypeDef* pwm_handle, uint32_t pwm_channel, TIM_HandleTypeDef* ic_handle, uint32_t ic_channel, uint32_t ic_active_channel, GPIO_TypeDef* pwm_port, uint16_t pwm_pin) {
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

	set_fan_pwm_percentage(id, 50);
}

void init_fans() {
	//init_fan(0, &htim2, TIM_CHANNEL_1, &htim1, TIM_CHANNEL_1, HAL_TIM_ACTIVE_CHANNEL_1, GPIOA, GPIO_PIN_0);
	init_fan(0, FAN1_PWM_TIMER, FAN1_PWM_CHANNEL, FAN1_IC_TIMER, FAN1_IC_CHANNEL, FAN1_IC_CHANNEL_ACTIVE, FAN1_PWM_PORT, FAN1_PWM_PIN);
	init_fan(1, FAN2_PWM_TIMER, FAN2_PWM_CHANNEL, FAN2_IC_TIMER, FAN2_IC_CHANNEL, FAN2_IC_CHANNEL_ACTIVE, FAN2_PWM_PORT, FAN2_PWM_PIN);
	init_fan(3, FAN3_PWM_TIMER, FAN3_PWM_CHANNEL, FAN3_IC_TIMER, FAN3_IC_CHANNEL, FAN3_IC_CHANNEL_ACTIVE, FAN3_PWM_PORT, FAN3_PWM_PIN);
	init_fan(4, FAN4_PWM_TIMER, FAN4_PWM_CHANNEL, FAN4_IC_TIMER, FAN4_IC_CHANNEL, FAN4_IC_CHANNEL_ACTIVE, FAN4_PWM_PORT, FAN4_PWM_PIN);
	init_fan(5, FAN5_PWM_TIMER, FAN5_PWM_CHANNEL, FAN5_IC_TIMER, FAN5_IC_CHANNEL, FAN5_IC_CHANNEL_ACTIVE, FAN5_PWM_PORT, FAN5_PWM_PIN);
	init_fan(6, FAN6_PWM_TIMER, FAN6_PWM_CHANNEL, FAN6_IC_TIMER, FAN6_IC_CHANNEL, FAN6_IC_CHANNEL_ACTIVE, FAN6_PWM_PORT, FAN6_PWM_PIN);
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
	
	/**
	 * TODO:
	 * 	add disable pin
	 * 		make it work with 3 and 4 pin fans
	 * 	push usb commands in a queue
	 * 		less volatile needed
	 * 		same possible for ic?
	 */
	init_fans();
	
	while(42) {
		__WFI();
	}

}
