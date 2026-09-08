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

#include "../../inc/MarlinConfigPre.h"

#if ANY(RESTORE_LEVELING_AFTER_G28, ENABLE_LEVELING_AFTER_G28)
  #define CAN_SET_LEVELING_AFTER_G28 1
#endif

#if ENABLED(PROBE_MANUALLY)
  extern bool g29_in_progress;
#else
  constexpr bool g29_in_progress = false;
#endif

void set_bed_leveling_enabled(const bool enable=true);
void reset_bed_level();

#if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
  void set_z_fade_height(const float zfh, const bool do_report=true);
#endif

#if ANY(MESH_BED_LEVELING, PROBE_MANUALLY)
  void _manual_goto_xy(const xy_pos_t &pos);
#endif

/**
 * A class to save and change the bed leveling state,
 * then restore it when it goes out of scope.
 */
class TemporaryBedLevelingState {
  bool saved;
  public:
    TemporaryBedLevelingState(const bool enable);
    ~TemporaryBedLevelingState() { set_bed_leveling_enabled(saved); }
};
#define TEMPORARY_BED_LEVELING_STATE(enable) const TemporaryBedLevelingState tbls(enable)

#ifdef GRID_MAX_POINTS_X
  constexpr xy_uint8_t grid_max_points = { GRID_MAX_POINTS_X, GRID_MAX_POINTS_Y };
#endif

