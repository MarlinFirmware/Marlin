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
// Calibrate Probe offset menu.
//

#include "../../inc/MarlinConfigPre.h"

#if ALL(HAS_MARLINUI_MENU, PROBE_OFFSET_WIZARD)

#include "menu_item.h"
#include "menu_addon.h"
#include "../../gcode/queue.h"
#include "../../module/motion.h"
#include "../../module/planner.h"
#include "../../module/probe.h"
#include "../../module/temperature.h"

#if HAS_LEVELING
  #include "../../feature/bedlevel/bedlevel.h"
#endif

void _goto_manual_move_z(const float);

// Global storage - TODO: Keep wizard/process data in a 'ui.scratch' union.
float z_offset_backup, calculated_z_offset, z_offset_ref;

// "Done" - Set the offset, re-enable leveling, go back to the previous screen.
void set_offset_and_go_back(const float z) {
  probe.offset.z = z;
  motion.set_soft_endstop_loose(false);
  TERN_(HAS_LEVELING, set_bed_leveling_enabled(menu_leveling_was_active));
  ui.goto_previous_screen_no_defer();
}

/**
 * @fn probe_offset_wizard_menu
 * @brief Display a menu to Move Z, Cancel, or signal Done
 */
void probe_offset_wizard_menu() {
  START_MENU();
  calculated_z_offset = probe.offset.z + motion.position.z - z_offset_ref;

  if (LCD_HEIGHT >= 4)
    STATIC_ITEM(MSG_MOVE_NOZZLE_TO_BED, SS_CENTER|SS_INVERT);

  STATIC_ITEM_F(F("Z"), SS_CENTER, ftostr42_52(motion.position.z));
  STATIC_ITEM_N(Z_AXIS, MSG_ZPROBE_OFFSET_N, SS_FULL, ftostr42_52(calculated_z_offset));

  SUBMENU_S(F("1.0"), MSG_MOVE_N_MM, []{ _goto_manual_move_z( 1.0f); });
  SUBMENU_S(F("0.1"), MSG_MOVE_N_MM, []{ _goto_manual_move_z( 0.1f); });

  if ((FINE_MANUAL_MOVE) > 0.0f && (FINE_MANUAL_MOVE) < 0.1f)
    SUBMENU_f(F(STRINGIFY(FINE_MANUAL_MOVE)), MSG_MOVE_N_MM, []{ _goto_manual_move_z(float(FINE_MANUAL_MOVE)); });

  ACTION_ITEM(MSG_BUTTON_DONE, []{
    set_offset_and_go_back(calculated_z_offset);
    motion.position.z = z_offset_ref;  // Set Z to z_offset_ref, as we can expect it is at probe height
    motion.sync_plan_position();
    motion.do_z_post_clearance();
  });

  ACTION_ITEM(MSG_BUTTON_CANCEL, []{
    set_offset_and_go_back(z_offset_backup);
    // On cancel the Z position needs correction
    #if HOMING_Z_WITH_PROBE && defined(PROBE_OFFSET_WIZARD_START_Z)
      motion.set_axis_never_homed(Z_AXIS);
      queue.inject(F("G28Z"));
    #else
      motion.do_z_post_clearance();
    #endif
  });

  END_MENU();
}

/**
 * The wizard runs as a sequence of stages, each with its own screen. Waiting
 * for heat and waiting for a move are polled rather than blocking, so the
 * display always shows what is being waited for. Probing still blocks inside
 * probe_at_point(), as it always has.
 *
 *   PREHEAT  Bring the bed and nozzle to probing temperature. Done first, so
 *            that neither homing nor probing later stops to heat with nothing
 *            on screen to explain the pause.
 *   HOME     Wait for the injected G28.
 *   PROBE    Probe a reference point, where the configuration calls for one.
 *   MOVE     Put the nozzle over the probed point, keeping the notice up
 *            for long enough to read however short the move turns out to be.
 *   ADJUST   probe_offset_wizard_menu(), for the Z adjustment itself.
 *
 * Each stage is a screen, so ui.goto_screen() moves between them and takes
 * care of drawing the new stage from scratch.
 */

// The line the stage notices are drawn on
#define WIZ_LINE ((LCD_HEIGHT - 1) / 2)

// Keep a stage notice up at least this long. The move to the probing position
// can be over in a blink, leaving a message no one had a chance to read.
#define WIZ_NOTICE_MIN_MS 1500
static millis_t wizard_notice_until;

