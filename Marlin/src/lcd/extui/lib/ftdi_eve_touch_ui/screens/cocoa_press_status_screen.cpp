/*********************************
 * cocoa_press_status_screen.cpp *
 *********************************/

/****************************************************************************
 *   Written By Mark Pelletier  2017 - Aleph Objects, Inc.                  *
 *   Written By Marcio Teixeira 2018 - Aleph Objects, Inc.                  *
 *   Written By Marcio Teixeira 2019 - Cocoa Press                          *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   To view a copy of the GNU General Public License, go to the following  *
 *   location: <https://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

#include "../config.h"
#include "screens.h"

#ifdef FTDI_COCOA_STATUS_SCREEN

#include "../ftdi_eve_lib/extras/poly_ui.h"

#include "cocoa_press_ui.h"

#define POLY(A) PolyUI::poly_reader_t(A, sizeof(A)/sizeof(A[0]))

const uint8_t shadow_depth = 5;

using namespace FTDI;
using namespace Theme;
using namespace ExtUI;

float StatusScreen::increment;

void StatusScreen::loadBitmaps() {
  constexpr uint32_t base = ftdi_memory_map::RAM_G;

  // Load fonts for internationalization
  #if ENABLED(TOUCH_UI_USE_UTF8)
    load_utf8_data(base + UTF8_FONT_OFFSET);
  #endif
}

void StatusScreen::draw_progress(draw_mode_t what) {
  CommandProcessor cmd;
  PolyUI ui(cmd, what);

  int16_t x, y, h, v;

  cmd.cmd(COLOR_RGB(accent_color_1));
  cmd.font(font_medium);

  if (what & BACKGROUND) {
    ui.bounds(POLY(print_time_label), x, y, h, v);
    cmd.text(x, y, h, v, GET_TEXT_F(MSG_ELAPSED_PRINT));
  }

  if (what & FOREGROUND) {
    const uint32_t elapsed = getProgress_seconds_elapsed();
    const uint8_t hrs = elapsed/3600;
    const uint8_t min = (elapsed/60)%60;

    char str[10];
    sprintf_P(str, PSTR(" %02d : %02d"), hrs, min);
    ui.bounds(POLY(print_time_hms), x, y, h, v);
    cmd.text(x, y, h, v, str);

    sprintf_P(str, PSTR("%-3d%%"), getProgress_percent() );
    ui.bounds(POLY(print_time_percent), x, y, h, v);
    cmd.text(x, y, h, v, str);
  }
}

void StatusScreen::draw_temperature(draw_mode_t what) {
  CommandProcessor cmd;
  PolyUI ui(cmd, what);

  int16_t x, y, h, v;

  if (what & BACKGROUND) {
    cmd.cmd(COLOR_RGB(fluid_rgb));
    cmd.font(font_medium).tag(10);

    ui.bounds(POLY(chocolate_label), x, y, h, v);
    cmd.text(x, y, h, v, GET_TEXT_F(MSG_CHOCOLATE));

    ui.bounds(POLY(h0_label), x, y, h, v);
    cmd.text(x, y, h, v, GET_TEXT_F(MSG_NOZZLE));

    ui.bounds(POLY(h1_label), x, y, h, v);
    cmd.text(x, y, h, v, GET_TEXT_F(MSG_BODY));

    #if ENABLED(COCOA_PRESS_EXTRA_HEATER)
      if (has_extra_heater()) {
        ui.bounds(POLY(h2_label), x, y, h, v);
        cmd.text(x, y, h, v, GET_TEXT_F(MSG_EXTERNAL));
      }
    #endif

    ui.bounds(POLY(h3_label), x, y, h, v);
    cmd.text(x, y, h, v, GET_TEXT_F(MSG_CHAMBER));

    #if ENABLED(TOUCH_UI_USE_UTF8)
      load_utf8_bitmaps(cmd); // Restore font bitmap handles
    #endif
  }

  if (what & FOREGROUND) {
    char str[15];
    cmd.cmd(COLOR_RGB(fluid_rgb));

    cmd.font(font_large).tag(10);

    format_temp(str, getActualTemp_celsius(E0));
    ui.bounds(POLY(h0_temp), x, y, h, v);
    cmd.text(x, y, h, v, str);

    format_temp(str, getActualTemp_celsius(E1));
    ui.bounds(POLY(h1_temp), x, y, h, v);
    cmd.text(x, y, h, v, str);

    #if ENABLED(COCOA_PRESS_EXTRA_HEATER)
      if (has_extra_heater()) {
        format_temp(str, getActualTemp_celsius(E2));
        ui.bounds(POLY(h2_temp), x, y, h, v);
        cmd.text(x, y, h, v, str);
      }
    #endif

    format_temp(str, getActualTemp_celsius(CHAMBER));
    ui.bounds(POLY(h3_temp), x, y, h, v);
    cmd.text(x, y, h, v, str);
  }
}

void StatusScreen::draw_syringe(draw_mode_t what) {
  #if ENABLED(COCOA_PRESS_CHOCOLATE_LEVEL_SENSOR)
    const float fill_level = get_chocolate_fill_level();
  #else
    constexpr float fill_level = 1.0f;
  #endif

  CommandProcessor cmd;
  PolyUI ui(cmd, what);

  if (what & BACKGROUND) {
    // Paint the shadow for the syringe
    ui.color(shadow_rgb);
    ui.shadow(POLY(syringe_outline), shadow_depth);
  }

  if (what & FOREGROUND) {
    int16_t x, y, h, v;

    // Paint the syringe icon
    ui.color(syringe_rgb);
    ui.fill(POLY(syringe_outline));

    ui.color(fluid_rgb);
    ui.bounds(POLY(syringe_fluid), x, y, h, v);
    cmd.cmd(SAVE_CONTEXT());
    cmd.cmd(SCISSOR_XY(x,y + v * (1.0 - fill_level)));
    cmd.cmd(SCISSOR_SIZE(h,  v *        fill_level));
    ui.fill(POLY(syringe_fluid), false);
    cmd.cmd(RESTORE_CONTEXT());

    ui.color(stroke_rgb);
    ui.fill(POLY(syringe));
  }
}

void StatusScreen::draw_buttons(draw_mode_t what) {
  int16_t x, y, h, v;

  const bool can_print        = isMediaInserted() && !isPrintingFromMedia();
  const bool sdOrHostPrinting = ExtUI::isPrinting();
  const bool sdOrHostPaused   = ExtUI::isPrintingPaused();

  CommandProcessor cmd;
  PolyUI ui(cmd, what);

  cmd.font(font_medium).colors(normal_btn);

  ui.bounds(POLY(park_btn), x, y, h, v);
  cmd.tag(1).button(x, y, h, v, GET_TEXT_F(MSG_FILAMENT_PARK_ENABLED));

  ui.bounds(POLY(load_chocolate_btn), x, y, h, v);
  cmd.tag(2).button(x, y, h, v, GET_TEXT_F(MSG_LOAD_UNLOAD));

  ui.bounds(POLY(preheat_chocolate_btn), x, y, h, v);
  cmd.tag(3).button(x, y, h, v, GET_TEXT_F(MSG_PREHEAT_CHOCOLATE));

  ui.bounds(POLY(menu_btn), x, y, h, v);
  cmd.tag(4).button(x, y, h, v, GET_TEXT_F(MSG_BUTTON_MENU));

  ui.bounds(POLY(pause_btn), x, y, h, v);
  cmd.tag(sdOrHostPaused ? 6 : 5).enabled(sdOrHostPrinting).button(x, y, h, v, sdOrHostPaused ? GET_TEXT_F(MSG_BUTTON_RESUME) : GET_TEXT_F(MSG_BUTTON_PAUSE));

  ui.bounds(POLY(stop_btn), x, y, h, v);
  cmd.tag(7).enabled(sdOrHostPrinting).button(x, y, h, v, GET_TEXT_F(MSG_BUTTON_STOP));

  ui.bounds(POLY(extrude_btn), x, y, h, v);
  cmd.tag(8).button(x, y, h, v, GET_TEXT_F(MSG_EXTRUDE));

  ui.bounds(POLY(print_btn), x, y, h, v);
  cmd.tag(9).colors(action_btn).enabled(can_print).button(x, y, h, v, GET_TEXT_F(MSG_BUTTON_PRINT));
}

void StatusScreen::onRedraw(draw_mode_t what) {
  if (what & BACKGROUND) {
    CommandProcessor cmd;
    cmd.cmd(CLEAR_COLOR_RGB(bg_color))
       .cmd(CLEAR(true,true,true))
       .tag(0);
  }

  draw_progress(what);
  draw_syringe(what);
  draw_temperature(what);
  draw_buttons(what);
}

bool StatusScreen::onTouchStart(uint8_t) {
  increment = 0;
  return true;
}

bool StatusScreen::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case  1: SpinnerDialogBox::enqueueAndWait_P(F("G28 O\nG27")); break;
    case  2: GOTO_SCREEN(LoadChocolateScreen); break;
    case  3: GOTO_SCREEN(PreheatMenu); break;
    case  4: GOTO_SCREEN(MainMenu); break;
    case  5:
      sound.play(twinkle, PLAY_ASYNCHRONOUS);
      if (ExtUI::isPrintingFromMedia())
        ExtUI::pausePrint();
      #ifdef ACTION_ON_PAUSE
        else host_action_pause();
      #endif
      GOTO_SCREEN(StatusScreen);
      break;
    case  6:
      sound.play(twinkle, PLAY_ASYNCHRONOUS);
      if (ExtUI::isPrintingFromMedia())
        ExtUI::resumePrint();
      #ifdef ACTION_ON_RESUME
        else host_action_resume();
      #endif
      GOTO_SCREEN(StatusScreen);
      break;
    case  7:
      GOTO_SCREEN(ConfirmAbortPrintDialogBox);
      current_screen.forget();
      PUSH_SCREEN(StatusScreen);
      break;
    case  9: GOTO_SCREEN(FilesScreen); break;
    case 10: GOTO_SCREEN(TemperatureScreen); break;
    default: return false;
  }
  // If a passcode is enabled, the LockScreen will prevent the
  // user from proceeding.
  LockScreen::check_passcode();
  return true;
}

bool StatusScreen::onTouchHeld(uint8_t tag) {
  if (tag == 8 && !ExtUI::isMoving()) {
    LoadChocolateScreen::setManualFeedrateAndIncrement(1, increment);
    UI_INCREMENT(AxisPosition_mm, E0);
    current_screen.onRefresh();
  }
  return false;
}

void StatusScreen::setStatusMessage(progmem_str) {
}

void StatusScreen::setStatusMessage(const char * const) {
}

void StatusScreen::onIdle() {
  reset_menu_timeout();
  if (refresh_timer.elapsed(STATUS_UPDATE_INTERVAL)) {
    if (!EventLoop::is_touch_held())
      onRefresh();
    refresh_timer.start();
  }
}

#endif // FTDI_COCOA_STATUS_SCREEN
