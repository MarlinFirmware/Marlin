/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * NOVIN3DP Galaxy UI - MarlinUI hooks (replaces ui_color_ui.cpp)
 * Boot / kill screens, Marlin list menus, edit and confirm screens in galaxy style.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "../../../inc/MarlinConfigPre.h"

#if ALL(TFT_COLOR_UI, NOVIN3DP_GALAXY_UI)

#include "gx_core.h"
#include "../ui_common.h"
#include "../../marlinui.h"
#include "../../menu/menu.h"
#include "../../../libs/numtostr.h"
#include "../../../module/temperature.h"
#include "../../../sd/cardreader.h"
#if HAS_BEEPER
  #include "../../../libs/buzzer.h"
#endif

#define MENU_PANEL_X  8
#define MENU_PANEL_W  (TFT_WIDTH - 16)

void MarlinUI::tft_idle() {
  #if ENABLED(TOUCH_SCREEN)
    #if HAS_DISPLAY_SLEEP
      static bool was_sleeping = false;
      if (lcd_sleep_task()) { was_sleeping = true; return; }
      if (was_sleeping) { was_sleeping = false; gx_clear_screen(); gx_invalidate(); }  // Woke up: redraw everything
    #endif
    if (draw_menu_navigation) {
      // Bottom bar for Marlin list menus: page up / back / page down
      const bool up = encoderTopLine > 0, down = encoderTopLine + LCD_HEIGHT < screen_items;
      gx_canvas(0, GX_FOOTER_Y, TFT_WIDTH, TFT_HEIGHT - GX_FOOTER_Y);
      gx_bar(0, 0, TFT_WIDTH, 1, GX_C_LINE);
      const uint16_t bw = 140, by = 4, bh = 38;
      gx_panel(10, by, bw, bh);
      gx_icon(10 + (bw - 24) / 2, by + 7, GXI_CHEV_U, 24, up ? GX_C_ACC : GX_C_LINE);
      gx_panel(170, by, bw, bh, GX_C_ACC);
      gx_icon(170 + (bw - 24) / 2, by + 7, GXI_BACK, 24);
      gx_panel(330, by, bw, bh);
      gx_icon(330 + (bw - 24) / 2, by + 7, GXI_CHEV_D, 24, down ? GX_C_ACC : GX_C_LINE);
      if (up) touch.add_control(PAGE_UP, 10, GX_FOOTER_Y, bw, 44);
      touch.add_control(BACK, 170, GX_FOOTER_Y, bw, 44);
      if (down) touch.add_control(PAGE_DOWN, 330, GX_FOOTER_Y, bw, 44);
      draw_menu_navigation = false;
    }
  #endif

  gx_idle();

  // The buzzer shares the display ribbon: its current corrupts SPI transfers.
  // Hold back display drawing while any tone is sounding.
  #if HAS_BEEPER
    if (buzzer.is_active()) return;
  #endif

  tft.queue.async();

  // Touch driver is not DMA-aware and shares the SPI bus: only read it when the display is idle
  #if ENABLED(TOUCH_SCREEN)
    if (tft.queue.is_empty() && !tft.is_busy()) { delayMicroseconds(20); touch.idle(); }
  #endif
}

#if ENABLED(SHOW_BOOTSCREEN)

  void MarlinUI::show_bootscreen() {
    tft.queue.reset();
    gx_canvas(0, 0, TFT_WIDTH, TFT_HEIGHT);
    tft.add_image(140, 112, imgNOVINBIG, GX_C_ACC);
    gx_text_center(0, 150, TFT_WIDTH, "X5 v3 \xC2\xB7 Marlin " SHORT_BUILD_VERSION, GX_C_DIM);
    gx_panel(150, 196, 180, 6, GX_C_LINE, GXP_BLEND | GXP_BORDER, GX_C_PANEL, 3);
    gx_panel(152, 198, 110, 2, GX_C_ACC, GXP_SOLID, GX_C_ACC, 1);
    gx_text_center(0, 214, TFT_WIDTH, "Initializing...", GX_C_DIM);
    tft.queue.sync();
  }

  void MarlinUI::bootscreen_completion(const millis_t sofar) {
    if ((BOOTSCREEN_TIMEOUT) > sofar) safe_delay((BOOTSCREEN_TIMEOUT) - sofar);
    clear_lcd();
  }

