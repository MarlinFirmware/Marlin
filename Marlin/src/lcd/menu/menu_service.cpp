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

//
// Service Menus
//

#include "../../inc/MarlinConfigPre.h"

#if HAS_LCD_MENU && HAS_SERVICE_INTERVALS

#include "menu.h"
#include "../../module/printcounter.h"

inline void _service_reset(const int index) {
  print_job_timer.resetServiceInterval(index);
  #if HAS_BUZZER
    ui.completion_feedback();
  #endif
  ui.reset_status();
  ui.return_to_status();
}

#if SERVICE_INTERVAL_1 > 0
  void menu_service1() {
    char sram[30];
    strncpy_P(sram, PSTR(SERVICE_NAME_1), 29);
    do_select_screen(
      GET_TEXT(MSG_BUTTON_RESET), GET_TEXT(MSG_BUTTON_CANCEL),
      []{ _service_reset(1); },
      ui.goto_previous_screen,
      GET_TEXT(MSG_SERVICE_RESET), sram, PSTR("?")
    );
  }
#endif

#if SERVICE_INTERVAL_2 > 0
  void menu_service2() {
    char sram[30];
    strncpy_P(sram, PSTR(SERVICE_NAME_2), 29);
    do_select_screen(
      GET_TEXT(MSG_BUTTON_RESET), GET_TEXT(MSG_BUTTON_CANCEL),
      []{ _service_reset(2); },
      ui.goto_previous_screen,
      GET_TEXT(MSG_SERVICE_RESET), sram, PSTR("?")
    );
  }
#endif

#if SERVICE_INTERVAL_3 > 0
  void menu_service3() {
    char sram[30];
    strncpy_P(sram, PSTR(SERVICE_NAME_3), 29);
    do_select_screen(
      GET_TEXT(MSG_BUTTON_RESET), GET_TEXT(MSG_BUTTON_CANCEL),
      []{ _service_reset(3); },
      ui.goto_previous_screen,
      GET_TEXT(MSG_SERVICE_RESET), sram, PSTR("?")
    );
  }
#endif

#endif // HAS_LCD_MENU && HAS_SERVICE_INTERVALS
