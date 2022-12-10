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
 * Cheaptronic v1.0 pin assignments
 * ATmega2560
 */

#include "env_validate.h"

#define BOARD_INFO_NAME "Cheaptronic v1.0"
//
// Limit Switches
//
#define X_STOP_PIN                            PinE5
#define Y_STOP_PIN                            PinE4
#define Z_STOP_PIN                            PinE3

//
// Steppers
//
#define X_STEP_PIN                            PinJ1
#define X_DIR_PIN                             PinJ0
#define X_ENABLE_PIN                          PinA2

#define Y_STEP_PIN                            PinC2
#define Y_DIR_PIN                             PinC1
#define Y_ENABLE_PIN                          PinC6

#define Z_STEP_PIN                            PinG1
#define Z_DIR_PIN                             PinG0
#define Z_ENABLE_PIN                          PinC0

#define E0_STEP_PIN                           PinA4
#define E0_DIR_PIN                            PinA6
#define E0_ENABLE_PIN                         PinA3

#define E1_STEP_PIN                           PinC4
#define E1_DIR_PIN                            PinC3
#define E1_ENABLE_PIN                         PinC7

//
// Temperature sensors
//
#define TEMP_0_PIN                            PinJ0  // Analog Input
#define TEMP_1_PIN                            PinJ1  // Analog Input
#define TEMP_BED_PIN                          PinB7  // Analog Input

//
// Heaters / Fans
//
#define HEATER_0_PIN                          PinD2  // EXTRUDER 1
#define HEATER_1_PIN                          PinA1  // EXTRUDER 2
#define HEATER_BED_PIN                        PinA0

//
// LCD / Controller
//
// Cheaptronic v1.0 doesn't support LCD
