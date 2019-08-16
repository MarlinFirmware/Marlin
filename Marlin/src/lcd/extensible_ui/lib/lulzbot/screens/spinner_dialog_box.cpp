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
 *   location: <http://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

#include "../config.h"

#if ENABLED(LULZBOT_TOUCH_UI)

#include "screens.h"
#include "screen_data.h"

using namespace FTDI;
using namespace ExtUI;

void SpinnerDialogBox::onRedraw(draw_mode_t) {
}

void SpinnerDialogBox::show(const progmem_str message) {
  drawMessage(message);
  drawSpinner();
  storeBackground();
  screen_data.SpinnerDialogBox.auto_hide = false;
}

void SpinnerDialogBox::hide() {
  CommandProcessor cmd;
  cmd.stop().execute();
}

void SpinnerDialogBox::enqueueAndWait_P(const progmem_str commands) {
  enqueueAndWait_P(F("Please wait..."), commands);
}

void SpinnerDialogBox::enqueueAndWait_P(const progmem_str message, const progmem_str commands) {
  show(message);
  GOTO_SCREEN(SpinnerDialogBox);
  ExtUI::injectCommands_P((const char*)commands);
  screen_data.SpinnerDialogBox.auto_hide = true;
}

void SpinnerDialogBox::onIdle() {
  if (screen_data.SpinnerDialogBox.auto_hide && !commandsInQueue()) {
    screen_data.SpinnerDialogBox.auto_hide = false;
    hide();
    GOTO_PREVIOUS();
  }
}

#endif // LULZBOT_TOUCH_UI
