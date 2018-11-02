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

#if ENABLED(DELTA) || ENABLED(X_DUAL_ENDSTOPS) || ENABLED(Y_DUAL_ENDSTOPS) || Z_MULTI_ENDSTOPS

#include "../gcode.h"

#if ENABLED(DELTA)

  #include "../../module/delta.h"
  #include "../../module/motion.h"

  /**
   * M666: Set delta endstop adjustment
   */
  void GcodeSuite::M666() {
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) {
        SERIAL_ECHOLNPGM(">>> M666");
      }
    #endif
    LOOP_XYZ(i) {
      if (parser.seen(axis_codes[i])) {
        const float v = parser.value_linear_units();
        if (v * Z_HOME_DIR <= 0) delta_endstop_adj[i] = v;
        #if ENABLED(DEBUG_LEVELING_FEATURE)
          if (DEBUGGING(LEVELING)) {
            SERIAL_ECHOPAIR("delta_endstop_adj[", axis_codes[i]);
            SERIAL_ECHOLNPAIR("] = ", delta_endstop_adj[i]);
          }
        #endif
      }
    }
    #if ENABLED(DEBUG_LEVELING_FEATURE)
      if (DEBUGGING(LEVELING)) {
        SERIAL_ECHOLNPGM("<<< M666");
      }
    #endif
  }

#elif ENABLED(X_DUAL_ENDSTOPS) || ENABLED(Y_DUAL_ENDSTOPS) || Z_MULTI_ENDSTOPS

  #include "../../module/endstops.h"

  /**
   * M666: Set Dual Endstops offsets for X, Y, and/or Z.
   *       With no parameters report current offsets.
   *
   * For Triple Z Endstops:
   *   Set Z2 Only: M666 S2 Z<offset>
   *   Set Z3 Only: M666 S3 Z<offset>
   *      Set Both: M666 Z<offset>
   */
  void GcodeSuite::M666() {
    #if ENABLED(X_DUAL_ENDSTOPS)
      if (parser.seenval('X')) endstops.x2_endstop_adj = parser.value_linear_units();
    #endif
    #if ENABLED(Y_DUAL_ENDSTOPS)
      if (parser.seenval('Y')) endstops.y2_endstop_adj = parser.value_linear_units();
    #endif
    #if ENABLED(Z_TRIPLE_ENDSTOPS)
      if (parser.seenval('Z')) {
        const float z_adj = parser.value_linear_units();
        const int ind = parser.intval('S');
        if (!ind || ind == 2) endstops.z2_endstop_adj = z_adj;
        if (!ind || ind == 3) endstops.z3_endstop_adj = z_adj;
      }
    #elif Z_MULTI_ENDSTOPS
      if (parser.seen('Z')) endstops.z2_endstop_adj = parser.value_linear_units();
    #endif
    if (!parser.seen("XYZ")) {
      SERIAL_ECHOPGM("Dual Endstop Adjustment (mm): ");
      #if ENABLED(X_DUAL_ENDSTOPS)
        SERIAL_ECHOPAIR(" X2:", endstops.x2_endstop_adj);
      #endif
      #if ENABLED(Y_DUAL_ENDSTOPS)
        SERIAL_ECHOPAIR(" Y2:", endstops.y2_endstop_adj);
      #endif
      #if Z_MULTI_ENDSTOPS
        SERIAL_ECHOPAIR(" Z2:", endstops.z2_endstop_adj);
      #endif
      #if ENABLED(Z_TRIPLE_ENDSTOPS)
        SERIAL_ECHOPAIR(" Z3:", endstops.z3_endstop_adj);
      #endif
      SERIAL_EOL();
    }
  }

#endif // X_DUAL_ENDSTOPS || Y_DUAL_ENDSTOPS || Z_DUAL_ENDSTOPS

#endif // DELTA || X_DUAL_ENDSTOPS || Y_DUAL_ENDSTOPS || Z_DUAL_ENDSTOPS
