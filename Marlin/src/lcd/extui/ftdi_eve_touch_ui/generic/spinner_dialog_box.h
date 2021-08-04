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

class SpinnerDialogBox : public BaseScreen {
  public:
    static void onEntry();
    static void onExit();
    static void onRedraw(draw_mode_t);
    static void onRefresh();
    static void onIdle();

    static void show(progmem_str);
    static void hide();

    template<typename T>
    static void enqueueAndWait(T commands) {enqueueAndWait(GET_TEXT_F(MSG_PLEASE_WAIT), commands);}

    static void enqueueAndWait(progmem_str message, char *commands);
    static void enqueueAndWait(progmem_str message, progmem_str commands);
};
