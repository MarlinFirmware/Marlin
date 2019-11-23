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

#if ENABLED(LULZBOT_TOUCH_UI) && defined(TOUCH_UI_LULZBOT_BIO)

#include "screens.h"

using namespace FTDI;

void BioConfirmHomeXYZ::onRedraw(draw_mode_t) {
  drawMessage(GET_TEXT_F(MSG_HOME_XYZ_WARNING));
  drawYesNoButtons(1);
}

bool BioConfirmHomeXYZ::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case 1:
      SpinnerDialogBox::enqueueAndWait_P(F(
       "G28\n"
       PARK_AND_RELEASE_COMMANDS
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
