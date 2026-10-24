/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#if ENABLED(RUNTIME_HOMING_DIRECTION)

#include "../gcode.h"
#include "../../module/motion.h"

/**
 * M574: Set or report the homing direction (RUNTIME_HOMING_DIRECTION)
 *
 * Axis values follow RepRapFirmware's endstop position:
 *  X<1|2> Y<1|2> Z<1|2> ... - Home to the low (1, MIN) or high (2, MAX) end of the axis
 *  R          - Reset to the configured [XYZ]_HOME_DIR values
 *
 * With no parameters report the current settings.
 *
 * The axis must have an endstop at the chosen end. Z homing with a probe and X with DUAL_X_CARRIAGE can't be changed.
 *
 * Example:
 *  M574 X1 Y2   ; Home X to MIN and Y to MAX
 */
void GcodeSuite::M574() {
  if (!parser.seen_any()) return M574_report();

  if (parser.seen_test('R')) return motion.reset_axis_home_dir();

  LOOP_NUM_AXES(i) if (parser.seenval(AXIS_CHAR(i))) {
    const AxisEnum axis = AxisEnum(i);
    const int v = parser.value_int();
    const int8_t dir = v == 1 ? -1 : v == 2 ? 1 : 0;
    if (!dir)
      SERIAL_ECHOLNPGM(GCODE_ERR_MSG("Use ", C(AXIS_CHAR(i)), "1 (MIN) or ", C(AXIS_CHAR(i)), "2 (MAX)"));
    else if (dir == motion.home_dir(axis))
      continue;
    else if (motion.home_dir_fixed(axis))
      SERIAL_ECHOLNPGM(GCODE_ERR_MSG("Can't change ", C(AXIS_CHAR(i)), " homing direction"));
    else if (!motion.can_home_to(axis, dir))
      SERIAL_ECHOLNPGM(GCODE_ERR_MSG("No ", C(AXIS_CHAR(i)), " endstop at ", dir < 0 ? "MIN" : "MAX"));
    else
      motion.axis_home_dir[i] = dir;
  }
}

void GcodeSuite::M574_report(const bool forReplay/*=true*/) {
  TERN_(MARLIN_SMALL_BUILD, return);

  report_heading(forReplay, F("Homing Direction (1=MIN, 2=MAX)"));
  report_echo_start(forReplay);
  SERIAL_ECHOPGM("  M574");
  LOOP_NUM_AXES(i) { SERIAL_CHAR(' ', AXIS_CHAR(i)); SERIAL_ECHO(motion.home_dir(AxisEnum(i)) < 0 ? 1 : 2); }
  SERIAL_EOL();
}

#endif // RUNTIME_HOMING_DIRECTION
