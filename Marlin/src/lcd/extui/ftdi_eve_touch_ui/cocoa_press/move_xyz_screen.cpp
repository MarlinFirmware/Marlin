/***********************************
 * cocoa_press/move_xyz_screen.cpp *
 ***********************************/

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
#include "../screens.h"
#include "../screen_data.h"

#ifdef COCOA_MOVE_XYZ_SCREEN

using namespace FTDI;
using namespace ExtUI;

void MoveXYZScreen::onRedraw(draw_mode_t what) {
  widgets_t w(what);
  w.precision(1);
  w.units(GET_TEXT_F(MSG_UNITS_MM));
  w.heading(                           GET_TEXT_F(MSG_XYZ_MOVE));
  w.home_buttons(20);
  w.color(Theme::x_axis).adjuster(  2, GET_TEXT_F(MSG_AXIS_X),  getAxisPosition_mm(X), canMove(X));
  w.color(Theme::y_axis).adjuster(  4, GET_TEXT_F(MSG_AXIS_Y),  getAxisPosition_mm(Y), canMove(Y));
  w.color(Theme::z_axis).adjuster(  6, GET_TEXT_F(MSG_AXIS_Z),  getAxisPosition_mm(Z), canMove(Z));
  w.increments();
}

void MoveXYZScreen::onIdle() {
  if (refresh_timer.elapsed(STATUS_UPDATE_INTERVAL)) {
    onRefresh();
    refresh_timer.start();
  }
  BaseScreen::onIdle();
}
#endif // COCOA_MOVE_XYZ_SCREEN
