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
 * stepper/indirection.h
 *
 * Stepper motor driver indirection to allow some stepper functions to
 * be done via SPI/I2c instead of direct pin manipulation.
 *
 * Copyright (c) 2015 Dominik Wenger
 */

#include "../../inc/MarlinConfig.h"

#if HAS_L64XX
  #include "L64xx.h"
#endif

#if HAS_DRIVER(TMC26X)
  #include "TMC26X.h"
#endif

#if HAS_TRINAMIC
  #include "trinamic.h"
#endif

void restore_stepper_drivers();  // Called by PSU_ON
void reset_stepper_drivers();    // Called by settings.load / settings.reset

// X Stepper
#ifndef X_ENABLE_INIT
  #define X_ENABLE_INIT() SET_OUTPUT(X_ENABLE_PIN)
  #define X_ENABLE_WRITE(STATE) WRITE(X_ENABLE_PIN,STATE)
  #define X_ENABLE_READ() bool(READ(X_ENABLE_PIN))
#endif
#ifndef X_DIR_INIT
  #define X_DIR_INIT() SET_OUTPUT(X_DIR_PIN)
  #define X_DIR_WRITE(STATE) WRITE(X_DIR_PIN,STATE)
  #define X_DIR_READ() bool(READ(X_DIR_PIN))
#endif
#define X_STEP_INIT() SET_OUTPUT(X_STEP_PIN)
#ifndef X_STEP_WRITE
  #define X_STEP_WRITE(STATE) WRITE(X_STEP_PIN,STATE)
#endif
#define X_STEP_READ() bool(READ(X_STEP_PIN))

// Y Stepper
#ifndef Y_ENABLE_INIT
  #define Y_ENABLE_INIT() SET_OUTPUT(Y_ENABLE_PIN)
  #define Y_ENABLE_WRITE(STATE) WRITE(Y_ENABLE_PIN,STATE)
  #define Y_ENABLE_READ() bool(READ(Y_ENABLE_PIN))
#endif
#ifndef Y_DIR_INIT
  #define Y_DIR_INIT() SET_OUTPUT(Y_DIR_PIN)
  #define Y_DIR_WRITE(STATE) WRITE(Y_DIR_PIN,STATE)
  #define Y_DIR_READ() bool(READ(Y_DIR_PIN))
#endif
#define Y_STEP_INIT() SET_OUTPUT(Y_STEP_PIN)
#ifndef Y_STEP_WRITE
  #define Y_STEP_WRITE(STATE) WRITE(Y_STEP_PIN,STATE)
#endif
#define Y_STEP_READ() bool(READ(Y_STEP_PIN))

// Z Stepper
#ifndef Z_ENABLE_INIT
  #define Z_ENABLE_INIT() SET_OUTPUT(Z_ENABLE_PIN)
  #define Z_ENABLE_WRITE(STATE) WRITE(Z_ENABLE_PIN,STATE)
  #define Z_ENABLE_READ() bool(READ(Z_ENABLE_PIN))
#endif
#ifndef Z_DIR_INIT
  #define Z_DIR_INIT() SET_OUTPUT(Z_DIR_PIN)
  #define Z_DIR_WRITE(STATE) WRITE(Z_DIR_PIN,STATE)
  #define Z_DIR_READ() bool(READ(Z_DIR_PIN))
#endif
#define Z_STEP_INIT() SET_OUTPUT(Z_STEP_PIN)
#ifndef Z_STEP_WRITE
  #define Z_STEP_WRITE(STATE) WRITE(Z_STEP_PIN,STATE)
#endif
#define Z_STEP_READ() bool(READ(Z_STEP_PIN))

// X2 Stepper
#if HAS_X2_ENABLE
  #ifndef X2_ENABLE_INIT
    #define X2_ENABLE_INIT() SET_OUTPUT(X2_ENABLE_PIN)
    #define X2_ENABLE_WRITE(STATE) WRITE(X2_ENABLE_PIN,STATE)
    #define X2_ENABLE_READ() bool(READ(X2_ENABLE_PIN))
  #endif
  #ifndef X2_DIR_INIT
    #define X2_DIR_INIT() SET_OUTPUT(X2_DIR_PIN)
    #define X2_DIR_WRITE(STATE) WRITE(X2_DIR_PIN,STATE)
    #define X2_DIR_READ() bool(READ(X2_DIR_PIN))
  #endif
  #define X2_STEP_INIT() SET_OUTPUT(X2_STEP_PIN)
  #ifndef X2_STEP_WRITE
    #define X2_STEP_WRITE(STATE) WRITE(X2_STEP_PIN,STATE)
  #endif
  #define X2_STEP_READ() bool(READ(X2_STEP_PIN))
#endif

// Y2 Stepper
#if HAS_Y2_ENABLE
  #ifndef Y2_ENABLE_INIT
    #define Y2_ENABLE_INIT() SET_OUTPUT(Y2_ENABLE_PIN)
    #define Y2_ENABLE_WRITE(STATE) WRITE(Y2_ENABLE_PIN,STATE)
    #define Y2_ENABLE_READ() bool(READ(Y2_ENABLE_PIN))
  #endif
  #ifndef Y2_DIR_INIT
    #define Y2_DIR_INIT() SET_OUTPUT(Y2_DIR_PIN)
    #define Y2_DIR_WRITE(STATE) WRITE(Y2_DIR_PIN,STATE)
    #define Y2_DIR_READ() bool(READ(Y2_DIR_PIN))
  #endif
  #define Y2_STEP_INIT() SET_OUTPUT(Y2_STEP_PIN)
  #ifndef Y2_STEP_WRITE
    #define Y2_STEP_WRITE(STATE) WRITE(Y2_STEP_PIN,STATE)
  #endif
  #define Y2_STEP_READ() bool(READ(Y2_STEP_PIN))
#else
  #define Y2_DIR_WRITE(STATE) NOOP
#endif

