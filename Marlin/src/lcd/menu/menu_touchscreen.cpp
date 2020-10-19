/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../inc/MarlinConfig.h"

#if HAS_LCD_MENU

#include "menu_item.h"
#include "menu_addon.h"

#if ENABLED(TOUCH_CALIBRATION)

#include "../../feature/touch/calibration.h"

TouchCalibration calibration;

void menu_touchscreen() {
  START_MENU();
  BACK_ITEM(MSG_MAIN);
  MENU_ITEM(submenu, MSG_CALIBRATION, enter_touch_calibrate);
#ifdef TS_V11
  STATIC_ITEM_P("ILI9341 v1.1", SS_LEFT);
#elif defined(TS_V12)
  STATIC_ITEM_P("ILI9341 v1.2", SS_LEFT);
#endif
  {
    char line[20];
    snprintf(line, 20, "X: %hd %hd", calibration.results[0], calibration.results[1]);
    STATIC_ITEM_P(line, SS_LEFT);
    snprintf(line, 20, "Y: %hd %hd", calibration.results[2], calibration.results[3]);
    STATIC_ITEM_P(line, SS_LEFT);
  }
  END_MENU();
}

void enter_touch_calibrate() {
  calibration.init_calibration(1);
}

#endif // TOUCH_CALIBRATION

#endif // HAS_LCD_MENU
