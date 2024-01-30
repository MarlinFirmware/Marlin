/**********************
 * move_axis_screen.h *
 **********************/

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

#define FTDI_MOVE_AXIS_SCREEN
#define FTDI_MOVE_AXIS_SCREEN_CLASS MoveAxisScreen

struct MoveAxisScreenData {
  struct BaseNumericAdjustmentScreenData placeholder;
  float e_rel[ExtUI::extruderCount];
};

class BaseMoveAxisScreen : public BaseNumericAdjustmentScreen {
  private:
    static float getManualFeedrate(const uint8_t axis, const_float_t increment_mm);
  public:
    static void raiseZtoTop();
    static void setManualFeedrate(const ExtUI::axis_t, const_float_t increment_mm);
    static void setManualFeedrate(const ExtUI::extruder_t, const_float_t increment_mm);

    static void onEntry();
    static bool onTouchHeld(const uint8_t tag);
};

class MoveAxisScreen : public BaseMoveAxisScreen, public CachedScreen<MOVE_AXIS_SCREEN_CACHE> {
  public:
    static void onRedraw(draw_mode_t);
    static void onIdle();
};
