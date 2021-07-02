/*************************
 * flow_percent_screen.h *
 *************************/

/****************************************************************************
 *   Written By Marcio Teixeira 2021 - Cocoa Press                          *
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

#define FTDI_FLOW_PERCENT_SCREEN
#define FTDI_FLOW_PERCENT_SCREEN_CLASS FlowPercentScreen

class FlowPercentScreen : public BaseNumericAdjustmentScreen, public CachedScreen<FLOW_PERCENT_SCREEN_CACHE> {
  public:
    static void onRedraw(draw_mode_t);
    static bool onTouchHeld(uint8_t tag);
};
