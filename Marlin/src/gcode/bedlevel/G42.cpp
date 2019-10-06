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
#include "../../module/probe.h" // for probe_offset
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

    destination = current_position;

    if (hasI) destination.x = _GET_MESH_X(ix);
    if (hasJ) destination.y = _GET_MESH_Y(iy);

    #if HAS_BED_PROBE
      if (parser.boolval('P')) {
        if (hasI) destination.x -= probe_offset.x;
        if (hasJ) destination.y -= probe_offset.y;
      }
    #endif

    const feedRate_t fval = parser.linearval('F'),
                     fr_mm_s = fval > 0 ? MMM_TO_MMS(fval) : 0.0f;

    // SCARA kinematic has "safe" XY raw moves
    #if IS_SCARA
      prepare_internal_fast_move_to_destination(fr_mm_s);
    #else
      prepare_internal_move_to_destination(fr_mm_s);
    #endif
  }
}

#endif // HAS_MESH
