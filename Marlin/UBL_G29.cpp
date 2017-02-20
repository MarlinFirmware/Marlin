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

  #include "UBL.h"
  #include "Marlin.h"
  #include "hex_print_routines.h"
  #include "configuration_store.h"
  #include "planner.h"
  #include "ultralcd.h"

  #include <math.h>

  void lcd_babystep_z();
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
  extern bool code_value_bool();
  extern bool code_has_value();
  extern float probe_pt(float x, float y, bool, int);
  extern bool set_probe_deployed(bool);
  #define DEPLOY_PROBE() set_probe_deployed(true)
  #define STOW_PROBE() set_probe_deployed(false)
  bool ProbeStay = true;

  constexpr float ubl_3_point_1_X = UBL_PROBE_PT_1_X,
                  ubl_3_point_1_Y = UBL_PROBE_PT_1_Y,
                  ubl_3_point_2_X = UBL_PROBE_PT_2_X,
                  ubl_3_point_2_Y = UBL_PROBE_PT_2_Y,
                  ubl_3_point_3_X = UBL_PROBE_PT_3_X,
                  ubl_3_point_3_Y = UBL_PROBE_PT_3_Y;

  #define SIZE_OF_LITTLE_RAISE 0
  #define BIG_RAISE_NOT_NEEDED 0
  extern void lcd_quick_feedback();

  /**
   *   G29: Unified Bed Leveling by Roxy
   *
   *   Parameters understood by this leveling system:
   *
   *   A     Activate   Activate the Unified Bed Leveling system.
   *
   *   B #   Business   Use the 'Business Card' mode of the Manual Probe subsystem. This is invoked as
   *                    G29 P2 B   The mode of G29 P2 allows you to use a bussiness card or recipe card
   *                    as a shim that the nozzle will pinch as it is lowered. The idea is that you
   *                    can easily feel the nozzle getting to the same height by the amount of resistance
   *                    the business card exhibits to movement. You should try to achieve the same amount
   *                    of resistance on each probed point to facilitate accurate and repeatable measurements.
   *                    You should be very careful not to drive the nozzle into the bussiness card with a
   *                    lot of force as it is very possible to cause damage to your printer if your are
   *                    careless. If you use the B option with G29 P2 B you can leave the number parameter off
   *                    on its first use to enable measurement of the business card thickness. Subsequent usage
   *                    of the B parameter can have the number previously measured supplied to the command.
   *                    Incidently, you are much better off using something like a Spark Gap feeler gauge than
   *                    something that compresses like a Business Card.
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
   *   F #   Fade   *   Fade the amount of Mesh Based Compensation over a specified height. At the
   *                    specified height, no correction is applied and natural printer kenimatics take over. If no
   *                    number is specified for the command, 10mm is assumed to be reasonable.
   *
   *   G #   Grid   *   Perform a Grid Based Leveling of the current Mesh using a grid with n points on a side.
   *
   *   H #   Height     Specify the Height to raise the nozzle after each manual probe of the bed. The
   *                    default is 5mm.
   *
   *   I #   Invalidate Invalidate specified number of Mesh Points. The nozzle location is used unless
   *                    the X and Y parameter are used. If no number is specified, only the closest Mesh
   *                    point to the location is invalidated. The M parameter is available as well to produce
   *                    a map after the operation. This command is useful to invalidate a portion of the
   *                    Mesh so it can be adjusted using other tools in the Unified Bed Leveling System. When
   *                    attempting to invalidate an isolated bad point in the mesh, the M option will indicate
   *                    where the nozzle is positioned in the Mesh with (#). You can move the nozzle around on
   *                    the bed and use this feature to select the center of the area (or cell) you want to
   *                    invalidate.
   *
   *   K #   Kompare    Kompare current Mesh with stored Mesh # replacing current Mesh with the result. This
   *                    command literally performs a diff between two Meshes.
   *
   *   L     Load   *   Load Mesh from the previously activated location in the EEPROM.
   *
   *   L #   Load   *   Load Mesh from the specified location in the EEPROM. Set this location as activated
   *                    for subsequent Load and Store operations.
   *
   *   O     Map   *    Display the Mesh Map Topology.
   *                    The parameter can be specified alone (ie. G29 O) or in combination with many of the
   *                    other commands. The Mesh Map option works with all of the Phase
   *                    commands (ie. G29 P4 R 5 X 50 Y100 C -.1 O)  The Map parameter can also of a Map Type
   *                    specified.  A map type of 0 is the default is user readable.   A map type of 1 can
   *                    be specified and is suitable to Cut & Paste into Excel to allow graphing of the user's
   *                    mesh.
   *
   *   N    No Home     G29 normally insists that a G28 has been performed. You can over rule this with an
   *                    N option. In general, you should not do this. This can only be done safely with
   *                    commands that do not move the nozzle.
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
   *                    the Z-Probe. Depending upon the values of DELTA_PROBEABLE_RADIUS and
   *                    DELTA_PRINTABLE_RADIUS some area of the bed will not have Mesh Data automatically
   *                    generated. This will be handled in Phase 2. If the Phase 1 command is given the
   *                    C (Continue) parameter it does not invalidate the Mesh prior to automatically
   *                    probing needed locations. This allows you to invalidate portions of the Mesh but still
   *                    use the automatic probing capabilities of the Unified Bed Leveling System. An X and Y
   *                    parameter can be given to prioritize where the command should be trying to measure points.
   *                    If the X and Y parameters are not specified the current probe position is used. Phase 1
   *                    allows you to specify the M (Map) parameter so you can watch the generation of the Mesh.
   *                    Phase 1 also watches for the LCD Panel's Encoder Switch being held in a depressed state.
   *                    It will suspend generation of the Mesh if it sees the user request that. (This check is
   *                    only done between probe points. You will need to press and hold the switch until the
   *                    Phase 1 command can detect it.)
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
   *                    print out a Mesh Map (G29 O ) to understand where the mesh is invalidated and where
   *                    the nozzle will need to move in order to complete the command. The C parameter is
   *                    available on the Phase 2 command also and indicates the search for points to measure should
   *                    be done based on the current location of the nozzle.
   *
   *                    A B parameter is also available for this command and described up above. It places the
   *                    manual probe subsystem into Business Card mode where the thickness of a business care is
   *                    measured and then used to accurately set the nozzle height in all manual probing for the
   *                    duration of the command. (S for Shim mode would be a better parameter name, but S is needed
   *                    for Save or Store of the Mesh to EEPROM)  A Business card can be used, but you will have
   *                    better results if you use a flexible Shim that does not compress very much. That makes it
   *                    easier for you to get the nozzle to press with similar amounts of force against the shim so you
   *                    can get accurate measurements. As you are starting to touch the nozzle against the shim try
   *                    to get it to grasp the shim with the same force as when you measured the thickness of the
   *                    shim at the start of the command.
   *
   *                    Phase 2 allows the O (Map) parameter to be specified. This helps the user see the progression
   *                    of the Mesh being built.
   *
   *   P3    Phase 3    Fill the unpopulated regions of the Mesh with a fixed value. The C parameter is
   *                    used to specify the 'constant' value to fill all invalid areas of the Mesh. If no C parameter
   *                    is specified, a value of 0.0 is assumed. The R parameter can be given to specify the number
   *                    of points to set. If the R parameter is specified the current nozzle position is used to
   *                    find the closest points to alter unless the X and Y parameter are used to specify the fill
   *                    location.
   *
   *   P4    Phase 4    Fine tune the Mesh. The Delta Mesh Compensation System assume the existance of
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
   *   Q     Test   *   Load specified Test Pattern to assist in checking correct operation of system. This
   *                    command is not anticipated to be of much value to the typical user. It is intended
   *                    for developers to help them verify correct operation of the Unified Bed Leveling System.
   *
   *   S     Store      Store the current Mesh in the Activated area of the EEPROM. It will also store the
   *                    current state of the Unified Bed Leveling system in the EEPROM.
   *
   *   S #   Store      Store the current Mesh at the specified location in EEPROM. Activate this location
   *                    for subsequent Load and Store operations. It will also store the current state of
   *                    the Unified Bed Leveling system in the EEPROM.
   *
   *   S -1  Store      Store the current Mesh as a print out that is suitable to be feed back into
   *                    the system at a later date. The text generated can be saved and later sent by PronterFace or
   *                    Repetier Host to reconstruct the current mesh on another machine.
   *
   *   T     3-Point    Perform a 3 Point Bed Leveling on the current Mesh
   *
   *   U     Unlevel    Perform a probe of the outer perimeter to assist in physically leveling unlevel beds.
   *                    Only used for G29 P1 O U   It will speed up the probing of the edge of the bed.  This
   *                    is useful when the entire bed does not need to be probed because it will be adjusted.
   *
   *   W     What?      Display valuable data the Unified Bed Leveling System knows.
   *
   *   X #   *      *   X Location for this line of commands
   *
   *   Y #   *      *   Y Location for this line of commands
   *
   *   Z     Zero   *   Probes to set the Z Height of the nozzle. The entire Mesh can be raised or lowered
   *                    by just doing a G29 Z
   *
   *   Z #   Zero   *   The entire Mesh can be raised or lowered to conform with the specified difference.
   *                    zprobe_zoffset is added to the calculation.
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
  static int g29_verbose_level, phase_value = -1, repetition_cnt,
             storage_slot = 0, map_type; //unlevel_value = -1;
  static bool repeat_flag, c_flag, x_flag, y_flag;
  static float x_pos, y_pos, measured_z, card_thickness = 0.0, ubl_constant = 0.0;

  #if ENABLED(ULTRA_LCD)
    extern void lcd_setstatus(const char* message, const bool persist);
    extern void lcd_setstatuspgm(const char* message, const uint8_t level);
  #endif

  void gcode_G29() {
    SERIAL_PROTOCOLLNPAIR("ubl.eeprom_start=", ubl.eeprom_start);
    if (ubl.eeprom_start < 0) {
      SERIAL_PROTOCOLLNPGM("?You need to enable your EEPROM and initialize it");
      SERIAL_PROTOCOLLNPGM("with M502, M500, M501 in that order.\n");
      return;
    }

    if (!code_seen('N') && axis_unhomed_error(true, true, true))  // Don't allow auto-leveling without homing first
      gcode_G28();

    if (g29_parameter_parsing()) return; // abort if parsing the simple parameters causes a problem,

    // Invalidate Mesh Points. This command is a little bit asymetrical because
    // it directly specifies the repetition count and does not use the 'R' parameter.
    if (code_seen('I')) {
      repetition_cnt = code_has_value() ? code_value_int() : 1;
      while (repetition_cnt--) {
        const mesh_index_pair location = find_closest_mesh_point_of_type(REAL, x_pos, y_pos, 0, NULL, false);  // The '0' says we want to use the nozzle's position
        if (location.x_index < 0) {
          SERIAL_PROTOCOLLNPGM("Entire Mesh invalidated.\n");
          break;            // No more invalid Mesh Points to populate
        }
        ubl.z_values[location.x_index][location.y_index] = NAN;
      }
      SERIAL_PROTOCOLLNPGM("Locations invalidated.\n");
    }

    if (code_seen('Q')) {

      const int test_pattern = code_has_value() ? code_value_int() : -1;
      if (!WITHIN(test_pattern, 0, 2)) {
        SERIAL_PROTOCOLLNPGM("Invalid test_pattern value. (0-2)\n");
        return;
      }
      SERIAL_PROTOCOLLNPGM("Loading test_pattern values.\n");
      switch (test_pattern) {
        case 0:
          for (uint8_t x = 0; x < UBL_MESH_NUM_X_POINTS; x++) {   // Create a bowl shape - similar to
            for (uint8_t y = 0; y < UBL_MESH_NUM_Y_POINTS; y++) { // a poorly calibrated Delta.
              const float p1 = 0.5 * (UBL_MESH_NUM_X_POINTS) - x,
                          p2 = 0.5 * (UBL_MESH_NUM_Y_POINTS) - y;
              ubl.z_values[x][y] += 2.0 * HYPOT(p1, p2);
            }
          }
          break;
        case 1:
          for (uint8_t x = 0; x < UBL_MESH_NUM_X_POINTS; x++) {  // Create a diagonal line several Mesh cells thick that is raised
            ubl.z_values[x][x] += 9.999;
            ubl.z_values[x][x + (x < UBL_MESH_NUM_Y_POINTS - 1) ? 1 : -1] += 9.999; // We want the altered line several mesh points thick
          }
          break;
        case 2:
          // Allow the user to specify the height because 10mm is a little extreme in some cases.
          for (uint8_t x = (UBL_MESH_NUM_X_POINTS) / 3; x < 2 * (UBL_MESH_NUM_X_POINTS) / 3; x++)   // Create a rectangular raised area in
            for (uint8_t y = (UBL_MESH_NUM_Y_POINTS) / 3; y < 2 * (UBL_MESH_NUM_Y_POINTS) / 3; y++) // the center of the bed
              ubl.z_values[x][y] += code_seen('C') ? ubl_constant : 9.99;
          break;
      }
    }

    /*
    if (code_seen('U')) {
      unlevel_value = code_value_int();
      //if (!WITHIN(unlevel_value, 0, 7)) {
      //  SERIAL_PROTOCOLLNPGM("Invalid Unlevel value. (0-4)\n");
      //  return;
      //}
    }
    //*/

    if (code_seen('P')) {
      phase_value = code_value_int();
      if (!WITHIN(phase_value, 0, 7)) {
        SERIAL_PROTOCOLLNPGM("Invalid Phase value. (0-4)\n");
        return;
      }
      switch (phase_value) {
        case 0:
          //
          // Zero Mesh Data
          //
          ubl.reset();
          SERIAL_PROTOCOLLNPGM("Mesh zeroed.\n");
          break;

        case 1:
          //
          // Invalidate Entire Mesh and Automatically Probe Mesh in areas that can be reached by the probe
          //
          if (!code_seen('C') ) {
            ubl.invalidate();
            SERIAL_PROTOCOLLNPGM("Mesh invalidated. Probing mesh.\n");
          }
          if (g29_verbose_level > 1) {
            SERIAL_ECHOPGM("Probing Mesh Points Closest to (");
            SERIAL_ECHO(x_pos);
            SERIAL_ECHOPAIR(",", y_pos);
            SERIAL_PROTOCOLLNPGM(")\n");
          }
          probe_entire_mesh(x_pos + X_PROBE_OFFSET_FROM_EXTRUDER, y_pos + Y_PROBE_OFFSET_FROM_EXTRUDER,
                            code_seen('O') || code_seen('M'), code_seen('E'), code_seen('U'));
          break;

        case 2: {
          //
          // Manually Probe Mesh in areas that can't be reached by the probe
          //
          SERIAL_PROTOCOLLNPGM("Manually probing unreachable mesh locations.\n");
          do_blocking_move_to_z(Z_CLEARANCE_BETWEEN_PROBES);
          if (!x_flag && !y_flag) {      // use a good default location for the path
            x_pos = X_MIN_POS;
            y_pos = Y_MIN_POS;
            if (X_PROBE_OFFSET_FROM_EXTRUDER > 0)   // The flipped > and < operators on these two comparisons is
              x_pos = X_MAX_POS;                    // intentional. It should cause the probed points to follow a

            if (Y_PROBE_OFFSET_FROM_EXTRUDER < 0)   // nice path on Cartesian printers. It may make sense to
              y_pos = Y_MAX_POS;                    // have Delta printers default to the center of the bed.

          }                                         // For now, until that is decided, it can be forced with the X
                                                    // and Y parameters.
          if (code_seen('C')) {
            x_pos = current_position[X_AXIS];
            y_pos = current_position[Y_AXIS];
          }

          const float height = code_seen('H') && code_has_value() ? code_value_float() : Z_CLEARANCE_BETWEEN_PROBES;

          if (code_seen('B')) {
            card_thickness = code_has_value() ? code_value_float() : measure_business_card_thickness(height);

            if (FABS(card_thickness) > 1.5) {
              SERIAL_PROTOCOLLNPGM("?Error in Business Card measurement.\n");
              return;
            }
          }
          manually_probe_remaining_mesh(x_pos, y_pos, height, card_thickness, code_seen('O') || code_seen('M'));

        } break;

        case 3: {
          //
          // Populate invalid Mesh areas with a constant
          //
          const float height = code_seen('C') ? ubl_constant : 0.0;
          // If no repetition is specified, do the whole Mesh
          if (!repeat_flag) repetition_cnt = 9999;
          while (repetition_cnt--) {
            const mesh_index_pair location = find_closest_mesh_point_of_type(INVALID, x_pos, y_pos, 0, NULL, false); // The '0' says we want to use the nozzle's position
            if (location.x_index < 0) break; // No more invalid Mesh Points to populate
            ubl.z_values[location.x_index][location.y_index] = height;
          }
        } break;

        case 4:
          //
          // Fine Tune (i.e., Edit) the Mesh
          //
          fine_tune_mesh(x_pos, y_pos, code_seen('O') || code_seen('M'));
          break;
        case 5:
          find_mean_mesh_height();
          break;
        case 6:
          shift_mesh_height();
          break;

        case 10:
          // [DEBUG] Pay no attention to this stuff. It can be removed soon.
          SERIAL_ECHO_START;
          SERIAL_ECHOLNPGM("Checking G29 has control of LCD Panel:");
          KEEPALIVE_STATE(PAUSED_FOR_USER);
          ubl.has_control_of_lcd_panel++;
          while (!ubl_lcd_clicked()) {
            safe_delay(250);
            if (ubl.encoder_diff) {
              SERIAL_ECHOLN((int)ubl.encoder_diff);
              ubl.encoder_diff = 0;
            }
          }
          SERIAL_ECHOLNPGM("G29 giving back control of LCD Panel.");
          ubl.has_control_of_lcd_panel = false;
          KEEPALIVE_STATE(IN_HANDLER);
          break;

        case 11:
          // [DEBUG] wait_for_user code. Pay no attention to this stuff. It can be removed soon.
          SERIAL_ECHO_START;
          SERIAL_ECHOLNPGM("Checking G29 has control of LCD Panel:");
          KEEPALIVE_STATE(PAUSED_FOR_USER);
          wait_for_user = true;
          while (wait_for_user) {
            safe_delay(250);
            if (ubl.encoder_diff) {
              SERIAL_ECHOLN((int)ubl.encoder_diff);
              ubl.encoder_diff = 0;
            }
          }
          SERIAL_ECHOLNPGM("G29 giving back control of LCD Panel.");
          KEEPALIVE_STATE(IN_HANDLER);
          break;
      }
    }

    if (code_seen('T')) {
      const float lx1 = LOGICAL_X_POSITION(ubl_3_point_1_X),
                  lx2 = LOGICAL_X_POSITION(ubl_3_point_2_X),
                  lx3 = LOGICAL_X_POSITION(ubl_3_point_3_X),
                  ly1 = LOGICAL_Y_POSITION(ubl_3_point_1_Y),
                  ly2 = LOGICAL_Y_POSITION(ubl_3_point_2_Y),
                  ly3 = LOGICAL_Y_POSITION(ubl_3_point_3_Y);

      float z1 = probe_pt(lx1, ly1, false /*Stow Flag*/, g29_verbose_level),
            z2 = probe_pt(lx2, ly2, false /*Stow Flag*/, g29_verbose_level),
            z3 = probe_pt(lx3, ly3, true  /*Stow Flag*/, g29_verbose_level);

      //  We need to adjust z1, z2, z3 by the Mesh Height at these points. Just because they are non-zero doesn't mean
      //  the Mesh is tilted!  (We need to compensate each probe point by what the Mesh says that location's height is)

      z1 -= ubl.get_z_correction(lx1, ly1);
      z2 -= ubl.get_z_correction(lx2, ly2);
      z3 -= ubl.get_z_correction(lx3, ly3);

      do_blocking_move_to_xy((X_MAX_POS - (X_MIN_POS)) / 2.0, (Y_MAX_POS - (Y_MIN_POS)) / 2.0);
      tilt_mesh_based_on_3pts(z1, z2, z3);
    }

    //
    // Much of the 'What?' command can be eliminated. But until we are fully debugged, it is
    // good to have the extra information. Soon... we prune this to just a few items
    //
    if (code_seen('W')) g29_what_command();

    //
    // When we are fully debugged, the EEPROM dump command will get deleted also. But
    // right now, it is good to have the extra information. Soon... we prune this.
    //
    if (code_seen('J')) g29_eeprom_dump();   // EEPROM Dump

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
      storage_slot = code_has_value() ? code_value_int() : ubl.state.eeprom_storage_slot;

      const int16_t j = (UBL_LAST_EEPROM_INDEX - ubl.eeprom_start) / sizeof(ubl.z_values);

      if (!WITHIN(storage_slot, 0, j - 1) || ubl.eeprom_start <= 0) {
        SERIAL_PROTOCOLLNPGM("?EEPROM storage not available for use.\n");
        return;
      }
      ubl.load_mesh(storage_slot);
      ubl.state.eeprom_storage_slot = storage_slot;
      if (storage_slot != ubl.state.eeprom_storage_slot)
        ubl.store_state();
      SERIAL_PROTOCOLLNPGM("Done.\n");
    }

    //
    // Store a Mesh in the EEPROM
    //

    if (code_seen('S')) {     // Store (or Save) Current Mesh Data
      storage_slot = code_has_value() ? code_value_int() : ubl.state.eeprom_storage_slot;

      if (storage_slot == -1) {                     // Special case, we are going to 'Export' the mesh to the
        SERIAL_ECHOLNPGM("G29 I 999");              // host in a form it can be reconstructed on a different machine
        for (uint8_t x = 0; x < UBL_MESH_NUM_X_POINTS; x++)
          for (uint8_t y = 0;  y < UBL_MESH_NUM_Y_POINTS; y++)
            if (!isnan(ubl.z_values[x][y])) {
              SERIAL_ECHOPAIR("M421 I ", x);
              SERIAL_ECHOPAIR(" J ", y);
              SERIAL_ECHOPGM(" Z ");
              SERIAL_ECHO_F(ubl.z_values[x][y], 6);
              SERIAL_EOL;
            }
        return;
      }

      const int16_t j = (UBL_LAST_EEPROM_INDEX - ubl.eeprom_start) / sizeof(ubl.z_values);

      if (!WITHIN(storage_slot, 0, j - 1) || ubl.eeprom_start <= 0) {
        SERIAL_PROTOCOLLNPGM("?EEPROM storage not available for use.\n");
        SERIAL_PROTOCOLLNPAIR("?Use 0 to ", j - 1);
        goto LEAVE;
      }
      ubl.store_mesh(storage_slot);
      ubl.state.eeprom_storage_slot = storage_slot;
      //
      //  if (storage_slot != ubl.state.eeprom_storage_slot)
      ubl.store_state();    // Always save an updated copy of the UBL State info

      SERIAL_PROTOCOLLNPGM("Done.\n");
    }

    if (code_seen('O') || code_seen('M'))
      ubl.display_map(code_has_value() ? code_value_int() : 0);

    if (code_seen('Z')) {
      if (code_has_value())
        ubl.state.z_offset = code_value_float();   // do the simple case. Just lock in the specified value
      else {
        save_ubl_active_state_and_disable();
        //measured_z = probe_pt(x_pos + X_PROBE_OFFSET_FROM_EXTRUDER, y_pos + Y_PROBE_OFFSET_FROM_EXTRUDER, ProbeDeployAndStow, g29_verbose_level);

        ubl.has_control_of_lcd_panel++;     // Grab the LCD Hardware
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

        ubl.has_control_of_lcd_panel++;   // There is a race condition for the Encoder Wheel getting clicked.
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
            lcd_setstatuspgm("Z-Offset Stopped");
            restore_ubl_active_state_and_leave();
            goto LEAVE;
          }
        }
        ubl.has_control_of_lcd_panel = false;
        safe_delay(20); // We don't want any switch noise.

        ubl.state.z_offset = measured_z;

        lcd_implementation_clear();
        restore_ubl_active_state_and_leave();
      }
    }

    LEAVE:

    #if ENABLED(ULTRA_LCD)
      lcd_reset_alert_level();
      lcd_setstatuspgm("");
      lcd_quick_feedback();
    #endif

    ubl.has_control_of_lcd_panel = false;
  }

  void find_mean_mesh_height() {
    uint8_t x, y;
    int n;
    float sum, sum_of_diff_squared, sigma, difference, mean;

    sum = sum_of_diff_squared = 0.0;
    n = 0;
    for (x = 0; x < UBL_MESH_NUM_X_POINTS; x++)
      for (y = 0; y < UBL_MESH_NUM_Y_POINTS; y++)
        if (!isnan(ubl.z_values[x][y])) {
          sum += ubl.z_values[x][y];
          n++;
        }

    mean = sum / n;

    //
    // Now do the sumation of the squares of difference from mean
    //
    for (x = 0; x < UBL_MESH_NUM_X_POINTS; x++)
      for (y = 0; y < UBL_MESH_NUM_Y_POINTS; y++)
        if (!isnan(ubl.z_values[x][y])) {
          difference = (ubl.z_values[x][y] - mean);
          sum_of_diff_squared += difference * difference;
        }

    SERIAL_ECHOLNPAIR("# of samples: ", n);
    SERIAL_ECHOPGM("Mean Mesh Height: ");
    SERIAL_ECHO_F(mean, 6);
    SERIAL_EOL;

    sigma = SQRT(sum_of_diff_squared / (n + 1));
    SERIAL_ECHOPGM("Standard Deviation: ");
    SERIAL_ECHO_F(sigma, 6);
    SERIAL_EOL;

    if (c_flag)
      for (x = 0; x < UBL_MESH_NUM_X_POINTS; x++)
        for (y = 0; y < UBL_MESH_NUM_Y_POINTS; y++)
          if (!isnan(ubl.z_values[x][y]))
            ubl.z_values[x][y] -= mean + ubl_constant;
  }

  void shift_mesh_height() {
    for (uint8_t x = 0; x < UBL_MESH_NUM_X_POINTS; x++)
      for (uint8_t y = 0; y < UBL_MESH_NUM_Y_POINTS; y++)
        if (!isnan(ubl.z_values[x][y]))
          ubl.z_values[x][y] += ubl_constant;
  }

  /**
   * Probe all invalidated locations of the mesh that can be reached by the probe.
   * This attempts to fill in locations closest to the nozzle's start location first.
   */
  void probe_entire_mesh(const float &lx, const float &ly, const bool do_ubl_mesh_map, const bool stow_probe, bool do_furthest) {
    mesh_index_pair location;

    ubl.has_control_of_lcd_panel++;
    save_ubl_active_state_and_disable();   // we don't do bed level correction because we want the raw data when we probe
    DEPLOY_PROBE();

    do {
      if (ubl_lcd_clicked()) {
        SERIAL_PROTOCOLLNPGM("\nMesh only partially populated.\n");
        lcd_quick_feedback();
        STOW_PROBE();
        while (ubl_lcd_clicked()) idle();
        ubl.has_control_of_lcd_panel = false;
        restore_ubl_active_state_and_leave();
        safe_delay(50);  // Debounce the Encoder wheel
        return;
      }

      location = find_closest_mesh_point_of_type(INVALID, lx, ly, 1, NULL, do_furthest );  // the '1' says we want the location to be relative to the probe
      if (location.x_index >= 0 && location.y_index >= 0) {

        const float rawx = ubl.mesh_index_to_xpos[location.x_index],
                    rawy = ubl.mesh_index_to_ypos[location.y_index];

        // TODO: Change to use `position_is_reachable` (for SCARA-compatibility)
        if (!WITHIN(rawx, MIN_PROBE_X, MAX_PROBE_X) || !WITHIN(rawy, MIN_PROBE_Y, MAX_PROBE_Y)) {
          SERIAL_ERROR_START;
          SERIAL_ERRORLNPGM("Attempt to probe off the bed.");
          ubl.has_control_of_lcd_panel = false;
          goto LEAVE;
        }
        const float measured_z = probe_pt(LOGICAL_X_POSITION(rawx), LOGICAL_Y_POSITION(rawy), stow_probe, g29_verbose_level);
        ubl.z_values[location.x_index][location.y_index] = measured_z;
      }

      if (do_ubl_mesh_map) ubl.display_map(map_type);

    } while (location.x_index >= 0 && location.y_index >= 0);

    LEAVE:

    STOW_PROBE();
    restore_ubl_active_state_and_leave();

    do_blocking_move_to_xy(
      constrain(lx - (X_PROBE_OFFSET_FROM_EXTRUDER), X_MIN_POS, X_MAX_POS),
      constrain(ly - (Y_PROBE_OFFSET_FROM_EXTRUDER), Y_MIN_POS, Y_MAX_POS)
    );
  }

  vector_3 tilt_mesh_based_on_3pts(const float &z1, const float &z2, const float &z3) {
    float c, d, t;
    int i, j;

    vector_3 v1 = vector_3( (ubl_3_point_1_X - ubl_3_point_2_X),
                            (ubl_3_point_1_Y - ubl_3_point_2_Y),
                            (z1 - z2) ),

             v2 = vector_3( (ubl_3_point_3_X - ubl_3_point_2_X),
                            (ubl_3_point_3_Y - ubl_3_point_2_Y),
                            (z3 - z2) ),

             normal = vector_3::cross(v1, v2);

    // printf("[%f,%f,%f]    ", normal.x, normal.y, normal.z);

    /**
     * This code does two things. This vector is normal to the tilted plane.
     * However, we don't know its direction. We need it to point up. So if
     * Z is negative, we need to invert the sign of all components of the vector
     * We also need Z to be unity because we are going to be treating this triangle
     * as the sin() and cos() of the bed's tilt
     */
    const float inv_z = 1.0 / normal.z;
    normal.x *= inv_z;
    normal.y *= inv_z;
    normal.z = 1.0;

    //
    // All of 3 of these points should give us the same d constant
    //
    t = normal.x * ubl_3_point_1_X + normal.y * ubl_3_point_1_Y;
    d = t + normal.z * z1;
    c = d - t;
    SERIAL_ECHOPGM("d from 1st point: ");
    SERIAL_ECHO_F(d, 6);
    SERIAL_ECHOPGM("  c: ");
    SERIAL_ECHO_F(c, 6);
    SERIAL_EOL;
    t = normal.x * ubl_3_point_2_X + normal.y * ubl_3_point_2_Y;
    d = t + normal.z * z2;
    c = d - t;
    SERIAL_ECHOPGM("d from 2nd point: ");
    SERIAL_ECHO_F(d, 6);
    SERIAL_ECHOPGM("  c: ");
    SERIAL_ECHO_F(c, 6);
    SERIAL_EOL;
    t = normal.x * ubl_3_point_3_X + normal.y * ubl_3_point_3_Y;
    d = t + normal.z * z3;
    c = d - t;
    SERIAL_ECHOPGM("d from 3rd point: ");
    SERIAL_ECHO_F(d, 6);
    SERIAL_ECHOPGM("  c: ");
    SERIAL_ECHO_F(c, 6);
    SERIAL_EOL;

    for (i = 0; i < UBL_MESH_NUM_X_POINTS; i++) {
      for (j = 0; j < UBL_MESH_NUM_Y_POINTS; j++) {
        c = -((normal.x * (UBL_MESH_MIN_X + i * (MESH_X_DIST)) + normal.y * (UBL_MESH_MIN_Y + j * (MESH_Y_DIST))) - d);
        ubl.z_values[i][j] += c;
      }
    }
    return normal;
  }

  float use_encoder_wheel_to_measure_point() {
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

  float measure_business_card_thickness(const float &in_height) {

    ubl.has_control_of_lcd_panel++;
    save_ubl_active_state_and_disable();   // we don't do bed level correction because we want the raw data when we probe

    SERIAL_PROTOCOLLNPGM("Place Shim Under Nozzle and Perform Measurement.");
    do_blocking_move_to_z(in_height);
    do_blocking_move_to_xy((float(X_MAX_POS) - float(X_MIN_POS)) / 2.0, (float(Y_MAX_POS) - float(Y_MIN_POS)) / 2.0);
      //, min( planner.max_feedrate_mm_s[X_AXIS], planner.max_feedrate_mm_s[Y_AXIS])/2.0);

    const float z1 = use_encoder_wheel_to_measure_point();
    do_blocking_move_to_z(current_position[Z_AXIS] + SIZE_OF_LITTLE_RAISE);
    ubl.has_control_of_lcd_panel = false;

    SERIAL_PROTOCOLLNPGM("Remove Shim and Measure Bed Height.");
    const float z2 = use_encoder_wheel_to_measure_point();
    do_blocking_move_to_z(current_position[Z_AXIS] + SIZE_OF_LITTLE_RAISE);

    if (g29_verbose_level > 1) {
      SERIAL_PROTOCOLPGM("Business Card is: ");
      SERIAL_PROTOCOL_F(abs(z1 - z2), 6);
      SERIAL_PROTOCOLLNPGM("mm thick.");
    }
    restore_ubl_active_state_and_leave();
    return abs(z1 - z2);
  }

  void manually_probe_remaining_mesh(const float &lx, const float &ly, const float &z_clearance, const float &card_thickness, const bool do_ubl_mesh_map) {

    ubl.has_control_of_lcd_panel++;
    save_ubl_active_state_and_disable();   // we don't do bed level correction because we want the raw data when we probe
    do_blocking_move_to_z(z_clearance);
    do_blocking_move_to_xy(lx, ly);

    float last_x = -9999.99, last_y = -9999.99;
    mesh_index_pair location;
    do {
      if (do_ubl_mesh_map) ubl.display_map(map_type);

      location = find_closest_mesh_point_of_type(INVALID, lx, ly, 0, NULL, false); // The '0' says we want to use the nozzle's position
      // It doesn't matter if the probe can't reach the NAN location. This is a manual probe.
      if (location.x_index < 0 && location.y_index < 0) continue;

      const float rawx = ubl.mesh_index_to_xpos[location.x_index],
                  rawy = ubl.mesh_index_to_ypos[location.y_index];

      // TODO: Change to use `position_is_reachable` (for SCARA-compatibility)
      if (!WITHIN(rawx, X_MIN_POS, X_MAX_POS) || !WITHIN(rawy, Y_MIN_POS, Y_MAX_POS)) {
        SERIAL_ERROR_START;
        SERIAL_ERRORLNPGM("Attempt to probe off the bed.");
        ubl.has_control_of_lcd_panel = false;
        goto LEAVE;
      }

      const float xProbe = LOGICAL_X_POSITION(rawx),
                  yProbe = LOGICAL_Y_POSITION(rawy),
                  dx = xProbe - last_x,
                  dy = yProbe - last_y;

      if (HYPOT(dx, dy) < BIG_RAISE_NOT_NEEDED)
        do_blocking_move_to_z(current_position[Z_AXIS] + SIZE_OF_LITTLE_RAISE);
      else
        do_blocking_move_to_z(z_clearance);

      do_blocking_move_to_xy(xProbe, yProbe);

      last_x = xProbe;
      last_y = yProbe;

      KEEPALIVE_STATE(PAUSED_FOR_USER);
      ubl.has_control_of_lcd_panel = true;

      while (!ubl_lcd_clicked()) {     // we need the loop to move the nozzle based on the encoder wheel here!
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
          restore_ubl_active_state_and_leave();
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
    restore_ubl_active_state_and_leave();
    KEEPALIVE_STATE(IN_HANDLER);
    do_blocking_move_to_z(Z_CLEARANCE_DEPLOY_PROBE);
    do_blocking_move_to_xy(lx, ly);
  }

  bool g29_parameter_parsing() {
    #if ENABLED(ULTRA_LCD)
      lcd_setstatuspgm("Doing G29 UBL!");
      lcd_quick_feedback();
    #endif

    g29_verbose_level = code_seen('V') ? code_value_int() : 0;
    if (!WITHIN(g29_verbose_level, 0, 4)) {
      SERIAL_PROTOCOLLNPGM("Invalid Verbose Level specified. (0-4)\n");
      return UBL_ERR;
    }

    x_flag = code_seen('X') && code_has_value();
    x_pos = x_flag ? code_value_float() : current_position[X_AXIS];
    if (!WITHIN(RAW_X_POSITION(x_pos), X_MIN_POS, X_MAX_POS)) {
      SERIAL_PROTOCOLLNPGM("Invalid X location specified.\n");
      return UBL_ERR;
    }

    y_flag = code_seen('Y') && code_has_value();
    y_pos = y_flag ? code_value_float() : current_position[Y_AXIS];
    if (!WITHIN(RAW_Y_POSITION(y_pos), Y_MIN_POS, Y_MAX_POS)) {
      SERIAL_PROTOCOLLNPGM("Invalid Y location specified.\n");
      return UBL_ERR;
    }

    if (x_flag != y_flag) {
      SERIAL_PROTOCOLLNPGM("Both X & Y locations must be specified.\n");
      return UBL_ERR;
    }

    if (code_seen('A')) {     // Activate the Unified Bed Leveling System
      ubl.state.active = 1;
      SERIAL_PROTOCOLLNPGM("Unified Bed Leveling System activated.\n");
      ubl.store_state();
    }

    c_flag = code_seen('C') && code_has_value();
    ubl_constant = c_flag ? code_value_float() : 0.0;

    if (code_seen('D')) {     // Disable the Unified Bed Leveling System
      ubl.state.active = 0;
      SERIAL_PROTOCOLLNPGM("Unified Bed Leveling System de-activated.\n");
      ubl.store_state();
    }

    #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
      if (code_seen('F') && code_has_value()) {
        const float fh = code_value_float();
        if (!WITHIN(fh, 0.0, 100.0)) {
          SERIAL_PROTOCOLLNPGM("?Bed Level Correction Fade Height Not Plausible.\n");
          return UBL_ERR;
        }
        ubl.state.g29_correction_fade_height = fh;
        ubl.state.g29_fade_height_multiplier = 1.0 / fh;
      }
    #endif

    repeat_flag = code_seen('R');
    repetition_cnt = repeat_flag ? (code_has_value() ? code_value_int() : 9999) : 1;
    if (repetition_cnt < 1) {
      SERIAL_PROTOCOLLNPGM("Invalid Repetition count.\n");
      return UBL_ERR;
    }

    map_type = code_seen('O') && code_has_value() ? code_value_int() : 0;
    if (!WITHIN(map_type, 0, 1)) {
      SERIAL_PROTOCOLLNPGM("Invalid map type.\n");
      return UBL_ERR;
    }

    /*
    if (code_seen('M')) {     // Check if a map type was specified
      map_type = code_has_value() ? code_value_int() : 0;
      if (!WITHIN(map_type, 0, 1)) {
        SERIAL_PROTOCOLLNPGM("Invalid map type.\n");
        return UBL_ERR;
      }
    }
    //*/

    return UBL_OK;
  }

  /**
   * This function goes away after G29 debug is complete. But for right now, it is a handy
   * routine to dump binary data structures.
   */
  void dump(char * const str, const float &f) {
    char *ptr;

    SERIAL_PROTOCOL(str);
    SERIAL_PROTOCOL_F(f, 8);
    SERIAL_PROTOCOLPGM("  ");
    ptr = (char*)&f;
    for (uint8_t i = 0; i < 4; i++)
      SERIAL_PROTOCOLPAIR("  ", hex_byte(*ptr++));
    SERIAL_PROTOCOLPAIR("  isnan()=", isnan(f));
    SERIAL_PROTOCOLPAIR("  isinf()=", isinf(f));

    if (f == -INFINITY)
      SERIAL_PROTOCOLPGM("  Minus Infinity detected.");

    SERIAL_EOL;
  }

  static int ubl_state_at_invocation = 0,
             ubl_state_recursion_chk = 0;

  void save_ubl_active_state_and_disable() {
    ubl_state_recursion_chk++;
    if (ubl_state_recursion_chk != 1) {
      SERIAL_ECHOLNPGM("save_ubl_active_state_and_disabled() called multiple times in a row.");
      lcd_setstatuspgm("save_UBL_active() error");
      lcd_quick_feedback();
      return;
    }
    ubl_state_at_invocation = ubl.state.active;
    ubl.state.active = 0;
  }

  void restore_ubl_active_state_and_leave() {
    if (--ubl_state_recursion_chk) {
      SERIAL_ECHOLNPGM("restore_ubl_active_state_and_leave() called too many times.");
      lcd_setstatuspgm("restore_UBL_active() error");
      lcd_quick_feedback();
      return;
    }
    ubl.state.active = ubl_state_at_invocation;
  }


  /**
   * Much of the 'What?' command can be eliminated. But until we are fully debugged, it is
   * good to have the extra information. Soon... we prune this to just a few items
   */
  void g29_what_command() {
    const uint16_t k = E2END - ubl.eeprom_start;

    SERIAL_PROTOCOLPGM("Unified Bed Leveling System Version " UBL_VERSION " ");
    if (ubl.state.active)
      SERIAL_PROTOCOLCHAR('A');
    else
      SERIAL_PROTOCOLPGM("In");
    SERIAL_PROTOCOLLNPGM("ctive.\n");
    safe_delay(50);

    if (ubl.state.eeprom_storage_slot == -1)
      SERIAL_PROTOCOLPGM("No Mesh Loaded.");
    else {
      SERIAL_PROTOCOLPAIR("Mesh ", ubl.state.eeprom_storage_slot);
      SERIAL_PROTOCOLPGM(" Loaded.");
    }
    SERIAL_EOL;
    safe_delay(50);

    #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
      SERIAL_PROTOCOLLNPAIR("g29_correction_fade_height : ", ubl.state.g29_correction_fade_height);
    #endif

    SERIAL_PROTOCOLPGM("z_offset: ");
    SERIAL_PROTOCOL_F(ubl.state.z_offset, 6);
    SERIAL_EOL;
    safe_delay(50);

    SERIAL_PROTOCOLPGM("X-Axis Mesh Points at: ");
    for (uint8_t i = 0; i < UBL_MESH_NUM_X_POINTS; i++) {
      SERIAL_PROTOCOL_F(LOGICAL_X_POSITION(ubl.mesh_index_to_xpos[i]), 1);
      SERIAL_PROTOCOLPGM("  ");
      safe_delay(50);
    }
    SERIAL_EOL;

    SERIAL_PROTOCOLPGM("Y-Axis Mesh Points at: ");
    for (uint8_t i = 0; i < UBL_MESH_NUM_Y_POINTS; i++) {
      SERIAL_PROTOCOL_F(LOGICAL_Y_POSITION(ubl.mesh_index_to_ypos[i]), 1);
      SERIAL_PROTOCOLPGM("  ");
      safe_delay(50);
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
    SERIAL_PROTOCOLLNPAIR("Free EEPROM space starts at: 0x", hex_word(ubl.eeprom_start));

    SERIAL_PROTOCOLLNPAIR("end of EEPROM              : 0x", hex_word(E2END));
    safe_delay(50);

    SERIAL_PROTOCOLLNPAIR("sizeof(ubl) :  ", (int)sizeof(ubl));
    SERIAL_EOL;
    SERIAL_PROTOCOLLNPAIR("z_value[][] size: ", (int)sizeof(ubl.z_values));
    SERIAL_EOL;
    safe_delay(50);

    SERIAL_PROTOCOLLNPAIR("EEPROM free for UBL: 0x", hex_word(k));
    safe_delay(50);

    SERIAL_PROTOCOLPAIR("EEPROM can hold ", k / sizeof(ubl.z_values));
    SERIAL_PROTOCOLLNPGM(" meshes.\n");
    safe_delay(50);

    SERIAL_PROTOCOLPAIR("sizeof(ubl.state) : ", (int)sizeof(ubl.state));

    SERIAL_PROTOCOLPAIR("\nUBL_MESH_NUM_X_POINTS  ", UBL_MESH_NUM_X_POINTS);
    SERIAL_PROTOCOLPAIR("\nUBL_MESH_NUM_Y_POINTS  ", UBL_MESH_NUM_Y_POINTS);
    safe_delay(50);
    SERIAL_PROTOCOLPAIR("\nUBL_MESH_MIN_X         ", UBL_MESH_MIN_X);
    SERIAL_PROTOCOLPAIR("\nUBL_MESH_MIN_Y         ", UBL_MESH_MIN_Y);
    safe_delay(50);
    SERIAL_PROTOCOLPAIR("\nUBL_MESH_MAX_X         ", UBL_MESH_MAX_X);
    SERIAL_PROTOCOLPAIR("\nUBL_MESH_MAX_Y         ", UBL_MESH_MAX_Y);
    safe_delay(50);
    SERIAL_PROTOCOLPGM("\nMESH_X_DIST        ");
    SERIAL_PROTOCOL_F(MESH_X_DIST, 6);
    SERIAL_PROTOCOLPGM("\nMESH_Y_DIST        ");
    SERIAL_PROTOCOL_F(MESH_Y_DIST, 6);
    SERIAL_EOL;
    safe_delay(50);

    if (!ubl.sanity_check())
      SERIAL_PROTOCOLLNPGM("Unified Bed Leveling sanity checks passed.");
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
    float tmp_z_values[UBL_MESH_NUM_X_POINTS][UBL_MESH_NUM_Y_POINTS];

    if (!code_has_value()) {
      SERIAL_PROTOCOLLNPGM("?Mesh # required.\n");
      return;
    }
    storage_slot = code_value_int();

    int16_t j = (UBL_LAST_EEPROM_INDEX - ubl.eeprom_start) / sizeof(tmp_z_values);

    if (!WITHIN(storage_slot, 0, j - 1) || ubl.eeprom_start <= 0) {
      SERIAL_PROTOCOLLNPGM("?EEPROM storage not available for use.\n");
      return;
    }

    j = UBL_LAST_EEPROM_INDEX - (storage_slot + 1) * sizeof(tmp_z_values);
    eeprom_read_block((void *)&tmp_z_values, (void *)j, sizeof(tmp_z_values));

    SERIAL_ECHOPAIR("Subtracting Mesh ", storage_slot);
    SERIAL_PROTOCOLLNPAIR(" loaded from EEPROM address 0x", hex_word(j)); // Soon, we can remove the extra clutter of printing
                                                                        // the address in the EEPROM where the Mesh is stored.

    for (uint8_t x = 0; x < UBL_MESH_NUM_X_POINTS; x++)
      for (uint8_t y = 0; y < UBL_MESH_NUM_Y_POINTS; y++)
        ubl.z_values[x][y] -= tmp_z_values[x][y];
  }

  mesh_index_pair find_closest_mesh_point_of_type(const MeshPointType type, const float &lx, const float &ly, const bool probe_as_reference, unsigned int bits[16], bool far_flag) {
    float distance, closest = far_flag ? -99999.99 : 99999.99;
    mesh_index_pair return_val;

    return_val.x_index = return_val.y_index = -1;

    const float current_x = current_position[X_AXIS],
                current_y = current_position[Y_AXIS];

    // Get our reference position. Either the nozzle or probe location.
    const float px = lx - (probe_as_reference ? X_PROBE_OFFSET_FROM_EXTRUDER : 0),
                py = ly - (probe_as_reference ? Y_PROBE_OFFSET_FROM_EXTRUDER : 0);

    for (uint8_t i = 0; i < UBL_MESH_NUM_X_POINTS; i++) {
      for (uint8_t j = 0; j < UBL_MESH_NUM_Y_POINTS; j++) {

        if ( (type == INVALID && isnan(ubl.z_values[i][j]))  // Check to see if this location holds the right thing
          || (type == REAL && !isnan(ubl.z_values[i][j]))
          || (type == SET_IN_BITMAP && is_bit_set(bits, i, j))
        ) {

          // We only get here if we found a Mesh Point of the specified type

          const float rawx = ubl.mesh_index_to_xpos[i], // Check if we can probe this mesh location
                      rawy = ubl.mesh_index_to_ypos[j];

          // If using the probe as the reference there are some unreachable locations.
          // Prune them from the list and ignore them till the next Phase (manual nozzle probing).

          if (probe_as_reference &&
            (!WITHIN(rawx, MIN_PROBE_X, MAX_PROBE_X) || !WITHIN(rawy, MIN_PROBE_Y, MAX_PROBE_Y))
          ) continue;

          // Unreachable. Check if it's the closest location to the nozzle.
          // Add in a weighting factor that considers the current location of the nozzle.

          const float mx = LOGICAL_X_POSITION(rawx), // Check if we can probe this mesh location
                      my = LOGICAL_Y_POSITION(rawy);

          distance = HYPOT(px - mx, py - my) + HYPOT(current_x - mx, current_y - my) * 0.1;

          if (far_flag) {                                           // If doing the far_flag action, we want to be as far as possible
            for (uint8_t k = 0; k < UBL_MESH_NUM_X_POINTS; k++) {   // from the starting point and from any other probed points.  We
              for (uint8_t l = 0; l < UBL_MESH_NUM_Y_POINTS; l++) { // want the next point spread out and filling in any blank spaces
                if (!isnan(ubl.z_values[k][l])) {                       // in the mesh. So we add in some of the distance to every probed
                  distance += sq(i - k) * (MESH_X_DIST) * .05       // point we can find.
                            + sq(j - l) * (MESH_Y_DIST) * .05;
                }
              }
            }
          }

          if (far_flag == (distance > closest) && distance != closest) {  // if far_flag, look for farthest point
            closest = distance;       // We found a closer/farther location with
            return_val.x_index = i;   // the specified type of mesh value.
            return_val.y_index = j;
            return_val.distance = closest;
          }
        }
      } // for j
    } // for i

    return return_val;
  }

  void fine_tune_mesh(const float &lx, const float &ly, const bool do_ubl_mesh_map) {
    mesh_index_pair location;
    uint16_t not_done[16];
    int32_t round_off;

    save_ubl_active_state_and_disable();
    memset(not_done, 0xFF, sizeof(not_done));

    #if ENABLED(ULTRA_LCD)
      lcd_setstatuspgm("Fine Tuning Mesh");
    #endif

    do_blocking_move_to_z(Z_CLEARANCE_DEPLOY_PROBE);
    do_blocking_move_to_xy(lx, ly);
    do {
      if (do_ubl_mesh_map) ubl.display_map(map_type);

      location = find_closest_mesh_point_of_type( SET_IN_BITMAP, lx,  ly, 0, not_done, false); // The '0' says we want to use the nozzle's position
                                                                                              // It doesn't matter if the probe can not reach this
                                                                                              // location. This is a manual edit of the Mesh Point.
      if (location.x_index < 0 && location.y_index < 0) continue; // abort if we can't find any more points.

      bit_clear(not_done, location.x_index, location.y_index);  // Mark this location as 'adjusted' so we will find a
                                                                // different location the next time through the loop

      const float rawx = ubl.mesh_index_to_xpos[location.x_index],
                  rawy = ubl.mesh_index_to_ypos[location.y_index];

      // TODO: Change to use `position_is_reachable` (for SCARA-compatibility)
      if (!WITHIN(rawx, X_MIN_POS, X_MAX_POS) || !WITHIN(rawy, Y_MIN_POS, Y_MAX_POS)) { // In theory, we don't need this check.
        SERIAL_ERROR_START;
        SERIAL_ERRORLNPGM("Attempt to edit off the bed."); // This really can't happen, but do the check for now
        ubl.has_control_of_lcd_panel = false;
        goto FINE_TUNE_EXIT;
      }

      do_blocking_move_to_z(Z_CLEARANCE_DEPLOY_PROBE);    // Move the nozzle to where we are going to edit
      do_blocking_move_to_xy(LOGICAL_X_POSITION(rawx), LOGICAL_Y_POSITION(rawy));

      float new_z = ubl.z_values[location.x_index][location.y_index];

      round_off = (int32_t)(new_z * 1000.0);    // we chop off the last digits just to be clean. We are rounding to the
      new_z = float(round_off) / 1000.0;

      KEEPALIVE_STATE(PAUSED_FOR_USER);
      ubl.has_control_of_lcd_panel = true;

      lcd_implementation_clear();
      lcd_mesh_edit_setup(new_z);

      do {
        new_z = lcd_mesh_edit();
        idle();
      } while (!ubl_lcd_clicked());

      lcd_return_to_status();

      ubl.has_control_of_lcd_panel = true; // There is a race condition for the Encoder Wheel getting clicked.
                                           // It could get detected in lcd_mesh_edit (actually _lcd_mesh_fine_tune)
                                           // or here.

      const millis_t nxt = millis() + 1500UL;
      while (ubl_lcd_clicked()) { // debounce and watch for abort
        idle();
        if (ELAPSED(millis(), nxt)) {
          lcd_return_to_status();
          //SERIAL_PROTOCOLLNPGM("\nFine Tuning of Mesh Stopped.");
          do_blocking_move_to_z(Z_CLEARANCE_DEPLOY_PROBE);
          lcd_setstatuspgm("Mesh Editing Stopped");

          while (ubl_lcd_clicked()) idle();

          goto FINE_TUNE_EXIT;
        }
      }

      safe_delay(20);                       // We don't want any switch noise.

      ubl.z_values[location.x_index][location.y_index] = new_z;

      lcd_implementation_clear();

    } while (location.x_index >= 0 && location.y_index >= 0 && --repetition_cnt);

    FINE_TUNE_EXIT:

    ubl.has_control_of_lcd_panel = false;
    KEEPALIVE_STATE(IN_HANDLER);

    if (do_ubl_mesh_map) ubl.display_map(map_type);
    restore_ubl_active_state_and_leave();
    do_blocking_move_to_z(Z_CLEARANCE_DEPLOY_PROBE);

    do_blocking_move_to_xy(lx, ly);

    #if ENABLED(ULTRA_LCD)
      lcd_setstatuspgm("Done Editing Mesh");
    #endif
    SERIAL_ECHOLNPGM("Done Editing Mesh");
  }

#endif // AUTO_BED_LEVELING_UBL