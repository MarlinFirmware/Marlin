/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * Unit tests for the bed leveling grid: grid size helpers, the shared
 * LevelingMesh lookup, and Z correction for Bilinear, UBL, and MBL.
 */

#include "../test/unit_tests.h"

#ifdef GRID_MAX_POINTS_X

#include "src/feature/bedlevel/bedlevel.h"

//
// Grid size helpers (all grid-based leveling)
//

MARLIN_TEST(bedlevel, reduced_grid_points_keeps_wide_spacing) {
  TEST_ASSERT_EQUAL(5, reduced_grid_points(200.0f, 5, 25.0f, false)); // 50mm cells
  TEST_ASSERT_EQUAL(5, reduced_grid_points(100.0f, 5, 25.0f, false)); // Exactly 25mm cells
}

MARLIN_TEST(bedlevel, reduced_grid_points_reduces_small_area) {
  TEST_ASSERT_EQUAL(3, reduced_grid_points(60.0f, 5, 25.0f, false));  // 30mm cells
  TEST_ASSERT_EQUAL(3, reduced_grid_points(70.0f, 5, 25.0f, false));  // 35mm cells
  TEST_ASSERT_EQUAL(4, reduced_grid_points(75.0f, 9, 25.0f, false));  // 25mm cells
  TEST_ASSERT_EQUAL(2, reduced_grid_points(10.0f, 5, 25.0f, false));  // Never fewer than 2
}

MARLIN_TEST(bedlevel, reduced_grid_points_disabled) {
  TEST_ASSERT_EQUAL(5, reduced_grid_points(10.0f, 5, 0.0f, false));   // No minimum spacing
  TEST_ASSERT_EQUAL(2, reduced_grid_points(10.0f, 2, 25.0f, false));  // Already minimal
}

MARLIN_TEST(bedlevel, reduced_grid_points_odd) {
  TEST_ASSERT_EQUAL(3, reduced_grid_points(60.0f, 7, 25.0f, true));   // 3 is already odd
  TEST_ASSERT_EQUAL(3, reduced_grid_points(80.0f, 7, 25.0f, true));   // 4 rounds down to 3
  TEST_ASSERT_EQUAL(5, reduced_grid_points(100.0f, 7, 25.0f, true));  // 5 is already odd
  TEST_ASSERT_EQUAL(3, reduced_grid_points(10.0f, 7, 25.0f, true));   // Never fewer than 3
  TEST_ASSERT_EQUAL(7, reduced_grid_points(300.0f, 7, 25.0f, true));  // No reduction
}

MARLIN_TEST(bedlevel, reduced_grid_points_never_too_close) {
  // For all sizes and point counts the result never exceeds the input,
  // and a reduced grid never has cells smaller than the minimum.
  for (uint8_t odd = 0; odd < 2; ++odd)
    for (uint8_t n = 2; n <= 15; ++n)
      for (uint16_t size = 1; size <= 400; size += 3) {
        const uint8_t r = reduced_grid_points(float(size), n, 25.0f, odd);
        TEST_ASSERT_TRUE(r <= n || (odd && r == 3));
        TEST_ASSERT_TRUE(r >= 2);
        if (r < n && r > (odd ? 3 : 2)) TEST_ASSERT_TRUE(float(size) / (r - 1) >= 25.0f);
        if (odd && r < n) TEST_ASSERT_TRUE(r & 1);
      }
}

MARLIN_TEST(bedlevel, grid_points_allowed) {
  TEST_ASSERT_FALSE(grid_points_allowed(1, 9, false));
  TEST_ASSERT_TRUE(grid_points_allowed(2, 9, false));
  TEST_ASSERT_TRUE(grid_points_allowed(9, 9, false));
  TEST_ASSERT_FALSE(grid_points_allowed(10, 9, false));
  TEST_ASSERT_FALSE(grid_points_allowed(4, 9, true));   // Even, for DELTA
  TEST_ASSERT_TRUE(grid_points_allowed(5, 9, true));
}

