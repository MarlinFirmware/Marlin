/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../../inc/MarlinConfigPre.h"

class bilinear_bed_leveling {
  static void extrapolate_one_point(const uint8_t x, const uint8_t y, const int8_t xdir, const int8_t ydir);

  #if ENABLED(ABL_BILINEAR_SUBDIVISION)
    #define ABL_GRID_POINTS_VIRT_X (GRID_MAX_CELLS_X * (BILINEAR_SUBDIVISIONS) + 1)
    #define ABL_GRID_POINTS_VIRT_Y (GRID_MAX_CELLS_Y * (BILINEAR_SUBDIVISIONS) + 1)

    static float z_values_virt[ABL_GRID_POINTS_VIRT_X][ABL_GRID_POINTS_VIRT_Y];
    static xy_pos_t grid_spacing_virt;
    static xy_float_t grid_factor_virt;

    static float bed_level_virt_coord(const uint8_t x, const uint8_t y);
    static float bed_level_virt_cmr(const float p[4], const uint8_t i, const float t);
    static float bed_level_virt_2cmr(const uint8_t x, const uint8_t y, const_float_t tx, const_float_t ty);
#endif

public:
  static xy_pos_t grid_spacing, grid_start;
  static xy_float_t grid_factor;
  static bed_mesh_t z_values;

  static void reset();
  static void extrapolate_unprobed_bed_level();
  static void print_leveling_grid();
  static void refresh_bed_level();
  static bool has_mesh() {return !!grid_spacing.x;}
  #if ENABLED(ABL_BILINEAR_SUBDIVISION)
    static void print_leveling_grid_virt();
    static void bed_level_virt_interpolate();
  #endif
  static float get_z_correction(const xy_pos_t &raw);

  #if IS_CARTESIAN && DISABLED(SEGMENT_LEVELED_MOVES)
    static void line_to_destination(const_feedRate_t scaled_fr_mm_s, uint16_t x_splits=0xFFFF, uint16_t y_splits=0xFFFF);
  #endif
};

extern bilinear_bed_leveling bbl;

#define _GET_MESH_X(I) float(bbl.grid_start.x + (I) * bbl.grid_spacing.x)
#define _GET_MESH_Y(J) float(bbl.grid_start.y + (J) * bbl.grid_spacing.y)
#define Z_VALUES_ARR bbl.z_values
