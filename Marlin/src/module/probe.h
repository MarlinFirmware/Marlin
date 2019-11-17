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
#pragma once

/**
 * probe.h - Move, deploy, enable, etc.
 */

#include "../inc/MarlinConfig.h"

#if HAS_BED_PROBE

  extern xyz_pos_t probe_offset;

  bool set_probe_deployed(const bool deploy);
  #ifdef Z_AFTER_PROBING
    void move_z_after_probing();
  #endif
  enum ProbePtRaise : unsigned char {
    PROBE_PT_NONE,  // No raise or stow after run_z_probe
    PROBE_PT_STOW,  // Do a complete stow after run_z_probe
    PROBE_PT_RAISE, // Raise to "between" clearance after run_z_probe
    PROBE_PT_BIG_RAISE  // Raise to big clearance after run_z_probe
  };
  float probe_at_point(const float &rx, const float &ry, const ProbePtRaise raise_after=PROBE_PT_NONE, const uint8_t verbose_level=0, const bool probe_relative=true);
  inline float probe_at_point(const xy_pos_t &pos, const ProbePtRaise raise_after=PROBE_PT_NONE, const uint8_t verbose_level=0, const bool probe_relative=true) {
    return probe_at_point(pos.x, pos.y, raise_after, verbose_level, probe_relative);
  }
  #define DEPLOY_PROBE() set_probe_deployed(true)
  #define STOW_PROBE() set_probe_deployed(false)
  #if HAS_HEATED_BED && ENABLED(WAIT_FOR_BED_HEATER)
    extern const char msg_wait_for_bed_heating[25];
  #endif

#else

  constexpr xyz_pos_t probe_offset{0};

  #define DEPLOY_PROBE()
  #define STOW_PROBE()

#endif

#if HAS_LEVELING && (HAS_BED_PROBE || ENABLED(PROBE_MANUALLY))
  inline float probe_min_x() {
    return _MAX(
      #if IS_KINEMATIC
        PROBE_X_MIN, MESH_MIN_X
      #else
        (X_MIN_BED) + (MIN_PROBE_EDGE_LEFT), (X_MIN_POS) + probe_offset.x
      #endif
    );
  }
  inline float probe_max_x() {
    return _MIN(
      #if IS_KINEMATIC
        PROBE_X_MAX, MESH_MAX_X
      #else
        (X_MAX_BED) - (MIN_PROBE_EDGE_RIGHT), (X_MAX_POS) + probe_offset.x
      #endif
    );
  }
  inline float probe_min_y() {
    return _MAX(
      #if IS_KINEMATIC
        PROBE_Y_MIN, MESH_MIN_Y
      #else
        (Y_MIN_BED) + (MIN_PROBE_EDGE_FRONT), (Y_MIN_POS) + probe_offset.y
      #endif
    );
  }
  inline float probe_max_y() {
    return _MIN(
      #if IS_KINEMATIC
        PROBE_Y_MAX, MESH_MAX_Y
      #else
        (Y_MAX_BED) - (MIN_PROBE_EDGE_BACK), (Y_MAX_POS) + probe_offset.y
      #endif
    );
  }
#else
  inline float probe_min_x() { return 0; };
  inline float probe_max_x() { return 0; };
  inline float probe_min_y() { return 0; };
  inline float probe_max_y() { return 0; };
#endif

#if HAS_Z_SERVO_PROBE
  void servo_probe_init();
#endif

#if QUIET_PROBING
  void probing_pause(const bool p);
#endif