// Z2 Stepper
#if HAS_Z2_ENABLE
  #ifndef Z2_ENABLE_INIT
    #define Z2_ENABLE_INIT() SET_OUTPUT(Z2_ENABLE_PIN)
    #define Z2_ENABLE_WRITE(STATE) WRITE(Z2_ENABLE_PIN,STATE)
    #define Z2_ENABLE_READ() bool(READ(Z2_ENABLE_PIN))
  #endif
  #ifndef Z2_DIR_INIT
    #define Z2_DIR_INIT() SET_OUTPUT(Z2_DIR_PIN)
    #define Z2_DIR_WRITE(STATE) WRITE(Z2_DIR_PIN,STATE)
    #define Z2_DIR_READ() bool(READ(Z2_DIR_PIN))
  #endif
  #define Z2_STEP_INIT() SET_OUTPUT(Z2_STEP_PIN)
  #ifndef Z2_STEP_WRITE
    #define Z2_STEP_WRITE(STATE) WRITE(Z2_STEP_PIN,STATE)
  #endif
  #define Z2_STEP_READ() bool(READ(Z2_STEP_PIN))
#else
  #define Z2_DIR_WRITE(STATE) NOOP
#endif

// Z3 Stepper
#if HAS_Z3_ENABLE
  #ifndef Z3_ENABLE_INIT
    #define Z3_ENABLE_INIT() SET_OUTPUT(Z3_ENABLE_PIN)
    #define Z3_ENABLE_WRITE(STATE) WRITE(Z3_ENABLE_PIN,STATE)
    #define Z3_ENABLE_READ() bool(READ(Z3_ENABLE_PIN))
  #endif
  #ifndef Z3_DIR_INIT
    #define Z3_DIR_INIT() SET_OUTPUT(Z3_DIR_PIN)
    #define Z3_DIR_WRITE(STATE) WRITE(Z3_DIR_PIN,STATE)
    #define Z3_DIR_READ() bool(READ(Z3_DIR_PIN))
  #endif
  #define Z3_STEP_INIT() SET_OUTPUT(Z3_STEP_PIN)
  #ifndef Z3_STEP_WRITE
    #define Z3_STEP_WRITE(STATE) WRITE(Z3_STEP_PIN,STATE)
  #endif
  #define Z3_STEP_READ() bool(READ(Z3_STEP_PIN))
#else
  #define Z3_DIR_WRITE(STATE) NOOP
#endif

// Z4 Stepper
#if HAS_Z4_ENABLE
  #ifndef Z4_ENABLE_INIT
    #define Z4_ENABLE_INIT() SET_OUTPUT(Z4_ENABLE_PIN)
    #define Z4_ENABLE_WRITE(STATE) WRITE(Z4_ENABLE_PIN,STATE)
    #define Z4_ENABLE_READ() READ(Z4_ENABLE_PIN)
  #endif
  #ifndef Z4_DIR_INIT
    #define Z4_DIR_INIT() SET_OUTPUT(Z4_DIR_PIN)
    #define Z4_DIR_WRITE(STATE) WRITE(Z4_DIR_PIN,STATE)
    #define Z4_DIR_READ() READ(Z4_DIR_PIN)
  #endif
  #define Z4_STEP_INIT SET_OUTPUT(Z4_STEP_PIN)
  #ifndef Z4_STEP_WRITE
    #define Z4_STEP_WRITE(STATE) WRITE(Z4_STEP_PIN,STATE)
  #endif
  #define Z4_STEP_READ READ(Z4_STEP_PIN)
#else
  #define Z4_DIR_WRITE(STATE) NOOP
#endif

// E0 Stepper
#ifndef E0_ENABLE_INIT
  #define E0_ENABLE_INIT() SET_OUTPUT(E0_ENABLE_PIN)
  #define E0_ENABLE_WRITE(STATE) WRITE(E0_ENABLE_PIN,STATE)
  #define E0_ENABLE_READ() bool(READ(E0_ENABLE_PIN))
#endif
#ifndef E0_DIR_INIT
  #define E0_DIR_INIT() SET_OUTPUT(E0_DIR_PIN)
  #define E0_DIR_WRITE(STATE) WRITE(E0_DIR_PIN,STATE)
  #define E0_DIR_READ() bool(READ(E0_DIR_PIN))
#endif
#define E0_STEP_INIT() SET_OUTPUT(E0_STEP_PIN)
#ifndef E0_STEP_WRITE
  #define E0_STEP_WRITE(STATE) WRITE(E0_STEP_PIN,STATE)
#endif
#define E0_STEP_READ() bool(READ(E0_STEP_PIN))

// E1 Stepper
#ifndef E1_ENABLE_INIT
  #define E1_ENABLE_INIT() SET_OUTPUT(E1_ENABLE_PIN)
  #define E1_ENABLE_WRITE(STATE) WRITE(E1_ENABLE_PIN,STATE)
  #define E1_ENABLE_READ() bool(READ(E1_ENABLE_PIN))
#endif
#ifndef E1_DIR_INIT
  #define E1_DIR_INIT() SET_OUTPUT(E1_DIR_PIN)
  #define E1_DIR_WRITE(STATE) WRITE(E1_DIR_PIN,STATE)
  #define E1_DIR_READ() bool(READ(E1_DIR_PIN))
#endif
#define E1_STEP_INIT() SET_OUTPUT(E1_STEP_PIN)
#ifndef E1_STEP_WRITE
  #define E1_STEP_WRITE(STATE) WRITE(E1_STEP_PIN,STATE)
#endif
#define E1_STEP_READ() bool(READ(E1_STEP_PIN))

// E2 Stepper
#ifndef E2_ENABLE_INIT
  #define E2_ENABLE_INIT() SET_OUTPUT(E2_ENABLE_PIN)
  #define E2_ENABLE_WRITE(STATE) WRITE(E2_ENABLE_PIN,STATE)
  #define E2_ENABLE_READ() bool(READ(E2_ENABLE_PIN))
#endif
#ifndef E2_DIR_INIT
  #define E2_DIR_INIT() SET_OUTPUT(E2_DIR_PIN)
  #define E2_DIR_WRITE(STATE) WRITE(E2_DIR_PIN,STATE)
  #define E2_DIR_READ() bool(READ(E2_DIR_PIN))
#endif
#define E2_STEP_INIT() SET_OUTPUT(E2_STEP_PIN)
#ifndef E2_STEP_WRITE
  #define E2_STEP_WRITE(STATE) WRITE(E2_STEP_PIN,STATE)
