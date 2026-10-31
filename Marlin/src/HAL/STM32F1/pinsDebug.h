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

/**
 * Pins Debugging for Maple STM32F1
 * See PinsDebugBase in pins/pinsDebug.h for the hooks.
 */

#ifndef BOARD_NR_GPIO_PINS // Only in MAPLE_STM32F1
  #error "Expected BOARD_NR_GPIO_PINS not found"
#endif

#include "fastio.h"

extern const stm32_pin_info PIN_MAP[BOARD_NR_GPIO_PINS];

#define NUM_DIGITAL_PINS BOARD_NR_GPIO_PINS

#include "../../inc/MarlinConfig.h" // Allow pins/pins.h to set density

#define PINS_DEBUG_CLASS

struct PinsDebug : PinsDebugBase<PinsDebug> {
  static constexpr uint16_t count = BOARD_NR_GPIO_PINS;
  static constexpr uint8_t name_pad = 21;   // space needed to be pretty if not first name assigned to a pin

  static bool is_valid(const pin_t pin) { return pin >= 0 && pin < BOARD_NR_GPIO_PINS; }

  // Pins that will cause hang/reset/disconnect in M43 Toggle and Watch utilities
  // SERIAL/USB pins PA9(TX) PA10(RX)
  static bool never_touch(const uint16_t index) { return index >= 9 && index <= 12; }

  static int16_t analog_index(const pin_t pin) {
    if (!is_valid(pin)) return -1;
    pin_t adc_channel = pin_t(PIN_MAP[pin].adc_channel);
    #ifdef NUM_ANALOG_INPUTS
      if (adc_channel >= NUM_ANALOG_INPUTS) adc_channel = (pin_t)ADCx;
    #endif
    return adc_channel;
  }

  static bool is_analog(const pin_t pin) {
    if (!is_valid(pin)) return false;
    if (PIN_MAP[pin].adc_channel != ADCx) {
      #ifdef NUM_ANALOG_INPUTS
        if (PIN_MAP[pin].adc_channel >= NUM_ANALOG_INPUTS) return false;
      #endif
      return _GET_MODE(pin) == GPIO_INPUT_ANALOG && !never_touch(pin);
    }
    return false;
  }

  static bool is_output(const pin_t pin) { return is_valid(pin) && !IS_INPUT(pin); }

  static bool is_digital_at(const uint16_t index) {
    const pin_t pin = getPinByIndex(index);
    return (!is_analog(pin)
      #ifdef NUM_ANALOG_INPUTS
        || PIN_MAP[pin].adc_channel >= NUM_ANALOG_INPUTS
      #endif
    );
  }

  static bool pwm_active(const pin_t pin) { return PWM_PIN(pin); }

  static void print_pwm(const pin_t pin) {
    if (PWM_PIN(pin)) {
      timer_dev * const tdev = PIN_MAP[pin].timer_device;
      const uint8_t channel = PIN_MAP[pin].timer_channel;
      const char num = (
        #if ANY(STM32_HIGH_DENSITY, STM32_XL_DENSITY)
          tdev == &timer8 ? '8' :
          tdev == &timer5 ? '5' :
        #endif
        tdev == &timer4 ? '4' :
        tdev == &timer3 ? '3' :
        tdev == &timer2 ? '2' :
        tdev == &timer1 ? '1' : '?'
      );
      char buffer[10];
      sprintf_P(buffer, PSTR(" TIM%c CH%c"), num, ('0' + channel));
      SERIAL_ECHO(buffer);
    }
  }

  static void print_port(const pin_t pin) {
    const char port = 'A' + char(pin >> 4); // pin div 16
    const int16_t gbit = PIN_MAP[pin].gpio_bit;
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
