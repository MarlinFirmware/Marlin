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

#define M91x_USE(ST) (AXIS_DRIVER_TYPE(ST, TMC2130) || (AXIS_DRIVER_TYPE(ST, TMC2208) && PIN_EXISTS(ST##_SERIAL_RX)))
#define M91x_USE_E(N) (E_STEPPERS > N && M91x_USE(E##N))

/**
 * M911: Report TMC stepper driver overtemperature pre-warn flag
 *       This flag is held by the library, persisting until cleared by M912
 */
void GcodeSuite::M911() {
  #if M91x_USE(X)
    tmc_report_otpw(stepperX, TMC_X);
  #endif
  #if M91x_USE(X2)
    tmc_report_otpw(stepperX2, TMC_X2);
  #endif
  #if M91x_USE(Y)
    tmc_report_otpw(stepperY, TMC_Y);
  #endif
  #if M91x_USE(Y2)
    tmc_report_otpw(stepperY2, TMC_Y2);
  #endif
  #if M91x_USE(Z)
    tmc_report_otpw(stepperZ, TMC_Z);
  #endif
  #if M91x_USE(Z2)
    tmc_report_otpw(stepperZ2, TMC_Z2);
  #endif
  #if M91x_USE_E(0)
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

    #if M91x_USE(X) || M91x_USE(X2)
      const uint8_t xval = parser.byteval(axis_codes[X_AXIS], 10);
      #if M91x_USE(X)
        if (hasNone || xval == 1 || (hasX && xval == 10)) tmc_clear_otpw(stepperX, TMC_X);
      #endif
      #if M91x_USE(X2)
        if (hasNone || xval == 2 || (hasX && xval == 10)) tmc_clear_otpw(stepperX2, TMC_X2);
      #endif
    #endif

    #if M91x_USE(Y) || M91x_USE(Y2)
      const uint8_t yval = parser.byteval(axis_codes[Y_AXIS], 10);
      #if M91x_USE(Y)
        if (hasNone || yval == 1 || (hasY && yval == 10)) tmc_clear_otpw(stepperY, TMC_Y);
      #endif
      #if M91x_USE(Y2)
        if (hasNone || yval == 2 || (hasY && yval == 10)) tmc_clear_otpw(stepperY2, TMC_Y2);
      #endif
    #endif

    #if M91x_USE(Z) || M91x_USE(Z2)
      const uint8_t zval = parser.byteval(axis_codes[Z_AXIS], 10);
      #if M91x_USE(Z)
        if (hasNone || zval == 1 || (hasZ && zval == 10)) tmc_clear_otpw(stepperZ, TMC_Z);
      #endif
      #if M91x_USE(Z2)
        if (hasNone || zval == 2 || (hasZ && zval == 10)) tmc_clear_otpw(stepperZ2, TMC_Z2);
      #endif
    #endif

    #if M91x_USE_E(0) || M91x_USE_E(1) || M91x_USE_E(2) || M91x_USE_E(3) || M91x_USE_E(4)
      const uint8_t eval = parser.byteval(axis_codes[E_AXIS], 10);
      #if M91x_USE_E(0)
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
          #if AXIS_HAS_STEALTHCHOP(X)
            if (index < 2) TMC_SET_PWMTHRS(X,X);
          #endif
          #if AXIS_HAS_STEALTHCHOP(X2)
            if (!(index & 1)) TMC_SET_PWMTHRS(X,X2);
          #endif
          break;
        case Y_AXIS:
          #if AXIS_HAS_STEALTHCHOP(Y)
            if (index < 2) TMC_SET_PWMTHRS(Y,Y);
          #endif
          #if AXIS_HAS_STEALTHCHOP(Y2)
            if (!(index & 1)) TMC_SET_PWMTHRS(Y,Y2);
          #endif
          break;
        case Z_AXIS:
          #if AXIS_HAS_STEALTHCHOP(Z)
            if (index < 2) TMC_SET_PWMTHRS(Z,Z);
          #endif
          #if AXIS_HAS_STEALTHCHOP(Z2)
            if (!(index & 1)) TMC_SET_PWMTHRS(Z,Z2);
          #endif
          break;
        case E_AXIS: {
          if (get_target_extruder_from_command()) return;
          switch (target_extruder) {
            #if AXIS_HAS_STEALTHCHOP(E0)
              case 0: TMC_SET_PWMTHRS_E(0); break;
            #endif
            #if E_STEPPERS > 1 && AXIS_HAS_STEALTHCHOP(E1)
              case 1: TMC_SET_PWMTHRS_E(1); break;
            #endif
            #if E_STEPPERS > 2 && AXIS_HAS_STEALTHCHOP(E2)
              case 2: TMC_SET_PWMTHRS_E(2); break;
            #endif
            #if E_STEPPERS > 3 && AXIS_HAS_STEALTHCHOP(E3)
              case 3: TMC_SET_PWMTHRS_E(3); break;
            #endif
            #if E_STEPPERS > 4 && AXIS_HAS_STEALTHCHOP(E4)
              case 4: TMC_SET_PWMTHRS_E(4); break;
            #endif
          }
        } break;
      }
    }

    if (report) {
      #if AXIS_HAS_STEALTHCHOP(X)
        TMC_SAY_PWMTHRS(X,X);
      #endif
      #if AXIS_HAS_STEALTHCHOP(X2)
        TMC_SAY_PWMTHRS(X,X2);
      #endif
      #if AXIS_HAS_STEALTHCHOP(Y)
        TMC_SAY_PWMTHRS(Y,Y);
      #endif
      #if AXIS_HAS_STEALTHCHOP(Y2)
        TMC_SAY_PWMTHRS(Y,Y2);
      #endif
      #if AXIS_HAS_STEALTHCHOP(Z)
        TMC_SAY_PWMTHRS(Z,Z);
      #endif
      #if AXIS_HAS_STEALTHCHOP(Z2)
        TMC_SAY_PWMTHRS(Z,Z2);
      #endif
      #if AXIS_HAS_STEALTHCHOP(E0)
        TMC_SAY_PWMTHRS_E(0);
      #endif
      #if E_STEPPERS > 1 && AXIS_HAS_STEALTHCHOP(E1)
        TMC_SAY_PWMTHRS_E(1);
      #endif
      #if E_STEPPERS > 2 && AXIS_HAS_STEALTHCHOP(E2)
        TMC_SAY_PWMTHRS_E(2);
      #endif
      #if E_STEPPERS > 3 && AXIS_HAS_STEALTHCHOP(E3)
        TMC_SAY_PWMTHRS_E(3);
      #endif
      #if E_STEPPERS > 4 && AXIS_HAS_STEALTHCHOP(E4)
        TMC_SAY_PWMTHRS_E(4);
      #endif
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
        #if X_SENSORLESS
          case X_AXIS:
            #if AXIS_HAS_STALLGUARD(X)
              if (index < 2) TMC_SET_SGT(X);
            #endif
            #if AXIS_HAS_STALLGUARD(X2)
              if (!(index & 1)) TMC_SET_SGT(X2);
            #endif
            break;
        #endif
        #if Y_SENSORLESS
          case Y_AXIS:
            #if AXIS_HAS_STALLGUARD(Y)
              if (index < 2) TMC_SET_SGT(Y);
            #endif
            #if AXIS_HAS_STALLGUARD(Y2)
              if (!(index & 1)) TMC_SET_SGT(Y2);
            #endif
            break;
        #endif
        #if Z_SENSORLESS
          case Z_AXIS:
            #if AXIS_HAS_STALLGUARD(Z)
              if (index < 2) TMC_SET_SGT(Z);
            #endif
            #if AXIS_HAS_STALLGUARD(Z2)
              if (!(index & 1)) TMC_SET_SGT(Z2);
            #endif
            break;
        #endif
      }
    }

    if (report) {
      #if X_SENSORLESS
        #if AXIS_HAS_STALLGUARD(X)
          TMC_SAY_SGT(X);
        #endif
        #if AXIS_HAS_STALLGUARD(X2)
          TMC_SAY_SGT(X2);
        #endif
      #endif
      #if Y_SENSORLESS
        #if AXIS_HAS_STALLGUARD(Y)
          TMC_SAY_SGT(Y);
        #endif
        #if AXIS_HAS_STALLGUARD(Y2)
          TMC_SAY_SGT(Y2);
        #endif
      #endif
      #if Z_SENSORLESS
        #if AXIS_HAS_STALLGUARD(Z)
          TMC_SAY_SGT(Z);
        #endif
        #if AXIS_HAS_STALLGUARD(Z2)
          TMC_SAY_SGT(Z2);
        #endif
      #endif
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

    if (!TEST(axis_known_position, Z_AXIS)) {
      SERIAL_ECHOLNPGM("\nPlease home Z axis first");
      return;
    }

    #if AXIS_IS_TMC(Z)
      const uint16_t Z_current_1 = stepperZ.getCurrent();
      stepperZ.setCurrent(_rms, R_SENSE, HOLD_MULTIPLIER);
    #endif
    #if AXIS_IS_TMC(Z2)
      const uint16_t Z2_current_1 = stepperZ2.getCurrent();
      stepperZ2.setCurrent(_rms, R_SENSE, HOLD_MULTIPLIER);
    #endif

    SERIAL_ECHOPAIR("\nCalibration current: Z", _rms);

    soft_endstops_enabled = false;

    do_blocking_move_to_z(Z_MAX_POS+_z);

    #if AXIS_IS_TMC(Z)
      stepperZ.setCurrent(Z_current_1, R_SENSE, HOLD_MULTIPLIER);
    #endif
    #if AXIS_IS_TMC(Z2)
      stepperZ2.setCurrent(Z2_current_1, R_SENSE, HOLD_MULTIPLIER);
    #endif

    do_blocking_move_to_z(Z_MAX_POS);
    soft_endstops_enabled = true;

    SERIAL_ECHOLNPGM("\nHoming Z because we lost steps");
    enqueue_and_echo_commands_P(PSTR("G28 Z"));
  }
#endif

#endif // HAS_TRINAMIC