#endif
#define E2_STEP_READ() bool(READ(E2_STEP_PIN))

// E3 Stepper
#ifndef E3_ENABLE_INIT
  #define E3_ENABLE_INIT() SET_OUTPUT(E3_ENABLE_PIN)
  #define E3_ENABLE_WRITE(STATE) WRITE(E3_ENABLE_PIN,STATE)
  #define E3_ENABLE_READ() bool(READ(E3_ENABLE_PIN))
#endif
#ifndef E3_DIR_INIT
  #define E3_DIR_INIT() SET_OUTPUT(E3_DIR_PIN)
  #define E3_DIR_WRITE(STATE) WRITE(E3_DIR_PIN,STATE)
  #define E3_DIR_READ() bool(READ(E3_DIR_PIN))
#endif
#define E3_STEP_INIT() SET_OUTPUT(E3_STEP_PIN)
#ifndef E3_STEP_WRITE
  #define E3_STEP_WRITE(STATE) WRITE(E3_STEP_PIN,STATE)
#endif
#define E3_STEP_READ() bool(READ(E3_STEP_PIN))

// E4 Stepper
#ifndef E4_ENABLE_INIT
  #define E4_ENABLE_INIT() SET_OUTPUT(E4_ENABLE_PIN)
  #define E4_ENABLE_WRITE(STATE) WRITE(E4_ENABLE_PIN,STATE)
  #define E4_ENABLE_READ() bool(READ(E4_ENABLE_PIN))
#endif
#ifndef E4_DIR_INIT
  #define E4_DIR_INIT() SET_OUTPUT(E4_DIR_PIN)
  #define E4_DIR_WRITE(STATE) WRITE(E4_DIR_PIN,STATE)
  #define E4_DIR_READ() bool(READ(E4_DIR_PIN))
#endif
#define E4_STEP_INIT() SET_OUTPUT(E4_STEP_PIN)
#ifndef E4_STEP_WRITE
  #define E4_STEP_WRITE(STATE) WRITE(E4_STEP_PIN,STATE)
#endif
#define E4_STEP_READ() bool(READ(E4_STEP_PIN))

// E5 Stepper
#ifndef E5_ENABLE_INIT
  #define E5_ENABLE_INIT() SET_OUTPUT(E5_ENABLE_PIN)
  #define E5_ENABLE_WRITE(STATE) WRITE(E5_ENABLE_PIN,STATE)
  #define E5_ENABLE_READ() bool(READ(E5_ENABLE_PIN))
#endif
#ifndef E5_DIR_INIT
  #define E5_DIR_INIT() SET_OUTPUT(E5_DIR_PIN)
  #define E5_DIR_WRITE(STATE) WRITE(E5_DIR_PIN,STATE)
  #define E5_DIR_READ() bool(READ(E5_DIR_PIN))
#endif
#define E5_STEP_INIT() SET_OUTPUT(E5_STEP_PIN)
#ifndef E5_STEP_WRITE
  #define E5_STEP_WRITE(STATE) WRITE(E5_STEP_PIN,STATE)
#endif
#define E5_STEP_READ() bool(READ(E5_STEP_PIN))

// E6 Stepper
#ifndef E6_ENABLE_INIT
  #define E6_ENABLE_INIT() SET_OUTPUT(E6_ENABLE_PIN)
  #define E6_ENABLE_WRITE(STATE) WRITE(E6_ENABLE_PIN,STATE)
  #define E6_ENABLE_READ() bool(READ(E6_ENABLE_PIN))
#endif
#ifndef E6_DIR_INIT
  #define E6_DIR_INIT() SET_OUTPUT(E6_DIR_PIN)
  #define E6_DIR_WRITE(STATE) WRITE(E6_DIR_PIN,STATE)
  #define E6_DIR_READ() bool(READ(E6_DIR_PIN))
#endif
#define E6_STEP_INIT() SET_OUTPUT(E6_STEP_PIN)
#ifndef E6_STEP_WRITE
  #define E6_STEP_WRITE(STATE) WRITE(E6_STEP_PIN,STATE)
#endif
#define E6_STEP_READ() bool(READ(E6_STEP_PIN))

// E7 Stepper
#ifndef E7_ENABLE_INIT
  #define E7_ENABLE_INIT() SET_OUTPUT(E7_ENABLE_PIN)
  #define E7_ENABLE_WRITE(STATE) WRITE(E7_ENABLE_PIN,STATE)
  #define E7_ENABLE_READ() bool(READ(E7_ENABLE_PIN))
#endif
#ifndef E7_DIR_INIT
  #define E7_DIR_INIT() SET_OUTPUT(E7_DIR_PIN)
  #define E7_DIR_WRITE(STATE) WRITE(E7_DIR_PIN,STATE)
  #define E7_DIR_READ() bool(READ(E7_DIR_PIN))
#endif
#define E7_STEP_INIT() SET_OUTPUT(E7_STEP_PIN)
#ifndef E7_STEP_WRITE
  #define E7_STEP_WRITE(STATE) WRITE(E7_STEP_PIN,STATE)
#endif
#define E7_STEP_READ() bool(READ(E7_STEP_PIN))

/**
 * Extruder indirection for the single E axis
 */
