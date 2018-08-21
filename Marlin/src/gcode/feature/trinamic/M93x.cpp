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

#include "../../../inc/MarlinConfig.h"

#if HAS_TRINAMIC

#include "../../gcode.h"
#include "../../../feature/tmc_util.h"
#include "../../../module/stepper_indirection.h"

/**
 * M930: TMC set blank_time.
 */
#if ENABLED(TMC_CHOP_CALIBRATION)
void GcodeSuite::M930() {
    if (parser.seenval('E')) {
      #if AXIS_IS_TMC(E0)
        tmc_set_blank_time(stepperE0, parser.value_int());
      #endif
    }
    if (parser.seenval('X')) {
      #if AXIS_IS_TMC(X)
        tmc_set_blank_time(stepperX, parser.value_int());
      #endif
      #if AXIS_IS_TMC(X2)
        tmc_set_blank_time(stepperX2, parser.value_int());
      #endif
    }
    if (parser.seenval('Y')) {
      #if AXIS_IS_TMC(Y)
        tmc_set_blank_time(stepperY, parser.value_int());
      #endif
      #if AXIS_IS_TMC(Y2)
        tmc_set_blank_time(stepperY2, parser.value_int());
      #endif
    }
    if (parser.seenval('Z')) {
      #if AXIS_IS_TMC(Z)
        tmc_set_blank_time(stepperZ, parser.value_int());
      #endif
      #if AXIS_IS_TMC(Z2)
        tmc_set_blank_time(stepperZ2, parser.value_int());
      #endif
    }
    if (parser.seen('E')) {
      #if AXIS_IS_TMC(E0)
        tmc_get_blank_time(stepperE0, TMC_E0);
      #endif
    }
    if (parser.seen('X')) {
      #if AXIS_IS_TMC(X)
        tmc_get_blank_time(stepperX, TMC_X);
      #endif
      #if AXIS_IS_TMC(X2)
        tmc_get_blank_time(stepperX2, TMC_X);
      #endif
    }
    if (parser.seen('Y')) {
      #if AXIS_IS_TMC(Y)
        tmc_get_blank_time(stepperY, TMC_Y);
      #endif
      #if AXIS_IS_TMC(Y2)
        tmc_get_blank_time(stepperY2, TMC_Y2);
      #endif
    }
    if (parser.seen('Z')) {
      #if AXIS_IS_TMC(Z)
        tmc_get_blank_time(stepperZ, TMC_Z);
      #endif
      #if AXIS_IS_TMC(Z2)
        tmc_get_blank_time(stepperZ2, TMC_Z2);
      #endif
    }
    if (!parser.seen_any()) {
      #if AXIS_IS_TMC(E0)
        tmc_get_blank_time(stepperE0, TMC_E0);
      #endif
      #if AXIS_IS_TMC(X)
        tmc_get_blank_time(stepperX, TMC_X);
      #endif
      #if AXIS_IS_TMC(X2)
        tmc_get_blank_time(stepperX2, TMC_X2);
      #endif
      #if AXIS_IS_TMC(Y)
        tmc_get_blank_time(stepperY, TMC_Y);
      #endif
      #if AXIS_IS_TMC(Y2)
        tmc_get_blank_time(stepperY2, TMC_Y2);
      #endif
      #if AXIS_IS_TMC(Z)
        tmc_get_blank_time(stepperZ, TMC_Z);
      #endif
      #if AXIS_IS_TMC(Z2)
        tmc_get_blank_time(stepperZ2, TMC_Z2);
      #endif
    }
}
#endif // TMC_CHOP_CALIBRATION

/**
 * M931: TMC set off_time.
 */
