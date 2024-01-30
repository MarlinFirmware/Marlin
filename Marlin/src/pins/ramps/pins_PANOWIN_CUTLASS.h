/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * Panowin V?.? as found in the Panowin F1 (MEGA2560) board pin assignments
 *
 * Ported from https://github.com/sambuls/MarlinOnTrinus
 * Board photo https://imgur.com/a/xvol1Bo
 */

#include "env_validate.h"

#ifndef BOARD_NAME
  #define BOARD_NAME "Panowin Cutlass"
#endif

#ifndef DEFAULT_MACHINE_NAME
  #define DEFAULT_MACHINE_NAME "Panowin F1"
#endif

//
// Limit Switches
//
#define X_STOP_PIN                            71  // G3
#define Y_STOP_PIN                            85  // H7
#define Z_STOP_PIN                            13  // B7

//
// Steppers
//
#define X_STEP_PIN                            38  // D7
#define X_DIR_PIN                             83
#define X_ENABLE_PIN                          82

#define Y_STEP_PIN                            37
#define Y_DIR_PIN                             40
#define Y_ENABLE_PIN                          41

#define Z_STEP_PIN                            30  // C7
#define Z_DIR_PIN                             32  // C5
#define Z_ENABLE_PIN                          34  // C3

#define E0_STEP_PIN                           42
#define E0_DIR_PIN                            43
#define E0_ENABLE_PIN                         44

// Microstepping mode pins
#define X_MS1_PIN                             19  // D2
#define X_MS2_PIN                             18  // D3
#define X_MS3_PIN                             81  // D4

#define Y_MS1_PIN                             19  // D2
#define Y_MS2_PIN                             18  // D3
#define Y_MS3_PIN                             81  // D4

#define Z_MS1_PIN                             73  // J3
#define Z_MS2_PIN                             75  // J4
#define Z_MS3_PIN                             76  // J5

#define E0_MS1_PIN                            46  // L3
#define E0_MS2_PIN                            47  // L2
#define E0_MS3_PIN                            45  // L4

//
// Temperature Sensors
//
#define TEMP_0_PIN                            12  // Analog Input
#define TEMP_BED_PIN                          14  // Analog Input

//
// Heaters / Fans
//
#define HEATER_0_PIN                           9  // H6
#define HEATER_BED_PIN                         8
#define FAN0_PIN                              62  // K0
#define FAN_SOFT_PWM_REQUIRED

//
// M3/M4/M5 - Spindle/Laser Control
//
#if HAS_CUTTER
  #define SPINDLE_LASER_ENA_PIN               74  // J7
#endif

//
// SD Card
//
#define SD_DETECT_PIN                         28  // A6 Onboard SD

//
// Misc. Functions
//
#define BEEPER_PIN                            24  // A2

//
// Onboard (bright!) RGB LED
//
#ifndef RGB_LED_R_PIN
  #define RGB_LED_R_PIN                       64  // K2
#endif
#ifndef RGB_LED_G_PIN
  #define RGB_LED_G_PIN                       65  // K3
#endif
#ifndef RGB_LED_B_PIN
  #define RGB_LED_B_PIN                       63  // K1
#endif
