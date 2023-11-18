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

#define COCOA_Z_OFFSET_WIZARD
#define COCOA_Z_OFFSET_WIZARD_CLASS ZOffsetWizard

struct ZOffsetWizardData : public BaseNumericAdjustmentScreenData {
  uint8_t increment;
  bool softEndstopState;
};

class ZOffsetWizard : public BaseScreen, public UncachedScreen {
  private:
    static float getIncrement();
  public:
    static void runWizard();
    static void onEntry();
    static void onExit();
    static void onRedraw(draw_mode_t);
    static bool onTouchHeld(uint8_t tag);
    static bool onTouchEnd(uint8_t tag);
};
