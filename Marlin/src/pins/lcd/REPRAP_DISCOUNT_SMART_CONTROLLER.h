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
 * REPRAP_DISCOUNT_SMART_CONTROLLER
 */

// LCD interface
#define LCD_PINS_RS                  EXP1_04_PIN
#define LCD_PINS_ENABLE              EXP1_03_PIN
#define LCD_PINS_D4                  EXP1_05_PIN
#define LCD_PINS_D5                  EXP1_06_PIN
#define LCD_PINS_D6                  EXP1_07_PIN
#define LCD_PINS_D7                  EXP1_08_PIN

// Beeper
#define BEEPER_PIN                   EXP1_01_PIN

// Encoder
#define BTN_ENC                      EXP1_02_PIN
#define BTN_EN1                      EXP2_03_PIN
#define BTN_EN2                      EXP2_05_PIN

// SD Card
#ifndef SD_DETECT_PIN
  #define SD_DETECT_PIN              EXP2_07_PIN
#endif

// Kill
#ifndef KILL_PIN
  #define KILL_PIN                   EXP2_08_PIN
#endif
