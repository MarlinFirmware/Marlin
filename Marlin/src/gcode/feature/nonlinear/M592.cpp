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
  report_heading_etc(forReplay, F(STR_NONLINEAR_EXTRUSION));
  const nonlinear_settings_t &sns = stepper.ne.settings;
  SERIAL_ECHOLNPGM("  M592 S", sns.enabled, " A", sns.coeff.A, " B", sns.coeff.B, " C", sns.coeff.C);
}

/**
 * M592: Get or set nonlinear extrusion parameters
 *  S<flag>     Enable / Disable Nonlinear Extrusion
 *  A<factor>   Quadratic coefficient (default 0.0)
 *  B<factor>   Linear coefficient (default 0.0)
 *  C<factor>   Constant coefficient (default 1.0)
 *
 * Adjusts the amount of extrusion based on the instantaneous velocity of extrusion, as a multiplier.
 * The amount of extrusion is multiplied by max(C, A*v^2 + B*v + C) where v is extruder velocity in mm/s.
 * Only adjusts forward extrusions, since those are the ones affected by backpressure.
 */
void GcodeSuite::M592() {
  if (!parser.seen_any()) return M592_report();

  nonlinear_t &ne = stepper.ne;
  nonlinear_settings_t &sns = ne.settings;

  if (parser.seen('S')) sns.enabled = parser.value_bool();
  if (parser.seenval('A')) sns.coeff.A = parser.value_float();
  if (parser.seenval('B')) sns.coeff.B = parser.value_float();
  if (parser.seenval('C')) sns.coeff.C = parser.value_float();

  #if ENABLED(SMOOTH_LIN_ADVANCE)
    ne.q30.A = _BV32(30) * (sns.coeff.A * planner.mm_per_step[E_AXIS_N(0)] * planner.mm_per_step[E_AXIS_N(0)]);
    ne.q30.B = _BV32(30) * (sns.coeff.B * planner.mm_per_step[E_AXIS_N(0)]);
    ne.q30.C = _BV32(30) * sns.coeff.C;
  #endif
}

#endif // NONLINEAR_EXTRUSION
