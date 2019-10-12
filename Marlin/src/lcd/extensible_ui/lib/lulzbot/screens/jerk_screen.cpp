/*******************
 * jerk_screen.cpp *
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

#if BOTH(LULZBOT_TOUCH_UI, CLASSIC_JERK)

#include "screens.h"

using namespace FTDI;
using namespace ExtUI;
using namespace Theme;

void JerkScreen::onRedraw(draw_mode_t what) {

  widgets_t w(what);
  w.precision(1);
  w.units(GET_TEXT_F(UNITS_MM_S));
  w.heading(GET_TEXT_F(JERK));
  w.color(x_axis) .adjuster( 2, GET_TEXT_F(AXIS_X), getAxisMaxJerk_mm_s(X) );
  w.color(y_axis) .adjuster( 4, GET_TEXT_F(AXIS_Y), getAxisMaxJerk_mm_s(Y) );
  w.color(z_axis) .adjuster( 6, GET_TEXT_F(AXIS_Z), getAxisMaxJerk_mm_s(Z) );
  w.color(e_axis) .adjuster( 8, GET_TEXT_F(AXIS_E), getAxisMaxJerk_mm_s(E0) );
  w.increments();
}

bool JerkScreen::onTouchHeld(uint8_t tag) {
  using namespace ExtUI;
  const float increment = getIncrement();
  switch (tag) {
    case  2: UI_DECREMENT(AxisMaxJerk_mm_s, X); break;
    case  3: UI_INCREMENT(AxisMaxJerk_mm_s, X); break;
    case  4: UI_DECREMENT(AxisMaxJerk_mm_s, Y); break;
    case  5: UI_INCREMENT(AxisMaxJerk_mm_s, Y); break;
    case  6: UI_DECREMENT(AxisMaxJerk_mm_s, Z); break;
    case  7: UI_INCREMENT(AxisMaxJerk_mm_s, Z); break;
    case  8: UI_DECREMENT(AxisMaxJerk_mm_s, E0); break;
    case  9: UI_INCREMENT(AxisMaxJerk_mm_s, E0); break;
    default:
      return false;
  }
  SaveSettingsDialogBox::settingsChanged();
  return true;
}

#endif // LULZBOT_TOUCH_UI && CLASSIC_JERK
