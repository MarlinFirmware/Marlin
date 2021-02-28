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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "../../inc/MarlinConfig.h"

#if ENABLED(G68_G69_ROTATE)

#include "../gcode.h"
#include "../../module/planner.h"

void GcodeSuite::G68() {
  float a = 0.0f, b = 0.0f, r;
  bool a_valid,b_valid;

  if ((a_valid = parser.seenval('A'))) {
    a = parser.value_float();
  }
  if ((b_valid = parser.seenval('B'))) {
    b = parser.value_float();
  }
  if (parser.seenval('R')) {
    r = parser.value_float();
  } else {
    r = planner.g68_rotation.r;
  }
  planner.g68_rotation.set_g68(a, a_valid, b, b_valid, r, true);
}

void GcodeSuite::G69() {
  planner.g68_rotation.reset(true);
}

#endif
