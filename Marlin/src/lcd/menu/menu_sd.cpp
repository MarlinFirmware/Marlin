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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

//
// LED Menu
//

#include "../../inc/MarlinConfigPre.h"

#include "menu.h"


void menu_sd() {
  START_MENU();
  BACK_ITEM(MSG_MAIN);
  const bool busy = printingIsActive()
    #if ENABLED(SDSUPPORT)
      , card_detected = card.isMounted()
      , card_open = card_detected && card.isFileOpen()
    #endif
  ;
  
 #if HAS_ENCODER_WHEEL && ENABLED(SDSUPPORT)

    // *** IF THIS SECTION IS CHANGED, REPRODUCE ABOVE ***

    //
    // Autostart
    //
    #if ENABLED(MENU_ADDAUTOSTART)
      if (!busy) ACTION_ITEM(MSG_AUTOSTART, card.beginautostart);
    #endif

    if (card_detected) {
      if (!card_open) {
        MENU_ITEM(gcode,
          #if PIN_EXISTS(SD_DETECT)
            MSG_CHANGE_MEDIA, M21_STR
          #else
            MSG_RELEASE_MEDIA, PSTR("M22")
          #endif
        );
        SUBMENU(MSG_MEDIA_MENU, menu_media);
      }
    }
    else {
      #if PIN_EXISTS(SD_DETECT)
        ACTION_ITEM(MSG_NO_MEDIA, nullptr);
      #else
        GCODES_ITEM(MSG_ATTACH_MEDIA, M21_STR);
        ACTION_ITEM(MSG_MEDIA_RELEASED, nullptr);
      #endif
    }

  #endif // HAS_ENCODER_WHEEL && SDSUPPORT

  END_MENU();
}