#if ENABLED(SWITCHING_EXTRUDER) // One stepper driver per two extruders, reversed on odd index
  #if EXTRUDERS > 7
    #define E_STEP_WRITE(E,V) do{ if (E < 2) { E0_STEP_WRITE(V); } else if (E < 4) { E1_STEP_WRITE(V); } else if (E < 6) { E2_STEP_WRITE(V); } else { E3_STEP_WRITE(V); } }while(0)
    #define   NORM_E_DIR(E)   do{ switch (E) { \
        case 0: E0_DIR_WRITE(!INVERT_E0_DIR); break; case 1: E0_DIR_WRITE( INVERT_E0_DIR); break; \
        case 2: E1_DIR_WRITE(!INVERT_E1_DIR); break; case 3: E1_DIR_WRITE( INVERT_E1_DIR); break; \
        case 4: E2_DIR_WRITE(!INVERT_E2_DIR); break; case 5: E2_DIR_WRITE( INVERT_E2_DIR); break; \
        case 6: E3_DIR_WRITE( INVERT_E3_DIR); break; case 7: E3_DIR_WRITE( INVERT_E3_DIR); break; \
      } }while(0)
    #define    REV_E_DIR(E)   do{ switch (E) { \
        case 0: E0_DIR_WRITE( INVERT_E0_DIR); break; case 1: E0_DIR_WRITE(!INVERT_E0_DIR); break; \
        case 2: E1_DIR_WRITE( INVERT_E1_DIR); break; case 3: E1_DIR_WRITE(!INVERT_E1_DIR); break; \
        case 4: E2_DIR_WRITE( INVERT_E2_DIR); break; case 5: E2_DIR_WRITE(!INVERT_E2_DIR); break; \
        case 6: E3_DIR_WRITE(!INVERT_E3_DIR); break; case 7: E3_DIR_WRITE(!INVERT_E3_DIR); break; \
      } }while(0)
  #elif EXTRUDERS > 6
    #define E_STEP_WRITE(E,V) do{ if (E < 2) { E0_STEP_WRITE(V); } else if (E < 4) { E1_STEP_WRITE(V); } else if (E < 6) { E2_STEP_WRITE(V); } else { E3_STEP_WRITE(V); } }while(0)
    #define   NORM_E_DIR(E)   do{ switch (E) { \
        case 0: E0_DIR_WRITE(!INVERT_E0_DIR); break; case 1: E0_DIR_WRITE( INVERT_E0_DIR); break; \
        case 2: E1_DIR_WRITE(!INVERT_E1_DIR); break; case 3: E1_DIR_WRITE( INVERT_E1_DIR); break; \
        case 4: E2_DIR_WRITE(!INVERT_E2_DIR); break; case 5: E2_DIR_WRITE( INVERT_E2_DIR); break; \
        case 6: E3_DIR_WRITE( INVERT_E3_DIR); break; \
      } }while(0)
    #define    REV_E_DIR(E)   do{ switch (E) { \
        case 0: E0_DIR_WRITE( INVERT_E0_DIR); break; case 1: E0_DIR_WRITE(!INVERT_E0_DIR); break; \
        case 2: E1_DIR_WRITE( INVERT_E1_DIR); break; case 3: E1_DIR_WRITE(!INVERT_E1_DIR); break; \
        case 4: E2_DIR_WRITE( INVERT_E2_DIR); break; case 5: E2_DIR_WRITE(!INVERT_E2_DIR); break; \
        case 6: E3_DIR_WRITE(!INVERT_E3_DIR); } }while(0)
  #elif EXTRUDERS > 5
    #define E_STEP_WRITE(E,V) do{ if (E < 2) { E0_STEP_WRITE(V); } else if (E < 4) { E1_STEP_WRITE(V); } else { E2_STEP_WRITE(V); } }while(0)
    #define   NORM_E_DIR(E)   do{ switch (E) { \
        case 0: E0_DIR_WRITE(!INVERT_E0_DIR); break; case 1: E0_DIR_WRITE( INVERT_E0_DIR); break; \
        case 2: E1_DIR_WRITE(!INVERT_E1_DIR); break; case 3: E1_DIR_WRITE( INVERT_E1_DIR); break; \
        case 4: E2_DIR_WRITE(!INVERT_E2_DIR); break; case 5: E2_DIR_WRITE( INVERT_E2_DIR); break; \
      } }while(0)
    #define    REV_E_DIR(E)   do{ switch (E) { \
        case 0: E0_DIR_WRITE( INVERT_E0_DIR); break; case 1: E0_DIR_WRITE(!INVERT_E0_DIR); break; \
        case 2: E1_DIR_WRITE( INVERT_E1_DIR); break; case 3: E1_DIR_WRITE(!INVERT_E1_DIR); break; \
        case 4: E2_DIR_WRITE( INVERT_E2_DIR); break; case 5: E2_DIR_WRITE(!INVERT_E2_DIR); break; \
      } }while(0)
  #elif EXTRUDERS > 4
    #define E_STEP_WRITE(E,V) do{ if (E < 2) { E0_STEP_WRITE(V); } else if (E < 4) { E1_STEP_WRITE(V); } else { E2_STEP_WRITE(V); } }while(0)
    #define   NORM_E_DIR(E)   do{ switch (E) { \
        case 0: E0_DIR_WRITE(!INVERT_E0_DIR); break; case 1: E0_DIR_WRITE( INVERT_E0_DIR); break; \
        case 2: E1_DIR_WRITE(!INVERT_E1_DIR); break; case 3: E1_DIR_WRITE( INVERT_E1_DIR); break; \
        case 4: E2_DIR_WRITE(!INVERT_E2_DIR); break; \
      } }while(0)
    #define    REV_E_DIR(E)   do{ switch (E) { \
        case 0: E0_DIR_WRITE( INVERT_E0_DIR); break; case 1: E0_DIR_WRITE(!INVERT_E0_DIR); break; \
        case 2: E1_DIR_WRITE( INVERT_E1_DIR); break; case 3: E1_DIR_WRITE(!INVERT_E1_DIR); break; \
        case 4: E2_DIR_WRITE( INVERT_E2_DIR); break; \
      } }while(0)
  #elif EXTRUDERS > 3
    #define E_STEP_WRITE(E,V) do{ if (E < 2) { E0_STEP_WRITE(V); } else { E1_STEP_WRITE(V); } }while(0)
    #define   NORM_E_DIR(E)   do{ switch (E) { \
        case 0: E0_DIR_WRITE(!INVERT_E0_DIR); break; case 1: E0_DIR_WRITE( INVERT_E0_DIR); break; \
        case 2: E1_DIR_WRITE(!INVERT_E1_DIR); break; case 3: E1_DIR_WRITE( INVERT_E1_DIR); break; \
      } }while(0)
    #define    REV_E_DIR(E)   do{ switch (E) { \
        case 0: E0_DIR_WRITE( INVERT_E0_DIR); break; case 1: E0_DIR_WRITE(!INVERT_E0_DIR); break; \
        case 2: E1_DIR_WRITE( INVERT_E1_DIR); break; case 3: E1_DIR_WRITE(!INVERT_E1_DIR); break; \
      } }while(0)
  #elif EXTRUDERS > 2
    #define E_STEP_WRITE(E,V) do{ if (E < 2) { E0_STEP_WRITE(V); } else { E1_STEP_WRITE(V); } }while(0)
    #define   NORM_E_DIR(E)   do{ switch (E) { \
        case 0: E0_DIR_WRITE(!INVERT_E0_DIR); break; case 1: E0_DIR_WRITE( INVERT_E0_DIR); break; \
        case 2: E1_DIR_WRITE(!INVERT_E1_DIR); break; \
      } }while(0)
    #define    REV_E_DIR(E)   do{ switch (E) { \
        case 0: E0_DIR_WRITE( INVERT_E0_DIR); break; case 1: E0_DIR_WRITE(!INVERT_E0_DIR); break; \
        case 2: E1_DIR_WRITE( INVERT_E1_DIR); break; \
      } }while(0)
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

  #if E_STEPPERS > 7

    #define _E_STEP_WRITE(E,V) do{ switch (E) { \
        case 0: E0_STEP_WRITE(V); break; case 1: E1_STEP_WRITE(V); break; case 2: E2_STEP_WRITE(V); break; case 3: E3_STEP_WRITE(V); break; \
        case 4: E4_STEP_WRITE(V); break; case 5: E5_STEP_WRITE(V); break; case 6: E6_STEP_WRITE(V); break; case 7: E7_STEP_WRITE(V); break; \
      } }while(0)
    #define   _NORM_E_DIR(E)   do{ switch (E) { \
        case 0: E0_DIR_WRITE(!INVERT_E0_DIR); break; case 1: E1_DIR_WRITE(!INVERT_E1_DIR); break; \
        case 2: E2_DIR_WRITE(!INVERT_E2_DIR); break; case 3: E3_DIR_WRITE(!INVERT_E3_DIR); break; \
        case 4: E4_DIR_WRITE(!INVERT_E4_DIR); break; case 5: E5_DIR_WRITE(!INVERT_E5_DIR); break; \
        case 6: E6_DIR_WRITE(!INVERT_E6_DIR); break; case 7: E7_DIR_WRITE(!INVERT_E7_DIR); break; \
      } }while(0)
    #define    _REV_E_DIR(E)   do{ switch (E) { \
        case 0: E0_DIR_WRITE( INVERT_E0_DIR); break; case 1: E1_DIR_WRITE( INVERT_E1_DIR); break; \
        case 2: E2_DIR_WRITE( INVERT_E2_DIR); break; case 3: E3_DIR_WRITE( INVERT_E3_DIR); break; \
        case 4: E4_DIR_WRITE( INVERT_E4_DIR); break; case 5: E5_DIR_WRITE( INVERT_E5_DIR); break; \
        case 6: E6_DIR_WRITE( INVERT_E6_DIR); break; case 7: E7_DIR_WRITE( INVERT_E7_DIR); break; \
      } }while(0)

  #elif E_STEPPERS > 6

    #define _E_STEP_WRITE(E,V) do{ switch (E) { \
        case 0: E0_STEP_WRITE(V); break; case 1: E1_STEP_WRITE(V); break; case 2: E2_STEP_WRITE(V); break; case 3: E3_STEP_WRITE(V); break; \
        case 4: E4_STEP_WRITE(V); break; case 5: E5_STEP_WRITE(V); break; case 6: E6_STEP_WRITE(V); break; \
      } }while(0)
    #define   _NORM_E_DIR(E)   do{ switch (E) { \
        case 0: E0_DIR_WRITE(!INVERT_E0_DIR); break; case 1: E1_DIR_WRITE(!INVERT_E1_DIR); break; \
        case 2: E2_DIR_WRITE(!INVERT_E2_DIR); break; case 3: E3_DIR_WRITE(!INVERT_E3_DIR); break; \
        case 4: E4_DIR_WRITE(!INVERT_E4_DIR); break; case 5: E5_DIR_WRITE(!INVERT_E5_DIR); break; \
        case 6: E6_DIR_WRITE(!INVERT_E6_DIR); break; \
      } }while(0)
    #define    _REV_E_DIR(E)   do{ switch (E) { \
        case 0: E0_DIR_WRITE( INVERT_E0_DIR); break; case 1: E1_DIR_WRITE( INVERT_E1_DIR); break; \
        case 2: E2_DIR_WRITE( INVERT_E2_DIR); break; case 3: E3_DIR_WRITE( INVERT_E3_DIR); break; \
        case 4: E4_DIR_WRITE( INVERT_E4_DIR); break; case 5: E5_DIR_WRITE( INVERT_E5_DIR); break; \
        case 6: E6_DIR_WRITE( INVERT_E6_DIR); break; \
      } }while(0)

  #elif E_STEPPERS > 5

    #define _E_STEP_WRITE(E,V) do{ switch (E) { \
        case 0: E0_STEP_WRITE(V); break; case 1: E1_STEP_WRITE(V); break; case 2: E2_STEP_WRITE(V); break; case 3: E3_STEP_WRITE(V); break; \
        case 4: E4_STEP_WRITE(V); break; case 5: E5_STEP_WRITE(V); break; \
      } }while(0)
    #define   _NORM_E_DIR(E)   do{ switch (E) { \
        case 0: E0_DIR_WRITE(!INVERT_E0_DIR); break; case 1: E1_DIR_WRITE(!INVERT_E1_DIR); break; \
        case 2: E2_DIR_WRITE(!INVERT_E2_DIR); break; case 3: E3_DIR_WRITE(!INVERT_E3_DIR); break; \
        case 4: E4_DIR_WRITE(!INVERT_E4_DIR); break; case 5: E5_DIR_WRITE(!INVERT_E5_DIR); break; \
      } }while(0)
    #define    _REV_E_DIR(E)   do{ switch (E) { \
        case 0: E0_DIR_WRITE( INVERT_E0_DIR); break; case 1: E1_DIR_WRITE( INVERT_E1_DIR); break; \
        case 2: E2_DIR_WRITE( INVERT_E2_DIR); break; case 3: E3_DIR_WRITE( INVERT_E3_DIR); break; \
        case 4: E4_DIR_WRITE( INVERT_E4_DIR); break; case 5: E5_DIR_WRITE( INVERT_E5_DIR); break; \
      } }while(0)

  #elif E_STEPPERS > 4

    #define _E_STEP_WRITE(E,V) do{ switch (E) { \
        case 0: E0_STEP_WRITE(V); break; case 1: E1_STEP_WRITE(V); break; case 2: E2_STEP_WRITE(V); break; case 3: E3_STEP_WRITE(V); break; \
        case 4: E4_STEP_WRITE(V); break; \
      } }while(0)
    #define   _NORM_E_DIR(E)   do{ switch (E) { \
        case 0: E0_DIR_WRITE(!INVERT_E0_DIR); break; case 1: E1_DIR_WRITE(!INVERT_E1_DIR); break; \
        case 2: E2_DIR_WRITE(!INVERT_E2_DIR); break; case 3: E3_DIR_WRITE(!INVERT_E3_DIR); break; \
        case 4: E4_DIR_WRITE(!INVERT_E4_DIR); break; \
      } }while(0)
    #define    _REV_E_DIR(E)   do{ switch (E) { \
        case 0: E0_DIR_WRITE( INVERT_E0_DIR); break; case 1: E1_DIR_WRITE( INVERT_E1_DIR); break; \
        case 2: E2_DIR_WRITE( INVERT_E2_DIR); break; case 3: E3_DIR_WRITE( INVERT_E3_DIR); break; \
        case 4: E4_DIR_WRITE( INVERT_E4_DIR); break; \
      } }while(0)

  #elif E_STEPPERS > 3

    #define _E_STEP_WRITE(E,V) do{ switch (E) { \
        case 0: E0_STEP_WRITE(V); break; case 1: E1_STEP_WRITE(V); break; case 2: E2_STEP_WRITE(V); break; case 3: E3_STEP_WRITE(V); break; \
      } }while(0)
    #define   _NORM_E_DIR(E)   do{ switch (E) { \
        case 0: E0_DIR_WRITE(!INVERT_E0_DIR); break; case 1: E1_DIR_WRITE(!INVERT_E1_DIR); break; \
        case 2: E2_DIR_WRITE(!INVERT_E2_DIR); break; case 3: E3_DIR_WRITE(!INVERT_E3_DIR); break; \
      } }while(0)
    #define    _REV_E_DIR(E)   do{ switch (E) { \
        case 0: E0_DIR_WRITE( INVERT_E0_DIR); break; case 1: E1_DIR_WRITE( INVERT_E1_DIR); break; \
        case 2: E2_DIR_WRITE( INVERT_E2_DIR); break; case 3: E3_DIR_WRITE( INVERT_E3_DIR); break; \
      } }while(0)

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
      #if E_STEPPERS > 7
        #define DUPE(T,V)     do{ _DUPE(0,T,V); _DUPE(1,T,V); _DUPE(2,T,V); _DUPE(3,T,V); _DUPE(4,T,V); _DUPE(5,T,V); _DUPE(6,T,V); _DUPE(7,T,V); }while(0)
        #define NORM_E_DIR(E) do{ if (extruder_duplication_enabled) { NDIR(0); NDIR(1); NDIR(2); NDIR(3); NDIR(4); NDIR(5); NDIR(6); NDIR(7); } else _NORM_E_DIR(E); }while(0)
        #define REV_E_DIR(E)  do{ if (extruder_duplication_enabled) { RDIR(0); RDIR(1); RDIR(2); RDIR(3); RDIR(4); RDIR(5); RDIR(6); RDIR(7); } else  _REV_E_DIR(E); }while(0)
      #elif E_STEPPERS > 6
        #define DUPE(T,V)     do{ _DUPE(0,T,V); _DUPE(1,T,V); _DUPE(2,T,V); _DUPE(3,T,V); _DUPE(4,T,V); _DUPE(5,T,V); _DUPE(6,T,V); }while(0)
        #define NORM_E_DIR(E) do{ if (extruder_duplication_enabled) { NDIR(0); NDIR(1); NDIR(2); NDIR(3); NDIR(4); NDIR(5); NDIR(6); } else _NORM_E_DIR(E); }while(0)
        #define REV_E_DIR(E)  do{ if (extruder_duplication_enabled) { RDIR(0); RDIR(1); RDIR(2); RDIR(3); RDIR(4); RDIR(5); RDIR(6); } else  _REV_E_DIR(E); }while(0)
      #elif E_STEPPERS > 5
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

