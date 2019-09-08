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
 * stepper/trinamic.h
 * Stepper driver indirection for Trinamic
 */

#include <TMCStepper.h>
#if TMCSTEPPER_VERSION < 0x000405
  #error "Update TMCStepper library to 0.4.5 or newer."
#endif

#include "../../inc/MarlinConfig.h"
#include "../../feature/tmc_util.h"

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

void restore_trinamic_drivers();
void reset_trinamic_drivers();

#define AXIS_HAS_SQUARE_WAVE(A) (AXIS_IS_TMC(A) && ENABLED(SQUARE_WAVE_STEPPING))

// X Stepper
#if AXIS_IS_TMC(X)
  extern TMC_CLASS(X, X) stepperX;
  #if ENABLED(SOFTWARE_DRIVER_ENABLE)
    #define X_ENABLE_INIT NOOP
    #define X_ENABLE_WRITE(STATE) stepperX.toff((STATE)==X_ENABLE_ON ? chopper_timing.toff : 0)
    #define X_ENABLE_READ() stepperX.isEnabled()
  #endif
  #if AXIS_HAS_SQUARE_WAVE(X)
    #define X_STEP_WRITE(STATE) do{ if(STATE) TOGGLE(X_STEP_PIN); }while(0)
  #endif
#endif

// Y Stepper
#if AXIS_IS_TMC(Y)
  extern TMC_CLASS(Y, Y) stepperY;
  #if ENABLED(SOFTWARE_DRIVER_ENABLE)
    #define Y_ENABLE_INIT NOOP
    #define Y_ENABLE_WRITE(STATE) stepperY.toff((STATE)==Y_ENABLE_ON ? chopper_timing.toff : 0)
    #define Y_ENABLE_READ() stepperY.isEnabled()
  #endif
  #if AXIS_HAS_SQUARE_WAVE(Y)
    #define Y_STEP_WRITE(STATE) do{ if (STATE) TOGGLE(Y_STEP_PIN); }while(0)
  #endif
#endif

// Z Stepper
#if AXIS_IS_TMC(Z)
  extern TMC_CLASS(Z, Z) stepperZ;
  #if ENABLED(SOFTWARE_DRIVER_ENABLE)
    #define Z_ENABLE_INIT NOOP
    #define Z_ENABLE_WRITE(STATE) stepperZ.toff((STATE)==Z_ENABLE_ON ? chopper_timing.toff : 0)
    #define Z_ENABLE_READ() stepperZ.isEnabled()
  #endif
  #if AXIS_HAS_SQUARE_WAVE(Z)
    #define Z_STEP_WRITE(STATE) do{ if(STATE) TOGGLE(Z_STEP_PIN); }while(0)
  #endif
#endif

// X2 Stepper
#if HAS_X2_ENABLE && AXIS_IS_TMC(X2)
  extern TMC_CLASS(X2, X) stepperX2;
  #if ENABLED(SOFTWARE_DRIVER_ENABLE)
    #define X2_ENABLE_INIT NOOP
    #define X2_ENABLE_WRITE(STATE) stepperX2.toff((STATE)==X_ENABLE_ON ? chopper_timing.toff : 0)
    #define X2_ENABLE_READ() stepperX2.isEnabled()
  #endif
  #if AXIS_HAS_SQUARE_WAVE(X2)
    #define X2_STEP_WRITE(STATE) do{ if(STATE) TOGGLE(X2_STEP_PIN); }while(0)
  #endif
#endif

// Y2 Stepper
#if HAS_Y2_ENABLE && AXIS_IS_TMC(Y2)
  extern TMC_CLASS(Y2, Y) stepperY2;
  #if ENABLED(SOFTWARE_DRIVER_ENABLE)
    #define Y2_ENABLE_INIT NOOP
    #define Y2_ENABLE_WRITE(STATE) stepperY2.toff((STATE)==Y_ENABLE_ON ? chopper_timing.toff : 0)
    #define Y2_ENABLE_READ() stepperY2.isEnabled()
  #endif
  #if AXIS_HAS_SQUARE_WAVE(Y2)
    #define Y2_STEP_WRITE(STATE) do{ if(STATE) TOGGLE(Y2_STEP_PIN); }while(0)
  #endif
#endif

// Z2 Stepper
#if HAS_Z2_ENABLE && AXIS_IS_TMC(Z2)
  extern TMC_CLASS(Z2, Z) stepperZ2;
  #if ENABLED(SOFTWARE_DRIVER_ENABLE) && AXIS_IS_TMC(Z2)
    #define Z2_ENABLE_INIT NOOP
    #define Z2_ENABLE_WRITE(STATE) stepperZ2.toff((STATE)==Z_ENABLE_ON ? chopper_timing.toff : 0)
    #define Z2_ENABLE_READ() stepperZ2.isEnabled()
  #endif
  #if AXIS_HAS_SQUARE_WAVE(Z2)
    #define Z2_STEP_WRITE(STATE) do{ if(STATE) TOGGLE(Z2_STEP_PIN); }while(0)
  #endif
#endif

