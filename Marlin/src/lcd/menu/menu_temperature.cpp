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

#include "../../inc/MarlinConfig.h"

#if HAS_LCD_MENU && HAS_TEMPERATURE

#include "menu.h"
#include "../../module/temperature.h"

#if FAN_COUNT > 1 || ENABLED(SINGLENOZZLE)
  #include "../../module/motion.h"
#endif

#if ENABLED(SINGLENOZZLE)
  #include "../../module/tool_change.h"
#endif

//
// "Temperature" submenu items
//

void Temperature::lcd_preheat(const int16_t e, const int8_t indh, const int8_t indb) {
  #if HAS_HOTEND
    if (indh >= 0 && ui.material_preset[indh].hotend_temp > 0)
      setTargetHotend(_MIN(thermalManager.heater_maxtemp[e] - HOTEND_OVERSHOOT, ui.material_preset[indh].hotend_temp), e);
  #else
    UNUSED(e); UNUSED(indh);
  #endif
  #if HAS_HEATED_BED
    if (indb >= 0 && ui.material_preset[indb].bed_temp > 0) setTargetBed(ui.material_preset[indb].bed_temp);
  #else
    UNUSED(indb);
  #endif
  #if HAS_FAN
    set_fan_speed((
      #if FAN_COUNT > 1
        active_extruder < FAN_COUNT ? active_extruder :
      #endif
      0), ui.material_preset[indh].fan_speed
    );
  #endif
  ui.return_to_status();
}

#if HAS_TEMP_HOTEND
  inline void _preheat_end(const uint8_t m, const uint8_t e) { thermalManager.lcd_preheat(e, m, -1); }
  #if HAS_HEATED_BED
    inline void _preheat_both(const uint8_t m, const uint8_t e) { thermalManager.lcd_preheat(e, m, m); }
  #endif
#endif
#if HAS_HEATED_BED
  inline void _preheat_bed(const uint8_t m) { thermalManager.lcd_preheat(-1, -1, m); }
#endif

