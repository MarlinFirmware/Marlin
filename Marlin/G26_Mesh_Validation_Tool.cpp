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

#if ENABLED(AUTO_BED_LEVELING_UBL) && ENABLED(UBL_G26_MESH_EDITING)

  #include "ubl.h"
  #include "Marlin.h"
  #include "planner.h"
  #include "stepper.h"
  #include "temperature.h"
  #include "ultralcd.h"

  #define EXTRUSION_MULTIPLIER 1.0
  #define RETRACTION_MULTIPLIER 1.0
  #define NOZZLE 0.3
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
   *   B #  Bed   Set the Bed Temperature.  If not specified, a default of 60 C. will be assumed.
   *
   *   C    Current   When searching for Mesh Intersection points to draw, use the current nozzle location
   *        as the base for any distance comparison.
   *
   *   D    Disable   Disable the Unified Bed Leveling System.  In the normal case the user is invoking this
   *        command to see how well a Mesh as been adjusted to match a print surface.  In order to do
   *        this the Unified Bed Leveling System is turned on by the G26 command.  The D parameter
   *        alters the command's normal behaviour and disables the Unified Bed Leveling System even if
   *        it is on.
   *
   *   H #  Hotend    Set the Nozzle Temperature.  If not specified, a default of 205 C. will be assumed.
   *
   *   F #  Filament  Used to specify the diameter of the filament being used.  If not specified
   *        1.75mm filament is assumed.  If you are not getting acceptable results by using the
   *        'correct' numbers, you can scale this number up or down a little bit to change the amount
   *        of filament that is being extruded during the printing of the various lines on the bed.
   *
   *   K    Keep-On   Keep the heaters turned on at the end of the command.
   *
   *   L #  Layer   Layer height.  (Height of nozzle above bed)  If not specified .20mm will be used.
   *
   *   Q #  Multiplier  Retraction Multiplier.  Normally not needed.  Retraction defaults to 1.0mm and
   *        un-retraction is at 1.2mm   These numbers will be scaled by the specified amount
   *
   *   N #  Nozzle    Used to control the size of nozzle diameter.  If not specified, a .4mm nozzle is assumed.
   *
   *   O #  Ooooze    How much your nozzle will Ooooze filament while getting in position to print.  This
   *        is over kill, but using this parameter will let you get the very first 'cicle' perfect
   *        so you have a trophy to peel off of the bed and hang up to show how perfectly you have your
   *        Mesh calibrated.  If not specified, a filament length of .3mm is assumed.
   *
   *   P #  Prime   Prime the nozzle with specified length of filament.  If this parameter is not
   *        given, no prime action will take place.  If the parameter specifies an amount, that much
   *        will be purged before continuing.  If no amount is specified the command will start
   *        purging filament until the user provides an LCD Click and then it will continue with
   *        printing the Mesh.  You can carefully remove the spent filament with a needle nose
   *        pliers while holding the LCD Click wheel in a depressed state.
   *
   *   R #  Random    Randomize the order that the circles are drawn on the bed.  The search for the closest
   *        undrawn cicle is still done.  But the distance to the location for each circle has a
   *        random number of the size specified added to it.  Specifying R50 will give an interesting
   *        deviation from the normal behaviour on a 10 x 10 Mesh.
   *
   *   X #  X coordinate  Specify the starting location of the drawing activity.
   *
   *   Y #  Y coordinate  Specify the starting location of the drawing activity.
   */

  extern float feedrate;
  extern Planner planner;
  //#if ENABLED(ULTRA_LCD)
    extern char lcd_status_message[];
  //#endif
  extern float destination[XYZE];
  extern void set_destination_to_current();
  extern void set_current_to_destination();
  extern float code_value_float();
  extern bool code_value_bool();
  extern bool code_has_value();
  extern void lcd_init();
  extern void lcd_setstatuspgm(const char* const message, const uint8_t level);
  #define PLANNER_XY_FEEDRATE() (min(planner.max_feedrate_mm_s[X_AXIS], planner.max_feedrate_mm_s[Y_AXIS])) //bob
  bool prepare_move_to_destination_cartesian();
  void line_to_destination();
  void line_to_destination(float );
  void gcode_G28();
  void sync_plan_position_e();
  void un_retract_filament(float where[XYZE]);
  void retract_filament(float where[XYZE]);
  void look_for_lines_to_connect();
  bool parse_G26_parameters();
  void move_to(const float&, const float&, const float&, const float&) ;
  void print_line_from_here_to_there(const float&, const float&, const float&, const float&, const float&, const float&);
  bool turn_on_heaters();
  bool prime_nozzle();
  void chirp_at_user();

  static uint16_t circle_flags[16], horizontal_mesh_line_flags[16], vertical_mesh_line_flags[16], continue_with_closest = 0;
  float g26_e_axis_feedrate = 0.020,
        random_deviation = 0.0,
        layer_height = LAYER_HEIGHT;

  bool g26_retracted = false; // We keep track of the state of the nozzle to know if it
                              // is currently retracted or not.  This allows us to be
                              // less careful because mis-matched retractions and un-retractions
                              // won't leave us in a bad state.

  float valid_trig_angle(float);
  mesh_index_pair find_closest_circle_to_print(const float&, const float&);

  static float extrusion_multiplier = EXTRUSION_MULTIPLIER,
               retraction_multiplier = RETRACTION_MULTIPLIER,
               nozzle = NOZZLE,
               filament_diameter = FILAMENT,
               prime_length = PRIME_LENGTH,
               x_pos, y_pos,
               bed_temp = BED_TEMP,
               hotend_temp = HOTEND_TEMP,
               ooze_amount = OOZE_AMOUNT;

  int8_t prime_flag = 0;

  bool keep_heaters_on = false;

  /**
   * G26: Mesh Validation Pattern generation.
   *
   * Used to interactively edit UBL's Mesh by placing the
   * nozzle in a problem area and doing a G29 P4 R command.
   */
  void gcode_G26() {
    float tmp, start_angle, end_angle;
    int   i, xi, yi;
    mesh_index_pair location;

    // Don't allow Mesh Validation without homing first,
    // or if the parameter parsing did not go OK, abort
    if (axis_unhomed_error(true, true, true) || parse_G26_parameters()) return;

    if (current_position[Z_AXIS] < Z_CLEARANCE_BETWEEN_PROBES) {
      do_blocking_move_to_z(Z_CLEARANCE_BETWEEN_PROBES);
      stepper.synchronize();
      set_current_to_destination();
    }

    if (turn_on_heaters()) goto LEAVE;

    current_position[E_AXIS] = 0.0;
    sync_plan_position_e();

    if (prime_flag && prime_nozzle()) goto LEAVE;

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
    destination[Z_AXIS] = layer_height;
    move_to(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], 0.0);
    move_to(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], ooze_amount);

    ubl.has_control_of_lcd_panel = true;
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

      if (ubl_lcd_clicked()) {              // Check if the user wants to stop the Mesh Validation
        #if ENABLED(ULTRA_LCD)
          lcd_setstatuspgm(PSTR("Mesh Validation Stopped."), 99);
          lcd_quick_feedback();
        #endif
        while (!ubl_lcd_clicked()) {         // Wait until the user is done pressing the
          idle();                            // Encoder Wheel if that is why we are leaving
          lcd_reset_alert_level();
          lcd_setstatuspgm(PSTR(""));
        }
        while (ubl_lcd_clicked()) {          // Wait until the user is done pressing the
          idle();                            // Encoder Wheel if that is why we are leaving
          lcd_setstatuspgm(PSTR("Unpress Wheel"), 99);
        }
        goto LEAVE;
      }

      location = continue_with_closest
        ? find_closest_circle_to_print(current_position[X_AXIS], current_position[Y_AXIS])
        : find_closest_circle_to_print(x_pos, y_pos); // Find the closest Mesh Intersection to where we are now.

      if (location.x_index >= 0 && location.y_index >= 0) {
        const float circle_x = pgm_read_float(&(ubl.mesh_index_to_xpos[location.x_index])),
                    circle_y = pgm_read_float(&(ubl.mesh_index_to_ypos[location.y_index]));

        // Let's do a couple of quick sanity checks.  We can pull this code out later if we never see it catch a problem
        #ifdef DELTA
          if (HYPOT2(circle_x, circle_y) > sq(DELTA_PRINTABLE_RADIUS)) {
            SERIAL_ERROR_START;
            SERIAL_ERRORLNPGM("Attempt to print outside of DELTA_PRINTABLE_RADIUS.");
            goto LEAVE;
          }
        #endif

        // TODO: Change this to use `position_is_reachable`
        if (!WITHIN(circle_x, X_MIN_POS, X_MAX_POS) || !WITHIN(circle_y, Y_MIN_POS, Y_MAX_POS)) {
          SERIAL_ERROR_START;
          SERIAL_ERRORLNPGM("Attempt to print off the bed.");
          goto LEAVE;
        }

        xi = location.x_index;  // Just to shrink the next few lines and make them easier to understand
        yi = location.y_index;

        if (ubl.g26_debug_flag) {
          SERIAL_ECHOPAIR("   Doing circle at: (xi=", xi);
          SERIAL_ECHOPAIR(", yi=", yi);
          SERIAL_CHAR(')');
          SERIAL_EOL;
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
          int tmp_div_30 = tmp / 30.0;
          if (tmp_div_30 < 0) tmp_div_30 += 360 / 30;
          if (tmp_div_30 > 11) tmp_div_30 -= 360 / 30;

          float x = circle_x + cos_table[tmp_div_30],    // for speed, these are now a lookup table entry
                y = circle_y + sin_table[tmp_div_30],
                xe = circle_x + cos_table[tmp_div_30 + 1],
                ye = circle_y + sin_table[tmp_div_30 + 1];
          #ifdef DELTA
            if (HYPOT2(x, y) > sq(DELTA_PRINTABLE_RADIUS))   // Check to make sure this part of
              continue;                                      // the 'circle' is on the bed.  If
          #else                                              // not, we need to skip
            x  = constrain(x, X_MIN_POS + 1, X_MAX_POS - 1); // This keeps us from bumping the endstops
            y  = constrain(y, Y_MIN_POS + 1, Y_MAX_POS - 1);
            xe = constrain(xe, X_MIN_POS + 1, X_MAX_POS - 1);
            ye = constrain(ye, Y_MIN_POS + 1, Y_MAX_POS - 1);
          #endif

          //if (ubl.g26_debug_flag) {
          //  char ccc, *cptr, seg_msg[50], seg_num[10];
          //  strcpy(seg_msg, "   segment: ");
          //  strcpy(seg_num, "    \n");
          //  cptr = (char*) "01234567890ABCDEF????????";
          //  ccc = cptr[tmp_div_30];
          //  seg_num[1] = ccc;
          //  strcat(seg_msg, seg_num);
          //  debug_current_and_destination(seg_msg);
          //}

          print_line_from_here_to_there(LOGICAL_X_POSITION(x), LOGICAL_Y_POSITION(y), layer_height, LOGICAL_X_POSITION(xe), LOGICAL_Y_POSITION(ye), layer_height);

        }

        //debug_current_and_destination(PSTR("Looking for lines to connect."));
        look_for_lines_to_connect();
        //debug_current_and_destination(PSTR("Done with line connect."));
      }

      //debug_current_and_destination(PSTR("Done with current circle."));

    } while (location.x_index >= 0 && location.y_index >= 0);

    LEAVE:
    lcd_reset_alert_level();
    lcd_setstatuspgm(PSTR("Leaving G26"));

    retract_filament(destination);
    destination[Z_AXIS] = Z_CLEARANCE_BETWEEN_PROBES;

    //debug_current_and_destination(PSTR("ready to do Z-Raise."));
    move_to(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], 0); // Raise the nozzle
    //debug_current_and_destination(PSTR("done doing Z-Raise."));

    destination[X_AXIS] = x_pos;                                               // Move back to the starting position
    destination[Y_AXIS] = y_pos;
    //destination[Z_AXIS] = Z_CLEARANCE_BETWEEN_PROBES;                        // Keep the nozzle where it is

    move_to(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], 0); // Move back to the starting position
    //debug_current_and_destination(PSTR("done doing X/Y move."));

    ubl.has_control_of_lcd_panel = false;     // Give back control of the LCD Panel!

    if (!keep_heaters_on) {
      #if HAS_TEMP_BED
        thermalManager.setTargetBed(0.0);
      #endif
      thermalManager.setTargetHotend(0.0, 0);
    }
  }


  float valid_trig_angle(float d) {
    while (d > 360.0) d -= 360.0;
    while (d < 0.0) d += 360.0;
    return d;
  }

  mesh_index_pair find_closest_circle_to_print(const float &X, const float &Y) {
    float closest = 99999.99;
    mesh_index_pair return_val;

    return_val.x_index = return_val.y_index = -1;

    for (uint8_t i = 0; i < GRID_MAX_POINTS_X; i++) {
      for (uint8_t j = 0; j < GRID_MAX_POINTS_Y; j++) {
        if (!is_bit_set(circle_flags, i, j)) {
          const float mx = pgm_read_float(&(ubl.mesh_index_to_xpos[i])),  // We found a circle that needs to be printed
                      my = pgm_read_float(&(ubl.mesh_index_to_ypos[j]));

          // Get the distance to this intersection
          float f = HYPOT(X - mx, Y - my);

          // It is possible that we are being called with the values
          // to let us find the closest circle to the start position.
          // But if this is not the case, add a small weighting to the
          // distance calculation to help it choose a better place to continue.
          f += HYPOT(x_pos - mx, y_pos - my) / 15.0;

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

  void look_for_lines_to_connect() {
    float sx, sy, ex, ey;

    for (uint8_t i = 0; i < GRID_MAX_POINTS_X; i++) {
      for (uint8_t j = 0; j < GRID_MAX_POINTS_Y; j++) {

        if (i < GRID_MAX_POINTS_X) { // We can't connect to anything to the right than GRID_MAX_POINTS_X.
                                         // This is already a half circle because we are at the edge of the bed.

          if (is_bit_set(circle_flags, i, j) && is_bit_set(circle_flags, i + 1, j)) { // check if we can do a line to the left
            if (!is_bit_set(horizontal_mesh_line_flags, i, j)) {

              //
              // We found two circles that need a horizontal line to connect them
              // Print it!
              //
              sx = pgm_read_float(&(ubl.mesh_index_to_xpos[  i  ])) + (SIZE_OF_INTERSECTION_CIRCLES - (SIZE_OF_CROSSHAIRS)); // right edge
              ex = pgm_read_float(&(ubl.mesh_index_to_xpos[i + 1])) - (SIZE_OF_INTERSECTION_CIRCLES - (SIZE_OF_CROSSHAIRS)); // left edge

              sx = constrain(sx, X_MIN_POS + 1, X_MAX_POS - 1);
              sy = ey = constrain(pgm_read_float(&(ubl.mesh_index_to_ypos[j])), Y_MIN_POS + 1, Y_MAX_POS - 1);
              ex = constrain(ex, X_MIN_POS + 1, X_MAX_POS - 1);

              if (ubl.g26_debug_flag) {
                SERIAL_ECHOPAIR(" Connecting with horizontal line (sx=", sx);
                SERIAL_ECHOPAIR(", sy=", sy);
                SERIAL_ECHOPAIR(") -> (ex=", ex);
                SERIAL_ECHOPAIR(", ey=", ey);
                SERIAL_CHAR(')');
                SERIAL_EOL;
                //debug_current_and_destination(PSTR("Connecting horizontal line."));
              }

              print_line_from_here_to_there(LOGICAL_X_POSITION(sx), LOGICAL_Y_POSITION(sy), layer_height, LOGICAL_X_POSITION(ex), LOGICAL_Y_POSITION(ey), layer_height);
              bit_set(horizontal_mesh_line_flags, i, j);   // Mark it as done so we don't do it again
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
                sy = pgm_read_float(&(ubl.mesh_index_to_ypos[  j  ])) + (SIZE_OF_INTERSECTION_CIRCLES - (SIZE_OF_CROSSHAIRS)); // top edge
                ey = pgm_read_float(&(ubl.mesh_index_to_ypos[j + 1])) - (SIZE_OF_INTERSECTION_CIRCLES - (SIZE_OF_CROSSHAIRS)); // bottom edge

                sx = ex = constrain(pgm_read_float(&(ubl.mesh_index_to_xpos[i])), X_MIN_POS + 1, X_MAX_POS - 1);
                sy = constrain(sy, Y_MIN_POS + 1, Y_MAX_POS - 1);
                ey = constrain(ey, Y_MIN_POS + 1, Y_MAX_POS - 1);

                if (ubl.g26_debug_flag) {
                  SERIAL_ECHOPAIR(" Connecting with vertical line (sx=", sx);
                  SERIAL_ECHOPAIR(", sy=", sy);
                  SERIAL_ECHOPAIR(") -> (ex=", ex);
                  SERIAL_ECHOPAIR(", ey=", ey);
                  SERIAL_CHAR(')');
                  SERIAL_EOL;
                  debug_current_and_destination(PSTR("Connecting vertical line."));
                }
                print_line_from_here_to_there(LOGICAL_X_POSITION(sx), LOGICAL_Y_POSITION(sy), layer_height, LOGICAL_X_POSITION(ex), LOGICAL_Y_POSITION(ey), layer_height);
                bit_set(vertical_mesh_line_flags, i, j);   // Mark it as done so we don't do it again
              }
            }
          }
        }
      }
    }
  }

  void move_to(const float &x, const float &y, const float &z, const float &e_delta) {
    float feed_value;
    static float last_z = -999.99;

    bool has_xy_component = (x != current_position[X_AXIS] || y != current_position[Y_AXIS]); // Check if X or Y is involved in the movement.

    //if (ubl.g26_debug_flag) SERIAL_ECHOLNPAIR("in move_to()  has_xy_component:", (int)has_xy_component);

    if (z != last_z) {
      //if (ubl.g26_debug_flag) SERIAL_ECHOLNPAIR("in move_to()  changing Z to ", (int)z);

      last_z = z;
      feed_value = planner.max_feedrate_mm_s[Z_AXIS]/(3.0);  // Base the feed rate off of the configured Z_AXIS feed rate

      destination[X_AXIS] = current_position[X_AXIS];
      destination[Y_AXIS] = current_position[Y_AXIS];
      destination[Z_AXIS] = z;                          // We know the last_z==z or we wouldn't be in this block of code.
      destination[E_AXIS] = current_position[E_AXIS];

      ubl_line_to_destination(feed_value, 0);

      stepper.synchronize();
      set_destination_to_current();

      //if (ubl.g26_debug_flag) debug_current_and_destination(PSTR(" in move_to() done with Z move"));
    }

    // Check if X or Y is involved in the movement.
    // Yes: a 'normal' movement. No: a retract() or un_retract()
    feed_value = has_xy_component ? PLANNER_XY_FEEDRATE() / 10.0 : planner.max_feedrate_mm_s[E_AXIS] / 1.5;

    if (ubl.g26_debug_flag) SERIAL_ECHOLNPAIR("in move_to() feed_value for XY:", feed_value);

    destination[X_AXIS] = x;
    destination[Y_AXIS] = y;
    destination[E_AXIS] += e_delta;

    //if (ubl.g26_debug_flag) debug_current_and_destination(PSTR(" in move_to() doing last move"));

    ubl_line_to_destination(feed_value, 0);

    //if (ubl.g26_debug_flag) debug_current_and_destination(PSTR(" in move_to() after last move"));

    stepper.synchronize();
    set_destination_to_current();

  }

  void retract_filament(float where[XYZE]) {
    if (!g26_retracted) { // Only retract if we are not already retracted!
      g26_retracted = true;
      //if (ubl.g26_debug_flag) SERIAL_ECHOLNPGM(" Decided to do retract.");
      move_to(where[X_AXIS], where[Y_AXIS], where[Z_AXIS], -1.0 * retraction_multiplier);
      //if (ubl.g26_debug_flag) SERIAL_ECHOLNPGM(" Retraction done.");
    }
  }

  void un_retract_filament(float where[XYZE]) {
    if (g26_retracted) { // Only un-retract if we are retracted.
      move_to(where[X_AXIS], where[Y_AXIS], where[Z_AXIS], 1.2 * retraction_multiplier);
      g26_retracted = false;
      //if (ubl.g26_debug_flag) SERIAL_ECHOLNPGM(" unretract done.");
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
  void print_line_from_here_to_there(const float &sx, const float &sy, const float &sz, const float &ex, const float &ey, const float &ez) {
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
    if (dist_end < dist_start && (SIZE_OF_INTERSECTION_CIRCLES) < abs(line_length)) {
      //if (ubl.g26_debug_flag) SERIAL_ECHOLNPGM("  Reversing start and end of print_line_from_here_to_there()");
      return print_line_from_here_to_there(ex, ey, ez, sx, sy, sz);
    }

    // Decide whether to retract.

    if (dist_start > 2.0) {
      retract_filament(destination);
      //if (ubl.g26_debug_flag) SERIAL_ECHOLNPGM("  filament retracted.");
    }
    move_to(sx, sy, sz, 0.0); // Get to the starting point with no extrusion

    const float e_pos_delta = line_length * g26_e_axis_feedrate * extrusion_multiplier;

    un_retract_filament(destination);

    //if (ubl.g26_debug_flag) {
    //  SERIAL_ECHOLNPGM("  doing printing move.");
    //  debug_current_and_destination(PSTR("doing final move_to() inside print_line_from_here_to_there()"));
    //}
    move_to(ex, ey, ez, e_pos_delta);  // Get to the ending point with an appropriate amount of extrusion
  }

  /**
   * This function used to be inline code in G26. But there are so many
   * parameters it made sense to turn them into static globals and get
   * this code out of sight of the main routine.
   */
  bool parse_G26_parameters() {

    extrusion_multiplier  = EXTRUSION_MULTIPLIER;
    retraction_multiplier = RETRACTION_MULTIPLIER;
    nozzle                = NOZZLE;
    filament_diameter     = FILAMENT;
    layer_height          = LAYER_HEIGHT;
    prime_length          = PRIME_LENGTH;
    bed_temp              = BED_TEMP;
    hotend_temp           = HOTEND_TEMP;
    ooze_amount           = OOZE_AMOUNT;
    prime_flag            = 0;
    keep_heaters_on       = false;

    if (code_seen('B')) {
      bed_temp = code_value_float();
      if (!WITHIN(bed_temp, 15.0, 140.0)) {
        SERIAL_PROTOCOLLNPGM("?Specified bed temperature not plausible.");
        return UBL_ERR;
      }
    }

    if (code_seen('C')) continue_with_closest++;

    if (code_seen('L')) {
      layer_height = code_value_float();
      if (!WITHIN(layer_height, 0.0, 2.0)) {
        SERIAL_PROTOCOLLNPGM("?Specified layer height not plausible.");
        return UBL_ERR;
      }
    }

    if (code_seen('Q')) {
      if (code_has_value()) {
        retraction_multiplier = code_value_float();
        if (!WITHIN(retraction_multiplier, 0.05, 15.0)) {
          SERIAL_PROTOCOLLNPGM("?Specified Retraction Multiplier not plausible.");
          return UBL_ERR;
        }
      }
      else {
        SERIAL_PROTOCOLLNPGM("?Retraction Multiplier must be specified.");
        return UBL_ERR;
      }
    }

    if (code_seen('N')) {
      nozzle = code_value_float();
      if (!WITHIN(nozzle, 0.1, 1.0)) {
        SERIAL_PROTOCOLLNPGM("?Specified nozzle size not plausible.");
        return UBL_ERR;
      }
    }

    if (code_seen('K')) keep_heaters_on++;

    if (code_seen('O') && code_has_value())
      ooze_amount = code_value_float();

    if (code_seen('P')) {
      if (!code_has_value())
        prime_flag = -1;
      else {
        prime_flag++;
        prime_length = code_value_float();
        if (!WITHIN(prime_length, 0.0, 25.0)) {
          SERIAL_PROTOCOLLNPGM("?Specified prime length not plausible.");
          return UBL_ERR;
        }
      }
    }

    if (code_seen('F')) {
      filament_diameter = code_value_float();
      if (!WITHIN(filament_diameter, 1.0, 4.0)) {
        SERIAL_PROTOCOLLNPGM("?Specified filament size not plausible.");
        return UBL_ERR;
      }
    }
    extrusion_multiplier *= sq(1.75) / sq(filament_diameter);         // If we aren't using 1.75mm filament, we need to
                                                                      // scale up or down the length needed to get the
                                                                      // same volume of filament

    extrusion_multiplier *= filament_diameter * sq(nozzle) / sq(0.3); // Scale up by nozzle size

    if (code_seen('H')) {
      hotend_temp = code_value_float();
      if (!WITHIN(hotend_temp, 165.0, 280.0)) {
        SERIAL_PROTOCOLLNPGM("?Specified nozzle temperature not plausible.");
        return UBL_ERR;
      }
    }

    if (code_seen('R')) {
      randomSeed(millis());
      random_deviation = code_has_value() ? code_value_float() : 50.0;
    }

    x_pos = current_position[X_AXIS];
    y_pos = current_position[Y_AXIS];

    if (code_seen('X')) {
      x_pos = code_value_float();
      if (!WITHIN(x_pos, X_MIN_POS, X_MAX_POS)) {
        SERIAL_PROTOCOLLNPGM("?Specified X coordinate not plausible.");
        return UBL_ERR;
      }
    }
    else

    if (code_seen('Y')) {
      y_pos = code_value_float();
      if (!WITHIN(y_pos, Y_MIN_POS, Y_MAX_POS)) {
        SERIAL_PROTOCOLLNPGM("?Specified Y coordinate not plausible.");
        return UBL_ERR;
      }
    }

    /**
     * We save the question of what to do with the Unified Bed Leveling System's Activation until the very
     * end.  The reason is, if one of the parameters specified up above is incorrect, we don't want to
     * alter the system's status.  We wait until we know everything is correct before altering the state
     * of the system.
     */
    ubl.state.active = !code_seen('D');

    return UBL_OK;
  }

  bool exit_from_g26() {
    //strcpy(lcd_status_message, "Leaving G26"); // We can't do lcd_setstatus() without having it continue;
    lcd_reset_alert_level();
    lcd_setstatuspgm(PSTR("Leaving G26"));
    while (ubl_lcd_clicked()) idle();
    return UBL_ERR;
  }

  /**
   * Turn on the bed and nozzle heat and
   * wait for them to get up to temperature.
   */
  bool turn_on_heaters() {
    #if HAS_TEMP_BED
      #if ENABLED(ULTRA_LCD)
        if (bed_temp > 25) {
          lcd_setstatuspgm(PSTR("G26 Heating Bed."), 99);
          lcd_quick_feedback();
      #endif
          ubl.has_control_of_lcd_panel = true;
          thermalManager.setTargetBed(bed_temp);
          while (abs(thermalManager.degBed() - bed_temp) > 3) {
            if (ubl_lcd_clicked()) return exit_from_g26();
            idle();
          }
      #if ENABLED(ULTRA_LCD)
        }
        lcd_setstatuspgm(PSTR("G26 Heating Nozzle."), 99);
        lcd_quick_feedback();
      #endif
    #endif

    // Start heating the nozzle and wait for it to reach temperature.
    thermalManager.setTargetHotend(hotend_temp, 0);
    while (abs(thermalManager.degHotend(0) - hotend_temp) > 3) {
      if (ubl_lcd_clicked()) return exit_from_g26();
      idle();
    }

    #if ENABLED(ULTRA_LCD)
      lcd_reset_alert_level();
      lcd_setstatuspgm(PSTR(""));
      lcd_quick_feedback();
    #endif

    return UBL_OK;
  }

  /**
   * Prime the nozzle if needed. Return true on error.
   */
  bool prime_nozzle() {
    float Total_Prime = 0.0;

    if (prime_flag == -1) {  // The user wants to control how much filament gets purged

      ubl.has_control_of_lcd_panel = true;

      lcd_setstatuspgm(PSTR("User-Controlled Prime"), 99);
      chirp_at_user();

      set_destination_to_current();

      un_retract_filament(destination); // Make sure G26 doesn't think the filament is retracted().

      while (!ubl_lcd_clicked()) {
        chirp_at_user();
        destination[E_AXIS] += 0.25;
        #ifdef PREVENT_LENGTHY_EXTRUDE
          Total_Prime += 0.25;
          if (Total_Prime >= EXTRUDE_MAXLENGTH) return UBL_ERR;
        #endif
        ubl_line_to_destination(planner.max_feedrate_mm_s[E_AXIS] / 15.0, 0);

        stepper.synchronize();    // Without this synchronize, the purge is more consistent,
                                  // but because the planner has a buffer, we won't be able
                                  // to stop as quickly.  So we put up with the less smooth
                                  // action to give the user a more responsive 'Stop'.
        set_destination_to_current();
        idle();
      }

      while (ubl_lcd_clicked()) idle();           // Debounce Encoder Wheel

      #if ENABLED(ULTRA_LCD)
        strcpy_P(lcd_status_message, PSTR("Done Priming")); // We can't do lcd_setstatuspgm() without having it continue;
                                                            // So...  We cheat to get a message up.
        lcd_setstatuspgm(PSTR("Done Priming"), 99);
        lcd_quick_feedback();
      #endif

      ubl.has_control_of_lcd_panel = false;

    }
    else {
      #if ENABLED(ULTRA_LCD)
        lcd_setstatuspgm(PSTR("Fixed Length Prime."), 99);
        lcd_quick_feedback();
      #endif
      set_destination_to_current();
      destination[E_AXIS] += prime_length;
      ubl_line_to_destination(planner.max_feedrate_mm_s[E_AXIS] / 15.0, 0);
      stepper.synchronize();
      set_destination_to_current();
      retract_filament(destination);
    }

    return UBL_OK;
  }

#endif // AUTO_BED_LEVELING_UBL && UBL_G26_MESH_EDITING
