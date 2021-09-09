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

/**
 * M211: Enable, Disable, and/or Report software endstops
 *
 * Usage: M211 S1 to enable, M211 S0 to disable, M211 alone for report
 */
void GcodeSuite::M211() {
  if (parser.seen('S'))
    soft_endstop._enabled = parser.value_bool();
  else
    M211_report();
}

void GcodeSuite::M211_report(const bool forReplay/*=true*/) {
  report_heading_etc(forReplay, PSTR(STR_SOFT_ENDSTOPS));
  SERIAL_ECHOPGM("  M211 S", AS_DIGIT(soft_endstop._enabled), " ; ");
  serialprintln_onoff(soft_endstop._enabled);

  report_echo_start(forReplay);
  const xyz_pos_t l_soft_min = soft_endstop.min.asLogical(),
                  l_soft_max = soft_endstop.max.asLogical();
  print_pos(l_soft_min, PSTR(STR_SOFT_MIN), PSTR(" "));
  print_pos(l_soft_max, PSTR(STR_SOFT_MAX));
}

#endif // HAS_SOFTWARE_ENDSTOPS
