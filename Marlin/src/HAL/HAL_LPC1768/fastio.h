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
 * Description: Fast IO functions LPC1768
 *
 * For TARGET LPC1768
 */

#ifndef _FASTIO_LPC1768_H
#define _FASTIO_LPC1768_H

#include <LPC17xx.h>
#include "arduino.h"
#include "pinmapping.h"

bool useable_hardware_PWM(uint8_t pin);
#define USEABLE_HARDWARE_PWM(pin) useable_hardware_PWM(pin)

#define LPC_PORT_OFFSET         (0x0020)
#define LPC_PIN(pin)            (1UL << pin)
#define LPC_GPIO(port)          ((volatile LPC_GPIO_TypeDef *)(LPC_GPIO0_BASE + LPC_PORT_OFFSET * port))

#define SET_DIR_INPUT(IO)       (LPC_GPIO(DIO ## IO ## _PORT)->FIODIR &= ~LPC_PIN(DIO ## IO ##_PIN))
#define SET_DIR_OUTPUT(IO)      (LPC_GPIO(DIO ## IO ## _PORT)->FIODIR |=  LPC_PIN(DIO ## IO ##_PIN))

#define SET_MODE(IO, mode)      (pin_mode((DIO ## IO ## _PORT, DIO ## IO ## _PIN), mode))

#define WRITE_PIN_SET(IO)       (LPC_GPIO(DIO ## IO ## _PORT)->FIOSET = LPC_PIN(DIO ## IO ##_PIN))
#define WRITE_PIN_CLR(IO)       (LPC_GPIO(DIO ## IO ## _PORT)->FIOCLR = LPC_PIN(DIO ## IO ##_PIN))

#define READ_PIN(IO)            ((LPC_GPIO(DIO ## IO ## _PORT)->FIOPIN & LPC_PIN(DIO ## IO ##_PIN)) ? 1 : 0)
#define WRITE_PIN(IO, v)        ((v) ? WRITE_PIN_SET(IO) : WRITE_PIN_CLR(IO))

/**
  magic I/O routines
  now you can simply SET_OUTPUT(STEP); WRITE(STEP, 1); WRITE(STEP, 0);
*/

/// Read a pin
#define _READ(IO) READ_PIN(IO)

/// Write to a pin
#define _WRITE_VAR(IO, v) digitalWrite(IO, v)

#define _WRITE(IO, v) WRITE_PIN(IO, v)

/// toggle a pin
#define _TOGGLE(IO) _WRITE(IO, !READ(IO))

/// set pin as input
#define _SET_INPUT(IO) SET_DIR_INPUT(IO)

/// set pin as output
#define _SET_OUTPUT(IO) SET_DIR_OUTPUT(IO)

/// set pin as input with pullup mode
#define _PULLUP(IO, v) (pinMode(IO, (v!=LOW ? INPUT_PULLUP : INPUT)))

/// check if pin is an input
#define _GET_INPUT(IO)
/// check if pin is an output
#define _GET_OUTPUT(IO)

/// check if pin is an timer
#define _GET_TIMER(IO)

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
#define SET_INPUT_PULLUP(IO) do{ _SET_INPUT(IO); _PULLUP(IO, HIGH); }while(0)
/// set pin as output wrapper
#define SET_OUTPUT(IO)  do{ _SET_OUTPUT(IO); _WRITE(IO, LOW); }while(0)

/// check if pin is an input wrapper
#define GET_INPUT(IO)  _GET_INPUT(IO) // todo: Never used?
/// check if pin is an output wrapper
#define GET_OUTPUT(IO)  _GET_OUTPUT(IO) //todo: Never Used?

/// check if pin is an timer wrapper
#define GET_TIMER(IO)  _GET_TIMER(IO)

// Shorthand
#define OUT_WRITE(IO, v) { SET_OUTPUT(IO); WRITE(IO, v); }

#endif // _FASTIO_LPC1768_H
