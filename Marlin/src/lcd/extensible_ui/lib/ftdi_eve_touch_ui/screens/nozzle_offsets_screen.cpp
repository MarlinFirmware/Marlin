/*****************************
 * nozzle_offsets_screen.cpp *
 *****************************/

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

#if ENABLED(TOUCH_UI_FTDI_EVE) && HOTENDS > 1

#include "screens.h"

using namespace FTDI;
using namespace ExtUI;

void NozzleOffsetScreen::onEntry() {
  // Since we don't allow the user to edit the offsets for E0,
  // make sure they are all zero.
  normalizeNozzleOffset(X);
  normalizeNozzleOffset(Y);
  normalizeNozzleOffset(Z);
}

void NozzleOffsetScreen::onRedraw(draw_mode_t what) {
  widgets_t w(what);
  w.precision(2).units(GET_TEXT_F(MSG_UNITS_MM));

  w.heading(                          GET_TEXT_F(MSG_OFFSETS_MENU));
  w.color(Theme::x_axis).adjuster(2,  GET_TEXT_F(MSG_AXIS_X), ExtUI::getNozzleOffset_mm(X, E1));
  w.color(Theme::y_axis).adjuster(4,  GET_TEXT_F(MSG_AXIS_Y), ExtUI::getNozzleOffset_mm(Y, E1));
  w.color(Theme::z_axis).adjuster(6,  GET_TEXT_F(MSG_AXIS_Z), ExtUI::getNozzleOffset_mm(Z, E1));
  #if ENABLED(CALIBRATION_GCODE)
  w.button(8, GET_TEXT_F(MSG_MEASURE_AUTOMATICALLY), !isPrinting());
  #endif
  w.increments();
}

bool NozzleOffsetScreen::onTouchHeld(uint8_t tag) {
  const float increment = getIncrement();
  switch (tag) {
    case  2: UI_DECREMENT(NozzleOffset_mm, X, E1); break;
    case  3: UI_INCREMENT(NozzleOffset_mm, X, E1); break;
    case  4: UI_DECREMENT(NozzleOffset_mm, Y, E1); break;
    case  5: UI_INCREMENT(NozzleOffset_mm, Y, E1); break;
    case  6: UI_DECREMENT(NozzleOffset_mm, Z, E1); break;
    case  7: UI_INCREMENT(NozzleOffset_mm, Z, E1); break;
    #if ENABLED(CALIBRATION_GCODE)
    case  8: GOTO_SCREEN(ConfirmAutoCalibrationDialogBox); return true;
    #endif
    default:
      return false;
  }
  SaveSettingsDialogBox::settingsChanged();
  return true;
}

#endif // TOUCH_UI_FTDI_EVE
