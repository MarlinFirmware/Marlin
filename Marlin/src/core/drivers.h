/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#pragma once

#include "../inc/MarlinConfigPre.h"

#define _A4988              0x001
#define _A5984              0x002
#define _DRV8825            0x003
#define _LV8729             0x004
#define _L6470              0x105
#define _L6480              0x106
#define _powerSTEP01        0x107
#define _TB6560             0x006
#define _TB6600             0x007
#define _TMC2100            0x008
#define _TMC2130            2130
#define _TMC2130_STANDALONE 0x009
#define _TMC2160            2160
#define _TMC2160_STANDALONE 2161
#define _TMC2208            2208
#define _TMC2208_STANDALONE 0x00A
#define _TMC26X             0x10B
#define _TMC26X_STANDALONE  0x00B
#define _TMC2660            2660
#define _TMC2660_STANDALONE 0x00C
#define _TMC5130            5130
#define _TMC5130_STANDALONE 5131
#define _TMC5160            5160
#define _TMC5160_STANDALONE 5161

#define _ACTUAL(V) _CAT(_, V)
#define _AXIS_DRIVER_TYPE(A,T) (defined(A##_DRIVER_TYPE) && _ACTUAL(A##_DRIVER_TYPE) == _CAT(_, T))

#define AXIS_DRIVER_TYPE_X(T) _AXIS_DRIVER_TYPE(X,T)
#define AXIS_DRIVER_TYPE_Y(T) _AXIS_DRIVER_TYPE(Y,T)
#define AXIS_DRIVER_TYPE_Z(T) _AXIS_DRIVER_TYPE(Z,T)
#define AXIS_DRIVER_TYPE_X2(T) _AXIS_DRIVER_TYPE(X2,T)
#define AXIS_DRIVER_TYPE_Y2(T) _AXIS_DRIVER_TYPE(Y2,T)
#define AXIS_DRIVER_TYPE_Z2(T) _AXIS_DRIVER_TYPE(Z2,T)
#define AXIS_DRIVER_TYPE_Z3(T) _AXIS_DRIVER_TYPE(Z3,T)
#define AXIS_DRIVER_TYPE_E0(T) (E_STEPPERS > 0 && _AXIS_DRIVER_TYPE(E0,T))
#define AXIS_DRIVER_TYPE_E1(T) (E_STEPPERS > 1 && _AXIS_DRIVER_TYPE(E1,T))
#define AXIS_DRIVER_TYPE_E2(T) (E_STEPPERS > 2 && _AXIS_DRIVER_TYPE(E2,T))
#define AXIS_DRIVER_TYPE_E3(T) (E_STEPPERS > 3 && _AXIS_DRIVER_TYPE(E3,T))
#define AXIS_DRIVER_TYPE_E4(T) (E_STEPPERS > 4 && _AXIS_DRIVER_TYPE(E4,T))
#define AXIS_DRIVER_TYPE_E5(T) (E_STEPPERS > 5 && _AXIS_DRIVER_TYPE(E5,T))

#define AXIS_DRIVER_TYPE(A,T) AXIS_DRIVER_TYPE_##A(T)

#define HAS_DRIVER(T)  (AXIS_DRIVER_TYPE_X(T)  || AXIS_DRIVER_TYPE_X2(T) || \
                        AXIS_DRIVER_TYPE_Y(T)  || AXIS_DRIVER_TYPE_Y2(T) || \
                        AXIS_DRIVER_TYPE_Z(T)  || AXIS_DRIVER_TYPE_Z2(T) || AXIS_DRIVER_TYPE_Z3(T) || \
                        AXIS_DRIVER_TYPE_E0(T) || AXIS_DRIVER_TYPE_E1(T) || \
                        AXIS_DRIVER_TYPE_E2(T) || AXIS_DRIVER_TYPE_E3(T) || \
                        AXIS_DRIVER_TYPE_E4(T) || AXIS_DRIVER_TYPE_E5(T) )

// Test for supported TMC drivers that require advanced configuration
// Does not match standalone configurations
#define HAS_TRINAMIC ( HAS_DRIVER(TMC2130) || HAS_DRIVER(TMC2160) || HAS_DRIVER(TMC2208) || HAS_DRIVER(TMC2660) || HAS_DRIVER(TMC5130) || HAS_DRIVER(TMC5160) )

#define AXIS_IS_TMC(A) ( AXIS_DRIVER_TYPE_##A(TMC2130) || \
                         AXIS_DRIVER_TYPE_##A(TMC2160) || \
                         AXIS_DRIVER_TYPE_##A(TMC2208) || \
                         AXIS_DRIVER_TYPE_##A(TMC2660) || \
                         AXIS_DRIVER_TYPE_##A(TMC5130) || \
                         AXIS_DRIVER_TYPE_##A(TMC5160))

