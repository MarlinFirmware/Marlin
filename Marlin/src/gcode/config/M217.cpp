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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "../../inc/MarlinConfigPre.h"

#if EXTRUDERS > 1

#include "../gcode.h"
#include "../../module/tool_change.h"

#if ENABLED(TOOLCHANGE_MIGRATION_FEATURE)
  #include "../../module/motion.h"
#endif

#include "../../MarlinCore.h" // for SP_X_STR, etc.

extern const char SP_X_STR[], SP_Y_STR[], SP_Z_STR[];

void M217_report(const bool eeprom=false) {

  #if ENABLED(TOOLCHANGE_FILAMENT_SWAP)
    serialprintPGM(eeprom ? PSTR("  M217") : PSTR("Toolchange:"));
    SERIAL_ECHOPAIR(" S", LINEAR_UNIT(toolchange_settings.swap_length),
                    " B", LINEAR_UNIT(toolchange_settings.extra_resume));
    SERIAL_ECHOPAIR_P(SP_E_STR, LINEAR_UNIT(toolchange_settings.extra_prime),
                      SP_P_STR, LINEAR_UNIT(toolchange_settings.prime_speed));
    SERIAL_ECHOPAIR(" R", LINEAR_UNIT(toolchange_settings.retract_speed),
                    " U", LINEAR_UNIT(toolchange_settings.unretract_speed),
                    " F", toolchange_settings.fan_speed,
                    " G", toolchange_settings.fan_time);

    #if ENABLED(TOOLCHANGE_MIGRATION_FEATURE)
      SERIAL_ECHOPAIR(" N", int(migration_auto));
      SERIAL_ECHOPAIR(" L", LINEAR_UNIT(migration_ending));
    #endif

    #if ENABLED(TOOLCHANGE_PARK)
      SERIAL_ECHOPAIR(" W", LINEAR_UNIT(toolchange_settings.enable_park));
      SERIAL_ECHOPAIR_P(SP_X_STR, LINEAR_UNIT(toolchange_settings.change_point.x));
      SERIAL_ECHOPAIR_P(SP_Y_STR, LINEAR_UNIT(toolchange_settings.change_point.y));
    #endif

  #else

    UNUSED(eeprom);

  #endif

  SERIAL_ECHOPAIR_P(SP_Z_STR, LINEAR_UNIT(toolchange_settings.z_raise));
  SERIAL_EOL();
}

/**
 * M217 - Set SINGLENOZZLE toolchange parameters
 *
 *  // Tool change command
 *  A           Prime active tool and exit
 *
 *  // Tool change settings
 *  S[linear]   Swap length
 *  B[linear]   Extra Swap length
 *  E[linear]   Prime length
 *  P[linear/m] Prime speed
 *  R[linear/m] Retract speed
 *  U[linear/m] UnRetract speed
 *  V[linear]   0/1 Enable auto prime first extruder used
 *  W[linear]   0/1 Enable park & Z Raise
 *  X[linear]   Park X (Requires TOOLCHANGE_PARK)
 *  Y[linear]   Park Y (Requires TOOLCHANGE_PARK)
 *  Z[linear]   Z Raise
 *  F[linear]   Fan Speed 0-255
 *  G[linear/s] Fan time
 *
 * Tool migration settings
 *  C[0|1]      Enable auto-migration on runout
 *  L[index]    Last extruder to use for auto-migration
 *
 * Tool migration command
 *  T[index]    Migrate to next extruder or the given extruder
 */
void GcodeSuite::M217() {

  #if ENABLED(TOOLCHANGE_FILAMENT_SWAP)

    static constexpr float max_extrude = TERN(PREVENT_LENGTHY_EXTRUDE, EXTRUDE_MAXLENGTH, 500);

    if (parser.seen('A')) { tool_change_prime(); return; }
    if (parser.seenval('S')) { const float v = parser.value_linear_units(); toolchange_settings.swap_length = constrain(v, 0, max_extrude); }
    if (parser.seenval('B')) { const float v = parser.value_linear_units(); toolchange_settings.extra_resume = constrain(v, -10, 10); }
    if (parser.seenval('E')) { const float v = parser.value_linear_units(); toolchange_settings.extra_prime = constrain(v, 0, max_extrude); }
    if (parser.seenval('P')) { const int16_t v = parser.value_linear_units(); toolchange_settings.prime_speed = constrain(v, 10, 5400); }
    if (parser.seenval('R')) { const int16_t v = parser.value_linear_units(); toolchange_settings.retract_speed = constrain(v, 10, 5400); }
    if (parser.seenval('U')) { const int16_t v = parser.value_linear_units(); toolchange_settings.unretract_speed = constrain(v, 10, 5400); }
    #if TOOLCHANGE_FIL_SWAP_FAN >= 0 && FAN_COUNT > 0
      if (parser.seenval('F')) { const int16_t v = parser.value_linear_units(); toolchange_settings.fan_speed = constrain(v, 0, 255); }
      if (parser.seenval('G')) { const int16_t v = parser.value_linear_units(); toolchange_settings.fan_time = constrain(v, 1, 30); }
    #endif
  #endif

  #if ENABLED(TOOLCHANGE_FIL_PRIME_FIRST_USED)
    if (parser.seenval('V')) { enable_first_prime = parser.value_linear_units(); }
  #endif

  #if ENABLED(TOOLCHANGE_PARK)
    if (parser.seenval('W')) { toolchange_settings.enable_park = parser.value_linear_units(); }
    if (parser.seenval('X')) { const int16_t v = parser.value_linear_units(); toolchange_settings.change_point.x = constrain(v, X_MIN_POS, X_MAX_POS); }
    if (parser.seenval('Y')) { const int16_t v = parser.value_linear_units(); toolchange_settings.change_point.y = constrain(v, Y_MIN_POS, Y_MAX_POS); }
  #endif

  if (parser.seenval('Z')) { toolchange_settings.z_raise = parser.value_linear_units(); }

  #if ENABLED(TOOLCHANGE_MIGRATION_FEATURE)
    migration_target = -1;  // init = disable = negative

    if (parser.seenval('L')) {  // ending
      const float lval = parser.value_linear_units();
      if (WITHIN(lval, 0, EXTRUDERS - 1)) {
        migration_ending = lval;
        migration_auto = (active_extruder < migration_ending);
      }
    }

    if (parser.seenval('C')) {  // auto on/off
      const float cval = parser.value_linear_units();
      if (WITHIN(cval, 0, 1)) migration_auto = cval;
    }

    if (parser.seen('T')) {     // Migrate now
      if (parser.has_value()) {
        const float tval = parser.value_linear_units();
        if (WITHIN(tval, 0, EXTRUDERS - 1) && tval != active_extruder) {
          migration_target = tval;
          extruder_migration();
          migration_target = -1;  // disable
          return;
        }
        else
          migration_target = -1;  // disable
      }
      else {
        extruder_migration();
        return;
      }
    }

  #endif

  M217_report();
}

#endif // EXTRUDERS > 1
