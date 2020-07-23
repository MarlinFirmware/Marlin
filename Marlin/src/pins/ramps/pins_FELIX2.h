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
 * FELIXprinters v2.0/3.0 (RAMPS v1.4) pin assignments
 */

#if HOTENDS > 2 || E_STEPPERS > 2
  #error "Felix 2.0+ supports up to 2 hotends / E-steppers. Comment out this line to continue."
#endif

#define BOARD_INFO_NAME "Felix 2.0+"

//
// Heaters / Fans
//
// Power outputs EFBF or EFBE
#define MOSFET_D_PIN                          7

#include "pins_RAMPS.h"

//
// Misc. Functions
//
#define SDPOWER_PIN                           1

#define PS_ON_PIN                            12

//
// LCD / Controller
//
#if BOTH(ULTRA_LCD, NEWPANEL)

  #define SD_DETECT_PIN                       6

#endif // NEWPANEL && ULTRA_LCD

//
// M3/M4/M5 - Spindle/Laser Control
//
#undef SPINDLE_LASER_PWM_PIN                      // Definitions in pins_RAMPS.h are not valid with this board
#undef SPINDLE_LASER_ENA_PIN
#undef SPINDLE_DIR_PIN
