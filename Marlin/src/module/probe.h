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

  #if HAS_PROBE_XY_OFFSET
    extern xyz_pos_t &probe_offset_xy;
  #else
    constexpr xy_pos_t probe_offset_xy{0};
  #endif

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
  constexpr xy_pos_t probe_offset_xy{0};

  #define DEPLOY_PROBE()
  #define STOW_PROBE()

#endif

#if HAS_BED_PROBE || HAS_LEVELING
  #if IS_KINEMATIC
    constexpr float printable_radius = (
      #if ENABLED(DELTA)
        DELTA_PRINTABLE_RADIUS
      #elif IS_SCARA
        SCARA_PRINTABLE_RADIUS
      #endif
    );

    inline float probe_radius() {
      return printable_radius - _MAX(MIN_PROBE_EDGE, HYPOT(probe_offset_xy.x, probe_offset_xy.y));
    }
  #endif

  inline float probe_min_x() {
    return (
      #if IS_KINEMATIC
        (X_CENTER) - probe_radius()
      #else
        _MAX((X_MIN_BED) + (MIN_PROBE_EDGE_LEFT), (X_MIN_POS) + probe_offset_xy.x)
      #endif
    );
  }
  inline float probe_max_x() {
    return (
      #if IS_KINEMATIC
        (X_CENTER) + probe_radius()
      #else
        _MIN((X_MAX_BED) - (MIN_PROBE_EDGE_RIGHT), (X_MAX_POS) + probe_offset_xy.x)
      #endif
    );
  }
  inline float probe_min_y() {
    return (
      #if IS_KINEMATIC
        (Y_CENTER) - probe_radius()
      #else
        _MAX((Y_MIN_BED) + (MIN_PROBE_EDGE_FRONT), (Y_MIN_POS) + probe_offset_xy.y)
      #endif
    );
  }
  inline float probe_max_y() {
    return (
      #if IS_KINEMATIC
        (Y_CENTER) + probe_radius()
      #else
        _MIN((Y_MAX_BED) - (MIN_PROBE_EDGE_BACK), (Y_MAX_POS) + probe_offset_xy.y)
      #endif
    );
  }

  #if NEEDS_THREE_PROBE_POINTS
    // Retrieve three points to probe the bed. Any type exposing set(X,Y) may be used.
    template <typename T>
    inline void get_three_probe_points(T points[3]) {
      #if ENABLED(HAS_FIXED_3POINT)
        points[0].set(PROBE_PT_1_X, PROBE_PT_1_Y);
        points[1].set(PROBE_PT_2_X, PROBE_PT_2_Y);
        points[2].set(PROBE_PT_3_X, PROBE_PT_3_Y);
      #else
        #if IS_KINEMATIC
          constexpr float SIN0 = 0.0, SIN120 = 0.866025, SIN240 = -0.866025,
                          COS0 = 1.0, COS120 = -0.5    , COS240 = -0.5;
          points[0].set((X_CENTER) + probe_radius() * COS0,   (Y_CENTER) + probe_radius() * SIN0);
          points[1].set((X_CENTER) + probe_radius() * COS120, (Y_CENTER) + probe_radius() * SIN120);
          points[2].set((X_CENTER) + probe_radius() * COS240, (Y_CENTER) + probe_radius() * SIN240);
        #else
          points[0].set(probe_min_x(), probe_min_y());
          points[1].set(probe_max_x(), probe_min_y());
          points[2].set((probe_max_x() - probe_min_x()) / 2, probe_max_y());
        #endif
      #endif
    }
  #endif
#endif

#if HAS_Z_SERVO_PROBE
  void servo_probe_init();
#endif

#if QUIET_PROBING
  void probing_pause(const bool p);
#endif
