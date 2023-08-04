/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2022 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(GLOBAL_MESH_Z_OFFSET)

#include "../gcode.h"
#include "../../module/planner.h"
#include "../../feature/bedlevel/bedlevel.h"

/**
 * M424: Set Global Mesh Z Offset
 */
void GcodeSuite::M424() {

  if (parser.seenval('Z')) {
    const float zval = parser.value_linear_units();
    if (WITHIN(zval, -2, 2)) {
      if (zval != bedlevel.z_base_offset) {
        planner.synchronize();
        bedlevel.z_base_offset = zval; // TODO: Handle the change in position when leveling is on
      }
    }
    else
      SERIAL_ECHOLNPGM("?Z out of range (-2..2)");
  }
  else
    SERIAL_ECHOLNPGM(STR_MESH_Z_OFFSET ": ", bedlevel.z_base_offset);
}

void GcodeSuite::M424_report(const bool forReplay/*=true*/) {
  report_heading_etc(forReplay, F(STR_MESH_Z_OFFSET));
  SERIAL_ECHOLNPGM("  M424 Z", p_float_t(LINEAR_UNIT(bedlevel.z_base_offset), 3));
}

#endif // GLOBAL_MESH_Z_OFFSET
