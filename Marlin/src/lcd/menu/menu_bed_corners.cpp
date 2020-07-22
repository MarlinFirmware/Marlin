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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

//
// Level Bed Corners menu
//

#include "../../inc/MarlinConfigPre.h"

#if BOTH(HAS_LCD_MENU, LEVEL_BED_CORNERS)

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

/**
 * Level corners, starting in the front-left corner.
 */
static int8_t bed_corner;
static inline void _lcd_goto_next_corner() {
  constexpr float lfrb[4] = LEVEL_CORNERS_INSET_LFRB;
  constexpr xy_pos_t lf { (X_MIN_BED) + lfrb[0], (Y_MIN_BED) + lfrb[1] },
                     rb { (X_MAX_BED) - lfrb[2], (Y_MAX_BED) - lfrb[3] };
  line_to_z(LEVEL_CORNERS_Z_HOP);
  switch (bed_corner) {
    case 0: current_position   = lf;   break; // copy xy
    case 1: current_position.x = rb.x; break;
    case 2: current_position.y = rb.y; break;
    case 3: current_position.x = lf.x; break;
    #if ENABLED(LEVEL_CENTER_TOO)
      case 4: current_position.set(X_CENTER, Y_CENTER); break;
    #endif
  }
  line_to_current_position(manual_feedrate_mm_s.x);
  line_to_z(LEVEL_CORNERS_HEIGHT);
  if (++bed_corner > 3 + ENABLED(LEVEL_CENTER_TOO)) bed_corner = 0;
}

static inline void _lcd_level_bed_corners_homing() {
  _lcd_draw_homing();
  if (all_axes_homed()) {
    bed_corner = 0;
    ui.goto_screen([]{
      MenuItem_confirm::select_screen(
          GET_TEXT(MSG_BUTTON_NEXT), GET_TEXT(MSG_BUTTON_DONE)
        , _lcd_goto_next_corner
        , []{
            TERN_(HAS_LEVELING, set_bed_leveling_enabled(leveling_was_active));
            ui.goto_previous_screen_no_defer();
          }
        , GET_TEXT(TERN(LEVEL_CENTER_TOO, MSG_LEVEL_BED_NEXT_POINT, MSG_NEXT_CORNER))
        , (const char*)nullptr, PSTR("?")
      );
    });
    ui.set_selection(true);
    _lcd_goto_next_corner();
  }
}

void _lcd_level_bed_corners() {
  ui.defer_status_screen();
  if (!all_axes_known()) {
    set_all_unhomed();
    queue.inject_P(G28_STR);
  }

  // Disable leveling so the planner won't mess with us
  #if HAS_LEVELING
    leveling_was_active = planner.leveling_active;
    set_bed_leveling_enabled(false);
  #endif

  ui.goto_screen(_lcd_level_bed_corners_homing);
}

#endif // HAS_LCD_MENU && LEVEL_BED_CORNERS
