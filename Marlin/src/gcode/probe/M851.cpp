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

#if HAS_BED_PROBE

#include "../gcode.h"
#include "../../feature/bedlevel/bedlevel.h"
#include "../../module/probe.h"

extern const char SP_Y_STR[], SP_Z_STR[];

/**
 * M851: Set the nozzle-to-probe offsets in current units
 */
void GcodeSuite::M851() {

  // Show usage with no parameters
  if (!parser.seen("XYZ")) {
    SERIAL_ECHOLNPAIR_P(
      #if HAS_PROBE_XY_OFFSET
        PSTR(MSG_PROBE_OFFSET " X"), probe_offset.x, SP_Y_STR, probe_offset.y, SP_Z_STR
      #else
        PSTR(MSG_PROBE_OFFSET " X0 Y0 Z")
      #endif
      , probe_offset.z
    );
    return;
  }

  // Start with current offsets and modify
  xyz_pos_t offs = probe_offset;

  // Assume no errors
  bool ok = true;

  if (parser.seenval('X')) {
    const float x = parser.value_float();
    #if HAS_PROBE_XY_OFFSET
      if (WITHIN(x, -(X_BED_SIZE), X_BED_SIZE))
        offs.x = x;
      else {
        SERIAL_ECHOLNPAIR("?X out of range (-", int(X_BED_SIZE), " to ", int(X_BED_SIZE), ")");
        ok = false;
      }
    #else
      if (x) SERIAL_ECHOLNPAIR("?X must be 0 (NOZZLE_AS_PROBE)."); // ...but let 'ok' stay true
    #endif
  }

  if (parser.seenval('Y')) {
    const float y = parser.value_float();
    #if HAS_PROBE_XY_OFFSET
      if (WITHIN(y, -(Y_BED_SIZE), Y_BED_SIZE))
        offs.y = y;
      else {
        SERIAL_ECHOLNPAIR("?Y out of range (-", int(Y_BED_SIZE), " to ", int(Y_BED_SIZE), ")");
        ok = false;
      }
    #else
      if (y) SERIAL_ECHOLNPAIR("?Y must be 0 (NOZZLE_AS_PROBE)."); // ...but let 'ok' stay true
    #endif
  }

  if (parser.seenval('Z')) {
    const float z = parser.value_float();
    if (WITHIN(z, Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX))
      offs.z = z;
    else {
      SERIAL_ECHOLNPAIR("?Z out of range (", int(Z_PROBE_OFFSET_RANGE_MIN), " to ", int(Z_PROBE_OFFSET_RANGE_MAX), ")");
      ok = false;
    }
  }

  // Save the new offsets
  if (ok) probe_offset = offs;
}

#endif // HAS_BED_PROBE
