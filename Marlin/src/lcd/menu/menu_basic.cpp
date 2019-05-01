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

// Prepare Printer menu

#include "../../inc/MarlinConfigPre.h"

#if HAS_LCD_MENU

#include "menu.h"
#include "../../module/motion.h"
#include "../../module/temperature.h"

void menu_preheat_m1();
void menu_level_bed_corners();
void menu_temp_e0_filament_change();
void menu_change_filament();

void menu_basic() {
  START_MENU();
  MENU_BACK(MSG_MAIN);

  MENU_ITEM(submenu, MSG_PREHEAT_1, menu_preheat_m1);
  MENU_ITEM(submenu, MSG_BED_LEVELING, menu_level_bed_corners);

  #if ENABLED(ADVANCED_PAUSE_FEATURE)
    #if E_STEPPERS == 1 && DISABLED(FILAMENT_LOAD_UNLOAD_GCODES)
      if (thermalManager.targetHotEnoughToExtrude(active_extruder))
        MENU_ITEM(gcode, MSG_FILAMENTCHANGE, PSTR("M600 B0"));
      else
        MENU_ITEM(submenu, MSG_FILAMENTCHANGE, menu_temp_e0_filament_change);
    #else
      MENU_ITEM(submenu, MSG_FILAMENTCHANGE, menu_change_filament);
    #endif
  #endif

  END_MENU();
}

#endif 