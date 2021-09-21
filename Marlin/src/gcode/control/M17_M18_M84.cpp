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
#include "../../MarlinCore.h" // for stepper_inactive_time, disable_e_steppers
#include "../../lcd/marlinui.h"
#include "../../module/stepper.h"

#if ENABLED(AUTO_BED_LEVELING_UBL)
  #include "../../feature/bedlevel/bedlevel.h"
#endif

#define DEBUG_OUT ENABLED(MARLIN_DEV_MODE)
#include "../../core/debug_out.h"
#include "../../libs/hex_print.h"

inline axis_flags_t selected_axis_bits() {
  axis_flags_t selected{0};
  #if HAS_EXTRUDERS
    if (parser.seen('E')) {
      if (parser.has_value()) {
        const uint8_t e = parser.value_int();
        if (e < E_STEPPERS)
          selected.bits = _BV(index_of_axis(E_AXIS, e));
      }
      else
        selected.bits = selected.e_bits();
    }
  #endif
  selected.bits |= LINEAR_AXIS_GANG(
      (parser.seen_test('X')        << X_AXIS),
    | (parser.seen_test('Y')        << Y_AXIS),
    | (parser.seen_test('Z')        << Z_AXIS),
    | (parser.seen_test(AXIS4_NAME) << I_AXIS),
    | (parser.seen_test(AXIS5_NAME) << J_AXIS),
    | (parser.seen_test(AXIS6_NAME) << K_AXIS)
  );
  return selected;
}

void do_enable(const axis_flags_t to_enable) {
  const uint16_t was_enabled = stepper.axis_enabled.bits,
                 shall_enable = to_enable.bits & ~was_enabled;

  DEBUG_ECHOLNPGM("Enabled: ", hex_word(stepper.axis_enabled.bits), " To Enable: ", hex_word(to_enable.bits), " | ", shall_enable);

  // Axes are already enabled?
  if (!shall_enable) return;

  auto has_enable_overlap = [](const uint8_t axis_index) {
    return (any_enable_overlap() && enable_overlap[axis_index] != _BV(axis_index));
  };

  auto overlap_warning = [](const uint8_t axis_index, const uint16_t was_enabled) {
    if (has_enable_overlap(axis_index)) {
      SERIAL_CHAR('(');
      const uint16_t other_bits = enable_overlap[axis_index] & ~(_BV(axis_index) | was_enabled);
      LOOP_LINEAR_AXES(a) if (TEST(other_bits, a)) SERIAL_CHAR(axis_codes[a]);
      #if HAS_EXTRUDERS
        #define _EN_STILLON(N) if (TEST(other_bits, index_of_axis(E_AXIS, N))) SERIAL_CHAR('E', '0' + N);
        REPEAT(E_STEPPERS, _EN_STILLON)
      #endif
      SERIAL_ECHOLNPGM(" also enabled)");
    }
  };

  // Enable all flagged axes
  LOOP_LINEAR_AXES(a) {
    if (TEST(shall_enable, a)) {
      stepper.enable_axis(AxisEnum(a));         // Mark and enable the requested axis
      DEBUG_ECHOLNPGM("Enabled ", axis_codes[a], " with overlap ", hex_word(enable_overlap[a]), " ... Enabled: ", hex_word(stepper.axis_enabled.bits));
      overlap_warning(a, was_enabled);
    }
  }
  #if HAS_EXTRUDERS
    LOOP_L_N(e, E_STEPPERS) {
      const uint8_t a = index_of_axis(E_AXIS, e);
      if (TEST(shall_enable, a)) {
        stepper.enable_e_stepper(e);
        DEBUG_ECHOLNPGM("Enabled E", AS_DIGIT(e), " (", a, ") with overlap ", hex_word(enable_overlap[a]), " ... ", hex_word(stepper.axis_enabled.bits));
        overlap_warning(a, was_enabled);
      }
    }
  #endif

  DEBUG_ECHOLNPGM("Enabled Now: ", hex_word(stepper.axis_enabled.bits));
}

/**
 * M17: Enable stepper motors
 */
void GcodeSuite::M17() {
  if (parser.seen_axis()) {
    if (any_enable_overlap())
      do_enable(selected_axis_bits());
    else {
      if (TERN0(HAS_EXTRUDERS, parser.seen('E'))) {
        if (parser.has_value()) {
          const uint8_t e = parser.value_int();
          if (e < E_STEPPERS) stepper.enable_e_stepper(e);
        }
        else
          stepper.enable_e_steppers();
      }
      LINEAR_AXIS_CODE(
        if (parser.seen_test('X'))        stepper.enable_axis(X_AXIS),
        if (parser.seen_test('Y'))        stepper.enable_axis(Y_AXIS),
        if (parser.seen_test('Z'))        stepper.enable_axis(Z_AXIS),
        if (parser.seen_test(AXIS4_NAME)) stepper.enable_axis(I_AXIS),
        if (parser.seen_test(AXIS5_NAME)) stepper.enable_axis(J_AXIS),
        if (parser.seen_test(AXIS6_NAME)) stepper.enable_axis(K_AXIS)
      );
    }
  }
  else {
    LCD_MESSAGEPGM(MSG_NO_MOVE);
    stepper.enable_all_steppers();
  }
}

