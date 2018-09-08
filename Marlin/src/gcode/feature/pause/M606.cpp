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
 * gcode.h + fwretract.h + ultralcd.cpp + language en + fr + runout.h pause.h gcode.cpp
 */

#include "../../../inc/MarlinConfig.h"

#if ENABLED(BUCKET_FEATURE)
  #include "../../gcode.h"
  #include "../../../feature/pause.h"
  #include "../../../module/motion.h"
  #include "../../../module/tool_change.h"
  #include "../../../feature/fwretract.h"
  #include "../../../module/temperature.h" 
  #include "../../../module/planner.h"
  #include "../../../module/stepper.h" 

  #include "../../../lcd/ultralcd.h"

  #define LAST_EXTRUDER  EXTRUDERS-1
  
		// Tool migration
  int tool_migration_last_target = 0;
  
  // Purge
  float bucket_purge_length   = BUCKET_PURGE_LENGTH 
    ,   bucket_purge_feedrate = BUCKET_PURGE_FEEDRATE
    ,   bucket_retract        = BUCKET_RETRACT ;
  #if DISABLED(FWRETRACT)
    float bucket_retract_feedrate = BUCKET_RETRACT_FEEDRATE
        , bucket_recover_feedrate = BUCKET_RECOVER_FEEDRATE;
  #endif
  int bucket_fan_dwell = BUCKET_FAN_DWELL;
  
  // Extrude function
  void tool_migration_e_move(const float length, const float fr) {
    set_destination_from_current();
    destination[E_AXIS] += length / planner.e_factor[active_extruder];
    planner.buffer_line_kinematic(destination, fr, active_extruder);
    planner.synchronize();
    set_current_from_destination();
  }
#endif

/**
 * M606: Tool Migration
 *
 *  L[count] - Last target tool - Set the last extruder for migration : 0 for disabling
 *  T[count] - Desired target tool - Force the migration to desired extruder
 *  Default  - Automatic migration on next extruder available
 *
 * Purge
 *  D[count] - s    - Fan dwell
 *  E[count] - mm   - Purge length
 *  F[count] - mm/s - Purge feedrate
 *  G[count] - mm   - Retract length
 *  H[count] - mm/s - Retract feedrate - If FWRETRACT disabled
 *  I[count] - mm/s - Recover feedrate - If FWRETRACT disabled
 *
 *  P1 - Purge extrusion
 *  Q1 - Fan dwell 
 *  R1 - Retract
 *  S1	- Recover
 */
	
