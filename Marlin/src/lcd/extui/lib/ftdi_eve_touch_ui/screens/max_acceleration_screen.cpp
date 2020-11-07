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
 *   location: <https://www.gnu.org/licenses/>.                             *
 ****************************************************************************/

#include "../config.h"

#if ENABLED(TOUCH_UI_FTDI_EVE)

#include "screens.h"

using namespace FTDI;
using namespace ExtUI;
using namespace Theme;

void MaxAccelerationScreen::onRedraw(draw_mode_t what) {
  widgets_t w(what);
  w.precision(0);
  w.units(GET_TEXT_F(MSG_UNITS_MM_S2));
  w.heading(GET_TEXT_F(MSG_ACCELERATION));
  w.color(x_axis)  .adjuster( 2, GET_TEXT_F(MSG_AMAX_X),  getAxisMaxAcceleration_mm_s2(X) );
  w.color(y_axis)  .adjuster( 4, GET_TEXT_F(MSG_AMAX_Y),  getAxisMaxAcceleration_mm_s2(Y) );
  w.color(z_axis)  .adjuster( 6, GET_TEXT_F(MSG_AMAX_Z),  getAxisMaxAcceleration_mm_s2(Z) );
  #if DISTINCT_E == 1
    w.color(e_axis).adjuster( 8, GET_TEXT_F(MSG_AMAX_E), getAxisMaxAcceleration_mm_s2(E0) );
  #elif DISTINCT_E > 1
    w.heading(GET_TEXT_F(MSG_AMAX_E));
    w.color(e_axis).adjuster( 8, F(LCD_STR_E0), getAxisMaxAcceleration_mm_s2(E0) );
    w.color(e_axis).adjuster(10, F(LCD_STR_E1), getAxisMaxAcceleration_mm_s2(E1) );
    #if DISTINCT_E > 2
    w.color(e_axis).adjuster(12, F(LCD_STR_E2), getAxisMaxAcceleration_mm_s2(E2) );
    #endif
    #if DISTINCT_E > 3
    w.color(e_axis).adjuster(14, F(LCD_STR_E3), getAxisMaxAcceleration_mm_s2(E3) );
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
    #if DISTINCT_E > 1
    case 10: UI_DECREMENT(AxisMaxAcceleration_mm_s2, E1); break;
    case 11: UI_INCREMENT(AxisMaxAcceleration_mm_s2, E1); break;
    #endif
    #if DISTINCT_E > 2
    case 12: UI_DECREMENT(AxisMaxAcceleration_mm_s2, E2); break;
    case 13: UI_INCREMENT(AxisMaxAcceleration_mm_s2, E2); break;
    #endif
    #if DISTINCT_E > 3
    case 14: UI_DECREMENT(AxisMaxAcceleration_mm_s2, E3); break;
    case 15: UI_INCREMENT(AxisMaxAcceleration_mm_s2, E3); break;
    #endif
    default:
      return false;
  }
  return true;
}

#endif // TOUCH_UI_FTDI_EVE
