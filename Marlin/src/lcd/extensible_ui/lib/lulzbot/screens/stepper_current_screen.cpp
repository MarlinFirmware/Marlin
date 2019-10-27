/******************************
 * stepper_current_screen.cpp *
 ******************************/

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

#if ENABLED(LULZBOT_TOUCH_UI) && HAS_TRINAMIC

#include "screens.h"

using namespace FTDI;
using namespace ExtUI;
using namespace Theme;

void StepperCurrentScreen::onRedraw(draw_mode_t what) {
  widgets_t w(what);
  w.precision(0);
  w.units(GET_TEXT_F(UNITS_MILLIAMP));
  w.heading(                     GET_TEXT_F(MOTOR_CURRENT));
  w.color(x_axis)  .adjuster( 2, GET_TEXT_F(AXIS_X),  getAxisCurrent_mA(X) );
  w.color(y_axis)  .adjuster( 4, GET_TEXT_F(AXIS_Y),  getAxisCurrent_mA(Y) );
  w.color(z_axis)  .adjuster( 6, GET_TEXT_F(AXIS_Z),  getAxisCurrent_mA(Z) );
  #if EXTRUDERS == 1
    w.color(e_axis).adjuster( 8, GET_TEXT_F(AXIS_E),  getAxisCurrent_mA(E0) );
  #elif EXTRUDERS > 1
    w.color(e_axis).adjuster( 8, GET_TEXT_F(AXIS_E1), getAxisCurrent_mA(E0) );
    w.color(e_axis).adjuster(10, GET_TEXT_F(AXIS_E2), getAxisCurrent_mA(E1) );
    #if EXTRUDERS > 2
    w.color(e_axis).adjuster(12, GET_TEXT_F(AXIS_E3), getAxisCurrent_mA(E2) );
    #endif
    #if EXTRUDERS > 3
    w.color(e_axis).adjuster(14, GET_TEXT_F(AXIS_E4), getAxisCurrent_mA(E3) );
    #endif
  #endif
  w.increments();
}

bool StepperCurrentScreen::onTouchHeld(uint8_t tag) {
  const float increment = getIncrement();
  switch (tag) {
    case  2: UI_DECREMENT(AxisCurrent_mA, X ); break;
    case  3: UI_INCREMENT(AxisCurrent_mA, X ); break;
    case  4: UI_DECREMENT(AxisCurrent_mA, Y ); break;
    case  5: UI_INCREMENT(AxisCurrent_mA, Y ); break;
    case  6: UI_DECREMENT(AxisCurrent_mA, Z ); break;
    case  7: UI_INCREMENT(AxisCurrent_mA, Z ); break;
    case  8: UI_DECREMENT(AxisCurrent_mA, E0); break;
    case  9: UI_INCREMENT(AxisCurrent_mA, E0); break;
    #if EXTRUDERS > 1
    case 10: UI_DECREMENT(AxisCurrent_mA, E1); break;
    case 11: UI_INCREMENT(AxisCurrent_mA, E1); break;
    #endif
    #if EXTRUDERS > 2
    case 12: UI_DECREMENT(AxisCurrent_mA, E2); break;
    case 13: UI_INCREMENT(AxisCurrent_mA, E2); break;
    #endif
    #if EXTRUDERS > 3
    case 14: UI_DECREMENT(AxisCurrent_mA, E3); break;
    case 15: UI_INCREMENT(AxisCurrent_mA, E3); break;
    #endif
    default:
      return false;
  }
  SaveSettingsDialogBox::settingsChanged();
  return true;
}

#endif // LULZBOT_TOUCH_UI
