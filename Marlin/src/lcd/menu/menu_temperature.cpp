/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
  if (temph > 0) thermalManager.setTargetHotend(MIN(heater_maxtemp[endnum] - 15, temph), endnum);
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

#if HOTENDS > 1

  void lcd_preheat_m1_e1_only() { _lcd_preheat(1, ui.preheat_hotend_temp[0], -1, ui.preheat_fan_speed[0]); }
  void lcd_preheat_m2_e1_only() { _lcd_preheat(1, ui.preheat_hotend_temp[1], -1, ui.preheat_fan_speed[1]); }
  #if HAS_HEATED_BED
    void lcd_preheat_m1_e1() { _lcd_preheat(1, ui.preheat_hotend_temp[0], ui.preheat_bed_temp[0], ui.preheat_fan_speed[0]); }
    void lcd_preheat_m2_e1() { _lcd_preheat(1, ui.preheat_hotend_temp[1], ui.preheat_bed_temp[1], ui.preheat_fan_speed[1]); }
  #endif
  #if HOTENDS > 2
    void lcd_preheat_m1_e2_only() { _lcd_preheat(2, ui.preheat_hotend_temp[0], -1, ui.preheat_fan_speed[0]); }
    void lcd_preheat_m2_e2_only() { _lcd_preheat(2, ui.preheat_hotend_temp[1], -1, ui.preheat_fan_speed[1]); }
    #if HAS_HEATED_BED
      void lcd_preheat_m1_e2() { _lcd_preheat(2, ui.preheat_hotend_temp[0], ui.preheat_bed_temp[0], ui.preheat_fan_speed[0]); }
      void lcd_preheat_m2_e2() { _lcd_preheat(2, ui.preheat_hotend_temp[1], ui.preheat_bed_temp[1], ui.preheat_fan_speed[1]); }
    #endif
    #if HOTENDS > 3
      void lcd_preheat_m1_e3_only() { _lcd_preheat(3, ui.preheat_hotend_temp[0], -1, ui.preheat_fan_speed[0]); }
      void lcd_preheat_m2_e3_only() { _lcd_preheat(3, ui.preheat_hotend_temp[1], -1, ui.preheat_fan_speed[1]); }
      #if HAS_HEATED_BED
        void lcd_preheat_m1_e3() { _lcd_preheat(3, ui.preheat_hotend_temp[0], ui.preheat_bed_temp[0], ui.preheat_fan_speed[0]); }
        void lcd_preheat_m2_e3() { _lcd_preheat(3, ui.preheat_hotend_temp[1], ui.preheat_bed_temp[1], ui.preheat_fan_speed[1]); }
      #endif
      #if HOTENDS > 4
        void lcd_preheat_m1_e4_only() { _lcd_preheat(4, ui.preheat_hotend_temp[0], -1, ui.preheat_fan_speed[0]); }
        void lcd_preheat_m2_e4_only() { _lcd_preheat(4, ui.preheat_hotend_temp[1], -1, ui.preheat_fan_speed[1]); }
        #if HAS_HEATED_BED
          void lcd_preheat_m1_e4() { _lcd_preheat(4, ui.preheat_hotend_temp[0], ui.preheat_bed_temp[0], ui.preheat_fan_speed[0]); }
          void lcd_preheat_m2_e4() { _lcd_preheat(4, ui.preheat_hotend_temp[1], ui.preheat_bed_temp[1], ui.preheat_fan_speed[1]); }
        #endif
        #if HOTENDS > 5
          void lcd_preheat_m1_e5_only() { _lcd_preheat(5, ui.preheat_hotend_temp[0], -1, ui.preheat_fan_speed[0]); }
          void lcd_preheat_m2_e5_only() { _lcd_preheat(5, ui.preheat_hotend_temp[1], -1, ui.preheat_fan_speed[1]); }
          #if HAS_HEATED_BED
            void lcd_preheat_m1_e5() { _lcd_preheat(5, ui.preheat_hotend_temp[0], ui.preheat_bed_temp[0], ui.preheat_fan_speed[0]); }
            void lcd_preheat_m2_e5() { _lcd_preheat(5, ui.preheat_hotend_temp[1], ui.preheat_bed_temp[1], ui.preheat_fan_speed[1]); }
          #endif
        #endif // HOTENDS > 5
      #endif // HOTENDS > 4
    #endif // HOTENDS > 3
  #endif // HOTENDS > 2

  #if HAS_HEATED_BED
    void lcd_preheat_m1_e0();
    void lcd_preheat_m2_e0();
  #else
    void lcd_preheat_m1_e0_only();
    void lcd_preheat_m2_e0_only();
  #endif

  void lcd_preheat_m1_all() {
    #if HOTENDS > 1
      thermalManager.setTargetHotend(ui.preheat_hotend_temp[0], 1);
      #if HOTENDS > 2
        thermalManager.setTargetHotend(ui.preheat_hotend_temp[0], 2);
        #if HOTENDS > 3
          thermalManager.setTargetHotend(ui.preheat_hotend_temp[0], 3);
          #if HOTENDS > 4
            thermalManager.setTargetHotend(ui.preheat_hotend_temp[0], 4);
            #if HOTENDS > 5
              thermalManager.setTargetHotend(ui.preheat_hotend_temp[0], 5);
            #endif // HOTENDS > 5
          #endif // HOTENDS > 4
        #endif // HOTENDS > 3
      #endif // HOTENDS > 2
    #endif // HOTENDS > 1
    #if HAS_HEATED_BED
      lcd_preheat_m1_e0();
    #else
      lcd_preheat_m1_e0_only();
    #endif
  }

  void lcd_preheat_m2_all() {
    #if HOTENDS > 1
      thermalManager.setTargetHotend(ui.preheat_hotend_temp[1], 1);
      #if HOTENDS > 2
        thermalManager.setTargetHotend(ui.preheat_hotend_temp[1], 2);
        #if HOTENDS > 3
          thermalManager.setTargetHotend(ui.preheat_hotend_temp[1], 3);
          #if HOTENDS > 4
            thermalManager.setTargetHotend(ui.preheat_hotend_temp[1], 4);
            #if HOTENDS > 5
              thermalManager.setTargetHotend(ui.preheat_hotend_temp[1], 5);
            #endif // HOTENDS > 5
          #endif // HOTENDS > 4
        #endif // HOTENDS > 3
      #endif // HOTENDS > 2
    #endif // HOTENDS > 1
    #if HAS_HEATED_BED
      lcd_preheat_m2_e0();
    #else
      lcd_preheat_m2_e0_only();
    #endif
  }

