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

#include "../../inc/MarlinConfigPre.h"

#if HAS_SOFTWARE_ENDSTOPS

#include "../gcode.h"
#include "../../module/motion.h"

#if ENABLED(ABORT_ON_SOFTWARE_ENDSTOP)
#include "../../module/planner.h"
#endif

/**
 * M211: Enable, Disable, and/or Report software endstops
 * 
 * Parameters
 *   S<bool>: flag to enable (1, default) or disable (0) software endstops.
 *   H<bool>: flag to abort print on software endstops (1) or clamp movement to software endstops (0, default) 
 *            if software endstops are enabled. Requires ABORT_ON_SOFTWARE_ENDSTOP
 * 
 * Report the software endstop status if no parameter is specified, 
 * 
 * Examples:
 *   M211 S0 to disable software endstops
 *   M211 S1 H1 to enable software endstops and halt printer on the next move beyond software endstops
 *   M211 S1 H0 to enable software endstops and clamp movement to software endstops (default)
 */
void GcodeSuite::M211() {
  if (parser.seen('S'))
    soft_endstop._enabled = parser.value_bool();
  #if ENABLED(ABORT_ON_SOFTWARE_ENDSTOP)
    if (parser.seen('H'))
      planner.abort_on_software_endstop = parser.value_bool();
  #endif
  else
    M211_report();
}

void GcodeSuite::M211_report(const bool forReplay/*=true*/) {
  report_heading_etc(forReplay, F(STR_SOFT_ENDSTOPS));
  SERIAL_ECHOPGM("  M211 S", AS_DIGIT(soft_endstop._enabled), " ; ");
  serialprintln_onoff(soft_endstop._enabled);

  report_echo_start(forReplay);
  const xyz_pos_t l_soft_min = soft_endstop.min.asLogical(),
                  l_soft_max = soft_endstop.max.asLogical();
  print_pos(l_soft_min, F(STR_SOFT_MIN), F(" "));
  print_pos(l_soft_max, F(STR_SOFT_MAX));
}

#endif // HAS_SOFTWARE_ENDSTOPS
