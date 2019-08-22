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
#pragma once

/**
 * stepper_indirection.h
 *
 * Stepper motor driver indirection to allow some stepper functions to
 * be done via SPI/I2c instead of direct pin manipulation.
 *
 * Copyright (c) 2015 Dominik Wenger
 */

#include "../inc/MarlinConfig.h"

// TMC26X drivers have STEP/DIR on normal pins, but ENABLE via SPI
#if HAS_DRIVER(TMC26X)
  #include <SPI.h>
  #if defined(STM32GENERIC) && defined(STM32F7)
    #include "../HAL/HAL_STM32_F4_F7/STM32F7/TMC2660.h"
  #else
    #include <TMC26XStepper.h>
  #endif
  void tmc26x_init_to_defaults();
#endif

#if HAS_TRINAMIC
  #include <TMCStepper.h>
  #include "../feature/tmc_util.h"
  #if TMCSTEPPER_VERSION < 0x000405
    #error "Update TMCStepper library to 0.4.5 or newer."
  #endif

  #define ____TMC_CLASS(MODEL, A, I, E) TMCMarlin<TMC##MODEL##Stepper, A, I, E>
  #define ___TMC_CLASS(MODEL, A, I, E) ____TMC_CLASS(MODEL, A, I, E)
  #define __TMC_CLASS(MODEL, A, I, E) ___TMC_CLASS(_##MODEL, A, I, E)
  #define _TMC_CLASS(MODEL, L, E) __TMC_CLASS(MODEL, L, E)
  #define TMC_CLASS(ST, A) _TMC_CLASS(ST##_DRIVER_TYPE, TMC_##ST##_LABEL, A##_AXIS)
  #if ENABLED(DISTINCT_E_FACTORS)
    #define TMC_CLASS_E(I) TMC_CLASS(E##I, E##I)
  #else
    #define TMC_CLASS_E(I) TMC_CLASS(E##I, E)
  #endif

  typedef struct {
    uint8_t toff;
    int8_t hend;
    uint8_t hstrt;
  } chopper_timing_t;

  static constexpr chopper_timing_t chopper_timing = CHOPPER_TIMING;

  #if HAS_TMC220x
    void tmc_serial_begin();
  #endif
#endif

// L6470 has STEP on normal pins, but DIR/ENABLE via SPI
#if HAS_DRIVER(L6470)
  #include "L6470/L6470_Marlin.h"
  #define L6470_WRITE_DIR_COMMAND(STATE,Q) do{ L6470_dir_commands[Q] = (STATE ?  dSPIN_STEP_CLOCK_REV : dSPIN_STEP_CLOCK_FWD); }while(0)
#endif

void restore_stepper_drivers();  // Called by PSU_ON
void reset_stepper_drivers();    // Called by settings.load / settings.reset

#define AXIS_HAS_SQUARE_WAVE(A) (AXIS_IS_TMC(A) && ENABLED(SQUARE_WAVE_STEPPING))

// X Stepper
#if AXIS_DRIVER_TYPE_X(L6470)
  extern L6470 stepperX;
  #define X_ENABLE_INIT NOOP
  #define X_ENABLE_WRITE(STATE) NOOP
  #define X_ENABLE_READ() (stepperX.getStatus() & STATUS_HIZ)
  #define X_DIR_INIT NOOP
  #define X_DIR_WRITE(STATE) L6470_WRITE_DIR_COMMAND(STATE,X)
  #define X_DIR_READ() (stepperX.getStatus() & STATUS_DIR)
#else
  #if AXIS_IS_TMC(X)
    extern TMC_CLASS(X, X) stepperX;
  #endif
  #if AXIS_DRIVER_TYPE_X(TMC26X)
    extern TMC26XStepper stepperX;
    #define X_ENABLE_INIT NOOP
    #define X_ENABLE_WRITE(STATE) stepperX.setEnabled(STATE)
    #define X_ENABLE_READ() stepperX.isEnabled()
  #elif ENABLED(SOFTWARE_DRIVER_ENABLE) && AXIS_IS_TMC(X)
    #define X_ENABLE_INIT NOOP
    #define X_ENABLE_WRITE(STATE) stepperX.toff((STATE)==X_ENABLE_ON ? chopper_timing.toff : 0)
    #define X_ENABLE_READ() stepperX.isEnabled()
  #else
    #define X_ENABLE_INIT SET_OUTPUT(X_ENABLE_PIN)
    #define X_ENABLE_WRITE(STATE) WRITE(X_ENABLE_PIN,STATE)
    #define X_ENABLE_READ() READ(X_ENABLE_PIN)
  #endif
  #define X_DIR_INIT SET_OUTPUT(X_DIR_PIN)
  #define X_DIR_WRITE(STATE) WRITE(X_DIR_PIN,STATE)
  #define X_DIR_READ() READ(X_DIR_PIN)
#endif
#define X_STEP_INIT SET_OUTPUT(X_STEP_PIN)
#if AXIS_HAS_SQUARE_WAVE(X)
  #define X_STEP_WRITE(STATE) do{ if(STATE) TOGGLE(X_STEP_PIN); }while(0)
#else
  #define X_STEP_WRITE(STATE) WRITE(X_STEP_PIN,STATE)
#endif
#define X_STEP_READ READ(X_STEP_PIN)

// Y Stepper
#if AXIS_DRIVER_TYPE_Y(L6470)
  extern L6470 stepperY;
  #define Y_ENABLE_INIT NOOP
  #define Y_ENABLE_WRITE(STATE) NOOP
  #define Y_ENABLE_READ() (stepperY.getStatus() & STATUS_HIZ)
  #define Y_DIR_INIT NOOP
  #define Y_DIR_WRITE(STATE) L6470_WRITE_DIR_COMMAND(STATE,Y)
  #define Y_DIR_READ() (stepperY.getStatus() & STATUS_DIR)
