/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../inc/MarlinConfigPre.h" // Access the top level configurations.
#include "planner.h"      // Access block type from planner.
#include "stepper.h"      // For stepper motion and direction

#include "ft_motion/trajectory_trapezoidal.h"
#if ENABLED(FTM_POLYS)
  #include "ft_motion/trajectory_poly5.h"
  #include "ft_motion/trajectory_poly6.h"
#endif
#if ENABLED(FTM_RESONANCE_TEST)
  #include "ft_motion/resonance_generator.h"
#endif

#if HAS_FTM_SHAPING
  #include "ft_motion/shaping.h"
#endif
#if ENABLED(FTM_SMOOTHING)
  #include "ft_motion/smoothing.h"
#endif
#include "ft_motion/stepping.h"

#define FTM_VERSION   2   // Change version when hosts need to know

#if ENABLED(FTM_DYNAMIC_FREQ)
  #define HAS_DYNAMIC_FREQ 1
  #if HAS_Z_AXIS
    #define HAS_DYNAMIC_FREQ_MM 1
  #endif
  #if HAS_EXTRUDERS
    #define HAS_DYNAMIC_FREQ_G 1
  #endif
#endif

/**
 * FTConfig - The active configured state of FT Motion
 */
typedef struct FTConfig {
  #if HAS_STANDARD_MOTION
    bool active = ENABLED(FTM_IS_DEFAULT_MOTION);   // Active (else Standard Motion)
  #else
    static constexpr bool active = true;                  // Always active with NO_STANDARD_MOTION
  #endif
  bool axis_sync_enabled = true;                          // Axis synchronization enabled

  #if HAS_FTM_SHAPING
    ft_shaped_shaper_t shaper =                           // Shaper type
      SHAPED_ARRAY(FTM_DEFAULT_SHAPER_X, FTM_DEFAULT_SHAPER_Y, FTM_DEFAULT_SHAPER_Z, FTM_DEFAULT_SHAPER_E);
    ft_shaped_float_t baseFreq =                          // Base frequency. [Hz]
      SHAPED_ARRAY(FTM_SHAPING_DEFAULT_FREQ_X, FTM_SHAPING_DEFAULT_FREQ_Y, FTM_SHAPING_DEFAULT_FREQ_Z, FTM_SHAPING_DEFAULT_FREQ_E);
    ft_shaped_float_t zeta =                              // Damping factor
      SHAPED_ARRAY(FTM_SHAPING_ZETA_X, FTM_SHAPING_ZETA_Y, FTM_SHAPING_ZETA_Z, FTM_SHAPING_ZETA_E);

    #if HAS_FTM_EI_SHAPING
      ft_shaped_float_t vtol =                              // Vibration Level
        SHAPED_ARRAY(FTM_SHAPING_V_TOL_X, FTM_SHAPING_V_TOL_Y, FTM_SHAPING_V_TOL_Z, FTM_SHAPING_V_TOL_E);
    #endif

    #if HAS_DYNAMIC_FREQ
      dynFreqMode_t dynFreqMode = FTM_DEFAULT_DYNFREQ_MODE; // Dynamic frequency mode configuration.
      ft_shaped_float_t dynFreqK = { 0.0f };                // Scaling / gain for dynamic frequency. [Hz/mm] or [Hz/g]
    #else
      static constexpr dynFreqMode_t dynFreqMode = dynFreqMode_DISABLED;
    #endif

  #endif // HAS_FTM_SHAPING

  #if ENABLED(FTM_SMOOTHING)
    ft_smoothed_float_t smoothingTime;                    // Smoothing time. [s]
  #endif

  #if ENABLED(FTM_POLYS)
    float poly6_acceleration_overshoot; // Overshoot factor for Poly6 (1.25 to 2.0)
    TrajectoryType trajectory_type = TrajectoryType::FTM_TRAJECTORY_TYPE; // Trajectory generator type
  #else
    static constexpr TrajectoryType trajectory_type = TrajectoryType::TRAPEZOIDAL;
  #endif

  #if HAS_STANDARD_MOTION
    bool setActive(const bool a) {
      if (a == active) return false;
      stepper.ftMotion_syncPosition();
      planner.synchronize();
      active = a;
      return true;
    }
  #endif

  bool setAxisSync(const bool ena) {
    if (ena == axis_sync_enabled) return false;
    planner.synchronize();
    axis_sync_enabled = ena;
    return true;
  }

  #if HAS_FTM_SHAPING

    bool setShaper(const AxisEnum a, const ftMotionShaper_t s) {
      if (s == shaper[a]) return false;
      planner.synchronize();
      shaper[a] = s;
      return true;
    }

    constexpr bool goodZeta(const float z) { return WITHIN(z, 0.01f, ftm_max_dampening); }

    bool setZeta(const AxisEnum a, const float z) {
      if (z == zeta[a]) return false;
      if (!goodZeta(z)) return false;
      planner.synchronize();
      zeta[a] = z;
      return true;
    }

    #if HAS_FTM_EI_SHAPING

      constexpr bool goodVtol(const float v) { return WITHIN(v, 0.00f, 1.0f); }

      bool setVtol(const AxisEnum a, const float v) {
        if (v == vtol[a]) return false;
        if (!goodVtol(v)) return false;
        planner.synchronize();
        vtol[a] = v;
        return true;
      }

    #endif

    #if HAS_DYNAMIC_FREQ

      uint8_t setDynFreqMode(const uint8_t m) {
        if (dynFreqMode_t(m) == dynFreqMode) return 0;
        switch (dynFreqMode_t(m)) {
          default: return 2;
          TERN_(HAS_DYNAMIC_FREQ_MM, case dynFreqMode_Z_BASED:)
          TERN_(HAS_DYNAMIC_FREQ_G, case dynFreqMode_MASS_BASED:)
          case dynFreqMode_DISABLED:
            planner.synchronize();
            dynFreqMode = dynFreqMode_t(m);
            break;
        }
        return 1;
      }

      bool modeUsesDynFreq() const {
        return (TERN0(HAS_DYNAMIC_FREQ_MM, dynFreqMode == dynFreqMode_Z_BASED)
             || TERN0(HAS_DYNAMIC_FREQ_G,  dynFreqMode == dynFreqMode_MASS_BASED));
      }

      bool setDynFreqK(const AxisEnum a, const float k) {
        if (!modeUsesDynFreq()) return false;
        if (k == dynFreqK[a]) return false;
        planner.synchronize();
        dynFreqK[a] = k;
        return true;
      }

    #endif // HAS_DYNAMIC_FREQ

  #endif // HAS_FTM_SHAPING

  constexpr bool goodBaseFreq(const float f) { return WITHIN(f, FTM_MIN_SHAPE_FREQ, (FTM_FS) / 2); }

  bool setBaseFreq(const AxisEnum a, const float f) {
    if (f == baseFreq[a]) return false;
    if (!goodBaseFreq(a)) return false;
    planner.synchronize();
    baseFreq[a] = f;
    return true;
  }

  void set_defaults() {
    #if HAS_STANDARD_MOTION
      active = ENABLED(FTM_IS_DEFAULT_MOTION);
    #endif

    #if HAS_FTM_SHAPING

      #define _SET_CFG_DEFAULTS(A) do{ \
        shaper.A   = FTM_DEFAULT_SHAPER_##A; \
        baseFreq.A = FTM_SHAPING_DEFAULT_FREQ_##A; \
        zeta.A     = FTM_SHAPING_ZETA_##A; \
        TERN_(HAS_FTM_EI_SHAPING, vtol.A = FTM_SHAPING_V_TOL_##A); \
      }while(0);

      SHAPED_MAP(_SET_CFG_DEFAULTS);
      #undef _SET_CFG_DEFAULTS

      #if HAS_DYNAMIC_FREQ
        dynFreqMode = FTM_DEFAULT_DYNFREQ_MODE;
        dynFreqK.reset();
      #endif

    #endif // HAS_FTM_SHAPING

    TERN_(FTM_POLYS, poly6_acceleration_overshoot = FTM_POLY6_ACCELERATION_OVERSHOOT);
  }

} ft_config_t;

