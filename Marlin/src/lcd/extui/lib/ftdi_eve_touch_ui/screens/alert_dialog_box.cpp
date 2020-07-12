/************************
 * alert_dialog_box.cpp *
 ************************/

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

#if ENABLED(TOUCH_UI_FTDI_EVE)

#include "screens.h"
#include "screen_data.h"

using namespace FTDI;
using namespace Theme;

void AlertDialogBox::onEntry() {
  BaseScreen::onEntry();
  sound.play(screen_data.AlertDialogBox.isError ? sad_trombone : twinkle, PLAY_ASYNCHRONOUS);
}

void AlertDialogBox::onRedraw(draw_mode_t what) {
  if (what & FOREGROUND) {
    drawOkayButton();
  }
}

template<typename T>
void AlertDialogBox::show(const T message) {
  drawMessage(message);
  storeBackground();
  screen_data.AlertDialogBox.isError = false;
  GOTO_SCREEN(AlertDialogBox);
}

template<typename T>
void AlertDialogBox::showError(const T message) {
  drawMessage(message);
  storeBackground();
  screen_data.AlertDialogBox.isError = true;
  GOTO_SCREEN(AlertDialogBox);
}

void AlertDialogBox::hide() {
  if (AT_SCREEN(AlertDialogBox))
    GOTO_PREVIOUS();
}

template void AlertDialogBox::show(const char *);
template void AlertDialogBox::show(const progmem_str);
template void AlertDialogBox::showError(const char *);
template void AlertDialogBox::showError(const progmem_str);

#endif // TOUCH_UI_FTDI_EVE
