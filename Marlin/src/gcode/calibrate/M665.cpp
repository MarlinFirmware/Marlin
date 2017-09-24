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

#if IS_KINEMATIC

#include "../gcode.h"
#include "../../module/motion.h"

#if ENABLED(DELTA)

  #include "../../module/delta.h"

  /**
   * M665: Set delta configurations
   *
   *    H = delta height
   *    L = diagonal rod
   *    R = delta radius
   *    S = segments per second
   *    B = delta calibration radius
   *    X = Alpha (Tower 1) angle trim
   *    Y = Beta (Tower 2) angle trim
   *    Z = Rotate A and B by this angle
   */
  void GcodeSuite::M665() {
    if (parser.seen('H')) {
      home_offset[Z_AXIS] = parser.value_linear_units() - DELTA_HEIGHT;
      update_software_endstops(Z_AXIS);
    }
    if (parser.seen('L')) delta_diagonal_rod             = parser.value_linear_units();
    if (parser.seen('R')) delta_radius                   = parser.value_linear_units();
    if (parser.seen('S')) delta_segments_per_second      = parser.value_float();
    if (parser.seen('B')) delta_calibration_radius       = parser.value_float();
    if (parser.seen('X')) delta_tower_angle_trim[A_AXIS] = parser.value_float();
    if (parser.seen('Y')) delta_tower_angle_trim[B_AXIS] = parser.value_float();
    if (parser.seen('Z')) delta_tower_angle_trim[C_AXIS] = parser.value_float();
    recalc_delta_settings(delta_radius, delta_diagonal_rod, delta_tower_angle_trim);
  }

#elif IS_SCARA

  #include "../../module/scara.h"

  /**
   * M665: Set SCARA settings
   *
   * Parameters:
   *
   *   S[segments-per-second] - Segments-per-second
   *   P[theta-psi-offset]    - Theta-Psi offset, added to the shoulder (A/X) angle
   *   T[theta-offset]        - Theta     offset, added to the elbow    (B/Y) angle
   *
   *   A, P, and X are all aliases for the shoulder angle
   *   B, T, and Y are all aliases for the elbow angle
   */
  void GcodeSuite::M665() {
    if (parser.seen('S')) delta_segments_per_second = parser.value_float();

    const bool hasA = parser.seen('A'), hasP = parser.seen('P'), hasX = parser.seen('X');
    const uint8_t sumAPX = hasA + hasP + hasX;
    if (sumAPX == 1)
      home_offset[A_AXIS] = parser.value_float();
    else if (sumAPX > 1) {
      SERIAL_ERROR_START();
      SERIAL_ERRORLNPGM("Only one of A, P, or X is allowed.");
      return;
    }

    const bool hasB = parser.seen('B'), hasT = parser.seen('T'), hasY = parser.seen('Y');
    const uint8_t sumBTY = hasB + hasT + hasY;
    if (sumBTY == 1)
      home_offset[B_AXIS] = parser.value_float();
    else if (sumBTY > 1) {
      SERIAL_ERROR_START();
      SERIAL_ERRORLNPGM("Only one of B, T, or Y is allowed.");
      return;
    }
  }

#endif

#endif // IS_KINEMATIC
