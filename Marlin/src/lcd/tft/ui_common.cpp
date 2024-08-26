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

#if HAS_GRAPHICAL_TFT

#include "ui_common.h"
#include "../lcdprint.h"
#include "../../libs/numtostr.h"
#include "../menu/menu.h"

void menu_pause_option();

static xy_uint_t cursor;

#if ENABLED(TOUCH_SCREEN)
  bool draw_menu_navigation = false;
#endif

#if HAS_TOUCH_SLEEP

  bool lcd_sleep_task() {
    static bool sleepCleared;
    if (touch.isSleeping()) {
      tft.queue.reset();
      if (!sleepCleared) {
        sleepCleared = true;
        ui.clear_lcd();
        tft.queue.async();
      }
      touch.idle();
      return true;
    }
    else
      sleepCleared = false;
    return false;
  }

#endif

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

//
// lcdprint.h functions
//

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

int lcd_put_lchar_max(const lchar_t &c, const pixel_len_t max_length) {
  if (max_length < 1) return 0;
  tft_string.set(c);
  tft.add_text(MENU_TEXT_X_OFFSET, MENU_TEXT_Y_OFFSET, COLOR_MENU_TEXT, tft_string);
  lcd_gotopixel((cursor.x + 1) * (TFT_COL_WIDTH) + tft_string.width(), cursor.y * MENU_LINE_HEIGHT);
  return tft_string.width();
}

int lcd_put_u8str_max_P(PGM_P utf8_pstr, const pixel_len_t max_length) {
  if (max_length < 1) return 0;
  tft_string.set(utf8_pstr);
  tft_string.trim();
  tft_string.truncate(max_length);
  tft.add_text(MENU_TEXT_X_OFFSET, MENU_TEXT_Y_OFFSET, COLOR_MENU_TEXT, tft_string);
  lcd_gotopixel((cursor.x + 1) * (TFT_COL_WIDTH) + tft_string.width(), cursor.y * MENU_LINE_HEIGHT);
  return tft_string.width();
}

int lcd_put_u8str_max(const char * utf8_str, const pixel_len_t max_length) {
  return lcd_put_u8str_max_P(utf8_str, max_length);
}

void lcd_put_int(const int i) {
  // 3 digits max for this one...
  const char* str = i16tostr3left(int16_t(i));
  lcd_put_u8str_max(str, 3);
}

//
// Menu Item methods
//

// Draw a generic menu item with pre_char (if selected) and post_char
void MenuItemBase::_draw(const bool sel, const uint8_t row, FSTR_P const fstr, const char pre_char, const char post_char) {
  menu_item(row, sel);

  const char *string = FTOP(fstr);
  MarlinImage image = noImage;
  switch (*string) {
    case 0x01: image = imgRefresh; break;  // LCD_STR_REFRESH
    case 0x02: image = imgDirectory; break;  // LCD_STR_FOLDER
  }

  uint8_t offset = MENU_TEXT_X_OFFSET;
  if (image != noImage) {
    string++;
    offset = MENU_ITEM_ICON_SPACE;
    tft.add_image(MENU_ITEM_ICON_X, MENU_ITEM_ICON_Y, image, COLOR_MENU_TEXT, sel ? COLOR_SELECTION_BG : COLOR_BACKGROUND);
  }

  tft_string.set(string, itemIndex, itemStringC, itemStringF);

  tft.add_text(offset, MENU_TEXT_Y_OFFSET, COLOR_MENU_TEXT, tft_string);
}

// Draw a menu item with a (potentially) editable value
void MenuEditItemBase::draw(const bool sel, const uint8_t row, FSTR_P const fstr, const char * const inStr, const bool pgm) {
  menu_item(row, sel);

  tft_string.set(fstr, itemIndex, itemStringC, itemStringF);
  tft.add_text(MENU_TEXT_X_OFFSET, MENU_TEXT_Y_OFFSET, COLOR_MENU_TEXT, tft_string);
  if (inStr) {
    tft_string.set(inStr);
    tft.add_text(TFT_WIDTH - MENU_TEXT_X_OFFSET - tft_string.width(), MENU_TEXT_Y_OFFSET, COLOR_MENU_VALUE, tft_string);
  }
}

// Draw a static item with no left-right margin required. Centered by default.
void MenuItem_static::draw(const uint8_t row, FSTR_P const fstr, const uint8_t style/*=SS_DEFAULT*/, const char * const vstr/*=nullptr*/) {
  menu_item(row);
  tft_string.set(fstr, itemIndex, itemStringC, itemStringF);
  if (vstr) tft_string.add(vstr);
  tft.add_text(tft_string.center(TFT_WIDTH), MENU_TEXT_Y_OFFSET, COLOR_YELLOW, tft_string);
}

