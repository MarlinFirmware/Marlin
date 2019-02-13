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

#if HOTENDS > 2 || E_STEPPERS > 2
  #error "Creality3D RAMPS supports only 2 hotends / E-steppers. Comment out this line to continue."
#endif

#define BOARD_NAME "Creality3D RAMPS"

//
// Heaters / Fans
//

// Power outputs EFBF or EFBE
#define MOSFET_D_PIN 7

#define FIL_RUNOUT_PIN 2

#include "pins_RAMPS.h"

#define EXP1_PIN 65   // A11
#define EXP2_PIN 66   // A12
#define EXP3_PIN 11   // SERVO0_PIN
#define EXP4_PIN 12   // PS_ON_PIN
