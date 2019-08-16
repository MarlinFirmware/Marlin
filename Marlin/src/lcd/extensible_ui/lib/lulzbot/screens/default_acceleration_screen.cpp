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
 *   location: <http://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

#include "../config.h"

#if ENABLED(LULZBOT_TOUCH_UI)

#include "screens.h"

using namespace FTDI;
using namespace ExtUI;
using namespace Theme;

void DefaultAccelerationScreen::onRedraw(draw_mode_t what) {
  widgets_t w(what);
  w.precision(0);
  w.units(PSTR("mm/s^2"));
  w.heading(      PSTR("Default Acceleration"));
  w.color(other);
  w.adjuster(  2, PSTR("Printing:"),   getPrintingAcceleration_mm_s2() );
  w.adjuster(  4, PSTR("Travel:"),     getTravelAcceleration_mm_s2() );
  w.adjuster(  6, PSTR("Retraction:"), getRetractAcceleration_mm_s2() );
  w.increments();
  w.button(    8, PSTR("Set Axis Maximum"));
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

#endif // LULZBOT_TOUCH_UI
