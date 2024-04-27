/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "common.h"
#include "hardware/i2c.h"
#include "pcf85063.h"

uint8_t addr = 0x51;

static void pcf85063_reset() {
    // Two byte reset. First byte register, second byte data
    // There are a load more options to set up the device in different ways that could be added here
    uint8_t buf[] = {0x00, 0x58};
    i2c_write_blocking(i2c_default, addr, buf, 2, false);
}

static void pcf85063_write_current_time() {
    // buf[0] is the register to write to
    // buf[1] is the value that will be written to the register
    uint8_t buf[2];

    //Write values for the current time in the array
    //index 0 -> second: bits 4-6 are responsible for the ten's digit and bits 0-3 for the unit's digit
    //index 1 -> minute: bits 4-6 are responsible for the ten's digit and bits 0-3 for the unit's digit
    //index 2 -> hour: bits 4-5 are responsible for the ten's digit and bits 0-3 for the unit's digit
    //index 3 -> day of the month: bits 4-5 are responsible for the ten's digit and bits 0-3 for the unit's digit
    //index 4 -> day of the week: where Sunday = 0x00, Monday = 0x01, Tuesday... ...Saturday = 0x06
    //index 5 -> month: bit 4 is responsible for the ten's digit and bits 0-3 for the unit's digit
    //index 6 -> year: bits 4-7 are responsible for the ten's digit and bits 0-3 for the unit's digit

    //NOTE: if the value in the year register is a multiple for 4, it will be considered a leap year and hence will include the 29th of February

    uint8_t current_val[7] = {0x00, 0x00, 0x14, 0x0f, 0x04, 0x04, 0x24};

    for (int i = 3; i < 10; ++i) {
        buf[0] = i;
        buf[1] = current_val[i - 3];
        i2c_write_blocking(i2c_default, addr, buf, 2, false);
    }
}

static void pcf85063_read_raw(uint8_t *buffer) {
    // For this particular device, we send the device the register we want to read
    // first, then subsequently read from the device. The register is auto incrementing
    // so we don't need to keep sending the register we want, just the first.

    // Start reading acceleration registers from register 0x3B for 6 bytes
    uint8_t val = 0x03;
    i2c_write_blocking(i2c_default, addr, &val, 1, true); // true to keep master control of bus
    i2c_read_blocking(i2c_default, addr, buffer, 7, false);
}

void pcf85063_set_alarm() {
    // buf[0] is the register to write to
    // buf[1] is the value that will be written to the register
    uint8_t buf[2];

    // Default value of alarm register is 0x80
    // Set bit 8 of values to 0 to activate that particular alarm
    // Index 0 -> minute: bits 4-5 are responsible for the ten's digit and bits 0-3 for the unit's digit
    // Index 1 -> hour: bits 4-6 are responsible for the ten's digit and bits 0-3 for the unit's digit
    // Index 2 -> day of the month: bits 4-5 are responsible for the ten's digit and bits 0-3 for the unit's digit
    // Index 3 -> day of the week: where Sunday = 0x00, Monday = 0x01, Tuesday... ...Saturday = 0x06

    uint8_t alarm_val[4] = {0x01, 0x80, 0x80, 0x80};
    // Write alarm values to registers
    for (int i = 10; i < 14; ++i) {
        buf[0] = (uint8_t) i;
        buf[1] = alarm_val[i - 10];
        i2c_write_blocking(i2c_default, addr, buf, 2, false);
    }
}

void pcf85063_check_alarm() {
    // Check bit 3 of control register 2 for alarm flags
    uint8_t status[1];
    uint8_t val = 0x01;
    i2c_write_blocking(i2c_default, addr, &val, 1, true); // true to keep master control of bus
    i2c_read_blocking(i2c_default, addr, status, 1, false);

    if ((status[0] & 0x08) == 0x08) {
        printf("ALARM RINGING");
    } else {
        printf("Alarm not triggered yet");
    }
}

void pcf85063_convert_time(int conv_time[7], const uint8_t raw_time[7]) {
    // Convert raw data into time
    conv_time[0] = (10 * (int) ((raw_time[0] & 0x70) >> 4)) + ((int) (raw_time[0] & 0x0F));
    conv_time[1] = (10 * (int) ((raw_time[1] & 0x70) >> 4)) + ((int) (raw_time[1] & 0x0F));
    conv_time[2] = (10 * (int) ((raw_time[2] & 0x30) >> 4)) + ((int) (raw_time[2] & 0x0F));
    conv_time[3] = (10 * (int) ((raw_time[3] & 0x30) >> 4)) + ((int) (raw_time[3] & 0x0F));
    conv_time[4] = (int) (raw_time[4] & 0x07);
    conv_time[5] = (10 * (int) ((raw_time[5] & 0x10) >> 4)) + ((int) (raw_time[5] & 0x0F));
    conv_time[6] = (10 * (int) ((raw_time[6] & 0xF0) >> 4)) + ((int) (raw_time[6] & 0x0F));
}


void pcf85063_init() 
{
    // This example will use I2C0 on the default SDA and SCL pins (4, 5 on a Pico)
    i2c_init(i2c_default, 400 * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);
    pcf85063_reset();
    pcf85063_write_current_time();
    pcf85063_set_alarm();
    pcf85063_check_alarm();
	
}

void pcf85063_print_time_date() 
{
    uint8_t raw_time[7];
    int real_time[7];
    char days_of_week[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
	pcf85063_read_raw(raw_time);
	pcf85063_convert_time(real_time, raw_time);
	printf("Time: %02d : %02d : %02d\n", real_time[2], real_time[1], real_time[0]);
	printf("Date: %s %02d / %02d / %02d\n", days_of_week[real_time[4]], real_time[3], real_time[5], real_time[6]);
	//pcf85063_check_alarm();
}
