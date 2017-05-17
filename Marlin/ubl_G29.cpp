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
  //#include "vector_3.h"
  //#include "qr_solve.h"

  #include "ubl.h"
  #include "Marlin.h"
  #include "hex_print_routines.h"
  #include "configuration_store.h"
  #include "ultralcd.h"
  #include "stepper.h"

  #include <math.h>
  #include "least_squares_fit.h"

  extern float destination[XYZE];
  extern float current_position[XYZE];

  void lcd_return_to_status();
  bool lcd_clicked();
  void lcd_implementation_clear();
  void lcd_mesh_edit_setup(float initial);
  float lcd_mesh_edit();
  void lcd_z_offset_edit_setup(float);
  float lcd_z_offset_edit();
  extern float meshedit_done;
  extern long babysteps_done;
  extern float code_value_float();
  extern uint8_t code_value_byte();
  extern bool code_value_bool();
  extern bool code_has_value();
  extern float probe_pt(float x, float y, bool, int);
  extern bool set_probe_deployed(bool);
  void smart_fill_mesh();
  float measure_business_card_thickness(float &in_height);
  void manually_probe_remaining_mesh(const float&, const float&, const float&, const float&, const bool);

  bool ProbeStay = true;

  #define SIZE_OF_LITTLE_RAISE 1
  #define BIG_RAISE_NOT_NEEDED 0
  extern void lcd_status_screen();
  typedef void (*screenFunc_t)();
  extern void lcd_goto_screen(screenFunc_t screen, const uint32_t encoder = 0);

  /**
   *   G29: Unified Bed Leveling by Roxy
   *
   *   Parameters understood by this leveling system:
   *
   *   A     Activate   Activate the Unified Bed Leveling system.
   *
   *   B #   Business   Use the 'Business Card' mode of the Manual Probe subsystem. This is invoked as
   *                    G29 P2 B. The mode of G29 P2 allows you to use a business card or recipe card
   *                    as a shim that the nozzle will pinch as it is lowered. The idea is that you
   *                    can easily feel the nozzle getting to the same height by the amount of resistance
   *                    the business card exhibits to movement. You should try to achieve the same amount
   *                    of resistance on each probed point to facilitate accurate and repeatable measurements.
   *                    You should be very careful not to drive the nozzle into the business card with a
   *                    lot of force as it is very possible to cause damage to your printer if your are
   *                    careless. If you use the B option with G29 P2 B you can omit the numeric value
   *                    on first use to measure the business card's thickness. Subsequent usage of 'B'
   *                    will apply the previously-measured thickness as the default.
   *                    Note: A non-compressible Spark Gap feeler gauge is recommended over a Business Card.
   *
   *   C     Continue   Continue, Constant, Current Location. This is not a primary command. C is used to
   *                    further refine the behaviour of several other commands. Issuing a G29 P1 C will
   *                    continue the generation of a partially constructed Mesh without invalidating what has
   *                    been done. Issuing a G29 P2 C will tell the Manual Probe subsystem to use the current
   *                    location in its search for the closest unmeasured Mesh Point. When used with a G29 Z C
   *                    it indicates to use the current location instead of defaulting to the center of the print bed.
   *
   *   D     Disable    Disable the Unified Bed Leveling system.
   *
   *   E     Stow_probe Stow the probe after each sampled point.
   *
   *   F #   Fade       Fade the amount of Mesh Based Compensation over a specified height. At the
   *                    specified height, no correction is applied and natural printer kenimatics take over. If no
   *                    number is specified for the command, 10mm is assumed to be reasonable.
   *
   *   H #   Height     Specify the Height to raise the nozzle after each manual probe of the bed. The
   *                    default is 5mm.
   *
   *   I #   Invalidate Invalidate specified number of Mesh Points. The nozzle location is used unless
   *                    the X and Y parameter are used. If no number is specified, only the closest Mesh
   *                    point to the location is invalidated. The 'T' parameter is also available to produce
   *                    a map after the operation. This command is useful to invalidate a portion of the
   *                    Mesh so it can be adjusted using other tools in the Unified Bed Leveling System. When
   *                    attempting to invalidate an isolated bad point in the mesh, the 'T' option will indicate
   *                    where the nozzle is positioned in the Mesh with (#). You can move the nozzle around on
   *                    the bed and use this feature to select the center of the area (or cell) you want to
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
   *                    the Z-Probe. Usually the probe can not reach all areas that the nozzle can reach.
   *                    In Cartesian printers, mesh points within the X_OFFSET_FROM_EXTRUDER and Y_OFFSET_FROM_EXTRUDER
   *                    area can not be automatically probed.  For Delta printers the area in which DELTA_PROBEABLE_RADIUS
   *                    and DELTA_PRINTABLE_RADIUS do not overlap will not be automatically probed.
   *
   *                    These points will be handled in Phase 2 and Phase 3. If the Phase 1 command is given the
   *                    C (Continue) parameter it does not invalidate the Mesh prior to automatically
   *                    probing needed locations. This allows you to invalidate portions of the Mesh but still
   *                    use the automatic probing capabilities of the Unified Bed Leveling System. An X and Y
   *                    parameter can be given to prioritize where the command should be trying to measure points.
   *                    If the X and Y parameters are not specified the current probe position is used.
   *                    P1 accepts a 'T' (Topology) parameter so you can observe mesh generation.
   *                    P1 also watches for the LCD Panel Encoder Switch to be held down, and will suspend
   *                    generation of the Mesh in that case. (Note: This check is only done between probe points,
   *                    so you must press and hold the switch until the Phase 1 command detects it.)
   *
   *   P2    Phase 2    Probe areas of the Mesh that can't be automatically handled. Phase 2 respects an H
   *                    parameter to control the height between Mesh points. The default height for movement
   *                    between Mesh points is 5mm. A smaller number can be used to make this part of the
   *                    calibration less time consuming. You will be running the nozzle down until it just barely
   *                    touches the glass. You should have the nozzle clean with no plastic obstructing your view.
   *                    Use caution and move slowly. It is possible to damage your printer if you are careless.
   *                    Note that this command will use the configuration #define SIZE_OF_LITTLE_RAISE if the
   *                    nozzle is moving a distance of less than BIG_RAISE_NOT_NEEDED.
   *
   *                    The H parameter can be set negative if your Mesh dips in a large area. You can press
   *                    and hold the LCD Panel's encoder wheel to terminate the current Phase 2 command. You
   *                    can then re-issue the G29 P 2 command with an H parameter that is more suitable for the
   *                    area you are manually probing. Note that the command tries to start you in a corner
   *                    of the bed where movement will be predictable. You can force the location to be used in
   *                    the distance calculations by using the X and Y parameters. You may find it is helpful to
   *                    print out a Mesh Map (G29 T) to understand where the mesh is invalidated and where
   *                    the nozzle will need to move in order to complete the command. The C parameter is
   *                    available on the Phase 2 command also and indicates the search for points to measure should
   *                    be done based on the current location of the nozzle.
   *
   *                    A B parameter is also available for this command and described up above. It places the
   *                    manual probe subsystem into Business Card mode where the thickness of a business card is
   *                    measured and then used to accurately set the nozzle height in all manual probing for the
   *                    duration of the command. (S for Shim mode would be a better parameter name, but S is needed
   *                    for Save or Store of the Mesh to EEPROM)  A Business card can be used, but you will have
   *                    better results if you use a flexible Shim that does not compress very much. That makes it
   *                    easier for you to get the nozzle to press with similar amounts of force against the shim so you
   *                    can get accurate measurements. As you are starting to touch the nozzle against the shim try
   *                    to get it to grasp the shim with the same force as when you measured the thickness of the
   *                    shim at the start of the command.
   *
   *                    Phase 2 allows the T (Map) parameter to be specified. This helps the user see the progression
   *                    of the Mesh being built.
   *
   *   P3    Phase 3    Fill the unpopulated regions of the Mesh with a fixed value. There are two different paths the
   *                    user can go down.  If the user specifies the value using the C parameter, the closest invalid
   *                    mesh points to the nozzle will be filled.   The user can specify a repeat count using the R
   *                    parameter with the C version of the command.
   *
   *                    A second version of the fill command is available if no C constant is specified.  Not
   *                    specifying a C constant will invoke the 'Smart Fill' algorithm.  The G29 P3 command will search
   *                    from the edges of the mesh inward looking for invalid mesh points.  It will look at the next
   *                    several mesh points to determine if the print bed is sloped up or down.  If the bed is sloped
   *                    upward from the invalid mesh point, it will be replaced with the value of the nearest mesh point.
   *                    If the bed is sloped downward from the invalid mesh point, it will be replaced with a value that
   *                    puts all three points in a line.   The second version of the G29 P3 command is a quick, easy and
   *                    usually safe way to populate the unprobed regions of your mesh so you can continue to the G26
   *                    Mesh Validation Pattern phase.   Please note that you are populating your mesh with unverified
   *                    numbers.  You should use some scrutiny and caution.
   *
   *   P4    Phase 4    Fine tune the Mesh. The Delta Mesh Compensation System assume the existence of
   *                    an LCD Panel. It is possible to fine tune the mesh without the use of an LCD Panel.
   *                    (More work and details on doing this later!)
   *                    The System will search for the closest Mesh Point to the nozzle. It will move the
   *                    nozzle to this location. The user can use the LCD Panel to carefully adjust the nozzle
   *                    so it is just barely touching the bed. When the user clicks the control, the System
   *                    will lock in that height for that point in the Mesh Compensation System.
   *
   *                    Phase 4 has several additional parameters that the user may find helpful. Phase 4
   *                    can be started at a specific location by specifying an X and Y parameter. Phase 4
   *                    can be requested to continue the adjustment of Mesh Points by using the R(epeat)
   *                    parameter. If the Repetition count is not specified, it is assumed the user wishes
   *                    to adjust the entire matrix. The nozzle is moved to the Mesh Point being edited.
   *                    The command can be terminated early (or after the area of interest has been edited) by
   *                    pressing and holding the encoder wheel until the system recognizes the exit request.
   *                    Phase 4's general form is G29 P4 [R # of points] [X position] [Y position]
   *
   *                    Phase 4 is intended to be used with the G26 Mesh Validation Command. Using the
   *                    information left on the printer's bed from the G26 command it is very straight forward
   *                    and easy to fine tune the Mesh. One concept that is important to remember and that
   *                    will make using the Phase 4 command easy to use is this:  You are editing the Mesh Points.
   *                    If you have too little clearance and not much plastic was extruded in an area, you want to
   *                    LOWER the Mesh Point at the location. If you did not get good adheasion, you want to
   *                    RAISE the Mesh Point at that location.
   *
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
   *   R #   Repeat     Repeat this command the specified number of times.  If no number is specified the
   *                    command will be repeated GRID_MAX_POINTS_X * GRID_MAX_POINTS_Y times.
   *
   *   S     Store      Store the current Mesh in the Activated area of the EEPROM. It will also store the
   *                    current state of the Unified Bed Leveling system in the EEPROM.
   *
   *   S #   Store      Store the current Mesh at the specified location in EEPROM. Activate this location
   *                    for subsequent Load and Store operations. Valid storage slot numbers begin at 0 and
   *                    extend to a limit related to the available EEPROM storage.
   *
   *   S -1  Store      Store the current Mesh as a print out that is suitable to be feed back into the system
   *                    at a later date. The GCode output can be saved and later replayed by the host software
   *                    to reconstruct the current mesh on another machine.
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
   *   kinds of problems. Enabling EEPROM Storage is highly recommended. With EEPROM Storage
   *   of the mesh, you are limited to 3-Point and Grid Leveling. (G29 P0 T and G29 P0 G
   *   respectively.)
   *
   *   When you do a G28 and then a G29 P1 to automatically build your first mesh, you are going to notice
   *   the Unified Bed Leveling probes points further and further away from the starting location. (The
   *   starting location defaults to the center of the bed.)   The original Grid and Mesh leveling used
   *   a Zig Zag pattern. The new pattern is better, especially for people with Delta printers. This
   *   allows you to get the center area of the Mesh populated (and edited) quicker. This allows you to
   *   perform a small print and check out your settings quicker. You do not need to populate the
   *   entire mesh to use it. (You don't want to spend a lot of time generating a mesh only to realize
   *   you don't have the resolution or zprobe_zoffset set correctly. The Mesh generation
   *   gathers points closest to where the nozzle is located unless you specify an (X,Y) coordinate pair.
   *
   *   The Unified Bed Leveling uses a lot of EEPROM storage to hold its data. And it takes some effort
   *   to get this Mesh data correct for a user's printer. We do not want this data destroyed as
   *   new versions of Marlin add or subtract to the items stored in EEPROM. So, for the benefit of
   *   the users, we store the Mesh data at the end of the EEPROM and do not keep it contiguous with the
   *   other data stored in the EEPROM. (For sure the developers are going to complain about this, but
   *   this is going to be helpful to the users!)
   *
   *   The foundation of this Bed Leveling System is built on Epatel's Mesh Bed Leveling code. A big
   *   'Thanks!' to him and the creators of 3-Point and Grid Based leveling. Combining their contributions
   *   we now have the functionality and features of all three systems combined.
   */

  // The simple parameter flags and values are 'static' so parameter parsing can be in a support routine.
  static int g29_verbose_level, phase_value, repetition_cnt,
             storage_slot = 0, map_type, grid_size;
  static bool repeat_flag, c_flag, x_flag, y_flag;
  static float x_pos, y_pos, measured_z, card_thickness = 0.0, ubl_constant = 0.0;

  extern void lcd_setstatus(const char* message, const bool persist);
  extern void lcd_setstatuspgm(const char* message, const uint8_t level);

  void __attribute__((optimize("O0"))) gcode_G29() {

    if (!settings.calc_num_meshes()) {
      SERIAL_PROTOCOLLNPGM("?You need to enable your EEPROM and initialize it");
      SERIAL_PROTOCOLLNPGM("with M502, M500, M501 in that order.\n");
      return;
    }

    // Check for commands that require the printer to be homed.
    if (axis_unhomed_error()) {
      if (code_seen('J'))
        home_all_axes();
      else if (code_seen('P')) {
        if (code_has_value()) {
          const int p_val = code_value_int();
          if (p_val == 1 || p_val == 2 || p_val == 4)
            home_all_axes();
        }
      }
    }

    if (g29_parameter_parsing()) return; // abort if parsing the simple parameters causes a problem,

    // Invalidate Mesh Points. This command is a little bit asymetrical because
    // it directly specifies the repetition count and does not use the 'R' parameter.
    if (code_seen('I')) {
      uint8_t cnt = 0;
      repetition_cnt = code_has_value() ? code_value_int() : 1;
      while (repetition_cnt--) {
        if (cnt > 20) { cnt = 0; idle(); }
        const mesh_index_pair location = find_closest_mesh_point_of_type(REAL, x_pos, y_pos, USE_NOZZLE_AS_REFERENCE, NULL, false);
        if (location.x_index < 0) {
          SERIAL_PROTOCOLLNPGM("Entire Mesh invalidated.\n");
          break;            // No more invalid Mesh Points to populate
        }
        ubl.z_values[location.x_index][location.y_index] = NAN;
        cnt++;
      }
      SERIAL_PROTOCOLLNPGM("Locations invalidated.\n");
    }

    if (code_seen('Q')) {
      const int test_pattern = code_has_value() ? code_value_int() : -99;
      if (!WITHIN(test_pattern, -1, 2)) {
        SERIAL_PROTOCOLLNPGM("Invalid test_pattern value. (0-2)\n");
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
              const float p1 = 0.5 * (GRID_MAX_POINTS_X) - x,
                          p2 = 0.5 * (GRID_MAX_POINTS_Y) - y;
              ubl.z_values[x][y] += 2.0 * HYPOT(p1, p2);
            }
          }
          break;
        case 1:
          for (uint8_t x = 0; x < GRID_MAX_POINTS_X; x++) {  // Create a diagonal line several Mesh cells thick that is raised
            ubl.z_values[x][x] += 9.999;
            ubl.z_values[x][x + (x < GRID_MAX_POINTS_Y - 1) ? 1 : -1] += 9.999; // We want the altered line several mesh points thick
          }
          break;
        case 2:
          // Allow the user to specify the height because 10mm is a little extreme in some cases.
          for (uint8_t x = (GRID_MAX_POINTS_X) / 3; x < 2 * (GRID_MAX_POINTS_X) / 3; x++)   // Create a rectangular raised area in
            for (uint8_t y = (GRID_MAX_POINTS_Y) / 3; y < 2 * (GRID_MAX_POINTS_Y) / 3; y++) // the center of the bed
              ubl.z_values[x][y] += code_seen('C') ? ubl_constant : 9.99;
          break;
      }
    }

    if (code_seen('J')) {
      if (grid_size) {  // if not 0 it is a normal n x n grid being probed
        ubl.save_ubl_active_state_and_disable();
        ubl.tilt_mesh_based_on_probed_grid(code_seen('T'));
        ubl.restore_ubl_active_state_and_leave();
      } else { // grid_size==0 which means a 3-Point leveling has been requested
        float z1 = probe_pt(LOGICAL_X_POSITION(UBL_PROBE_PT_1_X), LOGICAL_Y_POSITION(UBL_PROBE_PT_1_Y), false, g29_verbose_level),
              z2 = probe_pt(LOGICAL_X_POSITION(UBL_PROBE_PT_2_X), LOGICAL_Y_POSITION(UBL_PROBE_PT_2_Y), false, g29_verbose_level),
              z3 = probe_pt(LOGICAL_X_POSITION(UBL_PROBE_PT_3_X), LOGICAL_Y_POSITION(UBL_PROBE_PT_3_Y), true, g29_verbose_level);

        if ( isnan(z1) || isnan(z2) || isnan(z3)) {   // probe_pt will return NAN if unreachable
          SERIAL_ERROR_START;
          SERIAL_ERRORLNPGM("Attempt to probe off the bed.");
          goto LEAVE;
        }

      //  We need to adjust z1, z2, z3 by the Mesh Height at these points. Just because they are non-zero doesn't mean
      //  the Mesh is tilted!  (We need to compensate each probe point by what the Mesh says that location's height is)

        ubl.save_ubl_active_state_and_disable();
        z1 -= ubl.get_z_correction(LOGICAL_X_POSITION(UBL_PROBE_PT_1_X), LOGICAL_Y_POSITION(UBL_PROBE_PT_1_Y)) /* + zprobe_zoffset */ ;
        z2 -= ubl.get_z_correction(LOGICAL_X_POSITION(UBL_PROBE_PT_2_X), LOGICAL_Y_POSITION(UBL_PROBE_PT_2_Y)) /* + zprobe_zoffset */ ;
        z3 -= ubl.get_z_correction(LOGICAL_X_POSITION(UBL_PROBE_PT_3_X), LOGICAL_Y_POSITION(UBL_PROBE_PT_3_Y)) /* + zprobe_zoffset */ ;

        do_blocking_move_to_xy(0.5 * (UBL_MESH_MAX_X - (UBL_MESH_MIN_X)), 0.5 * (UBL_MESH_MAX_Y - (UBL_MESH_MIN_Y)));
        ubl.tilt_mesh_based_on_3pts(z1, z2, z3);
        ubl.restore_ubl_active_state_and_leave();
      }
    }

    if (code_seen('P')) {
      if (WITHIN(phase_value, 0, 1) && ubl.state.storage_slot == -1) {
        ubl.state.storage_slot = 0;
        SERIAL_PROTOCOLLNPGM("Default storage slot 0 selected.");
      }

      switch (phase_value) {
        case 0:
          //
          // Zero Mesh Data
          //
          ubl.reset();
          SERIAL_PROTOCOLLNPGM("Mesh zeroed.");
          break;

        case 1:
          //
          // Invalidate Entire Mesh and Automatically Probe Mesh in areas that can be reached by the probe
          //
          if (!code_seen('C')) {
            ubl.invalidate();
            SERIAL_PROTOCOLLNPGM("Mesh invalidated. Probing mesh.");
          }
          if (g29_verbose_level > 1) {
            SERIAL_PROTOCOLPAIR("Probing Mesh Points Closest to (", x_pos);
            SERIAL_PROTOCOLCHAR(',');
            SERIAL_PROTOCOL(y_pos);
            SERIAL_PROTOCOLLNPGM(").\n");
          }
          ubl.probe_entire_mesh(x_pos + X_PROBE_OFFSET_FROM_EXTRUDER, y_pos + Y_PROBE_OFFSET_FROM_EXTRUDER,
                            code_seen('T'), code_seen('E'), code_seen('U'));
          break;

        case 2: {
          //
          // Manually Probe Mesh in areas that can't be reached by the probe
          //
          SERIAL_PROTOCOLLNPGM("Manually probing unreachable mesh locations.");
          do_blocking_move_to_z(Z_CLEARANCE_BETWEEN_PROBES);
          if (!x_flag && !y_flag) {
            /**
             * Use a good default location for the path.
             * The flipped > and < operators in these comparisons is intentional.
             * It should cause the probed points to follow a nice path on Cartesian printers.
             * It may make sense to have Delta printers default to the center of the bed.
             * Until that is decided, this can be forced with the X and Y parameters.
             */
            #if IS_KINEMATIC
              x_pos = X_HOME_POS;
              y_pos = Y_HOME_POS;
            #else // cartesian
              x_pos = X_PROBE_OFFSET_FROM_EXTRUDER > 0 ? X_MAX_POS : X_MIN_POS;
              y_pos = Y_PROBE_OFFSET_FROM_EXTRUDER < 0 ? Y_MAX_POS : Y_MIN_POS;
            #endif
          }

          if (code_seen('C')) {
            x_pos = current_position[X_AXIS];
            y_pos = current_position[Y_AXIS];
          }

          float height = Z_CLEARANCE_BETWEEN_PROBES;

          if (code_seen('B')) {
            card_thickness = code_has_value() ? code_value_float() : measure_business_card_thickness(height);

            if (fabs(card_thickness) > 1.5) {
              SERIAL_PROTOCOLLNPGM("?Error in Business Card measurement.");
              return;
            }
          }

          if (code_seen('H') && code_has_value()) height = code_value_float();

          if ( !position_is_reachable_xy( x_pos, y_pos )) {
            SERIAL_PROTOCOLLNPGM("(X,Y) outside printable radius.");
            return;
          }

          manually_probe_remaining_mesh(x_pos, y_pos, height, card_thickness, code_seen('T'));
          SERIAL_PROTOCOLLNPGM("G29 P2 finished.");
        } break;

        case 3: {
          /**
           * Populate invalid mesh areas. Proceed with caution.
           * Two choices are available:
           *   - Specify a constant with the 'C' parameter.
           *   - Allow 'G29 P3' to choose a 'reasonable' constant.
           */
          if (c_flag) {
            if (repetition_cnt >= GRID_MAX_POINTS) {
              for (uint8_t x = 0; x < GRID_MAX_POINTS_X; x++) {
                for (uint8_t y = 0; y < GRID_MAX_POINTS_Y; y++) {
                  ubl.z_values[x][y] = ubl_constant;
                }
              }
            }
            else {
              while (repetition_cnt--) {  // this only populates reachable mesh points near
                const mesh_index_pair location = find_closest_mesh_point_of_type(INVALID, x_pos, y_pos, USE_NOZZLE_AS_REFERENCE, NULL, false);
                if (location.x_index < 0) break; // No more reachable invalid Mesh Points to populate
                ubl.z_values[location.x_index][location.y_index] = ubl_constant;
              }
            }
          } else {
            smart_fill_mesh(); // Do a 'Smart' fill using nearby known values
          }
          break;
        }

        case 4:
          //
          // Fine Tune (i.e., Edit) the Mesh
          //
          fine_tune_mesh(x_pos, y_pos, code_seen('T'));
          break;

        case 5: ubl.find_mean_mesh_height(); break;

        case 6: ubl.shift_mesh_height(); break;
      }
    }

    //
    // Much of the 'What?' command can be eliminated. But until we are fully debugged, it is
    // good to have the extra information. Soon... we prune this to just a few items
    //
    if (code_seen('W')) ubl.g29_what_command();

    //
    // When we are fully debugged, this may go away. But there are some valid
    // use cases for the users. So we can wait and see what to do with it.
    //

    if (code_seen('K')) // Kompare Current Mesh Data to Specified Stored Mesh
      g29_compare_current_mesh_to_stored_mesh();

    //
    // Load a Mesh from the EEPROM
    //

    if (code_seen('L')) {     // Load Current Mesh Data
      storage_slot = code_has_value() ? code_value_int() : ubl.state.storage_slot;

      int16_t a = settings.calc_num_meshes();

      if (!a) {
        SERIAL_PROTOCOLLNPGM("?EEPROM storage not available.");
        return;
      }

      if (!WITHIN(storage_slot, 0, a - 1)) {
        SERIAL_PROTOCOLLNPGM("?Invalid storage slot.");
        SERIAL_PROTOCOLLNPAIR("?Use 0 to ", a - 1);
        return;
      }

      settings.load_mesh(storage_slot);
      ubl.state.storage_slot = storage_slot;

      SERIAL_PROTOCOLLNPGM("Done.");
    }

    //
    // Store a Mesh in the EEPROM
    //

    if (code_seen('S')) {     // Store (or Save) Current Mesh Data
      storage_slot = code_has_value() ? code_value_int() : ubl.state.storage_slot;

      if (storage_slot == -1) {                     // Special case, we are going to 'Export' the mesh to the
        SERIAL_ECHOLNPGM("G29 I 999");              // host in a form it can be reconstructed on a different machine
        for (uint8_t x = 0; x < GRID_MAX_POINTS_X; x++)
          for (uint8_t y = 0;  y < GRID_MAX_POINTS_Y; y++)
            if (!isnan(ubl.z_values[x][y])) {
              SERIAL_ECHOPAIR("M421 I ", x);
              SERIAL_ECHOPAIR(" J ", y);
              SERIAL_ECHOPGM(" Z ");
              SERIAL_ECHO_F(ubl.z_values[x][y], 6);
              SERIAL_ECHOPAIR(" ; X ", LOGICAL_X_POSITION(pgm_read_float(&ubl.mesh_index_to_xpos[x])));
              SERIAL_ECHOPAIR(", Y ", LOGICAL_Y_POSITION(pgm_read_float(&ubl.mesh_index_to_ypos[y])));
              SERIAL_EOL;
            }
        return;
      }

      int16_t a = settings.calc_num_meshes();

      if (!a) {
        SERIAL_PROTOCOLLNPGM("?EEPROM storage not available.");
        goto LEAVE;
      }

      if (!WITHIN(storage_slot, 0, a - 1)) {
        SERIAL_PROTOCOLLNPGM("?Invalid storage slot.");
        SERIAL_PROTOCOLLNPAIR("?Use 0 to ", a - 1);
        goto LEAVE;
      }

      settings.store_mesh(storage_slot);
      ubl.state.storage_slot = storage_slot;

      SERIAL_PROTOCOLLNPGM("Done.");
    }

    if (code_seen('T'))
      ubl.display_map(code_has_value() ? code_value_int() : 0);

    /*
     * This code may not be needed...   Prepare for its removal...
     *
    if (code_seen('Z')) {
      if (code_has_value())
        ubl.state.z_offset = code_value_float();   // do the simple case. Just lock in the specified value
      else {
        ubl.save_ubl_active_state_and_disable();
        //measured_z = probe_pt(x_pos + X_PROBE_OFFSET_FROM_EXTRUDER, y_pos + Y_PROBE_OFFSET_FROM_EXTRUDER, ProbeDeployAndStow, g29_verbose_level);

        ubl.has_control_of_lcd_panel = true;     // Grab the LCD Hardware
        measured_z = 1.5;
        do_blocking_move_to_z(measured_z);  // Get close to the bed, but leave some space so we don't damage anything
                                            // The user is not going to be locking in a new Z-Offset very often so
                                            // it won't be that painful to spin the Encoder Wheel for 1.5mm
        lcd_implementation_clear();
        lcd_z_offset_edit_setup(measured_z);

        KEEPALIVE_STATE(PAUSED_FOR_USER);

        do {
          measured_z = lcd_z_offset_edit();
          idle();
          do_blocking_move_to_z(measured_z);
        } while (!ubl_lcd_clicked());

        ubl.has_control_of_lcd_panel = true;   // There is a race condition for the Encoder Wheel getting clicked.
                                               // It could get detected in lcd_mesh_edit (actually _lcd_mesh_fine_tune)
                                               // or here. So, until we are done looking for a long Encoder Wheel Press,
                                               // we need to take control of the panel

        KEEPALIVE_STATE(IN_HANDLER);

        lcd_return_to_status();

        const millis_t nxt = millis() + 1500UL;
        while (ubl_lcd_clicked()) { // debounce and watch for abort
          idle();
          if (ELAPSED(millis(), nxt)) {
            SERIAL_PROTOCOLLNPGM("\nZ-Offset Adjustment Stopped.");
            do_blocking_move_to_z(Z_CLEARANCE_DEPLOY_PROBE);
            LCD_MESSAGEPGM("Z-Offset Stopped"); // TODO: Make translatable string
            ubl.restore_ubl_active_state_and_leave();
            goto LEAVE;
          }
        }
        ubl.has_control_of_lcd_panel = false;
        safe_delay(20); // We don't want any switch noise.

        ubl.state.z_offset = measured_z;

        lcd_implementation_clear();
        ubl.restore_ubl_active_state_and_leave();
      }
    }
    */

    LEAVE:

    lcd_reset_alert_level();
    LCD_MESSAGEPGM("");
    lcd_quick_feedback();

    ubl.has_control_of_lcd_panel = false;
  }

  void unified_bed_leveling::find_mean_mesh_height() {
    float sum = 0.0;
    int n = 0;
    for (uint8_t x = 0; x < GRID_MAX_POINTS_X; x++)
      for (uint8_t y = 0; y < GRID_MAX_POINTS_Y; y++)
        if (!isnan(ubl.z_values[x][y])) {
          sum += ubl.z_values[x][y];
          n++;
        }

    const float mean = sum / n;

    //
    // Now do the sumation of the squares of difference from mean
    //
    float sum_of_diff_squared = 0.0;
    for (uint8_t x = 0; x < GRID_MAX_POINTS_X; x++)
      for (uint8_t y = 0; y < GRID_MAX_POINTS_Y; y++)
        if (!isnan(ubl.z_values[x][y]))
          sum_of_diff_squared += sq(ubl.z_values[x][y] - mean);

    SERIAL_ECHOLNPAIR("# of samples: ", n);
    SERIAL_ECHOPGM("Mean Mesh Height: ");
    SERIAL_ECHO_F(mean, 6);
    SERIAL_EOL;

    const float sigma = sqrt(sum_of_diff_squared / (n + 1));
    SERIAL_ECHOPGM("Standard Deviation: ");
    SERIAL_ECHO_F(sigma, 6);
    SERIAL_EOL;

    if (c_flag)
      for (uint8_t x = 0; x < GRID_MAX_POINTS_X; x++)
        for (uint8_t y = 0; y < GRID_MAX_POINTS_Y; y++)
          if (!isnan(ubl.z_values[x][y]))
            ubl.z_values[x][y] -= mean + ubl_constant;
  }

  void unified_bed_leveling::shift_mesh_height() {
    for (uint8_t x = 0; x < GRID_MAX_POINTS_X; x++)
      for (uint8_t y = 0; y < GRID_MAX_POINTS_Y; y++)
        if (!isnan(ubl.z_values[x][y]))
          ubl.z_values[x][y] += ubl_constant;
  }

  /**
   * Probe all invalidated locations of the mesh that can be reached by the probe.
   * This attempts to fill in locations closest to the nozzle's start location first.
   */
  void unified_bed_leveling::probe_entire_mesh(const float &lx, const float &ly, const bool do_ubl_mesh_map, const bool stow_probe, bool close_or_far) {
    mesh_index_pair location;

    ubl.has_control_of_lcd_panel = true;
    ubl.save_ubl_active_state_and_disable();   // we don't do bed level correction because we want the raw data when we probe
    DEPLOY_PROBE();

    uint16_t max_iterations = GRID_MAX_POINTS;

    do {
      if (ubl_lcd_clicked()) {
        SERIAL_PROTOCOLLNPGM("\nMesh only partially populated.\n");
        lcd_quick_feedback();
        STOW_PROBE();
        while (ubl_lcd_clicked()) idle();
        ubl.has_control_of_lcd_panel = false;
        ubl.restore_ubl_active_state_and_leave();
        safe_delay(50);  // Debounce the Encoder wheel
        return;
      }

      location = find_closest_mesh_point_of_type(INVALID, lx, ly, USE_PROBE_AS_REFERENCE, NULL, close_or_far);

      if (location.x_index >= 0) {    // mesh point found and is reachable by probe
        const float rawx = pgm_read_float(&ubl.mesh_index_to_xpos[location.x_index]),
                    rawy = pgm_read_float(&ubl.mesh_index_to_ypos[location.y_index]);

        const float measured_z = probe_pt(LOGICAL_X_POSITION(rawx), LOGICAL_Y_POSITION(rawy), stow_probe, g29_verbose_level);
        ubl.z_values[location.x_index][location.y_index] = measured_z;
      }

      if (do_ubl_mesh_map) ubl.display_map(map_type);
    } while ((location.x_index >= 0) && (--max_iterations));

    STOW_PROBE();
    ubl.restore_ubl_active_state_and_leave();

    do_blocking_move_to_xy(
      constrain(lx - (X_PROBE_OFFSET_FROM_EXTRUDER), UBL_MESH_MIN_X, UBL_MESH_MAX_X),
      constrain(ly - (Y_PROBE_OFFSET_FROM_EXTRUDER), UBL_MESH_MIN_Y, UBL_MESH_MAX_Y)
    );
  }

  void unified_bed_leveling::tilt_mesh_based_on_3pts(const float &z1, const float &z2, const float &z3) {
    matrix_3x3 rotation;
    vector_3 v1 = vector_3( (UBL_PROBE_PT_1_X - UBL_PROBE_PT_2_X),
                            (UBL_PROBE_PT_1_Y - UBL_PROBE_PT_2_Y),
                            (z1 - z2) ),

             v2 = vector_3( (UBL_PROBE_PT_3_X - UBL_PROBE_PT_2_X),
                            (UBL_PROBE_PT_3_Y - UBL_PROBE_PT_2_Y),
                            (z3 - z2) ),

             normal = vector_3::cross(v1, v2);

    normal = normal.get_normal();

    /**
     * This vector is normal to the tilted plane.
     * However, we don't know its direction. We need it to point up. So if
     * Z is negative, we need to invert the sign of all components of the vector
     */
    if (normal.z < 0.0) {
      normal.x = -normal.x;
      normal.y = -normal.y;
      normal.z = -normal.z;
    }

    rotation = matrix_3x3::create_look_at(vector_3(normal.x, normal.y, 1));

    if (g29_verbose_level > 2) {
      SERIAL_ECHOPGM("bed plane normal = [");
      SERIAL_PROTOCOL_F(normal.x, 7);
      SERIAL_PROTOCOLCHAR(',');
      SERIAL_PROTOCOL_F(normal.y, 7);
      SERIAL_PROTOCOLCHAR(',');
      SERIAL_PROTOCOL_F(normal.z, 7);
      SERIAL_ECHOLNPGM("]");
      rotation.debug(PSTR("rotation matrix:"));
    }

    //
    // All of 3 of these points should give us the same d constant
    //

    float t = normal.x * (UBL_PROBE_PT_1_X) + normal.y * (UBL_PROBE_PT_1_Y),
          d = t + normal.z * z1;

    if (g29_verbose_level>2) {
      SERIAL_ECHOPGM("D constant: ");
      SERIAL_PROTOCOL_F(d, 7);
      SERIAL_ECHOLNPGM(" ");
    }

    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) {
        SERIAL_ECHOPGM("d from 1st point: ");
        SERIAL_ECHO_F(d, 6);
        SERIAL_EOL;
        t = normal.x * (UBL_PROBE_PT_2_X) + normal.y * (UBL_PROBE_PT_2_Y);
        d = t + normal.z * z2;
        SERIAL_ECHOPGM("d from 2nd point: ");
        SERIAL_ECHO_F(d, 6);
        SERIAL_EOL;
        t = normal.x * (UBL_PROBE_PT_3_X) + normal.y * (UBL_PROBE_PT_3_Y);
        d = t + normal.z * z3;
        SERIAL_ECHOPGM("d from 3rd point: ");
        SERIAL_ECHO_F(d, 6);
        SERIAL_EOL;
      }
    #endif

    for (uint8_t i = 0; i < GRID_MAX_POINTS_X; i++) {
      for (uint8_t j = 0; j < GRID_MAX_POINTS_Y; j++) {
        float x_tmp = pgm_read_float(&ubl.mesh_index_to_xpos[i]),
              y_tmp = pgm_read_float(&ubl.mesh_index_to_ypos[j]),
              z_tmp = ubl.z_values[i][j];
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
        ubl.z_values[i][j] += z_tmp - d;
      }
    }
  }

  float use_encoder_wheel_to_measure_point() {

    while (ubl_lcd_clicked()) delay(50);  // wait for user to release encoder wheel
    delay(50);  // debounce

    KEEPALIVE_STATE(PAUSED_FOR_USER);
    while (!ubl_lcd_clicked()) {     // we need the loop to move the nozzle based on the encoder wheel here!
      idle();
      if (ubl.encoder_diff) {
        do_blocking_move_to_z(current_position[Z_AXIS] + 0.01 * float(ubl.encoder_diff));
        ubl.encoder_diff = 0;
      }
    }
    KEEPALIVE_STATE(IN_HANDLER);
    return current_position[Z_AXIS];
  }

  static void echo_and_take_a_measurement() {
    SERIAL_PROTOCOLLNPGM(" and take a measurement.");
  }

  float measure_business_card_thickness(float &in_height) {
    ubl.has_control_of_lcd_panel = true;
    ubl.save_ubl_active_state_and_disable();   // Disable bed level correction for probing

    do_blocking_move_to_z(in_height);
    do_blocking_move_to_xy(0.5 * (UBL_MESH_MAX_X - (UBL_MESH_MIN_X)), 0.5 * (UBL_MESH_MAX_Y - (UBL_MESH_MIN_Y)));
      //, min(planner.max_feedrate_mm_s[X_AXIS], planner.max_feedrate_mm_s[Y_AXIS]) / 2.0);
    stepper.synchronize();

    SERIAL_PROTOCOLPGM("Place shim under nozzle");
    LCD_MESSAGEPGM("Place shim & measure"); // TODO: Make translatable string
    lcd_goto_screen(lcd_status_screen);
    echo_and_take_a_measurement();

    const float z1 = use_encoder_wheel_to_measure_point();
    do_blocking_move_to_z(current_position[Z_AXIS] + SIZE_OF_LITTLE_RAISE);
    stepper.synchronize();

    SERIAL_PROTOCOLPGM("Remove shim");
    LCD_MESSAGEPGM("Remove & measure bed"); // TODO: Make translatable string
    echo_and_take_a_measurement();

    const float z2 = use_encoder_wheel_to_measure_point();

    do_blocking_move_to_z(current_position[Z_AXIS] + Z_CLEARANCE_BETWEEN_PROBES);

    const float thickness = abs(z1 - z2);

    if (g29_verbose_level > 1) {
      SERIAL_PROTOCOLPGM("Business Card is ");
      SERIAL_PROTOCOL_F(thickness, 4);
      SERIAL_PROTOCOLLNPGM("mm thick.");
    }

    in_height = current_position[Z_AXIS]; // do manual probing at lower height

    ubl.has_control_of_lcd_panel = false;

    ubl.restore_ubl_active_state_and_leave();

    return thickness;
  }

  void manually_probe_remaining_mesh(const float &lx, const float &ly, const float &z_clearance, const float &card_thickness, const bool do_ubl_mesh_map) {

    ubl.has_control_of_lcd_panel = true;
    ubl.save_ubl_active_state_and_disable();   // we don't do bed level correction because we want the raw data when we probe
    do_blocking_move_to_z(Z_CLEARANCE_BETWEEN_PROBES);
    do_blocking_move_to_xy(lx, ly);

    lcd_goto_screen(lcd_status_screen);
    mesh_index_pair location;
    do {
      location = find_closest_mesh_point_of_type(INVALID, lx, ly, USE_NOZZLE_AS_REFERENCE, NULL, false);
      // It doesn't matter if the probe can't reach the NAN location. This is a manual probe.
      if (location.x_index < 0 && location.y_index < 0) continue;

      const float rawx = pgm_read_float(&ubl.mesh_index_to_xpos[location.x_index]),
                  rawy = pgm_read_float(&ubl.mesh_index_to_ypos[location.y_index]),
                  xProbe = LOGICAL_X_POSITION(rawx),
                  yProbe = LOGICAL_Y_POSITION(rawy);

      if (!position_is_reachable_raw_xy(rawx, rawy)) break; // SHOULD NOT OCCUR (find_closest_mesh_point only returns reachable points)

      do_blocking_move_to_z(Z_CLEARANCE_BETWEEN_PROBES);

      LCD_MESSAGEPGM("Moving to next"); // TODO: Make translatable string

      do_blocking_move_to_xy(xProbe, yProbe);
      do_blocking_move_to_z(z_clearance);

      KEEPALIVE_STATE(PAUSED_FOR_USER);
      ubl.has_control_of_lcd_panel = true;

      if (do_ubl_mesh_map) ubl.display_map(map_type);  // show user where we're probing

      if (code_seen('B'))
        LCD_MESSAGEPGM("Place shim & measure"); // TODO: Make translatable string
      else
        LCD_MESSAGEPGM("Measure"); // TODO: Make translatable string

      while (ubl_lcd_clicked()) delay(50);             // wait for user to release encoder wheel
      delay(50);                                       // debounce
      while (!ubl_lcd_clicked()) {                     // we need the loop to move the nozzle based on the encoder wheel here!
        idle();
        if (ubl.encoder_diff) {
          do_blocking_move_to_z(current_position[Z_AXIS] + float(ubl.encoder_diff) / 100.0);
          ubl.encoder_diff = 0;
        }
      }

      const millis_t nxt = millis() + 1500L;
      while (ubl_lcd_clicked()) {     // debounce and watch for abort
        idle();
        if (ELAPSED(millis(), nxt)) {
          SERIAL_PROTOCOLLNPGM("\nMesh only partially populated.");
          do_blocking_move_to_z(Z_CLEARANCE_DEPLOY_PROBE);
          lcd_quick_feedback();
          while (ubl_lcd_clicked()) idle();
          ubl.has_control_of_lcd_panel = false;
          KEEPALIVE_STATE(IN_HANDLER);
          ubl.restore_ubl_active_state_and_leave();
          return;
        }
      }

      ubl.z_values[location.x_index][location.y_index] = current_position[Z_AXIS] - card_thickness;
      if (g29_verbose_level > 2) {
        SERIAL_PROTOCOLPGM("Mesh Point Measured at: ");
        SERIAL_PROTOCOL_F(ubl.z_values[location.x_index][location.y_index], 6);
        SERIAL_EOL;
      }
    } while (location.x_index >= 0 && location.y_index >= 0);

    if (do_ubl_mesh_map) ubl.display_map(map_type);

    LEAVE:
    ubl.restore_ubl_active_state_and_leave();
    KEEPALIVE_STATE(IN_HANDLER);
    do_blocking_move_to_z(Z_CLEARANCE_DEPLOY_PROBE);
    do_blocking_move_to_xy(lx, ly);
  }

  bool g29_parameter_parsing() {
    bool err_flag = false;

    LCD_MESSAGEPGM("Doing G29 UBL!"); // TODO: Make translatable string
    lcd_quick_feedback();

    ubl_constant = 0.0;
    repetition_cnt = 0;

    x_flag = code_seen('X') && code_has_value();
    x_pos = x_flag ? code_value_float() : current_position[X_AXIS];
    y_flag = code_seen('Y') && code_has_value();
    y_pos = y_flag ? code_value_float() : current_position[Y_AXIS];

    repeat_flag = code_seen('R');
    if (repeat_flag) {
      repetition_cnt = code_has_value() ? code_value_int() : GRID_MAX_POINTS;
      NOMORE(repetition_cnt, GRID_MAX_POINTS);
      if (repetition_cnt < 1) {
        SERIAL_PROTOCOLLNPGM("?(R)epetition count invalid (1+).\n");
        return UBL_ERR;
      }
    }

    g29_verbose_level = code_seen('V') ? code_value_int() : 0;
    if (!WITHIN(g29_verbose_level, 0, 4)) {
      SERIAL_PROTOCOLLNPGM("?(V)erbose level is implausible (0-4).\n");
      err_flag = true;
    }

    if (code_seen('P')) {
      phase_value = code_value_int();
      if (!WITHIN(phase_value, 0, 6)) {
        SERIAL_PROTOCOLLNPGM("?(P)hase value invalid (0-6).\n");
        err_flag = true;
      }
    }

    if (code_seen('J')) {
      grid_size = code_has_value() ? code_value_int() : 0;
      if (grid_size!=0 && !WITHIN(grid_size, 2, 9)) {
        SERIAL_PROTOCOLLNPGM("?Invalid grid size (J) specified (2-9).\n");
        err_flag = true;
      }
    }

    if (x_flag != y_flag) {
      SERIAL_PROTOCOLLNPGM("Both X & Y locations must be specified.\n");
      err_flag = true;
    }
    if (!WITHIN(RAW_X_POSITION(x_pos), X_MIN_POS, X_MAX_POS)) {
      SERIAL_PROTOCOLLNPGM("Invalid X location specified.\n");
      err_flag = true;
    }

    if (!WITHIN(RAW_Y_POSITION(y_pos), Y_MIN_POS, Y_MAX_POS)) {
      SERIAL_PROTOCOLLNPGM("Invalid Y location specified.\n");
      err_flag = true;
    }

    if (err_flag) return UBL_ERR;

    // Activate or deactivate UBL
    if (code_seen('A')) {
      if (code_seen('D')) {
        SERIAL_PROTOCOLLNPGM("?Can't activate and deactivate at the same time.\n");
        return UBL_ERR;
      }
      ubl.state.active = true;
      ubl.report_state();
    }
    else if (code_seen('D')) {
      ubl.state.active = false;
      ubl.report_state();
    }

    // Set global 'C' flag and its value
    if ((c_flag = code_seen('C')))
      ubl_constant = code_value_float();

    #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
      if (code_seen('F') && code_has_value()) {
        const float fh = code_value_float();
        if (!WITHIN(fh, 0.0, 100.0)) {
          SERIAL_PROTOCOLLNPGM("?(F)ade height for Bed Level Correction not plausible.\n");
          return UBL_ERR;
        }
        set_z_fade_height(fh);
      }
    #endif

    map_type = code_seen('T') && code_has_value() ? code_value_int() : 0;
    if (!WITHIN(map_type, 0, 1)) {
      SERIAL_PROTOCOLLNPGM("Invalid map type.\n");
      return UBL_ERR;
    }
    return UBL_OK;
  }

  static int ubl_state_at_invocation = 0,
             ubl_state_recursion_chk = 0;

  void unified_bed_leveling::save_ubl_active_state_and_disable() {
    ubl_state_recursion_chk++;
    if (ubl_state_recursion_chk != 1) {
      SERIAL_ECHOLNPGM("save_ubl_active_state_and_disabled() called multiple times in a row.");
      LCD_MESSAGEPGM("save_UBL_active() error"); // TODO: Make translatable string
      lcd_quick_feedback();
      return;
    }
    ubl_state_at_invocation = ubl.state.active;
    ubl.state.active = 0;
  }

  void unified_bed_leveling::restore_ubl_active_state_and_leave() {
    if (--ubl_state_recursion_chk) {
      SERIAL_ECHOLNPGM("restore_ubl_active_state_and_leave() called too many times.");
      LCD_MESSAGEPGM("restore_UBL_active() error"); // TODO: Make translatable string
      lcd_quick_feedback();
      return;
    }
    ubl.state.active = ubl_state_at_invocation;
  }

  /**
   * Much of the 'What?' command can be eliminated. But until we are fully debugged, it is
   * good to have the extra information. Soon... we prune this to just a few items
   */
  void unified_bed_leveling::g29_what_command() {
    report_state();

    if (state.storage_slot == -1)
      SERIAL_PROTOCOLPGM("No Mesh Loaded.");
    else {
      SERIAL_PROTOCOLPAIR("Mesh ", state.storage_slot);
      SERIAL_PROTOCOLPGM(" Loaded.");
    }
    SERIAL_EOL;
    safe_delay(50);

    SERIAL_PROTOCOLLNPAIR("UBL object count: ", (int)ubl_cnt);

    #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
      SERIAL_PROTOCOL("planner.z_fade_height : ");
      SERIAL_PROTOCOL_F(planner.z_fade_height, 4);
      SERIAL_EOL;
    #endif
    SERIAL_PROTOCOLPGM("zprobe_zoffset: ");
    SERIAL_PROTOCOL_F(zprobe_zoffset, 7);
    SERIAL_EOL;

    SERIAL_ECHOLNPAIR("UBL_MESH_MIN_X  " STRINGIFY(UBL_MESH_MIN_X) "=", UBL_MESH_MIN_X);
    SERIAL_ECHOLNPAIR("UBL_MESH_MIN_Y  " STRINGIFY(UBL_MESH_MIN_Y) "=", UBL_MESH_MIN_Y);
    safe_delay(25);
    SERIAL_ECHOLNPAIR("UBL_MESH_MAX_X  " STRINGIFY(UBL_MESH_MAX_X) "=", UBL_MESH_MAX_X);
    SERIAL_ECHOLNPAIR("UBL_MESH_MAX_Y  " STRINGIFY(UBL_MESH_MAX_Y) "=", UBL_MESH_MAX_Y);
    safe_delay(25);
    SERIAL_ECHOLNPAIR("GRID_MAX_POINTS_X  ", GRID_MAX_POINTS_X);
    SERIAL_ECHOLNPAIR("GRID_MAX_POINTS_Y  ", GRID_MAX_POINTS_Y);
    safe_delay(25);
    SERIAL_ECHOLNPAIR("MESH_X_DIST  ", MESH_X_DIST);
    SERIAL_ECHOLNPAIR("MESH_Y_DIST  ", MESH_Y_DIST);
    safe_delay(25);

    SERIAL_PROTOCOLPGM("X-Axis Mesh Points at: ");
    for (uint8_t i = 0; i < GRID_MAX_POINTS_X; i++) {
      SERIAL_PROTOCOL_F(LOGICAL_X_POSITION(pgm_read_float(&mesh_index_to_xpos[i])), 3);
      SERIAL_PROTOCOLPGM("  ");
      safe_delay(25);
    }
    SERIAL_EOL;

    SERIAL_PROTOCOLPGM("Y-Axis Mesh Points at: ");
    for (uint8_t i = 0; i < GRID_MAX_POINTS_Y; i++) {
      SERIAL_PROTOCOL_F(LOGICAL_Y_POSITION(pgm_read_float(&mesh_index_to_ypos[i])), 3);
      SERIAL_PROTOCOLPGM("  ");
      safe_delay(25);
    }
    SERIAL_EOL;

    #if HAS_KILL
      SERIAL_PROTOCOLPAIR("Kill pin on :", KILL_PIN);
      SERIAL_PROTOCOLLNPAIR("  state:", READ(KILL_PIN));
    #endif
    SERIAL_EOL;
    safe_delay(50);

    SERIAL_PROTOCOLLNPAIR("ubl_state_at_invocation :", ubl_state_at_invocation);
    SERIAL_EOL;
    SERIAL_PROTOCOLLNPAIR("ubl_state_recursion_chk :", ubl_state_recursion_chk);
    SERIAL_EOL;
    safe_delay(50);

    SERIAL_PROTOCOLPAIR("Meshes go from ", hex_address((void*)settings.get_start_of_meshes()));
    SERIAL_PROTOCOLLNPAIR(" to ", hex_address((void*)settings.get_end_of_meshes()));
    safe_delay(50);

    SERIAL_PROTOCOLLNPAIR("sizeof(ubl) :  ", (int)sizeof(ubl));
    SERIAL_EOL;
    SERIAL_PROTOCOLLNPAIR("z_value[][] size: ", (int)sizeof(z_values));
    SERIAL_EOL;
    safe_delay(25);

    SERIAL_PROTOCOLLNPAIR("EEPROM free for UBL: ", hex_address((void*)(settings.get_end_of_meshes() - settings.get_start_of_meshes())));
    safe_delay(50);

    SERIAL_PROTOCOLPAIR("EEPROM can hold ", settings.calc_num_meshes());
    SERIAL_PROTOCOLLNPGM(" meshes.\n");
    safe_delay(25);

    if (!sanity_check()) {
      echo_name();
      SERIAL_PROTOCOLLNPGM(" sanity checks passed.");
    }
  }

  /**
   * When we are fully debugged, the EEPROM dump command will get deleted also. But
   * right now, it is good to have the extra information. Soon... we prune this.
   */
  void g29_eeprom_dump() {
    unsigned char cccc;
    uint16_t kkkk;

    SERIAL_ECHO_START;
    SERIAL_ECHOLNPGM("EEPROM Dump:");
    for (uint16_t i = 0; i < E2END + 1; i += 16) {
      if (!(i & 0x3)) idle();
      print_hex_word(i);
      SERIAL_ECHOPGM(": ");
      for (uint16_t j = 0; j < 16; j++) {
        kkkk = i + j;
        eeprom_read_block(&cccc, (void *)kkkk, 1);
        print_hex_byte(cccc);
        SERIAL_ECHO(' ');
      }
      SERIAL_EOL;
    }
    SERIAL_EOL;
  }

  /**
   * When we are fully debugged, this may go away. But there are some valid
   * use cases for the users. So we can wait and see what to do with it.
   */
  void g29_compare_current_mesh_to_stored_mesh() {
    int16_t a = settings.calc_num_meshes();

    if (!a) {
      SERIAL_PROTOCOLLNPGM("?EEPROM storage not available.");
      return;
    }

    if (!code_has_value()) {
      SERIAL_PROTOCOLLNPGM("?Storage slot # required.");
      SERIAL_PROTOCOLLNPAIR("?Use 0 to ", a - 1);
      return;
    }

    storage_slot = code_value_int();

    if (!WITHIN(storage_slot, 0, a - 1)) {
      SERIAL_PROTOCOLLNPGM("?Invalid storage slot.");
      SERIAL_PROTOCOLLNPAIR("?Use 0 to ", a - 1);
      return;
    }

    float tmp_z_values[GRID_MAX_POINTS_X][GRID_MAX_POINTS_Y];
    settings.load_mesh(storage_slot, &tmp_z_values);

    SERIAL_PROTOCOLPAIR("Subtracting mesh in slot ", storage_slot);
    SERIAL_PROTOCOLLNPGM(" from current mesh.");

    for (uint8_t x = 0; x < GRID_MAX_POINTS_X; x++)
      for (uint8_t y = 0; y < GRID_MAX_POINTS_Y; y++)
        ubl.z_values[x][y] -= tmp_z_values[x][y];
  }

  mesh_index_pair find_closest_mesh_point_of_type(const MeshPointType type, const float &lx, const float &ly, const bool probe_as_reference, unsigned int bits[16], const bool far_flag) {
    mesh_index_pair out_mesh;
    out_mesh.x_index = out_mesh.y_index = -1;

    // Get our reference position. Either the nozzle or probe location.
    const float px = RAW_X_POSITION(lx) - (probe_as_reference == USE_PROBE_AS_REFERENCE ? X_PROBE_OFFSET_FROM_EXTRUDER : 0),
                py = RAW_Y_POSITION(ly) - (probe_as_reference == USE_PROBE_AS_REFERENCE ? Y_PROBE_OFFSET_FROM_EXTRUDER : 0),
                raw_x = RAW_CURRENT_POSITION(X), raw_y = RAW_CURRENT_POSITION(Y);

    float closest = far_flag ? -99999.99 : 99999.99;

    for (uint8_t i = 0; i < GRID_MAX_POINTS_X; i++) {
      for (uint8_t j = 0; j < GRID_MAX_POINTS_Y; j++) {

        if ( (type == INVALID && isnan(ubl.z_values[i][j]))  // Check to see if this location holds the right thing
          || (type == REAL && !isnan(ubl.z_values[i][j]))
          || (type == SET_IN_BITMAP && is_bit_set(bits, i, j))
        ) {

          // We only get here if we found a Mesh Point of the specified type

          const float mx = pgm_read_float(&ubl.mesh_index_to_xpos[i]), // Check if we can probe this mesh location
                      my = pgm_read_float(&ubl.mesh_index_to_ypos[j]);

          // If using the probe as the reference there are some unreachable locations.
          // Also for round beds, there are grid points outside the bed that nozzle can't reach.
          // Prune them from the list and ignore them till the next Phase (manual nozzle probing).

          if ( ! (probe_as_reference ? position_is_reachable_by_probe_raw_xy(mx, my) : position_is_reachable_raw_xy(mx, my)) )
            continue;

          // Reachable. Check if it's the closest location to the nozzle.
          // Add in a weighting factor that considers the current location of the nozzle.

          float distance = HYPOT(px - mx, py - my) + HYPOT(raw_x - mx, raw_y - my) * 0.1;

          /**
           * If doing the far_flag action, we want to be as far as possible
           * from the starting point and from any other probed points. We
           * want the next point spread out and filling in any blank spaces
           * in the mesh. So we add in some of the distance to every probed
           * point we can find.
           */
          if (far_flag) {
            for (uint8_t k = 0; k < GRID_MAX_POINTS_X; k++) {
              for (uint8_t l = 0; l < GRID_MAX_POINTS_Y; l++) {
                if (!isnan(ubl.z_values[k][l])) {
                  distance += sq(i - k) * (MESH_X_DIST) * .05
                            + sq(j - l) * (MESH_Y_DIST) * .05;
                }
              }
            }
          }

          // if far_flag, look for farthest point
          if (far_flag == (distance > closest) && distance != closest) {
            closest = distance;       // We found a closer/farther location with
            out_mesh.x_index = i;     // the specified type of mesh value.
            out_mesh.y_index = j;
            out_mesh.distance = closest;
          }
        }
      } // for j
    } // for i

    return out_mesh;
  }

  void fine_tune_mesh(const float &lx, const float &ly, const bool do_ubl_mesh_map) {
    if (!code_seen('R'))    // fine_tune_mesh() is special.  If no repetion count flag is specified
      repetition_cnt = 1;   // we know to do exactly one mesh location. Otherwise we use what the parser decided.

    mesh_index_pair location;
    uint16_t not_done[16];
    int32_t round_off;

    if ( ! position_is_reachable_xy( lx, ly )) {
      SERIAL_PROTOCOLLNPGM("(X,Y) outside printable radius.");
      return;
    }

    ubl.save_ubl_active_state_and_disable();

    memset(not_done, 0xFF, sizeof(not_done));

    LCD_MESSAGEPGM("Fine Tuning Mesh"); // TODO: Make translatable string

    do_blocking_move_to_z(Z_CLEARANCE_DEPLOY_PROBE);
    do_blocking_move_to_xy(lx, ly);
    do {
      location = find_closest_mesh_point_of_type(SET_IN_BITMAP, lx, ly, USE_NOZZLE_AS_REFERENCE, not_done, false);

      if (location.x_index < 0 ) break; // stop when we can't find any more reachable points.

      bit_clear(not_done, location.x_index, location.y_index);  // Mark this location as 'adjusted' so we will find a
                                                                // different location the next time through the loop

      const float rawx = pgm_read_float(&ubl.mesh_index_to_xpos[location.x_index]),
                  rawy = pgm_read_float(&ubl.mesh_index_to_ypos[location.y_index]);

      if ( ! position_is_reachable_raw_xy( rawx, rawy )) { // SHOULD NOT OCCUR because find_closest_mesh_point_of_type will only return reachable
        break;
      }

      float new_z = ubl.z_values[location.x_index][location.y_index];

      if (!isnan(new_z)) {  //can't fine tune a point that hasn't been probed

        do_blocking_move_to_z(Z_CLEARANCE_DEPLOY_PROBE);    // Move the nozzle to where we are going to edit
        do_blocking_move_to_xy(LOGICAL_X_POSITION(rawx), LOGICAL_Y_POSITION(rawy));

        round_off = (int32_t)(new_z * 1000.0);    // we chop off the last digits just to be clean. We are rounding to the
        new_z = float(round_off) / 1000.0;

        KEEPALIVE_STATE(PAUSED_FOR_USER);
        ubl.has_control_of_lcd_panel = true;

        if (do_ubl_mesh_map) ubl.display_map(map_type);  // show the user which point is being adjusted

        lcd_implementation_clear();

        lcd_mesh_edit_setup(new_z);

        do {
          new_z = lcd_mesh_edit();
          idle();
        } while (!ubl_lcd_clicked());

        lcd_return_to_status();

        // There is a race condition for the Encoder Wheel getting clicked.
        // It could get detected in lcd_mesh_edit (actually _lcd_mesh_fine_tune)
        // or here.
        ubl.has_control_of_lcd_panel = true;
      }

      const millis_t nxt = millis() + 1500UL;
      while (ubl_lcd_clicked()) { // debounce and watch for abort
        idle();
        if (ELAPSED(millis(), nxt)) {
          lcd_return_to_status();
          //SERIAL_PROTOCOLLNPGM("\nFine Tuning of Mesh Stopped.");
          do_blocking_move_to_z(Z_CLEARANCE_DEPLOY_PROBE);
          LCD_MESSAGEPGM("Mesh Editing Stopped"); // TODO: Make translatable string

          while (ubl_lcd_clicked()) idle();

          goto FINE_TUNE_EXIT;
        }
      }

      safe_delay(20);                       // We don't want any switch noise.

      ubl.z_values[location.x_index][location.y_index] = new_z;

      lcd_implementation_clear();

    } while (( location.x_index >= 0 ) && (--repetition_cnt>0));

    FINE_TUNE_EXIT:

    ubl.has_control_of_lcd_panel = false;
    KEEPALIVE_STATE(IN_HANDLER);

    if (do_ubl_mesh_map) ubl.display_map(map_type);
    ubl.restore_ubl_active_state_and_leave();
    do_blocking_move_to_z(Z_CLEARANCE_DEPLOY_PROBE);

    do_blocking_move_to_xy(lx, ly);

    LCD_MESSAGEPGM("Done Editing Mesh"); // TODO: Make translatable string
    SERIAL_ECHOLNPGM("Done Editing Mesh");
  }

  /**
   * 'Smart Fill': Scan from the outward edges of the mesh towards the center.
   * If an invalid location is found, use the next two points (if valid) to
   * calculate a 'reasonable' value for the unprobed mesh point.
   */

  bool smart_fill_one(const uint8_t x, const uint8_t y, const int8_t xdir, const int8_t ydir) {
    const int8_t x1 = x + xdir, x2 = x1 + xdir,
                 y1 = y + ydir, y2 = y1 + ydir;
    // A NAN next to a pair of real values?
    if (isnan(ubl.z_values[x][y]) && !isnan(ubl.z_values[x1][y1]) && !isnan(ubl.z_values[x2][y2])) {
      if (ubl.z_values[x1][y1] < ubl.z_values[x2][y2])                  // Angled downward?
        ubl.z_values[x][y] = ubl.z_values[x1][y1];                      // Use nearest (maybe a little too high.)
      else
        ubl.z_values[x][y] = 2.0 * ubl.z_values[x1][y1] - ubl.z_values[x2][y2];   // Angled upward...
      return true;
    }
    return false;
  }

  typedef struct { uint8_t sx, ex, sy, ey; bool yfirst; } smart_fill_info;

  void smart_fill_mesh() {
    const smart_fill_info info[] = {
      { 0, GRID_MAX_POINTS_X,      0, GRID_MAX_POINTS_Y - 2,  false },  // Bottom of the mesh looking up
      { 0, GRID_MAX_POINTS_X,      GRID_MAX_POINTS_Y - 1, 0,  false },  // Top of the mesh looking down
      { 0, GRID_MAX_POINTS_X - 2,  0, GRID_MAX_POINTS_Y,      true  },  // Left side of the mesh looking right
      { GRID_MAX_POINTS_X - 1, 0,  0, GRID_MAX_POINTS_Y,      true  }   // Right side of the mesh looking left
    };
    for (uint8_t i = 0; i < COUNT(info); ++i) {
      const smart_fill_info &f = info[i];
      if (f.yfirst) {
        const int8_t dir = f.ex > f.sx ? 1 : -1;
        for (uint8_t y = f.sy; y != f.ey; ++y)
          for (uint8_t x = f.sx; x != f.ex; x += dir)
            if (smart_fill_one(x, y, dir, 0)) break;
      }
      else {
        const int8_t dir = f.ey > f.sy ? 1 : -1;
         for (uint8_t x = f.sx; x != f.ex; ++x)
          for (uint8_t y = f.sy; y != f.ey; y += dir)
            if (smart_fill_one(x, y, 0, dir)) break;
      }
    }
  }

  void unified_bed_leveling::tilt_mesh_based_on_probed_grid(const bool do_ubl_mesh_map) {
    constexpr int16_t x_min = max(MIN_PROBE_X, UBL_MESH_MIN_X),
                      x_max = min(MAX_PROBE_X, UBL_MESH_MAX_X),
                      y_min = max(MIN_PROBE_Y, UBL_MESH_MIN_Y),
                      y_max = min(MAX_PROBE_Y, UBL_MESH_MAX_Y);

    const float dx = float(x_max - x_min) / (grid_size - 1.0),
                dy = float(y_max - y_min) / (grid_size - 1.0);

    struct linear_fit_data lsf_results;
    incremental_LSF_reset(&lsf_results);

    bool zig_zag = false;
    for (uint8_t ix = 0; ix < grid_size; ix++) {
      const float x = float(x_min) + ix * dx;
      for (int8_t iy = 0; iy < grid_size; iy++) {
        const float y = float(y_min) + dy * (zig_zag ? grid_size - 1 - iy : iy);
        float measured_z = probe_pt(LOGICAL_X_POSITION(x), LOGICAL_Y_POSITION(y), code_seen('E'), g29_verbose_level);
        #if ENABLED(DEBUG_LEVELING_FEATURE)
          if (DEBUGGING(LEVELING)) {
            SERIAL_CHAR('(');
            SERIAL_PROTOCOL_F(x, 7);
            SERIAL_CHAR(',');
            SERIAL_PROTOCOL_F(y, 7);
            SERIAL_ECHOPGM(")   logical: ");
            SERIAL_CHAR('(');
            SERIAL_PROTOCOL_F(LOGICAL_X_POSITION(x), 7);
            SERIAL_CHAR(',');
            SERIAL_PROTOCOL_F(LOGICAL_X_POSITION(y), 7);
            SERIAL_ECHOPGM(")   measured: ");
            SERIAL_PROTOCOL_F(measured_z, 7);
            SERIAL_ECHOPGM("   correction: ");
            SERIAL_PROTOCOL_F(get_z_correction(LOGICAL_X_POSITION(x), LOGICAL_Y_POSITION(y)), 7);
          }
        #endif

        measured_z -= get_z_correction(LOGICAL_X_POSITION(x), LOGICAL_Y_POSITION(y)) /* + zprobe_zoffset */ ;

        #if ENABLED(DEBUG_LEVELING_FEATURE)
          if (DEBUGGING(LEVELING)) {
            SERIAL_ECHOPGM("   final >>>---> ");
            SERIAL_PROTOCOL_F(measured_z, 7);
            SERIAL_EOL;
          }
        #endif

        incremental_LSF(&lsf_results, x, y, measured_z);
      }

      zig_zag ^= true;
    }

    if (finish_incremental_LSF(&lsf_results)) {
      SERIAL_ECHOPGM("Could not complete LSF!");
      return;
    }

    if (g29_verbose_level > 3) {
      SERIAL_ECHOPGM("LSF Results A=");
      SERIAL_PROTOCOL_F(lsf_results.A, 7);
      SERIAL_ECHOPGM("  B=");
      SERIAL_PROTOCOL_F(lsf_results.B, 7);
      SERIAL_ECHOPGM("  D=");
      SERIAL_PROTOCOL_F(lsf_results.D, 7);
      SERIAL_EOL;
    }

    vector_3 normal = vector_3(lsf_results.A, lsf_results.B, 1.0000).get_normal();

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
        float x_tmp = pgm_read_float(&mesh_index_to_xpos[i]),
              y_tmp = pgm_read_float(&mesh_index_to_ypos[j]),
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

        z_values[i][j] += z_tmp - lsf_results.D;
      }
    }

    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) {
        rotation.debug(PSTR("rotation matrix:"));
        SERIAL_ECHOPGM("LSF Results A=");
        SERIAL_PROTOCOL_F(lsf_results.A, 7);
        SERIAL_ECHOPGM("  B=");
        SERIAL_PROTOCOL_F(lsf_results.B, 7);
        SERIAL_ECHOPGM("  D=");
        SERIAL_PROTOCOL_F(lsf_results.D, 7);
        SERIAL_EOL;
        safe_delay(55);

        SERIAL_ECHOPGM("bed plane normal = [");
        SERIAL_PROTOCOL_F(normal.x, 7);
        SERIAL_PROTOCOLCHAR(',');
        SERIAL_PROTOCOL_F(normal.y, 7);
        SERIAL_PROTOCOLCHAR(',');
        SERIAL_PROTOCOL_F(normal.z, 7);
        SERIAL_ECHOPGM("]\n");
        SERIAL_EOL;
      }
    #endif
  }

#endif // AUTO_BED_LEVELING_UBL
