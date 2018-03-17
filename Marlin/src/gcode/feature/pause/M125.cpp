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

#if ENABLED(PARK_HEAD_ON_PAUSE)

#include "../../gcode.h"
#include "../../parser.h"
#include "../../../feature/pause.h"
#include "../../../module/motion.h"

#if DISABLED(SDSUPPORT)
  #include "../../../module/printcounter.h"
#endif

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
void GcodeSuite::M125() {
  // Initial retract before move to filament change position
  const float retract = -FABS(parser.seen('L') ? parser.value_axis_units(E_AXIS) : 0
    #ifdef PAUSE_PARK_RETRACT_LENGTH
      + (PAUSE_PARK_RETRACT_LENGTH)
    #endif
  );

  point_t park_point = NOZZLE_PARK_POINT;

  // Move XY axes to filament change position or given position
  if (parser.seenval('X')) park_point.x = parser.linearval('X');
  if (parser.seenval('Y')) park_point.y = parser.linearval('Y');

  // Lift Z axis
  if (parser.seenval('Z')) park_point.z = parser.linearval('Z');

  #if HOTENDS > 1 && DISABLED(DUAL_X_CARRIAGE) && DISABLED(DELTA)
    park_point.x += (active_extruder ? hotend_offset[X_AXIS][active_extruder] : 0);
    park_point.y += (active_extruder ? hotend_offset[Y_AXIS][active_extruder] : 0);
  #endif

  #if DISABLED(SDSUPPORT)
    const bool job_running = print_job_timer.isRunning();
  #endif

  if (pause_print(retract, park_point)) {
    #if DISABLED(SDSUPPORT)
      // Wait for lcd click or M108
      wait_for_filament_reload();

      // Return to print position and continue
      resume_print();

      if (job_running) print_job_timer.start();
    #endif
  }
}

#endif // PARK_HEAD_ON_PAUSE
