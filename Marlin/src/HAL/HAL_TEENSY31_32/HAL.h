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

/**
 * Description: HAL for Teensy 3.5 and Teensy 3.6
 */

#define CPU_32_BIT

#include "../shared/Marduino.h"
#include "../shared/math_32bit.h"
#include "../shared/HAL_SPI.h"

#include "fastio_Teensy.h"
#include "watchdog_Teensy.h"

#include "HAL_timers_Teensy.h"

#include <stdint.h>

#define ST7920_DELAY_1 DELAY_NS(600)
#define ST7920_DELAY_2 DELAY_NS(750)
#define ST7920_DELAY_3 DELAY_NS(750)

//#undef MOTHERBOARD
//#define MOTHERBOARD BOARD_TEENSY31_32

#define IS_32BIT_TEENSY defined(__MK20DX256__)
#define IS_TEENSY32 defined(__MK20DX256__)

#define NUM_SERIAL 1

#if SERIAL_PORT == -1
  #define MYSERIAL0 SerialUSB
#elif SERIAL_PORT == 0
  #define MYSERIAL0 Serial
#elif SERIAL_PORT == 1
  #define MYSERIAL0 Serial1
#elif SERIAL_PORT == 2
  #define MYSERIAL0 Serial2
#elif SERIAL_PORT == 3
  #define MYSERIAL0 Serial3
#endif

#define HAL_SERVO_LIB libServo

typedef int8_t pin_t;

#ifndef analogInputToDigitalPin
  #define analogInputToDigitalPin(p) ((p < 12u) ? (p) + 54u : -1)
#endif

#define CRITICAL_SECTION_START  uint32_t primask = __get_PRIMASK(); __disable_irq()
#define CRITICAL_SECTION_END    if (!primask) __enable_irq()
#define ISRS_ENABLED() (!__get_PRIMASK())
#define ENABLE_ISRS()  __enable_irq()
#define DISABLE_ISRS() __disable_irq()

#ifndef strncpy_P
  #define strncpy_P(dest, src, num) strncpy((dest), (src), (num))
#endif

// Fix bug in pgm_read_ptr
#undef pgm_read_ptr
#define pgm_read_ptr(addr) (*((void**)(addr)))
// Add type-checking to pgm_read_word
#undef pgm_read_word
#define pgm_read_word(addr) (*((uint16_t*)(addr)))

inline void HAL_init(void) { }

// Clear the reset reason
void HAL_clear_reset_source(void);

// Get the reason for the reset
uint8_t HAL_get_reset_source(void);

FORCE_INLINE void _delay_ms(const int delay_ms) { delay(delay_ms); }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
extern "C" {
  int freeMemory(void);
}
#pragma GCC diagnostic pop

// SPI: Extended functions which take a channel number (hardware SPI only)

// Write single byte to specified SPI channel
void spiSend(uint32_t chan, byte b);

// Write buffer to specified SPI channel
void spiSend(uint32_t chan, const uint8_t* buf, size_t n);

// Read single byte from specified SPI channel
uint8_t spiRec(uint32_t chan);

// ADC

void HAL_adc_init();

#define HAL_START_ADC(pin)  HAL_adc_start_conversion(pin)
#define HAL_READ_ADC()      HAL_adc_get_result()
#define HAL_ADC_READY()     true

#define HAL_ANALOG_SELECT(pin)

void HAL_adc_start_conversion(const uint8_t adc_pin);
uint16_t HAL_adc_get_result(void);

#define GET_PIN_MAP_PIN(index) index
#define GET_PIN_MAP_INDEX(pin) pin
#define PARSED_PIN_INDEX(code, dval) parser.intval(code, dval)
