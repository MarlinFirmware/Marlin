/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2025 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * Wanhao Duplicator 9 pin assignments (MK1, MK2 and MK3 share this board)
 * ATmega2560
 */

#include "env_validate.h"

#if NOT_TARGET(__AVR_ATmega2560__)
  #error "Oops! Select 'Arduino/Genuino Mega or Mega 2560' in 'Tools > Board.'"
#endif

#define BOARD_INFO_NAME      "Wanhao D9"
#define DEFAULT_MACHINE_NAME BOARD_INFO_NAME
#define BOARD_WEBSITE_URL    "github.com/Le-Syl21/WANHAO-Duplicator-9"

#define LARGE_FLASH true

//
// Limit Switches
//
#ifndef X_MIN_PIN
  #define X_MIN_PIN                           54
#endif
#ifndef Y_MIN_PIN
  #define Y_MIN_PIN                           24
#endif
#ifndef Z_MIN_PIN
  #define Z_MIN_PIN                            6
#endif

//
// Steppers
//
#define X_STEP_PIN                            61
#define X_DIR_PIN                             62
#define X_ENABLE_PIN                          60

#define Y_STEP_PIN                            64
#define Y_DIR_PIN                             65
#define Y_ENABLE_PIN                           2

#define Z_STEP_PIN                            67
#define Z_DIR_PIN                             69
#define Z_ENABLE_PIN                          66

#define E0_STEP_PIN                           58
#define E0_DIR_PIN                            59
#define E0_ENABLE_PIN                         57

//
// Temperature Sensors
//
#define TEMP_0_PIN                             1
#define TEMP_BED_PIN                          14

//
// Heaters / Fans
//
#define HEATER_0_PIN                           4
#define HEATER_BED_PIN                         3
#define FAN0_PIN                               5

//
// Servos
//
#define SERVO0_PIN                             7

//
// SD Card
//
#define SD_DETECT_PIN                         49
#define SD_SS_PIN                             53

//
// Misc. Functions
//
#define LED_PIN                               13
#define KILL_PIN                              -1

#ifndef FILWIDTH_PIN
  #define FILWIDTH_PIN                        12
#endif

//
// Filament Runout Sensor
//
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                       8
#endif

//
// LCD / Controller
//

#if HAS_WIRED_LCD
  #define BEEPER_PIN                          27

  #define LCD_PINS_RS                         37
  #define LCD_PINS_EN                         36

  #define LCD_PINS_D4                         34
  #define LCD_PINS_D5                         35
  #define LCD_PINS_D6                         32
  #define LCD_PINS_D7                         33

  #define BTN_ENC                             30
  #define BTN_EN1                             29
  #define BTN_EN2                             28
#endif // HAS_WIRED_LCD

//
// Power Loss Recovery
//
#ifndef POWER_LOSS_PIN
  #define POWER_LOSS_PIN                      63
#endif
#ifndef POWER_LOSS_STATE
  #define POWER_LOSS_STATE                   LOW
#endif
