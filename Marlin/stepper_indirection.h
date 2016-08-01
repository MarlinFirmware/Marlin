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
  stepper_indirection.h - stepper motor driver indirection macros
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

#ifndef STEPPER_INDIRECTION_H
#define STEPPER_INDIRECTION_H

#include "MarlinConfig.h"

// TMC26X drivers have STEP/DIR on normal pins, but ENABLE via SPI
#if ENABLED(HAVE_TMCDRIVER)
  #include <SPI.h>
  #include <TMC26XStepper.h>
  void tmc_init();
#endif

// L6470 has STEP on normal pins, but DIR/ENABLE via SPI
#if ENABLED(HAVE_L6470DRIVER)
  #include <SPI.h>
  #include <L6470.h>
  void L6470_init();
#endif

// X Stepper
#if ENABLED(HAVE_L6470DRIVER) && ENABLED(X_IS_L6470)
  extern L6470 stepperX;
  #define X_ENABLE_INIT NOOP
  #define X_ENABLE_WRITE(STATE) do{if(STATE) stepperX.Step_Clock(stepperX.getStatus() & STATUS_HIZ); else stepperX.softFree();}while(0)
  #define X_ENABLE_READ (stepperX.getStatus() & STATUS_HIZ)
  #define X_DIR_INIT NOOP
  #define X_DIR_WRITE(STATE) stepperX.Step_Clock(STATE)
  #define X_DIR_READ (stepperX.getStatus() & STATUS_DIR)
#else
  #if ENABLED(HAVE_TMCDRIVER) && ENABLED(X_IS_TMC)
    extern TMC26XStepper stepperX;
    #define X_ENABLE_INIT NOOP
    #define X_ENABLE_WRITE(STATE) stepperX.setEnabled(STATE)
    #define X_ENABLE_READ stepperX.isEnabled()
  #else
    #define X_ENABLE_INIT SET_OUTPUT(X_ENABLE_PIN)
    #define X_ENABLE_WRITE(STATE) WRITE(X_ENABLE_PIN,STATE)
    #define X_ENABLE_READ READ(X_ENABLE_PIN)
  #endif
  #define X_DIR_INIT SET_OUTPUT(X_DIR_PIN)
  #define X_DIR_WRITE(STATE) WRITE(X_DIR_PIN,STATE)
  #define X_DIR_READ READ(X_DIR_PIN)
#endif
#define X_STEP_INIT SET_OUTPUT(X_STEP_PIN)
#define X_STEP_WRITE(STATE) WRITE(X_STEP_PIN,STATE)
#define X_STEP_READ READ(X_STEP_PIN)

// Y Stepper
#if ENABLED(HAVE_L6470DRIVER) && ENABLED(Y_IS_L6470)
  extern L6470 stepperY;
  #define Y_ENABLE_INIT NOOP
  #define Y_ENABLE_WRITE(STATE) do{if(STATE) stepperY.Step_Clock(stepperY.getStatus() & STATUS_HIZ); else stepperY.softFree();}while(0)
  #define Y_ENABLE_READ (stepperY.getStatus() & STATUS_HIZ)
  #define Y_DIR_INIT NOOP
  #define Y_DIR_WRITE(STATE) stepperY.Step_Clock(STATE)
  #define Y_DIR_READ (stepperY.getStatus() & STATUS_DIR)
#else
  #if ENABLED(HAVE_TMCDRIVER) && ENABLED(Y_IS_TMC)
    extern TMC26XStepper stepperY;
    #define Y_ENABLE_INIT NOOP
    #define Y_ENABLE_WRITE(STATE) stepperY.setEnabled(STATE)
    #define Y_ENABLE_READ stepperY.isEnabled()
  #else
    #define Y_ENABLE_INIT SET_OUTPUT(Y_ENABLE_PIN)
    #define Y_ENABLE_WRITE(STATE) WRITE(Y_ENABLE_PIN,STATE)
    #define Y_ENABLE_READ READ(Y_ENABLE_PIN)
  #endif
  #define Y_DIR_INIT SET_OUTPUT(Y_DIR_PIN)
  #define Y_DIR_WRITE(STATE) WRITE(Y_DIR_PIN,STATE)
  #define Y_DIR_READ READ(Y_DIR_PIN)
