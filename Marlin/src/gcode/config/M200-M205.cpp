/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../gcode.h"
#include "../../Marlin.h"
#include "../../module/planner.h"

#if DISABLED(NO_VOLUMETRICS)

  /**
   * M200: Set filament diameter and set E axis units to cubic units
   *
   *    T<extruder> - Optional extruder number. Current extruder if omitted.
   *    D<linear> - Diameter of the filament. Use "D0" to switch back to linear units on the E axis.
   */
  void GcodeSuite::M200() {

    const int8_t target_extruder = get_target_extruder_from_command();
    if (target_extruder < 0) return;

    if (parser.seen('D')) {
      // setting any extruder filament size disables volumetric on the assumption that
      // slicers either generate in extruder values as cubic mm or as as filament feeds
      // for all extruders
      if ( (parser.volumetric_enabled = (parser.value_linear_units() != 0)) )
        planner.set_filament_size(target_extruder, parser.value_linear_units());
    }
    planner.calculate_volumetric_multipliers();
  }

#endif // !NO_VOLUMETRICS

/**
 * M201: Set max acceleration in units/s^2 for print moves (M201 X1000 Y1000)
 *
 *       With multiple extruders use T to specify which one.
 */
void GcodeSuite::M201() {

  const int8_t target_extruder = get_target_extruder_from_command();
  if (target_extruder < 0) return;

  LOOP_XYZE(i) {
    if (parser.seen(axis_codes[i])) {
      const uint8_t a = (i == E_AXIS ? E_AXIS_N(target_extruder) : i);
      #if DISABLED(MAX_ACCELERATION_CAP)
        planner.settings.max_acceleration_mm_per_s2[a] = parser.value_axis_units((AxisEnum)a);
      #else
        #ifdef MAX_ACCELERATION_MANUAL
          static constexpr float max_accel[] = MAX_ACCELERATION_MANUAL;
          static uint8_t ac_multiplier = 1;
        #else
          static constexpr float max_accel[] = DEFAULT_MAX_ACCELERATION;
          static uint8_t ac_multiplier = 2;
        #endif
        if(parser.value_axis_units((AxisEnum)a) > max_accel[(AxisEnum)a] * ac_multiplier)
          SERIAL_ECHOLNPAIR("Max acceleration clamped to ",  (max_accel[(AxisEnum)a] * ac_multiplier));
        planner.settings.max_acceleration_mm_per_s2[a] = constrain(parser.value_axis_units((AxisEnum)a), 1, max_accel[(AxisEnum)a] * ac_multiplier);
      #endif
    }
  }
  // steps per sq second need to be updated to agree with the units per sq second (as they are what is used in the planner)
  planner.reset_acceleration_rates();
}

/**
 * M203: Set maximum feedrate that your machine can sustain (M203 X200 Y200 Z300 E10000) in units/sec
 *
 *       With multiple extruders use T to specify which one.
 */
void GcodeSuite::M203() {

  const int8_t target_extruder = get_target_extruder_from_command();
  if (target_extruder < 0) return;

  LOOP_XYZE(i)
    if (parser.seen(axis_codes[i])) {
      const uint8_t a = (i == E_AXIS ? E_AXIS_N(target_extruder) : i);
      #if DISABLED(MAX_FEEDRATE_CAP)
        planner.settings.max_feedrate_mm_s[a] = parser.value_axis_units((AxisEnum)a);
      #else
        #ifdef MAX_FEEDRATE_MANUAL
          static constexpr float max_feedrates[] = MAX_FEEDRATE_MANUAL;
          static uint8_t fr_multiplier = 1;
        #else
          static constexpr float max_feedrates[] = DEFAULT_MAX_FEEDRATE;
          static uint8_t fr_multiplier = 2;
        #endif
        if(parser.value_axis_units((AxisEnum)a) > max_feedrates[(AxisEnum)a] * fr_multiplier)
          SERIAL_ECHOLNPAIR("Max feedrate clamped to ",  (max_feedrates[(AxisEnum)a] * fr_multiplier));
        planner.settings.max_feedrate_mm_s[a] = constrain(parser.value_axis_units((AxisEnum)a), 1, max_feedrates[(AxisEnum)a] * fr_multiplier);
      #endif
    }
}

/**
 * M204: Set Accelerations in units/sec^2 (M204 P1200 R3000 T3000)
 *
 *    P = Printing moves
 *    R = Retract only (no X, Y, Z) moves
 *    T = Travel (non printing) moves
 */