#if ENABLED(BUCKET_FEATURE) 

 void GcodeSuite::M606() {
  
  // Purge
  bool tool_migration_purge_return = false;
  
  // Purge Commands
  if( parser.floatval('P',0)) {
   tool_migration_e_move(bucket_purge_length, bucket_purge_feedrate);
   return;
  }
  
  if( parser.floatval('Q',0)) {
   dwell(bucket_fan_dwell *1000);
   return;
  }
  
  if( parser.floatval('R',0)) {
   tool_migration_e_move(-bucket_retract, 
   #if DISABLED(FWRETRACT)
     bucket_retract_feedrate
   #else
     fwretract.retract_feedrate_mm_s
   #endif
   );
   return;
  }  
  if( parser.floatval('S',0)) {
   tool_migration_e_move(bucket_retract, 
   #if DISABLED(FWRETRACT)
     bucket_recover_feedrate
   #else
     fwretract.retract_recover_feedrate_mm_s
   #endif
   );
   return;
  }  
  
		// Purge Settings
  if( parser.floatval('D',-1) >=0) {
   bucket_fan_dwell = parser.floatval('D');
   tool_migration_purge_return = true;
  }

  if( parser.floatval('E',-1) >=0) {
   bucket_purge_length = parser.floatval('E');
   tool_migration_purge_return = true;
  }
  if( parser.floatval('F',-1) >=0) {
   bucket_purge_feedrate = parser.floatval('F');
   tool_migration_purge_return = true;
  }
  if( parser.floatval('G',-1) >=0) {
   bucket_retract = parser.floatval('G');
   tool_migration_purge_return = true;
  } 
  #if DISABLED(FWRETRACT)
    if( parser.floatval('H',-1) >=0) {   
     bucket_retract_feedrate = parser.floatval('H');   
     tool_migration_purge_return = true;      
    }
    if( parser.floatval('I',-1) >=0) {
     bucket_recover_feedrate = parser.floatval('I');
     tool_migration_purge_return = true;
    } 
  #endif
  
  if(tool_migration_purge_return) return;
  // end Purge
  
  const int last_target_tmp = parser.intval('L',(EXTRUDERS));
  
  //Tool migration management
  const int desired_target_tmp = parser.intval('T',(EXTRUDERS));
  bool automatic_migration_enabled=false;	
			
  // Limiter
  if ( (last_target_tmp < 0) || ( last_target_tmp > 	EXTRUDERS) ) return;
  if ( (desired_target_tmp < 0) || (desired_target_tmp > EXTRUDERS) || (desired_target_tmp == active_extruder) ) return;		

  // Default values are for automatic next tool
  if ( (last_target_tmp == EXTRUDERS) && (desired_target_tmp == EXTRUDERS) ) automatic_migration_enabled=true;

  // Set the wanted last target
  if (last_target_tmp < EXTRUDERS) {
   tool_migration_last_target = last_target_tmp; 
   if (desired_target_tmp == EXTRUDERS) return; // If just a setting then return
  }

  // If no next extruder available
  if ( (active_extruder >= tool_migration_last_target) && automatic_migration_enabled ) return; 	

  // Don't allow filament change without homing first
  if (axis_unhomed_error()) home_all_axes();		

  
  //*********************
  // Migration beginning
  //*********************
  
  // Here the process continue if a desired migration :M600 T<...> or automatic :M606
  float resume_position[XYZE];
  int16_t fansp=fanSpeeds[BUCKET_FAN];
  planner.synchronize();
  // Save current position
  COPY(resume_position, current_position);
  
  //Settings transmiting  
  if (automatic_migration_enabled){ // If automatic next tool 
   //Same temperature
   thermalManager.setTargetHotend(thermalManager.degHotend(active_extruder), active_extruder+1);
   // Same flow after tool change
   planner.flow_percentage[active_extruder+1] = planner.flow_percentage[active_extruder];				
   // Same FwRetract/Swap statuses
   #if ENABLED(FWRETRACT)
				//New extruder have retract current status
				fwretract.retracted[active_extruder+1] = fwretract.retracted[active_extruder];
				fwretract.retracted_swap[active_extruder+1] = fwretract.retracted_swap[active_extruder];
			#endif
  }
  else { // If a desired tool is required
   //Same temperature
   thermalManager.setTargetHotend(thermalManager.degHotend(active_extruder), desired_target_tmp);
   // Same flow after tool change
   planner.flow_percentage[desired_target_tmp] = planner.flow_percentage[active_extruder];
   // Same FwRetract/Swap statuses
   #if ENABLED(FWRETRACT)
				//New extruder have retract current statuses
				fwretract.retracted[desired_target_tmp] = fwretract.retracted[active_extruder];
				fwretract.retracted_swap[desired_target_tmp] = fwretract.retracted_swap[active_extruder];
			#endif
  }

  // UNLOADING : If negative position or FwRetracted  then adjustment of the unload length
  #if ENABLED(FWRETRACT)	
    if (fwretract.retracted[active_extruder]) {
					//Unloading of old extruder
     tool_migration_e_move( -RETRACT_LENGTH_SWAP + fwretract.retract_length, RETRACT_FEEDRATE);					
    }	
    else {
					//If negative position ' rare or impossible when FWRETRACT used '
     if (resume_position[E_AXIS]<0) 
      tool_migration_e_move( -RETRACT_LENGTH_SWAP - resume_position[E_AXIS], RETRACT_FEEDRATE);								
     else tool_migration_e_move(-RETRACT_LENGTH_SWAP, RETRACT_FEEDRATE);
    }
  #else							
    if (resume_position[E_AXIS]<0) 
     tool_migration_e_move(-BUCKET_TOOL_MIGRATION_UNLOAD_LENGTH - resume_position[E_AXIS], BUCKET_TOOL_MIGRATION_UNLOAD_F);								
    else tool_migration_e_move(-BUCKET_TOOL_MIGRATION_UNLOAD_LENGTH, BUCKET_TOOL_MIGRATION_UNLOAD_F);	
  #endif					

  // FWRETRACT STATUSES : Old extruder receive swap status because unloaded
		#if ENABLED(FWRETRACT)
				fwretract.retracted_swap[active_extruder] = true;
				fwretract.retracted[active_extruder] = false;
  #endif
  
  // PARKING
  #if ENABLED(BUCKET_TOOL_MIGRATION_NOZZLE_PARK) && ENABLED(NOZZLE_PARK_FEATURE)
    Nozzle::park(2, NOZZLE_PARK_POINT);
  #endif  

  // TOOL CHANGE : Set the new active extruder		
  if (automatic_migration_enabled) active_extruder++;
  else active_extruder = desired_target_tmp;

  //TEMP : Ensure_safe_temperature	
  LCD_MESSAGEPGM(MSG_FILAMENT_CHANGE_HEATING_1);    
  while (thermalManager.wait_for_heating(active_extruder)) idle();
  LCD_MESSAGEPGM(" "); 

  // LOADING NEW EXTRUDER
  #if ENABLED(FWRETRACT)
				tool_migration_e_move(RETRACT_LENGTH_SWAP, RETRACT_RECOVER_FEEDRATE_SWAP);
		#else
				tool_migration_e_move(BUCKET_TOOL_MIGRATION_LOAD_LENGTH, BUCKET_TOOL_MIGRATION_LOAD_F);
		#endif
							
  // EXTRUDING / PURGING
  tool_migration_e_move(bucket_purge_length, bucket_purge_feedrate); 
  
		// RETRACT : 10mm retract min , to ensure no oozing and easy separation after cooling
		tool_migration_e_move(-bucket_retract, 
   #if ENABLED(FWRETRACT)
     fwretract.retract_feedrate_mm_s
   #else
     bucket_retract_feedrate
   #endif
			);
   
		// BLOWING FULL POWER : To cold the filament in the bucket 
  #if (BUCKET_FAN_SPEED >0 && BUCKET_FAN < FAN_COUNT)
    fansp=fanSpeeds[BUCKET_FAN];
    fanSpeeds[BUCKET_FAN]=BUCKET_FAN_SPEED ;
  #endif
		
		// PAUSE : Time of filament cooling
	 dwell(bucket_fan_dwell *1000);  
		
		// STOP BLOWING : Resume old fanspeed
		#if (BUCKET_FAN_SPEED >0 && BUCKET_FAN < FAN_COUNT)
    fanSpeeds[BUCKET_FAN]=fansp;
  #endif	  
		
  // BACK TO PRINT : Move XY to starting position, then Z
  do_blocking_move_to_xy(resume_position[X_AXIS], resume_position[Y_AXIS], NOZZLE_PARK_XY_FEEDRATE);

  // BACK TO PRINT : Set Z_AXIS to saved position  
  do_blocking_move_to_z(resume_position[Z_AXIS], NOZZLE_PARK_Z_FEEDRATE);
		
		// INTELLIGENT RESUMING		
		// TRUE ZERO OF THE GEAR : Extruder purged and full now the current extruder position =0 
  planner.set_e_position_mm(0); current_position[E_AXIS] = 0; destination[E_AXIS]=0;
		
		// RESUME FWRETRACT STATUSES : If FWretracted before goto pause 
  #if ENABLED(FWRETRACT)    
    if (fwretract.retracted[active_extruder]) {							
     tool_migration_e_move( - fwretract.retract_length , fwretract.retract_feedrate_mm_s); 
    }
  #endif    

  // RESUME EXACT POSITION OF THE GEAR :Positionning of the gear if old position negative 	
		if (resume_position[E_AXIS]<0) tool_migration_e_move(resume_position[E_AXIS], 
			#if ENABLED(FWRETRACT)
					RETRACT_FEEDRATE
			#else
					BUCKET_TOOL_MIGRATION_UNLOAD_F);
			#endif
		);
		
		// RECOVER : Recover the long retract inside the bucket
		tool_migration_e_move(bucket_retract, 
   #if ENABLED(FWRETRACT)
     fwretract.retract_feedrate_mm_s
   #else
     bucket_retract_feedrate
   #endif
			);

  //Now all extrusion positions are resumed and ready to be confirmed
  //Set extruder to saved position
  planner.set_e_position_mm(destination[E_AXIS] = current_position[E_AXIS] = resume_position[E_AXIS]);
  }			
#endif //BUCKET_FEATURE
		
		
