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
 * MKS BASE – Arduino Mega2560 with RAMPS pin assignments
 * ATmega2560
 */

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "MKS BASE"
#endif

#if MKS_BASE_VERSION >= 14
  //
  // Heaters / Fans
  //
  // Power outputs EFBF or EFBE
  #define MOSFET_D_PIN                         7

  //
  // M3/M4/M5 - Spindle/Laser Control
  //
  #if HAS_CUTTER
    #define SPINDLE_LASER_PWM_PIN              2  // Hardware PWM
    #define SPINDLE_LASER_ENA_PIN             15  // Pullup!
    #define SPINDLE_DIR_PIN                   19
  #endif

  #ifndef CASE_LIGHT_PIN
    #define CASE_LIGHT_PIN                     2
  #endif

#endif

//
// Microstepping pins
//
#if MKS_BASE_VERSION >= 14                        //     |===== 1.4 =====|===== 1.5+ =====|
  #define X_MS1_PIN                            5  // PE3 | Pin  5 | PWM5 |        | D3    | SERVO2_PIN
  #define X_MS2_PIN                            6  // PH3 | Pin 15 | PWM6 | Pin 14 | D6    | SERVO1_PIN
  #define Y_MS1_PIN                           59  // PF5 | Pin 92 | A5   |        |       |
  #define Y_MS2_PIN                           58  // PF4 | Pin 93 | A4   |        |       |
  #define Z_MS1_PIN                           22  // PA0 | Pin 78 | D22  |        |       |
  #define Z_MS2_PIN                           39  // PG2 | Pin 70 | D39  |        |       |
  #if MKS_BASE_VERSION == 14
    #define E0_MS1_PIN                        64  // PK2 | Pin 87 | A10  |        |       |
    #define E0_MS2_PIN                        63  // PK1 | Pin 88 | A9   |        |       |
  #else
    #define E0_MS1_PIN                        63  // PK1 |        |      | Pin 86 | A9    |
    #define E0_MS2_PIN                        64  // PK2 |        |      | Pin 87 | A10   |
  #endif
  #define E1_MS1_PIN                          57  // PF3 | Pin 94 | A3   | Pin 93 | A3    |
  #define E1_MS2_PIN                           4  // PG5 | Pin  1 | PWM4 |        | D4    | SERVO3_PIN
#endif

#include "pins_RAMPS.h"
