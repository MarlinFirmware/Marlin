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
// Custom Menus
//

#include "../../inc/MarlinConfigPre.h"

#if HAS_LCD_MENU && ( ENABLED(CUSTOM_MENUS_MAIN) || ENABLED(CUSTOM_MENUS_CONFIGURATION) )

#include "menu.h"
#include "../../gcode/queue.h"

#ifdef CUSTOM_MENU_MAIN_SCRIPT_DONE
  #define _DONE_SCRIPT "\n" CUSTOM_MENU_MAIN_SCRIPT_DONE
#else
  #define _DONE_SCRIPT ""
#endif

#ifdef CUSTOM_MENUS_CONFIGURATION_SCRIPT_DONE
  #define _DONE_SCRIPT "\n" CUSTOM_MENUS_CONFIGURATION_SCRIPT_DONE
#else
  #define _DONE_SCRIPT ""
#endif

void _lcd_custom_menu_main_gcode(PGM_P const cmd) {
  queue.inject_P(cmd);
  #if ENABLED(CUSTOM_MENU_MAIN_SCRIPT_AUDIBLE_FEEDBACK) && HAS_BUZZER
    ui.completion_feedback();
  #endif
  #if ENABLED(CUSTOM_MENU_MAIN_SCRIPT_RETURN)
    ui.return_to_status();
  #endif
}

void _lcd_custom_menus_configuration_gcode(PGM_P const cmd) {
  queue.inject_P(cmd);
  #if ENABLED(CUSTOM_MENUS_CONFIGURATION_SCRIPT_AUDIBLE_FEEDBACK) && HAS_BUZZER
    ui.completion_feedback();
  #endif
  #if ENABLED(CUSTOM_MENUS_CONFIGURATION_SCRIPT_RETURN)
    ui.return_to_status();
  #endif
}

void custom_menus_main() {
  START_MENU();
  BACK_ITEM(MSG_MAIN);
  #define HAS_CUSTOM_MENU_ITEM(N) (defined(CUSTOM_MENU_MAIN_DESC_##N) && defined(CUSTOM_MENU_MAIN_GCODE_##N))
  #define CUSTOM_MENU_ITEM(N) ACTION_ITEM_P(PSTR(CUSTOM_MENU_MAIN_DESC_##N), []{ _lcd_custom_menu_main_gcode(PSTR(CUSTOM_MENU_MAIN_GCODE_##N _DONE_SCRIPT)); });
  #if HAS_CUSTOM_MENU_ITEM(1)
    CUSTOM_MENU_ITEM(1);
  #endif
  #if HAS_CUSTOM_MENU_ITEM(2)
    CUSTOM_MENU_ITEM(2);
  #endif
  #if HAS_CUSTOM_MENU_ITEM(3)
    CUSTOM_MENU_ITEM(3);
  #endif
  #if HAS_CUSTOM_MENU_ITEM(4)
    CUSTOM_MENU_ITEM(4);
  #endif
  #if HAS_CUSTOM_MENU_ITEM(5)
    CUSTOM_MENU_ITEM(5);
  #endif
  #if HAS_CUSTOM_MENU_ITEM(6)
    CUSTOM_MENU_ITEM(6);
  #endif
  #if HAS_CUSTOM_MENU_ITEM(7)
    CUSTOM_MENU_ITEM(7);
  #endif
  #if HAS_CUSTOM_MENU_ITEM(8)
    CUSTOM_MENU_ITEM(8);
  #endif
  #if HAS_CUSTOM_MENU_ITEM(9)
    CUSTOM_MENU_ITEM(9);
  #endif
  #if HAS_CUSTOM_MENU_ITEM(10)
    CUSTOM_MENU_ITEM(10);
  #endif
  #if HAS_CUSTOM_MENU_ITEM(11)
    CUSTOM_MENU_ITEM(11);
  #endif
  #if HAS_CUSTOM_MENU_ITEM(12)
    CUSTOM_MENU_ITEM(12);
  #endif
  #if HAS_CUSTOM_MENU_ITEM(13)
    CUSTOM_MENU_ITEM(13);
  #endif
  #if HAS_CUSTOM_MENU_ITEM(14)
    CUSTOM_MENU_ITEM(14);
  #endif
  #if HAS_CUSTOM_MENU_ITEM(15)
    CUSTOM_MENU_ITEM(15);
  #endif
  #if HAS_CUSTOM_MENU_ITEM(16)
    CUSTOM_MENU_ITEM(16);
  #endif
  #if HAS_CUSTOM_MENU_ITEM(17)
    CUSTOM_MENU_ITEM(17);
  #endif
  #if HAS_CUSTOM_MENU_ITEM(18)
    CUSTOM_MENU_ITEM(18);
  #endif
  #if HAS_CUSTOM_MENU_ITEM(19)
    CUSTOM_MENU_ITEM(19);
  #endif
  #if HAS_CUSTOM_MENU_ITEM(20)
    CUSTOM_MENU_ITEM(20);
  #endif
  #if HAS_CUSTOM_MENU_ITEM(21)
    CUSTOM_MENU_ITEM(21);
  #endif
  #if HAS_CUSTOM_MENU_ITEM(22)
    CUSTOM_MENU_ITEM(22);
  #endif
  #if HAS_CUSTOM_MENU_ITEM(23)
    CUSTOM_MENU_ITEM(23);
  #endif
  #if HAS_CUSTOM_MENU_ITEM(24)
    CUSTOM_MENU_ITEM(24);
  #endif
  #if HAS_CUSTOM_MENU_ITEM(25)
    CUSTOM_MENU_ITEM(25);
  #endif
  END_MENU();
}

