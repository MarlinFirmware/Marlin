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

/**
 * ft_motion.cpp - Singleton to execute Fixed Time Motion planning
 *
 * Fixed-Time Motion concept contributed by Ulendo with integration and
 * overhaul optimizations by @thinkyhead, @narno2202, @dbuezas.
 */

#include "../inc/MarlinConfig.h"

#if ENABLED(FT_MOTION)

#include "ft_motion.h"
#include "ft_motion/trajectory_trapezoidal.h"
#include "ft_motion/trajectory_poly5.h"
#include "ft_motion/trajectory_poly6.h"
#if ENABLED(FTM_RESONANCE_TEST)
  #include "ft_motion/resonance_generator.h"
  #include "../gcode/gcode.h" // for home_all_axes
#endif

#include "stepper.h" // Access stepper block queue function and abort status.
#include "endstops.h"

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../feature/powerloss.h"
#endif

FTMotion ftMotion;

//-----------------------------------------------------------------
// Variables.
//-----------------------------------------------------------------

// Public variables.

ft_config_t FTMotion::cfg;
bool FTMotion::busy; // = false

AxisBits FTMotion::moving_axis_flags,           // These axes are moving in the planner block being processed
         FTMotion::axis_move_dir;               // ...in these directions

// Private variables.

// Block data variables.
xyze_pos_t   FTMotion::startPos,                    // (mm) Start position of block
             FTMotion::endPos_prevBlock = { 0.0f }; // (mm) End position of previous block
xyze_float_t FTMotion::ratio;                       // (ratio) Axis move ratio of block
float FTMotion::tau = 0.0f;                         // (s) Time since start of block

// Trajectory generators
TrapezoidalTrajectoryGenerator FTMotion::trapezoidalGenerator;
Poly5TrajectoryGenerator FTMotion::poly5Generator;
Poly6TrajectoryGenerator FTMotion::poly6Generator;
TrajectoryGenerator* FTMotion::currentGenerator = &FTMotion::trapezoidalGenerator;
TrajectoryType FTMotion::trajectoryType = TrajectoryType::FTM_TRAJECTORY_TYPE;

// Resonance Test
TERN_(FTM_RESONANCE_TEST,ResonanceGenerator FTMotion::rtg;) // Resonance trajectory generator instance

// Compact plan buffer
stepper_plan_t FTMotion::stepper_plan_buff[FTM_BUFFER_SIZE];
XYZEval<int64_t> FTMotion::curr_steps_q32_32 = {0};

uint32_t FTMotion::stepper_plan_tail = 0,           // The index to consume from
         FTMotion::stepper_plan_head = 0;           // The index to produce into

#if FTM_HAS_LIN_ADVANCE
  bool FTMotion::use_advance_lead;
#endif

#if ENABLED(DISTINCT_E_FACTORS)
  uint8_t FTMotion::block_extruder_axis;        // Cached E Axis from last-fetched block
#elif HAS_EXTRUDERS
  constexpr uint8_t FTMotion::block_extruder_axis;
#endif

// Shaping variables.
#if HAS_FTM_SHAPING
  shaping_t FTMotion::shaping = {
    zi_idx: 0
    #if HAS_X_AXIS
      , X:{ false, { 0.0f }, { 0.0f }, { 0 }, 0 } // ena, d_zi[], Ai[], Ni[], max_i
    #endif
    #if HAS_Y_AXIS
      , Y:{ false, { 0.0f }, { 0.0f }, { 0 }, 0 }
    #endif
    #if ENABLED(FTM_SHAPER_Z)
      , Z:{ false, { 0.0f }, { 0.0f }, { 0 }, 0 }
    #endif
    #if ENABLED(FTM_SHAPER_E)
      , E:{ false, { 0.0f }, { 0.0f }, { 0 }, 0 }
    #endif
  };
#endif

