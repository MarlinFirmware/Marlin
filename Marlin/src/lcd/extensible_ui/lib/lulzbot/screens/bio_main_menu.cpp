/*********************
 * bio_main_menu.cpp *
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
 *   location: <http://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

#include "../config.h"

#if ENABLED(LULZBOT_TOUCH_UI) && defined(LULZBOT_USE_BIOPRINTER_UI)

#include "screens.h"

using namespace FTDI;
using namespace Theme;

void MainMenu::onRedraw(draw_mode_t what) {
  #define GRID_ROWS 8
  #define GRID_COLS 2

  if (what & BACKGROUND) {
    CommandProcessor cmd;
    cmd.cmd(CLEAR_COLOR_RGB(Theme::bg_color))
       .cmd(CLEAR(true,true,true));
  }

  if (what & FOREGROUND) {
    CommandProcessor cmd;
    cmd.cmd(COLOR_RGB(bg_text_enabled))
       .font(font_large).text( BTN_POS(1,1), BTN_SIZE(2,1), F("Main Menu"))
       .colors(normal_btn)
       .font(font_medium)
       .tag(2).button( BTN_POS(1,2), BTN_SIZE(2,1), F("Load Syringe"))
       .tag(3).button( BTN_POS(1,3), BTN_SIZE(2,1), F("Unlock XY Axis"))
       .tag(4).button( BTN_POS(1,4), BTN_SIZE(2,1), F("Bed Temperature"))
       .tag(5).button( BTN_POS(1,5), BTN_SIZE(2,1), F("Interface Settings"))
       .tag(6).button( BTN_POS(1,6), BTN_SIZE(2,1), F("Advanced Settings"))
       .tag(7).button( BTN_POS(1,7), BTN_SIZE(2,1), F("About Printer"))
       .colors(action_btn)
       .tag(1).button( BTN_POS(1,8), BTN_SIZE(2,1), F("Back"));
  }

  #undef GRID_COLS
  #undef GRID_ROWS
}

bool MainMenu::onTouchEnd(uint8_t tag) {
  using namespace ExtUI;

  switch (tag) {
    case 1: SaveSettingsDialogBox::promptToSaveSettings();                               break;
    case 2: GOTO_SCREEN(BioConfirmHomeXYZ);                                              break;
    case 3: StatusScreen::unlockMotors();                                                break;
    case 4:  GOTO_SCREEN(TemperatureScreen);                                             break;
    case 5: GOTO_SCREEN(InterfaceSettingsScreen);                                        break;
    case 6: GOTO_SCREEN(AdvancedSettingsMenu);                                           break;
    case 7: GOTO_SCREEN(AboutScreen);                                                    break;
    default:
      return false;
  }
  return true;
}

#endif // LULZBOT_TOUCH_UI
