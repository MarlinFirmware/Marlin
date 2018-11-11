/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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

#include "../../inc/MarlinConfig.h"

#if ENABLED(PIDTEMPBED)

#include "../gcode.h"
#include "../../module/temperature.h"

void GcodeSuite::M304() {
  if (parser.seen('P')) thermalManager.bed_pid.Kp = parser.value_float();
  if (parser.seen('I')) thermalManager.bed_pid.Ki = scalePID_i(parser.value_float());
  if (parser.seen('D')) thermalManager.bed_pid.Kd = scalePID_d(parser.value_float());

  SERIAL_ECHO_START();
  SERIAL_ECHOPAIR(" p:", thermalManager.bed_pid.Kp);
  SERIAL_ECHOPAIR(" i:", unscalePID_i(thermalManager.bed_pid.Ki));
  SERIAL_ECHOLNPAIR(" d:", unscalePID_d(thermalManager.bed_pid.Kd));
}

#endif // PIDTEMPBED
