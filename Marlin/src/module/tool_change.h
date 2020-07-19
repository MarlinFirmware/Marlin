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
#pragma once

#include "../inc/MarlinConfigPre.h"
#include "../core/types.h"

#if EXTRUDERS > 1

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
    extern void tool_change_prime();
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

  #if ENABLED(PARKING_EXTRUDER_SOLENOIDS_INVERT)
    #define PE_MAGNET_ON_STATE !PARKING_EXTRUDER_SOLENOIDS_PINS_ACTIVE
  #else
    #define PE_MAGNET_ON_STATE PARKING_EXTRUDER_SOLENOIDS_PINS_ACTIVE
  #endif

  void pe_set_solenoid(const uint8_t extruder_num, const uint8_t state);

  inline void pe_activate_solenoid(const uint8_t extruder_num) { pe_set_solenoid(extruder_num, PE_MAGNET_ON_STATE); }
  inline void pe_deactivate_solenoid(const uint8_t extruder_num) { pe_set_solenoid(extruder_num, !PE_MAGNET_ON_STATE); }

  void pe_solenoid_init();

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

#if ENABLED(SINGLENOZZLE_STANDBY_TEMP)
  extern uint16_t singlenozzle_temp[EXTRUDERS];
#endif

#if BOTH(HAS_FAN, SINGLENOZZLE_STANDBY_FAN)
  extern uint8_t singlenozzle_fan_speed[EXTRUDERS];
#endif

TERN_(ELECTROMAGNETIC_SWITCHING_TOOLHEAD, void est_init());

TERN_(SWITCHING_TOOLHEAD, void swt_init());

/**
 * Perform a tool-change, which may result in moving the
 * previous tool out of the way and the new tool into place.
 */
void tool_change(const uint8_t tmp_extruder, bool no_move=false);