void GcodeSuite::M204() {
  if (!parser.seen("PRST")) {
    SERIAL_ECHOPAIR("Acceleration: P", planner.settings.acceleration);
    SERIAL_ECHOPAIR(" R", planner.settings.retract_acceleration);
    SERIAL_ECHOLNPAIR(" T", planner.settings.travel_acceleration);
  }
  else {
    //planner.synchronize();
    // 'S' for legacy compatibility. Should NOT BE USED for new development
    if (parser.seenval('S')) planner.settings.travel_acceleration = planner.settings.acceleration = parser.value_linear_units();
    if (parser.seenval('P')) planner.settings.acceleration = parser.value_linear_units();
    if (parser.seenval('R')) planner.settings.retract_acceleration = parser.value_linear_units();
    if (parser.seenval('T')) planner.settings.travel_acceleration = parser.value_linear_units();
  }
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
 *    J = Junction Deviation (mm) (Requires JUNCTION_DEVIATION)
 */
void GcodeSuite::M205() {
  #if ENABLED(JUNCTION_DEVIATION)
    #define J_PARAM  "J"
  #else
    #define J_PARAM
  #endif
  #if HAS_CLASSIC_JERK
    #define XYZE_PARAM "XYZE"
  #else
    #define XYZE_PARAM
  #endif
  if (!parser.seen("BST" J_PARAM XYZE_PARAM)) return;

  //planner.synchronize();
  if (parser.seen('B')) planner.settings.min_segment_time_us = parser.value_ulong();
  if (parser.seen('S')) planner.settings.min_feedrate_mm_s = parser.value_linear_units();
  if (parser.seen('T')) planner.settings.min_travel_feedrate_mm_s = parser.value_linear_units();
  #if ENABLED(JUNCTION_DEVIATION)
    if (parser.seen('J')) {
      const float junc_dev = parser.value_linear_units();
      if (WITHIN(junc_dev, 0.01f, 0.3f)) {
        planner.junction_deviation_mm = junc_dev;
        #if ENABLED(LIN_ADVANCE)
          planner.recalculate_max_e_jerk();
        #endif
      }
      else
        SERIAL_ERROR_MSG("?J out of range (0.01 to 0.3)");
    }
  #endif
  #if HAS_CLASSIC_JERK
    #ifdef MAX_JERK_MANUAL
      static constexpr float max_jerk[] = MAX_JERK_MANUAL;
    #endif
    if (parser.seen('X')) {
      #if DISABLED(MAX_JERK_CAP)
        planner.max_jerk[X_AXIS] = parser.value_linear_units();
      #else
        #ifdef MAX_JERK_MANUAL
          static float jrk_x_limit = max_jerk[X_AXIS];
        #else
          static float jrk_x_limit = DEFAULT_XJERK * 2;
        #endif
        if(parser.value_linear_units() > jrk_x_limit)
          SERIAL_ECHOLNPAIR("X Jerk clamped to ",  jrk_x_limit);
        planner.max_jerk[X_AXIS] = constrain(parser.value_linear_units(), 1, jrk_x_limit);
      #endif
    }
    if (parser.seen('Y')) {
      #if DISABLED(MAX_JERK_CAP)
        planner.max_jerk[Y_AXIS] = parser.value_linear_units();
      #else
        #ifdef MAX_JERK_MANUAL
          static float jrk_y_limit = max_jerk[Y_AXIS];
        #else
          static float jrk_y_limit = DEFAULT_YJERK * 2;
        #endif
        if(parser.value_linear_units() > jrk_y_limit)
          SERIAL_ECHOLNPAIR("Y Jerk clamped to ",  jrk_y_limit);
        planner.max_jerk[Y_AXIS] = constrain(parser.value_linear_units(), 1, jrk_y_limit);
      #endif
    }
    if (parser.seen('Z')) {
      #if DISABLED(MAX_JERK_CAP)
        planner.max_jerk[Z_AXIS] = parser.value_linear_units();
      #else
        #ifdef MAX_JERK_MANUAL
          static float jrk_z_limit = max_jerk[Z_AXIS];
        #else
          static float jrk_z_limit = DEFAULT_ZJERK * 2;
        #endif
        if(parser.value_linear_units() > jrk_z_limit)
          SERIAL_ECHOLNPAIR("Z Jerk clamped to ",  jrk_z_limit);
        planner.max_jerk[Z_AXIS] = constrain(parser.value_linear_units(), 1, jrk_z_limit);
      #endif
      #if HAS_MESH
        if (planner.max_jerk[Z_AXIS] <= 0.1f)
          SERIAL_ECHOLNPGM("WARNING! Low Z Jerk may lead to unwanted pauses.");
      #endif
    }
    #if DISABLED(JUNCTION_DEVIATION) || DISABLED(LIN_ADVANCE)
      if (parser.seen('E')) {
        #if DISABLED(MAX_JERK_CAP)
        planner.max_jerk[E_AXIS] = parser.value_linear_units();
      #else
        #ifdef MAX_JERK_MANUAL
          static float jrk_e_limit = max_jerk[E_AXIS];
        #else
          static float jrk_e_limit = DEFAULT_EJERK * 2;
        #endif
        if(parser.value_linear_units() > jrk_e_limit)
          SERIAL_ECHOLNPAIR("E Jerk clamped to ",  jrk_e_limit);
        planner.max_jerk[E_AXIS] = constrain(parser.value_linear_units(), 1, jrk_e_limit);
      #endif
      }
    #endif
  #endif
}
