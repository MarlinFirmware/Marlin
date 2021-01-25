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

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(TFT_COLOR_UI)

#include "ui_common.h"

void menu_pause_option();

void menu_line(const uint8_t row, uint16_t color) {
  cursor.set(0, row);
  tft.canvas(0, TFT_TOP_LINE_Y + cursor.y * MENU_LINE_HEIGHT, TFT_WIDTH, MENU_ITEM_HEIGHT);
  tft.set_background(color);
}

void menu_item(const uint8_t row, bool sel ) {
  #if ENABLED(TOUCH_SCREEN)
    if (row == 0) {
      touch.clear();
      draw_menu_navigation = TERN(ADVANCED_PAUSE_FEATURE, ui.currentScreen != menu_pause_option, true);
    }
  #endif

  menu_line(row, sel ? COLOR_SELECTION_BG : COLOR_BACKGROUND);
  #if ENABLED(TOUCH_SCREEN)
    const TouchControlType tct = TERN(SINGLE_TOUCH_NAVIGATION, true, sel) ? MENU_CLICK : MENU_ITEM;
    touch.add_control(tct, 0, TFT_TOP_LINE_Y + row * MENU_LINE_HEIGHT, TFT_WIDTH, MENU_ITEM_HEIGHT, encoderTopLine + row);
  #endif
}

#define TFT_COL_WIDTH ((TFT_WIDTH) / (LCD_WIDTH))

void lcd_gotopixel(const uint16_t x, const uint16_t y) {
  if (x >= TFT_WIDTH) return;
  cursor.set(x / (TFT_COL_WIDTH), y / MENU_LINE_HEIGHT);
  tft.canvas(x, TFT_TOP_LINE_Y + y, (TFT_WIDTH) - x, MENU_ITEM_HEIGHT);
  tft.set_background(COLOR_BACKGROUND);
}

void lcd_moveto(const lcd_uint_t col, const lcd_uint_t row) {
  lcd_gotopixel(int(col) * (TFT_COL_WIDTH), int(row) * MENU_LINE_HEIGHT);
}

int lcd_put_wchar_max(wchar_t c, pixel_len_t max_length) {
  if (max_length < 1) return 0;
  tft_string.set();
  tft_string.add(c);
  tft.add_text(MENU_TEXT_X_OFFSET, MENU_TEXT_Y_OFFSET, COLOR_MENU_TEXT, tft_string);
  lcd_gotopixel((cursor.x + 1) * (TFT_COL_WIDTH) + tft_string.width(), cursor.y * MENU_LINE_HEIGHT);
  return tft_string.width();
}

int lcd_put_u8str_max_P(PGM_P utf8_str_P, pixel_len_t max_length) {
  if (max_length < 1) return 0;
  tft_string.set(utf8_str_P);
  tft_string.trim();
  tft_string.truncate(max_length);
  tft.add_text(MENU_TEXT_X_OFFSET, MENU_TEXT_Y_OFFSET, COLOR_MENU_TEXT, tft_string);
  lcd_gotopixel((cursor.x + 1) * (TFT_COL_WIDTH) + tft_string.width(), cursor.y * MENU_LINE_HEIGHT);
  return tft_string.width();
}

int lcd_put_u8str_max(const char * utf8_str, pixel_len_t max_length) {
  return lcd_put_u8str_max_P(utf8_str, max_length);
}

void lcd_put_int(const int i) {
  // 3 digits max for this one...
  const char* str = i16tostr3left(int16_t(i));
  lcd_put_u8str_max(str, 3);
}

#endif // TFT_COLOR_UI
