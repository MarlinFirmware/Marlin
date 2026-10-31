/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * Pins Debugging for x86_64
 * See PinsDebugBase in pins/pinsDebug.h for the hooks.
 */

#define PINS_DEBUG_CLASS

struct PinsDebug : PinsDebugBase<PinsDebug> {
  static constexpr uint16_t count = NUM_DIGITAL_PINS;
  static constexpr uint8_t name_pad = 16;   // space needed to be pretty if not first name assigned to a pin

  static bool is_valid(const pin_t pin) { return pin >= 0 && pin < pin_t(count); }

  static int16_t analog_index(const pin_t pin) { return digitalPinToAnalogIndex(pin); }
  static bool is_analog(const pin_t pin) { return analog_index(pin) >= 0; }

  static int read_digital(const pin_t pin) { return digitalRead(pin); }

  // Active ADC function/mode/code values for PINSEL registers
  static int8_t adc_pin_mode(const pin_t) { return -1; }
  static int8_t pin_mode(const pin_t pin) { return is_valid(pin) ? 0 : -1; }

  // Input/output state
  static bool is_output(const pin_t pin) {
    const int8_t mode = pin_mode(pin);
    if (mode == -1 || mode == adc_pin_mode(pin)) return false; // Invalid pin or active analog pin
    return Gpio::getMode(pin) != 0;
  }

  // The pin and index are the same on this platform
  static bool is_digital_at(const uint16_t index) {
    const pin_t pin = pin_t(index);
    return !is_analog(pin) || pin_mode(pin) != adc_pin_mode(pin);
  }
};
