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
// Power Monitor Menu
//

#include "../../inc/MarlinConfigPre.h"

#if HAS_LCD_MENU && HAS_POWER_MONITOR

#include "menu_item.h"
#include "../../feature/power_monitor.h"

void menu_power_monitor() {
  START_MENU();
  BACK_ITEM(MSG_MAIN);

  #if ENABLED(POWER_MONITOR_CURRENT)
  {
    bool ena = power_monitor.current_display_enabled();
    EDIT_ITEM(bool, MSG_CURRENT, &ena, power_monitor.toggle_current_display);
  }
  #endif

  #if ENABLED(POWER_MONITOR_VOLTAGE)
  {
    bool ena = power_monitor.voltage_display_enabled();
    EDIT_ITEM(bool, MSG_VOLTAGE, &ena, power_monitor.toggle_voltage_display);
  }
  #endif

  #if HAS_POWER_MONITOR_WATTS
  {
    bool ena = power_monitor.power_display_enabled();
    EDIT_ITEM(bool, MSG_POWER, &ena, power_monitor.toggle_power_display);
  }
  #endif

  END_MENU();
}

#endif // HAS_LCD_MENU && HAS_POWER_MONITOR
