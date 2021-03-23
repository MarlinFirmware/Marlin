/************************************
 * cocoa_press_unload_cartridge.cpp *
 ************************************/

/****************************************************************************
 *   Written By Mark Pelletier  2017 - Aleph Objects, Inc.                  *
 *   Written By Marcio Teixeira 2018 - Aleph Objects, Inc.                  *
 *   Written By Marcio Teixeira 2020 - Cocoa Press                          *
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
#include "screen_data.h"

#ifdef FTDI_COCOA_LOAD_CHOCOLATE_SCREEN

#include "../ftdi_eve_lib/extras/poly_ui.h"

#include "cocoa_press_ui.h"

#define POLY(A) PolyUI::poly_reader_t(A, sizeof(A)/sizeof(A[0]))

const uint8_t shadow_depth = 5;

using namespace ExtUI;
using namespace FTDI;
using namespace Theme;

constexpr static LoadChocolateScreenData &mydata = screen_data.LoadChocolateScreen;

void LoadChocolateScreen::draw_syringe(draw_mode_t what) {
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

void LoadChocolateScreen::draw_buttons(draw_mode_t what) {
  int16_t x, y, h, v;

  CommandProcessor cmd;
  PolyUI ui(cmd, what);

  cmd.font(font_medium).colors(normal_btn);

  ui.bounds(POLY(load_screen_unload_btn), x, y, h, v);
  cmd.tag(2).button(x, y, h, v, GET_TEXT_F(MSG_FULL_UNLOAD));

  ui.bounds(POLY(load_screen_load_btn), x, y, h, v);
  cmd.tag(3).button(x, y, h, v, GET_TEXT_F(MSG_FULL_LOAD));

  ui.bounds(POLY(load_screen_back_btn), x, y, h, v);
  cmd.tag(1).colors(action_btn).button(x, y, h, v, GET_TEXT_F(MSG_BACK));
}

void LoadChocolateScreen::draw_text(draw_mode_t what) {
  if (what & BACKGROUND) {
    int16_t x, y, h, v;

    CommandProcessor cmd;
    PolyUI ui(cmd, what);

    cmd.font(font_medium).cmd(COLOR_RGB(bg_text_enabled));

    ui.bounds(POLY(load_sreen_title), x, y, h, v);
    cmd.tag(2).text(x, y, h, v, GET_TEXT_F(MSG_LOAD_UNLOAD));

    ui.bounds(POLY(load_screen_increment), x, y, h, v);
    cmd.tag(3).text(x, y, h, v, GET_TEXT_F(MSG_INCREMENT));
  }
}

void LoadChocolateScreen::draw_arrows(draw_mode_t what) {
  CommandProcessor cmd;
  PolyUI ui(cmd, what);

  ui.button_fill  (fill_rgb);
  ui.button_stroke(stroke_rgb, 28);
  ui.button_shadow(shadow_rgb, shadow_depth);

  constexpr uint8_t style = PolyUI::REGULAR;

  ui.button(4, POLY(load_screen_extrude), style);
  ui.button(5, POLY(load_screen_retract), style);
}

void LoadChocolateScreen::onEntry() {
  mydata.repeat_tag = 0;
}

void LoadChocolateScreen::onRedraw(draw_mode_t what) {
  if (what & BACKGROUND) {
    CommandProcessor cmd;
    cmd.cmd(CLEAR_COLOR_RGB(bg_color))
       .cmd(CLEAR(true,true,true))
       .tag(0);
  }

  draw_syringe(what);
  draw_arrows(what);
  draw_buttons(what);
  draw_text(what);
}

bool LoadChocolateScreen::onTouchStart(uint8_t) {
  mydata.repeat_tag = 0;
  return true;
}

bool LoadChocolateScreen::onTouchEnd(uint8_t tag) {
  using namespace ExtUI;
  switch (tag) {
    case 2:
      mydata.repeat_tag = (mydata.repeat_tag == 2) ? 0 : 2;
      break;
    case 3:
      mydata.repeat_tag = (mydata.repeat_tag == 3) ? 0 : 3;
      break;
    case 1: GOTO_PREVIOUS(); break;
  }
  return true;
}

void LoadChocolateScreen::setManualFeedrateAndIncrement(float feedrate_mm_s, float &increment_mm) {
  // Compute increment so feedrate so that the tool lags the adjuster when it is
  // being held down, this allows enough margin for the planner to
  // connect segments and even out the motion.
  ExtUI::setFeedrate_mm_s(feedrate_mm_s);
  increment_mm = feedrate_mm_s / ((TOUCH_REPEATS_PER_SECOND) * 0.80f);
}

bool LoadChocolateScreen::onTouchHeld(uint8_t tag) {
  if (ExtUI::isMoving()) return false; // Don't allow moves to accumulate
  float increment;
  setManualFeedrateAndIncrement(20, increment);
  #define UI_INCREMENT_AXIS(axis) UI_INCREMENT(AxisPosition_mm, axis);
  #define UI_DECREMENT_AXIS(axis) UI_DECREMENT(AxisPosition_mm, axis);
  switch (tag) {
    case 2: {
      if (get_chocolate_fill_level() < 0.1) {
        mydata.repeat_tag = 0;
        return false;
      }
      UI_INCREMENT_AXIS(E0);
      break;
    }
    case 3: {
      if (get_chocolate_fill_level() > 0.75) {
        mydata.repeat_tag = 0;
        return false;
      }
      UI_DECREMENT_AXIS(E0);
      break;
    }
    case 4:
      UI_INCREMENT_AXIS(E0);
      break;
    case 5:
      UI_DECREMENT_AXIS(E0);
      break;
    default: return false;
  }
  #undef UI_DECREMENT_AXIS
  #undef UI_INCREMENT_AXIS
  return false;
}

void LoadChocolateScreen::onIdle() {
  reset_menu_timeout();
  if (mydata.repeat_tag) onTouchHeld(mydata.repeat_tag);
  if (refresh_timer.elapsed(STATUS_UPDATE_INTERVAL)) {
    if (!EventLoop::is_touch_held())
      onRefresh();
    refresh_timer.start();
  }
  BaseScreen::onIdle();
}
#endif // FTDI_COCOA_LOAD_CHOCOLATE_SCREEN
