/*******************************
 * feedrate_percent_screen.cpp *
 *******************************/

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
using namespace ExtUI;

void FeedratePercentScreen::onRedraw(draw_mode_t what) {
  widgets_t w(what);
  w.precision(0).units(GET_TEXT_F(UNITS_PERCENT));

  w.heading(GET_TEXT_F(PRINT_SPEED));
  w.adjuster(4,  GET_TEXT_F(SPEED), getFeedrate_percent());
  w.increments();
}

bool FeedratePercentScreen::onTouchHeld(uint8_t tag) {
  const float increment = getIncrement();
  switch (tag) {
    case 4: UI_DECREMENT(Feedrate_percent); break;
    case 5: UI_INCREMENT(Feedrate_percent); break;
    default:
      return false;
  }
  return true;
}

#endif // LULZBOT_TOUCH_UI
