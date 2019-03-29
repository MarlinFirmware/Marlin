/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * Contributed by Triffid_Hunter and modified by Kliment, thinkyhead, Bob-the-Kuhn, et.al.
 */

#pragma once

#ifndef MASK
  #define MASK(PIN) (1 << PIN)
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

#define _READ(P) bool(CORE_PIN ## P ## _PINREG & CORE_PIN ## P ## _BITMASK)

#define _WRITE(P,V) do{ \
  if (V) CORE_PIN ## P ## _PORTSET = CORE_PIN ## P ## _BITMASK; \
  else CORE_PIN ## P ## _PORTCLEAR = CORE_PIN ## P ## _BITMASK; \
}while(0)

#define _TOGGLE(P) (*(&(CORE_PIN ## P ## _PORTCLEAR)+1) = CORE_PIN ## P ## _BITMASK)

#define _SET_INPUT(P) do{ \
  CORE_PIN ## P ## _CONFIG = PORT_PCR_MUX(1); \
  GPIO_BITBAND(CORE_PIN ## P ## _DDRREG , CORE_PIN ## P ## _BIT) = 0; \
}while(0)

#define _SET_OUTPUT(P) do{ \
  CORE_PIN ## P ## _CONFIG = PORT_PCR_MUX(1)|PORT_PCR_SRE|PORT_PCR_DSE; \
  GPIO_BITBAND(CORE_PIN ## P ## _DDRREG , CORE_PIN ## P ## _BIT) = 1; \
}while(0)

#define _SET_INPUT_PULLUP(P) do{ \
  CORE_PIN ## P ## _CONFIG = PORT_PCR_MUX(1) | PORT_PCR_PE | PORT_PCR_PS; \
  GPIO_BITBAND(CORE_PIN ## P ## _DDRREG , CORE_PIN ## P ## _BIT) = 0; \
}while(0)

#define _IS_INPUT(P)    ((CORE_PIN ## P ## _DDRREG & CORE_PIN ## P ## _BITMASK) == 0)
#define _IS_OUTPUT(P)   ((CORE_PIN ## P ## _DDRREG & CORE_PIN ## P ## _BITMASK) == 0)

#define READ(IO)              _READ(IO)

#define WRITE_VAR(IO,V)       _WRITE_VAR(IO,V)
#define WRITE(IO,V)           _WRITE(IO,V)
#define TOGGLE(IO)            _TOGGLE(IO)

#define SET_INPUT(IO)         _SET_INPUT(IO)
#define SET_INPUT_PULLUP(IO)  _SET_INPUT_PULLUP(IO)
#define SET_OUTPUT(IO)        _SET_OUTPUT(IO)
#define SET_PWM(IO)            SET_OUTPUT(IO)

#define IS_INPUT(IO)          _IS_INPUT(IO)
#define IS_OUTPUT(IO)         _IS_OUTPUT(IO)

#define OUT_WRITE(IO,V)       do{ SET_OUTPUT(IO); WRITE(IO,V); }while(0)

// digitalRead/Write wrappers
#define extDigitalRead(IO)    digitalRead(IO)
#define extDigitalWrite(IO,V) digitalWrite(IO,V)

#define PWM_PIN(P)            digitalPinHasPWM(P)
#define USEABLE_HARDWARE_PWM(P) PWM_PIN(P)

/**
 * Ports, functions, and pins
 */

#define DIO0_PIN 8