#if ENABLED(FTM_SMOOTHING)
  smoothing_t FTMotion::smoothing = {
    #if HAS_X_AXIS
      X:{ { 0.0f }, 0.0f, 0 },  // smoothing_pass[], alpha, delay_samples
    #endif
    #if HAS_Y_AXIS
      Y:{ { 0.0f }, 0.0f, 0 },
    #endif
    #if HAS_Z_AXIS
      Z:{ { 0.0f }, 0.0f, 0 },
    #endif
    #if HAS_EXTRUDERS
      E:{ { 0.0f }, 0.0f, 0 }
    #endif
  };
#endif

#if HAS_EXTRUDERS
  // Linear advance variables.
  float FTMotion::prev_traj_e = 0.0f;     // (ms) Unit delay of raw extruder position.
#endif

// Stepping variables.
stepping_t FTMotion::stepping;

//-----------------------------------------------------------------
// Function definitions.
//-----------------------------------------------------------------

// Public functions.

// Controller main, to be invoked from non-isr task.
void FTMotion::loop() {

  if (!cfg.active) return;

  /**
   * Handle block abort with the following sequence:
   * 1. Zero out commands in stepper ISR.
   * 2. Drain the motion buffer, stop processing until they are emptied.
   * 3. Reset all the states / memory.
   * 4. Signal ready for new block.
   */

  const bool using_resonance = TERN(FTM_RESONANCE_TEST, rtg.isActive(), false);

  #if ENABLED(FTM_RESONANCE_TEST)
    if (using_resonance) {
      // Resonance Test has priority over normal ft_motion operation.
      // Process resonance test if active. When it's done, generate the last data points for a clean ending.
      if (rtg.isActive()) {
        if (rtg.isDone()) {
          rtg.abort();
          return;
        }
        rtg.fill_stepper_plan_buffer();
      }
    }
  #endif

  if (!using_resonance) {
    if (stepper.abort_current_block) {
      discard_planner_block_protected();
      reset();
      currentGenerator->planRunout(0.0f);   // Reset generator state
      stepper.abort_current_block = false;  // Abort finished.
    }
    fill_stepper_plan_buffer();
  }

  // Set busy status for use by planner.busy()
  const bool oldBusy = busy;
  busy = stepping.bresenham_iterations_pending > 0 || !stepper_plan_is_empty();
  if (oldBusy && !busy) moving_axis_flags.reset();

}

#if HAS_FTM_SHAPING

  void FTMotion::update_shaping_params() {
    #define UPDATE_SHAPER(A) \
      shaping.A.ena = ftMotion.cfg.shaper.A != ftMotionShaper_NONE; \
      shaping.A.set_axis_shaping_A(cfg.shaper.A, cfg.zeta.A, cfg.vtol.A); \
      shaping.A.set_axis_shaping_N(cfg.shaper.A, cfg.baseFreq.A, cfg.zeta.A);

    SHAPED_MAP(UPDATE_SHAPER);
    shaping.refresh_largest_delay_samples();
  }

#endif // HAS_FTM_SHAPING

#if ENABLED(FTM_SMOOTHING)

  void FTMotion::update_smoothing_params() {
    #define _SMOOTH_PARAM(A) smoothing.A.set_smoothing_time(cfg.smoothingTime.A);
    CARTES_MAP(_SMOOTH_PARAM);
    smoothing.refresh_largest_delay_samples();
  }

  void FTMotion::set_smoothing_time(uint8_t axis, const float s_time) {
    #define _SMOOTH_CASE(A) case _AXIS(A): cfg.smoothingTime.A = s_time; break;
    switch (axis) {
      default:
      CARTES_MAP(_SMOOTH_CASE);
    }
    update_smoothing_params();
  }

#endif // FTM_SMOOTHING

// Reset all trajectory processing variables.
void FTMotion::reset() {
  const bool did_suspend = stepper.suspend();
  endPos_prevBlock.reset();
  tau = 0;
  stepper_plan_tail = stepper_plan_head = 0;
  stepping.reset();
  curr_steps_q32_32.reset();

  #if HAS_FTM_SHAPING
    #define _RESET_ZI(A) ZERO(shaping.A.d_zi);
    SHAPED_MAP(_RESET_ZI);
    shaping.zi_idx = 0;
  #endif

  TERN_(HAS_EXTRUDERS, prev_traj_e = 0.0f);  // Reset linear advance variables.
  TERN_(DISTINCT_E_FACTORS, block_extruder_axis = E_AXIS);

  moving_axis_flags.reset();

  if (did_suspend) stepper.wake_up();
}

