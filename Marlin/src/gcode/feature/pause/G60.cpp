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
 * G60: Save current position
 *
 *   S<slot> - Memory slot # (0-based) to save into (default 0)
 */
void GcodeSuite::G60() {
  const uint8_t slot = parser.byteval('S');

  if (slot >= SAVED_POSITIONS) {
    SERIAL_ERROR_MSG(STR_INVALID_POS_SLOT STRINGIFY(SAVED_POSITIONS));
    return;
  }

  stored_position[slot] = current_position;
  SBI(saved_slots[slot >> 3], slot & 0x07);

  #if ENABLED(SAVED_POSITIONS_DEBUG)
  {
    const xyze_pos_t &pos = stored_position[slot];
    DEBUG_ECHOPGM(STR_SAVED_POS " S", slot, " :");
    DEBUG_ECHOLNPGM_P(
      LIST_N(DOUBLE(NUM_AXES),
        SP_X_LBL, pos.x, SP_Y_LBL, pos.y, SP_Z_LBL, pos.z,
        SP_I_LBL, pos.i, SP_J_LBL, pos.j, SP_K_LBL, pos.k,
        SP_U_LBL, pos.u, SP_V_LBL, pos.v, SP_W_LBL, pos.w
      )
      #if HAS_EXTRUDERS
        , SP_E_LBL, pos.e
      #endif
    );
  }
  #endif
}

#endif // SAVED_POSITIONS
