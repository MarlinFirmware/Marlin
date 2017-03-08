/*
  * Marlin Firmware -- G26 - Mesh Validation Tool
*/

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
  *
  * About Marlin
  *
  * This firmware is a mashup between Sprinter and grbl.
  *  - https://github.com/kliment/Sprinter
  *  - https://github.com/simen/grbl/tree
  *
  * It has preliminary support for Matthew Roberts advance algorithm
  *  - http://reprap.org/pipermail/reprap-dev/2011-May/003323.html
*/

#define   EXTRUSION_MULTIPLIER 1.0    // This is too much clutter for the main Configuration.h file  But
#define   RETRACTION_MULTIPLIER 1.0   // some user have expressed an interest in being able to customize
#define   NOZZLE 0.3                  // these numbers for thier printer so they don't need to type all
#define   FILAMENT 1.75               // the options every time they do a Mesh Validation Print.
#define   LAYER_HEIGHT   .2
#define   PRIME_LENGTH 10.0           // So, we put these number in an easy to find and change place.
#define   BED_TEMP 60.0
#define   HOTEND_TEMP 205.0
#define   OOOOZE_AMOUNT 0.3

#include "Marlin.h"
#include "Configuration.h"
#include "planner.h"
#include "stepper.h"
#include "temperature.h"
#include "UBL.h"
#include "ultralcd.h"

#ifdef AUTO_BED_LEVELING_UBL

  #define SIZE_OF_INTERSECTION_CIRCLES  5
  #define SIZE_OF_CROSS_HAIRS   3 // cross hairs inside the circle.  This number should be
                                  // less than SIZE_OR_INTERSECTION_CIRCLES


  // Roxy's G26 Mesh Validation Tool
  //
  // G26 Is a Mesh Validation Tool intended to provide support for the Marlin Unified Bed Leveling System.
  // In order to fully utilize and benefit from the Marlin Unified Bed Leveling System an accurate Mesh must
  // be defined.  G29 is designed to allow the user to quickly validate the correctness of her Mesh.  It will
  // first heat the bed and nozzle. It will then print lines and circles along the Mesh Cell boundaries and
  // the intersections of those lines (respectively).
  //
  // This action allows the user to immediately see where the Mesh is properly defined and where it needs to
  // be edited.   The command will generate the Mesh lines closest to the nozzle's starting position.  Alternatively
  // the user can specify the X and Y position of interest with command parameters.  This allows the user to
  // focus on a particular area of the Mesh where attention is needed.
  //
  // B #  Bed   Set the Bed Temperature.  If not specified, a default of 60 C. will be assumed.
  //
  // C    Current   When searching for Mesh Intersection points to draw, use the current nozzle location
  //      as the base for any distance comparison.
  //
  // D    Disable   Disable the Unified Bed Leveling System.  In the normal case the user is invoking this
  //      command to see how well a Mesh as been adjusted to match a print surface.  In order to do
  //      this the Unified Bed Leveling System is turned on by the G26 command.   The D parameter
  //      alters the command's normal behaviour and disables the Unified Bed Leveling System even if
  //      it is on.
  //
  // H #  Hotend    Set the Nozzle Temperature.  If not specified, a default of 205 C. will be assumed.
  //
  // F #  Filament  Used to specify the diameter of the filament being used.  If not specified
  //      1.75mm filament is assumed.  If you are not getting acceptable results by using the
  //      'correct' numbers, you can scale this number up or down a little bit to change the amount
  //      of filament that is being extruded during the printing of the various lines on the bed.
  //
  // K    Keep-On   Keep the heaters turned on at the end of the command.
  //
  // L #  Layer   Layer height.  (Height of nozzle above bed)  If not specified .20mm will be used.
  //
  // Q #  Multiplier  Retraction Multiplier.   Normally not needed.  Retraction defaults to 1.0mm and
  //      un-retraction is at 1.2mm   These numbers will be scaled by the specified amount
  //
  // N #  Nozzle    Used to control the size of nozzle diameter.  If not specified, a .4mm nozzle is assumed.
  //
  // O #  Ooooze    How much your nozzle will Ooooze filament while getting in position to print.  This
  //      is over kill, but using this parameter will let you get the very first 'cicle' perfect
  //      so you have a trophy to peel off of the bed and hang up to show how perfectly you have your
  //      Mesh calibrated.  If not specified, a filament length of .3mm is assumed.
  //
  // P #  Prime   Prime the nozzle with specified length of filament.  If this parameter is not
  //      given, no prime action will take place.   If the parameter specifies an amount, that much
  //      will be purged before continuing.   If no amount is specified the command will start
  //      purging filament until the user provides an LCD Click and then it will continue with
  //      printing the Mesh.  You can carefully remove the spent filament with a needle nose
  //      pliers while holding the LCD Click wheel in a depressed state.
  //
  // R #  Random    Randomize the order that the circles are drawn on the bed.  The search for the closest
  //      undrawn cicle is still done.  But the distance to the location for each circle has a
  //      random number of the size specified added to it.  Specifying R50 will give an interesting
  //      deviation from the normal behaviour on a 10 x 10 Mesh.
  //
  // X #  X coordinate  Specify the starting location of the drawing activity.
  //
  // Y #  Y coordinate  Specify the starting location of the drawing activity.
  //

  extern int UBL_has_control_of_LCD_Panel;
  extern float feedrate;
