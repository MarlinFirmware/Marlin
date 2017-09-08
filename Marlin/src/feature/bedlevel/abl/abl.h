/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016, 2017 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef __ABL_H__
#define __ABL_H__

#include "../../../inc/MarlinConfig.h"

#if ENABLED(AUTO_BED_LEVELING_BILINEAR)

  #include "../bedlevel.h"

  extern int bilinear_grid_spacing[2], bilinear_start[2];
  extern float bilinear_grid_factor[2],
               z_values[GRID_MAX_POINTS_X][GRID_MAX_POINTS_Y];
  float bilinear_z_offset(const float logical[XYZ]);

  void extrapolate_unprobed_bed_level();
  void print_bilinear_leveling_grid();
  void refresh_bed_level();
  #if ENABLED(ABL_BILINEAR_SUBDIVISION)
    void print_bilinear_leveling_grid_virt();
    void bed_level_virt_interpolate();
  #endif

  #if !IS_KINEMATIC
    void bilinear_line_to_destination(const float fr_mm_s, uint16_t x_splits=0xFFFF, uint16_t y_splits=0xFFFF);
  #endif

#endif // AUTO_BED_LEVELING_BILINEAR

#endif // __ABL_H__
