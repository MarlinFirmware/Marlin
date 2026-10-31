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
 *
 *   - NUMBER_PINS_TOTAL
 *   - MULTI_NAME_PAD
 *   - getPinByIndex(index)
 *   - printPinNameByIndex(index)
 *   - getPinIsDigitalByIndex(index)
 *   - digitalPinToAnalogIndex(pin)
 *   - getValidPinMode(pin)
 *   - isValidPin(pin)
 *   - isAnalogPin(pin)
 *   - digitalRead_mod(pin)
 *   - pwm_status(pin)
 *   - printPinPWM(pin)
 *   - printPinPort(pin)
 *   - printPinNumber(pin)
 *   - printPinAnalog(pin)
 */

#define NUMBER_PINS_TOTAL NUM_DIGITAL_PINS
#define isAnalogPin(P) (digitalPinToAnalogIndex(P) >= 0)
#define printPinNumber printPinNumber
FORCE_INLINE void printPinNumber(const pin_t pin) {
  char buffer[12];
  sprintf_P(buffer, PSTR("P%d_%02d"), LPC176x::pin_port(pin), LPC176x::pin_bit(pin));
  SERIAL_ECHO(buffer);
}
#define printPinAnalog printPinAnalog
FORCE_INLINE void printPinAnalog(const pin_t pin) {
  char buffer[12];
  sprintf_P(buffer, PSTR("_A%d     "), LPC176x::pin_get_adc_channel(pin));
  SERIAL_ECHO(buffer);
}
#define MULTI_NAME_PAD 17 // space needed to be pretty if not first name assigned to a pin

// pins that will cause hang/reset/disconnect in M43 Toggle and Watch utilities
#ifndef M43_NEVER_TOUCH
  #define M43_NEVER_TOUCH(Q) ((Q) == P0_29 || (Q) == P0_30 || (Q) == P2_09)  // USB pins
#endif

bool getValidPinMode(const pin_t pin) {
  if (!LPC176x::pin_is_valid(pin) || LPC176x::pin_adc_enabled(pin)) // Invalid pin or active analog pin
    return false;

  return LPC176x::gpio_direction(pin);
}

void printPinPort(const pin_t) {}
void printPinPWM(const pin_t) {}
bool pwm_status(const pin_t) { return false; }
