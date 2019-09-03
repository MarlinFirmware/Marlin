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
 * stepper/L6470.h
 * Stepper driver indirection for L6470 drivers
 */

#include "../../inc/MarlinConfig.h"
#include "../../libs/L6470/L6470_Marlin.h"

// L6470 has STEP on normal pins, but DIR/ENABLE via SPI
#define L6470_WRITE_DIR_COMMAND(STATE,Q) do{ L6470_dir_commands[Q] = (STATE ?  dSPIN_STEP_CLOCK_REV : dSPIN_STEP_CLOCK_FWD); }while(0)

// X Stepper
#if AXIS_DRIVER_TYPE_X(L6470)
  extern L6470 stepperX;
  #define X_ENABLE_INIT NOOP
  #define X_ENABLE_WRITE(STATE) NOOP
  #define X_ENABLE_READ() (stepperX.getStatus() & STATUS_HIZ)
  #define X_DIR_INIT NOOP
  #define X_DIR_WRITE(STATE) L6470_WRITE_DIR_COMMAND(STATE,X)
  #define X_DIR_READ() (stepperX.getStatus() & STATUS_DIR)
#endif

// Y Stepper
#if AXIS_DRIVER_TYPE_Y(L6470)
  extern L6470 stepperY;
  #define Y_ENABLE_INIT NOOP
  #define Y_ENABLE_WRITE(STATE) NOOP
  #define Y_ENABLE_READ() (stepperY.getStatus() & STATUS_HIZ)
  #define Y_DIR_INIT NOOP
  #define Y_DIR_WRITE(STATE) L6470_WRITE_DIR_COMMAND(STATE,Y)
  #define Y_DIR_READ() (stepperY.getStatus() & STATUS_DIR)
#endif

// Z Stepper
#if AXIS_DRIVER_TYPE_Z(L6470)
  extern L6470 stepperZ;
  #define Z_ENABLE_INIT NOOP
  #define Z_ENABLE_WRITE(STATE) NOOP
  #define Z_ENABLE_READ() (stepperZ.getStatus() & STATUS_HIZ)
  #define Z_DIR_INIT NOOP
  #define Z_DIR_WRITE(STATE) L6470_WRITE_DIR_COMMAND(STATE,Z)
  #define Z_DIR_READ() (stepperZ.getStatus() & STATUS_DIR)
#endif

// X2 Stepper
#if HAS_X2_ENABLE && AXIS_DRIVER_TYPE_X2(L6470)
  extern L6470 stepperX2;
  #define X2_ENABLE_INIT NOOP
  #define X2_ENABLE_WRITE(STATE) NOOP
  #define X2_ENABLE_READ() (stepperX2.getStatus() & STATUS_HIZ)
  #define X2_DIR_INIT NOOP
  #define X2_DIR_WRITE(STATE) L6470_WRITE_DIR_COMMAND(STATE,X2)
  #define X2_DIR_READ() (stepperX2.getStatus() & STATUS_DIR)
#endif

// Y2 Stepper
#if HAS_Y2_ENABLE && AXIS_DRIVER_TYPE_Y2(L6470)
  extern L6470 stepperY2;
  #define Y2_ENABLE_INIT NOOP
  #define Y2_ENABLE_WRITE(STATE) NOOP
  #define Y2_ENABLE_READ() (stepperY2.getStatus() & STATUS_HIZ)
  #define Y2_DIR_INIT NOOP
  #define Y2_DIR_WRITE(STATE) L6470_WRITE_DIR_COMMAND(STATE,Y2)
  #define Y2_DIR_READ() (stepperY2.getStatus() & STATUS_DIR)
#endif

// Z2 Stepper
#if HAS_Z2_ENABLE && AXIS_DRIVER_TYPE_Z2(L6470)
  extern L6470 stepperZ2;
  #define Z2_ENABLE_INIT NOOP
  #define Z2_ENABLE_WRITE(STATE) NOOP
  #define Z2_ENABLE_READ() (stepperZ2.getStatus() & STATUS_HIZ)
  #define Z2_DIR_INIT NOOP
  #define Z2_DIR_WRITE(STATE) L6470_WRITE_DIR_COMMAND(STATE,Z2)
  #define Z2_DIR_READ() (stepperZ2.getStatus() & STATUS_DIR)
