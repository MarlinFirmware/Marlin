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
  #define MESH_X_DIST ((MESH_MAX_X - (MESH_MIN_X))/(MESH_NUM_X_POINTS - 1))
  #define MESH_Y_DIST ((MESH_MAX_Y - (MESH_MIN_Y))/(MESH_NUM_Y_POINTS - 1))

  class mesh_bed_leveling {
  public:
    uint8_t status; // Has Mesh and Is Active bits
    float z_offset;
    float z_values[MESH_NUM_Y_POINTS][MESH_NUM_X_POINTS];

    mesh_bed_leveling();

    void reset();

    static FORCE_INLINE float get_probe_x(int8_t i) { return MESH_MIN_X + (MESH_X_DIST) * i; }
    static FORCE_INLINE float get_probe_y(int8_t i) { return MESH_MIN_Y + (MESH_Y_DIST) * i; }
    void set_z(const int8_t px, const int8_t py, const float z) { z_values[py][px] = z; }

    bool active()                 { return TEST(status, MBL_STATUS_ACTIVE_BIT); }
    void set_active(bool onOff)   { if (onOff) SBI(status, MBL_STATUS_ACTIVE_BIT); else CBI(status, MBL_STATUS_ACTIVE_BIT); }
    bool has_mesh()               { return TEST(status, MBL_STATUS_HAS_MESH_BIT); }
    void set_has_mesh(bool onOff) { if (onOff) SBI(status, MBL_STATUS_HAS_MESH_BIT); else CBI(status, MBL_STATUS_HAS_MESH_BIT); }

    inline void zigzag(int8_t index, int8_t &px, int8_t &py) {
      px = index % (MESH_NUM_X_POINTS);
      py = index / (MESH_NUM_X_POINTS);
      if (py & 1) px = (MESH_NUM_X_POINTS - 1) - px; // Zig zag
    }

    void set_zigzag_z(int8_t index, float z) {
      int8_t px, py;
      zigzag(index, px, py);
      set_z(px, py, z);
    }

    int8_t cell_index_x(float x) {
      int8_t cx = int(x - (MESH_MIN_X)) / (MESH_X_DIST);
      return constrain(cx, 0, (MESH_NUM_X_POINTS) - 2);
    }

    int8_t cell_index_y(float y) {
      int8_t cy = int(y - (MESH_MIN_Y)) / (MESH_Y_DIST);
      return constrain(cy, 0, (MESH_NUM_Y_POINTS) - 2);
    }

    int8_t probe_index_x(float x) {
      int8_t px = int(x - (MESH_MIN_X) + (MESH_X_DIST) / 2) / (MESH_X_DIST);
      return (px >= 0 && px < (MESH_NUM_X_POINTS)) ? px : -1;
    }

    int8_t probe_index_y(float y) {
      int8_t py = int(y - (MESH_MIN_Y) + (MESH_Y_DIST) / 2) / (MESH_Y_DIST);
      return (py >= 0 && py < (MESH_NUM_Y_POINTS)) ? py : -1;
    }

    float calc_z0(float a0, float a1, float z1, float a2, float z2) {
      float delta_z = (z2 - z1) / (a2 - a1);
      float delta_a = a0 - a1;
      return z1 + delta_a * delta_z;
    }

    float get_z(float x0, float y0) {
      int8_t cx = cell_index_x(x0),
             cy = cell_index_y(y0);
      if (cx < 0 || cy < 0) return z_offset;
      float z1 = calc_z0(x0,
                         get_probe_x(cx), z_values[cy][cx],
                         get_probe_x(cx + 1), z_values[cy][cx + 1]);
      float z2 = calc_z0(x0,
                         get_probe_x(cx), z_values[cy + 1][cx],
                         get_probe_x(cx + 1), z_values[cy + 1][cx + 1]);
      float z0 = calc_z0(y0,
                         get_probe_y(cy), z1,
                         get_probe_y(cy + 1), z2);
      return z0 + z_offset;
    }
  };

  extern mesh_bed_leveling mbl;

#endif  // MESH_BED_LEVELING
