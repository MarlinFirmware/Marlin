/*********************
 * leveling_menu.cpp *
 *********************/

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
 *   location: <https://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

#include "../config.h"

#if BOTH(TOUCH_UI_FTDI_EVE,HAS_LEVELING)

#include "screens.h"

#if BOTH(HAS_BED_PROBE,BLTOUCH)
  #include "../../../../../feature/bltouch.h"
#endif

using namespace FTDI;
using namespace ExtUI;
using namespace Theme;

#ifdef TOUCH_UI_PORTRAIT
  #define GRID_ROWS 10
  #define GRID_COLS 2
  #define TITLE_POS          BTN_POS(1,1), BTN_SIZE(2,1)
  #define LEVEL_BED_POS      BTN_POS(1,2), BTN_SIZE(2,1)
  #define LEVEL_AXIS_POS     BTN_POS(1,3), BTN_SIZE(2,1)
  #define Z_AUTO_ALIGN_POS   BTN_POS(1,4), BTN_SIZE(2,1)
  #define SHOW_MESH_POS      BTN_POS(1,5), BTN_SIZE(2,1)
  #define BLTOUCH_TITLE_POS  BTN_POS(1,7), BTN_SIZE(2,1)
  #define BLTOUCH_RESET_POS  BTN_POS(1,8), BTN_SIZE(1,1)
  #define BLTOUCH_TEST_POS   BTN_POS(2,8), BTN_SIZE(1,1)
  #define BACK_POS           BTN_POS(1,10), BTN_SIZE(2,1)
#else
  #define GRID_ROWS 8
  #define GRID_COLS 2
  #define TITLE_POS          BTN_POS(1,1), BTN_SIZE(2,1)
  #define LEVEL_BED_POS      BTN_POS(1,2), BTN_SIZE(2,1)
  #define LEVEL_AXIS_POS     BTN_POS(1,3), BTN_SIZE(2,1)
  #define Z_AUTO_ALIGN_POS   BTN_POS(1,4), BTN_SIZE(2,1)
  #define SHOW_MESH_POS      BTN_POS(1,5), BTN_SIZE(2,1)
  #define BLTOUCH_TITLE_POS  BTN_POS(1,6), BTN_SIZE(2,1)
  #define BLTOUCH_RESET_POS  BTN_POS(1,7), BTN_SIZE(1,1)
  #define BLTOUCH_TEST_POS   BTN_POS(2,7), BTN_SIZE(1,1)
  #define BACK_POS           BTN_POS(1,8), BTN_SIZE(2,1)
#endif

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
       .text(TITLE_POS, GET_TEXT_F(MSG_LEVELING))
       .font(font_medium).colors(normal_btn)
       .tag(2).button(LEVEL_BED_POS, GET_TEXT_F(MSG_LEVEL_BED))
       .enabled(
         #ifdef AXIS_LEVELING_COMMANDS
           1
         #endif
        )
       .tag(3).button(LEVEL_AXIS_POS, GET_TEXT_F(MSG_AUTOLEVEL_X_AXIS))
       .enabled(ENABLED(Z_STEPPER_AUTO_ALIGN))
       .tag(4).button(Z_AUTO_ALIGN_POS, GET_TEXT_F(MSG_AUTO_Z_ALIGN))
       .enabled(ENABLED(HAS_MESH))
       .tag(5).button(SHOW_MESH_POS, GET_TEXT_F(MSG_SHOW_MESH));
    #if ENABLED(BLTOUCH)
      cmd.text(BLTOUCH_TITLE_POS, GET_TEXT_F(MSG_BLTOUCH))
         .tag(6).button(BLTOUCH_RESET_POS, GET_TEXT_F(MSG_BLTOUCH_RESET))
         .tag(7).button(BLTOUCH_TEST_POS,  GET_TEXT_F(MSG_BLTOUCH_SELFTEST));
    #endif
    cmd.colors(action_btn)
       .tag(1).button(BACK_POS, GET_TEXT_F(MSG_BACK));
  }
}

bool LevelingMenu::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case 1: GOTO_PREVIOUS();                   break;
    case 2:
    #if HAS_MESH
      BedMeshScreen::startMeshProbe();
    #else
      SpinnerDialogBox::enqueueAndWait_P(F(BED_LEVELING_COMMANDS));
    #endif
    break;
    #ifdef AXIS_LEVELING_COMMANDS
    case 3: SpinnerDialogBox::enqueueAndWait_P(F(AXIS_LEVELING_COMMANDS)); break;
    #endif
    #if ENABLED(Z_STEPPER_AUTO_ALIGN)
    case 4: SpinnerDialogBox::enqueueAndWait_P(F("G34")); break;
    #endif
    #if HAS_MESH
    case 5: GOTO_SCREEN(BedMeshScreen); break;
    #endif
    #if ENABLED(BLTOUCH)
    case 6: injectCommands_P(PSTR("M280 P0 S60")); break;
    case 7: SpinnerDialogBox::enqueueAndWait_P(F("M280 P0 S90\nG4 P100\nM280 P0 S120")); break;
    #endif
    default: return false;
  }
  return true;
}

#endif // BOTH(TOUCH_UI_FTDI_EVE,HAS_LEVELING)
