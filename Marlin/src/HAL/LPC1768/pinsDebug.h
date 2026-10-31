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
 * Pins Debugging for LPC1768/9
 * See PinsDebugBase in pins/pinsDebug.h for the hooks.
 */

#define PINS_DEBUG_CLASS

struct PinsDebug : PinsDebugBase<PinsDebug> {
  static constexpr uint16_t count = NUM_DIGITAL_PINS;
  static constexpr uint8_t name_pad = 17;   // space needed to be pretty if not first name assigned to a pin

  // Pins that will cause hang/reset/disconnect in M43 Toggle and Watch utilities
  static bool never_touch(const uint16_t index) { return index == P0_29 || index == P0_30 || index == P2_09; }  // USB pins

  static bool is_valid(const pin_t pin) { return isValidPin(pin); }
  static int16_t analog_index(const pin_t pin) { return digitalPinToAnalogIndex(pin); }
  static bool is_analog(const pin_t pin) { return analog_index(pin) >= 0; }

  static bool is_output(const pin_t pin) {
    if (!LPC176x::pin_is_valid(pin) || LPC176x::pin_adc_enabled(pin)) // Invalid pin or active analog pin
      return false;

    return LPC176x::gpio_direction(pin);
  }

  static void print_number(const pin_t pin) {
    char buffer[12];
    sprintf_P(buffer, PSTR("P%d_%02d"), LPC176x::pin_port(pin), LPC176x::pin_bit(pin));
    SERIAL_ECHO(buffer);
  }

  static void print_analog(const pin_t pin) {
    char buffer[12];
    sprintf_P(buffer, PSTR("_A%d     "), LPC176x::pin_get_adc_channel(pin));
    SERIAL_ECHO(buffer);
  }
};
