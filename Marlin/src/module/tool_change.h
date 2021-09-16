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
#pragma once

#include "../inc/MarlinConfig.h"

//#define DEBUG_TOOLCHANGE_MIGRATION_FEATURE

#if HAS_MULTI_EXTRUDER

  typedef struct {
    #if ENABLED(TOOLCHANGE_FILAMENT_SWAP)
      float swap_length, extra_prime, extra_resume;
      int16_t prime_speed, retract_speed, unretract_speed, fan, fan_speed, fan_time;
    #endif
    #if ENABLED(TOOLCHANGE_PARK)
      bool enable_park;
      xy_pos_t change_point;
    #endif
    float z_raise;
  } toolchange_settings_t;

  extern toolchange_settings_t toolchange_settings;

  #if ENABLED(TOOLCHANGE_FILAMENT_SWAP)
    void tool_change_prime();
  #endif

  #if ENABLED(TOOLCHANGE_FS_PRIME_FIRST_USED)
    extern bool enable_first_prime;
  #endif

  #if ENABLED(TOOLCHANGE_FS_INIT_BEFORE_SWAP)
    extern bool toolchange_extruder_ready[EXTRUDERS];
  #endif

  #if ENABLED(TOOLCHANGE_MIGRATION_FEATURE)
    typedef struct {
      uint8_t target, last;
      bool automode, in_progress;
    } migration_settings_t;
    constexpr migration_settings_t migration_defaults = { 0, 0, false, false };
    extern migration_settings_t migration;
    bool extruder_migration();
  #endif
#endif

#if DO_SWITCH_EXTRUDER
  void move_extruder_servo(const uint8_t e);
#endif

#if ENABLED(SWITCHING_NOZZLE)
  #if SWITCHING_NOZZLE_TWO_SERVOS
    void lower_nozzle(const uint8_t e);
    void raise_nozzle(const uint8_t e);
  #else
    void move_nozzle_servo(const uint8_t angle_index);
  #endif
#endif

#if ENABLED(PARKING_EXTRUDER)

  void pe_solenoid_set_pin_state(const uint8_t extruder_num, const uint8_t state);

  #define PE_MAGNET_ON_STATE TERN_(PARKING_EXTRUDER_SOLENOIDS_INVERT, !)PARKING_EXTRUDER_SOLENOIDS_PINS_ACTIVE
  inline void pe_solenoid_magnet_on(const uint8_t extruder_num)  { pe_solenoid_set_pin_state(extruder_num,  PE_MAGNET_ON_STATE); }
  inline void pe_solenoid_magnet_off(const uint8_t extruder_num) { pe_solenoid_set_pin_state(extruder_num, !PE_MAGNET_ON_STATE); }

  void pe_solenoid_init();

  extern bool extruder_parked;
  inline void parking_extruder_set_parked(const bool parked) { extruder_parked = parked; }
  bool parking_extruder_unpark_after_homing(const uint8_t final_tool, bool homed_towards_final_tool);

#elif ENABLED(MAGNETIC_PARKING_EXTRUDER)

  typedef struct MPESettings {
      float parking_xpos[2],      // M951 L R
            grab_distance;        // M951 I
 feedRate_t slow_feedrate,        // M951 J
            fast_feedrate;        // M951 H
      float travel_distance,      // M951 D
            compensation_factor;  // M951 C
  } mpe_settings_t;

  extern mpe_settings_t mpe_settings;

  void mpe_settings_init();

#endif

#if ENABLED(MANUAL_SWITCHING_TOOLHEAD)
  const char tool_name_0[] PROGMEM = TOOL_NAME_0;
  const char tool_name_1[] PROGMEM = TOOL_NAME_1;

  #if SWITCHING_TOOLHEAD_TOOL_QTY > 2
    const char tool_name_2[] PROGMEM = TOOL_NAME_2;
  #endif
  #if SWITCHING_TOOLHEAD_TOOL_QTY > 3
    const char tool_name_3[] PROGMEM = TOOL_NAME_3;
  #endif
  #if SWITCHING_TOOLHEAD_TOOL_QTY > 4
    const char tool_name_4[] PROGMEM = TOOL_NAME_4;
  #endif
  #if SWITCHING_TOOLHEAD_TOOL_QTY > 5
    const char tool_name_5[] PROGMEM = TOOL_NAME_5;
  #endif
  #if SWITCHING_TOOLHEAD_TOOL_QTY > 6
    const char tool_name_6[] PROGMEM = TOOL_NAME_6;
  #endif
  #if SWITCHING_TOOLHEAD_TOOL_QTY > 7
    const char tool_name_7[] PROGMEM = TOOL_NAME_7;
  #endif

  PGM_P const toolhead_names[SWITCHING_TOOLHEAD_TOOL_QTY] PROGMEM = {
    tool_name_0,
    tool_name_1
    #if SWITCHING_TOOLHEAD_TOOL_QTY > 2
      , tool_name_2
    #endif
    #if SWITCHING_TOOLHEAD_TOOL_QTY > 3
      , tool_name_3
    #endif
    #if SWITCHING_TOOLHEAD_TOOL_QTY > 4
      , tool_name_4
    #endif
    #if SWITCHING_TOOLHEAD_TOOL_QTY > 5
      , tool_name_5
    #endif
    #if SWITCHING_TOOLHEAD_TOOL_QTY > 6
      , tool_name_6
    #endif
    #if SWITCHING_TOOLHEAD_TOOL_QTY > 7
      , tool_name_7
    #endif
  };
#elif ENABLED(ELECTROMAGNETIC_SWITCHING_TOOLHEAD)
  void electromagnetic_toolchange_init();
#elif ENABLED(SERVO_SWITCHING_TOOLHEAD)
  void servo_toolchange_init();
#endif

#if ENABLED(TOOL_SENSOR)
  uint8_t check_tool_sensor_stats(const uint8_t active_tool, const bool kill_on_error=false, const bool disable=false);
#else
  inline uint8_t check_tool_sensor_stats(const uint8_t, const bool=false, const bool=false) { return 0; }
#endif

/**
 * Perform a tool-change, which may result in moving the
 * previous tool out of the way and the new tool into place.
 */
void tool_change(const uint8_t tmp_extruder, bool no_move=false);
