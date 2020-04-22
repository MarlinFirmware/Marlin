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

#if BOTH(TOUCH_UI_FTDI_EVE, BABYSTEPPING)

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
  screen_data.NudgeNozzleScreen.rel.reset();

  BaseNumericAdjustmentScreen::onEntry();
}

void NudgeNozzleScreen::onRedraw(draw_mode_t what) {
  widgets_t w(what);
  w.precision(2, BaseNumericAdjustmentScreen::DEFAULT_MIDRANGE).units(GET_TEXT_F(MSG_UNITS_MM));

  w.heading(GET_TEXT_F(MSG_NUDGE_NOZZLE));
  #if ENABLED(BABYSTEP_XY)
  w.color(x_axis).adjuster(2, GET_TEXT_F(MSG_AXIS_X), screen_data.NudgeNozzleScreen.rel.x / getAxisSteps_per_mm(X));
  w.color(y_axis).adjuster(4, GET_TEXT_F(MSG_AXIS_Y), screen_data.NudgeNozzleScreen.rel.y / getAxisSteps_per_mm(Y));
  #endif
  w.color(z_axis).adjuster(6, GET_TEXT_F(MSG_AXIS_Z), screen_data.NudgeNozzleScreen.rel.z / getAxisSteps_per_mm(Z));
  w.increments();
  #if EXTRUDERS > 1
    w.toggle(8, GET_TEXT_F(MSG_ADJUST_BOTH_NOZZLES), screen_data.NudgeNozzleScreen.link_nozzles);
  #endif

  #if EXTRUDERS > 1 || HAS_BED_PROBE
    w.toggle(9, GET_TEXT_F(MSG_SHOW_OFFSETS), screen_data.NudgeNozzleScreen.show_offsets);

    if (screen_data.NudgeNozzleScreen.show_offsets) {
      char str[19];

      w.draw_mode(BOTH);
      w.color(other);

      #if HAS_BED_PROBE
        dtostrf(getZOffset_mm(), 4, 2, str);
        strcat(str, " ");
        strcat_P(str, GET_TEXT(MSG_UNITS_MM));
        w.text_field(0, GET_TEXT_F(MSG_ZPROBE_ZOFFSET), str);
      #endif

      #if HAS_MULTI_HOTEND
        format_position(str, getNozzleOffset_mm(X, E1), getNozzleOffset_mm(Y, E1), getNozzleOffset_mm(Z, E1));
        w.text_field(0, GET_TEXT_F(MSG_OFFSETS_MENU), str);
      #endif
    }
  #endif
}

bool NudgeNozzleScreen::onTouchHeld(uint8_t tag) {
  const float inc = getIncrement();
  #if EXTRUDERS > 1
    const bool link = screen_data.NudgeNozzleScreen.link_nozzles;
  #else
    constexpr bool link = true;
  #endif
  int16_t steps;
  switch (tag) {
    case 2: steps = mmToWholeSteps(inc, X); smartAdjustAxis_steps(-steps, X, link); screen_data.NudgeNozzleScreen.rel.x -= steps; break;
    case 3: steps = mmToWholeSteps(inc, X); smartAdjustAxis_steps( steps, X, link); screen_data.NudgeNozzleScreen.rel.x += steps; break;
    case 4: steps = mmToWholeSteps(inc, Y); smartAdjustAxis_steps(-steps, Y, link); screen_data.NudgeNozzleScreen.rel.y -= steps; break;
    case 5: steps = mmToWholeSteps(inc, Y); smartAdjustAxis_steps( steps, Y, link); screen_data.NudgeNozzleScreen.rel.y += steps; break;
    case 6: steps = mmToWholeSteps(inc, Z); smartAdjustAxis_steps(-steps, Z, link); screen_data.NudgeNozzleScreen.rel.z -= steps; break;
    case 7: steps = mmToWholeSteps(inc, Z); smartAdjustAxis_steps( steps, Z, link); screen_data.NudgeNozzleScreen.rel.z += steps; break;
    #if EXTRUDERS > 1
      case 8: screen_data.NudgeNozzleScreen.link_nozzles = !link; break;
    #endif
    case 9: screen_data.NudgeNozzleScreen.show_offsets = !screen_data.NudgeNozzleScreen.show_offsets; break;
    default: return false;
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
  }
  else
    return BaseNumericAdjustmentScreen::onTouchEnd(tag);
}

void NudgeNozzleScreen::onIdle() {
  reset_menu_timeout();
}

#endif // TOUCH_UI_FTDI_EVE