/**
 * FTMotion - Singleton class encapsulating Fixed Time Motion
 */
class FTMotion {

  #if ENABLED(FTM_RESONANCE_TEST)
    friend void ResonanceGenerator::fill_stepper_plan_buffer();
  #endif

  public:

    // Public variables
    static ft_config_t cfg;
    static bool busy;

    static void set_defaults() {
      cfg.set_defaults();

      TERN_(HAS_FTM_SHAPING, update_shaping_params());

      #if ENABLED(FTM_SMOOTHING)
        #define _RESET_SMOOTH(A) (void)set_smoothing_time(_AXIS(A), FTM_SMOOTHING_TIME_##A);
        CARTES_MAP(_RESET_SMOOTH);
        #undef _RESET_SMOOTH
      #endif

      TERN_(FTM_POLYS, setTrajectoryType(TrajectoryType::FTM_TRAJECTORY_TYPE));

      reset();
    }

    static AxisBits moving_axis_flags,                    // These axes are moving in the planner block being processed
                    axis_move_dir;                        // ...in these directions

    // Public methods
    static void init();
    static void loop();                                   // Controller main, to be invoked from non-isr task.
    #if ENABLED(FTM_RESONANCE_TEST)
      static void start_resonance_test();                 // Start a resonance test with given parameters
      static ResonanceGenerator rtg;                      // Resonance trajectory generator instance
    #endif

