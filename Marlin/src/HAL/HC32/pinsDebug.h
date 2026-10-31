/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * Pins Debugging for HC32
 * See PinsDebugBase in pins/pinsDebug.h for the hooks.
 */

#include "../../inc/MarlinConfig.h"
#include "fastio.h"
#include <drivers/timera/timera_pwm.h>

#ifndef BOARD_NR_GPIO_PINS
  #error "Expected BOARD_NR_GPIO_PINS not found."
#endif

#define NUM_DIGITAL_PINS BOARD_NR_GPIO_PINS

// Note: pin_array is defined in `Marlin/src/pins/pinsDebug.h`, and since this file is included
// after it, it is available in this file as well.

#define PINS_DEBUG_CLASS

struct PinsDebug : PinsDebugBase<PinsDebug> {
  static constexpr uint16_t count = BOARD_NR_GPIO_PINS;
  static constexpr uint8_t name_pad = 21;   // Space needed to be pretty if not first name assigned to a pin

  static bool is_valid(const pin_t pin) { return IS_GPIO_PIN(pin); }

  //
  // Pins that will cause a hang / reset / disconnect in M43 Toggle and Watch utils
  //
  // Don't touch any of the following pins:
  // - Host serial pins, and
  // - Pins that could be connected to oscillators (see datasheet, Table 2.1):
  //   - XTAL = PH0, PH1
  //   - XTAL32 = PC14, PC15
  static bool never_touch(const uint16_t index) {
    const pin_t pin = pin_t(index);
    return pin == BOARD_USART2_TX_PIN || pin == BOARD_USART2_RX_PIN       // Host USART
        || pin == PH0 || pin == PH1 || pin == PC14 || pin == PC15;        // Oscillators
  }

  static int16_t analog_index(const pin_t pin) {
    if (!is_valid(pin)) return -1;
    const int8_t adc_channel = int8_t(PIN_MAP[pin].adc_info.channel);
    return pin_t(adc_channel);
  }

  static bool is_analog(const pin_t pin) {
    if (!is_valid(pin)) return false;

    if (PIN_MAP[pin].adc_info.channel != ADC_PIN_INVALID)
      return _GET_MODE(pin) == INPUT_ANALOG && !never_touch(pin);

    return false;
  }

  static bool is_output(const pin_t pin) { return is_valid(pin) && !IS_INPUT(pin); }

  static bool is_digital_at(const uint16_t index) { return !is_analog(getPinByIndex(index)); }

  /**
   * @brief print pin PWM status
   * @return true if pin is currently a PWM pin, false otherwise
   */
  static bool pwm_active(const pin_t pin) {
    // Get timer assignment for pin
    timera_config_t *unit;
    en_timera_channel_t channel;
    en_port_func_t port_function;
    if (!timera_get_assignment(pin, unit, channel, port_function) || unit == nullptr) {
      // No pwm pin or no unit assigned
      return false;
    }

    // A pin that is PWM output is:
    // - Assigned to a timerA unit (tested above)
    // - Unit is initialized
    // - Channel is active
    // - PinMode is OUTPUT_PWM
    return timera_is_unit_initialized(unit) && timera_is_channel_active(unit, channel) && getPinMode(pin) == OUTPUT_PWM;
  }

  static void print_pwm(const pin_t pin) {
    // Get timer assignment for pin
    timera_config_t *unit;
    en_timera_channel_t channel;
    en_port_func_t port_function;
    if (!timera_get_assignment(pin, unit, channel, port_function) || unit == nullptr)
      return; // No pwm pin or no unit assigned

    // Print timer assignment of pin, eg. "TimerA1Ch2 Func4"
    SERIAL_ECHO("TimerA", TIMERA_REG_TO_X(unit->peripheral.register_base),
                   "Ch", TIMERA_CHANNEL_TO_X(channel),
                   " Func", int(port_function));
    SERIAL_ECHO_SP(3); // 3 spaces

    // Print timer unit state, eg. "1/16 PERAR=1234" OR "N/A"
    if (timera_is_unit_initialized(unit)) {
      // Unit initialized, print
      // - Timer clock divider
      // - Timer period value (PERAR)
      const uint8_t clock_divider = timera_clk_div_to_n(unit->state.base_init->enClkDiv);
      const uint16_t period = TIMERA_GetPeriodValue(unit->peripheral.register_base);
      SERIAL_ECHO("1/", clock_divider, " PERAR=", period);
    }
    else {
      // Unit not initialized
      SERIAL_ECHO("N/A");
      return;
    }

    SERIAL_ECHO_SP(3); // 3 spaces

    // Print timer channel state, e.g. "CMPAR=1234" OR "N/A"
    if (timera_is_channel_active(unit, channel)) {
      // Channel active, print
      // - Channel compare value
      const uint16_t compare = TIMERA_GetCompareValue(unit->peripheral.register_base, channel);
      SERIAL_ECHO("CMPAR=", compare);
    }
    else {
      // Channel inactive
      SERIAL_ECHO("N/A");
    }
  }

  static void print_port(const pin_t pin) {
    const char port = 'A' + char(pin >> 4); // Pin div 16
    const int16_t gbit = PIN_MAP[pin].bit_pos;
    char buffer[8];
    sprintf_P(buffer, PSTR("P%c%hd "), port, gbit);
    if (gbit < 10) {
      SERIAL_CHAR(' ');
    }

    SERIAL_ECHO(buffer);
  }

  static void print_number(const pin_t pin) {
    char buffer[10];
    sprintf_P(buffer, PSTR("%3hd "), int16_t(pin));
    SERIAL_ECHO(buffer);
  }
};
