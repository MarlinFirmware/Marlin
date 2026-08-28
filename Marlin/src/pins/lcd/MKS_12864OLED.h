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
 * pins/lcd/MKS_12864OLED.h
 * MKS 128x64 OLED display via software SPI using EXP1 connector.
 * Covers both MKS_12864OLED (SH1106) and MKS_12864OLED_SSD1306 (SSD1306).
 * https://reprap.org/wiki/MKS_12864OLED
 *
 * IS_RRD_SC
 *   IS_ULTIPANEL
 *     HAS_WIRED_LCD
 *       HAS_DISPLAY
 *         HAS_STATUS_MESSAGE
 *     IS_NEWPANEL
 *     HAS_MARLINUI_MENU
 *       HAS_MANUAL_MOVE_MENU
 *
 * EXP1 connector pinout:
 *
 *        1     3     5     7     9
 * LCD1:  BEEP  MOSI  SCK   RS    GND
 *        ENC   CS    --    DC    VCC
 *        2     4     6     8     10
 *
 * BTN_ENC, BTN_EN1, BTN_EN2 are defined in the board's HAS_WIRED_LCD scope.
 * LCD_PINS_RS is used as LCD_RESET_PIN (see pins_lcd.h).
 */

#define LCD_PINS_DC               EXP1_06_PIN  // Set as output on init
#define LCD_PINS_RS               EXP1_07_PIN  // Pull low for 1s to init (reset)

// DOGM SPI LCD Support
#define DOGLCD_CS                 EXP1_04_PIN
#define DOGLCD_MOSI               EXP1_03_PIN
#define DOGLCD_SCK                EXP1_05_PIN
#define DOGLCD_A0                 LCD_PINS_DC