#endif

void MarlinUI::draw_kill_screen() {
  tft.queue.reset();
  gx_canvas(0, 0, TFT_WIDTH, TFT_HEIGHT);
  gx_icon((TFT_WIDTH - 64) / 2, 40, GXI_FLAME, 64, GX_C_BAD);
  gx_text_center(0, 118, TFT_WIDTH, "PRINTER HALTED", GX_C_BAD, GXF_L);
  gx_text_center(10, 166, TFT_WIDTH - 20, status_message, GX_C_TEXT);
  tft_string.set(GET_TEXT(MSG_PLEASE_RESET)); tft_string.trim();
  tft.add_text(tft_string.center(TFT_WIDTH), 200, GX_C_DIM, tft_string);
  gx_bar(0, GX_FOOTER_Y, TFT_WIDTH, 1, GX_C_BAD);
  tft.queue.sync();
}

void MarlinUI::clear_lcd() { gx_clear_screen(); }

void MarlinUI::draw_status_screen() { gx_status_screen(); }

//
// Marlin list menus
//
static uint8_t menu_row;

void text_line(const uint16_t y, uint16_t) { gx_canvas(0, y, TFT_WIDTH, MENU_ITEM_HEIGHT); }

void menu_line(const uint8_t row, uint16_t color) {
  menu_row = row;
  text_line(MENU_TOP_LINE_Y + row * MENU_LINE_HEIGHT, color);
}

void menu_pause_option();

uint16_t menu_item(const uint8_t row, const bool sel) {
  #if ENABLED(TOUCH_SCREEN)
    if (row == 0) {
      touch.clear();
      draw_menu_navigation = TERN(ADVANCED_PAUSE_FEATURE, ui.currentScreen != menu_pause_option, true);
    }
  #endif
  menu_line(row);
  gx_panel(MENU_PANEL_X, 0, MENU_PANEL_W, MENU_ITEM_HEIGHT, sel ? GX_C_ACC : GX_C_LINE);
  #if ENABLED(TOUCH_SCREEN)
    const TouchControlType tct = TERN(SINGLE_TOUCH_NAVIGATION, true, sel) ? MENU_CLICK : MENU_ITEM;
    touch.add_control(tct, 0, MENU_TOP_LINE_Y + row * MENU_LINE_HEIGHT, TFT_WIDTH, MENU_ITEM_HEIGHT, encoderTopLine + row);
  #endif
  return GX_C_BG;
}

#define ITEM_TEXT_X 22

void MenuItemBase::_draw(const bool sel, const uint8_t row, FSTR_P const ftpl, const char, const char post_char) {
  menu_item(row, sel);
  const char *string = FTOP(ftpl);
  GxIcon icon = GX_NO_ICON;
  switch (*string) {
    case LCD_STR_REFRESH[0]: icon = GXI_REFRESH; break;
    case LCD_STR_FOLDER[0]:  icon = GXI_FOLDER;  break;
  }
  uint16_t x = ITEM_TEXT_X;
  if (icon != GX_NO_ICON) { string++; gx_icon(ITEM_TEXT_X, 8, icon, 24, icon == GXI_FOLDER ? GX_C_HOT : GX_C_ACC); x += 32; }
  tft_string.set(string, itemIndex, itemStringC, itemStringF);
  tft.add_text(x, MENU_TEXT_Y, GX_C_TEXT, tft_string);
  if (post_char == LCD_STR_ARROW_RIGHT[0]) gx_icon(TFT_WIDTH - 40, 10, GXI_CHEV_R, 20, GX_C_DIM);
}

