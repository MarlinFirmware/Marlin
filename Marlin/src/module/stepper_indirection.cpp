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

#include "../inc/MarlinConfig.h"

//
// TMC26X Driver objects and inits
//
#if ENABLED(HAVE_TMC26X)
  #include <SPI.h>

  #ifdef STM32F7
    #include "../HAL/HAL_STM32F7/TMC2660.h"
  #else
    #include <TMC26XStepper.h>
  #endif

  #define _TMC26X_DEFINE(ST) TMC26XStepper stepper##ST(200, ST##_ENABLE_PIN, ST##_STEP_PIN, ST##_DIR_PIN, ST##_MAX_CURRENT, ST##_SENSE_RESISTOR)

  #if ENABLED(X_IS_TMC26X)
    _TMC26X_DEFINE(X);
  #endif
  #if ENABLED(X2_IS_TMC26X)
    _TMC26X_DEFINE(X2);
  #endif
  #if ENABLED(Y_IS_TMC26X)
    _TMC26X_DEFINE(Y);
  #endif
  #if ENABLED(Y2_IS_TMC26X)
    _TMC26X_DEFINE(Y2);
  #endif
  #if ENABLED(Z_IS_TMC26X)
    _TMC26X_DEFINE(Z);
  #endif
  #if ENABLED(Z2_IS_TMC26X)
    _TMC26X_DEFINE(Z2);
  #endif
  #if ENABLED(E0_IS_TMC26X)
    _TMC26X_DEFINE(E0);
  #endif
  #if ENABLED(E1_IS_TMC26X)
    _TMC26X_DEFINE(E1);
  #endif
  #if ENABLED(E2_IS_TMC26X)
    _TMC26X_DEFINE(E2);
  #endif
  #if ENABLED(E3_IS_TMC26X)
    _TMC26X_DEFINE(E3);
  #endif
  #if ENABLED(E4_IS_TMC26X)
    _TMC26X_DEFINE(E4);
  #endif

  #define _TMC26X_INIT(A) do{ \
    stepper##A.setMicrosteps(A##_MICROSTEPS); \
    stepper##A.start(); \
  }while(0)

  void tmc26x_init_to_defaults() {
    #if ENABLED(X_IS_TMC26X)
      _TMC26X_INIT(X);
    #endif
    #if ENABLED(X2_IS_TMC26X)
      _TMC26X_INIT(X2);
    #endif
    #if ENABLED(Y_IS_TMC26X)
      _TMC26X_INIT(Y);
    #endif
    #if ENABLED(Y2_IS_TMC26X)
      _TMC26X_INIT(Y2);
    #endif
    #if ENABLED(Z_IS_TMC26X)
      _TMC26X_INIT(Z);
    #endif
    #if ENABLED(Z2_IS_TMC26X)
      _TMC26X_INIT(Z2);
    #endif
    #if ENABLED(E0_IS_TMC26X)
      _TMC26X_INIT(E0);
    #endif
    #if ENABLED(E1_IS_TMC26X)
      _TMC26X_INIT(E1);
    #endif
    #if ENABLED(E2_IS_TMC26X)
      _TMC26X_INIT(E2);
    #endif
    #if ENABLED(E3_IS_TMC26X)
      _TMC26X_INIT(E3);
    #endif
    #if ENABLED(E4_IS_TMC26X)
      _TMC26X_INIT(E4);
    #endif
  }
#endif // HAVE_TMC26X

