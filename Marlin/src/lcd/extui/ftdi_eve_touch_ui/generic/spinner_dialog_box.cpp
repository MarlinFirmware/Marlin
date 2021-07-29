/**************************
 * spinner_dialog_box.cpp *
 **************************/

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

#ifdef FTDI_SPINNER_DIALOG_BOX

using namespace FTDI;
using namespace ExtUI;

constexpr static SpinnerDialogBoxData &mydata = screen_data.SpinnerDialogBox;

void SpinnerDialogBox::onEntry() {
  mydata.auto_hide = true;
}

void SpinnerDialogBox::onRedraw(draw_mode_t) {
}

void SpinnerDialogBox::show(progmem_str message) {
  drawMessage(message);
  drawSpinner();
  storeBackground();
  GOTO_SCREEN(SpinnerDialogBox);
  mydata.auto_hide = false;
}

void SpinnerDialogBox::hide() {
  CommandProcessor cmd;
  cmd.stop().execute();
}

void SpinnerDialogBox::enqueueAndWait(progmem_str message, progmem_str commands) {
  show(message);
  ExtUI::injectCommands_P((const char*)commands);
}

void SpinnerDialogBox::enqueueAndWait(progmem_str message, char *commands) {
  show(message);
  ExtUI::injectCommands(commands);
}

void SpinnerDialogBox::onIdle() {
  reset_menu_timeout();
  if (mydata.auto_hide && !commandsInQueue()) {
    mydata.auto_hide = false;
    hide();
    GOTO_PREVIOUS();
  }
}

#endif // FTDI_SPINNER_DIALOG_BOX
