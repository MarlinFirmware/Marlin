/*******************************
 * max_acceleration_screen.cpp *
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
using namespace Theme;

void MaxAccelerationScreen::onRedraw(draw_mode_t what) {
  widgets_t w(what);
  w.precision(0);
  w.units(GET_TEXT_F(UNITS_MM_S2));
  w.heading(GET_TEXT_F(ACCELERATION));
  w.color(x_axis)  .adjuster( 2, GET_TEXT_F(AMAX_X),  getAxisMaxAcceleration_mm_s2(X) );
  w.color(y_axis)  .adjuster( 4, GET_TEXT_F(AMAX_Y),  getAxisMaxAcceleration_mm_s2(Y) );
  w.color(z_axis)  .adjuster( 6, GET_TEXT_F(AMAX_Z),  getAxisMaxAcceleration_mm_s2(Z) );
  #if EXTRUDERS == 1 || DISABLED(DISTINCT_E_FACTORS)
    w.color(e_axis).adjuster( 8, GET_TEXT_F(AMAX_E1), getAxisMaxAcceleration_mm_s2(E0) );
  #elif EXTRUDERS > 1
    w.color(e_axis).adjuster( 8, GET_TEXT_F(AMAX_E1), getAxisMaxAcceleration_mm_s2(E0) );
    w.color(e_axis).adjuster(10, GET_TEXT_F(AMAX_E2), getAxisMaxAcceleration_mm_s2(E1) );
    #if EXTRUDERS > 2
    w.color(e_axis).adjuster(12, GET_TEXT_F(AMAX_E3), getAxisMaxAcceleration_mm_s2(E2) );
    #endif
    #if EXTRUDERS > 3
    w.color(e_axis).adjuster(14, GET_TEXT_F(AMAX_E4), getAxisMaxAcceleration_mm_s2(E3) );
    #endif
  #endif
  w.increments();
}

bool MaxAccelerationScreen::onTouchHeld(uint8_t tag) {
  const float increment = getIncrement();
  switch (tag) {
    case  2: UI_DECREMENT(AxisMaxAcceleration_mm_s2, X ); break;
    case  3: UI_INCREMENT(AxisMaxAcceleration_mm_s2, X ); break;
    case  4: UI_DECREMENT(AxisMaxAcceleration_mm_s2, Y ); break;
    case  5: UI_INCREMENT(AxisMaxAcceleration_mm_s2, Y ); break;
    case  6: UI_DECREMENT(AxisMaxAcceleration_mm_s2, Z ); break;
    case  7: UI_INCREMENT(AxisMaxAcceleration_mm_s2, Z ); break;
    case  8: UI_DECREMENT(AxisMaxAcceleration_mm_s2, E0); break;
    case  9: UI_INCREMENT(AxisMaxAcceleration_mm_s2, E0); break;
    #if EXTRUDERS > 1 && ENABLED(DISTINCT_E_FACTORS)
    case 10: UI_DECREMENT(AxisMaxAcceleration_mm_s2, E1); break;
    case 11: UI_INCREMENT(AxisMaxAcceleration_mm_s2, E1); break;
    #endif
    #if EXTRUDERS > 2 && ENABLED(DISTINCT_E_FACTORS)
    case 12: UI_DECREMENT(AxisMaxAcceleration_mm_s2, E2); break;
    case 13: UI_INCREMENT(AxisMaxAcceleration_mm_s2, E2); break;
    #endif
    #if EXTRUDERS > 3 && ENABLED(DISTINCT_E_FACTORS)
    case 14: UI_DECREMENT(AxisMaxAcceleration_mm_s2, E3); break;
    case 15: UI_INCREMENT(AxisMaxAcceleration_mm_s2, E3); break;
    #endif
    default:
      return false;
  }
  return true;
}

#endif // LULZBOT_TOUCH_UI
