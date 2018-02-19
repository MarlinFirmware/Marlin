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

/**
 * G29.cpp - Auto Bed Leveling
 */

#include "../../../inc/MarlinConfig.h"

#if OLDSCHOOL_ABL

#include "../../gcode.h"
#include "../../../feature/bedlevel/bedlevel.h"
#include "../../../module/motion.h"
#include "../../../module/planner.h"
#include "../../../module/stepper.h"
#include "../../../module/probe.h"
#include "../../queue.h"

#if ENABLED(LCD_BED_LEVELING) && ENABLED(PROBE_MANUALLY)
  #include "../../../lcd/ultralcd.h"
#endif

#if ENABLED(AUTO_BED_LEVELING_LINEAR)
  #include "../../../libs/least_squares_fit.h"
#endif

#if ABL_GRID
  #if ENABLED(PROBE_Y_FIRST)
    #define PR_OUTER_VAR xCount
    #define PR_OUTER_END abl_grid_points_x
    #define PR_INNER_VAR yCount
    #define PR_INNER_END abl_grid_points_y
  #else
    #define PR_OUTER_VAR yCount
    #define PR_OUTER_END abl_grid_points_y
    #define PR_INNER_VAR xCount
    #define PR_INNER_END abl_grid_points_x
  #endif
#endif

/**
 * G29: Detailed Z probe, probes the bed at 3 or more points.
 *      Will fail if the printer has not been homed with G28.
 *
 * Enhanced G29 Auto Bed Leveling Probe Routine
 *
 *  D  Dry-Run mode. Just evaluate the bed Topology - Don't apply
 *     or alter the bed level data. Useful to check the topology
 *     after a first run of G29.
 *
 *  J  Jettison current bed leveling data
 *
 *  V  Set the verbose level (0-4). Example: "G29 V3"
 *
 * Parameters With LINEAR leveling only:
 *
 *  P  Set the size of the grid that will be probed (P x P points).
 *     Example: "G29 P4"
 *
 *  X  Set the X size of the grid that will be probed (X x Y points).
 *     Example: "G29 X7 Y5"
 *
 *  Y  Set the Y size of the grid that will be probed (X x Y points).
 *
 *  T  Generate a Bed Topology Report. Example: "G29 P5 T" for a detailed report.
 *     This is useful for manual bed leveling and finding flaws in the bed (to
 *     assist with part placement).
 *     Not supported by non-linear delta printer bed leveling.
 *
 * Parameters With LINEAR and BILINEAR leveling only:
 *
 *  S  Set the XY travel speed between probe points (in units/min)
 *
 *  F  Set the Front limit of the probing grid
 *  B  Set the Back limit of the probing grid
 *  L  Set the Left limit of the probing grid
 *  R  Set the Right limit of the probing grid
 *
 * Parameters with DEBUG_LEVELING_FEATURE only:
 *
 *  C  Make a totally fake grid with no actual probing.
 *     For use in testing when no probing is possible.
 *
 * Parameters with BILINEAR leveling only:
 *
 *  Z  Supply an additional Z probe offset
 *
 * Extra parameters with PROBE_MANUALLY:
 *
 *  To do manual probing simply repeat G29 until the procedure is complete.
 *  The first G29 accepts parameters. 'G29 Q' for status, 'G29 A' to abort.
 *
 *  Q  Query leveling and G29 state
 *
 *  A  Abort current leveling procedure
 *
 * Extra parameters with BILINEAR only:
 *
 *  W  Write a mesh point. (If G29 is idle.)
 *  I  X index for mesh point
 *  J  Y index for mesh point
 *  X  X for mesh point, overrides I
 *  Y  Y for mesh point, overrides J
 *  Z  Z for mesh point. Otherwise, raw current Z.
 *
 * Without PROBE_MANUALLY:
 *
 *  E  By default G29 will engage the Z probe, test the bed, then disengage.
 *     Include "E" to engage/disengage the Z probe for each sample.
 *     There's no extra effect if you have a fixed Z probe.
 *
 */
