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
#include "../../../module/planner.h"
#include "../../queue.h"

#define M91x_USE(A) (ENABLED(A##_IS_TMC2130) || (ENABLED(A##_IS_TMC2208) && PIN_EXISTS(A##_SERIAL_RX)))
#define M91x_USE_E(N) (E_STEPPERS > N && M91x_USE(E##N))
#define M91x_USE_X  (ENABLED(IS_TRAMS) || M91x_USE(X))
#define M91x_USE_Y  (ENABLED(IS_TRAMS) || M91x_USE(Y))
#define M91x_USE_Z  (ENABLED(IS_TRAMS) || M91x_USE(Z))
#define M91x_USE_E0 (ENABLED(IS_TRAMS) || M91x_USE_E(0))

/**
 * M911: Report TMC stepper driver overtemperature pre-warn flag
 *       This flag is held by the library, persisting until cleared by M912
 */
void GcodeSuite::M911() {
  #if M91x_USE_X
    tmc_report_otpw(stepperX, TMC_X);
  #endif
  #if M91x_USE(X2)
    tmc_report_otpw(stepperX2, TMC_X2);
  #endif
  #if M91x_USE_Y
    tmc_report_otpw(stepperY, TMC_Y);
  #endif
  #if M91x_USE(Y2)
    tmc_report_otpw(stepperY2, TMC_Y2);
  #endif
  #if M91x_USE_Z
    tmc_report_otpw(stepperZ, TMC_Z);
  #endif
  #if M91x_USE(Z2)
    tmc_report_otpw(stepperZ2, TMC_Z2);
  #endif
  #if M91x_USE_E0
    tmc_report_otpw(stepperE0, TMC_E0);
  #endif
  #if M91x_USE_E(1)
    tmc_report_otpw(stepperE1, TMC_E1);
  #endif
  #if M91x_USE_E(2)
    tmc_report_otpw(stepperE2, TMC_E2);
  #endif
  #if M91x_USE_E(3)
    tmc_report_otpw(stepperE3, TMC_E3);
  #endif
  #if M91x_USE_E(4)
    tmc_report_otpw(stepperE4, TMC_E4);
  #endif
}

/**
 * M912: Clear TMC stepper driver overtemperature pre-warn flag held by the library
 *       Specify one or more axes with X, Y, Z, X1, Y1, Z1, X2, Y2, Z2, and E[index].
 *       If no axes are given, clear all.
 *
 * Examples:
 *       M912 X   ; clear X and X2
 *       M912 X1  ; clear X1 only
 *       M912 X2  ; clear X2 only
 *       M912 X E ; clear X, X2, and all E
 *       M912 E1  ; clear E1 only
 */
void GcodeSuite::M912() {
    const bool hasX = parser.seen(axis_codes[X_AXIS]),
               hasY = parser.seen(axis_codes[Y_AXIS]),
               hasZ = parser.seen(axis_codes[Z_AXIS]),
               hasE = parser.seen(axis_codes[E_AXIS]),
               hasNone = !hasX && !hasY && !hasZ && !hasE;

    #if M91x_USE_X || M91x_USE(X2)
      const uint8_t xval = parser.byteval(axis_codes[X_AXIS], 10);
      #if M91x_USE_X
        if (hasNone || xval == 1 || (hasX && xval == 10)) tmc_clear_otpw(stepperX, TMC_X);
      #endif
      #if M91x_USE(X2)
        if (hasNone || xval == 2 || (hasX && xval == 10)) tmc_clear_otpw(stepperX2, TMC_X2);
      #endif
    #endif

    #if M91x_USE_Y || M91x_USE(Y2)
      const uint8_t yval = parser.byteval(axis_codes[Y_AXIS], 10);
      #if M91x_USE_Y
        if (hasNone || yval == 1 || (hasY && yval == 10)) tmc_clear_otpw(stepperY, TMC_Y);
      #endif
      #if M91x_USE(Y2)
        if (hasNone || yval == 2 || (hasY && yval == 10)) tmc_clear_otpw(stepperY2, TMC_Y2);
      #endif
    #endif

    #if M91x_USE_Z || M91x_USE(Z2)
      const uint8_t zval = parser.byteval(axis_codes[Z_AXIS], 10);
      #if M91x_USE_Z
        if (hasNone || zval == 1 || (hasZ && zval == 10)) tmc_clear_otpw(stepperZ, TMC_Z);
      #endif
      #if M91x_USE(Z2)
        if (hasNone || zval == 2 || (hasZ && zval == 10)) tmc_clear_otpw(stepperZ2, TMC_Z2);
      #endif
    #endif

    #if M91x_USE_E0 || M91x_USE_E(1) || M91x_USE_E(2) || M91x_USE_E(3) || M91x_USE_E(4)
      const uint8_t eval = parser.byteval(axis_codes[E_AXIS], 10);
      #if M91x_USE_E0
        if (hasNone || eval == 0 || (hasE && eval == 10)) tmc_clear_otpw(stepperE0, TMC_E0);
      #endif
      #if M91x_USE_E(1)
        if (hasNone || eval == 1 || (hasE && eval == 10)) tmc_clear_otpw(stepperE1, TMC_E1);
      #endif
      #if M91x_USE_E(2)
        if (hasNone || eval == 2 || (hasE && eval == 10)) tmc_clear_otpw(stepperE2, TMC_E2);
      #endif
      #if M91x_USE_E(3)
        if (hasNone || eval == 3 || (hasE && eval == 10)) tmc_clear_otpw(stepperE3, TMC_E3);
      #endif
      #if M91x_USE_E(4)
        if (hasNone || eval == 4 || (hasE && eval == 10)) tmc_clear_otpw(stepperE4, TMC_E4);
      #endif
    #endif
}

