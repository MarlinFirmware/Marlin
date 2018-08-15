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
#if HAS_DRIVER(TMC26X)
  #include <SPI.h>
  #include <TMC26XStepper.h>

  #define _TMC26X_DEFINE(ST) TMC26XStepper stepper##ST(200, ST##_CS_PIN, ST##_STEP_PIN, ST##_DIR_PIN, ST##_MAX_CURRENT, ST##_SENSE_RESISTOR)

  #if AXIS_DRIVER_TYPE(X, TMC26X)
    _TMC26X_DEFINE(X);
  #endif
  #if AXIS_DRIVER_TYPE(X2, TMC26X)
    _TMC26X_DEFINE(X2);
  #endif
  #if AXIS_DRIVER_TYPE(Y, TMC26X)
    _TMC26X_DEFINE(Y);
  #endif
  #if AXIS_DRIVER_TYPE(Y2, TMC26X)
    _TMC26X_DEFINE(Y2);
  #endif
  #if AXIS_DRIVER_TYPE(Z, TMC26X)
    _TMC26X_DEFINE(Z);
  #endif
  #if AXIS_DRIVER_TYPE(Z2, TMC26X)
    _TMC26X_DEFINE(Z2);
  #endif
  #if AXIS_DRIVER_TYPE(E0, TMC26X)
    _TMC26X_DEFINE(E0);
  #endif
  #if AXIS_DRIVER_TYPE(E1, TMC26X)
    _TMC26X_DEFINE(E1);
  #endif
  #if AXIS_DRIVER_TYPE(E2, TMC26X)
    _TMC26X_DEFINE(E2);
  #endif
  #if AXIS_DRIVER_TYPE(E3, TMC26X)
    _TMC26X_DEFINE(E3);
  #endif
  #if AXIS_DRIVER_TYPE(E4, TMC26X)
    _TMC26X_DEFINE(E4);
  #endif

  #define _TMC26X_INIT(A) do{ \
    stepper##A.setMicrosteps(A##_MICROSTEPS); \
    stepper##A.start(); \
  }while(0)

  void tmc26x_init_to_defaults() {
    #if AXIS_DRIVER_TYPE(X, TMC26X)
      _TMC26X_INIT(X);
    #endif
    #if AXIS_DRIVER_TYPE(X2, TMC26X)
      _TMC26X_INIT(X2);
    #endif
    #if AXIS_DRIVER_TYPE(Y, TMC26X)
      _TMC26X_INIT(Y);
    #endif
    #if AXIS_DRIVER_TYPE(Y2, TMC26X)
      _TMC26X_INIT(Y2);
    #endif
    #if AXIS_DRIVER_TYPE(Z, TMC26X)
      _TMC26X_INIT(Z);
    #endif
    #if AXIS_DRIVER_TYPE(Z2, TMC26X)
      _TMC26X_INIT(Z2);
    #endif
    #if AXIS_DRIVER_TYPE(E0, TMC26X)
      _TMC26X_INIT(E0);
    #endif
    #if AXIS_DRIVER_TYPE(E1, TMC26X)
      _TMC26X_INIT(E1);
    #endif
    #if AXIS_DRIVER_TYPE(E2, TMC26X)
      _TMC26X_INIT(E2);
    #endif
    #if AXIS_DRIVER_TYPE(E3, TMC26X)
      _TMC26X_INIT(E3);
    #endif
    #if AXIS_DRIVER_TYPE(E4, TMC26X)
      _TMC26X_INIT(E4);
    #endif
  }
#endif // TMC26X