//
// TMC2130 Driver objects and inits
//
#if ENABLED(HAVE_TMC2130)

  #include <SPI.h>
  #include <TMC2130Stepper.h>
  #include "planner.h"
  #include "../core/enum.h"

  #if ENABLED(TMC_USE_SW_SPI)
    #define _TMC2130_DEFINE(ST) TMC2130Stepper stepper##ST(ST##_ENABLE_PIN, ST##_DIR_PIN, ST##_STEP_PIN, ST##_CS_PIN, TMC_SW_MOSI, TMC_SW_MISO, TMC_SW_SCK)
  #else
    #define _TMC2130_DEFINE(ST) TMC2130Stepper stepper##ST(ST##_ENABLE_PIN, ST##_DIR_PIN, ST##_STEP_PIN, ST##_CS_PIN)
  #endif

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
  void tmc2130_init(TMC2130Stepper &st, const uint16_t mA, const uint16_t microsteps, const uint32_t thrs, const float spmm) {
    st.begin();
    st.setCurrent(mA, R_SENSE, HOLD_MULTIPLIER);
    st.microsteps(microsteps);
    st.blank_time(24);
    st.off_time(5); // Only enables the driver if used with stealthChop
    st.interpolate(INTERPOLATE);
    st.power_down_delay(128); // ~2s until driver lowers to hold current
    st.hysteresis_start(3);
    st.hysteresis_end(2);
    st.diag1_active_high(1); // For sensorless homing
    #if ENABLED(STEALTHCHOP)
      st.stealth_freq(1); // f_pwm = 2/683 f_clk
      st.stealth_autoscale(1);
      st.stealth_gradient(5);
      st.stealth_amplitude(255);
      st.stealthChop(1);
      #if ENABLED(HYBRID_THRESHOLD)
        st.stealth_max_speed(12650000UL*microsteps/(256*thrs*spmm));
      #else
        UNUSED(thrs);
        UNUSED(spmm);
      #endif
    #elif ENABLED(SENSORLESS_HOMING)
      st.coolstep_min_speed(1024UL * 1024UL - 1UL);
    #endif
    st.GSTAT(); // Clear GSTAT
  }

  #define _TMC2130_INIT(ST, SPMM) tmc2130_init(stepper##ST, ST##_CURRENT, ST##_MICROSTEPS, ST##_HYBRID_THRESHOLD, SPMM)

  void tmc2130_init_to_defaults() {
    #if ENABLED(X_IS_TMC2130)
      _TMC2130_INIT( X, planner.axis_steps_per_mm[X_AXIS]);
    #endif
    #if ENABLED(X2_IS_TMC2130)
      _TMC2130_INIT(X2, planner.axis_steps_per_mm[X_AXIS]);
    #endif
    #if ENABLED(Y_IS_TMC2130)
      _TMC2130_INIT( Y, planner.axis_steps_per_mm[Y_AXIS]);
    #endif
    #if ENABLED(Y2_IS_TMC2130)
      _TMC2130_INIT(Y2, planner.axis_steps_per_mm[Y_AXIS]);
    #endif
    #if ENABLED(Z_IS_TMC2130)
      _TMC2130_INIT( Z, planner.axis_steps_per_mm[Z_AXIS]);
    #endif
    #if ENABLED(Z2_IS_TMC2130)
      _TMC2130_INIT(Z2, planner.axis_steps_per_mm[Z_AXIS]);
    #endif
    #if ENABLED(E0_IS_TMC2130)
      _TMC2130_INIT(E0, planner.axis_steps_per_mm[E_AXIS]);
    #endif
    #if ENABLED(E1_IS_TMC2130)
      { constexpr int extruder = 1; _TMC2130_INIT(E1, planner.axis_steps_per_mm[E_AXIS_N]); }
    #endif
    #if ENABLED(E2_IS_TMC2130)
      { constexpr int extruder = 2; _TMC2130_INIT(E2, planner.axis_steps_per_mm[E_AXIS_N]); }
    #endif
    #if ENABLED(E3_IS_TMC2130)
      { constexpr int extruder = 3; _TMC2130_INIT(E3, planner.axis_steps_per_mm[E_AXIS_N]); }
    #endif
    #if ENABLED(E4_IS_TMC2130)
      { constexpr int extruder = 4; _TMC2130_INIT(E4, planner.axis_steps_per_mm[E_AXIS_N]); }
    #endif

    #if ENABLED(SENSORLESS_HOMING)
      #define TMC_INIT_SGT(P,Q) stepper##Q.sgt(P##_HOMING_SENSITIVITY);
      #ifdef X_HOMING_SENSITIVITY
        #if ENABLED(X_IS_TMC2130) || ENABLED(IS_TRAMS)
          stepperX.sgt(X_HOMING_SENSITIVITY);
        #endif
        #if ENABLED(X2_IS_TMC2130)
          stepperX2.sgt(X_HOMING_SENSITIVITY);
        #endif
      #endif
      #ifdef Y_HOMING_SENSITIVITY
        #if ENABLED(Y_IS_TMC2130) || ENABLED(IS_TRAMS)
          stepperY.sgt(Y_HOMING_SENSITIVITY);
        #endif
        #if ENABLED(Y2_IS_TMC2130)
          stepperY2.sgt(Y_HOMING_SENSITIVITY);
        #endif
      #endif
      #ifdef Z_HOMING_SENSITIVITY
        #if ENABLED(Z_IS_TMC2130) || ENABLED(IS_TRAMS)
          stepperZ.sgt(Z_HOMING_SENSITIVITY);
        #endif
        #if ENABLED(Z2_IS_TMC2130)
          stepperZ2.sgt(Z_HOMING_SENSITIVITY);
        #endif
      #endif
    #endif
  }
