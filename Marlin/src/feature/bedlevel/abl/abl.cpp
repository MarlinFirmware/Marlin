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

#include "../../../inc/MarlinConfig.h"

#if ENABLED(AUTO_BED_LEVELING_BILINEAR)

#include "abl.h"

#include "../../../module/motion.h"

int bilinear_grid_spacing[2], bilinear_start[2];
float bilinear_grid_factor[2],
      z_values[GRID_MAX_POINTS_X][GRID_MAX_POINTS_Y];

/**
 * Extrapolate a single point from its neighbors
 */
static void extrapolate_one_point(const uint8_t x, const uint8_t y, const int8_t xdir, const int8_t ydir) {
  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) {
      SERIAL_ECHOPGM("Extrapolate [");
      if (x < 10) SERIAL_CHAR(' ');
      SERIAL_ECHO((int)x);
      SERIAL_CHAR(xdir ? (xdir > 0 ? '+' : '-') : ' ');
      SERIAL_CHAR(' ');
      if (y < 10) SERIAL_CHAR(' ');
      SERIAL_ECHO((int)y);
      SERIAL_CHAR(ydir ? (ydir > 0 ? '+' : '-') : ' ');
      SERIAL_CHAR(']');
    }
  #endif
  if (!isnan(z_values[x][y])) {
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPGM(" (done)");
    #endif
    return;  // Don't overwrite good values.
  }
  SERIAL_EOL();

  // Get X neighbors, Y neighbors, and XY neighbors
  const uint8_t x1 = x + xdir, y1 = y + ydir, x2 = x1 + xdir, y2 = y1 + ydir;
  float a1 = z_values[x1][y ], a2 = z_values[x2][y ],
        b1 = z_values[x ][y1], b2 = z_values[x ][y2],
        c1 = z_values[x1][y1], c2 = z_values[x2][y2];

  // Treat far unprobed points as zero, near as equal to far
  if (isnan(a2)) a2 = 0.0;
  if (isnan(a1)) a1 = a2;
  if (isnan(b2)) b2 = 0.0;
  if (isnan(b1)) b1 = b2;
  if (isnan(c2)) c2 = 0.0;
  if (isnan(c1)) c1 = c2;

  const float a = 2 * a1 - a2, b = 2 * b1 - b2, c = 2 * c1 - c2;

  // Take the average instead of the median
  z_values[x][y] = (a + b + c) / 3.0;

  // Median is robust (ignores outliers).
  // z_values[x][y] = (a < b) ? ((b < c) ? b : (c < a) ? a : c)
  //                                : ((c < b) ? b : (a < c) ? a : c);
}

//Enable this if your SCARA uses 180° of total area
//#define EXTRAPOLATE_FROM_EDGE

#if ENABLED(EXTRAPOLATE_FROM_EDGE)
  #if GRID_MAX_POINTS_X < GRID_MAX_POINTS_Y
    #define HALF_IN_X
  #elif GRID_MAX_POINTS_Y < GRID_MAX_POINTS_X
    #define HALF_IN_Y
  #endif
#endif

/**
 * Fill in the unprobed points (corners of circular print surface)
 * using linear extrapolation, away from the center.
 */
