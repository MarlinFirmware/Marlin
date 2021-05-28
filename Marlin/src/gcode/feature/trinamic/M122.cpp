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

#include "../../../inc/MarlinConfig.h"

#if HAS_TRINAMIC_CONFIG

#include "../../gcode.h"
#include "../../../feature/tmc_util.h"
#include "../../../module/stepper/indirection.h"

/**
 * M122: Debug TMC drivers
 */
void GcodeSuite::M122() {
  xyze_bool_t print_axis = ARRAY_N_1(LOGICAL_AXES, false);

  bool print_all = true;
  LOOP_LOGICAL_AXES(i) if (parser.seen(axis_codes[i])) { print_axis[i] = true; print_all = false; }

  if (print_all) LOOP_LOGICAL_AXES(i) print_axis[i] = true;

  if (parser.boolval('I')) restore_stepper_drivers();

  #if ENABLED(TMC_DEBUG)
    #if ENABLED(MONITOR_DRIVER_STATUS)
      uint16_t interval = MONITOR_DRIVER_STATUS_INTERVAL_MS;
      if (parser.seen('S') && !parser.value_bool()) interval = 0;
      if (parser.seenval('P')) NOMORE(interval, parser.value_ushort());
      tmc_set_report_interval(interval);
    #endif

    if (parser.seen_test('V')) {
      tmc_get_registers(
        LOGICAL_AXIS_LIST(print_axis.e, print_axis.x, print_axis.y, print_axis.z)
      );
    }
    else {
      tmc_report_all(
        LOGICAL_AXIS_LIST(print_axis.e, print_axis.x, print_axis.y, print_axis.z)
      );
    }
  #endif

  test_tmc_connection(
    LOGICAL_AXIS_LIST(print_axis.e, print_axis.x, print_axis.y, print_axis.z)
  );
}

#endif // HAS_TRINAMIC_CONFIG
