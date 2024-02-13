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

void on_usb_rx(void* data) {
	uint8_t* data_8 = (uint8_t*) data;
	uint8_t response[16] = {0};
	memset(response, 0, 16);
	uint8_t command_id = data_8[0];
	response[15] = 0x42;
	switch(command_id) {
		case CTL_GET_TMP_CNCT:
			// fill byte 1-4 with 1 if a temp probe is present
			break;
		case CTL_GET_FAN_CNCT:
			break;
		case CTL_GET_TMP:
			response[1] = 0x42;
			response[2] = 0x42;

		default:
			break;
	};
	for(int i = 1; i < 16; ++i){
		response[i] = 0xEE;
	}
	USBD_CUSTOM_HID_SendReport_FS(response, 16);
	//HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
}

int main() {
	init_cubemx();


	while(42) {
		__WFI();
	}

}
