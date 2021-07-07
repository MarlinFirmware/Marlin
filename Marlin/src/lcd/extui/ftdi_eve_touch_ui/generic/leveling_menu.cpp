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
 *   location: <https://www.gnu.org/licenses/>.                             *
 ****************************************************************************/

#include "../config.h"
#include "../screens.h"

#ifdef FTDI_LEVELING_MENU

#if BOTH(HAS_BED_PROBE,BLTOUCH)
  #include "../../../../feature/bltouch.h"
#endif

using namespace FTDI;
using namespace ExtUI;
using namespace Theme;

#if ENABLED(TOUCH_UI_PORTRAIT)
  #define GRID_ROWS 8
  #define GRID_COLS 2
  #define LEVELING_TITLE_POS BTN_POS(1,1), BTN_SIZE(2,1)
  #define LEVEL_AXIS_POS     BTN_POS(1,2), BTN_SIZE(2,1)
  #define BED_MESH_TITLE_POS BTN_POS(1,3), BTN_SIZE(2,1)
  #define PROBE_BED_POS      BTN_POS(1,4), BTN_SIZE(1,1)
  #define TEST_MESH_POS      BTN_POS(2,4), BTN_SIZE(1,1)
  #define SHOW_MESH_POS      BTN_POS(1,5), BTN_SIZE(1,1)
  #define EDIT_MESH_POS      BTN_POS(2,5), BTN_SIZE(1,1)
  #define BLTOUCH_TITLE_POS  BTN_POS(1,6), BTN_SIZE(2,1)
  #define BLTOUCH_RESET_POS  BTN_POS(1,7), BTN_SIZE(1,1)
  #define BLTOUCH_TEST_POS   BTN_POS(2,7), BTN_SIZE(1,1)
  #define BACK_POS           BTN_POS(1,8), BTN_SIZE(2,1)
#else
  #define GRID_ROWS 6
  #define GRID_COLS 3
  #define LEVELING_TITLE_POS BTN_POS(1,1), BTN_SIZE(3,1)
  #define LEVEL_AXIS_POS     BTN_POS(1,2), BTN_SIZE(3,1)
  #define BED_MESH_TITLE_POS BTN_POS(1,3), BTN_SIZE(2,1)
  #define PROBE_BED_POS      BTN_POS(1,4), BTN_SIZE(1,1)
  #define TEST_MESH_POS      BTN_POS(2,4), BTN_SIZE(1,1)
  #define SHOW_MESH_POS      BTN_POS(1,5), BTN_SIZE(1,1)
  #define EDIT_MESH_POS      BTN_POS(2,5), BTN_SIZE(1,1)
  #define BLTOUCH_TITLE_POS  BTN_POS(3,3), BTN_SIZE(1,1)
  #define BLTOUCH_RESET_POS  BTN_POS(3,4), BTN_SIZE(1,1)
  #define BLTOUCH_TEST_POS   BTN_POS(3,5), BTN_SIZE(1,1)
  #define BACK_POS           BTN_POS(1,6), BTN_SIZE(3,1)
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
       .cmd(COLOR_RGB(bg_text_enabled))
       .text(LEVELING_TITLE_POS, GET_TEXT_F(MSG_AXIS_LEVELING))
       .text(BED_MESH_TITLE_POS, GET_TEXT_F(MSG_BED_LEVELING))
    #if ENABLED(BLTOUCH)
       .text(BLTOUCH_TITLE_POS, GET_TEXT_F(MSG_BLTOUCH))
    #endif
       .font(font_medium).colors(normal_btn)
       .enabled(EITHER(Z_STEPPER_AUTO_ALIGN,MECHANICAL_GANTRY_CALIBRATION))
       .tag(2).button(LEVEL_AXIS_POS, GET_TEXT_F(MSG_LEVEL_X_AXIS))
       .enabled(ENABLED(HAS_BED_PROBE))
       .tag(3).button(PROBE_BED_POS, GET_TEXT_F(MSG_PROBE_BED))
       .enabled(ENABLED(HAS_MESH))
       .tag(4).button(SHOW_MESH_POS, GET_TEXT_F(MSG_SHOW_MESH))
       .enabled(ENABLED(HAS_MESH))
       .tag(5).button(EDIT_MESH_POS, GET_TEXT_F(MSG_EDIT_MESH))
       .enabled(ENABLED(G26_MESH_VALIDATION))
       .tag(6).button(TEST_MESH_POS, GET_TEXT_F(MSG_PRINT_TEST))
    #if ENABLED(BLTOUCH)
       .tag(7).button(BLTOUCH_RESET_POS, GET_TEXT_F(MSG_BLTOUCH_RESET))
       .tag(8).button(BLTOUCH_TEST_POS,  GET_TEXT_F(MSG_BLTOUCH_SELFTEST))
    #endif
       .colors(action_btn)
       .tag(1).button(BACK_POS, GET_TEXT_F(MSG_BACK));
  }
}

bool LevelingMenu::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case 1: GOTO_PREVIOUS(); break;
    #if EITHER(Z_STEPPER_AUTO_ALIGN,MECHANICAL_GANTRY_CALIBRATION)
      case 2: SpinnerDialogBox::enqueueAndWait_P(F("G34")); break;
    #endif
    #if ENABLED(HAS_BED_PROBE)
      case 3:
        #ifndef BED_LEVELING_COMMANDS
          #define BED_LEVELING_COMMANDS "G29"
        #endif
        #if ENABLED(AUTO_BED_LEVELING_UBL)
          BedMeshViewScreen::doProbe();
        #else
          SpinnerDialogBox::enqueueAndWait_P(F(BED_LEVELING_COMMANDS));
        #endif
        break;
    #endif
    #if ENABLED(AUTO_BED_LEVELING_UBL)
      case 4: BedMeshViewScreen::show(); break;
      case 5: BedMeshEditScreen::show(); break;
    #endif
    #if ENABLED(G26_MESH_VALIDATION)
      case 6: BedMeshViewScreen::doMeshValidation(); break;
    #endif
    #if ENABLED(BLTOUCH)
      case 7: injectCommands_P(PSTR("M280 P0 S60")); break;
      case 8: SpinnerDialogBox::enqueueAndWait_P(F("M280 P0 S90\nG4 P100\nM280 P0 S120")); break;
    #endif
    default: return false;
  }
  return true;
}

#endif // FTDI_LEVELING_MENU
