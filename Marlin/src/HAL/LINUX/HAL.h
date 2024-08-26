/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
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

#include "../../inc/MarlinConfigPre.h"

#include <iostream>
#include <stdint.h>
#include <stdarg.h>
#undef min
#undef max
#include <algorithm>

#include "hardware/Clock.h"
#include "../shared/Marduino.h"
#include "../shared/math_32bit.h"
#include "../shared/HAL_SPI.h"
#include "fastio.h"
#include "serial.h"

// ------------------------
// Defines
// ------------------------

#define CPU_32_BIT
#define SHARED_SERVOS HAS_SERVOS  // Use shared/servos.cpp

#define F_CPU 100000000UL
#define SystemCoreClock F_CPU

#define DELAY_CYCLES(x) Clock::delayCycles(x)

#define CPU_ST7920_DELAY_1 600
#define CPU_ST7920_DELAY_2 750
#define CPU_ST7920_DELAY_3 750

void _printf(const  char *format, ...);
void _putc(uint8_t c);
uint8_t _getc();

//arduino: Print.h
#define DEC 10
#define HEX 16
#define OCT  8
#define BIN  2
//arduino: binary.h (weird defines)
#define B01 1
#define B10 2

// ------------------------
// Serial ports
// ------------------------

extern MSerialT usb_serial;
#define MYSERIAL1 usb_serial

//
// Interrupts
//
#define CRITICAL_SECTION_START()
#define CRITICAL_SECTION_END()

// ADC
#define HAL_ADC_VREF_MV   5000
#define HAL_ADC_RESOLUTION  10

// ------------------------
// Class Utilities
// ------------------------

#pragma GCC diagnostic push
#if GCC_VERSION <= 50000
  #pragma GCC diagnostic ignored "-Wunused-function"
#endif

int freeMemory();

#pragma GCC diagnostic pop

// ------------------------
// MarlinHAL Class
// ------------------------

class MarlinHAL {
public:

  // Earliest possible init, before setup()
  MarlinHAL() {}

  // Watchdog
  static void watchdog_init() {}
  static void watchdog_refresh() {}

  static void init() {}        // Called early in setup()
  static void init_board() {}  // Called less early in setup()
  static void reboot();        // Reset the application state and GPIO

  // Interrupts
  static bool isr_state() { return true; }
  static void isr_on()  {}
  static void isr_off() {}

  static void delay_ms(const int ms) { _delay_ms(ms); }

  // Tasks, called from idle()
  static void idletask() {}

  // Reset
  static constexpr uint8_t reset_reason = RST_POWER_ON;
  static uint8_t get_reset_source() { return reset_reason; }
  static void clear_reset_source() {}

  // Free SRAM
  static int freeMemory() { return ::freeMemory(); }

  //
  // ADC Methods
  //

  static uint8_t active_ch;

  // Called by Temperature::init once at startup
  static void adc_init() {}

  // Called by Temperature::init for each sensor at startup
  static void adc_enable(const uint8_t) {}

  // Begin ADC sampling on the given channel
  static void adc_start(const uint8_t ch) { active_ch = ch; }

  // Is the ADC ready for reading?
  static bool adc_ready() { return true; }

  // The current value of the ADC register
  static uint16_t adc_value();

  /**
   * Set the PWM duty cycle for the pin to the given value.
   * No option to change the resolution or invert the duty cycle.
   */
  static void set_pwm_duty(const pin_t pin, const uint16_t v, const uint16_t=255, const bool=false) {
    analogWrite(pin, v);
  }

  static void set_pwm_frequency(const pin_t, int) {}
};
