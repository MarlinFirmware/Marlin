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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * ServoTimers.h - Interrupt driven Servo library for Arduino using 16 bit timers- Version 2
 * Copyright (c) 2009 Michael Margolis.  All right reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

/**
 * Defines for 16 bit timers used with  Servo library
 *
 * If _useTimerX is defined then TimerX is a 16 bit timer on the current board
 * timer16_Sequence_t enumerates the sequence that the timers should be allocated
 * _Nbr_16timers indicates how many 16 bit timers are available.
 */

/**
 * AVR Only definitions
 * --------------------
 */

#define TRIM_DURATION           2   // compensation ticks to trim adjust for digitalWrite delays
#define SERVO_TIMER_PRESCALER   8   // timer prescaler

// Say which 16 bit timers can be used and in what order
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  //#define _useTimer1
  #define _useTimer3
  #if NUM_SERVOS > SERVOS_PER_TIMER
    #define _useTimer4
    #if !HAS_MOTOR_CURRENT_PWM && SERVOS > 2 * SERVOS_PER_TIMER
      #define _useTimer5 // Timer 5 is used for motor current PWM and can't be used for servos.
    #endif
  #endif
#elif defined(__AVR_ATmega32U4__)
  #define _useTimer3
#elif defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__)
  #define _useTimer3
#elif defined(__AVR_ATmega128__) || defined(__AVR_ATmega1281__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega2561__)
  #define _useTimer3
#else
  // everything else
#endif

typedef enum {
  #ifdef _useTimer1
    _timer1,
  #endif
  #ifdef _useTimer3
    _timer3,
  #endif
  #ifdef _useTimer4
    _timer4,
  #endif
  #ifdef _useTimer5
    _timer5,
  #endif
  _Nbr_16timers
} timer16_Sequence_t;
