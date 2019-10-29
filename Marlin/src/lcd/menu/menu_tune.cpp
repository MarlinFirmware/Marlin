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

#if ENABLED(BABYSTEPPING)

  #include "../../feature/babystep.h"
  #include "../lcdprint.h"
  #if HAS_GRAPHICAL_LCD
    #include "../dogm/ultralcd_DOGM.h"
  #endif

  void _lcd_babystep(const AxisEnum axis, PGM_P const msg) {
    if (ui.use_click()) return ui.goto_previous_screen_no_defer();
    if (ui.encoderPosition) {
      const int16_t steps = int16_t(ui.encoderPosition) * (
        #if ENABLED(BABYSTEP_XY)
          axis != Z_AXIS ? BABYSTEP_MULTIPLICATOR_XY :
        #endif
        BABYSTEP_MULTIPLICATOR_Z
      );
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
          lcd_put_u8str_P(GET_TEXT(MSG_BABYSTEP_TOTAL));
          lcd_put_wchar(':');
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
    void _lcd_babystep_x() { _lcd_babystep(X_AXIS, GET_TEXT(MSG_BABYSTEP_X)); }
    void _lcd_babystep_y() { _lcd_babystep(Y_AXIS, GET_TEXT(MSG_BABYSTEP_Y)); }
  #endif

  #if DISABLED(BABYSTEP_ZPROBE_OFFSET)
    void _lcd_babystep_z() { _lcd_babystep(Z_AXIS, GET_TEXT(MSG_BABYSTEP_Z)); }
    void lcd_babystep_z()  { _lcd_babystep_go(_lcd_babystep_z); }
  #endif

#endif // BABYSTEPPING

void menu_tune() {
  START_MENU();
  BACK_ITEM(MSG_MAIN);

  //
  // Speed:
  //
  EDIT_ITEM(int3, MSG_SPEED, &feedrate_percentage, 10, 999);

  //
  // Manual bed leveling, Bed Z:
  //
  #if BOTH(MESH_BED_LEVELING, LCD_BED_LEVELING)
    EDIT_ITEM(float43, MSG_BED_Z, &mbl.z_offset, -1, 1);
  #endif

  //
  // Nozzle:
  // Nozzle [1-4]:
  //
  #if HOTENDS == 1
    EDIT_ITEM_FAST(int3, MSG_NOZZLE, &thermalManager.temp_hotend[0].target, 0, HEATER_0_MAXTEMP - 15, [](){ thermalManager.start_watching_hotend(0); });
  #elif HOTENDS > 1
    #define EDIT_NOZZLE(N) EDIT_ITEM_FAST(int3, MSG_NOZZLE_##N, &thermalManager.temp_hotend[N].target, 0, HEATER_##N##_MAXTEMP - 15, [](){ thermalManager.start_watching_hotend(N); })
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
    EDIT_ITEM_FAST(uint16_3, MSG_NOZZLE_STANDBY, &singlenozzle_temp[active_extruder ? 0 : 1], 0, HEATER_0_MAXTEMP - 15);
  #endif

  //
  // Bed:
  //
  #if HAS_HEATED_BED
    EDIT_ITEM_FAST(int3, MSG_BED, &thermalManager.temp_bed.target, 0, BED_MAXTEMP - 10, thermalManager.start_watching_bed);
  #endif

  //
  // Fan Speed:
  //
  #if FAN_COUNT > 0
    #if HAS_FAN0
      editable.uint8 = thermalManager.fan_speed[0];
      EDIT_ITEM_FAST(percent, MSG_FIRST_FAN_SPEED, &editable.uint8, 0, 255, [](){ thermalManager.set_fan_speed(0, editable.uint8); });
      #if ENABLED(EXTRA_FAN_SPEED)
        EDIT_ITEM_FAST(percent, MSG_FIRST_EXTRA_FAN_SPEED, &thermalManager.new_fan_speed[0], 3, 255);
      #endif
    #endif
    #if HAS_FAN1
      editable.uint8 = thermalManager.fan_speed[1];
      EDIT_ITEM_FAST(percent, MSG_FAN_SPEED_2, &editable.uint8, 0, 255, [](){ thermalManager.set_fan_speed(1, editable.uint8); });
      #if ENABLED(EXTRA_FAN_SPEED)
        EDIT_ITEM_FAST(percent, MSG_EXTRA_FAN_SPEED_2, &thermalManager.new_fan_speed[1], 3, 255);
      #endif
    #endif
    #if HAS_FAN2
      editable.uint8 = thermalManager.fan_speed[2];
      EDIT_ITEM_FAST(percent, MSG_FAN_SPEED_3, &editable.uint8, 0, 255, [](){ thermalManager.set_fan_speed(2, editable.uint8); });
      #if ENABLED(EXTRA_FAN_SPEED)
        EDIT_ITEM_FAST(percent, MSG_EXTRA_FAN_SPEED_3, &thermalManager.new_fan_speed[2], 3, 255);
      #endif
    #endif
  #endif // FAN_COUNT > 0

  //
  // Flow:
  // Flow [1-5]:
  //
  #if EXTRUDERS == 1
    EDIT_ITEM(int3, MSG_FLOW, &planner.flow_percentage[0], 10, 999, [](){ planner.refresh_e_factor(0); });
  #elif EXTRUDERS
    EDIT_ITEM(int3, MSG_FLOW, &planner.flow_percentage[active_extruder], 10, 999, [](){ planner.refresh_e_factor(active_extruder); });
    #define EDIT_FLOW(N) EDIT_ITEM(int3, MSG_FLOW_##N, &planner.flow_percentage[N], 10, 999, [](){ planner.refresh_e_factor(N); })
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
      SUBMENU(MSG_BABYSTEP_X, [](){ _lcd_babystep_go(_lcd_babystep_x); });
      SUBMENU(MSG_BABYSTEP_Y, [](){ _lcd_babystep_go(_lcd_babystep_y); });
    #endif
    #if ENABLED(BABYSTEP_ZPROBE_OFFSET)
      SUBMENU(MSG_ZPROBE_ZOFFSET, lcd_babystep_zoffset);
    #else
      SUBMENU(MSG_BABYSTEP_Z, lcd_babystep_z);
    #endif
  #endif

  END_MENU();
}

#endif // HAS_LCD_MENU
