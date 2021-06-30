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

  #define DEBUG_OUT ENABLED(DEBUG_LEVELING_FEATURE)
  #include "../../core/debug_out.h"

  /**
   * M666: Set delta endstop adjustment
   */
  void GcodeSuite::M666() {
    DEBUG_SECTION(log_M666, "M666", DEBUGGING(LEVELING));
    LOOP_LINEAR_AXES(i) {
      if (parser.seen(AXIS_CHAR(i))) {
        const float v = parser.value_linear_units();
        if (v * Z_HOME_DIR <= 0) delta_endstop_adj[i] = v;
        if (DEBUGGING(LEVELING)) DEBUG_ECHOLNPAIR("delta_endstop_adj[", AS_CHAR(AXIS_CHAR(i)), "] = ", delta_endstop_adj[i]);
      }
    }
  }

#elif HAS_EXTRA_ENDSTOPS

  #include "../../module/endstops.h"

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
    #if ENABLED(X_DUAL_ENDSTOPS)
      if (parser.seenval('X')) endstops.x2_endstop_adj = parser.value_linear_units();
    #endif
    #if ENABLED(Y_DUAL_ENDSTOPS)
      if (parser.seenval('Y')) endstops.y2_endstop_adj = parser.value_linear_units();
    #endif
    #if ENABLED(Z_MULTI_ENDSTOPS)
      if (parser.seenval('Z')) {
        #if NUM_Z_STEPPER_DRIVERS >= 3
          const float z_adj = parser.value_linear_units();
          const int ind = parser.intval('S');
          if (!ind || ind == 2) endstops.z2_endstop_adj = z_adj;
          if (!ind || ind == 3) endstops.z3_endstop_adj = z_adj;
          #if NUM_Z_STEPPER_DRIVERS >= 4
            if (!ind || ind == 4) endstops.z4_endstop_adj = z_adj;
          #endif
        #else
          endstops.z2_endstop_adj = parser.value_linear_units();
        #endif
      }
    #endif
    if (!parser.seen("XYZ")) {
      SERIAL_ECHOPGM("Dual Endstop Adjustment (mm): ");
      #if ENABLED(X_DUAL_ENDSTOPS)
        SERIAL_ECHOPAIR(" X2:", endstops.x2_endstop_adj);
      #endif
      #if ENABLED(Y_DUAL_ENDSTOPS)
        SERIAL_ECHOPAIR(" Y2:", endstops.y2_endstop_adj);
      #endif
      #if ENABLED(Z_MULTI_ENDSTOPS)
        #define _ECHO_ZADJ(N) SERIAL_ECHOPAIR(" Z" STRINGIFY(N) ":", endstops.z##N##_endstop_adj);
        REPEAT_S(2, INCREMENT(NUM_Z_STEPPER_DRIVERS), _ECHO_ZADJ)
      #endif
      SERIAL_EOL();
    }
  }

#endif // HAS_EXTRA_ENDSTOPS

#endif // DELTA || HAS_EXTRA_ENDSTOPS