// Private functions.

void FTMotion::discard_planner_block_protected() {
  if (stepper.current_block) {  // Safeguard in case current_block must not be null (it will
                                // be null when the "block" is a runout or generated) in order
                                // to use planner.release_current_block().
    stepper.current_block = nullptr;
    planner.release_current_block();  // FTM uses release_current_block() instead of discard_current_block(),
                                      // as in block_phase_isr(). This change is to avoid invoking axis_did_move.reset().
                                      // current_block = nullptr is added to replicate discard without axis_did_move reset.
                                      // Note invoking axis_did_move.reset() causes no issue since FTM's stepper refreshes
                                      // its values every ISR.
  }
}

uint32_t FTMotion::calc_runout_samples() {
  xyze_long_t delay = {0};
  #if ENABLED(FTM_SMOOTHING)
    #define _DELAY_ADD(A) delay.A += smoothing.A.delay_samples;
    LOGICAL_AXIS_MAP(_DELAY_ADD)
    #undef _DELAY_ADD
  #endif

  #if HAS_FTM_SHAPING
    // Ni[max_i] is the delay of the last pulse, but it is relative to Ni[0] (the negative delay centroid)
    #define _DELAY_ADD(A) if (shaping.A.ena) delay.A += shaping.A.Ni[shaping.A.max_i] - shaping.A.Ni[0];
    SHAPED_MAP(_DELAY_ADD)
    #undef _DELAY_ADD
  #endif
  return delay.large();
}

/**
 * Set up a pseudo block to allow motion to settle and buffers to empty.
 * Called when the planner has one block left. The buffers will be filled
 * with the last commanded position by setting the startPos block variable to
 * the last position of the previous block and all ratios to zero such that no
 * axes' positions are incremented.
 */
void FTMotion::plan_runout_block() {
  startPos = endPos_prevBlock;
  currentGenerator->planRunout(calc_runout_samples() * FTM_TS);
  ratio.reset(); // setting ratio to zero means no motion on any axis
}

// Initializes storage variables before startup.
void FTMotion::init() {
  update_shaping_params();
  TERN_(FTM_SMOOTHING, update_smoothing_params());
  setTrajectoryType(cfg.trajectory_type);
  reset(); // Precautionary.
}

// Set trajectory generator type
void FTMotion::setTrajectoryType(const TrajectoryType type) {
  cfg.trajectory_type = trajectoryType = type;
  switch (type) {
    default: cfg.trajectory_type = trajectoryType = TrajectoryType::FTM_TRAJECTORY_TYPE;
    case TrajectoryType::TRAPEZOIDAL: currentGenerator = &trapezoidalGenerator; break;
    case TrajectoryType::POLY5:       currentGenerator = &poly5Generator; break;
    case TrajectoryType::POLY6:       currentGenerator = &poly6Generator; break;
  }
}

