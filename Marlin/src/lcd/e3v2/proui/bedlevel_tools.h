/*
 * Marlin 3D Printer Firmware
 * Copyright (c) 2022 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * Bed Level Tools for Pro UI
 * Extended by: Miguel A. Risco-Castillo (MRISCOC)
 * Version: 2.0.0
 * Date: 2022/05/23
 *
 * Based on the original work of: Henri-J-Norden
 * https://github.com/Jyers/Marlin/pull/126
 */

#pragma once

#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(AUTO_BED_LEVELING_UBL)
  //#define USE_UBL_VIEWER 1
#endif

#define MESH_Z_OFFSET_MIN -3.0
#define MESH_Z_OFFSET_MAX  3.0

class BedLevelToolsClass {
public:
  #if USE_UBL_VIEWER
    static bool viewer_asymmetric_range;
    static bool viewer_print_value;
  #endif
  static bool goto_mesh_value;
  static uint8_t mesh_x;
  static uint8_t mesh_y;
  static uint8_t tilt_grid;

  #if ENABLED(AUTO_BED_LEVELING_UBL)
    static void manual_value_update(const uint8_t mesh_x, const uint8_t mesh_y, bool undefined=false);
    static bool create_plane_from_mesh();
  #else
    static void manual_value_update(const uint8_t mesh_x, const uint8_t mesh_y);
  #endif
  static void manual_move(const uint8_t mesh_x, const uint8_t mesh_y, bool zmove=false);
  static void MoveToXYZ();
  static void MoveToXY();
  static void MoveToZ();
  static void ProbeXY();
  static float get_max_value();
  static float get_min_value();
  static bool meshvalidate();
  #if USE_UBL_VIEWER
    static void Draw_Bed_Mesh(int16_t selected = -1, uint8_t gridline_width = 1, uint16_t padding_x = 8, uint16_t padding_y_top = 40 + 53 - 7);
    static void Set_Mesh_Viewer_Status();
  #endif
};

extern BedLevelToolsClass BedLevelTools;

void Goto_MeshViewer();
