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
// Bed Tramming menu
//

#include "../../inc/MarlinConfigPre.h"

#if ALL(HAS_MARLINUI_MENU, LCD_BED_TRAMMING)

#include "menu_item.h"
#include "../../module/motion.h"
#include "../../module/planner.h"

#if HAS_LEVELING
  #include "../../feature/bedlevel/bedlevel.h"
#endif

#ifndef BED_TRAMMING_Z_HOP
  #define BED_TRAMMING_Z_HOP 4.0
#endif
#ifndef BED_TRAMMING_HEIGHT
  #define BED_TRAMMING_HEIGHT 0.0
#endif

#if ALL(HAS_STOWABLE_PROBE, BED_TRAMMING_USE_PROBE) && DISABLED(BLTOUCH)
  #define NEEDS_PROBE_DEPLOY 1
#endif

#if ENABLED(BED_TRAMMING_USE_PROBE)
  #include "../../module/probe.h"
  #include "../../module/endstops.h"
  #if ENABLED(BLTOUCH)
    #include "../../feature/bltouch.h"
  #endif
  #ifndef BED_TRAMMING_PROBE_TOLERANCE
    #define BED_TRAMMING_PROBE_TOLERANCE 0.2
  #endif
  float last_z;
  int good_points;
  bool tramming_done, wait_for_probe;

  #if HAS_MARLINUI_U8GLIB
    #include "../dogm/marlinui_DOGM.h"
  #endif
  #define GOOD_POINTS_TO_STR(N) ui8tostr2(N)
  #define LAST_Z_TO_STR(N) ftostr53_63(N) //ftostr42_52(N)

#endif

static_assert(BED_TRAMMING_Z_HOP >= 0, "BED_TRAMMING_Z_HOP must be >= 0. Please update your configuration.");

#define LF 1
#define RF 2
#define RB 3
#define LB 4

#ifndef BED_TRAMMING_LEVELING_ORDER
  #define BED_TRAMMING_LEVELING_ORDER { LF, RF, LB, RB } // Default
  //#define BED_TRAMMING_LEVELING_ORDER { LF, LB, RF  }  // 3 hard-coded points
  //#define BED_TRAMMING_LEVELING_ORDER { LF, RF }       // 3-Point tramming - Rear
  //#define BED_TRAMMING_LEVELING_ORDER { LF, LB }       // 3-Point tramming - Right
  //#define BED_TRAMMING_LEVELING_ORDER { RF, RB }       // 3-Point tramming - Left
  //#define BED_TRAMMING_LEVELING_ORDER { LB, RB }       // 3-Point tramming - Front
#endif

constexpr int lco[] = BED_TRAMMING_LEVELING_ORDER;
constexpr bool tramming_3_points = COUNT(lco) == 2;
static_assert(tramming_3_points || COUNT(lco) == 4, "BED_TRAMMING_LEVELING_ORDER must have exactly 2 or 4 corners.");

constexpr int lcodiff = ABS(lco[0] - lco[1]);
static_assert(COUNT(lco) == 4 || lcodiff == 1 || lcodiff == 3, "The first two BED_TRAMMING_LEVELING_ORDER corners must be on the same edge.");

constexpr int nr_edge_points = tramming_3_points ? 3 : 4;
constexpr int available_points = nr_edge_points + ENABLED(BED_TRAMMING_INCLUDE_CENTER);
constexpr int center_index = TERN(BED_TRAMMING_INCLUDE_CENTER, available_points - 1, -1);
constexpr float inset_lfrb[4] = BED_TRAMMING_INSET_LFRB;
constexpr xy_pos_t lf { (X_MIN_BED) + inset_lfrb[0], (Y_MIN_BED) + inset_lfrb[1] },
                   rb { (X_MAX_BED) - inset_lfrb[2], (Y_MAX_BED) - inset_lfrb[3] };

static int8_t bed_corner;

/**
 * Move to the next corner coordinates
 */