// Draw the notice that says what this stage is waiting for
static void wizard_draw_notice(FSTR_P const fmsg) {
  if (ui.should_draw()) MenuItem_static::draw(WIZ_LINE, fmsg);
}

static void wizard_start_homing();
static void wizard_home_screen();
static void wizard_move_screen();

// Homing with the probe already leaves a Z reference, so the PROBE stage only
// exists where a point has to be probed for one
#if defined(PROBE_OFFSET_WIZARD_XY_POS) || !HOMING_Z_WITH_PROBE
  #define WIZ_PROBE_STAGE 1
  static void wizard_probe_screen();
  #define WIZ_AFTER_HOMING wizard_probe_screen
#else
  #define WIZ_AFTER_HOMING wizard_move_screen
#endif

#if ENABLED(PREHEAT_BEFORE_PROBING)

  /**
   * True while either heater is still below its probing temperature, using the
   * same margins Probe::preheat_for_probing() would have waited on. Once this
   * goes false, the preheat call inside probing has nothing left to wait for.
   */
  static bool wizard_preheat_pending() {
    #if HAS_HOTEND && PROBING_NOZZLE_TEMP
      if ((PROBING_NOZZLE_TEMP) > thermalManager.wholeDegHotend(0) + (TEMP_WINDOW)) return true;
    #endif
    #if HAS_HEATED_BED && PROBING_BED_TEMP
      if ((PROBING_BED_TEMP) > thermalManager.wholeDegBed() + (TEMP_BED_WINDOW)) return true;
    #endif
    return false;
  }

  // Heater labels, shortened to suit the width of the display
  #if LCD_WIDTH >= 21
    #define WIZ_HOTEND_LABEL "H" STR_N0 ":"
    #define WIZ_BED_LABEL    "BED:"
  #elif LCD_WIDTH >= 20
    #define WIZ_HOTEND_LABEL "H" STR_N0 ":"
    #define WIZ_BED_LABEL    "B:"
  #else
    #define WIZ_HOTEND_LABEL "H"   // No room for the tool index on a narrow display
    #define WIZ_BED_LABEL    "B"
  #endif

  /**
   * "H1:cur/set BED:cur/set" for the heaters being preheated, with the labels
   * above and drawn where the status message goes: the last line on a character
   * display, the bottom edge on a graphical one. Current temperatures are
   * right-justified in 3 columns so that nothing shifts as they rise through
   * 10 and 100 degrees.
   */
  static void wizard_draw_preheat_temps() {
    #if HAS_MARLINUI_U8GLIB

      // Below the last menu row, on the baseline the status screen draws to
      lcd_moveto(0, (LCD_PIXEL_HEIGHT) - (INFO_FONT_DESCENT));
      #if HAS_HOTEND && PROBING_NOZZLE_TEMP
        lcd_put_u8str(F(WIZ_HOTEND_LABEL));
        lcd_put_u8str(i16tostr3rj(thermalManager.wholeDegHotend(0)));
        lcd_put_u8str(F("/"));
        lcd_put_u8str(i16tostr3left(thermalManager.degTargetHotend(0)));
        lcd_put_u8str(F(" "));
      #endif
      #if HAS_HEATED_BED && PROBING_BED_TEMP
        lcd_put_u8str(F(WIZ_BED_LABEL));
        lcd_put_u8str(i16tostr3rj(thermalManager.wholeDegBed()));
        lcd_put_u8str(F("/"));
        lcd_put_u8str(i16tostr3left(thermalManager.degTargetBed()));
      #endif

    #else

      // A character display needs the line padded out, so build it first
      char buf[26] = "";
      char *b = buf;
      #if HAS_HOTEND && PROBING_NOZZLE_TEMP
        b += sprintf_P(b, PSTR(WIZ_HOTEND_LABEL "%3i/%i "), thermalManager.wholeDegHotend(0), thermalManager.degTargetHotend(0));
      #endif
      #if HAS_HEATED_BED && PROBING_BED_TEMP
        b += sprintf_P(b, PSTR(WIZ_BED_LABEL "%3i/%i"), thermalManager.wholeDegBed(), thermalManager.degTargetBed());
      #endif
      if (b != buf) MenuItem_static::draw(LCD_HEIGHT - 1, F(""), SS_LEFT, buf);

    #endif
  }

  // PREHEAT: watch the heaters come up to the probing temperatures
  static void wizard_preheat_screen() {
    wizard_draw_notice(GET_TEXT_F(MSG_PREHEATING));
    if (ui.should_draw()) wizard_draw_preheat_temps();
    // The heaters are all that changes here, so ask for the next redraw
    ui.refresh(LCDVIEW_CALL_REDRAW_NEXT);
    if (!wizard_preheat_pending()) wizard_start_homing();
  }

