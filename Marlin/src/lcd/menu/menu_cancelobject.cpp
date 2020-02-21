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
// Cancel Object Menu
//

#include "../../inc/MarlinConfigPre.h"

#if HAS_LCD_MENU && ENABLED(CANCEL_OBJECTS)

#include "menu.h"
#include "menu_addon.h"

#include "../../feature/cancel_object.h"

static void lcd_cancel_object_confirm() {
  const int8_t v = MenuItemBase::itemIndex;
  const char item_num[] = {
    ' ',
    char((v > 9) ? '0' + (v / 10) : ' '),
    char('0' + (v % 10)),
    '\0'
  };
  MenuItem_confirm::confirm_screen(
    []{
      cancelable.cancel_object(MenuItemBase::itemIndex - 1);
      #if HAS_BUZZER
        ui.completion_feedback();
      #endif
      ui.goto_previous_screen();
    },
    ui.goto_previous_screen,
    GET_TEXT(MSG_CANCEL_OBJECT), item_num, PSTR("?")
  );
}

void menu_cancelobject() {
  START_MENU();
  BACK_ITEM(MSG_MAIN);

  // Draw cancelable items in a loop
  int8_t a = cancelable.active_object;
  for (int8_t i = -1; i < cancelable.object_count; i++) {
    if (i == a) continue;
    int8_t j = i < 0 ? a : i;
    if (!cancelable.is_canceled(j))
      SUBMENU_N(j, MSG_CANCEL_OBJECT_N, lcd_cancel_object_confirm);
    if (i < 0) SKIP_ITEM();
  }

  END_MENU();
}

#endif // HAS_LCD_MENU && CANCEL_OBJECTS
