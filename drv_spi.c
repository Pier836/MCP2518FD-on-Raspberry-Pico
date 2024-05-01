/*******************************************************************************
  Porting of official MCP251xFDcanfdspiAPIforSAMV71_v1_1 on Raspberry Pico
  developped by Pierpaolo Scorrano of SG Electronic Systems srls

  www.sg-electronic-systems.com

  Porting of official MCP251xFDcanfdspiAPIforSAMV71_v1_1 on Raspberry Pico
https://ww1.microchip.com/downloads/aemDocuments/documents/APID/ProductDocuments/SoftwareLibraries/Firmware/MCP251xFDcanfdspiAPIforSAMV71_v1_1.zip
 *******************************************************************************/


/*******************************************************************************
  SPI Driver:  Implementation

  Company:
    Microchip Technology Inc.

  File Name:
    drv_spi.c

  Summary:
    Implementation of MCU specific SPI functions.

  Description:
    .
 *******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2018 Microchip Technology Inc. and its subsidiaries.

Subject to your compliance with these terms, you may use Microchip software and 
any derivatives exclusively with Microchip products. It is your responsibility 
to comply with third party license terms applicable to your use of third party 
software (including open source software) that may accompany Microchip software.

THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER EXPRESS, 
IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES 
OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE.

IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER 
RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF 
THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE FULLEST EXTENT ALLOWED 
BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO 
THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE PAID 
DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *******************************************************************************/
//DOM-IGNORE-END

// Include files
#include "drv_spi.h"
#include "common.h"


void test_spi_pins(void)
{
    gpio_init(MOSI_PIN);
    gpio_set_dir(MOSI_PIN, GPIO_OUT);
    gpio_init(MISO_PIN);
    gpio_set_dir(MISO_PIN, GPIO_OUT);
    gpio_init(SCK_PIN);
    gpio_set_dir(SCK_PIN, GPIO_OUT);
    gpio_init(CS_FD1);
    gpio_set_dir(CS_FD1, GPIO_OUT);
    gpio_put(MOSI_PIN, 1);
    gpio_put(MISO_PIN, 1);
    gpio_put(SCK_PIN, 1);
    gpio_put(CS_FD1, 1);
    sleep_ms(250);
    gpio_put(MOSI_PIN, 0);
    sleep_ms(250);

   // gpio_put(MISO_PIN, 1);
    sleep_ms(250);
    gpio_put(MISO_PIN, 0);
    sleep_ms(250);
 
   // gpio_put(SCK_PIN, 1);
    sleep_ms(250);
    gpio_put(SCK_PIN, 0);
    sleep_ms(250);

  //  gpio_put(CS_FD1, 1);
    sleep_ms(250);
    gpio_put(CS_FD1, 0);
    sleep_ms(250);
    gpio_put(MOSI_PIN, 1);
    gpio_put(MISO_PIN, 1);
    gpio_put(SCK_PIN, 1);
    gpio_put(CS_FD1, 1);
}

void set_spi_pins(void)
{
    gpio_init(MOSI_PIN);
    gpio_set_dir(MOSI_PIN, GPIO_OUT);
    gpio_put(MOSI_PIN, 1);
    gpio_init(MISO_PIN);
    gpio_set_dir(MISO_PIN, GPIO_IN);
    gpio_init(SCK_PIN);
    gpio_set_dir(SCK_PIN, GPIO_OUT);
    gpio_put(SCK_PIN, 1);
    gpio_init(CS_FD1);
    gpio_set_dir(CS_FD1, GPIO_OUT);
    gpio_put(CS_FD1, 1);
    gpio_init(CS_FD2);
    gpio_set_dir(CS_FD2, GPIO_OUT);
    gpio_put(CS_FD2, 1);
}


void DRV_SPI_Initialize(void)
{
	spi_master_init();
}

int8_t DRV_SPI_TransferData(uint8_t spiSlaveDeviceIndex, uint8_t *SpiTxData, uint8_t *SpiRxData, uint16_t spiTransferSize)
{

	if (spiSlaveDeviceIndex == CAN1)
	{
		gpio_put(CS_FD1, 0); // Select MCP2518FD
		spi_write_read_blocking(SPI_DEVICE, SpiTxData, SpiRxData, spiTransferSize);
		gpio_put(CS_FD1, 1); // Deselect MCP2518FD	
	}
	else if (spiSlaveDeviceIndex == CAN2)
	{
		gpio_put(CS_FD2, 0); // Select MCP2518FD
		spi_write_read_blocking(SPI_DEVICE, SpiTxData, SpiRxData, spiTransferSize);
		gpio_put(CS_FD2, 1); // Deselect MCP2518FD	
	}

  return 0;
}

void spi_master_init(void)
{
   // These are the defaults anyway:
    spi_set_format(SPI_DEVICE, 8U, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
    //
    // CAN FD board is clocked at 40MHz, so SPI must be no more than 17000000: FSCK must
    // be less than or equal to 0.85 * (FSYSCLK/2). Other boards may use different clocks for
    // the CAN controller and this should be reflected here.
    spi_init(SPI_DEVICE, 17000000);
    gpio_set_function(MISO_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(MOSI_PIN, GPIO_FUNC_SPI);
    gpio_pull_up(MOSI_PIN);
    gpio_pull_up(MISO_PIN);

    // Set the chip select pin for the MCP25xxFD as a GPIO port
    gpio_set_function(CS_FD1, GPIO_FUNC_SIO);
    gpio_set_function(CS_FD2, GPIO_FUNC_SIO);

}

//#define USE_SPI_FUNCTIONS