#endif
#define Y_STEP_INIT SET_OUTPUT(Y_STEP_PIN)
#define Y_STEP_WRITE(STATE) WRITE(Y_STEP_PIN,STATE)
#define Y_STEP_READ READ(Y_STEP_PIN)

// Z Stepper
#if ENABLED(HAVE_L6470DRIVER) && ENABLED(Z_IS_L6470)
  extern L6470 stepperZ;
  #define Z_ENABLE_INIT NOOP
  #define Z_ENABLE_WRITE(STATE) do{if(STATE) stepperZ.Step_Clock(stepperZ.getStatus() & STATUS_HIZ); else stepperZ.softFree();}while(0)
  #define Z_ENABLE_READ (stepperZ.getStatus() & STATUS_HIZ)
  #define Z_DIR_INIT NOOP
  #define Z_DIR_WRITE(STATE) stepperZ.Step_Clock(STATE)
  #define Z_DIR_READ (stepperZ.getStatus() & STATUS_DIR)
#else
  #if ENABLED(HAVE_TMCDRIVER) && ENABLED(Z_IS_TMC)
    extern TMC26XStepper stepperZ;
    #define Z_ENABLE_INIT NOOP
    #define Z_ENABLE_WRITE(STATE) stepperZ.setEnabled(STATE)
    #define Z_ENABLE_READ stepperZ.isEnabled()
  #else
    #define Z_ENABLE_INIT SET_OUTPUT(Z_ENABLE_PIN)
    #define Z_ENABLE_WRITE(STATE) WRITE(Z_ENABLE_PIN,STATE)
    #define Z_ENABLE_READ READ(Z_ENABLE_PIN)
  #endif
  #define Z_DIR_INIT SET_OUTPUT(Z_DIR_PIN)
  #define Z_DIR_WRITE(STATE) WRITE(Z_DIR_PIN,STATE)
  #define Z_DIR_READ READ(Z_DIR_PIN)
#endif
#define Z_STEP_INIT SET_OUTPUT(Z_STEP_PIN)
#define Z_STEP_WRITE(STATE) WRITE(Z_STEP_PIN,STATE)
#define Z_STEP_READ READ(Z_STEP_PIN)

// X2 Stepper
#if HAS_X2_ENABLE
  #if ENABLED(HAVE_L6470DRIVER) && ENABLED(X2_IS_L6470)
    extern L6470 stepperX2;
    #define X2_ENABLE_INIT NOOP
    #define X2_ENABLE_WRITE(STATE) do{if(STATE) stepperX2.Step_Clock(stepperX2.getStatus() & STATUS_HIZ); else stepperX2.softFree();}while(0)
    #define X2_ENABLE_READ (stepperX2.getStatus() & STATUS_HIZ)
    #define X2_DIR_INIT NOOP
    #define X2_DIR_WRITE(STATE) stepperX2.Step_Clock(STATE)
    #define X2_DIR_READ (stepperX2.getStatus() & STATUS_DIR)
  #else
    #if ENABLED(HAVE_TMCDRIVER) && ENABLED(X2_IS_TMC)
      extern TMC26XStepper stepperX2;
      #define X2_ENABLE_INIT NOOP
      #define X2_ENABLE_WRITE(STATE) stepperX2.setEnabled(STATE)
      #define X2_ENABLE_READ stepperX2.isEnabled()
    #else
      #define X2_ENABLE_INIT SET_OUTPUT(X2_ENABLE_PIN)
      #define X2_ENABLE_WRITE(STATE) WRITE(X2_ENABLE_PIN,STATE)
      #define X2_ENABLE_READ READ(X2_ENABLE_PIN)
    #endif
    #define X2_DIR_INIT SET_OUTPUT(X2_DIR_PIN)
    #define X2_DIR_WRITE(STATE) WRITE(X2_DIR_PIN,STATE)
    #define X2_DIR_READ READ(X2_DIR_PIN)
  #endif
  #define X2_STEP_INIT SET_OUTPUT(X2_STEP_PIN)
  #define X2_STEP_WRITE(STATE) WRITE(X2_STEP_PIN,STATE)
  #define X2_STEP_READ READ(X2_STEP_PIN)
