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
#include "../../module/motion.h" // for e_axis_mask
#include "../../module/planner.h"
#include "../../module/stepper.h"

#if ENABLED(AUTO_BED_LEVELING_UBL)
  #include "../../feature/bedlevel/bedlevel.h"
#endif

#define DEBUG_OUT ENABLED(MARLIN_DEV_MODE)
#include "../../core/debug_out.h"
#include "../../libs/hex_print.h"

inline stepper_flags_t selected_axis_bits() {
  stepper_flags_t selected{0};
  #if HAS_EXTRUDERS
    if (parser.seen('E')) {
      if (E_TERN0(parser.has_value())) {
        const uint8_t e = parser.value_int();
        if (e < EXTRUDERS)
          selected.bits = _BV(INDEX_OF_AXIS(E_AXIS, e));
      }
      else
        selected.bits = e_axis_mask;
    }
  #endif
  selected.bits |= NUM_AXIS_GANG(
      (parser.seen_test('X')        << X_AXIS),
    | (parser.seen_test('Y')        << Y_AXIS),
    | (parser.seen_test('Z')        << Z_AXIS),
    | (parser.seen_test(AXIS4_NAME) << I_AXIS),
    | (parser.seen_test(AXIS5_NAME) << J_AXIS),
    | (parser.seen_test(AXIS6_NAME) << K_AXIS)
  );
  return selected;
}

// Enable specified axes and warn about other affected axes
void do_enable(const stepper_flags_t to_enable) {
  const ena_mask_t was_enabled = stepper.axis_enabled.bits,
                  shall_enable = to_enable.bits & ~was_enabled;

  DEBUG_ECHOLNPGM("Now Enabled: ", hex_word(stepper.axis_enabled.bits), "  Enabling: ", hex_word(to_enable.bits), " | ", shall_enable);

  if (!shall_enable) return;    // All specified axes already enabled?

  ena_mask_t also_enabled = 0;    // Track steppers enabled due to overlap

  // Enable all flagged axes
  LOOP_NUM_AXES(a) {
    if (TEST(shall_enable, a)) {
      stepper.enable_axis(AxisEnum(a));         // Mark and enable the requested axis
      DEBUG_ECHOLNPGM("Enabled ", AXIS_CHAR(a), " (", a, ") with overlap ", hex_word(enable_overlap[a]), " ... Enabled: ", hex_word(stepper.axis_enabled.bits));
      also_enabled |= enable_overlap[a];
    }
  }
  #if HAS_EXTRUDERS
    EXTRUDER_LOOP() {
      const uint8_t a = INDEX_OF_AXIS(E_AXIS, e);
      if (TEST(shall_enable, a)) {
        stepper.ENABLE_EXTRUDER(e);
        DEBUG_ECHOLNPGM("Enabled E", AS_DIGIT(e), " (", a, ") with overlap ", hex_word(enable_overlap[a]), " ... ", hex_word(stepper.axis_enabled.bits));
        also_enabled |= enable_overlap[a];
      }
    }
  #endif

  if ((also_enabled &= ~(shall_enable | was_enabled))) {
    SERIAL_CHAR('(');
    LOOP_NUM_AXES(a) if (TEST(also_enabled, a)) SERIAL_CHAR(AXIS_CHAR(a), ' ');
    #if HAS_EXTRUDERS
      #define _EN_ALSO(N) if (TEST(also_enabled, INDEX_OF_AXIS(E_AXIS, N))) SERIAL_CHAR('E', '0' + N, ' ');
      REPEAT(EXTRUDERS, _EN_ALSO)
    #endif
    SERIAL_ECHOLNPGM("also enabled)");
  }

  DEBUG_ECHOLNPGM("Enabled Now: ", hex_word(stepper.axis_enabled.bits));
}

/**
 * M17: Enable stepper motor power for one or more axes.
 *      Print warnings for axes that share an ENABLE_PIN.
 *
 * Examples:
 *
 *  M17 XZ ; Enable X and Z axes
 *  M17 E  ; Enable all E steppers
 *  M17 E1 ; Enable just the E1 stepper
 */
void GcodeSuite::M17() {
  if (parser.seen_axis()) {
    if (any_enable_overlap())
      do_enable(selected_axis_bits());
    else {
      #if HAS_EXTRUDERS
        if (parser.seen('E')) {
          if (parser.has_value()) {
            const uint8_t e = parser.value_int();
            if (e < EXTRUDERS) stepper.ENABLE_EXTRUDER(e);
          }
          else
            stepper.enable_e_steppers();
        }
      #endif
      LOOP_NUM_AXES(a)
        if (parser.seen_test(AXIS_CHAR(a))) stepper.enable_axis((AxisEnum)a);
    }
  }
  else {
    LCD_MESSAGE(MSG_NO_MOVE);
    stepper.enable_all_steppers();
  }
}

