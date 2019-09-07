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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include "../../../inc/MarlinConfigPre.h"

#include <stdint.h>
#include "../hardware/Gpio.h"

typedef pin_type pin_t;

#define P_NC -1
constexpr uint16_t NUM_DIGITAL_PINS = Gpio::pin_count;
constexpr uint8_t NUM_ANALOG_INPUTS = 16;

#define HAL_SENSITIVE_PINS

// Get the digital pin for an analog index
pin_t analogInputToDigitalPin(const int8_t p);

// Return the index of a pin number
int16_t GET_PIN_MAP_INDEX(const pin_t pin);

// Test whether the pin is valid
bool VALID_PIN(const pin_t p);

// Get the analog index for a digital pin
int8_t DIGITAL_PIN_TO_ANALOG_PIN(const pin_t p);

// Test whether the pin is PWM
bool PWM_PIN(const pin_t p);

// Test whether the pin is interruptable
bool INTERRUPT_PIN(const pin_t p);

// Get the pin number at the given index
pin_t GET_PIN_MAP_PIN(const int16_t ind);

// Parse a G-code word into a pin index
int16_t PARSED_PIN_INDEX(const char code, const int16_t dval);
