/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#if HOTENDS > 2 || E_STEPPERS > 2
  #error "Dagoma3D F5 RAMPS supports only 2 hotends / E-steppers. Comment out this line to continue."
#endif

#define BOARD_NAME "Dagoma3D F5 RAMPS"

#define X_STOP_PIN          2
#define Y_STOP_PIN          3
#define Z_STOP_PIN         15
#define FIL_RUNOUT_PIN     39

#define ORIG_E0_AUTO_FAN_PIN 7

//
// Import RAMPS 1.4 pins
//
#include "pins_RAMPS.h"