#endif

// Z3 Stepper
#if HAS_Z3_ENABLE && AXIS_DRIVER_TYPE_Z3(L6470)
  extern L6470 stepperZ3;
  #define Z3_ENABLE_INIT NOOP
  #define Z3_ENABLE_WRITE(STATE) NOOP
  #define Z3_ENABLE_READ() (stepperZ3.getStatus() & STATUS_HIZ)
  #define Z3_DIR_INIT NOOP
  #define Z3_DIR_WRITE(STATE) L6470_WRITE_DIR_COMMAND(STATE,Z3)
  #define Z3_DIR_READ() (stepperZ3.getStatus() & STATUS_DIR)
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
#endif

// E1 Stepper
#if AXIS_DRIVER_TYPE_E1(L6470)
  extern L6470 stepperE1;
  #define E1_ENABLE_INIT NOOP
  #define E1_ENABLE_WRITE(STATE) NOOP
  #define E1_ENABLE_READ() (stepperE1.getStatus() & STATUS_HIZ)
  #define E1_DIR_INIT NOOP
  #define E1_DIR_WRITE(STATE) L6470_WRITE_DIR_COMMAND(STATE,E1)
  #define E1_DIR_READ() (stepperE1.getStatus() & STATUS_DIR)
#endif

// E2 Stepper
#if AXIS_DRIVER_TYPE_E2(L6470)
  extern L6470 stepperE2;
  #define E2_ENABLE_INIT NOOP
  #define E2_ENABLE_WRITE(STATE) NOOP
  #define E2_ENABLE_READ() (stepperE2.getStatus() & STATUS_HIZ)
  #define E2_DIR_INIT NOOP
  #define E2_DIR_WRITE(STATE) L6470_WRITE_DIR_COMMAND(STATE,E2)
  #define E2_DIR_READ() (stepperE2.getStatus() & STATUS_DIR)
#endif

// E3 Stepper
#if AXIS_DRIVER_TYPE_E3(L6470)
  extern L6470 stepperE3;
  #define E3_ENABLE_INIT NOOP
  #define E3_ENABLE_WRITE(STATE) NOOP
  #define E3_ENABLE_READ() (stepperE3.getStatus() & STATUS_HIZ)
  #define E3_DIR_INIT NOOP
  #define E3_DIR_WRITE(STATE) L6470_WRITE_DIR_COMMAND(STATE,E3)
  #define E3_DIR_READ() (stepperE3.getStatus() & STATUS_DIR)
#endif

// E4 Stepper
#if AXIS_DRIVER_TYPE_E4(L6470)
  extern L6470 stepperE4;
  #define E4_ENABLE_INIT NOOP
  #define E4_ENABLE_WRITE(STATE) NOOP
  #define E4_ENABLE_READ() (stepperE4.getStatus() & STATUS_HIZ)
  #define E4_DIR_INIT NOOP
  #define E4_DIR_WRITE(STATE) L6470_WRITE_DIR_COMMAND(STATE,E4)
  #define E4_DIR_READ() (stepperE4.getStatus() & STATUS_DIR)
#endif

// E5 Stepper
#if AXIS_DRIVER_TYPE_E5(L6470)
  extern L6470 stepperE5;
  #define E5_ENABLE_INIT NOOP
  #define E5_ENABLE_WRITE(STATE) NOOP
  #define E5_ENABLE_READ() (stepperE5.getStatus() & STATUS_HIZ)
  #define E5_DIR_INIT NOOP
  #define E5_DIR_WRITE(STATE) L6470_WRITE_DIR_COMMAND(STATE,E5)
  #define E5_DIR_READ() (stepperE5.getStatus() & STATUS_DIR)
#endif
