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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include "../../../inc/MarlinConfig.h"

#if ENABLED(AUTO_BED_LEVELING_UBL)

#include "../bedlevel.h"
#include "../../../module/planner.h"
#include "../../../module/stepper.h"
#include "../../../module/motion.h"

#if ENABLED(DELTA)
  #include "../../../module/delta.h"
#endif

#include "../../../MarlinCore.h"
#include <math.h>

#if !UBL_SEGMENTED

  void unified_bed_leveling::line_to_destination_cartesian(const feedRate_t &scaled_fr_mm_s, const uint8_t extruder) {
    /**
     * Much of the nozzle movement will be within the same cell. So we will do as little computation
     * as possible to determine if this is the case. If this move is within the same cell, we will
     * just do the required Z-Height correction, call the Planner's buffer_line() routine, and leave
     */
    #if HAS_POSITION_MODIFIERS
      xyze_pos_t start = current_position, end = destination;
      planner.apply_modifiers(start);
      planner.apply_modifiers(end);
    #else
      const xyze_pos_t &start = current_position, &end = destination;
    #endif

    const xy_int8_t istart = cell_indexes(start), iend = cell_indexes(end);

    // A move within the same cell needs no splitting
    if (istart == iend) {

      // For a move off the bed, use a constant Z raise
      if (!WITHIN(iend.x, 0, GRID_MAX_POINTS_X - 1) || !WITHIN(iend.y, 0, GRID_MAX_POINTS_Y - 1)) {

        // Note: There is no Z Correction in this case. We are off the grid and don't know what
        // a reasonable correction would be.  If the user has specified a UBL_Z_RAISE_WHEN_OFF_MESH
        // value, that will be used instead of a calculated (Bi-Linear interpolation) correction.

        #ifdef UBL_Z_RAISE_WHEN_OFF_MESH
          end.z += UBL_Z_RAISE_WHEN_OFF_MESH;
        #endif
        planner.buffer_segment(end, scaled_fr_mm_s, extruder);
        current_position = destination;
        return;
      }

      FINAL_MOVE:

      // The distance is always MESH_X_DIST so multiply by the constant reciprocal.
      const float xratio = (end.x - mesh_index_to_xpos(iend.x)) * RECIPROCAL(MESH_X_DIST);

      float z1, z2;
      if (iend.x >= GRID_MAX_POINTS_X - 1)
        z1 = z2 = 0.0;
      else {
        z1 = z_values[iend.x    ][iend.y    ] + xratio *
            (z_values[iend.x + 1][iend.y    ] - z_values[iend.x][iend.y    ]),
        z2 = z_values[iend.x    ][iend.y + 1] + xratio *
            (z_values[iend.x + 1][iend.y + 1] - z_values[iend.x][iend.y + 1]);
      }

      // X cell-fraction done. Interpolate the two Z offsets with the Y fraction for the final Z offset.
      const float yratio = (end.y - mesh_index_to_ypos(iend.y)) * RECIPROCAL(MESH_Y_DIST),
                  z0 = iend.y < GRID_MAX_POINTS_Y - 1 ? (z1 + (z2 - z1) * yratio) * planner.fade_scaling_factor_for_z(end.z) : 0.0;

      // Undefined parts of the Mesh in z_values[][] are NAN.
      // Replace NAN corrections with 0.0 to prevent NAN propagation.
      if (!isnan(z0)) end.z += z0;
      planner.buffer_segment(end, scaled_fr_mm_s, extruder);
      current_position = destination;
      return;
    }

    /**
     * Past this point the move is known to cross one or more mesh lines. Check for the most common
     * case - crossing only one X or Y line - after details are worked out to reduce computation.
     */

    const xy_float_t dist = end - start;
    const xy_bool_t neg { dist.x < 0, dist.y < 0 };
    const xy_int8_t ineg { int8_t(neg.x), int8_t(neg.y) };
    const xy_float_t sign { neg.x ? -1.0f : 1.0f, neg.y ? -1.0f : 1.0f };
    const xy_int8_t iadd { int8_t(iend.x == istart.x ? 0 : sign.x), int8_t(iend.y == istart.y ? 0 : sign.y) };

    /**
     * Compute the extruder scaling factor for each partial move, checking for
     * zero-length moves that would result in an infinite scaling factor.
     * A float divide is required for this, but then it just multiplies.
     * Also select a scaling factor based on the larger of the X and Y
     * components. The larger of the two is used to preserve precision.
     */

    const xy_float_t ad = sign * dist;
    const bool use_x_dist = ad.x > ad.y;

    float on_axis_distance = use_x_dist ? dist.x : dist.y,
          e_position = end.e - start.e,
          z_position = end.z - start.z;

    const float e_normalized_dist = e_position / on_axis_distance, // Allow divide by zero
                z_normalized_dist = z_position / on_axis_distance;

    xy_int8_t icell = istart;

    const float ratio = dist.y / dist.x,        // Allow divide by zero
                c = start.y - ratio * start.x;

    const bool inf_normalized_flag = isinf(e_normalized_dist),
               inf_ratio_flag = isinf(ratio);

    /**
     * Handle vertical lines that stay within one column.
     * These need not be perfectly vertical.
     */
    if (iadd.x == 0) {        // Vertical line?
      icell.y += ineg.y;      // Line going down? Just go to the bottom.
      while (icell.y != iend.y + ineg.y) {
        icell.y += iadd.y;
        const float next_mesh_line_y = mesh_index_to_ypos(icell.y);

        /**
         * Skip the calculations for an infinite slope.
         * For others the next X is the same so this can continue.
         * Calculate X at the next Y mesh line.
         */
        const float rx = inf_ratio_flag ? start.x : (next_mesh_line_y - c) / ratio;

        float z0 = z_correction_for_x_on_horizontal_mesh_line(rx, icell.x, icell.y)
                   * planner.fade_scaling_factor_for_z(end.z);

        // Undefined parts of the Mesh in z_values[][] are NAN.
        // Replace NAN corrections with 0.0 to prevent NAN propagation.
        if (isnan(z0)) z0 = 0.0;

        const float ry = mesh_index_to_ypos(icell.y);

        /**
         * Without this check, it's possible to generate a zero length move, as in the case where
         * the line is heading down, starting exactly on a mesh line boundary. Since this is rare
         * it might be fine to remove this check and let planner.buffer_segment() filter it out.
         */
        if (ry != start.y) {
          if (!inf_normalized_flag) { // fall-through faster than branch
            on_axis_distance = use_x_dist ? rx - start.x : ry - start.y;
            e_position = start.e + on_axis_distance * e_normalized_dist;
            z_position = start.z + on_axis_distance * z_normalized_dist;
          }
          else {
            e_position = end.e;
            z_position = end.z;
          }

          planner.buffer_segment(rx, ry, z_position + z0, e_position, scaled_fr_mm_s, extruder);
        } //else printf("FIRST MOVE PRUNED  ");
      }

      // At the final destination? Usually not, but when on a Y Mesh Line it's completed.
      if (xy_pos_t(current_position) != xy_pos_t(end))
        goto FINAL_MOVE;

      current_position = destination;
      return;
    }

    /**
     * Handle horizontal lines that stay within one row.
     * These need not be perfectly horizontal.
     */
    if (iadd.y == 0) {      // Horizontal line?
      icell.x += ineg.x;     // Heading left? Just go to the left edge of the cell for the first move.
      while (icell.x != iend.x + ineg.x) {
        icell.x += iadd.x;
        const float rx = mesh_index_to_xpos(icell.x);
        const float ry = ratio * rx + c;    // Calculate Y at the next X mesh line

        float z0 = z_correction_for_y_on_vertical_mesh_line(ry, icell.x, icell.y)
                     * planner.fade_scaling_factor_for_z(end.z);

        // Undefined parts of the Mesh in z_values[][] are NAN.
        // Replace NAN corrections with 0.0 to prevent NAN propagation.
        if (isnan(z0)) z0 = 0.0;

        /**
         * Without this check, it's possible to generate a zero length move, as in the case where
         * the line is heading left, starting exactly on a mesh line boundary. Since this is rare
         * it might be fine to remove this check and let planner.buffer_segment() filter it out.
         */
        if (rx != start.x) {
          if (!inf_normalized_flag) {
            on_axis_distance = use_x_dist ? rx - start.x : ry - start.y;
            e_position = start.e + on_axis_distance * e_normalized_dist;  // is based on X or Y because this is a horizontal move
            z_position = start.z + on_axis_distance * z_normalized_dist;
          }
          else {
            e_position = end.e;
            z_position = end.z;
          }

          if (!planner.buffer_segment(rx, ry, z_position + z0, e_position, scaled_fr_mm_s, extruder))
            break;
        } //else printf("FIRST MOVE PRUNED  ");
      }

      if (xy_pos_t(current_position) != xy_pos_t(end))
        goto FINAL_MOVE;

      current_position = destination;
      return;
    }

    /**
     *
     * Generic case of a line crossing both X and Y Mesh lines.
     *
     */

    xy_int8_t cnt = (istart - iend).ABS();

    icell += ineg;

    while (cnt) {

      const float next_mesh_line_x = mesh_index_to_xpos(icell.x + iadd.x),
                  next_mesh_line_y = mesh_index_to_ypos(icell.y + iadd.y),
                  ry = ratio * next_mesh_line_x + c,    // Calculate Y at the next X mesh line
                  rx = (next_mesh_line_y - c) / ratio;  // Calculate X at the next Y mesh line
                                                        // (No need to worry about ratio == 0.
                                                        //  In that case, it was already detected
                                                        //  as a vertical line move above.)

      if (neg.x == (rx > next_mesh_line_x)) { // Check if we hit the Y line first
        // Yes!  Crossing a Y Mesh Line next
        float z0 = z_correction_for_x_on_horizontal_mesh_line(rx, icell.x - ineg.x, icell.y + iadd.y)
                   * planner.fade_scaling_factor_for_z(end.z);

        // Undefined parts of the Mesh in z_values[][] are NAN.
        // Replace NAN corrections with 0.0 to prevent NAN propagation.
        if (isnan(z0)) z0 = 0.0;

        if (!inf_normalized_flag) {
          on_axis_distance = use_x_dist ? rx - start.x : next_mesh_line_y - start.y;
          e_position = start.e + on_axis_distance * e_normalized_dist;
          z_position = start.z + on_axis_distance * z_normalized_dist;
        }
        else {
          e_position = end.e;
          z_position = end.z;
        }
        if (!planner.buffer_segment(rx, next_mesh_line_y, z_position + z0, e_position, scaled_fr_mm_s, extruder))
          break;
        icell.y += iadd.y;
        cnt.y--;
      }
      else {
        // Yes!  Crossing a X Mesh Line next
        float z0 = z_correction_for_y_on_vertical_mesh_line(ry, icell.x + iadd.x, icell.y - ineg.y)
                   * planner.fade_scaling_factor_for_z(end.z);

        // Undefined parts of the Mesh in z_values[][] are NAN.
        // Replace NAN corrections with 0.0 to prevent NAN propagation.
        if (isnan(z0)) z0 = 0.0;

        if (!inf_normalized_flag) {
          on_axis_distance = use_x_dist ? next_mesh_line_x - start.x : ry - start.y;
          e_position = start.e + on_axis_distance * e_normalized_dist;
          z_position = start.z + on_axis_distance * z_normalized_dist;
        }
        else {
          e_position = end.e;
          z_position = end.z;
        }

        if (!planner.buffer_segment(next_mesh_line_x, ry, z_position + z0, e_position, scaled_fr_mm_s, extruder))
          break;
        icell.x += iadd.x;
        cnt.x--;
      }

      if (cnt.x < 0 || cnt.y < 0) break; // Too far! Exit the loop and go to FINAL_MOVE
    }

    if (xy_pos_t(current_position) != xy_pos_t(end))
      goto FINAL_MOVE;

    current_position = destination;
  }

