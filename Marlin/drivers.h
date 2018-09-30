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
#ifndef _DRIVERS_H_
#define _DRIVERS_H_

#include "MarlinConfig.h"

#define A4988               0x001
#define DRV8825             0x002
#define LV8729              0x003
#define L6470               0x104
#define TB6560              0x005
#define TB6600              0x006
#define TMC2100             0x007
#define TMC2130             0x108
#define TMC2130_STANDALONE  0x008
#define TMC2208             0x109
#define TMC2208_STANDALONE  0x009
#define TMC26X              0x10A
#define TMC26X_STANDALONE   0x00A
#define TMC2660             0x10B
#define TMC2660_STANDALONE  0x00B

#define _AXIS_DRIVER_TYPE(A,T) ( defined(A##_DRIVER_TYPE) && (A##_DRIVER_TYPE == T) )

#define AXIS_DRIVER_TYPE_X(T) _AXIS_DRIVER_TYPE(X,T)
#define AXIS_DRIVER_TYPE_Y(T) _AXIS_DRIVER_TYPE(Y,T)
#define AXIS_DRIVER_TYPE_Z(T) _AXIS_DRIVER_TYPE(Z,T)
#define AXIS_DRIVER_TYPE_X2(T) (ENABLED(X_DUAL_STEPPER_DRIVERS) || ENABLED(DUAL_X_CARRIAGE)) && _AXIS_DRIVER_TYPE(X2,T)
#define AXIS_DRIVER_TYPE_Y2(T) (ENABLED(Y_DUAL_STEPPER_DRIVERS) && _AXIS_DRIVER_TYPE(Y2,T))
#define AXIS_DRIVER_TYPE_Z2(T) (ENABLED(Z_DUAL_STEPPER_DRIVERS) && _AXIS_DRIVER_TYPE(Z2,T))
#define AXIS_DRIVER_TYPE_E0(T) (E_STEPPERS > 0 && _AXIS_DRIVER_TYPE(E0,T))
#define AXIS_DRIVER_TYPE_E1(T) (E_STEPPERS > 1 && _AXIS_DRIVER_TYPE(E1,T))
#define AXIS_DRIVER_TYPE_E2(T) (E_STEPPERS > 2 && _AXIS_DRIVER_TYPE(E2,T))
#define AXIS_DRIVER_TYPE_E3(T) (E_STEPPERS > 3 && _AXIS_DRIVER_TYPE(E3,T))
#define AXIS_DRIVER_TYPE_E4(T) (E_STEPPERS > 4 && _AXIS_DRIVER_TYPE(E4,T))

#define AXIS_DRIVER_TYPE(A,T) AXIS_DRIVER_TYPE_##A(T)

#define HAS_DRIVER(T)  (AXIS_DRIVER_TYPE_X(T)  || AXIS_DRIVER_TYPE_X2(T) || \
                        AXIS_DRIVER_TYPE_Y(T)  || AXIS_DRIVER_TYPE_Y2(T) || \
                        AXIS_DRIVER_TYPE_Z(T)  || AXIS_DRIVER_TYPE_Z2(T) || \
                        AXIS_DRIVER_TYPE_E0(T) || AXIS_DRIVER_TYPE_E1(T) || \
                        AXIS_DRIVER_TYPE_E2(T) || AXIS_DRIVER_TYPE_E3(T) || \
                        AXIS_DRIVER_TYPE_E4(T) )

// Test for supported TMC drivers that require advanced configuration
// Does not match standalone configurations
#define HAS_TRINAMIC (HAS_DRIVER(TMC2130) || HAS_DRIVER(TMC2208))

#define AXIS_IS_TMC(A) ( AXIS_DRIVER_TYPE_##A(TMC2130) || \
                         AXIS_DRIVER_TYPE_##A(TMC2208) )

#endif // _DRIVERS_H_
