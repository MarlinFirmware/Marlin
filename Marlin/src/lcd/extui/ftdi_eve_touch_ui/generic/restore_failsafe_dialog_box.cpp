/***********************************
 * restore_failsafe_dialog_box.cpp *
 ***********************************/

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

#ifdef FTDI_RESTORE_FAILSAFE_DIALOG_BOX

using namespace ExtUI;

void RestoreFailsafeDialogBox::onRedraw(draw_mode_t) {
  drawMessage(GET_TEXT_F(MSG_EEPROM_RESET_WARNING));
  drawYesNoButtons();
}

bool RestoreFailsafeDialogBox::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case 1:
      ExtUI::injectCommands(F("M502"));
      AlertDialogBox::show(GET_TEXT_F(MSG_EEPROM_RESET));
      // Remove RestoreFailsafeDialogBox from the stack
      // so the alert box doesn't return to it.
      current_screen.forget();
      SaveSettingsDialogBox::settingsChanged();
      return true;
    default:
      return DialogBoxBaseClass::onTouchEnd(tag);
  }
}

#endif // FTDI_RESTORE_FAILSAFE_DIALOG_BOX
