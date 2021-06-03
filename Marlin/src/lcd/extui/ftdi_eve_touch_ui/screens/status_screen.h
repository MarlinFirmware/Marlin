/*******************
 * status_screen.h *
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
 *   location: <https://www.gnu.org/licenses/>.                             *
 ****************************************************************************/

#pragma once

#define FTDI_STATUS_SCREEN
#define FTDI_STATUS_SCREEN_CLASS StatusScreen

class StatusScreen : public BaseScreen, public CachedScreen<STATUS_SCREEN_CACHE,STATUS_SCREEN_DL_SIZE> {
  private:
    static void draw_axis_position(draw_mode_t);
    static void draw_temperature(draw_mode_t);
    static void draw_progress(draw_mode_t);
    static void draw_interaction_buttons(draw_mode_t);
    static void draw_status_message(draw_mode_t, const char * const);
    static void _format_time(char *outstr, uint32_t time);
  public:
    static void loadBitmaps();
    static void setStatusMessage(const char *);
    static void setStatusMessage(progmem_str);
    static void onRedraw(draw_mode_t);
    static void onStartup();
    static void onEntry();
    static void onIdle();
    static bool onTouchEnd(uint8_t tag);
};
