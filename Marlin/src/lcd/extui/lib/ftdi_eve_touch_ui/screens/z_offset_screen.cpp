/***********************
 * z_offset_screen.cpp *
 ***********************/

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

#if BOTH(TOUCH_UI_FTDI_EVE, HAS_BED_PROBE)

#include "screens.h"

using namespace FTDI;
using namespace ExtUI;
using namespace Theme;

void ZOffsetScreen::onRedraw(draw_mode_t what) {
  widgets_t w(what);
  w.precision(2, BaseNumericAdjustmentScreen::DEFAULT_MIDRANGE).units(GET_TEXT_F(MSG_UNITS_MM));

  w.heading(                  GET_TEXT_F(MSG_ZPROBE_ZOFFSET));
  w.color(z_axis).adjuster(4, GET_TEXT_F(MSG_ZPROBE_ZOFFSET), getZOffset_mm());
  w.increments();
}

bool ZOffsetScreen::onTouchHeld(uint8_t tag) {
  const float increment = getIncrement();
  switch (tag) {
    case 4: UI_DECREMENT(ZOffset_mm); break;
    case 5: UI_INCREMENT(ZOffset_mm); break;
    default:
      return false;
  }
  SaveSettingsDialogBox::settingsChanged();
  return true;
}

#endif // TOUCH_UI_FTDI_EVE && HAS_BED_PROBE
