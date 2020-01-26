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

/**
 * stepper/TMC26X.cpp
 * Stepper driver indirection for TMC26X drivers
 */

#include "../../inc/MarlinConfig.h"

//
// TMC26X Driver objects and inits
//
#if HAS_DRIVER(TMC26X)

#include "TMC26X.h"

#define _TMC26X_DEFINE(ST) TMC26XStepper stepper##ST(200, ST##_CS_PIN, ST##_STEP_PIN, ST##_DIR_PIN, ST##_MAX_CURRENT, ST##_SENSE_RESISTOR)

#if AXIS_DRIVER_TYPE_X(TMC26X)
  _TMC26X_DEFINE(X);
#endif
#if AXIS_DRIVER_TYPE_X2(TMC26X)
  _TMC26X_DEFINE(X2);
#endif
#if AXIS_DRIVER_TYPE_Y(TMC26X)
  _TMC26X_DEFINE(Y);
#endif
#if AXIS_DRIVER_TYPE_Y2(TMC26X)
  _TMC26X_DEFINE(Y2);
#endif
#if AXIS_DRIVER_TYPE_Z(TMC26X)
  _TMC26X_DEFINE(Z);
#endif
#if AXIS_DRIVER_TYPE_Z2(TMC26X)
  _TMC26X_DEFINE(Z2);
#endif
#if AXIS_DRIVER_TYPE_Z3(TMC26X)
  _TMC26X_DEFINE(Z3);
#endif
#if AXIS_DRIVER_TYPE_Z4(TMC26X)
  _TMC26X_DEFINE(Z4);
#endif
#if AXIS_DRIVER_TYPE_E0(TMC26X)
  _TMC26X_DEFINE(E0);
#endif
#if AXIS_DRIVER_TYPE_E1(TMC26X)
  _TMC26X_DEFINE(E1);
#endif
#if AXIS_DRIVER_TYPE_E2(TMC26X)
  _TMC26X_DEFINE(E2);
#endif
#if AXIS_DRIVER_TYPE_E3(TMC26X)
  _TMC26X_DEFINE(E3);
#endif
#if AXIS_DRIVER_TYPE_E4(TMC26X)
  _TMC26X_DEFINE(E4);
#endif
#if AXIS_DRIVER_TYPE_E5(TMC26X)
  _TMC26X_DEFINE(E5);
#endif
#if AXIS_DRIVER_TYPE_E6(TMC26X)
  _TMC26X_DEFINE(E6);
#endif
#if AXIS_DRIVER_TYPE_E7(TMC26X)
  _TMC26X_DEFINE(E7);
#endif

#define _TMC26X_INIT(A) do{ \
  stepper##A.setMicrosteps(A##_MICROSTEPS); \
  stepper##A.start(); \
}while(0)

void tmc26x_init_to_defaults() {
  #if AXIS_DRIVER_TYPE_X(TMC26X)
    _TMC26X_INIT(X);
  #endif
  #if AXIS_DRIVER_TYPE_X2(TMC26X)
    _TMC26X_INIT(X2);
  #endif
  #if AXIS_DRIVER_TYPE_Y(TMC26X)
    _TMC26X_INIT(Y);
  #endif
  #if AXIS_DRIVER_TYPE_Y2(TMC26X)
    _TMC26X_INIT(Y2);
  #endif
  #if AXIS_DRIVER_TYPE_Z(TMC26X)
    _TMC26X_INIT(Z);
  #endif
  #if AXIS_DRIVER_TYPE_Z2(TMC26X)
    _TMC26X_INIT(Z2);
  #endif
  #if AXIS_DRIVER_TYPE_Z3(TMC26X)
    _TMC26X_INIT(Z3);
  #endif
  #if AXIS_DRIVER_TYPE_Z4(TMC26X)
    _TMC26X_INIT(Z4);
  #endif
  #if AXIS_DRIVER_TYPE_E0(TMC26X)
    _TMC26X_INIT(E0);
  #endif
  #if AXIS_DRIVER_TYPE_E1(TMC26X)
    _TMC26X_INIT(E1);
  #endif
  #if AXIS_DRIVER_TYPE_E2(TMC26X)
    _TMC26X_INIT(E2);
  #endif
  #if AXIS_DRIVER_TYPE_E3(TMC26X)
    _TMC26X_INIT(E3);
  #endif
  #if AXIS_DRIVER_TYPE_E4(TMC26X)
    _TMC26X_INIT(E4);
  #endif
  #if AXIS_DRIVER_TYPE_E5(TMC26X)
    _TMC26X_INIT(E5);
  #endif
  #if AXIS_DRIVER_TYPE_E6(TMC26X)
    _TMC26X_INIT(E6);
  #endif
  #if AXIS_DRIVER_TYPE_E7(TMC26X)
    _TMC26X_INIT(E7);
  #endif
}

#endif // TMC26X
