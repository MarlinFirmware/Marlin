/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
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

#if HAS_MESH

#include "../gcode.h"
#include "../../Marlin.h" // for IsRunning()
#include "../../module/motion.h"
#include "../../feature/bedlevel/bedlevel.h"

/**
 * G42: Move X & Y axes to mesh coordinates (I & J)
 */
void GcodeSuite::G42() {
  if (MOTION_CONDITIONS) {
    const bool hasI = parser.seenval('I');
    const int8_t ix = hasI ? parser.value_int() : 0;
    const bool hasJ = parser.seenval('J');
    const int8_t iy = hasJ ? parser.value_int() : 0;

    if ((hasI && !WITHIN(ix, 0, GRID_MAX_POINTS_X - 1)) || (hasJ && !WITHIN(iy, 0, GRID_MAX_POINTS_Y - 1))) {
      SERIAL_ECHOLNPGM(MSG_ERR_MESH_XY);
      return;
    }

    set_destination_from_current();
    if (hasI) destination[X_AXIS] = _GET_MESH_X(ix);
    if (hasJ) destination[Y_AXIS] = _GET_MESH_Y(iy);
    if (parser.boolval('P')) {
      if (hasI) destination[X_AXIS] -= X_PROBE_OFFSET_FROM_EXTRUDER;
      if (hasJ) destination[Y_AXIS] -= Y_PROBE_OFFSET_FROM_EXTRUDER;
    }

    const float fval = parser.linearval('F');
    if (fval > 0.0) feedrate_mm_s = MMM_TO_MMS(fval);

    // SCARA kinematic has "safe" XY raw moves
    #if IS_SCARA
      prepare_uninterpolated_move_to_destination();
    #else
      prepare_move_to_destination();
    #endif
  }
}

#endif // HAS_MESH
