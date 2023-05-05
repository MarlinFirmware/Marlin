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

#if ENABLED(ONE_CLICK_PRINT)

#include "menu_item.h"
#include "../marlinui.h"

extern char ocp_newest_file[];

inline void one_click_print_start_selected_file() {
  card.openAndPrintFile(ocp_newest_file);
  ui.return_to_status();
  ui.reset_status();
}

void one_click_print() {
  ui.defer_status_screen();

  MediaFile *diveDir;
  const char * const fname = card.diveToFile(true, diveDir, ocp_newest_file);
  card.selectFileByName(fname);

  START_MENU();

  STATIC_ITEM_F(GET_TEXT_F(MSG_ONE_CLICK_PRINT), SS_CENTER|SS_INVERT);
  STATIC_ITEM_F(FPSTR(NUL_STR), SS_CENTER, card.longFilename[0] ? card.longFilename : card.filename);

  #if HAS_MARLINUI_U8GLIB
    STATIC_ITEM_F(FPSTR(NUL_STR), SS_CENTER, "");
  #endif

  ACTION_ITEM(MSG_BUTTON_CONFIRM, one_click_print_start_selected_file);
  BACK_ITEM(MSG_BUTTON_CANCEL);

  END_MENU();
}

#endif // ONE_CLICK_PRINT
