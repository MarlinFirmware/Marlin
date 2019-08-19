/****************************
 * bio_confirm_home_xyz.cpp *
 ****************************/

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

#if ENABLED(LULZBOT_TOUCH_UI) && defined(LULZBOT_USE_BIOPRINTER_UI)

#include "screens.h"

using namespace FTDI;

void BioConfirmHomeXYZ::onRedraw(draw_mode_t) {
  drawMessage(F("About to home to loading position.\nEnsure the top and the bed of the printer are clear.\n\nContinue?"));
  drawYesNoButtons(1);
}

bool BioConfirmHomeXYZ::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case 1:
      SpinnerDialogBox::enqueueAndWait_P(F(
        "G28 X Y Z\n"             /* Home all axis */
        "G0 X115 Z50 F6000"       /* Move to park position */
      ));
      current_screen.forget();
      break;
    case 2:
      GOTO_SCREEN(StatusScreen);
      break;
    default:
      return DialogBoxBaseClass::onTouchEnd(tag);
  }
  return true;
}
#endif // LULZBOT_TOUCH_UI
