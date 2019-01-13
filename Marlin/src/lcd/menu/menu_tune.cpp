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
// Tune Menu
//

#include "../../inc/MarlinConfigPre.h"

#if HAS_LCD_MENU

#include "menu.h"
#include "../../module/motion.h"
#include "../../module/planner.h"
#include "../../module/temperature.h"
#include "../../Marlin.h"

#if HAS_LEVELING
  #include "../../feature/bedlevel/bedlevel.h"
#endif

// Refresh the E factor after changing flow
void _lcd_refresh_e_factor_0() { planner.refresh_e_factor(0); }
#if EXTRUDERS > 1
  void _lcd_refresh_e_factor() { planner.refresh_e_factor(active_extruder); }
  void _lcd_refresh_e_factor_1() { planner.refresh_e_factor(1); }
  #if EXTRUDERS > 2
    void _lcd_refresh_e_factor_2() { planner.refresh_e_factor(2); }
    #if EXTRUDERS > 3
      void _lcd_refresh_e_factor_3() { planner.refresh_e_factor(3); }
      #if EXTRUDERS > 4
        void _lcd_refresh_e_factor_4() { planner.refresh_e_factor(4); }
        #if EXTRUDERS > 5
          void _lcd_refresh_e_factor_5() { planner.refresh_e_factor(5); }
        #endif // EXTRUDERS > 5
      #endif // EXTRUDERS > 4
    #endif // EXTRUDERS > 3
  #endif // EXTRUDERS > 2
#endif // EXTRUDERS > 1

#if ENABLED(BABYSTEPPING)

  long babysteps_done = 0;

  void _lcd_babystep(const AxisEnum axis, PGM_P msg) {
    if (ui.use_click()) return ui.goto_previous_screen_no_defer();
    ui.encoder_direction_normal();
    if (ui.encoderPosition) {
      const int16_t babystep_increment = (int32_t)ui.encoderPosition * (BABYSTEP_MULTIPLICATOR);
      ui.encoderPosition = 0;
      ui.refresh(LCDVIEW_REDRAW_NOW);
      thermalManager.babystep_axis(axis, babystep_increment);
      babysteps_done += babystep_increment;
    }
    if (ui.should_draw())
      draw_edit_screen(msg, ftostr43sign(planner.steps_to_mm[axis] * babysteps_done));
  }

  #if ENABLED(BABYSTEP_XY)
    void _lcd_babystep_x() { _lcd_babystep(X_AXIS, PSTR(MSG_BABYSTEP_X)); }
    void _lcd_babystep_y() { _lcd_babystep(Y_AXIS, PSTR(MSG_BABYSTEP_Y)); }
    void lcd_babystep_x() { ui.goto_screen(_lcd_babystep_x); babysteps_done = 0; ui.defer_status_screen(true); }
    void lcd_babystep_y() { ui.goto_screen(_lcd_babystep_y); babysteps_done = 0; ui.defer_status_screen(true); }
  #endif

  #if DISABLED(BABYSTEP_ZPROBE_OFFSET)
    void _lcd_babystep_z() { _lcd_babystep(Z_AXIS, PSTR(MSG_BABYSTEP_Z)); }
    void lcd_babystep_z() { ui.goto_screen(_lcd_babystep_z); babysteps_done = 0; ui.defer_status_screen(true); }
  #endif

#endif // BABYSTEPPING

