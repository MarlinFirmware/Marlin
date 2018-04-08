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

    if (get_target_extruder_from_command()) return;

    if (parser.seen('D')) {
      // setting any extruder filament size disables volumetric on the assumption that
      // slicers either generate in extruder values as cubic mm or as as filament feeds
      // for all extruders
      if ( (parser.volumetric_enabled = (parser.value_linear_units() != 0.0)) )
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

  GET_TARGET_EXTRUDER();

  LOOP_XYZE(i) {
    if (parser.seen(axis_codes[i])) {
      const uint8_t a = i + (i == E_AXIS ? TARGET_EXTRUDER : 0);
      planner.max_acceleration_mm_per_s2[a] = parser.value_axis_units((AxisEnum)a);
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

  GET_TARGET_EXTRUDER();

  LOOP_XYZE(i)
    if (parser.seen(axis_codes[i])) {
      const uint8_t a = i + (i == E_AXIS ? TARGET_EXTRUDER : 0);
      planner.max_feedrate_mm_s[a] = parser.value_axis_units((AxisEnum)a);
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
  bool report = true;
  if (parser.seenval('S')) { // Kept for legacy compatibility. Should NOT BE USED for new developments.
    planner.travel_acceleration = planner.acceleration = parser.value_linear_units();
    report = false;
  }
  if (parser.seenval('P')) {
    planner.acceleration = parser.value_linear_units();
    report = false;
  }
  if (parser.seenval('R')) {
    planner.retract_acceleration = parser.value_linear_units();
    report = false;
  }
  if (parser.seenval('T')) {
    planner.travel_acceleration = parser.value_linear_units();
    report = false;
  }
  if (report) {
    SERIAL_ECHOPAIR("Acceleration: P", planner.acceleration);
    SERIAL_ECHOPAIR(" R", planner.retract_acceleration);
    SERIAL_ECHOLNPAIR(" T", planner.travel_acceleration);
  }
}

/**
 * M205: Set Advanced Settings
 *
 *    S = Min Feed Rate (units/s)
 *    T = Min Travel Feed Rate (units/s)
 *    B = Min Segment Time (µs)
 *    X = Max X Jerk (units/sec^2)
 *    Y = Max Y Jerk (units/sec^2)
 *    Z = Max Z Jerk (units/sec^2)
 *    E = Max E Jerk (units/sec^2)
 */
void GcodeSuite::M205() {
  if (parser.seen('S')) planner.min_feedrate_mm_s = parser.value_linear_units();
  if (parser.seen('T')) planner.min_travel_feedrate_mm_s = parser.value_linear_units();
  if (parser.seen('B')) planner.min_segment_time_us = parser.value_ulong();
  if (parser.seen('X')) planner.max_jerk[X_AXIS] = parser.value_linear_units();
  if (parser.seen('Y')) planner.max_jerk[Y_AXIS] = parser.value_linear_units();
  if (parser.seen('Z')) {
    planner.max_jerk[Z_AXIS] = parser.value_linear_units();
    #if HAS_MESH
      if (planner.max_jerk[Z_AXIS] <= 0.1)
        SERIAL_ECHOLNPGM("WARNING! Low Z Jerk may lead to unwanted pauses.");
    #endif
  }
  if (parser.seen('E')) planner.max_jerk[E_AXIS] = parser.value_linear_units();
}
