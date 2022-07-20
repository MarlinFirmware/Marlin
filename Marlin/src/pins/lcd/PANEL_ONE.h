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
 * PANEL_ONE <http://blog.think3dprint3d.com/2014/08/panelone-lcd-screen-for-33v-and-5v.html>
 *
 *         ------                 ------
 *    NC  | 1  2 | NC        DB4 | 1  2 | DB5
 *    VCC | 3  4 | ENC         E | 3  4 | DB6
 *   MISO | 5  6 | MOSI       RS | 5  6 | DB7
 *    SCK | 7  8 | SDSS      EN2 | 7  8 | EN1
 *    GND | 9 10 | NC        GND | 9 10 | 5V
 *         ------                 ------
 *           P1                     P2
 */

// LCD interface
#define LCD_PINS_RS                  EXP2_05_PIN
#define LCD_PINS_ENABLE              EXP2_03_PIN
#define LCD_PINS_D4                  EXP2_01_PIN
#define LCD_PINS_D5                  EXP2_02_PIN
#define LCD_PINS_D6                  EXP2_04_PIN
#define LCD_PINS_D7                  EXP2_06_PIN

// Encoder
#define BTN_ENC                      EXP1_04_PIN
#define BTN_EN1                      EXP2_08_PIN
#define BTN_EN2                      EXP2_07_PIN