//
// TMC2130 Driver objects and inits
//
#if HAS_DRIVER(TMC2130)

  #include <SPI.h>
  #include <TMC2130Stepper.h>
  #include "planner.h"
  #include "enum.h"

  #if TMC2130STEPPER_VERSION < 0x020201
    #error "Update TMC2130Stepper library to 2.2.1 or newer."
  #endif

  #if ENABLED(TMC_USE_SW_SPI)
    #define _TMC2130_DEFINE(ST) TMC2130Stepper stepper##ST(ST##_ENABLE_PIN, ST##_DIR_PIN, ST##_STEP_PIN, ST##_CS_PIN, TMC_SW_MOSI, TMC_SW_MISO, TMC_SW_SCK)
  #else
    #define _TMC2130_DEFINE(ST) TMC2130Stepper stepper##ST(ST##_ENABLE_PIN, ST##_DIR_PIN, ST##_STEP_PIN, ST##_CS_PIN)
  #endif

  // Stepper objects of TMC2130 steppers used
  #if AXIS_DRIVER_TYPE(X, TMC2130)
    _TMC2130_DEFINE(X);
  #endif
  #if AXIS_DRIVER_TYPE(X2, TMC2130)
    _TMC2130_DEFINE(X2);
  #endif
  #if AXIS_DRIVER_TYPE(Y, TMC2130)
    _TMC2130_DEFINE(Y);
  #endif
  #if AXIS_DRIVER_TYPE(Y2, TMC2130)
    _TMC2130_DEFINE(Y2);
  #endif
  #if AXIS_DRIVER_TYPE(Z, TMC2130)
    _TMC2130_DEFINE(Z);
  #endif
  #if AXIS_DRIVER_TYPE(Z2, TMC2130)
    _TMC2130_DEFINE(Z2);
  #endif
  #if AXIS_DRIVER_TYPE(E0, TMC2130)
    _TMC2130_DEFINE(E0);
  #endif
  #if AXIS_DRIVER_TYPE(E1, TMC2130)
    _TMC2130_DEFINE(E1);
  #endif
  #if AXIS_DRIVER_TYPE(E2, TMC2130)
    _TMC2130_DEFINE(E2);
  #endif
  #if AXIS_DRIVER_TYPE(E3, TMC2130)
    _TMC2130_DEFINE(E3);
  #endif
  #if AXIS_DRIVER_TYPE(E4, TMC2130)
    _TMC2130_DEFINE(E4);
  #endif

  // Use internal reference voltage for current calculations. This is the default.
  // Following values from Trinamic's spreadsheet with values for a NEMA17 (42BYGHW609)
  // https://www.trinamic.com/products/integrated-circuits/details/tmc2130/
  void tmc2130_init(TMC2130Stepper &st, const uint16_t mA, const uint16_t microsteps, const uint32_t thrs, const float spmm) {
    #if DISABLED(STEALTHCHOP) || DISABLED(HYBRID_THRESHOLD)
      UNUSED(thrs);
      UNUSED(spmm);
    #endif
    st.begin();
    st.setCurrent(mA, R_SENSE, HOLD_MULTIPLIER);
    st.microsteps(microsteps);
    st.blank_time(24);
    st.off_time(5); // Only enables the driver if used with stealthChop
    st.interpolate(INTERPOLATE);
    st.power_down_delay(128); // ~2s until driver lowers to hold current
    st.hysteresis_start(3);
    st.hysteresis_end(2);
    #if ENABLED(STEALTHCHOP)
      st.stealth_freq(1); // f_pwm = 2/683 f_clk
      st.stealth_autoscale(1);
      st.stealth_gradient(5);
      st.stealth_amplitude(255);
      st.stealthChop(1);
      #if ENABLED(HYBRID_THRESHOLD)
        st.stealth_max_speed(12650000UL*microsteps/(256*thrs*spmm));
      #endif
    #endif
    st.GSTAT(); // Clear GSTAT
  }

  #define _TMC2130_INIT(ST, SPMM) tmc2130_init(stepper##ST, ST##_CURRENT, ST##_MICROSTEPS, ST##_HYBRID_THRESHOLD, SPMM)

  void tmc2130_init_to_defaults() {
    #if AXIS_DRIVER_TYPE(X, TMC2130)
      _TMC2130_INIT( X, planner.axis_steps_per_mm[X_AXIS]);
    #endif
    #if AXIS_DRIVER_TYPE(X2, TMC2130)
      _TMC2130_INIT(X2, planner.axis_steps_per_mm[X_AXIS]);
    #endif
    #if AXIS_DRIVER_TYPE(Y, TMC2130)
      _TMC2130_INIT( Y, planner.axis_steps_per_mm[Y_AXIS]);
    #endif
    #if AXIS_DRIVER_TYPE(Y2, TMC2130)
      _TMC2130_INIT(Y2, planner.axis_steps_per_mm[Y_AXIS]);
    #endif
    #if AXIS_DRIVER_TYPE(Z, TMC2130)
      _TMC2130_INIT( Z, planner.axis_steps_per_mm[Z_AXIS]);
    #endif
    #if AXIS_DRIVER_TYPE(Z2, TMC2130)
      _TMC2130_INIT(Z2, planner.axis_steps_per_mm[Z_AXIS]);
    #endif
    #if AXIS_DRIVER_TYPE(E0, TMC2130)
      _TMC2130_INIT(E0, planner.axis_steps_per_mm[E_AXIS]);
    #endif
    #if AXIS_DRIVER_TYPE(E1, TMC2130)
      { constexpr int extruder = 1; _TMC2130_INIT(E1, planner.axis_steps_per_mm[E_AXIS_N]); }
    #endif
    #if AXIS_DRIVER_TYPE(E2, TMC2130)
      { constexpr int extruder = 2; _TMC2130_INIT(E2, planner.axis_steps_per_mm[E_AXIS_N]); }
    #endif
    #if AXIS_DRIVER_TYPE(E3, TMC2130)
      { constexpr int extruder = 3; _TMC2130_INIT(E3, planner.axis_steps_per_mm[E_AXIS_N]); }
    #endif
    #if AXIS_DRIVER_TYPE(E4, TMC2130)
      { constexpr int extruder = 4; _TMC2130_INIT(E4, planner.axis_steps_per_mm[E_AXIS_N]); }
    #endif

    #if ENABLED(SENSORLESS_HOMING)
      #define TMC_INIT_SGT(P,Q) stepper##Q.sgt(P##_HOMING_SENSITIVITY);
      #if X_SENSORLESS
        #if AXIS_DRIVER_TYPE(X, TMC2130)
          stepperX.sgt(X_HOMING_SENSITIVITY);
        #endif
        #if AXIS_DRIVER_TYPE(X2, TMC2130)
          stepperX2.sgt(X_HOMING_SENSITIVITY);
        #endif
      #endif
      #if Y_SENSORLESS
        #if AXIS_DRIVER_TYPE(Y, TMC2130)
          stepperY.sgt(Y_HOMING_SENSITIVITY);
        #endif
        #if AXIS_DRIVER_TYPE(Y2, TMC2130)
          stepperY2.sgt(Y_HOMING_SENSITIVITY);
        #endif
      #endif
      #if Z_SENSORLESS
        #if AXIS_DRIVER_TYPE(Z, TMC2130)
          stepperZ.sgt(Z_HOMING_SENSITIVITY);
        #endif
        #if AXIS_DRIVER_TYPE(Z2, TMC2130)
          stepperZ2.sgt(Z_HOMING_SENSITIVITY);
        #endif
      #endif
    #endif
  }
