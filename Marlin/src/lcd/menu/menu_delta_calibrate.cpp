/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
  #include "../../lcd/extensible_ui/ui_api.h"
#endif

void _man_probe_pt(const xy_pos_t &xy) {
  do_blocking_move_to(xy, Z_CLEARANCE_BETWEEN_PROBES);
  ui.synchronize();
  move_menu_scale = _MAX(PROBE_MANUALLY_STEP, MIN_STEPS_PER_SEGMENT / float(DEFAULT_XYZ_STEPS_PER_UNIT));
  ui.goto_screen(lcd_move_z);
}

#if ENABLED(DELTA_AUTO_CALIBRATION)

  #include "../../gcode/gcode.h"

  float lcd_probe_pt(const xy_pos_t &xy) {
    _man_probe_pt(xy);
    KEEPALIVE_STATE(PAUSED_FOR_USER);
    ui.defer_status_screen();
    wait_for_user = true;
    #if ENABLED(HOST_PROMPT_SUPPORT)
      host_prompt_do(PROMPT_USER_CONTINUE, PSTR("Delta Calibration in progress"), PSTR("Continue"));
    #endif
    #if ENABLED(EXTENSIBLE_UI)
      ExtUI::onUserConfirmRequired(PSTR("Delta Calibration in progress"));
    #endif
    while (wait_for_user) idle();
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
    queue.inject_P(PSTR("G28"));
    ui.goto_screen(_lcd_calibrate_homing);
  }

  void _goto_tower_a(const float &a) {
    xy_pos_t tower_vec = { cos(RADIANS(a)), sin(RADIANS(a)) };
    _man_probe_pt(tower_vec * delta_calibration_radius);
  }
  void _goto_tower_x() { _goto_tower_a(210); }
  void _goto_tower_y() { _goto_tower_a(330); }
  void _goto_tower_z() { _goto_tower_a( 90); }
  void _goto_center()  { xy_pos_t ctr{0}; _man_probe_pt(ctr); }

#endif

void _recalc_delta_settings() {
  #if HAS_LEVELING
    reset_bed_level(); // After changing kinematics bed-level data is no longer valid
  #endif
  recalc_delta_settings();
}

void lcd_delta_settings() {
  START_MENU();
  MENU_BACK(MSG_DELTA_CALIBRATE);
  MENU_ITEM_EDIT_CALLBACK(float52sign, MSG_DELTA_HEIGHT, &delta_height, delta_height - 10, delta_height + 10, _recalc_delta_settings);
  #define EDIT_ENDSTOP_ADJ(LABEL,N) MENU_ITEM_EDIT_CALLBACK(float43, LABEL, &delta_endstop_adj.N, -5, 5, _recalc_delta_settings)
  EDIT_ENDSTOP_ADJ("Ex",a);
  EDIT_ENDSTOP_ADJ("Ey",b);
  EDIT_ENDSTOP_ADJ("Ez",c);
  MENU_ITEM_EDIT_CALLBACK(float52sign, MSG_DELTA_RADIUS, &delta_radius, delta_radius - 5, delta_radius + 5, _recalc_delta_settings);
  #define EDIT_ANGLE_TRIM(LABEL,N) MENU_ITEM_EDIT_CALLBACK(float43, LABEL, &delta_tower_angle_trim.N, -5, 5, _recalc_delta_settings)
  EDIT_ANGLE_TRIM("Tx",a);
  EDIT_ANGLE_TRIM("Ty",b);
  EDIT_ANGLE_TRIM("Tz",c);
  MENU_ITEM_EDIT_CALLBACK(float52sign, MSG_DELTA_DIAG_ROD, &delta_diagonal_rod, delta_diagonal_rod - 5, delta_diagonal_rod + 5, _recalc_delta_settings);
  END_MENU();
}

void menu_delta_calibrate() {
  START_MENU();
  MENU_BACK(MSG_MAIN);

  #if ENABLED(DELTA_AUTO_CALIBRATION)
    MENU_ITEM(gcode, MSG_DELTA_AUTO_CALIBRATE, PSTR("G33"));
    #if ENABLED(EEPROM_SETTINGS)
      MENU_ITEM(function, MSG_STORE_EEPROM, lcd_store_settings);
      MENU_ITEM(function, MSG_LOAD_EEPROM, lcd_load_settings);
    #endif
  #endif

  MENU_ITEM(submenu, MSG_DELTA_SETTINGS, lcd_delta_settings);

  #if ENABLED(DELTA_CALIBRATION_MENU)
    MENU_ITEM(submenu, MSG_AUTO_HOME, _lcd_delta_calibrate_home);
    if (all_axes_homed()) {
      MENU_ITEM(submenu, MSG_DELTA_CALIBRATE_X, _goto_tower_x);
      MENU_ITEM(submenu, MSG_DELTA_CALIBRATE_Y, _goto_tower_y);
      MENU_ITEM(submenu, MSG_DELTA_CALIBRATE_Z, _goto_tower_z);
      MENU_ITEM(submenu, MSG_DELTA_CALIBRATE_CENTER, _goto_center);
    }
  #endif

  END_MENU();
}

#endif // HAS_LCD_MENU && (DELTA_CALIBRATION_MENU || DELTA_AUTO_CALIBRATION)