//
// X, Y, Z Stepper enable / disable
//
#if AXIS_DRIVER_TYPE_X(L6470)
  extern L6470 stepperX;
  #define X_enable()  NOOP
  #define X_disable() stepperX.free()
#elif HAS_X_ENABLE
  #define X_enable()  X_ENABLE_WRITE( X_ENABLE_ON)
  #define X_disable() X_ENABLE_WRITE(!X_ENABLE_ON)
#else
  #define X_enable()  NOOP
  #define X_disable() NOOP
#endif

#if AXIS_DRIVER_TYPE_X2(L6470)
  extern L6470 stepperX2;
  #define X2_enable()  NOOP
  #define X2_disable() stepperX2.free()
#elif HAS_X2_ENABLE
  #define X2_enable()  X2_ENABLE_WRITE( X_ENABLE_ON)
  #define X2_disable() X2_ENABLE_WRITE(!X_ENABLE_ON)
#else
  #define X2_enable()  NOOP
  #define X2_disable() NOOP
#endif

#define  enable_X() do{ X_enable(); X2_enable(); }while(0)
#define disable_X() do{ X_disable(); X2_disable(); CBI(axis_known_position, X_AXIS); }while(0)

#if AXIS_DRIVER_TYPE_Y(L6470)
  extern L6470 stepperY;
  #define Y_enable()  NOOP
  #define Y_disable() stepperY.free()
