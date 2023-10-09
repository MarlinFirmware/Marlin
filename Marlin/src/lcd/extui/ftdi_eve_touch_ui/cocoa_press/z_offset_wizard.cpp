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
 *   location: <https://www.gnu.org/licenses/>.                             *
 ****************************************************************************/

#include "../config.h"
#include "../screens.h"
#include "../screen_data.h"

#ifdef COCOA_Z_OFFSET_WIZARD

#include "cocoa_press_ui.h"

using namespace FTDI;
using namespace ExtUI;
using namespace Theme;

#define POLY(A) PolyUI::poly_reader_t(A, sizeof(A)/sizeof(A[0]))
#define SHEET_THICKNESS 0.1

constexpr static ZOffsetWizardData &mydata = screen_data.ZOffsetWizard;

void ZOffsetWizard::onEntry() {
  mydata.increment = 242;
  mydata.softEndstopState = getSoftEndstopState();
  BaseNumericAdjustmentScreen::onEntry();
  setSoftEndstopState(false);
}

void ZOffsetWizard::onExit() {
  setSoftEndstopState(mydata.softEndstopState);
}

void ZOffsetWizard::onRedraw(draw_mode_t what) {
  int16_t x, y, w, h;

  CommandProcessor cmd;
  PolyUI ui(cmd, what);

  cmd.cmd(CLEAR_COLOR_RGB(bg_color))
     .cmd(CLEAR(true,true,true))
     .tag(0)
     .font(font_medium).colors(normal_btn);

  char b[32];
  dtostrf(getZOffset_mm(), 5, 2, b);
  strcat_P(b, PSTR(" mm"));
  ui.bounds(POLY(z_wizard_edit_box), x, y, w, h);
  cmd.tag(0).fgcolor(z_axis).button(x, y, w, h, b);

  #define PREAMBLE(TAG) cmd.tag(TAG).colors(mydata.increment == TAG ? action_btn : normal_btn)
  ui.bounds(POLY(z_wizard_inc1_btn), x, y, w, h);
  PREAMBLE(241).button(x, y, w, h, F("0.01"));

  ui.bounds(POLY(z_wizard_inc2_btn), x, y, w, h);
  PREAMBLE(242).button(x, y, w, h, F("0.1"));

  ui.bounds(POLY(z_wizard_inc3_btn), x, y, w, h);
  PREAMBLE(243).button(x, y, w, h, F("1.0"));

  ui.bounds(POLY(z_wizard_neg_btn), x, y, w, h);
  cmd.tag(4).colors(action_btn).button(x, y, w, h, F(""));
  drawArrow(x, y, w, h, DOWN);

  ui.bounds(POLY(z_wizard_plus_btn), x, y, w, h);
  cmd.tag(5).colors(action_btn).button(x, y, w, h, F(""));
  drawArrow(x, y, w, h, UP);

  ui.bounds(POLY(z_wizard_done_btn), x, y, w, h);
  cmd.tag(1).colors(action_btn).button(x, y, w, h, GET_TEXT_F(MSG_BUTTON_DONE));

  cmd.tag(0);
  ui.color(bg_text_enabled);
  ui.fill(POLY(z_wizard_diagram));

  ui.bounds(POLY(z_wizard_heading), x, y, w, h);
  cmd.font(font_large)
     .text(x, y, w, h, F("Z Probe Wizard"));
}

float ZOffsetWizard::getIncrement() {
  switch (mydata.increment) {
    case 241: return 0.01;
    case 242: return 0.1;
    case 243: return 1.0;
    default:  return 0.0;
  }
}

void ZOffsetWizard::runWizard() {
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
  AlertDialogBox::show(F("\nOn the next screen, adjust the Z Offset so that a sheet of paper can pass between the nozzle and bed with slight resistance.\n\nOnce the printer stops moving, press Okay to begin.\n"));
  // Set the destination screen after the dialog box.
  current_screen.forget();
  PUSH_SCREEN(ZOffsetWizard);
}

bool ZOffsetWizard::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case 1:
      GOTO_PREVIOUS();
      break;
    case 4:
    case 5:
      return onTouchHeld(tag);
    case 241 ... 243:
      mydata.increment = tag;
      break;
    default:
      return false;
  }
  return true;
}

bool ZOffsetWizard::onTouchHeld(uint8_t tag) {
  const float increment = TERN(BABYSTEPPING,
    mmFromWholeSteps(mmToWholeSteps(getIncrement(), Z), Z), // Round increment to nearest steps
    getIncrement()
  );
  switch (tag) {
    case 4: UI_DECREMENT(ZOffset_mm); UI_DECREMENT(AxisPosition_mm, Z); break;
    case 5: UI_INCREMENT(ZOffset_mm); UI_INCREMENT(AxisPosition_mm, Z); break;
    default:
      return false;
  }
  SaveSettingsDialogBox::settingsChanged();
  return true;
}

#endif // COCOA_Z_OFFSET_WIZARD
