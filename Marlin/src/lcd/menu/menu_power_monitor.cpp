/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
// Power Monitor Menu
//

#include "../../inc/MarlinConfigPre.h"

#if HAS_LCD_MENU && EITHER(POWER_MONITOR_CURRENT, POWER_MONITOR_VOLTAGE)

#include "menu.h"
#include "../../feature/power_monitor.h"

void menu_power_monitor() {
  START_MENU();
  MENU_BACK(MSG_MAIN);

  #if ENABLED(POWER_MONITOR_CURRENT)
  {
    bool ena = power_monitor.current_display_enabled();
    MENU_ITEM_EDIT_CALLBACK(bool, MSG_CURRENT, &ena, power_monitor.toggle_current_display_enabled);
  }
  #endif

  #if ENABLED(POWER_MONITOR_VOLTAGE) || defined(POWER_MONITOR_FIXED_VOLTAGE)
  {
    bool ena = power_monitor.voltage_display_enabled();
    MENU_ITEM_EDIT_CALLBACK(bool, MSG_VOLTAGE, &ena, power_monitor.toggle_voltage_display_enabled);
  }
  #endif

  #if ENABLED(POWER_MONITOR_CURRENT) && (ENABLED(POWER_MONITOR_VOLTAGE) || defined(POWER_MONITOR_FIXED_VOLTAGE))
  {
    bool ena = power_monitor.power_display_enabled();
    MENU_ITEM_EDIT_CALLBACK(bool, MSG_POWER, &ena, power_monitor.toggle_power_display_enabled);
  }
  #endif

  END_MENU();
}

#endif // HAS_LCD_MENU && HAS_POWER_MONITOR
