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
#include "MarlinConfig.h"

#if ENABLED(AUTO_BED_LEVELING_UBL)

  #include "Marlin.h"
  #include "UBL.h"
  #include "planner.h"
  #include <avr/io.h>
  #include <math.h>

  extern void set_current_to_destination();
  extern void debug_current_and_destination(char *title);

  void ubl_line_to_destination(const float &x_end, const float &y_end, const float &z_end, const float &e_end, const float &feed_rate, uint8_t extruder) {

    int cell_start_xi, cell_start_yi, cell_dest_xi, cell_dest_yi,
        current_xi, current_yi,
        dxi, dyi, xi_cnt, yi_cnt;
    float x_start, y_start,
          x, y, z1, z2, z0 /*, z_optimized */,
          next_mesh_line_x, next_mesh_line_y, a0ma1diva2ma1,
          on_axis_distance, e_normalized_dist, e_position, e_start, z_normalized_dist, z_position, z_start,
          dx, dy, adx, ady, m, c;

    /**
     * Much of the nozzle movement will be within the same cell. So we will do as little computation
     * as possible to determine if this is the case. If this move is within the same cell, we will
     * just do the required Z-Height correction, call the Planner's buffer_line() routine, and leave
     */

    x_start = current_position[X_AXIS];
    y_start = current_position[Y_AXIS];
    z_start = current_position[Z_AXIS];
    e_start = current_position[E_AXIS];

    cell_start_xi = ubl.get_cell_index_x(x_start);
    cell_start_yi = ubl.get_cell_index_y(y_start);
    cell_dest_xi  = ubl.get_cell_index_x(x_end);
    cell_dest_yi  = ubl.get_cell_index_y(y_end);

    if (g26_debug_flag) {
      SERIAL_ECHOPGM(" ubl_line_to_destination(xe=");
      SERIAL_ECHO(x_end);
      SERIAL_ECHOPGM(", ye=");
      SERIAL_ECHO(y_end);
      SERIAL_ECHOPGM(", ze=");
      SERIAL_ECHO(z_end);
      SERIAL_ECHOPGM(", ee=");
      SERIAL_ECHO(e_end);
      SERIAL_ECHOPGM(")\n");
      debug_current_and_destination((char*)"Start of ubl_line_to_destination()");
    }

    if (cell_start_xi == cell_dest_xi && cell_start_yi == cell_dest_yi) { // if the whole move is within the same cell,
      /**
       * we don't need to break up the move
       *
       * If we are moving off the print bed, we are going to allow the move at this level.
       * But we detect it and isolate it. For now, we just pass along the request.
       */

      if (cell_dest_xi < 0 || cell_dest_yi < 0 || cell_dest_xi >= UBL_MESH_NUM_X_POINTS || cell_dest_yi >= UBL_MESH_NUM_Y_POINTS) {

        // Note:  There is no Z Correction in this case. We are off the grid and don't know what
        // a reasonable correction would be.

        planner.buffer_line(x_end, y_end, z_end + ubl.state.z_offset, e_end, feed_rate, extruder);
        set_current_to_destination();

        if (g26_debug_flag)
          debug_current_and_destination((char*)"out of bounds in ubl_line_to_destination()");

        return;
      }

      FINAL_MOVE:

      /**
       * Optimize some floating point operations here. We could call float get_z_correction(float x0, float y0) to
       * generate the correction for us. But we can lighten the load on the CPU by doing a modified version of the function.
       * We are going to only calculate the amount we are from the first mesh line towards the second mesh line once.
       * We will use this fraction in both of the original two Z Height calculations for the bi-linear interpolation. And,
       * instead of doing a generic divide of the distance, we know the distance is MESH_X_DIST so we can use the preprocessor
       * to create a 1-over number for us. That will allow us to do a floating point multiply instead of a floating point divide.
       */

      a0ma1diva2ma1 = (x_end - mesh_index_to_x_location[cell_dest_xi]) * 0.1 * (MESH_X_DIST);

      z1 = z_values[cell_dest_xi    ][cell_dest_yi    ] + a0ma1diva2ma1 *
          (z_values[cell_dest_xi + 1][cell_dest_yi    ] - z_values[cell_dest_xi][cell_dest_yi    ]);

      z2 = z_values[cell_dest_xi    ][cell_dest_yi + 1] + a0ma1diva2ma1 *
          (z_values[cell_dest_xi + 1][cell_dest_yi + 1] - z_values[cell_dest_xi][cell_dest_yi + 1]);

      // we are done with the fractional X distance into the cell. Now with the two Z-Heights we have calculated, we
      // are going to apply the Y-Distance into the cell to interpolate the final Z correction.

      a0ma1diva2ma1 = (y_end - mesh_index_to_y_location[cell_dest_yi]) * 0.1 * (MESH_Y_DIST);

      z0 = z1 + (z2 - z1) * a0ma1diva2ma1;

      /**
       * Debug code to use non-optimized get_z_correction() and to do a sanity check
       * that the correct value is being passed to planner.buffer_line()
       */
      /*
        z_optimized = z0;
        z0 = ubl.get_z_correction( x_end, y_end);
        if (fabs(z_optimized - z0) > .01 || isnan(z0) || isnan(z_optimized)) {
        debug_current_and_destination((char*)"FINAL_MOVE: z_correction()");
        if (isnan(z0)) SERIAL_ECHO(" z0==NAN  ");
        if (isnan(z_optimized)) SERIAL_ECHO(" z_optimized==NAN  ");
        SERIAL_ECHOPAIR("  x_end=", x_end);
        SERIAL_ECHOPAIR("  y_end=", y_end);
        SERIAL_ECHOPAIR("  z0=", z0);
        SERIAL_ECHOPAIR("  z_optimized=", z_optimized);
        SERIAL_ECHOPAIR("  err=",fabs(z_optimized - z0));
        SERIAL_EOL;
        }
      //*/
      z0 = z0 * ubl.fade_scaling_factor_for_z(z_end);

      /**
       * If part of the Mesh is undefined, it will show up as NAN
       * in z_values[][] and propagate through the
       * calculations. If our correction is NAN, we throw it out
       * because part of the Mesh is undefined and we don't have the
       * information we need to complete the height correction.
       */
      if (isnan(z0)) z0 = 0.0;

      planner.buffer_line(x_end, y_end, z_end + z0 + ubl.state.z_offset, e_end, feed_rate, extruder);

      if (g26_debug_flag)
        debug_current_and_destination((char*)"FINAL_MOVE in ubl_line_to_destination()");

      set_current_to_destination();
      return;
    }

    /**
     * If we get here, we are processing a move that crosses at least one Mesh Line. We will check
     * for the simple case of just crossing X or just crossing Y Mesh Lines after we get all the details
     * of the move figured out. We can process the easy case of just crossing an X or Y Mesh Line with less
     * computation and in fact most lines are of this nature. We will check for that in the following
     * blocks of code:
     */

    dx = x_end - x_start;
    dy = y_end - y_start;

    const int left_flag = dx < 0.0 ? 1 : 0,
              down_flag = dy < 0.0 ? 1 : 0;

    if (left_flag) { // figure out which way we need to move to get to the next cell
      dxi = -1;
      adx = -dx;  // absolute value of dx. We already need to check if dx and dy are negative.
    }
    else {      // We may as well generate the appropriate values for adx and ady right now
      dxi = 1;  // to save setting up the abs() function call and actually doing the call.
      adx = dx;
    }
    if (dy < 0.0) {
      dyi = -1;
      ady = -dy;  // absolute value of dy
    }
    else {
      dyi = 1;
      ady = dy;
    }

    if (cell_start_xi == cell_dest_xi) dxi = 0;
    if (cell_start_yi == cell_dest_yi) dyi = 0;

    /**
     * Compute the scaling factor for the extruder for each partial move.
     * We need to watch out for zero length moves because it will cause us to
     * have an infinate scaling factor. We are stuck doing a floating point
     * divide to get our scaling factor, but after that, we just multiply by this
     * number. We also pick our scaling factor based on whether the X or Y
     * component is larger. We use the biggest of the two to preserve precision.
     */

    const bool use_x_dist = adx > ady;

    on_axis_distance = use_x_dist ? x_end - x_start : y_end - y_start;

    e_position = e_end - e_start;
    e_normalized_dist = e_position / on_axis_distance;

    z_position = z_end - z_start;
    z_normalized_dist = z_position / on_axis_distance;

    const bool inf_normalized_flag = e_normalized_dist == INFINITY || e_normalized_dist == -INFINITY;

    current_xi = cell_start_xi;
    current_yi = cell_start_yi;

    m = dy / dx;
    c = y_start - m * x_start;
    const bool inf_m_flag = (m == INFINITY || m == -INFINITY);

    /**
     * This block handles vertical lines. These are lines that stay within the same
     * X Cell column. They do not need to be perfectly vertical. They just can
     * not cross into another X Cell column.
     */
    if (dxi == 0) {       // Check for a vertical line
      current_yi += down_flag;  // Line is heading down, we just want to go to the bottom
      while (current_yi != cell_dest_yi + down_flag) {
        current_yi += dyi;
        next_mesh_line_y = mesh_index_to_y_location[current_yi];

        /**
         * inf_m_flag? the slope of the line is infinite, we won't do the calculations
         * else, we know the next X is the same so we can recover and continue!
         * Calculate X at the next Y mesh line
         */
        x = inf_m_flag ? x_start : (next_mesh_line_y - c) / m;

        z0 = ubl.get_z_correction_along_horizontal_mesh_line_at_specific_X(x, current_xi, current_yi);

        /**
         * Debug code to use non-optimized get_z_correction() and to do a sanity check
         * that the correct value is being passed to planner.buffer_line()
         */
        /*
          z_optimized = z0;
          z0 = ubl.get_z_correction( x, next_mesh_line_y);
          if (fabs(z_optimized - z0) > .01 || isnan(z0) || isnan(z_optimized)) {
            debug_current_and_destination((char*)"VERTICAL z_correction()");
          if (isnan(z0)) SERIAL_ECHO(" z0==NAN  ");
            if (isnan(z_optimized)) SERIAL_ECHO(" z_optimized==NAN  ");
          SERIAL_ECHOPAIR("  x=", x);
          SERIAL_ECHOPAIR("  next_mesh_line_y=", next_mesh_line_y);
          SERIAL_ECHOPAIR("  z0=", z0);
          SERIAL_ECHOPAIR("  z_optimized=", z_optimized);
          SERIAL_ECHOPAIR("  err=",fabs(z_optimized-z0));
          SERIAL_ECHO("\n");
          }
        //*/

        z0 = z0 * ubl.fade_scaling_factor_for_z(z_end);

        /**
         * If part of the Mesh is undefined, it will show up as NAN
         * in z_values[][] and propagate through the
         * calculations. If our correction is NAN, we throw it out
         * because part of the Mesh is undefined and we don't have the
         * information we need to complete the height correction.
         */
        if (isnan(z0)) z0 = 0.0;     

        y = mesh_index_to_y_location[current_yi];

        /**
         * Without this check, it is possible for the algorithm to generate a zero length move in the case
         * where the line is heading down and it is starting right on a Mesh Line boundary. For how often that
         * happens, it might be best to remove the check and always 'schedule' the move because
         * the planner.buffer_line() routine will filter it if that happens.
         */
        if (y != y_start) {
          if (!inf_normalized_flag) {
            on_axis_distance = y - y_start;                               // we don't need to check if the extruder position
            e_position = e_start + on_axis_distance * e_normalized_dist;  // is based on X or Y because this is a vertical move
            z_position = z_start + on_axis_distance * z_normalized_dist;
          }
          else {
            e_position = e_start;
            z_position = z_start;
          }

          planner.buffer_line(x, y, z_position + z0 + ubl.state.z_offset, e_position, feed_rate, extruder);
        } //else printf("FIRST MOVE PRUNED  ");
      }

      if (g26_debug_flag)
        debug_current_and_destination((char*)"vertical move done in ubl_line_to_destination()");

      //
      // Check if we are at the final destination. Usually, we won't be, but if it is on a Y Mesh Line, we are done.
      //
      if (current_position[X_AXIS] != x_end || current_position[Y_AXIS] != y_end)
        goto FINAL_MOVE;

      set_current_to_destination();
      return;
    }

    /**
     *
     * This block handles horizontal lines. These are lines that stay within the same
     * Y Cell row. They do not need to be perfectly horizontal. They just can
     * not cross into another Y Cell row.
     *
     */

    if (dyi == 0) {             // Check for a horizontal line
      current_xi += left_flag;  // Line is heading left, we just want to go to the left
                                // edge of this cell for the first move.
      while (current_xi != cell_dest_xi + left_flag) {
        current_xi += dxi;
        next_mesh_line_x = mesh_index_to_x_location[current_xi];
        y = m * next_mesh_line_x + c;   // Calculate X at the next Y mesh line

        z0 = ubl.get_z_correction_along_vertical_mesh_line_at_specific_Y(y, current_xi, current_yi);

        /**
         * Debug code to use non-optimized get_z_correction() and to do a sanity check
         * that the correct value is being passed to planner.buffer_line()
         */
        /*
          z_optimized = z0;
          z0 = ubl.get_z_correction( next_mesh_line_x, y);
          if (fabs(z_optimized - z0) > .01 || isnan(z0) || isnan(z_optimized)) {
            debug_current_and_destination((char*)"HORIZONTAL z_correction()");
          if (isnan(z0)) SERIAL_ECHO(" z0==NAN  ");
            if (isnan(z_optimized)) SERIAL_ECHO(" z_optimized==NAN  ");
          SERIAL_ECHOPAIR("  next_mesh_line_x=", next_mesh_line_x);
          SERIAL_ECHOPAIR("  y=", y);
          SERIAL_ECHOPAIR("  z0=", z0);
          SERIAL_ECHOPAIR("  z_optimized=", z_optimized);
          SERIAL_ECHOPAIR("  err=",fabs(z_optimized-z0));
          SERIAL_ECHO("\n");
          }
        //*/

        z0 = z0 * ubl.fade_scaling_factor_for_z(z_end);

        /**
         * If part of the Mesh is undefined, it will show up as NAN
         * in z_values[][] and propagate through the
         * calculations. If our correction is NAN, we throw it out
         * because part of the Mesh is undefined and we don't have the
         * information we need to complete the height correction.
         */
        if (isnan(z0)) z0 = 0.0;

        x = mesh_index_to_x_location[current_xi];

        /**
         * Without this check, it is possible for the algorithm to generate a zero length move in the case
         * where the line is heading left and it is starting right on a Mesh Line boundary. For how often
         * that happens, it might be best to remove the check and always 'schedule' the move because
         * the planner.buffer_line() routine will filter it if that happens.
         */
        if (x != x_start) {
          if (!inf_normalized_flag) {
            on_axis_distance = x - x_start;                               // we don't need to check if the extruder position
            e_position = e_start + on_axis_distance * e_normalized_dist;  // is based on X or Y because this is a horizontal move
            z_position = z_start + on_axis_distance * z_normalized_dist;
          }
          else {
            e_position = e_start;
            z_position = z_start;
          }

          planner.buffer_line(x, y, z_position + z0 + ubl.state.z_offset, e_position, feed_rate, extruder);
        } //else printf("FIRST MOVE PRUNED  ");
      }

      if (g26_debug_flag)
        debug_current_and_destination((char*)"horizontal move done in ubl_line_to_destination()");

      if (current_position[X_AXIS] != x_end || current_position[Y_AXIS] != y_end)
        goto FINAL_MOVE;

      set_current_to_destination();
      return;
    }

    /**
     *
     * This block handles the generic case of a line crossing both X and Y Mesh lines.
     *
     */

    xi_cnt = cell_start_xi - cell_dest_xi;
    if (xi_cnt < 0) xi_cnt = -xi_cnt;

    yi_cnt = cell_start_yi - cell_dest_yi;
    if (yi_cnt < 0) yi_cnt = -yi_cnt;

    current_xi += left_flag;
    current_yi += down_flag;

    while (xi_cnt > 0 || yi_cnt > 0) {

      next_mesh_line_x = mesh_index_to_x_location[current_xi + dxi];
      next_mesh_line_y = mesh_index_to_y_location[current_yi + dyi];

      y = m * next_mesh_line_x + c; // Calculate Y at the next X mesh line
      x = (next_mesh_line_y - c) / m; // Calculate X at the next Y mesh line    (we don't have to worry
      // about m being equal to 0.0  If this was the case, we would have
      // detected this as a vertical line move up above and we wouldn't
      // be down here doing a generic type of move.

      if (left_flag == (x > next_mesh_line_x)) { // Check if we hit the Y line first
        //
        // Yes!  Crossing a Y Mesh Line next
        //
        z0 = ubl.get_z_correction_along_horizontal_mesh_line_at_specific_X(x, current_xi - left_flag, current_yi + dyi);

        /**
         * Debug code to use non-optimized get_z_correction() and to do a sanity check
         * that the correct value is being passed to planner.buffer_line()
         */
        /*
          z_optimized = z0;
          z0 = ubl.get_z_correction( x, next_mesh_line_y);
          if (fabs(z_optimized - z0) > .01 || isnan(z0) || isnan(z_optimized)) {
            debug_current_and_destination((char*)"General_1: z_correction()");
            if (isnan(z0)) SERIAL_ECHO(" z0==NAN  ");
            if (isnan(z_optimized)) SERIAL_ECHO(" z_optimized==NAN  "); {
              SERIAL_ECHOPAIR("  x=", x);
            }
            SERIAL_ECHOPAIR("  next_mesh_line_y=", next_mesh_line_y);
            SERIAL_ECHOPAIR("  z0=", z0);
            SERIAL_ECHOPAIR("  z_optimized=", z_optimized);
            SERIAL_ECHOPAIR("  err=",fabs(z_optimized-z0));
            SERIAL_ECHO("\n");
          }
        //*/

        z0 *= ubl.fade_scaling_factor_for_z(z_end);

        /**
         * If part of the Mesh is undefined, it will show up as NAN
         * in z_values[][] and propagate through the
         * calculations. If our correction is NAN, we throw it out
         * because part of the Mesh is undefined and we don't have the
         * information we need to complete the height correction.
         */
        if (isnan(z0)) z0 = 0.0;

        if (!inf_normalized_flag) {
          on_axis_distance = use_x_dist ? x - x_start : next_mesh_line_y - y_start;
          e_position = e_start + on_axis_distance * e_normalized_dist;
          z_position = z_start + on_axis_distance * z_normalized_dist;
        }
        else {
          e_position = e_start;
          z_position = z_start;
        }
        planner.buffer_line(x, next_mesh_line_y, z_position + z0 + ubl.state.z_offset, e_position, feed_rate, extruder);
        current_yi += dyi;
        yi_cnt--;
      }
      else {
        //
        // Yes!  Crossing a X Mesh Line next
        //
        z0 = ubl.get_z_correction_along_vertical_mesh_line_at_specific_Y(y, current_xi + dxi, current_yi - down_flag);

        /**
         * Debug code to use non-optimized get_z_correction() and to do a sanity check
         * that the correct value is being passed to planner.buffer_line()
         */
        /*
          z_optimized = z0;
          z0 = ubl.get_z_correction( next_mesh_line_x, y);
          if (fabs(z_optimized - z0) > .01 || isnan(z0) || isnan(z_optimized)) {
          debug_current_and_destination((char*)"General_2: z_correction()");
          if (isnan(z0)) SERIAL_ECHO(" z0==NAN  ");
          if (isnan(z_optimized)) SERIAL_ECHO(" z_optimized==NAN  ");
          SERIAL_ECHOPAIR("  next_mesh_line_x=", next_mesh_line_x);
          SERIAL_ECHOPAIR("  y=", y);
          SERIAL_ECHOPAIR("  z0=", z0);
          SERIAL_ECHOPAIR("  z_optimized=", z_optimized);
          SERIAL_ECHOPAIR("  err=",fabs(z_optimized-z0));
          SERIAL_ECHO("\n");
          }
        //*/

        z0 = z0 * ubl.fade_scaling_factor_for_z(z_end);

        /**
         * If part of the Mesh is undefined, it will show up as NAN
         * in z_values[][] and propagate through the
         * calculations. If our correction is NAN, we throw it out
         * because part of the Mesh is undefined and we don't have the
         * information we need to complete the height correction.
         */
        if (isnan(z0)) z0 = 0.0;

        if (!inf_normalized_flag) {
          on_axis_distance = use_x_dist ? next_mesh_line_x - x_start : y - y_start;
          e_position = e_start + on_axis_distance * e_normalized_dist;
          z_position = z_start + on_axis_distance * z_normalized_dist;
        }
        else {
          e_position = e_start;
          z_position = z_start;
        }

        planner.buffer_line(next_mesh_line_x, y, z_position + z0 + ubl.state.z_offset, e_position, feed_rate, extruder);
        current_xi += dxi;
        xi_cnt--;
      }
    }

    if (g26_debug_flag)
      debug_current_and_destination((char*)"generic move done in ubl_line_to_destination()");

    if (current_position[0] != x_end || current_position[1] != y_end)
      goto FINAL_MOVE;

    set_current_to_destination();
  }

#endif
