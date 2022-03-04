/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2022 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * M423.cpp - X-Axis Twist Compensation
 */

#include "../../inc/MarlinConfig.h"

#if ENABLED(X_AXIS_TWIST_COMPENSATION)

#include "../gcode.h"
#include "../../feature/x_twist.h"

/**
 * M423: Set a Z offset for X-Twist (added to the mesh on future G29).
 *  M423 [R] [A<startx>] [I<interval>] [X<index> Z<offset>]
 *
 *    R         - Reset the twist compensation data
 *    A<linear> - Override the X twist starting X position
 *    I<linear> - Override the X twist X-spacing
 *    X<index>  - Index of a Z value in the list
 *    Z<linear> - A Z value to set
 */
void GcodeSuite::M423() {

  const bool do_report = true;

  if (parser.seen_test('R')) {
    do_report = false;
    xatc.reset();
  }
  if (parser.seenval('A')) {
    do_report = false;
    xatc.start = parser.value_float();
  }
  if (parser.seenval('I')) {
    do_report = false;
    xatc.spacing = parser.value_float();
  }

  if (parser.seenval('X')) {
    do_report = false;
    const int8_t x = parser.value_int();
    if (!WITHIN(x, 0, XATC_MAX_POINTS - 1))
      SERIAL_ECHOLNPGM("?(X) out of range (0..", XATC_MAX_POINTS - 1, ").");
    else {
      if (parser.seenval('Z'))
        xatc.z_offset[x] = parser.value_linear_units();
      else
        SERIAL_ECHOLNPGM("?(Z) required.");
    }
  }

  if (do_report) M423_report();

}

void GcodeSuite::M423_report(const bool forReplay/*=true*/) {
  report_heading(forReplay, F("X-Twist Correction"));
  LOOP_L_N(x, XATC_MAX_POINTS) {
    const float z = xatc.z_offset[x];
    SERIAL_ECHOPGM("  M423 X", x, " Z");
    if (isnan(z))
      SERIAL_CHAR('0');
    else {
      if (z >= 0) SERIAL_CHAR('+');
      SERIAL_ECHO_F(z, 3);
    }
    SERIAL_EOL();
  }
}

#endif // X_AXIS_TWIST_COMPENSATION