//extern bool relative_mode;
  extern Planner planner;
  //#if ENABLED(ULTRA_LCD)
      extern char lcd_status_message[];
  //#endif
  extern float destination[];
  extern void set_destination_to_current();
  extern void set_current_to_destination();
  extern float code_value_float(); 
  extern bool code_value_bool();
  extern bool code_has_value();
  extern void lcd_init();
  #define PLANNER_XY_FEEDRATE() (min(planner.max_feedrate_mm_s[X_AXIS], planner.max_feedrate_mm_s[Y_AXIS])) //bob
  bool prepare_move_to_destination_cartesian();
  void line_to_destination();
  void line_to_destination(float );
  void gcode_G28();
  void sync_plan_position_e();
  void un_retract_filament();
  void retract_filament();
  void look_for_lines_to_connect();
  void bit_clear( unsigned int bits[16], int , int );
  void bit_set( unsigned int bits[16], int , int );
  bool is_bit_set( unsigned int bits[16], int , int );
  bool parse_G26_parameters();
  void move_to( float, float, float, float);
  void print_line_from_here_to_there( float sx, float sy, float sz, float ex, float ey, float ez );
  bool turn_on_heaters();
  int  prime_nozzle();
  void chirp_at_user();

  static unsigned circle_flags[16], horizontal_mesh_line_flags[16], vertical_mesh_line_flags[16], Continue_with_closest=0;
  static float G26_E_AXIS_feedrate = 0.020;
  static float Random_Deviation = 0.0, Layer_Height=LAYER_HEIGHT;

  static bool retracted=false;  // We keep track of the state of the nozzle to know if it
                                // is currently retracted or not.  This allows us to be
                                // less careful because mis-matched retractions and un-retractions
                                // won't leave us in a bad state.


  #if ENABLED(ULTRA_LCD)
    void lcd_setstatus(const char* message, bool persist);
  #endif

  float valid_trig_angle( float );
  struct mesh_index_pair find_closest_circle_to_print( float, float );
  void debug_current_and_destination(char *title);
  void UBL_line_to_destination(float, float, float, float, float, uint8_t );
  //    uint16_t x_splits = 0xffff, uint16_t y_splits = 0xffff);  /* needed for the old mesh_buffer_line() routine */

  static float E_Pos_Delta, Extrusion_Multiplier=EXTRUSION_MULTIPLIER;
  static float Retraction_Multiplier=RETRACTION_MULTIPLIER;
  static float Nozzle=NOZZLE , Filament=FILAMENT, Prime_Length=PRIME_LENGTH;
  static float X_Pos, Y_Pos, bed_temp=BED_TEMP, hotend_temp=HOTEND_TEMP, Ooooze_Amount=OOOOZE_AMOUNT;
  static int Prime_Flag=0, Keep_Heaters_On=0;

  int G26_Debug_flag=0;

  void gcode_G26() {
    float circle_x, circle_y, x, y, xe, ye, tmp;
    float start_angle, end_angle;
    int   i, xi, yi, lcd_init_counter=0;
    struct mesh_index_pair location;

    if ( axis_unhomed_error(true, true, true) ) { // Don't allow Mesh Validation without homing first
      gcode_G28();
    }

    if ( parse_G26_parameters() ) { // If the paramter parsing did not go OK, we abort the command
      return;
    }

    if ( current_position[Z_AXIS] < Z_CLEARANCE_BETWEEN_PROBES )  {
      do_blocking_move_to_z( Z_CLEARANCE_BETWEEN_PROBES);
      stepper.synchronize();
      set_current_to_destination();
    }

    if ( turn_on_heaters() ) {     // Turn on the heaters, leave the command if anything
      goto LEAVE;                  // has gone wrong.
    }
    axis_relative_modes[E_AXIS] = false;    // Get things setup so we can take control of the
//  relative_mode = false;                  // planner and stepper motors!
    current_position[E_AXIS] = 0.0;
    sync_plan_position_e();

    if (Prime_Flag) {
      if ( prime_nozzle() )       // if prime_nozzle() returns an error, we just bail out.
        goto LEAVE;
    }

    //
    //      Bed is preheated
    //
    //      Nozzle is at temperature
    //
    //      Filament is primed!
    //
    //      It's  "Show Time" !!!
    //


    //
    // Clear all of the flags we need
    //
    for (i=0; i<16; i++) {
      circle_flags[i]     = 0;
      horizontal_mesh_line_flags[i] = 0;
      vertical_mesh_line_flags[i] = 0;
    }

    //
    // Move nozzle to the specified height for the first layer
    //
    set_destination_to_current();
    destination[Z_AXIS] = Layer_Height;
    move_to( destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], 0.0 );
    move_to( destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], Ooooze_Amount );

    UBL_has_control_of_LCD_Panel = 1; // Take control of the LCD Panel!
    debug_current_and_destination( (char *) "Starting G26 Mesh Validation Pattern.");

    do {
      if ( G29_lcd_clicked() ) {                                 // Check if the user wants to stop the Mesh Validation
        strcpy( lcd_status_message, "Mesh Validation Stopped."); // We can't do lcd_setstatus() without having it continue;
        while ( G29_lcd_clicked() ) {                            // Debounce the switch click
          idle();
        }
        #if ENABLED(ULTRA_LCD)
          lcd_setstatus( "Mesh Validation Stopped.", true);
          lcd_quick_feedback();
        #endif
        goto LEAVE;
      }

      if ( Continue_with_closest ) {
        location = find_closest_circle_to_print( current_position[X_AXIS], current_position[Y_AXIS]);
      }
      else {
        location = find_closest_circle_to_print( X_Pos, Y_Pos ); // Find the closest Mesh Intersection to where
      }
      // we are now.
      if (location.x_index>=0 && location.y_index>=0 ) {
        circle_x = blm.map_x_index_to_bed_location(location.x_index);
        circle_y = blm.map_y_index_to_bed_location(location.y_index);

        // Let's do a couple of quick sanity checks.  We can pull this code out later if we never see it catch a problem

        #ifdef DELTA
          if ( sqrt( circle_x*circle_x + circle_y*circle_y) > DELTA_PRINTABLE_RADIUS) {
            SERIAL_PROTOCOLLNPGM("?Error: Attempt to print outside of DELTA_PRINTABLE_RADIUS.");
            goto LEAVE;
          }
        #endif
        if ( circle_x<X_MIN_POS || circle_x>X_MAX_POS || circle_y<Y_MIN_POS || circle_y>Y_MAX_POS)  {
          SERIAL_PROTOCOLLNPGM("?Error: Attempt to print off the bed.");
          goto LEAVE;
        }

        xi = location.x_index;  // Just to shrink the next few lines and make them easier to understand
        yi = location.y_index;

        if (G26_Debug_flag!=0) {
          SERIAL_ECHOPGM("   Doing circle at: (xi=");
          SERIAL_ECHO(xi);
          SERIAL_ECHOPGM(",yi=");
          SERIAL_ECHO(yi);
          SERIAL_ECHOPGM(")\n");
        }

        start_angle = 0.0;    // assume it is going to be a full circle
        end_angle   = 360.0;
        if ( xi==0 )  {       // Check for bottom edge
          start_angle = -90.0;
          end_angle   =  90.0;
          if ( yi==0 ) {      // it is an edge, check for the two left corners
            start_angle = 0.0;
          }
          else {
            if ( yi==UBL_MESH_NUM_Y_POINTS-1 ) {
              end_angle   =  0.0;
            }
          }
          goto GOT_ANGLES;
        }
        if ( xi==UBL_MESH_NUM_X_POINTS-1 ){ // Check for top edge
          start_angle =  90.0;
          end_angle   = 270.0;
          if ( yi==0 ) {                // it is an edge, check for the two right corners
            end_angle = 180.0;
          }
          else {
            if ( yi==UBL_MESH_NUM_Y_POINTS-1 ) {
              start_angle   =  180.0;
            }
          }
          goto GOT_ANGLES;
        }
        if ( yi==0 )  {
          start_angle =   0.0;         // only do the top   side of the cirlce
          end_angle   = 180.0;
        }
        if ( yi==UBL_MESH_NUM_Y_POINTS-1 )  {
          start_angle = 180.0;         // only do the bottom side of the cirlce
          end_angle   = 360.0;
        }

        GOT_ANGLES:

        //
        // Declare and generate a sin() & cos() table to be used during the circle drawing.   This will lighten
        // the CPU load and make the arc drawing faster and more smooth
        //
        float sin_table[360/30+1], cos_table[360/30+1];
        int tmp_div_30;
        for (i=0; i<=360/30; i++) {
          cos_table[i] = SIZE_OF_INTERSECTION_CIRCLES * cos( RADIANS( valid_trig_angle(i*30.0)) );
          sin_table[i] = SIZE_OF_INTERSECTION_CIRCLES * sin( RADIANS( valid_trig_angle(i*30.0)) );
        }

        for (tmp=start_angle; tmp<end_angle-.1; tmp+=30.0 ) {
          tmp_div_30 = tmp/30.0;
          if ( tmp_div_30 < 0 ) {
            tmp_div_30 += 360/30;
          }

          x = circle_x + cos_table[tmp_div_30];    // for speed, these are now a lookup table entry
          y = circle_y + sin_table[tmp_div_30];

          if ( tmp_div_30 > 11 ) {
            tmp_div_30 -= 360/30;
          }
          xe = circle_x + cos_table[tmp_div_30+1]; // for speed, these are now a lookup table entry
          ye = circle_y + sin_table[tmp_div_30+1];
          #ifdef DELTA
            if ((x*x+y*y) > (DELTA_PRINTABLE_RADIUS*DELTA_PRINTABLE_RADIUS)) { // Check to make sure this part of
              continue;                                                        // the 'circle' is on the bed.  If
            }
          #else                                                                // not, we need to skip

            x = constrain( x, X_MIN_POS+1, X_MAX_POS-1);                       // This keeps us from bumping the endstops
            y = constrain( y, Y_MIN_POS+1, Y_MAX_POS-1);
            xe= constrain( xe, X_MIN_POS+1, X_MAX_POS-1);
            ye= constrain( ye, Y_MIN_POS+1, Y_MAX_POS-1);
          #endif

          if (G26_Debug_flag!=0) {
            char ccc, *cptr, seg_msg[50], seg_num[10];
            strcpy(seg_msg,"   segment: ");
            strcpy(seg_num,"    \n");
            cptr = ( char * ) "01234567890ABCDEF????????";
            ccc = cptr[tmp_div_30];
            seg_num[1] = ccc;
            strcat(seg_msg, seg_num);
            debug_current_and_destination(seg_msg);
          }

          print_line_from_here_to_there( x, y, Layer_Height, xe, ye, Layer_Height);
        }
        lcd_init_counter++;
        if (lcd_init_counter > 10 ) {
          lcd_init_counter = 0;
          lcd_init(); // Some people's LCD Displays are locking up.  This might help them
        }

        debug_current_and_destination( (char *) "Looking for lines to connect.");
        look_for_lines_to_connect();
        debug_current_and_destination( (char *) "Done with line connect.");
      }

      debug_current_and_destination( (char *) "Done with current circle.");

    }
    while (location.x_index>=0 && location.y_index>=0 );

    LEAVE:


    retract_filament();
    destination[Z_AXIS] = Z_CLEARANCE_BETWEEN_PROBES;                             // Raise the nozzle

    debug_current_and_destination( (char *) "ready to do Z-Raise.");
    move_to( destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], 0); // Raise the nozzle
    debug_current_and_destination( (char *) "done doing Z-Raise.");

    destination[X_AXIS] = X_Pos;                                                // Move back to the starting position
    destination[Y_AXIS] = Y_Pos;
    destination[Z_AXIS] = Z_CLEARANCE_BETWEEN_PROBES;                             // Keep the nozzle where it is

    move_to( destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], 0); // Move back to the starting position
    debug_current_and_destination( (char *) "done doing X/Y move.");

    UBL_has_control_of_LCD_Panel = 0;     // Give back control of the LCD Panel!

    if ( !Keep_Heaters_On) {
      #if HAS_TEMP_BED
        thermalManager.setTargetBed( 0.0 );
      #endif
      thermalManager.setTargetHotend( 0.0 , 0 );
    }
    lcd_init(); // Some people's LCD Displays are locking up.  This might help them
  }


  float valid_trig_angle( float d ) {
    while ( d > 360.0 ) {
      d -= 360.0;
    }

    while ( d < 0.0 ) {
      d += 360.0;
    }
    return d;
  }

  struct mesh_index_pair find_closest_circle_to_print( float X, float Y ) {
    int i, j;
    float f, mx, my, dx, dy, closest=99999.99;
    struct mesh_index_pair return_val;

    return_val.x_index = -1;
    return_val.y_index = -1;

    for (i=0; i<UBL_MESH_NUM_X_POINTS; i++) {
      for (j=0; j<UBL_MESH_NUM_Y_POINTS; j++) {
        if ( !is_bit_set( circle_flags, i, j) )  {
          mx = blm.map_x_index_to_bed_location(i);  // We found a circle that needs to be printed
          my = blm.map_y_index_to_bed_location(j);

          dx = X - mx;        // Get the distance to this intersection
          dy = Y - my;
          f = sqrt(dx*dx+dy*dy);

          dx = X_Pos - mx;                  // It is possible that we are being called with the values
          dy = Y_Pos - my;                  // to let us find the closest circle to the start position.
          f += sqrt(dx*dx+dy*dy) / 15.0;    // But if this is not the case,
                                            // we are going to add in a small
                                            // weighting to the distance calculation to help it choose
                                            // a better place to continue.

          if (Random_Deviation > 1.0 ) {
            f += random(0.0,Random_Deviation);// Add in the specified amount of Random Noise to our search
          }

          if ( f < closest ) {
            closest = f;              // We found a closer location that is still
            return_val.x_index = i;   // un-printed  --- save the data for it
            return_val.y_index = j;
            return_val.distance= closest;
          }
        }
      }
    }
    bit_set( circle_flags, return_val.x_index, return_val.y_index);   // Mark this location as done.
    return return_val;
  }


  // These support functions allow the use of large bit arrays of flags that take very
  // little RAM. Currently they are limited to being 16x16 in size.  Changing the declaration
  // to unsigned long will allow us to go to 32x32 if higher resolution Mesh's are needed
  // in the future.

  void bit_clear( unsigned int bits[16], int x, int y) {
    unsigned mask;

    mask = 0x1 << x;
    bits[y] |= mask;  // set the bit and then
    bits[y] ^= mask;  // use XOR to invert the bit
  }

  void bit_set( unsigned int bits[16], int x, int y) {
    unsigned mask;

    mask = 0x1 << x;
    bits[y] |= mask;  // set the bit
  }

  bool is_bit_set( unsigned int bits[16], int x, int y) {
    unsigned mask;

    mask = 0x1 << x;
    return (bool) (bits[y] & mask); // return the specified bit's value
  }

  void look_for_lines_to_connect() {
    float sx, sy, ex, ey;
    int i, j;

    for (i=0; i<UBL_MESH_NUM_X_POINTS; i++ ) {
      for (j=0; j<UBL_MESH_NUM_Y_POINTS; j++ ) {

        if ( i < UBL_MESH_NUM_X_POINTS) { // We can't connect to anything to the right than UBL_MESH_NUM_X_POINTS.
                                      // This is already a half circle because we are at the edge of the bed.

          if ( is_bit_set( circle_flags, i, j) && is_bit_set( circle_flags, i+1, j) ) { // check if we can do a line to the left
            if ( !is_bit_set( horizontal_mesh_line_flags, i, j) )  {

              //
              // We found two circles that need a horizontal line to connect them
              // Print it!
              //
              sx = blm.map_x_index_to_bed_location(i);
              sx = sx+SIZE_OF_INTERSECTION_CIRCLES-SIZE_OF_CROSS_HAIRS; // get the right edge of the circle
              sy = blm.map_y_index_to_bed_location(j);

              ex = blm.map_x_index_to_bed_location(i+1);
              ex = ex-SIZE_OF_INTERSECTION_CIRCLES+SIZE_OF_CROSS_HAIRS; // get the left edge of the circle
              ey = sy;

              sx= constrain( sx, X_MIN_POS+1, X_MAX_POS-1);             // This keeps us from bumping the endstops
              sy= constrain( sy, Y_MIN_POS+1, Y_MAX_POS-1);
              ex= constrain( ex, X_MIN_POS+1, X_MAX_POS-1);
              ey= constrain( ey, Y_MIN_POS+1, Y_MAX_POS-1);

              if (G26_Debug_flag!=0) {
                SERIAL_ECHOPGM(" Connecting with horizontal line (sx=");
                SERIAL_ECHO(sx);
                SERIAL_ECHOPGM(",sy=");
                SERIAL_ECHO(sy);
                SERIAL_ECHOPGM(") -> (ex=");
                SERIAL_ECHO(ex);
                SERIAL_ECHOPGM(",ey=");
                SERIAL_ECHO(ey);
                SERIAL_ECHOPGM(")\n");
                debug_current_and_destination( (char *) "Connecting horizontal line.");
              }

              print_line_from_here_to_there( sx, sy, Layer_Height, ex, ey, Layer_Height );
              bit_set( horizontal_mesh_line_flags, i, j);   // Mark it as done so we don't do it again
            }
          }

          if ( j < UBL_MESH_NUM_Y_POINTS) { // We can't connect to anything further back than UBL_MESH_NUM_Y_POINTS.
                                        // This is already a half circle because we are at the edge  of the bed.

            if ( is_bit_set( circle_flags, i, j) && is_bit_set( circle_flags, i, j+1) ) { // check if we can do a line straight down
              if ( !is_bit_set( vertical_mesh_line_flags, i, j) )  {
                //
                // We found two circles that need a vertical line to connect them
                // Print it!
                //
                sx = blm.map_x_index_to_bed_location(i);
                sy = blm.map_y_index_to_bed_location(j);
                sy = sy+SIZE_OF_INTERSECTION_CIRCLES-SIZE_OF_CROSS_HAIRS; // get the top edge of the circle

                ex = sx;
                ey = blm.map_y_index_to_bed_location(j+1);
                ey = ey-SIZE_OF_INTERSECTION_CIRCLES+SIZE_OF_CROSS_HAIRS; // get the bottom edge of the circle

                sx= constrain( sx, X_MIN_POS+1, X_MAX_POS-1);             // This keeps us from bumping the endstops
                sy= constrain( sy, Y_MIN_POS+1, Y_MAX_POS-1);
                ex= constrain( ex, X_MIN_POS+1, X_MAX_POS-1);
                ey= constrain( ey, Y_MIN_POS+1, Y_MAX_POS-1);

                if (G26_Debug_flag!=0) {
                  SERIAL_ECHOPGM(" Connecting with vertical line (sx=");
                  SERIAL_ECHO(sx);
                  SERIAL_ECHOPGM(",sy=");
                  SERIAL_ECHO(sy);
                  SERIAL_ECHOPGM(") -> (ex=");
                  SERIAL_ECHO(ex);
                  SERIAL_ECHOPGM(",ey=");
                  SERIAL_ECHO(ey);
                  SERIAL_ECHOPGM(")\n");
                  debug_current_and_destination( (char *) "Connecting vertical line.");
                }
                print_line_from_here_to_there( sx, sy, Layer_Height, ex, ey, Layer_Height );
                bit_set( vertical_mesh_line_flags, i, j);   // Mark it as done so we don't do it again
              }
            }
          }
        }
      }
    }
  }

  void debug_current_and_destination(char *title) {
    float dx, dy, de, xy_dist, fpmm;

    if ( *title != '!' ) { // if the title message starts with a '!' it is so important, we are going to
                           // ignore the status of the G26_Debug_Flag
      if ( G26_Debug_flag==0) {
        return;
      }
    }
    dx = current_position[X_AXIS] - destination[X_AXIS];
    dy = current_position[Y_AXIS] - destination[Y_AXIS];
    de = destination[E_AXIS] - current_position[E_AXIS];
    if ( de==0.0) {
      idle();
      return;
    }

    xy_dist = sqrt( dx*dx + dy*dy );
    if ( xy_dist==0.0) {
      idle();
      return;
      SERIAL_ECHOPGM("   FPMM=");
      fpmm = de;
      SERIAL_PROTOCOL_F( fpmm, 6 );
    }
    else  {
      SERIAL_ECHOPGM("   fpmm=");
      fpmm = de / xy_dist;
      SERIAL_PROTOCOL_F( fpmm, 6 );
    }

    SERIAL_ECHOPGM("    current=( ");
    SERIAL_PROTOCOL_F( current_position[X_AXIS], 6 );
    SERIAL_ECHOPGM(" , ");
    SERIAL_PROTOCOL_F( current_position[Y_AXIS], 6 );
    SERIAL_ECHOPGM(" , ");
    SERIAL_PROTOCOL_F( current_position[Z_AXIS], 6 );
    SERIAL_ECHOPGM(" , ");
    SERIAL_PROTOCOL_F( current_position[E_AXIS], 6 );
    SERIAL_ECHOPGM(" )   destination=( ");
    if ( current_position[X_AXIS] == destination[X_AXIS] ) {
      SERIAL_ECHOPGM( "-------------" );
    }
    else {
      SERIAL_PROTOCOL_F( destination[X_AXIS], 6 );
    }
    SERIAL_ECHOPGM(" , ");

    if ( current_position[Y_AXIS] == destination[Y_AXIS] ) {
      SERIAL_ECHOPGM( "-------------" );
    }
    else {
      SERIAL_PROTOCOL_F( destination[Y_AXIS], 6 );
    }
    SERIAL_ECHOPGM(" , ");

    if ( current_position[Z_AXIS] == destination[Z_AXIS] ) {
      SERIAL_ECHOPGM( "-------------" );
    }
    else {
      SERIAL_PROTOCOL_F( destination[Z_AXIS], 6 );
    }
    SERIAL_ECHOPGM(" , ");

    if ( current_position[E_AXIS] == destination[E_AXIS] ) {
      SERIAL_ECHOPGM( "-------------" );
    }
    else {
      SERIAL_PROTOCOL_F( destination[E_AXIS], 6 );
    }

    SERIAL_ECHOPGM(" )   ");
    SERIAL_ECHO( title );
    SERIAL_ECHOPGM("  \n");

    pinMode(66, INPUT_PULLUP); // Roxy's Left Switch is on pin 66.  Right Switch is on pin 65

    //if ( been_to_2_6 ) {
    //while ( (digitalRead(66) & 0x01) != 0)
    //  idle();
    //}

  }

  void move_to( float x, float y, float z, float e_delta) {
    float feed_value;
    bool  has_XY_component = false;
    static float last_z = -999.99;

    if ( x!=current_position[X_AXIS] || y!=current_position[Y_AXIS]) {// Check if X or Y is involved in the movement.
      has_XY_component = true;
    }

    if (G26_Debug_flag!=0) {
      SERIAL_ECHOPAIR("in move_to()  has_XY_component:", (int) has_XY_component);
      SERIAL_ECHO("\n");
    }

    if ( z != last_z )  {

      if (G26_Debug_flag!=0) {
        SERIAL_ECHOPAIR("in move_to()  changing Z to ", (int) z );
        SERIAL_ECHO("\n");
      }
      last_z = z;
      feed_value = planner.max_feedrate_mm_s[Z_AXIS]/(3.0);  // Base the feed rate off of the configured Z_AXIS feed rate

      destination[X_AXIS] = current_position[X_AXIS];
      destination[Y_AXIS] = current_position[Y_AXIS];
      destination[Z_AXIS] = z;                          // We know the last_z==z or we wouldn't be in this block of code.
      destination[E_AXIS] = current_position[E_AXIS];

      UBL_line_to_destination( destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feed_value, 0 );

      stepper.synchronize();
      set_destination_to_current();

      if (G26_Debug_flag!=0) {
        debug_current_and_destination( (char *) " in move_to() done with Z move");
      }
    }

    if ( has_XY_component)  {                           // Check if X or Y is involved in the movement.
      feed_value = PLANNER_XY_FEEDRATE()/(10.0);        // Yes!  It is a 'normal' movement
    }
    else  {
      feed_value = planner.max_feedrate_mm_s[E_AXIS]/(1.5);  // it is just a retract() or un_retract()
    }

    if (G26_Debug_flag!=0) {
      SERIAL_ECHOPAIR("in move_to()  feed_value for XY:", feed_value );
      SERIAL_ECHO("\n");
    }

    destination[X_AXIS] = x;
    destination[Y_AXIS] = y;
    destination[E_AXIS] += e_delta;

    if (G26_Debug_flag!=0) {
      debug_current_and_destination( (char *) " in move_to() doing last move");
    }

    UBL_line_to_destination( destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS], feed_value, 0 );

    if (G26_Debug_flag!=0) {
      debug_current_and_destination( (char *) " in move_to() after last move");
    }
    stepper.synchronize();
    set_destination_to_current();
  }

  void retract_filament()   {
    if ( !retracted ) {   // Only retract if we are not already retracted!
      retracted = true;
      if (G26_Debug_flag!=0) {
        SERIAL_ECHOPGM(" Decided to do retract.\n");
      }
      move_to( destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], -1.0 * Retraction_Multiplier );
      if (G26_Debug_flag!=0) {
        SERIAL_ECHOPGM(" Retraction done.\n");
      }
    }
  }

  void un_retract_filament()   {
    if ( retracted ) {    // Only un-retract if we are retracted.
      move_to( destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS],  1.2 * Retraction_Multiplier );
      retracted = false;
      if (G26_Debug_flag!=0) {
        SERIAL_ECHOPGM(" unretract done.\n");
      }
    }
  }

  // print_line_from_here_to_there() takes two cartesian coordinates and draws a line from one
  // to the other.  But there are really three sets of coordinates involved.  The first coordinate
  // is the present location of the nozzle.   We don't necessarily want to print from this location.
  // We first need to move the nozzle to the start of line segment where we want to print.  Once
  // there, we can use the two coordinates supplied to draw the line.
  //
  // Note:  Although we assume the first set of coordinates is the start of the line and the second
  // set of coordinates is the end of the line, it does not always work out that way.  This function
  // optimizes the movement to minimize the travel distance before it can start printing.  This saves
  // a lot of time and eleminates a lot of non-sensical movement of the nozzle.   However, it does
  // cause a lot of very little short retracement of th nozzle when it draws the very first line
  // segment of a 'circle'.   The time this requires is very short and is easily saved by the other
  // cases where the optimization comes into play.

  void print_line_from_here_to_there( float sx, float sy, float sz, float ex, float ey, float ez ) {
    float dx, dy, dx_s, dy_s, dx_e, dy_e, dist_start, dist_end, Line_Length;

    dx_s = current_position[X_AXIS] - sx;   // find our distance from the start of the actual line segment
    dy_s = current_position[Y_AXIS] - sy;
    dist_start = dx_s*dx_s + dy_s*dy_s;     // We don't need to do a sqrt(), we can compare the distance^2
                                            // to save computation time

    dx_e = current_position[X_AXIS] - ex;   // find our distance from the end of the actual line segment
    dy_e = current_position[Y_AXIS] - ey;
    dist_end  = dx_e*dx_e + dy_e*dy_e;

    dx = ex - sx;
    dy = ey - sy;
    Line_Length= sqrt(dx*dx+dy*dy);

    // If the end point of the line is closer to the nozzle, we are going to
    // flip the direction of this line.   We will print it from the end to the start.
    // On very small lines we don't do the optimization because it just isn't worth it.
    //
    if ( (dist_end < dist_start) && ( SIZE_OF_INTERSECTION_CIRCLES < abs(Line_Length)) )  {
      if (G26_Debug_flag!=0) {
        SERIAL_ECHOPGM("  Reversing start and end of print_line_from_here_to_there()\n");
      }
      print_line_from_here_to_there( ex, ey, ez, sx, sy, sz );
      return;
    }

    // Now decide if we should retract.

    if ( dist_start > 2.0 )  {
      retract_filament();
      if (G26_Debug_flag!=0) {
        SERIAL_ECHOPGM("  filament retracted.\n");
      }
    }
    move_to( sx, sy, sz, 0.0 ); // Get to the starting point with no extrusion

    E_Pos_Delta = Line_Length * G26_E_AXIS_feedrate * Extrusion_Multiplier;

    un_retract_filament();
    if (G26_Debug_flag!=0) {
      SERIAL_ECHOPGM("  doing printing move.\n");
      debug_current_and_destination( (char *) "doing final move_to() inside print_line_from_here_to_there()");
    }
    move_to( ex, ey, ez, E_Pos_Delta);  // Get to the ending point with an appropriate amount of extrusion
  }

  // The parse_G26_Parameters() function used to be inline code for the function.   But
  // there are so many parameters, it made sense to turn the parameter variables into
  // static globals and get this code out of sight from the main routine.

  bool parse_G26_parameters() {

    Extrusion_Multiplier = EXTRUSION_MULTIPLIER;
    Retraction_Multiplier= RETRACTION_MULTIPLIER;
    Nozzle   = NOZZLE;
    Filament   = FILAMENT;
    Layer_Height   = LAYER_HEIGHT;
    Prime_Length   = PRIME_LENGTH;
    bed_temp   = BED_TEMP;
    hotend_temp    = HOTEND_TEMP;
    Ooooze_Amount  = OOOOZE_AMOUNT;
    Prime_Flag   =0;
    Keep_Heaters_On  =0;

    X_Pos = current_position[X_AXIS];
    Y_Pos = current_position[Y_AXIS];

    if (code_seen('B')) {
      bed_temp = code_value_float();
      if ( bed_temp<15.0 || bed_temp>140.0 ) {
        SERIAL_PROTOCOLPGM("?Specified bed temperature not plausable.\n");
        return true;
      }
    }

    if (code_seen('C')) {
      Continue_with_closest++;
    }

    if (code_seen('L')) {
      Layer_Height = code_value_float();
      if ( Layer_Height<0.0 || Layer_Height>2.0 ) {
        SERIAL_PROTOCOLPGM("?Specified layer height not plausable.\n");
        return true;
      }
    }

    if (code_seen('Q')) {
      if (code_has_value()) {
        Retraction_Multiplier = code_value_float();
        if ( Retraction_Multiplier<.05 || Retraction_Multiplier>15.0 ) {
          SERIAL_PROTOCOLPGM("?Specified Retraction Multiplier not plausable.\n");
          return true;
        }
      }
      else {
        SERIAL_PROTOCOLPGM("?Retraction Multiplier must be specified.\n");
        return true;
      }
    }

    if (code_seen('N')) {
      Nozzle = code_value_float();
      if ( Nozzle<.1 || Nozzle>1.0 ) {
        SERIAL_PROTOCOLPGM("?Specified nozzle size not plausable.\n");
        return true;
      }
    }

    if (code_seen('K')) {
      Keep_Heaters_On++;
    }

    if (code_seen('O'))  {
      if ( code_has_value() )
      Ooooze_Amount = code_value_float();
    }

    if (code_seen('P')) {
      Prime_Flag++;
      if ( !code_has_value() ) {
        Prime_Flag = -1;
      }
      else {
        Prime_Length = code_value_float();
        if ( Prime_Length<0.0 || Prime_Length>25.0 ) {
          SERIAL_PROTOCOLPGM("?Specified prime length not plausable.\n");
          return true;
        }
      }
    }

    if (code_seen('F')) {
      Filament = code_value_float();
      if ( Filament<1.0 || Filament>4.0 ) {
        SERIAL_PROTOCOLPGM("?Specified filament size not plausable.\n");
        return true;
      }
    }
    Extrusion_Multiplier *= 1.75*1.75 / (Filament*Filament);  // If we aren't using 1.75mm filament, we need to
                                                              // scale up or down the length needed to get the
                                                              // same volume of filament
    Extrusion_Multiplier *= Filament*(Nozzle*Nozzle)/(.3*.3); // Scale up by nozzle size

    if (code_seen('H')) {
      hotend_temp = code_value_float();
      if ( hotend_temp<165.0 || hotend_temp>280.0 ) {
        SERIAL_PROTOCOLPGM("?Specified nozzle temperature not plausable.\n");
        return true;
      }
    }

    if (code_seen('R') ) {
      randomSeed(millis());
      if ( code_has_value() ) {
        Random_Deviation = code_value_float();
      }
      else {
        Random_Deviation = 50.0;
      }
    }

    X_Pos = current_position[X_AXIS];
    Y_Pos = current_position[Y_AXIS];

    if (code_seen('X')) {
      X_Pos = code_value_float();
      if ( X_Pos<X_MIN_POS || X_Pos>X_MAX_POS ) {
        SERIAL_PROTOCOLPGM("?Specified X coordinate not plausable.\n");
        return true;
      }
    }

    if (code_seen('Y')) {
      Y_Pos = code_value_float();
      if ( Y_Pos<Y_MIN_POS || Y_Pos>Y_MAX_POS ) {
        SERIAL_PROTOCOLPGM("?Specified Y coordinate not plausable.\n");
        return true;
      }
    }

    //
    // We save the question of what to do with the Unified Bed Leveling System's Activation until the very
    // end.   The reason is, if one of the parameters specified up above is incorrect, we don't want to
    // alter the system's status.  We wait until we know everything is correct before altering the state
    // of the system.
    //
    if (code_seen('D')) {
      blm.state.active = 0;
    }
    else {
      blm.state.active = 1;
    }
    return false;
  }

  //
  //
  // Turn on the bed and  nozzle heat.   Wait for them to get up to temperature
  //
  //

  bool turn_on_heaters() {
    #if HAS_TEMP_BED
      #if ENABLED(ULTRA_LCD)
        if ( bed_temp > 25 ) {
          lcd_setstatus( "G26 Heating Bed.  ", true);
          lcd_quick_feedback();
      #endif
          UBL_has_control_of_LCD_Panel++;
          thermalManager.setTargetBed( bed_temp );
          while( abs(thermalManager.degBed()-bed_temp) > 3 ) {
            if ( G29_lcd_clicked() ) {
              strcpy( lcd_status_message, "Leaving G26    "); // We can't do lcd_setstatus() without having it continue;
              while ( G29_lcd_clicked() )                     // Debounce the switch
              idle();
              lcd_setstatus( "Leaving G26     ", true);       // Now we do it right.
              return 1;
            }
            idle();
          }
        #if ENABLED(ULTRA_LCD)
        }
        lcd_setstatus( "G26 Heating Nozzle. ", true);
        lcd_quick_feedback();
      #endif
    #endif

    //
    // Now turn on the hotend (nozzle) and wait for it to come up to
    // temperature.

    thermalManager.setTargetHotend( hotend_temp , 0 );
    while( abs(thermalManager.degHotend(0)-hotend_temp) > 3 ) {
      if ( G29_lcd_clicked() ) {
        strcpy( lcd_status_message, "Leaving G26    "); // We can't do lcd_setstatus() without having it continue;
        while ( G29_lcd_clicked() ) {                   // Debounce the switch
          idle();
        }
        lcd_setstatus( "Leaving G26     ", true);       // Now we do it right.
        return 1;
      }
      idle();
    }

    #if ENABLED(ULTRA_LCD)
      lcd_setstatus( "                  ", true);
      lcd_quick_feedback();
    #endif
    return 0;
  }

  //
  // This block of code primes the nozzle if needed.
  //

  int prime_nozzle() {
    float Total_Prime = 0.0;

    if ( Prime_Flag == -1 )  {  // The user wants to control how much filament gets purged
      lcd_setstatus( "User Controled Prime", true);
      chirp_at_user();

      set_destination_to_current();

      un_retract_filament();    // Lets make sure the G26 command doesn't think the filament is
                                // retracted().   We are here because we want to prime the nozzle.
                                // So let's just unretract just to be sure.
      UBL_has_control_of_LCD_Panel++;
      while( !G29_lcd_clicked() ) {
        chirp_at_user();
        destination[E_AXIS] += 0.25;
        #ifdef PREVENT_LENGTHY_EXTRUDE
          Total_Prime += 0.25;
          if (Total_Prime >= EXTRUDE_MAXLENGTH ) {
            return 1;
          }
        #endif
        UBL_line_to_destination(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS],
        //            planner.max_feedrate_mm_s[E_AXIS]/(15.0), 0, 0xffff, 0xffff );
        planner.max_feedrate_mm_s[E_AXIS]/(15.0), 0 );

        stepper.synchronize();    // Without this synchronize, the purge is more consistent,
                                  // but because the planner has a buffer, we won't be able
                                  // to stop as quickly.  So we put up with the less smooth
                                  // action to give the user a more responsive 'Stop'.
        set_destination_to_current();
        idle();
      }

      strcpy( lcd_status_message, "Done Priming     "); // We can't do lcd_setstatus() without having it continue;
                                                        // So...  We cheat to get a message up.

      while ( G29_lcd_clicked() )  ;                    // Debounce the switch

      #if ENABLED(ULTRA_LCD)
        UBL_has_control_of_LCD_Panel = 0;
        lcd_setstatus( "Done Priming     ", true);      // Now we do it right.
        lcd_quick_feedback();
      #endif
    }
    else {
      #if ENABLED(ULTRA_LCD)
        lcd_setstatus( "Fixed Length Prime.", true);
        lcd_quick_feedback();
      #endif
      set_destination_to_current();
      destination[E_AXIS] += Prime_Length;
      UBL_line_to_destination(destination[X_AXIS], destination[Y_AXIS], destination[Z_AXIS], destination[E_AXIS],
      //          planner.max_feedrate_mm_s[E_AXIS]/(15.0), 0, 0xffff, 0xffff );
      planner.max_feedrate_mm_s[E_AXIS]/(15.0), 0 );
      stepper.synchronize();
      set_destination_to_current();
      retract_filament();
    }
    return 0;
  }
#endif // AUTO_BED_LEVELING_UBL
