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

/**
 * module/probe.h - Move, deploy, enable, etc.
 */

#include "../inc/MarlinConfig.h"

#include "motion.h"

#if HAS_BED_PROBE
  enum ProbePtRaise : uint8_t {
    PROBE_PT_NONE,      // No raise or stow after run_z_probe
    PROBE_PT_STOW,      // Do a complete stow after run_z_probe
    PROBE_PT_RAISE,     // Raise to "between" clearance after run_z_probe
    PROBE_PT_BIG_RAISE  // Raise to big clearance after run_z_probe
  };
#endif

class Probe {
public:

  #if HAS_BED_PROBE

    static xyz_pos_t offset;

    static bool set_deployed(const bool deploy);


    #if IS_KINEMATIC

      #if HAS_PROBE_XY_OFFSET
        // Return true if the both nozzle and the probe can reach the given point.
        // Note: This won't work on SCARA since the probe offset rotates with the arm.
        static inline bool can_reach(const float &rx, const float &ry) {
          return position_is_reachable(rx - offset_xy.x, ry - offset_xy.y) // The nozzle can go where it needs to go?
              && position_is_reachable(rx, ry, ABS(MIN_PROBE_EDGE));       // Can the nozzle also go near there?
        }
      #else
        FORCE_INLINE static bool can_reach(const float &rx, const float &ry) {
          return position_is_reachable(rx, ry, MIN_PROBE_EDGE);
        }
      #endif

    #else

      /**
       * Return whether the given position is within the bed, and whether the nozzle
       * can reach the position required to put the probe at the given position.
       *
       * Example: For a probe offset of -10,+10, then for the probe to reach 0,0 the
       *          nozzle must be be able to reach +10,-10.
       */
      static inline bool can_reach(const float &rx, const float &ry) {
        return position_is_reachable(rx - offset_xy.x, ry - offset_xy.y)
            && WITHIN(rx, min_x() - fslop, max_x() + fslop)
            && WITHIN(ry, min_y() - fslop, max_y() + fslop);
      }

    #endif

    #ifdef Z_AFTER_PROBING
      static void move_z_after_probing();
    #endif
    static float probe_at_point(const float &rx, const float &ry, const ProbePtRaise raise_after=PROBE_PT_NONE, const uint8_t verbose_level=0, const bool probe_relative=true, const bool sanity_check=true);
    static inline float probe_at_point(const xy_pos_t &pos, const ProbePtRaise raise_after=PROBE_PT_NONE, const uint8_t verbose_level=0, const bool probe_relative=true, const bool sanity_check=true) {
      return probe_at_point(pos.x, pos.y, raise_after, verbose_level, probe_relative, sanity_check);
    }

  #else

    static constexpr xyz_pos_t offset = xyz_pos_t({ 0, 0, 0 }); // See #16767

    static bool set_deployed(const bool) { return false; }

    FORCE_INLINE static bool can_reach(const float &rx, const float &ry) { return position_is_reachable(rx, ry); }

  #endif

  FORCE_INLINE static bool can_reach(const xy_pos_t &pos) { return can_reach(pos.x, pos.y); }

  FORCE_INLINE static bool good_bounds(const xy_pos_t &lf, const xy_pos_t &rb) {
    return (
      #if IS_KINEMATIC
         can_reach(lf.x, 0) && can_reach(rb.x, 0) && can_reach(0, lf.y) && can_reach(0, rb.y)
      #else
         can_reach(lf) && can_reach(rb)
      #endif
    );
  }

  // Use offset_xy for read only access
  // More optimal the XY offset is known to always be zero.
  #if HAS_PROBE_XY_OFFSET
    static const xyz_pos_t &offset_xy;
  #else
    static constexpr xy_pos_t offset_xy = xy_pos_t({ 0, 0 });   // See #16767
  #endif

  static inline bool deploy() { return set_deployed(true); }
  static inline bool stow() { return set_deployed(false); }

  #if HAS_BED_PROBE || HAS_LEVELING
    #if IS_KINEMATIC
      static constexpr float printable_radius = (
        #if ENABLED(DELTA)
          DELTA_PRINTABLE_RADIUS
        #elif IS_SCARA
          SCARA_PRINTABLE_RADIUS
        #endif
      );

      static inline float probe_radius() {
        return printable_radius - _MAX(MIN_PROBE_EDGE, HYPOT(offset_xy.x, offset_xy.y));
      }
    #endif

    static inline float min_x() {
      return (
        #if IS_KINEMATIC
          (X_CENTER) - probe_radius()
        #else
          _MAX((X_MIN_BED) + (MIN_PROBE_EDGE_LEFT), (X_MIN_POS) + offset_xy.x)
        #endif
      );
    }
    static inline float max_x() {
      return (
        #if IS_KINEMATIC
          (X_CENTER) + probe_radius()
        #else
          _MIN((X_MAX_BED) - (MIN_PROBE_EDGE_RIGHT), (X_MAX_POS) + offset_xy.x)
        #endif
      );
    }
    static inline float min_y() {
      return (
        #if IS_KINEMATIC
          (Y_CENTER) - probe_radius()
        #else
          _MAX((Y_MIN_BED) + (MIN_PROBE_EDGE_FRONT), (Y_MIN_POS) + offset_xy.y)
        #endif
      );
    }
    static inline float max_y() {
      return (
        #if IS_KINEMATIC
          (Y_CENTER) + probe_radius()
        #else
          _MIN((Y_MAX_BED) - (MIN_PROBE_EDGE_BACK), (Y_MAX_POS) + offset_xy.y)
        #endif
      );
    }

    #if NEEDS_THREE_PROBE_POINTS
      // Retrieve three points to probe the bed. Any type exposing set(X,Y) may be used.
      template <typename T>
      static inline void get_three_points(T points[3]) {
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
            points[0].set(min_x(), min_y());
            points[1].set(max_x(), min_y());
            points[2].set((max_x() - min_x()) / 2, max_y());
          #endif
        #endif
      }
    #endif

  #endif // HAS_BED_PROBE

  #if HAS_Z_SERVO_PROBE
    static void servo_probe_init();
  #endif

  #if QUIET_PROBING
    static void set_probing_paused(const bool p);
  #endif

private:
  static bool probe_down_to_z(const float z, const feedRate_t fr_mm_s);
  static void do_z_raise(const float z_raise);
  static float run_z_probe(const bool sanity_check=true);
};

extern Probe probe;
