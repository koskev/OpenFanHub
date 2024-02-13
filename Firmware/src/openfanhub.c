
#include "stm32f1xx.h"
#include "STM32F1/Drivers/STM32F1xx_HAL_Driver/Inc/stm32f1xx_hal_flash.h"
#include <stm32f1xx_hal.h>
#include <stm32f1xx_hal_conf.h>
#include <main.h>

#include "STM32F1/Inc/usbd_custom_hid_if.h"

#include <gpio.h>

#include <stdint.h>
#include "xprintf.h"


extern int8_t USBD_CUSTOM_HID_SendReport_FS ( uint8_t *report,uint16_t len);



static uint8_t* USBD_HID_GetPos (void) {
	static int8_t HID_Buffer[9] = {0};
	return (uint8_t*)HID_Buffer;
}

#define LED_PORT                GPIOC
#define LED_PIN                 GPIO_PIN_13
 #define LED_PORT_CLK_ENABLE     __HAL_RCC_GPIOC_CLK_ENABLE




int main() {
	init_cubemx();


	while(42) {
		__WFI();
		//uint8_t* data = USBD_HID_GetPos();
		//USBD_CUSTOM_HID_SendReport_FS(data, 9);
	if ((HAL_GetTick() % 500) == 0)
		 HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
	}

}