#endif // TMC2130

//
// TMC2208 Driver objects and inits
//
#if HAS_DRIVER(TMC2208)

  #undef HardwareSerial_h // undo Marlin trickery
  #include <SoftwareSerial.h>
  #include <HardwareSerial.h>
  #include <TMC2208Stepper.h>
  #include "planner.h"

  #if TMC2208STEPPER_VERSION < 0x000101
    #error "Update TMC2208Stepper library to 0.1.1 or newer."
  #endif

  #define _TMC2208_DEFINE_HARDWARE(ST) TMC2208Stepper stepper##ST(&ST##_HARDWARE_SERIAL)
  #define _TMC2208_DEFINE_SOFTWARE(ST) TMC2208Stepper stepper##ST(ST##_SERIAL_RX_PIN, ST##_SERIAL_TX_PIN, ST##_SERIAL_RX_PIN > -1)

  // Stepper objects of TMC2208 steppers used
  #if AXIS_DRIVER_TYPE(X, TMC2208)
    #ifdef X_HARDWARE_SERIAL
      _TMC2208_DEFINE_HARDWARE(X);
    #else
      _TMC2208_DEFINE_SOFTWARE(X);
    #endif
  #endif
  #if AXIS_DRIVER_TYPE(X2, TMC2208)
    #ifdef X2_HARDWARE_SERIAL
      _TMC2208_DEFINE_HARDWARE(X2);
    #else
      _TMC2208_DEFINE_SOFTWARE(X2);
    #endif
  #endif
  #if AXIS_DRIVER_TYPE(Y, TMC2208)
    #ifdef Y_HARDWARE_SERIAL
      _TMC2208_DEFINE_HARDWARE(Y);
    #else
      _TMC2208_DEFINE_SOFTWARE(Y);
    #endif
  #endif
  #if AXIS_DRIVER_TYPE(Y2, TMC2208)
    #ifdef Y2_HARDWARE_SERIAL
      _TMC2208_DEFINE_HARDWARE(Y2);
    #else
      _TMC2208_DEFINE_SOFTWARE(Y2);
    #endif
  #endif
  #if AXIS_DRIVER_TYPE(Z, TMC2208)
    #ifdef Z_HARDWARE_SERIAL
      _TMC2208_DEFINE_HARDWARE(Z);
    #else
      _TMC2208_DEFINE_SOFTWARE(Z);
    #endif
  #endif
  #if AXIS_DRIVER_TYPE(Z2, TMC2208)
    #ifdef Z2_HARDWARE_SERIAL
      _TMC2208_DEFINE_HARDWARE(Z2);
    #else
      _TMC2208_DEFINE_SOFTWARE(Z2);
    #endif
  #endif
  #if AXIS_DRIVER_TYPE(E0, TMC2208)
    #ifdef E0_HARDWARE_SERIAL
      _TMC2208_DEFINE_HARDWARE(E0);
    #else
      _TMC2208_DEFINE_SOFTWARE(E0);
    #endif
  #endif
  #if AXIS_DRIVER_TYPE(E1, TMC2208)
    #ifdef E1_HARDWARE_SERIAL
      _TMC2208_DEFINE_HARDWARE(E1);
    #else
      _TMC2208_DEFINE_SOFTWARE(E1);
    #endif
  #endif
  #if AXIS_DRIVER_TYPE(E2, TMC2208)
    #ifdef E2_HARDWARE_SERIAL
      _TMC2208_DEFINE_HARDWARE(E2);
    #else
      _TMC2208_DEFINE_SOFTWARE(E2);
    #endif
  #endif
  #if AXIS_DRIVER_TYPE(E3, TMC2208)
    #ifdef E3_HARDWARE_SERIAL
      _TMC2208_DEFINE_HARDWARE(E3);
    #else
      _TMC2208_DEFINE_SOFTWARE(E3);
    #endif
  #endif
  #if AXIS_DRIVER_TYPE(E4, TMC2208)
    #ifdef E4_HARDWARE_SERIAL
      _TMC2208_DEFINE_HARDWARE(E4);
    #else
      _TMC2208_DEFINE_SOFTWARE(E4);
    #endif
  #endif

  void tmc2208_serial_begin() {
    #if AXIS_DRIVER_TYPE(X, TMC2208)
      #ifdef X_HARDWARE_SERIAL
        X_HARDWARE_SERIAL.begin(115200);
      #else
        stepperX.beginSerial(115200);
      #endif
    #endif
    #if AXIS_DRIVER_TYPE(X2, TMC2208)
      #ifdef X2_HARDWARE_SERIAL
        X2_HARDWARE_SERIAL.begin(115200);
      #else
        stepperX2.beginSerial(115200);
      #endif
    #endif
    #if AXIS_DRIVER_TYPE(Y, TMC2208)
      #ifdef Y_HARDWARE_SERIAL
        Y_HARDWARE_SERIAL.begin(115200);
      #else
        stepperY.beginSerial(115200);
      #endif
    #endif
    #if AXIS_DRIVER_TYPE(Y2, TMC2208)
      #ifdef Y2_HARDWARE_SERIAL
        Y2_HARDWARE_SERIAL.begin(115200);
      #else
        stepperY2.beginSerial(115200);
      #endif
    #endif
    #if AXIS_DRIVER_TYPE(Z, TMC2208)
      #ifdef Z_HARDWARE_SERIAL
        Z_HARDWARE_SERIAL.begin(115200);
      #else
        stepperZ.beginSerial(115200);
      #endif
    #endif
    #if AXIS_DRIVER_TYPE(Z2, TMC2208)
      #ifdef Z2_HARDWARE_SERIAL
        Z2_HARDWARE_SERIAL.begin(115200);
      #else
        stepperZ2.beginSerial(115200);
      #endif
    #endif
    #if AXIS_DRIVER_TYPE(E0, TMC2208)
      #ifdef E0_HARDWARE_SERIAL
        E0_HARDWARE_SERIAL.begin(115200);
      #else
        stepperE0.beginSerial(115200);
      #endif
    #endif
    #if AXIS_DRIVER_TYPE(E1, TMC2208)
      #ifdef E1_HARDWARE_SERIAL
        E1_HARDWARE_SERIAL.begin(115200);
      #else
        stepperE1.beginSerial(115200);
      #endif
    #endif
    #if AXIS_DRIVER_TYPE(E2, TMC2208)
      #ifdef E2_HARDWARE_SERIAL
        E2_HARDWARE_SERIAL.begin(115200);
      #else
        stepperE2.beginSerial(115200);
      #endif
    #endif
    #if AXIS_DRIVER_TYPE(E3, TMC2208)
      #ifdef E3_HARDWARE_SERIAL
        E3_HARDWARE_SERIAL.begin(115200);
      #else
        stepperE3.beginSerial(115200);
      #endif
    #endif
    #if AXIS_DRIVER_TYPE(E4, TMC2208)
      #ifdef E4_HARDWARE_SERIAL
        E4_HARDWARE_SERIAL.begin(115200);
      #else
        stepperE4.beginSerial(115200);
      #endif
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
    #if AXIS_DRIVER_TYPE(X, TMC2208)
      _TMC2208_INIT(X, planner.axis_steps_per_mm[X_AXIS]);
    #endif
    #if AXIS_DRIVER_TYPE(X2, TMC2208)
      _TMC2208_INIT(X2, planner.axis_steps_per_mm[X_AXIS]);
    #endif
    #if AXIS_DRIVER_TYPE(Y, TMC2208)
      _TMC2208_INIT(Y, planner.axis_steps_per_mm[Y_AXIS]);
    #endif
    #if AXIS_DRIVER_TYPE(Y2, TMC2208)
      _TMC2208_INIT(Y2, planner.axis_steps_per_mm[Y_AXIS]);
    #endif
    #if AXIS_DRIVER_TYPE(Z, TMC2208)
      _TMC2208_INIT(Z, planner.axis_steps_per_mm[Z_AXIS]);
    #endif
    #if AXIS_DRIVER_TYPE(Z2, TMC2208)
      _TMC2208_INIT(Z2, planner.axis_steps_per_mm[Z_AXIS]);
    #endif
    #if AXIS_DRIVER_TYPE(E0, TMC2208)
      _TMC2208_INIT(E0, planner.axis_steps_per_mm[E_AXIS]);
    #endif
    #if AXIS_DRIVER_TYPE(E1, TMC2208)
      { constexpr int extruder = 1; _TMC2208_INIT(E1, planner.axis_steps_per_mm[E_AXIS_N]); }
    #endif
    #if AXIS_DRIVER_TYPE(E2, TMC2208)
      { constexpr int extruder = 2; _TMC2208_INIT(E2, planner.axis_steps_per_mm[E_AXIS_N]); }
    #endif
    #if AXIS_DRIVER_TYPE(E3, TMC2208)
      { constexpr int extruder = 3; _TMC2208_INIT(E3, planner.axis_steps_per_mm[E_AXIS_N]); }
    #endif
    #if AXIS_DRIVER_TYPE(E4, TMC2208)
      { constexpr int extruder = 4; _TMC2208_INIT(E4, planner.axis_steps_per_mm[E_AXIS_N]); }
    #endif
  }
