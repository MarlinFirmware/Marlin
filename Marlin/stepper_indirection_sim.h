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
  stepper_indirection_sim.h - stepper motor driver empty macros
    allow Marlin to run in simulation without affecting hardware.
*/

#ifndef STEPPER_INDIRECTION_SIM_H
#define STEPPER_INDIRECTION_SIM_H

#include "Marlin.h"

// X Stepper
#define X_ENABLE_INIT         NOOP
#define X_ENABLE_WRITE(S)     NOOP
#define X_ENABLE_READ         NOOP
#define X_DIR_INIT            NOOP
#define X_DIR_WRITE(S)        NOOP
#define X_DIR_READ            NOOP
#define X_STEP_INIT           NOOP
#define X_STEP_WRITE(S)       NOOP
#define X_STEP_READ           NOOP

// Y Stepper
#define Y_ENABLE_INIT         NOOP
#define Y_ENABLE_WRITE(S)     NOOP
#define Y_ENABLE_READ         NOOP
#define Y_DIR_INIT            NOOP
#define Y_DIR_WRITE(S)        NOOP
#define Y_DIR_READ            NOOP
#define Y_STEP_INIT           NOOP
#define Y_STEP_WRITE(S)       NOOP
#define Y_STEP_READ           NOOP

// Z Stepper
#define Z_ENABLE_INIT         NOOP
#define Z_ENABLE_WRITE(S)     NOOP
#define Z_ENABLE_READ         NOOP
#define Z_DIR_INIT            NOOP
#define Z_DIR_WRITE(S)        NOOP
#define Z_DIR_READ            NOOP
#define Z_STEP_INIT           NOOP
#define Z_STEP_WRITE(S)       NOOP
#define Z_STEP_READ           NOOP

// X2 Stepper
#if HAS_X2_ENABLE
  #define X2_ENABLE_INIT         NOOP
  #define X2_ENABLE_WRITE(S)     NOOP
  #define X2_ENABLE_READ         NOOP
  #define X2_DIR_INIT            NOOP
  #define X2_DIR_WRITE(S)        NOOP
  #define X2_DIR_READ            NOOP
  #define X2_STEP_INIT           NOOP
  #define X2_STEP_WRITE(S)       NOOP
  #define X2_STEP_READ           NOOP
#endif

// Y2 Stepper
#if HAS_Y2_ENABLE
  #define Y2_ENABLE_INIT         NOOP
  #define Y2_ENABLE_WRITE(S)     NOOP
  #define Y2_ENABLE_READ         NOOP
  #define Y2_DIR_INIT            NOOP
  #define Y2_DIR_WRITE(S)        NOOP
  #define Y2_DIR_READ            NOOP
  #define Y2_STEP_INIT           NOOP
  #define Y2_STEP_WRITE(S)       NOOP
  #define Y2_STEP_READ           NOOP
#endif

// Z2 Stepper
#if HAS_Z2_ENABLE
  #define Z2_ENABLE_INIT         NOOP
  #define Z2_ENABLE_WRITE(S)     NOOP
  #define Z2_ENABLE_READ         NOOP
  #define Z2_DIR_INIT            NOOP
  #define Z2_DIR_WRITE(S)        NOOP
  #define Z2_DIR_READ            NOOP
  #define Z2_STEP_INIT           NOOP
  #define Z2_STEP_WRITE(S)       NOOP
  #define Z2_STEP_READ           NOOP
#endif

// E0 Stepper
#define E0_ENABLE_INIT         NOOP
#define E0_ENABLE_WRITE(S)     NOOP
#define E0_ENABLE_READ         NOOP
#define E0_DIR_INIT            NOOP
#define E0_DIR_WRITE(S)        NOOP
#define E0_DIR_READ            NOOP
#define E0_STEP_INIT           NOOP
#define E0_STEP_WRITE(S)       NOOP
#define E0_STEP_READ           NOOP

// E1 Stepper
#define E1_ENABLE_INIT         NOOP
#define E1_ENABLE_WRITE(S)     NOOP
#define E1_ENABLE_READ         NOOP
#define E1_DIR_INIT            NOOP
#define E1_DIR_WRITE(S)        NOOP
#define E1_DIR_READ            NOOP
#define E1_STEP_INIT           NOOP
#define E1_STEP_WRITE(S)       NOOP
#define E1_STEP_READ           NOOP

// E2 Stepper
#define E2_ENABLE_INIT         NOOP
#define E2_ENABLE_WRITE(S)     NOOP
#define E2_ENABLE_READ         NOOP
#define E2_DIR_INIT            NOOP
#define E2_DIR_WRITE(S)        NOOP
#define E2_DIR_READ            NOOP
#define E2_STEP_INIT           NOOP
#define E2_STEP_WRITE(S)       NOOP
#define E2_STEP_READ           NOOP

// E3 Stepper
#define E3_ENABLE_INIT         NOOP
#define E3_ENABLE_WRITE(S)     NOOP
#define E3_ENABLE_READ         NOOP
#define E3_DIR_INIT            NOOP
#define E3_DIR_WRITE(S)        NOOP
#define E3_DIR_READ            NOOP
#define E3_STEP_INIT           NOOP
#define E3_STEP_WRITE(S)       NOOP
#define E3_STEP_READ           NOOP

#endif // STEPPER_INDIRECTION_SIM_H
