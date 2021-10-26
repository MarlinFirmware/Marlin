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

#define CPU_ST7920_DELAY_1 600
#define CPU_ST7920_DELAY_2 750
#define CPU_ST7920_DELAY_3 750

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

#include "../../core/serial_hook.h"

#define Serial0 Serial
#define _DECLARE_SERIAL(X) \
  typedef ForwardSerial1Class<decltype(Serial##X)> DefaultSerial##X; \
  extern DefaultSerial##X MSerial##X
#define DECLARE_SERIAL(X) _DECLARE_SERIAL(X)

typedef ForwardSerial1Class<decltype(SerialUSB)> USBSerialType;
extern USBSerialType USBSerial;

#define _MSERIAL(X) MSerial##X
#define MSERIAL(X) _MSERIAL(X)

#if SERIAL_PORT == -1
  #define MYSERIAL1 USBSerial
#elif WITHIN(SERIAL_PORT, 0, 3)
  #define MYSERIAL1 MSERIAL(SERIAL_PORT)
  DECLARE_SERIAL(SERIAL_PORT);
#else
  #error "SERIAL_PORT must be from 0 to 3, or -1 for Native USB."
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

inline void HAL_init() {}

// Clear reset reason
void HAL_clear_reset_source();

// Reset reason
uint8_t HAL_get_reset_source();

void HAL_reboot();

FORCE_INLINE void _delay_ms(const int delay_ms) { delay(delay_ms); }

#pragma GCC diagnostic push
#if GCC_VERSION <= 50000
  #pragma GCC diagnostic ignored "-Wunused-function"
#endif

extern "C" int freeMemory();

#pragma GCC diagnostic pop

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