#if HAS_MESH

  #define CACHED_GRID_POINTS
  #define CACHED_DIST_RECIPROCAL

  typedef float bed_mesh_t[GRID_MAX_POINTS_X][GRID_MAX_POINTS_Y];

  // The Leveling Mesh is shared by MBL, ABL Bilinear, and UBL
  class LevelingMesh {
    public:
      bed_mesh_t z_values;

      #if ANY(HAS_PROUI_MESH_EDIT, VARIABLE_GRID_POINTS)
        static xy_pos_t mesh_min, mesh_max;
      #else
        static constexpr xy_pos_t mesh_min{ MESH_MIN_X, MESH_MIN_Y },
                                  mesh_max{ MESH_MAX_X, MESH_MAX_Y };
      #endif

      #if ENABLED(VARIABLE_GRID_POINTS)
        static xy_uint8_t nr_grid_points;
        static xy_float_t grid_spacing;
      #endif
      #if ENABLED(CACHED_DIST_RECIPROCAL)
        #if ENABLED(VARIABLE_GRID_POINTS)
          static xy_float_t grid_spacing_recip;
        #else
          static constexpr xy_float_t grid_spacing_recip = grid_spacing.reciprocal();
        #endif
      #endif

      #if ENABLED(CACHED_GRID_POINTS)
        static float index_to_xpos[GRID_MAX_POINTS_X], index_to_ypos[GRID_MAX_POINTS_Y];
      #endif

      static void refresh_mesh_lookup() {
        #if ENABLED(VARIABLE_GRID_POINTS)
          grid_spacing = (mesh_max - mesh_min) / nr_grid_points;
          grid_spacing_recip = grid_spacing_reciprocal();
        #endif
        #if ENABLED(CACHED_GRID_POINTS)
          for (uint8_t i = 0; i < GRID_MAX_POINTS_X; ++i)
            index_to_xpos[i] = mesh_min.x + i * grid_spacing.x;
          for (uint8_t i = 0; i < GRID_MAX_POINTS_Y; ++i)
            index_to_ypos[i] = mesh_min.y + i * grid_spacing.y;
        #endif
      }
      #if ENABLED(VARIABLE_GRID_POINTS)
        static void set_nr_grid_points(const xy_uint8_t &gp) {
          nr_grid_points = gp;
          refresh_mesh_lookup();
        }
      #else
        static constexpr xy_uint8_t nr_grid_points { GRID_MAX_POINTS_X, GRID_MAX_POINTS_Y };
        static constexpr xy_float_t grid_spacing = (mesh_max - mesh_min) / nr_grid_points
      #endif

      static xy_float_t grid_spacing_reciprocal() {
        return TERN(CACHED_DIST_RECIPROCAL, grid_spacing_recip, grid_spacing.reciprocal());
      }

      static void init() {
        #if ENABLED(VARIABLE_GRID_POINTS)
          mesh_min.set(MESH_MIN_X, MESH_MIN_Y);
          mesh_max.set(MESH_MAX_X, MESH_MAX_Y);
          set_nr_grid_points(grid_max_points);
        #endif
      }
      static void reset(const float v=0.0f);

      static bool has_mesh() { return !!grid_spacing.x; }
      static bool mesh_is_valid() { return has_mesh(); }

      static float _grid_x(const uint8_t i) { return mesh_min.x + i * grid_spacing.x; }
      static float _grid_y(const uint8_t i) { return mesh_min.y + i * grid_spacing.y; }
      static float grid_x(const uint8_t ix) { return TERN(CACHED_GRID_POINTS, index_to_xpos[ix], _grid_x(ix)); }
      static float grid_y(const uint8_t iy) { return TERN(CACHED_GRID_POINTS, index_to_ypos[iy], _grid_y(iy)); }
      static xy_pos_t grid_point(const xy_uint8_t &index) {
        return { grid_x(index.x), grid_y(index.y) };
      }

      static int8_t _cell_index_x(const float x) { return FLOOR((x - mesh_min.x) * grid_spacing_reciprocal().x); }
      static int8_t _cell_index_y(const float y) { return FLOOR((y - mesh_min.y) * grid_spacing_reciprocal().y); }
      static bool cell_index_x_valid(const float x) { return WITHIN(_cell_index_x(x), 0, nr_grid_points.x - 1); }
      static bool cell_index_y_valid(const float y) { return WITHIN(_cell_index_y(y), 0, nr_grid_points.y - 1); }
      static uint8_t cell_index_x(const float x) { return constrain(_cell_index_x(x), 0, nr_grid_points.x - 1); }
      static uint8_t cell_index_y(const float y) { return constrain(_cell_index_y(y), 0, nr_grid_points.y - 1); }
      static xy_uint8_t cell_indexes(const float x, const float y) { return { cell_index_x(x), cell_index_y(y) }; }
      static xy_uint8_t cell_indexes(const xy_pos_t &xy) { return cell_indexes(xy.x, xy.y); }

      static bool leveling_is_valid();
  };

  #define _BEDLEVEL_INCLUDE

  #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
    #include "abl/bbl.h"
  #elif ENABLED(AUTO_BED_LEVELING_UBL)
    #include "ubl/ubl.h"
  #elif ENABLED(MESH_BED_LEVELING)
    #include "mbl/mesh_bed_leveling.h"
  #endif

  #undef _BEDLEVEL_INCLUDE

  #if ANY(AUTO_BED_LEVELING_BILINEAR, MESH_BED_LEVELING)

    #include <stdint.h>

    typedef float (*element_2d_fn)(const uint8_t, const uint8_t);

    /**
     * Print calibration results for plotting or manual frame adjustment.
     */
    void print_2d_array(const uint8_t sx, const uint8_t sy, const uint8_t precision, const float *values OPTARG(VARIABLE_GRID_POINTS, xy_uint8_t printpos={0,0}));

    #if ENABLED(VARIABLE_GRID_POINTS)
      #define PRINT_2D_ARRAY(X, Y, P, V, M...) print_2d_array(X, Y, P, V, M)
    #else
      #define PRINT_2D_ARRAY(X, Y, P, V, M...) print_2d_array(X, Y, P, V)
    #endif

  #endif

  struct mesh_index_pair {
    xy_int8_t pos;
    float distance;   // When populated, the distance from the search location
    void invalidate() { pos = -1; }
    bool valid() const { return pos.x >= 0 && pos.y >= 0; }
    #if ENABLED(AUTO_BED_LEVELING_UBL)
      xy_pos_t meshpos() {
        return { bedlevel.get_mesh_x(pos.x), bedlevel.get_mesh_y(pos.y) };
      }
    #endif
    operator xy_int8_t&() { return pos; }
    operator const xy_int8_t&() const { return pos; }
  };

#endif
