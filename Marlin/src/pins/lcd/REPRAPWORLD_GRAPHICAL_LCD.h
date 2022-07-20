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
 * REPRAPWORLD_GRAPHICAL_LCD
 */

// LCD interface
#define LCD_PINS_RS                  EXP2_07_PIN  // CS chip select /SS chip slave select
#define LCD_PINS_ENABLE              EXP2_06_PIN  // SID (MOSI)
#define LCD_PINS_D4                  EXP2_02_PIN  // SCK (CLK) clock

// Encoder
#define BTN_EN1                      AUX2_05_PIN  // TODO: RRW GLCD Adapter
#define BTN_EN2                      AUX2_03_PIN
#define BTN_ENC                      AUX2_04_PIN

// SD Card
#ifndef SD_DETECT_PIN
  #define SD_DETECT_PIN              AUX2_08_PIN
#endif
