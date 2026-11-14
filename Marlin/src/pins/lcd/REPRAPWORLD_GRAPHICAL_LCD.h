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
 * pins/lcd/REPRAPWORLD_GRAPHICAL_LCD.h
 */

/**
 * https://reprapworld.com/electronics/autonomous-printing/smart-controller-graphical-lcd-12864-lcd/
 *
 *  RRW GLCD Headers      UT 2  4  6    PWR  -  +
 *                           1  3  5
 *
 * AUX1 1  2    MTICSP 2  4  6  8 10  SDOUT  2  4  6  8
 *      3  4           1  3  5  7  9         1  3  5  7
 *      5  6
 *      7  8
 *             KEYPAD  2  4  6  8 10    SPI  2  4  6  8
 *                     1  3  5  7  9         1  3  5  7
 */

// DOGLCD IS_U8GLIB_ST7920 IS_ULTIPANEL HAS_WIRED_LCD IS_NEWPANEL HAS_MARLINUI_MENU

/**
 * The RepRapWorld Graphical LCD is a serial ST7920 panel (like the RRD Full Graphic
 * Smart Controller) with an encoder and SD card. Its loose headers have no EXP1/EXP2
 * sockets, so it always connects with a custom cable. Boards with an established wiring
 * (RAMPS AUX2/AUX3, Megatronics, Minitronics, Ultratronics...) define its pins inline.
 *
 * Otherwise wire it like a Full Graphic Smart Controller. Boards with only one 10-pin
 * header use the CR10_STOCKDISPLAY layout instead.
 *
 *  Panel signal          EXP1 + EXP2      Single EXP1
 *  ------------------    -----------      -----------
 *  LCD CS   (RS)         EXP1_04          EXP1_07
 *  LCD SID  (EN)         EXP1_03          EXP1_08
 *  LCD CLK  (D4)         EXP1_05          EXP1_06
 *  Encoder push (ENC)    EXP1_02          EXP1_02
 *  Encoder A    (EN1)    EXP2_03          EXP1_03
 *  Encoder B    (EN2)    EXP2_05          EXP1_05
 *  SD MISO/SCK           EXP2_01/02       (no SD)
 *  SD SS/MOSI/DETECT     EXP2_04/06/07
 *  5V / GND              EXP1_10/09       EXP1_10/09
 */
#if _EXISTS(LCD2_03_PIN) && _EXISTS(LCD2_05_PIN)
  #ifndef LCD_PINS_RS
    #define LCD_PINS_RS     LCD1_04_PIN // CS
  #endif
  #ifndef LCD_PINS_EN
    #define LCD_PINS_EN     LCD1_03_PIN // SID
  #endif
  #ifndef LCD_PINS_D4
    #define LCD_PINS_D4     LCD1_05_PIN // CLK
  #endif
  #ifndef BTN_EN1
    #define BTN_EN1         LCD2_03_PIN
  #endif
  #ifndef BTN_EN2
    #define BTN_EN2         LCD2_05_PIN
  #endif
#else
  #ifndef LCD_PINS_RS
    #define LCD_PINS_RS     LCD1_07_PIN // CS
  #endif
  #ifndef LCD_PINS_EN
    #define LCD_PINS_EN     LCD1_08_PIN // SID
  #endif
  #ifndef LCD_PINS_D4
    #define LCD_PINS_D4     LCD1_06_PIN // CLK
  #endif
  #ifndef BTN_EN1
    #define BTN_EN1         LCD1_03_PIN
  #endif
  #ifndef BTN_EN2
    #define BTN_EN2         LCD1_05_PIN
  #endif
#endif
#ifndef BTN_ENC
  #define BTN_ENC           LCD1_02_PIN
#endif

#if !(_EXISTS(LCD_PINS_EN) && _EXISTS(LCD_PINS_RS) && _EXISTS(LCD_PINS_D4))
  #error "REPRAPWORLD_GRAPHICAL_LCD needs an EXP1 header (or inline pins) on your MOTHERBOARD. See pins/lcd/REPRAPWORLD_GRAPHICAL_LCD.h."
#endif
