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
   *    X = Alpha (Tower 1) angle trim
   *    Y = Beta  (Tower 2) angle trim
   *    Z = Gamma (Tower 3) angle trim
   *    A = Alpha (Tower 1) diagonal rod trim
   *    B = Beta  (Tower 2) diagonal rod trim
   *    C = Gamma (Tower 3) diagonal rod trim
   */
  void GcodeSuite::M665() {
    if (!parser.seen_any()) return M665_report();

    if (parser.seenval('H')) delta_height              = parser.value_linear_units();
    if (parser.seenval('L')) delta_diagonal_rod        = parser.value_linear_units();
    if (parser.seenval('R')) delta_radius              = parser.value_linear_units();
    if (parser.seenval('S')) segments_per_second       = parser.value_float();
    if (parser.seenval('X')) delta_tower_angle_trim.a  = parser.value_float();
    if (parser.seenval('Y')) delta_tower_angle_trim.b  = parser.value_float();
    if (parser.seenval('Z')) delta_tower_angle_trim.c  = parser.value_float();
    if (parser.seenval('A')) delta_diagonal_rod_trim.a = parser.value_float();
    if (parser.seenval('B')) delta_diagonal_rod_trim.b = parser.value_float();
    if (parser.seenval('C')) delta_diagonal_rod_trim.c = parser.value_float();
    recalc_delta_settings();
  }

  void GcodeSuite::M665_report(const bool forReplay/*=true*/) {
    report_heading_etc(forReplay, PSTR(STR_DELTA_SETTINGS));
    SERIAL_ECHOLNPGM_P(
        PSTR("  M665 L"), LINEAR_UNIT(delta_diagonal_rod)
      , PSTR(" R"), LINEAR_UNIT(delta_radius)
      , PSTR(" H"), LINEAR_UNIT(delta_height)
      , PSTR(" S"), segments_per_second
      , SP_X_STR, LINEAR_UNIT(delta_tower_angle_trim.a)
      , SP_Y_STR, LINEAR_UNIT(delta_tower_angle_trim.b)
      , SP_Z_STR, LINEAR_UNIT(delta_tower_angle_trim.c)
      , PSTR(" A"), LINEAR_UNIT(delta_diagonal_rod_trim.a)
      , PSTR(" B"), LINEAR_UNIT(delta_diagonal_rod_trim.b)
      , PSTR(" C"), LINEAR_UNIT(delta_diagonal_rod_trim.c)
    );
  }

#elif IS_SCARA

  #include "../../module/scara.h"

  /**
   * M665: Set SCARA settings
   *
   * Parameters:
   *
   *   S[segments-per-second] - Segments-per-second
   *
   * Without NO_WORKSPACE_OFFSETS:
   *
   *   P[theta-psi-offset]    - Theta-Psi offset, added to the shoulder (A/X) angle
   *   T[theta-offset]        - Theta     offset, added to the elbow    (B/Y) angle
   *   Z[z-offset]            - Z offset, added to Z
   *
   *   A, P, and X are all aliases for the shoulder angle
   *   B, T, and Y are all aliases for the elbow angle
   */
  void GcodeSuite::M665() {
    if (!parser.seen_any()) return M665_report();

    if (parser.seenval('S')) segments_per_second = parser.value_float();

    #if HAS_SCARA_OFFSET

      if (parser.seenval('Z')) scara_home_offset.z = parser.value_linear_units();

      const bool hasA = parser.seenval('A'), hasP = parser.seenval('P'), hasX = parser.seenval('X');
      const uint8_t sumAPX = hasA + hasP + hasX;
      if (sumAPX) {
        if (sumAPX == 1)
          scara_home_offset.a = parser.value_float();
        else {
          SERIAL_ERROR_MSG("Only one of A, P, or X is allowed.");
          return;
        }
      }

      const bool hasB = parser.seenval('B'), hasT = parser.seenval('T'), hasY = parser.seenval('Y');
      const uint8_t sumBTY = hasB + hasT + hasY;
      if (sumBTY) {
        if (sumBTY == 1)
          scara_home_offset.b = parser.value_float();
        else {
          SERIAL_ERROR_MSG("Only one of B, T, or Y is allowed.");
          return;
        }
      }

    #endif // HAS_SCARA_OFFSET
  }

  void GcodeSuite::M665_report(const bool forReplay/*=true*/) {
    report_heading_etc(forReplay, PSTR(STR_SCARA_SETTINGS " (" STR_S_SEG_PER_SEC TERN_(HAS_SCARA_OFFSET, " " STR_SCARA_P_T_Z) ")"));
    SERIAL_ECHOLNPGM_P(
      PSTR("  M665 S"), segments_per_second
      #if HAS_SCARA_OFFSET
        , SP_P_STR, scara_home_offset.a
        , SP_T_STR, scara_home_offset.b
        , SP_Z_STR, LINEAR_UNIT(scara_home_offset.z)
      #endif
    );
  }

#elif ENABLED(POLARGRAPH)

  #include "../../module/polargraph.h"

  /**
   * M665: Set POLARGRAPH settings
   *
   * Parameters:
   *
   *   S[segments-per-second] - Segments-per-second
   */
  void GcodeSuite::M665() {
    if (parser.seenval('S'))
      segments_per_second = parser.value_float();
    else
      M665_report();
  }

  void GcodeSuite::M665_report(const bool forReplay/*=true*/) {
    report_heading_etc(forReplay, PSTR(STR_POLARGRAPH_SETTINGS " (" STR_S_SEG_PER_SEC ")"));
    SERIAL_ECHOLNPGM("  M665 S", segments_per_second);
  }

#endif

#endif // IS_KINEMATIC
