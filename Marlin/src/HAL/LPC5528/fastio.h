/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

// #define _READ(IO)               // bool(READ_BIT(FastIOPortMap[STM_PORT(digitalPinToPinName(IO))]->IDR, _BV32(STM_PIN(digitalPinToPinName(IO)))))
// #define _TOGGLE(IO)             // TBI32(FastIOPortMap[STM_PORT(digitalPinToPinName(IO))]->ODR, STM_PIN(digitalPinToPinName(IO)))

#define _READ(IO)               digitalRead(IO)

#define _GET_MODE(IO)

#define _SET_MODE(IO,M)          pinMode(IO, M)
#define _SET_OUTPUT(IO)          pinMode(IO, OUTPUT)                               //!< Output Push Pull Mode & GPIO_NOPULL
#define _SET_OUTPUT_OD(IO)       pinMode(IO, OUTPUT_OPEN_DRAIN)

#define WRITE(IO,V)             digitalWrite(IO, V)
#define READ(IO)                digitalRead(IO)
#define TOGGLE(IO)              WRITE(IO, !READ(IO))

#define OUT_WRITE(IO,V)         do{ _SET_OUTPUT(IO); WRITE(IO,V); }while(0)
#define OUT_WRITE_OD(IO,V)      do{ _SET_OUTPUT_OD(IO); WRITE(IO,V); }while(0)

#define SET_INPUT(IO)           _SET_MODE(IO, INPUT)                              //!< Input Floating Mode
#define SET_INPUT_PULLUP(IO)    _SET_MODE(IO, INPUT_PULLUP)                       //!< Input with Pull-up activation
#define SET_INPUT_PULLDOWN(IO)  _SET_MODE(IO, INPUT_PULLUP)                     //!< Input with Pull-down activation
#define SET_OUTPUT(IO)           OUT_WRITE(IO, LOW)
#define SET_PWM(IO)             _SET_MODE(IO, PWM)

#define IS_INPUT(IO)
#define IS_OUTPUT(IO)

#define PWM_PIN(P)              10 //digitalPinHasPWM(P)
#define NO_COMPILE_TIME_PWM

// digitalRead/Write wrappers
#define extDigitalRead(IO)      //digitalRead(IO)
#define extDigitalWrite(IO,V)   //digitalWrite(IO,V)
