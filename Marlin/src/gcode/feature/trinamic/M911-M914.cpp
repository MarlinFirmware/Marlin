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

#if ENABLED(HAVE_TMC2130)

#include "../../gcode.h"
#include "../../../feature/tmc2130.h"
#include "../../../module/stepper_indirection.h"

inline void tmc2130_report_otpw(TMC2130Stepper &st, const char name) {
  SERIAL_CHAR(name);
  SERIAL_ECHOPGM(" axis temperature prewarn triggered: ");
  serialprintPGM(st.getOTPW() ? PSTR("true") : PSTR("false"));
  SERIAL_EOL();
}

/**
 * M911: Report TMC2130 stepper driver overtemperature pre-warn flag
 * The flag is held by the library and persist until manually cleared by M912
 */
void GcodeSuite::M911() {
  const bool reportX = parser.seen('X'), reportY = parser.seen('Y'), reportZ = parser.seen('Z'), reportE = parser.seen('E'),
           reportAll = (!reportX && !reportY && !reportZ && !reportE) || (reportX && reportY && reportZ && reportE);
  #if ENABLED(X_IS_TMC2130)
    if (reportX || reportAll) tmc2130_report_otpw(stepperX, 'X');
  #endif
  #if ENABLED(Y_IS_TMC2130)
    if (reportY || reportAll) tmc2130_report_otpw(stepperY, 'Y');
  #endif
  #if ENABLED(Z_IS_TMC2130)
    if (reportZ || reportAll) tmc2130_report_otpw(stepperZ, 'Z');
  #endif
  #if ENABLED(E0_IS_TMC2130)
    if (reportE || reportAll) tmc2130_report_otpw(stepperE0, 'E');
  #endif
}

inline void tmc2130_clear_otpw(TMC2130Stepper &st, const char name) {
  st.clear_otpw();
  SERIAL_CHAR(name);
  SERIAL_ECHOLNPGM(" prewarn flag cleared");
}

/**
 * M912: Clear TMC2130 stepper driver overtemperature pre-warn flag held by the library
 */
void GcodeSuite::M912() {
  const bool clearX = parser.seen('X'), clearY = parser.seen('Y'), clearZ = parser.seen('Z'), clearE = parser.seen('E'),
           clearAll = (!clearX && !clearY && !clearZ && !clearE) || (clearX && clearY && clearZ && clearE);
  #if ENABLED(X_IS_TMC2130)
    if (clearX || clearAll) tmc2130_clear_otpw(stepperX, 'X');
  #endif
  #if ENABLED(Y_IS_TMC2130)
    if (clearY || clearAll) tmc2130_clear_otpw(stepperY, 'Y');
  #endif
  #if ENABLED(Z_IS_TMC2130)
    if (clearZ || clearAll) tmc2130_clear_otpw(stepperZ, 'Z');
  #endif
  #if ENABLED(E0_IS_TMC2130)
    if (clearE || clearAll) tmc2130_clear_otpw(stepperE0, 'E');
  #endif
}

#if ENABLED(HYBRID_THRESHOLD)

  #include "../../../module/planner.h"

  inline void tmc2130_get_pwmthrs(TMC2130Stepper &st, const char name, const uint16_t spmm) {
    SERIAL_CHAR(name);
    SERIAL_ECHOPGM(" stealthChop max speed set to ");
    SERIAL_ECHOLN(12650000UL * st.microsteps() / (256 * st.stealth_max_speed() * spmm));
  }
  inline void tmc2130_set_pwmthrs(TMC2130Stepper &st, const char name, const int32_t thrs, const uint32_t spmm) {
    st.stealth_max_speed(12650000UL * st.microsteps() / (256 * thrs * spmm));
    tmc2130_get_pwmthrs(st, name, spmm);
  }

  /**
   * M913: Set HYBRID_THRESHOLD speed.
   */
  void GcodeSuite::M913() {
    uint16_t values[XYZE];
    LOOP_XYZE(i)
      values[i] = parser.intval(axis_codes[i]);

    #if ENABLED(X_IS_TMC2130)
      if (values[X_AXIS]) tmc2130_set_pwmthrs(stepperX, 'X', values[X_AXIS], planner.axis_steps_per_mm[X_AXIS]);
      else tmc2130_get_pwmthrs(stepperX, 'X', planner.axis_steps_per_mm[X_AXIS]);
    #endif
    #if ENABLED(Y_IS_TMC2130)
      if (values[Y_AXIS]) tmc2130_set_pwmthrs(stepperY, 'Y', values[Y_AXIS], planner.axis_steps_per_mm[Y_AXIS]);
      else tmc2130_get_pwmthrs(stepperY, 'Y', planner.axis_steps_per_mm[Y_AXIS]);
    #endif
    #if ENABLED(Z_IS_TMC2130)
      if (values[Z_AXIS]) tmc2130_set_pwmthrs(stepperZ, 'Z', values[Z_AXIS], planner.axis_steps_per_mm[Z_AXIS]);
      else tmc2130_get_pwmthrs(stepperZ, 'Z', planner.axis_steps_per_mm[Z_AXIS]);
    #endif
    #if ENABLED(E0_IS_TMC2130)
      if (values[E_AXIS]) tmc2130_set_pwmthrs(stepperE0, 'E', values[E_AXIS], planner.axis_steps_per_mm[E_AXIS]);
      else tmc2130_get_pwmthrs(stepperE0, 'E', planner.axis_steps_per_mm[E_AXIS]);
    #endif
  }

#endif // HYBRID_THRESHOLD

#if ENABLED(SENSORLESS_HOMING)

  inline void tmc2130_get_sgt(TMC2130Stepper &st, const char name) {
    SERIAL_CHAR(name);
    SERIAL_ECHOPGM(" driver homing sensitivity set to ");
    SERIAL_ECHOLN(st.sgt());
  }
  inline void tmc2130_set_sgt(TMC2130Stepper &st, const char name, const int8_t sgt_val) {
    st.sgt(sgt_val);
    tmc2130_get_sgt(st, name);
  }

  /**
   * M914: Set SENSORLESS_HOMING sensitivity.
   */
  void GcodeSuite::M914() {
    #if ENABLED(X_IS_TMC2130)
      if (parser.seen(axis_codes[X_AXIS])) tmc2130_set_sgt(stepperX, 'X', parser.value_int());
      else tmc2130_get_sgt(stepperX, 'X');
    #endif
    #if ENABLED(Y_IS_TMC2130)
      if (parser.seen(axis_codes[Y_AXIS])) tmc2130_set_sgt(stepperY, 'Y', parser.value_int());
      else tmc2130_get_sgt(stepperY, 'Y');
    #endif
  }

#endif // SENSORLESS_HOMING

#endif // HAVE_TMC2130