#endif // TMC2208

void restore_stepper_drivers() {
  #if AXIS_IS_TMC(X)
    stepperX.push();
  #endif
  #if AXIS_IS_TMC(X2)
    stepperX2.push();
  #endif
  #if AXIS_IS_TMC(Y)
    stepperY.push();
  #endif
  #if AXIS_IS_TMC(Y2)
    stepperY2.push();
  #endif
  #if AXIS_IS_TMC(Z)
    stepperZ.push();
  #endif
  #if AXIS_IS_TMC(Z2)
    stepperZ2.push();
  #endif
  #if AXIS_IS_TMC(E0)
    stepperE0.push();
  #endif
  #if AXIS_IS_TMC(E1)
    stepperE1.push();
  #endif
  #if AXIS_IS_TMC(E2)
    stepperE2.push();
  #endif
  #if AXIS_IS_TMC(E3)
    stepperE3.push();
  #endif
  #if AXIS_IS_TMC(E4)
    stepperE4.push();
  #endif
}

void reset_stepper_drivers() {
  #if HAS_DRIVER(TMC26X)
    tmc26x_init_to_defaults();
  #endif
  #if HAS_DRIVER(TMC2130)
    delay(100);
    tmc2130_init_to_defaults();
  #endif
  #if HAS_DRIVER(TMC2208)
    delay(100);
    tmc2208_init_to_defaults();
  #endif
  #ifdef TMC_ADV
    TMC_ADV()
  #endif
  #if HAS_DRIVER(L6470)
    L6470_init_to_defaults();
  #endif
}

