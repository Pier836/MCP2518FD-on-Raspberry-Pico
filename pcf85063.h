#ifndef PCF85063_H
#define PCF85063_H
/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "common.h"
#include "hardware/i2c.h"


static void pcf85063_reset(void);

static void pcf85063_write_current_time(void);

static void pcf85063_read_raw(uint8_t *buffer);

void pcf85063_set_alarm(void);

void pcf85063_check_alarm(void);

void pcf85063_convert_time(int conv_time[7], const uint8_t raw_time[7]);

void pcf85063_init(void);

void pcf85063_print_time_date(void);

#endif //PCF85063_H
