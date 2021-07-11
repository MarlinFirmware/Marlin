/***************************
 * flow_percent_screen.cpp *
 ***************************/

/****************************************************************************
 *   Written By Marcio Teixeira 2021 - Cocoa Press                          *
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
#include "../screens.h"

#ifdef FTDI_FLOW_PERCENT_SCREEN

using namespace FTDI;
using namespace ExtUI;

void FlowPercentScreen::onRedraw(draw_mode_t what) {
  widgets_t w(what);
  w.precision(0).units(GET_TEXT_F(MSG_UNITS_PERCENT));

  w.heading(GET_TEXT_F(MSG_FLOW));
  w.adjuster(4,  GET_TEXT_F(MSG_FLOW), getFlow_percent(E0));
  w.increments();
  w.button(   2, GET_TEXT_F(MSG_PROBE_WIZARD));
}

void ZOffsetScreen::move(float inc) {
  // We can't store state after the call to the AlertBox, so
  // check whether the current position equal mydata.z in order
  // to know whether the user started the wizard.
  if (getAxisPosition_mm(Z) == mydata.z) {
    mydata.z += inc;
    setAxisPosition_mm(mydata.z, Z);
  }
}

void ZOffsetScreen::runWizard() {
  // Restore the default Z offset
  constexpr float offset[] = NOZZLE_TO_PROBE_OFFSET;
  setZOffset_mm(offset[Z_AXIS]);
  // Move above probe point
  char cmd[64], str[10];
  strcpy_P(cmd, PSTR("G28 Z\nG0 F1000 X"));
  dtostrf(TERN(Z_SAFE_HOMING,Z_SAFE_HOMING_X_POINT,X_CENTER), 3, 1, str);
  strcat(cmd, str);
  strcat_P(cmd, PSTR("Y"));
  dtostrf(TERN(Z_SAFE_HOMING,Z_SAFE_HOMING_Y_POINT,Y_CENTER), 3, 1, str);
  strcat(cmd, str);
  strcat_P(cmd, PSTR("Z"));
  dtostrf(SHEET_THICKNESS, 3, 1, str);
  strcat(cmd, str);
  injectCommands(cmd);
  // Show instructions for user.
  AlertDialogBox::show(PSTR("After the printer finishes homing, adjust the Z Offset so that a sheet of paper can pass between the nozzle and bed with slight resistance."));
}

bool FlowPercentScreen::onTouchHeld(uint8_t tag) {
  const float increment = getIncrement();
  switch (tag) {
    case 4: UI_DECREMENT(Flow_percent, E0); break;
    case 5: UI_INCREMENT(Flow_percent, E0); break;
    default:
      return false;
  }
  return true;
}

#endif // FTDI_FLOW_PERCENT_SCREEN