#endif // PREHEAT_BEFORE_PROBING

// Home all axes, then wait for it on the HOME screen
static void wizard_start_homing() {
  ui.goto_screen(wizard_home_screen);
  queue.inject_P(G28_STR);
}

// HOME: wait for the injected G28
static void wizard_home_screen() {
  wizard_draw_notice(GET_TEXT_F(MSG_LEVEL_BED_HOMING));
  if (motion.all_axes_homed()) {
    z_offset_ref = 0;                   // Z value for the wizard position
    ui.goto_screen(WIZ_AFTER_HOMING);
  }
}

#if ENABLED(WIZ_PROBE_STAGE)

  // PROBE: probe a Z reference at the wizard position
  static void wizard_probe_screen() {
    wizard_draw_notice(GET_TEXT_F(MSG_PROBE_WIZARD_PROBING));

    if (ui.wait_for_move) return;

    #ifndef PROBE_OFFSET_WIZARD_XY_POS
      #define PROBE_OFFSET_WIZARD_XY_POS XY_CENTER
    #endif
    // Probe at the configured point, or the center of the bed
    ui.wait_for_move = true;
    z_offset_ref = probe.probe_at_point(xy_pos_t(PROBE_OFFSET_WIZARD_XY_POS), PROBE_PT_RAISE);
    ui.wait_for_move = false;

    probe.stow();   // probe_at_point leaves the probe deployed when it succeeds

    // A failed probe leaves no reference to adjust against, and probing
    // out of reach fails without even a message, so say so and back out
    // rather than offering a menu full of nonsense.
    if (isnan(z_offset_ref)) {
      LCD_MESSAGE(MSG_LCD_PROBING_FAILED);
      set_offset_and_go_back(z_offset_backup);
      return;
    }

    ui.goto_screen(wizard_move_screen);
  }

#endif

// MOVE: put the nozzle over the point that was probed
static void wizard_move_screen() {
  wizard_draw_notice(GET_TEXT_F(MSG_PROBE_WIZARD_MOVING));
  ui.refresh(LCDVIEW_CALL_REDRAW_NEXT);

  // Start the move on the first pass, then poll for it
  if (!ui.wait_for_move) {
    ui.wait_for_move = true;
    wizard_notice_until = millis() + (WIZ_NOTICE_MIN_MS);
    motion.position += probe.offset_xy;
    motion.goto_current_position(XY_PROBE_FEEDRATE_MM_S);
    return;
  }

  // Wait for the move to land, and for the notice to have been readable
  if (planner.busy() || PENDING(millis(), wizard_notice_until)) return;

  ui.wait_for_move = false;
  motion.set_soft_endstop_loose(true);  // Disable soft endstops for free Z movement

  ui.goto_screen(probe_offset_wizard_menu);
  ui.defer_status_screen();
}

/**
 * @fn goto_probe_offset_wizard
 * @brief Set up the wizard and start it at its first stage.
 */
void goto_probe_offset_wizard() {
  ui.defer_status_screen();
  motion.set_all_unhomed();

  // Store probe.offset.z for Case: Cancel
  z_offset_backup = probe.offset.z;

  #ifdef PROBE_OFFSET_WIZARD_START_Z
    probe.offset.z = PROBE_OFFSET_WIZARD_START_Z;
  #endif

  // Store Bed-Leveling-State and disable
  #if HAS_LEVELING
    menu_leveling_was_active = planner.leveling_active;
    set_bed_leveling_enabled(false);
  #endif

  #if ENABLED(PREHEAT_BEFORE_PROBING)
    // Set the probing temperatures now and watch them come up, rather than
    // letting homing or probing stop to heat with nothing on screen to say so.
    probe.preheat_for_probing(PROBING_NOZZLE_TEMP, PROBING_BED_TEMP, true);
    ui.goto_screen(wizard_preheat_screen);
  #else
    wizard_start_homing();
  #endif
}

#endif // HAS_MARLINUI_MENU && PROBE_OFFSET_WIZARD
