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

//
// Custom User Menu
//

#include "../../inc/MarlinConfigPre.h"

#if BOTH(HAS_LCD_MENU, CUSTOM_USER_MENUS)

#include "menu_item.h"
#include "../../gcode/queue.h"

#ifdef USER_SCRIPT_DONE
  #define _DONE_SCRIPT "\n" USER_SCRIPT_DONE
#else
  #define _DONE_SCRIPT ""
#endif

void _lcd_user_gcode(PGM_P const cmd) {
  queue.inject_P(cmd);
  TERN_(USER_SCRIPT_AUDIBLE_FEEDBACK, ui.completion_feedback());
  TERN_(USER_SCRIPT_RETURN, ui.return_to_status());
}

void menu_user() {
  START_MENU();
  BACK_ITEM(MSG_MAIN);
  #if ENABLED(CUSTOM_USER_MENU_CONFIRM)
    #define USER_ITEM_CONFIRM(N) SUBMENU_P(PSTR(USER_DESC_##N), []{ MenuItem_confirm::confirm_screen(   \
      []{                                                                                       \
        _lcd_user_gcode(PSTR(USER_GCODE_##N _DONE_SCRIPT));                                     \
        ui.goto_previous_screen();                                                              \
      },                                                                                        \
        ui.goto_previous_screen,                                                                \
        PSTR(USER_DESC_##N "?")                                                                 \
      );                                                                                        \
    });
  #endif

  #define USER_ITEM(N) ACTION_ITEM_P(PSTR(USER_DESC_##N), []{ _lcd_user_gcode(PSTR(USER_GCODE_##N _DONE_SCRIPT)); });

  #if HAS_USER_ITEM(1)
    #if ENABLED(USER_CONFIRM_1)
      USER_ITEM_CONFIRM(1);
    #else
      USER_ITEM(1);
    #endif
  #endif
  #if HAS_USER_ITEM(2)
    #if ENABLED(USER_CONFIRM_2)
      USER_ITEM_CONFIRM(2);
    #else
      USER_ITEM(2);
    #endif
  #endif
  #if HAS_USER_ITEM(3)
    #if ENABLED(USER_CONFIRM_3)
      USER_ITEM_CONFIRM(3);
    #else
      USER_ITEM(3);
    #endif
  #endif
  #if HAS_USER_ITEM(4)
    #if ENABLED(USER_CONFIRM_4)
      USER_ITEM_CONFIRM(4);
    #else
      USER_ITEM(4);
    #endif
  #endif
  #if HAS_USER_ITEM(5)
    #if ENABLED(USER_CONFIRM_5)
      USER_ITEM_CONFIRM(5);
    #else
      USER_ITEM(5);
    #endif
  #endif
  #if HAS_USER_ITEM(6)
    #if ENABLED(USER_CONFIRM_6)
      USER_ITEM_CONFIRM(6);
    #else
      USER_ITEM(6);
    #endif
  #endif
  #if HAS_USER_ITEM(7)
    #if ENABLED(USER_CONFIRM_7)
      USER_ITEM_CONFIRM(7);
    #else
      USER_ITEM(7);
    #endif
  #endif
  #if HAS_USER_ITEM(8)
    #if ENABLED(USER_CONFIRM_8)
      USER_ITEM_CONFIRM(8);
    #else
      USER_ITEM(8);
    #endif
  #endif
  #if HAS_USER_ITEM(9)
    #if ENABLED(USER_CONFIRM_9)
      USER_ITEM_CONFIRM(9);
    #else
      USER_ITEM(9);
    #endif
  #endif
  #if HAS_USER_ITEM(10)
    #if ENABLED(USER_CONFIRM_10)
      USER_ITEM_CONFIRM(10);
    #else
      USER_ITEM(10);
    #endif
  #endif
  #if HAS_USER_ITEM(11)
    #if ENABLED(USER_CONFIRM_11)
      USER_ITEM_CONFIRM(11);
    #else
      USER_ITEM(11);
    #endif
  #endif
  #if HAS_USER_ITEM(12)
    #if ENABLED(USER_CONFIRM_12)
      USER_ITEM_CONFIRM(12);
    #else
      USER_ITEM(12);
    #endif
  #endif
  #if HAS_USER_ITEM(13)
    #if ENABLED(USER_CONFIRM_13)
      USER_ITEM_CONFIRM(13);
    #else
      USER_ITEM(13);
    #endif
  #endif
  #if HAS_USER_ITEM(14)
    #if ENABLED(USER_CONFIRM_14)
      USER_ITEM_CONFIRM(14);
    #else
      USER_ITEM(14);
    #endif
  #endif
  #if HAS_USER_ITEM(15)
    #if ENABLED(USER_CONFIRM_15)
      USER_ITEM_CONFIRM(15);
    #else
      USER_ITEM(15);
    #endif
  #endif
  #if HAS_USER_ITEM(16)
    #if ENABLED(USER_CONFIRM_16)
      USER_ITEM_CONFIRM(16);
    #else
      USER_ITEM(16);
    #endif
  #endif
  #if HAS_USER_ITEM(17)
    #if ENABLED(USER_CONFIRM_17)
      USER_ITEM_CONFIRM(17);
    #else
      USER_ITEM(17);
    #endif
  #endif
  #if HAS_USER_ITEM(18)
    #if ENABLED(USER_CONFIRM_18)
      USER_ITEM_CONFIRM(18);
    #else
      USER_ITEM(18);
    #endif
  #endif
  #if HAS_USER_ITEM(19)
    #if ENABLED(USER_CONFIRM_19)
      USER_ITEM_CONFIRM(19);
    #else
      USER_ITEM(19);
    #endif
  #endif
  #if HAS_USER_ITEM(20)
    #if ENABLED(USER_CONFIRM_20)
      USER_ITEM_CONFIRM(20);
    #else
      USER_ITEM(20);
    #endif
  #endif
  #if HAS_USER_ITEM(21)
    #if ENABLED(USER_CONFIRM_21)
      USER_ITEM_CONFIRM(21);
    #else
      USER_ITEM(21);
    #endif
  #endif
  #if HAS_USER_ITEM(22)
    #if ENABLED(USER_CONFIRM_22)
      USER_ITEM_CONFIRM(22);
    #else
      USER_ITEM(22);
    #endif
  #endif
  #if HAS_USER_ITEM(23)
    #if ENABLED(USER_CONFIRM_23)
      USER_ITEM_CONFIRM(23);
    #else
      USER_ITEM(23);
    #endif
  #endif
  #if HAS_USER_ITEM(24)
    #if ENABLED(USER_CONFIRM_24)
      USER_ITEM_CONFIRM(24);
    #else
      USER_ITEM(24);
    #endif
  #endif
  #if HAS_USER_ITEM(25)
    #if ENABLED(USER_CONFIRM_25)
      USER_ITEM_CONFIRM(25);
    #else
      USER_ITEM(25);
    #endif
  #endif
  END_MENU();
}

#endif // HAS_LCD_MENU && CUSTOM_USER_MENUS