/**
 * M913: Set HYBRID_THRESHOLD speed.
 */
#if ENABLED(HYBRID_THRESHOLD)
  void GcodeSuite::M913() {
    #define TMC_SAY_PWMTHRS(A,Q) tmc_get_pwmthrs(stepper##Q, TMC_##Q, planner.axis_steps_per_mm[_AXIS(A)])
    #define TMC_SET_PWMTHRS(A,Q) tmc_set_pwmthrs(stepper##Q, value, planner.axis_steps_per_mm[_AXIS(A)])
    #define TMC_SAY_PWMTHRS_E(E) do{ const uint8_t extruder = E; tmc_get_pwmthrs(stepperE##E, TMC_E##E, planner.axis_steps_per_mm[E_AXIS_N]); }while(0)
    #define TMC_SET_PWMTHRS_E(E) do{ const uint8_t extruder = E; tmc_set_pwmthrs(stepperE##E, value, planner.axis_steps_per_mm[E_AXIS_N]); }while(0)

    bool report = true;
    const uint8_t index = parser.byteval('I');
    LOOP_XYZE(i) if (int32_t value = parser.longval(axis_codes[i])) {
      report = false;
      switch (i) {
        case X_AXIS:
          #if X_IS_TRINAMIC
            if (index == 0) TMC_SET_PWMTHRS(X,X);
          #endif
          #if X2_IS_TRINAMIC
            if (index == 1) TMC_SET_PWMTHRS(X,X2);
          #endif
          break;
        case Y_AXIS:
          #if Y_IS_TRINAMIC
            if (index == 0) TMC_SET_PWMTHRS(Y,Y);
          #endif
          #if Y2_IS_TRINAMIC
            if (index == 1) TMC_SET_PWMTHRS(Y,Y2);
          #endif
          break;
        case Z_AXIS:
          #if Z_IS_TRINAMIC
            if (index == 0) TMC_SET_PWMTHRS(Z,Z);
          #endif
          #if Z2_IS_TRINAMIC
            if (index == 1) TMC_SET_PWMTHRS(Z,Z2);
          #endif
          break;
        case E_AXIS: {
          if (get_target_extruder_from_command()) return;
          switch (target_extruder) {
            #if E0_IS_TRINAMIC
              case 0: TMC_SET_PWMTHRS_E(0); break;
            #endif
            #if E_STEPPERS > 1 && E1_IS_TRINAMIC
              case 1: TMC_SET_PWMTHRS_E(1); break;
            #endif
            #if E_STEPPERS > 2 && E2_IS_TRINAMIC
              case 2: TMC_SET_PWMTHRS_E(2); break;
            #endif
            #if E_STEPPERS > 3 && E3_IS_TRINAMIC
              case 3: TMC_SET_PWMTHRS_E(3); break;
            #endif
            #if E_STEPPERS > 4 && E4_IS_TRINAMIC
              case 4: TMC_SET_PWMTHRS_E(4); break;
            #endif
          }
        } break;
      }
    }

    if (report) LOOP_XYZE(i) switch (i) {
      case X_AXIS:
        #if X_IS_TRINAMIC
          TMC_SAY_PWMTHRS(X,X);
        #endif
        #if X2_IS_TRINAMIC
          TMC_SAY_PWMTHRS(X,X2);
        #endif
        break;
      case Y_AXIS:
        #if Y_IS_TRINAMIC
          TMC_SAY_PWMTHRS(Y,Y);
        #endif
        #if Y2_IS_TRINAMIC
          TMC_SAY_PWMTHRS(Y,Y2);
        #endif
        break;
      case Z_AXIS:
        #if Z_IS_TRINAMIC
          TMC_SAY_PWMTHRS(Z,Z);
        #endif
        #if Z2_IS_TRINAMIC
          TMC_SAY_PWMTHRS(Z,Z2);
        #endif
        break;
      case E_AXIS:
        #if E0_IS_TRINAMIC
          TMC_SAY_PWMTHRS_E(0);
        #endif
        #if E_STEPPERS > 1 && E1_IS_TRINAMIC
          TMC_SAY_PWMTHRS_E(1);
        #endif
        #if E_STEPPERS > 2 && E2_IS_TRINAMIC
          TMC_SAY_PWMTHRS_E(2);
        #endif
        #if E_STEPPERS > 3 && E3_IS_TRINAMIC
          TMC_SAY_PWMTHRS_E(3);
        #endif
        #if E_STEPPERS > 4 && E4_IS_TRINAMIC
          TMC_SAY_PWMTHRS_E(4);
        #endif
        break;
    }
  }
