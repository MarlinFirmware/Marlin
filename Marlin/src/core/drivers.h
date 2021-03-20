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
#define AXIS_DRIVER_TYPE_X2(T) (EITHER(X_DUAL_STEPPER_DRIVERS, DUAL_X_CARRIAGE) && _AXIS_DRIVER_TYPE(X2,T))
#define AXIS_DRIVER_TYPE_Y2(T) (ENABLED(Y_DUAL_STEPPER_DRIVERS) && _AXIS_DRIVER_TYPE(Y2,T))
#define AXIS_DRIVER_TYPE_Z2(T) (Z_MULTI_STEPPER_DRIVERS && _AXIS_DRIVER_TYPE(Z2,T))
#define AXIS_DRIVER_TYPE_Z3(T) (ENABLED(Z_TRIPLE_STEPPER_DRIVERS) && _AXIS_DRIVER_TYPE(Z3,T))
#define AXIS_DRIVER_TYPE_E0(T) (E_STEPPERS > 0 && _AXIS_DRIVER_TYPE(E0,T))
#define AXIS_DRIVER_TYPE_E1(T) (E_STEPPERS > 1 && _AXIS_DRIVER_TYPE(E1,T))
#define AXIS_DRIVER_TYPE_E2(T) (E_STEPPERS > 2 && _AXIS_DRIVER_TYPE(E2,T))
#define AXIS_DRIVER_TYPE_E3(T) (E_STEPPERS > 3 && _AXIS_DRIVER_TYPE(E3,T))
#define AXIS_DRIVER_TYPE_E4(T) (E_STEPPERS > 4 && _AXIS_DRIVER_TYPE(E4,T))
#define AXIS_DRIVER_TYPE_E5(T) (E_STEPPERS > 5 && _AXIS_DRIVER_TYPE(E5,T))

#define AXIS_DRIVER_TYPE(A,T) AXIS_DRIVER_TYPE_##A(T)

#define HAS_DRIVER(T) (    AXIS_DRIVER_TYPE_X(T)  || AXIS_DRIVER_TYPE_X2(T) \
                        || AXIS_DRIVER_TYPE_Y(T)  || AXIS_DRIVER_TYPE_Y2(T) \
                        || AXIS_DRIVER_TYPE_Z(T)  || AXIS_DRIVER_TYPE_Z2(T) || AXIS_DRIVER_TYPE_Z3(T) \
                        || AXIS_DRIVER_TYPE_E0(T) || AXIS_DRIVER_TYPE_E1(T) \
                        || AXIS_DRIVER_TYPE_E2(T) || AXIS_DRIVER_TYPE_E3(T) \
                        || AXIS_DRIVER_TYPE_E4(T) || AXIS_DRIVER_TYPE_E5(T) )

// Test for supported TMC drivers that require advanced configuration
// Does not match standalone configurations
#define HAS_TRINAMIC (    HAS_DRIVER(TMC2130) \
                       || HAS_DRIVER(TMC2160) \
                       || HAS_DRIVER(TMC2208) \
                       || HAS_DRIVER(TMC2660) \
                       || HAS_DRIVER(TMC5130) \
                       || HAS_DRIVER(TMC5160) )

#define AXIS_IS_TMC(A)   (    AXIS_DRIVER_TYPE(A,TMC2130) \
                           || AXIS_DRIVER_TYPE(A,TMC2160) \
                           || AXIS_DRIVER_TYPE(A,TMC2208) \
                           || AXIS_DRIVER_TYPE(A,TMC2660) \
                           || AXIS_DRIVER_TYPE(A,TMC5130) \
                           || AXIS_DRIVER_TYPE(A,TMC5160) )

// Test for a driver that uses SPI - this allows checking whether a _CS_ pin
// is considered sensitive
#define AXIS_HAS_SPI(A)  (    AXIS_DRIVER_TYPE(A,TMC2130) \
                           || AXIS_DRIVER_TYPE(A,TMC2160) \
                           || AXIS_DRIVER_TYPE(A,TMC2660) \
                           || AXIS_DRIVER_TYPE(A,TMC5130) \
                           || AXIS_DRIVER_TYPE(A,TMC5160) )

#define AXIS_HAS_STALLGUARD(A)   (    AXIS_DRIVER_TYPE(A,TMC2130) \
                                   || AXIS_DRIVER_TYPE(A,TMC2160) \
                                   || AXIS_DRIVER_TYPE(A,TMC2660) \
                                   || AXIS_DRIVER_TYPE(A,TMC5130) \
                                   || AXIS_DRIVER_TYPE(A,TMC5160) )

#define AXIS_HAS_STEALTHCHOP(A)  (    AXIS_DRIVER_TYPE(A,TMC2130) \
                                   || AXIS_DRIVER_TYPE(A,TMC2160) \
                                   || AXIS_DRIVER_TYPE(A,TMC2208) \
                                   || AXIS_DRIVER_TYPE(A,TMC5130) \
                                   || AXIS_DRIVER_TYPE(A,TMC5160) )
