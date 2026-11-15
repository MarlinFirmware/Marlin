/************************************
 * power_loss_resume_dialog_box.cpp *
 ************************************/

/****************************************************************************
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

#ifdef FTDI_POWER_LOSS_RESUME_DIALOG_BOX

void PowerLossResumeDialogBox::onRedraw(draw_mode_t) {
  drawMessage(GET_TEXT_F(MSG_RESUME_PRINT));
  drawYesNoButtons();
}

bool PowerLossResumeDialogBox::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case 1: ExtUI::injectCommands(F("M1000")); break;
    case 2: ExtUI::injectCommands(F("M1000 C")); break;
    default: return false;
  }
  GOTO_SCREEN(StatusScreen);
  current_screen.forget();
  return true;
}

#endif // FTDI_POWER_LOSS_RESUME_DIALOG_BOX
