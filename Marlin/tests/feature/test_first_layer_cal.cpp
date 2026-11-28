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

#include "../test/unit_tests.h"

#include "src/feature/first_layer_cal.h"

// Common line settings
static constexpr float lh = 0.2f, lw = 0.42f, fil = 1.75f,
                       spacing = lw - lh * (1 - float(M_PI) / 4);  // Patch line spacing

// Filament per mm for 0.2 x 0.42 line and 1.75 filament. Same as Prusa MK3 count_e().
static constexpr float epm = 0.031354f;

// Run whole pattern and check every line stays inside area
template <typename INSIDE>
static uint16_t run_pattern(FLCPattern &pattern, INSIDE inside, flc_segment_t &last, float &total_e) {
  uint16_t count = 0;
  total_e = 0;
  TEST_ASSERT_TRUE(inside(pattern.start_x(), pattern.start_y()));
  flc_segment_t seg;
  while (pattern.next(seg)) {
    TEST_ASSERT_TRUE(inside(seg.x, seg.y));
    TEST_ASSERT_TRUE(seg.e > 0);
    total_e += seg.e;
    last = seg;
    ++count;
    TEST_ASSERT_TRUE(count < 1000);
  }
  return count;
}

MARLIN_TEST(first_layer_cal, rect_rows_and_patch) {
  FLCPattern pattern;
  TEST_ASSERT_TRUE(pattern.init_rect(10, 190, 10, 190, 6, lh, lw, fil));
  TEST_ASSERT_FLOAT_WITHIN(0.001f, 36.0f, pattern.row_pitch());
  TEST_ASSERT_FLOAT_WITHIN(0.001f, 10.0f, pattern.start_x());
  TEST_ASSERT_FLOAT_WITHIN(0.001f, 190.0f, pattern.start_y());
  const uint8_t lines = pattern.patch_lines();
  TEST_ASSERT_EQUAL(32, lines);

  flc_segment_t last;
  float total_e;
  const uint16_t count = run_pattern(pattern,
    [](const float x, const float y) { return x >= 9.999f && x <= 190.001f && y >= 9.999f && y <= 190.001f; },
    last, total_e);

  // 2 purge lines, 6 rows, 5 connectors, then a step and a line per patch line
  TEST_ASSERT_EQUAL(2 + 6 + 5 + 2 * lines, count);

  // Patch is 20 wide, starts at front row where it ended, and is 12 tall to nearest line
  TEST_ASSERT_TRUE(last.x == 10.0f || last.x == 30.0f);
  TEST_ASSERT_TRUE(last.y > 10.0f && last.y < 10.0f + 12.0f + 0.5f * spacing);
}

MARLIN_TEST(first_layer_cal, extrusion_amounts) {
  FLCPattern pattern;
  TEST_ASSERT_TRUE(pattern.init_rect(10, 190, 10, 190, 6, lh, lw, fil));
  flc_segment_t seg;

  // Purge is 25mm at 4x width, 25mm at 2x width, then nominal
  TEST_ASSERT_TRUE(pattern.next(seg));
  TEST_ASSERT_FLOAT_WITHIN(0.001f, 35.0f, seg.x);
  TEST_ASSERT_FLOAT_WITHIN(0.01f, 25 * epm * FLCPattern::area(lh, 4 * lw) / FLCPattern::area(lh, lw), seg.e);
  TEST_ASSERT_TRUE(pattern.next(seg));
  TEST_ASSERT_FLOAT_WITHIN(0.001f, 60.0f, seg.x);
  TEST_ASSERT_FLOAT_WITHIN(0.01f, 25 * epm * FLCPattern::area(lh, 2 * lw) / FLCPattern::area(lh, lw), seg.e);
  TEST_ASSERT_TRUE(pattern.next(seg));
  TEST_ASSERT_FLOAT_WITHIN(0.001f, 190.0f, seg.x);
  TEST_ASSERT_FLOAT_WITHIN(0.01f, 130 * epm, seg.e);

  // Connector to next row
  TEST_ASSERT_TRUE(pattern.next(seg));
  TEST_ASSERT_FLOAT_WITHIN(0.001f, 190.0f, seg.x);
  TEST_ASSERT_FLOAT_WITHIN(0.001f, 154.0f, seg.y);
  TEST_ASSERT_FLOAT_WITHIN(0.01f, 36 * epm, seg.e);

  // Next row runs back the other way
  TEST_ASSERT_TRUE(pattern.next(seg));
  TEST_ASSERT_FLOAT_WITHIN(0.001f, 10.0f, seg.x);
  TEST_ASSERT_FLOAT_WITHIN(0.01f, 180 * epm, seg.e);
}

MARLIN_TEST(first_layer_cal, round_bed_stays_inside) {
  FLCPattern pattern;
  TEST_ASSERT_TRUE(pattern.init_round(0, 0, 130, 6, lh, lw, fil));
  flc_segment_t last;
  float total_e;
  const uint16_t count = run_pattern(pattern,
    [](const float x, const float y) { return x * x + y * y <= 130.01f * 130.01f; },
    last, total_e);
  TEST_ASSERT_EQUAL(2 + 6 + 5 + 2 * 32, count);

  // Rows span 80% of radius in Y, so last row is at Y-104
  TEST_ASSERT_TRUE(last.y > -104.0f && last.y < -104.0f + 12.0f + 0.5f * spacing);
}

MARLIN_TEST(first_layer_cal, patch_shrinks_with_close_rows) {
  FLCPattern pattern;
  TEST_ASSERT_TRUE(pattern.init_rect(10, 190, 10, 60, 6, lh, lw, fil));  // 10mm pitch
  TEST_ASSERT_FLOAT_WITHIN(0.001f, 10.0f, pattern.row_pitch());
  // Patch height is pitch minus 4 line widths, to nearest line
  TEST_ASSERT_EQUAL(uint8_t((10 - 4 * lw) / spacing + 0.5f), pattern.patch_lines());
}

MARLIN_TEST(first_layer_cal, rejects_bad_input) {
  FLCPattern pattern;
  flc_segment_t seg;
  TEST_ASSERT_FALSE(pattern.init_rect(10, 190, 10, 190, 1, lh, lw, fil));   // One row
  TEST_ASSERT_FALSE(pattern.next(seg));
  TEST_ASSERT_FALSE(pattern.init_rect(10, 190, 10, 190, 6, 0.5f, 0.4f, fil)); // Line narrower than tall
  TEST_ASSERT_FALSE(pattern.init_rect(10, 190, 10, 190, 6, lh, lw, 0));     // No filament diameter
  TEST_ASSERT_FALSE(pattern.init_rect(10, 60, 10, 190, 6, lh, lw, fil));    // Too narrow for purge
  TEST_ASSERT_FALSE(pattern.init_rect(10, 190, 10, 20, 6, lh, lw, fil));    // Rows too close for patch
  TEST_ASSERT_FALSE(pattern.init_round(0, 0, 40, 6, lh, lw, fil));          // Round bed too small
}
