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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * stepper.h - stepper motor driver: executes motion plans of planner.c using the stepper motors
 * Derived from Grbl
 *
 * Copyright (c) 2009-2011 Simen Svale Skogsrud
 *
 * Grbl is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Grbl is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Grbl.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "../inc/MarlinConfig.h"

#include "planner.h"
#include "stepper/indirection.h"
#include "stepper/cycles.h"
#ifdef __AVR__
  #include "stepper/speed_lookuptable.h"
#endif

#if ENABLED(FT_MOTION)
  #include "ft_types.h"
#endif

// TODO: Review and ensure proper handling for special E axes with commands like M17/M18, stepper timeout, etc.
#if ENABLED(MIXING_EXTRUDER)
  #define E_STATES EXTRUDERS  // All steppers are set together for each mixer. (Currently limited to 1.)
#elif HAS_SWITCHING_EXTRUDER
  #define E_STATES E_STEPPERS // One stepper for every two EXTRUDERS. The last extruder can be non-switching.
#elif HAS_PRUSA_MMU2
  #define E_STATES E_STEPPERS // One E stepper shared with all EXTRUDERS, so setting any only sets one.
#else
  #define E_STATES E_STEPPERS // One stepper for each extruder, so each can be disabled individually.
#endif

// Number of axes that could be enabled/disabled. Dual/multiple steppers are combined.
#define ENABLE_COUNT (NUM_AXES + E_STATES)
typedef bits_t(ENABLE_COUNT) ena_mask_t;

// Axis flags type, for enabled state or other simple state
typedef struct {
  union {
    ena_mask_t bits;
    struct {
      #if NUM_AXES
        bool NUM_AXIS_LIST(X:1, Y:1, Z:1, I:1, J:1, K:1, U:1, V:1, W:1);
      #endif
      #if E_STATES
        bool LIST_N(E_STATES, E0:1, E1:1, E2:1, E3:1, E4:1, E5:1, E6:1, E7:1);
      #endif
    };
  };
} stepper_flags_t;

typedef bits_t(NUM_AXES + E_STATES) e_axis_bits_t;
constexpr e_axis_bits_t e_axis_mask = (_BV(E_STATES) - 1) << NUM_AXES;

// All the stepper enable pins
constexpr pin_t ena_pins[] = {
  NUM_AXIS_LIST_(X_ENABLE_PIN, Y_ENABLE_PIN, Z_ENABLE_PIN, I_ENABLE_PIN, J_ENABLE_PIN, K_ENABLE_PIN, U_ENABLE_PIN, V_ENABLE_PIN, W_ENABLE_PIN)
  LIST_N(E_STEPPERS, E0_ENABLE_PIN, E1_ENABLE_PIN, E2_ENABLE_PIN, E3_ENABLE_PIN, E4_ENABLE_PIN, E5_ENABLE_PIN, E6_ENABLE_PIN, E7_ENABLE_PIN)
};

// Index of the axis or extruder element in a combined array
constexpr uint8_t index_of_axis(const AxisEnum axis E_OPTARG(const uint8_t eindex=0)) {
  return uint8_t(axis) + (E_TERN0(axis < NUM_AXES ? 0 : eindex));
}
//#define __IAX_N(N,V...)           _IAX_##N(V)
//#define _IAX_N(N,V...)            __IAX_N(N,V)
//#define _IAX_1(A)                 index_of_axis(A)
//#define _IAX_2(A,B)               index_of_axis(A E_OPTARG(B))
//#define INDEX_OF_AXIS(V...)       _IAX_N(TWO_ARGS(V),V)

#define INDEX_OF_AXIS(A,V...)     index_of_axis(A E_OPTARG(V+0))

// Bit mask for a matching enable pin, or 0
constexpr ena_mask_t ena_same(const uint8_t a, const uint8_t b) {
  return ena_pins[a] == ena_pins[b] ? _BV(b) : 0;
}

// Recursively get the enable overlaps mask for a given linear axis or extruder
constexpr ena_mask_t ena_overlap(const uint8_t a=0, const uint8_t b=0) {
  return b >= ENABLE_COUNT ? 0 : (a == b ? 0 : ena_same(a, b)) | ena_overlap(a, b + 1);
}

