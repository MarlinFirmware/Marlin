/***************************
 * max_velocity_screen.cpp *
 ***************************/

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

void MaxVelocityScreen::onRedraw(draw_mode_t what) {
  using namespace ExtUI;
  widgets_t w(what);
  w.precision(0);
  w.units(PSTR("mm/s"));
  w.heading(                        PSTR("Maximum Velocity"));
  w.color(x_axis)    .adjuster(  2, PSTR("X:"),  getAxisMaxFeedrate_mm_s(X) );
  w.color(y_axis)    .adjuster(  4, PSTR("Y:"),  getAxisMaxFeedrate_mm_s(Y) );
  w.color(z_axis)    .adjuster(  6, PSTR("Z:"),  getAxisMaxFeedrate_mm_s(Z) );
  #if EXTRUDERS == 1 || DISABLED(DISTINCT_E_FACTORS)
    w.color(e_axis)  .adjuster(  8, PSTR("E:"),  getAxisMaxFeedrate_mm_s(E0) );
  #elif EXTRUDERS > 1
    w.color(e_axis)  .adjuster(  8, PSTR("E1:"), getAxisMaxFeedrate_mm_s(E0) );
    w.color(e_axis)  .adjuster( 10, PSTR("E2:"), getAxisMaxFeedrate_mm_s(E1) );
    #if EXTRUDERS > 2
      w.color(e_axis).adjuster( 12, PSTR("E3:"), getAxisMaxFeedrate_mm_s(E2) );
    #endif
    #if EXTRUDERS > 3
      w.color(e_axis).adjuster( 14, PSTR("E4:"), getAxisMaxFeedrate_mm_s(E3) );
    #endif
  #endif
  w.increments();
}

bool MaxVelocityScreen::onTouchHeld(uint8_t tag) {
  const float increment = getIncrement();
  switch (tag) {
    case  2: UI_DECREMENT(AxisMaxFeedrate_mm_s, X); break;
    case  3: UI_INCREMENT(AxisMaxFeedrate_mm_s, X); break;
    case  4: UI_DECREMENT(AxisMaxFeedrate_mm_s, Y); break;
    case  5: UI_INCREMENT(AxisMaxFeedrate_mm_s, Y); break;
    case  6: UI_DECREMENT(AxisMaxFeedrate_mm_s, Z); break;
    case  7: UI_INCREMENT(AxisMaxFeedrate_mm_s, Z); break;
    case  8: UI_DECREMENT(AxisMaxFeedrate_mm_s, E0); break;
    case  9: UI_INCREMENT(AxisMaxFeedrate_mm_s, E0); break;
    #if EXTRUDERS > 1 && ENABLED(DISTINCT_E_FACTORS)
    case 10: UI_DECREMENT(AxisMaxFeedrate_mm_s, E1); break;
    case 11: UI_INCREMENT(AxisMaxFeedrate_mm_s, E1); break;
    #endif
    #if EXTRUDERS > 2 && ENABLED(DISTINCT_E_FACTORS)
    case 12: UI_DECREMENT(AxisMaxFeedrate_mm_s, E2); break;
    case 13: UI_INCREMENT(AxisMaxFeedrate_mm_s, E2); break;
    #endif
    #if EXTRUDERS > 3 && ENABLED(DISTINCT_E_FACTORS)
    case 14: UI_DECREMENT(AxisMaxFeedrate_mm_s, E3); break;
    case 15: UI_INCREMENT(AxisMaxFeedrate_mm_s, E3); break;
    #endif
    default:
      return false;
  }
  SaveSettingsDialogBox::settingsChanged();
  return true;
}

#endif // LULZBOT_TOUCH_UI
