/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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
// Setup Printer Environment Menu
//

#include "../../inc/MarlinConfigPre.h"

#if HAS_LCD_MENU

#include "menu.h"
#include "../../module/motion.h"
#include "../../module/temperature.h"

#if ENABLED(SDSUPPORT)

#include "../../sd/cardreader.h"
#include "../../gcode/queue.h"
#include "../../module/printcounter.h"

#endif // SDSUPPORT

void menu_temperature();
void menu_motion();
void _lcd_level_bed_corners();

void menu_prepare() {
  START_MENU();
  MENU_BACK(MSG_MAIN);
  
  MENU_ITEM(submenu, MSG_TEMPERATURE, menu_temperature);
  MENU_ITEM(submenu, MSG_LEVEL_CORNERS, _lcd_level_bed_corners);
  MENU_ITEM(submenu, MSG_MANUAL_MOTION, menu_motion);

  #if ENABLED(SDSUPPORT)
  
    if (card.flag.cardOK) {
      MENU_ITEM(function, MSG_NO_CARD, NULL);
      #if !PIN_EXISTS(SD_DETECT)
        MENU_ITEM(gcode, MSG_INIT_SDCARD, PSTR("M21")); // Manually initialize the SD-card via user interface
      #else 
        MENU_ITEM(submenu, MSG_CARD_MENU, menu_sdcard);
        #if !PIN_EXISTS(SD_DETECT)
          MENU_ITEM(gcode, MSG_CHANGE_SDCARD, PSTR("M21"));  // SD-card changed by user
        #endif
      #endif
    }
  #endif // SDSUPPORT

  END_MENU();
}

#endif // HAS_LCD_MENU
