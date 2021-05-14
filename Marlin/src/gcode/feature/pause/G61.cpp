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

#include "../../../inc/MarlinConfig.h"

#if SAVED_POSITIONS

#include "../../../module/planner.h"
#include "../../gcode.h"
#include "../../../module/motion.h"
#include "../../../module/planner.h"

#define DEBUG_OUT ENABLED(SAVED_POSITIONS_DEBUG)
#include "../../../core/debug_out.h"

/**
 * G61: Return to saved position
 *
 *   F<rate>  - Feedrate (optional) for the move back.
 *   S<slot>  - Slot # (0-based) to restore from (default 0).
 *   X Y Z    - Axes to restore. At least one is required.
 *   E - Restore extruder position
 *
 *   If XYZE are not given, default restore uses the smart blocking move.
 */
void GcodeSuite::G61(void) {

  const uint8_t slot = parser.byteval('S');

  #define SYNC_E(POINT) planner.set_e_position_mm((destination.e = current_position.e = (POINT)))

  #if SAVED_POSITIONS < 256
    if (slot >= SAVED_POSITIONS) {
      SERIAL_ERROR_MSG(STR_INVALID_POS_SLOT STRINGIFY(SAVED_POSITIONS));
      return;
    }
  #endif

  // No saved position? No axes being restored?
  if (!TEST(saved_slots[slot >> 3], slot & 0x07)) return;

  // Apply any given feedrate over 0.0
  feedRate_t saved_feedrate = feedrate_mm_s;
  const float fr = parser.linearval('F');
  if (fr > 0.0) feedrate_mm_s = MMM_TO_MMS(fr);

  if (!parser.seen_axis()) {
    DEBUG_ECHOLNPGM("Default position restore");
    do_blocking_move_to(stored_position[slot], feedrate_mm_s);
    SYNC_E(stored_position[slot].e);
  }
  else {
    if (parser.seen("XYZ")) {
      DEBUG_ECHOPAIR(STR_RESTORING_POS " S", slot);
      LOOP_XYZ(i) {
        destination[i] = parser.seen(XYZ_CHAR(i))
          ? stored_position[slot][i] + parser.value_axis_units((AxisEnum)i)
          : current_position[i];
        DEBUG_CHAR(' ', XYZ_CHAR(i));
        DEBUG_ECHO_F(destination[i]);
      }
      DEBUG_EOL();
      // Move to the saved position
      prepare_line_to_destination();
    }
    if (parser.seen_test('E')) {
      DEBUG_ECHOLNPAIR(STR_RESTORING_POS " S", slot, " E", current_position.e, "=>", stored_position[slot].e);
      SYNC_E(stored_position[slot].e);
    }
  }

  feedrate_mm_s = saved_feedrate;
}

#endif // SAVED_POSITIONS
