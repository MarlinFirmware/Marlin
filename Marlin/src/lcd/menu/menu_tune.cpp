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

#if ENABLED(SINGLENOZZLE)
  #include "../../module/tool_change.h"
#endif

// Refresh the E factor after changing flow
#if EXTRUDERS
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
#endif // EXTRUDERS

#if ENABLED(BABYSTEPPING)

  #include "../../feature/babystep.h"
  #include "../lcdprint.h"
  #if HAS_GRAPHICAL_LCD
    #include "../dogm/ultralcd_DOGM.h"
  #endif

  void _lcd_babystep(const AxisEnum axis, PGM_P const msg) {
    if (ui.use_click()) return ui.goto_previous_screen_no_defer();
    if (ui.encoderPosition) {
      const int16_t steps = int16_t(ui.encoderPosition) * (BABYSTEP_MULTIPLICATOR);
      ui.encoderPosition = 0;
      ui.refresh(LCDVIEW_REDRAW_NOW);
      babystep.add_steps(axis, steps);
    }
    if (ui.should_draw()) {
      const float spm = planner.steps_to_mm[axis];
      draw_edit_screen(msg, ftostr54sign(spm * babystep.accum));
      #if ENABLED(BABYSTEP_DISPLAY_TOTAL)
        const bool in_view = (true
          #if HAS_GRAPHICAL_LCD
            && PAGE_CONTAINS(LCD_PIXEL_HEIGHT - MENU_FONT_HEIGHT, LCD_PIXEL_HEIGHT - 1)
          #endif
        );
        if (in_view) {
          #if HAS_GRAPHICAL_LCD
            ui.set_font(FONT_MENU);
            lcd_moveto(0, LCD_PIXEL_HEIGHT - MENU_FONT_DESCENT);
          #else
            lcd_moveto(0, LCD_HEIGHT - 1);
          #endif
          lcd_put_u8str_P(PSTR(MSG_BABYSTEP_TOTAL ":"));
          lcd_put_u8str(ftostr54sign(spm * babystep.axis_total[BS_TOTAL_AXIS(axis)]));
        }
      #endif
    }
  }

  inline void _lcd_babystep_go(const screenFunc_t screen) {
    ui.goto_screen(screen);
    ui.defer_status_screen();
    babystep.accum = 0;
  }

  #if ENABLED(BABYSTEP_XY)
    void _lcd_babystep_x() { _lcd_babystep(X_AXIS, PSTR(MSG_BABYSTEP_X)); }
    void _lcd_babystep_y() { _lcd_babystep(Y_AXIS, PSTR(MSG_BABYSTEP_Y)); }
    void lcd_babystep_x() { _lcd_babystep_go(_lcd_babystep_x); }
    void lcd_babystep_y() { _lcd_babystep_go(_lcd_babystep_y); }
  #endif

  #if DISABLED(BABYSTEP_ZPROBE_OFFSET)
    void _lcd_babystep_z() { _lcd_babystep(Z_AXIS, PSTR(MSG_BABYSTEP_Z)); }
    void lcd_babystep_z() { _lcd_babystep_go(_lcd_babystep_z); }
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
  #if BOTH(MESH_BED_LEVELING, LCD_BED_LEVELING)
    MENU_ITEM_EDIT(float43, MSG_BED_Z, &mbl.z_offset, -1, 1);
  #endif

  //
  // Nozzle:
  // Nozzle [1-4]:
  //
  #if HOTENDS == 1
    MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(int3, MSG_NOZZLE, &thermalManager.temp_hotend[0].target, 0, HEATER_0_MAXTEMP - 15, thermalManager.start_watching_E0);
  #elif HOTENDS > 1
    #define EDIT_NOZZLE(N) MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(int3, MSG_NOZZLE MSG_LCD_N##N, &thermalManager.temp_hotend[N].target, 0, HEATER_##N##_MAXTEMP - 15, thermalManager.start_watching_E##N)
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

  #if ENABLED(SINGLENOZZLE)
    MENU_MULTIPLIER_ITEM_EDIT(uint16_3, MSG_NOZZLE_STANDBY, &singlenozzle_temp[active_extruder ? 0 : 1], 0, HEATER_0_MAXTEMP - 15);
  #endif

  //
  // Bed:
  //
  #if HAS_HEATED_BED
    MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(int3, MSG_BED, &thermalManager.temp_bed.target, 0, BED_MAXTEMP - 10, thermalManager.start_watching_bed);
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
  #elif EXTRUDERS
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
  #endif // EXTRUDERS

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
