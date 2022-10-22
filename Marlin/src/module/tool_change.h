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

  #if ENABLED(TOOLCHANGE_FILAMENT_SWAP)
    // Define any variables required
    extern Flags<EXTRUDERS> extruder_was_primed; // Extruders primed status
  #endif

  typedef struct {
    #if ENABLED(TOOLCHANGE_FILAMENT_SWAP)
      float swap_length;            // M217 S
      float extra_prime;            // M217 E
      float extra_resume;           // M217 B
      int16_t prime_speed;          // M217 P
      int16_t wipe_retract;         // M217 W
      int16_t retract_speed;        // M217 R
      int16_t unretract_speed;      // M217 U
      uint8_t fan_speed;            // M217 F
      uint8_t fan_time;             // M217 D
    #endif
    #if ENABLED(TOOLCHANGE_PARK)
      bool enable_park;             // M216 P
      xyz_pos_t change_point;       // M216 X Y I J K C H O
    #endif
    float z_raise;                  // M217 Z
    bool no_return;                 // M217 O
  } toolchange_settings_t;

  extern toolchange_settings_t toolchange_settings;

  #if ENABLED(TOOLCHANGE_FS_PRIME_FIRST_USED)
    extern bool enable_first_prime; // M217 V
  #endif

#if ENABLED(TOOLCHANGE_FILAMENT_SWAP)
  typedef struct {
    bool swap_mode,     // M217 N
         cut_wipe_mode, // M217 H
         do_swap,
         do_cut_wipe,
         in_progress;
    float resume_e,
          z_raise; // z_raise stored to avoid changing value by lcd between process
    toolchange_settings_t tool_settings;
    } smart_recover_settings_t;

    constexpr smart_recover_settings_t smart_recover_defaults = {
      //swap_mode
      TERN0(TOOLCHANGE_SMART_SWAP, true),
      //cut_wipe_mode
      TERN0(TOOLCHANGE_SMART_CUT_WIPE, true),
      //do_swap
      false,
      //do_cut_wipe
      false,
      // in_progress
      false,
      //Resume E
      0,
      //z_raise
      0
    };

    extern smart_recover_settings_t smart_recover;

  #endif

  #if ENABLED(TOOLCHANGE_MIGRATION_FEATURE)
    typedef struct {
      uint8_t target, last;
      bool automode, swap_only_mode, in_progress;
    } migration_settings_t;
    constexpr migration_settings_t migration_defaults = { 0, 0, false,
                TERN0(TOOLCHANGE_MIGRATION_SWAP_ONLY_MODE, true),
                false };
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

#if ENABLED(ELECTROMAGNETIC_SWITCHING_TOOLHEAD)
  void est_init();
#elif ENABLED(SWITCHING_TOOLHEAD)
  void swt_init();
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

/**
 * Recover active extruder after toolchange
 */
#if ENABLED(TOOLCHANGE_FILAMENT_SWAP)
  void extruder_prime();
  void extruder_cutting_recover(const_float_t e);
#endif
