/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * pins/lcd/ZONESTAR_12864LCD.h - Zonestar 12864 ST7920 character/graphic LCD
 *
 * A DOGLCD (ST7920) with IS_RRD_SC button wiring. Connected over EXP1 on the
 * Zonestar ZM3E2 / ZM3E4 boards, which share this mapping:
 *
 *   LCD_PINS_RS = EXP1_03   // ST7920 CS
 *   LCD_PINS_EN = EXP1_06   // ST7920 DAT
 *   LCD_PINS_D4 = EXP1_04   // ST7920 CLK
 *   BEEPER      = EXP1_02
 *   BTN_ENC     = EXP1_07
 *
 * BTN_EN1 / BTN_EN2 swap between EXP1_08 and EXP1_05 across the ZM3E2 vs
 * ZM3E4 boards, so those (and KILL) are left to the board pins file via the
 * #ifndef guards below.
 *
 * Boards with unique or non-standard wiring (e.g. ZMIB_V2 with FORCE_SOFT_SPI,
 * ZRIB_V53 with raw pins) pre-define the affected pins and are unaffected.
 */

// Output / control pins
#ifndef LCD_PINS_RS
  #define LCD_PINS_RS       EXP1_03_PIN  // ST7920 CS (LCD-4)
#endif
#ifndef LCD_PINS_EN
  #define LCD_PINS_EN       EXP1_06_PIN  // ST7920 DAT (LCD-5)
#endif
#ifndef LCD_PINS_D4
  #define LCD_PINS_D4       EXP1_04_PIN  // ST7920 CLK (LCD-6)
#endif

// Beeper and encoder
#ifndef BEEPER_PIN
  #define BEEPER_PIN        EXP1_02_PIN
#endif
#ifndef BTN_ENC
  #define BTN_ENC           EXP1_07_PIN
#endif