// Recursively get whether there's any overlap at all
constexpr bool any_enable_overlap(const uint8_t a=0) {
  return a >= ENABLE_COUNT ? false : ena_overlap(a) || any_enable_overlap(a + 1);
}

// Array of axes that overlap with each
// TODO: Consider cases where >=2 steppers are used by a linear axis or extruder
//       (e.g., CoreXY, Dual XYZ, or E with multiple steppers, etc.).
constexpr ena_mask_t enable_overlap[] = {
  #define _OVERLAP(N) ena_overlap(INDEX_OF_AXIS(AxisEnum(N))),
  REPEAT(NUM_AXES, _OVERLAP)
  #if HAS_EXTRUDERS
    #define _E_OVERLAP(N) ena_overlap(INDEX_OF_AXIS(E_AXIS, N)),
    REPEAT(E_STEPPERS, _E_OVERLAP)
  #endif
};

//static_assert(!any_enable_overlap(), "There is some overlap.");

#if HAS_ZV_SHAPING

  #ifdef SHAPING_MAX_STEPRATE
    constexpr float max_step_rate = SHAPING_MAX_STEPRATE;
  #else
    #define ISALIM(I, ARR) _MIN(I, COUNT(ARR) - 1)
    constexpr float     _ISDASU[] = DEFAULT_AXIS_STEPS_PER_UNIT;
    constexpr feedRate_t _ISDMF[] = DEFAULT_MAX_FEEDRATE;
    constexpr float max_shaped_rate = TERN0(INPUT_SHAPING_X, _ISDMF[X_AXIS] * _ISDASU[X_AXIS]) +
                                      TERN0(INPUT_SHAPING_Y, _ISDMF[Y_AXIS] * _ISDASU[Y_AXIS]);
    #if defined(__AVR__) || !defined(ADAPTIVE_STEP_SMOOTHING)
      // MIN_STEP_ISR_FREQUENCY is known at compile time on AVRs and any reduction in SRAM is welcome
      template<unsigned int INDEX=DISTINCT_AXES> constexpr float max_isr_rate() {
        return _MAX(_ISDMF[ISALIM(INDEX - 1, _ISDMF)] * _ISDASU[ISALIM(INDEX - 1, _ISDASU)], max_isr_rate<INDEX - 1>());
      }
      template<> constexpr float max_isr_rate<0>() {
        return TERN0(ADAPTIVE_STEP_SMOOTHING, MIN_STEP_ISR_FREQUENCY);
      }
      constexpr float max_step_rate = _MIN(max_isr_rate(), max_shaped_rate);
    #else
      constexpr float max_step_rate = max_shaped_rate;
    #endif
  #endif

  #ifndef SHAPING_MIN_FREQ
    #define SHAPING_MIN_FREQ _MIN(0x7FFFFFFFL OPTARG(INPUT_SHAPING_X, SHAPING_FREQ_X) OPTARG(INPUT_SHAPING_Y, SHAPING_FREQ_Y))
  #endif
  constexpr uint16_t shaping_min_freq = SHAPING_MIN_FREQ,
                     shaping_echoes = max_step_rate / shaping_min_freq / 2 + 3;

  typedef hal_timer_t shaping_time_t;
  enum shaping_echo_t { ECHO_NONE = 0, ECHO_FWD = 1, ECHO_BWD = 2 };
  struct shaping_echo_axis_t {
    TERN_(INPUT_SHAPING_X, shaping_echo_t x:2);
    TERN_(INPUT_SHAPING_Y, shaping_echo_t y:2);
  };

  class ShapingQueue {
    private:
      static shaping_time_t       now;
      static shaping_time_t       times[shaping_echoes];
      static shaping_echo_axis_t  echo_axes[shaping_echoes];
      static uint16_t             tail;

      #if ENABLED(INPUT_SHAPING_X)
        static shaping_time_t delay_x;    // = shaping_time_t(-1) to disable queueing
        static shaping_time_t peek_x_val;
        static uint16_t head_x;
        static uint16_t _free_count_x;
      #endif
      #if ENABLED(INPUT_SHAPING_Y)
        static shaping_time_t delay_y;    // = shaping_time_t(-1) to disable queueing
        static shaping_time_t peek_y_val;
        static uint16_t head_y;
        static uint16_t _free_count_y;
      #endif

    public:
      static void decrement_delays(const shaping_time_t interval) {
        now += interval;
        TERN_(INPUT_SHAPING_X, if (peek_x_val != shaping_time_t(-1)) peek_x_val -= interval);
        TERN_(INPUT_SHAPING_Y, if (peek_y_val != shaping_time_t(-1)) peek_y_val -= interval);
      }
      static void set_delay(const AxisEnum axis, const shaping_time_t delay) {
        TERN_(INPUT_SHAPING_X, if (axis == X_AXIS) delay_x = delay);
        TERN_(INPUT_SHAPING_Y, if (axis == Y_AXIS) delay_y = delay);
      }
      static void enqueue(const bool x_step, const bool x_forward, const bool y_step, const bool y_forward) {
        #if ENABLED(INPUT_SHAPING_X)
          if (x_step) {
            if (head_x == tail) peek_x_val = delay_x;
            echo_axes[tail].x = x_forward ? ECHO_FWD : ECHO_BWD;
            _free_count_x--;
          }
          else {
            echo_axes[tail].x = ECHO_NONE;
            if (head_x != tail)
              _free_count_x--;
            else if (++head_x == shaping_echoes)
              head_x = 0;
          }
        #endif
        #if ENABLED(INPUT_SHAPING_Y)
          if (y_step) {
            if (head_y == tail) peek_y_val = delay_y;
            echo_axes[tail].y = y_forward ? ECHO_FWD : ECHO_BWD;
            _free_count_y--;
          }
          else {
            echo_axes[tail].y = ECHO_NONE;
            if (head_y != tail)
              _free_count_y--;
            else if (++head_y == shaping_echoes)
              head_y = 0;
          }
        #endif
        times[tail] = now;
        if (++tail == shaping_echoes) tail = 0;
      }
      #if ENABLED(INPUT_SHAPING_X)
        static shaping_time_t peek_x() { return peek_x_val; }
        static bool dequeue_x() {
          bool forward = echo_axes[head_x].x == ECHO_FWD;
          do {
            _free_count_x++;
            if (++head_x == shaping_echoes) head_x = 0;
          } while (head_x != tail && echo_axes[head_x].x == ECHO_NONE);
          peek_x_val = head_x == tail ? shaping_time_t(-1) : times[head_x] + delay_x - now;
          return forward;
        }
        static bool empty_x() { return head_x == tail; }
        static uint16_t free_count_x() { return _free_count_x; }
      #endif
      #if ENABLED(INPUT_SHAPING_Y)
        static shaping_time_t peek_y() { return peek_y_val; }
        static bool dequeue_y() {
          bool forward = echo_axes[head_y].y == ECHO_FWD;
          do {
            _free_count_y++;
            if (++head_y == shaping_echoes) head_y = 0;
          } while (head_y != tail && echo_axes[head_y].y == ECHO_NONE);
          peek_y_val = head_y == tail ? shaping_time_t(-1) : times[head_y] + delay_y - now;
          return forward;
        }
        static bool empty_y() { return head_y == tail; }
        static uint16_t free_count_y() { return _free_count_y; }
      #endif
      static void purge() {
        const auto st = shaping_time_t(-1);
        #if ENABLED(INPUT_SHAPING_X)
          head_x = tail; _free_count_x = shaping_echoes - 1; peek_x_val = st;
        #endif
        #if ENABLED(INPUT_SHAPING_Y)
          head_y = tail; _free_count_y = shaping_echoes - 1; peek_y_val = st;
        #endif
      }
  };

  struct ShapeParams {
    float frequency;
    float zeta;
    bool enabled : 1;
    bool forward : 1;
    int16_t delta_error = 0;    // delta_error for seconday bresenham mod 128
    uint8_t factor1;
    uint8_t factor2;
    int32_t last_block_end_pos = 0;
  };

