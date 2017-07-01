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
 * Marlin Firmware -- G26 - Mesh Validation Tool
 */

#include "MarlinConfig.h"

#if ENABLED(AUTO_BED_LEVELING_UBL) && ENABLED(UBL_G26_MESH_VALIDATION)

  #include "ubl.h"
  #include "Marlin.h"
  #include "planner.h"
  #include "stepper.h"
  #include "temperature.h"
  #include "ultralcd.h"
  #include "gcode.h"

  #define EXTRUSION_MULTIPLIER 1.0
  #define RETRACTION_MULTIPLIER 1.0
  #define NOZZLE 0.4
  #define FILAMENT 1.75
  #define LAYER_HEIGHT 0.2
  #define PRIME_LENGTH 10.0
  #define BED_TEMP 60.0
  #define HOTEND_TEMP 205.0
  #define OOZE_AMOUNT 0.3

  #define SIZE_OF_INTERSECTION_CIRCLES 5
  #define SIZE_OF_CROSSHAIRS 3

  #if SIZE_OF_CROSSHAIRS >= SIZE_OF_INTERSECTION_CIRCLES
    #error "SIZE_OF_CROSSHAIRS must be less than SIZE_OF_INTERSECTION_CIRCLES."
  #endif

  /**
   *   G26 Mesh Validation Tool
   *
   *   G26 is a Mesh Validation Tool intended to provide support for the Marlin Unified Bed Leveling System.
   *   In order to fully utilize and benefit from the Marlin Unified Bed Leveling System an accurate Mesh must
   *   be defined.  G29 is designed to allow the user to quickly validate the correctness of her Mesh.  It will
   *   first heat the bed and nozzle. It will then print lines and circles along the Mesh Cell boundaries and
   *   the intersections of those lines (respectively).
   *
   *   This action allows the user to immediately see where the Mesh is properly defined and where it needs to
   *   be edited.  The command will generate the Mesh lines closest to the nozzle's starting position.  Alternatively
   *   the user can specify the X and Y position of interest with command parameters.  This allows the user to
   *   focus on a particular area of the Mesh where attention is needed.
   *
   *   B #  Bed         Set the Bed Temperature.  If not specified, a default of 60 C. will be assumed.
   *
   *   C    Current     When searching for Mesh Intersection points to draw, use the current nozzle location
   *                    as the base for any distance comparison.
   *
   *   D    Disable     Disable the Unified Bed Leveling System.  In the normal case the user is invoking this
   *                    command to see how well a Mesh as been adjusted to match a print surface.  In order to do
   *                    this the Unified Bed Leveling System is turned on by the G26 command.  The D parameter
   *                    alters the command's normal behaviour and disables the Unified Bed Leveling System even if
   *                    it is on.
   *
   *   H #  Hotend      Set the Nozzle Temperature.  If not specified, a default of 205 C. will be assumed.
   *
   *   F #  Filament    Used to specify the diameter of the filament being used.  If not specified
   *                    1.75mm filament is assumed.  If you are not getting acceptable results by using the
   *                    'correct' numbers, you can scale this number up or down a little bit to change the amount
   *                    of filament that is being extruded during the printing of the various lines on the bed.
   *
   *   K    Keep-On     Keep the heaters turned on at the end of the command.
   *
   *   L #  Layer       Layer height.  (Height of nozzle above bed)  If not specified .20mm will be used.
   *
   *   O #  Ooooze      How much your nozzle will Ooooze filament while getting in position to print.  This
   *                    is over kill, but using this parameter will let you get the very first 'circle' perfect
   *                    so you have a trophy to peel off of the bed and hang up to show how perfectly you have your
   *                    Mesh calibrated.  If not specified, a filament length of .3mm is assumed.
   *
   *   P #  Prime       Prime the nozzle with specified length of filament.  If this parameter is not
   *                    given, no prime action will take place.  If the parameter specifies an amount, that much
   *                    will be purged before continuing.  If no amount is specified the command will start
   *                    purging filament until the user provides an LCD Click and then it will continue with
   *                    printing the Mesh.  You can carefully remove the spent filament with a needle nose
   *                    pliers while holding the LCD Click wheel in a depressed state.  If you do not have
   *                    an LCD, you must specify a value if you use P.
   *
   *   Q #  Multiplier  Retraction Multiplier.  Normally not needed.  Retraction defaults to 1.0mm and
   *                    un-retraction is at 1.2mm   These numbers will be scaled by the specified amount
   *
   *   R #  Repeat      Prints the number of patterns given as a parameter, starting at the current location.
   *                    If a parameter isn't given, every point will be printed unless G26 is interrupted.
   *                    This works the same way that the UBL G29 P4 R parameter works.
   *
   *                    NOTE:  If you do not have an LCD, you -must- specify R.  This is to ensure that you are
   *                    aware that there's some risk associated with printing without the ability to abort in
   *                    cases where mesh point Z value may be inaccurate.  As above, if you do not include a
   *                    parameter, every point will be printed.
   *
   *   S #  Nozzle      Used to control the size of nozzle diameter.  If not specified, a .4mm nozzle is assumed.
   *
   *   U #  Random      Randomize the order that the circles are drawn on the bed.  The search for the closest
   *                    undrawn cicle is still done.  But the distance to the location for each circle has a
   *                    random number of the size specified added to it.  Specifying S50 will give an interesting
   *                    deviation from the normal behaviour on a 10 x 10 Mesh.
   *
   *   X #  X Coord.    Specify the starting location of the drawing activity.
   *
   *   Y #  Y Coord.    Specify the starting location of the drawing activity.
   */

  // External references

  extern float feedrate_mm_s; // must set before calling prepare_move_to_destination
  extern Planner planner;
  #if ENABLED(ULTRA_LCD)
    extern char lcd_status_message[];
  #endif
  extern float destination[XYZE];
  void set_destination_to_current();
  void prepare_move_to_destination();
  #if AVR_AT90USB1286_FAMILY  // Teensyduino & Printrboard IDE extensions have compile errors without this
    inline void sync_plan_position_e() { planner.set_e_position_mm(current_position[E_AXIS]); }
    inline void set_current_to_destination() { COPY(current_position, destination); }
  #else
    void sync_plan_position_e();
    void set_current_to_destination();
  #endif
  #if ENABLED(NEWPANEL)
    void lcd_setstatusPGM(const char* const message, const int8_t level);
    void chirp_at_user();
  #endif

  // Private functions

  static uint16_t circle_flags[16], horizontal_mesh_line_flags[16], vertical_mesh_line_flags[16];
  float g26_e_axis_feedrate = 0.020,
        random_deviation = 0.0;

  static bool g26_retracted = false; // Track the retracted state of the nozzle so mismatched
                                     // retracts/recovers won't result in a bad state.

  float valid_trig_angle(float);

  float unified_bed_leveling::g26_extrusion_multiplier,
        unified_bed_leveling::g26_retraction_multiplier,
        unified_bed_leveling::g26_nozzle,
        unified_bed_leveling::g26_filament_diameter,
        unified_bed_leveling::g26_layer_height,
        unified_bed_leveling::g26_prime_length,
        unified_bed_leveling::g26_x_pos,
        unified_bed_leveling::g26_y_pos,
        unified_bed_leveling::g26_ooze_amount;

  int16_t unified_bed_leveling::g26_bed_temp,
          unified_bed_leveling::g26_hotend_temp;

  int8_t unified_bed_leveling::g26_prime_flag;

  bool unified_bed_leveling::g26_continue_with_closest,
       unified_bed_leveling::g26_keep_heaters_on;

  int16_t unified_bed_leveling::g26_repeats;

  void unified_bed_leveling::G26_line_to_destination(const float &feed_rate) {
    const float save_feedrate = feedrate_mm_s;
    feedrate_mm_s = feed_rate;      // use specified feed rate
    prepare_move_to_destination();  // will ultimately call ubl.line_to_destination_cartesian or ubl.prepare_linear_move_to for UBL_DELTA
    feedrate_mm_s = save_feedrate;  // restore global feed rate
  }

  #if ENABLED(NEWPANEL)
    /**
     * Detect ubl_lcd_clicked, debounce it, and return true for cancel
     */
    bool user_canceled() {
      if (!ubl_lcd_clicked()) return false;
      safe_delay(10);                       // Wait for click to settle

      #if ENABLED(ULTRA_LCD)
        lcd_setstatusPGM(PSTR("Mesh Validation Stopped."), 99);
        lcd_quick_feedback();
      #endif

      while (!ubl_lcd_clicked()) idle();    // Wait for button release

      // If the button is suddenly pressed again,
      // ask the user to resolve the issue
      lcd_setstatusPGM(PSTR("Release button"), 99); // will never appear...
      while (ubl_lcd_clicked()) idle();             // unless this loop happens
      lcd_reset_status();

      return true;
    }
  #endif

  /**
   * G26: Mesh Validation Pattern generation.
   *
   * Used to interactively edit UBL's Mesh by placing the
   * nozzle in a problem area and doing a G29 P4 R command.
   */
  void unified_bed_leveling::G26() {
    SERIAL_ECHOLNPGM("G26 command started.  Waiting for heater(s).");
    float tmp, start_angle, end_angle;
    int   i, xi, yi;
    mesh_index_pair location;

    // Don't allow Mesh Validation without homing first,
    // or if the parameter parsing did not go OK, abort
    if (axis_unhomed_error() || parse_G26_parameters()) return;

    if (current_position[Z_AXIS] < Z_CLEARANCE_BETWEEN_PROBES) {
      do_blocking_move_to_z(Z_CLEARANCE_BETWEEN_PROBES);
      stepper.synchronize();
      set_current_to_destination();
    }

    if (turn_on_heaters()) goto LEAVE;

    current_position[E_AXIS] = 0.0;
    sync_plan_position_e();

    if (g26_prime_flag && prime_nozzle()) goto LEAVE;

    /**
     *  Bed is preheated
     *
     *  Nozzle is at temperature
     *
     *  Filament is primed!
     *
     *  It's  "Show Time" !!!
     */

    ZERO(circle_flags);
    ZERO(horizontal_mesh_line_flags);
    ZERO(vertical_mesh_line_flags);

    // Move nozzle to the specified height for the first layer
    set_destination_to_current();
    destination[Z_AXIS] = g26_layer_height;
    move_to(destination, 0.0);
    move_to(destination, g26_ooze_amount);

    has_control_of_lcd_panel = true;
    //debug_current_and_destination(PSTR("Starting G26 Mesh Validation Pattern."));

    /**
     * Declare and generate a sin() & cos() table to be used during the circle drawing.  This will lighten
     * the CPU load and make the arc drawing faster and more smooth
     */
    float sin_table[360 / 30 + 1], cos_table[360 / 30 + 1];
    for (i = 0; i <= 360 / 30; i++) {
      cos_table[i] = SIZE_OF_INTERSECTION_CIRCLES * cos(RADIANS(valid_trig_angle(i * 30.0)));
      sin_table[i] = SIZE_OF_INTERSECTION_CIRCLES * sin(RADIANS(valid_trig_angle(i * 30.0)));
    }

    do {
      location = g26_continue_with_closest
        ? find_closest_circle_to_print(current_position[X_AXIS], current_position[Y_AXIS])
        : find_closest_circle_to_print(g26_x_pos, g26_y_pos); // Find the closest Mesh Intersection to where we are now.

      if (location.x_index >= 0 && location.y_index >= 0) {
        const float circle_x = mesh_index_to_xpos(location.x_index),
                    circle_y = mesh_index_to_ypos(location.y_index);

        // If this mesh location is outside the printable_radius, skip it.

        if (!position_is_reachable_raw_xy(circle_x, circle_y)) continue;

        xi = location.x_index;  // Just to shrink the next few lines and make them easier to understand
        yi = location.y_index;

        if (g26_debug_flag) {
          SERIAL_ECHOPAIR("   Doing circle at: (xi=", xi);
          SERIAL_ECHOPAIR(", yi=", yi);
          SERIAL_CHAR(')');
          SERIAL_EOL();
        }

        start_angle = 0.0;    // assume it is going to be a full circle
        end_angle   = 360.0;
        if (xi == 0) {       // Check for bottom edge
          start_angle = -90.0;
          end_angle   =  90.0;
          if (yi == 0)        // it is an edge, check for the two left corners
            start_angle = 0.0;
          else if (yi == GRID_MAX_POINTS_Y - 1)
            end_angle = 0.0;
        }
        else if (xi == GRID_MAX_POINTS_X - 1) { // Check for top edge
          start_angle =  90.0;
          end_angle   = 270.0;
          if (yi == 0)                  // it is an edge, check for the two right corners
            end_angle = 180.0;
          else if (yi == GRID_MAX_POINTS_Y - 1)
            start_angle = 180.0;
        }
        else if (yi == 0) {
          start_angle =   0.0;         // only do the top   side of the cirlce
          end_angle   = 180.0;
        }
        else if (yi == GRID_MAX_POINTS_Y - 1) {
          start_angle = 180.0;         // only do the bottom side of the cirlce
          end_angle   = 360.0;
        }

        for (tmp = start_angle; tmp < end_angle - 0.1; tmp += 30.0) {

          #if ENABLED(NEWPANEL)
            if (user_canceled()) goto LEAVE;              // Check if the user wants to stop the Mesh Validation
          #endif

          int tmp_div_30 = tmp / 30.0;
          if (tmp_div_30 < 0) tmp_div_30 += 360 / 30;
          if (tmp_div_30 > 11) tmp_div_30 -= 360 / 30;

          float x = circle_x + cos_table[tmp_div_30],    // for speed, these are now a lookup table entry
                y = circle_y + sin_table[tmp_div_30],
                xe = circle_x + cos_table[tmp_div_30 + 1],
                ye = circle_y + sin_table[tmp_div_30 + 1];
          #if IS_KINEMATIC
            // Check to make sure this segment is entirely on the bed, skip if not.
            if (!position_is_reachable_raw_xy(x, y) || !position_is_reachable_raw_xy(xe, ye)) continue;
          #else                                              // not, we need to skip
            x  = constrain(x, X_MIN_POS + 1, X_MAX_POS - 1); // This keeps us from bumping the endstops
            y  = constrain(y, Y_MIN_POS + 1, Y_MAX_POS - 1);
            xe = constrain(xe, X_MIN_POS + 1, X_MAX_POS - 1);
            ye = constrain(ye, Y_MIN_POS + 1, Y_MAX_POS - 1);
          #endif

          //if (g26_debug_flag) {
          //  char ccc, *cptr, seg_msg[50], seg_num[10];
          //  strcpy(seg_msg, "   segment: ");
          //  strcpy(seg_num, "    \n");
          //  cptr = (char*) "01234567890ABCDEF????????";
          //  ccc = cptr[tmp_div_30];
          //  seg_num[1] = ccc;
          //  strcat(seg_msg, seg_num);
          //  debug_current_and_destination(seg_msg);
          //}

          print_line_from_here_to_there(LOGICAL_X_POSITION(x), LOGICAL_Y_POSITION(y), g26_layer_height, LOGICAL_X_POSITION(xe), LOGICAL_Y_POSITION(ye), g26_layer_height);

        }
        if (look_for_lines_to_connect())
          goto LEAVE;
      }
    } while (--g26_repeats && location.x_index >= 0 && location.y_index >= 0);

    LEAVE:
    lcd_setstatusPGM(PSTR("Leaving G26"), -1);

    retract_filament(destination);
    destination[Z_AXIS] = Z_CLEARANCE_BETWEEN_PROBES;

    //debug_current_and_destination(PSTR("ready to do Z-Raise."));
    move_to(destination, 0); // Raise the nozzle
    //debug_current_and_destination(PSTR("done doing Z-Raise."));

    destination[X_AXIS] = g26_x_pos;                                               // Move back to the starting position
    destination[Y_AXIS] = g26_y_pos;
    //destination[Z_AXIS] = Z_CLEARANCE_BETWEEN_PROBES;                        // Keep the nozzle where it is

    move_to(destination, 0); // Move back to the starting position
    //debug_current_and_destination(PSTR("done doing X/Y move."));

    has_control_of_lcd_panel = false;     // Give back control of the LCD Panel!

    if (!g26_keep_heaters_on) {
      #if HAS_TEMP_BED
        thermalManager.setTargetBed(0);
      #endif
      thermalManager.setTargetHotend(0, 0);
    }
  }

  float valid_trig_angle(float d) {
    while (d > 360.0) d -= 360.0;
    while (d < 0.0) d += 360.0;
    return d;
  }

  mesh_index_pair unified_bed_leveling::find_closest_circle_to_print(const float &X, const float &Y) {
    float closest = 99999.99;
    mesh_index_pair return_val;

    return_val.x_index = return_val.y_index = -1;

    for (uint8_t i = 0; i < GRID_MAX_POINTS_X; i++) {
      for (uint8_t j = 0; j < GRID_MAX_POINTS_Y; j++) {
        if (!is_bit_set(circle_flags, i, j)) {
          const float mx = mesh_index_to_xpos(i),  // We found a circle that needs to be printed
                      my = mesh_index_to_ypos(j);

          // Get the distance to this intersection
          float f = HYPOT(X - mx, Y - my);

          // It is possible that we are being called with the values
          // to let us find the closest circle to the start position.
          // But if this is not the case, add a small weighting to the
          // distance calculation to help it choose a better place to continue.
          f += HYPOT(g26_x_pos - mx, g26_y_pos - my) / 15.0;

          // Add in the specified amount of Random Noise to our search
          if (random_deviation > 1.0)
            f += random(0.0, random_deviation);

          if (f < closest) {
            closest = f;              // We found a closer location that is still
            return_val.x_index = i;   // un-printed  --- save the data for it
            return_val.y_index = j;
            return_val.distance = closest;
          }
        }
      }
    }
    bit_set(circle_flags, return_val.x_index, return_val.y_index);   // Mark this location as done.
    return return_val;
  }

  bool unified_bed_leveling::look_for_lines_to_connect() {
    float sx, sy, ex, ey;

    for (uint8_t i = 0; i < GRID_MAX_POINTS_X; i++) {
      for (uint8_t j = 0; j < GRID_MAX_POINTS_Y; j++) {

        #if ENABLED(NEWPANEL)
          if (user_canceled()) return true;     // Check if the user wants to stop the Mesh Validation
        #endif

        if (i < GRID_MAX_POINTS_X) { // We can't connect to anything to the right than GRID_MAX_POINTS_X.
                                     // This is already a half circle because we are at the edge of the bed.

          if (is_bit_set(circle_flags, i, j) && is_bit_set(circle_flags, i + 1, j)) { // check if we can do a line to the left
            if (!is_bit_set(horizontal_mesh_line_flags, i, j)) {

              //
              // We found two circles that need a horizontal line to connect them
              // Print it!
              //
              sx = mesh_index_to_xpos(  i  ) + (SIZE_OF_INTERSECTION_CIRCLES - (SIZE_OF_CROSSHAIRS)); // right edge
              ex = mesh_index_to_xpos(i + 1) - (SIZE_OF_INTERSECTION_CIRCLES - (SIZE_OF_CROSSHAIRS)); // left edge

              sx = constrain(sx, X_MIN_POS + 1, X_MAX_POS - 1);
              sy = ey = constrain(mesh_index_to_ypos(j), Y_MIN_POS + 1, Y_MAX_POS - 1);
              ex = constrain(ex, X_MIN_POS + 1, X_MAX_POS - 1);

              if (position_is_reachable_raw_xy(sx, sy) && position_is_reachable_raw_xy(ex, ey)) {

                if (g26_debug_flag) {
                  SERIAL_ECHOPAIR(" Connecting with horizontal line (sx=", sx);
                  SERIAL_ECHOPAIR(", sy=", sy);
                  SERIAL_ECHOPAIR(") -> (ex=", ex);
                  SERIAL_ECHOPAIR(", ey=", ey);
                  SERIAL_CHAR(')');
                  SERIAL_EOL();
                  //debug_current_and_destination(PSTR("Connecting horizontal line."));
                }

                print_line_from_here_to_there(LOGICAL_X_POSITION(sx), LOGICAL_Y_POSITION(sy), g26_layer_height, LOGICAL_X_POSITION(ex), LOGICAL_Y_POSITION(ey), g26_layer_height);
              }
              bit_set(horizontal_mesh_line_flags, i, j);   // Mark it as done so we don't do it again, even if we skipped it
            }
          }

          if (j < GRID_MAX_POINTS_Y) { // We can't connect to anything further back than GRID_MAX_POINTS_Y.
                                           // This is already a half circle because we are at the edge  of the bed.

            if (is_bit_set(circle_flags, i, j) && is_bit_set(circle_flags, i, j + 1)) { // check if we can do a line straight down
              if (!is_bit_set( vertical_mesh_line_flags, i, j)) {
                //
                // We found two circles that need a vertical line to connect them
                // Print it!
                //
                sy = mesh_index_to_ypos(  j  ) + (SIZE_OF_INTERSECTION_CIRCLES - (SIZE_OF_CROSSHAIRS)); // top edge
                ey = mesh_index_to_ypos(j + 1) - (SIZE_OF_INTERSECTION_CIRCLES - (SIZE_OF_CROSSHAIRS)); // bottom edge

                sx = ex = constrain(mesh_index_to_xpos(i), X_MIN_POS + 1, X_MAX_POS - 1);
                sy = constrain(sy, Y_MIN_POS + 1, Y_MAX_POS - 1);
                ey = constrain(ey, Y_MIN_POS + 1, Y_MAX_POS - 1);

                if (position_is_reachable_raw_xy(sx, sy) && position_is_reachable_raw_xy(ex, ey)) {

                  if (g26_debug_flag) {
                    SERIAL_ECHOPAIR(" Connecting with vertical line (sx=", sx);
                    SERIAL_ECHOPAIR(", sy=", sy);
                    SERIAL_ECHOPAIR(") -> (ex=", ex);
                    SERIAL_ECHOPAIR(", ey=", ey);
                    SERIAL_CHAR(')');
                    SERIAL_EOL();
                    debug_current_and_destination(PSTR("Connecting vertical line."));
                  }
                  print_line_from_here_to_there(LOGICAL_X_POSITION(sx), LOGICAL_Y_POSITION(sy), g26_layer_height, LOGICAL_X_POSITION(ex), LOGICAL_Y_POSITION(ey), g26_layer_height);
                }
                bit_set(vertical_mesh_line_flags, i, j);   // Mark it as done so we don't do it again, even if skipped
              }
            }
          }
        }
      }
    }
    return false;
  }

  void unified_bed_leveling::move_to(const float &x, const float &y, const float &z, const float &e_delta) {
    float feed_value;
    static float last_z = -999.99;

    bool has_xy_component = (x != current_position[X_AXIS] || y != current_position[Y_AXIS]); // Check if X or Y is involved in the movement.

    if (z != last_z) {
      last_z = z;
      feed_value = planner.max_feedrate_mm_s[Z_AXIS]/(3.0);  // Base the feed rate off of the configured Z_AXIS feed rate

      destination[X_AXIS] = current_position[X_AXIS];
      destination[Y_AXIS] = current_position[Y_AXIS];
      destination[Z_AXIS] = z;                          // We know the last_z==z or we wouldn't be in this block of code.
      destination[E_AXIS] = current_position[E_AXIS];

      G26_line_to_destination(feed_value);

      stepper.synchronize();
      set_destination_to_current();
    }

    // Check if X or Y is involved in the movement.
    // Yes: a 'normal' movement. No: a retract() or recover()
    feed_value = has_xy_component ? PLANNER_XY_FEEDRATE() / 10.0 : planner.max_feedrate_mm_s[E_AXIS] / 1.5;

    if (g26_debug_flag) SERIAL_ECHOLNPAIR("in move_to() feed_value for XY:", feed_value);

    destination[X_AXIS] = x;
    destination[Y_AXIS] = y;
    destination[E_AXIS] += e_delta;

    G26_line_to_destination(feed_value);

    stepper.synchronize();
    set_destination_to_current();

  }

  void unified_bed_leveling::retract_filament(const float where[XYZE]) {
    if (!g26_retracted) { // Only retract if we are not already retracted!
      g26_retracted = true;
      move_to(where, -1.0 * g26_retraction_multiplier);
    }
  }

  void unified_bed_leveling::recover_filament(const float where[XYZE]) {
    if (g26_retracted) { // Only un-retract if we are retracted.
      move_to(where, 1.2 * g26_retraction_multiplier);
      g26_retracted = false;
    }
  }

  /**
   * print_line_from_here_to_there() takes two cartesian coordinates and draws a line from one
   * to the other.  But there are really three sets of coordinates involved.  The first coordinate
   * is the present location of the nozzle.  We don't necessarily want to print from this location.
   * We first need to move the nozzle to the start of line segment where we want to print.  Once
   * there, we can use the two coordinates supplied to draw the line.
   *
   * Note:  Although we assume the first set of coordinates is the start of the line and the second
   * set of coordinates is the end of the line, it does not always work out that way.  This function
   * optimizes the movement to minimize the travel distance before it can start printing.  This saves
   * a lot of time and eleminates a lot of non-sensical movement of the nozzle.   However, it does
   * cause a lot of very little short retracement of th nozzle when it draws the very first line
   * segment of a 'circle'.   The time this requires is very short and is easily saved by the other
   * cases where the optimization comes into play.
   */
  void unified_bed_leveling::print_line_from_here_to_there(const float &sx, const float &sy, const float &sz, const float &ex, const float &ey, const float &ez) {
    const float dx_s = current_position[X_AXIS] - sx,   // find our distance from the start of the actual line segment
                dy_s = current_position[Y_AXIS] - sy,
                dist_start = HYPOT2(dx_s, dy_s),        // We don't need to do a sqrt(), we can compare the distance^2
                                                        // to save computation time
                dx_e = current_position[X_AXIS] - ex,   // find our distance from the end of the actual line segment
                dy_e = current_position[Y_AXIS] - ey,
                dist_end = HYPOT2(dx_e, dy_e),

                line_length = HYPOT(ex - sx, ey - sy);

    // If the end point of the line is closer to the nozzle, flip the direction,
    // moving from the end to the start. On very small lines the optimization isn't worth it.
    if (dist_end < dist_start && (SIZE_OF_INTERSECTION_CIRCLES) < FABS(line_length)) {
      return print_line_from_here_to_there(ex, ey, ez, sx, sy, sz);
    }

    // Decide whether to retract & bump

    if (dist_start > 2.0) {
      retract_filament(destination);
      //todo:  parameterize the bump height with a define
      move_to(current_position[X_AXIS], current_position[Y_AXIS], current_position[Z_AXIS] + 0.500, 0.0);  // Z bump to minimize scraping
      move_to(sx, sy, sz + 0.500, 0.0); // Get to the starting point with no extrusion while bumped
    }

    move_to(sx, sy, sz, 0.0); // Get to the starting point with no extrusion / un-Z bump

    const float e_pos_delta = line_length * g26_e_axis_feedrate * g26_extrusion_multiplier;

    recover_filament(destination);
    move_to(ex, ey, ez, e_pos_delta);  // Get to the ending point with an appropriate amount of extrusion
  }

  /**
   * This function used to be inline code in G26. But there are so many
   * parameters it made sense to turn them into static globals and get
   * this code out of sight of the main routine.
   */
  bool unified_bed_leveling::parse_G26_parameters() {

    g26_extrusion_multiplier  = EXTRUSION_MULTIPLIER;
    g26_retraction_multiplier = RETRACTION_MULTIPLIER;
    g26_nozzle                = NOZZLE;
    g26_filament_diameter     = FILAMENT;
    g26_layer_height          = LAYER_HEIGHT;
    g26_prime_length          = PRIME_LENGTH;
    g26_bed_temp              = BED_TEMP;
    g26_hotend_temp           = HOTEND_TEMP;
    g26_prime_flag            = 0;

    g26_ooze_amount           = parser.linearval('O', OOZE_AMOUNT);
    g26_keep_heaters_on       = parser.boolval('K');
    g26_continue_with_closest = parser.boolval('C');

    if (parser.seenval('B')) {
      g26_bed_temp = parser.value_celsius();
      if (!WITHIN(g26_bed_temp, 15, 140)) {
        SERIAL_PROTOCOLLNPGM("?Specified bed temperature not plausible.");
        return UBL_ERR;
      }
    }

    if (parser.seenval('L')) {
      g26_layer_height = parser.value_linear_units();
      if (!WITHIN(g26_layer_height, 0.0, 2.0)) {
        SERIAL_PROTOCOLLNPGM("?Specified layer height not plausible.");
        return UBL_ERR;
      }
    }

    if (parser.seen('Q')) {
      if (parser.has_value()) {
        g26_retraction_multiplier = parser.value_float();
        if (!WITHIN(g26_retraction_multiplier, 0.05, 15.0)) {
          SERIAL_PROTOCOLLNPGM("?Specified Retraction Multiplier not plausible.");
          return UBL_ERR;
        }
      }
      else {
        SERIAL_PROTOCOLLNPGM("?Retraction Multiplier must be specified.");
        return UBL_ERR;
      }
    }

    if (parser.seenval('S')) {
      g26_nozzle = parser.value_float();
      if (!WITHIN(g26_nozzle, 0.1, 1.0)) {
        SERIAL_PROTOCOLLNPGM("?Specified nozzle size not plausible.");
        return UBL_ERR;
      }
    }

    if (parser.seen('P')) {
      if (!parser.has_value()) {
        #if ENABLED(NEWPANEL)
          g26_prime_flag = -1;
        #else
          SERIAL_PROTOCOLLNPGM("?Prime length must be specified when not using an LCD.");
          return UBL_ERR;
        #endif
      }
      else {
        g26_prime_flag++;
        g26_prime_length = parser.value_linear_units();
        if (!WITHIN(g26_prime_length, 0.0, 25.0)) {
          SERIAL_PROTOCOLLNPGM("?Specified prime length not plausible.");
          return UBL_ERR;
        }
      }
    }

    if (parser.seenval('F')) {
      g26_filament_diameter = parser.value_linear_units();
      if (!WITHIN(g26_filament_diameter, 1.0, 4.0)) {
        SERIAL_PROTOCOLLNPGM("?Specified filament size not plausible.");
        return UBL_ERR;
      }
    }
    g26_extrusion_multiplier *= sq(1.75) / sq(g26_filament_diameter); // If we aren't using 1.75mm filament, we need to
                                                                      // scale up or down the length needed to get the
                                                                      // same volume of filament

    g26_extrusion_multiplier *= g26_filament_diameter * sq(g26_nozzle) / sq(0.3); // Scale up by nozzle size

    if (parser.seenval('H')) {
      g26_hotend_temp = parser.value_celsius();
      if (!WITHIN(g26_hotend_temp, 165, 280)) {
        SERIAL_PROTOCOLLNPGM("?Specified nozzle temperature not plausible.");
        return UBL_ERR;
      }
    }

    if (parser.seen('U')) {
      randomSeed(millis());
      // This setting will persist for the next G26
      random_deviation = parser.has_value() ? parser.value_float() : 50.0;
    }

    #if ENABLED(NEWPANEL)
      g26_repeats = parser.intval('R', GRID_MAX_POINTS + 1);
    #else
      if (!parser.seen('R')) {
        SERIAL_PROTOCOLLNPGM("?(R)epeat must be specified when not using an LCD.");
        return UBL_ERR;
      }
      else
        g26_repeats = parser.has_value() ? parser.value_int() : GRID_MAX_POINTS + 1;
    #endif
    if (g26_repeats < 1) {
      SERIAL_PROTOCOLLNPGM("?(R)epeat value not plausible; must be at least 1.");
      return UBL_ERR;
    }

    g26_x_pos = parser.linearval('X', current_position[X_AXIS]);
    g26_y_pos = parser.linearval('Y', current_position[Y_AXIS]);
    if (!position_is_reachable_xy(g26_x_pos, g26_y_pos)) {
      SERIAL_PROTOCOLLNPGM("?Specified X,Y coordinate out of bounds.");
      return UBL_ERR;
    }

    /**
     * Wait until all parameters are verified before altering the state!
     */
    set_bed_leveling_enabled(!parser.seen('D'));

    return UBL_OK;
  }

  #if ENABLED(NEWPANEL)
    bool unified_bed_leveling::exit_from_g26() {
      lcd_setstatusPGM(PSTR("Leaving G26"), -1);
      while (ubl_lcd_clicked()) idle();
      return UBL_ERR;
    }
  #endif

  /**
   * Turn on the bed and nozzle heat and
   * wait for them to get up to temperature.
   */
  bool unified_bed_leveling::turn_on_heaters() {
    millis_t next = millis() + 5000UL;
    #if HAS_TEMP_BED
      #if ENABLED(ULTRA_LCD)
        if (g26_bed_temp > 25) {
          lcd_setstatusPGM(PSTR("G26 Heating Bed."), 99);
          lcd_quick_feedback();
      #endif
          has_control_of_lcd_panel = true;
          thermalManager.setTargetBed(g26_bed_temp);
          while (abs(thermalManager.degBed() - g26_bed_temp) > 3) {

            #if ENABLED(NEWPANEL)
              if (ubl_lcd_clicked()) return exit_from_g26();
            #endif

            if (PENDING(millis(), next)) {
              next = millis() + 5000UL;
              print_heaterstates();
            }
            idle();
          }
      #if ENABLED(ULTRA_LCD)
        }
        lcd_setstatusPGM(PSTR("G26 Heating Nozzle."), 99);
        lcd_quick_feedback();
      #endif
    #endif

    // Start heating the nozzle and wait for it to reach temperature.
    thermalManager.setTargetHotend(g26_hotend_temp, 0);
    while (abs(thermalManager.degHotend(0) - g26_hotend_temp) > 3) {

      #if ENABLED(NEWPANEL)
        if (ubl_lcd_clicked()) return exit_from_g26();
      #endif

      if (PENDING(millis(), next)) {
        next = millis() + 5000UL;
        print_heaterstates();
      }
      idle();
    }

    #if ENABLED(ULTRA_LCD)
      lcd_reset_status();
      lcd_quick_feedback();
    #endif

    return UBL_OK;
  }

  /**
   * Prime the nozzle if needed. Return true on error.
   */
  bool unified_bed_leveling::prime_nozzle() {

    #if ENABLED(NEWPANEL)
      float Total_Prime = 0.0;

      if (g26_prime_flag == -1) {  // The user wants to control how much filament gets purged

        has_control_of_lcd_panel = true;
        lcd_setstatusPGM(PSTR("User-Controlled Prime"), 99);
        chirp_at_user();

        set_destination_to_current();

        recover_filament(destination); // Make sure G26 doesn't think the filament is retracted().

        while (!ubl_lcd_clicked()) {
          chirp_at_user();
          destination[E_AXIS] += 0.25;
          #ifdef PREVENT_LENGTHY_EXTRUDE
            Total_Prime += 0.25;
            if (Total_Prime >= EXTRUDE_MAXLENGTH) return UBL_ERR;
          #endif
          G26_line_to_destination(planner.max_feedrate_mm_s[E_AXIS] / 15.0);

          stepper.synchronize();    // Without this synchronize, the purge is more consistent,
                                    // but because the planner has a buffer, we won't be able
                                    // to stop as quickly.  So we put up with the less smooth
                                    // action to give the user a more responsive 'Stop'.
          set_destination_to_current();
          idle();
        }

        while (ubl_lcd_clicked()) idle();           // Debounce Encoder Wheel

        #if ENABLED(ULTRA_LCD)
          strcpy_P(lcd_status_message, PSTR("Done Priming")); // We can't do lcd_setstatusPGM() without having it continue;
                                                              // So...  We cheat to get a message up.
          lcd_setstatusPGM(PSTR("Done Priming"), 99);
          lcd_quick_feedback();
        #endif

        has_control_of_lcd_panel = false;

      }
      else {
    #else
    {
    #endif
      #if ENABLED(ULTRA_LCD)
        lcd_setstatusPGM(PSTR("Fixed Length Prime."), 99);
        lcd_quick_feedback();
      #endif
      set_destination_to_current();
      destination[E_AXIS] += g26_prime_length;
      G26_line_to_destination(planner.max_feedrate_mm_s[E_AXIS] / 15.0);
      stepper.synchronize();
      set_destination_to_current();
      retract_filament(destination);
    }

    return UBL_OK;
  }

#endif // AUTO_BED_LEVELING_UBL && UBL_G26_MESH_VALIDATION