#else
  #if AXIS_IS_TMC(Y)
    extern TMC_CLASS(Y, Y) stepperY;
  #endif
  #if AXIS_DRIVER_TYPE_Y(TMC26X)
    extern TMC26XStepper stepperY;
    #define Y_ENABLE_INIT NOOP
    #define Y_ENABLE_WRITE(STATE) stepperY.setEnabled(STATE)
    #define Y_ENABLE_READ() stepperY.isEnabled()
  #elif ENABLED(SOFTWARE_DRIVER_ENABLE) && AXIS_IS_TMC(Y)
    #define Y_ENABLE_INIT NOOP
    #define Y_ENABLE_WRITE(STATE) stepperY.toff((STATE)==Y_ENABLE_ON ? chopper_timing.toff : 0)
    #define Y_ENABLE_READ() stepperY.isEnabled()
  #else
    #define Y_ENABLE_INIT SET_OUTPUT(Y_ENABLE_PIN)
    #define Y_ENABLE_WRITE(STATE) WRITE(Y_ENABLE_PIN,STATE)
    #define Y_ENABLE_READ() READ(Y_ENABLE_PIN)
  #endif
  #define Y_DIR_INIT SET_OUTPUT(Y_DIR_PIN)
  #define Y_DIR_WRITE(STATE) WRITE(Y_DIR_PIN,STATE)
  #define Y_DIR_READ() READ(Y_DIR_PIN)
#endif
#define Y_STEP_INIT SET_OUTPUT(Y_STEP_PIN)
#if AXIS_HAS_SQUARE_WAVE(Y)
  #define Y_STEP_WRITE(STATE) do{ if (STATE) TOGGLE(Y_STEP_PIN); }while(0)
#else
  #define Y_STEP_WRITE(STATE) WRITE(Y_STEP_PIN,STATE)
#endif
#define Y_STEP_READ READ(Y_STEP_PIN)

// Z Stepper
#if AXIS_DRIVER_TYPE_Z(L6470)
  extern L6470 stepperZ;
  #define Z_ENABLE_INIT NOOP
  #define Z_ENABLE_WRITE(STATE) NOOP
  #define Z_ENABLE_READ() (stepperZ.getStatus() & STATUS_HIZ)
  #define Z_DIR_INIT NOOP
  #define Z_DIR_WRITE(STATE) L6470_WRITE_DIR_COMMAND(STATE,Z)
  #define Z_DIR_READ() (stepperZ.getStatus() & STATUS_DIR)
#else
  #if AXIS_IS_TMC(Z)
    extern TMC_CLASS(Z, Z) stepperZ;
  #endif
  #if AXIS_DRIVER_TYPE_Z(TMC26X)
    extern TMC26XStepper stepperZ;
    #define Z_ENABLE_INIT NOOP
    #define Z_ENABLE_WRITE(STATE) stepperZ.setEnabled(STATE)
    #define Z_ENABLE_READ() stepperZ.isEnabled()
  #elif ENABLED(SOFTWARE_DRIVER_ENABLE) && AXIS_IS_TMC(Z)
    #define Z_ENABLE_INIT NOOP
    #define Z_ENABLE_WRITE(STATE) stepperZ.toff((STATE)==Z_ENABLE_ON ? chopper_timing.toff : 0)
    #define Z_ENABLE_READ() stepperZ.isEnabled()
  #else
    #define Z_ENABLE_INIT SET_OUTPUT(Z_ENABLE_PIN)
    #define Z_ENABLE_WRITE(STATE) WRITE(Z_ENABLE_PIN,STATE)
    #define Z_ENABLE_READ() READ(Z_ENABLE_PIN)
  #endif
  #define Z_DIR_INIT SET_OUTPUT(Z_DIR_PIN)
  #define Z_DIR_WRITE(STATE) WRITE(Z_DIR_PIN,STATE)
  #define Z_DIR_READ() READ(Z_DIR_PIN)
#endif
#define Z_STEP_INIT SET_OUTPUT(Z_STEP_PIN)
#if AXIS_HAS_SQUARE_WAVE(Z)
  #define Z_STEP_WRITE(STATE) do{ if(STATE) TOGGLE(Z_STEP_PIN); }while(0)
#else
  #define Z_STEP_WRITE(STATE) WRITE(Z_STEP_PIN,STATE)
#endif
#define Z_STEP_READ READ(Z_STEP_PIN)

// X2 Stepper
#if HAS_X2_ENABLE
  #if AXIS_DRIVER_TYPE_X2(L6470)
    extern L6470 stepperX2;
    #define X2_ENABLE_INIT NOOP
    #define X2_ENABLE_WRITE(STATE) NOOP
    #define X2_ENABLE_READ() (stepperX2.getStatus() & STATUS_HIZ)
    #define X2_DIR_INIT NOOP
    #define X2_DIR_WRITE(STATE) L6470_WRITE_DIR_COMMAND(STATE,X2)
    #define X2_DIR_READ() (stepperX2.getStatus() & STATUS_DIR)
  #else
    #if AXIS_IS_TMC(X2)
      extern TMC_CLASS(X2, X) stepperX2;
    #endif
    #if AXIS_DRIVER_TYPE_X2(TMC26X)
      extern TMC26XStepper stepperX2;
      #define X2_ENABLE_INIT NOOP
      #define X2_ENABLE_WRITE(STATE) stepperX2.setEnabled(STATE)
      #define X2_ENABLE_READ() stepperX2.isEnabled()
    #elif ENABLED(SOFTWARE_DRIVER_ENABLE) && AXIS_IS_TMC(X2)
      #define X2_ENABLE_INIT NOOP
      #define X2_ENABLE_WRITE(STATE) stepperX2.toff((STATE)==X_ENABLE_ON ? chopper_timing.toff : 0)
      #define X2_ENABLE_READ() stepperX2.isEnabled()
    #else
      #define X2_ENABLE_INIT SET_OUTPUT(X2_ENABLE_PIN)
      #define X2_ENABLE_WRITE(STATE) WRITE(X2_ENABLE_PIN,STATE)
      #define X2_ENABLE_READ() READ(X2_ENABLE_PIN)
    #endif
    #define X2_DIR_INIT SET_OUTPUT(X2_DIR_PIN)
    #define X2_DIR_WRITE(STATE) WRITE(X2_DIR_PIN,STATE)
    #define X2_DIR_READ() READ(X2_DIR_PIN)
  #endif
  #define X2_STEP_INIT SET_OUTPUT(X2_STEP_PIN)
  #if AXIS_HAS_SQUARE_WAVE(X2)
    #define X2_STEP_WRITE(STATE) do{ if(STATE) TOGGLE(X2_STEP_PIN); }while(0)
  #else
    #define X2_STEP_WRITE(STATE) WRITE(X2_STEP_PIN,STATE)
  #endif

  #define X2_STEP_READ READ(X2_STEP_PIN)
#endif

