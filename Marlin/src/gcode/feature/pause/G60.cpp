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
    const xyze_pos_t &pos = stored_position[slot];
    DEBUG_ECHOPAIR(STR_SAVED_POS " S", slot);
    DEBUG_ECHOPAIR_F(" : X", pos.x);
    DEBUG_ECHOPAIR_F_P(SP_Y_STR, pos.y);
    DEBUG_ECHOPAIR_F_P(SP_Z_STR, pos.z);
    DEBUG_ECHOLNPAIR_F_P(SP_E_STR, pos.e);
  #endif
}

#endif // SAVED_POSITIONS
