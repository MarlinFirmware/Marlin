/**************************************
 * confirm_abort_print_dialog_box.cpp *
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
#include "screens.h"

#ifdef FTDI_CONFIRM_ABORT_PRINT_DIALOG_BOX

#include "../../../../../feature/host_actions.h"

using namespace ExtUI;

void ConfirmAbortPrintDialogBox::onRedraw(draw_mode_t) {
  drawMessage(GET_TEXT_F(MSG_ABORT_WARNING));
  drawYesNoButtons();
}

bool ConfirmAbortPrintDialogBox::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case 1:
      GOTO_PREVIOUS();
      if (ExtUI::isPrintingFromMedia())
         ExtUI::stopPrint();
      #ifdef ACTION_ON_CANCEL
        else host_action_cancel();
      #endif
      return true;
    default:
      return DialogBoxBaseClass::onTouchEnd(tag);
  }
}

#endif // FTDI_CONFIRM_ABORT_PRINT_DIALOG_BOX
