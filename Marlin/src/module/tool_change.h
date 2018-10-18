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
#pragma once

#include "../inc/MarlinConfigPre.h"

#if EXTRUDERS > 1

  typedef struct {
    #if ENABLED(TOOLCHANGE_FILAMENT_SWAP)
      float swap_length;
      int16_t prime_speed, retract_speed;
    #endif
    #if ENABLED(TOOLCHANGE_PARK)
      struct { float x, y; } change_point;
    #endif
    float z_raise;
  } toolchange_settings_t;

  extern toolchange_settings_t toolchange_settings;

#endif

#if DO_SWITCH_EXTRUDER
  void move_extruder_servo(const uint8_t e);
#endif

#if ENABLED(SWITCHING_NOZZLE)
  void move_nozzle_servo(const uint8_t e);
#endif

#if ENABLED(PARKING_EXTRUDER)

  #if ENABLED(PARKING_EXTRUDER_SOLENOIDS_INVERT)
    #define PE_MAGNET_ON_STATE !PARKING_EXTRUDER_SOLENOIDS_PINS_ACTIVE
  #else
    #define PE_MAGNET_ON_STATE PARKING_EXTRUDER_SOLENOIDS_PINS_ACTIVE
  #endif

  void pe_set_magnet(const uint8_t extruder_num, const uint8_t state);

  inline void pe_activate_magnet(const uint8_t extruder_num) { pe_set_magnet(extruder_num, PE_MAGNET_ON_STATE); }
  inline void pe_deactivate_magnet(const uint8_t extruder_num) { pe_set_magnet(extruder_num, !PE_MAGNET_ON_STATE); }

  void pe_magnet_init();

#endif // PARKING_EXTRUDER

#if ENABLED(SINGLENOZZLE)
  extern uint16_t singlenozzle_temp[EXTRUDERS];
  #if FAN_COUNT > 0
    extern uint8_t singlenozzle_fan_speed[EXTRUDERS];
  #endif
#endif

/**
 * Perform a tool-change, which may result in moving the
 * previous tool out of the way and the new tool into place.
 */
void tool_change(const uint8_t tmp_extruder, const float fr_mm_s=0.0, bool no_move=false);
