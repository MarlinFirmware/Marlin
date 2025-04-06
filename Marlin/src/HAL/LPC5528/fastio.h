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
 * Fast I/O Routines for LPC5528
 * Use direct port manipulation to save scads of processor time.
 * Contributed by Triffid_Hunter and modified by Kliment, thinkyhead, Bob-the-Kuhn, et.al.
 */

/**
 * Description: Fast IO functions LPC5528
 *
 * For TARGET LPC5528
 */

#include "../shared/Marduino.h"

#ifndef PWM
  #define PWM OUTPUT
#endif
#define NO_COMPILE_TIME_PWM
#define PWM_PIN(P)            true // All pins are PWM capable

/**
 * Magic I/O routines
 *
 * Now you can simply SET_OUTPUT(STEP); WRITE(STEP, HIGH); WRITE(STEP, LOW);
 *
 * Why double up on these macros? see https://gcc.gnu.org/onlinedocs/gcc-4.8.5/cpp/Stringification.html
 */

#define _GET_MODE(IO)
#define _SET_MODE(IO,M)          pinMode(IO, M)

/// Read a pin
#define _READ(IO)               digitalRead(IO)

/// Set pin as output
#define _SET_OUTPUT(IO)         pinMode(IO, OUTPUT)             //!< Output Push Pull Mode & GPIO_NOPULL
#define _SET_OUTPUT_OD(IO)      pinMode(IO, OUTPUT_OPEN_DRAIN)

/// Read a pin wrapper
#define READ(IO)                digitalRead(IO)

/// Write to a pin wrapper
#define WRITE(IO,V)             digitalWrite(IO, V)

/// Toggle a pin wrapper
#define TOGGLE(IO)              WRITE(IO, !READ(IO))

/// Set pin as input wrapper
#define SET_INPUT(IO)           _SET_MODE(IO, INPUT)            //!< Input Floating Mode
/// Set pin as input with pullup wrapper
#define SET_INPUT_PULLUP(IO)    _SET_MODE(IO, INPUT_PULLUP)     //!< Input with Pull-up activation
/// Set pin as input with pulldown wrapper
#define SET_INPUT_PULLDOWN(IO)  _SET_MODE(IO, INPUT_PULLUP)     //!< Input with Pull-down activation
/// Set pin as output wrapper  -  reads the pin and sets the output to that value
#define SET_OUTPUT(IO)           OUT_WRITE(IO, LOW)
/// Set pin as PWM
#define SET_PWM(IO)             _SET_MODE(IO, PWM)

/// Check if pin is an input wrapper
#define IS_INPUT(IO)
/// Check if pin is an output wrapper
#define IS_OUTPUT(IO)

// Shorthand
#define OUT_WRITE(IO,V)         do{ _SET_OUTPUT(IO); WRITE(IO,V); }while(0)
#define OUT_WRITE_OD(IO,V)      do{ _SET_OUTPUT_OD(IO); WRITE(IO,V); }while(0)

// digitalRead/Write wrappers
#define extDigitalRead(IO)      //digitalRead(IO)
#define extDigitalWrite(IO,V)   //digitalWrite(IO,V)
