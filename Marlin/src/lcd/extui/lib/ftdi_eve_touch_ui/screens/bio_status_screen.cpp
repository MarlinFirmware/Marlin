/*************************
 * bio_status_screen.cpp *
 *************************/

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

#if ENABLED(TOUCH_UI_FTDI_EVE) && ANY(TOUCH_UI_LULZBOT_BIO, TOUCH_UI_COCOA_PRESS)

#include "screens.h"

#include "../ftdi_eve_lib/extras/poly_ui.h"

#ifdef TOUCH_UI_PORTRAIT
  #include "bio_printer_ui_portrait.h"
#else
  #include "bio_printer_ui_landscape.h"
#endif

#define GRID_COLS 2
#define GRID_ROWS 9

#define POLY(A) PolyUI::poly_reader_t(A, sizeof(A)/sizeof(A[0]))

const uint8_t shadow_depth = 5;
const float   max_speed  = 1.00;
const float   min_speed  = 0.02;
const float   emax_speed = 2.00;
const float   emin_speed = 0.70;

using namespace FTDI;
using namespace Theme;
using namespace ExtUI;

float StatusScreen::increment;
bool  StatusScreen::jog_xy;
bool  StatusScreen::fine_motion;

void StatusScreen::unlockMotors() {
  injectCommands_P(PSTR("M84 XY"));
  jog_xy = false;
}

