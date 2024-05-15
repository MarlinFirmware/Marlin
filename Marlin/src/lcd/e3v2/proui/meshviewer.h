/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * Mesh Viewer for PRO UI
 * Based on the original work of: Miguel Risco-Castillo (MRISCOC)
 * https://github.com/mriscoc/Ender3V2S1
 * version: 4.2.1
 * Date: 2023/05/05
 */

class MeshViewer {
public:
  static float max, min;
  static void drawMeshGrid(const uint8_t csizex, const uint8_t csizey);
  static void drawMeshPoint(const uint8_t x, const uint8_t y, const float z);
  static void draw(const bool withsave=false, const bool redraw=true);
  static void drawMesh(const bed_mesh_t zval, const uint8_t csizex, const uint8_t csizey);
};

extern MeshViewer meshViewer;

void gotoMeshViewer(const bool redraw);
