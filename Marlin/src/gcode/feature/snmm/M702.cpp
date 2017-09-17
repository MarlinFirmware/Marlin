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

#include "../../../inc/MarlinConfig.h"

#if ENABLED(MK2_MULTIPLEXER)

#include "../../gcode.h"
#include "../../../feature/snmm.h"

/**
 * M702: Unload all extruders
 */
void GcodeSuite::M702() {
  for (uint8_t s = 0; s < E_STEPPERS; s++) {
    select_multiplexed_stepper(s);
    // TODO: standard unload filament function
    // MK2 firmware behavior:
    //  - Make sure temperature is high enough
    //  - Raise Z to at least 15 to make room
    //  - Extrude 1cm of filament in 1 second
    //  - Under 230C quickly purge ~12mm, over 230C purge ~10mm
    //  - Change E max feedrate to 80, eject the filament from the tube. Sync.
    //  - Restore E max feedrate to 50
  }
  // Go back to the last active extruder
  select_multiplexed_stepper(active_extruder);
  disable_e_steppers();
}

#endif // MK2_MULTIPLEXER
