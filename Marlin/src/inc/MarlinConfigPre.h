/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#ifndef __MARLIN_FIRMWARE__
#define __MARLIN_FIRMWARE__
#endif

//
// Prefix header to acquire configurations
//
#include <stdint.h>

#ifndef __MARLIN_DEPS__
  #include "../HAL/platforms.h"
#endif

#include "../core/macros.h"
#include "../core/boards.h"
#include "../../Configuration.h"

#define _PROBE_FIVE_PIN_PROBE_PORT          1
#define _PROBE_Z_MIN_PORT                   2
#define _PROBE_ID(V) _CAT(_PROBE_, V)
#define PROBE_CONNECTION_IS(V) (_PROBE_ID(PROBE_IS_PLUGGED_INTO) == _PROBE_ID(V))

#if (defined(PROBE_IS_PLUGGED_INTO) && !PROBE_CONNECTION_IS(Z_MIN_PORT) && !PROBE_CONNECTION_IS(FIVE_PIN_PROBE_PORT))
  #error "PROBE_IS_PLUGGED_INTO must be set to either Z_MIN_PORT or FIVE_PIN_PROBE_PORT"
#endif

#if PROBE_CONNECTION_IS(Z_MIN_PORT)
  #define PROBE_IS_ON_Z_MIN
#endif

#ifdef CUSTOM_VERSION_FILE
  #if __has_include(STRINGIFY(../../CUSTOM_VERSION_FILE))
    #include STRINGIFY(../../CUSTOM_VERSION_FILE)
  #endif
#endif

#include "Version.h"

#include "Conditionals_LCD.h"

#ifndef __MARLIN_DEPS__
  #include HAL_PATH(../HAL, inc/Conditionals_LCD.h)
#endif

#include "../core/drivers.h"
#include "../../Configuration_adv.h"

#include "Conditionals_adv.h"

#ifndef __MARLIN_DEPS__
  #include HAL_PATH(../HAL, inc/Conditionals_adv.h)
#endif
