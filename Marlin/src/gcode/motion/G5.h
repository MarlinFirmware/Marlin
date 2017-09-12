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

#include "../../module/planner_bezier.h"
#include "../../gcode/gcode.h"

void plan_cubic_move(const float offset[4]) {
  cubic_b_spline(current_position, destination, offset, MMS_SCALED(feedrate_mm_s), active_extruder);

  // As far as the parser is concerned, the position is now == destination. In reality the
  // motion control system might still be processing the action and the real tool position
  // in any intermediate location.
  set_current_to_destination();
}

/**
 * Parameters interpreted according to:
 * http://linuxcnc.org/docs/2.6/html/gcode/parser.html#sec:G5-Cubic-Spline
 * However I, J omission is not supported at this point; all
 * parameters can be omitted and default to zero.
 */

/**
 * G5: Cubic B-spline
 */
void gcode_G5() {
  if (IsRunning()) {

    #if ENABLED(CNC_WORKSPACE_PLANES)
      if (workspace_plane != PLANE_XY) {
        SERIAL_ERROR_START();
        SERIAL_ERRORLNPGM(MSG_ERR_BAD_PLANE_MODE);
        return;
      }
    #endif

    gcode.get_destination_from_command();

    const float offset[] = {
      parser.linearval('I'),
      parser.linearval('J'),
      parser.linearval('P'),
      parser.linearval('Q')
    };

    plan_cubic_move(offset);
  }
}