void extrapolate_unprobed_bed_level() {
  #ifdef HALF_IN_X
    constexpr uint8_t ctrx2 = 0, xlen = GRID_MAX_POINTS_X - 1;
  #else
    constexpr uint8_t ctrx1 = (GRID_MAX_POINTS_X - 1) / 2, // left-of-center
                      ctrx2 = (GRID_MAX_POINTS_X) / 2,     // right-of-center
                      xlen = ctrx1;
  #endif

  #ifdef HALF_IN_Y
    constexpr uint8_t ctry2 = 0, ylen = GRID_MAX_POINTS_Y - 1;
  #else
    constexpr uint8_t ctry1 = (GRID_MAX_POINTS_Y - 1) / 2, // top-of-center
                      ctry2 = (GRID_MAX_POINTS_Y) / 2,     // bottom-of-center
                      ylen = ctry1;
  #endif

  for (uint8_t xo = 0; xo <= xlen; xo++)
    for (uint8_t yo = 0; yo <= ylen; yo++) {
      uint8_t x2 = ctrx2 + xo, y2 = ctry2 + yo;
      #ifndef HALF_IN_X
        const uint8_t x1 = ctrx1 - xo;
      #endif
      #ifndef HALF_IN_Y
        const uint8_t y1 = ctry1 - yo;
        #ifndef HALF_IN_X
          extrapolate_one_point(x1, y1, +1, +1);   //  left-below + +
        #endif
        extrapolate_one_point(x2, y1, -1, +1);     // right-below - +
      #endif
      #ifndef HALF_IN_X
        extrapolate_one_point(x1, y2, +1, -1);     //  left-above + -
      #endif
      extrapolate_one_point(x2, y2, -1, -1);       // right-above - -
    }

}

void print_bilinear_leveling_grid() {
  SERIAL_ECHOLNPGM("Bilinear Leveling Grid:");
  print_2d_array(GRID_MAX_POINTS_X, GRID_MAX_POINTS_Y, 3,
    [](const uint8_t ix, const uint8_t iy) { return z_values[ix][iy]; }
  );
}

#if ENABLED(ABL_BILINEAR_SUBDIVISION)

  #define ABL_GRID_POINTS_VIRT_X (GRID_MAX_POINTS_X - 1) * (BILINEAR_SUBDIVISIONS) + 1
  #define ABL_GRID_POINTS_VIRT_Y (GRID_MAX_POINTS_Y - 1) * (BILINEAR_SUBDIVISIONS) + 1
  #define ABL_TEMP_POINTS_X (GRID_MAX_POINTS_X + 2)
  #define ABL_TEMP_POINTS_Y (GRID_MAX_POINTS_Y + 2)
  float z_values_virt[ABL_GRID_POINTS_VIRT_X][ABL_GRID_POINTS_VIRT_Y];
  int bilinear_grid_spacing_virt[2] = { 0 };
  float bilinear_grid_factor_virt[2] = { 0 };

  void print_bilinear_leveling_grid_virt() {
    SERIAL_ECHOLNPGM("Subdivided with CATMULL ROM Leveling Grid:");
    print_2d_array(ABL_GRID_POINTS_VIRT_X, ABL_GRID_POINTS_VIRT_Y, 5,
      [](const uint8_t ix, const uint8_t iy) { return z_values_virt[ix][iy]; }
    );
  }

  #define LINEAR_EXTRAPOLATION(E, I) ((E) * 2 - (I))
  float bed_level_virt_coord(const uint8_t x, const uint8_t y) {
    uint8_t ep = 0, ip = 1;
    if (!x || x == ABL_TEMP_POINTS_X - 1) {
      if (x) {
        ep = GRID_MAX_POINTS_X - 1;
        ip = GRID_MAX_POINTS_X - 2;
      }
      if (WITHIN(y, 1, ABL_TEMP_POINTS_Y - 2))
        return LINEAR_EXTRAPOLATION(
          z_values[ep][y - 1],
          z_values[ip][y - 1]
        );
      else
        return LINEAR_EXTRAPOLATION(
          bed_level_virt_coord(ep + 1, y),
          bed_level_virt_coord(ip + 1, y)
        );
    }
    if (!y || y == ABL_TEMP_POINTS_Y - 1) {
      if (y) {
        ep = GRID_MAX_POINTS_Y - 1;
        ip = GRID_MAX_POINTS_Y - 2;
      }
      if (WITHIN(x, 1, ABL_TEMP_POINTS_X - 2))
        return LINEAR_EXTRAPOLATION(
          z_values[x - 1][ep],
          z_values[x - 1][ip]
        );
      else
        return LINEAR_EXTRAPOLATION(
          bed_level_virt_coord(x, ep + 1),
          bed_level_virt_coord(x, ip + 1)
        );
    }
    return z_values[x - 1][y - 1];
  }

  static float bed_level_virt_cmr(const float p[4], const uint8_t i, const float t) {
    return (
        p[i-1] * -t * sq(1 - t)
      + p[i]   * (2 - 5 * sq(t) + 3 * t * sq(t))
      + p[i+1] * t * (1 + 4 * t - 3 * sq(t))
      - p[i+2] * sq(t) * (1 - t)
    ) * 0.5;
  }

  static float bed_level_virt_2cmr(const uint8_t x, const uint8_t y, const float &tx, const float &ty) {
    float row[4], column[4];
    for (uint8_t i = 0; i < 4; i++) {
      for (uint8_t j = 0; j < 4; j++) {
        column[j] = bed_level_virt_coord(i + x - 1, j + y - 1);
      }
      row[i] = bed_level_virt_cmr(column, 1, ty);
    }
    return bed_level_virt_cmr(row, 1, tx);
  }

  void bed_level_virt_interpolate() {
    bilinear_grid_spacing_virt[X_AXIS] = bilinear_grid_spacing[X_AXIS] / (BILINEAR_SUBDIVISIONS);
    bilinear_grid_spacing_virt[Y_AXIS] = bilinear_grid_spacing[Y_AXIS] / (BILINEAR_SUBDIVISIONS);
    bilinear_grid_factor_virt[X_AXIS] = RECIPROCAL(bilinear_grid_spacing_virt[X_AXIS]);
    bilinear_grid_factor_virt[Y_AXIS] = RECIPROCAL(bilinear_grid_spacing_virt[Y_AXIS]);
    for (uint8_t y = 0; y < GRID_MAX_POINTS_Y; y++)
      for (uint8_t x = 0; x < GRID_MAX_POINTS_X; x++)
        for (uint8_t ty = 0; ty < BILINEAR_SUBDIVISIONS; ty++)
          for (uint8_t tx = 0; tx < BILINEAR_SUBDIVISIONS; tx++) {
            if ((ty && y == GRID_MAX_POINTS_Y - 1) || (tx && x == GRID_MAX_POINTS_X - 1))
              continue;
            z_values_virt[x * (BILINEAR_SUBDIVISIONS) + tx][y * (BILINEAR_SUBDIVISIONS) + ty] =
              bed_level_virt_2cmr(
                x + 1,
                y + 1,
                (float)tx / (BILINEAR_SUBDIVISIONS),
                (float)ty / (BILINEAR_SUBDIVISIONS)
              );
          }
  }
