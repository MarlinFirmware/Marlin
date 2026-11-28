/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2025 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * @file G93_G94.cpp
 * @author DerAndere
 * @brief G93 (inverse time mode) and G94 (units per minute feedrate mode).
 *
 * Copyright 2025 DerAndere
 */

#include "../../inc/MarlinConfig.h"

#if ENABLED(FEEDRATE_MODE_SUPPORT)

#include "../gcode.h"

/**
 * G93: Set feedrate mode to inverse time
 */
void GcodeSuite::G93() { parser.inverse_time_enabled = true; }

/**
 * G94: Set feedrate mode to length units per minute
 */
void GcodeSuite::G94() { parser.inverse_time_enabled = false; }

#endif // FEEDRATE_MODE_SUPPORT
