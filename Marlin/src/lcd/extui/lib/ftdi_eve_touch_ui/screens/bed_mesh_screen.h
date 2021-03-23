/*********************
 * bed_mesh_screen.h *
 *********************/

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

#define FTDI_BED_MESH_SCREEN
#define FTDI_BED_MESH_SCREEN_CLASS BedMeshScreen

struct BedMeshScreenData {
  enum : uint8_t {
    MSG_NONE,
    MSG_MESH_COMPLETE,
    MSG_MESH_INCOMPLETE
  } message;
  uint8_t count;
  uint8_t highlightedTag;
  float zAdjustment;
};

class BedMeshScreen : public BaseScreen, public CachedScreen<BED_MESH_SCREEN_CACHE> {
  private:
    enum MeshOpts {
      USE_POINTS    = 0x01,
      USE_COLORS    = 0x02,
      USE_TAGS      = 0x04,
      USE_HIGHLIGHT = 0x08,
      USE_AUTOSCALE = 0x10
    };

    static uint8_t pointToTag(uint8_t x, uint8_t y);
    static bool tagToPoint(uint8_t tag, xy_uint8_t &pt);
    static float getHighlightedValue(bool nanAsZero);
    static void setHighlightedValue(float value);
    static void moveToHighlightedValue();
    static void adjustHighlightedValue(float increment);
    static void saveAdjustedHighlightedValue();
    static void changeHighlightedValue(uint8_t tag);
    static void drawHighlightedPointValue();
    static void drawMesh(int16_t x, int16_t y, int16_t w, int16_t h, ExtUI::bed_mesh_t data, uint8_t opts, float autoscale_max = 0.1);
  public:
    static void onMeshUpdate(const int8_t x, const int8_t y, const float val);
    static void onMeshUpdate(const int8_t x, const int8_t y, const ExtUI::probe_state_t);
    static void onEntry();
    static void onRedraw(draw_mode_t);
    static bool onTouchEnd(uint8_t tag);

    static void startMeshProbe();
    static void showMeshEditor();
};
