/*****************************
 * bio_printing_dialog_box.h *
 *****************************/

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

#define FTDI_BIO_PRINTING_DIALOG_BOX
#define FTDI_BIO_PRINTING_DIALOG_BOX_CLASS BioPrintingDialogBox

class BioPrintingDialogBox : public BaseScreen, public CachedScreen<PRINTING_SCREEN_CACHE,PRINTING_SCREEN_DL_SIZE> {
  private:
    static void draw_status_message(draw_mode_t, const char * const);
    static void draw_progress(draw_mode_t);
    static void draw_time_remaining(draw_mode_t);
    static void draw_interaction_buttons(draw_mode_t);
  public:
    static void onRedraw(draw_mode_t);

    static void show();

    static void setStatusMessage(const char *);
    static void setStatusMessage(FSTR_P);

    static void onIdle();
    static bool onTouchEnd(uint8_t tag);
};
