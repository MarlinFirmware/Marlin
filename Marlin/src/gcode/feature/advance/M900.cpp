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

#if ENABLED(LIN_ADVANCE)

#include "../../gcode.h"
#include "../../../module/planner.h"
#include "../../../module/stepper.h"

/**
 * M900: Get or Set Linear Advance K-factor
 *
 *  K<factor>   Set advance K factor
 */
void GcodeSuite::M900() {

  #if EXTRUDERS < 2
    constexpr uint8_t tmp_extruder = 0;
  #else
    const uint8_t tmp_extruder = parser.seenval('T') ? parser.value_int() : active_extruder;
    if (tmp_extruder >= EXTRUDERS) {
      SERIAL_ECHOLNPGM("?T value out of range.");
      return;
    }
  #endif

  if (parser.seenval('K')) {
    const float newK = parser.floatval('K');
    if (WITHIN(newK, 0, 10)) {
      planner.synchronize();
      planner.extruder_advance_K[tmp_extruder] = newK;
    }
    else
      SERIAL_ECHOLNPGM("?K value out of range (0-10).");
  }
  else {
    SERIAL_ECHO_START();
    #if EXTRUDERS < 2
      SERIAL_ECHOLNPAIR("Advance K=", planner.extruder_advance_K[0]);
    #else
      SERIAL_ECHOPGM("Advance K");
      LOOP_L_N(i, EXTRUDERS) {
        SERIAL_CHAR(' '); SERIAL_ECHO(int(i));
        SERIAL_CHAR('='); SERIAL_ECHO(planner.extruder_advance_K[i]);
      }
      SERIAL_EOL();
    #endif
  }
}

#endif // LIN_ADVANCE
