/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "../../inc/MarlinConfig.h"

#if ENABLED(AXIS_ENDSTOP_REPEATABILITY_TEST)

#include "../gcode.h"
#include "../../MarlinCore.h"
#include "../../module/endstops.h"
#include "../../module/motion.h"
#include "../../module/planner.h"

#if HAS_LEVELING
  #include "../../feature/bedlevel/bedlevel.h"
#endif

#if ENABLED(BACKLASH_COMPENSATION)
  #include "../../feature/backlash.h"
#endif

#ifndef AXIS_ENDSTOP_REPEATABILITY_MAX_OVERRUN
  #define AXIS_ENDSTOP_REPEATABILITY_MAX_OVERRUN 0.5f
#endif

static_assert(AXIS_ENDSTOP_REPEATABILITY_MAX_OVERRUN > 0, "AXIS_ENDSTOP_REPEATABILITY_MAX_OVERRUN must be positive.");

static EndstopEnum m49_physical_endstop(const AxisEnum axis) {
  switch (axis) {
    #if (X_HOME_TO_MIN && USE_X_MIN) || (X_HOME_TO_MAX && USE_X_MAX)
      case X_AXIS: return TERN(X_HOME_TO_MIN, X_MIN, X_MAX);
    #endif
    #if (Y_HOME_TO_MIN && USE_Y_MIN) || (Y_HOME_TO_MAX && USE_Y_MAX)
      case Y_AXIS: return TERN(Y_HOME_TO_MIN, Y_MIN, Y_MAX);
    #endif
    #if (Z_HOME_TO_MIN && USE_Z_MIN) || (Z_HOME_TO_MAX && USE_Z_MAX)
      case Z_AXIS: return TERN(Z_HOME_TO_MIN, Z_MIN, Z_MAX);
    #endif
    default: return NUM_ENDSTOP_STATES;
  }
}

static void m49_print_endstop(const AxisEnum axis) {
  switch (axis) {
    case X_AXIS: SERIAL_ECHOLNPGM(TERN(X_HOME_TO_MIN, "X_MIN", "X_MAX")); break;
    case Y_AXIS: SERIAL_ECHOLNPGM(TERN(Y_HOME_TO_MIN, "Y_MIN", "Y_MAX")); break;
    case Z_AXIS: SERIAL_ECHOLNPGM(TERN(Z_HOME_TO_MIN, "Z_MIN", "Z_MAX")); break;
    default: break;
  }
}

/**
 * Perform one synchronized single-axis move. No software limit clamping is
 * applied, so all targets must be validated by M49 before calling this helper.
 */
static bool m49_move_axis(
  const AxisEnum axis, const_float_t target, const_feedRate_t fr_mm_s,
  const EndstopEnum expected, const bool must_trigger,
  float &trigger_position, FSTR_P &failure
) {
  endstops.hit_on_purpose();
  current_position[axis] = target;
  line_to_current_position(fr_mm_s);
  planner.synchronize();

  if (!IsRunning()) {
    failure = F("firmware stopped during movement");
    return false;
  }

  const Endstops::endstop_mask_t hits = endstops.trigger_state(), expected_mask = _BV(expected);

  if (must_trigger) {
    if (!(hits & expected_mask)) {
      failure = F("expected physical endstop did not trigger");
      set_current_from_steppers_for_axis(axis);
      sync_plan_position();
      return false;
    }
    if (hits != expected_mask) {
      failure = F("unexpected additional endstop trigger");
      set_current_from_steppers_for_axis(axis);
      sync_plan_position();
      return false;
    }
    trigger_position = planner.triggered_position_mm(axis);
    set_current_from_steppers_for_axis(axis);
    sync_plan_position();
  }
  else if (hits) {
    failure = F("unexpected endstop trigger during exercise move");
    set_current_from_steppers_for_axis(axis);
    sync_plan_position();
    return false;
  }

  return true;
}

