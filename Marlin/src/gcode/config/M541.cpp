/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2022 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * @file M541.cpp
 * @author DerAndere
 * @brief M541: Set whether print should abort on software endstop hit
 */

#include "../../inc/MarlinConfig.h"

#if ENABLED(ABORT_ON_SOFTWARE_ENDSTOP)

#include "../gcode.h"
#include "../../module/planner.h"

/**
 * M541: Set whether print should abort on software endstop hit (M541 S<0|1>)
 */
void GcodeSuite::M541() {

  if (parser.seen('S'))
    planner.abort_on_software_endstop = parser.value_bool();

}

#endif // ABORT_ON_ENDSTOP
