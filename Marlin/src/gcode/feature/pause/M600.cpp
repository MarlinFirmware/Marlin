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

#include "../../gcode.h"
#include "../../../feature/pause.h"
#include "../../../module/motion.h"
#include "../../../module/printcounter.h"

#if EXTRUDERS > 1
  #include "../../../module/tool_change.h"
#endif

#if ENABLED(SINGLENOZZLE_SPOOL_SWAP)
  #include "../../../module/tool_change.h"
  #include "../../../feature/fwretract.h"
  #include "../../../module/temperature.h" 
  #include "../../../module/planner.h"
		#include "../../../module/stepper.h"   
  
  bool swap_spool_enabled = 0;
  int16_t swap_spool_stop = 1;
		int16_t fansp=fanSpeeds[SPOOL_SWAP_FAN];

		// Extrude function
		void swap_spool_e_move(const float &length, const float fr) {
				current_position[E_AXIS] += length / planner.e_factor[active_extruder];
				set_destination_from_current();
				#if IS_KINEMATIC
						planner.buffer_line_kinematic(destination, fr, active_extruder);
				#else
						buffer_line_to_destination(fr);
				#endif
				stepper.synchronize();
		}
#endif

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
 *  T[toolhead] - Select extruder for filament change
 *
 *  Default values are used for omitted arguments.
 *
 */
