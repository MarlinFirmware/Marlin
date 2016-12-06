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

    static FORCE_INLINE float get_probe_x(const int8_t i) { return MESH_MIN_X + (MESH_X_DIST) * i; }
    static FORCE_INLINE float get_probe_y(const int8_t i) { return MESH_MIN_Y + (MESH_Y_DIST) * i; }
    void set_z(const int8_t px, const int8_t py, const float &z) { z_values[py][px] = z; }

    bool active() const                 { return TEST(status, MBL_STATUS_ACTIVE_BIT); }
    void set_active(const bool onOff)   { onOff ? SBI(status, MBL_STATUS_ACTIVE_BIT) : CBI(status, MBL_STATUS_ACTIVE_BIT); }
    bool has_mesh() const               { return TEST(status, MBL_STATUS_HAS_MESH_BIT); }
    void set_has_mesh(const bool onOff) { onOff ? SBI(status, MBL_STATUS_HAS_MESH_BIT) : CBI(status, MBL_STATUS_HAS_MESH_BIT); }

    inline void zigzag(const int8_t index, int8_t &px, int8_t &py) const {
      px = index % (MESH_NUM_X_POINTS);
      py = index / (MESH_NUM_X_POINTS);
      if (py & 1) px = (MESH_NUM_X_POINTS - 1) - px; // Zig zag
    }

    void set_zigzag_z(const int8_t index, const float &z) {
      int8_t px, py;
      zigzag(index, px, py);
      set_z(px, py, z);
    }

    int8_t cell_index_x(const float &x) const {
      int8_t cx = (x - (MESH_MIN_X)) * (1.0 / (MESH_X_DIST));
      return constrain(cx, 0, (MESH_NUM_X_POINTS) - 2);
    }

    int8_t cell_index_y(const float &y) const {
      int8_t cy = (y - (MESH_MIN_Y)) * (1.0 / (MESH_Y_DIST));
      return constrain(cy, 0, (MESH_NUM_Y_POINTS) - 2);
    }

    int8_t probe_index_x(const float &x) const {
      int8_t px = (x - (MESH_MIN_X) + (MESH_X_DIST) * 0.5) * (1.0 / (MESH_X_DIST));
      return (px >= 0 && px < (MESH_NUM_X_POINTS)) ? px : -1;
    }

    int8_t probe_index_y(const float &y) const {
      int8_t py = (y - (MESH_MIN_Y) + (MESH_Y_DIST) * 0.5) * (1.0 / (MESH_Y_DIST));
      return (py >= 0 && py < (MESH_NUM_Y_POINTS)) ? py : -1;
    }

    float calc_z0(const float &a0, const float &a1, const float &z1, const float &a2, const float &z2) const {
      const float delta_z = (z2 - z1) / (a2 - a1);
      const float delta_a = a0 - a1;
      return z1 + delta_a * delta_z;
    }

    float get_z(const float &x0, const float &y0
      #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
        , const float &factor
      #endif
    ) const {
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

      #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
        return z0 * factor + z_offset;
      #else
        return z0 + z_offset;
      #endif
    }
  };

  extern mesh_bed_leveling mbl;

#endif  // MESH_BED_LEVELING
