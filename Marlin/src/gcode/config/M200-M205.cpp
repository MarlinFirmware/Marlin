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

#include "../gcode.h"
#include "../../MarlinCore.h"
#include "../../module/planner.h"

#if DISABLED(NO_VOLUMETRICS)

  /**
   * M200: Set filament diameter and set E axis units to cubic units
   *
   *    T<extruder> - Optional extruder number. Current extruder if omitted.
   *    D<linear>   - Set filament diameter and enable. D0 disables volumetric.
   *    S<bool>     - Turn volumetric ON or OFF.
   *
   * With VOLUMETRIC_EXTRUDER_LIMIT:
   *
   *    L<float>    - Volumetric extruder limit (in mm^3/sec). L0 disables the limit.
   */
  void GcodeSuite::M200() {
    if (!parser.seen("DST" TERN_(VOLUMETRIC_EXTRUDER_LIMIT, "L")))
      return M200_report();

    const int8_t target_extruder = get_target_extruder_from_command();
    if (target_extruder < 0) return;

    bool vol_enable = parser.volumetric_enabled,
         can_enable = true;

    if (parser.seenval('D')) {
      const float dval = parser.value_linear_units();
      if (dval) { // Set filament size for volumetric calculation
        planner.set_filament_size(target_extruder, dval);
        vol_enable = true;    // Dn = enable for compatibility
      }
      else
        can_enable = false;   // D0 = disable for compatibility
    }

    // Enable or disable with S1 / S0
    parser.volumetric_enabled = can_enable && parser.boolval('S', vol_enable);

    #if ENABLED(VOLUMETRIC_EXTRUDER_LIMIT)
      if (parser.seenval('L')) {
        // Set volumetric limit (in mm^3/sec)
        const float lval = parser.value_float();
        if (WITHIN(lval, 0, 20))
          planner.set_volumetric_extruder_limit(target_extruder, lval);
        else
          SERIAL_ECHOLNPGM("?L value out of range (0-20).");
      }
    #endif

    planner.calculate_volumetric_multipliers();
  }

  void GcodeSuite::M200_report(const bool forReplay/*=true*/) {
    if (!forReplay) {
      report_heading(forReplay, F(STR_FILAMENT_SETTINGS), false);
      if (!parser.volumetric_enabled) SERIAL_ECHOPGM(" (Disabled):");
      SERIAL_EOL();
      report_echo_start(forReplay);
    }

    #if EXTRUDERS == 1
    {
      SERIAL_ECHOLNPGM(
        "  M200 S", parser.volumetric_enabled, " D", LINEAR_UNIT(planner.filament_size[0])
        #if ENABLED(VOLUMETRIC_EXTRUDER_LIMIT)
          , " L", LINEAR_UNIT(planner.volumetric_extruder_limit[0])
        #endif
      );
    }
    #else
      SERIAL_ECHOLNPGM("  M200 S", parser.volumetric_enabled);
      LOOP_L_N(i, EXTRUDERS) {
        report_echo_start(forReplay);
        SERIAL_ECHOLNPGM(
          "  M200 T", i, " D", LINEAR_UNIT(planner.filament_size[i])
          #if ENABLED(VOLUMETRIC_EXTRUDER_LIMIT)
            , " L", LINEAR_UNIT(planner.volumetric_extruder_limit[i])
          #endif
        );
      }
    #endif
  }

#endif // !NO_VOLUMETRICS

/**
 * M201: Set max acceleration in units/s^2 for print moves (M201 X1000 Y1000)
 *
 *       With multiple extruders use T to specify which one.
 */
void GcodeSuite::M201() {
  if (!parser.seen("T" LOGICAL_AXES_STRING))
    return M201_report();

  const int8_t target_extruder = get_target_extruder_from_command();
  if (target_extruder < 0) return;

  #ifdef XY_FREQUENCY_LIMIT
    if (parser.seenval('F')) planner.set_frequency_limit(parser.value_byte());
    if (parser.seenval('G')) planner.xy_freq_min_speed_factor = constrain(parser.value_float(), 1, 100) / 100;
  #endif

  LOOP_LOGICAL_AXES(i) {
    if (parser.seenval(axis_codes[i])) {
      const uint8_t a = TERN(HAS_EXTRUDERS, (i == E_AXIS ? uint8_t(E_AXIS_N(target_extruder)) : i), i);
      planner.set_max_acceleration(a, parser.value_axis_units((AxisEnum)a));
    }
  }
}