#if HAS_MEDIA

  void MenuItem_sdbase::draw(const bool sel, const uint8_t row, FSTR_P const, CardReader &theCard, const bool isDir) {
    menu_item(row, sel);
    if (isDir) tft.add_image(MENU_ITEM_ICON_X, MENU_ITEM_ICON_Y, imgDirectory, COLOR_MENU_TEXT, sel ? COLOR_SELECTION_BG : COLOR_BACKGROUND);
    constexpr uint8_t maxlen = (MENU_ITEM_HEIGHT) - (MENU_TEXT_Y_OFFSET) + 1;
    tft.add_text(MENU_ITEM_ICON_SPACE, MENU_TEXT_Y_OFFSET, COLOR_MENU_TEXT, ui.scrolled_filename(theCard, maxlen, row, sel));
  }

#endif

//
// MarlinUI methods
//

bool MarlinUI::detected() { return true; }

void MarlinUI::init_lcd() {
  tft.init();
  tft.set_font(MENU_FONT_NAME);
  #ifdef SYMBOLS_FONT_NAME
    tft.add_glyphs(SYMBOLS_FONT_NAME);
  #endif
  TERN_(TOUCH_SCREEN, touch.init());
  clear_lcd();
}

void MarlinUI::clear_lcd() {
  #if ENABLED(TOUCH_SCREEN)
    touch.reset();
    draw_menu_navigation = false;
  #endif

  tft.queue.reset();
  tft.fill(0, 0, TFT_WIDTH, TFT_HEIGHT, COLOR_BACKGROUND);
  cursor.set(0, 0);
}

#if HAS_LCD_BRIGHTNESS

  void MarlinUI::_set_brightness() {
    #if PIN_EXISTS(TFT_BACKLIGHT)
      if (PWM_PIN(TFT_BACKLIGHT_PIN))
        analogWrite(pin_t(TFT_BACKLIGHT_PIN), backlight ? brightness : 0);
    #endif
  }

#endif

#if ENABLED(TOUCH_SCREEN_CALIBRATION)

  void MarlinUI::touch_calibration_screen() {
    uint16_t x, y;

    calibrationState stage = touch_calibration.get_calibration_state();

    if (stage == CALIBRATION_NONE) {
      defer_status_screen(true);
      clear_lcd();
      stage = touch_calibration.calibration_start();
    }
    else {
      x = touch_calibration.calibration_points[_MIN(stage - 1, CALIBRATION_BOTTOM_RIGHT)].x;
      y = touch_calibration.calibration_points[_MIN(stage - 1, CALIBRATION_BOTTOM_RIGHT)].y;
      tft.canvas(x - 15, y - 15, 31, 31);
      tft.set_background(COLOR_BACKGROUND);
    }

    touch.clear();

    if (stage < CALIBRATION_SUCCESS) {
      switch (stage) {
        case CALIBRATION_TOP_LEFT: tft_string.set(GET_TEXT(MSG_TOP_LEFT)); break;
        case CALIBRATION_BOTTOM_LEFT: tft_string.set(GET_TEXT(MSG_BOTTOM_LEFT)); break;
        case CALIBRATION_TOP_RIGHT: tft_string.set(GET_TEXT(MSG_TOP_RIGHT)); break;
        case CALIBRATION_BOTTOM_RIGHT: tft_string.set(GET_TEXT(MSG_BOTTOM_RIGHT)); break;
        default: break;
      }

      x = touch_calibration.calibration_points[stage].x;
      y = touch_calibration.calibration_points[stage].y;

      tft.canvas(x - 15, y - 15, 31, 31);
      tft.set_background(COLOR_BACKGROUND);
      tft.add_bar(0, 15, 31, 1, COLOR_TOUCH_CALIBRATION);
      tft.add_bar(15, 0, 1, 31, COLOR_TOUCH_CALIBRATION);

      touch.add_control(CALIBRATE, 0, 0, TFT_WIDTH, TFT_HEIGHT, uint32_t(x) << 16 | uint32_t(y));
    }
    else {
      tft_string.set(stage == CALIBRATION_SUCCESS ? GET_TEXT(MSG_CALIBRATION_COMPLETED) : GET_TEXT(MSG_CALIBRATION_FAILED));
      defer_status_screen(false);
      touch_calibration.calibration_end();
      touch.add_control(BACK, 0, 0, TFT_WIDTH, TFT_HEIGHT);
    }

    tft.canvas(0, (TFT_HEIGHT - tft_string.font_height()) >> 1, TFT_WIDTH, tft_string.font_height());
    tft.set_background(COLOR_BACKGROUND);
    tft.add_text(tft_string.center(TFT_WIDTH), 0, COLOR_MENU_TEXT, tft_string);
  }

#endif // TOUCH_SCREEN_CALIBRATION

#endif // HAS_GRAPHICAL_TFT
