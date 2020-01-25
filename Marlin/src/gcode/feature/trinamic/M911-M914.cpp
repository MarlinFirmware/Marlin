/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "../../../inc/MarlinConfig.h"

#if HAS_TRINAMIC

#include "../../gcode.h"
#include "../../../feature/tmc_util.h"
#include "../../../module/stepper/indirection.h"
#include "../../../module/planner.h"
#include "../../queue.h"

#if ENABLED(MONITOR_DRIVER_STATUS)

  #define M91x_USE(ST) (AXIS_DRIVER_TYPE(ST, TMC2130) || AXIS_DRIVER_TYPE(ST, TMC2160) || AXIS_DRIVER_TYPE(ST, TMC2208) || AXIS_DRIVER_TYPE(ST, TMC2209) || AXIS_DRIVER_TYPE(ST, TMC2660) || AXIS_DRIVER_TYPE(ST, TMC5130) || AXIS_DRIVER_TYPE(ST, TMC5160))
  #define M91x_USE_E(N) (E_STEPPERS > N && M91x_USE(E##N))

  #define M91x_SOME_X (M91x_USE(X) || M91x_USE(X2))
  #define M91x_SOME_Y (M91x_USE(Y) || M91x_USE(Y2))
  #define M91x_SOME_Z (M91x_USE(Z) || M91x_USE(Z2) || M91x_USE(Z3) || M91x_USE(Z4))
  #define M91x_SOME_E (M91x_USE_E(0) || M91x_USE_E(1) || M91x_USE_E(2) || M91x_USE_E(3) || M91x_USE_E(4) || M91x_USE_E(5) || M91x_USE_E(6) || M91x_USE_E(7))

  #if !M91x_SOME_X && !M91x_SOME_Y && !M91x_SOME_Z && !M91x_SOME_E
    #error "MONITOR_DRIVER_STATUS requires at least one TMC2130, 2160, 2208, 2209, 2660, 5130, or 5160."
  #endif

  /**
   * M911: Report TMC stepper driver overtemperature pre-warn flag
   *       This flag is held by the library, persisting until cleared by M912
   */
  void GcodeSuite::M911() {
    #if M91x_USE(X)
      tmc_report_otpw(stepperX);
    #endif
    #if M91x_USE(X2)
      tmc_report_otpw(stepperX2);
    #endif
    #if M91x_USE(Y)
      tmc_report_otpw(stepperY);
    #endif
    #if M91x_USE(Y2)
      tmc_report_otpw(stepperY2);
    #endif
    #if M91x_USE(Z)
      tmc_report_otpw(stepperZ);
    #endif
    #if M91x_USE(Z2)
      tmc_report_otpw(stepperZ2);
    #endif
    #if M91x_USE(Z3)
      tmc_report_otpw(stepperZ3);
    #endif
    #if M91x_USE(Z4)
      tmc_report_otpw(stepperZ4);
    #endif
    #if M91x_USE_E(0)
      tmc_report_otpw(stepperE0);
    #endif
    #if M91x_USE_E(1)
      tmc_report_otpw(stepperE1);
    #endif
    #if M91x_USE_E(2)
      tmc_report_otpw(stepperE2);
    #endif
    #if M91x_USE_E(3)
      tmc_report_otpw(stepperE3);
    #endif
    #if M91x_USE_E(4)
      tmc_report_otpw(stepperE4);
    #endif
    #if M91x_USE_E(5)
      tmc_report_otpw(stepperE5);
    #endif
    #if M91x_USE_E(6)
      tmc_report_otpw(stepperE6);
    #endif
    #if M91x_USE_E(7)
      tmc_report_otpw(stepperE7);
    #endif
  }

  /**
   * M912: Clear TMC stepper driver overtemperature pre-warn flag held by the library
   *       Specify one or more axes with X, Y, Z, X1, Y1, Z1, X2, Y2, Z2, Z3, Z4 and E[index].
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
    #if M91x_SOME_X
      const bool hasX = parser.seen(axis_codes.x);
    #else
      constexpr bool hasX = false;
    #endif

    #if M91x_SOME_Y
      const bool hasY = parser.seen(axis_codes.y);
    #else
      constexpr bool hasY = false;
    #endif

    #if M91x_SOME_Z
      const bool hasZ = parser.seen(axis_codes.z);
    #else
      constexpr bool hasZ = false;
    #endif

    #if M91x_SOME_E
      const bool hasE = parser.seen(axis_codes.e);
    #else
      constexpr bool hasE = false;
    #endif

    const bool hasNone = !hasX && !hasY && !hasZ && !hasE;

    #if M91x_SOME_X
      const int8_t xval = int8_t(parser.byteval(axis_codes.x, 0xFF));
      #if M91x_USE(X)
        if (hasNone || xval == 1 || (hasX && xval < 0)) tmc_clear_otpw(stepperX);
      #endif
      #if M91x_USE(X2)
        if (hasNone || xval == 2 || (hasX && xval < 0)) tmc_clear_otpw(stepperX2);
      #endif
    #endif

    #if M91x_SOME_Y
      const int8_t yval = int8_t(parser.byteval(axis_codes.y, 0xFF));
      #if M91x_USE(Y)
        if (hasNone || yval == 1 || (hasY && yval < 0)) tmc_clear_otpw(stepperY);
      #endif
      #if M91x_USE(Y2)
        if (hasNone || yval == 2 || (hasY && yval < 0)) tmc_clear_otpw(stepperY2);
      #endif
    #endif

    #if M91x_SOME_Z
      const int8_t zval = int8_t(parser.byteval(axis_codes.z, 0xFF));
      #if M91x_USE(Z)
        if (hasNone || zval == 1 || (hasZ && zval < 0)) tmc_clear_otpw(stepperZ);
      #endif
      #if M91x_USE(Z2)
        if (hasNone || zval == 2 || (hasZ && zval < 0)) tmc_clear_otpw(stepperZ2);
      #endif
      #if M91x_USE(Z3)
        if (hasNone || zval == 3 || (hasZ && zval < 0)) tmc_clear_otpw(stepperZ3);
      #endif
      #if M91x_USE(Z4)
        if (hasNone || zval == 4 || (hasZ && zval < 0)) tmc_clear_otpw(stepperZ4);
      #endif
    #endif

    #if M91x_SOME_E
      const int8_t eval = int8_t(parser.byteval(axis_codes.e, 0xFF));
      #if M91x_USE_E(0)
        if (hasNone || eval == 0 || (hasE && eval < 0)) tmc_clear_otpw(stepperE0);
      #endif
      #if M91x_USE_E(1)
        if (hasNone || eval == 1 || (hasE && eval < 0)) tmc_clear_otpw(stepperE1);
      #endif
      #if M91x_USE_E(2)
        if (hasNone || eval == 2 || (hasE && eval < 0)) tmc_clear_otpw(stepperE2);
      #endif
      #if M91x_USE_E(3)
        if (hasNone || eval == 3 || (hasE && eval < 0)) tmc_clear_otpw(stepperE3);
      #endif
      #if M91x_USE_E(4)
        if (hasNone || eval == 4 || (hasE && eval < 0)) tmc_clear_otpw(stepperE4);
      #endif
      #if M91x_USE_E(5)
        if (hasNone || eval == 5 || (hasE && eval < 0)) tmc_clear_otpw(stepperE5);
      #endif
      #if M91x_USE_E(6)
        if (hasNone || eval == 6 || (hasE && eval < 0)) tmc_clear_otpw(stepperE6);
      #endif
      #if M91x_USE_E(7)
        if (hasNone || eval == 7 || (hasE && eval < 0)) tmc_clear_otpw(stepperE7);
      #endif
    #endif
  }

#endif // MONITOR_DRIVER_STATUS

/**
 * M913: Set HYBRID_THRESHOLD speed.
 */