#endif

// Y2 Stepper
#if HAS_Y2_ENABLE
  #if ENABLED(HAVE_L6470DRIVER) && ENABLED(Y2_IS_L6470)
    extern L6470 stepperY2;
    #define Y2_ENABLE_INIT NOOP
    #define Y2_ENABLE_WRITE(STATE) do{if(STATE) stepperY2.Step_Clock(stepperY2.getStatus() & STATUS_HIZ); else stepperY2.softFree();}while(0)
    #define Y2_ENABLE_READ (stepperY2.getStatus() & STATUS_HIZ)
    #define Y2_DIR_INIT NOOP
    #define Y2_DIR_WRITE(STATE) stepperY2.Step_Clock(STATE)
    #define Y2_DIR_READ (stepperY2.getStatus() & STATUS_DIR)
  #else
    #if ENABLED(HAVE_TMCDRIVER) && ENABLED(Y2_IS_TMC)
      extern TMC26XStepper stepperY2;
      #define Y2_ENABLE_INIT NOOP
      #define Y2_ENABLE_WRITE(STATE) stepperY2.setEnabled(STATE)
      #define Y2_ENABLE_READ stepperY2.isEnabled()
    #else
      #define Y2_ENABLE_INIT SET_OUTPUT(Y2_ENABLE_PIN)
      #define Y2_ENABLE_WRITE(STATE) WRITE(Y2_ENABLE_PIN,STATE)
      #define Y2_ENABLE_READ READ(Y2_ENABLE_PIN)
    #endif
    #define Y2_DIR_INIT SET_OUTPUT(Y2_DIR_PIN)
    #define Y2_DIR_WRITE(STATE) WRITE(Y2_DIR_PIN,STATE)
    #define Y2_DIR_READ READ(Y2_DIR_PIN)
  #endif
  #define Y2_STEP_INIT SET_OUTPUT(Y2_STEP_PIN)
  #define Y2_STEP_WRITE(STATE) WRITE(Y2_STEP_PIN,STATE)
  #define Y2_STEP_READ READ(Y2_STEP_PIN)
#endif

// Z2 Stepper
#if HAS_Z2_ENABLE
  #if ENABLED(HAVE_L6470DRIVER) && ENABLED(Z2_IS_L6470)
    extern L6470 stepperZ2;
    #define Z2_ENABLE_INIT NOOP
    #define Z2_ENABLE_WRITE(STATE) do{if(STATE) stepperZ2.Step_Clock(stepperZ2.getStatus() & STATUS_HIZ); else stepperZ2.softFree();}while(0)
    #define Z2_ENABLE_READ (stepperZ2.getStatus() & STATUS_HIZ)
    #define Z2_DIR_INIT NOOP
    #define Z2_DIR_WRITE(STATE) stepperZ2.Step_Clock(STATE)
    #define Z2_DIR_READ (stepperZ2.getStatus() & STATUS_DIR)
  #else
    #if ENABLED(HAVE_TMCDRIVER) && ENABLED(Z2_IS_TMC)
      extern TMC26XStepper stepperZ2;
      #define Z2_ENABLE_INIT NOOP
      #define Z2_ENABLE_WRITE(STATE) stepperZ2.setEnabled(STATE)
      #define Z2_ENABLE_READ stepperZ2.isEnabled()
    #else
      #define Z2_ENABLE_INIT SET_OUTPUT(Z2_ENABLE_PIN)
      #define Z2_ENABLE_WRITE(STATE) WRITE(Z2_ENABLE_PIN,STATE)
      #define Z2_ENABLE_READ READ(Z2_ENABLE_PIN)
    #endif
    #define Z2_DIR_INIT SET_OUTPUT(Z2_DIR_PIN)
    #define Z2_DIR_WRITE(STATE) WRITE(Z2_DIR_PIN,STATE)
    #define Z2_DIR_READ READ(Z2_DIR_PIN)
  #endif
  #define Z2_STEP_INIT SET_OUTPUT(Z2_STEP_PIN)
  #define Z2_STEP_WRITE(STATE) WRITE(Z2_STEP_PIN,STATE)
  #define Z2_STEP_READ READ(Z2_STEP_PIN)
