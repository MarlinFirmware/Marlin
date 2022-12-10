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
 * Melzi pin assignments
 * ATmega644P, ATmega1284P
 */

#define BOARD_INFO_NAME "Melzi (Tronxy)"

#define Z_ENABLE_PIN                          PinD6

#define LCD_SDSS                              -1

#if ENABLED(CR10_STOCKDISPLAY)
  #define LCD_PINS_RS                         PinA1
  #define LCD_PINS_ENABLE                     PinA3
  #define LCD_PINS_D4                         PinC0
  #define LCD_PINS_D5                         PinC1
  #define LCD_PINS_D6                         PinA4
  #define LCD_PINS_D7                         PinA2
  #define BTN_EN1                             PinD2
  #define BTN_EN2                             PinD3
  #define BTN_ENC                             PinA5

  #define LCD_PINS_DEFINED
#endif

// Alter timing for graphical display
#if IS_U8GLIB_ST7920
  #define BOARD_ST7920_DELAY_1                 0
  #define BOARD_ST7920_DELAY_2               125
  #define BOARD_ST7920_DELAY_3                 0
#endif

#include "pins_MELZI.h" // ... SANGUINOLOLU_12 ... SANGUINOLOLU_11
