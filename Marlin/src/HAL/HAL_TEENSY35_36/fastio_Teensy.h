/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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

/**
  This code contributed by Triffid_Hunter and modified by Kliment
  why double up on these macros? see http://gcc.gnu.org/onlinedocs/cpp/Stringification.html
*/

/**
 * Description: Fast IO functions for Teensy 3.5 and Teensy 3.6
 */

#ifndef	_FASTIO_TEENSY_H
#define	_FASTIO_TEENSY_H

/**
  utility functions
*/

#ifndef MASK
  #define MASK(PIN)  (1 << PIN)
#endif

#define GPIO_BITBAND_ADDR(reg, bit) (((uint32_t)&(reg) - 0x40000000) * 32 + (bit) * 4 + 0x42000000)
#define GPIO_BITBAND(reg, bit) (*(uint32_t *)GPIO_BITBAND_ADDR((reg), (bit)))

/**
  magic I/O routines
  now you can simply SET_OUTPUT(STEP); WRITE(STEP, 1); WRITE(STEP, 0);
*/

/// Read a pin
#define _READ(p) ((bool)(CORE_PIN ## p ## _PINREG & CORE_PIN ## p ## _BITMASK))

/// Write to a pin
#define _WRITE(p, v) do { if (v) CORE_PIN ## p ## _PORTSET = CORE_PIN ## p ## _BITMASK; \
                            else CORE_PIN ## p ## _PORTCLEAR = CORE_PIN ## p ## _BITMASK; } while (0)

/// toggle a pin
#define _TOGGLE(p)  (*(&(CORE_PIN ## p ## _PORTCLEAR)+1) = CORE_PIN ## p ## _BITMASK)

/// set pin as input
  #define _SET_INPUT(p)   do { CORE_PIN ## p ## _CONFIG = PORT_PCR_MUX(1); \
                            GPIO_BITBAND(CORE_PIN ## p ## _DDRREG , CORE_PIN ## p ## _BIT) = 0; \
                            } while (0)
/// set pin as output
  #define _SET_OUTPUT(p)  do { CORE_PIN ## p ## _CONFIG = PORT_PCR_MUX(1)|PORT_PCR_SRE|PORT_PCR_DSE; \
                            GPIO_BITBAND(CORE_PIN ## p ## _DDRREG , CORE_PIN ## p ## _BIT) = 1; \
                            } while (0)

/// set pin as input with pullup mode
//#define _PULLUP(IO, v)  { pinMode(IO, (v!=LOW ? INPUT_PULLUP : INPUT)); }

/// check if pin is an input
#define _GET_INPUT(p)   ((CORE_PIN ## p ## _DDRREG & CORE_PIN ## p ## _BITMASK) == 0)
/// check if pin is an output
#define _GET_OUTPUT(p)  ((CORE_PIN ## p ## _DDRREG & CORE_PIN ## p ## _BITMASK) == 0)

/// check if pin is an timer
//#define _GET_TIMER(IO)

//  why double up on these macros? see http://gcc.gnu.org/onlinedocs/cpp/Stringification.html

/// Read a pin wrapper
#define READ(IO)  _READ(IO)

/// Write to a pin wrapper
#define WRITE_VAR(IO, v)  _WRITE_VAR(IO, v)
#define WRITE(IO, v)  _WRITE(IO, v)

/// toggle a pin wrapper
#define TOGGLE(IO)  _TOGGLE(IO)

/// set pin as input wrapper
#define SET_INPUT(IO)  _SET_INPUT(IO)
/// set pin as input with pullup wrapper
#define SET_INPUT_PULLUP(IO) do{ _SET_INPUT(IO); _WRITE(IO, HIGH); }while(0)
/// set pin as output wrapper
#define SET_OUTPUT(IO)  _SET_OUTPUT(IO)

/// check if pin is an input wrapper
#define GET_INPUT(IO)  _GET_INPUT(IO)
/// check if pin is an output wrapper
#define GET_OUTPUT(IO)  _GET_OUTPUT(IO)

// Shorthand
#define OUT_WRITE(IO, v) { SET_OUTPUT(IO); WRITE(IO, v); }

/**
  ports and functions

  added as necessary or if I feel like it- not a comprehensive list!
*/

/**
pins
*/

#define DIO0_PIN 8

#endif	/* _FASTIO_TEENSY_H */