//
// L6470 Driver objects and inits
//
#if HAS_DRIVER(L6470)

  #include <SPI.h>
  #include <L6470.h>

  #define _L6470_DEFINE(ST) L6470 stepper##ST(ST##_ENABLE_PIN)

  // L6470 Stepper objects
  #if AXIS_DRIVER_TYPE(X, L6470)
    _L6470_DEFINE(X);
  #endif
  #if AXIS_DRIVER_TYPE(X2, L6470)
    _L6470_DEFINE(X2);
  #endif
  #if AXIS_DRIVER_TYPE(Y, L6470)
    _L6470_DEFINE(Y);
  #endif
  #if AXIS_DRIVER_TYPE(Y2, L6470)
    _L6470_DEFINE(Y2);
  #endif
  #if AXIS_DRIVER_TYPE(Z, L6470)
    _L6470_DEFINE(Z);
  #endif
  #if AXIS_DRIVER_TYPE(Z2, L6470)
    _L6470_DEFINE(Z2);
  #endif
  #if AXIS_DRIVER_TYPE(E0, L6470)
    _L6470_DEFINE(E0);
  #endif
  #if AXIS_DRIVER_TYPE(E1, L6470)
    _L6470_DEFINE(E1);
  #endif
  #if AXIS_DRIVER_TYPE(E2, L6470)
    _L6470_DEFINE(E2);
  #endif
  #if AXIS_DRIVER_TYPE(E3, L6470)
    _L6470_DEFINE(E3);
  #endif
  #if AXIS_DRIVER_TYPE(E4, L6470)
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
    #if AXIS_DRIVER_TYPE(X, L6470)
      _L6470_INIT(X);
    #endif
    #if AXIS_DRIVER_TYPE(X2, L6470)
      _L6470_INIT(X2);
    #endif
    #if AXIS_DRIVER_TYPE(Y, L6470)
      _L6470_INIT(Y);
    #endif
    #if AXIS_DRIVER_TYPE(Y2, L6470)
      _L6470_INIT(Y2);
    #endif
    #if AXIS_DRIVER_TYPE(Z, L6470)
      _L6470_INIT(Z);
    #endif
    #if AXIS_DRIVER_TYPE(Z2, L6470)
      _L6470_INIT(Z2);
    #endif
    #if AXIS_DRIVER_TYPE(E0, L6470)
      _L6470_INIT(E0);
    #endif
    #if AXIS_DRIVER_TYPE(E1, L6470)
      _L6470_INIT(E1);
    #endif
    #if AXIS_DRIVER_TYPE(E2, L6470)
      _L6470_INIT(E2);
    #endif
    #if AXIS_DRIVER_TYPE(E3, L6470)
      _L6470_INIT(E3);
    #endif
    #if AXIS_DRIVER_TYPE(E4, L6470)
      _L6470_INIT(E4);
    #endif
  }

#endif // L6470
