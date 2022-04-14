/*****************
 * lock_screen.h *
 *****************/

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

#pragma once

#define FTDI_LOCK_SCREEN
#define FTDI_LOCK_SCREEN_CLASS LockScreen

struct LockScreenData {
  char passcode[5];
};

class LockScreen : public BaseScreen, public CachedScreen<LOCK_SCREEN_CACHE> {
  private:
    friend InterfaceSettingsScreen;

    static uint16_t passcode;

    static char & message_style();
    static uint16_t compute_checksum();
    static void onPasscodeEntered();
  public:
    static bool is_enabled();
    static void check_passcode();
    static void enable();
    static void disable();

    static void set_hash(uint16_t pass) { passcode = pass; }
    static uint16_t get_hash() { return passcode; }

    static void onEntry();
    static void onRedraw(draw_mode_t);
    static bool onTouchEnd(uint8_t tag);
};