#endif // HYBRID_THRESHOLD

/**
 * M914: Set SENSORLESS_HOMING sensitivity.
 */
#if ENABLED(SENSORLESS_HOMING)
  void GcodeSuite::M914() {
    #define TMC_SAY_SGT(Q) tmc_get_sgt(stepper##Q, TMC_##Q)
    #define TMC_SET_SGT(Q) tmc_set_sgt(stepper##Q, value)

    bool report = true;
    const uint8_t index = parser.byteval('I');
    LOOP_XYZ(i) if (parser.seen(axis_codes[i])) {
      const int8_t value = (int8_t)constrain(parser.value_int(), -64, 63);
      report = false;
      switch (i) {
        case X_AXIS:
          #if ENABLED(X_IS_TMC2130) || ENABLED(IS_TRAMS)
            if (index == 0) TMC_SET_SGT(X);
          #endif
          #if ENABLED(X2_IS_TMC2130)
            if (index == 1) TMC_SET_SGT(X2);
          #endif
          break;
        case Y_AXIS:
          #if ENABLED(Y_IS_TMC2130) || ENABLED(IS_TRAMS)
            if (index == 0) TMC_SET_SGT(Y);
          #endif
          #if ENABLED(Y2_IS_TMC2130)
            if (index == 1) TMC_SET_SGT(Y2);
          #endif
          break;
        case Z_AXIS:
          #if ENABLED(Z_IS_TMC2130) || ENABLED(IS_TRAMS)
            if (index == 0) TMC_SET_SGT(Z);
          #endif
          #if ENABLED(Z2_IS_TMC2130)
            if (index == 1) TMC_SET_SGT(Z2);
          #endif
          break;
      }
    }

    if (report) LOOP_XYZ(i) switch (i) {
      case X_AXIS:
        #if ENABLED(X_IS_TMC2130) || ENABLED(IS_TRAMS)
          TMC_SAY_SGT(X);
        #endif
        #if ENABLED(X2_IS_TMC2130)
          TMC_SAY_SGT(X2);
        #endif
        break;
      case Y_AXIS:
        #if ENABLED(Y_IS_TMC2130) || ENABLED(IS_TRAMS)
          TMC_SAY_SGT(Y);
        #endif
        #if ENABLED(Y2_IS_TMC2130)
          TMC_SAY_SGT(Y2);
        #endif
        break;
      case Z_AXIS:
        #if ENABLED(Z_IS_TMC2130) || ENABLED(IS_TRAMS)
          TMC_SAY_SGT(Z);
        #endif
        #if ENABLED(Z2_IS_TMC2130)
          TMC_SAY_SGT(Z2);
        #endif
        break;
    }
  }
#endif // SENSORLESS_HOMING

/**
 * TMC Z axis calibration routine
 */
#if ENABLED(TMC_Z_CALIBRATION)
  void GcodeSuite::M915() {
    const uint16_t _rms = parser.seenval('S') ? parser.value_int() : CALIBRATION_CURRENT,
                   _z = parser.seenval('Z') ? parser.value_linear_units() : CALIBRATION_EXTRA_HEIGHT;

    if (!axis_known_position[Z_AXIS]) {
      SERIAL_ECHOLNPGM("\nPlease home Z axis first");
      return;
    }

    #if Z_IS_TRINAMIC
      const uint16_t Z_current_1 = stepperZ.getCurrent();
      stepperZ.setCurrent(_rms, R_SENSE, HOLD_MULTIPLIER);
    #endif
    #if Z2_IS_TRINAMIC
      const uint16_t Z2_current_1 = stepperZ2.getCurrent();
      stepperZ2.setCurrent(_rms, R_SENSE, HOLD_MULTIPLIER);
    #endif

    SERIAL_ECHOPAIR("\nCalibration current: Z", _rms);

    soft_endstops_enabled = false;

    do_blocking_move_to_z(Z_MAX_POS+_z);

    #if Z_IS_TRINAMIC
      stepperZ.setCurrent(Z_current_1, R_SENSE, HOLD_MULTIPLIER);
    #endif
    #if Z2_IS_TRINAMIC
      stepperZ2.setCurrent(Z2_current_1, R_SENSE, HOLD_MULTIPLIER);
    #endif

    do_blocking_move_to_z(Z_MAX_POS);
    soft_endstops_enabled = true;

    SERIAL_ECHOLNPGM("\nHoming Z because we lost steps");
    enqueue_and_echo_commands_P(PSTR("G28 Z"));
  }
#endif

#endif // HAS_TRINAMIC
