/*********************************
 * cocoa_press_move_e_screen.cpp *
 *********************************/

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

#if BOTH(TOUCH_UI_FTDI_EVE, TOUCH_UI_COCOA_PRESS)

#include "screens.h"
#include "screen_data.h"

using namespace FTDI;
using namespace ExtUI;

void MoveEScreen::onRedraw(draw_mode_t what) {
  widgets_t w(what);
  w.precision(1);
  w.units(GET_TEXT_F(MSG_UNITS_MM));
  w.heading(                           GET_TEXT_F(MSG_E_MOVE));
  w.color(Theme::e_axis);
  #if EXTRUDERS == 1
    w.adjuster(  8, GET_TEXT_F(MSG_AXIS_E),  screen_data.MoveAxisScreen.e_rel[0], canMove(E0));
  #elif HAS_MULTI_EXTRUDER
    w.adjuster(  8, GET_TEXT_F(MSG_AXIS_E1), screen_data.MoveAxisScreen.e_rel[0], canMove(E0));
    w.adjuster( 10, GET_TEXT_F(MSG_AXIS_E2), screen_data.MoveAxisScreen.e_rel[1], canMove(E1));
    #if EXTRUDERS > 2
      w.adjuster( 12, GET_TEXT_F(MSG_AXIS_E3), screen_data.MoveAxisScreen.e_rel[2], canMove(E2));
    #endif
    #if EXTRUDERS > 3
      w.adjuster( 14, GET_TEXT_F(MSG_AXIS_E4), screen_data.MoveAxisScreen.e_rel[3], canMove(E3));
    #endif
  #endif
  w.increments();
}

void MoveEScreen::onIdle() {
  if (refresh_timer.elapsed(STATUS_UPDATE_INTERVAL)) {
    onRefresh();
    refresh_timer.start();
  }
  BaseScreen::onIdle();
}
#endif // TOUCH_UI_FTDI_EVE
