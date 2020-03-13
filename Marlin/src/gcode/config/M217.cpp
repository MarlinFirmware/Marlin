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
    SERIAL_ECHOPAIR(" S", LINEAR_UNIT(toolchange_settings.swap_length));
    SERIAL_ECHOPAIR_P(SP_E_STR, LINEAR_UNIT(toolchange_settings.extra_prime));
    SERIAL_ECHOPAIR_P(SP_P_STR, LINEAR_UNIT(toolchange_settings.prime_speed));
    SERIAL_ECHOPAIR(" R", LINEAR_UNIT(toolchange_settings.retract_speed));
    SERIAL_ECHOPAIR(" UR", LINEAR_UNIT(toolchange_settings.unretract_speed));
    SERIAL_ECHOPAIR(" FANS", LINEAR_UNIT(toolchange_settings.fan_speed));
    SERIAL_ECHOPAIR(" FanT", LINEAR_UNIT(toolchange_settings.fan_time));
    #if ENABLED(TOOLCHANGE_MIGRATION_FEATURE)
      SERIAL_ECHOPAIR(" AutoMig", LINEAR_UNIT(toolchange_settings.migration_auto));
      SERIAL_ECHOPAIR(" MigL", LINEAR_UNIT(toolchange_settings.migration_ending));
    #endif

    #if ENABLED(TOOLCHANGE_PARK)
      SERIAL_ECHOPAIR(" ParK", LINEAR_UNIT(toolchange_settings.enable_park));
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
 *  // Tool change
 *  S[linear]   Swap length
 *  E[linear]   Prime length
 *  P[linear/m] Prime speed
 *  R[linear/m] Retract speed
 *  U[linear/m] UnRetract speed
 *  W[linear]   Enable park & Z Raise
 *  X[linear]   Park X (Requires TOOLCHANGE_PARK)
 *  Y[linear]   Park Y (Requires TOOLCHANGE_PARK)
 *  Z[linear]   Z Raise
 *  F[linear]   Fan Speed 0-255
 *  G[linear/s] Fan time
 *
 * //Commands
 *  A           Prime active tool
 *
 *  Tool migration
 *
 *  //Settings
 *  L[linear]   1/2/3/4/5/6/7 - End/last extruder to reach after runouts
 *  N[linear]   0/1 Auto Migration to next extruder enable (By Runout)
 *
 *  //Commands
 *  Q           Migrate to next
 *  T[linear]   0/1/2/3/4/5/6/7 : Migration to desired extruder
 */
void GcodeSuite::M217() {

  #define SPR_PARAM
  #define XY_PARAM

  #if ENABLED(TOOLCHANGE_FILAMENT_SWAP)

    #undef SPR_PARAM
    #define SPR_PARAM "SPRE"

    static constexpr float max_extrude =
      #if ENABLED(PREVENT_LENGTHY_EXTRUDE)
        EXTRUDE_MAXLENGTH
      #else
        500
      #endif
    ;
    if (parser.seen('A')) { tool_change_prime(); return; }
    if (parser.seenval('S')) { const float v = parser.value_linear_units(); toolchange_settings.swap_length = constrain(v, 0, max_extrude); }
    if (parser.seenval('E')) { const float v = parser.value_linear_units(); toolchange_settings.extra_prime = constrain(v, 0, max_extrude); }
    if (parser.seenval('P')) { const int16_t v = parser.value_linear_units(); toolchange_settings.prime_speed = constrain(v, 10, 5400); }
    if (parser.seenval('R')) { const int16_t v = parser.value_linear_units(); toolchange_settings.retract_speed = constrain(v, 10, 5400); }
    if (parser.seenval('U')) { const int16_t v = parser.value_linear_units(); toolchange_settings.unretract_speed = constrain(v, 10, 5400); }
    #if TOOLCHANGE_FIL_SWAP_FAN >= 0 && FAN_COUNT > 0
      if (parser.seenval('F')) { const int16_t v = parser.value_linear_units(); toolchange_settings.fan_speed = constrain(v, 0, 255); }
      if (parser.seenval('G')) { const int16_t v = parser.value_linear_units(); toolchange_settings.fan_time = constrain(v, 1, 30); }
    #endif
  #endif

  #if ENABLED(TOOLCHANGE_PARK)
    #undef XY_PARAM
    #define XY_PARAM "XY"
    if (parser.seenval('W')) { toolchange_settings.enable_park = parser.value_linear_units(); }
    if (parser.seenval('X')) { toolchange_settings.change_point.x = parser.value_linear_units(); }
    if (parser.seenval('Y')) { toolchange_settings.change_point.y = parser.value_linear_units(); }
  #endif

  if (parser.seenval('Z')) { toolchange_settings.z_raise = parser.value_linear_units(); }

  #if ENABLED(TOOLCHANGE_MIGRATION_FEATURE)
    toolchange_settings.migration_target = -1;// init = disable = negative

    if (parser.seenval('L')) { //ending
     if(   (parser.value_linear_units() > 0 ) && (parser.value_linear_units() < EXTRUDERS )){
       toolchange_settings.migration_ending = parser.value_linear_units();
       if  (active_extruder >= toolchange_settings.migration_ending ) toolchange_settings.migration_auto = false;
       else  toolchange_settings.migration_auto = true;     }
     else return;
    }

    if (parser.seenval('N')) { //auto on/off
      if((parser.value_linear_units() >= 0 ) && (parser.value_linear_units() <= 1))
        toolchange_settings.migration_auto = parser.value_linear_units();
      else return;
    }

    if (parser.seenval('T')) { //specific
      if(   (parser.value_linear_units() >= 0 )
         && (parser.value_linear_units() < EXTRUDERS )
         && (parser.value_linear_units() != active_extruder)
        ){
        toolchange_settings.migration_target = parser.value_linear_units();
        extruder_migration();
        return ;
      }
      else return;
    }

    if (parser.seen('Q')) { extruder_migration(); return; }

  #endif
  if (!parser.seen(SPR_PARAM XY_PARAM "Z")) M217_report();
  }
#endif // EXTRUDERS > 1