void try_to_disable(const axis_flags_t to_disable) {
  uint16_t still_enabled = to_disable.bits & stepper.axis_enabled.bits;

  DEBUG_ECHOLNPGM("Enabled: ", hex_word(stepper.axis_enabled.bits), " To Disable: ", hex_word(to_disable.bits), " | ", hex_word(still_enabled));

  if (!still_enabled) return;

  // Attempt to disable all flagged axes
  LOOP_LINEAR_AXES(a)
    if (TEST(to_disable.bits, a)) {
      DEBUG_ECHOPGM("Try to disable ", axis_codes[a], " with overlap ", hex_word(enable_overlap[a]), " ... ");
      if (stepper.disable_axis(AxisEnum(a))) {  // Mark the requested axis and request to disable
        still_enabled &= ~enable_overlap[a];    // If actually disabled, clear one or more tracked bits
        DEBUG_ECHOPGM("OK");
      }
      else
        DEBUG_ECHOPGM("OVERLAP");
      DEBUG_ECHOLNPGM(" ... still_enabled=", hex_word(still_enabled));
    }
  #if HAS_EXTRUDERS
    LOOP_L_N(e, E_STEPPERS) {
      const uint8_t a = index_of_axis(E_AXIS, e);
      if (TEST(to_disable.bits, a)) {
        DEBUG_ECHOPGM("Try to disable E", AS_DIGIT(e), " (", a, ") with overlap ", hex_word(enable_overlap[a]), " ... ");
        if (stepper.disable_e_stepper(e)) {
          still_enabled &= ~enable_overlap[a];
          DEBUG_ECHOPGM("OK");
        }
        else
          DEBUG_ECHOPGM("OVERLAP");
        DEBUG_ECHOLNPGM(" ... still_enabled=", hex_word(still_enabled));
      }
    }
  #endif

  auto overlap_warning = [](const uint16_t axis_bits) {
    SERIAL_ECHOPGM(" not disabled. Shared with ");
    LOOP_LINEAR_AXES(a) if (TEST(axis_bits, a)) SERIAL_CHAR(axis_codes[a]);
    #if HAS_EXTRUDERS
      #define _EN_STILLON(N) if (TEST(axis_bits, index_of_axis(E_AXIS, N))) SERIAL_CHAR('E', '0' + N);
      REPEAT(E_STEPPERS, _EN_STILLON)
    #endif
    SERIAL_ECHOLNPGM(".");
  };

  // If any of the requested axes are still enabled, give a warning
  LOOP_LINEAR_AXES(a) {
    if (TEST(still_enabled, a)) {
      SERIAL_CHAR(axis_codes[a]);
      overlap_warning(stepper.axis_enabled.bits & enable_overlap[a]);
    }
  }
  #if HAS_EXTRUDERS
    LOOP_L_N(e, E_STEPPERS) {
      const uint8_t a = index_of_axis(E_AXIS, e);
      if (TEST(still_enabled, a)) {
        SERIAL_CHAR('E', '0' + e);
        overlap_warning(stepper.axis_enabled.bits & enable_overlap[a]);
      }
    }
  #endif

  DEBUG_ECHOLNPGM("Enabled Now: ", hex_word(stepper.axis_enabled.bits));
}

//static_assert(!any_enable_overlap(), "There is some overlap.");

/**
 * M18, M84: Disable stepper motors
 */
void GcodeSuite::M18_M84() {
  if (parser.seenval('S')) {
    reset_stepper_timeout();
    stepper_inactive_time = parser.value_millis_from_seconds();
  }
  else {
    if (parser.seen_axis()) {
      planner.synchronize();
      if (any_enable_overlap())
        try_to_disable(selected_axis_bits());
      else {
        #if HAS_EXTRUDERS
          if (parser.seen('E')) {
            if (parser.has_value())
              stepper.disable_e_stepper(parser.value_int());
            else
              stepper.disable_e_steppers();
          }
        #endif
        LINEAR_AXIS_CODE(
          if (parser.seen_test('X'))        stepper.disable_axis(X_AXIS),
          if (parser.seen_test('Y'))        stepper.disable_axis(Y_AXIS),
          if (parser.seen_test('Z'))        stepper.disable_axis(Z_AXIS),
          if (parser.seen_test(AXIS4_NAME)) stepper.disable_axis(I_AXIS),
          if (parser.seen_test(AXIS5_NAME)) stepper.disable_axis(J_AXIS),
          if (parser.seen_test(AXIS6_NAME)) stepper.disable_axis(K_AXIS)
        );
      }
    }
    else
      planner.finish_and_disable();

    TERN_(AUTO_BED_LEVELING_UBL, ubl.steppers_were_disabled());
  }
}