#endif // HAS_ZV_SHAPING

#if ENABLED(NONLINEAR_EXTRUSION)
  typedef struct { float A, B, C; void reset() { A = B = 0.0f; C = 1.0f; } } ne_coeff_t;
  typedef struct { int32_t A, B, C; } ne_fix_t;
#endif

//
// Stepper class definition
//
class Stepper {
  friend class Max7219;
  friend class FTMotion;
  friend void stepperTask(void *);

  public:

    #if ANY(HAS_EXTRA_ENDSTOPS, Z_STEPPER_AUTO_ALIGN)
      static bool separate_multi_axis;
    #endif

    #if HAS_MOTOR_CURRENT_SPI || HAS_MOTOR_CURRENT_PWM
      #if HAS_MOTOR_CURRENT_PWM
        #ifndef PWM_MOTOR_CURRENT
          #define PWM_MOTOR_CURRENT DEFAULT_PWM_MOTOR_CURRENT
        #endif
        #ifndef MOTOR_CURRENT_PWM_FREQUENCY
          #define MOTOR_CURRENT_PWM_FREQUENCY 31400
        #endif
        #define MOTOR_CURRENT_COUNT 3
      #elif HAS_MOTOR_CURRENT_SPI
        static constexpr uint32_t digipot_count[] = DIGIPOT_MOTOR_CURRENT;
        #define MOTOR_CURRENT_COUNT COUNT(Stepper::digipot_count)
      #endif
      static bool initialized;
      static uint32_t motor_current_setting[MOTOR_CURRENT_COUNT]; // Initialized by settings.load()
    #endif

