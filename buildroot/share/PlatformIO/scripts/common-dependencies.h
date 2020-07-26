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

/**
 * The purpose of this file is just include Marlin Configuration files,
 * to discover which FEATURES are enabled, without any HAL include.
 * Used by common-dependencies.py
 */

#include <stdint.h>

// Include platform headers
//#include "../../../../Marlin/src/HAL/platforms.h"

#include "../../../../Marlin/src/core/boards.h"
#include "../../../../Marlin/src/core/macros.h"
#include "../../../../Marlin/Configuration.h"

#include "../../../../Marlin/Version.h"

#include "../../../../Marlin/src/inc/Conditionals_LCD.h"

#ifdef HAL_PATH
  #include HAL_PATH(../../../../Marlin/src/HAL, inc/Conditionals_LCD.h)
#endif

#include "../../../../Marlin/src/core/drivers.h"
#include "../../../../Marlin/Configuration_adv.h"

#include "../../../../Marlin/src/inc/Conditionals_adv.h"

#ifdef HAL_PATH
  #include HAL_PATH(../../../../Marlin/src/HAL, inc/Conditionals_adv.h)
#endif

//#include "../../../../Marlin/src/pins/pins.h"

#ifdef HAL_PATH
  #include HAL_PATH(../../../../Marlin/src/HAL, timers.h)
  #include HAL_PATH(../../../../Marlin/src/HAL, spi_pins.h)
#endif

#include "../../../../Marlin/src/inc/Conditionals_post.h"

#ifdef HAL_PATH
  #include HAL_PATH(../../../../Marlin/src/HAL, inc/Conditionals_post.h)
#endif
