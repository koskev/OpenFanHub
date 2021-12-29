#ifndef __FAN_H__
#define __FAN_H__

#include <stdint.h>
#include <stm32f1xx_hal_conf.h>

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

int get_fan_type(fan_handle_t* fan);
uint16_t get_fan_rpm(fan_handle_t* fan);
uint16_t get_fan_pwm(fan_handle_t* fan);
void set_fan_pwm_percentage(fan_handle_t* fan, uint8_t pwm_percent);


#endif