// Y2 Stepper
#if HAS_Y2_ENABLE
  #if AXIS_DRIVER_TYPE_Y2(L6470)
    extern L6470 stepperY2;
    #define Y2_ENABLE_INIT NOOP
    #define Y2_ENABLE_WRITE(STATE) NOOP
    #define Y2_ENABLE_READ() (stepperY2.getStatus() & STATUS_HIZ)
    #define Y2_DIR_INIT NOOP
    #define Y2_DIR_WRITE(STATE) L6470_WRITE_DIR_COMMAND(STATE,Y2)
    #define Y2_DIR_READ() (stepperY2.getStatus() & STATUS_DIR)
  #else
    #if AXIS_IS_TMC(Y2)
      extern TMC_CLASS(Y2, Y) stepperY2;
    #endif
    #if AXIS_DRIVER_TYPE_Y2(TMC26X)
      extern TMC26XStepper stepperY2;
      #define Y2_ENABLE_INIT NOOP
      #define Y2_ENABLE_WRITE(STATE) stepperY2.setEnabled(STATE)
      #define Y2_ENABLE_READ() stepperY2.isEnabled()
    #elif ENABLED(SOFTWARE_DRIVER_ENABLE) && AXIS_IS_TMC(Y2)
      #define Y2_ENABLE_INIT NOOP
      #define Y2_ENABLE_WRITE(STATE) stepperY2.toff((STATE)==Y_ENABLE_ON ? chopper_timing.toff : 0)
      #define Y2_ENABLE_READ() stepperY2.isEnabled()
    #else
      #define Y2_ENABLE_INIT SET_OUTPUT(Y2_ENABLE_PIN)
      #define Y2_ENABLE_WRITE(STATE) WRITE(Y2_ENABLE_PIN,STATE)
      #define Y2_ENABLE_READ() READ(Y2_ENABLE_PIN)
    #endif
    #define Y2_DIR_INIT SET_OUTPUT(Y2_DIR_PIN)
    #define Y2_DIR_WRITE(STATE) WRITE(Y2_DIR_PIN,STATE)
    #define Y2_DIR_READ() READ(Y2_DIR_PIN)
  #endif
  #define Y2_STEP_INIT SET_OUTPUT(Y2_STEP_PIN)
  #if AXIS_HAS_SQUARE_WAVE(Y2)
    #define Y2_STEP_WRITE(STATE) do{ if(STATE) TOGGLE(Y2_STEP_PIN); }while(0)
  #else
    #define Y2_STEP_WRITE(STATE) WRITE(Y2_STEP_PIN,STATE)
  #endif

  #define Y2_STEP_READ READ(Y2_STEP_PIN)
#else
  #define Y2_DIR_WRITE(STATE) NOOP
#endif

// Z2 Stepper
#if HAS_Z2_ENABLE
  #if AXIS_DRIVER_TYPE_Z2(L6470)
    extern L6470 stepperZ2;
    #define Z2_ENABLE_INIT NOOP
    #define Z2_ENABLE_WRITE(STATE) NOOP
    #define Z2_ENABLE_READ() (stepperZ2.getStatus() & STATUS_HIZ)
    #define Z2_DIR_INIT NOOP
    #define Z2_DIR_WRITE(STATE) L6470_WRITE_DIR_COMMAND(STATE,Z2)
    #define Z2_DIR_READ() (stepperZ2.getStatus() & STATUS_DIR)
  #else
    #if AXIS_IS_TMC(Z2)
      extern TMC_CLASS(Z2, Z) stepperZ2;
    #endif
    #if AXIS_DRIVER_TYPE_Z2(TMC26X)
      extern TMC26XStepper stepperZ2;
      #define Z2_ENABLE_INIT NOOP
      #define Z2_ENABLE_WRITE(STATE) stepperZ2.setEnabled(STATE)
      #define Z2_ENABLE_READ() stepperZ2.isEnabled()
    #elif ENABLED(SOFTWARE_DRIVER_ENABLE) && AXIS_IS_TMC(Z2)
      #define Z2_ENABLE_INIT NOOP
      #define Z2_ENABLE_WRITE(STATE) stepperZ2.toff((STATE)==Z_ENABLE_ON ? chopper_timing.toff : 0)
      #define Z2_ENABLE_READ() stepperZ2.isEnabled()
    #else
      #define Z2_ENABLE_INIT SET_OUTPUT(Z2_ENABLE_PIN)
      #define Z2_ENABLE_WRITE(STATE) WRITE(Z2_ENABLE_PIN,STATE)
      #define Z2_ENABLE_READ() READ(Z2_ENABLE_PIN)
    #endif
    #define Z2_DIR_INIT SET_OUTPUT(Z2_DIR_PIN)
    #define Z2_DIR_WRITE(STATE) WRITE(Z2_DIR_PIN,STATE)
    #define Z2_DIR_READ() READ(Z2_DIR_PIN)
  #endif
  #define Z2_STEP_INIT SET_OUTPUT(Z2_STEP_PIN)
  #if AXIS_HAS_SQUARE_WAVE(Z2)
    #define Z2_STEP_WRITE(STATE) do{ if(STATE) TOGGLE(Z2_STEP_PIN); }while(0)
  #else
    #define Z2_STEP_WRITE(STATE) WRITE(Z2_STEP_PIN,STATE)
  #endif

  #define Z2_STEP_READ READ(Z2_STEP_PIN)
#else
  #define Z2_DIR_WRITE(STATE) NOOP
#endif

