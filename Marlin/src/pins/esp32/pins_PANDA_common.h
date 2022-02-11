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

#if ENABLED(MAX31856_PANDAPI)
  #define MAX31856_CLK_PIN                    29
  #define MAX31856_MISO_PIN                   24
  #define MAX31856_MOSI_PIN                   28
  #define MAX31856_CS_PIN                     27
#endif

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

/**            ------                              ------
 * (MISO 19?) |10  9 | (18 SCK?)     (BEEPER) 129 |10  9 | 12 (^ENC)
 *   (EN1) 33 | 8  7 | (5 SDSS?)     (EN)      26 | 8  7 | 27 (RS)
 *   (EN2) 32   6  5 | (23 MOSI?)    (D4)      14 | 6  5   --
 * (SDDET 2?) | 4  3 | (RESET)                 -- | 4  3 | --
 *         -- | 2  1 | --                   (GND) | 2  1 | (5V)
 *             ------                              ------
 *              EXP2                                EXP1
 */
#define EXP1_06_PIN                           14
#define EXP1_07_PIN                           27
#define EXP1_08_PIN                           26
#define EXP1_09_PIN                           12
#define EXP1_10_PIN                          129

#define EXP2_04_PIN                            2  // ?
#define EXP2_05_PIN                           23  // ?
#define EXP2_06_PIN                           32
#define EXP2_07_PIN                            5  // ?
#define EXP2_08_PIN                           33
#define EXP2_09_PIN                           18  // ?
#define EXP2_10_PIN                           19  // ?

//
// SD Card
//
#if ENABLED(SDSUPPORT)
  #define SD_MOSI_PIN                EXP2_05_PIN
  #define SD_MISO_PIN                EXP2_10_PIN
  #define SD_SCK_PIN                 EXP2_09_PIN
  #define SDSS                       EXP2_07_PIN
  #define SD_DETECT_PIN              EXP2_04_PIN
#endif

#if HAS_WIRED_LCD
  #define BEEPER_PIN                 EXP1_10_PIN

  #define BTN_ENC                    EXP1_09_PIN
  #define BTN_EN1                    EXP2_08_PIN
  #define BTN_EN2                    EXP2_06_PIN

  #define LCD_PINS_RS                EXP1_07_PIN
  #define LCD_PINS_ENABLE            EXP1_08_PIN
  #define LCD_PINS_D4                EXP1_06_PIN
#endif
