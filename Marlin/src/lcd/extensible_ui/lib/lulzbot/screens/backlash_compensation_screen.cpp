/************************************
 * backlash_compensation_screen.cpp *
 ************************************/

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

#if BOTH(LULZBOT_TOUCH_UI, BACKLASH_GCODE)

#include "screens.h"

using namespace FTDI;
using namespace ExtUI;
using namespace Theme;

void BacklashCompensationScreen::onRedraw(draw_mode_t what) {
  widgets_t w(what);
  w.precision(2).units(       GET_TEXT_F(MSG_UNITS_MM));
  w.heading(                  GET_TEXT_F(MSG_BACKLASH));
  w.color(x_axis).adjuster(2, GET_TEXT_F(MSG_AXIS_X), getAxisBacklash_mm(X));
  w.color(y_axis).adjuster(4, GET_TEXT_F(MSG_AXIS_Y), getAxisBacklash_mm(Y));
  w.color(z_axis).adjuster(6, GET_TEXT_F(MSG_AXIS_Z), getAxisBacklash_mm(Z));
  #if ENABLED(CALIBRATION_GCODE)
  w.button(12, GET_TEXT_F(MSG_MEASURE_AUTOMATICALLY));
  #endif
  w.color(other).adjuster(8,  GET_TEXT_F(MSG_SMOOTHING), getBacklashSmoothing_mm());
  w.precision(0).units(GET_TEXT_F(MSG_UNITS_PERCENT))
                .adjuster(10, GET_TEXT_F(MSG_CORRECTION), getBacklashCorrection_percent());
  w.precision(2).increments();
}

bool BacklashCompensationScreen::onTouchHeld(uint8_t tag) {
  const float increment = getIncrement();
  switch (tag) {
    case  2:  UI_DECREMENT(AxisBacklash_mm, X); break;
    case  3:  UI_INCREMENT(AxisBacklash_mm, X); break;
    case  4:  UI_DECREMENT(AxisBacklash_mm, Y); break;
    case  5:  UI_INCREMENT(AxisBacklash_mm, Y); break;
    case  6:  UI_DECREMENT(AxisBacklash_mm, Z); break;
    case  7:  UI_INCREMENT(AxisBacklash_mm, Z); break;
    case  8:  UI_DECREMENT(BacklashSmoothing_mm); break;
    case  9:  UI_INCREMENT(BacklashSmoothing_mm); break;
    case  10: UI_DECREMENT_BY(BacklashCorrection_percent, increment*100);  break;
    case  11: UI_INCREMENT_BY(BacklashCorrection_percent, increment*100);  break;
    #if ENABLED(CALIBRATION_GCODE)
    case  12: GOTO_SCREEN(ConfirmAutoCalibrationDialogBox); return true;
    #endif
    default:
      return false;
  }
  SaveSettingsDialogBox::settingsChanged();
  return true;
}

#endif // LULZBOT_TOUCH_UI
