/********************************
 * cocoa_press_load_chocolate.h *
 ********************************/

/****************************************************************************
 *   Written By Mark Pelletier  2017 - Aleph Objects, Inc.                  *
 *   Written By Marcio Teixeira 2018 - Aleph Objects, Inc.                  *
 *   Written By Marcio Teixeira 2020 - Cocoa Press                          *
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
 *   location: <https://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

#pragma once

#define FTDI_COCOA_LOAD_CHOCOLATE_SCREEN
#define FTDI_COCOA_LOAD_CHOCOLATE_SCREEN_CLASS LoadChocolateScreen

struct LoadChocolateScreenData {
  uint8_t repeat_tag;
};

class LoadChocolateScreen : public BaseScreen, public CachedScreen<LOAD_CHOCOLATE_SCREEN_CACHE> {
  private:
    static void draw_syringe(draw_mode_t what);
    static void draw_arrows(draw_mode_t what);
    static void draw_buttons(draw_mode_t what);
    static void draw_text(draw_mode_t what);
  public:
    static void setManualFeedrateAndIncrement(float feedrate_mm_s, float &increment);
    static void onEntry();
    static void onIdle();
    static void onRedraw(draw_mode_t);
    static bool onTouchStart(uint8_t tag);
    static bool onTouchEnd(uint8_t tag);
    static bool onTouchHeld(uint8_t tag);
};
