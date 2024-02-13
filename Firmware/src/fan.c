#include <stdint.h>


#include "fan.h"

/**
 * Returns 1 if a 4-pin fan is detected. 0 otherwise
 */
static int fan_detect_4pin(GPIO_TypeDef* pwm_port, uint16_t pwm_pin) {
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

void init_fan(fan_handle_t* fan, TIM_HandleTypeDef* pwm_handle, uint32_t pwm_channel, TIM_HandleTypeDef* ic_handle, uint32_t ic_channel, uint32_t ic_active_channel, GPIO_TypeDef* pwm_port, uint16_t pwm_pin, GPIO_TypeDef* power_switch_port, uint16_t power_switch_pin) {
	// Enable fan power
	HAL_GPIO_WritePin(power_switch_port, power_switch_pin, GPIO_PIN_SET);
	HAL_TIM_Base_Start_IT(pwm_handle);
	HAL_TIM_PWM_Start(pwm_handle, pwm_channel);
	__HAL_TIM_SET_COMPARE(pwm_handle, pwm_channel, 0);

	HAL_TIM_Base_Start_IT(ic_handle);
	HAL_TIM_IC_Start_IT(ic_handle, ic_channel);

	fan->pwm_handle = pwm_handle;
	fan->pwm_channel = pwm_channel;
	fan->ic_handle = ic_handle;
	fan->ic_channel = ic_channel;
	fan->ic_channel_active = ic_active_channel;
	fan->is_4pin = fan_detect_4pin(pwm_port, pwm_pin);
	fan->power_switch_port = power_switch_port;
	fan->power_switch_pin = power_switch_pin;

	set_fan_pwm_percentage(fan, 25);
}

int get_fan_type(fan_handle_t* fan) {
	if(fan->rpm == 0) {
		return 0x00;
	} else if(fan->is_4pin) {
		return 0x02;
	} else {
		return 0x01;
	}
}

uint16_t get_fan_rpm(fan_handle_t* fan) {
	return fan->rpm;
}

uint16_t get_fan_pwm(fan_handle_t* fan) {
	return fan->pwm_percent;
}


void set_fan_pwm_percentage(fan_handle_t* fan, uint8_t pwm_percent) {
	volatile TIM_HandleTypeDef* handle = fan->pwm_handle;
	if(handle) {
		uint16_t pwm = 0;
		if(pwm_percent > 0) {
			pwm = handle->Init.Period * (pwm_percent/100.0f) + 0.5;
			__HAL_TIM_SET_COMPARE(handle, fan->pwm_channel, pwm);
			HAL_GPIO_WritePin(fan->power_switch_port, fan->power_switch_pin, GPIO_PIN_SET);
		} else {
			// Shut down fan
			HAL_GPIO_WritePin(fan->power_switch_port, fan->power_switch_pin, GPIO_PIN_RESET);
		}
	}
	fan->pwm_percent = pwm_percent;
}
