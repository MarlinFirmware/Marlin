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
 * Leapfrog Driver board pin assignments
 *  ATmega2560, ATmega1280
 */

#define ALLOW_MEGA1280
#include "env_validate.h"

#define BOARD_INFO_NAME "Leapfrog"

//
// Limit Switches
//
#define X_MIN_PIN                             PinL2
#define X_MAX_PIN                             PinE4
#define Y_MIN_PIN                             PinL1
#define Y_MAX_PIN                             PinJ0
#define Z_MIN_PIN                             PinL0
#define Z_MAX_PIN                             -1

//
// Steppers
//
#define X_STEP_PIN                            PinA6
#define X_DIR_PIN                             PinK1
#define X_ENABLE_PIN                          PinA7

#define Y_STEP_PIN                            PinJ1  // A6
#define Y_DIR_PIN                             PinJ0  // A0
#define Y_ENABLE_PIN                          PinG2

#define Z_STEP_PIN                            PinC6  // A2
#define Z_DIR_PIN                             PinC5  // A6
#define Z_ENABLE_PIN                          PinC7  // A1

#define E0_STEP_PIN                           PinC4  // 34
#define E0_DIR_PIN                            PinC2  // 35
#define E0_ENABLE_PIN                         PinC4  // 33

#define E1_STEP_PIN                           PinC0  // 37
#define E1_DIR_PIN                            PinG1  // 40
#define E1_ENABLE_PIN                         PinC1  // 36

//
// Temperature Sensors
//
#define TEMP_0_PIN                            PinB7  // Analog Input (D27)
#define TEMP_1_PIN                            PinJ0  // Analog Input (1)
#define TEMP_BED_PIN                          PinJ1  // Analog Input (1,2 or I2C)

//
// Heaters / Fans
//
#define HEATER_0_PIN                          PinH6
#define HEATER_1_PIN                          PinH5  // 12
#define HEATER_2_PIN                          PinB5  // 13
#define HEATER_BED_PIN                        PinB4  // 14/15

#define FAN_PIN                               PinH4

//
// Misc. Functions
//
#define SDSS                                  PinB5
#define LED_PIN                               PinB7
#define SOL1_PIN                              PinH1
#define SOL2_PIN                              PinH0

/*  Unused (1) (2) (3) 4 5 6 7 8 9 10 11 12 13 (14) (15) (16) 17 (18) (19) (20) (21) (22) (23) 24 (25) (26) (27) 28 (29) (30) (31)  */
