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

/**
  stepper_indirection.c - stepper motor driver indirection
  to allow some stepper functions to be done via SPI/I2c instead of direct pin manipulation
  Part of Marlin

  Copyright (c) 2015 Dominik Wenger

  Marlin is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Marlin is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Marlin.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "stepper_indirection.h"

#include "MarlinConfig.h"

#if ENABLED(HAVE_TMCDRIVER)
  #include <SPI.h>
  #include <TMC26XStepper.h>
#endif

// Stepper objects of TMC steppers used
#if ENABLED(X_IS_TMC)
  TMC26XStepper stepperX(200, X_ENABLE_PIN, X_STEP_PIN, X_DIR_PIN, X_MAX_CURRENT, X_SENSE_RESISTOR);
#endif
#if ENABLED(X2_IS_TMC)
  TMC26XStepper stepperX2(200, X2_ENABLE_PIN, X2_STEP_PIN, X2_DIR_PIN, X2_MAX_CURRENT, X2_SENSE_RESISTOR);
#endif
#if ENABLED(Y_IS_TMC)
  TMC26XStepper stepperY(200, Y_ENABLE_PIN, Y_STEP_PIN, Y_DIR_PIN, Y_MAX_CURRENT, Y_SENSE_RESISTOR);
#endif
#if ENABLED(Y2_IS_TMC)
  TMC26XStepper stepperY2(200, Y2_ENABLE_PIN, Y2_STEP_PIN, Y2_DIR_PIN, Y2_MAX_CURRENT, Y2_SENSE_RESISTOR);
#endif
#if ENABLED(Z_IS_TMC)
  TMC26XStepper stepperZ(200, Z_ENABLE_PIN, Z_STEP_PIN, Z_DIR_PIN, Z_MAX_CURRENT, Z_SENSE_RESISTOR);
#endif
#if ENABLED(Z2_IS_TMC)
  TMC26XStepper stepperZ2(200, Z2_ENABLE_PIN, Z2_STEP_PIN, Z2_DIR_PIN, Z2_MAX_CURRENT, Z2_SENSE_RESISTOR);
#endif
#if ENABLED(E0_IS_TMC)
  TMC26XStepper stepperE0(200, E0_ENABLE_PIN, E0_STEP_PIN, E0_DIR_PIN, E0_MAX_CURRENT, E0_SENSE_RESISTOR);
#endif
#if ENABLED(E1_IS_TMC)
  TMC26XStepper stepperE1(200, E1_ENABLE_PIN, E1_STEP_PIN, E1_DIR_PIN, E1_MAX_CURRENT, E1_SENSE_RESISTOR);
#endif
#if ENABLED(E2_IS_TMC)
  TMC26XStepper stepperE2(200, E2_ENABLE_PIN, E2_STEP_PIN, E2_DIR_PIN, E2_MAX_CURRENT, E2_SENSE_RESISTOR);
#endif
#if ENABLED(E3_IS_TMC)
  TMC26XStepper stepperE3(200, E3_ENABLE_PIN, E3_STEP_PIN, E3_DIR_PIN, E3_MAX_CURRENT, E3_SENSE_RESISTOR);
#endif

#if ENABLED(HAVE_TMCDRIVER)
void tmc_init() {
  #if ENABLED(X_IS_TMC)
    stepperX.setMicrosteps(X_MICROSTEPS);
    stepperX.start();
  #endif
  #if ENABLED(X2_IS_TMC)
    stepperX2.setMicrosteps(X2_MICROSTEPS);
    stepperX2.start();
  #endif
  #if ENABLED(Y_IS_TMC)
    stepperY.setMicrosteps(Y_MICROSTEPS);
    stepperY.start();
  #endif
  #if ENABLED(Y2_IS_TMC)
    stepperY2.setMicrosteps(Y2_MICROSTEPS);
    stepperY2.start();
  #endif
  #if ENABLED(Z_IS_TMC)
    stepperZ.setMicrosteps(Z_MICROSTEPS);
    stepperZ.start();
  #endif
  #if ENABLED(Z2_IS_TMC)
    stepperZ2.setMicrosteps(Z2_MICROSTEPS);
    stepperZ2.start();
  #endif
  #if ENABLED(E0_IS_TMC)
    stepperE0.setMicrosteps(E0_MICROSTEPS);
    stepperE0.start();
  #endif
  #if ENABLED(E1_IS_TMC)
    stepperE1.setMicrosteps(E1_MICROSTEPS);
    stepperE1.start();
  #endif
  #if ENABLED(E2_IS_TMC)
    stepperE2.setMicrosteps(E2_MICROSTEPS);
    stepperE2.start();
  #endif
  #if ENABLED(E3_IS_TMC)
    stepperE3.setMicrosteps(E3_MICROSTEPS);
    stepperE3.start();
  #endif
}
#endif

#if ENABLED(HAVE_TMC2130DRIVER)

  #include <SPI.h>
  #include <Trinamic_TMC2130.h>

  #if ENABLED(TMC2130_ADVANCED_CONFIGURATION)

    #ifdef GLOBAL_I_SCALE_ANALOG
      #define _2130_set_I_scale_analog(A) stepper##A.set_I_scale_analog(GLOBAL_I_SCALE_ANALOG)
    #else
      #define _2130_set_I_scale_analog(A) NOOP
    #endif
    #ifdef GLOBAL_INTERNAL_RSENSE
      #define _2130_set_internal_Rsense(A) stepper##A.set_internal_Rsense(GLOBAL_INTERNAL_RSENSE)
    #else
      #define _2130_set_internal_Rsense(A) NOOP
    #endif
    #ifdef GLOBAL_EN_PWM_MODE
      #define _2130_set_en_pwm_mode(A) stepper##A.set_en_pwm_mode(GLOBAL_EN_PWM_MODE)
    #else
      #define _2130_set_en_pwm_mode(A) NOOP
    #endif
    #ifdef GLOBAL_ENC_COMMUTATION
      #define _2130_set_enc_commutation(A) stepper##A.set_enc_commutation(GLOBAL_ENC_COMMUTATION)
    #else
      #define _2130_set_enc_commutation(A) NOOP
    #endif
    #ifdef GLOBAL_SHAFT
      #define _2130_set_shaft(A) stepper##A.set_shaft(GLOBAL_SHAFT)
    #else
      #define _2130_set_shaft(A) NOOP
    #endif
    #ifdef GLOBAL_DIAG0_ERROR
      #define _2130_set_diag0_error(A) stepper##A.set_diag0_error(GLOBAL_DIAG0_ERROR)
    #else
      #define _2130_set_diag0_error(A) NOOP
    #endif
    #ifdef GLOBAL_DIAG0_OTPW
      #define _2130_set_diag0_otpw(A) stepper##A.set_diag0_otpw(GLOBAL_DIAG0_OTPW)
    #else
      #define _2130_set_diag0_otpw(A) NOOP
    #endif
    #ifdef GLOBAL_DIAG0_STALL
      #define _2130_set_diag0_stall(A) stepper##A.set_diag0_stall(GLOBAL_DIAG0_STALL)
    #else
      #define _2130_set_diag0_stall(A) NOOP
    #endif
    #ifdef GLOBAL_DIAG1_STALL
      #define _2130_set_diag1_stall(A) stepper##A.set_diag1_stall(GLOBAL_DIAG1_STALL)
    #else
      #define _2130_set_diag1_stall(A) NOOP
    #endif
    #ifdef GLOBAL_DIAG1_INDEX
      #define _2130_set_diag1_index(A) stepper##A.set_diag1_index(GLOBAL_DIAG1_INDEX)
    #else
      #define _2130_set_diag1_index(A) NOOP
    #endif
    #ifdef GLOBAL_DIAG1_ONSTATE
      #define _2130_set_diag1_onstate(A) stepper##A.set_diag1_onstate(GLOBAL_DIAG1_ONSTATE)
    #else
      #define _2130_set_diag1_onstate(A) NOOP
    #endif
    #ifdef GLOBAL_DIAG1_ONSTATE
      #define _2130_set_diag1_steps_skipped(A) stepper##A.set_diag1_steps_skipped(GLOBAL_DIAG1_ONSTATE)
    #else
      #define _2130_set_diag1_steps_skipped(A) NOOP
    #endif
    #ifdef GLOBAL_DIAG0_INT_PUSHPULL
      #define _2130_set_diag0_int_pushpull(A) stepper##A.set_diag0_int_pushpull(GLOBAL_DIAG0_INT_PUSHPULL)
    #else
      #define _2130_set_diag0_int_pushpull(A) NOOP
    #endif
    #ifdef GLOBAL_DIAG1_INT_PUSHPULL
      #define _2130_set_diag1_int_pushpull(A) stepper##A.set_diag1_int_pushpull(GLOBAL_DIAG1_INT_PUSHPULL)
    #else
      #define _2130_set_diag1_int_pushpull(A) NOOP
    #endif
    #ifdef GLOBAL_SMALL_HYSTERESIS
      #define _2130_set_small_hysteresis(A) stepper##A.set_small_hysteresis(GLOBAL_SMALL_HYSTERESIS)
    #else
      #define _2130_set_small_hysteresis(A) NOOP
    #endif
    #ifdef GLOBAL_STOP_ENABLE
      #define _2130_set_stop_enable(A) stepper##A.set_stop_enable(GLOBAL_STOP_ENABLE)
    #else
      #define _2130_set_stop_enable(A) NOOP
    #endif
    #ifdef GLOBAL_DIRECT_MODE
      #define _2130_set_direct_mode(A) stepper##A.set_direct_mode(GLOBAL_DIRECT_MODE)
    #else
      #define _2130_set_direct_mode(A) NOOP
    #endif
    #if defined(GLOBAL_IHOLD) && defined(GLOBAL_IRUN) && defined(GLOBAL_IHOLDDELAY)
      #define _2130_set_IHOLD_IRUN(A) stepper##A.set_IHOLD_IRUN(GLOBAL_IHOLD, GLOBAL_IRUN, GLOBAL_IHOLDDELAY)
    #else
      #define _2130_set_IHOLD_IRUN(A) NOOP
    #endif
    #ifdef GLOBAL_TPOWERDOWN
      #define _2130_set_TPOWERDOWN(A) stepper##A.set_TPOWERDOWN(GLOBAL_TPOWERDOWN)
    #else
      #define _2130_set_TPOWERDOWN(A) NOOP
    #endif
    #ifdef GLOBAL_TPWMTHRS
      #define _2130_set_TPWMTHRS(A) stepper##A.set_TPWMTHRS(GLOBAL_TPWMTHRS)
    #else
      #define _2130_set_TPWMTHRS(A) NOOP
    #endif
    #ifdef GLOBAL_TCOOLTHRS
      #define _2130_set_TCOOLTHRS(A) stepper##A.set_TCOOLTHRS(GLOBAL_TCOOLTHRS)
    #else
      #define _2130_set_TCOOLTHRS(A) NOOP
    #endif
    #ifdef GLOBAL_THIGH
      #define _2130_set_THIGH(A) stepper##A.set_THIGH(GLOBAL_THIGH)
    #else
      #define _2130_set_THIGH(A) NOOP
    #endif
    #ifdef GLOBAL_XDIRECT
      #define _2130_set_XDIRECT(A) stepper##A.set_XDIRECT(GLOBAL_XDIRECT)
    #else
      #define _2130_set_XDIRECT(A) NOOP
    #endif
    #ifdef GLOBAL_VDCMIN
      #define _2130_set_VDCMIN(A) stepper##A.set_VDCMIN(GLOBAL_VDCMIN)
    #else
      #define _2130_set_VDCMIN(A) NOOP
    #endif
    #ifdef GLOBAL_DEDGE
      #define _2130_set_dedge(A) stepper##A.set_dedge(GLOBAL_DEDGE)
    #else
      #define _2130_set_dedge(A) NOOP
    #endif
    #ifdef GLOBAL_DISS2G
      #define _2130_set_diss2g(A) stepper##A.set_diss2g(GLOBAL_DISS2G)
    #else
      #define _2130_set_diss2g(A) NOOP
    #endif
    #ifdef GLOBAL_INTPOL
      #define _2130_set_intpol(A) stepper##A.set_intpol(GLOBAL_INTPOL)
    #else
      #define _2130_set_intpol(A) NOOP
    #endif
    #ifdef GLOBAL_MRES
      #define _2130_set_mres(A) stepper##A.set_mres(GLOBAL_MRES)
    #else
      #define _2130_set_mres(A) NOOP
    #endif
    #ifdef GLOBAL_SYNC
      #define _2130_set_sync(A) stepper##A.set_sync(GLOBAL_SYNC)
    #else
      #define _2130_set_sync(A) NOOP
    #endif
    #ifdef GLOBAL_VHIGHCHM
      #define _2130_set_vhighchm(A) stepper##A.set_vhighchm(GLOBAL_VHIGHCHM)
    #else
      #define _2130_set_vhighchm(A) NOOP
    #endif
    #ifdef GLOBAL_VHIGHFS
      #define _2130_set_vhighfs(A) stepper##A.set_vhighfs(GLOBAL_VHIGHFS)
    #else
      #define _2130_set_vhighfs(A) NOOP
    #endif
    #ifdef GLOBAL_VSENSE
      #define _2130_set_vsense(A) stepper##A.set_vsense(GLOBAL_VSENSE)
    #else
      #define _2130_set_vsense(A) NOOP
    #endif
    #ifdef GLOBAL_TBL
      #define _2130_set_tbl(A) stepper##A.set_tbl(GLOBAL_TBL)
    #else
      #define _2130_set_tbl(A) NOOP
    #endif
    #ifdef GLOBAL_CHM
      #define _2130_set_chm(A) stepper##A.set_chm(GLOBAL_CHM)
    #else
      #define _2130_set_chm(A) NOOP
    #endif
    #ifdef GLOBAL_RNDTF
      #define _2130_set_rndtf(A) stepper##A.set_rndtf(GLOBAL_RNDTF)
    #else
      #define _2130_set_rndtf(A) NOOP
    #endif
    #ifdef GLOBAL_DISFDCC
      #define _2130_set_disfdcc(A) stepper##A.set_disfdcc(GLOBAL_DISFDCC)
    #else
      #define _2130_set_disfdcc(A) NOOP
    #endif
    #ifdef GLOBAL_FD
      #define _2130_set_fd(A) stepper##A.set_fd(GLOBAL_FD)
    #else
      #define _2130_set_fd(A) NOOP
    #endif
    #ifdef GLOBAL_HEND
      #define _2130_set_hend(A) stepper##A.set_hend(GLOBAL_HEND)
    #else
      #define _2130_set_hend(A) NOOP
    #endif
    #ifdef GLOBAL_HSTRT
      #define _2130_set_hstrt(A) stepper##A.set_hstrt(GLOBAL_HSTRT)
    #else
      #define _2130_set_hstrt(A) NOOP
    #endif
    #ifdef GLOBAL_TOFF
      #define _2130_set_toff(A) stepper##A.set_toff(GLOBAL_TOFF)
    #else
      #define _2130_set_toff(A) NOOP
    #endif
    #ifdef GLOBAL_SFILT
      #define _2130_set_sfilt(A) stepper##A.set_sfilt(GLOBAL_SFILT)
    #else
      #define _2130_set_sfilt(A) NOOP
    #endif
    #ifdef GLOBAL_SGT
      #define _2130_set_sgt(A) stepper##A.set_sgt(GLOBAL_SGT)
    #else
      #define _2130_set_sgt(A) NOOP
    #endif
    #ifdef GLOBAL_SEIMIN
      #define _2130_set_seimin(A) stepper##A.set_seimin(GLOBAL_SEIMIN)
    #else
      #define _2130_set_seimin(A) NOOP
    #endif
    #ifdef GLOBAL_SEDN
      #define _2130_set_sedn(A) stepper##A.set_sedn(GLOBAL_SEDN)
    #else
      #define _2130_set_sedn(A) NOOP
    #endif
    #ifdef GLOBAL_SEMAX
      #define _2130_set_semax(A) stepper##A.set_semax(GLOBAL_SEMAX)
    #else
      #define _2130_set_semax(A) NOOP
    #endif
    #ifdef GLOBAL_SEUP
      #define _2130_set_seup(A) stepper##A.set_seup(GLOBAL_SEUP)
    #else
      #define _2130_set_seup(A) NOOP
    #endif
    #ifdef GLOBAL_SEMIN
      #define _2130_set_semin(A) stepper##A.set_semin(GLOBAL_SEMIN)
    #else
      #define _2130_set_semin(A) NOOP
    #endif
    #if defined(GLOBAL_DC_TIME) && defined(GLOBAL_DC_SG)
      #define _2130_set_DCCTRL(A) stepper##A.set_DCCTRL(GLOBAL_DC_TIME, GLOBAL_DC_SG)
    #else
      #define _2130_set_DCCTRL(A) NOOP
    #endif
    #ifdef GLOBAL_FREEWHEEL
      #define _2130_set_freewheel(A) stepper##A.set_freewheel(GLOBAL_FREEWHEEL)
    #else
      #define _2130_set_freewheel(A) NOOP
    #endif
    #ifdef GLOBAL_PWM_SYMMETRIC
      #define _2130_set_pwm_symmetric(A) stepper##A.set_pwm_symmetric(GLOBAL_PWM_SYMMETRIC)
    #else
      #define _2130_set_pwm_symmetric(A) NOOP
    #endif
    #ifdef GLOBAL_PWM_AUTOSCALE
      #define _2130_set_pwm_autoscale(A) stepper##A.set_pwm_autoscale(GLOBAL_PWM_AUTOSCALE)
    #else
      #define _2130_set_pwm_autoscale(A) NOOP
    #endif
    #ifdef GLOBAL_PWM_FREQ
      #define _2130_set_pwm_freq(A) stepper##A.set_pwm_freq(GLOBAL_PWM_FREQ)
    #else
      #define _2130_set_pwm_freq(A) NOOP
    #endif
    #ifdef GLOBAL_PWM_GRAD
      #define _2130_set_PWM_GRAD(A) stepper##A.set_PWM_GRAD(GLOBAL_PWM_GRAD)
    #else
      #define _2130_set_PWM_GRAD(A) NOOP
    #endif
    #ifdef GLOBAL_PWM_AMPL
      #define _2130_set_PWM_AMPL(A) stepper##A.set_PWM_AMPL(GLOBAL_PWM_AMPL)
    #else
      #define _2130_set_PWM_AMPL(A) NOOP
    #endif
    #ifdef GLOBAL_ENCM_CTRL
      #define _2130_set_ENCM_CTRL(A) stepper##A.set_ENCM_CTRL(GLOBAL_ENCM_CTRL)
    #else
      #define _2130_set_ENCM_CTRL(A) NOOP
    #endif

    #define _TMC2130_INIT(A) do{ \
      stepper##A.init(); \
      _2130_set_I_scale_analog(A); \
      _2130_set_internal_Rsense(A); \
      _2130_set_en_pwm_mode(A); \
      _2130_set_enc_commutation(A); \
      _2130_set_shaft(A); \
      _2130_set_diag0_error(A); \
      _2130_set_diag0_otpw(A); \
      _2130_set_diag0_stall(A); \
      _2130_set_diag1_stall(A); \
      _2130_set_diag1_index(A); \
      _2130_set_diag1_onstate(A); \
      _2130_set_diag1_steps_skipped(A); \
      _2130_set_diag0_int_pushpull(A); \
      _2130_set_diag1_int_pushpull(A); \
      _2130_set_small_hysteresis(A); \
      _2130_set_stop_enable(A); \
      _2130_set_direct_mode(A); \
      _2130_set_IHOLD_IRUN(A); \
      _2130_set_TPOWERDOWN(A); \
      _2130_set_TPWMTHRS(A); \
      _2130_set_TCOOLTHRS(A); \
      _2130_set_THIGH(A); \
      _2130_set_XDIRECT(A); \
      _2130_set_VDCMIN(A); \
      _2130_set_dedge(A); \
      _2130_set_diss2g(A); \
      _2130_set_intpol(A); \
      _2130_set_mres(A); \
      _2130_set_sync(A); \
      _2130_set_vhighchm(A); \
      _2130_set_vhighfs(A); \
      _2130_set_vsense(A); \
      _2130_set_tbl(A); \
      _2130_set_chm(A); \
      _2130_set_rndtf(A); \
      _2130_set_disfdcc(A); \
      _2130_set_fd(A); \
      _2130_set_hend(A); \
      _2130_set_hstrt(A); \
      _2130_set_toff(A); \
      _2130_set_sfilt(A); \
      _2130_set_sgt(A); \
      _2130_set_seimin(A); \
      _2130_set_sedn(A); \
      _2130_set_semax(A); \
      _2130_set_seup(A); \
      _2130_set_semin(A); \
      _2130_set_DCCTRL(A); \
      _2130_set_freewheel(A); \
      _2130_set_pwm_symmetric(A); \
      _2130_set_pwm_autoscale(A); \
      _2130_set_pwm_freq(A); \
      _2130_set_PWM_GRAD(A); \
      _2130_set_PWM_AMPL(A); \
      _2130_set_ENCM_CTRL(A); \
    } while(0)

  #else // !TMC2130_ADVANCED_CONFIGURATION

    #define _TMC2130_INIT(A) do{ \
      stepper##A.init(); \
      stepper##A.set_mres(A##_MRES); \
      stepper##A.set_IHOLD_IRUN(A##_IHOLD, A##_IRUN, A##_IHOLDDELAY); \
      stepper##A.set_I_scale_analog(A##_I_SCALE_ANALOG); \
      stepper##A.set_tbl(A##_TBL); \
      stepper##A.set_toff(A##_TOFF); \
    } while(0)

  #endif // TMC2130_ADVANCED_CONFIGURATION

  // Stepper objects of TMC2310 steppers used
  #if ENABLED(X_IS_TMC2130)
    Trinamic_TMC2130 stepperX(X_CS_PIN);
  #endif
  #if ENABLED(X2_IS_TMC2130)
    Trinamic_TMC2130 stepperX2(X2_CS_PIN);
  #endif
  #if ENABLED(Y_IS_TMC2130)
    Trinamic_TMC2130 stepperY(Y_CS_PIN);
  #endif
  #if ENABLED(Y2_IS_TMC2130)
    Trinamic_TMC2130 stepperY2(Y2_CS_PINR);
  #endif
  #if ENABLED(Z_IS_TMC2130)
    Trinamic_TMC2130 stepperZ(Z_CS_PIN);
  #endif
  #if ENABLED(Z2_IS_TMC2130)
    Trinamic_TMC2130 stepperZ2(Z2_CS_PIN);
  #endif
  #if ENABLED(E0_IS_TMC2130)
    Trinamic_TMC2130 stepperE0(E0_CS_PIN);
  #endif
  #if ENABLED(E1_IS_TMC2130)
    Trinamic_TMC2130 stepperE1(E1_CS_PIN);
  #endif
  #if ENABLED(E2_IS_TMC2130)
    Trinamic_TMC2130 stepperE2(E2_CS_PIN);
  #endif
  #if ENABLED(E3_IS_TMC2130)
    Trinamic_TMC2130 stepperE3(E3_CS_PIN);
  #endif

  void tmc2130_init() {
    #if ENABLED(X_IS_TMC2130)
      _TMC2130_INIT(X);
    #endif
    #if ENABLED(X2_IS_TMC2130)
      _TMC2130_INIT(X2);
    #endif
    #if ENABLED(Y_IS_TMC2130)
      _TMC2130_INIT(Y);
    #endif
    #if ENABLED(Y2_IS_TMC2130)
      _TMC2130_INIT(Y2);
    #endif
    #if ENABLED(Z_IS_TMC2130)
      _TMC2130_INIT(Z);
    #endif
    #if ENABLED(Z2_IS_TMC2130)
      _TMC2130_INIT(Z2);
    #endif
    #if ENABLED(E0_IS_TMC2130)
      _TMC2130_INIT(E0);
    #endif
    #if ENABLED(E1_IS_TMC2130)
      _TMC2130_INIT(E1);
    #endif
    #if ENABLED(E2_IS_TMC2130)
      _TMC2130_INIT(E2);
    #endif
    #if ENABLED(E3_IS_TMC2130)
      _TMC2130_INIT(E3);
    #endif
  }

#endif // HAVE_TMC2130DRIVER

// L6470 Driver objects and inits

#if ENABLED(HAVE_L6470DRIVER)
  #include <SPI.h>
  #include <L6470.h>
#endif

// L6470 Stepper objects
#if ENABLED(X_IS_L6470)
  L6470 stepperX(X_ENABLE_PIN);
#endif
#if ENABLED(X2_IS_L6470)
  L6470 stepperX2(X2_ENABLE_PIN);
#endif
#if ENABLED(Y_IS_L6470)
  L6470 stepperY(Y_ENABLE_PIN);
#endif
#if ENABLED(Y2_IS_L6470)
  L6470 stepperY2(Y2_ENABLE_PIN);
#endif
#if ENABLED(Z_IS_L6470)
  L6470 stepperZ(Z_ENABLE_PIN);
#endif
#if ENABLED(Z2_IS_L6470)
  L6470 stepperZ2(Z2_ENABLE_PIN);
#endif
#if ENABLED(E0_IS_L6470)
  L6470 stepperE0(E0_ENABLE_PIN);
#endif
#if ENABLED(E1_IS_L6470)
  L6470 stepperE1(E1_ENABLE_PIN);
#endif
#if ENABLED(E2_IS_L6470)
  L6470 stepperE2(E2_ENABLE_PIN);
#endif
#if ENABLED(E3_IS_L6470)
  L6470 stepperE3(E3_ENABLE_PIN);
#endif


// init routine
#if ENABLED(HAVE_L6470DRIVER)
void L6470_init() {
  #if ENABLED(X_IS_L6470)
    stepperX.init(X_K_VAL);
    stepperX.softFree();
    stepperX.setMicroSteps(X_MICROSTEPS);
    stepperX.setOverCurrent(X_OVERCURRENT); //set overcurrent protection
    stepperX.setStallCurrent(X_STALLCURRENT);
  #endif
  #if ENABLED(X2_IS_L6470)
    stepperX2.init(X2_K_VAL);
    stepperX2.softFree();
    stepperX2.setMicroSteps(X2_MICROSTEPS);
    stepperX2.setOverCurrent(X2_OVERCURRENT); //set overcurrent protection
    stepperX2.setStallCurrent(X2_STALLCURRENT);
  #endif
  #if ENABLED(Y_IS_L6470)
    stepperY.init(Y_K_VAL);
    stepperY.softFree();
    stepperY.setMicroSteps(Y_MICROSTEPS);
    stepperY.setOverCurrent(Y_OVERCURRENT); //set overcurrent protection
    stepperY.setStallCurrent(Y_STALLCURRENT);
  #endif
  #if ENABLED(Y2_IS_L6470)
    stepperY2.init(Y2_K_VAL);
    stepperY2.softFree();
    stepperY2.setMicroSteps(Y2_MICROSTEPS);
    stepperY2.setOverCurrent(Y2_OVERCURRENT); //set overcurrent protection
    stepperY2.setStallCurrent(Y2_STALLCURRENT);
  #endif
  #if ENABLED(Z_IS_L6470)
    stepperZ.init(Z_K_VAL);
    stepperZ.softFree();
    stepperZ.setMicroSteps(Z_MICROSTEPS);
    stepperZ.setOverCurrent(Z_OVERCURRENT); //set overcurrent protection
    stepperZ.setStallCurrent(Z_STALLCURRENT);
  #endif
  #if ENABLED(Z2_IS_L6470)
    stepperZ2.init(Z2_K_VAL);
    stepperZ2.softFree();
    stepperZ2.setMicroSteps(Z2_MICROSTEPS);
    stepperZ2.setOverCurrent(Z2_OVERCURRENT); //set overcurrent protection
    stepperZ2.setStallCurrent(Z2_STALLCURRENT);
  #endif
  #if ENABLED(E0_IS_L6470)
    stepperE0.init(E0_K_VAL);
    stepperE0.softFree();
    stepperE0.setMicroSteps(E0_MICROSTEPS);
    stepperE0.setOverCurrent(E0_OVERCURRENT); //set overcurrent protection
    stepperE0.setStallCurrent(E0_STALLCURRENT);
  #endif
  #if ENABLED(E1_IS_L6470)
    stepperE1.init(E1_K_VAL);
    stepperE1.softFree();
    stepperE1.setMicroSteps(E1_MICROSTEPS);
    stepperE1.setOverCurrent(E1_OVERCURRENT); //set overcurrent protection
    stepperE1.setStallCurrent(E1_STALLCURRENT);
  #endif
  #if ENABLED(E2_IS_L6470)
    stepperE2.init(E2_K_VAL);
    stepperE2.softFree();
    stepperE2.setMicroSteps(E2_MICROSTEPS);
    stepperE2.setOverCurrent(E2_OVERCURRENT); //set overcurrent protection
    stepperE2.setStallCurrent(E2_STALLCURRENT);
  #endif
  #if ENABLED(E3_IS_L6470)
    stepperE3.init(E3_K_VAL);
    stepperE3.softFree();
    stepperE3.setMicroSteps(E3_MICROSTEPS);
    stepperE3.setOverCurrent(E3_OVERCURRENT); //set overcurrent protection
    stepperE3.setStallCurrent(E3_STALLCURRENT);
  #endif
}
#endif