#endif // ABL_BILINEAR_SUBDIVISION

// Refresh after other values have been updated
void refresh_bed_level() {
  bilinear_grid_factor[X_AXIS] = RECIPROCAL(bilinear_grid_spacing[X_AXIS]);
  bilinear_grid_factor[Y_AXIS] = RECIPROCAL(bilinear_grid_spacing[Y_AXIS]);
  #if ENABLED(ABL_BILINEAR_SUBDIVISION)
    bed_level_virt_interpolate();
  #endif
}

#if ENABLED(ABL_BILINEAR_SUBDIVISION)
  #define ABL_BG_SPACING(A) bilinear_grid_spacing_virt[A]
  #define ABL_BG_FACTOR(A)  bilinear_grid_factor_virt[A]
  #define ABL_BG_POINTS_X   ABL_GRID_POINTS_VIRT_X
  #define ABL_BG_POINTS_Y   ABL_GRID_POINTS_VIRT_Y
  #define ABL_BG_GRID(X,Y)  z_values_virt[X][Y]
#else
  #define ABL_BG_SPACING(A) bilinear_grid_spacing[A]
  #define ABL_BG_FACTOR(A)  bilinear_grid_factor[A]
  #define ABL_BG_POINTS_X   GRID_MAX_POINTS_X
  #define ABL_BG_POINTS_Y   GRID_MAX_POINTS_Y
  #define ABL_BG_GRID(X,Y)  z_values[X][Y]
#endif