#endif

// E0 Stepper
#if ENABLED(HAVE_L6470DRIVER) && ENABLED(E0_IS_L6470)
  extern L6470 stepperE0;
  #define E0_ENABLE_INIT NOOP
  #define E0_ENABLE_WRITE(STATE) do{if(STATE) stepperE0.Step_Clock(stepperE0.getStatus() & STATUS_HIZ); else stepperE0.softFree();}while(0)
  #define E0_ENABLE_READ (stepperE0.getStatus() & STATUS_HIZ)
  #define E0_DIR_INIT NOOP
  #define E0_DIR_WRITE(STATE) stepperE0.Step_Clock(STATE)
  #define E0_DIR_READ (stepperE0.getStatus() & STATUS_DIR)
#else
  #if ENABLED(HAVE_TMCDRIVER) && ENABLED(E0_IS_TMC)
    extern TMC26XStepper stepperE0;
    #define E0_ENABLE_INIT NOOP
    #define E0_ENABLE_WRITE(STATE) stepperE0.setEnabled(STATE)
    #define E0_ENABLE_READ stepperE0.isEnabled()
  #else
    #define E0_ENABLE_INIT SET_OUTPUT(E0_ENABLE_PIN)
    #define E0_ENABLE_WRITE(STATE) WRITE(E0_ENABLE_PIN,STATE)
    #define E0_ENABLE_READ READ(E0_ENABLE_PIN)
  #endif
  #define E0_DIR_INIT SET_OUTPUT(E0_DIR_PIN)
  #define E0_DIR_WRITE(STATE) WRITE(E0_DIR_PIN,STATE)
  #define E0_DIR_READ READ(E0_DIR_PIN)
#endif
#define E0_STEP_INIT SET_OUTPUT(E0_STEP_PIN)
#define E0_STEP_WRITE(STATE) WRITE(E0_STEP_PIN,STATE)
#define E0_STEP_READ READ(E0_STEP_PIN)

// E1 Stepper
#if ENABLED(HAVE_L6470DRIVER) && ENABLED(E1_IS_L6470)
  extern L6470 stepperE1;
  #define E1_ENABLE_INIT NOOP
  #define E1_ENABLE_WRITE(STATE) do{if(STATE) stepperE1.Step_Clock(stepperE1.getStatus() & STATUS_HIZ); else stepperE1.softFree();}while(0)
  #define E1_ENABLE_READ (stepperE1.getStatus() & STATUS_HIZ)
  #define E1_DIR_INIT NOOP
  #define E1_DIR_WRITE(STATE) stepperE1.Step_Clock(STATE)
  #define E1_DIR_READ (stepperE1.getStatus() & STATUS_DIR)