// Load / convert block data from planner to fixed-time control variables.
// Called from FTMotion::loop() at the fetch of the next planner block.
// Return whether a plan is available.
bool FTMotion::plan_next_block() {
  while (true) {

    const bool had_block = !!stepper.current_block;
    discard_planner_block_protected();                                  // Always clears stepper.current_block...
    block_t * const current_block = planner.get_current_block();  // ...so get the current block from the queue

    // The planner had a block and there was not another one?
    const bool planner_finished = had_block && !current_block;
    if (planner_finished) {
      plan_runout_block();
      return true;
    }

    // There was never a block? Run out the plan and bail.
    if (!current_block) {
      currentGenerator->planRunout(0);
      return false;
    }

    // Fetching this block for Stepper and for this loop
    stepper.current_block = current_block;

    // Handle sync blocks and skip others
    if (current_block->is_sync()) {
      if (current_block->is_sync_pos()) stepper._set_position(current_block->position);
      continue;
    }

    #if ENABLED(POWER_LOSS_RECOVERY)
      recovery.info.sdpos = current_block->sdpos;
      recovery.info.current_position = current_block->start_position;
    #endif

    // Some kinematics track axis motion in HX, HY, HZ
    #if ANY(CORE_IS_XY, CORE_IS_XZ, MARKFORGED_XY, MARKFORGED_YX)
      stepper.last_direction_bits.hx = current_block->direction_bits.hx;
    #endif
    #if ANY(CORE_IS_XY, CORE_IS_YZ, MARKFORGED_XY, MARKFORGED_YX)
      stepper.last_direction_bits.hy = current_block->direction_bits.hy;
    #endif
    #if ANY(CORE_IS_XZ, CORE_IS_YZ)
      stepper.last_direction_bits.hz = current_block->direction_bits.hz;
    #endif

    // Cache the extruder index for this block
    TERN_(DISTINCT_E_FACTORS, block_extruder_axis = E_AXIS_N(current_block->extruder));

    const float totalLength = current_block->millimeters;

    startPos = endPos_prevBlock;
    const xyze_pos_t& moveDist = current_block->dist_mm;
    ratio = moveDist / totalLength;

    const float mmps = totalLength / current_block->step_event_count, // (mm/step) Distance for each step
                initial_speed = mmps * current_block->initial_rate,   // (mm/s) Start feedrate
                final_speed = mmps * current_block->final_rate;       // (mm/s) End feedrate

    // Plan the trajectory using the trajectory generator
    currentGenerator->plan(initial_speed, final_speed, current_block->acceleration, current_block->nominal_speed, totalLength);

    endPos_prevBlock += moveDist;

    TERN_(FTM_HAS_LIN_ADVANCE, use_advance_lead = current_block->use_advance_lead);

    #define _SET_MOVE_END(A) do{ \
      if (moveDist.A) { \
        moving_axis_flags.A = true; \
        axis_move_dir.A = moveDist.A > 0; \
      } \
    }while(0);

    LOGICAL_AXIS_MAP(_SET_MOVE_END);

    // If the endstop is already pressed, endstop interrupts won't invoke
    // endstop_triggered and the move will grind. So check here for a
    // triggered endstop, which marks the block for discard on the next ISR.
    endstops.update();

    return true;
  }
}

