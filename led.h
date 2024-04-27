/*******************************************************************************
  Porting of official MCP251xFDcanfdspiAPIforSAMV71_v1_1 on Raspberry Pico
  developped by Pierpaolo Scorrano of SG Electronic Systems srls

  www.sg-electronic-systems.com

  Porting of official MCP251xFDcanfdspiAPIforSAMV71_v1_1 on Raspberry Pico
https://ww1.microchip.com/downloads/aemDocuments/documents/APID/ProductDocuments/SoftwareLibraries/Firmware/MCP251xFDcanfdspiAPIforSAMV71_v1_1.zip
 *******************************************************************************/

#ifndef LED_H_
#define LED_H_

void led_init(void);

void led_on(void);

void led_off(void);

void led1_on(void);

void led1_off(void);

void led2_on(void);

void led2_off(void);

void led_test(void);

#endif /* LED_H_ */
