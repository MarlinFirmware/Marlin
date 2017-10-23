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

#if ENABLED(ADVANCED_PAUSE_FEATURE)

#include "../../../module/tool_change.h"
#include "../../../feature/pause.h"
#include "../../../module/temperature.h"//steeve
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
 * M600: Next tool after filament runout or M600
 *  Usefull to finish spools without human intervention
 *  T[bool]     - Enable/disable Next Tool Change on M600 - until the last extruder is reached
 *
 *  Default values are used for omitted arguments.
 *
 */
void GcodeSuite::M600() {

  #if ENABLED(FILAMENT_RUNOUT_NEXT_TOOL) //steeve  
    // M600 T0/T1 Enable/Disable  
    extern bool runout_next_tool_enabled;
    extern bool run_next_sensor_armed ;
	const int runout_next_tool_enabled_tmp = parser.intval('T',2);             
    if (runout_next_tool_enabled_tmp<2){
	  // Activate or deactivate if next extruder available
	  if (active_extruder < (EXTRUDERS-1))runout_next_tool_enabled=runout_next_tool_enabled_tmp ;
	  // Deactivate in all case if no next extruders available
	  else runout_next_tool_enabled=false; 
	  return ;  
    }      
  #endif
	
  #if ENABLED(HOME_BEFORE_FILAMENT_CHANGE)
    // Don't allow filament change without homing first
    if (axis_unhomed_error()) home_all_axes();
  #endif

  // Initial retract before move to filament change position
  const float retract = parser.seen('E') ? parser.value_axis_units(E_AXIS) : 0
    #ifdef PAUSE_PARK_RETRACT_LENGTH
      - (PAUSE_PARK_RETRACT_LENGTH)
    #endif
  ;

  // Lift Z axis
  const float z_lift = parser.linearval('Z', 0
    #ifdef PAUSE_PARK_Z_ADD
      + PAUSE_PARK_Z_ADD
    #endif
  );

  // Move XY axes to filament exchange position
  const float x_pos = parser.linearval('X', 0
    #ifdef PAUSE_PARK_X_POS
      + PAUSE_PARK_X_POS
	#else
      + current_position[X_AXIS]
    #endif
  );
  const float y_pos = parser.linearval('Y', 0
    #ifdef PAUSE_PARK_Y_POS
      + PAUSE_PARK_Y_POS
	#else
      + current_position[Y_AXIS]
    #endif	
  );
  // Unload filament
  #if ENABLED(FILAMENT_RUNOUT_NEXT_TOOL)
      float unload_length = parser.seen('U') ? parser.value_axis_units(E_AXIS) : 0
      #ifdef FILAMENT_CHANGE_UNLOAD_LENGTH
        - FILAMENT_CHANGE_UNLOAD_LENGTH ;
      if (runout_next_tool_enabled) unload_length = -RUNOUT_NEXT_TOOL_UNLOAD_LENGTH;
      #endif 
  #else
      const float unload_length = parser.seen('U') ? parser.value_axis_units(E_AXIS) : 0
      #ifdef FILAMENT_CHANGE_UNLOAD_LENGTH
        - FILAMENT_CHANGE_UNLOAD_LENGTH;    
      #endif 
  #endif//end load filament
 
  // Load filament
  #if ENABLED(FILAMENT_RUNOUT_NEXT_TOOL)
    float load_length = parser.seen('L') ? parser.value_axis_units(E_AXIS) : 0
    #ifdef FILAMENT_CHANGE_LOAD_LENGTH
	+ FILAMENT_CHANGE_LOAD_LENGTH ;
    if (runout_next_tool_enabled) load_length = RUNOUT_NEXT_TOOL_LOAD_LENGTH;
    #endif 
  #else    
  const float load_length = parser.seen('L') ? parser.value_axis_units(E_AXIS) : 0
  #ifdef FILAMENT_CHANGE_LOAD_LENGTH
	+ FILAMENT_CHANGE_LOAD_LENGTH ;
  #endif    
  #endif //end load filament

  const int beep_count = parser.intval('B',
    #ifdef FILAMENT_CHANGE_NUMBER_OF_ALERT_BEEPS
      FILAMENT_CHANGE_NUMBER_OF_ALERT_BEEPS
    #else
      -1
    #endif
  );

  const bool job_running = print_job_timer.isRunning();

  #if ENABLED(FILAMENT_RUNOUT_NEXT_TOOL)
    // Next tool 
    if (runout_next_tool_enabled  ) {
      pause_print(retract, z_lift, x_pos, y_pos, unload_length, beep_count, false);
	  
	  // Heat the new , cold the old if no single nozzle   
	  //active_extruder++;
	  thermalManager.setTargetHotend(thermalManager.target_temperature[active_extruder], active_extruder+1);
      thermalManager.setTargetHotend(0, active_extruder);		  
	  tool_change(active_extruder+1, 0, false);// changing tool now
      	
      resume_print(load_length, RUNOUT_NEXT_TOOL_EXTRUDE_LENGTH, beep_count);
      if (active_extruder >= (EXTRUDERS-1))runout_next_tool_enabled=false ;
	  run_next_sensor_armed = false ;
    }//end next tool
    else 
     if (pause_print(retract, z_lift, x_pos, y_pos, unload_length, beep_count, true)) {
     wait_for_filament_reload(beep_count);
     resume_print(load_length, ADVANCED_PAUSE_EXTRUDE_LENGTH, beep_count);       
    }      
  #else 
        if (pause_print(retract, z_lift, x_pos, y_pos, unload_length, beep_count, true)) {
        wait_for_filament_reload(beep_count);
        resume_print(load_length, ADVANCED_PAUSE_EXTRUDE_LENGTH, beep_count);
        } 
  #endif

  // Resume the print job timer if it was running
  if (job_running) print_job_timer.start();
}

#endif // ADVANCED_PAUSE_FEATURE
