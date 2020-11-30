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

#include "../../inc/MarlinConfig.h"

#if ENABLED(SD_ABORT_ON_ENDSTOP_HIT)

#include "../gcode.h"
#include "../../module/stepper.h"

/**
 * M540: Set whether SD card print should abort on endstop hit (M540 S<0|1>)
 */
void GcodeSuite::M540() {

  if (parser.seen('S'))
    planner.abort_on_endstop_hit = parser.value_bool();

}

#endif // SD_ABORT_ON_ENDSTOP_HIT
