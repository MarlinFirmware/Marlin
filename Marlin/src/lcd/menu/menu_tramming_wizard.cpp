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
// Bed Tramming Wizard
// Implemented and supported by https://github.com/lukasradek
//

#include "../../inc/MarlinConfigPre.h"

#if ALL(HAS_MARLINUI_MENU, ASSISTED_TRAMMING_WIZARD)

#include "menu_item.h"

#include "../../feature/tramming.h"
#include "../../module/planner.h"
#include "../../module/motion.h"
#include "../../module/probe.h"
#include "../../gcode/queue.h"

#if ENABLED(BLTOUCH)
  #include "../../feature/bltouch.h"
#endif

#if HAS_LEVELING
  #include "../../feature/bedlevel/bedlevel.h"
#endif

#define SIMULATOR_TESTING

static uint8_t tram_target = 0;
static float z_measured[G35_PROBE_COUNT];
static Flags<G35_PROBE_COUNT> z_isvalid;

static bool reference_valid = false;
static float reference_z;

static bool menu_mode_measure = true;

#ifdef TRAMMING_SCREW_THREAD
  static bool units_mm = true;
  constexpr const uint8_t screw_size = (TRAMMING_SCREW_THREAD) / 10;
  static char z_turns[G35_PROBE_COUNT][8] = {0};
#endif

static void menu_tramming_wizard();

#if ENABLED(SIMULATOR_TESTING)
  /**
   * Mock function for probing in simulator, where "probe.probe_at_point" always returns 0.
   */
  static float _mock_probe_at_point(const xy_pos_t pos) {
    do_blocking_move_to_xy(pos, 100.0f);

    const float FL_z = 2, FR_z = -5, BL_z = 0, BR_z = 3, // bed corner Z values from which to interpolate
                FX_z = (FR_z - FL_z) / (X_BED_SIZE) * pos.x + FL_z,
                BX_z = (BR_z - BL_z) / (X_BED_SIZE) * pos.x + BL_z,
                YX_z = (BX_z - FX_z) / (Y_BED_SIZE) * pos.y + FX_z;
    return YX_z;
  }
#endif

static void _menu_jump_to_top() {
  encoderTopLine = 0;
  encoderLine = 1;
  ui.encoderPosition = encoderLine * ENCODER_STEPS_PER_MENU_ITEM;
  ui.refresh();
}

#ifdef TRAMMING_SCREW_THREAD

  static void _update_screw_turns_str_in_buffer(uint8_t tram_target) {

    const float threads_factor[] = { 0.5, 0.7, 0.8 };

    const float distance_mm = z_measured[tram_target] - reference_z;
    const float mm_abs = ABS(distance_mm);
    const float turns_abs = mm_abs < 0.001f ? 0 : mm_abs / threads_factor[screw_size-3];

    if (turns_abs != 0) {
      strcpy(z_turns[tram_target], ftostr31ns(turns_abs));
      if (z_turns[tram_target][0] == '0') z_turns[tram_target][0] = ' ';
      strcat_P(z_turns[tram_target], (TRAMMING_SCREW_THREAD & 1) == (distance_mm > 0) ? PSTR("ccw") : PSTR("cw"));
    }
    else
      strcpy(z_turns[tram_target], "0");
  }

#endif //TRAMMING_SCREW_THREAD

static void _set_reference_z(const_float_t z) {
  reference_z = z;
  reference_valid = true;
  #ifdef TRAMMING_SCREW_THREAD
    for (uint8_t i = 0; i < G35_PROBE_COUNT;i++)
      if (z_isvalid[i]) _update_screw_turns_str_in_buffer(i);
  #endif
}

static bool _probe_single_point() {

  const bool probing_reference = !reference_valid;

  if (probing_reference)
    ui.goto_message_screen(GET_TEXT_F(MSG_TW_MEASURING_REF), nullptr, FPSTR(pgm_read_ptr(&tramming_point_name[tram_target])));



  #if HAS_LEVELING
    const bool leveling_prev_state = planner.leveling_active;
    set_bed_leveling_enabled(false);
  #endif

  const float z_probed_height = probing_reference && z_isvalid[tram_target] ? z_measured[tram_target] : (
    #if ENABLED(SIMULATOR_TESTING)
      _mock_probe_at_point(tramming_points[tram_target])
    #else
      probe.probe_at_point(tramming_points[tram_target], PROBE_PT_RAISE, 0, true)
    #endif
  );

  z_isvalid.clear(tram_target);
  const bool v = !isnan(z_probed_height);

  if (v) {
    z_measured[tram_target] = z_probed_height;
    z_isvalid.set(tram_target);

    if (probing_reference)
      _set_reference_z(z_probed_height);
    else { // _set_reference_z internally calls _update_screw_turns_str_in_buffer, no need to repeat
      #ifdef TRAMMING_SCREW_THREAD
        _update_screw_turns_str_in_buffer(tram_target);
      #endif
    }
  }

  move_to_tramming_wait_pos();

  if (probing_reference) {
    ui.goto_previous_screen();
    _menu_jump_to_top();
  }

  TERN_(HAS_LEVELING, set_bed_leveling_enabled(leveling_prev_state));

  return v;
}

