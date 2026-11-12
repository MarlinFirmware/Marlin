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
 * MarlinConfigPre.h
 *
 * Prefix header to acquire Configurations. Includes the following:
 *
 * Conditionals-1-axes.h
 *   MarlinConfigPre-1-axes.h
 *     Config.h
 *     macros.h
 *     boards.h
 *     Configuration.h (if not Config.h)
 *     HAL/platforms.h
 *     Version.h
 * Conditionals-2-LCD.h
 * Conditionals-3-etc.h
 * Conditionals-4-adv.h
 *   MarlinConfigPre-4-adv.h
 *     Conditionals-3-etc.h (as above)
 *     drivers.h
 *     Configuration_adv.h (if not Config.h)
 */

#include "Conditionals-1-axes.h"
#include "Conditionals-2-LCD.h"
#include "Conditionals-3-etc.h"
#include "Conditionals-4-adv.h"

#ifndef __MARLIN_DEPS__
  #include HAL_PATH(.., inc/Conditionals_adv.h)
#endif
