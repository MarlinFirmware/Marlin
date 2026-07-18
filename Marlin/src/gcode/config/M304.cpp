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

#if ENABLED(PIDTEMPBED)

#include "../gcode.h"
#include "../../module/temperature.h"

/**
 * M304 - Set and/or Report the current Bed PID values
 *
 *  P<pval> - Set the Kp value
 *  I<ival> - Set the Ki value
 *  D<dval> - Set the Kd value
 *
 * With BED_ZONES enabled:
 *  A<index> - Area (zone) index (0-based, default 0). A is used because P/I/D are
 *             the PID parameters and X/Y/Z/E/U/V/W are axis letters.
 *
 * With no parameters, reports the current PID values (all zones when BED_ZONES).
 */
void GcodeSuite::M304() {
  #if HAS_BED_ZONES
    const uint8_t z = parser.seenval('A') ? _MIN((uint8_t)parser.value_byte(), (uint8_t)(BED_ZONES_COUNT - 1)) : 0;
    if (!parser.seen("PID")) return M304_report();
    if (parser.seenval('P')) thermalManager.temp_bed[z].pid.set_Kp(parser.value_float());
    if (parser.seenval('I')) thermalManager.temp_bed[z].pid.set_Ki(parser.value_float());
    if (parser.seenval('D')) thermalManager.temp_bed[z].pid.set_Kd(parser.value_float());
  #else
    if (!parser.seen("PID")) return M304_report();
    if (parser.seenval('P')) thermalManager.temp_bed.pid.set_Kp(parser.value_float());
    if (parser.seenval('I')) thermalManager.temp_bed.pid.set_Ki(parser.value_float());
    if (parser.seenval('D')) thermalManager.temp_bed.pid.set_Kd(parser.value_float());
  #endif
}

void GcodeSuite::M304_report(const bool forReplay/*=true*/) {
  TERN_(MARLIN_SMALL_BUILD, return);

  #if HAS_BED_ZONES
    report_heading(forReplay, F(STR_BED_PID));
    for (uint8_t z = 0; z < BED_ZONES_COUNT; z++) {
      report_echo_start(forReplay);
      SERIAL_ECHOLNPGM("  M304"
          " A", z
        , " P", thermalManager.temp_bed[z].pid.p()
        , " I", thermalManager.temp_bed[z].pid.i()
        , " D", thermalManager.temp_bed[z].pid.d()
      );
    }
  #else
    report_heading_etc(forReplay, F(STR_BED_PID));
    SERIAL_ECHOLNPGM("  M304"
        " P", thermalManager.temp_bed.pid.p()
      , " I", thermalManager.temp_bed.pid.i()
      , " D", thermalManager.temp_bed.pid.d()
    );
  #endif
}

#endif // PIDTEMPBED
