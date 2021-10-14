/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * stepper/TMC26X.h
 * Stepper driver indirection for TMC26X drivers
 */

#include "../../inc/MarlinConfig.h"

// TMC26X drivers have STEP/DIR on normal pins, but ENABLE via SPI

#include <SPI.h>
#include <TMC26XStepper.h>

void tmc26x_init_to_defaults();

// X Stepper
#if AXIS_DRIVER_TYPE_X(TMC26X)
  extern TMC26XStepper stepperX;
  #define X_ENABLE_INIT() NOOP
  #define X_ENABLE_WRITE(STATE) stepperX.setEnabled(STATE)
  #define X_ENABLE_READ() stepperX.isEnabled()
#endif

// Y Stepper
#if AXIS_DRIVER_TYPE_Y(TMC26X)
  extern TMC26XStepper stepperY;
  #define Y_ENABLE_INIT() NOOP
  #define Y_ENABLE_WRITE(STATE) stepperY.setEnabled(STATE)
  #define Y_ENABLE_READ() stepperY.isEnabled()
#endif

// Z Stepper
#if AXIS_DRIVER_TYPE_Z(TMC26X)
  extern TMC26XStepper stepperZ;
  #define Z_ENABLE_INIT() NOOP
  #define Z_ENABLE_WRITE(STATE) stepperZ.setEnabled(STATE)
  #define Z_ENABLE_READ() stepperZ.isEnabled()
#endif

// X2 Stepper
#if HAS_X2_ENABLE && AXIS_DRIVER_TYPE_X2(TMC26X)
  extern TMC26XStepper stepperX2;
  #define X2_ENABLE_INIT() NOOP
  #define X2_ENABLE_WRITE(STATE) stepperX2.setEnabled(STATE)
  #define X2_ENABLE_READ() stepperX2.isEnabled()
#endif

// Y2 Stepper
#if HAS_Y2_ENABLE && AXIS_DRIVER_TYPE_Y2(TMC26X)
  extern TMC26XStepper stepperY2;
  #define Y2_ENABLE_INIT() NOOP
  #define Y2_ENABLE_WRITE(STATE) stepperY2.setEnabled(STATE)
  #define Y2_ENABLE_READ() stepperY2.isEnabled()
#endif

// Z2 Stepper
#if HAS_Z2_ENABLE && AXIS_DRIVER_TYPE_Z2(TMC26X)
  extern TMC26XStepper stepperZ2;
  #define Z2_ENABLE_INIT() NOOP
  #define Z2_ENABLE_WRITE(STATE) stepperZ2.setEnabled(STATE)
  #define Z2_ENABLE_READ() stepperZ2.isEnabled()
#endif

// Z3 Stepper
#if HAS_Z3_ENABLE && AXIS_DRIVER_TYPE_Z3(TMC26X)
  extern TMC26XStepper stepperZ3;
  #define Z3_ENABLE_INIT() NOOP
  #define Z3_ENABLE_WRITE(STATE) stepperZ3.setEnabled(STATE)
  #define Z3_ENABLE_READ() stepperZ3.isEnabled()
#endif

// Z4 Stepper
#if HAS_Z4_ENABLE && AXIS_DRIVER_TYPE_Z4(TMC26X)
  extern TMC26XStepper stepperZ4;
  #define Z4_ENABLE_INIT() NOOP
  #define Z4_ENABLE_WRITE(STATE) stepperZ4.setEnabled(STATE)
  #define Z4_ENABLE_READ() stepperZ4.isEnabled()
#endif

// I Stepper
#if HAS_I_ENABLE && AXIS_DRIVER_TYPE_I(TMC26X)
  extern TMC26XStepper stepperI;
  #define I_ENABLE_INIT() NOOP
  #define I_ENABLE_WRITE(STATE) stepperI.setEnabled(STATE)
  #define I_ENABLE_READ() stepperI.isEnabled()
#endif

// J Stepper
#if HAS_J_ENABLE && AXIS_DRIVER_TYPE_J(TMC26X)
  extern TMC26XStepper stepperJ;
  #define J_ENABLE_INIT() NOOP
  #define J_ENABLE_WRITE(STATE) stepperJ.setEnabled(STATE)
  #define J_ENABLE_READ() stepperJ.isEnabled()
#endif

