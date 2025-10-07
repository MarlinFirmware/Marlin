/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * pins/lcd/WYH_L12864.h
 */

// DOGLCD
//   HAS_MARLINUI_U8GLIB > (128x64)
// IS_ULTIPANEL
//   HAS_WIRED_LCD
//     HAS_DISPLAY
//       HAS_STATUS_MESSAGE
//   IS_NEWPANEL
//   HAS_MARLINUI_MENU
//     HAS_MANUAL_MOVE_MENU
// IS_DOGM_12864
// IS_U8GLIB_ST7565_64128N
// ST7565_XOFFSET 0x04

/**
 * 1. Cut the tab off the LCD connector so it can be plugged into the "EXP1" connector the other way.
 * 2. Swap the LCD's +5V (Pin2) and GND (Pin1) wires. (This is the critical part!)
 * 3. On boards with open drain Pin 9 swap the LCD's MOSI (Pin9) and empty (Pin10) wires.
 *
 * !!! If you are unsure, ask for help! Your motherboard may be damaged in some circumstances !!!
 *
 * The WYH_L12864 connector plug:
 *
 *                  BEFORE                     AFTER
 *                  ______                     ______
 *                 | 1  2 | (MOSI)     (MOSI) |10  9 | --
 *       (BTN_ENC) | 3  4 | (SCK)   (BTN_ENC) | 8  7 | (SCK)
 *       (BTN_EN1)   5  6 | (SID)   (BTN_EN1)   6  5 | (SID)
 *       (BTN_EN2) | 7  8 | (CS)    (BTN_EN2) | 4  3 | (CS)
 *              5V | 9 10 | GND           GND | 2  1 | 5V
 *                  ------                     ------
 *                   LCD                        LCD
 */
#define BEEPER_PIN               -1

#define DOGLCD_CS         LCD1_08_PIN
#define DOGLCD_A0         LCD1_06_PIN
#define DOGLCD_SCK        LCD1_04_PIN
#ifndef DOGLCD_MOSI
  #define DOGLCD_MOSI     LCD1_02_PIN
#endif

#if IS_NEWPANEL
  #define BTN_ENC         LCD1_03_PIN
  #define BTN_EN1         LCD1_05_PIN
  #define BTN_EN2         LCD1_07_PIN
#endif

#define LCD_BACKLIGHT_PIN        -1

#define _LCD_CONTRAST_INIT        190