#endif // HAVE_TMC2130

//
// TMC2208 Driver objects and inits
//
#if ENABLED(HAVE_TMC2208)

  #include <SoftwareSerial.h>
  #include <HardwareSerial.h>
  #include <TMC2208Stepper.h>
  #include "planner.h"

  #define _TMC2208_DEFINE_HARDWARE(ST) TMC2208Stepper stepper##ST(&ST##_HARDWARE_SERIAL)
  #define _TMC2208_DEFINE_SOFTWARE(ST) SoftwareSerial ST##_HARDWARE_SERIAL = SoftwareSerial(ST##_SERIAL_RX_PIN, ST##_SERIAL_TX_PIN); \
                                       TMC2208Stepper stepper##ST(&ST##_HARDWARE_SERIAL, ST##_SERIAL_RX_PIN > -1)

  // Stepper objects of TMC2208 steppers used
  #if ENABLED(X_IS_TMC2208)
    #ifdef X_HARDWARE_SERIAL
      _TMC2208_DEFINE_HARDWARE(X);
    #else
      _TMC2208_DEFINE_SOFTWARE(X);
    #endif
  #endif
  #if ENABLED(X2_IS_TMC2208)
    #ifdef X2_HARDWARE_SERIAL
      _TMC2208_DEFINE_HARDWARE(X2);
    #else
      _TMC2208_DEFINE_SOFTWARE(X2);
    #endif
  #endif
  #if ENABLED(Y_IS_TMC2208)
    #ifdef Y_HARDWARE_SERIAL
      _TMC2208_DEFINE_HARDWARE(Y);
    #else
      _TMC2208_DEFINE_SOFTWARE(Y);
    #endif
  #endif
  #if ENABLED(Y2_IS_TMC2208)
    #ifdef Y2_HARDWARE_SERIAL
      _TMC2208_DEFINE_HARDWARE(Y2);
    #else
      _TMC2208_DEFINE_SOFTWARE(Y2);
    #endif
  #endif
  #if ENABLED(Z_IS_TMC2208)
    #ifdef Z_HARDWARE_SERIAL
      _TMC2208_DEFINE_HARDWARE(Z);
    #else
      _TMC2208_DEFINE_SOFTWARE(Z);
    #endif
  #endif
  #if ENABLED(Z2_IS_TMC2208)
    #ifdef Z2_HARDWARE_SERIAL
      _TMC2208_DEFINE_HARDWARE(Z2);
    #else
      _TMC2208_DEFINE_SOFTWARE(Z2);
    #endif
  #endif
  #if ENABLED(E0_IS_TMC2208)
    #ifdef E0_HARDWARE_SERIAL
      _TMC2208_DEFINE_HARDWARE(E0);
    #else
      _TMC2208_DEFINE_SOFTWARE(E0);
    #endif
  #endif
  #if ENABLED(E1_IS_TMC2208)
    #ifdef E1_HARDWARE_SERIAL
      _TMC2208_DEFINE_HARDWARE(E1);
    #else
      _TMC2208_DEFINE_SOFTWARE(E1);
    #endif
  #endif
  #if ENABLED(E2_IS_TMC2208)
    #ifdef E2_HARDWARE_SERIAL
      _TMC2208_DEFINE_HARDWARE(E2);
    #else
      _TMC2208_DEFINE_SOFTWARE(E2);
    #endif
  #endif
  #if ENABLED(E3_IS_TMC2208)
    #ifdef E3_HARDWARE_SERIAL
      _TMC2208_DEFINE_HARDWARE(E3);
    #else
      _TMC2208_DEFINE_SOFTWARE(E3);
    #endif
  #endif
  #if ENABLED(E4_IS_TMC2208)
    #ifdef E4_HARDWARE_SERIAL
      _TMC2208_DEFINE_HARDWARE(E4);
    #else
      _TMC2208_DEFINE_SOFTWARE(E4);
    #endif
  #endif

  void tmc2208_serial_begin() {
    #if ENABLED(X_IS_TMC2208)
      X_HARDWARE_SERIAL.begin(115200);
    #endif
    #if ENABLED(X2_IS_TMC2208)
      X2_HARDWARE_SERIAL.begin(115200);
    #endif
    #if ENABLED(Y_IS_TMC2208)
      Y_HARDWARE_SERIAL.begin(115200);
    #endif
    #if ENABLED(Y2_IS_TMC2208)
      Y2_HARDWARE_SERIAL.begin(115200);
    #endif
    #if ENABLED(Z_IS_TMC2208)
      Z_HARDWARE_SERIAL.begin(115200);
    #endif
    #if ENABLED(Z2_IS_TMC2208)
      Z2_HARDWARE_SERIAL.begin(115200);
    #endif
    #if ENABLED(E0_IS_TMC2208)
      E0_HARDWARE_SERIAL.begin(115200);
    #endif
    #if ENABLED(E1_IS_TMC2208)
      E1_HARDWARE_SERIAL.begin(115200);
    #endif
    #if ENABLED(E2_IS_TMC2208)
      E2_HARDWARE_SERIAL.begin(115200);
    #endif
    #if ENABLED(E3_IS_TMC2208)
      E3_HARDWARE_SERIAL.begin(115200);
    #endif
    #if ENABLED(E4_IS_TMC2208)
      E4_HARDWARE_SERIAL.begin(115200);
    #endif
  }

  // Use internal reference voltage for current calculations. This is the default.
  // Following values from Trinamic's spreadsheet with values for a NEMA17 (42BYGHW609)
  void tmc2208_init(TMC2208Stepper &st, const uint16_t mA, const uint16_t microsteps, const uint32_t thrs, const float spmm) {
    st.pdn_disable(true); // Use UART
    st.mstep_reg_select(true); // Select microsteps with UART
    st.I_scale_analog(false);
    st.rms_current(mA, HOLD_MULTIPLIER, R_SENSE);
    st.microsteps(microsteps);
    st.blank_time(24);
    st.toff(5);
    st.intpol(INTERPOLATE);
    st.TPOWERDOWN(128); // ~2s until driver lowers to hold current
    st.hysteresis_start(3);
    st.hysteresis_end(2);
    #if ENABLED(STEALTHCHOP)
      st.pwm_lim(12);
      st.pwm_reg(8);
      st.pwm_autograd(1);
      st.pwm_autoscale(1);
      st.pwm_freq(1);
      st.pwm_grad(14);
      st.pwm_ofs(36);
      st.en_spreadCycle(false);
      #if ENABLED(HYBRID_THRESHOLD)
        st.TPWMTHRS(12650000UL*microsteps/(256*thrs*spmm));
      #else
        UNUSED(thrs);
        UNUSED(spmm);
      #endif
    #else
      st.en_spreadCycle(true);
    #endif
    st.GSTAT(0b111); // Clear
    delay(200);
  }

  #define _TMC2208_INIT(ST, SPMM) tmc2208_init(stepper##ST, ST##_CURRENT, ST##_MICROSTEPS, ST##_HYBRID_THRESHOLD, SPMM)

  void tmc2208_init_to_defaults() {
    #if ENABLED(X_IS_TMC2208)
      _TMC2208_INIT(X, planner.axis_steps_per_mm[X_AXIS]);
    #endif
    #if ENABLED(X2_IS_TMC2208)
      _TMC2208_INIT(X2, planner.axis_steps_per_mm[X_AXIS]);
    #endif
    #if ENABLED(Y_IS_TMC2208)
      _TMC2208_INIT(Y, planner.axis_steps_per_mm[Y_AXIS]);
    #endif
    #if ENABLED(Y2_IS_TMC2208)
      _TMC2208_INIT(Y2, planner.axis_steps_per_mm[Y_AXIS]);
    #endif
    #if ENABLED(Z_IS_TMC2208)
      _TMC2208_INIT(Z, planner.axis_steps_per_mm[Z_AXIS]);
    #endif
    #if ENABLED(Z2_IS_TMC2208)
      _TMC2208_INIT(Z2, planner.axis_steps_per_mm[Z_AXIS]);
    #endif
    #if ENABLED(E0_IS_TMC2208)
      _TMC2208_INIT(E0, planner.axis_steps_per_mm[E_AXIS]);
    #endif
    #if ENABLED(E1_IS_TMC2208)
      { constexpr int extruder = 1; _TMC2208_INIT(E1, planner.axis_steps_per_mm[E_AXIS_N]); }
    #endif
    #if ENABLED(E2_IS_TMC2208)
      { constexpr int extruder = 2; _TMC2208_INIT(E2, planner.axis_steps_per_mm[E_AXIS_N]); }
    #endif
    #if ENABLED(E3_IS_TMC2208)
      { constexpr int extruder = 3; _TMC2208_INIT(E3, planner.axis_steps_per_mm[E_AXIS_N]); }
    #endif
    #if ENABLED(E4_IS_TMC2208)
      { constexpr int extruder = 4; _TMC2208_INIT(E4, planner.axis_steps_per_mm[E_AXIS_N]); }
    #endif
  }