#else
  #if ENABLED(HAVE_TMCDRIVER) && ENABLED(E1_IS_TMC)
    extern TMC26XStepper stepperE1;
    #define E1_ENABLE_INIT NOOP
    #define E1_ENABLE_WRITE(STATE) stepperE1.setEnabled(STATE)
    #define E1_ENABLE_READ stepperE1.isEnabled()
  #else
    #define E1_ENABLE_INIT SET_OUTPUT(E1_ENABLE_PIN)
    #define E1_ENABLE_WRITE(STATE) WRITE(E1_ENABLE_PIN,STATE)
    #define E1_ENABLE_READ READ(E1_ENABLE_PIN)
  #endif
  #define E1_DIR_INIT SET_OUTPUT(E1_DIR_PIN)
  #define E1_DIR_WRITE(STATE) WRITE(E1_DIR_PIN,STATE)
  #define E1_DIR_READ READ(E1_DIR_PIN)
#endif
#define E1_STEP_INIT SET_OUTPUT(E1_STEP_PIN)
#define E1_STEP_WRITE(STATE) WRITE(E1_STEP_PIN,STATE)
#define E1_STEP_READ READ(E1_STEP_PIN)

// E2 Stepper
#if ENABLED(HAVE_L6470DRIVER) && ENABLED(E2_IS_L6470)
  extern L6470 stepperE2;
  #define E2_ENABLE_INIT NOOP
  #define E2_ENABLE_WRITE(STATE) do{if(STATE) stepperE2.Step_Clock(stepperE2.getStatus() & STATUS_HIZ); else stepperE2.softFree();}while(0)
  #define E2_ENABLE_READ (stepperE2.getStatus() & STATUS_HIZ)
  #define E2_DIR_INIT NOOP
  #define E2_DIR_WRITE(STATE) stepperE2.Step_Clock(STATE)
  #define E2_DIR_READ (stepperE2.getStatus() & STATUS_DIR)
#else
  #if ENABLED(HAVE_TMCDRIVER) && ENABLED(E2_IS_TMC)
    extern TMC26XStepper stepperE2;
    #define E2_ENABLE_INIT NOOP
    #define E2_ENABLE_WRITE(STATE) stepperE2.setEnabled(STATE)
    #define E2_ENABLE_READ stepperE2.isEnabled()
  #else
    #define E2_ENABLE_INIT SET_OUTPUT(E2_ENABLE_PIN)
    #define E2_ENABLE_WRITE(STATE) WRITE(E2_ENABLE_PIN,STATE)
    #define E2_ENABLE_READ READ(E2_ENABLE_PIN)
  #endif
  #define E2_DIR_INIT SET_OUTPUT(E2_DIR_PIN)
  #define E2_DIR_WRITE(STATE) WRITE(E2_DIR_PIN,STATE)
  #define E2_DIR_READ READ(E2_DIR_PIN)
#endif
#define E2_STEP_INIT SET_OUTPUT(E2_STEP_PIN)
#define E2_STEP_WRITE(STATE) WRITE(E2_STEP_PIN,STATE)
#define E2_STEP_READ READ(E2_STEP_PIN)

// E3 Stepper
#if ENABLED(HAVE_L6470DRIVER) && ENABLED(E3_IS_L6470)
  extern L6470 stepperE3;
  #define E3_ENABLE_INIT NOOP
  #define E3_ENABLE_WRITE(STATE) do{if(STATE) stepperE3.Step_Clock(stepperE3.getStatus() & STATUS_HIZ); else stepperE3.softFree();}while(0)
  #define E3_ENABLE_READ (stepperE3.getStatus() & STATUS_HIZ)
  #define E3_DIR_INIT NOOP
  #define E3_DIR_WRITE(STATE) stepperE3.Step_Clock(STATE)
  #define E3_DIR_READ (stepperE3.getStatus() & STATUS_DIR)