#if HAS_TEMP_HOTEND || HAS_HEATED_BED

  #if HAS_TEMP_HOTEND && HAS_HEATED_BED

    // Indexed "Preheat ABC" and "Heat Bed" items
    #define PREHEAT_ITEMS(M,E) do{ \
      ACTION_ITEM_N_P(E, msg_preheat_h[M], []{ _preheat_both(M, MenuItemBase::itemIndex); }); \
      ACTION_ITEM_N_P(E, msg_preheat_end_e[M], []{ _preheat_end(M, MenuItemBase::itemIndex); }); \
    }while(0)

  #elif HAS_MULTI_HOTEND

    // No heated bed, so just indexed "Preheat ABC" items
    #define PREHEAT_ITEMS(M,E) ACTION_ITEM_N_P(E, msg_preheat_h[M], []{ _preheat_end(M, MenuItemBase::itemIndex); })

  #endif

  void menu_preheat_m(const uint8_t m) {

    #if HOTENDS == 1
      PGM_P msg_preheat[] = ARRAY_N(PREHEAT_COUNT, GET_TEXT(MSG_PREHEAT_1), GET_TEXT(MSG_PREHEAT_2), GET_TEXT(MSG_PREHEAT_3), GET_TEXT(MSG_PREHEAT_4), GET_TEXT(MSG_PREHEAT_5));
      #if HAS_HEATED_BED
        PGM_P msg_preheat_end[] = ARRAY_N(PREHEAT_COUNT, GET_TEXT(MSG_PREHEAT_1_END), GET_TEXT(MSG_PREHEAT_2_END), GET_TEXT(MSG_PREHEAT_3_END), GET_TEXT(MSG_PREHEAT_4_END), GET_TEXT(MSG_PREHEAT_5_END));
      #endif
    #elif HAS_MULTI_HOTEND
      PGM_P msg_preheat_all[] = ARRAY_N(PREHEAT_COUNT, GET_TEXT(MSG_PREHEAT_1_ALL), GET_TEXT(MSG_PREHEAT_2_ALL), GET_TEXT(MSG_PREHEAT_3_ALL), GET_TEXT(MSG_PREHEAT_4_ALL), GET_TEXT(MSG_PREHEAT_5_ALL));
    #endif

    #if HAS_TEMP_HOTEND && HAS_HEATED_BED && HAS_MULTI_HOTEND
      PGM_P msg_preheat_end_e[] = ARRAY_N(PREHEAT_COUNT, GET_TEXT(MSG_PREHEAT_1_END_E), GET_TEXT(MSG_PREHEAT_2_END_E), GET_TEXT(MSG_PREHEAT_3_END_E), GET_TEXT(MSG_PREHEAT_4_END_E), GET_TEXT(MSG_PREHEAT_5_END_E));
    #endif

    #if HAS_MULTI_HOTEND
      PGM_P msg_preheat_h[] = ARRAY_N(PREHEAT_COUNT, GET_TEXT(MSG_PREHEAT_1_H), GET_TEXT(MSG_PREHEAT_2_H), GET_TEXT(MSG_PREHEAT_3_H), GET_TEXT(MSG_PREHEAT_4_H), GET_TEXT(MSG_PREHEAT_5_H));
    #endif

    MenuItemBase::itemIndex = m;

    START_MENU();
    BACK_ITEM(MSG_TEMPERATURE);

    #if HOTENDS == 1

      #if HAS_HEATED_BED
        ACTION_ITEM_P(msg_preheat[m], []{ _preheat_both(MenuItemBase::itemIndex, 0); });
        ACTION_ITEM_P(msg_preheat_end[m], []{ _preheat_end(MenuItemBase::itemIndex, 0); });
      #else
        ACTION_ITEM_P(msg_preheat[m], []{ _preheat_end(MenuItemBase::itemIndex, 0); });
      #endif

    #elif HAS_MULTI_HOTEND

      LOOP_S_L_N(n, 0, HOTENDS) PREHEAT_ITEMS(MenuItemBase::itemIndex, n);
      ACTION_ITEM_P(msg_preheat_all[m], []() {
        TERN_(HAS_HEATED_BED, _preheat_bed(MenuItemBase::itemIndex));
        HOTEND_LOOP() thermalManager.setTargetHotend(ui.material_preset[MenuItemBase::itemIndex].hotend_temp, e);
      });

    #endif

    #if HAS_HEATED_BED
      PGM_P msg_preheat_bed[] = ARRAY_N(PREHEAT_COUNT, GET_TEXT(MSG_PREHEAT_1_BEDONLY), GET_TEXT(MSG_PREHEAT_2_BEDONLY), GET_TEXT(MSG_PREHEAT_3_BEDONLY), GET_TEXT(MSG_PREHEAT_4_BEDONLY), GET_TEXT(MSG_PREHEAT_5_BEDONLY));
      ACTION_ITEM_P(msg_preheat_bed[m], []{ _preheat_bed(MenuItemBase::itemIndex); });
    #endif

    END_MENU();
  }

  void menu_preheat_m1() { menu_preheat_m(0); }
  void menu_preheat_m2() { menu_preheat_m(1); }
  #if PREHEAT_COUNT >= 3
    void menu_preheat_m3() { menu_preheat_m(2); }
    #if PREHEAT_COUNT >= 4
      void menu_preheat_m4() { menu_preheat_m(3); }
      #if PREHEAT_COUNT >= 5
        void menu_preheat_m5() { menu_preheat_m(4); }
      #endif
    #endif
  #endif

  void lcd_cooldown() {
    thermalManager.zero_fan_speeds();
    thermalManager.disable_all_heaters();
    ui.return_to_status();
  }

#endif // HAS_TEMP_HOTEND || HAS_HEATED_BED