static void _lcd_goto_next_corner() {
  xy_pos_t corner_point = lf;                     // Left front

  if (tramming_3_points) {
    if (bed_corner >= available_points) bed_corner = 0; // Above max position -> move back to first corner
    switch (bed_corner) {
      case 0 ... 1:
        // First two corners set explicitly by the configuration
        switch (lco[bed_corner]) {
          case RF: corner_point.x = rb.x; break;  // Right Front
          case RB: corner_point   = rb;   break;  // Right Back
          case LB: corner_point.y = rb.y; break;  // Left Back
        }
        break;

      case 2:
        // Determine which edge to probe for 3rd point
        corner_point.set(lf.x + (rb.x - lf.x) / 2, lf.y + (rb.y - lf.y) / 2);
        if ((lco[0] == LB && lco[1] == RB) || (lco[0] == RB && lco[1] == LB)) corner_point.y = lf.y; // Front Center
        if ((lco[0] == LF && lco[1] == LB) || (lco[0] == LB && lco[1] == LF)) corner_point.x = rb.x; // Center Right
        if ((lco[0] == RF && lco[1] == RB) || (lco[0] == RB && lco[1] == RF)) corner_point.x = lf.x; // Left Center
        if ((lco[0] == LF && lco[1] == RF) || (lco[0] == RF && lco[1] == LF)) corner_point.y = rb.y; // Center Back
        #if ENABLED(BED_TRAMMING_USE_PROBE) && DISABLED(BED_TRAMMING_INCLUDE_CENTER)
          bed_corner++;  // Must increment the count to ensure it resets the loop if the 3rd point is out of tolerance
        #endif
        break;

      #if ENABLED(BED_TRAMMING_INCLUDE_CENTER)
        case 3:
          corner_point.set(X_CENTER, Y_CENTER);
          break;
      #endif
    }
  }
  else {
    // Four-Corner Bed Tramming with optional center
    if (TERN0(BED_TRAMMING_INCLUDE_CENTER, bed_corner == center_index)) {
      corner_point.set(X_CENTER, Y_CENTER);
    }
    else {
      switch (lco[bed_corner]) {
        case RF: corner_point.x = rb.x; break;  // Right Front
        case RB: corner_point   = rb;   break;  // Right Back
        case LB: corner_point.y = rb.y; break;  // Left Back
      }
    }
  }

  float z = BED_TRAMMING_Z_HOP;
  #if ALL(BED_TRAMMING_USE_PROBE, BLTOUCH)
    z += bltouch.z_extra_clearance();
  #endif
  line_to_z(z);
  do_blocking_move_to_xy(DIFF_TERN(BED_TRAMMING_USE_PROBE, corner_point, probe.offset_xy), manual_feedrate_mm_s.x);
  #if DISABLED(BED_TRAMMING_USE_PROBE)
    line_to_z(BED_TRAMMING_HEIGHT);
    if (++bed_corner >= available_points) bed_corner = 0;
  #endif
}

