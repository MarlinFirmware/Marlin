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
 * Panda common pin assignments
 */

#include "env_validate.h"

#define DEFAULT_MACHINE_NAME BOARD_INFO_NAME

//
// Servos
//
#define SERVO0_PIN                             0

//
// Limit Switches
//
#define X_STOP_PIN                             4
#define Y_STOP_PIN                            35
#define Z_STOP_PIN                            21

//
// Steppers
//
#define X_STEP_PIN                           101
#define X_DIR_PIN                            100

#define Y_STEP_PIN                           103
#define Y_DIR_PIN                            102

#define Z_STEP_PIN                           105
#define Z_DIR_PIN                            104

#define E0_STEP_PIN                          107
#define E0_DIR_PIN                           106

//
// Temperature Sensors
//
#define TEMP_0_PIN                            39  // Analog Input
#define TEMP_BED_PIN                          36  // Analog Input

//
// Heaters / Fans
//
#define HEATER_0_PIN                         108
#define HEATER_BED_PIN                       109
#define FAN_PIN                              118  // FAN0
#define FAN1_PIN                             119  // FAN1

#ifndef E0_AUTO_FAN_PIN
  #define E0_AUTO_FAN_PIN                    120  // FAN2
#endif

//
// SD card
//
#if ENABLED(SDSUPPORT)
  #define SD_MOSI_PIN                         23
  #define SD_MISO_PIN                         19
  #define SD_SCK_PIN                          18
  #define SDSS                                 5
  #define SD_DETECT_PIN                        2
#endif

#if HAS_WIRED_LCD
  #define BEEPER_PIN                         129
  #define BTN_ENC                             12

  #define BTN_EN1                             33
  #define BTN_EN2                             32

  #define LCD_PINS_RS                         27
  #define LCD_PINS_ENABLE                     26
  #define LCD_PINS_D4                         14
#endif
