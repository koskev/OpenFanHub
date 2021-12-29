CMAKE_MINIMUM_REQUIRED(VERSION 3.2)

SET(STM32_CHIP STM32F103C8)
SET(EXTRA_FIND_PATH /)
SET(CMAKE_BUILD_TYPE Release)
SET(STM32_CUBE_F1_PATH ~/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.4)
#SET(STM32_CUBE_F1_PATH ${CMAKE_CURRENT_SOURCE_DIR}/STM32F1)


SET(CMAKE_TOOLCHAIN_FILE ~/cmake/stm32/stm32_gcc.cmake)

PROJECT(OpenFanHub C ASM)


FIND_PACKAGE(CMSIS COMPONENTS STM32F1 REQUIRED)
find_package(HAL COMPONENTS 
	ADC
	USB
	PCD
	RCC
	GPIO
	CORTEX
	TIM
	UART
	I2C
	DMA

	STM32F1
	REQUIRED)

set(STM32HAL_USB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/STM32F1/Middlewares/ST/STM32_USB_Device_Library)

message(STATUS "${CMSIS_INCLUDE_DIRS} ${HAL_INCLUDE_DIRS} ${HAL_LIBRARIES}")
INCLUDE_DIRECTORIES(
	${CMSIS_INCLUDE_DIRS}
	${HAL_INCLUDE_DIRS}
	${CMAKE_CURRENT_SOURCE_DIR}/STM32F1/Inc
	${CMAKE_CURRENT_SOURCE_DIR}/STM32F1/Drivers/STM32F1xx_HAL_Driver/Inc
	${STM32HAL_USB_DIR}/Core/Inc
	${STM32HAL_USB_DIR}/Class/CustomHID/Inc
	hid_def/include
	)

SET(PROJECT_SOURCES
	openfanhub.c
	xprintf.c
	lib/logf.c
	./STM32F1/Src/main.c
	./STM32F1/Src/tim.c
	./STM32F1/Src/adc.c
	./STM32F1/Src/gpio.c
	./STM32F1/Src/usart.c
	./STM32F1/Src/i2c.c
	./STM32F1/Src/dma.c
	./STM32F1/Src/usb_device.c
	./STM32F1/Src/usbd_conf.c
	./STM32F1/Src/usbd_desc.c
	./STM32F1/Src/usbd_custom_hid_if.c
	./STM32F1/Src/stm32f1xx_it.c
	./STM32F1/Src/stm32f1xx_hal_msp.c
	
	#./STM32F1/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.c
	#./STM32F1/Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio_ex.c
	${STM32HAL_USB_DIR}/Core/Src/usbd_ctlreq.c
	${STM32HAL_USB_DIR}/Core/Src/usbd_core.c
	${STM32HAL_USB_DIR}/Core/Src/usbd_ctlreq.c
	${STM32HAL_USB_DIR}/Core/Src/usbd_ioreq.c
	${STM32HAL_USB_DIR}/Class/CustomHID/Src/usbd_customhid.c

	#
	)


add_executable(openfanhub ${PROJECT_SOURCES})
target_link_libraries(openfanhub
	HAL::STM32::F1::RCC
	HAL::STM32::F1::RCCEx
	HAL::STM32::F1::GPIO
	HAL::STM32::F1::GPIOEx
	HAL::STM32::F1::CORTEX
	HAL::STM32::F1::TIM
	HAL::STM32::F1::TIMEx
	HAL::STM32::F1::ADC
	HAL::STM32::F1::ADCEx
	HAL::STM32::F1::LL_USB
	HAL::STM32::F1::PCD
	HAL::STM32::F1::PCDEx
	HAL::STM32::F1::UART
	HAL::STM32::F1::I2C
	HAL::STM32::F1::DMA

	CMSIS::STM32::F103C8
	STM32::NoSys
	)

stm32_print_size_of_target(openfanhub)
