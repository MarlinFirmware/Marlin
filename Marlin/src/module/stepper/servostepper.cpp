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
 * stepper/servostepper.h
 * Stepper driver indirection for a servomotor
 */

#include "../../inc/MarlinConfig.h"

#if HAS_SERVOSTEPPER

#include "../../feature/servostepper.h"

#define _SERVOSTEPPER_DEFINE(ST) ServoStepper stepper##ST(SERVO_##ST##_INDEX)

#if AXIS_DRIVER_TYPE_X(SERVOSTEPPER)
  _SERVOSTEPPER_DEFINE(X);
#endif
#if AXIS_DRIVER_TYPE_X2(SERVOSTEPPER)
  _SERVOSTEPPER_DEFINE(X2);
#endif
#if AXIS_DRIVER_TYPE_Y(SERVOSTEPPER)
  _SERVOSTEPPER_DEFINE(Y);
#endif
#if AXIS_DRIVER_TYPE_Y2(SERVOSTEPPER)
  _SERVOSTEPPER_DEFINE(Y2);
#endif
#if AXIS_DRIVER_TYPE_Z(SERVOSTEPPER)
  _SERVOSTEPPER_DEFINE(Z);
#endif
#if AXIS_DRIVER_TYPE_Z2(SERVOSTEPPER)
  _SERVOSTEPPER_DEFINE(Z2);
#endif
#if AXIS_DRIVER_TYPE_Z3(SERVOSTEPPER)
  _SERVOSTEPPER_DEFINE(Z3);
#endif
#if AXIS_DRIVER_TYPE_E0(SERVOSTEPPER)
  _SERVOSTEPPER_DEFINE(E0);
#endif
#if AXIS_DRIVER_TYPE_E1(SERVOSTEPPER)
  _SERVOSTEPPER_DEFINE(E1);
#endif
#if AXIS_DRIVER_TYPE_E2(SERVOSTEPPER)
  _SERVOSTEPPER_DEFINE(E2);
#endif
#if AXIS_DRIVER_TYPE_E3(SERVOSTEPPER)
  _SERVOSTEPPER_DEFINE(E3);
#endif
#if AXIS_DRIVER_TYPE_E4(SERVOSTEPPER)
  _SERVOSTEPPER_DEFINE(E4);
#endif
#if AXIS_DRIVER_TYPE_E5(SERVOSTEPPER)
  _SERVOSTEPPER_DEFINE(E5);
#endif

#endif // HAS_SERVOSTEPPER