#if ENABLED(TMC_CHOP_CALIBRATION)
void GcodeSuite::M931() {
    if (parser.seenval('E')) {
      #if AXIS_IS_TMC(E0)
        tmc_set_off_time(stepperE0, parser.value_int());
      #endif
    }
    if (parser.seenval('X')) {
      #if AXIS_IS_TMC(X)
        tmc_set_off_time(stepperX, parser.value_int());
      #endif
      #if AXIS_IS_TMC(X2)
        tmc_set_off_time(stepperX2, parser.value_int());
      #endif
    }
    if (parser.seenval('Y')) {
      #if AXIS_IS_TMC(Y)
        tmc_set_off_time(stepperY, parser.value_int());
      #endif
      #if AXIS_IS_TMC(Y2)
        tmc_set_off_time(stepperY2, parser.value_int());
      #endif
    }
    if (parser.seenval('Z')) {
      #if AXIS_IS_TMC(Z)
        tmc_set_off_time(stepperZ, parser.value_int());
      #endif
      #if AXIS_IS_TMC(Z2)
        tmc_set_off_time(stepperZ2, parser.value_int());
      #endif
    }
    if (parser.seen('E')) {
      #if AXIS_IS_TMC(E0)
        tmc_get_off_time(stepperE0, TMC_E0);
      #endif
    }
    if (parser.seen('X')) {
      #if AXIS_IS_TMC(X)
        tmc_get_off_time(stepperX, TMC_X);
      #endif
      #if AXIS_IS_TMC(X2)
        tmc_get_off_time(stepperX2, TMC_X);
      #endif
    }
    if (parser.seen('Y')) {
      #if AXIS_IS_TMC(Y)
        tmc_get_off_time(stepperY, TMC_Y);
      #endif
      #if AXIS_IS_TMC(Y2)
        tmc_get_off_time(stepperY2, TMC_Y2);
      #endif
    }
    if (parser.seen('Z')) {
      #if AXIS_IS_TMC(Z)
        tmc_get_off_time(stepperZ, TMC_Z);
      #endif
      #if AXIS_IS_TMC(Z2)
        tmc_get_off_time(stepperZ2, TMC_Z2);
      #endif
    }
    if (!parser.seen_any()) {
      #if AXIS_IS_TMC(E0)
        tmc_get_off_time(stepperE0, TMC_E0);
      #endif
      #if AXIS_IS_TMC(X)
        tmc_get_off_time(stepperX, TMC_X);
      #endif
      #if AXIS_IS_TMC(X2)
        tmc_get_off_time(stepperX2, TMC_X2);
      #endif
      #if AXIS_IS_TMC(Y)
        tmc_get_off_time(stepperY, TMC_Y);
      #endif
      #if AXIS_IS_TMC(Y2)
        tmc_get_off_time(stepperY2, TMC_Y2);
      #endif
      #if AXIS_IS_TMC(Z)
        tmc_get_off_time(stepperZ, TMC_Z);
      #endif
      #if AXIS_IS_TMC(Z2)
        tmc_get_off_time(stepperZ2, TMC_Z2);
      #endif
    }
}
#endif // TMC_CHOP_CALIBRATION

/**
 * M932: TMC set hysteresis_start.
 */
#if ENABLED(TMC_CHOP_CALIBRATION)
void GcodeSuite::M932() {
    if (parser.seenval('E')) {
      #if AXIS_IS_TMC(E0)
        tmc_set_hysteresis_start(stepperE0, parser.value_int());
      #endif
    }
    if (parser.seenval('X')) {
      #if AXIS_IS_TMC(X)
        tmc_set_hysteresis_start(stepperX, parser.value_int());
      #endif
      #if AXIS_IS_TMC(X2)
        tmc_set_hysteresis_start(stepperX2, parser.value_int());
      #endif
    }
    if (parser.seenval('Y')) {
      #if AXIS_IS_TMC(Y)
        tmc_set_hysteresis_start(stepperY, parser.value_int());
      #endif
      #if AXIS_IS_TMC(Y2)
        tmc_set_hysteresis_start(stepperY2, parser.value_int());
      #endif
    }
    if (parser.seenval('Z')) {
      #if AXIS_IS_TMC(Z)
        tmc_set_hysteresis_start(stepperZ, parser.value_int());
      #endif
      #if AXIS_IS_TMC(Z2)
        tmc_set_hysteresis_start(stepperZ2, parser.value_int());
      #endif
    }
    if (parser.seen('E')) {
      #if AXIS_IS_TMC(E0)
        tmc_get_hysteresis_start(stepperE0, TMC_E0);
      #endif
    }
    if (parser.seen('X')) {
      #if AXIS_IS_TMC(X)
        tmc_get_hysteresis_start(stepperX, TMC_X);
      #endif
      #if AXIS_IS_TMC(X2)
        tmc_get_hysteresis_start(stepperX2, TMC_X);
      #endif
    }
    if (parser.seen('Y')) {
      #if AXIS_IS_TMC(Y)
        tmc_get_hysteresis_start(stepperY, TMC_Y);
      #endif
      #if AXIS_IS_TMC(Y2)
        tmc_get_hysteresis_start(stepperY2, TMC_Y2);
      #endif
    }
    if (parser.seen('Z')) {
      #if AXIS_IS_TMC(Z)
        tmc_get_hysteresis_start(stepperZ, TMC_Z);
      #endif
      #if AXIS_IS_TMC(Z2)
        tmc_get_hysteresis_start(stepperZ2, TMC_Z2);
      #endif
    }
    if (!parser.seen_any()) {
      #if AXIS_IS_TMC(E0)
        tmc_get_hysteresis_start(stepperE0, TMC_E0);
      #endif
      #if AXIS_IS_TMC(X)
        tmc_get_hysteresis_start(stepperX, TMC_X);
      #endif
      #if AXIS_IS_TMC(X2)
        tmc_get_hysteresis_start(stepperX2, TMC_X2);
      #endif
      #if AXIS_IS_TMC(Y)
        tmc_get_hysteresis_start(stepperY, TMC_Y);
      #endif
      #if AXIS_IS_TMC(Y2)
        tmc_get_hysteresis_start(stepperY2, TMC_Y2);
      #endif
      #if AXIS_IS_TMC(Z)
        tmc_get_hysteresis_start(stepperZ, TMC_Z);
      #endif
      #if AXIS_IS_TMC(Z2)
        tmc_get_hysteresis_start(stepperZ2, TMC_Z2);
      #endif
    }
}
#endif // TMC_CHOP_CALIBRATION

