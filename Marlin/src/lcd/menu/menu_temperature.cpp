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
// Temperature Menu
//

#include "../../inc/MarlinConfigPre.h"

#if HAS_LCD_MENU

#include "menu.h"
#include "../../module/temperature.h"

#if FAN_COUNT > 1 || ENABLED(SINGLENOZZLE)
  #include "../../module/motion.h"
#endif

#if ENABLED(SINGLENOZZLE)
  #include "../../module/tool_change.h"
#endif

// Initialized by settings.load()
int16_t MarlinUI::preheat_hotend_temp[2], MarlinUI::preheat_bed_temp[2];
uint8_t MarlinUI::preheat_fan_speed[2];

//
// "Temperature" submenu items
//

void _lcd_preheat(const int16_t endnum, const int16_t temph, const int16_t tempb, const uint8_t fan) {
  #if HAS_HOTEND
    if (temph > 0) thermalManager.setTargetHotend(_MIN(heater_maxtemp[endnum] - 15, temph), endnum);
  #endif
  #if HAS_HEATED_BED
    if (tempb >= 0) thermalManager.setTargetBed(tempb);
  #else
    UNUSED(tempb);
  #endif
  #if FAN_COUNT > 0
    #if FAN_COUNT > 1
      thermalManager.set_fan_speed(active_extruder < FAN_COUNT ? active_extruder : 0, fan);
    #else
      thermalManager.set_fan_speed(0, fan);
    #endif
  #else
    UNUSED(fan);
  #endif
  ui.return_to_status();
}

#if HAS_TEMP_HOTEND
  inline void _preheat_end(const uint8_t m, const uint8_t e) {
    _lcd_preheat(e, ui.preheat_hotend_temp[m], -1, ui.preheat_fan_speed[m]);
  }
  #if HAS_HEATED_BED
    inline void _preheat_both(const uint8_t m, const uint8_t e) {
      _lcd_preheat(e, ui.preheat_hotend_temp[m], ui.preheat_bed_temp[m], ui.preheat_fan_speed[m]);
    }
  #endif
#endif
#if HAS_HEATED_BED
  inline void _preheat_bed(const uint8_t m) {
    _lcd_preheat(0, 0, ui.preheat_bed_temp[m], ui.preheat_fan_speed[m]);
  }
#endif

