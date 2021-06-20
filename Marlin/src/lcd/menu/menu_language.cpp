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
// Language Selection Menu
//

#include "../../inc/MarlinConfig.h"

#if HAS_MULTI_LANGUAGE

#include "menu_item.h"
#include "../../MarlinCore.h"
#include "../../module/settings.h"

static void set_lcd_language(const uint8_t inlang) {
  ui.set_language(inlang);
  TERN_(LCD_LANGUAGE_AUTO_SAVE, (void)settings.save());
}

void menu_language() {
  START_MENU();
  BACK_ITEM(MSG_MAIN);

  MENU_ITEM_P(function, GET_LANG(LCD_LANGUAGE  )::LANGUAGE, []{ set_lcd_language(0); });
  MENU_ITEM_P(function, GET_LANG(LCD_LANGUAGE_2)::LANGUAGE, []{ set_lcd_language(1); });
  #if NUM_LANGUAGES > 2
    MENU_ITEM_P(function, GET_LANG(LCD_LANGUAGE_3)::LANGUAGE, []{ set_lcd_language(2); });
    #if NUM_LANGUAGES > 3
      MENU_ITEM_P(function, GET_LANG(LCD_LANGUAGE_4)::LANGUAGE, []{ set_lcd_language(3); });
      #if NUM_LANGUAGES > 4
        MENU_ITEM_P(function, GET_LANG(LCD_LANGUAGE_5)::LANGUAGE, []{ set_lcd_language(4); });
      #endif
    #endif
  #endif

  END_MENU();
}

#endif // HAS_MULTI_LANGUAGE
