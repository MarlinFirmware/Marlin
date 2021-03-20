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

#include "../../inc/MarlinConfig.h"

#if ENABLED(LONG_FILENAME_HOST_SUPPORT)

#include "../gcode.h"
#include "../../sd/cardreader.h"

/**
 * M33: Get the long full path of a file or folder
 *
 * Parameters:
 *   <dospath> Case-insensitive DOS-style path to a file or folder
 *
 * Example:
 *   M33 miscel~1/armchair/armcha~1.gco
 *
 * Output:
 *   /Miscellaneous/Armchair/Armchair.gcode
 */
void GcodeSuite::M33() {

  card.printLongPath(parser.string_arg);

}

#endif // LONG_FILENAME_HOST_SUPPORT