#else // UBL_SEGMENTED

  #if IS_SCARA
    #define DELTA_SEGMENT_MIN_LENGTH 0.25 // SCARA minimum segment size is 0.25mm
  #elif ENABLED(DELTA)
    #define DELTA_SEGMENT_MIN_LENGTH 0.10 // mm (still subject to DELTA_SEGMENTS_PER_SECOND)
  #else // CARTESIAN
    #ifdef LEVELED_SEGMENT_LENGTH
      #define DELTA_SEGMENT_MIN_LENGTH LEVELED_SEGMENT_LENGTH
    #else
      #define DELTA_SEGMENT_MIN_LENGTH 1.00 // mm (similar to G2/G3 arc segmentation)
    #endif
  #endif

  /**
   * Prepare a segmented linear move for DELTA/SCARA/CARTESIAN with UBL and FADE semantics.
   * This calls planner.buffer_segment multiple times for small incremental moves.
   * Returns true if did NOT move, false if moved (requires current_position update).
   */

  bool _O2 unified_bed_leveling::line_to_destination_segmented(const feedRate_t &scaled_fr_mm_s) {

    if (!position_is_reachable(destination))  // fail if moving outside reachable boundary
      return true;                            // did not move, so current_position still accurate

    const xyze_pos_t total = destination - current_position;

    const float cart_xy_mm_2 = HYPOT2(total.x, total.y),
                cart_xy_mm = SQRT(cart_xy_mm_2);                                     // Total XY distance

    #if IS_KINEMATIC
      const float seconds = cart_xy_mm / scaled_fr_mm_s;                             // Duration of XY move at requested rate
      uint16_t segments = LROUND(delta_segments_per_second * seconds),               // Preferred number of segments for distance @ feedrate
               seglimit = LROUND(cart_xy_mm * RECIPROCAL(DELTA_SEGMENT_MIN_LENGTH)); // Number of segments at minimum segment length
      NOMORE(segments, seglimit);                                                    // Limit to minimum segment length (fewer segments)
    #else
      uint16_t segments = LROUND(cart_xy_mm * RECIPROCAL(DELTA_SEGMENT_MIN_LENGTH)); // Cartesian fixed segment length
    #endif

    NOLESS(segments, 1U);                                                            // Must have at least one segment
    const float inv_segments = 1.0f / segments,                                      // Reciprocal to save calculation
                segment_xyz_mm = SQRT(cart_xy_mm_2 + sq(total.z)) * inv_segments;    // Length of each segment

    #if ENABLED(SCARA_FEEDRATE_SCALING)
      const float inv_duration = scaled_fr_mm_s / segment_xyz_mm;
    #endif

    xyze_float_t diff = total * inv_segments;

    // Note that E segment distance could vary slightly as z mesh height
    // changes for each segment, but small enough to ignore.

    xyze_pos_t raw = current_position;

    // Just do plain segmentation if UBL is inactive or the target is above the fade height
    if (!planner.leveling_active || !planner.leveling_active_at_z(destination.z)) {
      while (--segments) {
        raw += diff;
        planner.buffer_line(raw, scaled_fr_mm_s, active_extruder, segment_xyz_mm
          #if ENABLED(SCARA_FEEDRATE_SCALING)
            , inv_duration
          #endif
        );
      }
      planner.buffer_line(destination, scaled_fr_mm_s, active_extruder, segment_xyz_mm
        #if ENABLED(SCARA_FEEDRATE_SCALING)
          , inv_duration
        #endif
      );
      return false; // Did not set current from destination
    }

    // Otherwise perform per-segment leveling

    #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
      const float fade_scaling_factor = planner.fade_scaling_factor_for_z(destination.z);
    #endif

    // Move to first segment destination
    raw += diff;

    for (;;) {  // for each mesh cell encountered during the move

      // Compute mesh cell invariants that remain constant for all segments within cell.
      // Note for cell index, if point is outside the mesh grid (in MESH_INSET perimeter)
      // the bilinear interpolation from the adjacent cell within the mesh will still work.
      // Inner loop will exit each time (because out of cell bounds) but will come back
      // in top of loop and again re-find same adjacent cell and use it, just less efficient
      // for mesh inset area.

      xy_int8_t icell = {
        int8_t((raw.x - (MESH_MIN_X)) * RECIPROCAL(MESH_X_DIST)),
        int8_t((raw.y - (MESH_MIN_Y)) * RECIPROCAL(MESH_Y_DIST))
      };
      LIMIT(icell.x, 0, (GRID_MAX_POINTS_X) - 1);
      LIMIT(icell.y, 0, (GRID_MAX_POINTS_Y) - 1);

      float z_x0y0 = z_values[icell.x  ][icell.y  ],  // z at lower left corner
            z_x1y0 = z_values[icell.x+1][icell.y  ],  // z at upper left corner
            z_x0y1 = z_values[icell.x  ][icell.y+1],  // z at lower right corner
            z_x1y1 = z_values[icell.x+1][icell.y+1];  // z at upper right corner

      if (isnan(z_x0y0)) z_x0y0 = 0;              // ideally activating planner.leveling_active (G29 A)
      if (isnan(z_x1y0)) z_x1y0 = 0;              //   should refuse if any invalid mesh points
      if (isnan(z_x0y1)) z_x0y1 = 0;              //   in order to avoid isnan tests per cell,
      if (isnan(z_x1y1)) z_x1y1 = 0;              //   thus guessing zero for undefined points

      const xy_pos_t pos = { mesh_index_to_xpos(icell.x), mesh_index_to_ypos(icell.y) };
      xy_pos_t cell = raw - pos;

      const float z_xmy0 = (z_x1y0 - z_x0y0) * RECIPROCAL(MESH_X_DIST),   // z slope per x along y0 (lower left to lower right)
                  z_xmy1 = (z_x1y1 - z_x0y1) * RECIPROCAL(MESH_X_DIST);   // z slope per x along y1 (upper left to upper right)

            float z_cxy0 = z_x0y0 + z_xmy0 * cell.x;        // z height along y0 at cell.x (changes for each cell.x in cell)

      const float z_cxy1 = z_x0y1 + z_xmy1 * cell.x,        // z height along y1 at cell.x
                  z_cxyd = z_cxy1 - z_cxy0;                 // z height difference along cell.x from y0 to y1

            float z_cxym = z_cxyd * RECIPROCAL(MESH_Y_DIST); // z slope per y along cell.x from pos.y to y1 (changes for each cell.x in cell)

      //    float z_cxcy = z_cxy0 + z_cxym * cell.y;        // interpolated mesh z height along cell.x at cell.y (do inside the segment loop)

      // As subsequent segments step through this cell, the z_cxy0 intercept will change
      // and the z_cxym slope will change, both as a function of cell.x within the cell, and
      // each change by a constant for fixed segment lengths.

      const float z_sxy0 = z_xmy0 * diff.x,                                       // per-segment adjustment to z_cxy0
                  z_sxym = (z_xmy1 - z_xmy0) * RECIPROCAL(MESH_Y_DIST) * diff.x;  // per-segment adjustment to z_cxym

      for (;;) {  // for all segments within this mesh cell

        if (--segments == 0) raw = destination;     // if this is last segment, use destination for exact

        const float z_cxcy = (z_cxy0 + z_cxym * cell.y) // interpolated mesh z height along cell.x at cell.y
          #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
            * fade_scaling_factor                   // apply fade factor to interpolated mesh height
          #endif
        ;

        planner.buffer_line(raw.x, raw.y, raw.z + z_cxcy, raw.e, scaled_fr_mm_s, active_extruder, segment_xyz_mm
          #if ENABLED(SCARA_FEEDRATE_SCALING)
            , inv_duration
          #endif
        );

        if (segments == 0)                        // done with last segment
          return false;                           // didn't set current from destination

        raw += diff;
        cell += diff;

        if (!WITHIN(cell.x, 0, MESH_X_DIST) || !WITHIN(cell.y, 0, MESH_Y_DIST))    // done within this cell, break to next
          break;

        // Next segment still within same mesh cell, adjust the per-segment
        // slope and intercept to compute next z height.

        z_cxy0 += z_sxy0;   // adjust z_cxy0 by per-segment z_sxy0
        z_cxym += z_sxym;   // adjust z_cxym by per-segment z_sxym

      } // segment loop
    } // cell loop

    return false; // caller will update current_position
  }

#endif // UBL_SEGMENTED

#endif // AUTO_BED_LEVELING_UBL
