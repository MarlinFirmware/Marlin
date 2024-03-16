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

#if ENABLED(PIDTEMPCHAMBER)

#include "../gcode.h"
#include "../../module/temperature.h"

/**
 * M309 - Set and/or Report the current Chamber PID values
 *
 *  P<pval> - Set the P value
 *  I<ival> - Set the I value
 *  D<dval> - Set the D value
 */
void GcodeSuite::M309() {
  if (!parser.seen("PID")) return M309_report();
  if (parser.seenval('P')) thermalManager.temp_chamber.pid.set_Kp(parser.value_float());
  if (parser.seenval('I')) thermalManager.temp_chamber.pid.set_Ki(parser.value_float());
  if (parser.seenval('D')) thermalManager.temp_chamber.pid.set_Kd(parser.value_float());
}

void GcodeSuite::M309_report(const bool forReplay/*=true*/) {
  TERN_(MARLIN_SMALL_BUILD, return);

  report_heading_etc(forReplay, F(STR_CHAMBER_PID));
  SERIAL_ECHOLNPGM("  M309"
      " P", thermalManager.temp_chamber.pid.p()
    , " I", thermalManager.temp_chamber.pid.i()
    , " D", thermalManager.temp_chamber.pid.d()
  );
}

#endif // PIDTEMPCHAMBER