#if HAS_MESH

  // Reset to an empty mesh with the full grid. (MBL reset() keeps its grid size.)
  static void reset_full() {
    bedlevel.reset();
    TERN_(VARIABLE_GRID_POINTS, bedlevel.set_nr_grid_points(bedlevel.max_points));
  }

  // Fill the used part of the mesh with a tilted plane and the rest with NaN
  static float plane_z(const float x, const float y) { return 0.01f * x - 0.02f * y + 0.5f; }

  static void fill_plane() {
    GRID_LOOP_MAX(x, y) {
      const bool used = x < bedlevel.nr_grid_points.x && y < bedlevel.nr_grid_points.y;
      bedlevel.z_values[x][y] = used ? plane_z(bedlevel.get_mesh_x(x), bedlevel.get_mesh_y(y)) : NAN;
    }
  }

  // Check Z correction at grid points and between them inside the mesh
  static void check_plane(const float tolerance=0.0005f) {
    const xy_pos_t lo = bedlevel.mesh_min,
                   hi = { bedlevel.get_mesh_x(bedlevel.nr_grid_points.x - 1), bedlevel.get_mesh_y(bedlevel.nr_grid_points.y - 1) };
    for (uint8_t i = 0; i <= 10; ++i)
      for (uint8_t j = 0; j <= 10; ++j) {
        const xy_pos_t p = { lo.x + (hi.x - lo.x) * i / 10.0f, lo.y + (hi.y - lo.y) * j / 10.0f };
        #if ENABLED(AUTO_BED_LEVELING_UBL)
          const float z = bedlevel.get_z_correction(p.x, p.y);
        #else
          const float z = bedlevel.get_z_correction(p);
        #endif
        TEST_ASSERT_FLOAT_WITHIN(tolerance, plane_z(p.x, p.y), z);
      }
  }

  //
  // LevelingMesh: spacing and index lookup
  //

  MARLIN_TEST(bedlevel, mesh_cell_index_range) {
    reset_full();
    #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
      bedlevel.set_grid({ 50, 40 }, { 10, 20 });
    #endif
    // Below the mesh clamps to the first cell, beyond it to the last cell
    TEST_ASSERT_EQUAL(0, bedlevel.cell_index_x(bedlevel.mesh_min.x - 100));
    TEST_ASSERT_EQUAL(bedlevel.used_cells_x() - 1, bedlevel.cell_index_x(bedlevel.mesh_max.x + 100));
    TEST_ASSERT_EQUAL(bedlevel.used_cells_y() - 1, bedlevel.cell_index_y(bedlevel.mesh_max.y + 100));
    // The far edge is in the last cell, not past it
    TEST_ASSERT_EQUAL(bedlevel.used_cells_x() - 1, bedlevel.cell_index_x(bedlevel.mesh_max.x));
    TEST_ASSERT_TRUE(bedlevel.cell_index_x_valid(bedlevel.mesh_min.x + 0.1f));
    TEST_ASSERT_FALSE(bedlevel.cell_index_x_valid(bedlevel.mesh_max.x + bedlevel.grid_spacing.x));
  }

  MARLIN_TEST(bedlevel, mesh_nearest_index) {
    reset_full();
    #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
      bedlevel.set_grid({ 50, 40 }, { 10, 20 });
    #endif
    const float sx = bedlevel.grid_spacing.x;
    TEST_ASSERT_EQUAL(0, bedlevel.nearest_index_x(bedlevel.mesh_min.x + sx * 0.49f));
    TEST_ASSERT_EQUAL(1, bedlevel.nearest_index_x(bedlevel.mesh_min.x + sx * 0.51f));
    TEST_ASSERT_EQUAL(bedlevel.used_points_x() - 1, bedlevel.nearest_index_x(bedlevel.get_mesh_x(bedlevel.used_points_x() - 1)));
    TEST_ASSERT_EQUAL(-1, bedlevel.nearest_index_x(bedlevel.mesh_min.x - sx));
  }

  #if ENABLED(VARIABLE_GRID_POINTS)

    MARLIN_TEST(bedlevel, variable_points_set_and_reset) {
      #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
        bedlevel.set_grid({ 30, 30 }, { 10, 10 }, { 3, 2 });
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 70.0f, bedlevel.mesh_max.x);   // 10 + 30 * 2
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 40.0f, bedlevel.mesh_max.y);   // 10 + 30 * 1
      #else
        bedlevel.set_nr_grid_points({ 3, 2 });
        TEST_ASSERT_FLOAT_WITHIN(0.001f, (bedlevel.mesh_max.x - bedlevel.mesh_min.x) / 2, bedlevel.grid_spacing.x);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, bedlevel.mesh_max.y - bedlevel.mesh_min.y, bedlevel.grid_spacing.y);
      #endif
      TEST_ASSERT_EQUAL(3, bedlevel.nr_grid_points.x);
      TEST_ASSERT_EQUAL(2, bedlevel.nr_grid_points.y);
      TEST_ASSERT_EQUAL(6, bedlevel.used_points());
      TEST_ASSERT_FLOAT_WITHIN(0.0001f, 1.0f, bedlevel.grid_spacing.x * bedlevel.grid_spacing_reciprocal().x);

      // reset() returns to the full grid (except MBL, where G29 S1 sets the size)
      #if DISABLED(MESH_BED_LEVELING)
        bedlevel.reset();
        TEST_ASSERT_EQUAL(GRID_MAX_POINTS_X, bedlevel.nr_grid_points.x);
        TEST_ASSERT_EQUAL(GRID_MAX_POINTS_Y, bedlevel.nr_grid_points.y);
      #endif
      reset_full();
    }

    MARLIN_TEST(bedlevel, variable_points_validation) {
      TEST_ASSERT_TRUE(bedlevel.valid_grid_points({ 2, 2 }));
      TEST_ASSERT_TRUE(bedlevel.valid_grid_points({ GRID_MAX_POINTS_X, GRID_MAX_POINTS_Y }));
      TEST_ASSERT_FALSE(bedlevel.valid_grid_points({ 1, 2 }));
      TEST_ASSERT_FALSE(bedlevel.valid_grid_points({ GRID_MAX_POINTS_X + 1, 2 }));
      // The setter clamps bad values instead of indexing past the mesh
      bedlevel.set_nr_grid_points({ 0, 255 });
      TEST_ASSERT_EQUAL(2, bedlevel.nr_grid_points.x);
      TEST_ASSERT_EQUAL(GRID_MAX_POINTS_Y, bedlevel.nr_grid_points.y);
      reset_full();
    }

  #endif // VARIABLE_GRID_POINTS

  //
  // Z correction of a plane is exact for bilinear interpolation
  //

  MARLIN_TEST(bedlevel, z_correction_full_grid) {
    reset_full();
    #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
      bedlevel.set_grid({ 40, 30 }, { 20, 10 });
    #endif
    fill_plane();
    TERN_(AUTO_BED_LEVELING_BILINEAR, bedlevel.refresh_bed_level());
    check_plane();
  }

  #if ENABLED(VARIABLE_GRID_POINTS)

    MARLIN_TEST(bedlevel, z_correction_reduced_grid) {
      // A smaller grid, with NaN in the unused part of the mesh array
      reset_full();
      #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
        bedlevel.set_grid({ 60, 45 }, { 20, 10 }, { 3, 2 });
      #else
        bedlevel.set_nr_grid_points({ 3, 2 });
      #endif
      fill_plane();
      TERN_(AUTO_BED_LEVELING_BILINEAR, bedlevel.refresh_bed_level());
      check_plane();
      reset_full();
    }

  #endif

  #if ENABLED(AUTO_BED_LEVELING_BILINEAR)

    MARLIN_TEST(bedlevel, bilinear_reset_has_no_grid) {
      bedlevel.set_grid({ 40, 30 }, { 20, 10 });
      fill_plane();
      bedlevel.refresh_bed_level();
      TEST_ASSERT_TRUE(bedlevel.leveling_is_valid());
      // After a reset there is no grid, so leveling can't be enabled on NaN
      bedlevel.reset();
      TEST_ASSERT_FALSE(bedlevel.has_mesh());
      TEST_ASSERT_FALSE(bedlevel.leveling_is_valid());
    }

  #endif

  #if ENABLED(AUTO_BED_LEVELING_UBL)

    MARLIN_TEST(bedlevel, ubl_valid_ignores_unused_points) {
      bedlevel.reset();
      TERN_(VARIABLE_GRID_POINTS, bedlevel.set_nr_grid_points({ 3, 3 }));
      bedlevel.invalidate();
      TEST_ASSERT_FALSE(bedlevel.mesh_is_valid());
      fill_plane(); // NaN beyond the used grid
      TEST_ASSERT_TRUE(bedlevel.mesh_is_valid());
      reset_full();
    }

  #endif

#endif // HAS_MESH

#endif // GRID_MAX_POINTS_X
