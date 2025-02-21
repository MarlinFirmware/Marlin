/**************************
 * bed_mesh_edit_screen.h *
 *************************/

/****************************************************************************
 *   Written By Marcio Teixeira 2020                                        *
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

#define FTDI_BED_MESH_EDIT_SCREEN
#define FTDI_BED_MESH_EDIT_SCREEN_CLASS BedMeshEditScreen

struct BedMeshEditScreenData {
  bool needSave, savedMeshLevelingState, savedEndstopState;
  xy_uint8_t highlight;
  float zAdjustment;
};

class BedMeshEditScreen : public BedMeshBase, public CachedScreen<BED_MESH_EDIT_SCREEN_CACHE> {
  private:
    static void makeMeshValid();
    static float getHighlightedValue();
    static void setHighlightedValue(float value);
    static void moveToHighlightedValue();
    static void adjustHighlightedValue(float increment);
    static void saveAdjustedHighlightedValue();
    static bool changeHighlightedValue(uint8_t tag);
    static void drawHighlightedPointValue();
  public:
    static void onEntry();
    static void onExit();
    static void onRedraw(draw_mode_t);
    static bool onTouchHeld(uint8_t tag);
    static bool onTouchEnd(uint8_t tag);
    static void show();
};
