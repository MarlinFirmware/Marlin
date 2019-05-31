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
// Tune Menu
//

#include "../../inc/MarlinConfigPre.h"

#if HAS_LCD_MENU && ENABLED(POWER_MONITOR)
  #include "menu.h"
  #include "../../module/temperature.h"
  #include "../../Marlin.h"

  #include "../../feature/power_monitor.h"
  #include "../lcdprint.h"
  #if HAS_GRAPHICAL_LCD
    #include "../dogm/ultralcd_DOGM.h"
  #endif

  void menu_power_monitor() {
    START_MENU();
    MENU_BACK(MSG_MAIN);

    #if HAS_POWER_MONITOR_CURRENT_SENSOR
      // current LCD display: On/Off
      MENU_ITEM_EDIT(bool, MSG_CURRENT, &power_monitor.current_display_enabled);
    #endif

    #if HAS_POWER_MONITOR_VOLTAGE_SENSOR
    // voltage LCD display: On/Off
      MENU_ITEM_EDIT(bool, MSG_VOLTAGE, &power_monitor.voltage_display_enabled);
    #endif

    END_MENU();
  }
#endif
