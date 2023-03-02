/*********************************
 * cocoa_press/status_screen.cpp *
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
#include "../screens.h"

#ifdef COCOA_STATUS_SCREEN

#include "cocoa_press_ui.h"

#define POLY(A) PolyUI::poly_reader_t(A, sizeof(A)/sizeof(A[0]))
#define ICON_POS(x,y,w,h) x,     y,     h, h
#define TEXT_POS(x,y,w,h) x + h, y, w - h, h

const uint8_t shadow_depth = 5;

using namespace FTDI;
using namespace Theme;
using namespace ExtUI;

float StatusScreen::increment;

void StatusScreen::_format_time(char *outstr, uint32_t time) {
  const uint8_t hrs = time / 3600,
                min = (time / 60) % 60,
                sec = time % 60;
  if (hrs)
    sprintf_P(outstr, PSTR("%02d:%02d"), hrs, min);
  else
    sprintf_P(outstr, PSTR("%02d:%02ds"), min, sec);
}

void StatusScreen::loadBitmaps() {
  // Load the bitmaps for the status screen
  using namespace Theme;
  constexpr uint32_t base = ftdi_memory_map::RAM_G;
  CLCD::mem_write_xbm(base + Light_Bulb_Info.RAMG_offset,    Light_Bulb,    sizeof(Light_Bulb));
  CLCD::mem_write_xbm(base + Chamber_Icon_Info.RAMG_offset,  Chamber_Icon,  sizeof(Chamber_Icon));
  CLCD::mem_write_xbm(base + Clock_Icon_Info.RAMG_offset,    Clock_Icon,    sizeof(Clock_Icon));
  CLCD::mem_write_xbm(base + File_Icon_Info.RAMG_offset,     File_Icon,     sizeof(File_Icon));
  CLCD::mem_write_xbm(base + TD_Icon_Info.RAMG_offset,       TD_Icon,       sizeof(TD_Icon));
  CLCD::mem_write_xbm(base + Extruder_Icon_Info.RAMG_offset, Extruder_Icon, sizeof(Extruder_Icon));
  CLCD::mem_write_xbm(base + Bed_Heat_Icon_Info.RAMG_offset, Bed_Heat_Icon, sizeof(Bed_Heat_Icon));
  CLCD::mem_write_xbm(base + Fan_Icon_Info.RAMG_offset,      Fan_Icon,      sizeof(Fan_Icon));

  // Load fonts for internationalization
  #if ENABLED(TOUCH_UI_USE_UTF8)
    load_utf8_data(base + UTF8_FONT_OFFSET);
  #endif
}

void StatusScreen::draw_time(draw_mode_t what) {
  CommandProcessor cmd;
  PolyUI ui(cmd, what);

  int16_t x, y, w, h;
  ui.bounds(POLY(print_time_hms), x, y, w, h);

  if (what & BACKGROUND) {
    cmd.cmd(COLOR_RGB(bg_text_enabled))
       .cmd (BITMAP_SOURCE(Clock_Icon_Info))
       .cmd (BITMAP_LAYOUT(Clock_Icon_Info))
       .cmd (BITMAP_SIZE  (Clock_Icon_Info))
       .icon(ICON_POS(x, y, w, h), Clock_Icon_Info, icon_scale)
       .cmd(COLOR_RGB(bg_text_enabled));
  }

  if (what & FOREGROUND) {
    const uint32_t elapsed = getProgress_seconds_elapsed();
    char elapsed_str[10];
    _format_time(elapsed_str, elapsed);

    cmd.font(font_medium)
       .cmd(COLOR_RGB(bg_text_enabled))
       .text(TEXT_POS(x, y, w, h), elapsed_str);
  }
}


void StatusScreen::draw_progress(draw_mode_t what) {
  CommandProcessor cmd;
  PolyUI ui(cmd, what);

  int16_t x, y, w, h;

  cmd.cmd(COLOR_RGB(accent_color_1));
  cmd.font(font_medium);

  if (what & FOREGROUND) {
    // Draw progress bar
    ui.bounds(POLY(file_name), x, y, w, h);
    const uint16_t bar_width = w * getProgress_percent() / 100;
    cmd.tag(8)
       .cmd(COLOR_RGB(accent_color_5))
       .rectangle(x, y, bar_width, h);
  }
}

void StatusScreen::draw_temperature(draw_mode_t what) {
  CommandProcessor cmd;
  PolyUI ui(cmd, what);

  int16_t x, y, w, h;

  if (what & BACKGROUND) {
    cmd.cmd(COLOR_RGB(fluid_rgb));
    cmd.font(font_medium).tag(10);

    /*ui.bounds(POLY(temp_lbl), x, y, w, h);
    cmd.text(x, y, w, h, F("Temp"));

    ui.bounds(POLY(set_lbl), x, y, w, h);
    cmd.text(x, y, w, h, F("Set"));*/

    ui.bounds(POLY(chocolate_label), x, y, w, h);
    cmd.text(x, y, w, h, F("Cocoa Press"));

    ui.bounds(POLY(h0_label), x, y, w, h);
    cmd.text(x, y, w, h, GET_TEXT_F(MSG_NOZZLE));

    ui.bounds(POLY(h1_label), x, y, w, h);
    cmd.text(x, y, w, h, GET_TEXT_F(MSG_BODY));

    #if ENABLED(COCOA_PRESS_EXTRA_HEATER)
      if (has_extra_heater()) {
        ui.bounds(POLY(h2_label), x, y, w, h);
        cmd.text(x, y, w, h, GET_TEXT_F(MSG_EXTERNAL));
      }
    #endif

    #if ENABLED(COCOA_PRESS_CHAMBER_COOLER)
      ui.bounds(POLY(h3_label), x, y, w, h);
      cmd.text(x, y, w, h, GET_TEXT_F(MSG_CHAMBER));
    #endif

    #if ENABLED(TOUCH_UI_USE_UTF8)
      load_utf8_bitmaps(cmd); // Restore font bitmap handles
    #endif
  }

  if (what & FOREGROUND) {
    char str[15];
    cmd.cmd(COLOR_RGB(fluid_rgb));

    cmd.font(font_large).tag(10);

    // Show the actual temperatures

    format_temp(str, getActualTemp_celsius(E0));
    ui.bounds(POLY(h0_temp), x, y, w, h);
    cmd.text(x, y, w, h, str);

    format_temp(str, getActualTemp_celsius(E1));
    ui.bounds(POLY(h1_temp), x, y, w, h);
    cmd.text(x, y, w, h, str);

    #if ENABLED(COCOA_PRESS_EXTRA_HEATER)
      if (has_extra_heater()) {
        format_temp(str, getActualTemp_celsius(E2));
        ui.bounds(POLY(h2_temp), x, y, w, h);
        cmd.text(x, y, w, h, str);
      }
    #endif

    #if ENABLED(COCOA_PRESS_CHAMBER_COOLER)
      format_temp(str, getActualTemp_celsius(CHAMBER));
      ui.bounds(POLY(h3_temp), x, y, w, h);
      cmd.text(x, y, w, h, str);
    #endif

    /*// Show the set temperatures
    format_temp(str, getTargetTemp_celsius(E0));
    ui.bounds(POLY(h0_set), x, y, w, h);
    cmd.text(x, y, w, h, str);

    format_temp(str, getTargetTemp_celsius(E1));
    ui.bounds(POLY(h1_set), x, y, w, h);
    cmd.text(x, y, w, h, str);

    #if ENABLED(COCOA_PRESS_EXTRA_HEATER)
      if (has_extra_heater()) {
        format_temp(str, getTargetTemp_celsius(E2));
        ui.bounds(POLY(h2_set), x, y, w, h);
        cmd.text(x, y, w, h, str);
      }
    #endif

    #if ENABLED(COCOA_PRESS_CHAMBER_COOLER)
      format_temp(str, getTargetTemp_celsius(CHAMBER));
      ui.bounds(POLY(h3_set), x, y, w, h);
      cmd.text(x, y, w, h, str);
    #endif*/
  }
}