xyze_float_t FTMotion::calc_traj_point(const float dist) {
  xyze_float_t traj_coords;
  #define _SET_TRAJ(q) traj_coords.q = startPos.q + ratio.q * dist;
  LOGICAL_AXIS_MAP_LC(_SET_TRAJ);

  #if FTM_HAS_LIN_ADVANCE
    const float advK = planner.get_advance_k();
    if (advK) {
      const float traj_e = traj_coords.e;
      if (use_advance_lead) {
        // Don't apply LA to retract/unretract blocks
        const float e_rate = (traj_e - prev_traj_e) * (FTM_FS);
        traj_coords.e += e_rate * advK;
      }
      prev_traj_e = traj_e;
    }
  #endif

  // Update shaping parameters if needed.
  switch (cfg.dynFreqMode) {
    #if HAS_DYNAMIC_FREQ_MM
      case dynFreqMode_Z_BASED: {
        static float oldz = 0.0f;
        const float z = traj_coords.z;
        if (z != oldz) { // Only update if Z changed.
          oldz = z;
          #if HAS_X_AXIS
            const float xf = cfg.baseFreq.x + cfg.dynFreqK.x * z;
            shaping.X.set_axis_shaping_N(cfg.shaper.x, _MAX(xf, FTM_MIN_SHAPE_FREQ), cfg.zeta.x);
          #endif
          #if HAS_Y_AXIS
            const float yf = cfg.baseFreq.y + cfg.dynFreqK.y * z;
            shaping.Y.set_axis_shaping_N(cfg.shaper.y, _MAX(yf, FTM_MIN_SHAPE_FREQ), cfg.zeta.y);
          #endif
          shaping.refresh_largest_delay_samples();
        }
      } break;
    #endif

    #if HAS_DYNAMIC_FREQ_G
      case dynFreqMode_MASS_BASED:
        // Update constantly. The optimization done for Z value makes
        // less sense for E, as E is expected to constantly change.
        #if HAS_X_AXIS
          shaping.X.set_axis_shaping_N(cfg.shaper.x, cfg.baseFreq.x + cfg.dynFreqK.x * traj_coords.e, cfg.zeta.x);
        #endif
        #if HAS_Y_AXIS
          shaping.Y.set_axis_shaping_N(cfg.shaper.y, cfg.baseFreq.y + cfg.dynFreqK.y * traj_coords.e, cfg.zeta.y);
        #endif
        shaping.refresh_largest_delay_samples();
        break;
    #endif

    default: break;
  }

  #if ANY(FTM_SMOOTHING, HAS_FTM_SHAPING)
    uint32_t max_total_delay = 0;
  #endif

  #if ENABLED(FTM_SMOOTHING)

    #define _SMOOTHEN(A) /* Approximate gaussian smoothing via chained EMAs */ \
      if (smoothing.A.alpha > 0.0f) { \
        float smooth_val = traj_coords.A; \
        for (uint8_t _i = 0; _i < FTM_SMOOTHING_ORDER; ++_i) { \
          smoothing.A.smoothing_pass[_i] += (smooth_val - smoothing.A.smoothing_pass[_i]) * smoothing.A.alpha; \
          smooth_val = smoothing.A.smoothing_pass[_i]; \
        } \
        traj_coords.A = smooth_val; \
      }

    CARTES_MAP(_SMOOTHEN);
    max_total_delay += smoothing.largest_delay_samples;

  #endif // FTM_SMOOTHING

  #if HAS_FTM_SHAPING

    if (ftMotion.cfg.axis_sync_enabled)
      max_total_delay += shaping.largest_delay_samples;

    // Apply shaping if active on each axis
    #define _SHAPE(A) \
      do { \
        const uint32_t group_delay = ftMotion.cfg.axis_sync_enabled \
            ? max_total_delay - TERN0(FTM_SMOOTHING, smoothing.A.delay_samples) \
            : -shaping.A.Ni[0]; \
        /* α=1−exp(−(dt / (τ / order))) */ \
        shaping.A.d_zi[shaping.zi_idx] = traj_coords.A; \
        traj_coords.A = 0; \
        for (uint32_t i = 0; i <= shaping.A.max_i; i++) { \
          /* echo_delay is always positive since Ni[i] = echo_relative_delay - group_delay + max_total_delay */ \
          /* where echo_relative_delay > 0 and group_delay ≤ max_total_delay */ \
          const uint32_t echo_delay = group_delay + shaping.A.Ni[i]; \
          int32_t udiff = shaping.zi_idx - echo_delay; \
          if (udiff < 0) udiff += FTM_ZMAX; \
          traj_coords.A += shaping.A.Ai[i] * shaping.A.d_zi[udiff]; \
        } \
      } while (0);

    SHAPED_MAP(_SHAPE);

    if (++shaping.zi_idx == (FTM_ZMAX)) shaping.zi_idx = 0;

  #endif // HAS_FTM_SHAPING

  return traj_coords;
}

