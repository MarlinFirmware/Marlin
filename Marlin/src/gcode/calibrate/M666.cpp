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

#if ENABLED(DELTA) || ENABLED(Z_DUAL_ENDSTOPS)

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

#elif ENABLED(Z_DUAL_ENDSTOPS) // !DELTA && ENABLED(Z_DUAL_ENDSTOPS)

  #include "../../module/endstops.h"

  /**
   * M666: For a Dual Endstop setup, set offsets for any 2nd endstops.
   */
  void GcodeSuite::M666() {
    SERIAL_ECHOPGM("Dual Endstop Adjustment (mm): ");
    #if ENABLED(X_DUAL_ENDSTOPS)
      if (parser.seen('X')) endstops.x_endstop_adj = parser.value_linear_units();
      SERIAL_ECHOPAIR(" X", endstops.x_endstop_adj);
    #endif
    #if ENABLED(Y_DUAL_ENDSTOPS)
      if (parser.seen('Y')) endstops.y_endstop_adj = parser.value_linear_units();
      SERIAL_ECHOPAIR(" Y", endstops.y_endstop_adj);
    #endif
    #if ENABLED(Z_DUAL_ENDSTOPS)
      if (parser.seen('Z')) endstops.z_endstop_adj = parser.value_linear_units();
      SERIAL_ECHOPAIR(" Z", endstops.z_endstop_adj);
    #endif
    SERIAL_EOL();
  }

#endif

#endif // DELTA || Z_DUAL_ENDSTOPS
