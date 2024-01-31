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
 * Sanguinololu V1.2 pin assignments
 * Schematic (1.2): https://green-candy.osdn.jp/external/MarlinFW/board_schematics/Sanguinololu%20v1.2/schematic.png
 * Origin (1.2): https://github.com/mosfet/Sanguinololu/blob/master/rev1.2/sanguinololu.sch
 * Schematic (1.3): https://green-candy.osdn.jp/external/MarlinFW/board_schematics/Sanguinololu%20v1.3/schematic.png
 * Origin (1.3): https://github.com/mosfet/Sanguinololu/blob/master/rev1.3/sanguinololu.sch
 * Schematic (1.3a): https://green-candy.osdn.jp/external/MarlinFW/board_schematics/Sanguinololu%20v1.3a/schematic.png
 * Origin (1.3a): https://github.com/mosfet/Sanguinololu/blob/master/rev1.3a/sanguinololu.sch
 *
 * Applies to the following boards:
 *
 *  AZTEEG_X1
 *  MELZI
 *  MELZI_CREALITY
 *  MELZI_MAKR3D
 *  SANGUINOLOLU_12
 *  STB_11
 */

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "Sanguinololu 1.2"
#endif

#define HEATER_BED_PIN                        12  // (bed)
#define X_ENABLE_PIN                          14
#define Y_ENABLE_PIN                          14
#ifndef Z_ENABLE_PIN
  #define Z_ENABLE_PIN                        26
#endif
#define E0_ENABLE_PIN                         14

#if !defined(FAN0_PIN) && ENABLED(LCD_I2C_PANELOLU2)
  #define FAN0_PIN                             4  // Uses Transistor1 (PWM) on Panelolu2's Sanguino Adapter Board to drive the fan
#endif

#define SANGUINOLOLU_V_1_2
#include "pins_SANGUINOLOLU_11.h"
