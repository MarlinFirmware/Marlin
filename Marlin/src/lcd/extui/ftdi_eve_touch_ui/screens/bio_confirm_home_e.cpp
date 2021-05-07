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
 *   location: <https://www.gnu.org/licenses/>.                             *
 ****************************************************************************/

#include "../config.h"
#include "screens.h"

#ifdef FTDI_BIO_CONFIRM_HOME_E

using namespace FTDI;

void BioConfirmHomeE::onRedraw(draw_mode_t) {
  drawMessage(GET_TEXT_F(MSG_HOME_E_WARNING));
  drawYesNoButtons(1);
}

bool BioConfirmHomeE::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case 1:
      #if defined(AXIS_LEVELING_COMMANDS) && defined(PARK_AND_RELEASE_COMMANDS)
        SpinnerDialogBox::enqueueAndWait_P(F(
          "G28 E\n"
          AXIS_LEVELING_COMMANDS "\n"
          PARK_AND_RELEASE_COMMANDS
        ));
      #endif
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

#endif // FTDI_BIO_CONFIRM_HOME_E