/**
 * M933: TMC set hysteresis_end/sine_offset (chm = 0/1).
 */
#if ENABLED(TMC_CHOP_CALIBRATION)
void GcodeSuite::M933() {
    if (parser.seenval('E')) {
      #if AXIS_IS_TMC(E0)
        tmc_set_hysteresis_end(stepperE0, parser.value_int());
      #endif
    }
    if (parser.seenval('X')) {
      #if AXIS_IS_TMC(X)
        tmc_set_hysteresis_end(stepperX, parser.value_int());
      #endif
      #if AXIS_IS_TMC(X2)
        tmc_set_hysteresis_end(stepperX2, parser.value_int());
      #endif
    }
    if (parser.seenval('Y')) {
      #if AXIS_IS_TMC(Y)
        tmc_set_hysteresis_end(stepperY, parser.value_int());
      #endif
      #if AXIS_IS_TMC(Y2)
        tmc_set_hysteresis_end(stepperY2, parser.value_int());
      #endif
    }
    if (parser.seenval('Z')) {
      #if AXIS_IS_TMC(Z)
        tmc_set_hysteresis_end(stepperZ, parser.value_int());
      #endif
      #if AXIS_IS_TMC(Z2)
        tmc_set_fast_decay_time(stepperZ2, parser.value_int());
      #endif
    }
    if (parser.seen('E')) {
      #if AXIS_IS_TMC(E0)
        tmc_get_hysteresis_end(stepperE0, TMC_E0);
      #endif
    }
    if (parser.seen('X')) {
      #if AXIS_IS_TMC(X)
        tmc_get_hysteresis_end(stepperX, TMC_X);
      #endif
      #if AXIS_IS_TMC(X2)
        tmc_get_hysteresis_end(stepperX2, TMC_X);
      #endif
    }
    if (parser.seen('Y')) {
      #if AXIS_IS_TMC(Y)
        tmc_get_hysteresis_end(stepperY, TMC_Y);
      #endif
      #if AXIS_IS_TMC(Y2)
        tmc_get_hysteresis_end(stepperY2, TMC_Y2);
      #endif
    }
    if (parser.seen('Z')) {
      #if AXIS_IS_TMC(Z)
        tmc_get_hysteresis_end(stepperZ, TMC_Z);
      #endif
      #if AXIS_IS_TMC(Z2)
        tmc_get_hysteresis_end(stepperZ2, TMC_Z2);
      #endif
    }
    if (!parser.seen_any()) {
      #if AXIS_IS_TMC(E0)
        tmc_get_hysteresis_end(stepperE0, TMC_E0);
      #endif
      #if AXIS_IS_TMC(X)
        tmc_get_hysteresis_end(stepperX, TMC_X);
      #endif
      #if AXIS_IS_TMC(X2)
        tmc_get_hysteresis_end(stepperX2, TMC_X2);
      #endif
      #if AXIS_IS_TMC(Y)
        tmc_get_hysteresis_end(stepperY, TMC_Y);
      #endif
      #if AXIS_IS_TMC(Y2)
        tmc_get_hysteresis_end(stepperY2, TMC_Y2);
      #endif
      #if AXIS_IS_TMC(Z)
        tmc_get_hysteresis_end(stepperZ, TMC_Z);
      #endif
      #if AXIS_IS_TMC(Z2)
        tmc_get_hysteresis_end(stepperZ2, TMC_Z2);
      #endif
    }
}
#endif // TMC_CHOP_CALIBRATION

