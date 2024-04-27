/*******************************************************************************
  Porting of official MCP251xFDcanfdspiAPIforSAMV71_v1_1 on Raspberry Pico
  developped by Pierpaolo Scorrano of SG Electronic Systems srls

  www.sg-electronic-systems.com

  Porting of official MCP251xFDcanfdspiAPIforSAMV71_v1_1 on Raspberry Pico
https://ww1.microchip.com/downloads/aemDocuments/documents/APID/ProductDocuments/SoftwareLibraries/Firmware/MCP251xFDcanfdspiAPIforSAMV71_v1_1.zip
 *******************************************************************************/

#include "common.h"

void led_init(void)
{
#ifndef PICO_DEFAULT_LED_PIN
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed");
    }
#else
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
#endif

#ifdef ENABLE_LED
    gpio_init(LED_1);
    gpio_set_dir(LED_1, GPIO_OUT);
    gpio_init(LED_2);
    gpio_set_dir(LED_2, GPIO_OUT);
#endif
}

void led_on(void)
{
#ifndef PICO_DEFAULT_LED_PIN
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
#else
    gpio_put(PICO_DEFAULT_LED_PIN, 1);
#endif
#ifdef ENABLE_LED
    gpio_put(LED_1, 1);
    gpio_put(LED_2, 1);
#endif
}

void led_off(void)
{
#ifndef PICO_DEFAULT_LED_PIN
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
#else
    gpio_put(PICO_DEFAULT_LED_PIN, 0);
#endif
#ifdef ENABLE_LED
    gpio_put(LED_1, 0);
    gpio_put(LED_2, 0);
#endif
}

void led1_on(void)
{
    gpio_put(LED_1, 1);
}

void led1_off(void)
{
    gpio_put(LED_1, 0);
}

void led2_on(void)
{
    gpio_put(LED_2, 1);
}

void led2_off(void)
{
    gpio_put(LED_2, 0);
}

void led_test(void)
{
    led1_on();
    sleep_ms(250);
    led1_off();
    sleep_ms(250);
    led2_on();
    sleep_ms(250);
    led2_off();
}
