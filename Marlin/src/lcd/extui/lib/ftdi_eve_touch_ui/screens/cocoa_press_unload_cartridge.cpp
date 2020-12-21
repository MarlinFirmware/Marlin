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

#if ENABLED(TOUCH_UI_FTDI_EVE) && ENABLED(TOUCH_UI_COCOA_PRESS)

#include "screens.h"
#include "screen_data.h"

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

void UnloadCartridgeScreen::onRedraw(draw_mode_t what) {
  CommandProcessor cmd;

  if (what & BACKGROUND) {
    cmd.cmd(CLEAR_COLOR_RGB(bg_color))
       .cmd(CLEAR(true,true,true))
       .cmd(COLOR_RGB(bg_text_enabled))
       .tag(0)
       .font(font_large)
       .text(TITLE_POS, GET_TEXT_F(MSG_UNLOAD_CARTRIDGE));
       draw_text_box(cmd, DESCRIPTION_POS, F(
        "Press and hold the buttons below to help "
        "you unlock the cartridge. After unlocking, "
        "press and hold the Cartridge Out button "
        "until the cartridge is sticking out of the "
        "extruder enough to grip and remove. After "
        "removing the cartridge, continue holding the "
        "Cartridge Out button until the plunger adapter is "
        "visible at the bottom of the extruder."
       ),
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

bool UnloadCartridgeScreen::onTouchEnd(uint8_t tag) {
  using namespace ExtUI;
  switch (tag) {
    case 1: GOTO_PREVIOUS(); break;
  }
  return true;
}

bool UnloadCartridgeScreen::onTouchHeld(uint8_t tag) {
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

#endif // TOUCH_UI_FTDI_EVE
