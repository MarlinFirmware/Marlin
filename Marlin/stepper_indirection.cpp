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
#endif

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

#if ENABLED(HAVE_TMC2130DRIVER)

  #if ENABLED(TMC2130_ADVANCED_CONFIGURATION)

    void tmc2130_init() {

      // If you've enabled TMC2130_ADVANCED_CONFIGURATION configure your
      // steppers manually here. The ENABLED(XYZ_IS_TMC2130) is optional,

      #if ENABLED(X_IS_TMC2130)
        stepperX.init();
        stepperX.set_I_scale_analog(GLOBAL_I_SCALE_ANALOG);
        //stepperX.set_internal_Rsense(GLOBAL_INTERNAL_RSENSE);
        stepperX.set_en_pwm_mode(GLOBAL_EN_PWM_MODE);
        //stepperX.set_enc_commutation(GLOBAL_ENC_COMMUTATION);
        stepperX.set_shaft(GLOBAL_SHAFT);
        //stepperX.set_diag0_error(GLOBAL_DIAG0_ERROR);
        //stepperX.set_diag0_otpw(GLOBAL_DIAG0_OTPW);
        //stepperX.set_diag0_stall(GLOBAL_DIAG0_STALL);
        //stepperX.set_diag1_stall(GLOBAL_DIAG1_STALL);
        //stepperX.set_diag1_index(GLOBAL_DIAG1_INDEX);
        //stepperX.set_diag1_onstate(GLOBAL_DIAG1_ONSTATE);
        //stepperX.set_diag1_steps_skipped(GLOBAL_DIAG1_ONSTATE);
        //stepperX.set_diag0_int_pushpull(GLOBAL_DIAG0_INT_PUSHPULL);
        //stepperX.set_diag1_int_pushpull(GLOBAL_DIAG1_INT_PUSHPULL);
        //stepperX.set_small_hysteresis(GLOBAL_SMALL_HYSTERESIS);
        //stepperX.set_stop_enable(GLOBAL_STOP_ENABLE);
        //stepperX.set_direct_mode(GLOBAL_DIRECT_MODE);

        stepperX.set_IHOLD_IRUN(GLOBAL_IHOLD,GLOBAL_IRUN,GLOBAL_IHOLDDELAY);
        //stepperX.set_TPOWERDOWN(GLOBAL_TPOWERDOWN);
        //stepperX.set_TPWMTHRS(GLOBAL_TPWMTHRS);
        //stepperX.set_TCOOLTHRS(GLOBAL_TCOOLTHRS);
        stepperX.set_THIGH(GLOBAL_THIGH);
        //stepperX.set_XDIRECT(GLOBAL_XDIRECT);
        //stepperX.set_VDCMIN(GLOBAL_VDCMIN);

        //stepperX.set_dedge(GLOBAL_DEDGE);
        //stepperX.set_diss2g(GLOBAL_DISS2G);
        stepperX.set_intpol(GLOBAL_INTPOL);
        stepperX.set_mres(GLOBAL_MRES);
        stepperX.set_sync(GLOBAL_SYNC);
        stepperX.set_vhighchm(GLOBAL_VHIGHCHM);
        stepperX.set_vhighfs(GLOBAL_VHIGHFS);
        //stepperX.set_vsense(GLOBAL_VSENSE);
        stepperX.set_tbl(GLOBAL_TBL);
        stepperX.set_chm(GLOBAL_CHM);
        //stepperX.set_rndtf(GLOBAL_RNDTF);
        //stepperX.set_disfdcc(GLOBAL_DISFDCC);
        //stepperX.set_fd(GLOBAL_FD);
        //stepperX.set_hend(GLOBAL_HEND);
        //stepperX.set_hstrt(GLOBAL_HSTRT);
        stepperX.set_toff(GLOBAL_TOFF);

        //stepperX.set_sfilt(GLOBAL_SFILT);
        //stepperX.set_sgt(GLOBAL_SGT);
        //stepperX.set_seimin(GLOBAL_SEIMIN);
        //stepperX.set_sedn(GLOBAL_SEDN);
        //stepperX.set_semax(GLOBAL_SEMAX);
        //stepperX.set_seup(GLOBAL_SEUP);
        //stepperX.set_semin(GLOBAL_SEMIN);

        //stepperX.set_DCCTRL(GLOBAL_DC_TIME, GLOBAL_DC_SG);

        //stepperX.set_freewheel(GLOBAL_FREEWHEEL);
        //stepperX.set_pwm_symmetric(GLOBAL_PWM_SYMMETRIC);
        //stepperX.set_pwm_autoscale(GLOBAL_PWM_AUTOSCALE);
        //stepperX.set_pwm_freq(GLOBAL_PWM_FREQ);
        //stepperX.set_PWM_GRAD(GLOBAL_PWM_GRAD);
        //stepperX.set_PWM_AMPL(GLOBAL_PWM_AMPL);

        //stepperX.set_ENCM_CTRL(GLOBAL_ENCM_CTRL);
      #endif

      #if ENABLED(Y_IS_TMC2130)
        stepperY.init();
        stepperY.set_I_scale_analog(GLOBAL_I_SCALE_ANALOG);
        //stepperY.set_internal_Rsense(GLOBAL_INTERNAL_RSENSE);
        stepperY.set_en_pwm_mode(GLOBAL_EN_PWM_MODE);
        //stepperY.set_enc_commutation(GLOBAL_ENC_COMMUTATION);
        stepperY.set_shaft(GLOBAL_SHAFT);
        //stepperY.set_diag0_error(GLOBAL_DIAG0_ERROR);
        //stepperY.set_diag0_otpw(GLOBAL_DIAG0_OTPW);
        //stepperY.set_diag0_stall(GLOBAL_DIAG0_STALL);
        //stepperY.set_diag1_stall(GLOBAL_DIAG1_STALL);
        //stepperY.set_diag1_index(GLOBAL_DIAG1_INDEX);
        //stepperY.set_diag1_onstate(GLOBAL_DIAG1_ONSTATE);
        //stepperY.set_diag1_steps_skipped(GLOBAL_DIAG1_ONSTATE);
        //stepperY.set_diag0_int_pushpull(GLOBAL_DIAG0_INT_PUSHPULL);
        //stepperY.set_diag1_int_pushpull(GLOBAL_DIAG1_INT_PUSHPULL);
        //stepperY.set_small_hysteresis(GLOBAL_SMALL_HYSTERESIS);
        //stepperY.set_stop_enable(GLOBAL_STOP_ENABLE);
        //stepperY.set_direct_mode(GLOBAL_DIRECT_MODE);

        stepperY.set_IHOLD_IRUN(GLOBAL_IHOLD,GLOBAL_IRUN,GLOBAL_IHOLDDELAY);
        //stepperY.set_TPOWERDOWN(GLOBAL_TPOWERDOWN);
        //stepperY.set_TPWMTHRS(GLOBAL_TPWMTHRS);
        //stepperY.set_TCOOLTHRS(GLOBAL_TCOOLTHRS);
        stepperY.set_THIGH(GLOBAL_THIGH);
        //stepperY.set_XDIRECT(GLOBAL_XDIRECT);
        //stepperY.set_VDCMIN(GLOBAL_VDCMIN);

        //stepperY.set_dedge(GLOBAL_DEDGE);
        //stepperY.set_diss2g(GLOBAL_DISS2G);
        stepperY.set_intpol(GLOBAL_INTPOL);
        stepperY.set_mres(GLOBAL_MRES);
        stepperY.set_sync(GLOBAL_SYNC);
        stepperY.set_vhighchm(GLOBAL_VHIGHCHM);
        stepperY.set_vhighfs(GLOBAL_VHIGHFS);
        //stepperY.set_vsense(GLOBAL_VSENSE);
        stepperY.set_tbl(GLOBAL_TBL);
        stepperY.set_chm(GLOBAL_CHM);
        //stepperY.set_rndtf(GLOBAL_RNDTF);
        //stepperY.set_disfdcc(GLOBAL_DISFDCC);
        //stepperY.set_fd(GLOBAL_FD);
        //stepperY.set_hend(GLOBAL_HEND);
        //stepperY.set_hstrt(GLOBAL_HSTRT);
        stepperY.set_toff(GLOBAL_TOFF);

        //stepperY.set_sfilt(GLOBAL_SFILT);
        //stepperY.set_sgt(GLOBAL_SGT);
        //stepperY.set_seimin(GLOBAL_SEIMIN);
        //stepperY.set_sedn(GLOBAL_SEDN);
        //stepperY.set_semax(GLOBAL_SEMAX);
        //stepperY.set_seup(GLOBAL_SEUP);
        //stepperY.set_semin(GLOBAL_SEMIN);

        //stepperY.set_DCCTRL(GLOBAL_DC_TIME, GLOBAL_DC_SG);

        //stepperY.set_freewheel(GLOBAL_FREEWHEEL);
        //stepperY.set_pwm_symmetric(GLOBAL_PWM_SYMMETRIC);
        //stepperY.set_pwm_autoscale(GLOBAL_PWM_AUTOSCALE);
        //stepperY.set_pwm_freq(GLOBAL_PWM_FREQ);
        //stepperY.set_PWM_GRAD(GLOBAL_PWM_GRAD);
        //stepperY.set_PWM_AMPL(GLOBAL_PWM_AMPL);

        //stepperY.set_ENCM_CTRL(GLOBAL_ENCM_CTRL);
      #endif
    }

  #else // !TMC2130_ADVANCED_CONFIGURATION

    void tmc2130_init() {
      #if ENABLED(X_IS_TMC2130)
        stepperX.init();
        stepperX.set_mres(X_MRES);
        stepperX.set_IHOLD_IRUN(X_IHOLD,X_IRUN,X_IHOLDDELAY);
        stepperX.set_I_scale_analog(X_ISCALE);
        stepperX.set_tbl(X_TBL);
        stepperX.set_toff(X_TOFF);
      #endif
      #if ENABLED(X2_IS_TMC2130)
        stepperX2.init();
        stepperX2.set_mres(X2_MRES);
        stepperX2.set_IHOLD_IRUN(X2_IHOLD,X2_IRUN,X2_IHOLDDELAY);
        stepperX2.set_I_scale_analog(X2_ISCALE);
        stepperX2.set_tbl(X2_TBL);
        stepperX2.set_toff(X2_TOFF);
      #endif
      #if ENABLED(Y_IS_TMC2130)
        stepperY.init();
        stepperY.set_mres(Y_MRES);
        stepperY.set_IHOLD_IRUN(Y_IHOLD,Y_IRUN,Y_IHOLDDELAY);
        stepperY.set_I_scale_analog(Y_ISCALE);
        stepperY.set_tbl(Y_TBL);
        stepperY.set_toff(Y_TOFF);
      #endif
      #if ENABLED(Y2_IS_TMC2130)
        stepperY2.init();
        stepperY2.set_mres(Y2_MRES);
        stepperY2.set_IHOLD_IRUN(Y2_IHOLD,Y2_IRUN,Y2_IHOLDDELAY);
        stepperY2.set_I_scale_analog(Y2_ISCALE);
        stepperY2.set_tbl(Y2_TBL);
        stepperY2.set_toff(Y2_TOFF);
      #endif
      #if ENABLED(Z_IS_TMC2130)
        stepperZ.init();
        stepperZ.set_mres(Z_MRES);
        stepperZ.set_IHOLD_IRUN(Z_IHOLD,Z_IRUN,Z_IHOLDDELAY);
        stepperZ.set_I_scale_analog(Z_ISCALE);
        stepperZ.set_tbl(Z_TBL);
        stepperZ.set_toff(Z_TOFF);
      #endif
      #if ENABLED(Z2_IS_TMC2130)
        stepperZ2.init();
        stepperZ2.set_mres(Z2_MRES);
        stepperZ2.set_IHOLD_IRUN(Z2_IHOLD,Z2_IRUN,Z2_IHOLDDELAY);
        stepperZ2.set_I_scale_analog(Z2_ISCALE);
        stepperZ2.set_tbl(Z2_TBL);
        stepperZ2.set_toff(Z2_TOFF);
      #endif
      #if ENABLED(E0_IS_TMC2130)
        stepperE0.init();
        stepperE0.set_mres(E0_MRES);
        stepperE0.set_IHOLD_IRUN(E0_IHOLD,E0_IRUN,E0_IHOLDDELAY);
        stepperE0.set_I_scale_analog(E0_ISCALE);
        stepperE0.set_tbl(E0_TBL);
        stepperE0.set_toff(E0_TOFF);
      #endif
      #if ENABLED(E1_IS_TMC2130)
        stepperE1.init();
        stepperE1.set_mres(E1_MRES);
        stepperE1.set_IHOLD_IRUN(E1_IHOLD,E1_IRUN,E1_IHOLDDELAY);
        stepperE1.set_I_scale_analog(E1_ISCALE);
        stepperE1.set_tbl(E1_TBL);
        stepperE1.set_toff(E1_TOFF);
      #endif
      #if ENABLED(E2_IS_TMC2130)
        stepperE2.init();
        stepperE2.set_mres(E2_MRES);
        stepperE2.set_IHOLD_IRUN(E2_IHOLD,E2_IRUN,E2_IHOLDDELAY);
        stepperE2.set_I_scale_analog(E2_ISCALE);
        stepperE2.set_tbl(E2_TBL);
        stepperE2.set_toff(E2_TOFF);
      #endif
      #if ENABLED(E3_IS_TMC2130)
        stepperE3.init();
        stepperE3.set_mres(E3_MRES);
        stepperE3.set_IHOLD_IRUN(E3_IHOLD,E3_IRUN,E3_IHOLDDELAY);
        stepperE3.set_I_scale_analog(E3_ISCALE);
        stepperE3.set_tbl(E3_TBL);
        stepperE3.set_toff(E3_TOFF);
      #endif
    }

  #endif // TMC2130_ADVANCED_CONFIGURATION

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

