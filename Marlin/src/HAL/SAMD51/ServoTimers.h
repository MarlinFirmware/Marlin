/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

#define _useTimer1
#define _useTimer2

#define TRIM_DURATION           5   // compensation ticks to trim adjust for digitalWrite delays
#define SERVO_TIMER_PRESCALER   64  // timer prescaler factor to 64 (avoid overflowing 16-bit clock counter, at 120MHz this is 1831 ticks per millisecond

#define SERVO_TC                3

typedef enum {
  #ifdef _useTimer1
    _timer1,
  #endif
  #ifdef _useTimer2
    _timer2,
  #endif
  _Nbr_16timers
} timer16_Sequence_t;
