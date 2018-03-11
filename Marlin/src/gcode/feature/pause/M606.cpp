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

#if ENABLED(DYNAMIC_TOOL_MIGRATION)
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
			
  int tool_migration_last_target = 0;
  // Extrude function
  void tool_migration_e_move(const float length, const float fr) {
    set_destination_from_current();
    destination[E_AXIS] += length / planner.e_factor[active_extruder];
    planner.buffer_line_kinematic(destination, fr, active_extruder);
    stepper.synchronize();
    set_current_from_destination();
  }
#endif

/**
 * M606: Tool Migration
 *
 *  L[count] - Last target tool - Set the last extruder for migration : 0 for disabling
 *  T[count] - Desired target tool - Force the migration to desired extruder
 *  Default  - Automatic migration on next extruder available
 */
	
#if ENABLED(DYNAMIC_TOOL_MIGRATION) 

 void GcodeSuite::M606() {
		  
  const int last_target_tmp = parser.intval('L',(EXTRUDERS));
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

  // Here the process continue if a desired migration :M600 T<...> or automatic :M606
  // Migration beginning
  float resume_position[XYZE];
  int16_t fansp=fanSpeeds[DYNAMIC_TOOL_MIGRATION_FAN];
  stepper.synchronize();
  // Save current position
  COPY(resume_position, current_position);

  if (automatic_migration_enabled){ 
   // If automatic next tool
   //Same temperature
   thermalManager.setTargetHotend(thermalManager.degHotend(active_extruder), active_extruder+1);
   // Same flow after tool change
   planner.flow_percentage[active_extruder+1] = planner.flow_percentage[active_extruder];				
   // Same FwRetract/Swap statuses
   fwretract.retracted[active_extruder+1] = fwretract.retracted[active_extruder];
   fwretract.retracted_swap[active_extruder+1] = fwretract.retracted_swap[active_extruder];
  }
  else {
   // If a desired tool is required
   //Same temperature
   thermalManager.setTargetHotend(thermalManager.degHotend(active_extruder), desired_target_tmp);
   // Same flow after tool change
   planner.flow_percentage[desired_target_tmp] = planner.flow_percentage[active_extruder];
   // Same FwRetract/Swap statuses
   fwretract.retracted[desired_target_tmp] = fwretract.retracted[active_extruder];
   fwretract.retracted_swap[desired_target_tmp] = fwretract.retracted_swap[active_extruder];
  }

  // Because no human intervention , all must be perfect, no wasted distances 
  // If negative position or FwRetracted  then adjustment of the unload length
  // Can just be retracted and not swapped , because no dual extrusion printing with tool migration
  #if ENABLED(FWRETRACT)	
    if (fwretract.retracted[active_extruder]) {																		
     tool_migration_e_move( -DYNAMIC_TOOL_MIGRATION_UNLOAD_LENGTH + fwretract.retract_length, DYNAMIC_TOOL_MIGRATION_UNLOAD_F); 
    }	
    else {
     if (resume_position[E_AXIS]<0) 
      tool_migration_e_move( -DYNAMIC_TOOL_MIGRATION_UNLOAD_LENGTH - resume_position[E_AXIS], DYNAMIC_TOOL_MIGRATION_UNLOAD_F);								
     else tool_migration_e_move(-DYNAMIC_TOOL_MIGRATION_UNLOAD_LENGTH, DYNAMIC_TOOL_MIGRATION_UNLOAD_F);
    }
  #else							
    if (resume_position[E_AXIS]<0) 
     tool_migration_e_move(-DYNAMIC_TOOL_MIGRATION_UNLOAD_LENGTH - resume_position[E_AXIS], DYNAMIC_TOOL_MIGRATION_UNLOAD_F);								
    else tool_migration_e_move(-DYNAMIC_TOOL_MIGRATION_UNLOAD_LENGTH, DYNAMIC_TOOL_MIGRATION_UNLOAD_F);	
  #endif					

  // Park the nozzle by moving up by z_lift and then moving to (x_pos, y_pos)
  #if ENABLED(DYNAMIC_TOOL_MIGRATION_NOZZLE_PARK) && ENABLED(NOZZLE_PARK_FEATURE)
    Nozzle::park(2, NOZZLE_PARK_POINT);
  #endif

  // Set the new active extruder
  if (automatic_migration_enabled) active_extruder++;
  else active_extruder = desired_target_tmp;

  //Ensure_safe_temperature	
  LCD_MESSAGEPGM(MSG_FILAMENT_CHANGE_HEATING_1);    
  while (thermalManager.wait_for_heating(active_extruder)) idle();
  LCD_MESSAGEPGM(" "); 

  // Load filament
  tool_migration_e_move(DYNAMIC_TOOL_MIGRATION_LOAD_LENGTH, DYNAMIC_TOOL_MIGRATION_LOAD_F);

  // Start blowing
  #if (DYNAMIC_TOOL_MIGRATION_FANSPEED >0 && DYNAMIC_TOOL_MIGRATION_FAN < FAN_COUNT)
    fansp=fanSpeeds[DYNAMIC_TOOL_MIGRATION_FAN];
    fanSpeeds[DYNAMIC_TOOL_MIGRATION_FAN]=DYNAMIC_TOOL_MIGRATION_FANSPEED ;
  #endif

  // Extrusion/purge
  tool_migration_e_move(DYNAMIC_TOOL_MIGRATION_EXTRUDE_LENGTH, DYNAMIC_TOOL_MIGRATION_EXTRUDE_F);  

  // Resume blowing after delay
  #ifdef DYNAMIC_TOOL_MIGRATION_DWELL
    delay(DYNAMIC_TOOL_MIGRATION_DWELL);
  #endif

  #if (DYNAMIC_TOOL_MIGRATION_FANSPEED >0 && DYNAMIC_TOOL_MIGRATION_FAN < FAN_COUNT)
    fanSpeeds[DYNAMIC_TOOL_MIGRATION_FAN]=fansp;
  #endif

  // Intelligent resuming						
  #if ENABLED(FWRETRACT)
    // If retracted before goto pause 
    if (fwretract.retracted[active_extruder]) {							
     tool_migration_e_move( - fwretract.retract_length , fwretract.retract_feedrate_mm_s); 
    }
  #endif  

  // Extruder purged and full & FwRetracted positionned ,now the current extruder position =0 
  planner.set_e_position_mm(0); current_position[E_AXIS] = 0; destination[E_AXIS]=0;	

  // Positionning of the gear if old resume_position negative 
  if (resume_position[E_AXIS]<0) tool_migration_e_move(resume_position[E_AXIS], DYNAMIC_TOOL_MIGRATION_UNLOAD_F);

  // Move XY to starting position, then Z
  do_blocking_move_to_xy(resume_position[X_AXIS], resume_position[Y_AXIS], NOZZLE_PARK_XY_FEEDRATE);

  //Set Z_AXIS to saved position  
  do_blocking_move_to_z(resume_position[Z_AXIS], NOZZLE_PARK_Z_FEEDRATE);

  //Now all extrusion positions are resumed and ready to be confirmed
  //Set extruder to saved position
  planner.set_e_position_mm(destination[E_AXIS] = current_position[E_AXIS] = resume_position[E_AXIS]);
  }			
#endif //DYNAMIC_TOOL_MIGRATION
		
		