stepper_plan_t FTMotion::calc_stepper_plan(xyze_float_t traj_coords) {
  // 1) Convert trajectory to step delta
  #define _TOSTEPS_q32(A, B) int64_t(traj_coords.A * planner.settings.axis_steps_per_mm[B] * (1ULL << 32))
  XYZEval<int64_t> next_steps_q32_32 = LOGICAL_AXIS_ARRAY(
    _TOSTEPS_q32(e, block_extruder_axis),
    _TOSTEPS_q32(x, X_AXIS), _TOSTEPS_q32(y, Y_AXIS), _TOSTEPS_q32(z, Z_AXIS),
    _TOSTEPS_q32(i, I_AXIS), _TOSTEPS_q32(j, J_AXIS), _TOSTEPS_q32(k, K_AXIS),
    _TOSTEPS_q32(u, U_AXIS), _TOSTEPS_q32(v, V_AXIS), _TOSTEPS_q32(w, W_AXIS)
  );
  #undef _TOSTEPS_q32

  constexpr uint32_t ITERATIONS_PER_TRAJ_INV_uq0_32 = (1ULL << 32) / ITERATIONS_PER_TRAJ;
  stepper_plan_t stepper_plan;

  #define _RUN_AXIS(A) do{                                                                                   \
      int64_t delta_q32_32 = (next_steps_q32_32.A - curr_steps_q32_32.A);                                    \
      /* 2) Set stepper plan direction bits */                                                               \
      int16_t sign = (delta_q32_32 > 0) - (delta_q32_32 < 0);                                                \
      stepper_plan.dir_bits.A = delta_q32_32 > 0;                                                            \
      /* 3) Set per-iteration advance dividend Q0.32 */                                                      \
      uint64_t delta_uq32_32 = ABS(delta_q32_32);                                                            \
      /* dividend = delta_q32_32 / ITERATIONS_PER_TRAJ, but avoiding division and an intermediate int128 */  \
      /* Note the integer part would overflow if there is eq or more than 1 steps per isr */                 \
      uint32_t integer_part = (delta_uq32_32 >> 32) * ITERATIONS_PER_TRAJ_INV_uq0_32;                        \
      uint32_t fractional_part = ((delta_uq32_32 & UINT32_MAX) * ITERATIONS_PER_TRAJ_INV_uq0_32) >> 32;      \
      stepper_plan.advance_dividend_q0_32.A = integer_part + fractional_part;                                \
      /* 4) Advance curr_steps by the exact integer steps that Bresenham will emit */                        \
      /* It's like doing current_steps = next_steps, but considering any fractional error */                 \
      /* from the dividend. This way there can be no drift. */                                               \
      curr_steps_q32_32.A += (int64_t)stepper_plan.advance_dividend_q0_32.A * sign * ITERATIONS_PER_TRAJ;    \
    } while(0);
  LOGICAL_AXIS_MAP(_RUN_AXIS);
  #undef _RUN_AXIS

  return stepper_plan;
}

/**
 * Generate stepper data of the trajectory.
 * Called from FTMotion::loop()
 */
void FTMotion::fill_stepper_plan_buffer() {
  while (!stepper_plan_is_full()) {
    float total_duration = currentGenerator->getTotalDuration(); // If the current plan is empty, it will have zero duration.
    while (tau + FTM_TS > total_duration) {
      /**
       * We’ve reached the end of the current block.
       *
       * `tau` is the time that has elapsed inside this block. After a block is finished, the next one may
       * start at any point between *just before* the last sampled time (one step earlier, i.e. `-FTM_TS`)
       * and *exactly at* the last sampled time (0). IOW the real start of the next block could be anywhere
       * in the interval (-FTM_TS, 0].
       *
       * To account for that uncertainty we simply subtract the duration of the finished block from `tau`.
       * This brings us back to a time value that is valid for the next block, while still allowing the next
       * block’s start to be offset by up to one time step into the past.
       */
      tau -= total_duration;
      const bool plan_available = plan_next_block();
      if (!plan_available) return;
      total_duration = currentGenerator->getTotalDuration();
    }
    tau += FTM_TS; // (s) Time since start of block

    // Get distance from trajectory generator
    xyze_float_t traj_coords = calc_traj_point(currentGenerator->getDistanceAtTime(tau));

    stepper_plan_t plan = calc_stepper_plan(traj_coords);

    // Store in buffer
    enqueue_stepper_plan(plan);

  }
}

#if ENABLED(FTM_RESONANCE_TEST)

  // Start Resonance Testing
  void FTMotion::start_resonance_test() {
    home_if_needed(); // Ensure known axes first

    ftm_resonance_test_params_t &p = rtg.rt_params;

    // Safe Acceleration per Hz for Z axis
    if (p.axis == Z_AXIS && p.accel_per_hz > 15.0f)
      p.accel_per_hz = 15.0f;

    // Always move to the center of the bed
    do_blocking_move_to_xy(X_CENTER, Y_CENTER, Z_CLEARANCE_FOR_HOMING);

    // Start test at the current position with the configured time-step
    rtg.start(current_position, FTM_TS);
  }

#endif // FTM_RESONANCE_TEST

#endif // FT_MOTION
