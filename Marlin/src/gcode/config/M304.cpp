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
 *  P<pval> - Set the P value
 *  I<ival> - Set the I value
 *  D<dval> - Set the D value
 */
void GcodeSuite::M304() {
  if (parser.seen('P')) thermalManager.temp_bed.pid.Kp = parser.value_float();
  if (parser.seen('I')) thermalManager.temp_bed.pid.Ki = scalePID_i(parser.value_float());
  if (parser.seen('D')) thermalManager.temp_bed.pid.Kd = scalePID_d(parser.value_float());

  SERIAL_ECHO_START();
  SERIAL_ECHOLNPAIR(" p:", thermalManager.temp_bed.pid.Kp,
                    " i:", unscalePID_i(thermalManager.temp_bed.pid.Ki),
                    " d:", unscalePID_d(thermalManager.temp_bed.pid.Kd));
}

#endif // PIDTEMPBED
