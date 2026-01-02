/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2022 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * Based on WEEDO 62A pin configuration
 * Copyright (c) 2019 WEEDO3D Perron
 * ATmega2560
 */

#include "env_validate.h"

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "WEEDO 62B"
#endif

#define LARGE_FLASH true //depricated maybe? was in manuf pin file

//
// Limit Switches (only has max limit switches, but manuf pin file had mins)
//
//#define X_MIN_PIN                            3 //manuf file
#define X_MAX_PIN                              2
//#define Y_MIN_PIN                           -1 //manuf file
#define Y_MAX_PIN                             41
#define Z_MIN_PIN                             18  // manuf file has 18, but why not 32 to match probe pin?
#define Z_MAX_PIN                             19

//
// Steppers
//
#define X_STEP_PIN                            54
#define X_DIR_PIN                             55
#define X_ENABLE_PIN                          38

#define Y_STEP_PIN                            60
#define Y_DIR_PIN                             61
#define Y_ENABLE_PIN                          56

#define Z_STEP_PIN                            46
#define Z_DIR_PIN                             48
#define Z_ENABLE_PIN                          62

#define E0_STEP_PIN                           26
#define E0_DIR_PIN                            28
#define E0_ENABLE_PIN                         24

//
// Temperature Sensors
//
#define TEMP_0_PIN                            13  // ANALOG NUMBERING
#define TEMP_BED_PIN                          14  // ANALOG NUMBERING

//
// Heaters / Fans
//
#define HEATER_0_PIN                          10
#define HEATER_BED_PIN                         8
#define FAN0_PIN                               9

//
// Misc. Functions
//
//#define PS_ON_PIN                           12 //from WEEDO_62A, what is?
#define LED_PIN                               13
#define CASE_LIGHT_PIN                        13 //not sure which?
#define FIL_RUNOUT_PIN						             3
#define DOOROPEN_PIN						              40  //from manuf file, is this implemented?
#define Z_MIN_PROBE_PIN						            32
//#define SOL1_PIN							              19  //from manuf file, same as Z_MAX_PIN?, not sure what it could be

//
// SD Support
//
#if HAS_MEDIA
  #define SD_SS_PIN                           53
  #define SD_DETECT_PIN                       49
#endif

//
// LCD / Controller
//

#if HAS_WIRED_LCD
  #define BEEPER_PIN                          37

  #define DOGLCD_A0                           27
  #define DOGLCD_CS                           29
  #define LCD_RESET_PIN                       25
  #define LCD_CONTRAST_INIT                  205

  #define BTN_EN1                             33
  #define BTN_EN2                             31
  #define BTN_ENC                             35
#endif
