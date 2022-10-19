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

#include "../../inc/MarlinConfigPre.h"

#if HAS_MULTI_EXTRUDER && ENABLED(TOOLCHANGE_PARK)

#include "../gcode.h"
#include "../../module/tool_change.h"
#include "../../MarlinCore.h" // for SP_X_STR, etc.

/**
 * M216 - Set Tool change Park parameters
 *
 *  // Tool change Park settings
 *  P[linear]     0/1 Enable park
 *  R[linear]     0/1 Enable return to previous position before park
 *  X[linear]     Park X
 *  Y[linear]     Park Y
 *  I[linear]     Park I (Requires NUM_AXES >= 4)
 *  J[linear]     Park J (Requires NUM_AXES >= 5)
 *  K[linear]     Park K (Requires NUM_AXES >= 6)
 *  C[linear]     Park U (Requires NUM_AXES >= 7)
 *  H[linear]     Park V (Requires NUM_AXES >= 8)
 *  O[linear]     Park W (Requires NUM_AXES >= 9)
 *
 */
void GcodeSuite::M216() {

  if (parser.seenval('P')) { toolchange_settings.enable_park = parser.value_linear_units(); }
  if (parser.seenval('R')) { toolchange_settings.no_return = parser.value_linear_units(); }
  if (parser.seenval('X')) { const int16_t v = parser.value_linear_units(); toolchange_settings.change_point.x = constrain(v, X_MIN_POS, X_MAX_POS); }
  #if HAS_Y_AXIS
    if (parser.seenval('Y')) { const int16_t v = parser.value_linear_units(); toolchange_settings.change_point.y = constrain(v, Y_MIN_POS, Y_MAX_POS); }
  #endif
  #if HAS_I_AXIS
    if (parser.seenval('I')) { const int16_t v = parser.TERN(AXIS4_ROTATES, value_int, value_linear_units)(); toolchange_settings.change_point.i = constrain(v, I_MIN_POS, I_MAX_POS); }
  #endif
  #if HAS_J_AXIS
    if (parser.seenval('J')) { const int16_t v = parser.TERN(AXIS5_ROTATES, value_int, value_linear_units)(); toolchange_settings.change_point.j = constrain(v, J_MIN_POS, J_MAX_POS); }
  #endif
  #if HAS_K_AXIS
    if (parser.seenval('K')) { const int16_t v = parser.TERN(AXIS6_ROTATES, value_int, value_linear_units)(); toolchange_settings.change_point.k = constrain(v, K_MIN_POS, K_MAX_POS); }
  #endif
  #if HAS_U_AXIS
    if (parser.seenval('C')) { const int16_t v = parser.TERN(AXIS7_ROTATES, value_int, value_linear_units)(); toolchange_settings.change_point.u = constrain(v, U_MIN_POS, U_MAX_POS); }
  #endif
  #if HAS_V_AXIS
    if (parser.seenval('H')) { const int16_t v = parser.TERN(AXIS8_ROTATES, value_int, value_linear_units)(); toolchange_settings.change_point.v = constrain(v, V_MIN_POS, V_MAX_POS); }
  #endif
  #if HAS_W_AXIS
    if (parser.seenval('O')) { const int16_t v = parser.TERN(AXIS9_ROTATES, value_int, value_linear_units)(); toolchange_settings.change_point.w = constrain(v, W_MIN_POS, W_MAX_POS); }
  #endif

  M216_report();
}

void GcodeSuite::M216_report(const bool forReplay/*=true*/) {

  SERIAL_ECHOPGM("  M216");

  SERIAL_ECHOPGM(" P", LINEAR_UNIT(toolchange_settings.enable_park));
  SERIAL_ECHOPGM(" R", LINEAR_UNIT(toolchange_settings.no_return));
  SERIAL_ECHOPGM_P(
        SP_X_STR, LINEAR_UNIT(toolchange_settings.change_point.x)
    #if HAS_Y_AXIS
      , SP_Y_STR, LINEAR_UNIT(toolchange_settings.change_point.y)
    #endif
    #if SECONDARY_AXES >= 1
      , LIST_N(DOUBLE(SECONDARY_AXES)
          , SP_I_STR,   I_AXIS_UNIT(toolchange_settings.change_point.i)
          , SP_J_STR,   J_AXIS_UNIT(toolchange_settings.change_point.j)
          , SP_K_STR,   K_AXIS_UNIT(toolchange_settings.change_point.k)
          , SP_C_STR,   U_AXIS_UNIT(toolchange_settings.change_point.u)
          , PSTR(" H"), V_AXIS_UNIT(toolchange_settings.change_point.v)
          , PSTR(" O"), W_AXIS_UNIT(toolchange_settings.change_point.w)
        )
    #endif
  );
}

#endif // HAS_MULTI_EXTRUDER && ENABLED(TOOLCHANGE_PARK)