    #if HAS_FTM_SHAPING
      // Refresh gains and indices used by shaping functions.
      static void update_shaping_params();
    #endif

    #if ENABLED(FTM_SMOOTHING)
      // Refresh alpha and delay samples used by smoothing functions.
      static void update_smoothing_params();
      // Setters for smoothingTime that update alpha and delay
      static bool set_smoothing_time(const AxisEnum axis, const float s_time);
    #endif

    static void reset();                                  // Reset all states of the fixed time conversion to defaults.

    // Safely toggle the active state of FT Motion
    #if ALL(FT_MOTION, HAS_STANDARD_MOTION)
      static bool toggle() {
        stepper.ftMotion_syncPosition();
        cfg.setActive(!cfg.active);
        update_shaping_params();
        return cfg.active;
      }
    #endif

    // Setters for baseFreq, zeta, vtol
    static bool setBaseFreq(const AxisEnum a, const float f) {
      if (!cfg.setBaseFreq(a, f)) return false;
      update_shaping_params();
      return true;
    }
    static bool setZeta(const AxisEnum a, const float z) {
      if (!cfg.setZeta(a, z)) return false;
      update_shaping_params();
      return true;
    }

    #if HAS_FTM_EI_SHAPING
      static bool setVtol(const AxisEnum a, const float v) {
        if (!cfg.setVtol(a, v)) return false;
        update_shaping_params();
        return true;
      }
    #endif

    // Trajectory generator selection
    #if ENABLED(FTM_POLYS)
      static void setTrajectoryType(const TrajectoryType type);
      static bool updateTrajectoryType(const TrajectoryType type);
    #endif
    static TrajectoryType getTrajectoryType() { return TERN(FTM_POLYS, trajectoryType, TrajectoryType::TRAPEZOIDAL); }
    static FSTR_P getTrajectoryName();

    FORCE_INLINE static bool axis_is_moving(const AxisEnum axis) {
      return cfg.active ? moving_axis_flags[axis] : TERN0(HAS_STANDARD_MOTION, stepper.axis_is_moving(axis));
    }
    FORCE_INLINE static bool motor_direction(const AxisEnum axis) {
      return cfg.active ? axis_move_dir[axis] : stepper.last_direction_bits[axis];
    }

