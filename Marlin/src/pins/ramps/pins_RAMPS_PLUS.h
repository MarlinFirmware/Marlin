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
 * Arduino Mega with RAMPS v1.4Plus, also known as 3DYMY version, pin assignments
 * ATmega2560, ATmega1280
 *
 * Applies to the following boards:
 *
 *  RAMPS_PLUS_EFB (Extruder, Fan, Bed)
 *  RAMPS_PLUS_EEB (Extruder, Extruder, Bed)
 *  RAMPS_PLUS_EFF (Extruder, Fan, Fan)
 *  RAMPS_PLUS_EEF (Extruder, Extruder, Fan)
 *  RAMPS_PLUS_SF  (Spindle, Controller Fan)
 *
 * Differences from RAMPS v1.4:
 *  - Swap heater E0 with E1
 *  - Swap pins 8 and 10. Bed/Fan/Hotend as labeled on the board are on pins 8/9/10.
 *  - Change EXP1/2 pins
 */

#include "env_validate.h"

#define BOARD_INFO_NAME "RAMPS 1.4 Plus"

#define MOSFET_A_PIN                          PinH5
#define MOSFET_C_PIN                          PinB4

//
// Steppers
//
#define X_CS_PIN                              -1
#define Y_CS_PIN                              -1
#define Z_CS_PIN                              -1

// Swap E0 / E1 on 3DYMY
#define E0_STEP_PIN                           PinC1
#define E0_DIR_PIN                            PinC3
#define E0_ENABLE_PIN                         PinC7
#define E0_CS_PIN                             -1

#define E1_STEP_PIN                           PinA4
#define E1_DIR_PIN                            PinA6
#define E1_ENABLE_PIN                         PinA2
#define E1_CS_PIN                             -1

/**             3DYMY Expansion Headers
 *         ------                     ------
 *     37 | 1  2 | 35      (MISO) 50 | 1  2 | 52 (SCK)
 *     31 | 3  4 | 41             29 | 3  4 | 53
 *     33   5  6 | 23             25   5  6 | 51 (MOSI)
 *     42 | 7  8 | 44             49 | 7  8 | 27
 *    GND | 9 10 | 5V            GND | 9 10 | --
 *         ------                     ------
 *          EXP1                       EXP2
 */
#define EXP1_01_PIN                           PinC0
#define EXP1_02_PIN                           PinC2
#define EXP1_03_PIN                           PinC6
#define EXP1_04_PIN                           PinG0
#define EXP1_05_PIN                           PinC4
#define EXP1_06_PIN                           PinA1
#define EXP1_07_PIN                           PinL7
#define EXP1_08_PIN                           PinL5

#define EXP2_01_PIN                           PinB3
#define EXP2_02_PIN                           PinB1
#define EXP2_03_PIN                           PinA7
#define EXP2_04_PIN                           PinB0
#define EXP2_05_PIN                           PinA3
#define EXP2_06_PIN                           PinB2
#define EXP2_07_PIN                           PinL0
#define EXP2_08_PIN                           PinA5

#include "pins_RAMPS.h"
