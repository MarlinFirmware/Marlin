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
 * ELB_FULL_GRAPHIC_CONTROLLER
 *
 *         ------                 ------
 *    EN2 | 1  2 | EN1       --- | 1  2 | ---
 *    --- | 3  4 | ---       ENC | 3  4 | ---
 *   BEEP | 5  6 | ---        BL | 5  6 | ---
 *     A0 | 7  8 | CS        SDD | 7  8 | KILL
 *    --- | 9 10 | ---       --- | 9 10 | ---
 *         ------                 ------
 *          EXP1                   EXP2
 */

// LCD interface
#define DOGLCD_CS                    EXP1_08_PIN
#define DOGLCD_A0                    EXP1_07_PIN

// Beeper
#define BEEPER_PIN                   EXP1_05_PIN

// Encoder
#define BTN_EN1                      EXP1_02_PIN
#define BTN_EN2                      EXP1_01_PIN
#define BTN_ENC                      EXP2_03_PIN

// SD Card
#define LCD_SDSS                            SDSS

#ifndef SD_DETECT_PIN
  #define SD_DETECT_PIN              EXP2_07_PIN
#endif

// Extras
#define LCD_BACKLIGHT_PIN            EXP2_05_PIN
#define KILL_PIN                     EXP2_08_PIN
