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
 * Brainwave 1.0 pin assignments (AT90USB646)
 * https://www.reprap.org/wiki/Brainwave
 *
 * Schematic: https://github.com/unrepentantgeek/Brainwave/blob/master/brainwave/brainwave.sch
 *
 * Build with PlatformIO for best results.
 *
 * Arduino IDE requires hardware bundle
 *   https://github.com/unrepentantgeek/brainwave-arduino
 *   (May also require retired Marlin_AT90USB package)
 *
 *  Rev B  16 JAN 2017
 *  - Added pointer to a currently available Arduino IDE extension that will
 *    allow this board to use the latest Marlin software
 *
 *  Rev C  2 JUN 2017
 *  - Converted to Arduino pin numbering
 */

#if NOT_TARGET(__AVR_AT90USB646__, __AVR_AT90USB1286__)
  #error "Oops! Select 'Brainwave' in 'Tools > Board.'"
#endif

#define BOARD_INFO_NAME "Brainwave"

//
// Limit Switches
//
#define X_STOP_PIN                            35  // A7
#define Y_STOP_PIN                            34  // A6
#define Z_STOP_PIN                            33  // A5

//
// Steppers
//
#define X_STEP_PIN                             3  // D3
#define X_DIR_PIN                              5  // D5
#define X_ENABLE_PIN                           4  // D4
#define X_ATT_PIN                              2  // D2

#define Y_STEP_PIN                             7  // D7
#define Y_DIR_PIN                              9  // E1
#define Y_ENABLE_PIN                           8  // E0
#define Y_ATT_PIN                              6  // D6

#define Z_STEP_PIN                            11  // C1
#define Z_DIR_PIN                             13  // C3
#define Z_ENABLE_PIN                          12  // C2
#define Z_ATT_PIN                             10  // C0

#define E0_STEP_PIN                           15  // C5
#define E0_DIR_PIN                            17  // C7
#define E0_ENABLE_PIN                         16  // C6
#define E0_ATT_PIN                            14  // C4

//
// Temperature Sensors
//
#define TEMP_0_PIN                             7  // F7  Analog Input
#define TEMP_BED_PIN                           6  // F6  Analog Input

//
// Heaters / Fans
//
#define HEATER_0_PIN                          32  // A4 Extruder
#define HEATER_BED_PIN                        18  // E6 Bed

#ifndef FAN0_PIN
  #define FAN0_PIN                            31  // A3 Fan
#endif

//
// Misc. Functions
//
#define LED_PIN                               19  // E7