void custom_menus_configuration() {
  START_MENU();
  BACK_ITEM(MSG_MAIN);
  #define HAS_CUSTOM_MENU_ITEM(N) (defined(CUSTOM_MENU_CONFIGURATION_DESC_##N) && defined(CUSTOM_MENU_CONFIGURATION_GCODE_##N))
  #define CUSTOM_MENU_ITEM(N) ACTION_ITEM_P(PSTR(CUSTOM_MENU_CONFIGURATION_DESC_##N), []{ _lcd_custom_menus_configuration_gcode(PSTR(CUSTOM_MENU_CONFIGURATION_GCODE_##N _DONE_SCRIPT)); });
  #if HAS_CUSTOM_MENU_ITEM(1)
    CUSTOM_MENU_ITEM(1);
  #endif
  #if HAS_CUSTOM_MENU_ITEM(2)
    CUSTOM_MENU_ITEM(2);
  #endif
  #if HAS_CUSTOM_MENU_ITEM(3)
    CUSTOM_MENU_ITEM(3);
  #endif
  #if HAS_CUSTOM_MENU_ITEM(4)
    CUSTOM_MENU_ITEM(4);
  #endif
  #if HAS_CUSTOM_MENU_ITEM(5)
    CUSTOM_MENU_ITEM(5);
  #endif
  #if HAS_CUSTOM_MENU_ITEM(6)
    CUSTOM_MENU_ITEM(6);
  #endif
  #if HAS_CUSTOM_MENU_ITEM(7)
    CUSTOM_MENU_ITEM(7);
  #endif
  #if HAS_CUSTOM_MENU_ITEM(8)
    CUSTOM_MENU_ITEM(8);
  #endif
  #if HAS_CUSTOM_MENU_ITEM(9)
    CUSTOM_MENU_ITEM(9);
  #endif
  #if HAS_CUSTOM_MENU_ITEM(10)
    CUSTOM_MENU_ITEM(10);
  #endif
  #if HAS_CUSTOM_MENU_ITEM(11)
    CUSTOM_MENU_ITEM(11);
  #endif
  #if HAS_CUSTOM_MENU_ITEM(12)
    CUSTOM_MENU_ITEM(12);
  #endif
  #if HAS_CUSTOM_MENU_ITEM(13)
    CUSTOM_MENU_ITEM(13);
  #endif
  #if HAS_CUSTOM_MENU_ITEM(14)
    CUSTOM_MENU_ITEM(14);
  #endif
  #if HAS_CUSTOM_MENU_ITEM(15)
    CUSTOM_MENU_ITEM(15);
  #endif
  #if HAS_CUSTOM_MENU_ITEM(16)
    CUSTOM_MENU_ITEM(16);
  #endif
  #if HAS_CUSTOM_MENU_ITEM(17)
    CUSTOM_MENU_ITEM(17);
  #endif
  #if HAS_CUSTOM_MENU_ITEM(18)
    CUSTOM_MENU_ITEM(18);
  #endif
  #if HAS_CUSTOM_MENU_ITEM(19)
    CUSTOM_MENU_ITEM(19);
  #endif
  #if HAS_CUSTOM_MENU_ITEM(20)
    CUSTOM_MENU_ITEM(20);
  #endif
  #if HAS_CUSTOM_MENU_ITEM(21)
    CUSTOM_MENU_ITEM(21);
  #endif
  #if HAS_CUSTOM_MENU_ITEM(22)
    CUSTOM_MENU_ITEM(22);
  #endif
  #if HAS_CUSTOM_MENU_ITEM(23)
    CUSTOM_MENU_ITEM(23);
  #endif
  #if HAS_CUSTOM_MENU_ITEM(24)
    CUSTOM_MENU_ITEM(24);
  #endif
  #if HAS_CUSTOM_MENU_ITEM(25)
    CUSTOM_MENU_ITEM(25);
  #endif
  END_MENU();
}

#endif // HAS_LCD_MENU && (  CUSTOM_MENUS_MAIN || CUSTOM_MENUS_CONFIGURATION )
