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

#include "../../../inc/MarlinConfig.h"

#if HAS_TRINAMIC

#include "../../gcode.h"
#include "../../../feature/tmc_util.h"

/**
 * M122: Debug TMC drivers
 */
void GcodeSuite::M122() {
  xyze_bool_t print_axis = { false, false, false, false };
  bool print_all = true;
  LOOP_XYZE(i) if (parser.seen(axis_codes[i])) { print_axis[i] = true; print_all = false; }

  if (print_all) LOOP_XYZE(i) print_axis[i] = true;

  #if ENABLED(TMC_DEBUG)
    #if ENABLED(MONITOR_DRIVER_STATUS)
      const bool sflag = parser.seen('S'), s0 = sflag && !parser.value_bool();
      if (sflag) tmc_set_report_interval(s0 ? 0 : MONITOR_DRIVER_STATUS_INTERVAL_MS);
      if (!s0 && parser.seenval('P')) tmc_set_report_interval(_MIN(parser.value_ushort(), MONITOR_DRIVER_STATUS_INTERVAL_MS));
    #endif

    if (parser.seen('V'))
      tmc_get_registers(print_axis.x, print_axis.y, print_axis.z, print_axis.e);
    else
      tmc_report_all(print_axis.x, print_axis.y, print_axis.z, print_axis.e);
  #endif

  test_tmc_connection(print_axis.x, print_axis.y, print_axis.z, print_axis.e);
}

#endif // HAS_TRINAMIC
