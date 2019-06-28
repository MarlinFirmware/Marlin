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
// Spindle / Laser Menu
//

#include "../../inc/MarlinConfig.h"

#if HAS_CUTTER

  #include "menu.h"

  #include "../../feature/spindle_laser.h"

  void menu_spindle_laser() {

    START_MENU();
    MENU_BACK(MSG_MAIN);
    if (cutter.enabled()) {
      #if ENABLED(SPINDLE_LASER_PWM)
        MENU_ITEM_EDIT_CALLBACK(CUTTER_MENU_TYPE, MSG_CUTTER(POWER), &cutter.power, SPEED_POWER_MIN, SPEED_POWER_MAX, cutter.update_output);
      #endif
      MENU_ITEM(function, MSG_CUTTER(OFF), cutter.disable);
    }
    else {
      MENU_ITEM(function, MSG_CUTTER(ON), cutter.enable_forward);
      #if ENABLED(SPINDLE_CHANGE_DIR)
        MENU_ITEM(function, MSG_SPINDLE_REVERSE, cutter.enable_reverse);
      #endif
    }
    END_MENU();
  }

#endif // HAS_CUTTER
