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

struct StatusScreenData {
  bool gotMessage;
};

class StatusScreen : public BaseScreen, public CachedScreen<STATUS_SCREEN_CACHE, STATUS_SCREEN_DL_SIZE> {
  private:
    static void _format_time(char *outstr, uint32_t time);

    static void draw_time(draw_mode_t what);
    static void draw_percent(draw_mode_t what);
    static void draw_temperature(draw_mode_t what);
    static void draw_buttons(draw_mode_t what);
    static void draw_file(draw_mode_t what);
    static void draw_message(draw_mode_t what, const char *message);
    static void draw_bkgnd(draw_mode_t what);

    static void send_buffer(CommandProcessor &cmd, const void *data, uint16_t len);
    static void load_background(const void *data, uint16_t len);

    static bool isFileSelected();
  public:
    static void loadBitmaps();

    static void setStatusMessage(const char *);
    static void setStatusMessage(FSTR_P);

    static void onRedraw(draw_mode_t);

    static void onEntry();
    static bool onTouchHeld(uint8_t tag);
    static bool onTouchEnd(uint8_t tag);
    static void onIdle();
    static void onMediaInserted();
    static void onMediaRemoved();
};