/**
 * M934: TMC set fast_decay_time (chm = 1). 
 */
#if ENABLED(TMC_CHOP_CALIBRATION)
void GcodeSuite::M934() {
    if (parser.seenval('E')) {
      #if AXIS_IS_TMC(E0)
        tmc_set_fast_decay_time(stepperE0, (uint8_t)parser.value_int());
      #endif
    }
    if (parser.seenval('X')) {
      #if AXIS_IS_TMC(X)
        tmc_set_fast_decay_time(stepperX, (uint8_t)parser.value_int());
      #endif
      #if AXIS_IS_TMC(X2)
        tmc_set_fast_decay_time(stepperX2, (uint8_t)parser.value_int());
      #endif
    }
    if (parser.seenval('Y')) {
      #if AXIS_IS_TMC(Y)
        tmc_set_fast_decay_time(stepperY, (uint8_t)parser.value_int());
      #endif
      #if AXIS_IS_TMC(Y2)
        tmc_set_fast_decay_time(stepperY2, (uint8_t)parser.value_int());
      #endif
    }
    if (parser.seenval('Z')) {
      #if AXIS_IS_TMC(Z)
        tmc_set_fast_decay_time(stepperZ, (uint8_t)parser.value_int());
      #endif
      #if AXIS_IS_TMC(Z2)
        tmc_set_fast_decay_time(stepperZ2, (uint8_t)parser.value_int());
      #endif
    }
    if (parser.seen('E')) {
      #if AXIS_IS_TMC(E0)
        tmc_get_fast_decay_time(stepperE0, TMC_E0);
      #endif
    }
    if (parser.seen('X')) {
      #if AXIS_IS_TMC(X)
        tmc_get_fast_decay_time(stepperX, TMC_X);
      #endif
      #if AXIS_IS_TMC(X2)
        tmc_get_fast_decay_time(stepperX2, TMC_X);
      #endif
    }
    if (parser.seen('Y')) {
      #if AXIS_IS_TMC(Y)
        tmc_get_fast_decay_time(stepperY, TMC_Y);
      #endif
      #if AXIS_IS_TMC(Y2)
        tmc_get_fast_decay_time(stepperY2, TMC_Y2);
      #endif
    }
    if (parser.seen('Z')) {
      #if AXIS_IS_TMC(Z)
        tmc_get_fast_decay_time(stepperZ, TMC_Z);
      #endif
      #if AXIS_IS_TMC(Z2)
        tmc_get_fast_decay_time(stepperZ2, TMC_Z2);
      #endif
    }
    if (!parser.seen_any()) {
      #if AXIS_IS_TMC(E0)
        tmc_get_fast_decay_time(stepperE0, TMC_E0);
      #endif
      #if AXIS_IS_TMC(X)
        tmc_get_fast_decay_time(stepperX, TMC_X);
      #endif
      #if AXIS_IS_TMC(X2)
        tmc_get_fast_decay_time(stepperX2, TMC_X2);
      #endif
      #if AXIS_IS_TMC(Y)
        tmc_get_fast_decay_time(stepperY, TMC_Y);
      #endif
      #if AXIS_IS_TMC(Y2)
        tmc_get_fast_decay_time(stepperY2, TMC_Y2);
      #endif
      #if AXIS_IS_TMC(Z)
        tmc_get_fast_decay_time(stepperZ, TMC_Z);
      #endif
      #if AXIS_IS_TMC(Z2)
        tmc_get_fast_decay_time(stepperZ2, TMC_Z2);
      #endif
    }
}
#endif // TMC_CHOP_CALIBRATION

/**
 * M935: TMC set disable_I_comparator (chm = 1).
 */
