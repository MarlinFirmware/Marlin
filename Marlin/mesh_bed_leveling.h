/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "Marlin.h"

#if ENABLED(MESH_BED_LEVELING)

  enum MeshLevelingState {
    MeshReport,
    MeshStart,
    MeshNext,
    MeshSet,
    MeshSetZOffset,
    MeshReset
  };

  enum MBLStatus {
    MBL_STATUS_NONE = 0,
    MBL_STATUS_HAS_MESH_BIT = 0,
    MBL_STATUS_ACTIVE_BIT = 1
  };

  #define MESH_X_DIST ((MESH_MAX_X - (MESH_MIN_X)) / (GRID_MAX_POINTS_X - 1))
  #define MESH_Y_DIST ((MESH_MAX_Y - (MESH_MIN_Y)) / (GRID_MAX_POINTS_Y - 1))

  class mesh_bed_leveling {
  public:
    static uint8_t status; // Has Mesh and Is Active bits
    static float z_offset,
                 z_values[GRID_MAX_POINTS_X][GRID_MAX_POINTS_Y],
                 index_to_xpos[GRID_MAX_POINTS_X],
                 index_to_ypos[GRID_MAX_POINTS_Y];

    mesh_bed_leveling();

    static void reset();

    static void set_z(const int8_t px, const int8_t py, const float &z) { z_values[px][py] = z; }

    static bool active()                       { return TEST(status, MBL_STATUS_ACTIVE_BIT); }
    static void set_active(const bool onOff)   { onOff ? SBI(status, MBL_STATUS_ACTIVE_BIT) : CBI(status, MBL_STATUS_ACTIVE_BIT); }
    static bool has_mesh()                     { return TEST(status, MBL_STATUS_HAS_MESH_BIT); }
    static void set_has_mesh(const bool onOff) { onOff ? SBI(status, MBL_STATUS_HAS_MESH_BIT) : CBI(status, MBL_STATUS_HAS_MESH_BIT); }

    static inline void zigzag(const int8_t index, int8_t &px, int8_t &py) {
      px = index % (GRID_MAX_POINTS_X);
      py = index / (GRID_MAX_POINTS_X);
      if (py & 1) px = (GRID_MAX_POINTS_X - 1) - px; // Zig zag
    }

    static void set_zigzag_z(const int8_t index, const float &z) {
      int8_t px, py;
      zigzag(index, px, py);
      set_z(px, py, z);
    }

    static int8_t cell_index_x(const float &x) {
      int8_t cx = (x - (MESH_MIN_X)) * (1.0 / (MESH_X_DIST));
      return constrain(cx, 0, (GRID_MAX_POINTS_X) - 2);
    }

    static int8_t cell_index_y(const float &y) {
      int8_t cy = (y - (MESH_MIN_Y)) * (1.0 / (MESH_Y_DIST));
      return constrain(cy, 0, (GRID_MAX_POINTS_Y) - 2);
    }

    static int8_t probe_index_x(const float &x) {
      int8_t px = (x - (MESH_MIN_X) + 0.5 * (MESH_X_DIST)) * (1.0 / (MESH_X_DIST));
      return WITHIN(px, 0, GRID_MAX_POINTS_X - 1) ? px : -1;
    }

    static int8_t probe_index_y(const float &y) {
      int8_t py = (y - (MESH_MIN_Y) + 0.5 * (MESH_Y_DIST)) * (1.0 / (MESH_Y_DIST));
      return WITHIN(py, 0, GRID_MAX_POINTS_Y - 1) ? py : -1;
    }

    static float calc_z0(const float &a0, const float &a1, const float &z1, const float &a2, const float &z2) {
      const float delta_z = (z2 - z1) / (a2 - a1);
      const float delta_a = a0 - a1;
      return z1 + delta_a * delta_z;
    }

    static float get_z(const float &x0, const float &y0
      #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
        , const float &factor
      #endif
    ) {
      const int8_t cx = cell_index_x(x0), cy = cell_index_y(y0);
      const float z1 = calc_z0(x0, index_to_xpos[cx], z_values[cx][cy], index_to_xpos[cx + 1], z_values[cx + 1][cy]),
                  z2 = calc_z0(x0, index_to_xpos[cx], z_values[cx][cy + 1], index_to_xpos[cx + 1], z_values[cx + 1][cy + 1]),
                  z0 = calc_z0(y0, index_to_ypos[cy], z1, index_to_ypos[cy + 1], z2);

      return z_offset + z0
        #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
          * factor
        #endif
      ;
    }
  };

  extern mesh_bed_leveling mbl;

#endif  // MESH_BED_LEVELING
