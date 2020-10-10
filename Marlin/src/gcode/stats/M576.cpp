/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 * 
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
#include "../../MarlinCore.h" // for SP_P/B_STR, etc.

#if ENABLED(BUFFER_MONITORING)

#include "../gcode.h"
#include "../queue.h"

/**
 * M576: Return buffer stats, and optionally set auto-report interval.
 * Usage: M576 [S<seconds>]
 * 
 * When called, printer emits the following output:
 * "M576 P<nn> B<nn> PU<nn> PD<nn> BU<nn> BD<nn>"
 * Where:
 *   P: Planner buffers available 
 *   B: Command buffers available
 *   PU: Planner buffer underruns since last report
 *   PD: Maximum time in ms planner buffer was empty since last report
 *   BU: Command buffer underruns since last report
 *   BD: Maximum time in ms command buffer was empty since last report
 */
void GcodeSuite::M576() {
  if (parser.seenval('S')) {
    queue.set_auto_report_interval((uint8_t)parser.value_byte());
  }

  queue.report_buffer_statistics();
}

#endif // BUFFER_MONITORING 
