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
  bool sync_needed = false;
  if (parser.seenval('A')) {
    sync_needed = planner.g68_rotation.setA(parser.value_float());
  }
  if (parser.seenval('B')) {
    sync_needed = sync_needed || planner.g68_rotation.setB(parser.value_float());
  }
  if (parser.seenval('R')) {
    sync_needed = sync_needed || planner.g68_rotation.setR(parser.value_float());
  }
  if (DEBUGGING(INFO)) planner.g68_rotation.report_rotation();
  if (sync_needed) planner.g68_rotation.update_current_position();
}

void GcodeSuite::G69() {
  if (planner.g68_rotation.reset()) {
    if (DEBUGGING(INFO)) planner.g68_rotation.report_rotation();
    planner.g68_rotation.update_current_position();
  } else {
    if (DEBUGGING(INFO)) planner.g68_rotation.report_rotation();
  }
}

#endif