#endif // HOTENDS > 1

#if HAS_TEMP_HOTEND || HAS_HEATED_BED

  void lcd_preheat_m1_e0_only() { _lcd_preheat(0, ui.preheat_hotend_temp[0], -1, ui.preheat_fan_speed[0]); }
  void lcd_preheat_m2_e0_only() { _lcd_preheat(0, ui.preheat_hotend_temp[1], -1, ui.preheat_fan_speed[1]); }

  #if HAS_HEATED_BED
    void lcd_preheat_m1_e0() { _lcd_preheat(0, ui.preheat_hotend_temp[0], ui.preheat_bed_temp[0], ui.preheat_fan_speed[0]); }
    void lcd_preheat_m2_e0() { _lcd_preheat(0, ui.preheat_hotend_temp[1], ui.preheat_bed_temp[1], ui.preheat_fan_speed[1]); }
    void lcd_preheat_m1_bedonly() { _lcd_preheat(0, 0, ui.preheat_bed_temp[0], ui.preheat_fan_speed[0]); }
    void lcd_preheat_m2_bedonly() { _lcd_preheat(0, 0, ui.preheat_bed_temp[1], ui.preheat_fan_speed[1]); }
  #endif

  void menu_preheat_m1() {
    START_MENU();
    MENU_BACK(MSG_TEMPERATURE);
    #if HOTENDS == 1
      #if HAS_HEATED_BED
        MENU_ITEM(function, MSG_PREHEAT_1, lcd_preheat_m1_e0);
        MENU_ITEM(function, MSG_PREHEAT_1_END, lcd_preheat_m1_e0_only);
      #else
        MENU_ITEM(function, MSG_PREHEAT_1, lcd_preheat_m1_e0_only);
      #endif
    #elif HOTENDS > 1
      #if HAS_HEATED_BED
        MENU_ITEM(function, MSG_PREHEAT_1_N MSG_H1, lcd_preheat_m1_e0);
        MENU_ITEM(function, MSG_PREHEAT_1_END " " MSG_E1, lcd_preheat_m1_e0_only);
        MENU_ITEM(function, MSG_PREHEAT_1_N MSG_H2, lcd_preheat_m1_e1);
        MENU_ITEM(function, MSG_PREHEAT_1_END " " MSG_E2, lcd_preheat_m1_e1_only);
      #else
        MENU_ITEM(function, MSG_PREHEAT_1_N MSG_H1, lcd_preheat_m1_e0_only);
        MENU_ITEM(function, MSG_PREHEAT_1_N MSG_H2, lcd_preheat_m1_e1_only);
      #endif
      #if HOTENDS > 2
        #if HAS_HEATED_BED
          MENU_ITEM(function, MSG_PREHEAT_1_N MSG_H3, lcd_preheat_m1_e2);
          MENU_ITEM(function, MSG_PREHEAT_1_END " " MSG_E3, lcd_preheat_m1_e2_only);
        #else
          MENU_ITEM(function, MSG_PREHEAT_1_N MSG_H3, lcd_preheat_m1_e2_only);
        #endif
        #if HOTENDS > 3
          #if HAS_HEATED_BED
            MENU_ITEM(function, MSG_PREHEAT_1_N MSG_H4, lcd_preheat_m1_e3);
            MENU_ITEM(function, MSG_PREHEAT_1_END " " MSG_E4, lcd_preheat_m1_e3_only);
          #else
            MENU_ITEM(function, MSG_PREHEAT_1_N MSG_H4, lcd_preheat_m1_e3_only);
          #endif
          #if HOTENDS > 4
            #if HAS_HEATED_BED
              MENU_ITEM(function, MSG_PREHEAT_1_N MSG_H5, lcd_preheat_m1_e4);
              MENU_ITEM(function, MSG_PREHEAT_1_END " " MSG_E5, lcd_preheat_m1_e4_only);
            #else
              MENU_ITEM(function, MSG_PREHEAT_1_N MSG_H5, lcd_preheat_m1_e4_only);
            #endif
            #if HOTENDS > 5
              #if HAS_HEATED_BED
                MENU_ITEM(function, MSG_PREHEAT_1_N MSG_H6, lcd_preheat_m1_e5);
                MENU_ITEM(function, MSG_PREHEAT_1_END " " MSG_E6, lcd_preheat_m1_e5_only);
              #else
                MENU_ITEM(function, MSG_PREHEAT_1_N MSG_H6, lcd_preheat_m1_e5_only);
              #endif
            #endif // HOTENDS > 5
          #endif // HOTENDS > 4
        #endif // HOTENDS > 3
      #endif // HOTENDS > 2
      MENU_ITEM(function, MSG_PREHEAT_1_ALL, lcd_preheat_m1_all);
    #endif // HOTENDS > 1
    #if HAS_HEATED_BED
      MENU_ITEM(function, MSG_PREHEAT_1_BEDONLY, lcd_preheat_m1_bedonly);
    #endif
    END_MENU();
  }

  void menu_preheat_m2() {
    START_MENU();
    MENU_BACK(MSG_TEMPERATURE);
    #if HOTENDS == 1
      #if HAS_HEATED_BED
        MENU_ITEM(function, MSG_PREHEAT_2, lcd_preheat_m2_e0);
        MENU_ITEM(function, MSG_PREHEAT_2_END, lcd_preheat_m2_e0_only);
      #else
        MENU_ITEM(function, MSG_PREHEAT_2, lcd_preheat_m2_e0_only);
      #endif
    #elif HOTENDS > 1
      #if HAS_HEATED_BED
        MENU_ITEM(function, MSG_PREHEAT_2_N MSG_H1, lcd_preheat_m2_e0);
        MENU_ITEM(function, MSG_PREHEAT_2_END " " MSG_E1, lcd_preheat_m2_e0_only);
        MENU_ITEM(function, MSG_PREHEAT_2_N MSG_H2, lcd_preheat_m2_e1);
        MENU_ITEM(function, MSG_PREHEAT_2_END " " MSG_E2, lcd_preheat_m2_e1_only);
      #else
        MENU_ITEM(function, MSG_PREHEAT_2_N MSG_H1, lcd_preheat_m2_e0_only);
        MENU_ITEM(function, MSG_PREHEAT_2_N MSG_H2, lcd_preheat_m2_e1_only);
      #endif
      #if HOTENDS > 2
        #if HAS_HEATED_BED
          MENU_ITEM(function, MSG_PREHEAT_2_N MSG_H3, lcd_preheat_m2_e2);
          MENU_ITEM(function, MSG_PREHEAT_2_END " " MSG_E3, lcd_preheat_m2_e2_only);
        #else
          MENU_ITEM(function, MSG_PREHEAT_2_N MSG_H3, lcd_preheat_m2_e2_only);
        #endif
        #if HOTENDS > 3
          #if HAS_HEATED_BED
            MENU_ITEM(function, MSG_PREHEAT_2_N MSG_H4, lcd_preheat_m2_e3);
            MENU_ITEM(function, MSG_PREHEAT_2_END " " MSG_E4, lcd_preheat_m2_e3_only);
          #else
            MENU_ITEM(function, MSG_PREHEAT_2_N MSG_H4, lcd_preheat_m2_e3_only);
          #endif
          #if HOTENDS > 4
            #if HAS_HEATED_BED
              MENU_ITEM(function, MSG_PREHEAT_2_N MSG_H5, lcd_preheat_m2_e4);
              MENU_ITEM(function, MSG_PREHEAT_2_END " " MSG_E5, lcd_preheat_m2_e4_only);
            #else
              MENU_ITEM(function, MSG_PREHEAT_2_N MSG_H5, lcd_preheat_m2_e4_only);
            #endif
            #if HOTENDS > 5
              #if HAS_HEATED_BED
                MENU_ITEM(function, MSG_PREHEAT_2_N MSG_H6, lcd_preheat_m2_e5);
                MENU_ITEM(function, MSG_PREHEAT_2_END " " MSG_E6, lcd_preheat_m2_e5_only);
              #else
                MENU_ITEM(function, MSG_PREHEAT_2_N MSG_H6, lcd_preheat_m2_e5_only);
              #endif
            #endif // HOTENDS > 5
          #endif // HOTENDS > 4
        #endif // HOTENDS > 3
      #endif // HOTENDS > 2
      MENU_ITEM(function, MSG_PREHEAT_2_ALL, lcd_preheat_m2_all);
    #endif // HOTENDS > 1
    #if HAS_HEATED_BED
      MENU_ITEM(function, MSG_PREHEAT_2_BEDONLY, lcd_preheat_m2_bedonly);
    #endif
    END_MENU();
  }

  void lcd_cooldown() {
    thermalManager.zero_fan_speeds();
    thermalManager.disable_all_heaters();
    ui.return_to_status();
  }

