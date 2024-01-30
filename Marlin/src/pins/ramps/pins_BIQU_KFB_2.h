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
 * KFB 2.0 – Arduino Mega2560 with RAMPS v1.4 pin assignments
 * ATmega2560
 */

#if HOTENDS > 2 || E_STEPPERS > 2
  #error "KFB 2.0 supports up to 2 hotends / E steppers."
#endif

#define BOARD_INFO_NAME "KFB 2.0"

//
// Heaters / Fans
//
#define MOSFET_D_PIN                           7

#include "pins_RAMPS.h"
