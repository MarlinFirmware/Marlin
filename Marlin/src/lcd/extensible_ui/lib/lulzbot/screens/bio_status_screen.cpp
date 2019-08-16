/*************************
 * bio_status_screen.cpp *
 *************************/

/****************************************************************************
 *   Written By Mark Pelletier  2017 - Aleph Objects, Inc.                  *
 *   Written By Marcio Teixeira 2018 - Aleph Objects, Inc.                  *
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
 *   location: <http://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

#include "../config.h"

#if ENABLED(LULZBOT_TOUCH_UI) && defined(LULZBOT_USE_BIOPRINTER_UI)

#include "screens.h"

#include "../ftdi_eve_lib/extras/poly_ui.h"
#include "bio_printer_ui.h"

#define E_TRAVEL_LIMIT 60

#define GRID_COLS 2
#define GRID_ROWS 9

#define POLY(A) PolyUI::poly_reader_t(A, sizeof(A)/sizeof(A[0]))

#if ENABLED(SDSUPPORT) && defined(LULZBOT_MANUAL_USB_STARTUP)
  #include "../../../../sd/cardreader.h"
#endif

const uint8_t shadow_depth = 5;

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

    // Draw touch surfaces
    ui.bounds(POLY(target_temp), x, y, h, v);
    cmd.rectangle(x, y, h, v);
    ui.bounds(POLY(actual_temp), x, y, h, v);
    cmd.rectangle(x, y, h, v);
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
  }

  if (what & FOREGROUND) {
    char bed_str[15];

    cmd.font(font_xlarge)
       .cmd(COLOR_RGB(bg_text_enabled));

    if (!isHeaterIdle(BED) && getTargetTemp_celsius(BED) > 0) {
      sprintf_P(bed_str, PSTR("%-3d C"), ROUND(getTargetTemp_celsius(BED)));
      ui.bounds(POLY(target_temp), x, y, h, v);
      cmd.text(x, y, h, v, bed_str);
    }

    sprintf_P(bed_str, PSTR("%-3d C"), ROUND(getActualTemp_celsius(BED)));
    ui.bounds(POLY(actual_temp), x, y, h, v);
    cmd.text(x, y, h, v, bed_str);
  }
}

void StatusScreen::draw_syringe(draw_mode_t what) {
  int16_t x, y, h, v;
  const float fill_level = 1.0 - min(1.0, max(0.0, getAxisPosition_mm(E0) / E_TRAVEL_LIMIT));
  const bool  e_homed = isAxisPositionKnown(E0);

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
  const bool  e_homed = isAxisPositionKnown(E0);
  const bool  z_homed = isAxisPositionKnown(Z);

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
    ui.button(7, POLY(e_neg));
    ui.button(8, POLY(e_pos));
  }
}

void StatusScreen::draw_fine_motion(draw_mode_t what) {
  int16_t x, y, h, v;
  CommandProcessor cmd;
  PolyUI ui(cmd, what);

  cmd.font(font_medium)
     .tag(16);

  if (what & BACKGROUND) {

    ui.bounds(POLY(fine_label), x, y, h, v);
    cmd.cmd(COLOR_RGB(bg_text_enabled))
       .text(x, y, h, v, F("Fine motion:"));
  }

  if (what & FOREGROUND) {
    ui.bounds(POLY(fine_toggle), x, y, h, v);
    cmd.colors(ui_toggle)
       .toggle(x, y, h, v, F("no\xFFyes"), fine_motion);
  }
}

void StatusScreen::draw_overlay_icons(draw_mode_t what) {
  const bool  e_homed = isAxisPositionKnown(E0);
  const bool  z_homed = isAxisPositionKnown(Z);

  CommandProcessor cmd;
  PolyUI ui(cmd, what);

  if (what & FOREGROUND) {
    ui.button_fill  (fill_rgb);
    ui.button_stroke(stroke_rgb, 28);
    ui.button_shadow(shadow_rgb, shadow_depth);

    if (!jog_xy) {
      ui.button(12, POLY(padlock));
    }

    if (!e_homed) {
      ui.button(13, POLY(home_e));
    }

    if (!z_homed) {
      ui.button(14, POLY(home_z));
    }
  }
}

void StatusScreen::draw_buttons(draw_mode_t) {
  const bool has_media = isMediaInserted() && !isPrintingFromMedia();

  CommandProcessor cmd;

  cmd.font(font_medium)
     .colors(normal_btn)
    #if ENABLED(USB_FLASH_DRIVE_SUPPORT) && defined(LULZBOT_MANUAL_USB_STARTUP)
      .enabled(!Sd2Card::ready() || has_media)
    #else
      .enabled(has_media)
    #endif
     .colors(has_media ? action_btn : normal_btn)
     .tag(9).button(BTN_POS(1,9), BTN_SIZE(1,1),
        isPrintingFromMedia() ?
          F("Printing") :
      #if ENABLED(USB_FLASH_DRIVE_SUPPORT)
        #ifdef LULZBOT_MANUAL_USB_STARTUP
        (Sd2Card::ready() ? F("USB Drive") : F("Enable USB"))
        #else
        F("USB Drive")
        #endif
      #else
        F("SD Card")
      #endif
      );

  cmd.colors(!has_media ? action_btn : normal_btn).tag(10).button(BTN_POS(2,9), BTN_SIZE(1,1), F("Menu"));
}

void StatusScreen::onStartup() {
  // Load the bitmaps for the status screen
  constexpr uint32_t base = ftdi_memory_map::RAM_G;
  CLCD::mem_write_pgm(base + Bed_Heat_Icon_Info.RAMG_offset, Bed_Heat_Icon, sizeof(Bed_Heat_Icon));
}

void StatusScreen::onRedraw(draw_mode_t what) {
  if (what & BACKGROUND) {
    CommandProcessor cmd;
    cmd.cmd(CLEAR_COLOR_RGB(bg_color));
    cmd.cmd(CLEAR(true,true,true));
  }

  draw_syringe(what);
  draw_temperature(what);
  draw_arrows(what);
  draw_overlay_icons(what);
  draw_buttons(what);
  draw_fine_motion(what);
}

bool StatusScreen::onTouchStart(uint8_t) {
  increment = fine_motion ? 0.25 : 1;
  return true;
}

bool StatusScreen::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 12:
      if (!jog_xy) {
        jog_xy = true;
        injectCommands_P(PSTR("M17"));
      }
      break;
    case 9:
      #if ENABLED(USB_FLASH_DRIVE_SUPPORT) && defined(LULZBOT_MANUAL_USB_STARTUP)
      if (!Sd2Card::ready()) {
        StatusScreen::setStatusMessage(F("Insert USB drive..."));
        Sd2Card::usbStartup();
      } else {
        GOTO_SCREEN(FilesScreen);
      }
      #else
        GOTO_SCREEN(FilesScreen);
      #endif
      break;
    case 10: GOTO_SCREEN(MainMenu); break;
    case 13: SpinnerDialogBox::enqueueAndWait_P(F("G112"));  break;
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
  if (ExtUI::isMoving()) return false; // Don't allow moves to accumulate
  #define UI_INCREMENT_AXIS(axis) MoveAxisScreen::setManualFeedrate(axis, increment); UI_INCREMENT(AxisPosition_mm, axis);
  #define UI_DECREMENT_AXIS(axis) MoveAxisScreen::setManualFeedrate(axis, increment); UI_DECREMENT(AxisPosition_mm, axis);
  switch (tag) {
    case 1: UI_DECREMENT_AXIS(X);  break;
    case 2: UI_INCREMENT_AXIS(X);  break;
    case 4: UI_DECREMENT_AXIS(Y);  break; // NOTE: Y directions inverted because bed rather than needle moves
    case 3: UI_INCREMENT_AXIS(Y);  break;
    case 5: UI_DECREMENT_AXIS(Z);  break;
    case 6: UI_INCREMENT_AXIS(Z);  break;
    case 7: UI_DECREMENT_AXIS(E0); break;
    case 8: UI_INCREMENT_AXIS(E0); break;
    default: return false;
  }
  #undef UI_DECREMENT_AXIS
  #undef UI_INCREMENT_AXIS
  if (increment < 10 && !fine_motion)
    increment += 0.5;
  current_screen.onRefresh();
  return false;
}

void StatusScreen::setStatusMessage(progmem_str pstr) {
  BioPrintingDialogBox::setStatusMessage(pstr);
}

void StatusScreen::setStatusMessage(const char * const str) {
  BioPrintingDialogBox::setStatusMessage(str);
}

void StatusScreen::onIdle() {
  if (isPrintingFromMedia())
    BioPrintingDialogBox::show();

  if (refresh_timer.elapsed(STATUS_UPDATE_INTERVAL)) {
    onRefresh();
    refresh_timer.start();
  }
}

#endif // LULZBOT_TOUCH_UI
