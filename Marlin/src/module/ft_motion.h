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

    typedef struct Stepping {
      uint32_t interval = 0;
      uint32_t steps_pending = 0;
      float interval_carry = 0;
      xyze_float_t curr_pos{0};
      XYZEval<uint32_t>  advance_dividend{0};
      XYZEval<uint32_t> delta_error{0};
      AxisBits last_direction_bits;

      void reset(){
        interval = 0;
        steps_pending = 0;
        interval_carry = 0;
        curr_pos.reset();
        advance_dividend.reset();
        delta_error.reset();
        // last_direction_bits.reset(); DONT reset this to avoid axis starting on the wrong direction
      }

      uint32_t plan() {
        if (steps_pending > 0) return interval;

        #define OVERSAMPLING 5
        float delta_max = 0;
        xyze_float_t delta = {0};
        static uint32_t trajs = 0;
        while (delta_max < 1) {
          if (trajBuff_isEmpty()) {
            return HAL_TIMER_TYPE_MAX;
          }
          #define TOSTEPS(A, B) (traj.A[traj_consume_i] * planner.settings.axis_steps_per_mm[B])
            xyze_float_t next_pos = LOGICAL_AXIS_ARRAY(
              TOSTEPS(e, block_extruder_axis),
              TOSTEPS(x, X_AXIS), TOSTEPS(y, Y_AXIS), TOSTEPS(z, Z_AXIS),
              TOSTEPS(i, I_AXIS), TOSTEPS(j, J_AXIS), TOSTEPS(k, K_AXIS),
              TOSTEPS(u, U_AXIS), TOSTEPS(v, V_AXIS), TOSTEPS(w, W_AXIS)
            );
          #undef TOSTEPS
          delta = (next_pos - curr_pos);
          delta_max = delta.ABS().large() * OVERSAMPLING;
          traj_consume_i++;
          if (traj_consume_i == (FTM_WINDOW_SIZE)) traj_consume_i = 0;
          trajs++;
        }

        #define SET_DIR(A)                                                      \
            if (delta.A != 0 && ((delta.A > 0) != last_direction_bits.A)) {     \
              delta_error.A *= -1;                                              \
              last_direction_bits.A = !last_direction_bits.A;                   \
            }

          LOGICAL_AXIS_MAP(SET_DIR);
        #undef SET_DIR

        if (delta_max > 0) {
          // Convert (delta / delta_max) to UQ0.32 per-axis in uint32_t clamped to [0, 1)
          XYZEval<float> dividend = delta.ABS() / delta_max;
          // clamp to [0, 1).
          // nextafterf(1, 0) the biggest float smaller than 1 ("returns the next representable value of 'from' in the direction of 'to'")
          const float one_minus = nextafterf(1.0f, 0.0f);
          #define CLAMP(A) LIMIT(dividend.A, 0.0f, one_minus);
            LOGICAL_AXIS_MAP(CLAMP);
          #undef CLAMP

          // “<< 32” as float (exact power-of-two shift), then truncate.
          // ldexpf "Multiplies a floating-point value arg by the number 2 raised to the exp power."
          #define SHIFT32(A) dividend.A = ldexpf(dividend.A, 32);
            LOGICAL_AXIS_MAP(SHIFT32);
          #undef SHIFT32

          advance_dividend = dividend.asULong();  // XYZEval<uint32_t>{ (uint32_t)e, (uint32_t)x, ... }

        }

        static float interval_carry = 0;
        float interval_till_next_traj = STEPPER_TIMER_RATE * FTM_TS * trajs + interval_carry;

        trajs = 0;
        interval = interval_till_next_traj / delta_max + .5;

        steps_pending = FLOOR(delta_max);
        interval_carry = (1 - steps_pending/delta_max) * interval_till_next_traj;
        curr_pos = curr_pos + delta * (float)steps_pending;



        return interval;
      }

      ft_command_t pop_command() {
        ft_command_t cmd = 0;
        if (steps_pending == 0) return cmd;
        steps_pending--;
        #define RUN_AXIS(A)                                                   \
          do {                                                                \
            /* MAGIC: error wraps around when a step is due */                \
            delta_error.A += advance_dividend.A;                              \
            /* MAGIC: do_step_now  is the carry of the previous sum */        \
            ft_command_t do_step_now = delta_error.A < advance_dividend.A;    \
            ft_command_t dir = last_direction_bits.A;                         \
            cmd |= dir         << FT_BIT_DIR_##A;                             \
            cmd |= do_step_now << FT_BIT_STEP_##A;                            \
          } while (0);

        LOGICAL_AXIS_MAP(RUN_AXIS);
        #undef RUN_AXIS
        return cmd;
      }
    } stepping_t;
    static stepping_t stepping;

  private:

    static xyze_trajectory_t traj;

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

    // fill_trajectory_buffer variables.
    static uint32_t traj_consume_i,
                    traj_produce_i;



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
    static void runoutBlock();
    static void fill_trajectory_buffer();
    static bool plan_next_block();
    static bool trajBuff_isFull();
    static bool trajBuff_isEmpty();
    static uint32_t trajBuff_count();

    FORCE_INLINE static int32_t num_samples_shaper_settle() {
      #define _OR_ENA(A) || shaping.A.ena
      return false SHAPED_MAP(_OR_ENA) ? FTM_ZMAX : 0;
      #undef _OR_ENA
    }

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

