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
// Level Bed Corners menu
//

#include "../../inc/MarlinConfigPre.h"

#if BOTH(HAS_LCD_MENU, LEVEL_BED_CORNERS)

#include "menu_item.h"
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

#if ENABLED(LEVEL_CORNERS_USE_PROBE)
  #include "../../module/probe.h"
  #include "../../module/endstops.h"
  #if ENABLED(BLTOUCH)
    #include "../../feature/bltouch.h"
  #endif
  #ifndef LEVEL_CORNERS_PROBE_TOLERANCE
    #define LEVEL_CORNERS_PROBE_TOLERANCE 0.1
  #endif
  #if ENABLED(LEVEL_CORNERS_AUDIO_FEEDBACK)
    #include "../../libs/buzzer.h"
    #define PROBE_BUZZ() BUZZ(200, 600)
  #else
    #define PROBE_BUZZ() NOOP
  #endif
  static float last_z;
  static bool corner_probing_done;
  static bool verify_corner;
  static int good_points;
#endif

static_assert(LEVEL_CORNERS_Z_HOP >= 0, "LEVEL_CORNERS_Z_HOP must be >= 0. Please update your configuration.");

extern const char G28_STR[];

#if HAS_LEVELING
  static bool leveling_was_active = false;
#endif

static int8_t bed_corner;

/**
 * Level corners, starting in the front-left corner.
 */
#if ENABLED(LEVEL_CORNERS_USE_PROBE)

  static inline void _lcd_level_bed_corners_probing() {
    ui.goto_screen([]{ MenuItem_static::draw((LCD_HEIGHT - 1) / 2, GET_TEXT(MSG_PROBING_MESH)); });

    float lfrb[4] = LEVEL_CORNERS_INSET_LFRB;
    xy_pos_t lf { (X_MIN_BED) + lfrb[0] - probe.offset_xy.x , (Y_MIN_BED) + lfrb[1] - probe.offset_xy.y },
             rb { (X_MAX_BED) - lfrb[2] - probe.offset_xy.x , (Y_MAX_BED) - lfrb[3] - probe.offset_xy.y };

    do_blocking_move_to_z(LEVEL_CORNERS_Z_HOP - probe.offset.z);

    switch (bed_corner) {
      case 0: current_position   = lf;   break; // copy xy
      case 1: current_position.x = rb.x; break;
      case 2: current_position.y = rb.y; break;
      case 3: current_position.x = lf.x; break;
      #if ENABLED(LEVEL_CENTER_TOO)
        case 4: current_position.set(X_CENTER - probe.offset_xy.x, Y_CENTER - probe.offset_xy.y); good_points--; break;
      #endif
    }

    do_blocking_move_to_xy(current_position);

    #if ENABLED(BLTOUCH) && DISABLED(BLTOUCH_HS_MODE)
      bltouch.deploy(); // DEPLOY in LOW SPEED MODE on every probe action
    #endif
    TERN_(QUIET_PROBING, probe.set_probing_paused(true));

    // Move down until the probe is triggered
    do_blocking_move_to_z(last_z - (LEVEL_CORNERS_PROBE_TOLERANCE), manual_feedrate_mm_s.z);

    // Check to see if the probe was triggered
    bool probe_triggered = TEST(endstops.trigger_state(), TERN(Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN, Z_MIN, Z_MIN_PROBE));
    if (!probe_triggered) {

      static bool wait_for_probe;

      ui.goto_screen([]{
        MenuItem_confirm::select_screen(
          GET_TEXT(MSG_BUTTON_DONE), GET_TEXT(MSG_BUTTON_SKIP)
          , []{ corner_probing_done = true;
                wait_for_probe = false;
                TERN_(HAS_LEVELING, set_bed_leveling_enabled(leveling_was_active));
                ui.goto_previous_screen_no_defer();
            }
          , []{ wait_for_probe = false; }
          , GET_TEXT(MSG_LEVEL_CORNERS_RAISE)
          , (const char*)nullptr, PSTR("")
        );
      });
      ui.set_selection(true);

      wait_for_probe = true;
      while (wait_for_probe && !probe_triggered) {
        probe_triggered = PROBE_TRIGGERED();
        if (probe_triggered) PROBE_BUZZ();
        idle();
      }
      wait_for_probe = false;

      TERN_(LEVEL_CORNERS_VERIFY_RAISED, verify_corner = true);
    }

    TERN_(QUIET_PROBING, probe.set_probing_paused(false));

    #if ENABLED(BLTOUCH) && DISABLED(BLTOUCH_HS_MODE)
      bltouch.stow();
    #endif

    if (probe_triggered) {
      endstops.hit_on_purpose();
      if (!WITHIN(current_position.z, last_z - (LEVEL_CORNERS_PROBE_TOLERANCE), last_z + (LEVEL_CORNERS_PROBE_TOLERANCE))) {
        last_z = current_position.z;
        good_points = 0;
      }
      if (!verify_corner) good_points++;
    }

    if (!corner_probing_done) {
      if (!verify_corner) bed_corner++;
      if (bed_corner > 3) bed_corner = 0;
      verify_corner = false;
      if (good_points < 4)
        _lcd_level_bed_corners_probing();
      else {
        ui.goto_screen([]{
          MenuItem_confirm::confirm_screen(
            []{ ui.goto_previous_screen_no_defer();
                queue.inject_P(TERN(HAS_LEVELING, PSTR("G28\nG29"), G28_STR));
              }
            , []{ ui.goto_previous_screen_no_defer(); }
            , GET_TEXT(MSG_LEVEL_CORNERS_IN_RANGE)
            , (const char*)nullptr, PSTR("?")
          );
        });
        ui.set_selection(true);
      }
    }
  }

#else

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

#endif

static inline void _lcd_level_bed_corners_homing() {
  _lcd_draw_homing();
  if (all_axes_homed()) {
    #if ENABLED(LEVEL_CORNERS_USE_PROBE)
      TERN_(LEVEL_CENTER_TOO, bed_corner = 4);
      endstops.enable_z_probe(true);
      ui.goto_screen(_lcd_level_bed_corners_probing);
    #else
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
    #endif
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

  #if ENABLED(LEVEL_CORNERS_USE_PROBE)
    last_z = LEVEL_CORNERS_HEIGHT;
    corner_probing_done = false;
    verify_corner = false;
    good_points = 0;
  #endif

  ui.goto_screen(_lcd_level_bed_corners_homing);
}

#endif // HAS_LCD_MENU && LEVEL_BED_CORNERS
