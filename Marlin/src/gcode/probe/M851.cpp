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

#if HAS_BED_PROBE

#include "../gcode.h"
#include "../../feature/bedlevel/bedlevel.h"
#include "../../module/probe.h"

void GcodeSuite::M851() {
  SERIAL_ECHO_START();
  SERIAL_ECHOPGM(MSG_PROBE_Z_OFFSET);
  if (parser.seen('Z')) {
    const float value = parser.value_linear_units();
    if (!WITHIN(value, Z_PROBE_OFFSET_RANGE_MIN, Z_PROBE_OFFSET_RANGE_MAX)) {
      SERIAL_ECHOLNPGM(" " MSG_Z_MIN " " STRINGIFY(Z_PROBE_OFFSET_RANGE_MIN) " " MSG_Z_MAX " " STRINGIFY(Z_PROBE_OFFSET_RANGE_MAX));
      return;
    }
    zprobe_zoffset = value;
  }
  SERIAL_ECHOLNPAIR(": ", zprobe_zoffset);
}

#endif // HAS_BED_PROBE