#if ENABLED(TMC_CHOP_CALIBRATION)
void GcodeSuite::M935() {
    if (parser.seenval('E')) {
      #if AXIS_IS_TMC(E0)
        tmc_set_disable_I_comparator(stepperE0, parser.value_int());
      #endif
    }
    if (parser.seenval('X')) {
      #if AXIS_IS_TMC(X)
        tmc_set_disable_I_comparator(stepperX, parser.value_int());
      #endif
      #if AXIS_IS_TMC(X2)
        tmc_set_disable_I_comparator(stepperX2, parser.value_int());
      #endif
    }
    if (parser.seenval('Y')) {
      #if AXIS_IS_TMC(Y)
        tmc_set_disable_I_comparator(stepperY, parser.value_int());
      #endif
      #if AXIS_IS_TMC(Y2)
        tmc_set_disable_I_comparator(stepperY2, parser.value_int());
      #endif
    }
    if (parser.seenval('Z')) {
      #if AXIS_IS_TMC(Z)
        tmc_set_disable_I_comparator(stepperZ, parser.value_int());
      #endif
      #if AXIS_IS_TMC(Z2)
        tmc_set_disable_I_comparator(stepperZ2, parser.value_int());
      #endif
    }
    if (parser.seen('E')) {
      #if AXIS_IS_TMC(E0)
        tmc_get_disable_I_comparator(stepperE0, TMC_E0);
      #endif
    }
    if (parser.seen('X')) {
      #if AXIS_IS_TMC(X)
        tmc_get_disable_I_comparator(stepperX, TMC_X);
      #endif
      #if AXIS_IS_TMC(X2)
        tmc_get_disable_I_comparator(stepperX2, TMC_X);
      #endif
    }
    if (parser.seen('Y')) {
      #if AXIS_IS_TMC(Y)
        tmc_get_disable_I_comparator(stepperY, TMC_Y);
      #endif
      #if AXIS_IS_TMC(Y2)
        tmc_get_disable_I_comparator(stepperY2, TMC_Y2);
      #endif
    }
    if (parser.seen('Z')) {
      #if AXIS_IS_TMC(Z)
        tmc_get_disable_I_comparator(stepperZ, TMC_Z);
      #endif
      #if AXIS_IS_TMC(Z2)
        tmc_get_disable_I_comparator(stepperZ2, TMC_Z2);
      #endif
    }
    if (!parser.seen_any()) {
      #if AXIS_IS_TMC(E0)
        tmc_get_disable_I_comparator(stepperE0, TMC_E0);
      #endif
      #if AXIS_IS_TMC(X)
        tmc_get_disable_I_comparator(stepperX, TMC_X);
      #endif
      #if AXIS_IS_TMC(X2)
        tmc_get_disable_I_comparator(stepperX2, TMC_X2);
      #endif
      #if AXIS_IS_TMC(Y)
        tmc_get_disable_I_comparator(stepperY, TMC_Y);
      #endif
      #if AXIS_IS_TMC(Y2)
        tmc_get_disable_I_comparator(stepperY2, TMC_Y2);
      #endif
      #if AXIS_IS_TMC(Z)
        tmc_get_disable_I_comparator(stepperZ, TMC_Z);
      #endif
      #if AXIS_IS_TMC(Z2)
        tmc_get_disable_I_comparator(stepperZ2, TMC_Z2);
      #endif
    }
}
#endif // TMC_CHOP_CALIBRATION

/**
 * M936: TMC set stealth_gradient. 
 */