void StatusScreen::draw_temperature(draw_mode_t what) {
  CommandProcessor cmd;
  PolyUI ui(cmd, what);

  int16_t x, y, h, v;

  cmd.tag(15);

  if (what & BACKGROUND) {
    cmd.cmd(COLOR_RGB(bg_color));

    #if ENABLED(TOUCH_UI_LULZBOT_BIO)
       // The LulzBot Bio shows the temperature for
       // the bed.

      #ifdef TOUCH_UI_PORTRAIT
        // Draw touch surfaces
        ui.bounds(POLY(target_temp), x, y, h, v);
        cmd.rectangle(x, y, h, v);
        ui.bounds(POLY(actual_temp), x, y, h, v);
        cmd.rectangle(x, y, h, v);
      #else
        ui.bounds(POLY(bed_temp), x, y, h, v);
        cmd.rectangle(x, y, h, v);
      #endif
      ui.bounds(POLY(bed_icon), x, y, h, v);
      cmd.rectangle(x, y, h, v);

      // Draw bed icon
      cmd.cmd(BITMAP_SOURCE(Bed_Heat_Icon_Info))
         .cmd(BITMAP_LAYOUT(Bed_Heat_Icon_Info))
         .cmd(BITMAP_SIZE  (Bed_Heat_Icon_Info))
         .cmd(COLOR_RGB(shadow_rgb))
         .icon (x + 2, y + 2, h, v, Bed_Heat_Icon_Info, icon_scale * 2)
         .cmd(COLOR_RGB(bg_text_enabled))
         .icon (x, y, h, v, Bed_Heat_Icon_Info, icon_scale * 2);
    #elif ENABLED(TOUCH_UI_COCOA_PRESS) && DISABLED(TOUCH_UI_PORTRAIT)
      // The CocoaPress shows the temperature for two
      // heating zones, but has no bed temperature

      cmd.cmd(COLOR_RGB(bg_text_enabled));
      cmd.font(font_medium);

      ui.bounds(POLY(h0_label), x, y, h, v);
      cmd.text(x, y, h, v, GET_TEXT_F(MSG_ZONE_1));

      ui.bounds(POLY(h1_label), x, y, h, v);
      cmd.text(x, y, h, v, GET_TEXT_F(MSG_ZONE_2));

      ui.bounds(POLY(h2_label), x, y, h, v);
      cmd.text(x, y, h, v, GET_TEXT_F(MSG_ZONE_3));

      ui.bounds(POLY(h3_label), x, y, h, v);
      cmd.text(x, y, h, v, GET_TEXT_F(MSG_CHAMBER));
    #else
      UNUSED(x);
      UNUSED(y);
      UNUSED(h);
      UNUSED(v);
    #endif

    #ifdef TOUCH_UI_USE_UTF8
      load_utf8_bitmaps(cmd); // Restore font bitmap handles
    #endif
  }

  if (what & FOREGROUND) {
    char str[15];
    cmd.cmd(COLOR_RGB(bg_text_enabled));
    #if ENABLED(TOUCH_UI_LULZBOT_BIO)
      cmd.font(font_medium);

      #ifdef TOUCH_UI_PORTRAIT
        if (!isHeaterIdle(BED) && getTargetTemp_celsius(BED) > 0)
          format_temp(str, getTargetTemp_celsius(BED));
        else
          strcpy_P(str, GET_TEXT(MSG_BED));

        ui.bounds(POLY(target_temp), x, y, h, v);
        cmd.text(x, y, h, v, str);

        format_temp(str, getActualTemp_celsius(BED));
        ui.bounds(POLY(actual_temp), x, y, h, v);
        cmd.text(x, y, h, v, str);
      #else
        if (!isHeaterIdle(BED) && getTargetTemp_celsius(BED) > 0)
          format_temp_and_temp(str, getActualTemp_celsius(BED), getTargetTemp_celsius(BED));
        else
          format_temp_and_idle(str, getActualTemp_celsius(BED));

        ui.bounds(POLY(bed_temp), x, y, h, v);
        cmd.text(x, y, h, v, str);
      #endif

    #elif ENABLED(TOUCH_UI_COCOA_PRESS) && DISABLED(TOUCH_UI_PORTRAIT)
      // The CocoaPress shows the temperature for two
      // heating zones, but has no bed temperature

      cmd.font(font_large);

      if (!isHeaterIdle(E0) && getTargetTemp_celsius(E0) > 0)
        format_temp_and_temp(str, getActualTemp_celsius(E0), getTargetTemp_celsius(E0));
      else
        format_temp_and_idle(str, getActualTemp_celsius(E0));

      ui.bounds(POLY(h0_temp), x, y, h, v);
      cmd.text(x, y, h, v, str);

      if (!isHeaterIdle(E1) && getTargetTemp_celsius(E1) > 0)
        format_temp_and_temp(str, getActualTemp_celsius(E1), getTargetTemp_celsius(E1));
      else
        format_temp_and_idle(str, getActualTemp_celsius(E1));

      ui.bounds(POLY(h1_temp), x, y, h, v);
      cmd.text(x, y, h, v, str);

      if (!isHeaterIdle(E2) && getTargetTemp_celsius(E2) > 0)
        format_temp_and_temp(str, getActualTemp_celsius(E2), getTargetTemp_celsius(E2));
      else
        format_temp_and_idle(str, getActualTemp_celsius(E2));

      ui.bounds(POLY(h2_temp), x, y, h, v);
      cmd.text(x, y, h, v, str);

      if (!isHeaterIdle(CHAMBER) && getTargetTemp_celsius(CHAMBER) > 0)
        format_temp_and_temp(str, getActualTemp_celsius(CHAMBER), getTargetTemp_celsius(CHAMBER));
      else
        format_temp_and_idle(str, getActualTemp_celsius(CHAMBER));

      ui.bounds(POLY(h3_temp), x, y, h, v);
      cmd.text(x, y, h, v, str);
    #else
      UNUSED(str);
    #endif
  }
}

