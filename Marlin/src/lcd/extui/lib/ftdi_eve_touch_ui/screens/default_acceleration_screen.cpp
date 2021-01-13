/***********************************
 * default_acceleration_screen.cpp *
 ***********************************/

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

#if ENABLED(TOUCH_UI_FTDI_EVE)

#include "screens.h"

using namespace FTDI;
using namespace ExtUI;
using namespace Theme;

void DefaultAccelerationScreen::onRedraw(draw_mode_t what) {
  widgets_t w(what);
  w.precision(0);
  w.units(GET_TEXT_F(MSG_UNITS_MM_S2));
  w.heading(      GET_TEXT_F(MSG_ACCELERATION));
  w.color(other);
  w.adjuster(  2, GET_TEXT_F(MSG_ACCEL_PRINTING), getPrintingAcceleration_mm_s2() );
  w.adjuster(  4, GET_TEXT_F(MSG_ACCEL_TRAVEL),   getTravelAcceleration_mm_s2() );
  w.adjuster(  6, GET_TEXT_F(MSG_ACCEL_RETRACT),  getRetractAcceleration_mm_s2() );
  w.increments();
  w.button(   8, GET_TEXT_F(MSG_SET_MAXIMUM));
}

bool DefaultAccelerationScreen::onTouchHeld(uint8_t tag) {
  const float increment = getIncrement();
  switch (tag) {
    case  2: UI_DECREMENT(PrintingAcceleration_mm_s2); break;
    case  3: UI_INCREMENT(PrintingAcceleration_mm_s2); break;
    case  4: UI_DECREMENT(TravelAcceleration_mm_s2);   break;
    case  5: UI_INCREMENT(TravelAcceleration_mm_s2);   break;
    case  6: UI_DECREMENT(RetractAcceleration_mm_s2);  break;
    case  7: UI_INCREMENT(RetractAcceleration_mm_s2);  break;
    case  8: GOTO_SCREEN(MaxAccelerationScreen);       break;
    default:
      return false;
  }
  SaveSettingsDialogBox::settingsChanged();
  return true;
}

#endif // TOUCH_UI_FTDI_EVE
