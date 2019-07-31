/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 * SAMD51 HAL developed by Giuliano Zaro (AKA GMagician)
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
 * This comes from Arduino library which at the moment is buggy and uncompilable
 */

#pragma once

#define _useTimer1
//#define _useTimer2    // <- TODO do not activate until the code in Servo.cpp has been changed to
                        //         manage more than one channel per timer on the SAMD architecture

#define TRIM_DURATION  5      // compensation ticks to trim adjust for digitalWrite delays
#define PRESCALER      64     // timer prescaler

#ifdef _useTimer1
  #define TC_FOR_TIMER1             TC3
  #define CHANNEL_FOR_TIMER1        0
  #define INTENSET_BIT_FOR_TIMER_1  TC_INTENSET_MC0
  #define INTENCLR_BIT_FOR_TIMER_1  TC_INTENCLR_MC0
  #define INTFLAG_BIT_FOR_TIMER_1   TC_INTFLAG_MC0
  #define IRQn_FOR_TIMER1           TC3_IRQn
  #define HANDLER_FOR_TIMER1        TC3_Handler
#endif

#ifdef _useTimer2
  #define TC_FOR_TIMER2             TC3
  #define CHANNEL_FOR_TIMER2        1
  #define INTENSET_BIT_FOR_TIMER_2  TC_INTENSET_MC1
  #define INTENCLR_BIT_FOR_TIMER_2  TC_INTENCLR_MC1
  #define IRQn_FOR_TIMER2           TC3_IRQn
  #define HANDLER_FOR_TIMER2        TC3_Handler
#endif

typedef enum {
  #ifdef _useTimer1
    _timer1,
  #endif
  #ifdef _useTimer2
    _timer2,
  #endif
  _Nbr_16timers
} timer16_Sequence_t;
