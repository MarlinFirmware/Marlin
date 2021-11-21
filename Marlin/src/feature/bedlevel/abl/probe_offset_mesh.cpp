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

#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(PROBE_OFFSET_MESH)

#include "../../../module/planner.h"
#include "../bedlevel.h"
#include "probe_offset_mesh.h"

probe_offset_mesh_t z_offset_mesh;
xy_pos_t z_offset_mesh_grid_spacing;
xy_float_t z_offset_mesh_grid_factor;

void print_z_offset_grid() {
  SERIAL_ECHOLNPGM("Z-Offset Grid:");
  print_2d_array(PROBE_OFFSET_MESH_GRID_MAX_POINTS_X, PROBE_OFFSET_MESH_GRID_MAX_POINTS_Y, 3,
    [](const uint8_t ix, const uint8_t iy) { return z_offset_mesh[ix][iy]; }
  );
}


#define POM_BG_SPACING(A) z_offset_mesh_grid_spacing.A
#define POM_BG_FACTOR(A)  z_offset_mesh_grid_factor.A
#define POM_BG_POINTS_X   GRID_MAX_POINTS_X
#define POM_BG_POINTS_Y   GRID_MAX_POINTS_Y
#define POM_BG_GRID(X,Y)  z_offset_mesh[X][Y]

// Refresh after other values have been updated
void refresh_probe_offset_mesh() {
  z_offset_mesh_grid_factor = z_offset_mesh_grid_spacing.reciprocal();
}

// Get the Z adjustment for non-linear bed leveling
float z_offset_mesh_from_raw_position(const xy_pos_t &raw) {

  static float z1, d2, z3, d4, L, D;

  static xy_pos_t prev { -999.999, -999.999 }, ratio;

  // Whole units for the grid line indices. Constrained within bounds.
  static xy_int8_t thisg, nextg, lastg { -99, -99 };

  // XY relative to the probed area
  xy_pos_t rel = raw - bilinear_start.asFloat();

  #if ENABLED(EXTRAPOLATE_BEYOND_GRID)
    #define FAR_EDGE_OR_BOX 2   // Keep using the last grid box
  #else
    #define FAR_EDGE_OR_BOX 1   // Just use the grid far edge
  #endif

  if (prev.x != rel.x) {
    prev.x = rel.x;
    ratio.x = rel.x * POM_BG_FACTOR(x);
    const float gx = constrain(FLOOR(ratio.x), 0, POM_BG_POINTS_X - (FAR_EDGE_OR_BOX));
    ratio.x -= gx;      // Subtract whole to get the ratio within the grid box

    #if DISABLED(EXTRAPOLATE_BEYOND_GRID)
      // Beyond the grid maintain height at grid edges
      NOLESS(ratio.x, 0); // Never <0 (>1 is ok when nextg.x==thisg.x)
    #endif

    thisg.x = gx;
    nextg.x = _MIN(thisg.x + 1, POM_BG_POINTS_X - 1);
  }

  if (prev.y != rel.y || lastg.x != thisg.x) {

    if (prev.y != rel.y) {
      prev.y = rel.y;
      ratio.y = rel.y * POM_BG_FACTOR(y);
      const float gy = constrain(FLOOR(ratio.y), 0, POM_BG_POINTS_Y - (FAR_EDGE_OR_BOX));
      ratio.y -= gy;

      #if DISABLED(EXTRAPOLATE_BEYOND_GRID)
        // Beyond the grid maintain height at grid edges
        NOLESS(ratio.y, 0); // Never < 0.0. (> 1.0 is ok when nextg.y==thisg.y.)
      #endif

      thisg.y = gy;
      nextg.y = _MIN(thisg.y + 1, POM_BG_POINTS_Y - 1);
    }

    if (lastg != thisg) {
      lastg = thisg;
      // Z at the box corners
      z1 = POM_BG_GRID(thisg.x, thisg.y);       // left-front
      d2 = POM_BG_GRID(thisg.x, nextg.y) - z1;  // left-back (delta)
      z3 = POM_BG_GRID(nextg.x, thisg.y);       // right-front
      d4 = POM_BG_GRID(nextg.x, nextg.y) - z3;  // right-back (delta)
    }

    // Bilinear interpolate. Needed since rel.y or thisg.x has changed.
                L = z1 + d2 * ratio.y;   // Linear interp. LF -> LB
    const float R = z3 + d4 * ratio.y;   // Linear interp. RF -> RB

    D = R - L;
  }

  const float offset = L + ratio.x * D;   // the offset almost always changes

  /*
  static float last_offset = 0;
  if (ABS(last_offset - offset) > 0.2) {
    SERIAL_ECHOLNPGM("Sudden Shift at x=", rel.x, " / ", bilinear_grid_spacing.x, " -> thisg.x=", thisg.x);
    SERIAL_ECHOLNPGM(" y=", rel.y, " / ", bilinear_grid_spacing.y, " -> thisg.y=", thisg.y);
    SERIAL_ECHOLNPGM(" ratio.x=", ratio.x, " ratio.y=", ratio.y);
    SERIAL_ECHOLNPGM(" z1=", z1, " z2=", z2, " z3=", z3, " z4=", z4);
    SERIAL_ECHOLNPGM(" L=", L, " R=", R, " offset=", offset);
  }
  last_offset = offset;
  //*/

  return offset;
}

#endif //PROBE_OFFSET_MESH