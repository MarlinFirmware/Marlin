/********************
 * nudge_nozzle.cpp *
 ********************/

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

#if BOTH(LULZBOT_TOUCH_UI, BABYSTEPPING)

#include "screens.h"
#include "screen_data.h"

using namespace FTDI;
using namespace Theme;
using namespace ExtUI;

void NudgeNozzleScreen::onEntry() {
  screen_data.NudgeNozzleScreen.show_offsets = false;
  #if EXTRUDERS > 1
    screen_data.NudgeNozzleScreen.link_nozzles = true;
  #endif
  LOOP_XYZ(i) {
    screen_data.NudgeNozzleScreen.rel[i] = 0;
  }
  BaseNumericAdjustmentScreen::onEntry();
}

void NudgeNozzleScreen::onRedraw(draw_mode_t what) {
  widgets_t w(what);
  w.precision(2, BaseNumericAdjustmentScreen::DEFAULT_MIDRANGE).units(PSTR("mm"));

  w.heading(                          PSTR("Nudge Nozzle"));
  #if ENABLED(BABYSTEP_XY)
  w.color(x_axis).adjuster(2,  PSTR("X:"), screen_data.NudgeNozzleScreen.rel[0] / getAxisSteps_per_mm(X));
  w.color(y_axis).adjuster(4,  PSTR("Y:"), screen_data.NudgeNozzleScreen.rel[1] / getAxisSteps_per_mm(Y));
  #endif
  w.color(z_axis).adjuster(6,  PSTR("Z:"), screen_data.NudgeNozzleScreen.rel[2] / getAxisSteps_per_mm(Z));
  w.increments();
  #if EXTRUDERS > 1
    w.toggle  (8,  PSTR("Adjust Both Nozzles:"), PSTR("no\xFFyes"), screen_data.NudgeNozzleScreen.link_nozzles, PSTR("Yes\nNo"));
  #endif

  #if EXTRUDERS > 1 || HAS_BED_PROBE
    w.toggle  (9,  PSTR("Show Offsets:"), PSTR("no\xFFyes"), screen_data.NudgeNozzleScreen.show_offsets, PSTR("Yes\nNo"));

    if (screen_data.NudgeNozzleScreen.show_offsets) {
      char str[19], num1[7];

      w.draw_mode(BOTH);
      w.color(other);

      #if HAS_BED_PROBE
        dtostrf(getZOffset_mm(), 4, 2, num1);
        sprintf_P(str, PSTR("%s mm"), num1);
        w.text_field  (0,  PSTR("Z Offset"), str);
      #endif

      #if EXTRUDERS > 1
        char num2[7], num3[7];
        dtostrf(getNozzleOffset_mm(X, E1), 4, 2, num1);
        dtostrf(getNozzleOffset_mm(Y, E1), 4, 2, num2);
        dtostrf(getNozzleOffset_mm(Z, E1), 4, 2, num3);
        sprintf_P(str, PSTR("%s; %s; %s mm"), num1, num2, num3);
        w.text_field  (0,  PSTR("Noz. Offset"), str);
      #endif
    }
  #endif
}

bool NudgeNozzleScreen::onTouchHeld(uint8_t tag) {
  const float inc  = getIncrement();
  #if EXTRUDERS > 1
    const bool  link = screen_data.NudgeNozzleScreen.link_nozzles;
  #else
    constexpr bool link = true;
  #endif
  int16_t steps;
  switch (tag) {
    case  2: steps = mmToWholeSteps(inc, X); smartAdjustAxis_steps(-steps, X, link); screen_data.NudgeNozzleScreen.rel[0] -= steps; break;
    case  3: steps = mmToWholeSteps(inc, X); smartAdjustAxis_steps( steps, X, link); screen_data.NudgeNozzleScreen.rel[0] += steps; break;
    case  4: steps = mmToWholeSteps(inc, Y); smartAdjustAxis_steps(-steps, Y, link); screen_data.NudgeNozzleScreen.rel[1] -= steps; break;
    case  5: steps = mmToWholeSteps(inc, Y); smartAdjustAxis_steps( steps, Y, link); screen_data.NudgeNozzleScreen.rel[1] += steps; break;
    case  6: steps = mmToWholeSteps(inc, Z); smartAdjustAxis_steps(-steps, Z, link); screen_data.NudgeNozzleScreen.rel[2] -= steps; break;
    case  7: steps = mmToWholeSteps(inc, Z); smartAdjustAxis_steps( steps, Z, link); screen_data.NudgeNozzleScreen.rel[2] += steps; break;
    #if EXTRUDERS > 1
      case  8: screen_data.NudgeNozzleScreen.link_nozzles = !link; break;
    #endif
    case  9: screen_data.NudgeNozzleScreen.show_offsets = !screen_data.NudgeNozzleScreen.show_offsets; break;
    default:
      return false;
  }
  #if EXTRUDERS > 1 || HAS_BED_PROBE
    SaveSettingsDialogBox::settingsChanged();
  #endif
  return true;
}

bool NudgeNozzleScreen::onTouchEnd(uint8_t tag) {
  if (tag == 1) {
    SaveSettingsDialogBox::promptToSaveSettings();
    return true;
  } else {
    return BaseNumericAdjustmentScreen::onTouchEnd(tag);
  }
}

void NudgeNozzleScreen::onIdle() {
  reset_menu_timeout();
}

#endif // LULZBOT_TOUCH_UI
