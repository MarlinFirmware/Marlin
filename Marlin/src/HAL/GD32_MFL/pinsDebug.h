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
 * See PinsDebugBase in pins/pinsDebug.h for the hooks.
 */

#include "../../inc/MarlinConfig.h"
#include <Arduino.h>
#include <PinOps.hpp>
#include <Analog.h>

#ifndef TOTAL_PIN_COUNT
  #error "Expected TOTAL_PIN_COUNT not found."
#endif

#define NUM_DIGITAL_PINS  TOTAL_PIN_COUNT

#define PINS_DEBUG_CLASS

struct PinsDebug : PinsDebugBase<PinsDebug> {
  static constexpr uint16_t count = TOTAL_PIN_COUNT;
  static constexpr uint8_t name_pad = 21;   // Space needed to be pretty if not first name assigned to a pin

  static bool is_valid(const pin_t pin) { return WITHIN(pin, 0, (NUM_DIGITAL_PINS - 1)); }

  // SERIAL pins: PA9(TX) PA10(RX)
  static bool never_touch(const uint16_t index) { return WITHIN(index, 9, 10); }

  static bool is_analog(const pin_t pin) {
    if (!is_valid(pin)) return false;

    if (getAdcChannel(pin) != adc::ADC_Channel::INVALID) {
      const PortPinPair& pp = port_pin_map[pin];
      auto& instance = gpio::GPIO::get_instance(pp.port).value();
      return instance.get_pin_mode(pp.pin) == gpio::Pin_Mode::ANALOG && !never_touch(pin);
    }

    return false;
  }

  static int16_t analog_index(const pin_t pin) {
    if (!is_valid(pin) || !is_analog(pin)) return -1;
    return pin; // Analog and digital pin indexes are shared
  }

  static bool is_output(const pin_t pin) {
    if (!is_valid(pin)) return false;

    const PortPinPair& pp = port_pin_map[pin];
    auto& instance = gpio::GPIO::get_instance(pp.port).value();
    gpio::Pin_Mode mode = instance.get_pin_mode(pp.pin);

    return mode != gpio::Pin_Mode::ANALOG && mode != gpio::Pin_Mode::INPUT_FLOATING &&
           mode != gpio::Pin_Mode::INPUT_PULLUP && mode != gpio::Pin_Mode::INPUT_PULLDOWN;
  }

  static bool is_digital_at(const uint16_t index) { return !is_analog(getPinByIndex(index)); }

  // TODO: pwm_active, print_pwm, print_port

  static void print_number(const pin_t pin) {
    char buffer[10];
    sprintf_P(buffer, PSTR("%3hd "), pin_t(pin));
    SERIAL_ECHO(buffer);
  }

  static void print_analog(const pin_t pin) {
    char buffer[12];
    sprintf_P(buffer, PSTR(" (A%2d)  "), pin_t(getAdcChannelFromPin(pin)));
    SERIAL_ECHO(buffer);
  }
};
