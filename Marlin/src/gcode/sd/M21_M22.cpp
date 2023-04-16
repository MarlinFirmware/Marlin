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
 * M21: Select / Mount the SD Card or USB Flash Drive
 *
 * With MULTI_VOLUME:
 *  P0 or S - Change to the SD Card and mount it
 *  P1 or U - Change to the USB Drive and mount it
 *  P2 or O - Change to the SDIO Card and mount it
 */
void GcodeSuite::M21() {
  #if ENABLED(MULTI_VOLUME)
    static int8_t vol = (
      #if HAS_USB_FLASH_DRIVE && !SHARED_VOLUME_IS(SD_ONBOARD)
        1
      #else
        0
      #endif
    );
    const int8_t newvol = (
                                     parser.seen_test('S')  ? 0 : // "S" for SD Card
      TERN0(USB_FLASH_DRIVE_SUPPORT, parser.seen_test('U')) ? 1 : // "U" for USB
      TERN0(SDIO_SUPPORT,            parser.seen_test('O')) ? 2 : // "O" for Onboard (SDIO)
                                     parser.intval('P', vol)      // "P" for S, U, O
    );

    if (newvol != vol) {
      vol = newvol;
      switch (newvol + 1) {
        default: card.changeMedia(&card.media_driver_sdcard); break;
        #if ENABLED(USB_FLASH_DRIVE_SUPPORT)
          case SV_USB_FLASH_DRIVE: card.changeMedia(&card.media_driver_usbFlash); break;
        #endif
        #if ENABLED(SDIO_SUPPORT)
          case SV_SDIO_ONBOARD: card.changeMedia(&card.media_driver_sdiocard); break;
        #endif
      }
    }

  #endif // MULTI_VOLUME

  card.mount();
}

/**
 * M22: Release the SD Card or USB Flash Drive
 */
void GcodeSuite::M22() {
  if (!IS_SD_PRINTING()) card.release();
}

#endif // HAS_MEDIA
