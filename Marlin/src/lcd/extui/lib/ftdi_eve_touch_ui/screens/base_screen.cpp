/*******************
 * base_screen.cpp *
 *******************/

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

using namespace FTDI;
using namespace Theme;

void BaseScreen::onEntry() {
  CommandProcessor cmd;
  cmd.set_button_style_callback(buttonStyleCallback);
  reset_menu_timeout();
  UIScreen::onEntry();
}

bool BaseScreen::buttonIsPressed(uint8_t tag) {
  return tag != 0 && EventLoop::get_pressed_tag() == tag;
}

bool BaseScreen::buttonStyleCallback(CommandProcessor &cmd, uint8_t tag, uint8_t &style, uint16_t &options, bool post) {
  if (post) {
    cmd.colors(normal_btn);
    return false;
  }

  #if LCD_TIMEOUT_TO_STATUS > 0
    if (EventLoop::get_pressed_tag() != 0) {
      reset_menu_timeout();
    }
  #endif

  if (buttonIsPressed(tag)) {
    options = OPT_FLAT;
  }

  if (style & cmd.STYLE_DISABLED) {
    cmd.tag(0);
    style &= ~cmd.STYLE_DISABLED;
    cmd.colors(disabled_btn);
    return true; // Call me again to reset the colors
  }
  return false;
}

void BaseScreen::onIdle() {
  #if LCD_TIMEOUT_TO_STATUS > 0
    if ((millis() - last_interaction) > LCD_TIMEOUT_TO_STATUS) {
      reset_menu_timeout();
      #if ENABLED(TOUCH_UI_DEBUG)
        SERIAL_ECHO_MSG("Returning to status due to menu timeout");
      #endif
      GOTO_SCREEN(StatusScreen);
    }
  #endif
}

void BaseScreen::reset_menu_timeout() {
  #if LCD_TIMEOUT_TO_STATUS > 0
    last_interaction = millis();
  #endif
}

#if LCD_TIMEOUT_TO_STATUS > 0
  uint32_t BaseScreen::last_interaction;
#endif

#endif // TOUCH_UI_FTDI_EVE