#elif HAS_Y_ENABLE
  #define Y_enable()  Y_ENABLE_WRITE( Y_ENABLE_ON)
  #define Y_disable() Y_ENABLE_WRITE(!Y_ENABLE_ON)
#else
  #define Y_enable()  NOOP
  #define Y_disable() NOOP
#endif

#if AXIS_DRIVER_TYPE_Y2(L6470)
  extern L6470 stepperY2;
  #define Y2_enable()  NOOP
  #define Y2_disable() stepperY2.free()
#elif HAS_Y2_ENABLE
  #define Y2_enable()  Y2_ENABLE_WRITE( Y_ENABLE_ON)
  #define Y2_disable() Y2_ENABLE_WRITE(!Y_ENABLE_ON)
#else
  #define Y2_enable()  NOOP
  #define Y2_disable() NOOP
#endif

#define  enable_Y() do{ Y_enable(); Y2_enable(); }while(0)
#define disable_Y() do{ Y_disable(); Y2_disable(); CBI(axis_known_position, Y_AXIS); }while(0)

#if AXIS_DRIVER_TYPE_Z(L6470)
  extern L6470 stepperZ;
  #define Z_enable()  NOOP
  #define Z_disable() stepperZ.free()
#elif HAS_Z_ENABLE
  #define Z_enable()  Z_ENABLE_WRITE( Z_ENABLE_ON)
  #define Z_disable() Z_ENABLE_WRITE(!Z_ENABLE_ON)