#if ENABLED(HYBRID_THRESHOLD)
  void GcodeSuite::M913() {
    #define TMC_SAY_PWMTHRS(A,Q) tmc_print_pwmthrs(stepper##Q)
    #define TMC_SET_PWMTHRS(A,Q) stepper##Q.set_pwm_thrs(value)
    #define TMC_SAY_PWMTHRS_E(E) tmc_print_pwmthrs(stepperE##E)
    #define TMC_SET_PWMTHRS_E(E) stepperE##E.set_pwm_thrs(value)

    bool report = true;
    #if AXIS_IS_TMC(X) || AXIS_IS_TMC(X2) || AXIS_IS_TMC(Y) || AXIS_IS_TMC(Y2) || AXIS_IS_TMC(Z) || AXIS_IS_TMC(Z2) || AXIS_IS_TMC(Z3) || AXIS_IS_TMC(Z4)
      const uint8_t index = parser.byteval('I');
    #endif
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
            if (index == 0 || index == 2) TMC_SET_PWMTHRS(Z,Z2);
          #endif
          #if AXIS_HAS_STEALTHCHOP(Z3)
            if (index == 0 || index == 3) TMC_SET_PWMTHRS(Z,Z3);
          #endif
          #if AXIS_HAS_STEALTHCHOP(Z4)
            if (index == 0 || index == 4) TMC_SET_PWMTHRS(Z,Z4);
          #endif
          break;
        case E_AXIS: {
          #if E_STEPPERS
            const int8_t target_extruder = get_target_extruder_from_command();
            if (target_extruder < 0) return;
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
              #if E_STEPPERS > 5 && AXIS_HAS_STEALTHCHOP(E5)
                case 5: TMC_SET_PWMTHRS_E(5); break;
              #endif
              #if E_STEPPERS > 6 && AXIS_HAS_STEALTHCHOP(E6)
                case 6: TMC_SET_PWMTHRS_E(6); break;
              #endif
              #if E_STEPPERS > 7 && AXIS_HAS_STEALTHCHOP(E7)
                case 7: TMC_SET_PWMTHRS_E(7); break;
              #endif
            }
          #endif // E_STEPPERS
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
      #if AXIS_HAS_STEALTHCHOP(Z3)
        TMC_SAY_PWMTHRS(Z,Z3);
      #endif
      #if AXIS_HAS_STEALTHCHOP(Z4)
        TMC_SAY_PWMTHRS(Z,Z4);
      #endif
      #if E_STEPPERS && AXIS_HAS_STEALTHCHOP(E0)
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
      #if E_STEPPERS > 5 && AXIS_HAS_STEALTHCHOP(E5)
        TMC_SAY_PWMTHRS_E(5);
      #endif
      #if E_STEPPERS > 6 && AXIS_HAS_STEALTHCHOP(E6)
        TMC_SAY_PWMTHRS_E(6);
      #endif
      #if E_STEPPERS > 7 && AXIS_HAS_STEALTHCHOP(E7)
        TMC_SAY_PWMTHRS_E(7);
      #endif
    }
  }
