#include <stdint.h>


#include "fan.h"


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
