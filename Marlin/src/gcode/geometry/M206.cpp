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

#if HAS_M206_COMMAND

#include "../gcode.h"
#include "../../module/motion.h"

/**
 * M206: Set Additional Homing Offset (X Y Z). SCARA aliases T=X, P=Y
 *
 * *** @thinkyhead: I recommend deprecating M206 for SCARA in favor of M665.
 * ***              M206 for SCARA will remain enabled in 1.1.x for compatibility.
 * ***              In the 2.0 release, it will simply be disabled by default.
 */
void GcodeSuite::M206() {
  LOOP_XYZ(i)
    if (parser.seen(axis_codes[i]))
      set_home_offset((AxisEnum)i, parser.value_linear_units());

  #if ENABLED(MORGAN_SCARA)
    if (parser.seen('T')) set_home_offset(A_AXIS, parser.value_linear_units()); // Theta
    if (parser.seen('P')) set_home_offset(B_AXIS, parser.value_linear_units()); // Psi
  #endif

  SYNC_PLAN_POSITION_KINEMATIC();
  report_current_position();
}

#endif // HAS_M206_COMMAND
