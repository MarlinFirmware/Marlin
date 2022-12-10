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

/**
 * Based on WEEDO 62A pin configuration
 * Copyright (c) 2019 WEEDO3D Perron
 * ATmega2560
 */

#pragma once

#include "env_validate.h"

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "WEEDO 62A"
#endif

//
// Limit Switches
//
#define X_MIN_PIN                             PinE5
#define X_MAX_PIN                             PinE4
#define Y_MIN_PIN                             PinG1
#define Y_MAX_PIN                             PinG0
#define Z_MIN_PIN                             PinD3
#define Z_MAX_PIN                             PinD2

//
// Steppers
//
#define X_STEP_PIN                            PinA4
#define X_DIR_PIN                             PinA6
#define X_ENABLE_PIN                          PinA2

#define Y_STEP_PIN                            PinF6
#define Y_DIR_PIN                             PinF7
#define Y_ENABLE_PIN                          PinF2

#define Z_STEP_PIN                            PinL3
#define Z_DIR_PIN                             PinL1
#define Z_ENABLE_PIN                          PinK0

#define E0_STEP_PIN                           PinF0
#define E0_DIR_PIN                            PinF1
#define E0_ENABLE_PIN                         PinD7

//
// Temperature Sensors
//
#define TEMP_0_PIN                            PinB7  // ANALOG NUMBERING
#define TEMP_BED_PIN                          PinJ1  // ANALOG NUMBERING

//
// Heaters / Fans
//
#define HEATER_0_PIN                          PinB4  // EXTRUDER 1
#define HEATER_BED_PIN                        PinH5  // BED
#define FAN_PIN                               PinG5  // IO pin. Buffer needed

//
// Misc. Functions
//
#define PS_ON_PIN                             PinB6
#define LED_PIN                               PinB7

//
// SD Support
//
#if ENABLED(SDSUPPORT)
  #define SDSS                                PinB0
  #define SD_DETECT_PIN                       PinL0
#endif

//
// LCD / Controller
//
#if HAS_WIRED_LCD
  #define BEEPER_PIN                          PinC0

  #define DOGLCD_A0                           PinA5
  #define DOGLCD_CS                           PinA7
  #define LCD_RESET_PIN                       PinA3
  #define LCD_CONTRAST_INIT                  255

  #define BTN_EN1                             PinC4
  #define BTN_EN2                             PinC6
  #define BTN_ENC                             PinC2
#endif
