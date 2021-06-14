/***********************
 * z_offset_screen.h *
 ***********************/

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

#define FTDI_Z_OFFSET_SCREEN
#define FTDI_Z_OFFSET_SCREEN_CLASS ZOffsetScreen

struct ZOffsetScreenData : public BaseNumericAdjustmentScreenData {
  float z;
};

class ZOffsetScreen : public BaseNumericAdjustmentScreen, public CachedScreen<ZOFFSET_SCREEN_CACHE> {
  private:
    static void move(float mm, int16_t steps);
    static void runWizard();
  public:
    static void onEntry();
    static void onRedraw(draw_mode_t);
    static bool onTouchHeld(uint8_t tag);
};
