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

#include "../../inc/MarlinConfig.h"

#if ENABLED(COOLANT_ENABLE)

#include "../gcode.h"
#include "../../module/stepper.h"

#if ENABLED(COOLANT_MIST)
/**
 * M7: Mist Coolant On
 *
 */

void GcodeSuite::M7() {
  planner.synchronize();   // wait until previous movement commands (G0/G0/G2/G3) have completed before playing with the spindle

  WRITE(COOLANT_MIST_PIN, !COOLANT_MIST_INVERT);  // turn on mist coolant
}
#endif // COOLANT_MIST

#if ENABLED(COOLANT_FLOOD)
/**
 * M8: Flood Coolant On
 *
 */

void GcodeSuite::M8() {
  planner.synchronize();   // wait until previous movement commands (G0/G0/G2/G3) have completed before playing with the spindle

  WRITE(COOLANT_FLOOD_PIN, !COOLANT_FLOOD_INVERT);  // turn on flood coolant
}
#endif // COOLANT_FLOOD

/**
 * M9 turn off coolant
 */
void GcodeSuite::M9() {
  planner.synchronize();
  #if ENABLED(COOLANT_MIST)
    WRITE(COOLANT_MIST_PIN, COOLANT_MIST_INVERT);  // turn off mist coolant
  #endif
  #if ENABLED(COOLANT_FLOOD)
    WRITE(COOLANT_FLOOD_PIN, COOLANT_FLOOD_INVERT);  // turn off flood coolant
  #endif
}

#endif // COOLANT_ENABLE
