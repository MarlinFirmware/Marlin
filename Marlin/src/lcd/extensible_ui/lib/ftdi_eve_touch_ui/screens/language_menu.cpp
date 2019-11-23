/*********************
 * language_menu.cpp *
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
#include "../language/language.h"

#if ENABLED(TOUCH_UI_FTDI_EVE) && NUM_LANGUAGES > 1

#include "screens.h"

using namespace FTDI;
using namespace Theme;

void LanguageMenu::onRedraw(draw_mode_t) {
  CommandProcessor cmd;
  cmd.cmd(CLEAR_COLOR_RGB(Theme::bg_color))
     .cmd(CLEAR(true,true,true))
     .colors(normal_btn)
     .font(Theme::font_medium);

  #define GRID_ROWS 8
  #define GRID_COLS 1

  cmd.tag(1).button(BTN_POS(1,1), BTN_SIZE(1,1), GET_LANGUAGE_NAME(1));
  cmd.tag(2).button(BTN_POS(1,2), BTN_SIZE(1,1), GET_LANGUAGE_NAME(2));
  #if NUM_LANGUAGES > 2
    cmd.tag(3).button(BTN_POS(1,3), BTN_SIZE(1,1), GET_LANGUAGE_NAME(3));
    #if NUM_LANGUAGES > 3
      cmd.tag(4).button(BTN_POS(1,4), BTN_SIZE(1,1), GET_LANGUAGE_NAME(4));
      #if NUM_LANGUAGES > 5
        cmd.tag(5).button(BTN_POS(1,5), BTN_SIZE(1,1), GET_LANGUAGE_NAME(5));
      #endif
    #endif
  #endif
}

bool LanguageMenu::onTouchEnd(uint8_t tag) {

  if (tag > 0 && tag <= NUM_LANGUAGES) {
    lang = tag - 1;
    GOTO_SCREEN(StatusScreen);
    return true;
  }
  return false;
}

#endif // TOUCH_UI_FTDI_EVE
