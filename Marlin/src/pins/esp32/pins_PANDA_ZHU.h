/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * Panda ZHU pin assignments
 */

#define BOARD_INFO_NAME "Panda_ZHU"

#if E_STEPPERS > 5
  #error "PANDA ZHU supports up to 5 E steppers."
#endif
#if HAS_MULTI_HOTEND
  #error "PANDA ZHU only supports 1 hotend."
#endif

#include "pins_PANDA_common.h"

//
// Steppers
//
#define X_ENABLE_PIN                         128  // Shared with all steppers
#define Y_ENABLE_PIN                X_ENABLE_PIN
#define Z_ENABLE_PIN                X_ENABLE_PIN
#define E0_ENABLE_PIN               X_ENABLE_PIN

//#define X_CS_PIN                             0
//#define Y_CS_PIN                            13
//#define Z_CS_PIN                             5  // SS_PIN
//#define E0_CS_PIN                           21

#define E1_STEP_PIN                          115
#define E1_DIR_PIN                           114
#define E1_ENABLE_PIN               X_ENABLE_PIN

#define E2_STEP_PIN                          112
#define E2_DIR_PIN                           113
#define E2_ENABLE_PIN               X_ENABLE_PIN

#define E3_STEP_PIN                          110
#define E3_DIR_PIN                           111
#define E3_ENABLE_PIN               X_ENABLE_PIN

#define E4_STEP_PIN                          121
#define E4_DIR_PIN                           122
#define E4_ENABLE_PIN               X_ENABLE_PIN

#define HEATER_1_PIN                         123
