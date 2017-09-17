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

#include "../gcode.h"
#include "../../module/planner.h"

/**
 * M204: Set Accelerations in units/sec^2 (M204 P1200 R3000 T3000)
 *
 *    P = Printing moves
 *    R = Retract only (no X, Y, Z) moves
 *    T = Travel (non printing) moves
 *
 *  Also sets minimum segment time in ms (B20000) to prevent buffer under-runs and M20 minimum feedrate
 */
void GcodeSuite::M204() {
  if (parser.seen('S')) {  // Kept for legacy compatibility. Should NOT BE USED for new developments.
    planner.travel_acceleration = planner.acceleration = parser.value_linear_units();
    SERIAL_ECHOLNPAIR("Setting Print and Travel Acceleration: ", planner.acceleration);
  }
  if (parser.seen('P')) {
    planner.acceleration = parser.value_linear_units();
    SERIAL_ECHOLNPAIR("Setting Print Acceleration: ", planner.acceleration);
  }
  if (parser.seen('R')) {
    planner.retract_acceleration = parser.value_linear_units();
    SERIAL_ECHOLNPAIR("Setting Retract Acceleration: ", planner.retract_acceleration);
  }
  if (parser.seen('T')) {
    planner.travel_acceleration = parser.value_linear_units();
    SERIAL_ECHOLNPAIR("Setting Travel Acceleration: ", planner.travel_acceleration);
  }
}
