/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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



#include "../../inc/MarlinConfig.h"

#if ENABLED(DRILLING_CANNED_CYCLES)

#include "../../gcode/gcode.h"
#include "../../feature/bedlevel/bedlevel.h"

#include "../../MarlinCore.h"
#include "../../module/planner.h"
#include "../../module/stepper.h"
#include "../../module/motion.h"
#include "../../module/tool_change.h"
#include "../../module/temperature.h"
#include "../../lcd/ultralcd.h"

bool fast_move;/*=false*/


/**
 *   G81 Peck Drill Canned Cycle
 *
 *  X Y - Hole position
 *  Z - Hole Depth - Relative to the Retraction plan R
 *  R - Position on Retraction plan
 *  Q - Max Depth for each Cut/Peck
 *  P - Dwell time on bottom of each Cut/Peck
 *  F - Cutting feedrate 
 * 
 */

// Private functions


float G81_Z,
      G81_R,
      G81_Q=0,
      G81_F,
      G81_N_D = 0;

millis_t G81_P=0;
      

/**
 * G81
 */
void GcodeSuite::G81()
{
  
    
      #ifdef G0_FEEDRATE
        float g0_feedrate_mm_s, saved_feedrate_mm_s;

            saved_feedrate_mm_s = feedrate_mm_s;

      #endif

      if (parser.seen('Z')) G81_Z = parser.value_float();

      SERIAL_ECHO("Z");
      SERIAL_ECHOLN(G81_Z);
      
      if (parser.seen('R'))G81_R = parser.value_float();

      SERIAL_ECHO("R");
      SERIAL_ECHOLN(G81_R);

      if (parser.seen('F')) {
        
          G81_F = parser.value_float();
          feedrate_mm_s = MMM_TO_MMS(G81_F);
      }

      SERIAL_ECHO("F");
      SERIAL_ECHOLN(G81_F);

      g0_feedrate_mm_s = MMM_TO_MMS(G0_FEEDRATE);

    
      float sav_pos_X = current_position[X_AXIS],
            sav_pos_Y = current_position[Y_AXIS],
            sav_pos_Z = current_position[Z_AXIS];

      
      float z_zero_pos = sav_pos_Z - G81_R;

      float Z_fin_pos = z_zero_pos + G81_Z;

      destination[X_AXIS] = sav_pos_X;
      destination[Y_AXIS] = sav_pos_Y;

    
        destination[Z_AXIS] = z_zero_pos + G81_R;
        prepare_internal_move_to_destination();
        planner.synchronize();

      if(G81_Q == 0){
        G81_N_D = Z_fin_pos;
      }
      else{
      G81_N_D = z_zero_pos + G81_R - G81_Q;
      }

      while (Z_fin_pos < G81_N_D){

        destination[Z_AXIS] = G81_N_D;
        prepare_internal_move_to_destination();
        planner.synchronize();

        dwell(G81_P);

        feedrate_mm_s = g0_feedrate_mm_s;

        destination[Z_AXIS] = z_zero_pos + G81_R;
        prepare_internal_move_to_destination();
        planner.synchronize();

        feedrate_mm_s = MMM_TO_MMS(G81_F);

        G81_N_D = G81_N_D - G81_Q;
      }

      destination[Z_AXIS] = z_zero_pos + G81_Z;
      prepare_internal_move_to_destination();
      planner.synchronize();

      dwell(G81_P);

      feedrate_mm_s = g0_feedrate_mm_s;

      destination[Z_AXIS] = z_zero_pos + G81_R;
      prepare_internal_move_to_destination();
      planner.synchronize();
  

      #ifdef G0_FEEDRATE
        // Restore the motion mode feedrate
        if (fast_move) feedrate_mm_s = saved_feedrate_mm_s;
      #endif

    

    

}



/**
 *   G82 Peck Drill Canned Cycle
 *
 *  X Y - Hole position
 *  Z - Hole Depth - Relative to the Retraction plan R
 *  R - Position on Retraction plan
 *  Q - Max Depth for each Cut/Peck
 *  P - Dwell time on bottom of each Cut/Peck
 *  F - Cutting feedrate 
 * 
 */

// Private functions


float G82_Z,
      G82_R,
      G82_Q=0,
      G82_F,
      G82_N_D = 0;

millis_t G82_P;
      

/**
 * G82
 */
