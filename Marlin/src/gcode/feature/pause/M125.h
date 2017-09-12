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

#include "common.h"

/**
 * M125: Store current position and move to filament change position.
 *       Called on pause (by M25) to prevent material leaking onto the
 *       object. On resume (M24) the head will be moved back and the
 *       print will resume.
 *
 *       If Marlin is compiled without SD Card support, M125 can be
 *       used directly to pause the print and move to park position,
 *       resuming with a button click or M108.
 *
 *    L = override retract length
 *    X = override X
 *    Y = override Y
 *    Z = override Z raise
 */
void gcode_M125() {

  // Initial retract before move to filament change position
  const float retract = parser.seen('L') ? parser.value_axis_units(E_AXIS) : 0
    #ifdef PAUSE_PARK_RETRACT_LENGTH
      - (PAUSE_PARK_RETRACT_LENGTH)
    #endif
  ;

  // Lift Z axis
  const float z_lift = parser.linearval('Z')
    #ifdef PAUSE_PARK_Z_ADD
      + PAUSE_PARK_Z_ADD
    #endif
  ;

  // Move XY axes to filament change position or given position
  const float x_pos = parser.linearval('X')
    #ifdef PAUSE_PARK_X_POS
      + PAUSE_PARK_X_POS
    #endif
    #if HOTENDS > 1 && DISABLED(DUAL_X_CARRIAGE)
      + (active_extruder ? hotend_offset[X_AXIS][active_extruder] : 0)
    #endif
  ;
  const float y_pos = parser.linearval('Y')
    #ifdef PAUSE_PARK_Y_POS
      + PAUSE_PARK_Y_POS
    #endif
    #if HOTENDS > 1 && DISABLED(DUAL_X_CARRIAGE)
      + (active_extruder ? hotend_offset[Y_AXIS][active_extruder] : 0)
    #endif
  ;

  #if DISABLED(SDSUPPORT)
    const bool job_running = print_job_timer.isRunning();
  #endif

  if (pause_print(retract, z_lift, x_pos, y_pos)) {
    #if DISABLED(SDSUPPORT)
      // Wait for lcd click or M108
      wait_for_filament_reload();

      // Return to print position and continue
      resume_print();

      if (job_running) print_job_timer.start();
    #endif
  }
}
