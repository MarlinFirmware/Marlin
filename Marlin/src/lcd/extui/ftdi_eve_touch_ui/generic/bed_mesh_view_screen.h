/**************************
 * bed_mesh_view_screen.h *
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

#define FTDI_BED_MESH_VIEW_SCREEN
#define FTDI_BED_MESH_VIEW_SCREEN_CLASS BedMeshViewScreen

struct BedMeshViewScreenData {
  FSTR_P message;
  uint8_t count;
  xy_uint8_t highlight;
};

class BedMeshViewScreen : public BedMeshBase, public CachedScreen<BED_MESH_VIEW_SCREEN_CACHE> {
  private:
    static float getHighlightedValue();
    static bool changeHighlightedValue(uint8_t tag);
    static void drawHighlightedPointValue();
  public:
    static void onMeshUpdate(const int8_t x, const int8_t y, const float val);
    static void onMeshUpdate(const int8_t x, const int8_t y, const ExtUI::probe_state_t);
    static void onEntry();
    static void onRedraw(draw_mode_t);
    static bool onTouchEnd(uint8_t tag);

    static void doProbe();
    static void show();
};