#define HAS_L64XX (HAS_DRIVER(L6470) || HAS_DRIVER(L6480) || HAS_DRIVER(powerSTEP01))

#define AXIS_IS_L64XX(A) (AXIS_DRIVER_TYPE_##A(L6470) || AXIS_DRIVER_TYPE_##A(L6480) || AXIS_DRIVER_TYPE_##A(powerSTEP01))

#define AXIS_CLASS(A) AXIS_CLASS_##A

#if AXIS_IS_TMC(X)
  #define AXIS_CLASS_X TMC_CLASS(X)
#elif AXIS_DRIVER_TYPE_X(TMC26X)
  #define AXIS_CLASS_X TMC26XStepper
#elif AXIS_DRIVER_TYPE_X(L6470)
  #define AXIS_CLASS_X L6470
#elif AXIS_DRIVER_TYPE_X(L6480)
  #define AXIS_CLASS_X L6480
#elif AXIS_DRIVER_TYPE_X(powerSTEP01)
  #define AXIS_CLASS_X powerSTEP01
#else
  #define AXIS_CLASS_X no_class
#endif

#if AXIS_IS_TMC(Y)
  #define AXIS_CLASS_Y TMC_CLASS(Y)
#elif AXIS_DRIVER_TYPE_Y(TMC26X)
  #define AXIS_CLASS_Y TMC26XStepper
#elif AXIS_DRIVER_TYPE_Y(L6470)
  #define AXIS_CLASS_Y L6470
#elif AXIS_DRIVER_TYPE_Y(L6480)
  #define AXIS_CLASS_Y L6480
#elif AXIS_DRIVER_TYPE_Y(powerSTEP01)
  #define AXIS_CLASS_Y powerSTEP01
#else
  #define AXIS_CLASS_Y no_class
#endif

#if AXIS_IS_TMC(Z)
  #define AXIS_CLASS_Z TMC_CLASS(Z)
#elif AXIS_DRIVER_TYPE_Z(TMC26X)
  #define AXIS_CLASS_Z TMC26XStepper
#elif AXIS_DRIVER_TYPE_Z(L6470)
  #define AXIS_CLASS_Z L6470
#elif AXIS_DRIVER_TYPE_Z(L6480)
  #define AXIS_CLASS_Z L6480
#elif AXIS_DRIVER_TYPE_Z(powerSTEP01)
  #define AXIS_CLASS_Z powerSTEP01
#else
  #define AXIS_CLASS_Z no_class
#endif

#if AXIS_IS_TMC(X2)
  #define AXIS_CLASS_X2 TMC_CLASS(X2)
#elif AXIS_DRIVER_TYPE_X2(TMC26X)
  #define AXIS_CLASS_X2 TMC26XStepper
#elif AXIS_DRIVER_TYPE_X2(L6470)
  #define AXIS_CLASS_X2 L6470
#elif AXIS_DRIVER_TYPE_X2(L6480)
  #define AXIS_CLASS_X2 L6480
#elif AXIS_DRIVER_TYPE_X2(powerSTEP01)
  #define AXIS_CLASS_X2 powerSTEP01
#else
  #define AXIS_CLASS_X2 no_class
#endif

#if AXIS_IS_TMC(Y2)
  #define AXIS_CLASS_Y2 TMC_CLASS(Y2)
#elif AXIS_DRIVER_TYPE_Y2(TMC26X)
  #define AXIS_CLASS_Y2 TMC26XStepper
#elif AXIS_DRIVER_TYPE_Y2(L6470)
  #define AXIS_CLASS_Y2 L6470
#elif AXIS_DRIVER_TYPE_Y2(L6480)
  #define AXIS_CLASS_Y2 L6480
#elif AXIS_DRIVER_TYPE_Y2(powerSTEP01)
  #define AXIS_CLASS_Y2 powerSTEP01
#else
  #define AXIS_CLASS_Y2 no_class
#endif

#if AXIS_IS_TMC(Z2)
  #define AXIS_CLASS_Z2 TMC_CLASS(Z2)
#elif AXIS_DRIVER_TYPE_Z2(TMC26X)
  #define AXIS_CLASS_Z2 TMC26XStepper
#elif AXIS_DRIVER_TYPE_Z2(L6470)
  #define AXIS_CLASS_Z2 L6470
#elif AXIS_DRIVER_TYPE_Z2(L6480)
  #define AXIS_CLASS_Z2 L6480
#elif AXIS_DRIVER_TYPE_Z2(powerSTEP01)
  #define AXIS_CLASS_Z2 powerSTEP01
#else
  #define AXIS_CLASS_Z2 no_class
#endif

#if AXIS_IS_TMC(Z3)
  #define AXIS_CLASS_Z3 TMC_CLASS(Z3)
