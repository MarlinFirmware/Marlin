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

#if DISABLED(EMERGENCY_PARSER)

#include "../gcode.h"
#include "../../Marlin.h" // for wait_for_heatup, kill, quickstop_stepper

/**
 * M108: Stop the waiting for heaters in M109, M190, M303. Does not affect the target temperature.
 */
void GcodeSuite::M108() {
  #if HAS_RESUME_CONTINUE
    wait_for_user = false;
  #endif
  wait_for_heatup = false;
}

/**
 * M112: Emergency Stop
 */
void GcodeSuite::M112() {
  kill();
}

/**
 * M410: Quickstop - Abort all planned moves
 *
 * This will stop the carriages mid-move, so most likely they
 * will be out of sync with the stepper position after this.
 */
void GcodeSuite::M410() {
  quickstop_stepper();
}

#endif // !EMERGENCY_PARSER
