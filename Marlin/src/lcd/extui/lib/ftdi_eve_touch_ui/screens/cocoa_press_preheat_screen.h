/*********************************
 * cocoapress_preheat_screen.cpp *
 *********************************/

/****************************************************************************
 *   Written By Marcio Teixeira 2019 - Cocoa Press                          *
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

#define FTDI_COCOA_PREHEAT_SCREEN
#define FTDI_COCOA_PREHEAT_SCREEN_CLASS PreheatTimerScreen

struct PreheatTimerScreenData {
    uint32_t start_ms;
};

class PreheatTimerScreen : public BaseScreen, public CachedScreen<PREHEAT_TIMER_SCREEN_CACHE> {
  private:
    static uint16_t secondsRemaining();

    static void draw_message(draw_mode_t);
    static void draw_time_remaining(draw_mode_t);
    static void draw_interaction_buttons(draw_mode_t);
    static void draw_adjuster(draw_mode_t, uint8_t tag, progmem_str label, float value, int16_t x, int16_t y, int16_t w, int16_t h);
  public:
    static void onRedraw(draw_mode_t);

    static void onEntry();
    static void onIdle();
    static bool onTouchHeld(uint8_t tag);
    static bool onTouchEnd(uint8_t tag);
};
