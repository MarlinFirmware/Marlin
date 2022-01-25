/*****************
 * base_screen.h *
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

#define FTDI_BASE_SCREEN
#define FTDI_BASE_SCREEN_CLASS BaseScreen

class BaseScreen : public UIScreen {
  protected:
    #if SCREENS_CAN_TIME_OUT
      static uint32_t last_interaction;
    #endif

    static bool buttonIsPressed(uint8_t tag);

  public:
    static bool buttonStyleCallback(CommandProcessor &, uint8_t, uint8_t &, uint16_t &, bool);

    static void reset_menu_timeout();

    static void onEntry();
    static void onIdle();
};