void try_to_disable(const stepper_flags_t to_disable) {
  ena_mask_t still_enabled = to_disable.bits & stepper.axis_enabled.bits;

  DEBUG_ECHOLNPGM("Enabled: ", hex_word(stepper.axis_enabled.bits), " To Disable: ", hex_word(to_disable.bits), " | ", hex_word(still_enabled));

  if (!still_enabled) return;

  // Attempt to disable all flagged axes
  LOOP_NUM_AXES(a)
    if (TEST(to_disable.bits, a)) {
      DEBUG_ECHOPGM("Try to disable ", AXIS_CHAR(a), " (", a, ") with overlap ", hex_word(enable_overlap[a]), " ... ");
      if (stepper.disable_axis(AxisEnum(a))) {            // Mark the requested axis and request to disable
        DEBUG_ECHOPGM("OK");
        still_enabled &= ~(_BV(a) | enable_overlap[a]);   // If actually disabled, clear one or more tracked bits
      }
      else
        DEBUG_ECHOPGM("OVERLAP");
      DEBUG_ECHOLNPGM(" ... still_enabled=", hex_word(still_enabled));
    }
  #if HAS_EXTRUDERS
    EXTRUDER_LOOP() {
      const uint8_t a = INDEX_OF_AXIS(E_AXIS, e);
      if (TEST(to_disable.bits, a)) {
        DEBUG_ECHOPGM("Try to disable E", AS_DIGIT(e), " (", a, ") with overlap ", hex_word(enable_overlap[a]), " ... ");
        if (stepper.DISABLE_EXTRUDER(e)) {
          DEBUG_ECHOPGM("OK");
          still_enabled &= ~(_BV(a) | enable_overlap[a]);
        }
        else
          DEBUG_ECHOPGM("OVERLAP");
        DEBUG_ECHOLNPGM(" ... still_enabled=", hex_word(still_enabled));
      }
    }
  #endif

  auto overlap_warning = [](const ena_mask_t axis_bits) {
    SERIAL_ECHOPGM(" not disabled. Shared with");
    LOOP_NUM_AXES(a) if (TEST(axis_bits, a)) SERIAL_ECHOPGM_P((PGM_P)pgm_read_ptr(&SP_AXIS_STR[a]));
    #if HAS_EXTRUDERS
      #define _EN_STILLON(N) if (TEST(axis_bits, INDEX_OF_AXIS(E_AXIS, N))) SERIAL_CHAR(' ', 'E', '0' + N);
      REPEAT(EXTRUDERS, _EN_STILLON)
    #endif
    SERIAL_ECHOLNPGM(".");
  };

  // If any of the requested axes are still enabled, give a warning
  LOOP_NUM_AXES(a) {
    if (TEST(still_enabled, a)) {
      SERIAL_CHAR(AXIS_CHAR(a));
      overlap_warning(stepper.axis_enabled.bits & enable_overlap[a]);
    }
  }
  #if HAS_EXTRUDERS
    EXTRUDER_LOOP() {
      const uint8_t a = INDEX_OF_AXIS(E_AXIS, e);
      if (TEST(still_enabled, a)) {
        SERIAL_CHAR('E', '0' + e);
        overlap_warning(stepper.axis_enabled.bits & enable_overlap[a]);
      }
    }
  #endif

  DEBUG_ECHOLNPGM("Enabled Now: ", hex_word(stepper.axis_enabled.bits));
}

/**
 * M18, M84: Disable stepper motor power for one or more axes.
 *           Print warnings for axes that share an ENABLE_PIN.
 */
void GcodeSuite::M18_M84() {
  if (parser.seenval('S')) {
    reset_stepper_timeout();
    #if HAS_DISABLE_INACTIVE_AXIS
      const millis_t ms = parser.value_millis_from_seconds();
      #if LASER_SAFETY_TIMEOUT_MS > 0
        if (ms && ms <= LASER_SAFETY_TIMEOUT_MS) {
          SERIAL_ECHO_MSG("M18 timeout must be > ", MS_TO_SEC(LASER_SAFETY_TIMEOUT_MS + 999), " s for laser safety.");
          return;
        }
      #endif
      stepper_inactive_time = ms;
    #endif
  }
  else {
    if (parser.seen_axis()) {
      planner.synchronize();
      if (any_enable_overlap())
        try_to_disable(selected_axis_bits());
      else {
        #if HAS_EXTRUDERS
          if (parser.seen('E')) {
            if (E_TERN0(parser.has_value()))
              stepper.DISABLE_EXTRUDER(parser.value_int());
            else
              stepper.disable_e_steppers();
          }
        #endif
        LOOP_NUM_AXES(a)
          if (parser.seen_test(AXIS_CHAR(a))) stepper.disable_axis((AxisEnum)a);
      }
    }
    else
      planner.finish_and_disable();

    TERN_(AUTO_BED_LEVELING_UBL, bedlevel.steppers_were_disabled());
  }
}