    // Last-moved extruder, as set when the last movement was fetched from planner
    #if HAS_MULTI_EXTRUDER
      static uint8_t last_moved_extruder;
    #else
      static constexpr uint8_t last_moved_extruder = 0;
    #endif

    #if ENABLED(FREEZE_FEATURE)
      static bool frozen;                 // Set this flag to instantly freeze motion
    #endif

    #if ENABLED(NONLINEAR_EXTRUSION)
      static ne_coeff_t ne;
    #endif

  private:

    static block_t* current_block;        // A pointer to the block currently being traced

    static AxisBits last_direction_bits,  // The next stepping-bits to be output
                    axis_did_move;        // Last Movement in the given direction is not null, as computed when the last movement was fetched from planner

    static bool abort_current_block;      // Signals to the stepper that current block should be aborted

    #if ENABLED(X_DUAL_ENDSTOPS)
      static bool locked_X_motor, locked_X2_motor;
    #endif
    #if ENABLED(Y_DUAL_ENDSTOPS)
      static bool locked_Y_motor, locked_Y2_motor;
    #endif
    #if ANY(Z_MULTI_ENDSTOPS, Z_STEPPER_AUTO_ALIGN)
      static bool locked_Z_motor, locked_Z2_motor
                  #if NUM_Z_STEPPERS >= 3
                    , locked_Z3_motor
                    #if NUM_Z_STEPPERS >= 4
                      , locked_Z4_motor
                    #endif
                  #endif
                  ;
    #endif

    static uint32_t acceleration_time, deceleration_time; // time measured in Stepper Timer ticks

    #if MULTISTEPPING_LIMIT == 1
      static constexpr uint8_t steps_per_isr = 1; // Count of steps to perform per Stepper ISR call
    #else
      static uint8_t steps_per_isr;
    #endif

    #if DISABLED(OLD_ADAPTIVE_MULTISTEPPING)
      static hal_timer_t time_spent_in_isr, time_spent_out_isr;
    #endif

    #if ENABLED(ADAPTIVE_STEP_SMOOTHING)
      static uint8_t oversampling_factor; // Oversampling factor (log2(multiplier)) to increase temporal resolution of axis
    #else
      static constexpr uint8_t oversampling_factor = 0;
    #endif

    // Delta error variables for the Bresenham line tracer
    static xyze_long_t delta_error;
    static xyze_long_t advance_dividend;
    static uint32_t advance_divisor,
                    step_events_completed,  // The number of step events executed in the current block
                    accelerate_until,       // The point from where we need to stop acceleration
                    decelerate_after,       // The point from where we need to start decelerating
                    step_event_count;       // The total event count for the current block

