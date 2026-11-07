/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 */
#pragma once

/**
 * Pins Debugging for AT32
 * See PinsDebugBase in pins/pinsDebug.h for the hooks.
 */

#include "../../inc/MarlinConfig.h"
#include "fastio.h"

// AT32 pin debugging uses the Arduino PIN_MAP infrastructure
#define NUM_DIGITAL_PINS  PIN_MAX

#ifndef BOARD_USART1_TX_PIN
  #define BOARD_USART1_TX_PIN PA9
#endif
#ifndef BOARD_USART1_RX_PIN
  #define BOARD_USART1_RX_PIN PA10
#endif

#ifndef ADC_PIN_INVALID
  #define ADC_PIN_INVALID 0xFF
#endif

struct PinsDebug : PinsDebugBase<PinsDebug> {
  static constexpr uint16_t count = PIN_MAX;
  static constexpr uint8_t name_pad = 21;   // Space needed to be pretty if not first name assigned to a pin

  static bool is_valid(const pin_t pin) { return IS_PIN(pin); }

  //
  // Pins that will cause a hang / reset / disconnect in M43 Toggle and Watch utils
  //
  static bool never_touch(const uint16_t index) {
    const pin_t pin = pin_t(index);
    return pin == BOARD_USART1_TX_PIN || pin == BOARD_USART1_RX_PIN     // Host USART
        || pin == PD0 || pin == PD1 || pin == PC14 || pin == PC15;      // Oscillators (HEXT, LEXT)
  }

  static bool has_adc(const pin_t pin) {
    return PIN_MAP[pin].ADCx != NULL && PIN_MAP[pin].ADC_Channel != ADC_PIN_INVALID;
  }

  static int16_t analog_index(const pin_t pin) {
    if (!is_valid(pin) || !has_adc(pin)) return -1;
    return int16_t(PIN_MAP[pin].ADC_Channel);
  }

  static bool is_analog(const pin_t pin) {
    if (!is_valid(pin) || !has_adc(pin)) return false;
    return _GET_MODE(pin) == INPUT_ANALOG && !never_touch(pin);
  }

  static bool is_output(const pin_t pin) { return is_valid(pin) && !IS_INPUT(pin); }

  static bool is_digital_at(const uint16_t index) { return !is_analog(getPinByIndex(index)); }

  static bool pwm_active(const pin_t pin) { return is_valid(pin) && IS_PWM_PIN(pin); }

  static void print_pwm(const pin_t pin) {
    if (pwm_active(pin)) SERIAL_ECHOPGM("PWM");
  }

  static void print_port(const pin_t pin) {
    const char port = 'A' + char(pin >> 4);
    const uint8_t gbit = pin & 0x0F;
    char buffer[8];
    sprintf_P(buffer, PSTR("P%c%hd "), port, gbit);
    if (gbit < 10) SERIAL_CHAR(' ');
    SERIAL_ECHO(buffer);
  }

  static void print_number(const pin_t pin) {
    char buffer[10];
    sprintf_P(buffer, PSTR("%3hd "), int16_t(pin));
    SERIAL_ECHO(buffer);
  }
};
