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
 * servo_private.h - Interrupt driven Servo library for Arduino using 16 bit timers- Version 2
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

#include <stdint.h>

// Architecture specific include
#ifdef __AVR__
  #include "../AVR/ServoTimers.h"
#elif defined(ARDUINO_ARCH_SAM)
  #include "../DUE/ServoTimers.h"
#elif defined(__SAMD51__)
  #include "../SAMD51/ServoTimers.h"
#elif defined(__SAMD21__)
  #include "../SAMD21/ServoTimers.h"
#else
  #error "This library only supports boards with an AVR, SAM3X, SAMD21 or SAMD51 processor."
#endif

// Macros

#define MIN_PULSE_WIDTH       544     // the shortest pulse sent to a servo
#define MAX_PULSE_WIDTH      2400     // the longest pulse sent to a servo
#define DEFAULT_PULSE_WIDTH  1500     // default pulse width when servo is attached
#define REFRESH_INTERVAL    20000     // minimum time to refresh servos in microseconds

#define SERVOS_PER_TIMER       12     // the maximum number of servos controlled by one timer
#define MAX_SERVOS   (_Nbr_16timers  * SERVOS_PER_TIMER)

#define INVALID_SERVO         255     // flag indicating an invalid servo index

// Convert microseconds to ticks and back (PRESCALER depends on architecture)
#define usToTicks(_us)    (clockCyclesPerMicrosecond() * (_us) / (SERVO_TIMER_PRESCALER))
#define ticksToUs(_ticks) (unsigned(_ticks) * (SERVO_TIMER_PRESCALER) / clockCyclesPerMicrosecond())

// convenience macros
#define SERVO_INDEX_TO_TIMER(_servo_nbr) timer16_Sequence_t(_servo_nbr / (SERVOS_PER_TIMER)) // the timer controlling this servo
#define SERVO_INDEX_TO_CHANNEL(_servo_nbr) (_servo_nbr % (SERVOS_PER_TIMER))      // the index of the servo on this timer
#define SERVO_INDEX(_timer,_channel)  ((_timer*(SERVOS_PER_TIMER)) + _channel)    // servo index by timer and channel
#define SERVO(_timer,_channel)  servo_info[SERVO_INDEX(_timer,_channel)]          // servo class by timer and channel

// Types

typedef struct {
  uint8_t nbr        : 7 ;            // a pin number from 0 to 127
  uint8_t isActive   : 1 ;            // true if this channel is enabled, pin not pulsed if false
} ServoPin_t;

typedef struct {
  ServoPin_t Pin;
  unsigned int ticks;
} ServoInfo_t;

// Global variables

extern uint8_t ServoCount;
extern ServoInfo_t servo_info[MAX_SERVOS];

// Public functions

void initISR(const timer16_Sequence_t timer_index);
void finISR(const timer16_Sequence_t timer_index);