    #if ANY(HAS_MULTI_EXTRUDER, MIXING_EXTRUDER)
      static uint8_t stepper_extruder;
    #else
      static constexpr uint8_t stepper_extruder = 0;
    #endif

    #if ENABLED(S_CURVE_ACCELERATION)
      static int32_t bezier_A,     // A coefficient in Bézier speed curve
                     bezier_B,     // B coefficient in Bézier speed curve
                     bezier_C;     // C coefficient in Bézier speed curve
      static uint32_t bezier_F,    // F coefficient in Bézier speed curve
                      bezier_AV;   // AV coefficient in Bézier speed curve
      #ifdef __AVR__
        static bool A_negative;    // If A coefficient was negative
      #endif
      static bool bezier_2nd_half; // If Bézier curve has been initialized or not
    #endif

    #if HAS_ZV_SHAPING
      #if ENABLED(INPUT_SHAPING_X)
        static ShapeParams shaping_x;
      #endif
      #if ENABLED(INPUT_SHAPING_Y)
        static ShapeParams shaping_y;
      #endif
    #endif

    #if ENABLED(LIN_ADVANCE)
      static constexpr hal_timer_t LA_ADV_NEVER = HAL_TIMER_TYPE_MAX;
      static hal_timer_t nextAdvanceISR,
                         la_interval;      // Interval between ISR calls for LA
      static int32_t     la_delta_error,   // Analogue of delta_error.e for E steps in LA ISR
                         la_dividend,      // Analogue of advance_dividend.e for E steps in LA ISR
                         la_advance_steps; // Count of steps added to increase nozzle pressure
      static bool        la_active;        // Whether linear advance is used on the present segment.
    #endif

    #if ENABLED(NONLINEAR_EXTRUSION)
      static int32_t ne_edividend;
      static uint32_t ne_scale;
      static ne_fix_t ne_fix;
    #endif

    #if ENABLED(BABYSTEPPING)
      static constexpr hal_timer_t BABYSTEP_NEVER = HAL_TIMER_TYPE_MAX;
      static hal_timer_t nextBabystepISR;
    #endif

    #if ENABLED(DIRECT_STEPPING)
      static page_step_state_t page_step_state;
    #endif

    static hal_timer_t ticks_nominal;
    #if DISABLED(S_CURVE_ACCELERATION)
      static uint32_t acc_step_rate; // needed for deceleration start point
    #endif

    // Exact steps at which an endstop was triggered
    static xyz_long_t endstops_trigsteps;

    // Positions of stepper motors, in step units
    static xyze_long_t count_position;

    // Current stepper motor directions (+1 or -1)
    static xyze_int8_t count_direction;

  public:
    // Initialize stepper hardware
    static void init();

    // Interrupt Service Routine and phases

    // The stepper subsystem goes to sleep when it runs out of things to execute.
    // Call this to notify the subsystem that it is time to go to work.
    static void wake_up() { ENABLE_STEPPER_DRIVER_INTERRUPT(); }

    static bool is_awake() { return STEPPER_ISR_ENABLED(); }

    static bool suspend() {
      const bool awake = is_awake();
      if (awake) DISABLE_STEPPER_DRIVER_INTERRUPT();
      return awake;
    }

    // The ISR scheduler
    static void isr();

    // The stepper pulse ISR phase
    static void pulse_phase_isr();

    // The stepper block processing ISR phase
    static hal_timer_t block_phase_isr();

    #if HAS_ZV_SHAPING
      static void shaping_isr();
    #endif

    #if ENABLED(LIN_ADVANCE)
      // The Linear advance ISR phase
      static void advance_isr();
    #endif

    #if ENABLED(BABYSTEPPING)
      // The Babystepping ISR phase
      static hal_timer_t babystepping_isr();
      FORCE_INLINE static void initiateBabystepping() {
        if (nextBabystepISR == BABYSTEP_NEVER) {
          nextBabystepISR = 0;
          wake_up();
        }
      }
    #endif

    // Check if the given block is busy or not - Must not be called from ISR contexts
    static bool is_block_busy(const block_t * const block);

