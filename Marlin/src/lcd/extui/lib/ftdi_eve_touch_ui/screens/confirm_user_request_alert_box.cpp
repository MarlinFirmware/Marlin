/**************************************
 * confirm_user_request_alert_box.cpp *
 **************************************/

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

#if ENABLED(TOUCH_UI_FTDI_EVE)

#include "screens.h"
#include "screen_data.h"

using namespace FTDI;

void ConfirmUserRequestAlertBox::onRedraw(draw_mode_t mode) {
  AlertDialogBox::onRedraw(mode); // Required for the GOTO_SCREEN function to work
}

bool ConfirmUserRequestAlertBox::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case 1:
      ExtUI::setUserConfirmed();
      GOTO_PREVIOUS();
      return true;
    case 2: GOTO_PREVIOUS(); return true;
    default:                 return false;
  }
}

void ConfirmUserRequestAlertBox::show(const char* msg) {
  drawMessage(msg);
  storeBackground();
  screen_data.AlertDialogBox.isError = false;
  GOTO_SCREEN(ConfirmUserRequestAlertBox);
}

void ConfirmUserRequestAlertBox::hide() {
  if (AT_SCREEN(ConfirmUserRequestAlertBox))
    GOTO_PREVIOUS();
}
#endif // TOUCH_UI_FTDI_EVE
