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
// Backlash Menu
//

#include "../../inc/MarlinConfigPre.h"

#if HAS_LCD_MENU && ENABLED(BACKLASH_GCODE)

#include "menu.h"

#include "../../feature/backlash.h"

void menu_backlash() {
  START_MENU();
  MENU_BACK(MSG_MAIN);

  MENU_MULTIPLIER_ITEM_EDIT(percent, MSG_BACKLASH_CORRECTION, &backlash.correction, all_off, all_on);

  #define EDIT_BACKLASH_DISTANCE(N) MENU_MULTIPLIER_ITEM_EDIT(float43, MSG_##N, &backlash.distance_mm[_AXIS(N)], 0.0f, 9.9f);
  EDIT_BACKLASH_DISTANCE(A);
  EDIT_BACKLASH_DISTANCE(B);
  EDIT_BACKLASH_DISTANCE(C);

  #ifdef BACKLASH_SMOOTHING_MM
    MENU_MULTIPLIER_ITEM_EDIT(float43, MSG_BACKLASH_SMOOTHING, &backlash.smoothing_mm, 0.0f, 9.9f);
  #endif

  END_MENU();
}

#endif // HAS_LCD_MENU && BACKLASH_COMPENSATION