#endif // HAVE_TMC2208

void restore_stepper_drivers() {
  #if X_IS_TRINAMIC
    stepperX.push();
  #endif
  #if X2_IS_TRINAMIC
    stepperX2.push();
  #endif
  #if Y_IS_TRINAMIC
    stepperY.push();
  #endif
  #if Y2_IS_TRINAMIC
    stepperY2.push();
  #endif
  #if Z_IS_TRINAMIC
    stepperZ.push();
  #endif
  #if Z2_IS_TRINAMIC
    stepperZ2.push();
  #endif
  #if E0_IS_TRINAMIC
    stepperE0.push();
  #endif
  #if E1_IS_TRINAMIC
    stepperE1.push();
  #endif
  #if E2_IS_TRINAMIC
    stepperE2.push();
  #endif
  #if E3_IS_TRINAMIC
    stepperE3.push();
  #endif
  #if E4_IS_TRINAMIC
    stepperE4.push();
  #endif
}

void reset_stepper_drivers() {
  #if ENABLED(HAVE_TMC26X)
    tmc26x_init_to_defaults();
  #endif
  #if ENABLED(HAVE_TMC2130)
    delay(100);
    tmc2130_init_to_defaults();
  #endif
  #if ENABLED(HAVE_TMC2208)
    delay(100);
    tmc2208_init_to_defaults();
  #endif
  #ifdef TMC_ADV
    TMC_ADV()
  #endif
  #if ENABLED(HAVE_L6470DRIVER)
    L6470_init_to_defaults();
  #endif
}

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
    stepper##A.init(); \
    stepper##A.softFree(); \
    stepper##A.setMicroSteps(A##_MICROSTEPS); \
    stepper##A.setOverCurrent(A##_OVERCURRENT); \
    stepper##A.setStallCurrent(A##_STALLCURRENT); \
  }while(0)

  void L6470_init_to_defaults() {
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
