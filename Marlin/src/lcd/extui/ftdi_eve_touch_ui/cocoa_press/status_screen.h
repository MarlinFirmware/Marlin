/*******************************
 * cocoa_press/status_screen.h *
 *******************************/

/****************************************************************************
 *   Written By Mark Pelletier  2017 - Aleph Objects, Inc.                  *
 *   Written By Marcio Teixeira 2018 - Aleph Objects, Inc.                  *
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

#define COCOA_STATUS_SCREEN
#define COCOA_STATUS_SCREEN_CLASS StatusScreen

class StatusScreen : public BaseScreen, public CachedScreen<STATUS_SCREEN_CACHE> {
  private:
    static float increment;
    static bool  jog_xy;
    static bool  fine_motion;

    static void draw_progress(draw_mode_t what);
    static void draw_temperature(draw_mode_t what);
    static void draw_syringe(draw_mode_t what);
    static void draw_arrows(draw_mode_t what);
    static void draw_overlay_icons(draw_mode_t what);
    static void draw_fine_motion(draw_mode_t what);
    static void draw_buttons(draw_mode_t what);
  public:
    static void loadBitmaps();
    static void unlockMotors();

    static void setStatusMessage(const char *);
    static void setStatusMessage(progmem_str);

    static void onRedraw(draw_mode_t);

    static bool onTouchStart(uint8_t tag);
    static bool onTouchHeld(uint8_t tag);
    static bool onTouchEnd(uint8_t tag);
    static void onIdle();
};