void GcodeSuite::M201_report(const bool forReplay/*=true*/) {
  report_heading_etc(forReplay, F(STR_MAX_ACCELERATION));
  SERIAL_ECHOLNPGM_P(
    LIST_N(DOUBLE(LINEAR_AXES),
      PSTR("  M201 X"), LINEAR_UNIT(planner.settings.max_acceleration_mm_per_s2[X_AXIS]),
      SP_Y_STR, LINEAR_UNIT(planner.settings.max_acceleration_mm_per_s2[Y_AXIS]),
      SP_Z_STR, LINEAR_UNIT(planner.settings.max_acceleration_mm_per_s2[Z_AXIS]),
      SP_I_STR, LINEAR_UNIT(planner.settings.max_acceleration_mm_per_s2[I_AXIS]),
      SP_J_STR, LINEAR_UNIT(planner.settings.max_acceleration_mm_per_s2[J_AXIS]),
      SP_K_STR, LINEAR_UNIT(planner.settings.max_acceleration_mm_per_s2[K_AXIS])
    )
    #if HAS_EXTRUDERS && DISABLED(DISTINCT_E_FACTORS)
      , SP_E_STR, VOLUMETRIC_UNIT(planner.settings.max_acceleration_mm_per_s2[E_AXIS])
    #endif
  );
  #if ENABLED(DISTINCT_E_FACTORS)
    LOOP_L_N(i, E_STEPPERS) {
      report_echo_start(forReplay);
      SERIAL_ECHOLNPGM_P(
          PSTR("  M201 T"), i
        , SP_E_STR, VOLUMETRIC_UNIT(planner.settings.max_acceleration_mm_per_s2[E_AXIS_N(i)])
      );
    }
  #endif
}

/**
 * M203: Set maximum feedrate that your machine can sustain (M203 X200 Y200 Z300 E10000) in units/sec
 *
 *       With multiple extruders use T to specify which one.
 */
void GcodeSuite::M203() {
  if (!parser.seen("T" LOGICAL_AXES_STRING))
    return M203_report();

  const int8_t target_extruder = get_target_extruder_from_command();
  if (target_extruder < 0) return;

  LOOP_LOGICAL_AXES(i)
    if (parser.seenval(axis_codes[i])) {
      const uint8_t a = TERN(HAS_EXTRUDERS, (i == E_AXIS ? uint8_t(E_AXIS_N(target_extruder)) : i), i);
      planner.set_max_feedrate(a, parser.value_axis_units((AxisEnum)a));
    }
}

void GcodeSuite::M203_report(const bool forReplay/*=true*/) {
  report_heading_etc(forReplay, F(STR_MAX_FEEDRATES));
  SERIAL_ECHOLNPGM_P(
    LIST_N(DOUBLE(LINEAR_AXES),
      PSTR("  M203 X"), LINEAR_UNIT(planner.settings.max_feedrate_mm_s[X_AXIS]),
      SP_Y_STR, LINEAR_UNIT(planner.settings.max_feedrate_mm_s[Y_AXIS]),
      SP_Z_STR, LINEAR_UNIT(planner.settings.max_feedrate_mm_s[Z_AXIS]),
      SP_I_STR, LINEAR_UNIT(planner.settings.max_feedrate_mm_s[I_AXIS]),
      SP_J_STR, LINEAR_UNIT(planner.settings.max_feedrate_mm_s[J_AXIS]),
      SP_K_STR, LINEAR_UNIT(planner.settings.max_feedrate_mm_s[K_AXIS])
    )
    #if HAS_EXTRUDERS && DISABLED(DISTINCT_E_FACTORS)
      , SP_E_STR, VOLUMETRIC_UNIT(planner.settings.max_feedrate_mm_s[E_AXIS])
    #endif
  );
  #if ENABLED(DISTINCT_E_FACTORS)
    LOOP_L_N(i, E_STEPPERS) {
      SERIAL_ECHO_START();
      SERIAL_ECHOLNPGM_P(
          PSTR("  M203 T"), i
        , SP_E_STR, VOLUMETRIC_UNIT(planner.settings.max_feedrate_mm_s[E_AXIS_N(i)])
      );
    }
  #endif
}

/**
 * M204: Set Accelerations in units/sec^2 (M204 P1200 R3000 T3000)
 *
 *    P = Printing moves
 *    R = Retract only (no X, Y, Z) moves
 *    T = Travel (non printing) moves
 */
void GcodeSuite::M204() {
  if (!parser.seen("PRST"))
    return M204_report();
  else {
    //planner.synchronize();
    // 'S' for legacy compatibility. Should NOT BE USED for new development
    if (parser.seenval('S')) planner.settings.travel_acceleration = planner.settings.acceleration = parser.value_linear_units();
    if (parser.seenval('P')) planner.settings.acceleration = parser.value_linear_units();
    if (parser.seenval('R')) planner.settings.retract_acceleration = parser.value_linear_units();
    if (parser.seenval('T')) planner.settings.travel_acceleration = parser.value_linear_units();
  }
}

void GcodeSuite::M204_report(const bool forReplay/*=true*/) {
  report_heading_etc(forReplay, F(STR_ACCELERATION_P_R_T));
  SERIAL_ECHOLNPGM_P(
      PSTR("  M204 P"), LINEAR_UNIT(planner.settings.acceleration)
    , PSTR(" R"), LINEAR_UNIT(planner.settings.retract_acceleration)
    , SP_T_STR, LINEAR_UNIT(planner.settings.travel_acceleration)
  );
}

