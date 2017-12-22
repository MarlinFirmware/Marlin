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
 *  Default values are used for omitted arguments.
 *
 */
void GcodeSuite::M600() {
  point_t park_point = NOZZLE_PARK_POINT;

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
  if (parser.seenval('Z'))
    park_point.z = parser.linearval('Z');

  // Move XY axes to filament change position or given position
  if (parser.seenval('X'))
    park_point.x = parser.linearval('X');

  if (parser.seenval('Y'))
    park_point.y = parser.linearval('Y');

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

  // Resume the print job timer if it was running
  if (job_running) print_job_timer.start();
}

#endif // ADVANCED_PAUSE_FEATURE
