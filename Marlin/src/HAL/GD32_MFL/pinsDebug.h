/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2025 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * Pins Debugging for GD32
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

#include "../../inc/MarlinConfig.h"
#include <Arduino.h>
#include <PinOps.hpp>
#include <Analog.h>

#ifndef TOTAL_PIN_COUNT
  #error "Expected TOTAL_PIN_COUNT not found."
#endif

#define NUM_DIGITAL_PINS  TOTAL_PIN_COUNT
#define NUMBER_PINS_TOTAL TOTAL_PIN_COUNT

#define getPinByIndex(x) pin_t(pin_array[x].pin)
#define isValidPin(P) WITHIN(P, 0, (NUM_DIGITAL_PINS - 1))
#define digitalRead_mod(P) extDigitalRead(P)
#define printPinNumber(P) do { sprintf_P(buffer, PSTR("%3hd "), pin_t(P)); SERIAL_ECHO(buffer); } while (0)
#define printPinAnalog(P) do { sprintf_P(buffer, PSTR(" (A%2d)  "), pin_t(getAdcChannelFromPin(P))); SERIAL_ECHO(buffer); } while (0)
#define printPinNameByIndex(x) do { sprintf_P(buffer, PSTR("%-" STRINGIFY(MAX_NAME_LENGTH) "s"), pin_array[x].name); SERIAL_ECHO(buffer); } while (0)

#define MULTI_NAME_PAD 21 // Space needed to be pretty if not first name assigned to a pin

#ifndef M43_NEVER_TOUCH
  #define M43_NEVER_TOUCH(x) WITHIN(x, 9, 10) // SERIAL pins: PA9(TX) PA10(RX)
#endif

bool isAnalogPin(const pin_t pin) {
  if (!isValidPin(pin)) return false;

  if (getAdcChannel(pin) != adc::ADC_Channel::INVALID) {
    const PortPinPair& pp = port_pin_map[pin];
    auto& instance = gpio::GPIO::get_instance(pp.port).value();
    return instance.get_pin_mode(pp.pin) == gpio::Pin_Mode::ANALOG && !M43_NEVER_TOUCH(pin);
  }

  return false;
}

bool getValidPinMode(const pin_t pin) {
  if (!isValidPin(pin)) return false;

  const PortPinPair& pp = port_pin_map[pin];
  auto& instance = gpio::GPIO::get_instance(pp.port).value();
  gpio::Pin_Mode mode = instance.get_pin_mode(pp.pin);

  return mode != gpio::Pin_Mode::ANALOG && mode != gpio::Pin_Mode::INPUT_FLOATING &&
         mode != gpio::Pin_Mode::INPUT_PULLUP && mode != gpio::Pin_Mode::INPUT_PULLDOWN;
}

bool getPinIsDigitalByIndex(const int16_t index) {
  const pin_t pin = getPinByIndex(index);
  return (!isAnalogPin(pin));
}

int8_t digitalPinToAnalogIndex(const pin_t pin) {
  if (!isValidPin(pin) || !isAnalogPin(pin)) return -1;
  return pin; // Analog and digital pin indexes are shared
}

bool pwm_status(const pin_t pin) { return false; }
void printPinPWM(const pin_t pin) { /* TODO */ }
void printPinPort(const pin_t pin) { /* TODO */ }
