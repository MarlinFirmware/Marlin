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
 *   location: <https://www.gnu.org/licenses/>.                             *
 ****************************************************************************/

#include "../config.h"
#include "../screens.h"

#ifdef FTDI_KILL_SCREEN

using namespace FTDI;

#define GRID_COLS 4
#define GRID_ROWS 8

// The kill screen is an oddball that happens after Marlin has killed the events
// loop. So we only have a show() method rather than onRedraw(). The KillScreen
// should not be used as a model for other UI screens as it is an exception.

void KillScreen::show(const char *message) {
  CommandProcessor cmd;

  cmd.cmd(CMD_DLSTART)
     .cmd(CLEAR_COLOR_RGB(Theme::bg_color))
     .cmd(CLEAR(true,true,true))
     .tag(0);

  cmd.font(Theme::font_large)
     .cmd(COLOR_RGB(Theme::bg_text_enabled))
     .text(BTN_POS(1,2), BTN_SIZE(4,1), message)
     .text(BTN_POS(1,3), BTN_SIZE(4,1), GET_TEXT_F(MSG_HALTED))
     .text(BTN_POS(1,6), BTN_SIZE(4,1), GET_TEXT_F(MSG_PLEASE_RESET));

  cmd.cmd(DL::DL_DISPLAY)
     .cmd(CMD_SWAP)
     .execute();

  InterfaceSoundsScreen::playEventSound(InterfaceSoundsScreen::PRINTING_FAILED, PLAY_SYNCHRONOUS);
}

#endif // FTDI_KILL_SCREEN