#endif // HAS_TEMP_HOTEND || HAS_HEATED_BED

#if ENABLED(SPINDLE_LASER_ENABLE)

  extern uint8_t spindle_laser_power;
  bool spindle_laser_enabled();
  void set_spindle_laser_enabled(const bool enabled);
  #if ENABLED(SPINDLE_LASER_PWM)
    void update_spindle_laser_power();
  #endif

  inline void _lcd_spindle_laser_off() { set_spindle_laser_enabled(false); }
  inline void _lcd_spindle_laser_on(const bool is_M4) {
    #if SPINDLE_DIR_CHANGE
      set_spindle_direction(is_M4);
    #endif
    set_spindle_laser_enabled(true);
  }
  inline void _lcd_spindle_laser_on() { _lcd_spindle_laser_on(false); }
  #if SPINDLE_DIR_CHANGE
    inline void _lcd_spindle_on_reverse() { _lcd_spindle_laser_on(true); }
  #endif

  void menu_spindle_laser() {
    START_MENU();
    MENU_BACK(MSG_MAIN);
    if (spindle_laser_enabled()) {
      #if ENABLED(SPINDLE_LASER_PWM)
        MENU_ITEM_EDIT_CALLBACK(int3, MSG_LASER_POWER, &spindle_laser_power, SPEED_POWER_MIN, SPEED_POWER_MAX, update_spindle_laser_power);
      #endif
      MENU_ITEM(function, MSG_LASER_OFF, _lcd_spindle_laser_off);
    }
    else {
      MENU_ITEM(function, MSG_LASER_ON, _lcd_spindle_laser_on);
      #if SPINDLE_DIR_CHANGE
        MENU_ITEM(function, MSG_SPINDLE_REVERSE, _lcd_spindle_on_reverse);
      #endif
    }
    END_MENU();
  }

