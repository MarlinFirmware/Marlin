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

#include "../../inc/MarlinConfig.h"

#if HOTENDS > 1

#include "../gcode.h"
#include "../../module/motion.h"

/**
 * M218 - set hotend offset (in linear units)
 *
 *   T<tool>
 *   X<xoffset>
 *   Y<yoffset>
 *   Z<zoffset> - Available with DUAL_X_CARRIAGE and SWITCHING_NOZZLE
 */
void GcodeSuite::M218() {
  if (get_target_extruder_from_command() || target_extruder == 0) return;

  bool report = true;
  if (parser.seenval('X')) {
    hotend_offset[X_AXIS][target_extruder] = parser.value_linear_units();
    report = false;
  }
  if (parser.seenval('Y')) {
    hotend_offset[Y_AXIS][target_extruder] = parser.value_linear_units();
    report = false;
  }

  #if ENABLED(DUAL_X_CARRIAGE) || ENABLED(SWITCHING_NOZZLE) || ENABLED(PARKING_EXTRUDER)
    if (parser.seenval('Z')) {
      hotend_offset[Z_AXIS][target_extruder] = parser.value_linear_units();
      report = false;
    }
  #endif

  if (report) {
    SERIAL_ECHO_START();
    SERIAL_ECHOPGM(MSG_HOTEND_OFFSET);
    HOTEND_LOOP() {
      SERIAL_CHAR(' ');
      SERIAL_ECHO(hotend_offset[X_AXIS][e]);
      SERIAL_CHAR(',');
      SERIAL_ECHO(hotend_offset[Y_AXIS][e]);
      #if ENABLED(DUAL_X_CARRIAGE) || ENABLED(SWITCHING_NOZZLE) || ENABLED(PARKING_EXTRUDER)
        SERIAL_CHAR(',');
        SERIAL_ECHO(hotend_offset[Z_AXIS][e]);
      #endif
    }
    SERIAL_EOL();
  }

  #if ENABLED(DELTA)
    if (target_extruder == active_extruder)
      do_blocking_move_to_xy(current_position[X_AXIS], current_position[Y_AXIS], planner.max_feedrate_mm_s[X_AXIS]);
  #endif
}

#endif // HOTENDS > 1