void MenuEditItemBase::draw(const bool sel, const uint8_t row, FSTR_P const ftpl, const char * const inStr, const bool) {
  menu_item(row, sel);
  tft_string.set(ftpl, itemIndex, itemStringC, itemStringF);
  tft.add_text(ITEM_TEXT_X, MENU_TEXT_Y, GX_C_TEXT, tft_string);
  if (inStr) {
    tft_string.set(inStr);
    tft_string.trim();
    tft.add_text(TFT_WIDTH - ITEM_TEXT_X - tft_string.width(), MENU_TEXT_Y, GX_C_ACC, tft_string);
  }
}

void MenuItem_static::draw(const uint8_t row, FSTR_P const ftpl, const uint8_t style/*=SS_DEFAULT*/, const char *vstr/*=nullptr*/) {
  #if ENABLED(TOUCH_SCREEN)
    if (row == 0) {
      touch.clear();
      draw_menu_navigation = TERN(ADVANCED_PAUSE_FEATURE, ui.currentScreen != menu_pause_option, true);
    }
  #endif
  menu_line(row);
  if (ftpl) tft_string.set(ftpl, itemIndex, itemStringC, itemStringF); else tft_string.set();

  const bool center = bool(style & SS_CENTER), full = bool(style & SS_FULL);
  if (!full || !vstr) {
    if (vstr) tft_string.add(vstr);
    tft.add_text(center ? tft_string.center(TFT_WIDTH) : ITEM_TEXT_X, MENU_TEXT_Y, center ? GX_C_ACC : GX_C_TEXT, tft_string);
    return;
  }
  if (*vstr == ':') { tft_string.add(':'); vstr++; }
  tft.add_text(ITEM_TEXT_X, MENU_TEXT_Y, GX_C_DIM, tft_string);
  while (*vstr == ' ') vstr++;
  tft_string.set(vstr);
  tft.add_text(TFT_WIDTH - ITEM_TEXT_X - tft_string.width(), MENU_TEXT_Y, GX_C_TEXT, tft_string);
}

#if HAS_MEDIA
  void MenuItem_sdbase::draw(const bool sel, const uint8_t row, FSTR_P const, CardReader &theCard, const bool isDir) {
    menu_item(row, sel);
    if (isDir) gx_icon(ITEM_TEXT_X, 8, GXI_FOLDER, 24, GX_C_HOT);
    uint8_t maxlen = (MENU_ITEM_HEIGHT) - (MENU_TEXT_Y) + 1;
    tft.add_text(ITEM_TEXT_X + 32, MENU_TEXT_Y, GX_C_TEXT, ui.scrolled_filename(theCard, maxlen, sel));
  }
#endif

//
// Marlin edit screen (values edited from Marlin's own list menus)
//
void MenuEditItemBase::draw_edit_screen(FSTR_P const ftpl, const char * const value/*=nullptr*/) {
  ui.encoder_direction_normal();
  TERN_(TOUCH_SCREEN, touch.clear());

  // Title
  gx_canvas(0, 0, TFT_WIDTH, GX_HEADER_H);
  tft_string.set(ftpl, itemIndex, itemStringC, itemStringF);
  tft_string.trim();
  tft.add_text(20, 8 + tft_string.vcenter(36), GX_C_ACC, tft_string);
  gx_bar(20, 50, TFT_WIDTH - 30, 1, GX_C_ACC);

  // Value
  gx_canvas(90, 66, 300, 76);
  gx_panel(0, 0, 300, 76);
  if (value) {
    char v[24]; strlcpy(v, value, sizeof(v));
    char *s = v; while (*s == ' ') s++;
    gx_text_center(0, gx_vcenter(76, GXF_L), 300, s, GX_C_ACC, GXF_L);
  }

  // Slider
  if (ui.can_show_slider() && maxEditValue > 0) {
    constexpr uint16_t SX = 40, SY = 158, SW = TFT_WIDTH - 80;
    gx_canvas(SX, SY, SW, 28);
    const uint16_t pos = uint32_t(SW - 24) * ui.encoderPosition / maxEditValue;
    gx_panel(0, 10, SW, 8, GX_C_LINE, GXP_BLEND | GXP_BORDER, GX_C_PANEL, 4);
    if (pos > 2) gx_panel(1, 11, pos + 12, 6, GX_C_ACC, GXP_SOLID, GX_C_ACC, 3);
    gx_panel(pos, 2, 24, 24, GX_C_ACC, GXP_SOLID | GXP_BORDER, GX_C_ACC, 11);
    TERN_(TOUCH_SCREEN, touch.add_control(SLIDER, SX, SY - 8, SW, 44, maxEditValue));
  }

  tft.draw_edit_screen_buttons();
}

