/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * Rambo ThinkerV2 pin assignments
 */

#define BOARD_INFO_NAME "Rambo ThinkerV2"

#define SERVO0_PIN                             4  // Motor header MX1
#define SERVO2_PIN                            -1  // Motor header MX3

#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                      10
#endif

// Support BLTouch and fixed probes
#if ENABLED(BLTOUCH)
  #if ENABLED(Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN)
    #define Z_MIN_PIN                         22
  #elif !defined(Z_MIN_PROBE_PIN)
    #define Z_MIN_PROBE_PIN                   22
  #endif
#elif ENABLED(FIX_MOUNTED_PROBE)
  #if ENABLED(Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN)
    #define Z_MIN_PIN                          4
  #elif !defined(Z_MIN_PROBE_PIN)
    #define Z_MIN_PROBE_PIN                    4
  #endif
#endif

// Eryone has the fan pins reversed
#define FAN1_PIN                               2
#define FAN2_PIN                               6

// Encoder
#define BTN_EN1                               64
#define BTN_EN2                               63

#include "pins_RAMBO.h"