    #if HAS_ZV_SHAPING
      // Check whether the stepper is processing any input shaping echoes
      static bool input_shaping_busy() {
        const bool was_on = hal.isr_state();
        hal.isr_off();

        const bool result = TERN0(INPUT_SHAPING_X, !ShapingQueue::empty_x()) || TERN0(INPUT_SHAPING_Y, !ShapingQueue::empty_y());

        if (was_on) hal.isr_on();

        return result;
      }
    #endif

    // Get the position of a stepper, in steps
    static int32_t position(const AxisEnum axis);

    // Set the current position in steps
    static void set_position(const xyze_long_t &spos);
    static void set_axis_position(const AxisEnum a, const int32_t &v);

    // Report the positions of the steppers, in steps
    static void report_a_position(const xyz_long_t &pos);
    static void report_positions();

    // Discard current block and free any resources
    FORCE_INLINE static void discard_current_block() {
      #if ENABLED(DIRECT_STEPPING)
        if (current_block->is_page()) page_manager.free_page(current_block->page_idx);
      #endif
      current_block = nullptr;
      axis_did_move.reset();
      planner.release_current_block();
      TERN_(LIN_ADVANCE, la_interval = nextAdvanceISR = LA_ADV_NEVER);
    }

    // Quickly stop all steppers
    FORCE_INLINE static void quick_stop() { abort_current_block = true; }

    // The direction of a single motor. A true result indicates forward or positive motion.
    FORCE_INLINE static bool motor_direction(const AxisEnum axis) { return last_direction_bits[axis]; }

    // The last movement direction was not null on the specified axis. Note that motor direction is not necessarily the same.
    FORCE_INLINE static bool axis_is_moving(const AxisEnum axis) { return axis_did_move[axis]; }

    // Handle a triggered endstop
    static void endstop_triggered(const AxisEnum axis);

    // Triggered position of an axis in steps
    static int32_t triggered_position(const AxisEnum axis);

    #if HAS_MOTOR_CURRENT_SPI || HAS_MOTOR_CURRENT_PWM
      static void set_digipot_value_spi(const int16_t address, const int16_t value);
      static void set_digipot_current(const uint8_t driver, const int16_t current);
    #endif

    #if HAS_MICROSTEPS
      static void microstep_ms(const uint8_t driver, const int8_t ms1, const int8_t ms2, const int8_t ms3);
      static void microstep_mode(const uint8_t driver, const uint8_t stepping);
      static void microstep_readings();
    #endif

    #if ANY(HAS_EXTRA_ENDSTOPS, Z_STEPPER_AUTO_ALIGN)
      FORCE_INLINE static void set_separate_multi_axis(const bool state) { separate_multi_axis = state; }
    #endif
    #if ENABLED(X_DUAL_ENDSTOPS)
      FORCE_INLINE static void set_x_lock(const bool state) { locked_X_motor = state; }
      FORCE_INLINE static void set_x2_lock(const bool state) { locked_X2_motor = state; }
    #endif
    #if ENABLED(Y_DUAL_ENDSTOPS)
      FORCE_INLINE static void set_y_lock(const bool state) { locked_Y_motor = state; }
      FORCE_INLINE static void set_y2_lock(const bool state) { locked_Y2_motor = state; }
    #endif
    #if ANY(Z_MULTI_ENDSTOPS, Z_STEPPER_AUTO_ALIGN)
      FORCE_INLINE static void set_z1_lock(const bool state) { locked_Z_motor = state; }
      FORCE_INLINE static void set_z2_lock(const bool state) { locked_Z2_motor = state; }
      #if NUM_Z_STEPPERS >= 3
        FORCE_INLINE static void set_z3_lock(const bool state) { locked_Z3_motor = state; }
        #if NUM_Z_STEPPERS >= 4
          FORCE_INLINE static void set_z4_lock(const bool state) { locked_Z4_motor = state; }
        #endif
      #endif
      static void set_all_z_lock(const bool lock, const int8_t except=-1) {
        set_z1_lock(lock ^ (except == 0));
        set_z2_lock(lock ^ (except == 1));
        #if NUM_Z_STEPPERS >= 3
          set_z3_lock(lock ^ (except == 2));
          #if NUM_Z_STEPPERS >= 4
            set_z4_lock(lock ^ (except == 3));
          #endif
        #endif
      }
    #endif

