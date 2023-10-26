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
 *   location: <https://www.gnu.org/licenses/>.                             *
 ****************************************************************************/

#include "../config.h"
#include "../screens.h"

#ifdef FTDI_MAIN_MENU

using namespace FTDI;
using namespace Theme;

void MainMenu::onRedraw(draw_mode_t what) {
  if (what & BACKGROUND) {
    CommandProcessor cmd;
    cmd.cmd(CLEAR_COLOR_RGB(Theme::bg_color))
       .cmd(CLEAR(true,true,true));
  }

  #if ENABLED(TOUCH_UI_PORTRAIT)
    #define GRID_COLS 2
    #define GRID_ROWS 8
    #define ABOUT_PRINTER_POS     BTN_POS(1,1), BTN_SIZE(2,1)
    #define ADVANCED_SETTINGS_POS BTN_POS(1,2), BTN_SIZE(2,1)
    #if ENABLED(CUSTOM_MENU_MAIN)
      #define FILAMENTCHANGE_POS  BTN_POS(1,3), BTN_SIZE(1,1)
      #define CUSTOM_MENU_POS     BTN_POS(2,3), BTN_SIZE(1,1)
    #else
      #define FILAMENTCHANGE_POS  BTN_POS(1,3), BTN_SIZE(2,1)
    #endif
    #define TEMPERATURE_POS       BTN_POS(1,4), BTN_SIZE(2,1)
    #define DISABLE_STEPPERS_POS  BTN_POS(1,5), BTN_SIZE(2,1)
    #define MOVE_AXIS_POS         BTN_POS(1,6), BTN_SIZE(1,1)
    #define LEVELING_POS          BTN_POS(2,6), BTN_SIZE(1,1)
    #define AUTO_HOME_POS         BTN_POS(1,7), BTN_SIZE(1,1)
    #define CLEAN_NOZZLE_POS      BTN_POS(2,7), BTN_SIZE(1,1)
    #define BACK_POS              BTN_POS(1,8), BTN_SIZE(2,1)
  #else
    #define GRID_COLS 6
    #define GRID_ROWS 5
    #define ADVANCED_SETTINGS_POS BTN_POS(1,1), BTN_SIZE(3,1)
    #define ABOUT_PRINTER_POS     BTN_POS(4,1), BTN_SIZE(3,1)
    #define AUTO_HOME_POS         BTN_POS(1,2), BTN_SIZE(3,1)
    #define CLEAN_NOZZLE_POS      BTN_POS(4,2), BTN_SIZE(3,1)
    #define MOVE_AXIS_POS         BTN_POS(1,3), BTN_SIZE(3,1)
    #define DISABLE_STEPPERS_POS  BTN_POS(4,3), BTN_SIZE(3,1)
    #if ENABLED(CUSTOM_MENU_MAIN)
      #define TEMPERATURE_POS     BTN_POS(1,4), BTN_SIZE(2,1)
      #define FILAMENTCHANGE_POS  BTN_POS(3,4), BTN_SIZE(2,1)
      #define CUSTOM_MENU_POS     BTN_POS(5,4), BTN_SIZE(2,1)
    #else
      #define TEMPERATURE_POS     BTN_POS(1,4), BTN_SIZE(3,1)
      #define FILAMENTCHANGE_POS  BTN_POS(4,4), BTN_SIZE(3,1)
    #endif
    #define LEVELING_POS          BTN_POS(1,5), BTN_SIZE(3,1)
    #define BACK_POS              BTN_POS(4,5), BTN_SIZE(3,1)
  #endif

  if (what & FOREGROUND) {
    CommandProcessor cmd;
    cmd.colors(normal_btn)
       .font(Theme::font_medium)
       .tag( 2).button(AUTO_HOME_POS,       GET_TEXT_F(MSG_AUTO_HOME))
               .enabled(ENABLED(NOZZLE_CLEAN_FEATURE))
       .tag( 3).button(CLEAN_NOZZLE_POS,    GET_TEXT_F(MSG_CLEAN_NOZZLE))
       .tag( 4).button(MOVE_AXIS_POS,       GET_TEXT_F(MSG_MOVE_AXIS))
       .tag( 5).button(DISABLE_STEPPERS_POS,GET_TEXT_F(MSG_DISABLE_STEPPERS))
       .tag( 6).button(TEMPERATURE_POS,     GET_TEXT_F(MSG_TEMPERATURE))
               .enabled(DISABLED(TOUCH_UI_LULZBOT_BIO))
       .tag( 7).button(FILAMENTCHANGE_POS,  GET_TEXT_F(MSG_FILAMENTCHANGE))
       .tag( 8).button(ADVANCED_SETTINGS_POS, GET_TEXT_F(MSG_ADVANCED_SETTINGS))
               .enabled(ENABLED(HAS_LEVELING))
       .tag( 9).button(LEVELING_POS,        GET_TEXT_F(MSG_LEVELING))
       .tag(10).button(ABOUT_PRINTER_POS,   GET_TEXT_F(MSG_INFO_MENU))
       #if ENABLED(CUSTOM_MENU_MAIN)
        .tag(11).button(CUSTOM_MENU_POS,    GET_TEXT_F(MSG_CUSTOM_COMMANDS))
       #endif
       .colors(action_btn)
       .tag(1).button(BACK_POS,             GET_TEXT_F(MSG_BUTTON_DONE));
  }
}

bool MainMenu::onTouchEnd(uint8_t tag) {
  using namespace ExtUI;

  switch (tag) {
    case 1: SaveSettingsDialogBox::promptToSaveSettings(); break;
    case 2: SpinnerDialogBox::enqueueAndWait(F("G28")); break;
    #if ENABLED(NOZZLE_CLEAN_FEATURE)
      case 3:
        injectCommands(F("G12"));
        GOTO_SCREEN(StatusScreen);             break;
    #endif
    case 4: GOTO_SCREEN(MoveAxisScreen);       break;
    case 5:
      injectCommands(F("M84 E"
        TERN_(DISABLE_INACTIVE_X, " X")
        TERN_(DISABLE_INACTIVE_Y, " Y")
        TERN_(DISABLE_INACTIVE_Z, " Z")
      ));
      break;
    case 6: GOTO_SCREEN(TemperatureScreen);    break;
    case 7: GOTO_SCREEN(ChangeFilamentScreen); break;
    case 8: GOTO_SCREEN(AdvancedSettingsMenu); break;
    #if HAS_LEVELING
      case 9: GOTO_SCREEN(LevelingMenu);       break;
    #endif
    case 10: GOTO_SCREEN(AboutScreen);         break;
    #if ENABLED(CUSTOM_MENU_MAIN)
      case 11: GOTO_SCREEN(CustomUserMenus);   break;
    #endif

    default:
      return false;
  }
  return true;
}

#endif // FTDI_MAIN_MENU
