/********************************
 * save_settings_dialog_box.cpp *
 ********************************/

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

#if ENABLED(LULZBOT_TOUCH_UI)

#include "screens.h"

using namespace ExtUI;

bool SaveSettingsDialogBox::needs_save = false;

void SaveSettingsDialogBox::onRedraw(draw_mode_t) {
  drawMessage(F("Do you wish to save these settings as defaults?"));
  drawYesNoButtons();
}

bool SaveSettingsDialogBox::onTouchEnd(uint8_t tag) {
  needs_save = false;
  switch (tag) {
    case 1:
      injectCommands_P(PSTR("M500"));
      AlertDialogBox::show(F("Settings saved!"));
      // Remove SaveSettingsDialogBox from the stack
      // so the alert box doesn't return to me.
      current_screen.forget();
      return true;
    default:
      return DialogBoxBaseClass::onTouchEnd(tag);
  }
}

void SaveSettingsDialogBox::promptToSaveSettings() {
   if (needs_save) {
     // Remove current screen from the stack
     // so SaveSettingsDialogBox doesn't return here.
     GOTO_SCREEN(SaveSettingsDialogBox);
     current_screen.forget();
   } else {
     // No save needed.
     GOTO_PREVIOUS();
   }
}

#endif // LULZBOT_TOUCH_UI
