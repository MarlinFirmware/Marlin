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

using namespace ExtUI;
using namespace FTDI;
using namespace Theme;

#define GRID_COLS 2
#define GRID_ROWS 6

#define TITLE_POS             BTN_POS(1,1), BTN_SIZE(2,1)
#define DESCRIPTION_POS       BTN_POS(1,2), BTN_SIZE(2,3)
#define CARTRIDGE_OUT_BTN_POS BTN_POS(1,5), BTN_SIZE(1,1)
#define CARTRIDGE_IN_BTN_POS  BTN_POS(2,5), BTN_SIZE(1,1)
#define BACK_BTN_POS          BTN_POS(1,6), BTN_SIZE(2,1)

void LoadChocolateScreen::onRedraw(draw_mode_t what) {
  CommandProcessor cmd;

  if (what & BACKGROUND) {
    cmd.cmd(CLEAR_COLOR_RGB(bg_color))
       .cmd(CLEAR(true,true,true))
       .cmd(COLOR_RGB(bg_text_enabled))
       .tag(0)
       .font(font_large)
       .text(TITLE_POS, GET_TEXT_F(MSG_LOAD_CHOCOLATE));
       draw_text_box(cmd, DESCRIPTION_POS, F(
        "Drop your chocolate refill into the cartridge. "
        "Press and hold the Cartridge Out button until "
        "the plunger adapter is visible at the bottom of "
        "the extruder. Securely attach a red plunger to "
        "the plunger adapter and load the cartridge onto "
        "the plunger. Press and hold Cartridge In button "
        "until cartridge is fully loaded into the extruder, "
        "and use the buttons to help follow the locking path "
        "to lock"),
        OPT_CENTERY, font_medium);
  }

  if (what & FOREGROUND) {
    cmd.font(font_medium)
       .colors(normal_btn)
       .tag(2).button(CARTRIDGE_OUT_BTN_POS, GET_TEXT_F(MSG_CARTRIDGE_OUT))
       .tag(3).button(CARTRIDGE_IN_BTN_POS,  GET_TEXT_F(MSG_CARTRIDGE_IN))
              .colors(action_btn)
       .tag(1).button(BACK_BTN_POS,    GET_TEXT_F(MSG_BACK));
  }
}

bool LoadChocolateScreen::onTouchEnd(uint8_t tag) {
  using namespace ExtUI;
  switch (tag) {
    case 1: GOTO_PREVIOUS(); break;
  }
  return true;
}

bool LoadChocolateScreen::onTouchHeld(uint8_t tag) {
  if (ExtUI::isMoving()) return false; // Don't allow moves to accumulate
  constexpr float increment = 0.25;
  MoveAxisScreen::setManualFeedrate(E0, increment);
  #define UI_INCREMENT_AXIS(axis) UI_INCREMENT(AxisPosition_mm, axis);
  #define UI_DECREMENT_AXIS(axis) UI_DECREMENT(AxisPosition_mm, axis);
  switch (tag) {
    case 2: UI_DECREMENT_AXIS(E0); break;
    case 3: UI_INCREMENT_AXIS(E0); break;
    default: return false;
  }
  #undef UI_DECREMENT_AXIS
  #undef UI_INCREMENT_AXIS
  return false;
}

#endif // FTDI_COCOA_LOAD_CHOCOLATE_SCREEN
