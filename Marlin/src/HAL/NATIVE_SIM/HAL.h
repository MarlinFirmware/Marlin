/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#define HAL_IDLETASK
void HAL_idletask();

#define F_CPU 100000000
#define SystemCoreClock F_CPU
#include <stdint.h>
#include <stdarg.h>

#undef min
#undef max

#include <algorithm>
#include "pinmapping.h"

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

#include "../shared/Marduino.h"
#include "../shared/math_32bit.h"
#include "../shared/HAL_SPI.h"
#include "fastio.h"
#include "watchdog.h"
#include "serial.h"

#define SHARED_SERVOS HAS_SERVOS

extern MSerialT serial_stream_0;
extern MSerialT serial_stream_1;
extern MSerialT serial_stream_2;
extern MSerialT serial_stream_3;

#define _MSERIAL(X) serial_stream_##X
#define MSERIAL(X) _MSERIAL(X)

#if WITHIN(SERIAL_PORT, 0, 3)
  #define MYSERIAL1 MSERIAL(SERIAL_PORT)
#else
  #error "SERIAL_PORT must be from 0 to 3. Please update your configuration."
#endif

#ifdef SERIAL_PORT_2
  #if WITHIN(SERIAL_PORT_2, 0, 3)
    #define MYSERIAL2 MSERIAL(SERIAL_PORT_2)
  #else
    #error "SERIAL_PORT_2 must be from 0 to 3. Please update your configuration."
  #endif
#endif

#ifdef MMU2_SERIAL_PORT
  #if WITHIN(MMU2_SERIAL_PORT, 0, 3)
    #define MMU2_SERIAL MSERIAL(MMU2_SERIAL_PORT)
  #else
    #error "MMU2_SERIAL_PORT must be from 0 to 3. Please update your configuration."
  #endif
#endif

#ifdef LCD_SERIAL_PORT
  #if WITHIN(LCD_SERIAL_PORT, 0, 3)
    #define LCD_SERIAL MSERIAL(LCD_SERIAL_PORT)
  #else
    #error "LCD_SERIAL_PORT must be from 0 to 3. Please update your configuration."
  #endif
#endif


#define CPU_ST7920_DELAY_1 600
#define CPU_ST7920_DELAY_2 750
#define CPU_ST7920_DELAY_3 750

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
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
int freeMemory();
#pragma GCC diagnostic pop

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

/* ---------------- Delay in cycles */

#define DELAY_CYCLES(x) Kernel::delayCycles(x)
#define SYSTEM_YIELD() Kernel::yield()

// Maple Compatibility
typedef void (*systickCallback_t)(void);
void systick_attach_callback(systickCallback_t cb);
extern volatile uint32_t systick_uptime_millis;

// Marlin uses strstr in constexpr context, this is not supported, workaround by defining constexpr versions of the required functions.
#define strstr(a, b) strstr_constexpr((a), (b))

constexpr inline std::size_t strlen_constexpr(const char* str) {
  // https://github.com/gcc-mirror/gcc/blob/5c7634a0e5f202935aa6c11b6ea953b8bf80a00a/libstdc%2B%2B-v3/include/bits/char_traits.h#L329
  if (str != nullptr) {
    std::size_t i = 0;
    while (str[i] != '\0') {
      ++i;
    }

    return i;
  }

  return 0;
}

constexpr inline int strncmp_constexpr(const char* lhs, const char* rhs, std::size_t count) {
  // https://github.com/gcc-mirror/gcc/blob/13b9cbfc32fe3ac4c81c4dd9c42d141c8fb95db4/libstdc%2B%2B-v3/include/bits/char_traits.h#L655
  if (lhs == nullptr || rhs == nullptr) {
    return rhs != nullptr ? -1 : 1;
  }

  for (std::size_t i = 0; i < count; ++i) {
    if (lhs[i] != rhs[i]) {
      return lhs[i] < rhs[i] ? -1 : 1;
    } else if (lhs[i] == '\0') {
      return 0;
    }
  }

  return 0;
}

constexpr inline const char* strstr_constexpr(const char* str, const char* target) {
  // https://github.com/freebsd/freebsd/blob/master/sys/libkern/strstr.c
  if (char c = target != nullptr ? *target++ : '\0'; c != '\0' && str != nullptr) {
    std::size_t len = strlen_constexpr(target);
    do {
      char sc = {};
      do {
        if ((sc = *str++) == '\0') {
          return nullptr;
        }
      } while (sc != c);
    } while (strncmp_constexpr(str, target, len) != 0);
    --str;
  }

  return str;
}

constexpr inline char* strstr_constexpr(char* str, const char* target) {
  // https://github.com/freebsd/freebsd/blob/master/sys/libkern/strstr.c
  if (char c = target != nullptr ? *target++ : '\0'; c != '\0' && str != nullptr) {
    std::size_t len = strlen_constexpr(target);
    do {
      char sc = {};
      do {
        if ((sc = *str++) == '\0') {
          return nullptr;
        }
      } while (sc != c);
    } while (strncmp_constexpr(str, target, len) != 0);
    --str;
  }
  return str;
}
