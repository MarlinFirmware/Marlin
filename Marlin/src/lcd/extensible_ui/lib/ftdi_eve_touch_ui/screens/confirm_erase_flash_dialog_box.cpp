/**************************************
 * confirm_erase_flash_dialog_box.cpp *
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
 *   location: <http://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

#include "../config.h"

#if BOTH(TOUCH_UI_FTDI_EVE, TOUCH_UI_DEVELOPER_MENU)

#include "screens.h"

#include "../archim2-flash/flash_storage.h"

using namespace FTDI;

void ConfirmEraseFlashDialogBox::onRedraw(draw_mode_t) {
  drawMessage(GET_TEXT_F(MSG_ERASE_FLASH_WARNING));
  drawYesNoButtons();
}

bool ConfirmEraseFlashDialogBox::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case 1:
      SpinnerDialogBox::show(GET_TEXT_F(MSG_ERASING));
      UIFlashStorage::format_flash();
      SpinnerDialogBox::hide();
      AlertDialogBox::show(GET_TEXT_F(MSG_ERASED));
      // Remove ConfirmEraseFlashDialogBox from the stack
      // so the alert box doesn't return to me.
      current_screen.forget();
      return true;
    default:
      return DialogBoxBaseClass::onTouchEnd(tag);
  }
}

#endif // TOUCH_UI_FTDI_EVE
