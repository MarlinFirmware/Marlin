/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * M421.cpp - Auto Bed Leveling
 */

#include "../../../inc/MarlinConfig.h"

#if ENABLED(AUTO_BED_LEVELING_BILINEAR)

#include "../../gcode.h"
#include "../../../feature/bedlevel/bedlevel.h"

#if ENABLED(EXTENSIBLE_UI)
  #include "../../../lcd/extui/ui_api.h"
#endif

/**
 * M421: Set a single Mesh Bed Leveling Z coordinate
 *
 * Usage:
 *   M421 I<xindex> J<yindex> Z<linear>
 *   M421 I<xindex> J<yindex> Q<offset>
 */
void GcodeSuite::M421() {
  int8_t ix = parser.intval('I', -1), iy = parser.intval('J', -1);
  const bool hasI = ix >= 0,
             hasJ = iy >= 0,
             hasZ = parser.seen('Z'),
             hasQ = !hasZ && parser.seen('Q');

  if (!hasI || !hasJ || !(hasZ || hasQ))
    SERIAL_ERROR_MSG(STR_ERR_M421_PARAMETERS);
  else if (!WITHIN(ix, 0, GRID_MAX_POINTS_X - 1) || !WITHIN(iy, 0, GRID_MAX_POINTS_Y - 1))
    SERIAL_ERROR_MSG(STR_ERR_MESH_XY);
  else {
    z_values[ix][iy] = parser.value_linear_units() + (hasQ ? z_values[ix][iy] : 0);
    TERN_(ABL_BILINEAR_SUBDIVISION, bed_level_virt_interpolate());
    TERN_(EXTENSIBLE_UI, ExtUI::onMeshUpdate(ix, iy, z_values[ix][iy]));
  }
}

#endif // AUTO_BED_LEVELING_BILINEAR
