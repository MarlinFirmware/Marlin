/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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
// Configuration Menu
//

#include "../../inc/MarlinConfigPre.h"

#if HAS_LCD_MENU

  #include "menu.h"
  #include "../../module/configuration_store.h"

  void menu_advanced_settings();
  static void lcd_factory_settings() {
    settings.reset();
    ui.completion_feedback();
  }

  #if !DISABLED(SLIM_LCD_MENUS)
    void menu_configuration(const uint8_t material) {
      #if HOTENDS > 5
        #define MINTEMP_ALL MIN(HEATER_0_MINTEMP, HEATER_1_MINTEMP, HEATER_2_MINTEMP, HEATER_3_MINTEMP, HEATER_4_MINTEMP, HEATER_5_MINTEMP)
        #define MAXTEMP_ALL MAX(HEATER_0_MAXTEMP, HEATER_1_MAXTEMP, HEATER_2_MAXTEMP, HEATER_3_MAXTEMP, HEATER_4_MAXTEMP, HEATER_5_MAXTEMP)
      #elif HOTENDS > 4
        #define MINTEMP_ALL MIN(HEATER_0_MINTEMP, HEATER_1_MINTEMP, HEATER_2_MINTEMP, HEATER_3_MINTEMP, HEATER_4_MINTEMP)
        #define MAXTEMP_ALL MAX(HEATER_0_MAXTEMP, HEATER_1_MAXTEMP, HEATER_2_MAXTEMP, HEATER_3_MAXTEMP, HEATER_4_MAXTEMP)
      #elif HOTENDS > 3
        #define MINTEMP_ALL MIN(HEATER_0_MINTEMP, HEATER_1_MINTEMP, HEATER_2_MINTEMP, HEATER_3_MINTEMP)
        #define MAXTEMP_ALL MAX(HEATER_0_MAXTEMP, HEATER_1_MAXTEMP, HEATER_2_MAXTEMP, HEATER_3_MAXTEMP)
      #elif HOTENDS > 2
        #define MINTEMP_ALL MIN(HEATER_0_MINTEMP, HEATER_1_MINTEMP, HEATER_2_MINTEMP)
        #define MAXTEMP_ALL MAX(HEATER_0_MAXTEMP, HEATER_1_MAXTEMP, HEATER_2_MAXTEMP)
      #elif HOTENDS > 1
        #define MINTEMP_ALL MIN(HEATER_0_MINTEMP, HEATER_1_MINTEMP)
        #define MAXTEMP_ALL MAX(HEATER_0_MAXTEMP, HEATER_1_MAXTEMP)
      #else
        #define MINTEMP_ALL HEATER_0_MINTEMP
        #define MAXTEMP_ALL HEATER_0_MAXTEMP
      #endif

      START_MENU();
      MENU_BACK(MSG_MAIN);

      // Debug Menu when certain options are enabled
      #if HAS_DEBUG_MENU
        MENU_ITEM(submenu, MSG_DEBUG_MENU, menu_debug);
      #endif

      MENU_ITEM(submenu, MSG_ADVANCED_SETTINGS, menu_advanced_settings);
  
      MENU_ITEM_EDIT(int8, MSG_FAN_SPEED, &ui.preheat_fan_speed[material], 0, 255);
      #if HAS_TEMP_HOTEND
        MENU_ITEM_EDIT(int3, MSG_NOZZLE, &ui.preheat_hotend_temp[material], MINTEMP_ALL, MAXTEMP_ALL - 15);
      #endif
      #if HAS_HEATED_BED
        MENU_ITEM_EDIT(int3, MSG_BED, &ui.preheat_bed_temp[material], BED_MINTEMP, BED_MAXTEMP - 15);
      #endif
      #if ENABLED(EEPROM_SETTINGS)
        MENU_ITEM(function, MSG_STORE_EEPROM, lcd_store_settings);
      #endif

      const bool busy = printer_busy();
      if (!busy) {
      MENU_ITEM(function, MSG_RESTORE_FAILSAFE, lcd_factory_settings);
      }
      END_MENU();
    }

    void menu_preheat_material1_settings() { menu_configuration(0); }
    void menu_preheat_material2_settings() { menu_configuration(1); }
  #endif

#endif // HAS_LCD_MENU
