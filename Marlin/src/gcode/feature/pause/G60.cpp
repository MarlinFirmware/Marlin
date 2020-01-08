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
#include "../../gcode.h"
#include "../../../module/motion.h"

/**
 * G60:  save current position
 *        S<slot> specifies memory slot # (0-based) to save into (default 0)
 */
void GcodeSuite::G60() {
  const uint8_t slot = parser.byteval('S');

  if (slot >= NUM_POSITON_SLOTS) {
    SERIAL_ERROR_START();
    SERIAL_ECHOLNPAIR_F(MSG_INVALID_POS_SLOT, NUM_POSITON_SLOTS);
    return;
  } 
  stored_position[slot] = current_position;
  isPosSaved = true;
  //setPosSaved(true);

  SERIAL_ECHOPGM(MSG_SAVED_POS);
  SERIAL_ECHOPAIR_F(" S", slot);
  SERIAL_ECHOPAIR_F("<-X:", stored_position[slot].x);
  SERIAL_ECHOPAIR_F(" Y:", stored_position[slot].y);
  SERIAL_ECHOPAIR_F(" Z:", stored_position[slot].z);
  SERIAL_ECHOLNPAIR_F(" E:", stored_position[slot].e);
  
}