#if ENABLED(TMC_CHOP_CALIBRATION)
void GcodeSuite::M936() {
    if (parser.seenval('E')) {
      #if AXIS_IS_TMC(E0)
        tmc_set_stealth_gradient(stepperE0, parser.value_int());
      #endif
    }
    if (parser.seenval('X')) {
      #if AXIS_IS_TMC(X)
        tmc_set_stealth_gradient(stepperX, parser.value_int());
      #endif
      #if AXIS_IS_TMC(X2)
        tmc_set_stealth_gradient(stepperX2, parser.value_int());
      #endif
    }
    if (parser.seenval('Y')) {
      #if AXIS_IS_TMC(Y)
        tmc_set_stealth_gradient(stepperY, parser.value_int());
      #endif
      #if AXIS_IS_TMC(Y2)
        tmc_set_stealth_gradient(stepperY2, parser.value_int());
      #endif
    }
    if (parser.seenval('Z')) {
      #if AXIS_IS_TMC(Z)
        tmc_set_stealth_gradient(stepperZ, parser.value_int());
      #endif
      #if AXIS_IS_TMC(Z2)
        tmc_set_stealth_gradient(stepperZ2, parser.value_int());
      #endif
    }
    if (parser.seen('E')) {
      #if AXIS_IS_TMC(E0)
        tmc_get_stealth_gradient(stepperE0, TMC_E0);
      #endif
    }
    if (parser.seen('X')) {
      #if AXIS_IS_TMC(X)
        tmc_get_stealth_gradient(stepperX, TMC_X);
      #endif
      #if AXIS_IS_TMC(X2)
        tmc_get_stealth_gradient(stepperX2, TMC_X);
      #endif
    }
    if (parser.seen('Y')) {
      #if AXIS_IS_TMC(Y)
        tmc_get_stealth_gradient(stepperY, TMC_Y);
      #endif
      #if AXIS_IS_TMC(Y2)
        tmc_get_stealth_gradient(stepperY2, TMC_Y2);
      #endif
    }
    if (parser.seen('Z')) {
      #if AXIS_IS_TMC(Z)
        tmc_get_stealth_gradient(stepperZ, TMC_Z);
      #endif
      #if AXIS_IS_TMC(Z2)
        tmc_get_stealth_gradient(stepperZ2, TMC_Z2);
      #endif
    }
    if (!parser.seen_any()) {
      #if AXIS_IS_TMC(E0)
        tmc_get_stealth_gradient(stepperE0, TMC_E0);
      #endif
      #if AXIS_IS_TMC(X)
        tmc_get_stealth_gradient(stepperX, TMC_X);
      #endif
      #if AXIS_IS_TMC(X2)
        tmc_get_stealth_gradient(stepperX2, TMC_X2);
      #endif
      #if AXIS_IS_TMC(Y)
        tmc_get_stealth_gradient(stepperY, TMC_Y);
      #endif
      #if AXIS_IS_TMC(Y2)
        tmc_get_stealth_gradient(stepperY2, TMC_Y2);
      #endif
      #if AXIS_IS_TMC(Z)
        tmc_get_stealth_gradient(stepperZ, TMC_Z);
      #endif
      #if AXIS_IS_TMC(Z2)
        tmc_get_stealth_gradient(stepperZ2, TMC_Z2);
      #endif
    }
}
#endif // TMC_CHOP_CALIBRATION

/**
 * M937: TMC set stealth_amplitude.
 */
