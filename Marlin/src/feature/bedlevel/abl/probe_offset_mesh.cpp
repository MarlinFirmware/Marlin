#include "../../../inc/MarlinConfigPre.h"

#include "../../../module/planner.h"
#include "../bedlevel.h"
#include "probe_offset_mesh.h"

bed_mesh_t z_offset_mesh;

void print_z_offset_grid() {
  SERIAL_ECHOLNPGM("Z-Offset Grid:");
  print_2d_array(GRID_MAX_POINTS_X, GRID_MAX_POINTS_Y, 3,
    [](const uint8_t ix, const uint8_t iy) { return z_offset_mesh[ix][iy]; }
  );
}

#if ENABLED(ABL_BILINEAR_SUBDIVISION)
  #define ABL_BG_SPACING(A) bilinear_grid_spacing_virt.A
  #define ABL_BG_FACTOR(A)  bilinear_grid_factor_virt.A
  #define ABL_BG_POINTS_X   ABL_GRID_POINTS_VIRT_X
  #define ABL_BG_POINTS_Y   ABL_GRID_POINTS_VIRT_Y
  #define ABL_BG_GRID(X,Y)  z_values_virt[X][Y]
#else
  #define ABL_BG_SPACING(A) bilinear_grid_spacing.A
  #define ABL_BG_FACTOR(A)  bilinear_grid_factor.A
  #define ABL_BG_POINTS_X   GRID_MAX_POINTS_X
  #define ABL_BG_POINTS_Y   GRID_MAX_POINTS_Y
  #define ABL_BG_GRID(X,Y)  z_offset_mesh[X][Y]
#endif

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
    ratio.x = rel.x * ABL_BG_FACTOR(x);
    const float gx = constrain(FLOOR(ratio.x), 0, ABL_BG_POINTS_X - (FAR_EDGE_OR_BOX));
    ratio.x -= gx;      // Subtract whole to get the ratio within the grid box

    #if DISABLED(EXTRAPOLATE_BEYOND_GRID)
      // Beyond the grid maintain height at grid edges
      NOLESS(ratio.x, 0); // Never <0 (>1 is ok when nextg.x==thisg.x)
    #endif

    thisg.x = gx;
    nextg.x = _MIN(thisg.x + 1, ABL_BG_POINTS_X - 1);
  }

  if (prev.y != rel.y || lastg.x != thisg.x) {

    if (prev.y != rel.y) {
      prev.y = rel.y;
      ratio.y = rel.y * ABL_BG_FACTOR(y);
      const float gy = constrain(FLOOR(ratio.y), 0, ABL_BG_POINTS_Y - (FAR_EDGE_OR_BOX));
      ratio.y -= gy;

      #if DISABLED(EXTRAPOLATE_BEYOND_GRID)
        // Beyond the grid maintain height at grid edges
        NOLESS(ratio.y, 0); // Never < 0.0. (> 1.0 is ok when nextg.y==thisg.y.)
      #endif

      thisg.y = gy;
      nextg.y = _MIN(thisg.y + 1, ABL_BG_POINTS_Y - 1);
    }

    if (lastg != thisg) {
      lastg = thisg;
      // Z at the box corners
      z1 = ABL_BG_GRID(thisg.x, thisg.y);       // left-front
      d2 = ABL_BG_GRID(thisg.x, nextg.y) - z1;  // left-back (delta)
      z3 = ABL_BG_GRID(nextg.x, thisg.y);       // right-front
      d4 = ABL_BG_GRID(nextg.x, nextg.y) - z3;  // right-back (delta)
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