
#include "stm32f1xx.h"
#include "STM32F1/Drivers/STM32F1xx_HAL_Driver/Inc/stm32f1xx_hal_flash.h"
#include <stm32f1xx_hal_conf.h>
#include <main.h>

#include "STM32F1/Inc/usbd_custom_hid_if.h"

#include <stdint.h>
#include "xprintf.h"


extern int8_t USBD_CUSTOM_HID_SendReport_FS ( uint8_t *report,uint16_t len);



static uint8_t* USBD_HID_GetPos (void) {
	static int8_t HID_Buffer[9] = {0};
	return (uint8_t*)HID_Buffer;
}

void send_command(int command, const void *message)
{
   asm("mov r0, %[cmd];"
       "mov r1, %[msg];"
       "bkpt #0xAB"
         :
         : [cmd] "r" (command), [msg] "r" (message)
         : "r0", "r1", "memory");
}

// dirty hack but its faster
void serial_putc( uint8_t c ){
	static char buf[512];
	static uint16_t buffPos = 0;

	buf[buffPos++] = c;

	if(c == '\n' || buffPos >510){
		buf[buffPos++] = 0;
		send_command(0x04, buf);
		buffPos = 0;
	}

}


int main() {
	init_cubemx();

	while(42) {
		HAL_Delay(200);
		uint8_t* data = USBD_HID_GetPos();
		USBD_CUSTOM_HID_SendReport_FS(data, 9);
	}

}
