/*
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
#include "Configuration.h"

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

