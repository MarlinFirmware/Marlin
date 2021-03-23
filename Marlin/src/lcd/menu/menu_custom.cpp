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
// Custom Menus
//

#include "../../inc/MarlinConfigPre.h"

#if HAS_LCD_MENU && EITHER(CUSTOM_MENUS_MAIN, CUSTOM_MENUS_CONFIGURATION)

#include "menu_item.h"
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

#if ENABLED(CUSTOM_MENUS_MAIN)

  void _lcd_custom_menu_main_gcode(PGM_P const cmd) {
    queue.inject_P(cmd);
    TERN_(CUSTOM_MENU_MAIN_SCRIPT_AUDIBLE_FEEDBACK, ui.completion_feedback());
    TERN_(CUSTOM_MENU_MAIN_SCRIPT_RETURN, ui.return_to_status());
  }

  void custom_menus_main() {
    START_MENU();
    BACK_ITEM(MSG_MAIN);

    #define HAS_CUSTOM_ITEM_MAIN(N) (defined(CUSTOM_MENU_MAIN_DESC_##N) && defined(CUSTOM_MENU_MAIN_GCODE_##N))

    #define CUSTOM_TEST_MAIN(N) { \
      constexpr char c = CUSTOM_MENU_MAIN_GCODE_##N[strlen(CUSTOM_MENU_MAIN_GCODE_##N) - 1]; \
      static_assert(c != '\n' && c != '\r', "CUSTOM_MENU_MAIN_GCODE_" STRINGIFY(N) " cannot have a newline at the end. Please remove it."); \
    }

    #define GCODE_LAMBDA_MAIN(N) []{ _lcd_custom_menu_main_gcode(PSTR(CUSTOM_MENU_MAIN_GCODE_##N _DONE_SCRIPT)); }
    #define _CUSTOM_ITEM_MAIN(N) ACTION_ITEM_P(PSTR(CUSTOM_MENU_MAIN_DESC_##N), GCODE_LAMBDA_MAIN(N));
    #define _CUSTOM_ITEM_MAIN_CONFIRM(N)    \
      SUBMENU_P(PSTR(USER_DESC_##N), []{    \
          MenuItem_confirm::confirm_screen( \
            GCODE_LAMBDA_MAIN(N),           \
            ui.goto_previous_screen,        \
            PSTR(USER_DESC_##N "?")         \
          );                                \
        })

    #define CUSTOM_ITEM_MAIN(N) do{ if (ENABLED(CUSTOM_MENU_MAIN_CONFIRM_##N)) _CUSTOM_ITEM_MAIN_CONFIRM(N); else _CUSTOM_ITEM_MAIN(N); }while(0)

    #if HAS_CUSTOM_ITEM_MAIN(1)
      CUSTOM_TEST_MAIN(1);
      CUSTOM_ITEM_MAIN(1);
    #endif
    #if HAS_CUSTOM_ITEM_MAIN(2)
      CUSTOM_TEST_MAIN(2);
      CUSTOM_ITEM_MAIN(2);
    #endif
    #if HAS_CUSTOM_ITEM_MAIN(3)
      CUSTOM_TEST_MAIN(3);
      CUSTOM_ITEM_MAIN(3);
    #endif
    #if HAS_CUSTOM_ITEM_MAIN(4)
      CUSTOM_TEST_MAIN(4);
      CUSTOM_ITEM_MAIN(4);
    #endif
    #if HAS_CUSTOM_ITEM_MAIN(5)
      CUSTOM_TEST_MAIN(5);
      CUSTOM_ITEM_MAIN(5);
    #endif
    #if HAS_CUSTOM_ITEM_MAIN(6)
      CUSTOM_TEST_MAIN(6);
      CUSTOM_ITEM_MAIN(6);
    #endif
    #if HAS_CUSTOM_ITEM_MAIN(7)
      CUSTOM_TEST_MAIN(7);
      CUSTOM_ITEM_MAIN(7);
    #endif
    #if HAS_CUSTOM_ITEM_MAIN(8)
      CUSTOM_TEST_MAIN(8);
      CUSTOM_ITEM_MAIN(8);
    #endif
    #if HAS_CUSTOM_ITEM_MAIN(9)
      CUSTOM_TEST_MAIN(9);
      CUSTOM_ITEM_MAIN(9);
    #endif
    #if HAS_CUSTOM_ITEM_MAIN(10)
      CUSTOM_TEST_MAIN(10);
      CUSTOM_ITEM_MAIN(10);
    #endif
    #if HAS_CUSTOM_ITEM_MAIN(11)
      CUSTOM_TEST_MAIN(11);
      CUSTOM_ITEM_MAIN(11);
    #endif
    #if HAS_CUSTOM_ITEM_MAIN(12)
      CUSTOM_TEST_MAIN(12);
      CUSTOM_ITEM_MAIN(12);
    #endif
    #if HAS_CUSTOM_ITEM_MAIN(13)
      CUSTOM_TEST_MAIN(13);
      CUSTOM_ITEM_MAIN(13);
    #endif
    #if HAS_CUSTOM_ITEM_MAIN(14)
      CUSTOM_TEST_MAIN(14);
      CUSTOM_ITEM_MAIN(14);
    #endif
    #if HAS_CUSTOM_ITEM_MAIN(15)
      CUSTOM_TEST_MAIN(15);
      CUSTOM_ITEM_MAIN(15);
    #endif
    #if HAS_CUSTOM_ITEM_MAIN(16)
      CUSTOM_TEST_MAIN(16);
      CUSTOM_ITEM_MAIN(16);
    #endif
    #if HAS_CUSTOM_ITEM_MAIN(17)
      CUSTOM_TEST_MAIN(17);
      CUSTOM_ITEM_MAIN(17);
    #endif
    #if HAS_CUSTOM_ITEM_MAIN(18)
      CUSTOM_TEST_MAIN(18);
      CUSTOM_ITEM_MAIN(18);
    #endif
    #if HAS_CUSTOM_ITEM_MAIN(19)
      CUSTOM_TEST_MAIN(19);
      CUSTOM_ITEM_MAIN(19);
    #endif
    #if HAS_CUSTOM_ITEM_MAIN(20)
      CUSTOM_TEST_MAIN(20);
      CUSTOM_ITEM_MAIN(20);
    #endif
    #if HAS_CUSTOM_ITEM_MAIN(21)
      CUSTOM_TEST_MAIN(21);
      CUSTOM_ITEM_MAIN(21);
    #endif
    #if HAS_CUSTOM_ITEM_MAIN(22)
      CUSTOM_TEST_MAIN(22);
      CUSTOM_ITEM_MAIN(22);
    #endif
    #if HAS_CUSTOM_ITEM_MAIN(23)
      CUSTOM_TEST_MAIN(23);
      CUSTOM_ITEM_MAIN(23);
    #endif
    #if HAS_CUSTOM_ITEM_MAIN(24)
      CUSTOM_TEST_MAIN(24);
      CUSTOM_ITEM_MAIN(24);
    #endif
    #if HAS_CUSTOM_ITEM_MAIN(25)
      CUSTOM_TEST_MAIN(25);
      CUSTOM_ITEM_MAIN(25);
    #endif
    END_MENU();
  }

