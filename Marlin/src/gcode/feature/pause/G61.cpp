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
 *   F<rate>   - Feedrate (optional) for the move back.
 *   S<slot>   - Slot # (0-based) to restore from (default 0).
 *   X<offset> - Restore X axis, applying the given offset (default 0)
 *   Y<offset> - Restore Y axis, applying the given offset (default 0)
 *   Z<offset> - Restore Z axis, applying the given offset (default 0)
 *
 * If there is an Extruder:
 *   E<offset> - Restore E axis, applying the given offset (default 0)
 *
 * With extra axes using default names:
 *   A<offset> - Restore 4th axis, applying the given offset (default 0)
 *   B<offset> - Restore 5th axis, applying the given offset (default 0)
 *   C<offset> - Restore 6th axis, applying the given offset (default 0)
 *   U<offset> - Restore 7th axis, applying the given offset (default 0)
 *   V<offset> - Restore 8th axis, applying the given offset (default 0)
 *   W<offset> - Restore 9th axis, applying the given offset (default 0)
 *
 *   If no axes are specified then all axes are restored.
 */
void GcodeSuite::G61() {

  const uint8_t slot = parser.byteval('S');

  #define SYNC_E(POINT) TERN_(HAS_EXTRUDERS, planner.set_e_position_mm((destination.e = current_position.e = (POINT))))

  #if SAVED_POSITIONS < 256
    if (slot >= SAVED_POSITIONS) {
      SERIAL_ERROR_MSG(STR_INVALID_POS_SLOT STRINGIFY(SAVED_POSITIONS));
      return;
    }
  #endif

  // No saved position? No axes being restored?
  if (!TEST(saved_slots[slot >> 3], slot & 0x07)) return;

  // Apply any given feedrate over 0.0
  REMEMBER(saved, feedrate_mm_s);
  const float fr = parser.linearval('F');
  if (fr > 0.0) feedrate_mm_s = MMM_TO_MMS(fr);

  if (!parser.seen_axis()) {
    DEBUG_ECHOLNPGM("Default position restore");
    do_blocking_move_to(stored_position[slot], feedrate_mm_s);
    SYNC_E(stored_position[slot].e);
  }
  else {
    if (parser.seen(STR_AXES_MAIN)) {
      DEBUG_ECHOPGM(STR_RESTORING_POS " S", slot);
      LOOP_NUM_AXES(i) {
        destination[i] = parser.seen(AXIS_CHAR(i))
          ? stored_position[slot][i] + parser.value_axis_units((AxisEnum)i)
          : current_position[i];
        DEBUG_ECHO(C(' '), C(AXIS_CHAR(i)), p_float_t(destination[i], 2));
      }
      DEBUG_EOL();
      // Move to the saved position
      prepare_line_to_destination();
    }
    #if HAS_EXTRUDERS
      if (parser.seen_test('E')) {
        DEBUG_ECHOLNPGM(STR_RESTORING_POS " S", slot, " E", current_position.e, "=>", stored_position[slot].e);
        SYNC_E(stored_position[slot].e);
      }
    #endif
  }
}

#endif // SAVED_POSITIONS