// Z3 Stepper
#if HAS_Z3_ENABLE
  #if AXIS_DRIVER_TYPE_Z3(L6470)
    extern L6470 stepperZ3;
    #define Z3_ENABLE_INIT NOOP
    #define Z3_ENABLE_WRITE(STATE) NOOP
    #define Z3_ENABLE_READ() (stepperZ3.getStatus() & STATUS_HIZ)
    #define Z3_DIR_INIT NOOP
    #define Z3_DIR_WRITE(STATE) L6470_WRITE_DIR_COMMAND(STATE,Z3)
    #define Z3_DIR_READ() (stepperZ3.getStatus() & STATUS_DIR)
  #else
    #if AXIS_IS_TMC(Z3)
      extern TMC_CLASS(Z3, Z) stepperZ3;
    #endif
    #if ENABLED(Z3_IS_TMC26X)
      extern TMC26XStepper stepperZ3;
      #define Z3_ENABLE_INIT NOOP
      #define Z3_ENABLE_WRITE(STATE) stepperZ3.setEnabled(STATE)
      #define Z3_ENABLE_READ() stepperZ3.isEnabled()
    #elif ENABLED(SOFTWARE_DRIVER_ENABLE) && AXIS_IS_TMC(Z3)
      #define Z3_ENABLE_INIT NOOP
      #define Z3_ENABLE_WRITE(STATE) stepperZ3.toff((STATE)==Z_ENABLE_ON ? chopper_timing.toff : 0)
      #define Z3_ENABLE_READ() stepperZ3.isEnabled()
    #else
      #define Z3_ENABLE_INIT SET_OUTPUT(Z3_ENABLE_PIN)
      #define Z3_ENABLE_WRITE(STATE) WRITE(Z3_ENABLE_PIN,STATE)
      #define Z3_ENABLE_READ() READ(Z3_ENABLE_PIN)
    #endif
    #define Z3_DIR_INIT SET_OUTPUT(Z3_DIR_PIN)
    #define Z3_DIR_WRITE(STATE) WRITE(Z3_DIR_PIN,STATE)
    #define Z3_DIR_READ() READ(Z3_DIR_PIN)
  #endif
  #define Z3_STEP_INIT SET_OUTPUT(Z3_STEP_PIN)
  #if AXIS_HAS_SQUARE_WAVE(Z3)
    #define Z3_STEP_WRITE(STATE) do{ if(STATE) TOGGLE(Z3_STEP_PIN); }while(0)
  #else
    #define Z3_STEP_WRITE(STATE) WRITE(Z3_STEP_PIN,STATE)
  #endif

  #define Z3_STEP_READ READ(Z3_STEP_PIN)
#else
  #define Z3_DIR_WRITE(STATE) NOOP
#endif

// E0 Stepper
#if AXIS_DRIVER_TYPE_E0(L6470)
  extern L6470 stepperE0;
  #define E0_ENABLE_INIT NOOP
  #define E0_ENABLE_WRITE(STATE) NOOP
  #define E0_ENABLE_READ() (stepperE0.getStatus() & STATUS_HIZ)
  #define E0_DIR_INIT NOOP
  #define E0_DIR_WRITE(STATE) L6470_WRITE_DIR_COMMAND(STATE,E0)
  #define E0_DIR_READ() (stepperE0.getStatus() & STATUS_DIR)
#else
  #if AXIS_IS_TMC(E0)
    extern TMC_CLASS_E(0) stepperE0;
  #endif
  #if AXIS_DRIVER_TYPE_E0(TMC26X)
    extern TMC26XStepper stepperE0;
    #define E0_ENABLE_INIT NOOP
    #define E0_ENABLE_WRITE(STATE) stepperE0.setEnabled(STATE)
    #define E0_ENABLE_READ() stepperE0.isEnabled()
  #elif ENABLED(SOFTWARE_DRIVER_ENABLE) && AXIS_IS_TMC(E0)
    #define E0_ENABLE_INIT NOOP
    #define E0_ENABLE_WRITE(STATE) stepperE0.toff((STATE)==E_ENABLE_ON ? chopper_timing.toff : 0)
    #define E0_ENABLE_READ() stepperE0.isEnabled()
  #else
    #define E0_ENABLE_INIT SET_OUTPUT(E0_ENABLE_PIN)
    #define E0_ENABLE_WRITE(STATE) WRITE(E0_ENABLE_PIN,STATE)
    #define E0_ENABLE_READ() READ(E0_ENABLE_PIN)
  #endif
  #define E0_DIR_INIT SET_OUTPUT(E0_DIR_PIN)
  #define E0_DIR_WRITE(STATE) WRITE(E0_DIR_PIN,STATE)
  #define E0_DIR_READ() READ(E0_DIR_PIN)
#endif
#define E0_STEP_INIT SET_OUTPUT(E0_STEP_PIN)
#if AXIS_HAS_SQUARE_WAVE(E0)
  #define E0_STEP_WRITE(STATE) do{ if(STATE) TOGGLE(E0_STEP_PIN); }while(0)
#else
  #define E0_STEP_WRITE(STATE) WRITE(E0_STEP_PIN,STATE)
#endif
#define E0_STEP_READ READ(E0_STEP_PIN)

// E1 Stepper
#if AXIS_DRIVER_TYPE_E1(L6470)
  extern L6470 stepperE1;
  #define E1_ENABLE_INIT NOOP
  #define E1_ENABLE_WRITE(STATE) NOOP
  #define E1_ENABLE_READ() (stepperE1.getStatus() & STATUS_HIZ)
  #define E1_DIR_INIT NOOP
  #define E1_DIR_WRITE(STATE) L6470_WRITE_DIR_COMMAND(STATE,E1)
  #define E1_DIR_READ() (stepperE1.getStatus() & STATUS_DIR)
#else
  #if AXIS_IS_TMC(E1)
    extern TMC_CLASS_E(1) stepperE1;
  #endif
  #if AXIS_DRIVER_TYPE_E1(TMC26X)
    extern TMC26XStepper stepperE1;
    #define E1_ENABLE_INIT NOOP
    #define E1_ENABLE_WRITE(STATE) stepperE1.setEnabled(STATE)
    #define E1_ENABLE_READ() stepperE1.isEnabled()
  #elif ENABLED(SOFTWARE_DRIVER_ENABLE) && AXIS_IS_TMC(E1)
    #define E1_ENABLE_INIT NOOP
    #define E1_ENABLE_WRITE(STATE) stepperE1.toff((STATE)==E_ENABLE_ON ? chopper_timing.toff : 0)
    #define E1_ENABLE_READ() stepperE1.isEnabled()
  #else
    #define E1_ENABLE_INIT SET_OUTPUT(E1_ENABLE_PIN)
    #define E1_ENABLE_WRITE(STATE) WRITE(E1_ENABLE_PIN,STATE)
    #define E1_ENABLE_READ() READ(E1_ENABLE_PIN)
  #endif
  #define E1_DIR_INIT SET_OUTPUT(E1_DIR_PIN)
  #define E1_DIR_WRITE(STATE) WRITE(E1_DIR_PIN,STATE)
  #define E1_DIR_READ() READ(E1_DIR_PIN)
#endif
#define E1_STEP_INIT SET_OUTPUT(E1_STEP_PIN)
#if AXIS_HAS_SQUARE_WAVE(E1)
  #define E1_STEP_WRITE(STATE) do{ if(STATE) TOGGLE(E1_STEP_PIN); }while(0)
#else
  #define E1_STEP_WRITE(STATE) WRITE(E1_STEP_PIN,STATE)