    // A frame of the stepping plan
    static stepping_t stepping;

    // Add a single set of coordinates in the stepping plan
    FORCE_INLINE static void stepping_enqueue(const xyze_float_t traj_coords) {
      #define _TOSTEPS_q16(A, B) int64_t(traj_coords.A * planner.settings.axis_steps_per_mm[B] * (1ULL << 16))
      XYZEval<int64_t> next_steps_q48_16 = LOGICAL_AXIS_ARRAY(
        _TOSTEPS_q16(e, block_extruder_axis),
        _TOSTEPS_q16(x, X_AXIS), _TOSTEPS_q16(y, Y_AXIS), _TOSTEPS_q16(z, Z_AXIS),
        _TOSTEPS_q16(i, I_AXIS), _TOSTEPS_q16(j, J_AXIS), _TOSTEPS_q16(k, K_AXIS),
        _TOSTEPS_q16(u, U_AXIS), _TOSTEPS_q16(v, V_AXIS), _TOSTEPS_q16(w, W_AXIS)
      );
      #undef _TOSTEPS_q16
      stepping.enqueue(next_steps_q48_16);
    }

  private:
    // Block data variables.
    static xyze_pos_t   startPos,         // (mm) Start position of block
                        endPos_prevBlock; // (mm) End position of previous block
    static xyze_float_t ratio;            // (ratio) Axis move ratio of block
    static float tau;                     // (s) Time since start of block

    // Trajectory generators
    static TrapezoidalTrajectoryGenerator trapezoidalGenerator;
    #if ENABLED(FTM_POLYS)
      static Poly5TrajectoryGenerator poly5Generator;
      static Poly6TrajectoryGenerator poly6Generator;
      static TrajectoryType trajectoryType;
      static TrajectoryGenerator* currentGenerator;
    #else
      static constexpr TrajectoryGenerator *currentGenerator = &trapezoidalGenerator;
    #endif

    #if FTM_HAS_LIN_ADVANCE
      static bool use_advance_lead;
    #endif

    #if ENABLED(DISTINCT_E_FACTORS)
      static uint8_t block_extruder_axis;  // Cached extruder axis index
    #elif HAS_EXTRUDERS
      static constexpr uint8_t block_extruder_axis = E_AXIS;
    #endif

    #if HAS_FTM_SHAPING
      static shaping_t shaping; // Shaping data
    #endif

    #if ENABLED(FTM_SMOOTHING)
      // Smoothing data for XYZE axes
      static smoothing_t smoothing;
    #endif

    // Linear advance variables.
    #if HAS_EXTRUDERS
      static float prev_traj_e;
    #endif

    // Buffers
    static void discard_planner_block_protected();
    static uint32_t calc_runout_samples();
    static void plan_runout_block();
    static void fill_stepper_plan_buffer();
    static xyze_float_t calc_traj_point(const float dist);
    static bool plan_next_block();
    static void ensure_float_precision() IF_DISABLED(HAS_EXTRUDERS, {});

}; // class FTMotion

extern FTMotion ftMotion; // Use ftMotion.thing, not FTMotion::thing.

/**
 * Optional behavior to turn FT Motion off for homing/probing.
 * Applies when FTM_HOME_AND_PROBE is disabled.
 */
#if DISABLED(FTM_HOME_AND_PROBE)
  typedef struct FTMotionDisableInScope {
    bool isactive;
    FTMotionDisableInScope() {
      isactive = ftMotion.cfg.active;
      ftMotion.cfg.setActive(false);
    }
    ~FTMotionDisableInScope() {
      ftMotion.cfg.setActive(isactive);
      if (isactive) ftMotion.init();
    }
  } FTMotionDisableInScope_t;
#endif

#define FTM_DISABLE_IN_SCOPE() TERN(FTM_HOME_AND_PROBE, NOOP, FTMotionDisableInScope FT_Disabler)
