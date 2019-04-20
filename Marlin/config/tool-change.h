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

/**
 * Universal tool change settings.
 * Applies to all types of extruders except where explicitly noted.
 */
#if EXTRUDERS > 1
  // Z raise distance for tool-change, as needed for some extruders
  #define TOOLCHANGE_ZRAISE     2  // (mm)

  // Retract and prime filament on tool-change
  //#define TOOLCHANGE_FILAMENT_SWAP
  #if ENABLED(TOOLCHANGE_FILAMENT_SWAP)
    #define TOOLCHANGE_FIL_SWAP_LENGTH          12  // (mm)
    #define TOOLCHANGE_FIL_EXTRA_PRIME           2  // (mm)
    #define TOOLCHANGE_FIL_SWAP_RETRACT_SPEED 3600  // (mm/m)
    #define TOOLCHANGE_FIL_SWAP_PRIME_SPEED   3600  // (mm/m)
  #endif

  /**
   * Position to park head during tool change.
   * Doesn't apply to SWITCHING_TOOLHEAD, DUAL_X_CARRIAGE, or PARKING_EXTRUDER
   */
  //#define TOOLCHANGE_PARK
  #if ENABLED(TOOLCHANGE_PARK)
    #define TOOLCHANGE_PARK_XY    { X_MIN_POS + 10, Y_MIN_POS + 10 }
    #define TOOLCHANGE_PARK_XY_FEEDRATE 6000  // (mm/m)
  #endif
#endif

