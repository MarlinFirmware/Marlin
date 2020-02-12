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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "../gcode.h"
#include "../../module/motion.h"

/**
 * M220: Set speed percentage factor, aka "Feed Rate"
 *
 * Parameters
 *   S<percent> : Set the feed rate percentage factor
 *
 * With PRUSA_MMU2...
 *   B : Flag to back up the current factor
 *   R : Flag to restore the last-saved factor
 */
void GcodeSuite::M220() {

  #if ENABLED(PRUSA_MMU2)
    static int16_t backup_feedrate_percentage = 100;
    if (parser.seen('B')) backup_feedrate_percentage = feedrate_percentage;
    if (parser.seen('R')) feedrate_percentage = backup_feedrate_percentage;
  #endif

  if (parser.seenval('S')) feedrate_percentage = parser.value_int();

}
