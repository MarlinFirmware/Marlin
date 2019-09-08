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
// Cancel Object Menu
//

#include "../../inc/MarlinConfigPre.h"

#if HAS_LCD_MENU && ENABLED(CANCEL_OBJECTS)

#include "menu.h"

#include "../../feature/cancel_object.h"

//
// TODO: Select the active object
// upon entry to the menu and present
// a confirmation screen.
//
void menu_cancelobject() {
  START_MENU();
  MENU_BACK(MSG_MAIN);

  GCODES_ITEM(MSG_OBJECT_CANCEL, PSTR("M486 C"));

  // Draw cancelable items in a loop
  for (int8_t i = 0; i < cancelable.object_count; i++) {
    if (!TEST(cancelable.canceled, i)) {
      editable.int8 = i;
      ACTION_ITEM(MSG_OBJECT_CANCEL, [](){
        cancelable.cancel_object(editable.int8);
        ui.quick_feedback();
      });
      MENU_ITEM_ADDON_START(LCD_WIDTH - 2 - (i >= 10));
        lcd_put_int(i);
      MENU_ITEM_ADDON_END();
    }
  }

  /*
  MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(int3, MSG_OBJECT_CANCEL, &editable.int8, -1, 32, [](){
    if (editable.int8 > -1) {
      cancelable.cancel_object(editable.int8);
      ui.quick_feedback();
      editable.int8 = -1;
    }
  });
  */

  END_MENU();
}

#endif // HAS_LCD_MENU && CANCEL_OBJECTS
