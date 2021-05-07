/************************
 * spinner_dialog_box.h *
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
 *   location: <https://www.gnu.org/licenses/>.                             *
 ****************************************************************************/

#pragma once

#define FTDI_SPINNER_DIALOG_BOX
#define FTDI_SPINNER_DIALOG_BOX_CLASS SpinnerDialogBox

struct SpinnerDialogBoxData {
  bool auto_hide;
};

class SpinnerDialogBox : public DialogBoxBaseClass, public CachedScreen<SPINNER_CACHE,SPINNER_DL_SIZE> {
  public:
    static void onRedraw(draw_mode_t);
    static void onIdle();

    static void show(const progmem_str);
    static void hide();
    static void enqueueAndWait_P(const progmem_str commands);
    static void enqueueAndWait_P(const progmem_str message, const progmem_str commands);
};
