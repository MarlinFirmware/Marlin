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

#if HAS_MULTI_EXTRUDER

#include "../gcode.h"
#include "../../module/tool_change.h"

#if ENABLED(TOOLCHANGE_MIGRATION_FEATURE)
  #include "../../module/motion.h"
#endif

#include "../../MarlinCore.h" // for SP_X_STR, etc.

/**
 * M217 - Set toolchange parameters
 *
 *  // Tool change command
 *  Q           Prime active tool and exit
 *
 *  // Tool change settings
 *  S[linear]     Swap length
 *  B[linear]     Extra Swap resume length
 *  E[linear]     Extra Prime length (as used by M217 Q)
 *  P[linear/min] Prime speed
 *  R[linear/min] Retract speed
 *  U[linear/min] UnRetract speed
 *  V[linear]     0/1 Enable auto prime first extruder used
 *  W[linear]     0/1 Enable park & Z Raise
 *  X[linear]     Park X (Requires TOOLCHANGE_PARK)
 *  Y[linear]     Park Y (Requires TOOLCHANGE_PARK)
 *  I[linear]     Park I (Requires TOOLCHANGE_PARK and NUM_AXES >= 4)
 *  J[linear]     Park J (Requires TOOLCHANGE_PARK and NUM_AXES >= 5)
 *  K[linear]     Park K (Requires TOOLCHANGE_PARK and NUM_AXES >= 6)
 *  Z[linear]     Z Raise
 *  F[speed]      Fan Speed 0-255
 *  D[seconds]    Fan time
 *
 * Tool migration settings
 *  A[0|1]      Enable auto-migration on runout
 *  L[index]    Last extruder to use for auto-migration
 *
 * Tool migration command
 *  T[index]    Migrate to next extruder or the given extruder
 */
void GcodeSuite::M217() {

  #if ENABLED(TOOLCHANGE_FILAMENT_SWAP)

    static constexpr float max_extrude = TERN(PREVENT_LENGTHY_EXTRUDE, EXTRUDE_MAXLENGTH, 500);

    if (parser.seen('Q')) { tool_change_prime(); return; }

    if (parser.seenval('S')) { const float v = parser.value_linear_units(); toolchange_settings.swap_length = constrain(v, 0, max_extrude); }
    if (parser.seenval('B')) { const float v = parser.value_linear_units(); toolchange_settings.extra_resume = constrain(v, -10, 10); }
    if (parser.seenval('E')) { const float v = parser.value_linear_units(); toolchange_settings.extra_prime = constrain(v, 0, max_extrude); }
    if (parser.seenval('P')) { const int16_t v = parser.value_linear_units(); toolchange_settings.prime_speed = constrain(v, 10, 5400); }
    if (parser.seenval('R')) { const int16_t v = parser.value_linear_units(); toolchange_settings.retract_speed = constrain(v, 10, 5400); }
    if (parser.seenval('U')) { const int16_t v = parser.value_linear_units(); toolchange_settings.unretract_speed = constrain(v, 10, 5400); }
    #if TOOLCHANGE_FS_FAN >= 0 && HAS_FAN
      if (parser.seenval('F')) { const uint16_t v = parser.value_ushort(); toolchange_settings.fan_speed = constrain(v, 0, 255); }
      if (parser.seenval('D')) { const uint16_t v = parser.value_ushort(); toolchange_settings.fan_time = constrain(v, 1, 30); }
    #endif
  #endif

  #if ENABLED(TOOLCHANGE_FS_PRIME_FIRST_USED)
    if (parser.seenval('V')) { enable_first_prime = parser.value_linear_units(); }
  #endif

  #if ENABLED(TOOLCHANGE_PARK)
    if (parser.seenval('W')) { toolchange_settings.enable_park = parser.value_linear_units(); }
    if (parser.seenval('X')) { const int16_t v = parser.value_linear_units(); toolchange_settings.change_point.x = constrain(v, X_MIN_POS, X_MAX_POS); }
    #if HAS_Y_AXIS
      if (parser.seenval('Y')) { const int16_t v = parser.value_linear_units(); toolchange_settings.change_point.y = constrain(v, Y_MIN_POS, Y_MAX_POS); }
    #endif
    #if HAS_I_AXIS
      if (parser.seenval('I')) { const int16_t v = parser.value_linear_units(); toolchange_settings.change_point.i = constrain(v, I_MIN_POS, I_MAX_POS); }
    #endif
    #if HAS_J_AXIS
      if (parser.seenval('J')) { const int16_t v = parser.value_linear_units(); toolchange_settings.change_point.j = constrain(v, J_MIN_POS, J_MAX_POS); }
    #endif
    #if HAS_K_AXIS
      if (parser.seenval('K')) { const int16_t v = parser.value_linear_units(); toolchange_settings.change_point.k = constrain(v, K_MIN_POS, K_MAX_POS); }
    #endif
  #endif

  #if HAS_Z_AXIS
    if (parser.seenval('Z')) { toolchange_settings.z_raise = parser.value_linear_units(); }
  #endif

  #if ENABLED(TOOLCHANGE_MIGRATION_FEATURE)
    migration.target = 0;       // 0 = disabled

    if (parser.seenval('L')) {  // Last
      const int16_t lval = parser.value_int();
      if (WITHIN(lval, 0, EXTRUDERS - 1)) {
        migration.last = lval;
        migration.automode = (active_extruder < migration.last);
      }
    }

    if (parser.seen('A'))       // Auto on/off
      migration.automode = parser.value_bool();

    if (parser.seen('T')) {     // Migrate now
      if (parser.has_value()) {
        const int16_t tval = parser.value_int();
        if (WITHIN(tval, 0, EXTRUDERS - 1) && tval != active_extruder) {
          migration.target = tval + 1;
          extruder_migration();
          migration.target = 0; // disable
          return;
        }
        else
          migration.target = 0; // disable
      }
      else {
        extruder_migration();
        return;
      }
    }

  #endif

  M217_report();
}