#else
  #define Z_enable()  NOOP
  #define Z_disable() NOOP
#endif

#if AXIS_DRIVER_TYPE_Z2(L6470)
  extern L6470 stepperZ2;
  #define Z2_enable()  NOOP
  #define Z2_disable() stepperZ2.free()
#elif HAS_Z2_ENABLE
  #define Z2_enable()  Z2_ENABLE_WRITE( Z_ENABLE_ON)
  #define Z2_disable() Z2_ENABLE_WRITE(!Z_ENABLE_ON)
#else
  #define Z2_enable()  NOOP
  #define Z2_disable() NOOP
#endif

#if AXIS_DRIVER_TYPE_Z3(L6470)
  extern L6470 stepperZ3;
  #define Z3_enable()  NOOP
  #define Z3_disable() stepperZ3.free()
#elif HAS_Z3_ENABLE
  #define Z3_enable()  Z3_ENABLE_WRITE( Z_ENABLE_ON)
  #define Z3_disable() Z3_ENABLE_WRITE(!Z_ENABLE_ON)
#else
  #define Z3_enable()  NOOP
  #define Z3_disable() NOOP
#endif

#if AXIS_DRIVER_TYPE_Z4(L6470)
  extern L6470 stepperZ4;
  #define Z4_enable()  NOOP
  #define Z4_disable() stepperZ4.free()
#elif HAS_Z4_ENABLE
  #define Z4_enable()  Z4_ENABLE_WRITE( Z_ENABLE_ON)
  #define Z4_disable() Z4_ENABLE_WRITE(!Z_ENABLE_ON)
#else
  #define Z4_enable()  NOOP
  #define Z4_disable() NOOP
#endif

#define  enable_Z() do{ Z_enable();  Z2_enable();  Z3_enable();  Z4_enable(); }while(0)
#define disable_Z() do{ Z_disable(); Z2_disable(); Z3_disable(); Z4_disable(); CBI(axis_known_position, Z_AXIS); }while(0)

//
// Extruder Stepper enable / disable
//

// define the individual enables/disables
#if AXIS_DRIVER_TYPE_E0(L6470)
  extern L6470 stepperE0;
  #define  E0_enable() NOOP
  #define E0_disable() do{ stepperE0.free(); CBI(axis_known_position, E_AXIS); }while(0)
#elif HAS_E0_ENABLE
  #define  E0_enable() E0_ENABLE_WRITE( E_ENABLE_ON)
  #define E0_disable() E0_ENABLE_WRITE(!E_ENABLE_ON)
#else
  #define  E0_enable() NOOP
  #define E0_disable() NOOP
#endif

#if AXIS_DRIVER_TYPE_E1(L6470)
  extern L6470 stepperE1;
  #define  E1_enable() NOOP
  #define E1_disable() do{ stepperE1.free(); CBI(axis_known_position, E_AXIS); }while(0)
#elif E_STEPPERS > 1 && HAS_E1_ENABLE
  #define  E1_enable() E1_ENABLE_WRITE( E_ENABLE_ON)
  #define E1_disable() E1_ENABLE_WRITE(!E_ENABLE_ON)
#else
  #define  E1_enable() NOOP
  #define E1_disable() NOOP
#endif

#if AXIS_DRIVER_TYPE_E2(L6470)
  extern L6470 stepperE2;
  #define  E2_enable() NOOP
  #define E2_disable() do{ stepperE2.free(); CBI(axis_known_position, E_AXIS); }while(0)
#elif E_STEPPERS > 2 && HAS_E2_ENABLE
  #define  E2_enable() E2_ENABLE_WRITE( E_ENABLE_ON)
  #define E2_disable() E2_ENABLE_WRITE(!E_ENABLE_ON)
#else
  #define  E2_enable() NOOP
  #define E2_disable() NOOP
#endif

#if AXIS_DRIVER_TYPE_E3(L6470)
  extern L6470 stepperE3;
  #define  E3_enable() NOOP
  #define E3_disable() do{ stepperE3.free(); CBI(axis_known_position, E_AXIS); }while(0)
#elif E_STEPPERS > 3 && HAS_E3_ENABLE
  #define  E3_enable() E3_ENABLE_WRITE( E_ENABLE_ON)
  #define E3_disable() E3_ENABLE_WRITE(!E_ENABLE_ON)
#else
  #define  E3_enable() NOOP
  #define E3_disable() NOOP
#endif

#if AXIS_DRIVER_TYPE_E4(L6470)
  extern L6470 stepperE4;
  #define  E4_enable() NOOP
  #define E4_disable() do{ stepperE4.free(); CBI(axis_known_position, E_AXIS); }while(0)
#elif E_STEPPERS > 4 && HAS_E4_ENABLE
  #define  E4_enable() E4_ENABLE_WRITE( E_ENABLE_ON)
  #define E4_disable() E4_ENABLE_WRITE(!E_ENABLE_ON)
#else
  #define  E4_enable() NOOP
  #define E4_disable() NOOP
#endif

#if AXIS_DRIVER_TYPE_E5(L6470)
  extern L6470 stepperE5;
  #define  E5_enable() NOOP
  #define E5_disable() do{ stepperE5.free(); CBI(axis_known_position, E_AXIS); }while(0)