#elif AXIS_DRIVER_TYPE_Z3(TMC26X)
  #define AXIS_CLASS_Z3 TMC26XStepper
#elif AXIS_DRIVER_TYPE_Z3(L6470)
  #define AXIS_CLASS_Z3 L6470
#elif AXIS_DRIVER_TYPE_Z3(L6480)
  #define AXIS_CLASS_Z3 L6480
#elif AXIS_DRIVER_TYPE_Z3(powerSTEP01)
  #define AXIS_CLASS_Z3 powerSTEP01
#else
  #define AXIS_CLASS_Z3 no_class
#endif

#if AXIS_IS_TMC(E0)
  #define AXIS_CLASS_E0 TMC_CLASS(E0)
#elif AXIS_DRIVER_TYPE_E0(TMC26X)
  #define AXIS_CLASS_E0 TMC26XStepper
#elif AXIS_DRIVER_TYPE_E0(L6470)
  #define AXIS_CLASS_E0 L6470
#elif AXIS_DRIVER_TYPE_E0(L6480)
  #define AXIS_CLASS_E0 L6480
#elif AXIS_DRIVER_TYPE_E0(powerSTEP01)
  #define AXIS_CLASS_E0 powerSTEP01
#else
  #define AXIS_CLASS_E0 no_class
#endif

#if AXIS_IS_TMC(E1)
  #define AXIS_CLASS_E1 TMC_CLASS(E1)
#elif AXIS_DRIVER_TYPE_E1(TMC26X)
  #define AXIS_CLASS_E1 TMC26XStepper
#elif AXIS_DRIVER_TYPE_E1(L6470)
  #define AXIS_CLASS_E1 L6470
#elif AXIS_DRIVER_TYPE_E1(L6480)
  #define AXIS_CLASS_E1 L6480
#elif AXIS_DRIVER_TYPE_E1(powerSTEP01)
  #define AXIS_CLASS_E1 powerSTEP01
#else
  #define AXIS_CLASS_E1 no_class
#endif

#if AXIS_IS_TMC(E2)
  #define AXIS_CLASS_E2 TMC_CLASS(E2)
#elif AXIS_DRIVER_TYPE_E2(TMC26X)
  #define AXIS_CLASS_E2 TMC26XStepper
#elif AXIS_DRIVER_TYPE_E2(L6470)
  #define AXIS_CLASS_E2 L6470
#elif AXIS_DRIVER_TYPE_E2(L6480)
  #define AXIS_CLASS_E2 L6480
#elif AXIS_DRIVER_TYPE_E2(powerSTEP01)
  #define AXIS_CLASS_E2 powerSTEP01
#else
  #define AXIS_CLASS_E2 no_class
#endif

#if AXIS_IS_TMC(E3)
  #define AXIS_CLASS_E3 TMC_CLASS(E3)
#elif AXIS_DRIVER_TYPE_E3(TMC26X)
  #define AXIS_CLASS_E3 TMC26XStepper
#elif AXIS_DRIVER_TYPE_E3(L6470)
  #define AXIS_CLASS_E3 L6470
#elif AXIS_DRIVER_TYPE_E3(L6480)
  #define AXIS_CLASS_E3 L6480
#elif AXIS_DRIVER_TYPE_E3(powerSTEP01)
  #define AXIS_CLASS_E3 powerSTEP01
#else
  #define AXIS_CLASS_E3 no_class
#endif

#if AXIS_IS_TMC(E4)
  #define AXIS_CLASS_E4 TMC_CLASS(E4)
#elif AXIS_DRIVER_TYPE_E4(TMC26X)
  #define AXIS_CLASS_E4 TMC26XStepper
#elif AXIS_DRIVER_TYPE_E4(L6470)
  #define AXIS_CLASS_E4 L6470
#elif AXIS_DRIVER_TYPE_E4(L6480)
  #define AXIS_CLASS_E4 L6480
#elif AXIS_DRIVER_TYPE_E4(powerSTEP01)
  #define AXIS_CLASS_E4 powerSTEP01
#else
  #define AXIS_CLASS_E4 no_class
#endif

#if AXIS_IS_TMC(E5)
  #define AXIS_CLASS_E5 TMC_CLASS(E5)
#elif AXIS_DRIVER_TYPE_E5(TMC26X)
  #define AXIS_CLASS_E5 TMC26XStepper
#elif AXIS_DRIVER_TYPE_E5(L6470)
  #define AXIS_CLASS_E5 L6470
#elif AXIS_DRIVER_TYPE_E5(L6480)
  #define AXIS_CLASS_E5 L6480
#elif AXIS_DRIVER_TYPE_E5(powerSTEP01)
  #define AXIS_CLASS_E5 powerSTEP01
#else
  #define AXIS_CLASS_E5 no_class
#endif
