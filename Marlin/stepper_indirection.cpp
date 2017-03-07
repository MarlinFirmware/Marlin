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
 * stepper_indirection.cpp
 *
 * Stepper motor driver indirection to allow some stepper functions to
 * be done via SPI/I2c instead of direct pin manipulation.
 *
 * Part of Marlin
 *
 * Copyright (c) 2015 Dominik Wenger
 */

#include "stepper_indirection.h"

#include "MarlinConfig.h"

//
// TMC26X Driver objects and inits
//
#if ENABLED(HAVE_TMCDRIVER)

  #include <SPI.h>
  #include <TMC26XStepper.h>

  #define _TMC_DEFINE(ST) TMC26XStepper stepper##ST(200, ST##_ENABLE_PIN, ST##_STEP_PIN, ST##_DIR_PIN, ST##_MAX_CURRENT, ST##_SENSE_RESISTOR)

  #if ENABLED(X_IS_TMC)
    _TMC_DEFINE(X);
  #endif
  #if ENABLED(X2_IS_TMC)
    _TMC_DEFINE(X2);
  #endif
  #if ENABLED(Y_IS_TMC)
    _TMC_DEFINE(Y);
  #endif
  #if ENABLED(Y2_IS_TMC)
    _TMC_DEFINE(Y2);
  #endif
  #if ENABLED(Z_IS_TMC)
    _TMC_DEFINE(Z);
  #endif
  #if ENABLED(Z2_IS_TMC)
    _TMC_DEFINE(Z2);
  #endif
  #if ENABLED(E0_IS_TMC)
    _TMC_DEFINE(E0);
  #endif
  #if ENABLED(E1_IS_TMC)
    _TMC_DEFINE(E1);
  #endif
  #if ENABLED(E2_IS_TMC)
    _TMC_DEFINE(E2);
  #endif
  #if ENABLED(E3_IS_TMC)
    _TMC_DEFINE(E3);
  #endif

  #define _TMC_INIT(A) do{ \
    stepper##A.setMicrosteps(A##_MICROSTEPS); \
    stepper##A.start(); \
  }while(0)

  void tmc_init() {
    #if ENABLED(X_IS_TMC)
      _TMC_INIT(X);
    #endif
    #if ENABLED(X2_IS_TMC)
      _TMC_INIT(X2);
    #endif
    #if ENABLED(Y_IS_TMC)
      _TMC_INIT(Y);
    #endif
    #if ENABLED(Y2_IS_TMC)
      _TMC_INIT(Y2);
    #endif
    #if ENABLED(Z_IS_TMC)
      _TMC_INIT(Z);
    #endif
    #if ENABLED(Z2_IS_TMC)
      _TMC_INIT(Z2);
    #endif
    #if ENABLED(E0_IS_TMC)
      _TMC_INIT(E0);
    #endif
    #if ENABLED(E1_IS_TMC)
      _TMC_INIT(E1);
    #endif
    #if ENABLED(E2_IS_TMC)
      _TMC_INIT(E2);
    #endif
    #if ENABLED(E3_IS_TMC)
      _TMC_INIT(E3);
    #endif
  }

#endif // HAVE_TMCDRIVER

//
// TMC2130 Driver objects and inits
//
#if ENABLED(HAVE_TMC2130)

  #include <SPI.h>
  #include <TMC2130Stepper.h>

  #define _TMC2130_DEFINE(ST) TMC2130Stepper stepper##ST(ST##_ENABLE_PIN, ST##_DIR_PIN, ST##_STEP_PIN, ST##_CHIP_SELECT)

  // Stepper objects of TMC2130 steppers used
  #if ENABLED(X_IS_TMC2130)
    _TMC2130_DEFINE(X);
  #endif
  #if ENABLED(X2_IS_TMC2130)
    _TMC2130_DEFINE(X2);
  #endif
  #if ENABLED(Y_IS_TMC2130)
    _TMC2130_DEFINE(Y);
  #endif
  #if ENABLED(Y2_IS_TMC2130)
    _TMC2130_DEFINE(Y2);
  #endif
  #if ENABLED(Z_IS_TMC2130)
    _TMC2130_DEFINE(Z);
  #endif
  #if ENABLED(Z2_IS_TMC2130)
    _TMC2130_DEFINE(Z2);
  #endif
  #if ENABLED(E0_IS_TMC2130)
    _TMC2130_DEFINE(E0);
  #endif
  #if ENABLED(E1_IS_TMC2130)
    _TMC2130_DEFINE(E1);
  #endif
  #if ENABLED(E2_IS_TMC2130)
    _TMC2130_DEFINE(E2);
  #endif
  #if ENABLED(E3_IS_TMC2130)
    _TMC2130_DEFINE(E3);
  #endif

  // Use internal reference voltage for current calculations. This is the default.
  // Following values from Trinamic's spreadsheet with values for a NEMA17 (42BYGHW609)
  void tmc2130_init(TMC2130Stepper &st, const uint16_t max_current, const uint16_t microsteps) {
    st.begin();
    st.setCurrent(st.getCurrent(), R_SENSE, HOLD_MULTIPLIER);
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

  #define _TMC2130_INIT(ST) tmc2130_init(stepper##ST, ST##_MAX_CURRENT, ST##_MICROSTEPS)

  void tmc2130_init() {
    delay(500); // Let power stabilize before configuring the steppers
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

    TMC2130_ADV()
  }
#endif // HAVE_TMC2130


//
// L6470 Driver objects and inits
//
#if ENABLED(HAVE_L6470DRIVER)

  #include <SPI.h>
  #include <L6470.h>

  #define _L6470_DEFINE(ST) L6470 stepper##ST(ST##_ENABLE_PIN)

  // L6470 Stepper objects
  #if ENABLED(X_IS_L6470)
    _L6470_DEFINE(X);
  #endif
  #if ENABLED(X2_IS_L6470)
    _L6470_DEFINE(X2);
  #endif
  #if ENABLED(Y_IS_L6470)
    _L6470_DEFINE(Y);
  #endif
  #if ENABLED(Y2_IS_L6470)
    _L6470_DEFINE(Y2);
  #endif
  #if ENABLED(Z_IS_L6470)
    _L6470_DEFINE(Z);
  #endif
  #if ENABLED(Z2_IS_L6470)
    _L6470_DEFINE(Z2);
  #endif
  #if ENABLED(E0_IS_L6470)
    _L6470_DEFINE(E0);
  #endif
  #if ENABLED(E1_IS_L6470)
    _L6470_DEFINE(E1);
  #endif
  #if ENABLED(E2_IS_L6470)
    _L6470_DEFINE(E2);
  #endif
  #if ENABLED(E3_IS_L6470)
    _L6470_DEFINE(E3);
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

