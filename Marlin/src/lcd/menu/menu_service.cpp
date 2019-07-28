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

#if HAS_LCD_MENU && HAS_SERVICE_INTERVALS && ENABLED(PRINTCOUNTER)

#include "menu.h"
#include "../../module/printcounter.h"

inline void _menu_service(const int index, PGM_P const name) {
  char sram[30];
  strncpy_P(sram, name, 29);
  do_select_screen(
    PSTR(MSG_BUTTON_RESET), PSTR(MSG_BUTTON_CANCEL),
    []{
      print_job_timer.resetServiceInterval(index);
      ui.completion_feedback(true);
      ui.reset_status();
      ui.return_to_status();
    },
    ui.goto_previous_screen,
    PSTR(MSG_SERVICE_RESET), sram, PSTR("?")
  );
}

#if SERVICE_INTERVAL_1 > 0
  void menu_service1() { _menu_service(1, PSTR(SERVICE_NAME_1)); }
#endif

#if SERVICE_INTERVAL_2 > 0
  void menu_service2() { _menu_service(2, PSTR(SERVICE_NAME_2)); }
#endif

#if SERVICE_INTERVAL_3 > 0
  void menu_service3() { _menu_service(3, PSTR(SERVICE_NAME_3)); }
#endif

#endif // HAS_LCD_MENU && HAS_SERVICE_INTERVALS && PRINTCOUNTER
