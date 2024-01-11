/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(ONE_CLICK_PRINT) && DISABLED(DWIN_LCD_PROUI)

#include "menu.h"

void one_click_print() {
  ui.goto_screen([]{
    char * const filename = card.longest_filename();
    MenuItem_confirm::select_screen(
      GET_TEXT_F(MSG_BUTTON_PRINT), GET_TEXT_F(MSG_BUTTON_CANCEL),
      []{
        card.openAndPrintFile(card.filename);
        ui.return_to_status();
        ui.reset_status();
      }, nullptr,
      GET_TEXT_F(MSG_START_PRINT), filename, F("?")
    );
  });
}

#endif // ONE_CLICK_PRINT
