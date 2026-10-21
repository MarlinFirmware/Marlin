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
  extern uint16_t g29_total_points; // Number of points to probe in the G29 in progress
#else
  constexpr bool g29_in_progress = false;
#endif

void set_bed_leveling_enabled(const bool enable=true);
void reset_bed_level();
bool leveling_is_valid();

#if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
  void set_z_fade_height(const float zfh, const bool do_report=true);
#endif

#if ANY(MESH_BED_LEVELING, PROBE_MANUALLY)
  void _manual_goto_xy(const xy_pos_t &pos);
#endif

#ifdef GRID_MAX_POINTS_X
  constexpr xy_uint8_t grid_max_points = { GRID_MAX_POINTS_X, GRID_MAX_POINTS_Y };

  // DELTA requires an odd number of grid points so there is a center point
  constexpr bool grid_points_must_be_odd = ENABLED(DELTA);

  /**
   * Reduce the number of grid points on each axis so the grid spacing
   * across 'size' is not smaller than 'min_spacing'. Never fewer than 2.
   * With 'odd' the result is odd (and at least 3), so 'points' must be odd.
   * An axis with min_spacing <= 0 keeps its number of points.
   */
  inline uint8_t reduced_grid_points(const float size, const uint8_t points, const float min_spacing, const bool odd=grid_points_must_be_odd) {
    if (min_spacing <= 0 || points < 3 || size / (points - 1) >= min_spacing) return points;
    uint8_t n = constrain(uint8_t(size / min_spacing) + 1, 2, points);
    if (odd && !(n & 1)) n = _MAX(n - 1, 3);
    return n;
  }
  inline xy_uint8_t reduced_grid_points(const xy_float_t &size, const xy_uint8_t &points, const xy_float_t &min_spacing, const bool odd=grid_points_must_be_odd) {
    return { reduced_grid_points(size.x, points.x, min_spacing.x, odd), reduced_grid_points(size.y, points.y, min_spacing.y, odd) };
  }

  // Is a grid size allowed? Between 2 and GRID_MAX_POINTS_[XY], and odd for DELTA.
  inline bool grid_points_allowed(const uint8_t n, const uint8_t nmax, const bool odd=grid_points_must_be_odd) {
    return WITHIN(n, 2, nmax) && (!odd || (n & 1));
  }
#endif