#if HAS_TEMP_HOTEND || HAS_HEATED_BED

  #define _PREHEAT_ITEMS(M,N) do{ \
    ACTION_ITEM_N(N, MSG_PREHEAT_##M##_H, []{ _preheat_both(M-1, MenuItemBase::itemIndex); }); \
    ACTION_ITEM_N(N, MSG_PREHEAT_##M##_END_E, []{ _preheat_end(M-1, MenuItemBase::itemIndex); }); \
  }while(0)
  #if HAS_HEATED_BED
    #define PREHEAT_ITEMS(M,N) _PREHEAT_ITEMS(M,N)
  #else
    #define PREHEAT_ITEMS(M,N) \
      ACTION_ITEM_N(N, MSG_PREHEAT_##M##_H, []{ _preheat_end(M-1, MenuItemBase::itemIndex); })
  #endif

  void menu_preheat_m1() {
    START_MENU();
    BACK_ITEM(MSG_TEMPERATURE);
    #if HOTENDS == 1
      #if HAS_HEATED_BED
        ACTION_ITEM(MSG_PREHEAT_1, []{ _preheat_both(0, 0); });
        ACTION_ITEM(MSG_PREHEAT_1_END, []{ _preheat_end(0, 0); });
      #else
        ACTION_ITEM(MSG_PREHEAT_1, []{ _preheat_end(0, 0); });
      #endif
    #elif HAS_MULTI_HOTEND
      #if HAS_HEATED_BED
        _PREHEAT_ITEMS(1,0);
      #endif
      LOOP_S_L_N(n, 1, HOTENDS) PREHEAT_ITEMS(1,n);
      ACTION_ITEM(MSG_PREHEAT_1_ALL, []() {
        TERN_(HAS_HEATED_BED, _preheat_bed(0));
        HOTEND_LOOP() thermalManager.setTargetHotend(ui.preheat_hotend_temp[0], e);
      });
    #endif // HAS_MULTI_HOTEND
    #if HAS_HEATED_BED
      ACTION_ITEM(MSG_PREHEAT_1_BEDONLY, []{ _preheat_bed(0); });
    #endif
    END_MENU();
  }

  void menu_preheat_m2() {
    START_MENU();
    BACK_ITEM(MSG_TEMPERATURE);
    #if HOTENDS == 1
      #if HAS_HEATED_BED
        ACTION_ITEM(MSG_PREHEAT_2, []{ _preheat_both(1, 0); });
        ACTION_ITEM(MSG_PREHEAT_2_END, []{ _preheat_end(1, 0); });
      #else
        ACTION_ITEM(MSG_PREHEAT_2, []{ _preheat_end(1, 0); });
      #endif
    #elif HAS_MULTI_HOTEND
      #if HAS_HEATED_BED
        _PREHEAT_ITEMS(2,0);
      #endif
      LOOP_S_L_N(n, 1, HOTENDS) PREHEAT_ITEMS(2,n);
      ACTION_ITEM(MSG_PREHEAT_2_ALL, []() {
        TERN_(HAS_HEATED_BED, _preheat_bed(1));
        HOTEND_LOOP() thermalManager.setTargetHotend(ui.preheat_hotend_temp[1], e);
      });
    #endif // HAS_MULTI_HOTEND
    #if HAS_HEATED_BED
      ACTION_ITEM(MSG_PREHEAT_2_BEDONLY, []{ _preheat_bed(1); });
    #endif
    END_MENU();
  }

  void lcd_cooldown() {
    thermalManager.zero_fan_speeds();
    thermalManager.disable_all_heaters();
    ui.return_to_status();
  }

#endif // HAS_TEMP_HOTEND || HAS_HEATED_BED

void menu_temperature() {
  START_MENU();
  BACK_ITEM(MSG_MAIN);

  //
  // Nozzle:
  // Nozzle [1-5]:
  //
  #if HOTENDS == 1
    EDIT_ITEM_FAST(int3, MSG_NOZZLE, &thermalManager.temp_hotend[0].target, 0, HEATER_0_MAXTEMP - 15, []{ thermalManager.start_watching_hotend(0); });
  #elif HAS_MULTI_HOTEND
    HOTEND_LOOP()
      EDIT_ITEM_FAST_N(int3, e, MSG_NOZZLE_N, &thermalManager.temp_hotend[e].target, 0, heater_maxtemp[e] - 15, []{ thermalManager.start_watching_hotend(MenuItemBase::itemIndex); });
  #endif

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
  // Chamber:
  //
  #if HAS_HEATED_CHAMBER
    EDIT_ITEM_FAST(int3, MSG_CHAMBER, &thermalManager.temp_chamber.target, 0, CHAMBER_MAXTEMP - 10, thermalManager.start_watching_chamber);
  #endif

  //
  // Fan Speed:
  //
  #if FAN_COUNT > 0

    auto on_fan_update = []{
      thermalManager.set_fan_speed(MenuItemBase::itemIndex, editable.uint8);
    };

    #if HAS_FAN1 || HAS_FAN2 || HAS_FAN3 || HAS_FAN4 || HAS_FAN5 || HAS_FAN6 || HAS_FAN7
      auto fan_edit_items = [&](const uint8_t f) {
        editable.uint8 = thermalManager.fan_speed[f];
        EDIT_ITEM_FAST_N(percent, f, MSG_FAN_SPEED_N, &editable.uint8, 0, 255, on_fan_update);
        #if ENABLED(EXTRA_FAN_SPEED)
          EDIT_ITEM_FAST_N(percent, f, MSG_EXTRA_FAN_SPEED_N, &thermalManager.new_fan_speed[f], 3, 255);
        #endif
      };
    #endif

    #define SNFAN(N) (ENABLED(SINGLENOZZLE) && !HAS_FAN##N && EXTRUDERS > N)
    #if SNFAN(1) || SNFAN(2) || SNFAN(3) || SNFAN(4) || SNFAN(5) || SNFAN(6) || SNFAN(7)
      auto singlenozzle_item = [&](const uint8_t f) {
        editable.uint8 = thermalManager.fan_speed[f];
        EDIT_ITEM_FAST_N(percent, f, MSG_STORED_FAN_N, &editable.uint8, 0, 255, on_fan_update);
      };
    #endif

    #if HAS_FAN0
      editable.uint8 = thermalManager.fan_speed[0];
      EDIT_ITEM_FAST_N(percent, 0, MSG_FIRST_FAN_SPEED, &editable.uint8, 0, 255, on_fan_update);
      #if ENABLED(EXTRA_FAN_SPEED)
        EDIT_ITEM_FAST_N(percent, 0, MSG_FIRST_EXTRA_FAN_SPEED, &thermalManager.new_fan_speed[0], 3, 255);
      #endif
    #endif
    #if HAS_FAN1
      fan_edit_items(1);
    #elif SNFAN(1)
      singlenozzle_item(1);
    #endif
    #if HAS_FAN2
      fan_edit_items(2);
    #elif SNFAN(2)
      singlenozzle_item(1);
    #endif
    #if HAS_FAN3
      fan_edit_items(3);
    #elif SNFAN(3)
      singlenozzle_item(1);
    #endif
    #if HAS_FAN4
      fan_edit_items(4);
    #elif SNFAN(4)
      singlenozzle_item(1);
    #endif
    #if HAS_FAN5
      fan_edit_items(5);
    #elif SNFAN(5)
      singlenozzle_item(1);
    #endif
    #if HAS_FAN6
      fan_edit_items(6);
    #elif SNFAN(6)
      singlenozzle_item(1);
    #endif
    #if HAS_FAN7
      fan_edit_items(7);
    #elif SNFAN(7)
      singlenozzle_item(1);
    #endif

  #endif // FAN_COUNT > 0

  #if HAS_TEMP_HOTEND

    //
    // Preheat for Material 1 and 2
    //
    #if TEMP_SENSOR_1 != 0 || TEMP_SENSOR_2 != 0 || TEMP_SENSOR_3 != 0 || TEMP_SENSOR_4 != 0 || TEMP_SENSOR_5 != 0 || TEMP_SENSOR_6 != 0 || TEMP_SENSOR_7 != 0 || HAS_HEATED_BED
      SUBMENU(MSG_PREHEAT_1, menu_preheat_m1);
      SUBMENU(MSG_PREHEAT_2, menu_preheat_m2);
    #else
      ACTION_ITEM(MSG_PREHEAT_1, []{ _preheat_end(0, 0); });
      ACTION_ITEM(MSG_PREHEAT_2, []{ _preheat_end(1, 0); });
    #endif

    //
    // Cooldown
    //
    bool has_heat = false;
    HOTEND_LOOP() if (thermalManager.temp_hotend[HOTEND_INDEX].target) { has_heat = true; break; }
    if (TERN0(HAS_HEATED_BED, thermalManager.temp_bed.target)) has_heat = true;
    if (has_heat) ACTION_ITEM(MSG_COOLDOWN, lcd_cooldown);

  #endif // HAS_TEMP_HOTEND

  END_MENU();
}

#endif // HAS_LCD_MENU