// Get the Z adjustment for non-linear bed leveling
float bilinear_z_offset(const float raw[XYZ]) {

  static float z1, d2, z3, d4, L, D, ratio_x, ratio_y,
               last_x = -999.999, last_y = -999.999;

  // Whole units for the grid line indices. Constrained within bounds.
  static int8_t gridx, gridy, nextx, nexty,
                last_gridx = -99, last_gridy = -99;

  // XY relative to the probed area
  const float rx = raw[X_AXIS] - bilinear_start[X_AXIS],
              ry = raw[Y_AXIS] - bilinear_start[Y_AXIS];

  #if ENABLED(EXTRAPOLATE_BEYOND_GRID)
    // Keep using the last grid box
    #define FAR_EDGE_OR_BOX 2
  #else
    // Just use the grid far edge
    #define FAR_EDGE_OR_BOX 1
  #endif

  if (last_x != rx) {
    last_x = rx;
    ratio_x = rx * ABL_BG_FACTOR(X_AXIS);
    const float gx = constrain(FLOOR(ratio_x), 0, ABL_BG_POINTS_X - FAR_EDGE_OR_BOX);
    ratio_x -= gx;      // Subtract whole to get the ratio within the grid box

    #if DISABLED(EXTRAPOLATE_BEYOND_GRID)
      // Beyond the grid maintain height at grid edges
      NOLESS(ratio_x, 0); // Never < 0.0. (> 1.0 is ok when nextx==gridx.)
    #endif

    gridx = gx;
    nextx = min(gridx + 1, ABL_BG_POINTS_X - 1);
  }

  if (last_y != ry || last_gridx != gridx) {

    if (last_y != ry) {
      last_y = ry;
      ratio_y = ry * ABL_BG_FACTOR(Y_AXIS);
      const float gy = constrain(FLOOR(ratio_y), 0, ABL_BG_POINTS_Y - FAR_EDGE_OR_BOX);
      ratio_y -= gy;

      #if DISABLED(EXTRAPOLATE_BEYOND_GRID)
        // Beyond the grid maintain height at grid edges
        NOLESS(ratio_y, 0); // Never < 0.0. (> 1.0 is ok when nexty==gridy.)
      #endif

      gridy = gy;
      nexty = min(gridy + 1, ABL_BG_POINTS_Y - 1);
    }

    if (last_gridx != gridx || last_gridy != gridy) {
      last_gridx = gridx;
      last_gridy = gridy;
      // Z at the box corners
      z1 = ABL_BG_GRID(gridx, gridy);       // left-front
      d2 = ABL_BG_GRID(gridx, nexty) - z1;  // left-back (delta)
      z3 = ABL_BG_GRID(nextx, gridy);       // right-front
      d4 = ABL_BG_GRID(nextx, nexty) - z3;  // right-back (delta)
    }

    // Bilinear interpolate. Needed since ry or gridx has changed.
                L = z1 + d2 * ratio_y;   // Linear interp. LF -> LB
    const float R = z3 + d4 * ratio_y;   // Linear interp. RF -> RB

    D = R - L;
  }

  const float offset = L + ratio_x * D;   // the offset almost always changes

  /*
  static float last_offset = 0;
  if (FABS(last_offset - offset) > 0.2) {
    SERIAL_ECHOPGM("Sudden Shift at ");
    SERIAL_ECHOPAIR("x=", rx);
    SERIAL_ECHOPAIR(" / ", bilinear_grid_spacing[X_AXIS]);
    SERIAL_ECHOLNPAIR(" -> gridx=", gridx);
    SERIAL_ECHOPAIR(" y=", ry);
    SERIAL_ECHOPAIR(" / ", bilinear_grid_spacing[Y_AXIS]);
    SERIAL_ECHOLNPAIR(" -> gridy=", gridy);
    SERIAL_ECHOPAIR(" ratio_x=", ratio_x);
    SERIAL_ECHOLNPAIR(" ratio_y=", ratio_y);
    SERIAL_ECHOPAIR(" z1=", z1);
    SERIAL_ECHOPAIR(" z2=", z2);
    SERIAL_ECHOPAIR(" z3=", z3);
    SERIAL_ECHOLNPAIR(" z4=", z4);
    SERIAL_ECHOPAIR(" L=", L);
    SERIAL_ECHOPAIR(" R=", R);
    SERIAL_ECHOLNPAIR(" offset=", offset);
  }
  last_offset = offset;
  //*/

  return offset;
}