#endif // SPINDLE_LASER_ENABLE

void menu_temperature() {
  START_MENU();
  MENU_BACK(MSG_MAIN);

  //
  // Nozzle:
  // Nozzle [1-5]:
  //
  #if HOTENDS == 1
    MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(int3, MSG_NOZZLE, &thermalManager.temp_hotend[0].target, 0, HEATER_0_MAXTEMP - 15, thermalManager.start_watching_E0);
  #else // HOTENDS > 1
    #define EDIT_TARGET(N) MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(int3, MSG_NOZZLE MSG_LCD_N##N, &thermalManager.temp_hotend[N].target, 0, HEATER_##N##_MAXTEMP - 15, thermalManager.start_watching_E##N)
    EDIT_TARGET(0);
    EDIT_TARGET(1);
    #if HOTENDS > 2
      EDIT_TARGET(2);
      #if HOTENDS > 3
        EDIT_TARGET(3);
        #if HOTENDS > 4
          EDIT_TARGET(4);
          #if HOTENDS > 5
            EDIT_TARGET(5);
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
    MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(int3, MSG_BED, &thermalManager.temp_bed.target, 0, BED_MAXTEMP - 5, thermalManager.start_watching_bed);
  #endif

  //
  // Chamber:
  //
  #if HAS_HEATED_CHAMBER
    MENU_MULTIPLIER_ITEM_EDIT_CALLBACK(int3, MSG_CHAMBER, &thermalManager.temp_chamber.target, 0, CHAMBER_MAXTEMP - 5, thermalManager.start_watching_chamber);
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

  #if HAS_TEMP_HOTEND

    //
    // Cooldown
    //
    bool has_heat = false;
    HOTEND_LOOP() if (thermalManager.temp_hotend[HOTEND_INDEX].target) { has_heat = true; break; }
    #if HAS_TEMP_BED
      if (thermalManager.temp_bed.target) has_heat = true;
    #endif
    if (has_heat) MENU_ITEM(function, MSG_COOLDOWN, lcd_cooldown);

    //
    // Preheat for Material 1 and 2
    //
    #if TEMP_SENSOR_1 != 0 || TEMP_SENSOR_2 != 0 || TEMP_SENSOR_3 != 0 || TEMP_SENSOR_4 != 0 || TEMP_SENSOR_5 != 0 || HAS_HEATED_BED
      MENU_ITEM(submenu, MSG_PREHEAT_1, menu_preheat_m1);
      MENU_ITEM(submenu, MSG_PREHEAT_2, menu_preheat_m2);
    #else
      MENU_ITEM(function, MSG_PREHEAT_1, lcd_preheat_m1_e0_only);
      MENU_ITEM(function, MSG_PREHEAT_2, lcd_preheat_m2_e0_only);
    #endif

  #endif // HAS_TEMP_HOTEND

  #if ENABLED(SPINDLE_LASER_ENABLE)
    MENU_ITEM(submenu, MSG_LASER_MENU, menu_spindle_laser);
  #endif

  END_MENU();
}

#endif // HAS_LCD_MENU
