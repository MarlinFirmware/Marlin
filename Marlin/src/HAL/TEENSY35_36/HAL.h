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

/**
 * HAL for Teensy 3.5 (MK64FX512) and Teensy 3.6 (MK66FX1M0)
 */

#define CPU_32_BIT

#include "../shared/Marduino.h"
#include "../shared/math_32bit.h"
#include "../shared/HAL_SPI.h"

#include "fastio.h"
#include "watchdog.h"

#include <stdint.h>
#include <util/atomic.h>

#define ST7920_DELAY_1 DELAY_NS(600)
#define ST7920_DELAY_2 DELAY_NS(750)
#define ST7920_DELAY_3 DELAY_NS(750)

// ------------------------
// Defines
// ------------------------

#define IS_32BIT_TEENSY 1
#define IS_TEENSY_35_36 1
#ifdef __MK66FX1M0__
  #define IS_TEENSY36 1
#else // __MK64FX512__
  #define IS_TEENSY35 1
#endif

#define _MSERIAL(X) Serial##X
#define MSERIAL(X) _MSERIAL(X)
#define Serial0 Serial

#if SERIAL_PORT == -1
  #define MYSERIAL0 SerialUSB
#elif WITHIN(SERIAL_PORT, 0, 3)
  #define MYSERIAL0 MSERIAL(SERIAL_PORT)
#endif

#define HAL_SERVO_LIB libServo

typedef int8_t pin_t;

#ifndef analogInputToDigitalPin
  #define analogInputToDigitalPin(p) ((p < 12U) ? (p) + 54U : -1)
#endif

#define CRITICAL_SECTION_START()  uint32_t primask = __get_primask(); __disable_irq()
#define CRITICAL_SECTION_END()    if (!primask) __enable_irq()
#define ISRS_ENABLED() (!__get_primask())
#define ENABLE_ISRS()  __enable_irq()
#define DISABLE_ISRS() __disable_irq()

#undef sq
#define sq(x) ((x)*(x))

#ifndef strncpy_P
  #define strncpy_P(dest, src, num) strncpy((dest), (src), (num))
#endif

// Fix bug in pgm_read_ptr
#undef pgm_read_ptr
#define pgm_read_ptr(addr) (*((void**)(addr)))
// Add type-checking to pgm_read_word
#undef pgm_read_word
#define pgm_read_word(addr) (*((uint16_t*)(addr)))

inline void HAL_init() {}

// Clear reset reason
void HAL_clear_reset_source();

// Reset reason
uint8_t HAL_get_reset_source();

inline void HAL_reboot() {}  // reboot the board or restart the bootloader

FORCE_INLINE void _delay_ms(const int delay_ms) { delay(delay_ms); }

#if GCC_VERSION <= 50000
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wunused-function"
#endif

extern "C" int freeMemory();

#if GCC_VERSION <= 50000
  #pragma GCC diagnostic pop
#endif

// ADC

void HAL_adc_init();

#define HAL_ADC_VREF         3.3
#define HAL_ADC_RESOLUTION  10
#define HAL_START_ADC(pin)  HAL_adc_start_conversion(pin)
#define HAL_READ_ADC()      HAL_adc_get_result()
#define HAL_ADC_READY()     true

#define HAL_ANALOG_SELECT(pin)

void HAL_adc_start_conversion(const uint8_t adc_pin);
uint16_t HAL_adc_get_result();

#define GET_PIN_MAP_PIN(index) index
#define GET_PIN_MAP_INDEX(pin) pin
#define PARSED_PIN_INDEX(code, dval) parser.intval(code, dval)