#if ENABLED(TFT_COLOR_TOUCH)
  void MenuEditItemBase::reset_edit_screen_state() {}  // Galaxy edit screen keeps no keypad state
#endif

void TFT::draw_edit_screen_buttons(const bool, const bool) {
  constexpr uint16_t BY = 212, BH = 56;
  gx_button(20, BY, 130, BH, GXI_MINUS, nullptr, 0);
  gx_button(175, BY, 130, BH, GXI_CHECK, "OK", 0, GXB_PRIMARY);
  gx_button(330, BY, 130, BH, GXI_PLUS, nullptr, 0);
  #if ENABLED(TOUCH_SCREEN)
    touch.add_control(DECREASE, 20, BY, 130, BH);
    touch.add_control(CLICK, 175, BY, 130, BH);
    touch.add_control(INCREASE, 330, BY, 130, BH);
  #endif
}

//
// Marlin confirm screen (two buttons)
//
void MenuItem_confirm::draw_select_screen(FSTR_P const yes, FSTR_P const no, const bool yesno, FSTR_P const fpre, const char * const string/*=nullptr*/, FSTR_P const fsuf/*=nullptr*/) {
  constexpr uint16_t DX = 40, DY = 36, DW = TFT_WIDTH - 80, DH = 248;
  TERN_(TOUCH_SCREEN, touch.clear());
  gx_canvas(DX, DY, DW, DH);
  gx_panel(0, 0, DW, DH, GX_C_ACC, GXP_SOLID | GXP_BORDER, GX_C_POP, 8);
  gx_icon((DW - 48) / 2, 14, GXI_WARNING, 48, GX_C_HOT);
  uint16_t ty = 76;
  if (fpre) { tft_string.set(fpre); tft_string.trim(); tft.add_text(tft_string.center(DW), ty, GX_C_TEXT, tft_string); ty += 28; }
  if (string) { tft_string.set(string); tft_string.trim(); tft.add_text(tft_string.center(DW), ty, GX_C_ACC, tft_string); ty += 28; }
  if (fsuf) { tft_string.set(fsuf); tft_string.trim(); tft.add_text(tft_string.center(DW), ty, GX_C_TEXT, tft_string); }

  const uint16_t bw = (DW - 50) / 2, by = DY + DH - 62;
  if (no) {
    gx_button(DX + 20, by, bw, 46, GXI_CLOSE, FTOP(no), 0, yesno ? GXB_NORMAL : GXB_SELECTED);
    TERN_(TOUCH_SCREEN, touch.add_control(CANCEL_ITEM, DX + 20, by, bw, 46));
  }
  if (yes) {
    gx_button(no ? DX + 30 + bw : DX + 20, by, no ? bw : DW - 40, 46, GXI_CHECK, FTOP(yes), 0, yesno ? GXB_SELECTED : GXB_PRIMARY);
    TERN_(TOUCH_SCREEN, touch.add_control(CONFIRM, no ? DX + 30 + bw : DX + 20, by, no ? bw : DW - 40, 46));
  }
}

