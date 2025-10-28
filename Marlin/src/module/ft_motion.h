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

#include "ft_types.h"
#include "ft_motion/trajectory_generator.h"
#include "ft_motion/trapezoidal_trajectory_generator.h"
#include "ft_motion/poly5_trajectory_generator.h"
#include "ft_motion/poly6_trajectory_generator.h"

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

    #if ENABLED(FTM_SMOOTHING)
      ft_smoothed_float_t smoothingTime;                  // Smoothing time. [s]
    #endif

    #if HAS_DYNAMIC_FREQ
      dynFreqMode_t dynFreqMode = FTM_DEFAULT_DYNFREQ_MODE; // Dynamic frequency mode configuration.
      ft_shaped_float_t dynFreqK = { 0.0f };                // Scaling / gain for dynamic frequency. [Hz/mm] or [Hz/g]
    #else
      static constexpr dynFreqMode_t dynFreqMode = dynFreqMode_DISABLED;
    #endif

  #endif // HAS_FTM_SHAPING

  #if HAS_EXTRUDERS
    bool linearAdvEna = FTM_LINEAR_ADV_DEFAULT_ENA;       // Linear advance enable configuration.
    float linearAdvK = FTM_LINEAR_ADV_DEFAULT_K;          // Linear advance gain.
  #endif

  TrajectoryType trajectory_type = TrajectoryType::FTM_TRAJECTORY_TYPE; // Trajectory generator type
  float poly6_acceleration_overshoot; // Overshoot factor for Poly6 (1.25 to 2.0)
} ft_config_t;

/**
 * FTMotion - Singleton class encapsulating Fixed Time Motion
 */
class FTMotion {

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

      #if HAS_EXTRUDERS
        cfg.linearAdvEna = FTM_LINEAR_ADV_DEFAULT_ENA;
        cfg.linearAdvK = FTM_LINEAR_ADV_DEFAULT_K;
      #endif

      cfg.poly6_acceleration_overshoot = FTM_POLY6_ACCELERATION_OVERSHOOT;

      setTrajectoryType(TrajectoryType::FTM_TRAJECTORY_TYPE);

