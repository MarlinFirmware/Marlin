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
#pragma once

/**
 * HAL/shared/Marduino.h
 */

#undef DISABLED       // Redefined by ESP32
#undef M_PI           // Redefined by all
#undef _BV            // Redefined by some
#undef sq             // Redefined by teensy3/wiring.h

#include <Arduino.h>  // NOTE: If included earlier then this line is a NOOP

#undef DISABLED
#define DISABLED(V...) DO(DIS,&&,V)

#undef _BV
#define _BV(b) (1UL << (b))

#undef sq
#define sq(x) ((x)*(x))

#ifndef SBI
  #define SBI(A,B) (A |= (1 << (B)))
#endif

#ifndef CBI
  #define CBI(A,B) (A &= ~(1 << (B)))
#endif
