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

#include "../../../inc/MarlinConfig.h"

#if ENABLED(PAUSE_SPOOL_SWAP)
  #include "../../../module/tool_change.h"
  #include "../../../feature/fwretract.h"
  #include "../../../module/temperature.h" 
  #include "../../../module/planner.h"     
#endif 

#if ENABLED(SMART_PAUSE)

#include "../../../feature/pause.h"
#include "../../gcode.h"
#include "../../../module/motion.h"
#include "../../parser.h"
#include "../../../module/printcounter.h"

/**
 * M600: Pause for filament change
 *
 *  E[distance] - Retract the filament this far (negative value)
 *  Z[distance] - Move the Z axis by this distance
 *  X[position] - Move to this X position, with Y
 *  Y[position] - Move to this Y position, with X
 *  U[distance] - Retract distance for removal (negative value) (manual reload)
 *  L[distance] - Extrude distance for insertion (positive value) (manual reload)
 *  B[count]    - Number of times to beep, -1 for indefinite (if equipped with a buzzer)
 *
 * Spool_Swap : Tool change after filament runout
 *
 *  T[int]: 0=disable : 1/2/3 Enable the max number of swapping
 *
 *  Starting Purge  : Goto pause, extrude and return 
 *                  : Only one instruction to prime nozzle (one by one only)
 *  P[bool] : 0 = disable  : 1 = activated
 *  For ex M600 E0 U0 L0 B0 P1 = goto pause, extrude and return
 *  Default values are used for omitted arguments.
 *
 */
void GcodeSuite::M600() {
	
  #if ENABLED(PAUSE_SPOOL_SWAP) 
	const int swap_spool_tmp = parser.intval('T',(FIL_RUNOUT_SENSORS));             
    
	// M600 T0/T1/2/3/4 Enable/Disable     
	if (swap_spool_tmp<FIL_RUNOUT_SENSORS){
	  if (swap_spool_tmp>0){
	    // Activate or deactivate if next extruder available
	    swap_spool_enabled = true ;
		//Apply max number of swapping 
		swap_spool_stop = swap_spool_tmp;
	  }
	  // Deactivate in all case if no next extruders available
	  else swap_spool_enabled=false; 
	  // Applying number or next extruders
	  
	  return ;  
    }      
  #endif
	
  #if ENABLED(PAUSE_HOME_BEFORE)
    // Don't allow filament change without homing first
    if (axis_unhomed_error()) home_all_axes();
  #endif

  // Initial retract before move to filament change position
  const float retract = parser.seen('E') ? parser.value_axis_units(E_AXIS) : 0
    #ifdef PAUSE_RET
      - (PAUSE_RET)
    #endif
  ;

  // Lift Z axis
  const float z_lift = parser.linearval('Z', 0
    #ifdef PAUSE_Z_ADD
      + PAUSE_Z_ADD
    #endif
  );

  // Move XY axes to filament exchange position
  const float x_pos = parser.linearval('X', 0
    #ifdef PAUSE_X_POS
      + PAUSE_X_POS
	#else
      + current_position[X_AXIS]
    #endif
  );
  const float y_pos = parser.linearval('Y', 0
    #ifdef PAUSE_Y_POS
      + PAUSE_Y_POS
	#else
      + current_position[Y_AXIS]
    #endif	
  );
  
  // Unload filament
  #if ENABLED(PAUSE_SPOOL_SWAP)
      float unload_length = parser.seen('U') ? parser.value_axis_units(E_AXIS) : 0
      #ifdef PAUSE_UNLOAD
        - PAUSE_UNLOAD ;
      if (swap_spool_enabled) unload_length = -SPOOL_SWAP_UNLOAD;
      #endif 
  #else
      const float unload_length = parser.seen('U') ? parser.value_axis_units(E_AXIS) : 0
      #ifdef PAUSE_UNLOAD
        - PAUSE_UNLOAD;    
      #endif 
  #endif//end unload filament
 
  // Load filament
  #if ENABLED(PAUSE_SPOOL_SWAP)
    float load_length = parser.seen('L') ? parser.value_axis_units(E_AXIS) : 0
    #ifdef PAUSE_LOAD
	+ PAUSE_LOAD ;
    if (swap_spool_enabled) load_length = SPOOL_SWAP_LOAD;
    #endif 
  #else    
    const float load_length = parser.seen('L') ? parser.value_axis_units(E_AXIS) : 0
    #ifdef PAUSE_LOAD
	  + PAUSE_LOAD ;
    #endif    
  #endif //end load filament

  const int beep_count = parser.intval('B',
    #ifdef PAUSE_BEEPS
      PAUSE_BEEPS
    #else
      -1
    #endif
  );

  const bool job_running = print_job_timer.isRunning();

  #if ENABLED(PAUSE_SPOOL_SWAP)
    //Starting Purge
   if (parser.intval('P',(FIL_RUNOUT_SENSORS))){
     //Temporary activation to inhibit all menus in resume_print
     bool old_swap_spool_enabled = swap_spool_enabled ;
     swap_spool_enabled = true ;
     //Pause + Resume without LCD
     if (pause_print(0/*retract*/, z_lift, x_pos, y_pos, 0/*unload_length*/, 0/*beep_count*/, false)) {
      resume_print(0/*load_length*/, PAUSE_EXTRUDE, 0/*beep_count*/);
     }
     swap_spool_enabled = old_swap_spool_enabled ;// restore old state
     return;	  
   } ;
    // Next tool	
    if (swap_spool_enabled ) {
		
      pause_print(retract, z_lift, x_pos, y_pos, unload_length, beep_count, false);
	  
	  #if ENABLED(SPOOL_SWAP_FLOW)
	    // Same flow after tool change 'for same nozzle size '
	    planner.flow_percentage[active_extruder+1] = planner.flow_percentage[active_extruder];
	  #endif
	  // Changing tool now
	  tool_change(active_extruder+1, 0, false); 
	  
	  // FwRetract statut 
	  #if ENABLED(FWRETRACT)
	    fwretract.retracted[active_extruder]=fwretract.retracted[active_extruder-1];
	    fwretract.retracted_swap[active_extruder]=fwretract.retracted_swap[active_extruder-1];	    
	  #endif
	  
	  #if HOTENDS>1   //If no single nozzle  
 	    // Heat the new , cold the old  
	    thermalManager.setTargetHotend(thermalManager.target_temperature[active_extruder-1], active_extruder);
	    thermalManager.setTargetHotend(0, active_extruder-1);
	  #endif
	  
      resume_print(load_length, SPOOL_SWAP_EXTRUDE, beep_count);
	  
      //Managing if swap next time
	  if (active_extruder >= (EXTRUDERS-1)) swap_spool_enabled = false ;
	  if (swap_spool_stop <= active_extruder) swap_spool_enabled = false ;
      	  
    }//end next tool
    else 
     if (pause_print(retract, z_lift, x_pos, y_pos, unload_length, beep_count, true)) {
     wait_for_filament_reload(beep_count);
     resume_print(load_length, PAUSE_EXTRUDE, beep_count);       
    }      
  #else 
        if (pause_print(retract, z_lift, x_pos, y_pos, unload_length, beep_count, true)) {
        wait_for_filament_reload(beep_count);
        resume_print(load_length, PAUSE_EXTRUDE, beep_count);
        } 
  #endif

  // Resume the print job timer if it was running
  if (job_running) print_job_timer.start();
}

#endif // SMART_PAUSE
