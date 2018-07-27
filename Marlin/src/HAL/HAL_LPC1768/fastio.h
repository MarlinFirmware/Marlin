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
 * Fast I/O Routines for LPC1768/9
 * Use direct port manipulation to save scads of processor time.
 * Contributed by Triffid_Hunter and modified by Kliment, thinkyhead, Bob-the-Kuhn, et.al.
 */

/**
 * Description: Fast IO functions LPC1768
 *
 * For TARGET LPC1768
 */

#ifndef _FASTIO_LPC1768_H
#define _FASTIO_LPC1768_H

#include <LPC17xx.h>
#include <Arduino.h>
#include <pinmapping.h>

bool useable_hardware_PWM(pin_t pin);
#define USEABLE_HARDWARE_PWM(pin) useable_hardware_PWM(pin)

#define LPC_PORT_OFFSET         (0x0020)
#define LPC_PIN(pin)            (1UL << pin)
#define LPC_GPIO(port)          ((volatile LPC_GPIO_TypeDef *)(LPC_GPIO0_BASE + LPC_PORT_OFFSET * port))

#define SET_DIR_INPUT(IO)       (LPC_GPIO(LPC1768_PIN_PORT(IO))->FIODIR &= ~LPC_PIN(LPC1768_PIN_PIN(IO)))
#define SET_DIR_OUTPUT(IO)      (LPC_GPIO(LPC1768_PIN_PORT(IO))->FIODIR |=  LPC_PIN(LPC1768_PIN_PIN(IO)))

#define SET_MODE(IO, mode)      (pin_mode((LPC1768_PIN_PORT(IO), LPC1768_PIN_PIN(IO)), mode))

#define WRITE_PIN_SET(IO)       (LPC_GPIO(LPC1768_PIN_PORT(IO))->FIOSET = LPC_PIN(LPC1768_PIN_PIN(IO)))
#define WRITE_PIN_CLR(IO)       (LPC_GPIO(LPC1768_PIN_PORT(IO))->FIOCLR = LPC_PIN(LPC1768_PIN_PIN(IO)))

#define READ_PIN(IO)            ((LPC_GPIO(LPC1768_PIN_PORT(IO))->FIOPIN & LPC_PIN(LPC1768_PIN_PIN(IO))) ? 1 : 0)
#define WRITE_PIN(IO,V)         ((V) ? WRITE_PIN_SET(IO) : WRITE_PIN_CLR(IO))

/**
 * Magic I/O routines
 *
 * Now you can simply SET_OUTPUT(STEP); WRITE(STEP, HIGH); WRITE(STEP, LOW);
 *
 * Why double up on these macros? see http://gcc.gnu.org/onlinedocs/cpp/Stringification.html
 */

/// Read a pin
#define _READ(IO)         READ_PIN(IO)

/// Write to a pin
#define _WRITE_VAR(IO,V)  digitalWrite(IO,V)

#define _WRITE(IO,V)      WRITE_PIN(IO,V)

/// toggle a pin
#define _TOGGLE(IO)       _WRITE(IO, !READ(IO))

/// set pin as input
#define _SET_INPUT(IO)    SET_DIR_INPUT(IO)

/// set pin as output
#define _SET_OUTPUT(IO)   SET_DIR_OUTPUT(IO)

/// set pin as input with pullup mode
#define _PULLUP(IO,V)     pinMode(IO, (V) ? INPUT_PULLUP : INPUT)

/// set pin as input with pulldown mode
#define _PULLDOWN(IO,V)   pinMode(IO, (V) ? INPUT_PULLDOWN : INPUT)

/// check if pin is an input
#define _GET_INPUT(IO)    (LPC_GPIO(LPC1768_PIN_PORT(IO))->FIODIR & LPC_PIN(LPC1768_PIN_PIN(IO)) != 0)

/// check if pin is an output
#define _GET_OUTPUT(IO)   (LPC_GPIO(LPC1768_PIN_PORT(IO))->FIODIR & LPC_PIN(LPC1768_PIN_PIN(IO)) == 0)

/// check if pin is a timer
/// all gpio pins are pwm capable, either interrupt or hardware pwm controlled
#define _GET_TIMER(IO)    TRUE

/// Read a pin wrapper
#define READ(IO)          _READ(IO)

/// Write to a pin wrapper
#define WRITE_VAR(IO,V)   _WRITE_VAR(IO,V)
#define WRITE(IO,V)       _WRITE(IO,V)

/// toggle a pin wrapper
#define TOGGLE(IO)        _TOGGLE(IO)

/// set pin as input wrapper
#define SET_INPUT(IO)     _SET_INPUT(IO)
/// set pin as input with pullup wrapper
#define SET_INPUT_PULLUP(IO)    do{ _SET_INPUT(IO); _PULLUP(IO, HIGH); }while(0)
/// set pin as input with pulldown wrapper
#define SET_INPUT_PULLDOWN(IO)  do{ _SET_INPUT(IO); _PULLDOWN(IO, HIGH); }while(0)
/// set pin as output wrapper  -  reads the pin and sets the output to that value
#define SET_OUTPUT(IO)          do{ _WRITE(IO, _READ(IO)); _SET_OUTPUT(IO); }while(0)

/// check if pin is an input wrapper
#define GET_INPUT(IO)     _GET_INPUT(IO)
/// check if pin is an output wrapper
#define GET_OUTPUT(IO)    _GET_OUTPUT(IO)

/// check if pin is a timer (wrapper)
#define GET_TIMER(IO)     _GET_TIMER(IO)

// Shorthand
#define OUT_WRITE(IO,V)   do{ SET_OUTPUT(IO); WRITE(IO,V); }while(0)

#endif // _FASTIO_LPC1768_H