#if ENABLED(TMC_CHOP_CALIBRATION)
  void GcodeSuite::M937() {
    if (parser.seenval('E')) {
      #if AXIS_IS_TMC(E0)
        tmc_set_stealth_amplitude(stepperE0, parser.value_int());
      #endif
    }
    if (parser.seenval('X')) {
      #if AXIS_IS_TMC(X)
        tmc_set_stealth_amplitude(stepperX, parser.value_int());
      #endif
      #if AXIS_IS_TMC(X2)
        tmc_set_stealth_amplitude(stepperX2, parser.value_int());
      #endif
    }
    if (parser.seenval('Y')) {
      #if AXIS_IS_TMC(Y)
        tmc_set_stealth_amplitude(stepperY, parser.value_int());
      #endif
      #if AXIS_IS_TMC(Y2)
        tmc_set_stealth_amplitude(stepperY2, parser.value_int());
      #endif
    }
    if (parser.seenval('Z')) {
      #if AXIS_IS_TMC(Z)
        tmc_set_stealth_amplitude(stepperZ, parser.value_int());
      #endif
      #if AXIS_IS_TMC(Z2)
        tmc_set_stealth_amplitude(stepperZ2, parser.value_int());
      #endif
    }
    if (parser.seen('E')) {
      #if AXIS_IS_TMC(E0)
        tmc_get_stealth_amplitude(stepperE0, TMC_E0);
      #endif
    }
    if (parser.seen('X')) {
      #if AXIS_IS_TMC(X)
        tmc_get_stealth_amplitude(stepperX, TMC_X);
      #endif
      #if AXIS_IS_TMC(X2)
        tmc_get_stealth_amplitude(stepperX2, TMC_X);
      #endif
    }
    if (parser.seen('Y')) {
      #if AXIS_IS_TMC(Y)
        tmc_get_stealth_amplitude(stepperY, TMC_Y);
      #endif
      #if AXIS_IS_TMC(Y2)
        tmc_get_stealth_amplitude(stepperY2, TMC_Y2);
      #endif
    }
    if (parser.seen('Z')) {
      #if AXIS_IS_TMC(Z)
        tmc_get_stealth_amplitude(stepperZ, TMC_Z);
      #endif
      #if AXIS_IS_TMC(Z2)
        tmc_get_stealth_amplitude(stepperZ2, TMC_Z2);
      #endif
    }
    if (!parser.seen_any()) {
      #if AXIS_IS_TMC(E0)
        tmc_get_stealth_amplitude(stepperE0, TMC_E0);
      #endif
      #if AXIS_IS_TMC(X)
        tmc_get_stealth_amplitude(stepperX, TMC_X);
      #endif
      #if AXIS_IS_TMC(X2)
        tmc_get_stealth_amplitude(stepperX2, TMC_X2);
      #endif
      #if AXIS_IS_TMC(Y)
        tmc_get_stealth_amplitude(stepperY, TMC_Y);
      #endif
      #if AXIS_IS_TMC(Y2)
        tmc_get_stealth_amplitude(stepperY2, TMC_Y2);
      #endif
      #if AXIS_IS_TMC(Z)
        tmc_get_stealth_amplitude(stepperZ, TMC_Z);
      #endif
      #if AXIS_IS_TMC(Z2)
        tmc_get_stealth_amplitude(stepperZ2, TMC_Z2);
      #endif
    }
}
#endif // TMC_CHOP_CALIBRATION

/**
 * M938: TMC set stealth_freq.
 */
#if ENABLED(TMC_CHOP_CALIBRATION)
  void GcodeSuite::M938() {
    if (parser.seenval('E')) {
      #if AXIS_IS_TMC(E0)
        tmc_set_stealth_freq(stepperE0, parser.value_int());
      #endif
    }
    if (parser.seenval('X')) {
      #if AXIS_IS_TMC(X)
        tmc_set_stealth_freq(stepperX, parser.value_int());
      #endif
      #if AXIS_IS_TMC(X2)
        tmc_set_stealth_freq(stepperX2, parser.value_int());
      #endif
    }
    if (parser.seenval('Y')) {
      #if AXIS_IS_TMC(Y)
        tmc_set_stealth_freq(stepperY, parser.value_int());
      #endif
      #if AXIS_IS_TMC(Y2)
        tmc_set_stealth_freq(stepperY2, parser.value_int());
      #endif
    }
    if (parser.seenval('Z')) {
      #if AXIS_IS_TMC(Z)
        tmc_set_stealth_freq(stepperZ, parser.value_int());
      #endif
      #if AXIS_IS_TMC(Z2)
        tmc_set_stealth_freq(stepperZ2, parser.value_int());
      #endif
    }
    if (parser.seen('E')) {
      #if AXIS_IS_TMC(E0)
        tmc_get_stealth_freq(stepperE0, TMC_E0);
      #endif
    }
    if (parser.seen('X')) {
      #if AXIS_IS_TMC(X)
        tmc_get_stealth_freq(stepperX, TMC_X);
      #endif
      #if AXIS_IS_TMC(X2)
        tmc_get_stealth_freq(stepperX2, TMC_X);
      #endif
    }
    if (parser.seen('Y')) {
      #if AXIS_IS_TMC(Y)
        tmc_get_stealth_freq(stepperY, TMC_Y);
      #endif
      #if AXIS_IS_TMC(Y2)
        tmc_get_stealth_freq(stepperY2, TMC_Y2);
      #endif
    }
    if (parser.seen('Z')) {
      #if AXIS_IS_TMC(Z)
        tmc_get_stealth_freq(stepperZ, TMC_Z);
      #endif
      #if AXIS_IS_TMC(Z2)
        tmc_get_stealth_freq(stepperZ2, TMC_Z2);
      #endif
    }
    if (!parser.seen_any()) {
      #if AXIS_IS_TMC(E0)
        tmc_get_stealth_freq(stepperE0, TMC_E0);
      #endif
      #if AXIS_IS_TMC(X)
        tmc_get_stealth_freq(stepperX, TMC_X);
      #endif
      #if AXIS_IS_TMC(X2)
        tmc_get_stealth_freq(stepperX2, TMC_X2);
      #endif
      #if AXIS_IS_TMC(Y)
        tmc_get_stealth_freq(stepperY, TMC_Y);
      #endif
      #if AXIS_IS_TMC(Y2)
        tmc_get_stealth_freq(stepperY2, TMC_Y2);
      #endif
      #if AXIS_IS_TMC(Z)
        tmc_get_stealth_freq(stepperZ, TMC_Z);
      #endif
      #if AXIS_IS_TMC(Z2)
        tmc_get_stealth_freq(stepperZ2, TMC_Z2);
      #endif
    }
}
#endif // TMC_CHOP_CALIBRATION

