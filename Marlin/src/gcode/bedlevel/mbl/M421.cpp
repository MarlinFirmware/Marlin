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

/**
 * M421.cpp - Mesh Bed Leveling
 */

#include "../../../inc/MarlinConfig.h"

#if ENABLED(MESH_BED_LEVELING)

#include "../../gcode.h"
#include "../../../module/motion.h"
#include "../../../feature/bedlevel/mbl/mesh_bed_leveling.h"

/**
 * M421: Set a single Mesh Bed Leveling Z coordinate
 *
 * Usage:
 *   M421 X<linear> Y<linear> Z<linear>
 *   M421 X<linear> Y<linear> Q<offset>
 *   M421 I<xindex> J<yindex> Z<linear>
 *   M421 I<xindex> J<yindex> Q<offset>
 */
void GcodeSuite::M421() {
  const bool hasX = parser.seen('X'), hasI = parser.seen('I');
  const int8_t ix = hasI ? parser.value_int() : hasX ? mbl.probe_index_x(RAW_X_POSITION(parser.value_linear_units())) : -1;
  const bool hasY = parser.seen('Y'), hasJ = parser.seen('J');
  const int8_t iy = hasJ ? parser.value_int() : hasY ? mbl.probe_index_y(RAW_Y_POSITION(parser.value_linear_units())) : -1;
  const bool hasZ = parser.seen('Z'), hasQ = !hasZ && parser.seen('Q');

  if (int(hasI && hasJ) + int(hasX && hasY) != 1 || !(hasZ || hasQ)) {
    SERIAL_ERROR_START();
    SERIAL_ERRORLNPGM(MSG_ERR_M421_PARAMETERS);
  }
  else if (ix < 0 || iy < 0) {
    SERIAL_ERROR_START();
    SERIAL_ERRORLNPGM(MSG_ERR_MESH_XY);
  }
  else
    mbl.set_z(ix, iy, parser.value_linear_units() + (hasQ ? mbl.z_values[ix][iy] : 0));
}

#endif // MESH_BED_LEVELING
