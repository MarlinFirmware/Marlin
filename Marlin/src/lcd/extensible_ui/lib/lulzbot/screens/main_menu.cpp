/*****************
 * main_menu.cpp *
 *****************/

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
 *   location: <http://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

#include "../config.h"

#if ENABLED(LULZBOT_TOUCH_UI) && !defined(TOUCH_UI_LULZBOT_BIO)

#include "screens.h"

using namespace FTDI;
using namespace Theme;

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
    #ifdef TOUCH_UI_PORTRAIT
      #define GRID_ROWS 8
      #define GRID_COLS 2
        .tag(2).button( BTN_POS(1,1), BTN_SIZE(1,1), GET_TEXT_F(MSG_AUTO_HOME))
        #if ENABLED(NOZZLE_CLEAN_FEATURE)
         .enabled(1)
        #else
         .enabled(0)
        #endif
        .tag(3).button( BTN_POS(2,1), BTN_SIZE(1,1), GET_TEXT_F(MSG_CLEAN_NOZZLE))
        .tag(4).button( BTN_POS(1,2), BTN_SIZE(1,1), GET_TEXT_F(MSG_MOVE_AXIS))
        .tag(5).button( BTN_POS(2,2), BTN_SIZE(1,1), GET_TEXT_F(MSG_DISABLE_STEPPERS))
        .tag(6).button( BTN_POS(1,3), BTN_SIZE(2,1), GET_TEXT_F(MSG_TEMPERATURE))
        #if DISABLED(TOUCH_UI_LULZBOT_BIO) && DISABLED(TOUCH_UI_COCOA_PRESS)
         .enabled(1)
        #else
         .enabled(0)
        #endif
        .tag(7).button( BTN_POS(1,4), BTN_SIZE(2,1), GET_TEXT_F(MSG_FILAMENTCHANGE))
        .tag(8).button( BTN_POS(1,5), BTN_SIZE(2,1), GET_TEXT_F(MSG_ADVANCED_SETTINGS))
        #ifdef PRINTCOUNTER
         .enabled(1)
        #else
         .enabled(0)
        #endif
        .tag(9).button( BTN_POS(1,7), BTN_SIZE(2,1), GET_TEXT_F(MSG_INFO_STATS_MENU))
        .tag(10).button( BTN_POS(1,6), BTN_SIZE(2,1), GET_TEXT_F(MSG_INFO_MENU))
        .colors(action_btn)
        .tag(1).button( BTN_POS(1,8), BTN_SIZE(2,1), GET_TEXT_F(MSG_BACK));
      #undef GRID_COLS
      #undef GRID_ROWS
    #else
      #define GRID_ROWS 5
      #define GRID_COLS 2
        .tag(2).button( BTN_POS(1,1), BTN_SIZE(1,1), GET_TEXT_F(MSG_AUTO_HOME))
        #if ENABLED(NOZZLE_CLEAN_FEATURE)
         .enabled(1)
        #else
         .enabled(0)
        #endif
        .tag(3).button( BTN_POS(2,1), BTN_SIZE(1,1), GET_TEXT_F(MSG_CLEAN_NOZZLE))
        .tag(4).button( BTN_POS(1,2), BTN_SIZE(1,1), GET_TEXT_F(MSG_MOVE_AXIS))
        .tag(5).button( BTN_POS(2,2), BTN_SIZE(1,1), GET_TEXT_F(MSG_DISABLE_STEPPERS))
        .tag(6).button( BTN_POS(1,3), BTN_SIZE(1,1), GET_TEXT_F(MSG_TEMPERATURE))
        #if DISABLED(TOUCH_UI_LULZBOT_BIO) && DISABLED(TOUCH_UI_COCOA_PRESS)
         .enabled(1)
        #else
         .enabled(0)
        #endif
        .tag(7).button( BTN_POS(2,3), BTN_SIZE(1,1), GET_TEXT_F(MSG_FILAMENTCHANGE))
        .tag(8).button( BTN_POS(1,4), BTN_SIZE(1,1), GET_TEXT_F(MSG_ADVANCED_SETTINGS))
        #ifdef PRINTCOUNTER
         .enabled(1)
        #else
         .enabled(0)
        #endif
        .tag(9).button( BTN_POS(2,4), BTN_SIZE(1,1), GET_TEXT_F(MSG_INFO_STATS_MENU))
        .tag(10).button( BTN_POS(1,5), BTN_SIZE(1,1), GET_TEXT_F(MSG_INFO_MENU))
        .colors(action_btn)
        .tag(1).button( BTN_POS(2,5), BTN_SIZE(1,1), GET_TEXT_F(MSG_BACK));
      #undef GRID_COLS
      #undef GRID_ROWS
    #endif
  }
}

bool MainMenu::onTouchEnd(uint8_t tag) {
  using namespace ExtUI;

  switch (tag) {
    case 1:  GOTO_PREVIOUS();                                         break;
    case 2:  SpinnerDialogBox::enqueueAndWait_P(F("G28"));            break;
    #if ENABLED(NOZZLE_CLEAN_FEATURE)
    case 3:  injectCommands_P(PSTR("G12")); GOTO_SCREEN(StatusScreen); break;
    #endif
    case 4:  GOTO_SCREEN(MoveAxisScreen);                             break;
    case 5:  injectCommands_P(PSTR("M84"));                           break;
    case 6:  GOTO_SCREEN(TemperatureScreen);                          break;
    case 7:  GOTO_SCREEN(ChangeFilamentScreen);                       break;
    case 8:  GOTO_SCREEN(AdvancedSettingsMenu);                       break;
#if ENABLED(PRINTCOUNTER)
    case 9:  GOTO_SCREEN(StatisticsScreen);                           break;
#endif
    case 10: GOTO_SCREEN(AboutScreen);                                break;
    default:
      return false;
  }
  return true;
}

#endif // LULZBOT_TOUCH_UI
