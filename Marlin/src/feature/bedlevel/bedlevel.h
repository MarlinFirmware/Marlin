/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#pragma once

#include "../../inc/MarlinConfigPre.h"

typedef struct {
  int8_t x_index, y_index;
  float distance; // When populated, the distance from the search location
} mesh_index_pair;

#if ENABLED(PROBE_MANUALLY)
  extern bool g29_in_progress;
#else
  constexpr bool g29_in_progress = false;
#endif

bool leveling_is_valid();
void set_bed_leveling_enabled(const bool enable=true);
void reset_bed_level();

#if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
  void set_z_fade_height(const float zfh, const bool do_report=true);
#endif

#if EITHER(MESH_BED_LEVELING, PROBE_MANUALLY)
  void _manual_goto_xy(const float &x, const float &y);
#endif

#if HAS_MESH

  typedef float (&bed_mesh_t)[GRID_MAX_POINTS_X][GRID_MAX_POINTS_Y];

  #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
    #include "abl/abl.h"
  #elif ENABLED(AUTO_BED_LEVELING_UBL)
    #include "ubl/ubl.h"
  #elif ENABLED(MESH_BED_LEVELING)
    #include "mbl/mesh_bed_leveling.h"
  #endif

  #define Z_VALUES(X,Y) Z_VALUES_ARR[X][Y]

  #if EITHER(AUTO_BED_LEVELING_BILINEAR, MESH_BED_LEVELING)

    #include <stdint.h>

    typedef float (*element_2d_fn)(const uint8_t, const uint8_t);

    /**
     * Print calibration results for plotting or manual frame adjustment.
     */
    void print_2d_array(const uint8_t sx, const uint8_t sy, const uint8_t precision, element_2d_fn fn);

  #endif

#endif
