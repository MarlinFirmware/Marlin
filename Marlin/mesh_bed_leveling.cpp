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

#include "mesh_bed_leveling.h"

#if ENABLED(MESH_BED_LEVELING)

  mesh_bed_leveling mbl;

  mesh_bed_leveling::mesh_bed_leveling() { reset(); }

  void mesh_bed_leveling::reset() {
    active = 0;
    z_offset = 0;
    for (int y = 0; y < MESH_NUM_Y_POINTS; y++)
      for (int x = 0; x < MESH_NUM_X_POINTS; x++)
        z_values[y][x] = 0;
  }

#endif  // MESH_BED_LEVELING