void GcodeSuite::M600() {
  #if ENABLED(SINGLENOZZLE_SPOOL_SWAP)
    const int swap_spool_tmp = parser.intval('S',(FIL_RUNOUT_SENSORS));  
    // Swap Spool managing 
    // Applying number or next extruders
    // M600 S0/S1/2/3/4 Enable/Disable     
    if (swap_spool_tmp<FIL_RUNOUT_SENSORS) {
      if (swap_spool_tmp>0) {
        // Activate or deactivate if next extruder available
        swap_spool_enabled = true ;
        //Apply max number of swapping 
        swap_spool_stop = swap_spool_tmp;
      }
      // Deactivate in all case if no next extruders available
      else {
							swap_spool_enabled=false;
							
							#if FIL_RUNOUT_SENSORS <2 // not usefull if only 2 extruders because only one possibility of swapping
       swap_spool_stop = 1;
							#endif
						}
      return ;  
    } 
				
				#if ENABLED(HOME_BEFORE_FILAMENT_CHANGE)
    // Don't allow filament change without homing first
    if (axis_unhomed_error()) home_all_axes();
				#endif
				
    // Next tool 	
    if (swap_spool_enabled) {
						// Init
						stepper.synchronize();
						const int swap_spool_tmp = parser.intval('S',(FIL_RUNOUT_SENSORS));             
						float resume_position[XYZE];
						int16_t fansp=fanSpeeds[SPOOL_SWAP_FAN];
						int16_t swap_spool_stop=0;

						// Save current position
						COPY(resume_position, current_position);

						// Same flow after tool change
						planner.flow_percentage[active_extruder+1] = planner.flow_percentage[active_extruder];
										
						// If negative position then Adjustment of the unload length
						#if ENABLED(FWRETRACT)	
								if (fwretract.retracted[active_extruder]) {																		
										swap_spool_e_move( - fwretract.swap_retract_length 
												+ fwretract.retracted[active_extruder] ? fwretract.retract_length : 0 
												, fwretract.retract_feedrate_mm_s); 
								}							
						#else							
								if (resume_position[E_AXIS]<0) 
										swap_spool_e_move( - SPOOL_SWAP_LOAD_LENGTH - resume_position[E_AXIS], FILAMENT_CHANGE_UNLOAD_FEEDRATE);								
								else swap_spool_e_move(-SPOOL_SWAP_LOAD_LENGTH, FILAMENT_CHANGE_UNLOAD_FEEDRATE);
						#endif						
      						
						// Park the nozzle by moving up by z_lift and then moving to (x_pos, y_pos)
						Nozzle::park(2, NOZZLE_PARK_POINT);						
				
						// Set the new active extruder
						active_extruder++;

						// Load filament
						swap_spool_e_move(SPOOL_SWAP_LOAD_LENGTH, FILAMENT_CHANGE_LOAD_FEEDRATE);

						// Start blowing
						#if (SPOOL_SWAP_FANSPEED >0 && SPOOL_SWAP_FAN < FAN_COUNT)
								fansp=fanSpeeds[SPOOL_SWAP_FAN];
								fanSpeeds[SPOOL_SWAP_FAN]=SPOOL_SWAP_FANSPEED ;
						#endif

						// Extrusion/purge
						swap_spool_e_move(SPOOL_SWAP_EXTRUDE_LENGTH, ADVANCED_PAUSE_EXTRUDE_FEEDRATE);  
						// Extruder purged = initialisation = 0
						planner.set_e_position_mm(0); current_position[E_AXIS] = 0; destination[E_AXIS]=0;

						// Resume blowing after delay
						#ifdef SPOOL_SWAP_DWELL
								delay(SPOOL_SWAP_DWELL);
						#endif
						#if (SPOOL_SWAP_FANSPEED >0 && SPOOL_SWAP_FAN < FAN_COUNT)
								fanSpeeds[SPOOL_SWAP_FAN]=fansp;
						#endif
											
						// Intelligent resuming						
						#if ENABLED(FWRETRACT)
						// If retracted before goto pause
						if (fwretract.retracted[active_extruder]) {																		
										swap_spool_e_move( - fwretract.retract_length , fwretract.retract_feedrate_mm_s); 
								}
						#else    
								// If resume_position negative
								if (resume_position[E_AXIS]<0) swap_spool_e_move(resume_position[E_AXIS], PAUSE_PARK_RETRACT_FEEDRATE);
						#endif
						
						// Move XY to starting position, then Z
						do_blocking_move_to_xy(resume_position[X_AXIS], resume_position[Y_AXIS], NOZZLE_PARK_XY_FEEDRATE);

						//Set Z_AXIS to saved position  
						do_blocking_move_to_z(resume_position[Z_AXIS], NOZZLE_PARK_Z_FEEDRATE);
						
						//Now all extrusion positions are resumed and ready to be confirmed
						//Set extruder to saved position
						planner.set_e_position_mm(destination[E_AXIS] = current_position[E_AXIS] = resume_position[E_AXIS]);

						//Managing if swap next time
						if (active_extruder >= (EXTRUDERS-1)) swap_spool_enabled = false ;
						if (swap_spool_stop <= active_extruder) swap_spool_enabled = false ;
						
						return;
				}     
		#endif //end SPOOL_SWAP
  
  point_t park_point = NOZZLE_PARK_POINT;

  #if ENABLED(HOME_BEFORE_FILAMENT_CHANGE)
    // Don't allow filament change without homing first
    if (axis_unhomed_error()) home_all_axes();
  #endif

  #if EXTRUDERS > 1
    // Change toolhead if specified
    uint8_t active_extruder_before_filament_change = -1;
    if (parser.seen('T')) {
      const uint8_t extruder = parser.value_byte();
      if (active_extruder != extruder) {
        active_extruder_before_filament_change = active_extruder;
        tool_change(extruder, 0, true);
      }
    }
  #endif

  // Initial retract before move to filament change position
  const float retract = parser.seen('E') ? parser.value_axis_units(E_AXIS) : 0
    #ifdef PAUSE_PARK_RETRACT_LENGTH
      - (PAUSE_PARK_RETRACT_LENGTH)
    #endif
  ;

  // Move XY axes to filament change position or given position
  if (parser.seenval('X')) park_point.x = parser.linearval('X');
  if (parser.seenval('Y')) park_point.y = parser.linearval('Y');

  // Lift Z axis
  if (parser.seenval('Z')) park_point.z = parser.linearval('Z');

  #if HOTENDS > 1 && DISABLED(DUAL_X_CARRIAGE)
    park_point.x += (active_extruder ? hotend_offset[X_AXIS][active_extruder] : 0);
    park_point.y += (active_extruder ? hotend_offset[Y_AXIS][active_extruder] : 0);
  #endif

  // Unload filament
  const float unload_length = parser.seen('U') ? parser.value_axis_units(E_AXIS) : 0
    #if defined(FILAMENT_CHANGE_UNLOAD_LENGTH) && FILAMENT_CHANGE_UNLOAD_LENGTH > 0
      - (FILAMENT_CHANGE_UNLOAD_LENGTH)
    #endif
  ;

  // Load filament
  const float load_length = parser.seen('L') ? parser.value_axis_units(E_AXIS) : 0
    #ifdef FILAMENT_CHANGE_LOAD_LENGTH
      + FILAMENT_CHANGE_LOAD_LENGTH
    #endif
  ;

  const int beep_count = parser.intval('B',
    #ifdef FILAMENT_CHANGE_NUMBER_OF_ALERT_BEEPS
      FILAMENT_CHANGE_NUMBER_OF_ALERT_BEEPS
    #else
      -1
    #endif
  );

  const bool job_running = print_job_timer.isRunning();

  if (pause_print(retract, park_point, unload_length, beep_count, true)) {
    wait_for_filament_reload(beep_count);
    resume_print(load_length, ADVANCED_PAUSE_EXTRUDE_LENGTH, beep_count);
  }

  #if EXTRUDERS > 1
    // Restore toolhead if it was changed
    if (active_extruder_before_filament_change >= 0)
      tool_change(active_extruder_before_filament_change, 0, true);
  #endif

  // Resume the print job timer if it was running
  if (job_running) print_job_timer.start();
}

#endif // ADVANCED_PAUSE_FEATURE
