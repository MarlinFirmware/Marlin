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
 * CNControls V15 for HMS434 pin assignments
 * ATmega2560, ATmega1280
 */

#define ALLOW_MEGA1280
#include "env_validate.h"

#define BOARD_INFO_NAME "CN Controls V15"

//
// Servos
//
#define SERVO0_PIN                            PinH3

//
// Limit Switches
//
#define X_STOP_PIN                            PinC3
#define Y_STOP_PIN                            PinG2
#define Z_STOP_PIN                            PinK0

#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                     PinL0
#endif

//
// Steppers
//
#define X_STEP_PIN                            PinJ1
#define X_DIR_PIN                             PinA3
#define X_ENABLE_PIN                          PinA4

#define Y_STEP_PIN                            PinB5
#define Y_DIR_PIN                             PinB6
#define Y_ENABLE_PIN                          PinJ0

#define Z_STEP_PIN                            PinA2
#define Z_DIR_PIN                             PinA5
#define Z_ENABLE_PIN                          PinA6

#define E0_STEP_PIN                           PinK2
#define E0_DIR_PIN                            PinK3
#define E0_ENABLE_PIN                         PinK1

//
// Temperature Sensors
// Analog Inputs
//
#define TEMP_0_PIN                            PinE4  // Analog Input
#define TEMP_BED_PIN                          PinG5  // Analog Input

#ifndef TEMP_CHAMBER_PIN
  #define TEMP_CHAMBER_PIN                    PinE3  // Analog Input
#endif

//
// Heaters
//
#define HEATER_0_PIN                          PinG5
#define HEATER_BED_PIN                        PinC5
#define HEATER_CHAMBER_PIN                    PinC4

//
// Fans
//
#define FAN_PIN                               PinH5

//
// Auto fans
//
#define AUTO_FAN_PIN                          PinC7
#ifndef E0_AUTO_FAN_PIN
  #define E0_AUTO_FAN_PIN           AUTO_FAN_PIN
#endif
#ifndef E1_AUTO_FAN_PIN
  #define E1_AUTO_FAN_PIN           AUTO_FAN_PIN
#endif
#ifndef E2_AUTO_FAN_PIN
  #define E2_AUTO_FAN_PIN           AUTO_FAN_PIN
#endif
#ifndef E3_AUTO_FAN_PIN
  #define E3_AUTO_FAN_PIN           AUTO_FAN_PIN
#endif
#ifndef CHAMBER_AUTO_FAN_PIN
  //#define CHAMBER_AUTO_FAN_PIN              PinB4
#endif

//
// Misc. Functions
//
#define SDSS                                  PinB0
#define SD_DETECT_PIN                         PinG1

// Common I/O

#define FIL_RUNOUT_PIN                        PinH6
//#define FIL_RUNOUT_PIN                      PinA7  // encoder sensor
//#define PWM_1_PIN                           PinB6
//#define PWM_2_PIN                           PinB7
//#define SPARE_IO                            PinH0
#define BEEPER_PIN                            PinB7
#define STAT_LED_BLUE_PIN                     -1
#define STAT_LED_RED_PIN                      PinB4  // 31