/**
 * M205: Set Advanced Settings
 *
 *    B = Min Segment Time (µs)
 *    S = Min Feed Rate (units/s)
 *    T = Min Travel Feed Rate (units/s)
 *    X = Max X Jerk (units/sec^2)
 *    Y = Max Y Jerk (units/sec^2)
 *    Z = Max Z Jerk (units/sec^2)
 *    E = Max E Jerk (units/sec^2)
 *    J = Junction Deviation (mm) (If not using CLASSIC_JERK)
 */
void GcodeSuite::M205() {
  if (!parser.seen("BST" TERN_(HAS_JUNCTION_DEVIATION, "J") TERN_(HAS_CLASSIC_JERK, "XYZE")))
    return M205_report();

  //planner.synchronize();
  if (parser.seenval('B')) planner.settings.min_segment_time_us = parser.value_ulong();
  if (parser.seenval('S')) planner.settings.min_feedrate_mm_s = parser.value_linear_units();
  if (parser.seenval('T')) planner.settings.min_travel_feedrate_mm_s = parser.value_linear_units();
  #if HAS_JUNCTION_DEVIATION
    #if HAS_CLASSIC_JERK && AXIS_COLLISION('J')
      #error "Can't set_max_jerk for 'J' axis because 'J' is used for Junction Deviation."
    #endif
    if (parser.seenval('J')) {
      const float junc_dev = parser.value_linear_units();
      if (WITHIN(junc_dev, 0.01f, 0.3f)) {
        planner.junction_deviation_mm = junc_dev;
        TERN_(LIN_ADVANCE, planner.recalculate_max_e_jerk());
      }
      else
        SERIAL_ERROR_MSG("?J out of range (0.01 to 0.3)");
    }
  #endif
  #if HAS_CLASSIC_JERK
    bool seenZ = false;
    LOGICAL_AXIS_CODE(
      if (parser.seenval('E')) planner.set_max_jerk(E_AXIS, parser.value_linear_units()),
      if (parser.seenval('X')) planner.set_max_jerk(X_AXIS, parser.value_linear_units()),
      if (parser.seenval('Y')) planner.set_max_jerk(Y_AXIS, parser.value_linear_units()),
      if ((seenZ = parser.seenval('Z'))) planner.set_max_jerk(Z_AXIS, parser.value_linear_units()),
      if (parser.seenval(AXIS4_NAME)) planner.set_max_jerk(I_AXIS, parser.value_linear_units()),
      if (parser.seenval(AXIS5_NAME)) planner.set_max_jerk(J_AXIS, parser.value_linear_units()),
      if (parser.seenval(AXIS6_NAME)) planner.set_max_jerk(K_AXIS, parser.value_linear_units())
    );
    #if HAS_MESH && DISABLED(LIMITED_JERK_EDITING)
      if (seenZ && planner.max_jerk.z <= 0.1f)
        SERIAL_ECHOLNPGM("WARNING! Low Z Jerk may lead to unwanted pauses.");
    #endif
  #endif // HAS_CLASSIC_JERK
}

void GcodeSuite::M205_report(const bool forReplay/*=true*/) {
  report_heading_etc(forReplay, F(
    "Advanced (B<min_segment_time_us> S<min_feedrate> T<min_travel_feedrate>"
    TERN_(HAS_JUNCTION_DEVIATION, " J<junc_dev>")
    #if HAS_CLASSIC_JERK
      LINEAR_AXIS_GANG(
        " X<max_jerk>", " Y<max_jerk>", " Z<max_jerk>",
        " " STR_I "<max_jerk>", " " STR_J "<max_jerk>", " " STR_K "<max_jerk>"
      )
    #endif
    TERN_(HAS_CLASSIC_E_JERK, " E<max_jerk>")
    ")"
  ));
  SERIAL_ECHOLNPGM_P(
      PSTR("  M205 B"), LINEAR_UNIT(planner.settings.min_segment_time_us)
    , PSTR(" S"), LINEAR_UNIT(planner.settings.min_feedrate_mm_s)
    , SP_T_STR, LINEAR_UNIT(planner.settings.min_travel_feedrate_mm_s)
    #if HAS_JUNCTION_DEVIATION
      , PSTR(" J"), LINEAR_UNIT(planner.junction_deviation_mm)
    #endif
    #if HAS_CLASSIC_JERK
      , LIST_N(DOUBLE(LINEAR_AXES),
        SP_X_STR, LINEAR_UNIT(planner.max_jerk.x),
        SP_Y_STR, LINEAR_UNIT(planner.max_jerk.y),
        SP_Z_STR, LINEAR_UNIT(planner.max_jerk.z),
        SP_I_STR, LINEAR_UNIT(planner.max_jerk.i),
        SP_J_STR, LINEAR_UNIT(planner.max_jerk.j),
        SP_K_STR, LINEAR_UNIT(planner.max_jerk.k)
      )
      #if HAS_CLASSIC_E_JERK
        , SP_E_STR, LINEAR_UNIT(planner.max_jerk.e)
      #endif
    #endif
  );
}