#if ENABLED(BED_TRAMMING_USE_PROBE)

  #define VALIDATE_POINT(X, Y, STR) static_assert(Probe::build_time::can_reach((X), (Y)), \
    "BED_TRAMMING_INSET_LFRB " STR " inset is not reachable with the default NOZZLE_TO_PROBE offset and PROBING_MARGIN.")
  VALIDATE_POINT(lf.x, Y_CENTER, "left"); VALIDATE_POINT(X_CENTER, lf.y, "front");
  VALIDATE_POINT(rb.x, Y_CENTER, "right"); VALIDATE_POINT(X_CENTER, rb.y, "back");

  #ifndef PAGE_CONTAINS
    #define PAGE_CONTAINS(...) true
  #endif

  void _lcd_draw_probing() {
    if (!ui.should_draw()) return;

    TERN_(HAS_MARLINUI_U8GLIB, ui.set_font(FONT_MENU)); // Set up the font for extra info

    MenuItem_static::draw(0, GET_TEXT_F(MSG_PROBING_POINT), SS_INVERT); // "Probing Mesh" heading

    uint8_t cy = TERN(TFT_COLOR_UI, 3, LCD_HEIGHT - 1), y = LCD_ROW_Y(cy);

    // Display # of good points found vs total needed
    if (PAGE_CONTAINS(y - (MENU_FONT_HEIGHT), y)) {
      SETCURSOR(TERN(TFT_COLOR_UI, 2, 0), cy);
      lcd_put_u8str(GET_TEXT_F(MSG_BED_TRAMMING_GOOD_POINTS));
      TERN_(TFT_COLOR_UI, lcd_moveto(12, cy));
      lcd_put_u8str(GOOD_POINTS_TO_STR(good_points));
      lcd_put_u8str(F("/"));
      lcd_put_u8str(GOOD_POINTS_TO_STR(nr_edge_points));
    }

    --cy;
    y -= MENU_LINE_HEIGHT;

    // Display the Last Z value
    if (PAGE_CONTAINS(y - (MENU_FONT_HEIGHT), y)) {
      SETCURSOR(TERN(TFT_COLOR_UI, 2, 0), cy);
      lcd_put_u8str(GET_TEXT_F(MSG_BED_TRAMMING_LAST_Z));
      TERN_(TFT_COLOR_UI, lcd_moveto(12, 2));
      lcd_put_u8str(LAST_Z_TO_STR(last_z));
    }
  }

  void _lcd_draw_raise() {
    if (!ui.should_draw()) return;
    MenuItem_confirm::select_screen(
        GET_TEXT_F(MSG_BUTTON_DONE), GET_TEXT_F(MSG_BUTTON_SKIP)
      , []{ tramming_done = true; wait_for_probe = false; }
      , []{ wait_for_probe = false; }
      , GET_TEXT_F(MSG_BED_TRAMMING_RAISE)
    );
  }

  void _lcd_draw_level_prompt() {
    if (!ui.should_draw()) return;
    MenuItem_confirm::select_screen(
        GET_TEXT_F(TERN(HAS_LEVELING, MSG_BUTTON_LEVEL, MSG_BUTTON_DONE))
      , TERN(HAS_LEVELING, GET_TEXT_F(MSG_BUTTON_BACK), nullptr)
      , []{
          tramming_done = true;
          queue.inject(TERN(HAS_LEVELING, F("G29N"), FPSTR(G28_STR)));
          ui.goto_previous_screen_no_defer();
        }
      , []{
          tramming_done = true;
          TERN_(HAS_LEVELING, ui.goto_previous_screen_no_defer());
          TERN_(NEEDS_PROBE_DEPLOY, probe.stow(true));
        }
      , GET_TEXT_F(MSG_BED_TRAMMING_IN_RANGE)
    );
  }

  bool _lcd_bed_tramming_probe(const bool verify=false) {
    if (verify) line_to_z(BED_TRAMMING_Z_HOP); // do clearance if needed
    TERN_(BLTOUCH, if (!bltouch.high_speed_mode) bltouch.deploy()); // Deploy in LOW SPEED MODE on every probe action
    do_blocking_move_to_z(last_z - BED_TRAMMING_PROBE_TOLERANCE, MMM_TO_MMS(Z_PROBE_FEEDRATE_SLOW)); // Move down to lower tolerance
    if (TEST(endstops.trigger_state(), Z_MIN_PROBE)) { // check if probe triggered
      endstops.hit_on_purpose();
      set_current_from_steppers_for_axis(Z_AXIS);
      sync_plan_position();

      TERN_(BLTOUCH, if (!bltouch.high_speed_mode) bltouch.stow()); // Stow in LOW SPEED MODE on every trigger

      // Triggered outside tolerance range?
      if (ABS(current_position.z - last_z) > BED_TRAMMING_PROBE_TOLERANCE) {
        last_z = current_position.z; // Above tolerance. Set a new Z for subsequent corners.
        good_points = 0;             // ...and start over
      }

      // Raise the probe after the last point to give clearance for stow
      if (TERN0(NEEDS_PROBE_DEPLOY, good_points == nr_edge_points - 1))
        line_to_z(BED_TRAMMING_Z_HOP);

      return true; // probe triggered
    }
    line_to_z(last_z); // go back to tolerance middle point before raise
    return false; // probe not triggered
  }

  bool _lcd_bed_tramming_raise() {
    bool probe_triggered = false;
    tramming_done = false;
    wait_for_probe = true;
    ui.goto_screen(_lcd_draw_raise); // show raise screen
    ui.set_selection(true);
    while (wait_for_probe && !probe_triggered) { // loop while waiting to bed raise and probe trigger
      probe_triggered = PROBE_TRIGGERED();
      if (probe_triggered) {
        endstops.hit_on_purpose();
        TERN_(BED_TRAMMING_AUDIO_FEEDBACK, BUZZ(200, 600));
      }
      idle();
    }
    TERN_(BLTOUCH, if (!bltouch.high_speed_mode) bltouch.stow());
    ui.goto_screen(_lcd_draw_probing);
    return (probe_triggered);
  }

  void _lcd_test_corners() {
    bed_corner = TERN(BED_TRAMMING_INCLUDE_CENTER, center_index, 0);
    last_z = BED_TRAMMING_HEIGHT;
    endstops.enable_z_probe(true);
    good_points = 0;
    ui.goto_screen(_lcd_draw_probing);
    do {
      ui.refresh(LCDVIEW_REDRAW_NOW);
      _lcd_draw_probing();                                // update screen with # of good points

      _lcd_goto_next_corner();                            // Goto corner

      TERN_(BLTOUCH, if (bltouch.high_speed_mode) bltouch.deploy()); // Deploy in HIGH SPEED MODE
      if (!_lcd_bed_tramming_probe()) {                   // Probe down to tolerance
        if (_lcd_bed_tramming_raise()) {                  // Prompt user to raise bed if needed
          #if ENABLED(BED_TRAMMING_VERIFY_RAISED)         // Verify
            while (!_lcd_bed_tramming_probe(true)) {      // Loop while corner verified
              if (!_lcd_bed_tramming_raise()) {           // Prompt user to raise bed if needed
                if (tramming_done) return;                // Done was selected
                break;                                    // Skip was selected
              }
            }
          #endif
        }
        else if (tramming_done)                           // Done was selected
          return;
      }

      if (bed_corner != center_index) good_points++; // ignore center
      if (++bed_corner > 3) bed_corner = 0;

    } while (good_points < nr_edge_points); // loop until all points within tolerance

    #if ENABLED(BLTOUCH)
      if (bltouch.high_speed_mode) {
        // In HIGH SPEED MODE do stow and clearance at the very end
        bltouch.stow();
        do_z_clearance(BED_TRAMMING_Z_HOP);
      }
    #endif

    ui.goto_screen(_lcd_draw_level_prompt); // prompt for bed leveling
    ui.set_selection(true);
  }