#endif
#define E1_STEP_READ READ(E1_STEP_PIN)

// E2 Stepper
#if AXIS_DRIVER_TYPE_E2(L6470)
  extern L6470 stepperE2;
  #define E2_ENABLE_INIT NOOP
  #define E2_ENABLE_WRITE(STATE) NOOP
  #define E2_ENABLE_READ() (stepperE2.getStatus() & STATUS_HIZ)
  #define E2_DIR_INIT NOOP
  #define E2_DIR_WRITE(STATE) L6470_WRITE_DIR_COMMAND(STATE,E2)
  #define E2_DIR_READ() (stepperE2.getStatus() & STATUS_DIR)
#else
  #if AXIS_IS_TMC(E2)
    extern TMC_CLASS_E(2) stepperE2;
  #endif
  #if AXIS_DRIVER_TYPE_E2(TMC26X)
    extern TMC26XStepper stepperE2;
    #define E2_ENABLE_INIT NOOP
    #define E2_ENABLE_WRITE(STATE) stepperE2.setEnabled(STATE)
    #define E2_ENABLE_READ() stepperE2.isEnabled()
  #elif ENABLED(SOFTWARE_DRIVER_ENABLE) && AXIS_IS_TMC(E2)
    #define E2_ENABLE_INIT NOOP
    #define E2_ENABLE_WRITE(STATE) stepperE2.toff((STATE)==E_ENABLE_ON ? chopper_timing.toff : 0)
    #define E2_ENABLE_READ() stepperE2.isEnabled()
  #else
    #define E2_ENABLE_INIT SET_OUTPUT(E2_ENABLE_PIN)
    #define E2_ENABLE_WRITE(STATE) WRITE(E2_ENABLE_PIN,STATE)
    #define E2_ENABLE_READ() READ(E2_ENABLE_PIN)
  #endif
  #define E2_DIR_INIT SET_OUTPUT(E2_DIR_PIN)
  #define E2_DIR_WRITE(STATE) WRITE(E2_DIR_PIN,STATE)
  #define E2_DIR_READ() READ(E2_DIR_PIN)
#endif
#define E2_STEP_INIT SET_OUTPUT(E2_STEP_PIN)
#if AXIS_HAS_SQUARE_WAVE(E2)
  #define E2_STEP_WRITE(STATE) do{ if(STATE) TOGGLE(E2_STEP_PIN); }while(0)
#else
  #define E2_STEP_WRITE(STATE) WRITE(E2_STEP_PIN,STATE)
#endif
#define E2_STEP_READ READ(E2_STEP_PIN)

// E3 Stepper
#if AXIS_DRIVER_TYPE_E3(L6470)
  extern L6470 stepperE3;
  #define E3_ENABLE_INIT NOOP
  #define E3_ENABLE_WRITE(STATE) NOOP
  #define E3_ENABLE_READ() (stepperE3.getStatus() & STATUS_HIZ)
  #define E3_DIR_INIT NOOP
  #define E3_DIR_WRITE(STATE) L6470_WRITE_DIR_COMMAND(STATE,E3)
  #define E3_DIR_READ() (stepperE3.getStatus() & STATUS_DIR)
#else
  #if AXIS_IS_TMC(E3)
    extern TMC_CLASS_E(3) stepperE3;
  #endif
  #if AXIS_DRIVER_TYPE_E3(TMC26X)
    extern TMC26XStepper stepperE3;
    #define E3_ENABLE_INIT NOOP
    #define E3_ENABLE_WRITE(STATE) stepperE3.setEnabled(STATE)
    #define E3_ENABLE_READ() stepperE3.isEnabled()
  #elif ENABLED(SOFTWARE_DRIVER_ENABLE) && AXIS_IS_TMC(E3)
    #define E3_ENABLE_INIT NOOP
    #define E3_ENABLE_WRITE(STATE) stepperE3.toff((STATE)==E_ENABLE_ON ? chopper_timing.toff : 0)
    #define E3_ENABLE_READ() stepperE3.isEnabled()
  #else
    #define E3_ENABLE_INIT SET_OUTPUT(E3_ENABLE_PIN)
    #define E3_ENABLE_WRITE(STATE) WRITE(E3_ENABLE_PIN,STATE)
    #define E3_ENABLE_READ() READ(E3_ENABLE_PIN)
  #endif
  #define E3_DIR_INIT SET_OUTPUT(E3_DIR_PIN)
  #define E3_DIR_WRITE(STATE) WRITE(E3_DIR_PIN,STATE)
  #define E3_DIR_READ() READ(E3_DIR_PIN)
#endif
#define E3_STEP_INIT SET_OUTPUT(E3_STEP_PIN)
#if AXIS_HAS_SQUARE_WAVE(E3)
  #define E3_STEP_WRITE(STATE) do{ if(STATE) TOGGLE(E3_STEP_PIN); }while(0)
#else
  #define E3_STEP_WRITE(STATE) WRITE(E3_STEP_PIN,STATE)
#endif
#define E3_STEP_READ READ(E3_STEP_PIN)

// E4 Stepper
#if AXIS_DRIVER_TYPE_E4(L6470)
  extern L6470 stepperE4;
  #define E4_ENABLE_INIT NOOP
  #define E4_ENABLE_WRITE(STATE) NOOP
  #define E4_ENABLE_READ() (stepperE4.getStatus() & STATUS_HIZ)
  #define E4_DIR_INIT NOOP
  #define E4_DIR_WRITE(STATE) L6470_WRITE_DIR_COMMAND(STATE,E4)
  #define E4_DIR_READ() (stepperE4.getStatus() & STATUS_DIR)
#else
  #if AXIS_IS_TMC(E4)
    extern TMC_CLASS_E(4) stepperE4;
  #endif
  #if AXIS_DRIVER_TYPE_E4(TMC26X)
    extern TMC26XStepper stepperE4;
    #define E4_ENABLE_INIT NOOP
    #define E4_ENABLE_WRITE(STATE) stepperE4.setEnabled(STATE)
    #define E4_ENABLE_READ() stepperE4.isEnabled()
  #elif ENABLED(SOFTWARE_DRIVER_ENABLE) && AXIS_IS_TMC(E4)
    #define E4_ENABLE_INIT NOOP
    #define E4_ENABLE_WRITE(STATE) stepperE4.toff((STATE)==E_ENABLE_ON ? chopper_timing.toff : 0)
    #define E4_ENABLE_READ() stepperE4.isEnabled()
  #else
    #define E4_ENABLE_INIT SET_OUTPUT(E4_ENABLE_PIN)
    #define E4_ENABLE_WRITE(STATE) WRITE(E4_ENABLE_PIN,STATE)
    #define E4_ENABLE_READ() READ(E4_ENABLE_PIN)
  #endif
  #define E4_DIR_INIT SET_OUTPUT(E4_DIR_PIN)
  #define E4_DIR_WRITE(STATE) WRITE(E4_DIR_PIN,STATE)
  #define E4_DIR_READ() READ(E4_DIR_PIN)
