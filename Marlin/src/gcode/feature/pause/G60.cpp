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

#include "../../gcode.h"
#include "../../../module/motion.h"

#define DEBUG_OUT ENABLED(SAVED_POSITIONS_DEBUG)
#include "../../../core/debug_out.h"

/**
 * G60: Saved Positions
 *
 *   S<slot> - Save to a memory slot. (default 0)
 *   Q<slot> - Restore from a memory slot. (default 0)
 *   D<slot> - Delete a memory slot. With no number, delete all.
 */
void GcodeSuite::G60() {
  // With no parameters report any saved positions
  if (!parser.seen_any()) {
    uint8_t count = 0;
    for (uint8_t s = 0; s < SAVED_POSITIONS; ++s) {
      if (!did_save_position[s]) continue;
      ++count;
      const xyze_pos_t &pos = stored_position[s];
      SERIAL_ECHO(STR_SAVED_POS, s, C(':'));
      #if NUM_AXES
        SERIAL_ECHOPGM_P(
          LIST_N(DOUBLE(NUM_AXES),
            SP_X_LBL, pos.x, SP_Y_LBL, pos.y, SP_Z_LBL, pos.z,
            SP_I_LBL, pos.i, SP_J_LBL, pos.j, SP_K_LBL, pos.k,
            SP_U_LBL, pos.u, SP_V_LBL, pos.v, SP_W_LBL, pos.w
          )
        );
      #endif
      #if HAS_EXTRUDERS
        SERIAL_ECHOPGM_P(SP_E_LBL, pos.e);
      #endif
      SERIAL_EOL();
    }
    if (!count) SERIAL_ECHOLNPGM("No Saved Positions");
    return;
  }

  // Only one of these parameters is permitted
  const uint8_t seenD = parser.seen_test('D'),
                seenQ = parser.seen_test('Q'),
                seenS = parser.seen_test('S');
  if (seenD + seenQ + seenS > 1) return;

  // G60 D : Delete all saved positions
  if (seenD && !parser.seenval('D')) {
    did_save_position.reset();
    return;
  }

  // G60 Dn / Q / S : Get the slot value
  const uint8_t slot = parser.byteval(seenD ? 'D' : seenQ ? 'Q' : 'S');

  // G60 Q : Redirect to G61(slot)
  if (seenQ) return G61(slot);

  // Valid slot number?
  if (SAVED_POSITIONS < 256 && slot >= SAVED_POSITIONS) {
    SERIAL_ERROR_MSG(STR_INVALID_POS_SLOT STRINGIFY(SAVED_POSITIONS));
    return;
  }

  // G60 Dn
  if (seenD) {
    did_save_position.clear(slot);
    return;
  }

  // G60 S
  stored_position[slot] = current_position;
  did_save_position.set(slot);

  #if ENABLED(SAVED_POSITIONS_DEBUG)
  {
    const xyze_pos_t &pos = stored_position[slot];
    DEBUG_ECHOPGM(STR_SAVED_POS, slot, ": ");
    #if NUM_AXES
      DEBUG_ECHOPGM_P(
        LIST_N(DOUBLE(NUM_AXES),
          SP_X_LBL, pos.x, SP_Y_LBL, pos.y, SP_Z_LBL, pos.z,
          SP_I_LBL, pos.i, SP_J_LBL, pos.j, SP_K_LBL, pos.k,
          SP_U_LBL, pos.u, SP_V_LBL, pos.v, SP_W_LBL, pos.w
        )
      );
    #endif
    #if HAS_EXTRUDERS
      DEBUG_ECHOPGM_P(SP_E_LBL, pos.e);
    #endif
    DEBUG_EOL();
  }
  #endif
}

#endif // SAVED_POSITIONS