/**
 * M49: Physical axis-endstop repeatability diagnostic.
 *
 *   M49 A<X|Y|Z> D<distance> R<cycles> F<positive mm/min>
 *       B<negative mm/min> M<margin> S<approach mm/min>
 *       P<samples> V<verbosity>
 *
 * The compact forms AX, AY, and AZ are accepted. The selected axis must be
 * homed and trusted. Its physical homing endstop is used; a bed probe is never
 * substituted for Z_MIN.
 */
void GcodeSuite::M49() {

  #if IS_KINEMATIC
    SERIAL_ECHOLNPGM("Error: M49 currently supports Cartesian and Core machines only");
    return;
  #endif

  if (printJobOngoing() || printingIsPaused()) {
    SERIAL_ECHOLNPGM("Error: M49 is not allowed during a print job");
    return;
  }

  const bool select_x = parser.seen_test('X'),
             select_y = parser.seen_test('Y'),
             select_z = parser.seen_test('Z');

  if (!parser.seen_test('A') || parser.seenval('A')
      || uint8_t(select_x) + uint8_t(select_y) + uint8_t(select_z) != 1) {
    SERIAL_ECHOLNPGM("Error: M49 requires exactly one axis: AX, AY, or AZ");
    return;
  }

  const AxisEnum axis = select_x ? X_AXIS : select_y ? Y_AXIS : Z_AXIS;

  if (false
      || TERN0(X_DUAL_ENDSTOPS, axis == X_AXIS)
      || TERN0(Y_DUAL_ENDSTOPS, axis == Y_AXIS)
      || TERN0(Z_MULTI_ENDSTOPS, axis == Z_AXIS)) {
    SERIAL_ECHOLNPGM("Error: M49 does not yet support multiple endstops on one axis");
    return;
  }

  if (false
      || TERN0(X_SENSORLESS, axis == X_AXIS)
      || TERN0(Y_SENSORLESS, axis == Y_AXIS)
      || TERN0(Z_SENSORLESS, axis == Z_AXIS)) {
    SERIAL_ECHOLNPGM("Error: M49 requires a physical switch, not sensorless homing");
    return;
  }

  const EndstopEnum physical_endstop = m49_physical_endstop(axis);
  if (physical_endstop == NUM_ENDSTOP_STATES) {
    SERIAL_ECHOPGM("Error: ", C(AXIS_CHAR(axis)));
    SERIAL_ECHOLNPGM(" physical homing endstop unavailable");
    return;
  }

  if (!axis_was_homed(axis) || !axis_is_trusted(axis)) {
    SERIAL_ECHOPGM("Error: Home axis ", C(AXIS_CHAR(axis)));
    SERIAL_ECHOLNPGM(" before M49");
    return;
  }

  constexpr char valued_parameters[] = "DBFMSPRV";
  for (uint8_t i = 0; i < COUNT(valued_parameters) - 1; ++i) {
    const char c = valued_parameters[i];
    if (parser.seen(c) && !parser.has_value()) {
      SERIAL_ECHOPGM("Error: M49 parameter ", C(c));
      SERIAL_ECHOLNPGM(" requires a value");
      return;
    }
  }

  const float distance = parser.linearval('D', 4.0f),
              margin = parser.linearval('M', 2.0f);
  const feedRate_t forward_fr = parser.feedrateval('F', MMM_TO_MMS(240.0f)),
                   backward_fr = parser.feedrateval('B', MMM_TO_MMS(120.0f)),
                   approach_fr = parser.feedrateval('S', MMM_TO_MMS(30.0f));
  const int16_t cycles_arg = parser.intval('R', 10),
                samples_arg = parser.intval('P', 10),
                verbose_arg = parser.intval('V', 1);

  if (!(distance > 0) || !(margin > 0)) {
    SERIAL_ECHOLNPGM("Error: M49 D and M must be positive");
    return;
  }
  if (!WITHIN(cycles_arg, 0, 1000)) {
    SERIAL_ECHOLNPGM("Error: M49 R must be in the range 0-1000");
    return;
  }
  if (!WITHIN(samples_arg, 1, 50)) {
    SERIAL_ECHOLNPGM("Error: M49 P must be in the range 1-50");
    return;
  }
  if (!WITHIN(verbose_arg, 0, 3)) {
    SERIAL_ECHOLNPGM("Error: M49 V must be in the range 0-3");
    return;
  }
  if (!(forward_fr > 0) || !(backward_fr > 0) || !(approach_fr > 0)
      || forward_fr > planner.settings.max_feedrate_mm_s[axis]
      || backward_fr > planner.settings.max_feedrate_mm_s[axis]
      || approach_fr > planner.settings.max_feedrate_mm_s[axis]) {
    SERIAL_ECHOLNPGM("Error: M49 speeds must be positive and no greater than the axis maximum");
    return;
  }

  const int8_t hdir = home_dir(axis);
  const float reference = base_home_pos(axis),
              margin_position = reference - hdir * margin,
              far_position = margin_position - hdir * distance,
              approach_target = reference + hdir * float(AXIS_ENDSTOP_REPEATABILITY_MAX_OVERRUN);

  if (!WITHIN(margin_position, base_min_pos(axis), base_max_pos(axis))
      || !WITHIN(far_position, base_min_pos(axis), base_max_pos(axis))) {
    SERIAL_ECHOLNPGM("Error: M49 margin and exercise distance exceed axis travel limits");
    return;
  }

  const uint16_t cycles = cycles_arg;
  const uint8_t samples = samples_arg, verbose = verbose_arg;
  const feedRate_t away_fr = hdir < 0 ? forward_fr : backward_fr,
                   toward_fr = hdir < 0 ? backward_fr : forward_fr;

  if (verbose) {
    SERIAL_ECHOLNPGM("M49 Axis-Endstop Repeatability Test");
    SERIAL_ECHOPGM("Axis: ", C(AXIS_CHAR(axis)), "\nPhysical endstop: ");
    m49_print_endstop(axis);
    SERIAL_ECHOLNPAIR_F("Distance: ", distance, 3);
    SERIAL_ECHOLNPGM("Exercise cycles/sample: ", cycles);
    SERIAL_ECHOLNPAIR_F("Forward speed (mm/min): ", MMS_TO_MMM(forward_fr), 1);
    SERIAL_ECHOLNPAIR_F("Backward speed (mm/min): ", MMS_TO_MMM(backward_fr), 1);
    SERIAL_ECHOLNPAIR_F("Endstop margin: ", margin, 3);
    SERIAL_ECHOLNPAIR_F("Measurement speed (mm/min): ", MMS_TO_MMM(approach_fr), 1);
    SERIAL_ECHOLNPGM("Samples: ", samples);
    SERIAL_ECHOLNPGM("Backlash compensation: OFF");
  }

  planner.synchronize();

  #if HAS_LEVELING
    const bool leveling_was_active = planner.leveling_active;
    set_bed_leveling_enabled(false);
  #endif
  #if ENABLED(BACKLASH_COMPENSATION)
    const uint8_t saved_backlash_correction = backlash.get_correction_uint8();
    backlash.set_correction_uint8(0);
  #endif
  #if HAS_BED_PROBE
    const bool probe_was_enabled = endstops.z_probe_enabled;
    endstops.enable_z_probe(false);
  #endif

  remember_feedrate_scaling_off();

  bool test_ok = true;
  FSTR_P failure = nullptr;
  uint8_t completed_samples = 0;
  float trigger_position = 0,
        mean = 0,
        sum_squared_delta = 0,
        minimum = 999999.0f,
        maximum = -999999.0f;

  {
    TemporaryGlobalEndstopsState enable_endstops(true);

    // Establish one physical reference without invoking normal multi-stage homing.
    test_ok = m49_move_axis(axis, margin_position, away_fr, physical_endstop, false, trigger_position, failure);
    if (test_ok) {
      endstops.resync();
      if (TEST(endstops.state(), physical_endstop)) {
        failure = F("physical endstop did not release at the requested margin");
        test_ok = false;
      }
    }
    if (test_ok)
      test_ok = m49_move_axis(axis, approach_target, approach_fr, physical_endstop, true, trigger_position, failure);

    if (test_ok) {
      // The physical switch, not a Z probe, defines the diagnostic origin.
      set_axis_is_at_home(axis);
      current_position[axis] = reference;
      sync_plan_position();
      endstops.hit_on_purpose();
      if (verbose > 1) SERIAL_ECHOLNPGM("Initial physical reference acquired");
    }

    for (uint8_t sample = 0; test_ok && sample < samples; ++sample) {
      if (verbose > 1) SERIAL_ECHOLNPGM("Starting sample ", sample + 1, " of ", samples);

      test_ok = m49_move_axis(axis, margin_position, away_fr, physical_endstop, false, trigger_position, failure);
      if (test_ok) {
        endstops.resync();
        if (TEST(endstops.state(), physical_endstop)) {
          failure = F("physical endstop did not release at the requested margin");
          test_ok = false;
        }
      }

      for (uint16_t cycle = 0; test_ok && cycle < cycles; ++cycle) {
        test_ok = m49_move_axis(axis, far_position, away_fr, physical_endstop, false, trigger_position, failure)
               && m49_move_axis(axis, margin_position, toward_fr, physical_endstop, false, trigger_position, failure);
        if (test_ok && verbose > 2)
          SERIAL_ECHOLNPGM("  cycle ", cycle + 1, " of ", cycles);
      }

      if (test_ok)
        test_ok = m49_move_axis(axis, approach_target, approach_fr, physical_endstop, true, trigger_position, failure);

      if (test_ok) {
        const float measured = trigger_position - reference;
        ++completed_samples;
        NOMORE(minimum, measured);
        NOLESS(maximum, measured);
        const float delta = measured - mean;
        mean += delta / completed_samples;
        sum_squared_delta += delta * (measured - mean);

        if (verbose) {
          SERIAL_ECHO(completed_samples);
          SERIAL_ECHOPGM(" of ", samples);
          SERIAL_ECHOPAIR_F(": trigger position: ", measured, 6);
          if (verbose > 2) SERIAL_ECHOPAIR_F(" absolute: ", trigger_position, 6);
          SERIAL_EOL();
        }
        endstops.hit_on_purpose();
      }
    }

    // Leave the tested axis off the switch when possible.
    if (test_ok)
      test_ok = m49_move_axis(axis, margin_position, away_fr, physical_endstop, false, trigger_position, failure);
  }

  restore_feedrate_and_scaling();
  TERN_(HAS_BED_PROBE, endstops.enable_z_probe(probe_was_enabled));
  TERN_(BACKLASH_COMPENSATION, backlash.set_correction_uint8(saved_backlash_correction));
  TERN_(HAS_LEVELING, set_bed_leveling_enabled(leveling_was_active));

  if (test_ok) {
    const float sigma = SQRT(sum_squared_delta / completed_samples);
    SERIAL_ECHOLNPGM("Finished!");
    SERIAL_ECHOPAIR_F("Mean: ", mean, 6);
    SERIAL_ECHOPAIR_F(" Min: ", minimum, 6);
    SERIAL_ECHOPAIR_F(" Max: ", maximum, 6);
    SERIAL_ECHOLNPAIR_F(" Range: ", maximum - minimum, 6);
    SERIAL_ECHOLNPAIR_F("Standard Deviation: ", sigma, 6);
  }
  else {
    SERIAL_ECHOLNPGM("M49 aborted");
    SERIAL_ECHOPGM("Reason: ");
    SERIAL_ECHOLNF(failure);
    SERIAL_ECHOLNPGM("Completed samples: ", completed_samples, " / ", samples);
    SERIAL_ECHOLNPAIR_F("Last known axis position: ", current_position[axis], 6);
  }

  endstops.hit_on_purpose();
  destination = current_position;
  report_current_position();
}

#endif // AXIS_ENDSTOP_REPEATABILITY_TEST