#endif
#define E4_STEP_INIT SET_OUTPUT(E4_STEP_PIN)
#if AXIS_HAS_SQUARE_WAVE(E4)
  #define E4_STEP_WRITE(STATE) do{ if(STATE) TOGGLE(E4_STEP_PIN); }while(0)
#else
  #define E4_STEP_WRITE(STATE) WRITE(E4_STEP_PIN,STATE)
#endif
#define E4_STEP_READ READ(E4_STEP_PIN)

// E5 Stepper
#if AXIS_DRIVER_TYPE_E5(L6470)
  extern L6470 stepperE5;
  #define E5_ENABLE_INIT NOOP
  #define E5_ENABLE_WRITE(STATE) NOOP
  #define E5_ENABLE_READ() (stepperE5.getStatus() & STATUS_HIZ)
  #define E5_DIR_INIT NOOP
  #define E5_DIR_WRITE(STATE) L6470_WRITE_DIR_COMMAND(STATE,E5)
  #define E5_DIR_READ() (stepperE5.getStatus() & STATUS_DIR)
#else
  #if AXIS_IS_TMC(E5)
    extern TMC_CLASS_E(5) stepperE5;
  #endif
  #if AXIS_DRIVER_TYPE_E5(TMC26X)
    extern TMC26XStepper stepperE5;
    #define E5_ENABLE_INIT NOOP
    #define E5_ENABLE_WRITE(STATE) stepperE5.setEnabled(STATE)
    #define E5_ENABLE_READ() stepperE5.isEnabled()
  #elif ENABLED(SOFTWARE_DRIVER_ENABLE) && AXIS_IS_TMC(E5)
    #define E5_ENABLE_INIT NOOP
    #define E5_ENABLE_WRITE(STATE) stepperE5.toff((STATE)==E_ENABLE_ON ? chopper_timing.toff : 0)
    #define E5_ENABLE_READ() stepperE5.isEnabled()
  #else
    #define E5_ENABLE_INIT SET_OUTPUT(E5_ENABLE_PIN)
    #define E5_ENABLE_WRITE(STATE) WRITE(E5_ENABLE_PIN,STATE)
    #define E5_ENABLE_READ() READ(E5_ENABLE_PIN)
  #endif
  #define E5_DIR_INIT SET_OUTPUT(E5_DIR_PIN)
  #define E5_DIR_WRITE(STATE) WRITE(E5_DIR_PIN,STATE)
  #define E5_DIR_READ() READ(E5_DIR_PIN)
#endif
#define E5_STEP_INIT SET_OUTPUT(E5_STEP_PIN)
#if AXIS_HAS_SQUARE_WAVE(E5)
  #define E5_STEP_WRITE(STATE) do{ if(STATE) TOGGLE(E5_STEP_PIN); }while(0)
#else
  #define E5_STEP_WRITE(STATE) WRITE(E5_STEP_PIN,STATE)
#endif
#define E5_STEP_READ READ(E5_STEP_PIN)

/**
 * Extruder indirection for the single E axis
 */
#if ENABLED(SWITCHING_EXTRUDER) // One stepper driver per two extruders, reversed on odd index
  #if EXTRUDERS > 5
    #define E_STEP_WRITE(E,V) do{ if (E < 2) { E0_STEP_WRITE(V); } else if (E < 4) { E1_STEP_WRITE(V); } else { E2_STEP_WRITE(V); } }while(0)
    #define   NORM_E_DIR(E)   do{ switch (E) { case 0: E0_DIR_WRITE(!INVERT_E0_DIR); break; case 1: E0_DIR_WRITE( INVERT_E0_DIR); break; case 2: E1_DIR_WRITE(!INVERT_E1_DIR); break; case 3: E1_DIR_WRITE( INVERT_E1_DIR); break; case 4: E2_DIR_WRITE(!INVERT_E2_DIR); case 5: E2_DIR_WRITE( INVERT_E2_DIR); } }while(0)
    #define    REV_E_DIR(E)   do{ switch (E) { case 0: E0_DIR_WRITE( INVERT_E0_DIR); break; case 1: E0_DIR_WRITE(!INVERT_E0_DIR); break; case 2: E1_DIR_WRITE( INVERT_E1_DIR); break; case 3: E1_DIR_WRITE(!INVERT_E1_DIR); break; case 4: E2_DIR_WRITE( INVERT_E2_DIR); case 5: E2_DIR_WRITE(!INVERT_E2_DIR); } }while(0)
  #elif EXTRUDERS > 4
    #define E_STEP_WRITE(E,V) do{ if (E < 2) { E0_STEP_WRITE(V); } else if (E < 4) { E1_STEP_WRITE(V); } else { E2_STEP_WRITE(V); } }while(0)
    #define   NORM_E_DIR(E)   do{ switch (E) { case 0: E0_DIR_WRITE(!INVERT_E0_DIR); break; case 1: E0_DIR_WRITE( INVERT_E0_DIR); break; case 2: E1_DIR_WRITE(!INVERT_E1_DIR); break; case 3: E1_DIR_WRITE( INVERT_E1_DIR); break; case 4: E2_DIR_WRITE(!INVERT_E2_DIR); } }while(0)
    #define    REV_E_DIR(E)   do{ switch (E) { case 0: E0_DIR_WRITE( INVERT_E0_DIR); break; case 1: E0_DIR_WRITE(!INVERT_E0_DIR); break; case 2: E1_DIR_WRITE( INVERT_E1_DIR); break; case 3: E1_DIR_WRITE(!INVERT_E1_DIR); break; case 4: E2_DIR_WRITE( INVERT_E2_DIR); } }while(0)
  #elif EXTRUDERS > 3
    #define E_STEP_WRITE(E,V) do{ if (E < 2) { E0_STEP_WRITE(V); } else { E1_STEP_WRITE(V); } }while(0)
    #define   NORM_E_DIR(E)   do{ switch (E) { case 0: E0_DIR_WRITE(!INVERT_E0_DIR); break; case 1: E0_DIR_WRITE( INVERT_E0_DIR); break; case 2: E1_DIR_WRITE(!INVERT_E1_DIR); break; case 3: E1_DIR_WRITE( INVERT_E1_DIR); } }while(0)
    #define    REV_E_DIR(E)   do{ switch (E) { case 0: E0_DIR_WRITE( INVERT_E0_DIR); break; case 1: E0_DIR_WRITE(!INVERT_E0_DIR); break; case 2: E1_DIR_WRITE( INVERT_E1_DIR); break; case 3: E1_DIR_WRITE(!INVERT_E1_DIR); } }while(0)
  #elif EXTRUDERS > 2
    #define E_STEP_WRITE(E,V) do{ if (E < 2) { E0_STEP_WRITE(V); } else { E1_STEP_WRITE(V); } }while(0)
    #define   NORM_E_DIR(E)   do{ switch (E) { case 0: E0_DIR_WRITE(!INVERT_E0_DIR); break; case 1: E0_DIR_WRITE( INVERT_E0_DIR); break; case 2: E1_DIR_WRITE(!INVERT_E1_DIR); } }while(0)
    #define    REV_E_DIR(E)   do{ switch (E) { case 0: E0_DIR_WRITE( INVERT_E0_DIR); break; case 1: E0_DIR_WRITE(!INVERT_E0_DIR); break; case 2: E1_DIR_WRITE( INVERT_E1_DIR); } }while(0)
  #else
    #define E_STEP_WRITE(E,V) E0_STEP_WRITE(V)
    #define   NORM_E_DIR(E)   do{ E0_DIR_WRITE(E ?  INVERT_E0_DIR : !INVERT_E0_DIR); }while(0)
    #define    REV_E_DIR(E)   do{ E0_DIR_WRITE(E ? !INVERT_E0_DIR :  INVERT_E0_DIR); }while(0)
  #endif