void StatusScreen::draw_syringe(draw_mode_t what) {
  int16_t x, y, h, v;
  const float fill_level = (
    #ifdef E_MAX_POS
      1.0 - min(1.0, max(0.0, getAxisPosition_mm(E0) / E_MAX_POS))
    #else
      0.75
    #endif
  );
  const bool e_homed = TERN0(TOUCH_UI_LULZBOT_BIO, isAxisPositionKnown(E0));

  CommandProcessor cmd;
  PolyUI ui(cmd, what);

  if (what & BACKGROUND) {
    // Paint the shadow for the syringe
    ui.color(shadow_rgb);
    ui.shadow(POLY(syringe_outline), shadow_depth);
  }

  if (what & FOREGROUND && e_homed) {
    // Paint the syringe icon
    ui.color(syringe_rgb);
    ui.fill(POLY(syringe_outline));

    ui.color(fill_rgb);
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

void StatusScreen::draw_arrows(draw_mode_t what) {
  const bool e_homed = TERN1(TOUCH_UI_LULZBOT_BIO, isAxisPositionKnown(E0)),
             z_homed = isAxisPositionKnown(Z);

  CommandProcessor cmd;
  PolyUI ui(cmd, what);

  ui.button_fill  (fill_rgb);
  ui.button_stroke(stroke_rgb, 28);
  ui.button_shadow(shadow_rgb, shadow_depth);

  if ((what & BACKGROUND) || jog_xy) {
    ui.button(1, POLY(x_neg));
    ui.button(2, POLY(x_pos));
    ui.button(3, POLY(y_neg));
    ui.button(4, POLY(y_pos));
  }

  if ((what & BACKGROUND) || z_homed) {
    ui.button(5, POLY(z_neg));
    ui.button(6, POLY(z_pos));
  }

  if ((what & BACKGROUND) || e_homed) {
    #if DISABLED(TOUCH_UI_COCOA_PRESS)
      ui.button(7, POLY(e_neg));
    #endif
    ui.button(8, POLY(e_pos));
  }
}

void StatusScreen::draw_fine_motion(draw_mode_t what) {
  int16_t x, y, h, v;
  CommandProcessor cmd;
  PolyUI ui(cmd, what);

  cmd.font(
    #ifdef TOUCH_UI_PORTRAIT
      font_medium
    #else
      font_small
    #endif
  )
  .tag(16);

  if (what & BACKGROUND) {
    ui.bounds(POLY(fine_label), x, y, h, v);
    cmd.cmd(COLOR_RGB(bg_text_enabled))
       .text(x, y, h, v, GET_TEXT_F(MSG_FINE_MOTION));
  }

  if (what & FOREGROUND) {
    ui.bounds(POLY(fine_toggle), x, y, h, v);
    cmd.colors(ui_toggle)
       .toggle2(x, y, h, v, GET_TEXT_F(MSG_NO), GET_TEXT_F(MSG_YES), fine_motion);
  }
}

void StatusScreen::draw_overlay_icons(draw_mode_t what) {
  const bool e_homed = TERN1(TOUCH_UI_LULZBOT_BIO, isAxisPositionKnown(E0)),
             z_homed = isAxisPositionKnown(Z);

  CommandProcessor cmd;
  PolyUI ui(cmd, what);

  if (what & FOREGROUND) {
    ui.button_fill  (fill_rgb);
    ui.button_stroke(stroke_rgb, 28);
    ui.button_shadow(shadow_rgb, shadow_depth);

    if (!jog_xy)  ui.button(12, POLY(padlock));
    if (!e_homed) ui.button(13, POLY(home_e));
    if (!z_homed) ui.button(14, POLY(home_z));
  }
}

void StatusScreen::draw_buttons(draw_mode_t what) {
  int16_t x, y, h, v;

  const bool has_media = isMediaInserted() && !isPrintingFromMedia();

  CommandProcessor cmd;
  PolyUI ui(cmd, what);

  ui.bounds(POLY(usb_btn), x, y, h, v);
  cmd.font(font_medium)
     .colors(normal_btn)
     .enabled(has_media)
     .colors(has_media ? action_btn : normal_btn)
     .tag(9).button(x, y, h, v,
        isPrintingFromMedia() ?
          GET_TEXT_F(MSG_PRINTING) :
          GET_TEXT_F(MSG_BUTTON_MEDIA)
      );

  ui.bounds(POLY(menu_btn), x, y, h, v);
  cmd.colors(!has_media ? action_btn : normal_btn).tag(10).button(x, y, h, v, GET_TEXT_F(MSG_BUTTON_MENU));
}

void StatusScreen::loadBitmaps() {
  // Load the bitmaps for the status screen
  constexpr uint32_t base = ftdi_memory_map::RAM_G;
  CLCD::mem_write_pgm(base + Bed_Heat_Icon_Info.RAMG_offset, Bed_Heat_Icon, sizeof(Bed_Heat_Icon));

  // Load fonts for internationalization
  #ifdef TOUCH_UI_USE_UTF8
    load_utf8_data(base + UTF8_FONT_OFFSET);
  #endif
}

void StatusScreen::onRedraw(draw_mode_t what) {
  if (what & BACKGROUND) {
    CommandProcessor cmd;
    cmd.cmd(CLEAR_COLOR_RGB(bg_color))
       .cmd(CLEAR(true,true,true))
       .tag(0);
  }

  draw_syringe(what);
  draw_temperature(what);
  draw_arrows(what);
  draw_overlay_icons(what);
  draw_buttons(what);
  draw_fine_motion(what);
}

bool StatusScreen::onTouchStart(uint8_t) {
  increment = 0;
  return true;
}

bool StatusScreen::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case  1:
    case  2:
    case  3:
    case  4:
    case 12:
      if (!jog_xy) {
        jog_xy = true;
        injectCommands_P(PSTR("M17"));
      }
      jog({ 0, 0, 0 });
      break;
    case  5:
    case  6:
      jog({ 0, 0, 0 });
      break;
    case  9: GOTO_SCREEN(FilesScreen); break;
    case 10: GOTO_SCREEN(MainMenu); break;
    #if ENABLED(TOUCH_UI_LULZBOT_BIO)
      case 13: GOTO_SCREEN(BioConfirmHomeE); break;
    #endif
    case 14: SpinnerDialogBox::enqueueAndWait_P(F("G28 Z")); break;
    case 15: GOTO_SCREEN(TemperatureScreen);  break;
    case 16: fine_motion = !fine_motion; break;
    default: return false;
  }
  // If a passcode is enabled, the LockScreen will prevent the
  // user from proceeding.
  LockScreen::check_passcode();
  return true;
}

