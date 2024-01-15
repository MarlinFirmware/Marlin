/********************************
 * cocoa_press/preheat_menu.cpp *
 ********************************/

/****************************************************************************
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
 *   location: <https://www.gnu.org/licenses/>.                             *
 ****************************************************************************/

#include "../config.h"
#include "../screens.h"

#ifdef COCOA_PREHEAT_MENU

using namespace FTDI;
using namespace ExtUI;
using namespace Theme;

#define GRID_COLS 2
#define GRID_ROWS 5

void PreheatMenu::onRedraw(draw_mode_t what) {
  if (what & BACKGROUND) {
    CommandProcessor cmd;
    cmd.cmd(CLEAR_COLOR_RGB(Theme::bg_color))
       .cmd(CLEAR(true,true,true))
       .cmd(COLOR_RGB(bg_text_enabled))
       .font(Theme::font_medium)
       .tag(0).text(  BTN_POS(1,1), BTN_SIZE(2,1), GET_TEXT_F(MSG_SELECT_CHOCOLATE_TYPE));
  }

  if (what & FOREGROUND) {
    CommandProcessor cmd;
    cmd.font(Theme::font_medium)
       .colors(normal_btn)
       .tag(2).button(BTN_POS(1,2), BTN_SIZE(2,1), F("Dark Chocolate"))
       .tag(3).button(BTN_POS(1,3), BTN_SIZE(2,1), F("Milk Chocolate"))
       .tag(4).button(BTN_POS(1,4), BTN_SIZE(2,1), F("White Chocolate"))
       .colors(action_btn)
       .tag(1).button(BTN_POS(1,5), BTN_SIZE(2,1), GET_TEXT_F(MSG_BUTTON_DONE));
  }
}

bool PreheatMenu::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case 1: GOTO_PREVIOUS();                   break;
    case 2:
      #ifdef COCOA_PRESS_PREHEAT_DARK_CHOCOLATE_SCRIPT
        injectCommands(F(COCOA_PRESS_PREHEAT_DARK_CHOCOLATE_SCRIPT));
      #endif
      GOTO_SCREEN(PreheatTimerScreen);
      break;
    case 3:
      #ifdef COCOA_PRESS_PREHEAT_MILK_CHOCOLATE_SCRIPT
        injectCommands(F(COCOA_PRESS_PREHEAT_MILK_CHOCOLATE_SCRIPT));
      #endif
      GOTO_SCREEN(PreheatTimerScreen);
      break;
    case 4:
      #ifdef COCOA_PRESS_PREHEAT_WHITE_CHOCOLATE_SCRIPT
        injectCommands(F(COCOA_PRESS_PREHEAT_WHITE_CHOCOLATE_SCRIPT));
      #endif
      GOTO_SCREEN(PreheatTimerScreen);
      break;
    default: return false;
  }
  return true;
}

#endif // COCOA_PREHEAT_MENU
