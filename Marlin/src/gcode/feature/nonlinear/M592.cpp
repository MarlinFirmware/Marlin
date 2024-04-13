/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../../inc/MarlinConfig.h"

#if ENABLED(NONLINEAR_EXTRUSION)

#include "../../gcode.h"
#include "../../../module/stepper.h"

void GcodeSuite::M592_report(const bool forReplay/*=true*/) {
  TERN_(MARLIN_SMALL_BUILD, return);
  report_heading(forReplay, F(STR_NONLINEAR_EXTRUSION));
  SERIAL_ECHOLNPGM("  M592 A", stepper.ne.A, " B", stepper.ne.B, " C", stepper.ne.C);
}

/**
 * M592: Get or set nonlinear extrusion parameters
 *  A<factor>   Linear coefficient (default 0.0)
 *  B<factor>   Quadratic coefficient (default 0.0)
 *  C<factor>   Constant coefficient (default 1.0)
 *
 * Adjusts the amount of extrusion based on the instantaneous velocity of extrusion, as a multiplier.
 * The amount of extrusion is multiplied by max(C, C + A*v + B*v^2) where v is extruder velocity in mm/s.
 * Only adjusts forward extrusions, since those are the ones affected by backpressure.
 */
void GcodeSuite::M592() {
  if (!parser.seen_any()) return M592_report();

  if (parser.seenval('A')) stepper.ne.A = parser.value_float();
  if (parser.seenval('B')) stepper.ne.B = parser.value_float();
  if (parser.seenval('C')) stepper.ne.C = parser.value_float();
}

#endif // NONLINEAR_EXTRUSION
