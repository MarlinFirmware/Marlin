/*****************************
 * cocoa_press/main_menu.cpp *
 *****************************/

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

#ifdef COCOA_MAIN_MENU

using namespace FTDI;
using namespace Theme;

#define GRID_ROWS 6
#define GRID_COLS 2

#define ZPROBE_ZOFFSET_POS    BTN_POS(1,1), BTN_SIZE(1,1)
#define MOVE_XYZ_POS          BTN_POS(1,2), BTN_SIZE(1,1)
#define TEMPERATURE_POS       BTN_POS(2,1), BTN_SIZE(1,1)
#define MOVE_E_POS            BTN_POS(2,2), BTN_SIZE(1,1)
#define SPEED_POS             BTN_POS(1,3), BTN_SIZE(1,1)
#define FLOW_POS              BTN_POS(2,3), BTN_SIZE(1,1)
#define ADVANCED_SETTINGS_POS BTN_POS(1,4), BTN_SIZE(1,1)
#define DISABLE_STEPPERS_POS  BTN_POS(2,4), BTN_SIZE(1,1)
#define LEVELING_POS          BTN_POS(1,5), BTN_SIZE(1,1)
#define ABOUT_PRINTER_POS     BTN_POS(2,5), BTN_SIZE(1,1)
#define BACK_POS              BTN_POS(1,6), BTN_SIZE(2,1)

void MainMenu::onRedraw(draw_mode_t what) {
  if (what & BACKGROUND) {
    CommandProcessor cmd;
    cmd.cmd(CLEAR_COLOR_RGB(Theme::bg_color))
       .cmd(CLEAR(true,true,true));
  }

  if (what & FOREGROUND) {
    CommandProcessor cmd;
    cmd.colors(normal_btn)
       .font(Theme::font_medium)
       .tag( 2).button(MOVE_XYZ_POS,          GET_TEXT_F(MSG_XYZ_MOVE))
       .tag( 3).button(TEMPERATURE_POS,       GET_TEXT_F(MSG_TEMPERATURE))
       .enabled(BOTH(HAS_LEVELING, HAS_BED_PROBE))
       .tag( 4).button(ZPROBE_ZOFFSET_POS,    GET_TEXT_F(MSG_ZPROBE_ZOFFSET))
       .tag( 5).button(MOVE_E_POS,            GET_TEXT_F(MSG_E_MOVE))
       .tag( 6).button(SPEED_POS,             GET_TEXT_F(MSG_PRINT_SPEED))
       .tag( 7).button(FLOW_POS,              GET_TEXT_F(MSG_FLOW))
       .tag( 8).button(ADVANCED_SETTINGS_POS, GET_TEXT_F(MSG_ADVANCED_SETTINGS))
       .tag( 9).button(DISABLE_STEPPERS_POS,  GET_TEXT_F(MSG_DISABLE_STEPPERS))
       .enabled(HAS_LEVELING)
       .tag(10).button(LEVELING_POS,          GET_TEXT_F(MSG_LEVELING))
       .tag(11).button(ABOUT_PRINTER_POS,     GET_TEXT_F(MSG_INFO_MENU))
       .colors(action_btn)
       .tag(1).button(BACK_POS,               GET_TEXT_F(MSG_BUTTON_DONE));
  }
}

bool MainMenu::onTouchEnd(uint8_t tag) {
  using namespace ExtUI;

  switch (tag) {
    case  1: SaveSettingsDialogBox::promptToSaveSettings(); break;
    case  2: GOTO_SCREEN(MoveXYZScreen);                    break;
    case  3: GOTO_SCREEN(TemperatureScreen);                break;
    #if BOTH(HAS_LEVELING, HAS_BED_PROBE)
    case  4: GOTO_SCREEN(ZOffsetScreen);                    break;
    #endif
    case  5: GOTO_SCREEN(MoveEScreen);                      break;
    case  6: GOTO_SCREEN(FeedratePercentScreen);            break;
    case  7: GOTO_SCREEN(FlowPercentScreen);                break;
    case  8: GOTO_SCREEN(AdvancedSettingsMenu);             break;
    case  9: injectCommands(F("M84"));                 break;
    #if HAS_LEVELING
    case 10:  GOTO_SCREEN(LevelingMenu);                    break;
    #endif
    case 11: GOTO_SCREEN(AboutScreen);                      break;
    default:
      return false;
  }
  return true;
}

#endif // COCOA_MAIN_MENU