#endif // HYBRID_THRESHOLD

/**
 * M914: Set StallGuard sensitivity.
 */
#if USE_SENSORLESS
  void GcodeSuite::M914() {

    bool report = true;
    const uint8_t index = parser.byteval('I');
    LOOP_XYZ(i) if (parser.seen(axis_codes[i])) {
      const int16_t value = parser.value_int();
      report = false;
      switch (i) {
        #if X_SENSORLESS
          case X_AXIS:
            #if AXIS_HAS_STALLGUARD(X)
              if (index < 2) stepperX.homing_threshold(value);
            #endif
            #if AXIS_HAS_STALLGUARD(X2)
              if (!(index & 1)) stepperX2.homing_threshold(value);
            #endif
            break;
        #endif
        #if Y_SENSORLESS
          case Y_AXIS:
            #if AXIS_HAS_STALLGUARD(Y)
              if (index < 2) stepperY.homing_threshold(value);
            #endif
            #if AXIS_HAS_STALLGUARD(Y2)
              if (!(index & 1)) stepperY2.homing_threshold(value);
            #endif
            break;
        #endif
        #if Z_SENSORLESS
          case Z_AXIS:
            #if AXIS_HAS_STALLGUARD(Z)
              if (index < 2) stepperZ.homing_threshold(value);
            #endif
            #if AXIS_HAS_STALLGUARD(Z2)
              if (index == 0 || index == 2) stepperZ2.homing_threshold(value);
            #endif
            #if AXIS_HAS_STALLGUARD(Z3)
              if (index == 0 || index == 3) stepperZ3.homing_threshold(value);
            #endif
            #if AXIS_HAS_STALLGUARD(Z4)
              if (index == 0 || index == 4) stepperZ4.homing_threshold(value);
            #endif
            break;
        #endif
      }
    }

    if (report) {
      #if X_SENSORLESS
        #if AXIS_HAS_STALLGUARD(X)
          tmc_print_sgt(stepperX);
        #endif
        #if AXIS_HAS_STALLGUARD(X2)
          tmc_print_sgt(stepperX2);
        #endif
      #endif
      #if Y_SENSORLESS
        #if AXIS_HAS_STALLGUARD(Y)
          tmc_print_sgt(stepperY);
        #endif
        #if AXIS_HAS_STALLGUARD(Y2)
          tmc_print_sgt(stepperY2);
        #endif
      #endif
      #if Z_SENSORLESS
        #if AXIS_HAS_STALLGUARD(Z)
          tmc_print_sgt(stepperZ);
        #endif
        #if AXIS_HAS_STALLGUARD(Z2)
          tmc_print_sgt(stepperZ2);
        #endif
        #if AXIS_HAS_STALLGUARD(Z3)
          tmc_print_sgt(stepperZ3);
        #endif
        #if AXIS_HAS_STALLGUARD(Z4)
          tmc_print_sgt(stepperZ4);
        #endif
      #endif
    }
  }
#endif // USE_SENSORLESS

#endif // HAS_TRINAMIC
