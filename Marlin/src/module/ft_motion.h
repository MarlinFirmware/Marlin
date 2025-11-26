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
#include "ft_motion/trajectory_poly5.h"
#include "ft_motion/trajectory_poly6.h"
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

#if HAS_X_AXIS && (HAS_Z_AXIS || HAS_EXTRUDERS)
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
  bool active = ENABLED(FTM_IS_DEFAULT_MOTION);           // Active (else standard motion)
  bool axis_sync_enabled = true;                          // Axis synchronization enabled

  #if HAS_FTM_SHAPING
    ft_shaped_shaper_t shaper =                           // Shaper type
      SHAPED_ARRAY(FTM_DEFAULT_SHAPER_X, FTM_DEFAULT_SHAPER_Y, FTM_DEFAULT_SHAPER_Z, FTM_DEFAULT_SHAPER_E);
    ft_shaped_float_t baseFreq =                          // Base frequency. [Hz]
      SHAPED_ARRAY(FTM_SHAPING_DEFAULT_FREQ_X, FTM_SHAPING_DEFAULT_FREQ_Y, FTM_SHAPING_DEFAULT_FREQ_Z, FTM_SHAPING_DEFAULT_FREQ_E);
    ft_shaped_float_t zeta =                              // Damping factor
      SHAPED_ARRAY(FTM_SHAPING_ZETA_X, FTM_SHAPING_ZETA_Y, FTM_SHAPING_ZETA_Z, FTM_SHAPING_ZETA_E);
    ft_shaped_float_t vtol =                              // Vibration Level
      SHAPED_ARRAY(FTM_SHAPING_V_TOL_X, FTM_SHAPING_V_TOL_Y, FTM_SHAPING_V_TOL_Z, FTM_SHAPING_V_TOL_E);

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

  TrajectoryType trajectory_type = TrajectoryType::FTM_TRAJECTORY_TYPE; // Trajectory generator type
  float poly6_acceleration_overshoot; // Overshoot factor for Poly6 (1.25 to 2.0)
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
      cfg.active = ENABLED(FTM_IS_DEFAULT_MOTION);

      #if HAS_FTM_SHAPING

        #define _SET_CFG_DEFAULTS(A) do{ \
          cfg.shaper.A   = FTM_DEFAULT_SHAPER_##A; \
          cfg.baseFreq.A = FTM_SHAPING_DEFAULT_FREQ_##A; \
          cfg.zeta.A     = FTM_SHAPING_ZETA_##A; \
          cfg.vtol.A     = FTM_SHAPING_V_TOL_##A; \
        }while(0);

        SHAPED_MAP(_SET_CFG_DEFAULTS);
        #undef _SET_CFG_DEFAULTS

        #if HAS_DYNAMIC_FREQ
          cfg.dynFreqMode = FTM_DEFAULT_DYNFREQ_MODE;
          //ZERO(cfg.dynFreqK);
          #define _DYN_RESET(A) cfg.dynFreqK.A = 0.0f;
          SHAPED_MAP(_DYN_RESET);
          #undef _DYN_RESET
        #endif

        update_shaping_params();

      #endif // HAS_FTM_SHAPING

      #if ENABLED(FTM_SMOOTHING)
        #define _SET_SMOOTH(A) set_smoothing_time(_AXIS(A), FTM_SMOOTHING_TIME_##A);
        CARTES_MAP(_SET_SMOOTH);
        #undef _SET_SMOOTH
      #endif

      cfg.poly6_acceleration_overshoot = FTM_POLY6_ACCELERATION_OVERSHOOT;

      setTrajectoryType(TrajectoryType::FTM_TRAJECTORY_TYPE);

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
      static void set_smoothing_time(uint8_t axis, const float s_time);
    #endif

    static void reset();                                  // Reset all states of the fixed time conversion to defaults.

    // Safely toggle the active state of FT Motion
    static bool toggle() {
      stepper.ftMotion_syncPosition();
      FLIP(cfg.active);
      update_shaping_params();
      return cfg.active;
    }

    // Trajectory generator selection
    static void setTrajectoryType(const TrajectoryType type);
    static TrajectoryType getTrajectoryType() { return trajectoryType; }

    FORCE_INLINE static bool axis_is_moving(const AxisEnum axis) {
      return cfg.active ? moving_axis_flags[axis] : stepper.axis_is_moving(axis);
    }
    FORCE_INLINE static bool motor_direction(const AxisEnum axis) {
      return cfg.active ? axis_move_dir[axis] : stepper.last_direction_bits[axis];
    }


    static stepping_t stepping;
    FORCE_INLINE static bool stepper_plan_is_empty() {
      return stepper_plan_head == stepper_plan_tail;
    }
    FORCE_INLINE static bool stepper_plan_is_full() {
      return ((stepper_plan_head + 1) & FTM_BUFFER_MASK) == stepper_plan_tail;
    }
    FORCE_INLINE static uint32_t stepper_plan_count() {
      return (stepper_plan_head - stepper_plan_tail) & FTM_BUFFER_MASK;
    }
    // Enqueue a plan
    FORCE_INLINE static void enqueue_stepper_plan(const stepper_plan_t& d) {
      stepper_plan_buff[stepper_plan_head] = d;
      stepper_plan_head = (stepper_plan_head + 1u) & FTM_BUFFER_MASK;
    }
    // Dequeue a plan.
    // Zero-copy consume; caller must use it before next dequeue if they keep a ref.
    // Done like this to avoid double copy.
    // e.g do: stepper_plan_t data = dequeue_stepper_plan(); this is ok
    FORCE_INLINE static stepper_plan_t& dequeue_stepper_plan() {
      const uint32_t i = stepper_plan_tail;
      stepper_plan_tail = (i + 1u) & FTM_BUFFER_MASK;
      return stepper_plan_buff[i];
    }

  private:
    // Block data variables.
    static xyze_pos_t   startPos,         // (mm) Start position of block
                        endPos_prevBlock; // (mm) End position of previous block
    static xyze_float_t ratio;            // (ratio) Axis move ratio of block
    static float tau;                     // (s) Time since start of block

    // Trajectory generators
    static TrapezoidalTrajectoryGenerator trapezoidalGenerator;
    static Poly5TrajectoryGenerator poly5Generator;
    static Poly6TrajectoryGenerator poly6Generator;
    static TrajectoryGenerator* currentGenerator;
    static TrajectoryType trajectoryType;

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
    static stepper_plan_t calc_stepper_plan(xyze_float_t delta);
    static bool plan_next_block();
    // stepper_plan buffer variables.
    static stepper_plan_t stepper_plan_buff[FTM_BUFFER_SIZE];
    static uint32_t stepper_plan_tail, stepper_plan_head;
    static XYZEval<int64_t> curr_steps_q32_32;
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
      ftMotion.cfg.active = false;
    }
    ~FTMotionDisableInScope() {
      ftMotion.cfg.active = isactive;
      if (isactive) ftMotion.init();
    }
  } FTMotionDisableInScope_t;
#endif

#define FTM_DISABLE_IN_SCOPE() TERN(FTM_HOME_AND_PROBE, NOOP, FTMotionDisableInScope FT_Disabler)
