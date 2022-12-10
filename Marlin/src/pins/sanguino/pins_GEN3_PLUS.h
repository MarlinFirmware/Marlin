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
 * Gen3+ pin assignments
 */

/**
 * Rev B    26 DEC 2016
 *
 * added pointer to a current Arduino IDE extension
 */

/**
 * A useable Arduino IDE extension (board manager) can be found at
 * https://github.com/Lauszus/Sanguino
 *
 * This extension has been tested on Arduino 1.6.12 & 1.8.0
 *
 * Here's the JSON path:
 * https://raw.githubusercontent.com/Lauszus/Sanguino/master/package_lauszus_sanguino_index.json
 *
 * When installing select 1.0.2
 *
 * Installation instructions can be found at https://learn.sparkfun.com/pages/CustomBoardsArduino
 * Just use the above JSON URL instead of Sparkfun's JSON.
 *
 * Once installed select the SANGUINO board and then select the CPU.
 */

#define ALLOW_MEGA644P
#include "env_validate.h"

#define BOARD_INFO_NAME "Gen3+"

//
// Limit Switches
//
#define X_STOP_PIN                            PinC4
#define Y_STOP_PIN                            PinA6
#define Z_STOP_PIN                            PinA1

//
// Steppers
//
#define X_STEP_PIN                            PinD7
#define X_DIR_PIN                             PinC2
#define X_ENABLE_PIN                          PinC3

#define Y_STEP_PIN                            PinC7
#define Y_DIR_PIN                             PinC6
#define Y_ENABLE_PIN                          PinA7

#define Z_STEP_PIN                            PinA4
#define Z_DIR_PIN                             PinA3
#define Z_ENABLE_PIN                          PinA2

#define E0_STEP_PIN                           PinC1
#define E0_DIR_PIN                            PinC5
#define E0_ENABLE_PIN                         PinD5

//
// Temperature Sensors
//
#define TEMP_0_PIN                            PinB0  // Analog Input (pin 33 extruder)
#define TEMP_BED_PIN                          PinB5  // Analog Input (pin 34 bed)

//
// Heaters
//
#define HEATER_0_PIN                          PinD4
#define HEATER_BED_PIN                        PinC0

//
// Misc. Functions
//
#define SDSS                                  PinB4
#define PS_ON_PIN                             PinD6
