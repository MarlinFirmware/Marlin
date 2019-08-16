/*******************
 * kill_screen.cpp *
 *******************/

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

using namespace FTDI;

// The kill screen is an oddball that happens after Marlin has killed the events
// loop. So we only have a show() method rather than onRedraw(). The KillScreen
// should not be used as a model for other UI screens as it is an exception.

void KillScreen::show(progmem_str message) {
  CommandProcessor cmd;

  cmd.cmd(CMD_DLSTART)
     .cmd(CLEAR_COLOR_RGB(Theme::bg_color))
     .cmd(CLEAR(true,true,true))
     .tag(0);

  #define GRID_COLS 4
  #define GRID_ROWS 8

  cmd.font(Theme::font_large)
     .cmd(COLOR_RGB(Theme::bg_text_enabled))
     .text(BTN_POS(1,2), BTN_SIZE(4,1), message)
     .text(BTN_POS(1,3), BTN_SIZE(4,1), F("PRINTER HALTED"))
     .text(BTN_POS(1,6), BTN_SIZE(4,1), F("Please reset"));

  #undef GRID_COLS
  #undef GRID_ROWS

  cmd.cmd(DL::DL_DISPLAY)
     .cmd(CMD_SWAP)
     .execute();

  InterfaceSoundsScreen::playEventSound(InterfaceSoundsScreen::PRINTING_FAILED, PLAY_SYNCHRONOUS);
}

#endif // LULZBOT_TOUCH_UI
