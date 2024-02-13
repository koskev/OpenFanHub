#include "stm32f1xx.h"
#include "STM32F1/Drivers/STM32F1xx_HAL_Driver/Inc/stm32f1xx_hal_flash.h"
#include <stm32f1xx_hal.h>
#include <stm32f1xx_hal_conf.h>
#include <main.h>

#include "STM32F1/Inc/usbd_custom_hid_if.h"

#include <gpio.h>

#include <stdint.h>
#include <string.h>

extern int8_t USBD_CUSTOM_HID_SendReport_FS ( uint8_t *report,uint16_t len);

enum CONTROL {
	CTL_GET_TMP_CNCT = 0x10,
	CTL_GET_TMP = 0x11,
	CTL_GET_FAN_CNCT = 0x20,
	CTL_GET_FAN_RPM = 0x21,
	CTL_GET_FAN_PWM = 0x22,
	CTL_SET_FAN_FPWM = 0x23,
	CTL_SET_FAN_TARGET = 0x24
};



#define LED_PORT                GPIOC
#define LED_PIN                 GPIO_PIN_13
#define LED_PORT_CLK_ENABLE     __HAL_RCC_GPIOC_CLK_ENABLE
#define RESPONSE_SIZE 16


int get_fan_type(int fan_id) {
	return 0x02;
}

uint16_t get_fan_rpm(int fan_id) {
	return 1500;
}


void on_usb_rx(void* data) {
	uint8_t* data_8 = (uint8_t*) data;
	static uint8_t response[RESPONSE_SIZE];
	memset(response, 0, RESPONSE_SIZE);
	uint8_t command_id = data_8[0];
	switch(command_id) {
		case CTL_GET_TMP_CNCT:
			// fill byte 1-4 with 1 if a temp probe is present
			break;
		case CTL_GET_FAN_CNCT:
			for(int i = 0; i < 6; ++i) {
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
		case CTL_GET_TMP:
			response[1] = 0x42;
			response[2] = 0x42;
			break;
		default:
			break;
	};
	USBD_CUSTOM_HID_SendReport_FS(response, 16);
	//HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
}

int main() {
	init_cubemx();


	while(42) {
		__WFI();
	}

}
