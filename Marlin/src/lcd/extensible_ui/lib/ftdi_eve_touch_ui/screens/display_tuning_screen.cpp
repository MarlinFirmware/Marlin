/*****************************
 * display_tuning_screen.cpp *
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

#if ENABLED(LULZBOT_TOUCH_UI)

#include "screens.h"

using namespace FTDI;
using namespace Theme;

void DisplayTuningScreen::onRedraw(draw_mode_t what) {
  widgets_t w(what);
  w.precision(0, BaseNumericAdjustmentScreen::DEFAULT_LOWEST);
  w.units(F(""));
  w.heading(GET_TEXT_F(MSG_DISPLAY_MENU));
  w.color(other);
  w.adjuster( 2, GET_TEXT_F(MSG_H_OFFSET), CLCD::mem_read_16(CLCD::REG::HOFFSET) );
  w.adjuster( 4, GET_TEXT_F(MSG_V_OFFSET), CLCD::mem_read_16(CLCD::REG::VOFFSET) );
  w.increments();
  w.heading(     GET_TEXT_F(MSG_TOUCH_SCREEN));
  w.button(6,    GET_TEXT_F(MSG_CALIBRATE));
}

bool DisplayTuningScreen::onTouchHeld(uint8_t tag) {
  #define REG_INCREMENT(a,i) CLCD::mem_write_16(CLCD::REG::a, CLCD::mem_read_16(CLCD::REG::a) + i)
  const float increment = getIncrement();
  switch (tag) {
    case  2: REG_INCREMENT(HOFFSET, -increment);  break;
    case  3: REG_INCREMENT(HOFFSET,  increment);  break;
    case  4: REG_INCREMENT(VOFFSET, -increment);  break;
    case  5: REG_INCREMENT(VOFFSET,  increment);  break;
    case  6: GOTO_SCREEN(TouchCalibrationScreen); break;
    default:
      return false;
  }
  SaveSettingsDialogBox::settingsChanged();
  return true;
}

#endif // LULZBOT_TOUCH_UI