// Z3 Stepper
#if HAS_Z3_ENABLE && AXIS_IS_TMC(Z3)
  extern TMC_CLASS(Z3, Z) stepperZ3;
  #if ENABLED(SOFTWARE_DRIVER_ENABLE)
    #define Z3_ENABLE_INIT NOOP
    #define Z3_ENABLE_WRITE(STATE) stepperZ3.toff((STATE)==Z_ENABLE_ON ? chopper_timing.toff : 0)
    #define Z3_ENABLE_READ() stepperZ3.isEnabled()
  #endif
  #if AXIS_HAS_SQUARE_WAVE(Z3)
    #define Z3_STEP_WRITE(STATE) do{ if(STATE) TOGGLE(Z3_STEP_PIN); }while(0)
  #endif
#endif

// E0 Stepper
#if AXIS_IS_TMC(E0)
  extern TMC_CLASS_E(0) stepperE0;
  #if ENABLED(SOFTWARE_DRIVER_ENABLE) && AXIS_IS_TMC(E0)
    #define E0_ENABLE_INIT NOOP
    #define E0_ENABLE_WRITE(STATE) stepperE0.toff((STATE)==E_ENABLE_ON ? chopper_timing.toff : 0)
    #define E0_ENABLE_READ() stepperE0.isEnabled()
  #endif
  #if AXIS_HAS_SQUARE_WAVE(E0)
    #define E0_STEP_WRITE(STATE) do{ if(STATE) TOGGLE(E0_STEP_PIN); }while(0)
  #endif
#endif

// E1 Stepper
#if AXIS_IS_TMC(E1)
  extern TMC_CLASS_E(1) stepperE1;
  #if ENABLED(SOFTWARE_DRIVER_ENABLE) && AXIS_IS_TMC(E1)
    #define E1_ENABLE_INIT NOOP
    #define E1_ENABLE_WRITE(STATE) stepperE1.toff((STATE)==E_ENABLE_ON ? chopper_timing.toff : 0)
    #define E1_ENABLE_READ() stepperE1.isEnabled()
  #endif
  #if AXIS_HAS_SQUARE_WAVE(E1)
    #define E1_STEP_WRITE(STATE) do{ if(STATE) TOGGLE(E1_STEP_PIN); }while(0)
  #endif
#endif

// E2 Stepper
#if AXIS_IS_TMC(E2)
  extern TMC_CLASS_E(1) stepperE2;
  #if ENABLED(SOFTWARE_DRIVER_ENABLE) && AXIS_IS_TMC(E2)
    #define E2_ENABLE_INIT NOOP
    #define E2_ENABLE_WRITE(STATE) stepperE2.toff((STATE)==E_ENABLE_ON ? chopper_timing.toff : 0)
    #define E2_ENABLE_READ() stepperE2.isEnabled()
  #endif
  #if AXIS_HAS_SQUARE_WAVE(E2)
    #define E2_STEP_WRITE(STATE) do{ if(STATE) TOGGLE(E2_STEP_PIN); }while(0)
  #endif
#endif

// E3 Stepper
#if AXIS_IS_TMC(E3)
  extern TMC_CLASS_E(1) stepperE3;
  #if ENABLED(SOFTWARE_DRIVER_ENABLE) && AXIS_IS_TMC(E3)
    #define E3_ENABLE_INIT NOOP
    #define E3_ENABLE_WRITE(STATE) stepperE3.toff((STATE)==E_ENABLE_ON ? chopper_timing.toff : 0)
    #define E3_ENABLE_READ() stepperE3.isEnabled()
  #endif
  #if AXIS_HAS_SQUARE_WAVE(E3)
    #define E3_STEP_WRITE(STATE) do{ if(STATE) TOGGLE(E3_STEP_PIN); }while(0)
  #endif
#endif

// E4 Stepper
#if AXIS_IS_TMC(E4)
  extern TMC_CLASS_E(1) stepperE4;
  #if ENABLED(SOFTWARE_DRIVER_ENABLE) && AXIS_IS_TMC(E4)
    #define E4_ENABLE_INIT NOOP
    #define E4_ENABLE_WRITE(STATE) stepperE4.toff((STATE)==E_ENABLE_ON ? chopper_timing.toff : 0)
    #define E4_ENABLE_READ() stepperE4.isEnabled()
  #endif
  #if AXIS_HAS_SQUARE_WAVE(E4)
    #define E4_STEP_WRITE(STATE) do{ if(STATE) TOGGLE(E4_STEP_PIN); }while(0)
  #endif
#endif

// E5 Stepper
#if AXIS_IS_TMC(E5)
  extern TMC_CLASS_E(1) stepperE5;
  #if ENABLED(SOFTWARE_DRIVER_ENABLE) && AXIS_IS_TMC(E5)
    #define E5_ENABLE_INIT NOOP
    #define E5_ENABLE_WRITE(STATE) stepperE5.toff((STATE)==E_ENABLE_ON ? chopper_timing.toff : 0)
    #define E5_ENABLE_READ() stepperE5.isEnabled()
  #endif
  #if AXIS_HAS_SQUARE_WAVE(E5)
    #define E5_STEP_WRITE(STATE) do{ if(STATE) TOGGLE(E5_STEP_PIN); }while(0)
  #endif
#endif
