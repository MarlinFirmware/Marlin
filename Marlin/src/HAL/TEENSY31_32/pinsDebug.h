/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#error "PINS_DEBUGGING is not yet supported for Teensy 3.1 / 3.2!"

/**
 * Pins Debugging for ESP32
 * See PinsDebugBase in pins/pinsDebug.h for the hooks.
 */

#define PINS_DEBUG_CLASS

struct PinsDebug : PinsDebugBase<PinsDebug> {
  static constexpr uint16_t count = NUM_DIGITAL_PINS;
  static constexpr uint8_t name_pad = 16;   // space needed to be pretty if not first name assigned to a pin

  static bool is_valid(const pin_t pin) { return pin >= 0 && pin < pin_t(count); }
  static int16_t analog_index(const pin_t pin) { return int(pin - analogInputToDigitalPin(0)); }
  static bool is_analog(const pin_t pin) {
    return WITHIN(pin, pin_t(analogInputToDigitalPin(0)), pin_t(analogInputToDigitalPin(NUM_ANALOG_INPUTS - 1)));
  }

  static bool is_output(const pin_t pin) { return is_valid(pin) /* && !IS_INPUT(pin) */ ; }

  // TODO: Report the PWM channel and duty when fast pwm is operative

  static void print_number(const pin_t pin) {
    char buffer[10];
    sprintf_P(buffer, PSTR("%02d"), pin);
    SERIAL_ECHO(buffer);
  }
};
