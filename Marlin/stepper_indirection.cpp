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

//
// TMC26X Driver objects and inits
//
#if ENABLED(HAVE_TMCDRIVER)

  #include <SPI.h>
  #include <TMC26XStepper.h>

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

#if ENABLED(HAVE_TMC2130)
  #define DEFINE_STEPPER(ST) TMC2130Stepper stepper##ST(ST##_ENABLE_PIN, ST##_DIR_PIN, ST##_STEP_PIN, ST##_CHIP_SELECT)
// Stepper objects of TMC2130 steppers used
  #if ENABLED(X_IS_TMC2130)
    DEFINE_STEPPER(X);
  #endif
  #if ENABLED(X2_IS_TMC2130)
    DEFINE_STEPPER(X2);
  #endif
  #if ENABLED(Y_IS_TMC2130)
    DEFINE_STEPPER(Y);
  #endif
  #if ENABLED(Y2_IS_TMC2130)
    DEFINE_STEPPER(Y2);
  #endif
  #if ENABLED(Z_IS_TMC2130)
    DEFINE_STEPPER(Z);
  #endif
  #if ENABLED(Z2_IS_TMC2130)
    DEFINE_STEPPER(Z2);
  #endif
  #if ENABLED(E0_IS_TMC2130)
    DEFINE_STEPPER(E0);
  #endif
  #if ENABLED(E1_IS_TMC2130)
    DEFINE_STEPPER(E1);
  #endif
  #if ENABLED(E2_IS_TMC2130)
    DEFINE_STEPPER(E2);
  #endif
  #if ENABLED(E3_IS_TMC2130)
    DEFINE_STEPPER(E3);
  #endif

// Use internal reference voltage for current calculations. This is the default.
// Following values from Trinamic's spreadsheet with values for a NEMA17 (42BYGHW609)
  void tmc2130_init(TMC2130Stepper &st, uint16_t max_current, uint16_t microsteps) {
    st.begin();
    st.setCurrent(max_current, R_SENSE, HOLD_MULTIPLIER);
    st.microsteps(microsteps);
    st.blank_time(24);
    st.off_time(8);
    st.interpolate(INTERPOLATE);
    #if ENABLED(STEALTHCHOP)
      st.stealthChop(1);
    #endif
    #if ENABLED(SENSORLESS_HOMING)
      st.coolstep_min_speed(1048575);
      st.sg_stall_value(STALL_THRESHOLD);
      st.sg_filter(1);
      st.diag1_stall(1);
      st.diag1_active_high(1);
    #endif
  }

  #define _TMC2130_INIT(ST) tmc2130_init(stepper##ST, ST##_MAX_CURRENT, ST##_MICROSTEPS);

  void tmc2130_init() {
    delay(500); // Let power stabilize before we start configuring the steppers
    #if ENABLED(X_IS_TMC2130)
      _TMC2130_INIT(X)
    #endif
    #if ENABLED(X2_IS_TMC2130)
      _TMC2130_INIT(X2)
    #endif
    #if ENABLED(Y_IS_TMC2130)
      _TMC2130_INIT(Y)
    #endif
    #if ENABLED(Y2_IS_TMC2130)
      _TMC2130_INIT(Y2)
    #endif
    #if ENABLED(Z_IS_TMC2130)
      _TMC2130_INIT(Z)
    #endif
    #if ENABLED(Z2_IS_TMC2130)
      _TMC2130_INIT(Z2)
    #endif
    #if ENABLED(E0_IS_TMC2130)
      _TMC2130_INIT(E0)
    #endif
    #if ENABLED(E1_IS_TMC2130)
      _TMC2130_INIT(E1)
    #endif
    #if ENABLED(E2_IS_TMC2130)
      _TMC2130_INIT(E2)
    #endif
    #if ENABLED(E3_IS_TMC2130)
      _TMC2130_INIT(E3)
    #endif
  }
#endif

// L6470 Driver objects and inits

//
// L6470 Driver objects and inits
//
#if ENABLED(HAVE_L6470DRIVER)

  #include <SPI.h>
  #include <L6470.h>

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

  #define _L6470_INIT(A) do{ \
    stepper##A.init(A##_K_VAL); \
    stepper##A.softFree(); \
    stepper##A.setMicroSteps(A##_MICROSTEPS); \
    stepper##A.setOverCurrent(A##_OVERCURRENT); \
    stepper##A.setStallCurrent(A##_STALLCURRENT); \
  } while(0)

  void L6470_init() {
    #if ENABLED(X_IS_L6470)
      _L6470_INIT(X);
    #endif
    #if ENABLED(X2_IS_L6470)
      _L6470_INIT(X2);
    #endif
    #if ENABLED(Y_IS_L6470)
      _L6470_INIT(Y);
    #endif
    #if ENABLED(Y2_IS_L6470)
      _L6470_INIT(Y2);
    #endif
    #if ENABLED(Z_IS_L6470)
      _L6470_INIT(Z);
    #endif
    #if ENABLED(Z2_IS_L6470)
      _L6470_INIT(Z2);
    #endif
    #if ENABLED(E0_IS_L6470)
      _L6470_INIT(E0);
    #endif
    #if ENABLED(E1_IS_L6470)
      _L6470_INIT(E1);
    #endif
    #if ENABLED(E2_IS_L6470)
      _L6470_INIT(E2);
    #endif
    #if ENABLED(E3_IS_L6470)
      _L6470_INIT(E3);
    #endif
  }

#endif // HAVE_L6470DRIVER

