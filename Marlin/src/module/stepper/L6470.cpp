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
 * stepper/L6470.cpp
 * Stepper driver indirection for L6470 drivers
 */

#include "../../inc/MarlinConfig.h"

#if HAS_DRIVER(L6470)

#include "L6470.h"

#define _L6470_DEFINE(ST) L6470 stepper##ST((const int)L6470_CHAIN_SS_PIN)

// L6470 Stepper objects
#if AXIS_DRIVER_TYPE_X(L6470)
  _L6470_DEFINE(X);
#endif
#if AXIS_DRIVER_TYPE_X2(L6470)
  _L6470_DEFINE(X2);
#endif
#if AXIS_DRIVER_TYPE_Y(L6470)
  _L6470_DEFINE(Y);
#endif
#if AXIS_DRIVER_TYPE_Y2(L6470)
  _L6470_DEFINE(Y2);
#endif
#if AXIS_DRIVER_TYPE_Z(L6470)
  _L6470_DEFINE(Z);
#endif
#if AXIS_DRIVER_TYPE_Z2(L6470)
  _L6470_DEFINE(Z2);
#endif
#if AXIS_DRIVER_TYPE_Z3(L6470)
  _L6470_DEFINE(Z3);
#endif
#if AXIS_DRIVER_TYPE_E0(L6470)
  _L6470_DEFINE(E0);
#endif
#if AXIS_DRIVER_TYPE_E1(L6470)
  _L6470_DEFINE(E1);
#endif
#if AXIS_DRIVER_TYPE_E2(L6470)
  _L6470_DEFINE(E2);
#endif
#if AXIS_DRIVER_TYPE_E3(L6470)
  _L6470_DEFINE(E3);
#endif
#if AXIS_DRIVER_TYPE_E4(L6470)
  _L6470_DEFINE(E4);
#endif
#if AXIS_DRIVER_TYPE_E5(L6470)
  _L6470_DEFINE(E5);
#endif

// not using L6470 library's init command because it
// briefly sends power to the steppers

#define _L6470_INIT_CHIP(Q) do{                             \
  stepper##Q.resetDev();                                    \
  stepper##Q.softFree();                                    \
  stepper##Q.SetParam(L6470_CONFIG, CONFIG_PWM_DIV_1        \
                                  | CONFIG_PWM_MUL_2        \
                                  | CONFIG_SR_290V_us       \
                                  | CONFIG_OC_SD_DISABLE    \
                                  | CONFIG_VS_COMP_DISABLE  \
                                  | CONFIG_SW_HARD_STOP     \
                                  | CONFIG_INT_16MHZ);      \
  stepper##Q.SetParam(L6470_KVAL_RUN, 0xFF);                \
  stepper##Q.SetParam(L6470_KVAL_ACC, 0xFF);                \
  stepper##Q.SetParam(L6470_KVAL_DEC, 0xFF);                \
  stepper##Q.setMicroSteps(Q##_MICROSTEPS);                 \
  stepper##Q.setOverCurrent(Q##_OVERCURRENT);               \
  stepper##Q.setStallCurrent(Q##_STALLCURRENT);             \
  stepper##Q.SetParam(L6470_KVAL_HOLD, Q##_MAX_VOLTAGE);    \
  stepper##Q.SetParam(L6470_ABS_POS, 0);                    \
  stepper##Q.getStatus();                                   \
}while(0)

void L6470_Marlin::init_to_defaults() {
  #if AXIS_DRIVER_TYPE_X(L6470)
    _L6470_INIT_CHIP(X);
  #endif
  #if AXIS_DRIVER_TYPE_X2(L6470)
    _L6470_INIT_CHIP(X2);
  #endif
  #if AXIS_DRIVER_TYPE_Y(L6470)
    _L6470_INIT_CHIP(Y);
  #endif
  #if AXIS_DRIVER_TYPE_Y2(L6470)
    _L6470_INIT_CHIP(Y2);
  #endif
  #if AXIS_DRIVER_TYPE_Z(L6470)
    _L6470_INIT_CHIP(Z);
  #endif
  #if AXIS_DRIVER_TYPE_Z2(L6470)
    _L6470_INIT_CHIP(Z2);
  #endif
  #if AXIS_DRIVER_TYPE_Z3(L6470)
    _L6470_INIT_CHIP(Z3);
  #endif
  #if AXIS_DRIVER_TYPE_E0(L6470)
    _L6470_INIT_CHIP(E0);
  #endif
  #if AXIS_DRIVER_TYPE_E1(L6470)
    _L6470_INIT_CHIP(E1);
  #endif
  #if AXIS_DRIVER_TYPE_E2(L6470)
    _L6470_INIT_CHIP(E2);
  #endif
  #if AXIS_DRIVER_TYPE_E3(L6470)
    _L6470_INIT_CHIP(E3);
  #endif
  #if AXIS_DRIVER_TYPE_E4(L6470)
    _L6470_INIT_CHIP(E4);
  #endif
  #if AXIS_DRIVER_TYPE_E5(L6470)
    _L6470_INIT_CHIP(E5);
  #endif
}

#endif // L6470
