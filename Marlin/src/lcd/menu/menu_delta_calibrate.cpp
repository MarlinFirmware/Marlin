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
// Delta Calibrate Menu
//

#include "../../inc/MarlinConfigPre.h"

#if HAS_LCD_MENU && EITHER(DELTA_CALIBRATION_MENU, DELTA_AUTO_CALIBRATION)

#include "menu.h"
#include "../../module/delta.h"
#include "../../module/motion.h"

#if HAS_LEVELING
  #include "../../feature/bedlevel/bedlevel.h"
#endif

#if ENABLED(EXTENSIBLE_UI)
  #include "../../lcd/extui/ui_api.h"
#endif

void _man_probe_pt(const xy_pos_t &xy) {
  if (!ui.wait_for_move) {
    ui.wait_for_move = true;
    do_blocking_move_to_xy_z(xy, Z_CLEARANCE_BETWEEN_PROBES);
    ui.wait_for_move = false;
    ui.synchronize();
    move_menu_scale = _MAX(PROBE_MANUALLY_STEP, MIN_STEPS_PER_SEGMENT / float(DEFAULT_XYZ_STEPS_PER_UNIT));
    ui.goto_screen(lcd_move_z);
  }
}

#if ENABLED(DELTA_AUTO_CALIBRATION)

  #include "../../gcode/gcode.h"

  #if ENABLED(HOST_PROMPT_SUPPORT)
    #include "../../feature/host_actions.h" // for host_prompt_do
  #endif

  float lcd_probe_pt(const xy_pos_t &xy) {
    _man_probe_pt(xy);
    ui.defer_status_screen();
    TERN_(HOST_PROMPT_SUPPORT, host_prompt_do(PROMPT_USER_CONTINUE, PSTR("Delta Calibration in progress"), CONTINUE_STR));
    TERN_(EXTENSIBLE_UI, ExtUI::onUserConfirmRequired_P(PSTR("Delta Calibration in progress")));
    wait_for_user_response();
    ui.goto_previous_screen_no_defer();
    return current_position.z;
  }

#endif

#if ENABLED(DELTA_CALIBRATION_MENU)

  #include "../../gcode/queue.h"

  void _lcd_calibrate_homing() {
    _lcd_draw_homing();
    if (all_axes_homed()) ui.goto_previous_screen();
  }

  void _lcd_delta_calibrate_home() {
    queue.inject_P(G28_STR);
    ui.goto_screen(_lcd_calibrate_homing);
  }

  void _goto_tower_a(const float &a) {
    xy_pos_t tower_vec = { cos(RADIANS(a)), sin(RADIANS(a)) };
    _man_probe_pt(tower_vec * delta_calibration_radius());
  }
  void _goto_tower_x() { _goto_tower_a(210); }
  void _goto_tower_y() { _goto_tower_a(330); }
  void _goto_tower_z() { _goto_tower_a( 90); }
  void _goto_center()  { xy_pos_t ctr{0}; _man_probe_pt(ctr); }

#endif

void lcd_delta_settings() {
  auto _recalc_delta_settings = []{
    TERN_(HAS_LEVELING, reset_bed_level()); // After changing kinematics bed-level data is no longer valid
    recalc_delta_settings();
  };
  START_MENU();
  BACK_ITEM(MSG_DELTA_CALIBRATE);
  EDIT_ITEM(float52sign, MSG_DELTA_HEIGHT, &delta_height, delta_height - 10, delta_height + 10, _recalc_delta_settings);
  #define EDIT_ENDSTOP_ADJ(LABEL,N) EDIT_ITEM_P(float43, PSTR(LABEL), &delta_endstop_adj.N, -5, 5, _recalc_delta_settings)
  EDIT_ENDSTOP_ADJ("Ex", a);
  EDIT_ENDSTOP_ADJ("Ey", b);
  EDIT_ENDSTOP_ADJ("Ez", c);
  EDIT_ITEM(float52sign, MSG_DELTA_RADIUS, &delta_radius, delta_radius - 5, delta_radius + 5, _recalc_delta_settings);
  #define EDIT_ANGLE_TRIM(LABEL,N) EDIT_ITEM_P(float43, PSTR(LABEL), &delta_tower_angle_trim.N, -5, 5, _recalc_delta_settings)
  EDIT_ANGLE_TRIM("Tx", a);
  EDIT_ANGLE_TRIM("Ty", b);
  EDIT_ANGLE_TRIM("Tz", c);
  EDIT_ITEM(float52sign, MSG_DELTA_DIAG_ROD, &delta_diagonal_rod, delta_diagonal_rod - 5, delta_diagonal_rod + 5, _recalc_delta_settings);
  END_MENU();
}

void menu_delta_calibrate() {
  const bool all_homed = all_axes_homed();

  START_MENU();
  BACK_ITEM(MSG_MAIN);

  #if ENABLED(DELTA_AUTO_CALIBRATION)
    GCODES_ITEM(MSG_DELTA_AUTO_CALIBRATE, PSTR("G33"));
    #if ENABLED(EEPROM_SETTINGS)
      ACTION_ITEM(MSG_STORE_EEPROM, lcd_store_settings);
      ACTION_ITEM(MSG_LOAD_EEPROM, lcd_load_settings);
    #endif
  #endif

  SUBMENU(MSG_DELTA_SETTINGS, lcd_delta_settings);

  #if ENABLED(DELTA_CALIBRATION_MENU)
    SUBMENU(MSG_AUTO_HOME, _lcd_delta_calibrate_home);
    if (all_homed) {
      SUBMENU(MSG_DELTA_CALIBRATE_X, _goto_tower_x);
      SUBMENU(MSG_DELTA_CALIBRATE_Y, _goto_tower_y);
      SUBMENU(MSG_DELTA_CALIBRATE_Z, _goto_tower_z);
      SUBMENU(MSG_DELTA_CALIBRATE_CENTER, _goto_center);
    }
  #endif

  END_MENU();
}

#endif // HAS_LCD_MENU && (DELTA_CALIBRATION_MENU || DELTA_AUTO_CALIBRATION)