#if IS_CARTESIAN && DISABLED(SEGMENT_LEVELED_MOVES)

  #define CELL_INDEX(A,V) ((V - bilinear_start[A##_AXIS]) * ABL_BG_FACTOR(A##_AXIS))

  /**
   * Prepare a bilinear-leveled linear move on Cartesian,
   * splitting the move where it crosses grid borders.
   */
  void bilinear_line_to_destination(const float fr_mm_s, uint16_t x_splits, uint16_t y_splits) {
    // Get current and destination cells for this line
    int cx1 = CELL_INDEX(X, current_position[X_AXIS]),
        cy1 = CELL_INDEX(Y, current_position[Y_AXIS]),
        cx2 = CELL_INDEX(X, destination[X_AXIS]),
        cy2 = CELL_INDEX(Y, destination[Y_AXIS]);
    cx1 = constrain(cx1, 0, ABL_BG_POINTS_X - 2);
    cy1 = constrain(cy1, 0, ABL_BG_POINTS_Y - 2);
    cx2 = constrain(cx2, 0, ABL_BG_POINTS_X - 2);
    cy2 = constrain(cy2, 0, ABL_BG_POINTS_Y - 2);

    // Start and end in the same cell? No split needed.
    if (cx1 == cx2 && cy1 == cy2) {
      buffer_line_to_destination(fr_mm_s);
      set_current_from_destination();
      return;
    }

    #define LINE_SEGMENT_END(A) (current_position[A ##_AXIS] + (destination[A ##_AXIS] - current_position[A ##_AXIS]) * normalized_dist)

    float normalized_dist, end[XYZE];
    const int8_t gcx = max(cx1, cx2), gcy = max(cy1, cy2);

    // Crosses on the X and not already split on this X?
    // The x_splits flags are insurance against rounding errors.
    if (cx2 != cx1 && TEST(x_splits, gcx)) {
      // Split on the X grid line
      CBI(x_splits, gcx);
      COPY(end, destination);
      destination[X_AXIS] = bilinear_start[X_AXIS] + ABL_BG_SPACING(X_AXIS) * gcx;
      normalized_dist = (destination[X_AXIS] - current_position[X_AXIS]) / (end[X_AXIS] - current_position[X_AXIS]);
      destination[Y_AXIS] = LINE_SEGMENT_END(Y);
    }
    // Crosses on the Y and not already split on this Y?
    else if (cy2 != cy1 && TEST(y_splits, gcy)) {
      // Split on the Y grid line
      CBI(y_splits, gcy);
      COPY(end, destination);
      destination[Y_AXIS] = bilinear_start[Y_AXIS] + ABL_BG_SPACING(Y_AXIS) * gcy;
      normalized_dist = (destination[Y_AXIS] - current_position[Y_AXIS]) / (end[Y_AXIS] - current_position[Y_AXIS]);
      destination[X_AXIS] = LINE_SEGMENT_END(X);
    }
    else {
      // Must already have been split on these border(s)
      // This should be a rare case.
      buffer_line_to_destination(fr_mm_s);
      set_current_from_destination();
      return;
    }

    destination[Z_AXIS] = LINE_SEGMENT_END(Z);
    destination[E_AXIS] = LINE_SEGMENT_END(E);

    // Do the split and look for more borders
    bilinear_line_to_destination(fr_mm_s, x_splits, y_splits);

    // Restore destination from stack
    COPY(destination, end);
    bilinear_line_to_destination(fr_mm_s, x_splits, y_splits);
  }

#endif // IS_CARTESIAN && !SEGMENT_LEVELED_MOVES

#endif // AUTO_BED_LEVELING_BILINEAR
