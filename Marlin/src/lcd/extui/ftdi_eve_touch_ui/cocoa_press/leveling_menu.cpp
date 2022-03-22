/*********************************
 * cocoa_press/leveling_menu.cpp *
 *********************************/

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
 *   location: <https://www.gnu.org/licenses/>.                             *
 ****************************************************************************/

#include "../config.h"
#include "../screens.h"

#if ENABLED(COCOA_LEVELING_MENU)

#if BOTH(HAS_BED_PROBE, BLTOUCH)
  #include "../../../../feature/bltouch.h"
#endif

using namespace FTDI;
using namespace ExtUI;
using namespace Theme;

#define GRID_ROWS 5
#define GRID_COLS 3
#define BED_MESH_TITLE_POS BTN_POS(1,1), BTN_SIZE(3,1)
#define PROBE_BED_POS      BTN_POS(1,2), BTN_SIZE(1,1)
#define SHOW_MESH_POS      BTN_POS(2,2), BTN_SIZE(1,1)
#define EDIT_MESH_POS      BTN_POS(3,2), BTN_SIZE(1,1)
#define BLTOUCH_TITLE_POS  BTN_POS(1,3), BTN_SIZE(3,1)
#define BLTOUCH_RESET_POS  BTN_POS(1,4), BTN_SIZE(1,1)
#define BLTOUCH_TEST_POS   BTN_POS(2,4), BTN_SIZE(1,1)
#define BACK_POS           BTN_POS(1,5), BTN_SIZE(3,1)

void LevelingMenu::onRedraw(draw_mode_t what) {
  if (what & BACKGROUND) {
    CommandProcessor cmd;
    cmd.cmd(CLEAR_COLOR_RGB(Theme::bg_color))
       .cmd(CLEAR(true,true,true))
       .tag(0);
  }

  if (what & FOREGROUND) {
    CommandProcessor cmd;
    cmd.font(font_large)
       .cmd(COLOR_RGB(bg_text_enabled))
       .text(BED_MESH_TITLE_POS, GET_TEXT_F(MSG_BED_LEVELING))
       .text(BLTOUCH_TITLE_POS, GET_TEXT_F(MSG_BLTOUCH))
       .font(font_medium).colors(normal_btn)
       .tag(2).button(PROBE_BED_POS, GET_TEXT_F(MSG_PROBE_BED))
       .enabled(ENABLED(HAS_MESH))
       .tag(3).button(SHOW_MESH_POS, GET_TEXT_F(MSG_MESH_VIEW))
       .enabled(ENABLED(HAS_MESH))
       .tag(4).button(EDIT_MESH_POS, GET_TEXT_F(MSG_EDIT_MESH))
       #undef  GRID_COLS
       #define GRID_COLS 2
       .tag(5).button(BLTOUCH_RESET_POS, GET_TEXT_F(MSG_BLTOUCH_RESET))
       .tag(6).button(BLTOUCH_TEST_POS,  GET_TEXT_F(MSG_BLTOUCH_SELFTEST))
       #undef  GRID_COLS
       #define GRID_COLS 3
       .colors(action_btn)
       .tag(1).button(BACK_POS, GET_TEXT_F(MSG_BUTTON_DONE));
  }
}

bool LevelingMenu::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case 1: GOTO_PREVIOUS(); break;
    case 2: BedMeshViewScreen::doProbe(); break;
    case 3: BedMeshViewScreen::show(); break;
    case 4: BedMeshEditScreen::show(); break;
    case 5: injectCommands(F("M280 P0 S60")); break;
    case 6: SpinnerDialogBox::enqueueAndWait(F("M280 P0 S90\nG4 P100\nM280 P0 S120")); break;
    default: return false;
  }
  return true;
}

#endif // COCOA_LEVELING_MENU