void menu_tune() {
  START_MENU();
  MENU_BACK(MSG_MAIN);

  //
  // Speed:
  //
  MENU_ITEM_EDIT(int3, MSG_SPEED, &feedrate_percentage, 10, 999);

  //
  // Manual bed leveling, Bed Z:
  //
  #if ENABLED(MESH_BED_LEVELING) && ENABLED(LCD_BED_LEVELING)
    MENU_ITEM_EDIT(float43, MSG_BED_Z, &mbl.z_offset, -1, 1);
  #endif

  //
  // Nozzle:
  // Nozzle [1-4]:
  //
  #if HOTENDS == 1
    MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(int3, MSG_NOZZLE, &thermalManager.target_temperature[0], 0, HEATER_0_MAXTEMP - 15, watch_temp_callback_E0);
  #else // HOTENDS > 1
    #define EDIT_NOZZLE(N) MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(int3, MSG_NOZZLE MSG_LCD_N##N, &thermalManager.target_temperature[N], 0, HEATER_##N##_MAXTEMP - 15, watch_temp_callback_E##N)
    EDIT_NOZZLE(0);
    EDIT_NOZZLE(1);
    #if HOTENDS > 2
      EDIT_NOZZLE(2);
      #if HOTENDS > 3
        EDIT_NOZZLE(3);
        #if HOTENDS > 4
          EDIT_NOZZLE(4);
          #if HOTENDS > 5
            EDIT_NOZZLE(5);
          #endif // HOTENDS > 5
        #endif // HOTENDS > 4
      #endif // HOTENDS > 3
    #endif // HOTENDS > 2
  #endif // HOTENDS > 1

  //
  // Bed:
  //
  #if HAS_HEATED_BED
    MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(int3, MSG_BED, &thermalManager.target_temperature_bed, 0, BED_MAXTEMP - 15, watch_temp_callback_bed);
  #endif

  //
  // Fan Speed:
  //
  #if FAN_COUNT > 0
    #if HAS_FAN0
      MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(uint8, MSG_FAN_SPEED FAN_SPEED_1_SUFFIX, &thermalManager.lcd_tmpfan_speed[0], 0, 255, thermalManager.lcd_setFanSpeed0);
      #if ENABLED(EXTRA_FAN_SPEED)
        MENU_MULTIPLIER_ITEM_EDIT(uint8, MSG_EXTRA_FAN_SPEED FAN_SPEED_1_SUFFIX, &thermalManager.new_fan_speed[0], 3, 255);
      #endif
    #endif
    #if HAS_FAN1 || (ENABLED(SINGLENOZZLE) && EXTRUDERS > 1)
      MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(uint8, MSG_FAN_SPEED " 2", &thermalManager.lcd_tmpfan_speed[1], 0, 255, thermalManager.lcd_setFanSpeed1);
      #if ENABLED(EXTRA_FAN_SPEED)
        MENU_MULTIPLIER_ITEM_EDIT(uint8, MSG_EXTRA_FAN_SPEED " 2", &thermalManager.new_fan_speed[1], 3, 255);
      #endif
    #endif
    #if HAS_FAN2 || (ENABLED(SINGLENOZZLE) && EXTRUDERS > 2)
      MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(uint8, MSG_FAN_SPEED " 3", &thermalManager.lcd_tmpfan_speed[2], 0, 255, thermalManager.lcd_setFanSpeed2);
      #if ENABLED(EXTRA_FAN_SPEED)
        MENU_MULTIPLIER_ITEM_EDIT(uint8, MSG_EXTRA_FAN_SPEED " 3", &thermalManager.new_fan_speed[2], 3, 255);
      #endif
    #endif
  #endif // FAN_COUNT > 0

  //
  // Flow:
  // Flow [1-5]:
  //
  #if EXTRUDERS == 1
    MENU_ITEM_EDIT_CALLBACK(int3, MSG_FLOW, &planner.flow_percentage[0], 10, 999, _lcd_refresh_e_factor_0);
  #else // EXTRUDERS > 1
    MENU_ITEM_EDIT_CALLBACK(int3, MSG_FLOW, &planner.flow_percentage[active_extruder], 10, 999, _lcd_refresh_e_factor);
    #define EDIT_FLOW(N) MENU_ITEM_EDIT_CALLBACK(int3, MSG_FLOW MSG_LCD_N##N, &planner.flow_percentage[N], 10, 999, _lcd_refresh_e_factor_##N)
    EDIT_FLOW(0);
    EDIT_FLOW(1);
    #if EXTRUDERS > 2
      EDIT_FLOW(2);
      #if EXTRUDERS > 3
        EDIT_FLOW(3);
        #if EXTRUDERS > 4
          EDIT_FLOW(4);
          #if EXTRUDERS > 5
            EDIT_FLOW(5);
          #endif // EXTRUDERS > 5
        #endif // EXTRUDERS > 4
      #endif // EXTRUDERS > 3
    #endif // EXTRUDERS > 2
  #endif // EXTRUDERS > 1

  //
  // Babystep X:
  // Babystep Y:
  // Babystep Z:
  //
  #if ENABLED(BABYSTEPPING)
    #if ENABLED(BABYSTEP_XY)
      MENU_ITEM(submenu, MSG_BABYSTEP_X, lcd_babystep_x);
      MENU_ITEM(submenu, MSG_BABYSTEP_Y, lcd_babystep_y);
    #endif
    #if ENABLED(BABYSTEP_ZPROBE_OFFSET)
      MENU_ITEM(submenu, MSG_ZPROBE_ZOFFSET, lcd_babystep_zoffset);
    #else
      MENU_ITEM(submenu, MSG_BABYSTEP_Z, lcd_babystep_z);
    #endif
  #endif

  END_MENU();
}

#endif // HAS_LCD_MENU