    #if ENABLED(BABYSTEPPING)
      static void do_babystep(const AxisEnum axis, const bool direction); // perform a short step with a single stepper motor, outside of any convention
    #endif

    #if HAS_MOTOR_CURRENT_PWM
      static void refresh_motor_power();
    #endif

    static stepper_flags_t axis_enabled;  // Axis stepper(s) ENABLED states

    static bool axis_is_enabled(const AxisEnum axis E_OPTARG(const uint8_t eindex=0)) {
      return TEST(axis_enabled.bits, INDEX_OF_AXIS(axis, eindex));
    }
    static void mark_axis_enabled(const AxisEnum axis E_OPTARG(const uint8_t eindex=0)) {
      SBI(axis_enabled.bits, INDEX_OF_AXIS(axis, eindex));
    }
    static void mark_axis_disabled(const AxisEnum axis E_OPTARG(const uint8_t eindex=0)) {
      CBI(axis_enabled.bits, INDEX_OF_AXIS(axis, eindex));
    }
    static bool can_axis_disable(const AxisEnum axis E_OPTARG(const uint8_t eindex=0)) {
      return !any_enable_overlap() || !(axis_enabled.bits & enable_overlap[INDEX_OF_AXIS(axis, eindex)]);
    }

    static void enable_axis(const AxisEnum axis);
    static bool disable_axis(const AxisEnum axis);

    #if HAS_EXTRUDERS
      static void enable_extruder(E_TERN_(const uint8_t eindex=0));
      static bool disable_extruder(E_TERN_(const uint8_t eindex=0));
      static void enable_e_steppers();
      static void disable_e_steppers();
    #else
      static void enable_extruder() {}
      static bool disable_extruder() { return true; }
      static void enable_e_steppers() {}
      static void disable_e_steppers() {}
    #endif

    #define  ENABLE_EXTRUDER(N)  enable_extruder(E_TERN_(N))
    #define DISABLE_EXTRUDER(N) disable_extruder(E_TERN_(N))
    #define AXIS_IS_ENABLED(N,V...) axis_is_enabled(N E_OPTARG(#V))

    static void enable_all_steppers();
    static void disable_all_steppers();

    // Update direction states for all steppers
    static void apply_directions();

    // Set direction bits and update all stepper DIR states
    static void set_directions(const AxisBits bits) {
      last_direction_bits = bits;
      apply_directions();
    }

    #if ENABLED(FT_MOTION)
      // Manage the planner
      static void ftMotion_blockQueueUpdate();
      // Set current position in steps when reset flag is set in M493 and planner already synchronized
      static void ftMotion_syncPosition();
    #endif

    #if HAS_ZV_SHAPING
      static void set_shaping_damping_ratio(const AxisEnum axis, const_float_t zeta);
      static float get_shaping_damping_ratio(const AxisEnum axis);
      static void set_shaping_frequency(const AxisEnum axis, const_float_t freq);
      static float get_shaping_frequency(const AxisEnum axis);
    #endif

  private:

    // Set the current position in steps
    static void _set_position(const abce_long_t &spos);

    // Calculate the timing interval for the given step rate
    static hal_timer_t calc_timer_interval(uint32_t step_rate);

    // Calculate timing interval and steps-per-ISR for the given step rate
    static hal_timer_t calc_multistep_timer_interval(uint32_t step_rate);

    #if ENABLED(NONLINEAR_EXTRUSION)
      static void calc_nonlinear_e(uint32_t step_rate);
    #endif

    #if ENABLED(S_CURVE_ACCELERATION)
      static void _calc_bezier_curve_coeffs(const int32_t v0, const int32_t v1, const uint32_t av);
      static int32_t _eval_bezier_curve(const uint32_t curr_step);
    #endif

    #if HAS_MOTOR_CURRENT_SPI || HAS_MOTOR_CURRENT_PWM
      static void digipot_init();
    #endif

    #if HAS_MICROSTEPS
      static void microstep_init();
    #endif

    #if ENABLED(FT_MOTION)
      static void ftMotion_stepper();
    #endif

};

extern Stepper stepper;
