/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 * Copyright (c) 2016 Bob Cousins bobcousins42@googlemail.com
 * Copyright (c) 2015-2016 Nico Tonnhofer wurstnase.reprap@gmail.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

#define CPU_32_BIT

#define F_CPU 100000000
#define SystemCoreClock F_CPU
#include <iostream>
#include <stdint.h>
#include <stdarg.h>

#undef min
#undef max

#include <algorithm>

void _printf (const  char *format, ...);
void _putc(uint8_t c);
uint8_t _getc();

//extern "C" volatile uint32_t _millis;

//arduino: Print.h
#define DEC 10
#define HEX 16
#define OCT  8
#define BIN  2
//arduino: binary.h (weird defines)
#define B01 1
#define B10 2

#include "hardware/Clock.h"

#include "../shared/Marduino.h"
#include "../shared/math_32bit.h"
#include "../shared/HAL_SPI.h"
#include "fastio.h"
#include "watchdog.h"
#include "HAL_timers.h"
#include "serial.h"

#define SHARED_SERVOS HAS_SERVOS

extern HalSerial usb_serial;
#define MYSERIAL0 usb_serial
#define NUM_SERIAL 1

#define ST7920_DELAY_1 DELAY_NS(600)
#define ST7920_DELAY_2 DELAY_NS(750)
#define ST7920_DELAY_3 DELAY_NS(750)

//
// Interrupts
//
#define CRITICAL_SECTION_START
#define CRITICAL_SECTION_END
#define ISRS_ENABLED()
#define ENABLE_ISRS()
#define DISABLE_ISRS()

inline void HAL_init(void) { }

// Utility functions
int freeMemory(void);

// SPI: Extended functions which take a channel number (hardware SPI only)
/** Write single byte to specified SPI channel */
void spiSend(uint32_t chan, byte b);
/** Write buffer to specified SPI channel */
void spiSend(uint32_t chan, const uint8_t* buf, size_t n);
/** Read single byte from specified SPI channel */
uint8_t spiRec(uint32_t chan);

// ADC
#define HAL_ANALOG_SELECT(pin) HAL_adc_enable_channel(pin)
#define HAL_START_ADC(pin)     HAL_adc_start_conversion(pin)
#define HAL_READ_ADC()         HAL_adc_get_result()
#define HAL_ADC_READY()        true

void HAL_adc_init(void);
void HAL_adc_enable_channel(int pin);
void HAL_adc_start_conversion(const uint8_t adc_pin);
uint16_t HAL_adc_get_result(void);

/* ---------------- Delay in cycles */
FORCE_INLINE static void DELAY_CYCLES(uint64_t x) {
  Clock::delayCycles(x);
}