#else
  #if ENABLED(HAVE_TMCDRIVER) && ENABLED(E3_IS_TMC)
    extern TMC26XStepper stepperE3;
    #define E3_ENABLE_INIT NOOP
    #define E3_ENABLE_WRITE(STATE) stepperE3.setEnabled(STATE)
    #define E3_ENABLE_READ stepperE3.isEnabled()
  #else
    #define E3_ENABLE_INIT SET_OUTPUT(E3_ENABLE_PIN)
    #define E3_ENABLE_WRITE(STATE) WRITE(E3_ENABLE_PIN,STATE)
    #define E3_ENABLE_READ READ(E3_ENABLE_PIN)
  #endif
  #define E3_DIR_INIT SET_OUTPUT(E3_DIR_PIN)
  #define E3_DIR_WRITE(STATE) WRITE(E3_DIR_PIN,STATE)
  #define E3_DIR_READ READ(E3_DIR_PIN)
#endif
#define E3_STEP_INIT SET_OUTPUT(E3_STEP_PIN)
#define E3_STEP_WRITE(STATE) WRITE(E3_STEP_PIN,STATE)
#define E3_STEP_READ READ(E3_STEP_PIN)

/**
 * Extruder indirection for the single E axis
 */
#if ENABLED(SWITCHING_EXTRUDER)
  #define E_STEP_WRITE(v) E0_STEP_WRITE(v)
  #define NORM_E_DIR() E0_DIR_WRITE(current_block->active_extruder ?  INVERT_E0_DIR : !INVERT_E0_DIR)
  #define  REV_E_DIR() E0_DIR_WRITE(current_block->active_extruder ? !INVERT_E0_DIR :  INVERT_E0_DIR)
#elif EXTRUDERS > 3
  #define E_STEP_WRITE(v) { switch (current_block->active_extruder) { case 0: E0_STEP_WRITE(v); break; case 1: E1_STEP_WRITE(v); break; case 2: E2_STEP_WRITE(v); break; case 3: E3_STEP_WRITE(v); } }
  #define NORM_E_DIR() { switch (current_block->active_extruder) { case 0: E0_DIR_WRITE(!INVERT_E0_DIR); break; case 1: E1_DIR_WRITE(!INVERT_E1_DIR); break; case 2: E2_DIR_WRITE(!INVERT_E2_DIR); break; case 3: E3_DIR_WRITE(!INVERT_E3_DIR); } }
  #define REV_E_DIR() { switch (current_block->active_extruder) { case 0: E0_DIR_WRITE(INVERT_E0_DIR); break; case 1: E1_DIR_WRITE(INVERT_E1_DIR); break; case 2: E2_DIR_WRITE(INVERT_E2_DIR); break; case 3: E3_DIR_WRITE(INVERT_E3_DIR); } }
#elif EXTRUDERS > 2
  #define E_STEP_WRITE(v) { switch (current_block->active_extruder) { case 0: E0_STEP_WRITE(v); break; case 1: E1_STEP_WRITE(v); break; case 2: E2_STEP_WRITE(v); } }
  #define NORM_E_DIR() { switch (current_block->active_extruder) { case 0: E0_DIR_WRITE(!INVERT_E0_DIR); break; case 1: E1_DIR_WRITE(!INVERT_E1_DIR); break; case 2: E2_DIR_WRITE(!INVERT_E2_DIR); } }
  #define REV_E_DIR() { switch (current_block->active_extruder) { case 0: E0_DIR_WRITE(INVERT_E0_DIR); break; case 1: E1_DIR_WRITE(INVERT_E1_DIR); break; case 2: E2_DIR_WRITE(INVERT_E2_DIR); } }
