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
// Level Bed Corners menu
//

#include "../../inc/MarlinConfigPre.h"

#if HAS_LCD_MENU && ENABLED(LEVEL_BED_CORNERS)

#include "menu.h"
#include "../../module/motion.h"
#include "../../module/planner.h"

#ifndef LEVEL_CORNERS_Z_HOP
  #define LEVEL_CORNERS_Z_HOP 4.0
#endif

static_assert(LEVEL_CORNERS_Z_HOP >= 0, "LEVEL_CORNERS_Z_HOP must be >= 0. Please update your configuration.");

/**
 * Level corners, starting in the front-left corner.
 */
static int8_t bed_corner;
void _lcd_goto_next_corner() {
  line_to_z(LEVEL_CORNERS_Z_HOP);
  switch (bed_corner) {
    case 0:
      current_position[X_AXIS] = X_MIN_BED + LEVEL_CORNERS_INSET;
      current_position[Y_AXIS] = Y_MIN_BED + LEVEL_CORNERS_INSET;
      break;
    case 1:
      current_position[X_AXIS] = X_MAX_BED - LEVEL_CORNERS_INSET;
      break;
    case 2:
      current_position[Y_AXIS] = Y_MAX_BED - LEVEL_CORNERS_INSET;
      break;
    case 3:
      current_position[X_AXIS] = X_MIN_BED + LEVEL_CORNERS_INSET;
      break;
    #if ENABLED(LEVEL_CENTER_TOO)
      case 4:
        current_position[X_AXIS] = X_CENTER;
        current_position[Y_AXIS] = Y_CENTER;
        break;
    #endif
  }
  planner.buffer_line(current_position, MMM_TO_MMS(manual_feedrate_mm_m[X_AXIS]), active_extruder);
  line_to_z(0.0);
  if (++bed_corner > 3
    #if ENABLED(LEVEL_CENTER_TOO)
      + 1
    #endif
  ) bed_corner = 0;
}

void menu_level_bed_corners() {
  START_MENU();
  MENU_ITEM(function,
    #if ENABLED(LEVEL_CENTER_TOO)
      MSG_LEVEL_BED_NEXT_POINT
    #else
      MSG_NEXT_CORNER
    #endif
    , _lcd_goto_next_corner);
  MENU_ITEM(function, MSG_BACK, ui.goto_previous_screen_no_defer);
  END_MENU();
}

void _lcd_level_bed_corners_homing() {
  _lcd_draw_homing();
  if (all_axes_homed()) {
    bed_corner = 0;
    ui.goto_screen(menu_level_bed_corners);
    _lcd_goto_next_corner();
  }
}

void _lcd_level_bed_corners() {
  ui.defer_status_screen(true);
  if (!all_axes_known()) {
    set_all_unhomed();
    enqueue_and_echo_commands_P(PSTR("G28"));
  }
  ui.goto_screen(_lcd_level_bed_corners_homing);
}

#endif // HAS_LCD_MENU && LEVEL_BED_CORNERS
