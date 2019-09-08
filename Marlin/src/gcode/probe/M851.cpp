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

/**
 * M851: Set the nozzle-to-probe offsets in current units
 */
void GcodeSuite::M851() {

  // Show usage with no parameters
  if (!parser.seen("XYZ")) {
    SERIAL_ECHO_START();
    SERIAL_ECHOLNPAIR(MSG_PROBE_OFFSET " X", zprobe_offset[X_AXIS],
                                       " Y", zprobe_offset[Y_AXIS],
                                       " Z", zprobe_offset[Z_AXIS]);
    return;
  }

  // Get the modified offsets
  const float offs[] = {
    parser.floatval('X', zprobe_offset[X_AXIS]),
    parser.floatval('Y', zprobe_offset[Y_AXIS]),
    parser.floatval('Z', zprobe_offset[Z_AXIS])
  };

  // Error-check
  if (!WITHIN(offs[X_AXIS], -(X_BED_SIZE), X_BED_SIZE)) {
    SERIAL_ERROR_MSG("?X out of range (-" STRINGIFY(X_BED_SIZE) " to " STRINGIFY(X_BED_SIZE) ")");
    return;
  }
  if (!WITHIN(offs[Y_AXIS], -(Y_BED_SIZE), Y_BED_SIZE)) {
    SERIAL_ERROR_MSG("?Y out of range (-" STRINGIFY(Y_BED_SIZE) " to " STRINGIFY(Y_BED_SIZE) ")");
    return;
  }
  if (!WITHIN(offs[Z_AXIS], Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX)) {
    SERIAL_ERROR_MSG("?Z out of range (" STRINGIFY(Z_PROBE_OFFSET_RANGE_MIN) " to " STRINGIFY(Z_PROBE_OFFSET_RANGE_MAX) ")");
    return;
  }

  // Save the new offsets
  LOOP_XYZ(a) zprobe_offset[a] = offs[a];
}

#endif // HAS_BED_PROBE