#if ENABLED(ADVANCED_PAUSE_FEATURE)

  void MarlinUI::draw_hotend_status(const uint8_t row, const uint8_t extruder) {
    #if ENABLED(TOUCH_SCREEN)
      touch.clear();
      draw_menu_navigation = false;
      touch.add_control(RESUME_CONTINUE, 0, 0, TFT_WIDTH, TFT_HEIGHT);
    #endif
    menu_line(row);
    gx_icon(ITEM_TEXT_X, 8, GXI_NOZZLE, 24, GX_C_HOT);
    tft_string.set(GET_TEXT(MSG_FILAMENT_CHANGE_NOZZLE));
    tft_string.add('E');
    tft_string.add((char)('1' + extruder));
    tft_string.add(' ');
    tft_string.add(i16tostr3rj(thermalManager.wholeDegHotend(extruder)));
    tft_string.add(LCD_STR_DEGREE);
    tft_string.add(F(" / "));
    tft_string.add(i16tostr3rj(thermalManager.degTargetHotend(extruder)));
    tft_string.add(LCD_STR_DEGREE);
    tft_string.trim();
    tft.add_text(tft_string.center(TFT_WIDTH), MENU_TEXT_Y, GX_C_TEXT, tft_string);
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
      x = touch_calibration.calibration_points[_MIN(stage - 1, CALIBRATION_BOTTOM_LEFT)].x;
      y = touch_calibration.calibration_points[_MIN(stage - 1, CALIBRATION_BOTTOM_LEFT)].y;
      gx_canvas(x - 20, y - 20, 41, 41);
    }

    touch.clear();

    if (stage < CALIBRATION_SUCCESS) {
      switch (stage) {
        case CALIBRATION_TOP_LEFT:     tft_string.set(GET_TEXT(MSG_TOP_LEFT));     break;
        case CALIBRATION_TOP_RIGHT:    tft_string.set(GET_TEXT(MSG_TOP_RIGHT));    break;
        case CALIBRATION_BOTTOM_RIGHT: tft_string.set(GET_TEXT(MSG_BOTTOM_RIGHT)); break;
        case CALIBRATION_BOTTOM_LEFT:  tft_string.set(GET_TEXT(MSG_BOTTOM_LEFT));  break;
        default: break;
      }
      x = touch_calibration.calibration_points[stage].x;
      y = touch_calibration.calibration_points[stage].y;
      gx_canvas(x - 20, y - 20, 41, 41);
      gx_bar(0, 20, 41, 1, GX_C_ACC);
      gx_bar(20, 0, 1, 41, GX_C_ACC);
      gx_panel(12, 12, 17, 17, GX_C_ACC, GXP_BORDER, 0, 8);
      touch.add_control(CALIBRATE, 0, 0, TFT_WIDTH, TFT_HEIGHT, uint32_t(x) << 16 | uint32_t(y));
    }
    else {
      tft_string.set(stage == CALIBRATION_SUCCESS ? GET_TEXT(MSG_CALIBRATION_COMPLETED) : GET_TEXT(MSG_CALIBRATION_FAILED));
      defer_status_screen(false);
      touch_calibration.calibration_end();
      touch.add_control(BACK, 0, 0, TFT_WIDTH, TFT_HEIGHT);
    }

    gx_canvas(0, (TFT_HEIGHT - tft_string.font_height()) >> 1, TFT_WIDTH, tft_string.font_height());
    tft.add_text(tft_string.center(TFT_WIDTH), 0, GX_C_TEXT, tft_string);
  }

#endif // TOUCH_SCREEN_CALIBRATION

// Kept for code that still references the Color UI status widgets
void draw_heater_status(uint16_t, uint16_t, const int8_t) {}
void draw_fan_status(uint16_t, uint16_t, const bool) {}

#endif // TFT_COLOR_UI && NOVIN3DP_GALAXY_UI