#elif ENABLED(PRUSA_MMU2)
  #define E_STEP_WRITE(E,V) E0_STEP_WRITE(V)
  #define   NORM_E_DIR(E)   E0_DIR_WRITE(!INVERT_E0_DIR)
  #define    REV_E_DIR(E)   E0_DIR_WRITE( INVERT_E0_DIR)

#elif ENABLED(MK2_MULTIPLEXER) // One multiplexed stepper driver, reversed on odd index
  #define E_STEP_WRITE(E,V) E0_STEP_WRITE(V)
  #define   NORM_E_DIR(E)   do{ E0_DIR_WRITE(TEST(E, 0) ? !INVERT_E0_DIR:  INVERT_E0_DIR); }while(0)
  #define    REV_E_DIR(E)   do{ E0_DIR_WRITE(TEST(E, 0) ?  INVERT_E0_DIR: !INVERT_E0_DIR); }while(0)

#elif E_STEPPERS > 1

  #if E_STEPPERS > 5
    #define _E_STEP_WRITE(E,V) do{ switch (E) { case 0: E0_STEP_WRITE(V); break; case 1: E1_STEP_WRITE(V); break; case 2: E2_STEP_WRITE(V); break; case 3: E3_STEP_WRITE(V); break; case 4: E4_STEP_WRITE(V); case 5: E5_STEP_WRITE(V); } }while(0)
    #define   _NORM_E_DIR(E)   do{ switch (E) { case 0: E0_DIR_WRITE(!INVERT_E0_DIR); break; case 1: E1_DIR_WRITE(!INVERT_E1_DIR); break; case 2: E2_DIR_WRITE(!INVERT_E2_DIR); break; case 3: E3_DIR_WRITE(!INVERT_E3_DIR); break; case 4: E4_DIR_WRITE(!INVERT_E4_DIR); case 5: E5_DIR_WRITE(!INVERT_E5_DIR); } }while(0)
    #define    _REV_E_DIR(E)   do{ switch (E) { case 0: E0_DIR_WRITE( INVERT_E0_DIR); break; case 1: E1_DIR_WRITE( INVERT_E1_DIR); break; case 2: E2_DIR_WRITE( INVERT_E2_DIR); break; case 3: E3_DIR_WRITE( INVERT_E3_DIR); break; case 4: E4_DIR_WRITE( INVERT_E4_DIR); case 5: E5_DIR_WRITE( INVERT_E5_DIR); } }while(0)
  #elif E_STEPPERS > 4
    #define _E_STEP_WRITE(E,V) do{ switch (E) { case 0: E0_STEP_WRITE(V); break; case 1: E1_STEP_WRITE(V); break; case 2: E2_STEP_WRITE(V); break; case 3: E3_STEP_WRITE(V); break; case 4: E4_STEP_WRITE(V); } }while(0)
    #define   _NORM_E_DIR(E)   do{ switch (E) { case 0: E0_DIR_WRITE(!INVERT_E0_DIR); break; case 1: E1_DIR_WRITE(!INVERT_E1_DIR); break; case 2: E2_DIR_WRITE(!INVERT_E2_DIR); break; case 3: E3_DIR_WRITE(!INVERT_E3_DIR); break; case 4: E4_DIR_WRITE(!INVERT_E4_DIR); } }while(0)
    #define    _REV_E_DIR(E)   do{ switch (E) { case 0: E0_DIR_WRITE( INVERT_E0_DIR); break; case 1: E1_DIR_WRITE( INVERT_E1_DIR); break; case 2: E2_DIR_WRITE( INVERT_E2_DIR); break; case 3: E3_DIR_WRITE( INVERT_E3_DIR); break; case 4: E4_DIR_WRITE( INVERT_E4_DIR); } }while(0)
  #elif E_STEPPERS > 3
    #define _E_STEP_WRITE(E,V) do{ switch (E) { case 0: E0_STEP_WRITE(V); break; case 1: E1_STEP_WRITE(V); break; case 2: E2_STEP_WRITE(V); break; case 3: E3_STEP_WRITE(V); } }while(0)
    #define   _NORM_E_DIR(E)   do{ switch (E) { case 0: E0_DIR_WRITE(!INVERT_E0_DIR); break; case 1: E1_DIR_WRITE(!INVERT_E1_DIR); break; case 2: E2_DIR_WRITE(!INVERT_E2_DIR); break; case 3: E3_DIR_WRITE(!INVERT_E3_DIR); } }while(0)
    #define    _REV_E_DIR(E)   do{ switch (E) { case 0: E0_DIR_WRITE( INVERT_E0_DIR); break; case 1: E1_DIR_WRITE( INVERT_E1_DIR); break; case 2: E2_DIR_WRITE( INVERT_E2_DIR); break; case 3: E3_DIR_WRITE( INVERT_E3_DIR); } }while(0)
  #elif E_STEPPERS > 2
    #define _E_STEP_WRITE(E,V) do{ switch (E) { case 0: E0_STEP_WRITE(V); break; case 1: E1_STEP_WRITE(V); break; case 2: E2_STEP_WRITE(V); } }while(0)
    #define   _NORM_E_DIR(E)   do{ switch (E) { case 0: E0_DIR_WRITE(!INVERT_E0_DIR); break; case 1: E1_DIR_WRITE(!INVERT_E1_DIR); break; case 2: E2_DIR_WRITE(!INVERT_E2_DIR); } }while(0)
    #define    _REV_E_DIR(E)   do{ switch (E) { case 0: E0_DIR_WRITE( INVERT_E0_DIR); break; case 1: E1_DIR_WRITE( INVERT_E1_DIR); break; case 2: E2_DIR_WRITE( INVERT_E2_DIR); } }while(0)
  #else
    #define _E_STEP_WRITE(E,V) do{ if (E == 0) { E0_STEP_WRITE(V); } else { E1_STEP_WRITE(V); } }while(0)
    #define   _NORM_E_DIR(E)   do{ if (E == 0) { E0_DIR_WRITE(!INVERT_E0_DIR); } else { E1_DIR_WRITE(!INVERT_E1_DIR); } }while(0)
    #define    _REV_E_DIR(E)   do{ if (E == 0) { E0_DIR_WRITE( INVERT_E0_DIR); } else { E1_DIR_WRITE( INVERT_E1_DIR); } }while(0)
  #endif

  #if HAS_DUPLICATION_MODE

    #if ENABLED(MULTI_NOZZLE_DUPLICATION)
      #define _DUPE(N,T,V)  do{ if (TEST(duplication_e_mask, N)) E##N##_##T##_WRITE(V); }while(0)
    #else
      #define _DUPE(N,T,V)  E##N##_##T##_WRITE(V)
    #endif

    #define NDIR(N) _DUPE(N,DIR,!INVERT_E##N##_DIR)
    #define RDIR(N) _DUPE(N,DIR, INVERT_E##N##_DIR)

    #define E_STEP_WRITE(E,V) do{ if (extruder_duplication_enabled) { DUPE(STEP,V); } else _E_STEP_WRITE(E,V); }while(0)

    #if E_STEPPERS > 2
      #if E_STEPPERS > 5
        #define DUPE(T,V)     do{ _DUPE(0,T,V); _DUPE(1,T,V); _DUPE(2,T,V); _DUPE(3,T,V); _DUPE(4,T,V); _DUPE(5,T,V); }while(0)
        #define NORM_E_DIR(E) do{ if (extruder_duplication_enabled) { NDIR(0); NDIR(1); NDIR(2); NDIR(3); NDIR(4); NDIR(5); } else _NORM_E_DIR(E); }while(0)
        #define REV_E_DIR(E)  do{ if (extruder_duplication_enabled) { RDIR(0); RDIR(1); RDIR(2); RDIR(3); RDIR(4); RDIR(5); } else  _REV_E_DIR(E); }while(0)
      #elif E_STEPPERS > 4
        #define DUPE(T,V)     do{ _DUPE(0,T,V); _DUPE(1,T,V); _DUPE(2,T,V); _DUPE(3,T,V); _DUPE(4,T,V); }while(0)
        #define NORM_E_DIR(E) do{ if (extruder_duplication_enabled) { NDIR(0); NDIR(1); NDIR(2); NDIR(3); NDIR(4); } else _NORM_E_DIR(E); }while(0)
        #define REV_E_DIR(E)  do{ if (extruder_duplication_enabled) { RDIR(0); RDIR(1); RDIR(2); RDIR(3); RDIR(4); } else  _REV_E_DIR(E); }while(0)
      #elif E_STEPPERS > 3
        #define DUPE(T,V)     do{ _DUPE(0,T,V); _DUPE(1,T,V); _DUPE(2,T,V); _DUPE(3,T,V); }while(0)
        #define NORM_E_DIR(E) do{ if (extruder_duplication_enabled) { NDIR(0); NDIR(1); NDIR(2); NDIR(3); } else _NORM_E_DIR(E); }while(0)
        #define REV_E_DIR(E)  do{ if (extruder_duplication_enabled) { RDIR(0); RDIR(1); RDIR(2); RDIR(3); } else  _REV_E_DIR(E); }while(0)
      #else
        #define DUPE(T,V)     do{ _DUPE(0,T,V); _DUPE(1,T,V); _DUPE(2,T,V); }while(0)
        #define NORM_E_DIR(E) do{ if (extruder_duplication_enabled) { NDIR(0); NDIR(1); NDIR(2); } else _NORM_E_DIR(E); }while(0)
        #define REV_E_DIR(E)  do{ if (extruder_duplication_enabled) { RDIR(0); RDIR(1); RDIR(2); } else  _REV_E_DIR(E); }while(0)
      #endif
    #else
      #define DUPE(T,V)     do{ _DUPE(0,T,V); _DUPE(1,T,V); }while(0)
      #define NORM_E_DIR(E) do{ if (extruder_duplication_enabled) { NDIR(0); NDIR(1); } else _NORM_E_DIR(E); }while(0)
      #define REV_E_DIR(E)  do{ if (extruder_duplication_enabled) { RDIR(0); RDIR(1); } else  _REV_E_DIR(E); }while(0)
    #endif

  #else

    #define E_STEP_WRITE(E,V) _E_STEP_WRITE(E,V)
    #define   NORM_E_DIR(E)   _NORM_E_DIR(E)
    #define    REV_E_DIR(E)   _REV_E_DIR(E)

  #endif

#elif E_STEPPERS
  #define E_STEP_WRITE(E,V) E0_STEP_WRITE(V)
  #define   NORM_E_DIR(E)   E0_DIR_WRITE(!INVERT_E0_DIR)
  #define    REV_E_DIR(E)   E0_DIR_WRITE( INVERT_E0_DIR)

#else
  #define E_STEP_WRITE(E,V) NOOP
  #define   NORM_E_DIR(E)   NOOP
  #define    REV_E_DIR(E)   NOOP

#endif
