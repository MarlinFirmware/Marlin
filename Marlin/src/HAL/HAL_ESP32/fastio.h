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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include "i2s.h"

/**
 * Utility functions
 */

// I2S expander pin mapping.
#define IS_I2S_EXPANDER_PIN(IO) TEST(IO, 7)
#define I2S_EXPANDER_PIN_INDEX(IO) (IO & 0x7F)

// Set pin as input
#define _SET_INPUT(IO)          pinMode(IO, INPUT)

// Set pin as output
#define _SET_OUTPUT(IO)         pinMode(IO, OUTPUT)

// Set pin as input with pullup mode
#define _PULLUP(IO, v)          pinMode(IO, v ? INPUT_PULLUP : INPUT)

// Read a pin wrapper
#define READ(IO)                (IS_I2S_EXPANDER_PIN(IO) ? i2s_state(I2S_EXPANDER_PIN_INDEX(IO)) : digitalRead(IO))

// Write to a pin wrapper
#define WRITE(IO, v)            (IS_I2S_EXPANDER_PIN(IO) ? i2s_write(I2S_EXPANDER_PIN_INDEX(IO), v) : digitalWrite(IO, v))

// Set pin as input wrapper
#define SET_INPUT(IO)           _SET_INPUT(IO)

// Set pin as input with pullup wrapper
#define SET_INPUT_PULLUP(IO)    do{ _SET_INPUT(IO); _PULLUP(IO, HIGH); }while(0)

// Set pin as output wrapper
#define SET_OUTPUT(IO)          do{ _SET_OUTPUT(IO); }while(0)

// Set pin as PWM
#define SET_PWM(IO)             SET_OUTPUT(IO)

// Set pin as output and init
#define OUT_WRITE(IO,V)         do{ _SET_OUTPUT(IO); WRITE(IO,V); }while(0)

// digitalRead/Write wrappers
#define extDigitalRead(IO)      digitalRead(IO)
#define extDigitalWrite(IO,V)   digitalWrite(IO,V)

// PWM outputs
#define PWM_PIN(P)              (P < 34 || P > 127) // NOTE Pins >= 34 are input only on ESP32, so they can't be used for output.

// Toggle pin value
#define TOGGLE(IO)              WRITE(IO, !READ(IO))

//
// Ports and functions
//

// UART
#define RXD        3
#define TXD        1

// TWI (I2C)
#define SCL        5
#define SDA        4
