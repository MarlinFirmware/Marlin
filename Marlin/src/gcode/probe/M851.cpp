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

void GcodeSuite::M851() {
  if (parser.seenval('Z')) {
    const float value = parser.value_linear_units();
    if (WITHIN(value, Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX))
      zprobe_offset[Z_AXIS] = value;
    else
      SERIAL_ERROR_MSG("?Z out of range (" STRINGIFY(Z_PROBE_OFFSET_RANGE_MIN) " to " STRINGIFY(Z_PROBE_OFFSET_RANGE_MAX) ")");
    return;
  }
  if (parser.seenval('X')) {
    const float value = parser.value_linear_units();
    if (WITHIN(value, 0-X_BED_SIZE, X_BED_SIZE))
      zprobe_offset[X_AXIS] = value;
    else
      SERIAL_ERROR_MSG("?X out of range (" STRINGIFY(0-X_BED_SIZE) " to " STRINGIFY(X_BED_SIZE) ")");
    return;
  }
  if (parser.seenval('Y')) {
    const float value = parser.value_linear_units();
    if (WITHIN(value, 0-Y_BED_SIZE, Y_BED_SIZE))
      zprobe_offset[Y_AXIS] = value;
    else
      SERIAL_ERROR_MSG("?Y out of range (" STRINGIFY(0-Y_BED_SIZE) " to " STRINGIFY(Y_BED_SIZE) ")");
    return;
  }
  SERIAL_ECHO_START();
  SERIAL_ECHOLNPAIR(MSG_PROBE_X_OFFSET ": ", zprobe_offset[X_AXIS]);
  SERIAL_ECHOLNPAIR(MSG_PROBE_Y_OFFSET ": ", zprobe_offset[Y_AXIS]);
  SERIAL_ECHOLNPAIR(MSG_PROBE_Z_OFFSET ": ", zprobe_offset[Z_AXIS]);
}

#endif // HAS_BED_PROBE
