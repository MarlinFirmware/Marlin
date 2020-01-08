/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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


#include "../../../inc/MarlinConfig.h"
#include "../../../core/language.h"
#include "../../module/planner.h"
#include "../../gcode.h"
#include "../../../module/motion.h"

/**
 * G61:  Apply/restore saved coordinates.
 *        X Y Z E - Value to add at stored coordinates.
 *        F<speed> - Set Feedrate.
 *        S<slot> specifies memory slot # (0-based) to save into (default 0).
 */
void GcodeSuite::G61(void) {

  if (!isPosSaved) return;

  const uint8_t slot = parser.byteval('S');

  if (slot >= NUM_POSITON_SLOTS) {
    SERIAL_ERROR_START();
    SERIAL_ECHOLNPAIR_F(MSG_INVALID_POS_SLOT, NUM_POSITON_SLOTS);
    return;
  }

  SERIAL_ECHOPGM(MSG_RESTORING_POS);
  SERIAL_ECHOPAIR_F(" S", int(slot));
  SERIAL_ECHOPGM("->");

  if (parser.seen('F') && parser.value_linear_units() > 0.0)
    feedrate_mm_s = MMM_TO_MMS(parser.value_linear_units());

  LOOP_XYZE(i) {
    if (parser.seen(axis_codes[i])) {
      destination[i] = parser.value_axis_units((AxisEnum)i) + stored_position[slot][i];
    }
    else {
      destination[i] = stored_position[slot][i]; //current_position[i];
    }
    SERIAL_ECHOPAIR_F(" ", axis_codes[i]);
    SERIAL_ECHOPAIR_F(":", destination[i]);
  }
  SERIAL_EOL();

  // finish moves
  prepare_move_to_destination();
  planner.synchronize();
} 