void GcodeSuite::G82()
{
  
    
      #ifdef G0_FEEDRATE
        float g0_feedrate_mm_s, saved_feedrate_mm_s;

            saved_feedrate_mm_s = feedrate_mm_s;

      #endif

      if (parser.seen('Z')) G82_Z = parser.value_float();

      SERIAL_ECHO("Z");
      SERIAL_ECHOLN(G82_Z);
      
      if (parser.seen('R'))G82_R = parser.value_float();

      SERIAL_ECHO("R");
      SERIAL_ECHOLN(G82_R);
       
      if (parser.seenval('P')) G82_P = parser.value_millis();

      SERIAL_ECHO("P");
      SERIAL_ECHOLN(G82_P);

      if (parser.seen('F')) {
        
          G82_F = parser.value_float();
          feedrate_mm_s = MMM_TO_MMS(G82_F);
      }

      SERIAL_ECHO("F");
      SERIAL_ECHOLN(G82_F);

      g0_feedrate_mm_s = MMM_TO_MMS(G0_FEEDRATE);

    
      float sav_pos_X = current_position[X_AXIS],
            sav_pos_Y = current_position[Y_AXIS],
            sav_pos_Z = current_position[Z_AXIS];

      
      float z_zero_pos = sav_pos_Z - G82_R;

      float Z_fin_pos = z_zero_pos + G82_Z;

      destination[X_AXIS] = sav_pos_X;
      destination[Y_AXIS] = sav_pos_Y;

    
        destination[Z_AXIS] = z_zero_pos + G82_R;
        prepare_internal_move_to_destination();
        planner.synchronize();

      if(G82_Q == 0){
        G82_N_D = Z_fin_pos;
      }
      else{
      G82_N_D = z_zero_pos + G82_R - G82_Q;
      }

      while (Z_fin_pos < G82_N_D){

        destination[Z_AXIS] = G82_N_D;
        prepare_internal_move_to_destination();
        planner.synchronize();

        dwell(G82_P);

        feedrate_mm_s = g0_feedrate_mm_s;

        destination[Z_AXIS] = z_zero_pos + G82_R;
        prepare_internal_move_to_destination();
        planner.synchronize();

        feedrate_mm_s = MMM_TO_MMS(G82_F);

        G82_N_D = G82_N_D - G82_Q;
      }

      destination[Z_AXIS] = z_zero_pos + G82_Z;
      prepare_internal_move_to_destination();
      planner.synchronize();

      dwell(G82_P);

      feedrate_mm_s = g0_feedrate_mm_s;

      destination[Z_AXIS] = z_zero_pos + G82_R;
      prepare_internal_move_to_destination();
      planner.synchronize();
  

      #ifdef G0_FEEDRATE
        // Restore the motion mode feedrate
        if (fast_move) feedrate_mm_s = saved_feedrate_mm_s;
      #endif

    

    

}



/**
 *   G83 Peck Drill Canned Cycle
 *
 *  X Y - Hole position
 *  Z - Hole Depth - Relative to the Retraction plan R
 *  R - Position on Retraction plan
 *  Q - Max Depth for each Cut/Peck
 *  P - Dwell time on bottom of each Cut/Peck
 *  F - Cutting feedrate 
 * 
 */

// External references

// Private functions


float G83_Z,
      G83_R,
      G83_Q,
      G83_F,
      N_D = 0;

millis_t G83_P;
      

/**
 * G83
 */
void GcodeSuite::G83()
{
  
    
      #ifdef G0_FEEDRATE
        float g0_feedrate_mm_s, saved_feedrate_mm_s;

            saved_feedrate_mm_s = feedrate_mm_s;

      #endif

      if (parser.seen('Z')) G83_Z = parser.value_float();

      SERIAL_ECHO("Z");
      SERIAL_ECHOLN(G83_Z);
      
      if (parser.seen('R'))G83_R = parser.value_float();

      SERIAL_ECHO("R");
      SERIAL_ECHOLN(G83_R);

      if (parser.seen('Q')) G83_Q = parser.value_float();

      SERIAL_ECHO("Q");
      SERIAL_ECHOLN(G83_Q);
       
      if (parser.seenval('P')) G83_P = parser.value_millis();

      SERIAL_ECHO("P");
      SERIAL_ECHOLN(G83_P);

      if (parser.seen('F')) {
        
          G83_F = parser.value_float();
          feedrate_mm_s = MMM_TO_MMS(G83_F);
      }

      SERIAL_ECHO("F");
      SERIAL_ECHOLN(G83_F);

      g0_feedrate_mm_s = MMM_TO_MMS(G0_FEEDRATE);

    
      float sav_pos_X = current_position[X_AXIS],
            sav_pos_Y = current_position[Y_AXIS],
            sav_pos_Z = current_position[Z_AXIS];

      
      float z_zero_pos = sav_pos_Z - G83_R;

      float Z_fin_pos = z_zero_pos + G83_Z;

      destination[X_AXIS] = sav_pos_X;
      destination[Y_AXIS] = sav_pos_Y;

    
        destination[Z_AXIS] = z_zero_pos + G83_R;
        prepare_internal_move_to_destination();
        planner.synchronize();

      if(G83_Q == 0){
        N_D = Z_fin_pos;
      }
      else{
      N_D = z_zero_pos + G83_R - G83_Q;
      }

      while (Z_fin_pos < N_D){

        destination[Z_AXIS] = N_D;
        prepare_internal_move_to_destination();
        planner.synchronize();

        dwell(G83_P);

        feedrate_mm_s = g0_feedrate_mm_s;

        destination[Z_AXIS] = z_zero_pos + G83_R;
        prepare_internal_move_to_destination();
        planner.synchronize();

        feedrate_mm_s = MMM_TO_MMS(G83_F);

        N_D = N_D - G83_Q;
      }

      destination[Z_AXIS] = z_zero_pos + G83_Z;
      prepare_internal_move_to_destination();
      planner.synchronize();

      dwell(G83_P);

      feedrate_mm_s = g0_feedrate_mm_s;

      destination[Z_AXIS] = z_zero_pos + G83_R;
      prepare_internal_move_to_destination();
      planner.synchronize();
  

      #ifdef G0_FEEDRATE
        // Restore the motion mode feedrate
        if (fast_move) feedrate_mm_s = saved_feedrate_mm_s;
      #endif

    

    

}

#endif

