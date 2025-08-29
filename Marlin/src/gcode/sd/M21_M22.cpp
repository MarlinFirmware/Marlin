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

#include "../../inc/MarlinConfig.h"

#if HAS_MEDIA

#include "../gcode.h"
#include "../../sd/cardreader.h"

/**
 * M21: Mount Media
 *
 * With MULTI_VOLUME:
 *  P0 or S - Change to the SD Card and mount it
 *  P1 or U - Change to the USB Drive and mount it
 */
void GcodeSuite::M21() {
  #if HAS_MULTI_VOLUME
    const int8_t vol = parser.intval('P', -1);
    if (vol == 0 || parser.seen_test('S'))       // "S" for SD Card
      card.selectMediaSDCard();
    else if (vol == 1 || parser.seen_test('U'))  // "U" for USB
      card.selectMediaFlashDrive();
  #endif
  card.mount();
}

/**
 * M22: Release Media
 */
void GcodeSuite::M22() {
  if (!card.isStillPrinting()) card.release();
}

#endif // HAS_MEDIA
