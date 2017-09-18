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

#ifndef __MIXING_H__
#define __MIXING_H__

#include "../inc/MarlinConfig.h"

extern float mixing_factor[MIXING_STEPPERS]; // Reciprocal of mix proportion. 0.0 = off, otherwise >= 1.0.

#if MIXING_VIRTUAL_TOOLS > 1
  extern float mixing_virtual_tool_mix[MIXING_VIRTUAL_TOOLS][MIXING_STEPPERS];
  void mixing_tools_init();
#endif

void normalize_mix();

#if ENABLED(DIRECT_MIXING_IN_G1)
  void gcode_get_mix();
#endif

#endif // __MIXING_H__