#endif

#if ENABLED(CUSTOM_MENUS_CONFIGURATION)

  void _lcd_custom_menus_configuration_gcode(PGM_P const cmd) {
    queue.inject_P(cmd);
    TERN_(CUSTOM_MENUS_CONFIGURATION_SCRIPT_AUDIBLE_FEEDBACK, ui.completion_feedback());
    TERN_(CUSTOM_MENUS_CONFIGURATION_SCRIPT_RETURN, ui.return_to_status());
  }

  void custom_menus_configuration() {
    START_MENU();
    BACK_ITEM(MSG_MAIN);

    #define HAS_CUSTOM_ITEM_CONF(N) (defined(CUSTOM_MENU_CONFIGURATION_DESC_##N) && defined(CUSTOM_MENU_CONFIGURATION_GCODE_##N))

    #define CUSTOM_TEST_CONF(N) { \
      constexpr char c = CUSTOM_MENU_CONFIGURATION_GCODE_##N[strlen(CUSTOM_MENU_CONFIGURATION_GCODE_##N) - 1]; \
      static_assert(c != '\n' && c != '\r', "CUSTOM_MENU_CONFIGURATION_GCODE_" STRINGIFY(N) " cannot have a newline at the end. Please remove it."); \
    }

    #define GCODE_LAMBDA_CONF(N) []{ _lcd_custom_menus_configuration_gcode(PSTR(CUSTOM_MENU_CONFIGURATION_GCODE_##N _DONE_SCRIPT)); }
    #define _CUSTOM_ITEM_CONF(N) ACTION_ITEM_P(PSTR(CUSTOM_MENU_CONFIGURATION_DESC_##N), GCODE_LAMBDA_CONF(N));
    #define _CUSTOM_ITEM_CONF_CONFIRM(N)    \
      SUBMENU_P(PSTR(USER_DESC_##N), []{    \
          MenuItem_confirm::confirm_screen( \
            GCODE_LAMBDA_CONF(N),           \
            ui.goto_previous_screen,        \
            PSTR(USER_DESC_##N "?")         \
          );                                \
        })

    #define CUSTOM_ITEM_CONF(N) do{ if (ENABLED(CUSTOM_MENU_CONFIGURATION_CONFIRM_##N)) _CUSTOM_ITEM_CONF_CONFIRM(N); else _CUSTOM_ITEM_CONF(N); }while(0)

    #if HAS_CUSTOM_ITEM_CONF(1)
      CUSTOM_TEST_CONF(1);
      CUSTOM_ITEM_CONF(1);
    #endif
    #if HAS_CUSTOM_ITEM_CONF(2)
      CUSTOM_TEST_CONF(2);
      CUSTOM_ITEM_CONF(2);
    #endif
    #if HAS_CUSTOM_ITEM_CONF(3)
      CUSTOM_TEST_CONF(3);
      CUSTOM_ITEM_CONF(3);
    #endif
    #if HAS_CUSTOM_ITEM_CONF(4)
      CUSTOM_TEST_CONF(4);
      CUSTOM_ITEM_CONF(4);
    #endif
    #if HAS_CUSTOM_ITEM_CONF(5)
      CUSTOM_TEST_CONF(5);
      CUSTOM_ITEM_CONF(5);
    #endif
    #if HAS_CUSTOM_ITEM_CONF(6)
      CUSTOM_TEST_CONF(6);
      CUSTOM_ITEM_CONF(6);
    #endif
    #if HAS_CUSTOM_ITEM_CONF(7)
      CUSTOM_TEST_CONF(7);
      CUSTOM_ITEM_CONF(7);
    #endif
    #if HAS_CUSTOM_ITEM_CONF(8)
      CUSTOM_TEST_CONF(8);
      CUSTOM_ITEM_CONF(8);
    #endif
    #if HAS_CUSTOM_ITEM_CONF(9)
      CUSTOM_TEST_CONF(9);
      CUSTOM_ITEM_CONF(9);
    #endif
    #if HAS_CUSTOM_ITEM_CONF(10)
      CUSTOM_TEST_CONF(10);
      CUSTOM_ITEM_CONF(10);
    #endif
    #if HAS_CUSTOM_ITEM_CONF(11)
      CUSTOM_TEST_CONF(11);
      CUSTOM_ITEM_CONF(11);
    #endif
    #if HAS_CUSTOM_ITEM_CONF(12)
      CUSTOM_TEST_CONF(12);
      CUSTOM_ITEM_CONF(12);
    #endif
    #if HAS_CUSTOM_ITEM_CONF(13)
      CUSTOM_TEST_CONF(13);
      CUSTOM_ITEM_CONF(13);
    #endif
    #if HAS_CUSTOM_ITEM_CONF(14)
      CUSTOM_TEST_CONF(14);
      CUSTOM_ITEM_CONF(14);
    #endif
    #if HAS_CUSTOM_ITEM_CONF(15)
      CUSTOM_TEST_CONF(15);
      CUSTOM_ITEM_CONF(15);
    #endif
    #if HAS_CUSTOM_ITEM_CONF(16)
      CUSTOM_TEST_CONF(16);
      CUSTOM_ITEM_CONF(16);
    #endif
    #if HAS_CUSTOM_ITEM_CONF(17)
      CUSTOM_TEST_CONF(17);
      CUSTOM_ITEM_CONF(17);
    #endif
    #if HAS_CUSTOM_ITEM_CONF(18)
      CUSTOM_TEST_CONF(18);
      CUSTOM_ITEM_CONF(18);
    #endif
    #if HAS_CUSTOM_ITEM_CONF(19)
      CUSTOM_TEST_CONF(19);
      CUSTOM_ITEM_CONF(19);
    #endif
    #if HAS_CUSTOM_ITEM_CONF(20)
      CUSTOM_TEST_CONF(20);
      CUSTOM_ITEM_CONF(20);
    #endif
    #if HAS_CUSTOM_ITEM_CONF(21)
      CUSTOM_TEST_CONF(21);
      CUSTOM_ITEM_CONF(21);
    #endif
    #if HAS_CUSTOM_ITEM_CONF(22)
      CUSTOM_TEST_CONF(22);
      CUSTOM_ITEM_CONF(22);
    #endif
    #if HAS_CUSTOM_ITEM_CONF(23)
      CUSTOM_TEST_CONF(23);
      CUSTOM_ITEM_CONF(23);
    #endif
    #if HAS_CUSTOM_ITEM_CONF(24)
      CUSTOM_TEST_CONF(24);
      CUSTOM_ITEM_CONF(24);
    #endif
    #if HAS_CUSTOM_ITEM_CONF(25)
      CUSTOM_TEST_CONF(25);
      CUSTOM_ITEM_CONF(25);
    #endif
    END_MENU();
  }

#endif

#endif // HAS_LCD_MENU && (CUSTOM_MENUS_MAIN || CUSTOM_MENUS_CONFIGURATION)