void StatusScreen::draw_buttons(draw_mode_t what) {
  int16_t x, y, w, h;

  const bool can_print        = !isPrinting() && isMediaInserted() && isFileSelected();
  const bool can_select       = !isPrinting() && isMediaInserted();
  const bool sdOrHostPrinting = ExtUI::isPrinting();
  const bool sdOrHostPaused   = ExtUI::isPrintingPaused();

  CommandProcessor cmd;
  PolyUI ui(cmd, what);

  cmd.font(font_medium).colors(normal_btn);

  ui.bounds(POLY(load_chocolate_btn), x, y, w, h);
  cmd.tag(1).button(x, y, w, h, GET_TEXT_F(MSG_LOAD_UNLOAD));

  ui.bounds(POLY(extrude_btn), x, y, w, h);
  cmd.tag(2).button(x, y, w, h, GET_TEXT_F(MSG_EXTRUDE));

  ui.bounds(POLY(preheat_chocolate_btn), x, y, w, h);
  cmd.tag(3).button(x, y, w, h, GET_TEXT_F(MSG_PREHEAT_CHOCOLATE));

  ui.bounds(POLY(menu_btn), x, y, w, h);
  cmd.tag(4).button(x, y, w, h, GET_TEXT_F(MSG_BUTTON_MENU));

  ui.bounds(POLY(media_btn), x, y, w, h);
  cmd.tag(5).enabled(can_select).button(x, y, w, h, GET_TEXT_F(MSG_BUTTON_MEDIA));

  ui.bounds(POLY(print_btn), x, y, w, h);
  cmd.tag(6).colors(action_btn).enabled(can_print).button(x, y, w, h, GET_TEXT_F(MSG_BUTTON_PRINT));

  ui.bounds(POLY(pause_btn), x, y, w, h);
  cmd.tag(sdOrHostPaused ? 8 : 7).enabled(sdOrHostPrinting).button(x, y, w, h, sdOrHostPaused ? GET_TEXT_F(MSG_BUTTON_RESUME) : GET_TEXT_F(MSG_BUTTON_PAUSE));

  ui.bounds(POLY(stop_btn), x, y, w, h);
  cmd.tag(9).enabled(sdOrHostPrinting).button(x, y, w, h, GET_TEXT_F(MSG_BUTTON_STOP));
}

