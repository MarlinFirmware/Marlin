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

  //#define UBL_DEVEL_DEBUGGING

  #include "ubl.h"
  #include "Marlin.h"
  #include "hex_print_routines.h"
  #include "configuration_store.h"
  #include "ultralcd.h"
  #include "stepper.h"
  #include "planner.h"
  #include "parser.h"
  #include "serial.h"
  #include "bitmap_flags.h"

  #include <math.h>
  #include "least_squares_fit.h"

  #define UBL_G29_P31

  extern float destination[XYZE], current_position[XYZE];

  #if ENABLED(NEWPANEL)
    void lcd_return_to_status();
    void _lcd_ubl_output_map_lcd();
  #endif

  #define SIZE_OF_LITTLE_RAISE 1
  #define BIG_RAISE_NOT_NEEDED 0

  int    unified_bed_leveling::g29_verbose_level,
         unified_bed_leveling::g29_phase_value,
         unified_bed_leveling::g29_repetition_cnt,
         unified_bed_leveling::g29_storage_slot = 0,
         unified_bed_leveling::g29_map_type;
  bool   unified_bed_leveling::g29_c_flag,
         unified_bed_leveling::g29_x_flag,
         unified_bed_leveling::g29_y_flag;
  float  unified_bed_leveling::g29_x_pos,
         unified_bed_leveling::g29_y_pos,
         unified_bed_leveling::g29_card_thickness = 0,
         unified_bed_leveling::g29_constant = 0;

  #if HAS_BED_PROBE
    int  unified_bed_leveling::g29_grid_size;
  #endif

  /**
   *   G29: Unified Bed Leveling by Roxy
   *
   *   Parameters understood by this leveling system:
   *
   *   A     Activate   Activate the Unified Bed Leveling system.
   *
   *   B #   Business   Use the 'Business Card' mode of the Manual Probe subsystem with P2.
   *                    Note: A non-compressible Spark Gap feeler gauge is recommended over a business card.
   *                    In this mode of G29 P2, a business or index card is used as a shim that the nozzle can
   *                    grab onto as it is lowered. In principle, the nozzle-bed distance is the same when the
   *                    same resistance is felt in the shim. You can omit the numerical value on first invocation
   *                    of G29 P2 B to measure shim thickness. Subsequent use of 'B' will apply the previously-
   *                    measured thickness by default.
   *
   *   C     Continue   G29 P1 C continues the generation of a partially-constructed Mesh without invalidating
   *                    previous measurements.
   *
   *   C                G29 P2 C tells the Manual Probe subsystem to not use the current nozzle
   *                    location in its search for the closest unmeasured Mesh Point. Instead, attempt to
   *                    start at one end of the uprobed points and Continue sequentially.
   *
   *                    G29 P3 C specifies the Constant for the fill. Otherwise, uses a "reasonable" value.
   *
   *   C     Current    G29 Z C uses the Current location (instead of bed center or nearest edge).
   *
   *   D     Disable    Disable the Unified Bed Leveling system.
   *
   *   E     Stow_probe Stow the probe after each sampled point.
   *
   *   F #   Fade       Fade the amount of Mesh Based Compensation over a specified height. At the
   *                    specified height, no correction is applied and natural printer kenimatics take over. If no
   *                    number is specified for the command, 10mm is assumed to be reasonable.
   *
   *   H #   Height     With P2, 'H' specifies the Height to raise the nozzle after each manual probe of the bed.
   *                    If omitted, the nozzle will raise by Z_CLEARANCE_BETWEEN_PROBES.
   *
   *   H #   Offset     With P4, 'H' specifies the Offset above the mesh height to place the nozzle.
   *                    If omitted, Z_CLEARANCE_BETWEEN_PROBES will be used.
   *
   *   I #   Invalidate Invalidate the specified number of Mesh Points near the given 'X' 'Y'. If X or Y are omitted,
   *                    the nozzle location is used. If no 'I' value is given, only the point nearest to the location
   *                    is invalidated. Use 'T' to produce a map afterward. This command is useful to invalidate a
   *                    portion of the Mesh so it can be adjusted using other UBL tools. When attempting to invalidate
   *                    an isolated bad mesh point, the 'T' option shows the nozzle position in the Mesh with (#). You
   *                    can move the nozzle around and use this feature to select the center of the area (or cell) to
   *                    invalidate.
   *
   *   J #   Grid       Perform a Grid Based Leveling of the current Mesh using a grid with n points on a side.
   *                    Not specifying a grid size will invoke the 3-Point leveling function.
   *
   *   K #   Kompare    Kompare current Mesh with stored Mesh # replacing current Mesh with the result. This
   *                    command literally performs a diff between two Meshes.
   *
   *   L     Load       Load Mesh from the previously activated location in the EEPROM.
   *
   *   L #   Load       Load Mesh from the specified location in the EEPROM. Set this location as activated
   *                    for subsequent Load and Store operations.
   *
   *   The P or Phase commands are used for the bulk of the work to setup a Mesh. In general, your Mesh will
   *   start off being initialized with a G29 P0 or a G29 P1. Further refinement of the Mesh happens with
   *   each additional Phase that processes it.
   *
   *   P0    Phase 0    Zero Mesh Data and turn off the Mesh Compensation System. This reverts the
   *                    3D Printer to the same state it was in before the Unified Bed Leveling Compensation
   *                    was turned on. Setting the entire Mesh to Zero is a special case that allows
   *                    a subsequent G or T leveling operation for backward compatibility.
   *
   *   P1    Phase 1    Invalidate entire Mesh and continue with automatic generation of the Mesh data using
   *                    the Z-Probe. Usually the probe can't reach all areas that the nozzle can reach. For delta
   *                    printers only the areas where the probe and nozzle can both reach will be automatically probed.
   *
   *                    Unreachable points will be handled in Phase 2 and Phase 3.
   *
   *                    Use 'C' to leave the previous mesh intact and automatically probe needed points. This allows you
   *                    to invalidate parts of the Mesh but still use Automatic Probing.
   *
   *                    The 'X' and 'Y' parameters prioritize where to try and measure points. If omitted, the current
   *                    probe position is used.
   *
   *                    Use 'T' (Topology) to generate a report of mesh generation.
   *
   *                    P1 will suspend Mesh generation if the controller button is held down. Note that you may need
   *                    to press and hold the switch for several seconds if moves are underway.
   *
   *   P2    Phase 2    Probe unreachable points.
   *
   *                    Use 'H' to set the height between Mesh points. If omitted, Z_CLEARANCE_BETWEEN_PROBES is used.
   *                    Smaller values will be quicker. Move the nozzle down till it barely touches the bed. Make sure the
   *                    nozzle is clean and unobstructed. Use caution and move slowly. This can damage your printer!
   *                    (Uses SIZE_OF_LITTLE_RAISE mm if the nozzle is moving less than BIG_RAISE_NOT_NEEDED mm.)
   *
   *                    The 'H' value can be negative if the Mesh dips in a large area. Press and hold the
   *                    controller button to terminate the current Phase 2 command. You can then re-issue "G29 P 2"
   *                    with an 'H' parameter more suitable for the area you're manually probing. Note that the command
   *                    tries to start in a corner of the bed where movement will be predictable. Override the distance
   *                    calculation location with the X and Y parameters. You can print a Mesh Map (G29 T) to see where
   *                    the mesh is invalidated and where the nozzle needs to move to complete the command. Use 'C' to
   *                    indicate that the search should be based on the current position.
   *
   *                    The 'B' parameter for this command is described above. It places the manual probe subsystem into
   *                    Business Card mode where the thickness of a business card is measured and then used to accurately
   *                    set the nozzle height in all manual probing for the duration of the command. A Business card can
   *                    be used, but you'll get better results with a flexible Shim that doesn't compress. This makes it
   *                    easier to produce similar amounts of force and get more accurate measurements. Google if you're
   *                    not sure how to use a shim.
   *
   *                    The 'T' (Map) parameter helps track Mesh building progress.
   *
   *                    NOTE: P2 requires an LCD controller!
   *
   *   P3    Phase 3    Fill the unpopulated regions of the Mesh with a fixed value. There are two different paths to
   *                    go down:
   *
   *                    - If a 'C' constant is specified, the closest invalid mesh points to the nozzle will be filled,
   *                      and a repeat count can then also be specified with 'R'.
   *
   *                    - Leaving out 'C' invokes Smart Fill, which scans the mesh from the edges inward looking for
   *                      invalid mesh points. Adjacent points are used to determine the bed slope. If the bed is sloped
   *                      upward from the invalid point, it takes the value of the nearest point. If sloped downward, it's
   *                      replaced by a value that puts all three points in a line. This version of G29 P3 is a quick, easy
   *                      and (usually) safe way to populate unprobed mesh regions before continuing to G26 Mesh Validation
   *                      Pattern. Note that this populates the mesh with unverified values. Pay attention and use caution.
   *
   *   P4    Phase 4    Fine tune the Mesh. The Delta Mesh Compensation System assumes the existence of
   *                    an LCD Panel. It is possible to fine tune the mesh without an LCD Panel using
   *                    G42 and M421. See the UBL documentation for further details.
   *
   *                    Phase 4 is meant to be used with G26 Mesh Validation to fine tune the mesh by direct editing
   *                    of Mesh Points. Raise and lower points to fine tune the mesh until it gives consistently reliable
   *                    adhesion.
   *
   *                    P4 moves to the closest Mesh Point (and/or the given X Y), raises the nozzle above the mesh height
   *                    by the given 'H' offset (or default 0), and waits while the controller is used to adjust the nozzle
   *                    height. On click the displayed height is saved in the mesh.
   *
   *                    Start Phase 4 at a specific location with X and Y. Adjust a specific number of Mesh Points with
   *                    the 'R' (Repeat) parameter. (If 'R' is left out, the whole matrix is assumed.) This command can be
   *                    terminated early (e.g., after editing the area of interest) by pressing and holding the encoder button.
   *
   *                    The general form is G29 P4 [R points] [X position] [Y position]
   *
   *                    The H [offset] parameter is useful if a shim is used to fine-tune the mesh. For a 0.4mm shim the
   *                    command would be G29 P4 H0.4. The nozzle is moved to the shim height, you adjust height to the shim,
   *                    and on click the height minus the shim thickness will be saved in the mesh.
   *
   *                    !!Use with caution, as a very poor mesh could cause the nozzle to crash into the bed!!
   *
   *                    NOTE:  P4 is not available unless you have LCD support enabled!
   *
   *   P5    Phase 5    Find Mean Mesh Height and Standard Deviation. Typically, it is easier to use and
   *                    work with the Mesh if it is Mean Adjusted. You can specify a C parameter to
   *                    Correct the Mesh to a 0.00 Mean Height. Adding a C parameter will automatically
   *                    execute a G29 P6 C <mean height>.
   *
   *   P6    Phase 6    Shift Mesh height. The entire Mesh's height is adjusted by the height specified
   *                    with the C parameter. Being able to adjust the height of a Mesh is useful tool. It
   *                    can be used to compensate for poorly calibrated Z-Probes and other errors. Ideally,
   *                    you should have the Mesh adjusted for a Mean Height of 0.00 and the Z-Probe measuring
   *                    0.000 at the Z Home location.
   *
   *   Q     Test       Load specified Test Pattern to assist in checking correct operation of system. This
   *                    command is not anticipated to be of much value to the typical user. It is intended
   *                    for developers to help them verify correct operation of the Unified Bed Leveling System.
   *
   *   R #   Repeat     Repeat this command the specified number of times. If no number is specified the
   *                    command will be repeated GRID_MAX_POINTS_X * GRID_MAX_POINTS_Y times.
   *
   *   S     Store      Store the current Mesh in the Activated area of the EEPROM. It will also store the
   *                    current state of the Unified Bed Leveling system in the EEPROM.
   *
   *   S #   Store      Store the current Mesh at the specified location in EEPROM. Activate this location
   *                    for subsequent Load and Store operations. Valid storage slot numbers begin at 0 and
   *                    extend to a limit related to the available EEPROM storage.
   *
   *   S -1  Store      Print the current Mesh as G-code that can be used to restore the mesh anytime.
   *
   *   T     Topology   Display the Mesh Map Topology.
   *                    'T' can be used alone (e.g., G29 T) or in combination with most of the other commands.
   *                    This option works with all Phase commands (e.g., G29 P4 R 5 T X 50 Y100 C -.1 O)
   *                    This parameter can also specify a Map Type. T0 (the default) is user-readable. T1 can
   *                    is suitable to paste into a spreadsheet for a 3D graph of the mesh.
   *
   *   U     Unlevel    Perform a probe of the outer perimeter to assist in physically leveling unlevel beds.
   *                    Only used for G29 P1 T U. This speeds up the probing of the edge of the bed. Useful
   *                    when the entire bed doesn't need to be probed because it will be adjusted.
   *
   *   V #   Verbosity  Set the verbosity level (0-4) for extra details. (Default 0)
   *
   *   W     What?      Display valuable Unified Bed Leveling System data.
   *
   *   X #              X Location for this command
   *
   *   Y #              Y Location for this command
   *
   *
   *   Release Notes:
   *   You MUST do M502, M500 to initialize the storage. Failure to do this will cause all
   *   kinds of problems. Enabling EEPROM Storage is required.
   *
   *   When you do a G28 and G29 P1 to automatically build your first mesh, you are going to notice that
   *   UBL probes points increasingly further from the starting location. (The starting location defaults
   *   to the center of the bed.) In contrast, ABL and MBL follow a zigzag pattern. The spiral pattern is
   *   especially better for Delta printers, since it populates the center of the mesh first, allowing for
   *   a quicker test print to verify settings. You don't need to populate the entire mesh to use it.
   *   After all, you don't want to spend a lot of time generating a mesh only to realize the resolution
   *   or zprobe_zoffset are incorrect. Mesh-generation gathers points starting closest to the nozzle unless
   *   an (X,Y) coordinate pair is given.
   *
   *   Unified Bed Leveling uses a lot of EEPROM storage to hold its data, and it takes some effort to get
   *   the mesh just right. To prevent this valuable data from being destroyed as the EEPROM structure
   *   evolves, UBL stores all mesh data at the end of EEPROM.
   *
   *   UBL is founded on Edward Patel's Mesh Bed Leveling code. A big 'Thanks!' to him and the creators of
   *   3-Point and Grid Based leveling. Combining their contributions we now have the functionality and
   *   features of all three systems combined.
   */

  void unified_bed_leveling::G29() {

    if (g29_parameter_parsing()) return; // Abort on parameter error

    const int8_t p_val = parser.intval('P', -1);
    const bool may_move = p_val == 1 || p_val == 2 || p_val == 4 || parser.seen('J');

    // Check for commands that require the printer to be homed
    if (may_move) {
      #if ENABLED(DUAL_X_CARRIAGE)
        if (active_extruder != 0) tool_change(0);
      #endif
      if (axis_unhomed_error()) home_all_axes();
    }

    // Invalidate Mesh Points. This command is a little bit asymmetrical because
    // it directly specifies the repetition count and does not use the 'R' parameter.
    if (parser.seen('I')) {
      uint8_t cnt = 0;
      g29_repetition_cnt = parser.has_value() ? parser.value_int() : 1;
      if (g29_repetition_cnt >= GRID_MAX_POINTS) {
        set_all_mesh_points_to_value(NAN);
      }
      else {
        while (g29_repetition_cnt--) {
          if (cnt > 20) { cnt = 0; idle(); }
          const mesh_index_pair location = find_closest_mesh_point_of_type(REAL, g29_x_pos, g29_y_pos, USE_NOZZLE_AS_REFERENCE, NULL);
          if (location.x_index < 0) {
            // No more REACHABLE mesh points to invalidate, so we ASSUME the user
            // meant to invalidate the ENTIRE mesh, which cannot be done with
            // find_closest_mesh_point loop which only returns REACHABLE points.
            set_all_mesh_points_to_value(NAN);
            SERIAL_PROTOCOLLNPGM("Entire Mesh invalidated.\n");
            break;            // No more invalid Mesh Points to populate
          }
          z_values[location.x_index][location.y_index] = NAN;
          cnt++;
        }
      }
      SERIAL_PROTOCOLLNPGM("Locations invalidated.\n");
    }

    if (parser.seen('Q')) {
      const int test_pattern = parser.has_value() ? parser.value_int() : -99;
      if (!WITHIN(test_pattern, -1, 2)) {
        SERIAL_PROTOCOLLNPGM("Invalid test_pattern value. (-1 to 2)\n");
        return;
      }
      SERIAL_PROTOCOLLNPGM("Loading test_pattern values.\n");
      switch (test_pattern) {
        case -1:
          g29_eeprom_dump();
          break;
        case 0:
          for (uint8_t x = 0; x < GRID_MAX_POINTS_X; x++) {   // Create a bowl shape - similar to
            for (uint8_t y = 0; y < GRID_MAX_POINTS_Y; y++) { // a poorly calibrated Delta.
              const float p1 = 0.5f * (GRID_MAX_POINTS_X) - x,
                          p2 = 0.5f * (GRID_MAX_POINTS_Y) - y;
              z_values[x][y] += 2.0f * HYPOT(p1, p2);
            }
          }
          break;
        case 1:
          for (uint8_t x = 0; x < GRID_MAX_POINTS_X; x++) {  // Create a diagonal line several Mesh cells thick that is raised
            z_values[x][x] += 9.999f;
            z_values[x][x + (x < GRID_MAX_POINTS_Y - 1) ? 1 : -1] += 9.999f; // We want the altered line several mesh points thick
          }
          break;
        case 2:
          // Allow the user to specify the height because 10mm is a little extreme in some cases.
          for (uint8_t x = (GRID_MAX_POINTS_X) / 3; x < 2 * (GRID_MAX_POINTS_X) / 3; x++)   // Create a rectangular raised area in
            for (uint8_t y = (GRID_MAX_POINTS_Y) / 3; y < 2 * (GRID_MAX_POINTS_Y) / 3; y++) // the center of the bed
              z_values[x][y] += parser.seen('C') ? g29_constant : 9.99f;
          break;
      }
    }

    #if HAS_BED_PROBE

      if (parser.seen('J')) {
        if (g29_grid_size) {  // if not 0 it is a normal n x n grid being probed
          save_ubl_active_state_and_disable();
          tilt_mesh_based_on_probed_grid(false /* false says to do normal grid probing */ );
          restore_ubl_active_state_and_leave();
        }
        else { // grid_size == 0 : A 3-Point leveling has been requested

          save_ubl_active_state_and_disable();
          tilt_mesh_based_on_probed_grid(true /* true says to do 3-Point leveling */ );
          restore_ubl_active_state_and_leave();
        }
        do_blocking_move_to_xy(0.5f * (MESH_MAX_X - (MESH_MIN_X)), 0.5f * (MESH_MAX_Y - (MESH_MIN_Y)));
        report_current_position();
      }

    #endif // HAS_BED_PROBE

    if (parser.seen('P')) {
      if (WITHIN(g29_phase_value, 0, 1) && storage_slot == -1) {
        storage_slot = 0;
        SERIAL_PROTOCOLLNPGM("Default storage slot 0 selected.");
      }

      switch (g29_phase_value) {
        case 0:
          //
          // Zero Mesh Data
          //
          reset();
          SERIAL_PROTOCOLLNPGM("Mesh zeroed.");
          break;

        #if HAS_BED_PROBE

          case 1:
            //
            // Invalidate Entire Mesh and Automatically Probe Mesh in areas that can be reached by the probe
            //
            if (!parser.seen('C')) {
              invalidate();
              SERIAL_PROTOCOLLNPGM("Mesh invalidated. Probing mesh.");
            }
            if (g29_verbose_level > 1) {
              SERIAL_PROTOCOLPAIR("Probing Mesh Points Closest to (", g29_x_pos);
              SERIAL_PROTOCOLCHAR(',');
              SERIAL_PROTOCOL(g29_y_pos);
              SERIAL_PROTOCOLLNPGM(").\n");
            }
            probe_entire_mesh(g29_x_pos + X_PROBE_OFFSET_FROM_EXTRUDER, g29_y_pos + Y_PROBE_OFFSET_FROM_EXTRUDER,
                              parser.seen('T'), parser.seen('E'), parser.seen('U'));

            report_current_position();
            break;

        #endif // HAS_BED_PROBE

        case 2: {
          #if ENABLED(NEWPANEL)
            //
            // Manually Probe Mesh in areas that can't be reached by the probe
            //
            SERIAL_PROTOCOLLNPGM("Manually probing unreachable mesh locations.");
            do_blocking_move_to_z(Z_CLEARANCE_BETWEEN_PROBES);

            if (parser.seen('C') && !g29_x_flag && !g29_y_flag) {
              /**
               * Use a good default location for the path.
               * The flipped > and < operators in these comparisons is intentional.
               * It should cause the probed points to follow a nice path on Cartesian printers.
               * It may make sense to have Delta printers default to the center of the bed.
               * Until that is decided, this can be forced with the X and Y parameters.
               */
              #if IS_KINEMATIC
                g29_x_pos = X_HOME_POS;
                g29_y_pos = Y_HOME_POS;
              #else // cartesian
                g29_x_pos = X_PROBE_OFFSET_FROM_EXTRUDER > 0 ? X_BED_SIZE : 0;
                g29_y_pos = Y_PROBE_OFFSET_FROM_EXTRUDER < 0 ? Y_BED_SIZE : 0;
              #endif
            }

            if (parser.seen('B')) {
              g29_card_thickness = parser.has_value() ? parser.value_float() : measure_business_card_thickness((float) Z_CLEARANCE_BETWEEN_PROBES);
              if (ABS(g29_card_thickness) > 1.5f) {
                SERIAL_PROTOCOLLNPGM("?Error in Business Card measurement.");
                return;
              }
            }

            if (!position_is_reachable(g29_x_pos, g29_y_pos)) {
              SERIAL_PROTOCOLLNPGM("XY outside printable radius.");
              return;
            }

            const float height = parser.floatval('H', Z_CLEARANCE_BETWEEN_PROBES);
            manually_probe_remaining_mesh(g29_x_pos, g29_y_pos, height, g29_card_thickness, parser.seen('T'));

            SERIAL_PROTOCOLLNPGM("G29 P2 finished.");

            report_current_position();

          #else

            SERIAL_PROTOCOLLNPGM("?P2 is only available when an LCD is present.");
            return;

          #endif
        } break;

        case 3: {
          /**
           * Populate invalid mesh areas. Proceed with caution.
           * Two choices are available:
           *   - Specify a constant with the 'C' parameter.
           *   - Allow 'G29 P3' to choose a 'reasonable' constant.
           */

          if (g29_c_flag) {
            if (g29_repetition_cnt >= GRID_MAX_POINTS) {
              set_all_mesh_points_to_value(g29_constant);
            }
            else {
              while (g29_repetition_cnt--) {  // this only populates reachable mesh points near
                const mesh_index_pair location = find_closest_mesh_point_of_type(INVALID, g29_x_pos, g29_y_pos, USE_NOZZLE_AS_REFERENCE, NULL);
                if (location.x_index < 0) {
                  // No more REACHABLE INVALID mesh points to populate, so we ASSUME
                  // user meant to populate ALL INVALID mesh points to value
                  for (uint8_t x = 0; x < GRID_MAX_POINTS_X; x++)
                    for (uint8_t y = 0; y < GRID_MAX_POINTS_Y; y++)
                      if (isnan(z_values[x][y]))
                        z_values[x][y] = g29_constant;
                  break; // No more invalid Mesh Points to populate
                }
                z_values[location.x_index][location.y_index] = g29_constant;
              }
            }
          }
          else {
            const float cvf = parser.value_float();
            switch ((int)truncf(cvf * 10.0f) - 30) {   // 3.1 -> 1
              #if ENABLED(UBL_G29_P31)
                case 1: {

                  // P3.1  use least squares fit to fill missing mesh values
                  // P3.10 zero weighting for distance, all grid points equal, best fit tilted plane
                  // P3.11 10X weighting for nearest grid points versus farthest grid points
                  // P3.12 100X distance weighting
                  // P3.13 1000X distance weighting, approaches simple average of nearest points

                  const float weight_power  = (cvf - 3.10f) * 100.0f,  // 3.12345 -> 2.345
                              weight_factor = weight_power ? POW(10.0f, weight_power) : 0;
                  smart_fill_wlsf(weight_factor);
                }
                break;
              #endif
              case 0:   // P3 or P3.0
              default:  // and anything P3.x that's not P3.1
                smart_fill_mesh();  // Do a 'Smart' fill using nearby known values
                break;
            }
          }
          break;
        }

        case 4: // Fine Tune (i.e., Edit) the Mesh
          #if ENABLED(NEWPANEL)
            fine_tune_mesh(g29_x_pos, g29_y_pos, parser.seen('T'));
          #else
            SERIAL_PROTOCOLLNPGM("?P4 is only available when an LCD is present.");
            return;
          #endif
          break;

        case 5: adjust_mesh_to_mean(g29_c_flag, g29_constant); break;

        case 6: shift_mesh_height(); break;
      }
    }

    //
    // Much of the 'What?' command can be eliminated. But until we are fully debugged, it is
    // good to have the extra information. Soon... we prune this to just a few items
    //
    if (parser.seen('W')) g29_what_command();

    //
    // When we are fully debugged, this may go away. But there are some valid
    // use cases for the users. So we can wait and see what to do with it.
    //

    if (parser.seen('K')) // Kompare Current Mesh Data to Specified Stored Mesh
      g29_compare_current_mesh_to_stored_mesh();

    //
    // Load a Mesh from the EEPROM
    //

    if (parser.seen('L')) {     // Load Current Mesh Data
      g29_storage_slot = parser.has_value() ? parser.value_int() : storage_slot;

      int16_t a = settings.calc_num_meshes();

      if (!a) {
        SERIAL_PROTOCOLLNPGM("?EEPROM storage not available.");
        return;
      }

      if (!WITHIN(g29_storage_slot, 0, a - 1)) {
        SERIAL_PROTOCOLLNPGM("?Invalid storage slot.");
        SERIAL_PROTOCOLLNPAIR("?Use 0 to ", a - 1);
        return;
      }

      settings.load_mesh(g29_storage_slot);
      storage_slot = g29_storage_slot;

      SERIAL_PROTOCOLLNPGM("Done.");
    }

    //
    // Store a Mesh in the EEPROM
    //

    if (parser.seen('S')) {     // Store (or Save) Current Mesh Data
      g29_storage_slot = parser.has_value() ? parser.value_int() : storage_slot;

      if (g29_storage_slot == -1)                     // Special case, the user wants to 'Export' the mesh to the
        return report_current_mesh();                 // host program to be saved on the user's computer

      int16_t a = settings.calc_num_meshes();

      if (!a) {
        SERIAL_PROTOCOLLNPGM("?EEPROM storage not available.");
        goto LEAVE;
      }

      if (!WITHIN(g29_storage_slot, 0, a - 1)) {
        SERIAL_PROTOCOLLNPGM("?Invalid storage slot.");
        SERIAL_PROTOCOLLNPAIR("?Use 0 to ", a - 1);
        goto LEAVE;
      }

      settings.store_mesh(g29_storage_slot);
      storage_slot = g29_storage_slot;

      SERIAL_PROTOCOLLNPGM("Done.");
    }

    if (parser.seen('T'))
      display_map(g29_map_type);

    LEAVE:

    #if ENABLED(NEWPANEL)
      lcd_reset_alert_level();
      lcd_quick_feedback(true);
      lcd_reset_status();
      lcd_external_control = false;
    #endif

    return;
  }

  void unified_bed_leveling::adjust_mesh_to_mean(const bool cflag, const float value) {
    float sum = 0;
    int n = 0;
    for (uint8_t x = 0; x < GRID_MAX_POINTS_X; x++)
      for (uint8_t y = 0; y < GRID_MAX_POINTS_Y; y++)
        if (!isnan(z_values[x][y])) {
          sum += z_values[x][y];
          n++;
        }

    const float mean = sum / n;

    //
    // Sum the squares of difference from mean
    //
    float sum_of_diff_squared = 0;
    for (uint8_t x = 0; x < GRID_MAX_POINTS_X; x++)
      for (uint8_t y = 0; y < GRID_MAX_POINTS_Y; y++)
        if (!isnan(z_values[x][y]))
          sum_of_diff_squared += sq(z_values[x][y] - mean);

    SERIAL_ECHOLNPAIR("# of samples: ", n);
    SERIAL_ECHOPGM("Mean Mesh Height: ");
    SERIAL_ECHO_F(mean, 6);
    SERIAL_EOL();

    const float sigma = SQRT(sum_of_diff_squared / (n + 1));
    SERIAL_ECHOPGM("Standard Deviation: ");
    SERIAL_ECHO_F(sigma, 6);
    SERIAL_EOL();

    if (cflag)
      for (uint8_t x = 0; x < GRID_MAX_POINTS_X; x++)
        for (uint8_t y = 0; y < GRID_MAX_POINTS_Y; y++)
          if (!isnan(z_values[x][y]))
            z_values[x][y] -= mean + value;
  }

  void unified_bed_leveling::shift_mesh_height() {
    for (uint8_t x = 0; x < GRID_MAX_POINTS_X; x++)
      for (uint8_t y = 0; y < GRID_MAX_POINTS_Y; y++)
        if (!isnan(z_values[x][y]))
          z_values[x][y] += g29_constant;
  }

  #if ENABLED(NEWPANEL)

    typedef void (*clickFunc_t)();

    bool click_and_hold(const clickFunc_t func=NULL) {
      if (is_lcd_clicked()) {
        lcd_quick_feedback(false); // Do NOT clear button status!  If cleared, the code
                                   // code can not look for a 'click and hold'
        const millis_t nxt = millis() + 1500UL;
        while (is_lcd_clicked()) {                // Loop while the encoder is pressed. Uses hardware flag!
          idle();                                 // idle, of course
          if (ELAPSED(millis(), nxt)) {           // After 1.5 seconds
            lcd_quick_feedback(true);
            if (func) (*func)();
            wait_for_release();
            safe_delay(50);                       // Debounce the Encoder wheel
            return true;
          }
        }
      }
      safe_delay(15);
      return false;
    }

  #endif // NEWPANEL

  #if HAS_BED_PROBE
    /**
     * Probe all invalidated locations of the mesh that can be reached by the probe.
     * This attempts to fill in locations closest to the nozzle's start location first.
     */
    void unified_bed_leveling::probe_entire_mesh(const float &rx, const float &ry, const bool do_ubl_mesh_map, const bool stow_probe, const bool do_furthest) {
      mesh_index_pair location;

      #if ENABLED(NEWPANEL)
        lcd_external_control = true;
      #endif

      save_ubl_active_state_and_disable();   // we don't do bed level correction because we want the raw data when we probe
      DEPLOY_PROBE();

      uint16_t count = GRID_MAX_POINTS;

      do {
        if (do_ubl_mesh_map) display_map(g29_map_type);

        #if ENABLED(NEWPANEL)
          if (is_lcd_clicked()) {
            SERIAL_PROTOCOLLNPGM("\nMesh only partially populated.\n");
            lcd_quick_feedback(false);
            STOW_PROBE();
            while (is_lcd_clicked()) idle();
            lcd_external_control = false;
            restore_ubl_active_state_and_leave();
            lcd_quick_feedback(true);
            safe_delay(50);  // Debounce the Encoder wheel
            return;
          }
        #endif

        if (do_furthest)
          location = find_furthest_invalid_mesh_point();
        else
          location = find_closest_mesh_point_of_type(INVALID, rx, ry, USE_PROBE_AS_REFERENCE, NULL);

        if (location.x_index >= 0) {    // mesh point found and is reachable by probe
          const float rawx = mesh_index_to_xpos(location.x_index),
                      rawy = mesh_index_to_ypos(location.y_index);

          const float measured_z = probe_pt(rawx, rawy, stow_probe ? PROBE_PT_STOW : PROBE_PT_RAISE, g29_verbose_level); // TODO: Needs error handling
          z_values[location.x_index][location.y_index] = measured_z;
        }
        SERIAL_FLUSH(); // Prevent host M105 buffer overrun.
      } while (location.x_index >= 0 && --count);

      STOW_PROBE();

      #ifdef Z_AFTER_PROBING
        move_z_after_probing();
      #endif

      restore_ubl_active_state_and_leave();

      do_blocking_move_to_xy(
        constrain(rx - (X_PROBE_OFFSET_FROM_EXTRUDER), MESH_MIN_X, MESH_MAX_X),
        constrain(ry - (Y_PROBE_OFFSET_FROM_EXTRUDER), MESH_MIN_Y, MESH_MAX_Y)
      );
    }


  #endif // HAS_BED_PROBE

  #if ENABLED(NEWPANEL)

    void unified_bed_leveling::move_z_with_encoder(const float &multiplier) {
      wait_for_release();
      while (!is_lcd_clicked()) {
        idle();
        reset_stepper_timeout(); // Keep steppers powered
        if (encoder_diff) {
          do_blocking_move_to_z(current_position[Z_AXIS] + float(encoder_diff) * multiplier);
          encoder_diff = 0;
        }
      }
    }

    float unified_bed_leveling::measure_point_with_encoder() {
      KEEPALIVE_STATE(PAUSED_FOR_USER);
      move_z_with_encoder(0.01f);
      KEEPALIVE_STATE(IN_HANDLER);
      return current_position[Z_AXIS];
    }

    static void echo_and_take_a_measurement() { SERIAL_PROTOCOLLNPGM(" and take a measurement."); }

    float unified_bed_leveling::measure_business_card_thickness(float in_height) {
      lcd_external_control = true;
      save_ubl_active_state_and_disable();   // Disable bed level correction for probing

      do_blocking_move_to(0.5f * (MESH_MAX_X - (MESH_MIN_X)), 0.5f * (MESH_MAX_Y - (MESH_MIN_Y)), in_height);
        //, MIN(planner.max_feedrate_mm_s[X_AXIS], planner.max_feedrate_mm_s[Y_AXIS]) * 0.5f);
      planner.synchronize();

      SERIAL_PROTOCOLPGM("Place shim under nozzle");
      LCD_MESSAGEPGM(MSG_UBL_BC_INSERT);
      lcd_return_to_status();
      echo_and_take_a_measurement();

      const float z1 = measure_point_with_encoder();
      do_blocking_move_to_z(current_position[Z_AXIS] + SIZE_OF_LITTLE_RAISE);
      planner.synchronize();

      SERIAL_PROTOCOLPGM("Remove shim");
      LCD_MESSAGEPGM(MSG_UBL_BC_REMOVE);
      echo_and_take_a_measurement();

      const float z2 = measure_point_with_encoder();

      do_blocking_move_to_z(current_position[Z_AXIS] + Z_CLEARANCE_BETWEEN_PROBES);

      const float thickness = ABS(z1 - z2);

      if (g29_verbose_level > 1) {
        SERIAL_PROTOCOLPGM("Business Card is ");
        SERIAL_PROTOCOL_F(thickness, 4);
        SERIAL_PROTOCOLLNPGM("mm thick.");
      }

      lcd_external_control = false;

      restore_ubl_active_state_and_leave();

      return thickness;
    }

    void abort_manual_probe_remaining_mesh() {
      SERIAL_PROTOCOLLNPGM("\nMesh only partially populated.");
      do_blocking_move_to_z(Z_CLEARANCE_DEPLOY_PROBE);
      lcd_external_control = false;
      KEEPALIVE_STATE(IN_HANDLER);
      lcd_quick_feedback(true);
      ubl.restore_ubl_active_state_and_leave();
    }

    void unified_bed_leveling::manually_probe_remaining_mesh(const float &rx, const float &ry, const float &z_clearance, const float &thick, const bool do_ubl_mesh_map) {

      lcd_external_control = true;

      save_ubl_active_state_and_disable();   // we don't do bed level correction because we want the raw data when we probe
      do_blocking_move_to(current_position[X_AXIS], current_position[Y_AXIS], z_clearance);

      lcd_return_to_status();

      mesh_index_pair location;
      do {
        location = find_closest_mesh_point_of_type(INVALID, rx, ry, USE_NOZZLE_AS_REFERENCE, NULL);
        // It doesn't matter if the probe can't reach the NAN location. This is a manual probe.
        if (location.x_index < 0 && location.y_index < 0) continue;

        const float xProbe = mesh_index_to_xpos(location.x_index),
                    yProbe = mesh_index_to_ypos(location.y_index);

        if (!position_is_reachable(xProbe, yProbe)) break; // SHOULD NOT OCCUR (find_closest_mesh_point only returns reachable points)

        LCD_MESSAGEPGM(MSG_UBL_MOVING_TO_NEXT);

        do_blocking_move_to(xProbe, yProbe, Z_CLEARANCE_BETWEEN_PROBES);
        do_blocking_move_to_z(z_clearance);

        KEEPALIVE_STATE(PAUSED_FOR_USER);
        lcd_external_control = true;

        if (do_ubl_mesh_map) display_map(g29_map_type);  // show user where we're probing

        serialprintPGM(parser.seen('B') ? PSTR(MSG_UBL_BC_INSERT) : PSTR(MSG_UBL_BC_INSERT2));

        const float z_step = 0.01f;                         // existing behavior: 0.01mm per click, occasionally step
        //const float z_step = planner.steps_to_mm[Z_AXIS]; // approx one step each click

        move_z_with_encoder(z_step);

        if (click_and_hold()) {
          SERIAL_PROTOCOLLNPGM("\nMesh only partially populated.");
          do_blocking_move_to_z(Z_CLEARANCE_DEPLOY_PROBE);
          lcd_external_control = false;
          KEEPALIVE_STATE(IN_HANDLER);
          restore_ubl_active_state_and_leave();
          return;
        }

        z_values[location.x_index][location.y_index] = current_position[Z_AXIS] - thick;
        if (g29_verbose_level > 2) {
          SERIAL_PROTOCOLPGM("Mesh Point Measured at: ");
          SERIAL_PROTOCOL_F(z_values[location.x_index][location.y_index], 6);
          SERIAL_EOL();
        }
        SERIAL_FLUSH(); // Prevent host M105 buffer overrun.
      } while (location.x_index >= 0 && location.y_index >= 0);

      if (do_ubl_mesh_map) display_map(g29_map_type);  // show user where we're probing

      restore_ubl_active_state_and_leave();
      KEEPALIVE_STATE(IN_HANDLER);
      do_blocking_move_to(rx, ry, Z_CLEARANCE_DEPLOY_PROBE);
    }
  #endif // NEWPANEL

  bool unified_bed_leveling::g29_parameter_parsing() {
    bool err_flag = false;

    #if ENABLED(NEWPANEL)
      LCD_MESSAGEPGM(MSG_UBL_DOING_G29);
      lcd_quick_feedback(true);
    #endif

    g29_constant = 0;
    g29_repetition_cnt = 0;

    g29_x_flag = parser.seenval('X');
    g29_x_pos = g29_x_flag ? parser.value_float() : current_position[X_AXIS];
    g29_y_flag = parser.seenval('Y');
    g29_y_pos = g29_y_flag ? parser.value_float() : current_position[Y_AXIS];

    if (parser.seen('R')) {
      g29_repetition_cnt = parser.has_value() ? parser.value_int() : GRID_MAX_POINTS;
      NOMORE(g29_repetition_cnt, GRID_MAX_POINTS);
      if (g29_repetition_cnt < 1) {
        SERIAL_PROTOCOLLNPGM("?(R)epetition count invalid (1+).\n");
        return UBL_ERR;
      }
    }

    g29_verbose_level = parser.seen('V') ? parser.value_int() : 0;
    if (!WITHIN(g29_verbose_level, 0, 4)) {
      SERIAL_PROTOCOLLNPGM("?(V)erbose level is implausible (0-4).\n");
      err_flag = true;
    }

    if (parser.seen('P')) {
      const int pv = parser.value_int();
      #if !HAS_BED_PROBE
        if (pv == 1) {
          SERIAL_PROTOCOLLNPGM("G29 P1 requires a probe.\n");
          err_flag = true;
        }
        else
      #endif
        {
          g29_phase_value = pv;
          if (!WITHIN(g29_phase_value, 0, 6)) {
            SERIAL_PROTOCOLLNPGM("?(P)hase value invalid (0-6).\n");
            err_flag = true;
          }
        }
    }

    if (parser.seen('J')) {
      #if HAS_BED_PROBE
        g29_grid_size = parser.has_value() ? parser.value_int() : 0;
        if (g29_grid_size && !WITHIN(g29_grid_size, 2, 9)) {
          SERIAL_PROTOCOLLNPGM("?Invalid grid size (J) specified (2-9).\n");
          err_flag = true;
        }
      #else
        SERIAL_PROTOCOLLNPGM("G29 J action requires a probe.\n");
        err_flag = true;
      #endif
    }

    if (g29_x_flag != g29_y_flag) {
      SERIAL_PROTOCOLLNPGM("Both X & Y locations must be specified.\n");
      err_flag = true;
    }

    // If X or Y are not valid, use center of the bed values
    if (!WITHIN(g29_x_pos, X_MIN_BED, X_MAX_BED)) g29_x_pos = X_CENTER;
    if (!WITHIN(g29_y_pos, Y_MIN_BED, Y_MAX_BED)) g29_y_pos = Y_CENTER;

    if (err_flag) return UBL_ERR;

    /**
     * Activate or deactivate UBL
     * Note: UBL's G29 restores the state set here when done.
     *       Leveling is being enabled here with old data, possibly
     *       none. Error handling should disable for safety...
     */
    if (parser.seen('A')) {
      if (parser.seen('D')) {
        SERIAL_PROTOCOLLNPGM("?Can't activate and deactivate at the same time.\n");
        return UBL_ERR;
      }
      set_bed_leveling_enabled(true);
      report_state();
    }
    else if (parser.seen('D')) {
      set_bed_leveling_enabled(false);
      report_state();
    }

    // Set global 'C' flag and its value
    if ((g29_c_flag = parser.seen('C')))
      g29_constant = parser.value_float();

    #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
      if (parser.seenval('F')) {
        const float fh = parser.value_float();
        if (!WITHIN(fh, 0, 100)) {
          SERIAL_PROTOCOLLNPGM("?(F)ade height for Bed Level Correction not plausible.\n");
          return UBL_ERR;
        }
        set_z_fade_height(fh);
      }
    #endif

    g29_map_type = parser.intval('T');
    if (!WITHIN(g29_map_type, 0, 2)) {
      SERIAL_PROTOCOLLNPGM("Invalid map type.\n");
      return UBL_ERR;
    }
    return UBL_OK;
  }

  static uint8_t ubl_state_at_invocation = 0;

  #ifdef UBL_DEVEL_DEBUGGING
    static uint8_t ubl_state_recursion_chk = 0;
  #endif

  void unified_bed_leveling::save_ubl_active_state_and_disable() {
    #ifdef UBL_DEVEL_DEBUGGING
      ubl_state_recursion_chk++;
      if (ubl_state_recursion_chk != 1) {
        SERIAL_ECHOLNPGM("save_ubl_active_state_and_disabled() called multiple times in a row.");
        #if ENABLED(NEWPANEL)
          LCD_MESSAGEPGM(MSG_UBL_SAVE_ERROR);
          lcd_quick_feedback(true);
        #endif
        return;
      }
    #endif
    ubl_state_at_invocation = planner.leveling_active;
    set_bed_leveling_enabled(false);
  }

  void unified_bed_leveling::restore_ubl_active_state_and_leave() {
    #ifdef UBL_DEVEL_DEBUGGING
      if (--ubl_state_recursion_chk) {
        SERIAL_ECHOLNPGM("restore_ubl_active_state_and_leave() called too many times.");
        #if ENABLED(NEWPANEL)
          LCD_MESSAGEPGM(MSG_UBL_RESTORE_ERROR);
          lcd_quick_feedback(true);
        #endif
        return;
      }
    #endif
    set_bed_leveling_enabled(ubl_state_at_invocation);
  }

  /**
   * Much of the 'What?' command can be eliminated. But until we are fully debugged, it is
   * good to have the extra information. Soon... we prune this to just a few items
   */
  void unified_bed_leveling::g29_what_command() {
    report_state();

    if (storage_slot == -1)
      SERIAL_PROTOCOLPGM("No Mesh Loaded.");
    else {
      SERIAL_PROTOCOLPAIR("Mesh ", storage_slot);
      SERIAL_PROTOCOLPGM(" Loaded.");
    }
    SERIAL_EOL();
    safe_delay(50);

    SERIAL_PROTOCOLLNPAIR("UBL object count: ", (int)ubl_cnt);

    #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
      SERIAL_PROTOCOLPGM("planner.z_fade_height : ");
      SERIAL_PROTOCOL_F(planner.z_fade_height, 4);
      SERIAL_EOL();
    #endif

    adjust_mesh_to_mean(g29_c_flag, g29_constant);

    #if HAS_BED_PROBE
      SERIAL_PROTOCOLPGM("zprobe_zoffset: ");
      SERIAL_PROTOCOL_F(zprobe_zoffset, 7);
      SERIAL_EOL();
    #endif

    SERIAL_ECHOLNPAIR("MESH_MIN_X  " STRINGIFY(MESH_MIN_X) "=", MESH_MIN_X);
    safe_delay(50);
    SERIAL_ECHOLNPAIR("MESH_MIN_Y  " STRINGIFY(MESH_MIN_Y) "=", MESH_MIN_Y);
    safe_delay(50);
    SERIAL_ECHOLNPAIR("MESH_MAX_X  " STRINGIFY(MESH_MAX_X) "=", MESH_MAX_X);
    safe_delay(50);
    SERIAL_ECHOLNPAIR("MESH_MAX_Y  " STRINGIFY(MESH_MAX_Y) "=", MESH_MAX_Y);
    safe_delay(50);
    SERIAL_ECHOLNPAIR("GRID_MAX_POINTS_X  ", GRID_MAX_POINTS_X);
    safe_delay(50);
    SERIAL_ECHOLNPAIR("GRID_MAX_POINTS_Y  ", GRID_MAX_POINTS_Y);
    safe_delay(50);
    SERIAL_ECHOLNPAIR("MESH_X_DIST  ", MESH_X_DIST);
    SERIAL_ECHOLNPAIR("MESH_Y_DIST  ", MESH_Y_DIST);
    safe_delay(50);

    SERIAL_PROTOCOLPGM("X-Axis Mesh Points at: ");
    for (uint8_t i = 0; i < GRID_MAX_POINTS_X; i++) {
      SERIAL_PROTOCOL_F(LOGICAL_X_POSITION(mesh_index_to_xpos(i)), 3);
      SERIAL_PROTOCOLPGM("  ");
      safe_delay(25);
    }
    SERIAL_EOL();

    SERIAL_PROTOCOLPGM("Y-Axis Mesh Points at: ");
    for (uint8_t i = 0; i < GRID_MAX_POINTS_Y; i++) {
      SERIAL_PROTOCOL_F(LOGICAL_Y_POSITION(mesh_index_to_ypos(i)), 3);
      SERIAL_PROTOCOLPGM("  ");
      safe_delay(25);
    }
    SERIAL_EOL();

    #if HAS_KILL
      SERIAL_PROTOCOLPAIR("Kill pin on :", KILL_PIN);
      SERIAL_PROTOCOLLNPAIR("  state:", READ(KILL_PIN));
    #endif
    SERIAL_EOL();
    safe_delay(50);

    #ifdef UBL_DEVEL_DEBUGGING
      SERIAL_PROTOCOLLNPAIR("ubl_state_at_invocation :", ubl_state_at_invocation);
      SERIAL_EOL();
      SERIAL_PROTOCOLLNPAIR("ubl_state_recursion_chk :", ubl_state_recursion_chk);
      SERIAL_EOL();
      safe_delay(50);

      SERIAL_PROTOCOLPAIR("Meshes go from ", hex_address((void*)settings.meshes_start_index()));
      SERIAL_PROTOCOLLNPAIR(" to ", hex_address((void*)settings.meshes_end_index()));
      safe_delay(50);

      SERIAL_PROTOCOLLNPAIR("sizeof(ubl) :  ", (int)sizeof(ubl));
      SERIAL_EOL();
      SERIAL_PROTOCOLLNPAIR("z_value[][] size: ", (int)sizeof(z_values));
      SERIAL_EOL();
      safe_delay(25);

      SERIAL_PROTOCOLLNPAIR("EEPROM free for UBL: ", hex_address((void*)(settings.meshes_end_index() - settings.meshes_start_index())));
      safe_delay(50);

      SERIAL_PROTOCOLPAIR("EEPROM can hold ", settings.calc_num_meshes());
      SERIAL_PROTOCOLLNPGM(" meshes.\n");
      safe_delay(25);
    #endif // UBL_DEVEL_DEBUGGING

    if (!sanity_check()) {
      echo_name();
      SERIAL_PROTOCOLLNPGM(" sanity checks passed.");
    }
  }

  /**
   * When we are fully debugged, the EEPROM dump command will get deleted also. But
   * right now, it is good to have the extra information. Soon... we prune this.
   */
  void unified_bed_leveling::g29_eeprom_dump() {
    unsigned char cccc;
    unsigned int  kkkk;  // Needs to be of unspecfied size to compile clean on all platforms

    SERIAL_ECHO_START();
    SERIAL_ECHOLNPGM("EEPROM Dump:");
    for (uint16_t i = 0; i <= E2END; i += 16) {
      if (!(i & 0x3)) idle();
      print_hex_word(i);
      SERIAL_ECHOPGM(": ");
      for (uint16_t j = 0; j < 16; j++) {
        kkkk = i + j;
        eeprom_read_block(&cccc, (const void *)kkkk, sizeof(unsigned char));
        print_hex_byte(cccc);
        SERIAL_ECHO(' ');
      }
      SERIAL_EOL();
    }
    SERIAL_EOL();
  }

  /**
   * When we are fully debugged, this may go away. But there are some valid
   * use cases for the users. So we can wait and see what to do with it.
   */
  void unified_bed_leveling::g29_compare_current_mesh_to_stored_mesh() {
    int16_t a = settings.calc_num_meshes();

    if (!a) {
      SERIAL_PROTOCOLLNPGM("?EEPROM storage not available.");
      return;
    }

    if (!parser.has_value()) {
      SERIAL_PROTOCOLLNPGM("?Storage slot # required.");
      SERIAL_PROTOCOLLNPAIR("?Use 0 to ", a - 1);
      return;
    }

    g29_storage_slot = parser.value_int();

    if (!WITHIN(g29_storage_slot, 0, a - 1)) {
      SERIAL_PROTOCOLLNPGM("?Invalid storage slot.");
      SERIAL_PROTOCOLLNPAIR("?Use 0 to ", a - 1);
      return;
    }

    float tmp_z_values[GRID_MAX_POINTS_X][GRID_MAX_POINTS_Y];
    settings.load_mesh(g29_storage_slot, &tmp_z_values);

    SERIAL_PROTOCOLPAIR("Subtracting mesh in slot ", g29_storage_slot);
    SERIAL_PROTOCOLLNPGM(" from current mesh.");

    for (uint8_t x = 0; x < GRID_MAX_POINTS_X; x++)
      for (uint8_t y = 0; y < GRID_MAX_POINTS_Y; y++)
        z_values[x][y] -= tmp_z_values[x][y];
  }

  mesh_index_pair unified_bed_leveling::find_furthest_invalid_mesh_point() {

    bool found_a_NAN  = false, found_a_real = false;

    mesh_index_pair out_mesh;
    out_mesh.x_index = out_mesh.y_index = -1;
    out_mesh.distance = -99999.99f;

    for (int8_t i = 0; i < GRID_MAX_POINTS_X; i++) {
      for (int8_t j = 0; j < GRID_MAX_POINTS_Y; j++) {

        if (isnan(z_values[i][j])) { // Check to see if this location holds an invalid mesh point

          const float mx = mesh_index_to_xpos(i),
                      my = mesh_index_to_ypos(j);

          if (!position_is_reachable_by_probe(mx, my))  // make sure the probe can get to the mesh point
            continue;

          found_a_NAN = true;

          int8_t closest_x = -1, closest_y = -1;
          float d1, d2 = 99999.9f;
          for (int8_t k = 0; k < GRID_MAX_POINTS_X; k++) {
            for (int8_t l = 0; l < GRID_MAX_POINTS_Y; l++) {
              if (!isnan(z_values[k][l])) {
                found_a_real = true;

                // Add in a random weighting factor that scrambles the probing of the
                // last half of the mesh (when every unprobed mesh point is one index
                // from a probed location).

                d1 = HYPOT(i - k, j - l) + (1.0f / ((millis() % 47) + 13));

                if (d1 < d2) {    // found a closer distance from invalid mesh point at (i,j) to defined mesh point at (k,l)
                  d2 = d1;        // found a closer location with
                  closest_x = i;  // an assigned mesh point value
                  closest_y = j;
                }
              }
            }
          }

          //
          // At this point d2 should have the closest defined mesh point to invalid mesh point (i,j)
          //

          if (found_a_real && (closest_x >= 0) && (d2 > out_mesh.distance)) {
            out_mesh.distance = d2;         // found an invalid location with a greater distance
            out_mesh.x_index = closest_x;   // to a defined mesh point
            out_mesh.y_index = closest_y;
          }
        }
      } // for j
    } // for i

    if (!found_a_real && found_a_NAN) {        // if the mesh is totally unpopulated, start the probing
      out_mesh.x_index = GRID_MAX_POINTS_X / 2;
      out_mesh.y_index = GRID_MAX_POINTS_Y / 2;
      out_mesh.distance = 1;
    }
    return out_mesh;
  }

  mesh_index_pair unified_bed_leveling::find_closest_mesh_point_of_type(const MeshPointType type, const float &rx, const float &ry, const bool probe_as_reference, uint16_t bits[16]) {
    mesh_index_pair out_mesh;
    out_mesh.x_index = out_mesh.y_index = -1;
    out_mesh.distance = -99999.9f;

    // Get our reference position. Either the nozzle or probe location.
    const float px = rx + (probe_as_reference == USE_PROBE_AS_REFERENCE ? X_PROBE_OFFSET_FROM_EXTRUDER : 0),
                py = ry + (probe_as_reference == USE_PROBE_AS_REFERENCE ? Y_PROBE_OFFSET_FROM_EXTRUDER : 0);

    float best_so_far = 99999.99f;

    for (int8_t i = 0; i < GRID_MAX_POINTS_X; i++) {
      for (int8_t j = 0; j < GRID_MAX_POINTS_Y; j++) {

        if ( (type == INVALID && isnan(z_values[i][j]))  // Check to see if this location holds the right thing
          || (type == REAL && !isnan(z_values[i][j]))
          || (type == SET_IN_BITMAP && is_bitmap_set(bits, i, j))
        ) {
          // We only get here if we found a Mesh Point of the specified type

          const float mx = mesh_index_to_xpos(i),
                      my = mesh_index_to_ypos(j);

          // If using the probe as the reference there are some unreachable locations.
          // Also for round beds, there are grid points outside the bed the nozzle can't reach.
          // Prune them from the list and ignore them till the next Phase (manual nozzle probing).

          if (probe_as_reference ? !position_is_reachable_by_probe(mx, my) : !position_is_reachable(mx, my))
            continue;

          // Reachable. Check if it's the best_so_far location to the nozzle.

          float distance = HYPOT(px - mx, py - my);

          // factor in the distance from the current location for the normal case
          // so the nozzle isn't running all over the bed.
          distance += HYPOT(current_position[X_AXIS] - mx, current_position[Y_AXIS] - my) * 0.1f;
          if (distance < best_so_far) {
            best_so_far = distance;   // We found a closer location with
            out_mesh.x_index = i;     // the specified type of mesh value.
            out_mesh.y_index = j;
            out_mesh.distance = best_so_far;
          }
        }
      } // for j
    } // for i

    return out_mesh;
  }

  #if ENABLED(NEWPANEL)

    void abort_fine_tune() {
      lcd_return_to_status();
      do_blocking_move_to_z(Z_CLEARANCE_BETWEEN_PROBES);
      LCD_MESSAGEPGM(MSG_EDITING_STOPPED);
      lcd_quick_feedback(true);
    }

    void unified_bed_leveling::fine_tune_mesh(const float &rx, const float &ry, const bool do_ubl_mesh_map) {
      if (!parser.seen('R'))    // fine_tune_mesh() is special. If no repetition count flag is specified
        g29_repetition_cnt = 1;   // do exactly one mesh location. Otherwise use what the parser decided.

      #if ENABLED(UBL_MESH_EDIT_MOVES_Z)
        const float h_offset = parser.seenval('H') ? parser.value_linear_units() : 0;
        if (!WITHIN(h_offset, 0, 10)) {
          SERIAL_PROTOCOLLNPGM("Offset out of bounds. (0 to 10mm)\n");
          return;
        }
      #endif

      mesh_index_pair location;

      if (!position_is_reachable(rx, ry)) {
        SERIAL_PROTOCOLLNPGM("(X,Y) outside printable radius.");
        return;
      }

      save_ubl_active_state_and_disable();

      LCD_MESSAGEPGM(MSG_UBL_FINE_TUNE_MESH);
      lcd_external_control = true;                                  // Take over control of the LCD encoder

      do_blocking_move_to(rx, ry, Z_CLEARANCE_BETWEEN_PROBES);      // Move to the given XY with probe clearance

      #if ENABLED(UBL_MESH_EDIT_MOVES_Z)
        do_blocking_move_to_z(h_offset);                            // Move Z to the given 'H' offset
      #endif

      uint16_t not_done[16];
      memset(not_done, 0xFF, sizeof(not_done));
      do {
        location = find_closest_mesh_point_of_type(SET_IN_BITMAP, rx, ry, USE_NOZZLE_AS_REFERENCE, not_done);

        if (location.x_index < 0) break;                            // Stop when there are no more reachable points

        bitmap_clear(not_done, location.x_index, location.y_index); // Mark this location as 'adjusted' so a new
                                                                    // location is used on the next loop

        const float rawx = mesh_index_to_xpos(location.x_index),
                    rawy = mesh_index_to_ypos(location.y_index);

        if (!position_is_reachable(rawx, rawy)) break;              // SHOULD NOT OCCUR because find_closest_mesh_point_of_type will only return reachable

        do_blocking_move_to(rawx, rawy, Z_CLEARANCE_BETWEEN_PROBES); // Move the nozzle to the edit point with probe clearance

        #if ENABLED(UBL_MESH_EDIT_MOVES_Z)
          do_blocking_move_to_z(h_offset);                          // Move Z to the given 'H' offset before editing
        #endif

        KEEPALIVE_STATE(PAUSED_FOR_USER);

        if (do_ubl_mesh_map) display_map(g29_map_type);             // Display the current point

        lcd_refresh();

        float new_z = z_values[location.x_index][location.y_index];
        if (isnan(new_z)) new_z = 0;                                // Invalid points begin at 0
        new_z = FLOOR(new_z * 1000) * 0.001f;                       // Chop off digits after the 1000ths place

        lcd_mesh_edit_setup(new_z);

        do {
          new_z = lcd_mesh_edit();
          #if ENABLED(UBL_MESH_EDIT_MOVES_Z)
            do_blocking_move_to_z(h_offset + new_z);                // Move the nozzle as the point is edited
          #endif
          idle();
          SERIAL_FLUSH();                                           // Prevent host M105 buffer overrun.
        } while (!is_lcd_clicked());

        if (!lcd_map_control) lcd_return_to_status();               // Just editing a single point? Return to status

        if (click_and_hold(abort_fine_tune)) goto FINE_TUNE_EXIT;   // If the click is held down, abort editing

        z_values[location.x_index][location.y_index] = new_z;       // Save the updated Z value

        safe_delay(20);                                             // No switch noise
        lcd_refresh();

      } while (location.x_index >= 0 && --g29_repetition_cnt > 0);

      FINE_TUNE_EXIT:

      lcd_external_control = false;
      KEEPALIVE_STATE(IN_HANDLER);

      if (do_ubl_mesh_map) display_map(g29_map_type);
      restore_ubl_active_state_and_leave();

      do_blocking_move_to(rx, ry, Z_CLEARANCE_BETWEEN_PROBES);

      LCD_MESSAGEPGM(MSG_UBL_DONE_EDITING_MESH);
      SERIAL_ECHOLNPGM("Done Editing Mesh");

      if (lcd_map_control)
        lcd_goto_screen(_lcd_ubl_output_map_lcd);
      else
        lcd_return_to_status();
    }

  #endif // NEWPANEL

  /**
   * 'Smart Fill': Scan from the outward edges of the mesh towards the center.
   * If an invalid location is found, use the next two points (if valid) to
   * calculate a 'reasonable' value for the unprobed mesh point.
   */

  bool unified_bed_leveling::smart_fill_one(const uint8_t x, const uint8_t y, const int8_t xdir, const int8_t ydir) {
    const int8_t x1 = x + xdir, x2 = x1 + xdir,
                 y1 = y + ydir, y2 = y1 + ydir;
    // A NAN next to a pair of real values?
    if (isnan(z_values[x][y]) && !isnan(z_values[x1][y1]) && !isnan(z_values[x2][y2])) {
      if (z_values[x1][y1] < z_values[x2][y2])                  // Angled downward?
        z_values[x][y] = z_values[x1][y1];                      // Use nearest (maybe a little too high.)
      else
        z_values[x][y] = 2.0f * z_values[x1][y1] - z_values[x2][y2];   // Angled upward...
      return true;
    }
    return false;
  }

  typedef struct { uint8_t sx, ex, sy, ey; bool yfirst; } smart_fill_info;

  void unified_bed_leveling::smart_fill_mesh() {
    static const smart_fill_info
      info0 PROGMEM = { 0, GRID_MAX_POINTS_X,      0, GRID_MAX_POINTS_Y - 2,  false },  // Bottom of the mesh looking up
      info1 PROGMEM = { 0, GRID_MAX_POINTS_X,      GRID_MAX_POINTS_Y - 1, 0,  false },  // Top of the mesh looking down
      info2 PROGMEM = { 0, GRID_MAX_POINTS_X - 2,  0, GRID_MAX_POINTS_Y,      true  },  // Left side of the mesh looking right
      info3 PROGMEM = { GRID_MAX_POINTS_X - 1, 0,  0, GRID_MAX_POINTS_Y,      true  };  // Right side of the mesh looking left
    static const smart_fill_info * const info[] PROGMEM = { &info0, &info1, &info2, &info3 };

    for (uint8_t i = 0; i < COUNT(info); ++i) {
      const smart_fill_info *f = (smart_fill_info*)pgm_read_ptr(&info[i]);
      const int8_t sx = pgm_read_byte(&f->sx), sy = pgm_read_byte(&f->sy),
                   ex = pgm_read_byte(&f->ex), ey = pgm_read_byte(&f->ey);
      if (pgm_read_byte(&f->yfirst)) {
        const int8_t dir = ex > sx ? 1 : -1;
        for (uint8_t y = sy; y != ey; ++y)
          for (uint8_t x = sx; x != ex; x += dir)
            if (smart_fill_one(x, y, dir, 0)) break;
      }
      else {
        const int8_t dir = ey > sy ? 1 : -1;
         for (uint8_t x = sx; x != ex; ++x)
          for (uint8_t y = sy; y != ey; y += dir)
            if (smart_fill_one(x, y, 0, dir)) break;
      }
    }
  }

  #if HAS_BED_PROBE

    #include "vector_3.h"

    void unified_bed_leveling::tilt_mesh_based_on_probed_grid(const bool do_3_pt_leveling) {
      constexpr int16_t x_min = MAX(MIN_PROBE_X, MESH_MIN_X),
                        x_max = MIN(MAX_PROBE_X, MESH_MAX_X),
                        y_min = MAX(MIN_PROBE_Y, MESH_MIN_Y),
                        y_max = MIN(MAX_PROBE_Y, MESH_MAX_Y);

      bool abort_flag = false;

      float measured_z;

      const float dx = float(x_max - x_min) / (g29_grid_size - 1),
                  dy = float(y_max - y_min) / (g29_grid_size - 1);

      struct linear_fit_data lsf_results;

      //float z1, z2, z3;  // Needed for algorithm validation down below.

      incremental_LSF_reset(&lsf_results);

      if (do_3_pt_leveling) {
        measured_z = probe_pt(PROBE_PT_1_X, PROBE_PT_1_Y, PROBE_PT_RAISE, g29_verbose_level);
        if (isnan(measured_z))
          abort_flag = true;
        else {
          measured_z -= get_z_correction(PROBE_PT_1_X, PROBE_PT_1_Y);
          //z1 = measured_z;
          if (g29_verbose_level > 3) {
            serial_spaces(16);
            SERIAL_ECHOLNPAIR("Corrected_Z=", measured_z);
          }
          incremental_LSF(&lsf_results, PROBE_PT_1_X, PROBE_PT_1_Y, measured_z);
        }

        if (!abort_flag) {
          measured_z = probe_pt(PROBE_PT_2_X, PROBE_PT_2_Y, PROBE_PT_RAISE, g29_verbose_level);
          //z2 = measured_z;
          if (isnan(measured_z))
            abort_flag = true;
          else {
            measured_z -= get_z_correction(PROBE_PT_2_X, PROBE_PT_2_Y);
            if (g29_verbose_level > 3) {
              serial_spaces(16);
              SERIAL_ECHOLNPAIR("Corrected_Z=", measured_z);
            }
            incremental_LSF(&lsf_results, PROBE_PT_2_X, PROBE_PT_2_Y, measured_z);
          }
        }

        if (!abort_flag) {
          measured_z = probe_pt(PROBE_PT_3_X, PROBE_PT_3_Y, PROBE_PT_STOW, g29_verbose_level);
          //z3 = measured_z;
          if (isnan(measured_z))
            abort_flag = true;
          else {
            measured_z -= get_z_correction(PROBE_PT_3_X, PROBE_PT_3_Y);
            if (g29_verbose_level > 3) {
              serial_spaces(16);
              SERIAL_ECHOLNPAIR("Corrected_Z=", measured_z);
            }
            incremental_LSF(&lsf_results, PROBE_PT_3_X, PROBE_PT_3_Y, measured_z);
          }
        }

        STOW_PROBE();
        #ifdef Z_AFTER_PROBING
          move_z_after_probing();
        #endif

        if (abort_flag) {
          SERIAL_ECHOPGM("?Error probing point.  Aborting operation.\n");
          return;
        }
      }
      else { // !do_3_pt_leveling

        bool zig_zag = false;
        for (uint8_t ix = 0; ix < g29_grid_size; ix++) {
          const float rx = float(x_min) + ix * dx;
          for (int8_t iy = 0; iy < g29_grid_size; iy++) {
            const float ry = float(y_min) + dy * (zig_zag ? g29_grid_size - 1 - iy : iy);

            if (!abort_flag) {
              measured_z = probe_pt(rx, ry, parser.seen('E') ? PROBE_PT_STOW : PROBE_PT_RAISE, g29_verbose_level); // TODO: Needs error handling

              abort_flag = isnan(measured_z);

              #if ENABLED(DEBUG_LEVELING_FEATURE)
                if (DEBUGGING(LEVELING)) {
                  SERIAL_CHAR('(');
                  SERIAL_PROTOCOL_F(rx, 7);
                  SERIAL_CHAR(',');
                  SERIAL_PROTOCOL_F(ry, 7);
                  SERIAL_ECHOPGM(")   logical: ");
                  SERIAL_CHAR('(');
                  SERIAL_PROTOCOL_F(LOGICAL_X_POSITION(rx), 7);
                  SERIAL_CHAR(',');
                  SERIAL_PROTOCOL_F(LOGICAL_Y_POSITION(ry), 7);
                  SERIAL_ECHOPGM(")   measured: ");
                  SERIAL_PROTOCOL_F(measured_z, 7);
                  SERIAL_ECHOPGM("   correction: ");
                  SERIAL_PROTOCOL_F(get_z_correction(rx, ry), 7);
                }
              #endif

              measured_z -= get_z_correction(rx, ry) /* + zprobe_zoffset */ ;

              #if ENABLED(DEBUG_LEVELING_FEATURE)
                if (DEBUGGING(LEVELING)) {
                  SERIAL_ECHOPGM("   final >>>---> ");
                  SERIAL_PROTOCOL_F(measured_z, 7);
                  SERIAL_EOL();
                }
              #endif
              if (g29_verbose_level > 3) {
                serial_spaces(16);
                SERIAL_ECHOLNPAIR("Corrected_Z=", measured_z);
              }
              incremental_LSF(&lsf_results, rx, ry, measured_z);
            }
          }

          zig_zag ^= true;
        }
      }
      STOW_PROBE();
      #ifdef Z_AFTER_PROBING
        move_z_after_probing();
      #endif

      if (abort_flag || finish_incremental_LSF(&lsf_results)) {
        SERIAL_ECHOPGM("Could not complete LSF!");
        return;
      }

      vector_3 normal = vector_3(lsf_results.A, lsf_results.B, 1).get_normal();

      if (g29_verbose_level > 2) {
        SERIAL_ECHOPGM("bed plane normal = [");
        SERIAL_PROTOCOL_F(normal.x, 7);
        SERIAL_PROTOCOLCHAR(',');
        SERIAL_PROTOCOL_F(normal.y, 7);
        SERIAL_PROTOCOLCHAR(',');
        SERIAL_PROTOCOL_F(normal.z, 7);
        SERIAL_ECHOLNPGM("]");
      }

      matrix_3x3 rotation = matrix_3x3::create_look_at(vector_3(lsf_results.A, lsf_results.B, 1));

      for (uint8_t i = 0; i < GRID_MAX_POINTS_X; i++) {
        for (uint8_t j = 0; j < GRID_MAX_POINTS_Y; j++) {
          float x_tmp = mesh_index_to_xpos(i),
                y_tmp = mesh_index_to_ypos(j),
                z_tmp = z_values[i][j];

          #if ENABLED(DEBUG_LEVELING_FEATURE)
            if (DEBUGGING(LEVELING)) {
              SERIAL_ECHOPGM("before rotation = [");
              SERIAL_PROTOCOL_F(x_tmp, 7);
              SERIAL_PROTOCOLCHAR(',');
              SERIAL_PROTOCOL_F(y_tmp, 7);
              SERIAL_PROTOCOLCHAR(',');
              SERIAL_PROTOCOL_F(z_tmp, 7);
              SERIAL_ECHOPGM("]   ---> ");
              safe_delay(20);
            }
          #endif

          apply_rotation_xyz(rotation, x_tmp, y_tmp, z_tmp);

          #if ENABLED(DEBUG_LEVELING_FEATURE)
            if (DEBUGGING(LEVELING)) {
              SERIAL_ECHOPGM("after rotation = [");
              SERIAL_PROTOCOL_F(x_tmp, 7);
              SERIAL_PROTOCOLCHAR(',');
              SERIAL_PROTOCOL_F(y_tmp, 7);
              SERIAL_PROTOCOLCHAR(',');
              SERIAL_PROTOCOL_F(z_tmp, 7);
              SERIAL_ECHOLNPGM("]");
              safe_delay(55);
            }
          #endif

          z_values[i][j] = z_tmp - lsf_results.D;
        }
      }

      #if ENABLED(DEBUG_LEVELING_FEATURE)
        if (DEBUGGING(LEVELING)) {
          rotation.debug(PSTR("rotation matrix:\n"));
          SERIAL_ECHOPGM("LSF Results A=");
          SERIAL_PROTOCOL_F(lsf_results.A, 7);
          SERIAL_ECHOPGM("  B=");
          SERIAL_PROTOCOL_F(lsf_results.B, 7);
          SERIAL_ECHOPGM("  D=");
          SERIAL_PROTOCOL_F(lsf_results.D, 7);
          SERIAL_EOL();
          safe_delay(55);

          SERIAL_ECHOPGM("bed plane normal = [");
          SERIAL_PROTOCOL_F(normal.x, 7);
          SERIAL_PROTOCOLCHAR(',');
          SERIAL_PROTOCOL_F(normal.y, 7);
          SERIAL_PROTOCOLCHAR(',');
          SERIAL_PROTOCOL_F(normal.z, 7);
          SERIAL_ECHOPGM("]\n");
          SERIAL_EOL();

          /**
           * The following code can be used to check the validity of the mesh tilting algorithm.
           * When a 3-Point Mesh Tilt is done, the same algorithm is used as the grid based tilting.
           * The only difference is just 3 points are used in the calculations.   That fact guarantees
           * each probed point should have an exact match when a get_z_correction() for that location
           * is calculated.  The Z error between the probed point locations and the get_z_correction()
           * numbers for those locations should be 0.
           */
          #if 0
          float t, t1, d;
          t = normal.x * (PROBE_PT_1_X) + normal.y * (PROBE_PT_1_Y);
          d = t + normal.z * z1;
          SERIAL_ECHOPGM("D from 1st point: ");
          SERIAL_ECHO_F(d, 6);
          SERIAL_ECHOPGM("   Z error: ");
          SERIAL_ECHO_F(normal.z*z1-get_z_correction(PROBE_PT_1_X, PROBE_PT_1_Y), 6);
          SERIAL_EOL();

          t = normal.x * (PROBE_PT_2_X) + normal.y * (PROBE_PT_2_Y);
          d = t + normal.z * z2;
          SERIAL_EOL();
          SERIAL_ECHOPGM("D from 2nd point: ");
          SERIAL_ECHO_F(d, 6);
          SERIAL_ECHOPGM("   Z error: ");
          SERIAL_ECHO_F(normal.z*z2-get_z_correction(PROBE_PT_2_X, PROBE_PT_2_Y), 6);
          SERIAL_EOL();

          t = normal.x * (PROBE_PT_3_X) + normal.y * (PROBE_PT_3_Y);
          d = t + normal.z * z3;
          SERIAL_ECHOPGM("D from 3rd point: ");
          SERIAL_ECHO_F(d, 6);
          SERIAL_ECHOPGM("   Z error: ");
          SERIAL_ECHO_F(normal.z*z3-get_z_correction(PROBE_PT_3_X, PROBE_PT_3_Y), 6);
          SERIAL_EOL();

          t = normal.x * (Z_SAFE_HOMING_X_POINT) + normal.y * (Z_SAFE_HOMING_Y_POINT);
          d = t + normal.z * 0;
          SERIAL_ECHOPGM("D from home location with Z=0 : ");
          SERIAL_ECHO_F(d, 6);
          SERIAL_EOL();

          t = normal.x * (Z_SAFE_HOMING_X_POINT) + normal.y * (Z_SAFE_HOMING_Y_POINT);
          d = t + get_z_correction(Z_SAFE_HOMING_X_POINT, Z_SAFE_HOMING_Y_POINT); // normal.z * 0;
          SERIAL_ECHOPGM("D from home location using mesh value for Z: ");
          SERIAL_ECHO_F(d, 6);

          SERIAL_ECHOPAIR("   Z error: (", Z_SAFE_HOMING_X_POINT);
          SERIAL_ECHOPAIR(",", Z_SAFE_HOMING_Y_POINT );
          SERIAL_ECHOPGM(") = ");
          SERIAL_ECHO_F(get_z_correction(Z_SAFE_HOMING_X_POINT, Z_SAFE_HOMING_Y_POINT), 6);
          SERIAL_EOL();
          #endif
        } // DEBUGGING(LEVELING)
      #endif

    }

  #endif // HAS_BED_PROBE

  #if ENABLED(UBL_G29_P31)
    void unified_bed_leveling::smart_fill_wlsf(const float &weight_factor) {

      // For each undefined mesh point, compute a distance-weighted least squares fit
      // from all the originally populated mesh points, weighted toward the point
      // being extrapolated so that nearby points will have greater influence on
      // the point being extrapolated.  Then extrapolate the mesh point from WLSF.

      static_assert(GRID_MAX_POINTS_Y <= 16, "GRID_MAX_POINTS_Y too big");
      uint16_t bitmap[GRID_MAX_POINTS_X] = { 0 };
      struct linear_fit_data lsf_results;

      SERIAL_ECHOPGM("Extrapolating mesh...");

      const float weight_scaled = weight_factor * MAX(MESH_X_DIST, MESH_Y_DIST);

      for (uint8_t jx = 0; jx < GRID_MAX_POINTS_X; jx++)
        for (uint8_t jy = 0; jy < GRID_MAX_POINTS_Y; jy++)
          if (!isnan(z_values[jx][jy]))
            SBI(bitmap[jx], jy);

      for (uint8_t ix = 0; ix < GRID_MAX_POINTS_X; ix++) {
        const float px = mesh_index_to_xpos(ix);
        for (uint8_t iy = 0; iy < GRID_MAX_POINTS_Y; iy++) {
          const float py = mesh_index_to_ypos(iy);
          if (isnan(z_values[ix][iy])) {
            // undefined mesh point at (px,py), compute weighted LSF from original valid mesh points.
            incremental_LSF_reset(&lsf_results);
            for (uint8_t jx = 0; jx < GRID_MAX_POINTS_X; jx++) {
              const float rx = mesh_index_to_xpos(jx);
              for (uint8_t jy = 0; jy < GRID_MAX_POINTS_Y; jy++) {
                if (TEST(bitmap[jx], jy)) {
                  const float ry = mesh_index_to_ypos(jy),
                              rz = z_values[jx][jy],
                              w  = 1 + weight_scaled / HYPOT((rx - px), (ry - py));
                  incremental_WLSF(&lsf_results, rx, ry, rz, w);
                }
              }
            }
            if (finish_incremental_LSF(&lsf_results)) {
              SERIAL_ECHOLNPGM("Insufficient data");
              return;
            }
            const float ez = -lsf_results.D - lsf_results.A * px - lsf_results.B * py;
            z_values[ix][iy] = ez;
            idle();   // housekeeping
          }
        }
      }

      SERIAL_ECHOLNPGM("done");
    }
  #endif // UBL_G29_P31

#endif // AUTO_BED_LEVELING_UBL