#elif E_STEPPERS > 5 && HAS_E5_ENABLE
  #define  E5_enable() E5_ENABLE_WRITE( E_ENABLE_ON)
  #define E5_disable() E5_ENABLE_WRITE(!E_ENABLE_ON)
#else
  #define  E5_enable() NOOP
  #define E5_disable() NOOP
#endif

#if AXIS_DRIVER_TYPE_E6(L6470)
  extern L6470 stepperE6;
  #define  E6_enable() NOOP
  #define E6_disable() do{ stepperE6.free(); CBI(axis_known_position, E_AXIS); }while(0)
#elif E_STEPPERS > 6 && HAS_E6_ENABLE
  #define  E6_enable() E6_ENABLE_WRITE( E_ENABLE_ON)
  #define E6_disable() E6_ENABLE_WRITE(!E_ENABLE_ON)
#else
  #define  E6_enable() NOOP
  #define E6_disable() NOOP
#endif

#if AXIS_DRIVER_TYPE_E7(L6470)
  extern L6470 stepperE7;
  #define  E7_enable() NOOP
  #define E7_disable() do{ stepperE7.free(); CBI(axis_known_position, E_AXIS); }while(0)
#elif E_STEPPERS > 7 && HAS_E7_ENABLE
  #define  E7_enable() E7_ENABLE_WRITE( E_ENABLE_ON)
  #define E7_disable() E7_ENABLE_WRITE(!E_ENABLE_ON)
#else
  #define  E7_enable() NOOP
  #define E7_disable() NOOP
#endif

#if ENABLED(MIXING_EXTRUDER)

  /**
   * Mixing steppers synchronize their enable (and direction) together
   */
  #if MIXING_STEPPERS > 7
    #define  enable_E0() { E0_enable();  E1_enable();  E2_enable();  E3_enable();  E4_enable();  E5_enable();  E6_enable();  E7_enable(); }
    #define disable_E0() { E0_disable(); E1_disable(); E2_disable(); E3_disable(); E4_disable(); E5_disable(); E6_disable(); E7_disable(); }
  #elif MIXING_STEPPERS > 6
    #define  enable_E0() { E0_enable();  E1_enable();  E2_enable();  E3_enable();  E4_enable();  E5_enable();  E6_enable(); }
    #define disable_E0() { E0_disable(); E1_disable(); E2_disable(); E3_disable(); E4_disable(); E5_disable(); E6_disable(); }
  #elif MIXING_STEPPERS > 5
    #define  enable_E0() { E0_enable();  E1_enable();  E2_enable();  E3_enable();  E4_enable();  E5_enable(); }
    #define disable_E0() { E0_disable(); E1_disable(); E2_disable(); E3_disable(); E4_disable(); E5_disable(); }
  #elif MIXING_STEPPERS > 4
    #define  enable_E0() { E0_enable();  E1_enable();  E2_enable();  E3_enable();  E4_enable(); }
    #define disable_E0() { E0_disable(); E1_disable(); E2_disable(); E3_disable(); E4_disable(); }
  #elif MIXING_STEPPERS > 3
    #define  enable_E0() { E0_enable();  E1_enable();  E2_enable();  E3_enable(); }
    #define disable_E0() { E0_disable(); E1_disable(); E2_disable(); E3_disable(); }
  #elif MIXING_STEPPERS > 2
    #define  enable_E0() { E0_enable();  E1_enable();  E2_enable(); }
    #define disable_E0() { E0_disable(); E1_disable(); E2_disable(); }
  #else
    #define  enable_E0() { E0_enable();  E1_enable(); }
    #define disable_E0() { E0_disable(); E1_disable(); }
  #endif
  #define  enable_E1() NOOP
  #define disable_E1() NOOP
  #define  enable_E2() NOOP
  #define disable_E2() NOOP
  #define  enable_E3() NOOP
  #define disable_E3() NOOP
  #define  enable_E4() NOOP
  #define disable_E4() NOOP
  #define  enable_E5() NOOP
  #define disable_E5() NOOP
  #define  enable_E6() NOOP
  #define disable_E6() NOOP
  #define  enable_E7() NOOP
  #define disable_E7() NOOP

#else // !MIXING_EXTRUDER

  #if HAS_E0_ENABLE
    #define  enable_E0() E0_enable()
    #define disable_E0() E0_disable()
  #else
    #define  enable_E0() NOOP
    #define disable_E0() NOOP
  #endif

  #if E_STEPPERS > 1 && HAS_E1_ENABLE
    #define  enable_E1() E1_enable()
    #define disable_E1() E1_disable()
  #else
    #define  enable_E1() NOOP
    #define disable_E1() NOOP
  #endif

  #if E_STEPPERS > 2 && HAS_E2_ENABLE
    #define  enable_E2() E2_enable()
    #define disable_E2() E2_disable()
  #else
    #define  enable_E2() NOOP
    #define disable_E2() NOOP
  #endif

  #if E_STEPPERS > 3 && HAS_E3_ENABLE
    #define  enable_E3() E3_enable()
    #define disable_E3() E3_disable()
  #else
    #define  enable_E3() NOOP
    #define disable_E3() NOOP
  #endif

  #if E_STEPPERS > 4 && HAS_E4_ENABLE
    #define  enable_E4() E4_enable()
    #define disable_E4() E4_disable()
  #else
    #define  enable_E4() NOOP
    #define disable_E4() NOOP
  #endif

  #if E_STEPPERS > 5 && HAS_E5_ENABLE
    #define  enable_E5() E5_enable()
    #define disable_E5() E5_disable()
  #else
    #define  enable_E5() NOOP
    #define disable_E5() NOOP
  #endif

  #if E_STEPPERS > 6 && HAS_E6_ENABLE
    #define  enable_E6() E6_enable()
    #define disable_E6() E6_disable()
  #else
    #define  enable_E6() NOOP
    #define disable_E6() NOOP
  #endif

  #if E_STEPPERS > 7 && HAS_E7_ENABLE
    #define  enable_E7() E7_enable()
    #define disable_E7() E7_disable()
  #else
    #define  enable_E7() NOOP
    #define disable_E7() NOOP
  #endif

#endif // !MIXING_EXTRUDER
