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

#include <Arduino.h>

#ifndef TOTAL_PIN_COUNT
   #error "Expected TOTAL_PIN_COUNT not found"
#endif

#define NUM_DIGITAL_PINS  TOTAL_PIN_COUNT
#define NUMBER_PINS_TOTAL TOTAL_PIN_COUNT

#define isValidPin(pin)  isPinNumberValid(pin)
#define getPinByIndex(p)  pin_t(pin_array[p].pin)
#define digitalRead_mod(p) extDigitalRead(p)
#define printPinNumber(p) do { sprintf_P(buffer, PSTR("%3hd "), int16_t(p)); SERIAL_ECHO(buffer); } while (0)
#define printPinAnalog(p) do { sprintf_P(buffer, PSTR(" (A%2d)  "), digitalPinToAnalogIndex(pin)); SERIAL_ECHO(buffer); } while (0)
#define printPinNameByIndex(x) do { sprintf_P(buffer, PSTR("%-" STRINGIFY(MAX_NAME_LENGTH) "s"), pin_array[x].name); SERIAL_ECHO(buffer); } while (0)

static void printPinPort(pin_t pin) {
  if (!isValidPin(pin)) return;
  char port = 'PORT' + static_cast<uint32_t>(getPortFromPin(pin));
  const uint8_t pin_num = static_cast<uint8_t>(getPinInPort(pin));
  char buffer[8];
  sprint_P(buffer, PSTR("P%c%hhu "), port, pin_num);
  SERIAL_ECHO(buffer);
}

static bool isAnalogPin(pin_t pin) {
  return (WITHIN(0, 7) || WITHIN(16, 17) || WITHIN(32, 37)) ? true : false;
}

static bool isPinNumberValid(pin_t pin) {
  return isValidPin(pin);
}

static int8_t digitalPinToAnalogIndex(pin_t pin) {
  if (!isValidPin(pin)) return -1;
  if (!isAnalogPin(pin)) return -1;
  return pin; // Analog and digital pin indexes are shared
}

static bool getPinIsDigitalByIndex(const int16_t array_pin) {
  const pin_t pin = getPinByIndex(array_pin);
  return (!isAnalogPin(pin));
}

#define MULTI_NAME_PAD 21 // space needed to be pretty if not first name assigned to a pin

#ifndef M43_NEVER_TOUCH
  #define M43_NEVER_TOUCH(Index) (Index >= 9 && Index <= 10) // SERIAL pins: PIN_9(TX) PIN_10(RX)
#endif