void GcodeSuite::M217_report(const bool forReplay/*=true*/) {
  report_heading_etc(forReplay, F(STR_TOOL_CHANGING));

  SERIAL_ECHOPGM("  M217");

  #if ENABLED(TOOLCHANGE_FILAMENT_SWAP)
    SERIAL_ECHOPGM(" S", LINEAR_UNIT(toolchange_settings.swap_length));
    SERIAL_ECHOPGM_P(SP_B_STR, LINEAR_UNIT(toolchange_settings.extra_resume),
                     SP_E_STR, LINEAR_UNIT(toolchange_settings.extra_prime),
                     SP_P_STR, LINEAR_UNIT(toolchange_settings.prime_speed));
    SERIAL_ECHOPGM(" R", LINEAR_UNIT(toolchange_settings.retract_speed),
                   " U", LINEAR_UNIT(toolchange_settings.unretract_speed),
                   " F", toolchange_settings.fan_speed,
                   " D", toolchange_settings.fan_time);

    #if ENABLED(TOOLCHANGE_MIGRATION_FEATURE)
      SERIAL_ECHOPGM(" A", migration.automode);
      SERIAL_ECHOPGM(" L", LINEAR_UNIT(migration.last));
    #endif

    #if ENABLED(TOOLCHANGE_PARK)
    {
      SERIAL_ECHOPGM(" W", LINEAR_UNIT(toolchange_settings.enable_park));
      SERIAL_ECHOPGM_P(
            SP_X_STR, LINEAR_UNIT(toolchange_settings.change_point.x)
        #if HAS_Y_AXIS
          , SP_Y_STR, LINEAR_UNIT(toolchange_settings.change_point.y)
        #endif
        #if SECONDARY_AXES >= 1
          , LIST_N(DOUBLE(SECONDARY_AXES),
              SP_I_STR, I_AXIS_UNIT(toolchange_settings.change_point.i),
              SP_J_STR, J_AXIS_UNIT(toolchange_settings.change_point.j),
              SP_K_STR, K_AXIS_UNIT(toolchange_settings.change_point.k)
            )
        #endif
      );
    }
    #endif

    #if ENABLED(TOOLCHANGE_FS_PRIME_FIRST_USED)
      SERIAL_ECHOPGM(" V", LINEAR_UNIT(enable_first_prime));
    #endif

  #endif

  SERIAL_ECHOLNPGM_P(SP_Z_STR, LINEAR_UNIT(toolchange_settings.z_raise));
}

#endif // HAS_MULTI_EXTRUDER