// K Stepper
#if HAS_K_ENABLE && AXIS_DRIVER_TYPE_K(TMC26X)
  extern TMC26XStepper stepperK;
  #define K_ENABLE_INIT() NOOP
  #define K_ENABLE_WRITE(STATE) stepperK.setEnabled(STATE)
  #define K_ENABLE_READ() stepperK.isEnabled()
#endif

// U Stepper
#if HAS_U_ENABLE && AXIS_DRIVER_TYPE_U(TMC26X)
  extern TMC26XStepper stepperU;
  #define U_ENABLE_INIT() NOOP
  #define U_ENABLE_WRITE(STATE) stepperU.setEnabled(STATE)
  #define U_ENABLE_READ() stepperU.isEnabled()
#endif

// V Stepper
#if HAS_V_ENABLE && AXIS_DRIVER_TYPE_V(TMC26X)
  extern TMC26XStepper stepperV;
  #define V_ENABLE_INIT() NOOP
  #define V_ENABLE_WRITE(STATE) stepperV.setEnabled(STATE)
  #define V_ENABLE_READ() stepperV.isEnabled()
#endif

// W Stepper
#if HAS_W_ENABLE && AXIS_DRIVER_TYPE_W(TMC26X)
  extern TMC26XStepper stepperW;
  #define W_ENABLE_INIT() NOOP
  #define W_ENABLE_WRITE(STATE) stepperW.setEnabled(STATE)
  #define W_ENABLE_READ() stepperW.isEnabled()
#endif

// E0 Stepper
#if AXIS_DRIVER_TYPE_E0(TMC26X)
  extern TMC26XStepper stepperE0;
  #define E0_ENABLE_INIT() NOOP
  #define E0_ENABLE_WRITE(STATE) stepperE0.setEnabled(STATE)
  #define E0_ENABLE_READ() stepperE0.isEnabled()
#endif

// E1 Stepper
#if AXIS_DRIVER_TYPE_E1(TMC26X)
  extern TMC26XStepper stepperE1;
  #define E1_ENABLE_INIT() NOOP
  #define E1_ENABLE_WRITE(STATE) stepperE1.setEnabled(STATE)
  #define E1_ENABLE_READ() stepperE1.isEnabled()
#endif

// E2 Stepper
#if AXIS_DRIVER_TYPE_E2(TMC26X)
  extern TMC26XStepper stepperE2;
  #define E2_ENABLE_INIT() NOOP
  #define E2_ENABLE_WRITE(STATE) stepperE2.setEnabled(STATE)
  #define E2_ENABLE_READ() stepperE2.isEnabled()
#endif

// E3 Stepper
#if AXIS_DRIVER_TYPE_E3(TMC26X)
  extern TMC26XStepper stepperE3;
  #define E3_ENABLE_INIT() NOOP
  #define E3_ENABLE_WRITE(STATE) stepperE3.setEnabled(STATE)
  #define E3_ENABLE_READ() stepperE3.isEnabled()
#endif

// E4 Stepper
#if AXIS_DRIVER_TYPE_E4(TMC26X)
  extern TMC26XStepper stepperE4;
  #define E4_ENABLE_INIT() NOOP
  #define E4_ENABLE_WRITE(STATE) stepperE4.setEnabled(STATE)
  #define E4_ENABLE_READ() stepperE4.isEnabled()
#endif

// E5 Stepper
#if AXIS_DRIVER_TYPE_E5(TMC26X)
  extern TMC26XStepper stepperE5;
  #define E5_ENABLE_INIT() NOOP
  #define E5_ENABLE_WRITE(STATE) stepperE5.setEnabled(STATE)
  #define E5_ENABLE_READ() stepperE5.isEnabled()
#endif

// E6 Stepper
#if AXIS_DRIVER_TYPE_E6(TMC26X)
  extern TMC26XStepper stepperE6;
  #define E6_ENABLE_INIT() NOOP
  #define E6_ENABLE_WRITE(STATE) stepperE6.setEnabled(STATE)
  #define E6_ENABLE_READ() stepperE6.isEnabled()
#endif

// E7 Stepper
#if AXIS_DRIVER_TYPE_E7(TMC26X)
  extern TMC26XStepper stepperE7;
  #define E7_ENABLE_INIT() NOOP
  #define E7_ENABLE_WRITE(STATE) stepperE7.setEnabled(STATE)
  #define E7_ENABLE_READ() stepperE7.isEnabled()
#endif