#elif EXTRUDERS > 1
  #if ENABLED(DUAL_X_CARRIAGE) || ENABLED(DUAL_NOZZLE_DUPLICATION_MODE)
    #define E_STEP_WRITE(v) { if (extruder_duplication_enabled) { E0_STEP_WRITE(v); E1_STEP_WRITE(v); } else if (current_block->active_extruder == 0) { E0_STEP_WRITE(v); } else { E1_STEP_WRITE(v); } }
    #define NORM_E_DIR() { if (extruder_duplication_enabled) { E0_DIR_WRITE(!INVERT_E0_DIR); E1_DIR_WRITE(!INVERT_E1_DIR); } else if (current_block->active_extruder == 0) { E0_DIR_WRITE(!INVERT_E0_DIR); } else { E1_DIR_WRITE(!INVERT_E1_DIR); } }
    #define REV_E_DIR() { if (extruder_duplication_enabled) { E0_DIR_WRITE(INVERT_E0_DIR); E1_DIR_WRITE(INVERT_E1_DIR); } else if (current_block->active_extruder == 0) { E0_DIR_WRITE(INVERT_E0_DIR); } else { E1_DIR_WRITE(INVERT_E1_DIR); } }
  #else
    #define E_STEP_WRITE(v) { if (current_block->active_extruder == 0) { E0_STEP_WRITE(v); } else { E1_STEP_WRITE(v); } }
    #define NORM_E_DIR() { if (current_block->active_extruder == 0) { E0_DIR_WRITE(!INVERT_E0_DIR); } else { E1_DIR_WRITE(!INVERT_E1_DIR); } }
    #define REV_E_DIR() { if (current_block->active_extruder == 0) { E0_DIR_WRITE(INVERT_E0_DIR); } else { E1_DIR_WRITE(INVERT_E1_DIR); } }
  #endif
#elif ENABLED(MIXING_EXTRUDER)
  #define E_STEP_WRITE(v) NOOP /* not used for mixing extruders! */
  #if MIXING_STEPPERS > 3
    #define En_STEP_WRITE(n,v) { switch (n) { case 0: E0_STEP_WRITE(v); break; case 1: E1_STEP_WRITE(v); break; case 2: E2_STEP_WRITE(v); break; case 3: E3_STEP_WRITE(v); } }
    #define NORM_E_DIR() { E0_DIR_WRITE(!INVERT_E0_DIR); E1_DIR_WRITE(!INVERT_E1_DIR); E2_DIR_WRITE(!INVERT_E2_DIR); E3_DIR_WRITE(!INVERT_E3_DIR); }
    #define REV_E_DIR()  { E0_DIR_WRITE( INVERT_E0_DIR); E1_DIR_WRITE( INVERT_E1_DIR); E2_DIR_WRITE( INVERT_E2_DIR); E3_DIR_WRITE( INVERT_E3_DIR); }
  #elif MIXING_STEPPERS > 2
    #define En_STEP_WRITE(n,v) { switch (n) { case 0: E0_STEP_WRITE(v); break; case 1: E1_STEP_WRITE(v); break; case 2: E2_STEP_WRITE(v); } }
    #define NORM_E_DIR() { E0_DIR_WRITE(!INVERT_E0_DIR); E1_DIR_WRITE(!INVERT_E1_DIR); E2_DIR_WRITE(!INVERT_E2_DIR); }
    #define REV_E_DIR()  { E0_DIR_WRITE( INVERT_E0_DIR); E1_DIR_WRITE( INVERT_E1_DIR); E2_DIR_WRITE( INVERT_E2_DIR); }
  #else
    #define En_STEP_WRITE(n,v) { switch (n) { case 0: E0_STEP_WRITE(v); break; case 1: E1_STEP_WRITE(v); } }
    #define NORM_E_DIR() { E0_DIR_WRITE(!INVERT_E0_DIR); E1_DIR_WRITE(!INVERT_E1_DIR); }
    #define REV_E_DIR()  { E0_DIR_WRITE( INVERT_E0_DIR); E1_DIR_WRITE( INVERT_E1_DIR); }
  #endif
#else
  #define E_STEP_WRITE(v) E0_STEP_WRITE(v)
  #define NORM_E_DIR() E0_DIR_WRITE(!INVERT_E0_DIR)
  #define REV_E_DIR() E0_DIR_WRITE(INVERT_E0_DIR)
#endif

#endif // STEPPER_INDIRECTION_H
