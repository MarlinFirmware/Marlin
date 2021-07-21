/*******************
 * bed_mesh_base.h *
 *******************/

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

#define FTDI_BED_MESH_BASE

class BedMeshBase : public BaseScreen {
  protected:
    typedef float (*mesh_getter_ptr)(uint8_t x, uint8_t y, void *data);

  private:
    enum MeshOpts {
      USE_POINTS    = 0x01,
      USE_COLORS    = 0x02,
      USE_TAGS      = 0x04,
      USE_HIGHLIGHT = 0x08,
      USE_AUTOSCALE = 0x10
    };

    static void _drawMesh(CommandProcessor &, int16_t x, int16_t y, int16_t w, int16_t h, uint8_t opts, float autoscale_max, uint8_t highlightedTag, mesh_getter_ptr func, void *data);

  protected:
    static void drawMeshForeground(CommandProcessor &cmd, int16_t x, int16_t y, int16_t w, int16_t h, mesh_getter_ptr func, void *data, uint8_t highlightedTag = 0, float progress = 1.0);
    static void drawMeshBackground(CommandProcessor &cmd, int16_t x, int16_t y, int16_t w, int16_t h);
    static uint8_t pointToTag(uint8_t x, uint8_t y);
    static bool tagToPoint(uint8_t tag, xy_uint8_t &pt);
};
