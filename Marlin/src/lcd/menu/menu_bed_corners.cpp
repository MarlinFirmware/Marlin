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
    #define LEVEL_CORNERS_PROBE_TOLERANCE 0.2
  #endif
  float last_z;
  int good_points;
  bool corner_probing_done, wait_for_probe;
#endif

static_assert(LEVEL_CORNERS_Z_HOP >= 0, "LEVEL_CORNERS_Z_HOP must be >= 0. Please update your configuration.");

extern const char G28_STR[];

#if HAS_LEVELING
  static bool leveling_was_active = false;
#endif

static int8_t bed_corner;

constexpr float inset_lfrb[4] = LEVEL_CORNERS_INSET_LFRB;
constexpr xy_pos_t lf { (X_MIN_BED) + inset_lfrb[0], (Y_MIN_BED) + inset_lfrb[1] },
                   rb { (X_MAX_BED) - inset_lfrb[2], (Y_MAX_BED) - inset_lfrb[3] };

/**
 * Level corners, starting in the front-left corner.
 */
#if ENABLED(LEVEL_CORNERS_USE_PROBE)

  #define VALIDATE_POINT(X, Y, STR) static_assert(Probe::build_time::can_reach((X), (Y)), \
    "LEVEL_CORNERS_INSET_LFRB " STR " inset is not reachable with the default NOZZLE_TO_PROBE offset and PROBING_MARGIN.")
  VALIDATE_POINT(lf.x, Y_CENTER, "left"); VALIDATE_POINT(X_CENTER, lf.y, "front");
  VALIDATE_POINT(rb.x, Y_CENTER, "right"); VALIDATE_POINT(X_CENTER, rb.y, "back");

  void _lcd_draw_probing() {
    if (ui.should_draw()) MenuItem_static::draw((LCD_HEIGHT - 1) / 2, GET_TEXT(MSG_PROBING_MESH));
  }

  void _lcd_draw_raise() {
    if (!ui.should_draw()) return;
    MenuItem_confirm::select_screen(
      GET_TEXT(MSG_BUTTON_DONE), GET_TEXT(MSG_BUTTON_SKIP)
      , []{ corner_probing_done = true; wait_for_probe = false; }
      , []{ wait_for_probe = false; }
      , GET_TEXT(MSG_LEVEL_CORNERS_RAISE)
      , (const char*)nullptr, PSTR("")
    );
  }

  void _lcd_draw_level_prompt() {
    if (!ui.should_draw()) return;
    MenuItem_confirm::confirm_screen(
      []{ queue.inject_P(TERN(HAS_LEVELING, PSTR("G28\nG29"), G28_STR));
          ui.return_to_status();
      }
      , []{ ui.goto_previous_screen_no_defer(); }
      , GET_TEXT(MSG_LEVEL_CORNERS_IN_RANGE)
      , (const char*)nullptr, PSTR("?")
    );
  }

  bool _lcd_level_bed_corners_probe(bool verify=false) {
    if (verify) do_blocking_move_to_z(current_position.z + LEVEL_CORNERS_Z_HOP); // do clearance if needed
    TERN_(BLTOUCH_SLOW_MODE, bltouch.deploy()); // Deploy in LOW SPEED MODE on every probe action
    do_blocking_move_to_z(last_z - LEVEL_CORNERS_PROBE_TOLERANCE, manual_feedrate_mm_s.z); // Move down to lower tolerance
    if (TEST(endstops.trigger_state(), TERN(Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN, Z_MIN, Z_MIN_PROBE))) { // check if probe triggered
      endstops.hit_on_purpose();
      set_current_from_steppers_for_axis(Z_AXIS);
      sync_plan_position();
      TERN_(BLTOUCH_SLOW_MODE, bltouch.stow()); // Stow in LOW SPEED MODE on every trigger
      // Triggered outside tolerance range?
      if (ABS(current_position.z - last_z) > LEVEL_CORNERS_PROBE_TOLERANCE) {
        last_z = current_position.z; // Above tolerance. Set a new Z for subsequent corners.
        good_points = 0;             // ...and start over
      }
      return true; // probe triggered
    }
    do_blocking_move_to_z(last_z); // go back to tolerance middle point before raise
    return false; // probe not triggered
  }

  bool _lcd_level_bed_corners_raise() {
    bool probe_triggered = false;
    corner_probing_done = false;
    wait_for_probe = true;
    ui.goto_screen(_lcd_draw_raise); // show raise screen
    ui.set_selection(true);
    while (wait_for_probe && !probe_triggered) { //loop while waiting to bed raise and probe trigger
      probe_triggered = PROBE_TRIGGERED();
      if (probe_triggered) {
        endstops.hit_on_purpose();
        TERN_(LEVEL_CORNERS_AUDIO_FEEDBACK, ui.buzz(200, 600));
      }
      idle();
    }
    TERN_(BLTOUCH_SLOW_MODE, bltouch.stow());
    ui.goto_screen(_lcd_draw_probing);
    return (probe_triggered);
  }

  void _lcd_test_corners() {
    ui.goto_screen(_lcd_draw_probing);
    bed_corner = TERN(LEVEL_CENTER_TOO, 4, 0);
    last_z = LEVEL_CORNERS_HEIGHT;
    endstops.enable_z_probe(true);
    good_points = 0;

    do {
      do_blocking_move_to_z(current_position.z + LEVEL_CORNERS_Z_HOP); // clearance
      // Select next corner coordinates
      xy_pos_t plf = lf - probe.offset_xy, prb = rb - probe.offset_xy;
      switch (bed_corner) {
        case 0: current_position   = plf;   break; // copy xy
        case 1: current_position.x = prb.x; break;
        case 2: current_position.y = prb.y; break;
        case 3: current_position.x = plf.x; break;
        #if ENABLED(LEVEL_CENTER_TOO)
          case 4: current_position.set(X_CENTER - probe.offset_xy.x, Y_CENTER - probe.offset_xy.y); break;
        #endif
      }
      do_blocking_move_to_xy(current_position);           // Goto corner

      if (!_lcd_level_bed_corners_probe()) {              // Probe down to tolerance
        if (_lcd_level_bed_corners_raise()) {             // Prompt user to raise bed if needed
          #if ENABLED(LEVEL_CORNERS_VERIFY_RAISED)        // Verify
            while (!_lcd_level_bed_corners_probe(true)) { // Loop while corner verified
              if (!_lcd_level_bed_corners_raise()) {      // Prompt user to raise bed if needed
                if (corner_probing_done) return;          // Done was selected
                break;                                    // Skip was selected
              }
            }
          #endif
        }
        else if (corner_probing_done)                     // Done was selected
          return;
      }

      if (bed_corner != 4) good_points++; // ignore center
      if (++bed_corner > 3) bed_corner = 0;

    } while (good_points < 4); // loop until all corners whitin tolerance

    ui.goto_screen(_lcd_draw_level_prompt); // prompt for bed leveling
    ui.set_selection(true);
  }

#else // !LEVEL_CORNERS_USE_PROBE

  static inline void _lcd_goto_next_corner() {
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

#endif // !LEVEL_CORNERS_USE_PROBE

static inline void _lcd_level_bed_corners_homing() {
  _lcd_draw_homing();
  if (!all_axes_homed()) return;
  #if ENABLED(LEVEL_CORNERS_USE_PROBE)
    _lcd_test_corners();
    if (corner_probing_done) ui.goto_previous_screen_no_defer();
    TERN_(HAS_LEVELING, set_bed_leveling_enabled(leveling_was_active));
    endstops.enable_z_probe(false);
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

void _lcd_level_bed_corners() {
  ui.defer_status_screen();
  if (!all_axes_trusted()) {
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
