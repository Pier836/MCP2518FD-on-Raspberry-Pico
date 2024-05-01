/*******************************************************************************
  Porting of official MCP251xFDcanfdspiAPIforSAMV71_v1_1 on Raspberry Pico
  developped by Pierpaolo Scorrano of SG Electronic Systems srls

  www.sg-electronic-systems.com

  Porting of official MCP251xFDcanfdspiAPIforSAMV71_v1_1 on Raspberry Pico
https://ww1.microchip.com/downloads/aemDocuments/documents/APID/ProductDocuments/SoftwareLibraries/Firmware/MCP251xFDcanfdspiAPIforSAMV71_v1_1.zip
 *******************************************************************************/

#ifndef COMMON_H_
#define COMMON_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "led.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"
#include "pico/binary_info.h"
#include "drv_spi.h"
#include "drv_canfdspi_api.h"
#include "pcf85063.h"
#ifndef PICO_DEFAULT_LED_PIN
#include "pico/cyw43_arch.h"
#endif

#define MOSI_PIN	3   //RPI_PIN19 SPI0_TX
#define MISO_PIN	0   //RPI_PIN21 SPI0_RX
#define SCK_PIN		2	  //RPI_PIN23 SPI0_SCK
#define CS_FD1 		1   //RPI_PIN24 SPI0_CS0
#define CS_FD2 		9	  //RPI_PIN26 SPI0_CS1
#define INT_FD1 	26	//RPI_PIN18 GP26
#define INT_FD2 	12	//RPI_PIN22 IRQ2
#define LED_1 		18	//RPI_PIN13 LED1
#define LED_2 		19	//RPI_PIN7 LED2
#define SDA_PIN  	20	//RPI_PIN3 I2C_0_SDA
#define SCL_PIN  	21	//RPI_PIN5 I2C_0_SCL

#define ENABLE_RTC
#define ENABLE_CAN1
#define ENABLE_CAN2

#define CAN1	1
#define CAN2	2

#define SPI_DEVICE spi0

#define DRV_CANFDSPI_INDEX_0 0 

#define LED_COUNT 2

//#define STATE_MACHINE_DEBUG

#endif //COMMON_H_