void GcodeSuite::G29() {

  #if ENABLED(DEBUG_LEVELING_FEATURE) || ENABLED(PROBE_MANUALLY)
    const bool seenQ = parser.seen('Q');
  #else
    constexpr bool seenQ = false;
  #endif

  // G29 Q is also available if debugging
  #if ENABLED(DEBUG_LEVELING_FEATURE)
    const uint8_t old_debug_flags = marlin_debug_flags;
    if (seenQ) marlin_debug_flags |= DEBUG_LEVELING;
    if (DEBUGGING(LEVELING)) {
      DEBUG_POS(">>> G29", current_position);
      log_machine_info();
    }
    marlin_debug_flags = old_debug_flags;
    #if DISABLED(PROBE_MANUALLY)
      if (seenQ) return;
    #endif
  #endif

  #if ENABLED(PROBE_MANUALLY)
    const bool seenA = parser.seen('A');
  #else
    constexpr bool seenA = false;
  #endif

  const bool  no_action = seenA || seenQ,
              faux =
                #if ENABLED(DEBUG_LEVELING_FEATURE) && DISABLED(PROBE_MANUALLY)
                  parser.boolval('C')
                #else
                  no_action
                #endif
              ;

  // Don't allow auto-leveling without homing first
  if (axis_unhomed_error()) return;

  // Define local vars 'static' for manual probing, 'auto' otherwise
  #if ENABLED(PROBE_MANUALLY)
    #define ABL_VAR static
  #else
    #define ABL_VAR
  #endif

  ABL_VAR int verbose_level;
  ABL_VAR float xProbe, yProbe, measured_z;
  ABL_VAR bool dryrun, abl_should_enable;

  #if ENABLED(PROBE_MANUALLY) || ENABLED(AUTO_BED_LEVELING_LINEAR)
    ABL_VAR int abl_probe_index;
  #endif

  #if HAS_SOFTWARE_ENDSTOPS && ENABLED(PROBE_MANUALLY)
    ABL_VAR bool enable_soft_endstops = true;
  #endif

  #if ABL_GRID

    #if ENABLED(PROBE_MANUALLY)
      ABL_VAR uint8_t PR_OUTER_VAR;
      ABL_VAR  int8_t PR_INNER_VAR;
    #endif

    ABL_VAR int left_probe_bed_position, right_probe_bed_position, front_probe_bed_position, back_probe_bed_position;
    ABL_VAR float xGridSpacing = 0, yGridSpacing = 0;

    #if ENABLED(AUTO_BED_LEVELING_LINEAR)
      ABL_VAR uint8_t abl_grid_points_x = GRID_MAX_POINTS_X,
                      abl_grid_points_y = GRID_MAX_POINTS_Y;
      ABL_VAR bool do_topography_map;
    #else // Bilinear
      uint8_t constexpr abl_grid_points_x = GRID_MAX_POINTS_X,
                        abl_grid_points_y = GRID_MAX_POINTS_Y;
    #endif

    #if ENABLED(AUTO_BED_LEVELING_LINEAR)
      ABL_VAR int abl2;
    #elif ENABLED(PROBE_MANUALLY) // Bilinear
      int constexpr abl2 = GRID_MAX_POINTS;
    #endif

    #if ENABLED(AUTO_BED_LEVELING_BILINEAR)

      ABL_VAR float zoffset;

    #elif ENABLED(AUTO_BED_LEVELING_LINEAR)

      ABL_VAR int indexIntoAB[GRID_MAX_POINTS_X][GRID_MAX_POINTS_Y];

      ABL_VAR float eqnAMatrix[GRID_MAX_POINTS * 3], // "A" matrix of the linear system of equations
                    eqnBVector[GRID_MAX_POINTS],     // "B" vector of Z points
                    mean;
    #endif

  #elif ENABLED(AUTO_BED_LEVELING_3POINT)

    #if ENABLED(PROBE_MANUALLY)
      int constexpr abl2 = 3; // used to show total points
    #endif

    // Probe at 3 arbitrary points
    ABL_VAR vector_3 points[3] = {
      vector_3(ABL_PROBE_PT_1_X, ABL_PROBE_PT_1_Y, 0),
      vector_3(ABL_PROBE_PT_2_X, ABL_PROBE_PT_2_Y, 0),
      vector_3(ABL_PROBE_PT_3_X, ABL_PROBE_PT_3_Y, 0)
    };

  #endif // AUTO_BED_LEVELING_3POINT

  #if ENABLED(AUTO_BED_LEVELING_LINEAR)
    struct linear_fit_data lsf_results;
    incremental_LSF_reset(&lsf_results);
  #endif

  /**
   * On the initial G29 fetch command parameters.
   */
  if (!g29_in_progress) {

    #if ENABLED(PROBE_MANUALLY) || ENABLED(AUTO_BED_LEVELING_LINEAR)
      abl_probe_index = -1;
    #endif

    abl_should_enable = planner.leveling_active;

    #if ENABLED(AUTO_BED_LEVELING_BILINEAR)

      const bool seen_w = parser.seen('W');
      if (seen_w) {
        if (!leveling_is_valid()) {
          SERIAL_ERROR_START();
          SERIAL_ERRORLNPGM("No bilinear grid");
          return;
        }

        const float rz = parser.seenval('Z') ? RAW_Z_POSITION(parser.value_linear_units()) : current_position[Z_AXIS];
        if (!WITHIN(rz, -10, 10)) {
          SERIAL_ERROR_START();
          SERIAL_ERRORLNPGM("Bad Z value");
          return;
        }

        const float rx = RAW_X_POSITION(parser.linearval('X', NAN)),
                    ry = RAW_Y_POSITION(parser.linearval('Y', NAN));
        int8_t i = parser.byteval('I', -1),
               j = parser.byteval('J', -1);

        if (!isnan(rx) && !isnan(ry)) {
          // Get nearest i / j from rx / ry
          i = (rx - bilinear_start[X_AXIS] + 0.5 * xGridSpacing) / xGridSpacing;
          j = (ry - bilinear_start[Y_AXIS] + 0.5 * yGridSpacing) / yGridSpacing;
          i = constrain(i, 0, GRID_MAX_POINTS_X - 1);
          j = constrain(j, 0, GRID_MAX_POINTS_Y - 1);
        }
        if (WITHIN(i, 0, GRID_MAX_POINTS_X - 1) && WITHIN(j, 0, GRID_MAX_POINTS_Y)) {
          set_bed_leveling_enabled(false);
          z_values[i][j] = rz;
          #if ENABLED(ABL_BILINEAR_SUBDIVISION)
            bed_level_virt_interpolate();
          #endif
          set_bed_leveling_enabled(abl_should_enable);
          if (abl_should_enable) report_current_position();
        }
        return;
      } // parser.seen('W')

    #else

      constexpr bool seen_w = false;

    #endif

    // Jettison bed leveling data
    if (!seen_w && parser.seen('J')) {
      reset_bed_level();
      return;
    }

    verbose_level = parser.intval('V');
    if (!WITHIN(verbose_level, 0, 4)) {
      SERIAL_PROTOCOLLNPGM("?(V)erbose level is implausible (0-4).");
      return;
    }

    dryrun = parser.boolval('D')
      #if ENABLED(PROBE_MANUALLY)
        || no_action
      #endif
    ;

    #if ENABLED(AUTO_BED_LEVELING_LINEAR)

      do_topography_map = verbose_level > 2 || parser.boolval('T');

      // X and Y specify points in each direction, overriding the default
      // These values may be saved with the completed mesh
      abl_grid_points_x = parser.intval('X', GRID_MAX_POINTS_X);
      abl_grid_points_y = parser.intval('Y', GRID_MAX_POINTS_Y);
      if (parser.seenval('P')) abl_grid_points_x = abl_grid_points_y = parser.value_int();

      if (!WITHIN(abl_grid_points_x, 2, GRID_MAX_POINTS_X)) {
        SERIAL_PROTOCOLLNPGM("?Probe points (X) is implausible (2-" STRINGIFY(GRID_MAX_POINTS_X) ").");
        return;
      }
      if (!WITHIN(abl_grid_points_y, 2, GRID_MAX_POINTS_Y)) {
        SERIAL_PROTOCOLLNPGM("?Probe points (Y) is implausible (2-" STRINGIFY(GRID_MAX_POINTS_Y) ").");
        return;
      }

      abl2 = abl_grid_points_x * abl_grid_points_y;
      mean = 0;

    #elif ENABLED(AUTO_BED_LEVELING_BILINEAR)

      zoffset = parser.linearval('Z');

    #endif

    #if ABL_GRID

      xy_probe_feedrate_mm_s = MMM_TO_MMS(parser.linearval('S', XY_PROBE_SPEED));

      left_probe_bed_position  = parser.seenval('L') ? (int)RAW_X_POSITION(parser.value_linear_units()) : LEFT_PROBE_BED_POSITION;
      right_probe_bed_position = parser.seenval('R') ? (int)RAW_X_POSITION(parser.value_linear_units()) : RIGHT_PROBE_BED_POSITION;
      front_probe_bed_position = parser.seenval('F') ? (int)RAW_Y_POSITION(parser.value_linear_units()) : FRONT_PROBE_BED_POSITION;
      back_probe_bed_position  = parser.seenval('B') ? (int)RAW_Y_POSITION(parser.value_linear_units()) : BACK_PROBE_BED_POSITION;

      const bool left_out_l = left_probe_bed_position < MIN_PROBE_X,
                 left_out = left_out_l || left_probe_bed_position > right_probe_bed_position - (MIN_PROBE_EDGE),
                 right_out_r = right_probe_bed_position > MAX_PROBE_X,
                 right_out = right_out_r || right_probe_bed_position < left_probe_bed_position + MIN_PROBE_EDGE,
                 front_out_f = front_probe_bed_position < MIN_PROBE_Y,
                 front_out = front_out_f || front_probe_bed_position > back_probe_bed_position - (MIN_PROBE_EDGE),
                 back_out_b = back_probe_bed_position > MAX_PROBE_Y,
                 back_out = back_out_b || back_probe_bed_position < front_probe_bed_position + MIN_PROBE_EDGE;

      if (left_out || right_out || front_out || back_out) {
        if (left_out) {
          out_of_range_error(PSTR("(L)eft"));
          left_probe_bed_position = left_out_l ? MIN_PROBE_X : right_probe_bed_position - (MIN_PROBE_EDGE);
        }
        if (right_out) {
          out_of_range_error(PSTR("(R)ight"));
          right_probe_bed_position = right_out_r ? MAX_PROBE_X : left_probe_bed_position + MIN_PROBE_EDGE;
        }
        if (front_out) {
          out_of_range_error(PSTR("(F)ront"));
          front_probe_bed_position = front_out_f ? MIN_PROBE_Y : back_probe_bed_position - (MIN_PROBE_EDGE);
        }
        if (back_out) {
          out_of_range_error(PSTR("(B)ack"));
          back_probe_bed_position = back_out_b ? MAX_PROBE_Y : front_probe_bed_position + MIN_PROBE_EDGE;
        }
        return;
      }

      // probe at the points of a lattice grid
      xGridSpacing = (right_probe_bed_position - left_probe_bed_position) / (abl_grid_points_x - 1);
      yGridSpacing = (back_probe_bed_position - front_probe_bed_position) / (abl_grid_points_y - 1);

    #endif // ABL_GRID

    if (verbose_level > 0) {
      SERIAL_PROTOCOLPGM("G29 Auto Bed Leveling");
      if (dryrun) SERIAL_PROTOCOLPGM(" (DRYRUN)");
      SERIAL_EOL();
    }

    stepper.synchronize();

    // Disable auto bed leveling during G29.
    // Be formal so G29 can be done successively without G28.
    if (!no_action) set_bed_leveling_enabled(false);

    #if HAS_BED_PROBE
      // Deploy the probe. Probe will raise if needed.
      if (DEPLOY_PROBE()) {
        set_bed_leveling_enabled(abl_should_enable);
        return;
      }
    #endif

    if (!faux) setup_for_endstop_or_probe_move();

    #if ENABLED(AUTO_BED_LEVELING_BILINEAR)

      #if ENABLED(PROBE_MANUALLY)
        if (!no_action)
      #endif
      if ( xGridSpacing != bilinear_grid_spacing[X_AXIS]
        || yGridSpacing != bilinear_grid_spacing[Y_AXIS]
        || left_probe_bed_position != bilinear_start[X_AXIS]
        || front_probe_bed_position != bilinear_start[Y_AXIS]
      ) {
        // Reset grid to 0.0 or "not probed". (Also disables ABL)
        reset_bed_level();

        // Initialize a grid with the given dimensions
        bilinear_grid_spacing[X_AXIS] = xGridSpacing;
        bilinear_grid_spacing[Y_AXIS] = yGridSpacing;
        bilinear_start[X_AXIS] = left_probe_bed_position;
        bilinear_start[Y_AXIS] = front_probe_bed_position;

        // Can't re-enable (on error) until the new grid is written
        abl_should_enable = false;
      }

    #endif // AUTO_BED_LEVELING_BILINEAR

    #if ENABLED(AUTO_BED_LEVELING_3POINT)

      #if ENABLED(DEBUG_LEVELING_FEATURE)
        if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPGM("> 3-point Leveling");
      #endif

      // Probe at 3 arbitrary points
      points[0].z = points[1].z = points[2].z = 0;

    #endif // AUTO_BED_LEVELING_3POINT

  } // !g29_in_progress

  #if ENABLED(PROBE_MANUALLY)

    // For manual probing, get the next index to probe now.
    // On the first probe this will be incremented to 0.
    if (!no_action) {
      ++abl_probe_index;
      g29_in_progress = true;
    }

    // Abort current G29 procedure, go back to idle state
    if (seenA && g29_in_progress) {
      SERIAL_PROTOCOLLNPGM("Manual G29 aborted");
      #if HAS_SOFTWARE_ENDSTOPS
        soft_endstops_enabled = enable_soft_endstops;
      #endif
      set_bed_leveling_enabled(abl_should_enable);
      g29_in_progress = false;
      #if ENABLED(LCD_BED_LEVELING)
        lcd_wait_for_move = false;
      #endif
    }

    // Query G29 status
    if (verbose_level || seenQ) {
      SERIAL_PROTOCOLPGM("Manual G29 ");
      if (g29_in_progress) {
        SERIAL_PROTOCOLPAIR("point ", min(abl_probe_index + 1, abl2));
        SERIAL_PROTOCOLLNPAIR(" of ", abl2);
      }
      else
        SERIAL_PROTOCOLLNPGM("idle");
    }

    if (no_action) return;

    if (abl_probe_index == 0) {
      // For the initial G29 save software endstop state
      #if HAS_SOFTWARE_ENDSTOPS
        enable_soft_endstops = soft_endstops_enabled;
      #endif
    }
    else {
      // For G29 after adjusting Z.
      // Save the previous Z before going to the next point
      measured_z = current_position[Z_AXIS];

      #if ENABLED(AUTO_BED_LEVELING_LINEAR)

        mean += measured_z;
        eqnBVector[abl_probe_index] = measured_z;
        eqnAMatrix[abl_probe_index + 0 * abl2] = xProbe;
        eqnAMatrix[abl_probe_index + 1 * abl2] = yProbe;
        eqnAMatrix[abl_probe_index + 2 * abl2] = 1;

        incremental_LSF(&lsf_results, xProbe, yProbe, measured_z);

      #elif ENABLED(AUTO_BED_LEVELING_BILINEAR)

        z_values[xCount][yCount] = measured_z + zoffset;

        #if ENABLED(DEBUG_LEVELING_FEATURE)
          if (DEBUGGING(LEVELING)) {
            SERIAL_PROTOCOLPAIR("Save X", xCount);
            SERIAL_PROTOCOLPAIR(" Y", yCount);
            SERIAL_PROTOCOLLNPAIR(" Z", measured_z + zoffset);
          }
        #endif

      #elif ENABLED(AUTO_BED_LEVELING_3POINT)

        points[abl_probe_index].z = measured_z;

      #endif
    }

    //
    // If there's another point to sample, move there with optional lift.
    //

    #if ABL_GRID

      // Skip any unreachable points
      while (abl_probe_index < abl2) {

        // Set xCount, yCount based on abl_probe_index, with zig-zag
        PR_OUTER_VAR = abl_probe_index / PR_INNER_END;
        PR_INNER_VAR = abl_probe_index - (PR_OUTER_VAR * PR_INNER_END);

        // Probe in reverse order for every other row/column
        bool zig = (PR_OUTER_VAR & 1); // != ((PR_OUTER_END) & 1);

        if (zig) PR_INNER_VAR = (PR_INNER_END - 1) - PR_INNER_VAR;

        const float xBase = xCount * xGridSpacing + left_probe_bed_position,
                    yBase = yCount * yGridSpacing + front_probe_bed_position;

        xProbe = FLOOR(xBase + (xBase < 0 ? 0 : 0.5));
        yProbe = FLOOR(yBase + (yBase < 0 ? 0 : 0.5));

        #if ENABLED(AUTO_BED_LEVELING_LINEAR)
          indexIntoAB[xCount][yCount] = abl_probe_index;
        #endif

        // Keep looping till a reachable point is found
        if (position_is_reachable(xProbe, yProbe)) break;
        ++abl_probe_index;
      }

      // Is there a next point to move to?
      if (abl_probe_index < abl2) {
        _manual_goto_xy(xProbe, yProbe); // Can be used here too!
        #if HAS_SOFTWARE_ENDSTOPS
          // Disable software endstops to allow manual adjustment
          // If G29 is not completed, they will not be re-enabled
          soft_endstops_enabled = false;
        #endif
        return;
      }
      else {

        // Leveling done! Fall through to G29 finishing code below

        SERIAL_PROTOCOLLNPGM("Grid probing done.");

        // Re-enable software endstops, if needed
        #if HAS_SOFTWARE_ENDSTOPS
          soft_endstops_enabled = enable_soft_endstops;
        #endif
      }

    #elif ENABLED(AUTO_BED_LEVELING_3POINT)

      // Probe at 3 arbitrary points
      if (abl_probe_index < abl2) {
        xProbe = points[abl_probe_index].x;
        yProbe = points[abl_probe_index].y;
        _manual_goto_xy(xProbe, yProbe);
        #if HAS_SOFTWARE_ENDSTOPS
          // Disable software endstops to allow manual adjustment
          // If G29 is not completed, they will not be re-enabled
          soft_endstops_enabled = false;
        #endif
        return;
      }
      else {

        SERIAL_PROTOCOLLNPGM("3-point probing done.");

        // Re-enable software endstops, if needed
        #if HAS_SOFTWARE_ENDSTOPS
          soft_endstops_enabled = enable_soft_endstops;
        #endif

        if (!dryrun) {
          vector_3 planeNormal = vector_3::cross(points[0] - points[1], points[2] - points[1]).get_normal();
          if (planeNormal.z < 0) {
            planeNormal.x *= -1;
            planeNormal.y *= -1;
            planeNormal.z *= -1;
          }
          planner.bed_level_matrix = matrix_3x3::create_look_at(planeNormal);

          // Can't re-enable (on error) until the new grid is written
          abl_should_enable = false;
        }

      }

    #endif // AUTO_BED_LEVELING_3POINT

  #else // !PROBE_MANUALLY
  {
    const bool stow_probe_after_each = parser.boolval('E');

    measured_z = 0;

    #if ABL_GRID

      bool zig = PR_OUTER_END & 1;  // Always end at RIGHT and BACK_PROBE_BED_POSITION

      measured_z = 0;

      // Outer loop is Y with PROBE_Y_FIRST disabled
      for (uint8_t PR_OUTER_VAR = 0; PR_OUTER_VAR < PR_OUTER_END && !isnan(measured_z); PR_OUTER_VAR++) {

        int8_t inStart, inStop, inInc;

        if (zig) { // away from origin
          inStart = 0;
          inStop = PR_INNER_END;
          inInc = 1;
        }
        else {     // towards origin
          inStart = PR_INNER_END - 1;
          inStop = -1;
          inInc = -1;
        }

        zig ^= true; // zag

        // Inner loop is Y with PROBE_Y_FIRST enabled
        for (int8_t PR_INNER_VAR = inStart; PR_INNER_VAR != inStop; PR_INNER_VAR += inInc) {

          float xBase = left_probe_bed_position + xGridSpacing * xCount,
                yBase = front_probe_bed_position + yGridSpacing * yCount;

          xProbe = FLOOR(xBase + (xBase < 0 ? 0 : 0.5));
          yProbe = FLOOR(yBase + (yBase < 0 ? 0 : 0.5));

          #if ENABLED(AUTO_BED_LEVELING_LINEAR)
            indexIntoAB[xCount][yCount] = ++abl_probe_index; // 0...
          #endif

          #if IS_KINEMATIC
            // Avoid probing outside the round or hexagonal area
            if (!position_is_reachable_by_probe(xProbe, yProbe)) continue;
          #endif

          measured_z = faux ? 0.001 * random(-100, 101) : probe_pt(xProbe, yProbe, stow_probe_after_each, verbose_level);

          if (isnan(measured_z)) {
            set_bed_leveling_enabled(abl_should_enable);
            break;
          }

          #if ENABLED(AUTO_BED_LEVELING_LINEAR)

            mean += measured_z;
            eqnBVector[abl_probe_index] = measured_z;
            eqnAMatrix[abl_probe_index + 0 * abl2] = xProbe;
            eqnAMatrix[abl_probe_index + 1 * abl2] = yProbe;
            eqnAMatrix[abl_probe_index + 2 * abl2] = 1;

            incremental_LSF(&lsf_results, xProbe, yProbe, measured_z);

          #elif ENABLED(AUTO_BED_LEVELING_BILINEAR)

            z_values[xCount][yCount] = measured_z + zoffset;

          #endif

          abl_should_enable = false;
          idle();

        } // inner
      } // outer

    #elif ENABLED(AUTO_BED_LEVELING_3POINT)

      // Probe at 3 arbitrary points

      for (uint8_t i = 0; i < 3; ++i) {
        // Retain the last probe position
        xProbe = points[i].x;
        yProbe = points[i].y;
        measured_z = faux ? 0.001 * random(-100, 101) : probe_pt(xProbe, yProbe, stow_probe_after_each, verbose_level);
        if (isnan(measured_z)) {
          set_bed_leveling_enabled(abl_should_enable);
          break;
        }
        points[i].z = measured_z;
      }

      if (!dryrun && !isnan(measured_z)) {
        vector_3 planeNormal = vector_3::cross(points[0] - points[1], points[2] - points[1]).get_normal();
        if (planeNormal.z < 0) {
          planeNormal.x *= -1;
          planeNormal.y *= -1;
          planeNormal.z *= -1;
        }
        planner.bed_level_matrix = matrix_3x3::create_look_at(planeNormal);

        // Can't re-enable (on error) until the new grid is written
        abl_should_enable = false;
      }

    #endif // AUTO_BED_LEVELING_3POINT

    // Raise to _Z_CLEARANCE_DEPLOY_PROBE. Stow the probe.
    if (STOW_PROBE()) {
      set_bed_leveling_enabled(abl_should_enable);
      measured_z = NAN;
    }
  }
  #endif // !PROBE_MANUALLY

  //
  // G29 Finishing Code
  //
  // Unless this is a dry run, auto bed leveling will
  // definitely be enabled after this point.
  //
  // If code above wants to continue leveling, it should
  // return or loop before this point.
  //

  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) DEBUG_POS("> probing complete", current_position);
  #endif

  #if ENABLED(PROBE_MANUALLY)
    g29_in_progress = false;
    #if ENABLED(LCD_BED_LEVELING)
      lcd_wait_for_move = false;
    #endif
  #endif

  // Calculate leveling, print reports, correct the position
  if (!isnan(measured_z)) {
    #if ENABLED(AUTO_BED_LEVELING_BILINEAR)

      if (!dryrun) extrapolate_unprobed_bed_level();
      print_bilinear_leveling_grid();

      refresh_bed_level();

      #if ENABLED(ABL_BILINEAR_SUBDIVISION)
        print_bilinear_leveling_grid_virt();
      #endif

    #elif ENABLED(AUTO_BED_LEVELING_LINEAR)

      // For LINEAR leveling calculate matrix, print reports, correct the position

      /**
       * solve the plane equation ax + by + d = z
       * A is the matrix with rows [x y 1] for all the probed points
       * B is the vector of the Z positions
       * the normal vector to the plane is formed by the coefficients of the
       * plane equation in the standard form, which is Vx*x+Vy*y+Vz*z+d = 0
       * so Vx = -a Vy = -b Vz = 1 (we want the vector facing towards positive Z
       */
      float plane_equation_coefficients[3];

      finish_incremental_LSF(&lsf_results);
      plane_equation_coefficients[0] = -lsf_results.A;  // We should be able to eliminate the '-' on these three lines and down below
      plane_equation_coefficients[1] = -lsf_results.B;  // but that is not yet tested.
      plane_equation_coefficients[2] = -lsf_results.D;

      mean /= abl2;

      if (verbose_level) {
        SERIAL_PROTOCOLPGM("Eqn coefficients: a: ");
        SERIAL_PROTOCOL_F(plane_equation_coefficients[0], 8);
        SERIAL_PROTOCOLPGM(" b: ");
        SERIAL_PROTOCOL_F(plane_equation_coefficients[1], 8);
        SERIAL_PROTOCOLPGM(" d: ");
        SERIAL_PROTOCOL_F(plane_equation_coefficients[2], 8);
        SERIAL_EOL();
        if (verbose_level > 2) {
          SERIAL_PROTOCOLPGM("Mean of sampled points: ");
          SERIAL_PROTOCOL_F(mean, 8);
          SERIAL_EOL();
        }
      }

      // Create the matrix but don't correct the position yet
      if (!dryrun)
        planner.bed_level_matrix = matrix_3x3::create_look_at(
          vector_3(-plane_equation_coefficients[0], -plane_equation_coefficients[1], 1)    // We can eliminate the '-' here and up above
        );

      // Show the Topography map if enabled
      if (do_topography_map) {

        SERIAL_PROTOCOLLNPGM("\nBed Height Topography:\n"
                               "   +--- BACK --+\n"
                               "   |           |\n"
                               " L |    (+)    | R\n"
                               " E |           | I\n"
                               " F | (-) N (+) | G\n"
                               " T |           | H\n"
                               "   |    (-)    | T\n"
                               "   |           |\n"
                               "   O-- FRONT --+\n"
                               " (0,0)");

        float min_diff = 999;

        for (int8_t yy = abl_grid_points_y - 1; yy >= 0; yy--) {
          for (uint8_t xx = 0; xx < abl_grid_points_x; xx++) {
            int ind = indexIntoAB[xx][yy];
            float diff = eqnBVector[ind] - mean,
                  x_tmp = eqnAMatrix[ind + 0 * abl2],
                  y_tmp = eqnAMatrix[ind + 1 * abl2],
                  z_tmp = 0;

            apply_rotation_xyz(planner.bed_level_matrix, x_tmp, y_tmp, z_tmp);

            NOMORE(min_diff, eqnBVector[ind] - z_tmp);

            if (diff >= 0.0)
              SERIAL_PROTOCOLPGM(" +");   // Include + for column alignment
            else
              SERIAL_PROTOCOLCHAR(' ');
            SERIAL_PROTOCOL_F(diff, 5);
          } // xx
          SERIAL_EOL();
        } // yy
        SERIAL_EOL();

        if (verbose_level > 3) {
          SERIAL_PROTOCOLLNPGM("\nCorrected Bed Height vs. Bed Topology:");

          for (int8_t yy = abl_grid_points_y - 1; yy >= 0; yy--) {
            for (uint8_t xx = 0; xx < abl_grid_points_x; xx++) {
              int ind = indexIntoAB[xx][yy];
              float x_tmp = eqnAMatrix[ind + 0 * abl2],
                    y_tmp = eqnAMatrix[ind + 1 * abl2],
                    z_tmp = 0;

              apply_rotation_xyz(planner.bed_level_matrix, x_tmp, y_tmp, z_tmp);

              float diff = eqnBVector[ind] - z_tmp - min_diff;
              if (diff >= 0.0)
                SERIAL_PROTOCOLPGM(" +");
              // Include + for column alignment
              else
                SERIAL_PROTOCOLCHAR(' ');
              SERIAL_PROTOCOL_F(diff, 5);
            } // xx
            SERIAL_EOL();
          } // yy
          SERIAL_EOL();
        }
      } //do_topography_map

    #endif // AUTO_BED_LEVELING_LINEAR

    #if ABL_PLANAR

      // For LINEAR and 3POINT leveling correct the current position

      if (verbose_level > 0)
        planner.bed_level_matrix.debug(PSTR("\n\nBed Level Correction Matrix:"));

      if (!dryrun) {
        //
        // Correct the current XYZ position based on the tilted plane.
        //

        #if ENABLED(DEBUG_LEVELING_FEATURE)
          if (DEBUGGING(LEVELING)) DEBUG_POS("G29 uncorrected XYZ", current_position);
        #endif

        float converted[XYZ];
        COPY(converted, current_position);

        planner.leveling_active = true;
        planner.unapply_leveling(converted); // use conversion machinery
        planner.leveling_active = false;

        // Use the last measured distance to the bed, if possible
        if ( NEAR(current_position[X_AXIS], xProbe - (X_PROBE_OFFSET_FROM_EXTRUDER))
          && NEAR(current_position[Y_AXIS], yProbe - (Y_PROBE_OFFSET_FROM_EXTRUDER))
        ) {
          const float simple_z = current_position[Z_AXIS] - measured_z;
          #if ENABLED(DEBUG_LEVELING_FEATURE)
            if (DEBUGGING(LEVELING)) {
              SERIAL_ECHOPAIR("Z from Probe:", simple_z);
              SERIAL_ECHOPAIR("  Matrix:", converted[Z_AXIS]);
              SERIAL_ECHOLNPAIR("  Discrepancy:", simple_z - converted[Z_AXIS]);
            }
          #endif
          converted[Z_AXIS] = simple_z;
        }

        // The rotated XY and corrected Z are now current_position
        COPY(current_position, converted);

        #if ENABLED(DEBUG_LEVELING_FEATURE)
          if (DEBUGGING(LEVELING)) DEBUG_POS("G29 corrected XYZ", current_position);
        #endif
      }

    #elif ENABLED(AUTO_BED_LEVELING_BILINEAR)

      if (!dryrun) {
        #if ENABLED(DEBUG_LEVELING_FEATURE)
          if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPAIR("G29 uncorrected Z:", current_position[Z_AXIS]);
        #endif

        // Unapply the offset because it is going to be immediately applied
        // and cause compensation movement in Z
        current_position[Z_AXIS] -= bilinear_z_offset(current_position);

        #if ENABLED(DEBUG_LEVELING_FEATURE)
          if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPAIR(" corrected Z:", current_position[Z_AXIS]);
        #endif
      }

    #endif // ABL_PLANAR

    #ifdef Z_PROBE_END_SCRIPT
      #if ENABLED(DEBUG_LEVELING_FEATURE)
        if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPAIR("Z Probe End Script: ", Z_PROBE_END_SCRIPT);
      #endif
      enqueue_and_echo_commands_P(PSTR(Z_PROBE_END_SCRIPT));
      stepper.synchronize();
    #endif

    // Auto Bed Leveling is complete! Enable if possible.
    planner.leveling_active = dryrun ? abl_should_enable : true;
  } // !isnan(measured_z)

  // Restore state after probing
  if (!faux) clean_up_after_endstop_or_probe_move();

  #if ENABLED(DEBUG_LEVELING_FEATURE)
    if (DEBUGGING(LEVELING)) SERIAL_ECHOLNPGM("<<< G29");
  #endif

  report_current_position();

  KEEPALIVE_STATE(IN_HANDLER);

  if (planner.leveling_active)
    SYNC_PLAN_POSITION_KINEMATIC();
}

#endif // OLDSCHOOL_ABL
