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
  #if ENABLED(E4_IS_TMC)
    _TMC_DEFINE(E4);
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
    #if ENABLED(E4_IS_TMC)
      _TMC_INIT(E4);
    #endif
  }

#endif // HAVE_TMCDRIVER

//
// TMC2130 Driver objects and inits
//
#if ENABLED(HAVE_TMC2130)

  #include <SPI.h>
  #include <TMC2130Stepper.h>
  #include "enum.h"

  #define _TMC2130_DEFINE(ST) TMC2130Stepper stepper##ST(ST##_ENABLE_PIN, ST##_DIR_PIN, ST##_STEP_PIN, ST##_CS_PIN)

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
  #if ENABLED(E4_IS_TMC2130)
    _TMC2130_DEFINE(E4);
  #endif

  // Use internal reference voltage for current calculations. This is the default.
  // Following values from Trinamic's spreadsheet with values for a NEMA17 (42BYGHW609)
  // https://www.trinamic.com/products/integrated-circuits/details/tmc2130/
  void tmc2130_init(TMC2130Stepper &st, const uint16_t microsteps, const uint32_t thrs, const uint32_t spmm) {
    st.begin();
    st.setCurrent(st.getCurrent(), R_SENSE, HOLD_MULTIPLIER);
    st.microsteps(microsteps);
    st.blank_time(36);
    st.off_time(5); // Only enables the driver if used with stealthChop
    st.interpolate(INTERPOLATE);
    st.power_down_delay(128); // ~2s until driver lowers to hold current
    st.hysterisis_start(0); // HSTRT = 1
    st.hysterisis_low(1); // HEND = -2
    st.diag1_active_high(1); // For sensorless homing
    #if ENABLED(STEALTHCHOP)
      st.stealth_freq(1); // f_pwm = 2/683 f_clk
      st.stealth_autoscale(1);
      st.stealth_gradient(5);
      st.stealth_amplitude(255);
      st.stealthChop(1);
      #if ENABLED(HYBRID_THRESHOLD)
        st.stealth_max_speed(12650000UL*st.microsteps()/(256*thrs*spmm));
      #endif
    #elif ENABLED(SENSORLESS_HOMING)
      st.coolstep_min_speed(1024UL * 1024UL - 1UL);
    #endif
  }

  #define _TMC2130_INIT(ST, SPMM) tmc2130_init(stepper##ST, ST##_MICROSTEPS, ST##_HYBRID_THRESHOLD, SPMM)

  void tmc2130_init() {
    constexpr uint16_t steps_per_mm[] = DEFAULT_AXIS_STEPS_PER_UNIT;
    #if ENABLED(X_IS_TMC2130)
      _TMC2130_INIT( X, steps_per_mm[X_AXIS]);
      #if ENABLED(SENSORLESS_HOMING)
        stepperX.sg_stall_value(X_HOMING_SENSITIVITY);
      #endif
    #endif
    #if ENABLED(X2_IS_TMC2130)
      _TMC2130_INIT(X2, steps_per_mm[X_AXIS]);
    #endif
    #if ENABLED(Y_IS_TMC2130)
      _TMC2130_INIT( Y, steps_per_mm[Y_AXIS]);
      #if ENABLED(SENSORLESS_HOMING)
        stepperY.sg_stall_value(Y_HOMING_SENSITIVITY);
      #endif
    #endif
    #if ENABLED(Y2_IS_TMC2130)
      _TMC2130_INIT(Y2, steps_per_mm[Y_AXIS]);
    #endif
    #if ENABLED(Z_IS_TMC2130)
      _TMC2130_INIT( Z, steps_per_mm[Z_AXIS]);
    #endif
    #if ENABLED(Z2_IS_TMC2130)
      _TMC2130_INIT(Z2, steps_per_mm[Z_AXIS]);
    #endif
    #if ENABLED(E0_IS_TMC2130)
      _TMC2130_INIT(E0, steps_per_mm[E_AXIS]);
    #endif
    #if ENABLED(E1_IS_TMC2130)
      { constexpr int extruder = 1; _TMC2130_INIT(E1, steps_per_mm[E_AXIS_N]); }
    #endif
    #if ENABLED(E2_IS_TMC2130)
      { constexpr int extruder = 2; _TMC2130_INIT(E2, steps_per_mm[E_AXIS_N]); }
    #endif
    #if ENABLED(E3_IS_TMC2130)
      { constexpr int extruder = 3; _TMC2130_INIT(E3, steps_per_mm[E_AXIS_N]); }
    #endif
    #if ENABLED(E4_IS_TMC2130)
      { constexpr int extruder = 4; _TMC2130_INIT(E4, steps_per_mm[E_AXIS_N]); }
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
  #if ENABLED(E4_IS_L6470)
    _L6470_DEFINE(E4);
  #endif

  #define _L6470_INIT(A) do{ \
    stepper##A.init(A##_K_VAL); \
    stepper##A.softFree(); \
    stepper##A.setMicroSteps(A##_MICROSTEPS); \
    stepper##A.setOverCurrent(A##_OVERCURRENT); \
    stepper##A.setStallCurrent(A##_STALLCURRENT); \
  }while(0)

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
    #if ENABLED(E4_IS_L6470)
      _L6470_INIT(E4);
    #endif
  }

#endif // HAVE_L6470DRIVER

