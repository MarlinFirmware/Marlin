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
// Level Bed Corners menu
//

#include "../../inc/MarlinConfigPre.h"

#if HAS_LCD_MENU && ENABLED(LEVEL_BED_CORNERS)

#include "menu.h"
#include "../../module/motion.h"
#include "../../module/planner.h"

#if HAS_LEVELING
  #include "../../feature/bedlevel/bedlevel.h"
#endif

#ifndef LEVEL_CORNERS_Z_HOP
  #define LEVEL_CORNERS_Z_HOP 4.0
#endif

#ifndef LEVEL_CORNERS_HEIGHT
  #define LEVEL_CORNERS_HEIGHT 0.0
#endif

static_assert(LEVEL_CORNERS_Z_HOP >= 0, "LEVEL_CORNERS_Z_HOP must be >= 0. Please update your configuration.");

#if HAS_LEVELING
  static bool leveling_was_active = false;
#endif

static inline void _lcd_level_bed_corners_back() {
  #if HAS_LEVELING
    set_bed_leveling_enabled(leveling_was_active);
  #endif
  ui.goto_previous_screen_no_defer();
}

/**
 * Level corners, starting in the front-left corner.
 */
static int8_t bed_corner;
static inline void _lcd_goto_next_corner() {
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
  line_to_z(LEVEL_CORNERS_HEIGHT);
  if (++bed_corner > 3
    #if ENABLED(LEVEL_CENTER_TOO)
      + 1
    #endif
  ) bed_corner = 0;
}

static inline void menu_level_bed_corners() {
  START_MENU();
  MENU_ITEM(function,
    #if ENABLED(LEVEL_CENTER_TOO)
      MSG_LEVEL_BED_NEXT_POINT
    #else
      MSG_NEXT_CORNER
    #endif
    , _lcd_goto_next_corner
  );
  MENU_ITEM(function, MSG_BACK, _lcd_level_bed_corners_back);
  END_MENU();
}

static inline void _lcd_level_bed_corners_homing() {
  _lcd_draw_homing();
  if (all_axes_homed()) {
    bed_corner = 0;
    ui.goto_screen(menu_level_bed_corners);
    _lcd_goto_next_corner();
  }
}

void _lcd_level_bed_corners() {
  ui.defer_status_screen();
  if (!all_axes_known()) {
    set_all_unhomed();
    enqueue_and_echo_commands_P(PSTR("G28"));
  }

  // Disable leveling so the planner won't mess with us
  #if HAS_LEVELING
    leveling_was_active = planner.leveling_active;
    set_bed_leveling_enabled(false);
  #endif

  ui.goto_screen(_lcd_level_bed_corners_homing);
}

#endif // HAS_LCD_MENU && LEVEL_BED_CORNERS
