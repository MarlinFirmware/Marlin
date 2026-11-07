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
 * LiquidCrystal_WS0010
 * Winstar WS0010 character OLED on direct-connected HD44780 pins
 * Used by REPRAP_DISCOUNT_SMART_CONTROLLER_OLED (Winstar WEH002004 panel)
 */

#include "../../inc/MarlinConfig.h"
#include "../../HAL/shared/Delay.h"
#include <LiquidCrystal.h>

#if DISPLAY_CHARSET_HD44780 == WESTERN
  #define WS0010_FONT_TABLE 0x01  // Western European I
#elif DISPLAY_CHARSET_HD44780 == CYRILLIC
  #define WS0010_FONT_TABLE 0x02  // English/Russian
#else
  #define WS0010_FONT_TABLE 0x00  // English/Japanese
#endif

class LiquidCrystal_WS0010 : public LiquidCrystal {
public:
  using LiquidCrystal::LiquidCrystal;

  // WS0010 keeps its nibble state through MCU reset, so resync after HD44780 init
  void begin(uint8_t cols, uint8_t rows, uint8_t charsize=LCD_5x8DOTS) {
    LiquidCrystal::begin(cols, rows, charsize);
    WRITE(LCD_PINS_RS, LOW);
    nibble(0x03);         // Back to 8-bit mode
    nibble(0x08);
    nibble(0x02);         // 4-bit mode
    // Function Set (and font table) is only accepted right after interface length change
    const uint8_t fs = LCD_FUNCTIONSET | LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS | WS0010_FONT_TABLE;
    nibble(fs >> 4);
    nibble(fs & 0x0F);
    nibble(0x01);         // Character mode, internal power on
    nibble(0x07);
    noDisplay();
    clear();
    leftToRight();
    home();
    display();
  }

  // Clear Display takes up to 6.2ms. LiquidCrystal waits 2ms.
  void clear() { LiquidCrystal::clear(); DELAY_US(4200); }

private:
  static void nibble(const uint8_t n) {
    WRITE(LCD_PINS_D4, TEST(n, 0));
    WRITE(LCD_PINS_D5, TEST(n, 1));
    WRITE(LCD_PINS_D6, TEST(n, 2));
    WRITE(LCD_PINS_D7, TEST(n, 3));
    WRITE(LCD_PINS_EN, HIGH);
    DELAY_US(1);
    WRITE(LCD_PINS_EN, LOW);
    safe_delay(5);
  }
};