void menu_temperature() {
  #if HAS_TEMP_HOTEND
    bool has_heat = false;
    HOTEND_LOOP() if (thermalManager.temp_hotend[HOTEND_INDEX].target) { has_heat = true; break; }
  #endif

  START_MENU();
  BACK_ITEM(MSG_MAIN);

  //
  // Nozzle:
  // Nozzle [1-5]:
  //
  #if HOTENDS == 1
    EDIT_ITEM_FAST(int3, MSG_NOZZLE, &thermalManager.temp_hotend[0].target, 0, HEATER_0_MAXTEMP - HOTEND_OVERSHOOT, []{ thermalManager.start_watching_hotend(0); });
  #elif HAS_MULTI_HOTEND
    HOTEND_LOOP()
      EDIT_ITEM_FAST_N(int3, e, MSG_NOZZLE_N, &thermalManager.temp_hotend[e].target, 0, thermalManager.heater_maxtemp[e] - HOTEND_OVERSHOOT, []{ thermalManager.start_watching_hotend(MenuItemBase::itemIndex); });
  #endif

  #if ENABLED(SINGLENOZZLE_STANDBY_TEMP)
    LOOP_S_L_N(e, 1, EXTRUDERS)
      EDIT_ITEM_FAST_N(uint16_3, e, MSG_NOZZLE_STANDBY, &singlenozzle_temp[e], 0, thermalManager.heater_maxtemp[0] - HOTEND_OVERSHOOT);
  #endif

  //
  // Bed:
  //
  #if HAS_HEATED_BED
    EDIT_ITEM_FAST(int3, MSG_BED, &thermalManager.temp_bed.target, 0, BED_MAX_TARGET, thermalManager.start_watching_bed);
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
  #if HAS_FAN

    auto on_fan_update = []{
      thermalManager.set_fan_speed(MenuItemBase::itemIndex, editable.uint8);
    };

    #if ENABLED(EXTRA_FAN_SPEED)
      #define EDIT_EXTRA_FAN_SPEED(V...) EDIT_ITEM_FAST_N(V)
    #else
      #define EDIT_EXTRA_FAN_SPEED(...)
    #endif

    #if FAN_COUNT > 1
      #define FAN_EDIT_ITEMS(F) do{ \
        editable.uint8 = thermalManager.fan_speed[F]; \
        EDIT_ITEM_FAST_N(percent, F, MSG_FAN_SPEED_N, &editable.uint8, 0, 255, on_fan_update); \
        EDIT_EXTRA_FAN_SPEED(percent, F, MSG_EXTRA_FAN_SPEED_N, &thermalManager.new_fan_speed[F], 3, 255); \
      }while(0)
    #endif

    #define SNFAN(N) (ENABLED(SINGLENOZZLE_STANDBY_FAN) && !HAS_FAN##N && EXTRUDERS > N)
    #if SNFAN(1) || SNFAN(2) || SNFAN(3) || SNFAN(4) || SNFAN(5) || SNFAN(6) || SNFAN(7)
      auto singlenozzle_item = [&](const uint8_t f) {
        editable.uint8 = singlenozzle_fan_speed[f];
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
      FAN_EDIT_ITEMS(1);
    #elif SNFAN(1)
      singlenozzle_item(1);
    #endif
    #if HAS_FAN2
      FAN_EDIT_ITEMS(2);
    #elif SNFAN(2)
      singlenozzle_item(1);
    #endif
    #if HAS_FAN3
      FAN_EDIT_ITEMS(3);
    #elif SNFAN(3)
      singlenozzle_item(1);
    #endif
    #if HAS_FAN4
      FAN_EDIT_ITEMS(4);
    #elif SNFAN(4)
      singlenozzle_item(1);
    #endif
    #if HAS_FAN5
      FAN_EDIT_ITEMS(5);
    #elif SNFAN(5)
      singlenozzle_item(1);
    #endif
    #if HAS_FAN6
      FAN_EDIT_ITEMS(6);
    #elif SNFAN(6)
      singlenozzle_item(1);
    #endif
    #if HAS_FAN7
      FAN_EDIT_ITEMS(7);
    #elif SNFAN(7)
      singlenozzle_item(1);
    #endif

  #endif // HAS_FAN

  #if HAS_TEMP_HOTEND

    //
    // Preheat for Material 1 and 2
    //
    #if HOTENDS > 1 || HAS_HEATED_BED
      SUBMENU(MSG_PREHEAT_1, menu_preheat_m1);
      SUBMENU(MSG_PREHEAT_2, menu_preheat_m2);
      #if PREHEAT_COUNT >= 3
        SUBMENU(MSG_PREHEAT_3, menu_preheat_m3);
        #if PREHEAT_COUNT >= 4
          SUBMENU(MSG_PREHEAT_4, menu_preheat_m4);
          #if PREHEAT_COUNT >= 5
            SUBMENU(MSG_PREHEAT_5, menu_preheat_m5);
          #endif
        #endif
      #endif
    #else
      ACTION_ITEM(MSG_PREHEAT_1, []{ _preheat_end(0, 0); });
      ACTION_ITEM(MSG_PREHEAT_2, []{ _preheat_end(1, 0); });
      #if PREHEAT_COUNT >= 3
        ACTION_ITEM(MSG_PREHEAT_3, []{ _preheat_end(2, 0); });
        #if PREHEAT_COUNT >= 3
          ACTION_ITEM(MSG_PREHEAT_4, []{ _preheat_end(3, 0); });
          #if PREHEAT_COUNT >= 3
            ACTION_ITEM(MSG_PREHEAT_5, []{ _preheat_end(4, 0); });
          #endif
        #endif
      #endif
    #endif

    //
    // Cooldown
    //
    if (TERN0(HAS_HEATED_BED, thermalManager.temp_bed.target)) has_heat = true;
    if (has_heat) ACTION_ITEM(MSG_COOLDOWN, lcd_cooldown);

  #endif // HAS_TEMP_HOTEND

  END_MENU();
}

#endif // HAS_LCD_MENU && HAS_TEMPERATURE