static void _probe_all_points() {
  for (tram_target = 0; tram_target < G35_PROBE_COUNT; tram_target++) {
    if (_probe_single_point()) ui.refresh();
  }
}

inline const char * _get_mms_str(const uint8_t t) {
  return z_isvalid[t] ? ftostr42_52(z_measured[t] - reference_z) : "---";
}

#ifdef TRAMMING_SCREW_THREAD
  inline const char * _get_screw_turns_str(const uint8_t t) {
    return z_isvalid[t] ? z_turns[t] : "---";
  }
#endif

static void menu_tram_point() {

  START_MENU();

  STATIC_ITEM_F(FPSTR(pgm_read_ptr(&tramming_point_name[tram_target])), SS_FULL, _get_mms_str(tram_target));
  #ifdef TRAMMING_SCREW_THREAD
    STATIC_ITEM_N_F(screw_size, F("M= screw turns"), SS_FULL, _get_screw_turns_str(tram_target));
  #endif

  ACTION_ITEM(MSG_MEASURE, []{ if (_probe_single_point()) ui.refresh(); });
  if (!z_isvalid[tram_target] || ABS(z_measured[tram_target] - reference_z) > 0.001f) {
    ACTION_ITEM(MSG_TW_SET_AS_REF, []{ reference_valid = false; if (_probe_single_point()) ui.refresh(); });
  }
  ACTION_ITEM(MSG_BUTTON_BACK, ui.goto_previous_screen);
  END_MENU();
}

static void menu_tramming_wizard() {

  START_MENU();

  // Menu content is different based on reference_valid
  // i.e. whether the menu is in Reference point selection mode or Measuring mode

  STATIC_ITEM_F(reference_valid ? GET_TEXT_F(MSG_TW_SELECT_TRAM_POINT) : GET_TEXT_F(MSG_TW_SELECT_TRAM_REF));
  if (reference_valid) ACTION_ITEM_F(F("Measure All"), _probe_all_points);

  // Draw a menu item for each tramming point
  static uint8_t t;
  for (t = 0; t < G35_PROBE_COUNT; t++) {
    const char *tram_val = !z_isvalid[t] ? "---" : (
      #ifdef TRAMMING_SCREW_THREAD
        units_mm ? ftostr42_52(z_measured[t] - reference_z) :  z_turns[t]
      #else
        ftostr42_52(z_measured[t] - reference_z)
      #endif
    );

    if (!reference_valid || menu_mode_measure)
      ACTION_ITEM_F(
        FPSTR(pgm_read_ptr(&tramming_point_name[t])),
        []{ tram_target = t; if (_probe_single_point()) { ui.refresh(); } },
        SS_FULL, tram_val
      );
    else
      SUBMENU_F(
        FPSTR(pgm_read_ptr(&tramming_point_name[t])),
        []{ tram_target = t; menu_tram_point(); },
        SS_FULL, tram_val
      );
  }

  if (!reference_valid)
    ACTION_ITEM_F(F("Z = 0"), []{ _set_reference_z(0.0f); _menu_jump_to_top(); });

  STATIC_ITEM(MSG_MENU_DIVIDER);

  if (reference_valid) {
    ACTION_ITEM_F(menu_mode_measure ? GET_TEXT_F(MSG_TW_MENU_MODE_MEASURE) : GET_TEXT_F(MSG_TW_MENU_MODE_DETAIL), []{ menu_mode_measure ^= true; ui.refresh(); });
    #ifdef TRAMMING_SCREW_THREAD
      ACTION_ITEM_N_F(screw_size, (units_mm ? GET_TEXT_F(MSG_TW_UNITS_MM) : GET_TEXT_F(MSG_TW_UNITS_SCREW)), []{ units_mm = !units_mm; ui.refresh(); });
    #endif
    ACTION_ITEM(MSG_TW_SELECT_TRAM_REF, []{ reference_valid = false; _menu_jump_to_top(); });
  }

  ACTION_ITEM(MSG_BUTTON_DONE, []{ probe.stow(); ui.goto_previous_screen_no_defer();});

  END_MENU();
}

// Init the wizard and enter the submenu
void goto_tramming_wizard() {
  ui.defer_status_screen();

  // Reset wizard
  z_isvalid.reset();
  reference_valid = false;
  menu_mode_measure = true;
  tram_target = 0;

  // Inject G28, wait for homing to complete,
  set_all_unhomed();
  queue.inject(TERN(CAN_SET_LEVELING_AFTER_G28, F("G28L0"), FPSTR(G28_STR)));

  ui.goto_screen([]{
    _lcd_draw_homing();
    if (all_axes_homed())
      ui.goto_screen(menu_tramming_wizard);
  });
}

#endif // HAS_MARLINUI_MENU && ASSISTED_TRAMMING_WIZARD