bool StatusScreen::onTouchHeld(uint8_t tag) {
  if (tag >= 1 && tag <= 4 && !jog_xy) return false;
  const float s = min_speed + (fine_motion ? 0 : (max_speed - min_speed) * sq(increment));
  switch (tag) {
    case 1: jog({-s,  0,  0}); break;
    case 2: jog({ s,  0,  0}); break;
    case 4: jog({ 0, -s,  0}); break; // NOTE: Y directions inverted because bed rather than needle moves
    case 3: jog({ 0,  s,  0}); break;
    case 5: jog({ 0,  0, -s}); break;
    case 6: jog({ 0,  0,  s}); break;
    case 7: case 8:
    {
      if (ExtUI::isMoving()) return false;
      const feedRate_t feedrate = emin_speed + (fine_motion ? 0 : (emax_speed - emin_speed) * sq(increment));
      const float increment = 0.25 * feedrate * (tag == 7 ? -1 : 1);
      MoveAxisScreen::setManualFeedrate(E0, feedrate);
      UI_INCREMENT(AxisPosition_mm, E0);
      current_screen.onRefresh();
      break;
    }
    default:
      return false;
  }
  increment = min(1.0f, increment + 0.1f);
  return false;
}

void StatusScreen::setStatusMessage(progmem_str pstr) {
  #ifdef TOUCH_UI_LULZBOT_BIO
    BioPrintingDialogBox::setStatusMessage(pstr);
  #else
    UNUSED(pstr);
  #endif
}

void StatusScreen::setStatusMessage(const char * const str) {
  #ifdef TOUCH_UI_LULZBOT_BIO
    BioPrintingDialogBox::setStatusMessage(str);
  #else
    UNUSED(str);
  #endif
}

void StatusScreen::onIdle() {
  reset_menu_timeout();
  if (refresh_timer.elapsed(STATUS_UPDATE_INTERVAL)) {
    if (!EventLoop::is_touch_held())
      onRefresh();
    #ifdef TOUCH_UI_LULZBOT_BIO
      if (isPrintingFromMedia())
        BioPrintingDialogBox::show();
    #endif
    refresh_timer.start();
  }
}

#endif // TOUCH_UI_FTDI_EVE
