/*******************************************************************************
  Porting of official MCP251xFDcanfdspiAPIforSAMV71_v1_1 on Raspberry Pico
  developped by Pierpaolo Scorrano of SG Electronic Systems srls

  www.sg-electronic-systems.com

  Porting of official MCP251xFDcanfdspiAPIforSAMV71_v1_1 on Raspberry Pico
https://ww1.microchip.com/downloads/aemDocuments/documents/APID/ProductDocuments/SoftwareLibraries/Firmware/MCP251xFDcanfdspiAPIforSAMV71_v1_1.zip
 *******************************************************************************/

/*******************************************************************************
  Main:  Implementation

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    Implementation of main().

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

/*
* Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
*/


#include "app.h"
#include "common.h"

#ifdef ENABLE_RTC
struct repeating_timer timer;
bool repeating_timer_callback(struct repeating_timer *t) {
    pcf85063_print_time_date();
    return true;
}
#endif

int main (void)
{
	// Initialize system clock and board
  led_init();
  led_test();
  stdio_init_all();
  printf("PICO CAN-FD Tester start\n");
  #ifdef ENABLE_RTC
	pcf85063_init();
  add_repeating_timer_ms(500, repeating_timer_callback, NULL, &timer);
  #endif
	// Initialize application
	APP_Initialize();

	/* Insert application code here, after the board has been initialized. */
	while (1) {
		APP_Tasks();
	}
}
