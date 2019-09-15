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

#if ENABLED(LULZBOT_TOUCH_UI) && defined(TOUCH_UI_LANGUAGE_MENU)

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

  for (uint8_t i = 0; i < get_language_count(); i++)
    cmd.tag(1 + i).button(BTN_POS(1,i + 1), BTN_SIZE(1,1), get_text(i, String_Indices::LANGUAGE));
}

bool LanguageMenu::onTouchEnd(uint8_t tag) {
  const uint8_t lang = tag - 1;
  if (tag != 0) {
    set_language(lang);
    GOTO_SCREEN(StatusScreen);
    return true;
  }
  return false;
}

#endif // LULZBOT_TOUCH_UI