#endif // BED_TRAMMING_USE_PROBE

void _lcd_bed_tramming_homing() {
  if (!all_axes_homed() && TERN1(NEEDS_PROBE_DEPLOY, probe.deploy())) return;

  #if HAS_LEVELING // Disable leveling so the planner won't mess with us
    menu_leveling_was_active = planner.leveling_active;
    set_bed_leveling_enabled(false);
  #endif

  #if ENABLED(BED_TRAMMING_USE_PROBE)

    if (!tramming_done) _lcd_test_corners(); // May set tramming_done
    if (tramming_done) {
      ui.goto_previous_screen_no_defer();
      TERN_(NEEDS_PROBE_DEPLOY, probe.stow(true));
    }
    tramming_done = true;
    TERN_(HAS_LEVELING, set_bed_leveling_enabled(menu_leveling_was_active));
    TERN_(BLTOUCH, endstops.enable_z_probe(false));

  #else // !BED_TRAMMING_USE_PROBE

    bed_corner = 0;
    ui.goto_screen([]{
      MenuItem_confirm::select_screen(
          GET_TEXT_F(MSG_BUTTON_NEXT), GET_TEXT_F(MSG_BUTTON_DONE)
        , _lcd_goto_next_corner
        , []{
            line_to_z(BED_TRAMMING_Z_HOP); // Raise Z off the bed when done
            TERN_(HAS_LEVELING, set_bed_leveling_enabled(menu_leveling_was_active));
            ui.goto_previous_screen_no_defer();
          }
        , GET_TEXT_F(TERN(BED_TRAMMING_INCLUDE_CENTER, MSG_LEVEL_BED_NEXT_POINT, MSG_NEXT_CORNER))
        , (const char*)nullptr, F("?")
      );
    });
    ui.set_selection(true);
    _lcd_goto_next_corner();

  #endif // !BED_TRAMMING_USE_PROBE
}

#if NEEDS_PROBE_DEPLOY

  void deploy_probe() {
    if (!tramming_done) probe.deploy(true);
    ui.goto_screen([]{
      if (ui.should_draw()) MenuItem_static::draw((LCD_HEIGHT - 1) / 2, GET_TEXT_F(MSG_MANUAL_DEPLOY));
      if (!probe.deploy() && !tramming_done) _lcd_bed_tramming_homing();
    });
  }

#endif // NEEDS_PROBE_DEPLOY

void _lcd_bed_tramming() {
  TERN_(BED_TRAMMING_USE_PROBE, tramming_done = false);
  ui.defer_status_screen();
  set_all_unhomed();
  queue.inject(TERN(CAN_SET_LEVELING_AFTER_G28, F("G28L0"), FPSTR(G28_STR)));
  ui.goto_screen([]{
    _lcd_draw_homing();
    if (!all_axes_homed()) return;
    TERN(NEEDS_PROBE_DEPLOY, deploy_probe(), ui.goto_screen(_lcd_bed_tramming_homing));
  });
}

#endif // HAS_MARLINUI_MENU && LCD_BED_TRAMMING
