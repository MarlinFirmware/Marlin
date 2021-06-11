/**************************************
 * confirm_start_print_dialog_box.cpp *
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
#include "../screens.h"
#include "../screen_data.h"

#ifdef FTDI_CONFIRM_START_PRINT_DIALOG_BOX

using namespace FTDI;
using namespace Theme;
using namespace ExtUI;

constexpr static ConfirmStartPrintDialogBoxData &mydata = screen_data.ConfirmStartPrintDialogBox;

void ConfirmStartPrintDialogBox::onRedraw(draw_mode_t) {
  const char *filename = getLongFilename();
  char buffer[strlen_P(GET_TEXT(MSG_START_PRINT_CONFIRMATION)) + strlen(filename) + 1];
  sprintf_P(buffer, GET_TEXT(MSG_START_PRINT_CONFIRMATION), filename);
  drawMessage((const char *)buffer);
  drawYesNoButtons(1);
}

bool ConfirmStartPrintDialogBox::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case 1:
      printFile(getShortFilename());
      StatusScreen::setStatusMessage(GET_TEXT_F(MSG_PRINT_STARTING));
      GOTO_SCREEN(StatusScreen);
      return true;
    case 2: GOTO_PREVIOUS(); return true;
    default:                 return false;
  }
}

const char *ConfirmStartPrintDialogBox::getFilename(bool longName) {
  FileList files;
  files.seek(mydata.file_index, true);
  return longName ? files.longFilename() : files.shortFilename();
}

void ConfirmStartPrintDialogBox::show(uint8_t file_index) {
  mydata.file_index = file_index;
   GOTO_SCREEN(ConfirmStartPrintDialogBox);
}

#endif // FTDI_CONFIRM_START_PRINT_DIALOG_BOX
