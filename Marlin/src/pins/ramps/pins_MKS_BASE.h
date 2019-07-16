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
#pragma once

/**
 * MKS BASE 1.0 – Arduino Mega2560 with RAMPS v1.4 pin assignments
 *
 * Rev B - Override pin definitions for CASE_LIGHT and M3/M4/M5 spindle control
 */

#if HOTENDS > 2 || E_STEPPERS > 2
  #error "MKS BASE 1.0 supports up to 2 hotends / E-steppers. Comment out this line to continue."
#endif

#define BOARD_NAME "MKS BASE 1.0"

//
// Heaters / Fans
//
// Power outputs EFBF or EFBE
#define MOSFET_D_PIN              7

#define CASE_LIGHT_PIN            2

//
// M3/M4/M5 - Spindle/Laser Control
//
#define SPINDLE_LASER_PWM_PIN     2   // Hardware PWM
#define SPINDLE_LASER_ENA_PIN    15   // Pullup!
#define SPINDLE_DIR_PIN          19

#include "pins_RAMPS.h"
