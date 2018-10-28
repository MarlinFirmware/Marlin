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
// Delta Calibrate Menu
//

#include "../../inc/MarlinConfigPre.h"

#if HAS_LCD_MENU && (ENABLED(DELTA_CALIBRATION_MENU) || ENABLED(DELTA_AUTO_CALIBRATION))

#include "menu.h"
#include "../../module/delta.h"
#include "../../module/motion.h"

#if HAS_LEVELING
  #include "../../feature/bedlevel/bedlevel.h"
#endif

void _man_probe_pt(const float &rx, const float &ry) {
  do_blocking_move_to(rx, ry, Z_CLEARANCE_BETWEEN_PROBES);
  lcd_synchronize();
  move_menu_scale = MAX(PROBE_MANUALLY_STEP, MIN_STEPS_PER_SEGMENT / float(DEFAULT_XYZ_STEPS_PER_UNIT));
  lcd_goto_screen(lcd_move_z);
}

#if ENABLED(DELTA_AUTO_CALIBRATION)

  #include "../../gcode/gcode.h"

  float lcd_probe_pt(const float &rx, const float &ry) {
    _man_probe_pt(rx, ry);
    KEEPALIVE_STATE(PAUSED_FOR_USER);
    defer_return_to_status = true;
    wait_for_user = true;
    while (wait_for_user) idle();
    KEEPALIVE_STATE(IN_HANDLER);
    lcd_goto_previous_menu_no_defer();
    return current_position[Z_AXIS];
  }

#endif

#if ENABLED(DELTA_CALIBRATION_MENU)

  #include "../../gcode/queue.h"

  void _lcd_calibrate_homing() {
    _lcd_draw_homing();
    if (all_axes_homed()) lcd_goto_previous_menu();
  }

  void _lcd_delta_calibrate_home() {
    enqueue_and_echo_commands_P(PSTR("G28"));
    lcd_goto_screen(_lcd_calibrate_homing);
  }

  void _goto_tower_x() { _man_probe_pt(cos(RADIANS(210)) * delta_calibration_radius, sin(RADIANS(210)) * delta_calibration_radius); }
  void _goto_tower_y() { _man_probe_pt(cos(RADIANS(330)) * delta_calibration_radius, sin(RADIANS(330)) * delta_calibration_radius); }
  void _goto_tower_z() { _man_probe_pt(cos(RADIANS( 90)) * delta_calibration_radius, sin(RADIANS( 90)) * delta_calibration_radius); }
  void _goto_center()  { _man_probe_pt(0,0); }

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
  #define EDIT_ENDSTOP_ADJ(LABEL,N) MENU_ITEM_EDIT_CALLBACK(float43, LABEL, &delta_endstop_adj[_AXIS(N)], -5, 5, _recalc_delta_settings)
  EDIT_ENDSTOP_ADJ("Ex",A);
  EDIT_ENDSTOP_ADJ("Ey",B);
  EDIT_ENDSTOP_ADJ("Ez",C);
  MENU_ITEM_EDIT_CALLBACK(float52sign, MSG_DELTA_RADIUS, &delta_radius, delta_radius - 5, delta_radius + 5, _recalc_delta_settings);
  #define EDIT_ANGLE_TRIM(LABEL,N) MENU_ITEM_EDIT_CALLBACK(float43, LABEL, &delta_tower_angle_trim[_AXIS(N)], -5, 5, _recalc_delta_settings)
  EDIT_ANGLE_TRIM("Tx",A);
  EDIT_ANGLE_TRIM("Ty",B);
  EDIT_ANGLE_TRIM("Tz",C);
  MENU_ITEM_EDIT_CALLBACK(float52sign, MSG_DELTA_DIAG_ROD, &delta_diagonal_rod, delta_diagonal_rod - 5, delta_diagonal_rod + 5, _recalc_delta_settings);
  END_MENU();
}

void menu_delta_calibrate() {
  START_MENU();
  MENU_BACK(MSG_MAIN);

  #if ENABLED(DELTA_AUTO_CALIBRATION)
    MENU_ITEM(gcode, MSG_DELTA_AUTO_CALIBRATE, PSTR("G33"));
    MENU_ITEM(gcode, MSG_DELTA_HEIGHT_CALIBRATE, PSTR("G33 S P1"));
    MENU_ITEM(gcode, MSG_DELTA_Z_OFFSET_CALIBRATE, PSTR("G33 P-1"));
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
