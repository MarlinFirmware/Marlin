/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

#define CPU_32_BIT

#define F_CPU 100000000UL
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
#include "serial.h"

#define SHARED_SERVOS HAS_SERVOS

extern MSerialT usb_serial;
#define MYSERIAL1 usb_serial

#define ST7920_DELAY_1 DELAY_NS(600)
#define ST7920_DELAY_2 DELAY_NS(750)
#define ST7920_DELAY_3 DELAY_NS(750)

//
// Interrupts
//
#define CRITICAL_SECTION_START()
#define CRITICAL_SECTION_END()
#define ISRS_ENABLED()
#define ENABLE_ISRS()
#define DISABLE_ISRS()

inline void HAL_init() {}

// Utility functions
#if GCC_VERSION <= 50000
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wunused-function"
#endif

int freeMemory();

#if GCC_VERSION <= 50000
  #pragma GCC diagnostic pop
#endif

// ADC
#define HAL_ADC_VREF           5.0
#define HAL_ADC_RESOLUTION    10
#define HAL_ANALOG_SELECT(ch) HAL_adc_enable_channel(ch)
#define HAL_START_ADC(ch)     HAL_adc_start_conversion(ch)
#define HAL_READ_ADC()        HAL_adc_get_result()
#define HAL_ADC_READY()       true

void HAL_adc_init();
void HAL_adc_enable_channel(const uint8_t ch);
void HAL_adc_start_conversion(const uint8_t ch);
uint16_t HAL_adc_get_result();

// Reset source
inline void HAL_clear_reset_source(void) {}
inline uint8_t HAL_get_reset_source(void) { return RST_POWER_ON; }

inline void HAL_reboot() {}  // reboot the board or restart the bootloader

/* ---------------- Delay in cycles */
FORCE_INLINE static void DELAY_CYCLES(uint64_t x) {
  Clock::delayCycles(x);
}