      reset();
    }

    static XYZEval<millis_t> axis_move_end_ti;
    static AxisBits axis_move_dir;

    // Public methods
    static void init();
    static void loop();                                   // Controller main, to be invoked from non-isr task.

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
      return cfg.active ? PENDING(millis(), axis_move_end_ti[axis]) : stepper.axis_is_moving(axis);
    }
    FORCE_INLINE static bool motor_direction(const AxisEnum axis) {
      return cfg.active ? axis_move_dir[axis] : stepper.last_direction_bits[axis];
    }

    // Stepping plan handles the counting at the table
    typedef struct Stepping {
      stepper_plan_t stepper_plan;
      xyze_ulong_t delta_error_q32{ LOGICAL_AXIS_LIST_1(_BV32(31)) };
      AxisBits step_bits;
      uint32_t bresenham_iterations_pending;

      void reset() {
        stepper_plan.reset();
        delta_error_q32.set(LOGICAL_AXIS_ARRAY_1(_BV32(31))); // start as 0.5 in q32 so steps are rounded
        step_bits = 0;
        bresenham_iterations_pending = 0;
      }

      #define INTERVAL_PER_ITERATION (STEPPER_TIMER_RATE / FTM_STEPPER_FS)
      #define INTERVAL_PER_TRAJ_POINT (STEPPER_TIMER_RATE / FTM_FS)
      #define ITERATIONS_PER_TRAJ (FTM_STEPPER_FS * FTM_TS)

      // Updates error and bresenham_iterations_pending, sets step_bits and returns interval until it executes
      uint32_t advance_until_step_old() {
        step_bits = 0;
        uint32_t interval = 0;
        while (!step_bits && bresenham_iterations_pending > 0) {
          interval += INTERVAL_PER_ITERATION;
          bresenham_iterations_pending--;
          // Note: defining a macro inside a loop doesn't change what the preprocessor does. Keeping it here to be closest to its usage and easier to read
          #define RUN_AXIS(A) do{                                                      \
              delta_error_q32.A += stepper_plan.advance_dividend_q0_32.A;              \
              step_bits.A = delta_error_q32.A < stepper_plan.advance_dividend_q0_32.A; \
            }while(0);
          LOGICAL_AXIS_MAP(RUN_AXIS);
          #undef RUN_AXIS
        }
        return interval;
      }

      // Updates error and bresenham_iterations_pending, sets step_bits and returns interval until it executes
      // For perf, it searches in powers of two instead of bresenham by one until a step is due.
      // equivalent to iterations = (space/dividend).small() but avoiding division
      uint32_t advance_until_step() {
        uint32_t iterations = 0;
        { // block just to group variables
          xyze_ulong_t dividend_scaled = stepper_plan.advance_dividend_q0_32;
          uint32_t jump = 1;

          xyze_ulong_t space = -delta_error_q32 + UINT32_MAX; // How much accumulation until a step in any axis is due
                                                                   // scalar in the right hand because types.h is missing scalar on left cases
          // using halves since we'll double the dividend and iteration_multiplier inside the while.
          xyze_ulong_t half_space = space >> 1;                         // ensures (dividend_scaled<<1) <= space
          uint32_t half_pending = (bresenham_iterations_pending - 1) >> 1;   // ensures (iter<<1) < pending

          // —— Grow phase: keep doubling until right before a step would occur (or pending iterations exhausted)
          while (dividend_scaled <= half_space && jump <= half_pending) {
            jump <<= 1;               // stays < pending
            dividend_scaled <<= 1;    // safe: fits after doubling
          }

          // —— Shrink phase: keep adding and halving until there's 1 iteration left for actual stepping
          while (bresenham_iterations_pending > 1 && jump > 0) {
            if (jump < bresenham_iterations_pending && dividend_scaled <= space) {
              space -= dividend_scaled;
              bresenham_iterations_pending -= jump;
              delta_error_q32 += dividend_scaled;
              iterations += jump;
            }
            dividend_scaled >>= 1;
            jump >>= 1;
          }
        }

        step_bits = 0;

        // if there are still iteration pending (should be exactly 1), run through it
        if (bresenham_iterations_pending) {
          iterations++;
          bresenham_iterations_pending--;
          #define RUN_AXIS(A) do{                                                      \
              delta_error_q32.A += stepper_plan.advance_dividend_q0_32.A;              \
              step_bits.A = delta_error_q32.A < stepper_plan.advance_dividend_q0_32.A; \
            }while(0);
          LOGICAL_AXIS_MAP(RUN_AXIS);
          #undef RUN_AXIS
        }

        // TODO: find out why not subtracting 1 results in 1 bad pulse train per trajectory point.
        return (iterations - 1) * INTERVAL_PER_ITERATION;
      }

      /**
       * If bresenham_iterations_pending, advance to next actual step.
       * Else, consume stepper data point
       * Then return interval until that next step
       */
      uint32_t plan() {
        if (bresenham_iterations_pending > 0) return advance_until_step();

        if (stepper_plan_is_empty()) {
          bresenham_iterations_pending = 0;
          step_bits = 0;
          return HAL_TIMER_TYPE_MAX;
        }

        AxisBits old_dir_bits = stepper_plan.dir_bits;
        stepper_plan = dequeue_stepper_plan();
        const AxisBits dir_flip_mask = old_dir_bits ^ stepper_plan.dir_bits;  // axes that must toggle now
        if (dir_flip_mask) {
          #define _HANDLE_DIR_CHANGES(A) if (dir_flip_mask.A) delta_error_q32.A *= -1;
          LOGICAL_AXIS_MAP(_HANDLE_DIR_CHANGES);
          #undef _HANDLE_DIR_CHANGES
        }

        if (!(bool)stepper_plan.advance_dividend_q0_32) {
          // don't waste time in zero motion traj points
          bresenham_iterations_pending = 0;
          step_bits = 0;
          return INTERVAL_PER_TRAJ_POINT;
        }

        bresenham_iterations_pending = ITERATIONS_PER_TRAJ;
        return advance_until_step();
      }
    } stepping_t;

    static stepping_t stepping;

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

    // Number of batches needed to propagate the current trajectory to the stepper.
    static constexpr uint32_t PROP_BATCHES = 1;

    #if ENABLED(DISTINCT_E_FACTORS)
      static uint8_t block_extruder_axis;  // Cached extruder axis index
    #elif HAS_EXTRUDERS
      static constexpr uint8_t block_extruder_axis = E_AXIS;
      static bool use_advance_lead;
    #endif

    #if HAS_FTM_SHAPING
      // Shaping data
      typedef struct AxisShaping {
        bool ena = false;                 // Enabled indication
        float d_zi[FTM_ZMAX] = { 0.0f };  // Data point delay vector
        float Ai[5];                      // Shaping gain vector
        int32_t Ni[5];                    // Shaping time index vector
        uint32_t max_i;                   // Vector length for the selected shaper

        void set_axis_shaping_N(const ftMotionShaper_t shaper, const float f, const float zeta);    // Sets the gains used by shaping functions.
        void set_axis_shaping_A(const ftMotionShaper_t shaper, const float zeta, const float vtol); // Sets the indices used by shaping functions.

      } axis_shaping_t;

      typedef struct Shaping {
        uint32_t zi_idx;           // Index of storage in the data point delay vectors.
        axis_shaping_t SHAPED_AXIS_NAMES;
      } shaping_t;

      static shaping_t shaping; // Shaping data

    #endif // HAS_FTM_SHAPING

    #if ENABLED(FTM_SMOOTHING)
      // Smoothing data for each axis
      typedef struct AxisSmoothing {
        float smoothing_pass[FTM_SMOOTHING_ORDER] = { 0.0f }; // Last value of each of the exponential smoothing passes
        float alpha = 0.0f;               // Pre-calculated alpha for smoothing.
        uint32_t delay_samples = 0;       // Pre-calculated delay in samples for smoothing.
        void set_smoothing_time(const float s_time); // Set smoothing time, recalculate alpha and delay.
      } axis_smoothing_t;

      // Smoothing data for XYZE axes
      typedef struct Smoothing {
        axis_smoothing_t CARTES_AXIS_NAMES;
      } smoothing_t;
      static smoothing_t smoothing;       // Smoothing data
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
    static xyze_float_t calc_traj_point(float dist);
    static stepper_plan_t calc_stepper_plan(xyze_float_t delta);
    static bool plan_next_block();
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
typedef struct FTMotionDisableInScope {
  #if DISABLED(FTM_HOME_AND_PROBE)
    bool isactive;
    FTMotionDisableInScope() {
      isactive = ftMotion.cfg.active;
      ftMotion.cfg.active = false;
    }
    ~FTMotionDisableInScope() {
      ftMotion.cfg.active = isactive;
      if (isactive) ftMotion.init();
    }
  #endif
} FTMotionDisableInScope_t;
