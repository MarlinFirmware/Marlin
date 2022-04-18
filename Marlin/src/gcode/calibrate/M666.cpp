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

#if ENABLED(DELTA) || HAS_EXTRA_ENDSTOPS

#include "../gcode.h"

#if ENABLED(DELTA)
  #include "../../module/delta.h"
  #include "../../module/motion.h"
#else
  #include "../../module/endstops.h"
#endif

#define DEBUG_OUT ENABLED(DEBUG_LEVELING_FEATURE)
#include "../../core/debug_out.h"

#if ENABLED(DELTA)

  /**
   * M666: Set delta endstop adjustment
   */
  void GcodeSuite::M666() {
    DEBUG_SECTION(log_M666, "M666", DEBUGGING(LEVELING));
    bool is_err = false, is_set = false;
    LOOP_LINEAR_AXES(i) {
      if (parser.seenval(AXIS_CHAR(i))) {
        is_set = true;
        const float v = parser.value_linear_units();
        if (v > 0)
          is_err = true;
        else {
          delta_endstop_adj[i] = v;
          if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPGM("delta_endstop_adj[", AS_CHAR(AXIS_CHAR(i)), "] = ", v);
        }
      }
    }
    if (is_err) SERIAL_ECHOLNPGM("?M666 offsets must be <= 0");
    if (!is_set) M666_report();
  }

  void GcodeSuite::M666_report(const bool forReplay/*=true*/) {
    report_heading_etc(forReplay, F(STR_ENDSTOP_ADJUSTMENT));
    SERIAL_ECHOLNPGM_P(
        PSTR("  M666 X"), LINEAR_UNIT(delta_endstop_adj.a)
      , SP_Y_STR, LINEAR_UNIT(delta_endstop_adj.b)
      , SP_Z_STR, LINEAR_UNIT(delta_endstop_adj.c)
    );
  }

#else

  /**
   * M666: Set Dual Endstops offsets for X, Y, and/or Z.
   *       With no parameters report current offsets.
   *
   * For Triple / Quad Z Endstops:
   *   Set Z2 Only: M666 S2 Z<offset>
   *   Set Z3 Only: M666 S3 Z<offset>
   *   Set Z4 Only: M666 S4 Z<offset>
   *       Set All: M666 Z<offset>
   */
  void GcodeSuite::M666() {
    if (!parser.seen_any()) return M666_report();

    #if ENABLED(X_DUAL_ENDSTOPS)
      if (parser.seenval('X')) endstops.x2_endstop_adj = parser.value_linear_units();
    #endif
    #if ENABLED(Y_DUAL_ENDSTOPS)
      if (parser.seenval('Y')) endstops.y2_endstop_adj = parser.value_linear_units();
    #endif
    #if ENABLED(Z_MULTI_ENDSTOPS)
      if (parser.seenval('Z')) {
        const float z_adj = parser.value_linear_units();
        #if NUM_Z_STEPPERS == 2
          endstops.z2_endstop_adj = z_adj;
        #else
          const int ind = parser.intval('S');
          #define _SET_ZADJ(N) if (!ind || ind == N) endstops.z##N##_endstop_adj = z_adj;
          REPEAT_S(2, INCREMENT(NUM_Z_STEPPERS), _SET_ZADJ)
        #endif
      }
    #endif
  }

  void GcodeSuite::M666_report(const bool forReplay/*=true*/) {
    report_heading_etc(forReplay, F(STR_ENDSTOP_ADJUSTMENT));
    SERIAL_ECHOPGM("  M666");
    #if ENABLED(X_DUAL_ENDSTOPS)
      SERIAL_ECHOLNPGM_P(SP_X_STR, LINEAR_UNIT(endstops.x2_endstop_adj));
    #endif
    #if ENABLED(Y_DUAL_ENDSTOPS)
      SERIAL_ECHOLNPGM_P(SP_Y_STR, LINEAR_UNIT(endstops.y2_endstop_adj));
    #endif
    #if ENABLED(Z_MULTI_ENDSTOPS)
      #if NUM_Z_STEPPERS >= 3
        SERIAL_ECHOPGM(" S2 Z", LINEAR_UNIT(endstops.z3_endstop_adj));
        report_echo_start(forReplay);
        SERIAL_ECHOPGM("  M666 S3 Z", LINEAR_UNIT(endstops.z3_endstop_adj));
        #if NUM_Z_STEPPERS >= 4
          report_echo_start(forReplay);
          SERIAL_ECHOPGM("  M666 S4 Z", LINEAR_UNIT(endstops.z4_endstop_adj));
        #endif
      #else
        SERIAL_ECHOLNPGM_P(SP_Z_STR, LINEAR_UNIT(endstops.z2_endstop_adj));
      #endif
    #endif
  }

#endif // HAS_EXTRA_ENDSTOPS

#endif // DELTA || HAS_EXTRA_ENDSTOPS
