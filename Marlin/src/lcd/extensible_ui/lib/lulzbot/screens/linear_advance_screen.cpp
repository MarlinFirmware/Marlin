/*****************************
 * linear_advance_screen.cpp *
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

#if BOTH(LULZBOT_TOUCH_UI, LIN_ADVANCE)

#include "screens.h"

using namespace FTDI;
using namespace ExtUI;
using namespace Theme;

void LinearAdvanceScreen::onRedraw(draw_mode_t what) {
  widgets_t w(what);
  w.precision(2, DEFAULT_LOWEST).color(e_axis);
  w.heading(           PSTR("Linear Advance:"));
  #if EXTRUDERS == 1
    w.adjuster(     2, PSTR("K:"),    getLinearAdvance_mm_mm_s(E0) );
  #else
    w.adjuster(     2, PSTR("K E1:"), getLinearAdvance_mm_mm_s(E0) );
    w.adjuster(     4, PSTR("K E2:"), getLinearAdvance_mm_mm_s(E1) );
    #if EXTRUDERS > 2
      w.adjuster(   6, PSTR("K E3:"), getLinearAdvance_mm_mm_s(E2) );
      #if EXTRUDERS > 3
        w.adjuster( 8, PSTR("K E4:"), getLinearAdvance_mm_mm_s(E3) );
      #endif
    #endif
  #endif
  w.increments();
}

bool LinearAdvanceScreen::onTouchHeld(uint8_t tag) {
  using namespace ExtUI;
  const float increment = getIncrement();
  switch (tag) {
    case  2: UI_DECREMENT(LinearAdvance_mm_mm_s, E0); break;
    case  3: UI_INCREMENT(LinearAdvance_mm_mm_s, E0); break;
    #if EXTRUDERS > 1
      case  4: UI_DECREMENT(LinearAdvance_mm_mm_s, E1);  break;
      case  5: UI_INCREMENT(LinearAdvance_mm_mm_s, E1); break;
      #if EXTRUDERS > 2
        case  6: UI_DECREMENT(LinearAdvance_mm_mm_s, E2);  break;
        case  7: UI_INCREMENT(LinearAdvance_mm_mm_s, E2);  break;
        #if EXTRUDERS > 3
          case  8: UI_DECREMENT(LinearAdvance_mm_mm_s, E3);  break;
          case  9: UI_INCREMENT(LinearAdvance_mm_mm_s, E3);  break;
        #endif
      #endif
    #endif
    default:
      return false;
  }
  SaveSettingsDialogBox::settingsChanged();
  return true;
}

#endif // LULZBOT_TOUCH_UI
