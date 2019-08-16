/********************
 * about_screen.cpp *
 ********************/

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

#if ENABLED(LULZBOT_TOUCH_UI)

#include "screens.h"

#define GRID_COLS 4
#define GRID_ROWS 9

using namespace FTDI;
using namespace Theme;
using namespace ExtUI;

void AboutScreen::onEntry() {
  BaseScreen::onEntry();
  sound.play(chimes, PLAY_ASYNCHRONOUS);
}

void AboutScreen::onRedraw(draw_mode_t) {
  CommandProcessor cmd;
  cmd.cmd(CLEAR_COLOR_RGB(bg_color))
     .cmd(CLEAR(true,true,true))
     .cmd(COLOR_RGB(bg_text_enabled))
     .tag(0);

  draw_text_box(cmd, BTN_POS(1,2), BTN_SIZE(4,1), F(
      #ifdef LULZBOT_LCD_MACHINE_NAME
      LULZBOT_LCD_MACHINE_NAME
      #else
      "Color Touch Panel"
      #endif
    ), OPT_CENTER, font_xlarge);

  cmd.tag(2);
  draw_text_box(cmd, BTN_POS(1,3), BTN_SIZE(4,3), F(
      #ifdef LULZBOT_LCD_TOOLHEAD_NAME
        "Firmware for toolhead:\n" LULZBOT_LCD_TOOLHEAD_NAME "\n\n"
      #endif
      "(C) 2019 Aleph Objects, Inc.\n\nwww.lulzbot.com"
  ), OPT_CENTER, font_medium);

  cmd.tag(0);
  draw_text_box(cmd, BTN_POS(1,6), BTN_SIZE(4,2), progmem_str(getFirmwareName_str()), OPT_CENTER, font_medium);

  cmd.font(font_medium).colors(action_btn).tag(1).button(BTN_POS(2,8), BTN_SIZE(2,1), F("Okay"));
}

bool AboutScreen::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case 1: GOTO_PREVIOUS();            return true;
#if ENABLED(DEVELOPER_SCREENS)
    case 2: GOTO_SCREEN(DeveloperMenu); return true;
#endif
    default:                            return false;
  }
}

#endif // LULZBOT_TOUCH_UI