/**
 * M939: TMC switch stealth_autoscale. 
 */
#if ENABLED(TMC_CHOP_CALIBRATION)
  void GcodeSuite::M939() {
    if (parser.seenval('E')) {
      #if AXIS_IS_TMC(E0)
        tmc_set_stealth_autoscale(stepperE0, parser.value_int());
      #endif
    }
    if (parser.seenval('X')) {
      #if AXIS_IS_TMC(X)
        tmc_set_stealth_autoscale(stepperX, parser.value_int());
      #endif
      #if AXIS_IS_TMC(X2)
        tmc_set_stealth_autoscale(stepperX2, parser.value_int());
      #endif
    }
    if (parser.seenval('Y')) {
      #if AXIS_IS_TMC(Y)
        tmc_set_stealth_autoscale(stepperY, parser.value_int());
      #endif
      #if AXIS_IS_TMC(Y2)
        tmc_set_stealth_autoscale(stepperY2, parser.value_int());
      #endif
    }
    if (parser.seenval('Z')) {
      #if AXIS_IS_TMC(Z)
        tmc_set_stealth_autoscale(stepperZ, parser.value_int());
      #endif
      #if AXIS_IS_TMC(Z2)
        tmc_set_stealth_autoscale(stepperZ2, parser.value_int());
      #endif
    }
    if (parser.seen('E')) {
      #if AXIS_IS_TMC(E0)
        tmc_get_stealth_autoscale(stepperE0, TMC_E0);
      #endif
    }
    if (parser.seen('X')) {
      #if AXIS_IS_TMC(X)
        tmc_get_stealth_autoscale(stepperX, TMC_X);
      #endif
      #if AXIS_IS_TMC(X2)
        tmc_get_stealth_autoscale(stepperX2, TMC_X);
      #endif
    }
    if (parser.seen('Y')) {
      #if AXIS_IS_TMC(Y)
        tmc_get_stealth_autoscale(stepperY, TMC_Y);
      #endif
      #if AXIS_IS_TMC(Y2)
        tmc_get_stealth_autoscale(stepperY2, TMC_Y2);
      #endif
    }
    if (parser.seen('Z')) {
      #if AXIS_IS_TMC(Z)
        tmc_get_stealth_autoscale(stepperZ, TMC_Z);
      #endif
      #if AXIS_IS_TMC(Z2)
        tmc_get_stealth_autoscale(stepperZ2, TMC_Z2);
      #endif
    }
    if (!parser.seen_any()) {
      #if AXIS_IS_TMC(E0)
        tmc_get_stealth_autoscale(stepperE0, TMC_E0);
      #endif
      #if AXIS_IS_TMC(X)
        tmc_get_stealth_autoscale(stepperX, TMC_X);
      #endif
      #if AXIS_IS_TMC(X2)
        tmc_get_stealth_autoscale(stepperX2, TMC_X2);
      #endif
      #if AXIS_IS_TMC(Y)
        tmc_get_stealth_autoscale(stepperY, TMC_Y);
      #endif
      #if AXIS_IS_TMC(Y2)
        tmc_get_stealth_autoscale(stepperY2, TMC_Y2);
      #endif
      #if AXIS_IS_TMC(Z)
        tmc_get_stealth_autoscale(stepperZ, TMC_Z);
      #endif
      #if AXIS_IS_TMC(Z2)
        tmc_get_stealth_autoscale(stepperZ2, TMC_Z2);
      #endif
    }
}
#endif // TMC_CHOP_CALIBRATION

#endif // HAS_TRINAMIC