void StatusScreen::draw_file(draw_mode_t what) {
  int16_t x, y, w, h;

  CommandProcessor cmd;
  PolyUI ui(cmd, what);

  ui.bounds(POLY(file_name), x, y, w, h);

  if (what & BACKGROUND) {
    cmd.tag(5)
       .cmd(COLOR_RGB(bg_text_enabled))
       .cmd (BITMAP_SOURCE(File_Icon_Info))
       .cmd (BITMAP_LAYOUT(File_Icon_Info))
       .cmd (BITMAP_SIZE  (File_Icon_Info))
       .icon(ICON_POS(x, y, w, h), File_Icon_Info, icon_scale);
  }

  if (what & FOREGROUND) {
    cmd.cmd(COLOR_RGB(bg_text_enabled));

    if(!isMediaInserted())
      draw_text_with_ellipsis(cmd, TEXT_POS(x, y, w, h), F("No media present"), OPT_CENTERY, font_small);
    else if(isFileSelected()) {
      FileList list;
      draw_text_with_ellipsis(cmd, TEXT_POS(x, y, w, h), list.filename(), OPT_CENTERY, font_small);
    } else
      draw_text_with_ellipsis(cmd, TEXT_POS(x, y, w, h), F("No file selected"), OPT_CENTERY, font_small);
  }
}

bool StatusScreen::isFileSelected() {
  if(!isMediaInserted()) return false;
  FileList list;
  if(list.isDir()) return false;
  const char *filename = list.filename();
  if(filename[0] == '\0') return false;
  return true;
}

void StatusScreen::onRedraw(draw_mode_t what) {
  if (what & BACKGROUND) {
    CommandProcessor cmd;
    cmd.cmd(CLEAR_COLOR_RGB(bg_color))
       .cmd(CLEAR(true,true,true))
       .tag(0);
  }

  draw_file(what);
  draw_time(what);
  draw_progress(what);
  draw_temperature(what);
  draw_buttons(what);
}

bool StatusScreen::onTouchStart(uint8_t) {
  increment = 0;
  return true;
}

bool StatusScreen::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case  1: GOTO_SCREEN(LoadChocolateScreen); break;
    case  3: GOTO_SCREEN(PreheatMenu); break;
    case  4: GOTO_SCREEN(MainMenu); break;
    case  5: GOTO_SCREEN(FilesScreen); break;
    case  6: GOTO_SCREEN(ConfirmStartPrintDialogBox); break;
    case  7:
      sound.play(twinkle, PLAY_ASYNCHRONOUS);
      if (ExtUI::isPrintingFromMedia())
        ExtUI::pausePrint();
      #ifdef ACTION_ON_PAUSE
        else hostui.pause();
      #endif
      GOTO_SCREEN(StatusScreen);
      break;
    case  8:
      sound.play(twinkle, PLAY_ASYNCHRONOUS);
      if (ExtUI::isPrintingFromMedia())
        ExtUI::resumePrint();
      #ifdef ACTION_ON_RESUME
        else hostui.resume();
      #endif
      GOTO_SCREEN(StatusScreen);
      break;
    case  9:
      GOTO_SCREEN(ConfirmAbortPrintDialogBox);
      current_screen.forget();
      PUSH_SCREEN(StatusScreen);
      break;

    case 10: GOTO_SCREEN(TemperatureScreen); break;
    default: return false;
  }
  // If a passcode is enabled, the LockScreen will prevent the
  // user from proceeding.
  LockScreen::check_passcode();
  return true;
}

bool StatusScreen::onTouchHeld(uint8_t tag) {
  if (tag == 2 && !ExtUI::isMoving()) {
    LoadChocolateScreen::setManualFeedrateAndIncrement(1, increment);
    UI_INCREMENT(AxisPosition_mm, E0);
    current_screen.onRefresh();
  }
  return false;
}

void StatusScreen::setStatusMessage(FSTR_P) {
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

void StatusScreen::onMediaInserted() {
  if (AT_SCREEN(StatusScreen))
    setStatusMessage(GET_TEXT_F(MSG_MEDIA_INSERTED));
}

void StatusScreen::onMediaRemoved() {
  if (AT_SCREEN(StatusScreen) || ExtUI::isPrintingFromMedia())
    setStatusMessage(GET_TEXT_F(MSG_MEDIA_REMOVED));
}

#endif // COCOA_STATUS_SCREEN
