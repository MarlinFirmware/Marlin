/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
  SERIAL_ECHO_START();
  SERIAL_ECHOPGM(MSG_SOFT_ENDSTOPS);
  if (parser.seen('S')) soft_endstops_enabled = parser.value_bool();
  serialprint_onoff(soft_endstops_enabled);
  SERIAL_ECHOPGM(MSG_SOFT_MIN);
  SERIAL_ECHOPAIR(    MSG_X, LOGICAL_X_POSITION(soft_endstop[X_AXIS].min));
  SERIAL_ECHOPAIR(" " MSG_Y, LOGICAL_Y_POSITION(soft_endstop[Y_AXIS].min));
  SERIAL_ECHOPAIR(" " MSG_Z, LOGICAL_Z_POSITION(soft_endstop[Z_AXIS].min));
  SERIAL_ECHOPGM(MSG_SOFT_MAX);
  SERIAL_ECHOPAIR(    MSG_X, LOGICAL_X_POSITION(soft_endstop[X_AXIS].max));
  SERIAL_ECHOPAIR(" " MSG_Y, LOGICAL_Y_POSITION(soft_endstop[Y_AXIS].max));
  SERIAL_ECHOLNPAIR(" " MSG_Z, LOGICAL_Z_POSITION(soft_endstop[Z_AXIS].max));
}

#endif
