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
 * Fast I/O Routines for Teensy 3.5 and Teensy 3.6
 * Use direct port manipulation to save scads of processor time.
 * Contributed by Triffid_Hunter. Modified by Kliment and the Marlin team.
 */

#ifndef _FASTIO_TEENSY_H
#define _FASTIO_TEENSY_H

#ifndef MASK
  #define MASK(PIN)  (1 << PIN)
#endif

#define GPIO_BITBAND_ADDR(reg, bit) (((uint32_t)&(reg) - 0x40000000) * 32 + (bit) * 4 + 0x42000000)
#define GPIO_BITBAND(reg, bit) (*(uint32_t *)GPIO_BITBAND_ADDR((reg), (bit)))

/**
 * Magic I/O routines
 *
 * Now you can simply SET_OUTPUT(PIN); WRITE(PIN, HIGH); WRITE(PIN, LOW);
 *
 * Why double up on these macros? see http://gcc.gnu.org/onlinedocs/cpp/Stringification.html
 */

#define _READ(p) ((bool)(CORE_PIN ## p ## _PINREG & CORE_PIN ## p ## _BITMASK))
#define _WRITE(p, v) do { if (v) CORE_PIN ## p ## _PORTSET = CORE_PIN ## p ## _BITMASK; \
                            else CORE_PIN ## p ## _PORTCLEAR = CORE_PIN ## p ## _BITMASK; } while (0)
#define _TOGGLE(p)  (*(&(CORE_PIN ## p ## _PORTCLEAR)+1) = CORE_PIN ## p ## _BITMASK)
#define _SET_INPUT(p)   do { CORE_PIN ## p ## _CONFIG = PORT_PCR_MUX(1); \
                          GPIO_BITBAND(CORE_PIN ## p ## _DDRREG , CORE_PIN ## p ## _BIT) = 0; \
                          } while (0)
#define _SET_OUTPUT(p)  do { CORE_PIN ## p ## _CONFIG = PORT_PCR_MUX(1)|PORT_PCR_SRE|PORT_PCR_DSE; \
                          GPIO_BITBAND(CORE_PIN ## p ## _DDRREG , CORE_PIN ## p ## _BIT) = 1; \
                          } while (0)

//#define _PULLUP(IO, v)  { pinMode(IO, (v!=LOW ? INPUT_PULLUP : INPUT)); }

#define _GET_INPUT(p)   ((CORE_PIN ## p ## _DDRREG & CORE_PIN ## p ## _BITMASK) == 0)
#define _GET_OUTPUT(p)  ((CORE_PIN ## p ## _DDRREG & CORE_PIN ## p ## _BITMASK) == 0)

//#define _GET_TIMER(IO)

#define READ(IO)  _READ(IO)

#define WRITE_VAR(IO, v)  _WRITE_VAR(IO, v)
#define WRITE(IO, v)  _WRITE(IO, v)
#define TOGGLE(IO)  _TOGGLE(IO)

#define SET_INPUT(IO)  _SET_INPUT(IO)
#define SET_INPUT_PULLUP(IO) do{ _SET_INPUT(IO); _WRITE(IO, HIGH); }while(0)
#define SET_OUTPUT(IO)  _SET_OUTPUT(IO)

#define GET_INPUT(IO)  _GET_INPUT(IO)
#define GET_OUTPUT(IO)  _GET_OUTPUT(IO)

#define OUT_WRITE(IO, v) { SET_OUTPUT(IO); WRITE(IO, v); }

/**
 * Ports, functions, and pins
 */

#define DIO0_PIN 8

#endif  /* _FASTIO_TEENSY_H */