#if HAS_MESH

  #define CACHED_DIST_RECIPROCAL

  typedef float bed_mesh_t[GRID_MAX_POINTS_X][GRID_MAX_POINTS_Y];

  // The Leveling Mesh is shared by MBL, ABL Bilinear, and UBL
  class LevelingMesh {
    public:
      static bed_mesh_t z_values;

      static constexpr xy_uint8_t max_points = { GRID_MAX_POINTS_X, GRID_MAX_POINTS_Y },
                                  max_cells = { GRID_MAX_POINTS_X - 1, GRID_MAX_POINTS_Y - 1 };

      // Mesh bounds and spacing are runtime values when they can be changed by
      // G29 (Bilinear), the ProUI Mesh Inset menu, or a variable number of points.
      #if HAS_DYNAMIC_MESH_GRID
        static xy_pos_t mesh_min, mesh_max;
        static xy_float_t grid_spacing;
        #if ENABLED(CACHED_DIST_RECIPROCAL)
          static xy_float_t grid_spacing_recip;
        #endif
      #else
        static constexpr xy_pos_t mesh_min{ MESH_MIN_X, MESH_MIN_Y },
                                  mesh_max{ MESH_MAX_X, MESH_MAX_Y };
        static constexpr xy_float_t grid_spacing = {
          (float(MESH_MAX_X) - float(MESH_MIN_X)) / float(GRID_MAX_POINTS_X - 1),
          (float(MESH_MAX_Y) - float(MESH_MIN_Y)) / float(GRID_MAX_POINTS_Y - 1)
        };
        #if ENABLED(CACHED_DIST_RECIPROCAL)
          static constexpr xy_float_t grid_spacing_recip = grid_spacing.reciprocal();
        #endif
      #endif

      #if ENABLED(VARIABLE_GRID_POINTS)
        static xy_uint8_t nr_grid_points;
      #else
        static constexpr xy_uint8_t nr_grid_points = max_points;
      #endif

      // Update the grid spacing after changing mesh_min, mesh_max, or nr_grid_points
      static void refresh_mesh_lookup() {
        #if HAS_DYNAMIC_MESH_GRID
          grid_spacing.set((mesh_max.x - mesh_min.x) / float(nr_grid_points.x - 1),
                           (mesh_max.y - mesh_min.y) / float(nr_grid_points.y - 1));
          TERN_(CACHED_DIST_RECIPROCAL, grid_spacing_recip = grid_spacing.reciprocal());
        #endif
      }

      #if ENABLED(VARIABLE_GRID_POINTS)
        // Is a grid size valid for the mesh array?
        static bool valid_grid_points(const xy_uint8_t &gp) {
          return grid_points_allowed(gp.x, GRID_MAX_POINTS_X) && grid_points_allowed(gp.y, GRID_MAX_POINTS_Y);
        }
        static void set_nr_grid_points(const xy_uint8_t &gp) {
          nr_grid_points.set(constrain(gp.x, 2, GRID_MAX_POINTS_X), constrain(gp.y, 2, GRID_MAX_POINTS_Y));
          refresh_mesh_lookup();
        }
      #endif

      static uint8_t used_points_x() { return nr_grid_points.x; }
      static uint8_t used_points_y() { return nr_grid_points.y; }
      static uint16_t used_points()  { return (uint16_t)used_points_x() * (uint16_t)used_points_y(); }
      static uint8_t used_cells_x()  { return used_points_x() - 1; }
      static uint8_t used_cells_y()  { return used_points_y() - 1; }

      static xy_float_t grid_spacing_reciprocal() {
        return TERN(CACHED_DIST_RECIPROCAL, grid_spacing_recip, grid_spacing.reciprocal());
      }

      // Reset mesh bounds and the number of points to the configured defaults
      static void init() {
        #if HAS_DYNAMIC_MESH_GRID && !HAS_PROUI_MESH_EDIT
          mesh_min.set(MESH_MIN_X, MESH_MIN_Y);
          mesh_max.set(MESH_MAX_X, MESH_MAX_Y);
        #endif
        TERN_(VARIABLE_GRID_POINTS, nr_grid_points = max_points);
        refresh_mesh_lookup();
      }

      // Reset the number of points and fill the whole mesh with a value
      static void reset(const float v=0.0f);

      static bool has_mesh() { return !!grid_spacing.x; }
      static bool mesh_is_valid() { return has_mesh(); }

      static float _grid_x(const uint8_t i) { return mesh_min.x + i * grid_spacing.x; }
      static float _grid_y(const uint8_t i) { return mesh_min.y + i * grid_spacing.y; }
      static float grid_x(const uint8_t ix) { return _grid_x(ix); }
      static float grid_y(const uint8_t iy) { return _grid_y(iy); }
      static xy_pos_t grid_point(const xy_uint8_t &index) {
        return { grid_x(index.x), grid_y(index.y) };
      }

      // Cell indexes. A cell is the area between two grid lines, so the
      // last valid cell index is one less than the number of cells.
      static int8_t _cell_index_x(const float x) { return FLOOR((x - mesh_min.x) * grid_spacing_reciprocal().x); }
      static int8_t _cell_index_y(const float y) { return FLOOR((y - mesh_min.y) * grid_spacing_reciprocal().y); }
      static bool cell_index_x_valid(const float x) { return WITHIN(_cell_index_x(x), 0, used_cells_x() - 1); }
      static bool cell_index_y_valid(const float y) { return WITHIN(_cell_index_y(y), 0, used_cells_y() - 1); }
      static uint8_t cell_index_x(const float x) { return constrain(_cell_index_x(x), 0, used_cells_x() - 1); }
      static uint8_t cell_index_y(const float y) { return constrain(_cell_index_y(y), 0, used_cells_y() - 1); }

      // Nearest grid point index, or -1 if outside the grid
      static int8_t nearest_index_x(const float x) {
        const int8_t px = FLOOR((x - mesh_min.x) * grid_spacing_reciprocal().x + 0.5f);
        return WITHIN(px, 0, used_points_x() - 1) ? px : -1;
      }
      static int8_t nearest_index_y(const float y) {
        const int8_t py = FLOOR((y - mesh_min.y) * grid_spacing_reciprocal().y + 0.5f);
        return WITHIN(py, 0, used_points_y() - 1) ? py : -1;
      }
      static xy_uint8_t cell_indexes(const float x, const float y) { return { cell_index_x(x), cell_index_y(y) }; }
      static xy_uint8_t cell_indexes(const xy_pos_t &xy) { return cell_indexes(xy.x, xy.y); }

      static bool leveling_is_valid();
  };

  //
  // Include to get 'extern bedlevel' as a sub-class of LevelingMesh
  //
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
     * Print sx * sy values from an array whose second dimension is stride_y.
     */
    void print_2d_array(const uint8_t sx, const uint8_t sy, const uint8_t precision, const float *values, const uint8_t stride_y);

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

#endif // HAS_MESH

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
