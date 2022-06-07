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

#include "../../inc/MarlinConfig.h"

#if ENABLED(TOUCH_SCREEN_CALIBRATION)

#include "../gcode.h"

#if HAS_TFT_LVGL_UI
  #include "../../lcd/extui/mks_ui/draw_touch_calibration.h"
#else
  #include "../../lcd/menu/menu.h"
#endif

/**
 * M995: Touch screen calibration for TFT display
 */
void GcodeSuite::M995() {

  #if HAS_TFT_LVGL_UI
    lv_draw_touch_calibration_screen();
  #else
    ui.goto_screen(touch_screen_calibration);
  #endif

}

#endif // TOUCH_SCREEN_CALIBRATION
