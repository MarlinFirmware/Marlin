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
 * Wanhao 0ne+ pin assignments
 * ATmega2560
 */

#include "env_validate.h"

#define BOARD_INFO_NAME      "Wanhao i3 Mini 0ne+"
#define DEFAULT_MACHINE_NAME "i3 Mini"
#define BOARD_WEBSITE_URL    "tinyurl.com/yyxw7se7"

//
// Limit Switches
//
#define X_STOP_PIN                            PinD2
#define Y_STOP_PIN                            PinD3
#define Z_STOP_PIN                            PinD7

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                     PinD7
#endif

//
// Steppers
//
#define X_STEP_PIN                            PinA0
#define X_DIR_PIN                             PinA1
#define X_ENABLE_PIN                          PinF3

#define Y_STEP_PIN                            PinA3
#define Y_DIR_PIN                             PinA4
#define Y_ENABLE_PIN                          PinA2

#define Z_STEP_PIN                            PinA7
#define Z_DIR_PIN                             PinG2
#define Z_ENABLE_PIN                          PinA6

#define E0_STEP_PIN                           PinF1
#define E0_DIR_PIN                            PinF2
#define E0_ENABLE_PIN                         PinF0

//
// Temperature Sensors
//
#define TEMP_0_PIN                            PinB7
#define TEMP_BED_PIN                          PinJ1

//
// Heaters / Fans
//
#define HEATER_0_PIN                          PinG5
#define HEATER_BED_PIN                        PinL5
#define FAN_PIN                               PinB6  // IO pin. Buffer needed

//
// SD Card
//
#define SD_DETECT_PIN                         PinD6
#define SDSS                                  PinB0

//
// Misc. Functions
//
#define BEEPER_PIN                            PinC0
#define KILL_PIN                              PinK2

//
// LCD / Controller (Integrated MINIPANEL)
//
#if ENABLED(MINIPANEL)
  #define DOGLCD_A0                           PinG1
  #define DOGLCD_CS                           PinG0
  #define LCD_BACKLIGHT_PIN                   PinK3  // Backlight LED on A11/D65
  #define LCD_RESET_PIN                       PinA5

  #define BTN_EN1                             PinE4
  #define BTN_EN2                             PinE5
  #define BTN_ENC                             PinE3

  // This display has adjustable contrast
  #define LCD_CONTRAST_MIN                     0
  #define LCD_CONTRAST_MAX                   255
  #define LCD_CONTRAST_INIT     LCD_CONTRAST_MAX
#endif
