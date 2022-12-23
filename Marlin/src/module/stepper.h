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
#ifdef __AVR__
  #include "stepper/speed_lookuptable.h"
#endif

// Disable multiple steps per ISR
//#define DISABLE_MULTI_STEPPING

//
// Estimate the amount of time the Stepper ISR will take to execute
//

/**
 * The method of calculating these cycle-constants is unclear.
 * Most of them are no longer used directly for pulse timing, and exist
 * only to estimate a maximum step rate based on the user's configuration.
 * As 32-bit processors continue to diverge, maintaining cycle counts
 * will become increasingly difficult and error-prone.
 */

#ifdef CPU_32_BIT
  /**
   * Duration of START_TIMED_PULSE
   *
   * ...as measured on an LPC1768 with a scope and converted to cycles.
   * Not applicable to other 32-bit processors, but as long as others
   * take longer, pulses will be longer. For example the SKR Pro
   * (stm32f407zgt6) requires ~60 cyles.
   */
  #define TIMER_READ_ADD_AND_STORE_CYCLES 34UL

  // The base ISR
  #define ISR_BASE_CYCLES 770UL

  // Linear advance base time is 64 cycles
  #if ENABLED(LIN_ADVANCE)
    #define ISR_LA_BASE_CYCLES 64UL
  #else
    #define ISR_LA_BASE_CYCLES 0UL
  #endif

  // S curve interpolation adds 40 cycles
  #if ENABLED(S_CURVE_ACCELERATION)
    #ifdef STM32G0B1xx
      #define ISR_S_CURVE_CYCLES 500UL
    #else
      #define ISR_S_CURVE_CYCLES 40UL
    #endif
  #else
    #define ISR_S_CURVE_CYCLES 0UL
  #endif

  // Input shaping base time
  #if HAS_SHAPING
    #define ISR_SHAPING_BASE_CYCLES 180UL
  #else
    #define ISR_SHAPING_BASE_CYCLES 0UL
  #endif

  // Stepper Loop base cycles
  #define ISR_LOOP_BASE_CYCLES 4UL

  // And each stepper (start + stop pulse) takes in worst case
  #define ISR_STEPPER_CYCLES 100UL

#else
  // Cycles to perform actions in START_TIMED_PULSE
  #define TIMER_READ_ADD_AND_STORE_CYCLES 13UL

  // The base ISR
  #define ISR_BASE_CYCLES  1000UL

  // Linear advance base time is 32 cycles
  #if ENABLED(LIN_ADVANCE)
    #define ISR_LA_BASE_CYCLES 32UL
  #else
    #define ISR_LA_BASE_CYCLES 0UL
  #endif

  // S curve interpolation adds 160 cycles
  #if ENABLED(S_CURVE_ACCELERATION)
    #define ISR_S_CURVE_CYCLES 160UL
  #else
    #define ISR_S_CURVE_CYCLES 0UL
  #endif

  // Input shaping base time
  #if HAS_SHAPING
    #define ISR_SHAPING_BASE_CYCLES 290UL
  #else
    #define ISR_SHAPING_BASE_CYCLES 0UL
  #endif

  // Stepper Loop base cycles
  #define ISR_LOOP_BASE_CYCLES 32UL

  // And each stepper (start + stop pulse) takes in worst case
  #define ISR_STEPPER_CYCLES 88UL

#endif

// If linear advance is disabled, the loop also handles them
#if DISABLED(LIN_ADVANCE) && ENABLED(MIXING_EXTRUDER)
  #define ISR_MIXING_STEPPER_CYCLES ((MIXING_STEPPERS) * (ISR_STEPPER_CYCLES))
#else
  #define ISR_MIXING_STEPPER_CYCLES  0UL
#endif

// Add time for each stepper
#if HAS_X_STEP
  #define ISR_X_STEPPER_CYCLES  ISR_STEPPER_CYCLES
#endif
#if HAS_Y_STEP
  #define ISR_Y_STEPPER_CYCLES  ISR_STEPPER_CYCLES
#endif
#if HAS_Z_STEP
  #define ISR_Z_STEPPER_CYCLES  ISR_STEPPER_CYCLES
#endif
#if HAS_I_STEP
  #define ISR_I_STEPPER_CYCLES  ISR_STEPPER_CYCLES
#endif
#if HAS_J_STEP
  #define ISR_J_STEPPER_CYCLES  ISR_STEPPER_CYCLES
#endif
#if HAS_K_STEP
  #define ISR_K_STEPPER_CYCLES  ISR_STEPPER_CYCLES
#endif
#if HAS_U_STEP
  #define ISR_U_STEPPER_CYCLES  ISR_STEPPER_CYCLES
#endif
#if HAS_V_STEP
  #define ISR_V_STEPPER_CYCLES  ISR_STEPPER_CYCLES
#endif
#if HAS_W_STEP
  #define ISR_W_STEPPER_CYCLES  ISR_STEPPER_CYCLES
#endif
#if HAS_EXTRUDERS
  #define ISR_E_STEPPER_CYCLES  ISR_STEPPER_CYCLES    // E is always interpolated, even for mixing extruders
#endif

// And the total minimum loop time, not including the base
#define MIN_ISR_LOOP_CYCLES (ISR_MIXING_STEPPER_CYCLES LOGICAL_AXIS_GANG(+ ISR_E_STEPPER_CYCLES, + ISR_X_STEPPER_CYCLES, + ISR_Y_STEPPER_CYCLES, + ISR_Z_STEPPER_CYCLES, + ISR_I_STEPPER_CYCLES, + ISR_J_STEPPER_CYCLES, + ISR_K_STEPPER_CYCLES, + ISR_U_STEPPER_CYCLES, + ISR_V_STEPPER_CYCLES, + ISR_W_STEPPER_CYCLES))

// Calculate the minimum MPU cycles needed per pulse to enforce, limited to the max stepper rate
#define _MIN_STEPPER_PULSE_CYCLES(N) _MAX(uint32_t((F_CPU) / (MAXIMUM_STEPPER_RATE)), ((F_CPU) / 500000UL) * (N))
#if MINIMUM_STEPPER_PULSE
  #define MIN_STEPPER_PULSE_CYCLES _MIN_STEPPER_PULSE_CYCLES(uint32_t(MINIMUM_STEPPER_PULSE))
#elif HAS_DRIVER(LV8729)
  #define MIN_STEPPER_PULSE_CYCLES uint32_t((((F_CPU) - 1) / 2000000) + 1) // 0.5µs, aka 500ns
#else
  #define MIN_STEPPER_PULSE_CYCLES _MIN_STEPPER_PULSE_CYCLES(1UL)
#endif

// Calculate the minimum pulse times (high and low)
#if MINIMUM_STEPPER_PULSE && MAXIMUM_STEPPER_RATE
  constexpr uint32_t _MIN_STEP_PERIOD_NS = 1000000000UL / MAXIMUM_STEPPER_RATE;
  constexpr uint32_t _MIN_PULSE_HIGH_NS = 1000UL * MINIMUM_STEPPER_PULSE;
  constexpr uint32_t _MIN_PULSE_LOW_NS = _MAX((_MIN_STEP_PERIOD_NS - _MIN(_MIN_STEP_PERIOD_NS, _MIN_PULSE_HIGH_NS)), _MIN_PULSE_HIGH_NS);
#elif MINIMUM_STEPPER_PULSE
  // Assume 50% duty cycle
  constexpr uint32_t _MIN_PULSE_HIGH_NS = 1000UL * MINIMUM_STEPPER_PULSE;
  constexpr uint32_t _MIN_PULSE_LOW_NS = _MIN_PULSE_HIGH_NS;
#elif MAXIMUM_STEPPER_RATE
  // Assume 50% duty cycle
  constexpr uint32_t _MIN_PULSE_HIGH_NS = 500000000UL / MAXIMUM_STEPPER_RATE;
  constexpr uint32_t _MIN_PULSE_LOW_NS = _MIN_PULSE_HIGH_NS;
#else
  #error "Expected at least one of MINIMUM_STEPPER_PULSE or MAXIMUM_STEPPER_RATE to be defined"
#endif

// The loop takes the base time plus the time for all the bresenham logic for R pulses plus the time
// between pulses for (R-1) pulses. But the user could be enforcing a minimum time so the loop time is:
#define ISR_LOOP_CYCLES(R) ((ISR_LOOP_BASE_CYCLES + MIN_ISR_LOOP_CYCLES + MIN_STEPPER_PULSE_CYCLES) * (R - 1) + _MAX(MIN_ISR_LOOP_CYCLES, MIN_STEPPER_PULSE_CYCLES))

// Model input shaping as an extra loop call
#define ISR_SHAPING_LOOP_CYCLES(R) ((TERN0(HAS_SHAPING, ISR_LOOP_BASE_CYCLES) + TERN0(INPUT_SHAPING_X, ISR_X_STEPPER_CYCLES) + TERN0(INPUT_SHAPING_Y, ISR_Y_STEPPER_CYCLES)) * (R) + (MIN_ISR_LOOP_CYCLES) * (R - 1))

// If linear advance is enabled, then it is handled separately
#if ENABLED(LIN_ADVANCE)

  // Estimate the minimum LA loop time
  #if ENABLED(MIXING_EXTRUDER) // ToDo: ???
    // HELP ME: What is what?
    // Directions are set up for MIXING_STEPPERS - like before.
    // Finding the right stepper may last up to MIXING_STEPPERS loops in get_next_stepper().
    //   These loops are a bit faster than advancing a bresenham counter.
    // Always only one E stepper is stepped.
    #define MIN_ISR_LA_LOOP_CYCLES ((MIXING_STEPPERS) * (ISR_STEPPER_CYCLES))
  #else
    #define MIN_ISR_LA_LOOP_CYCLES ISR_STEPPER_CYCLES
  #endif

  // And the real loop time
  #define ISR_LA_LOOP_CYCLES _MAX(MIN_STEPPER_PULSE_CYCLES, MIN_ISR_LA_LOOP_CYCLES)

#else
  #define ISR_LA_LOOP_CYCLES 0UL
#endif

// Now estimate the total ISR execution time in cycles given a step per ISR multiplier
#define ISR_EXECUTION_CYCLES(R) (((ISR_BASE_CYCLES + ISR_S_CURVE_CYCLES + ISR_SHAPING_BASE_CYCLES + ISR_LOOP_CYCLES(R) + ISR_SHAPING_LOOP_CYCLES(R) + ISR_LA_BASE_CYCLES + ISR_LA_LOOP_CYCLES)) / (R))

// The maximum allowable stepping frequency when doing x128-x1 stepping (in Hz)
#define MAX_STEP_ISR_FREQUENCY_128X ((F_CPU) / ISR_EXECUTION_CYCLES(128))
#define MAX_STEP_ISR_FREQUENCY_64X  ((F_CPU) / ISR_EXECUTION_CYCLES(64))
#define MAX_STEP_ISR_FREQUENCY_32X  ((F_CPU) / ISR_EXECUTION_CYCLES(32))
#define MAX_STEP_ISR_FREQUENCY_16X  ((F_CPU) / ISR_EXECUTION_CYCLES(16))
#define MAX_STEP_ISR_FREQUENCY_8X   ((F_CPU) / ISR_EXECUTION_CYCLES(8))
#define MAX_STEP_ISR_FREQUENCY_4X   ((F_CPU) / ISR_EXECUTION_CYCLES(4))
#define MAX_STEP_ISR_FREQUENCY_2X   ((F_CPU) / ISR_EXECUTION_CYCLES(2))
#define MAX_STEP_ISR_FREQUENCY_1X   ((F_CPU) / ISR_EXECUTION_CYCLES(1))

// The minimum step ISR rate used by ADAPTIVE_STEP_SMOOTHING to target 50% CPU usage
// This does not account for the possibility of multi-stepping.
// Perhaps DISABLE_MULTI_STEPPING should be required with ADAPTIVE_STEP_SMOOTHING.
#define MIN_STEP_ISR_FREQUENCY (MAX_STEP_ISR_FREQUENCY_1X / 2)

#define ENABLE_COUNT (NUM_AXES + E_STEPPERS)
#if ENABLE_COUNT > 16
  typedef uint32_t ena_mask_t;
#else
  typedef IF<(ENABLE_COUNT > 8), uint16_t, uint8_t>::type ena_mask_t;
#endif

// Axis flags type, for enabled state or other simple state
typedef struct {
  union {
    ena_mask_t bits;
    struct {
      bool NUM_AXIS_LIST(X:1, Y:1, Z:1, I:1, J:1, K:1, U:1, V:1, W:1);
      #if HAS_EXTRUDERS
        bool LIST_N(EXTRUDERS, E0:1, E1:1, E2:1, E3:1, E4:1, E5:1, E6:1, E7:1);
      #endif
    };
  };
} stepper_flags_t;

// All the stepper enable pins
constexpr pin_t ena_pins[] = {
  NUM_AXIS_LIST(X_ENABLE_PIN, Y_ENABLE_PIN, Z_ENABLE_PIN, I_ENABLE_PIN, J_ENABLE_PIN, K_ENABLE_PIN, U_ENABLE_PIN, V_ENABLE_PIN, W_ENABLE_PIN),
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

#if HAS_SHAPING

  #ifdef SHAPING_MAX_STEPRATE
    constexpr float max_step_rate = SHAPING_MAX_STEPRATE;
  #else
    constexpr float     _ISDASU[] = DEFAULT_AXIS_STEPS_PER_UNIT;
    constexpr feedRate_t _ISDMF[] = DEFAULT_MAX_FEEDRATE;
    constexpr float max_shaped_rate = TERN0(INPUT_SHAPING_X, _ISDMF[X_AXIS] * _ISDASU[X_AXIS]) +
                                      TERN0(INPUT_SHAPING_Y, _ISDMF[Y_AXIS] * _ISDASU[Y_AXIS]);
    #if defined(__AVR__) || !defined(ADAPTIVE_STEP_SMOOTHING)
      // MIN_STEP_ISR_FREQUENCY is known at compile time on AVRs and any reduction in SRAM is welcome
      template<int INDEX=DISTINCT_AXES> constexpr float max_isr_rate() {
        return _MAX(_ISDMF[INDEX - 1] * _ISDASU[INDEX - 1], max_isr_rate<INDEX - 1>());
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

  typedef IF<ENABLED(__AVR__), uint16_t, uint32_t>::type shaping_time_t;
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
        TERN_(INPUT_SHAPING_X, if (head_x == tail && x_step) peek_x_val = delay_x);
        TERN_(INPUT_SHAPING_Y, if (head_y == tail && y_step) peek_y_val = delay_y);
        times[tail] = now;
        TERN_(INPUT_SHAPING_X, echo_axes[tail].x = x_step ? (x_forward ? ECHO_FWD : ECHO_BWD) : ECHO_NONE);
        TERN_(INPUT_SHAPING_Y, echo_axes[tail].y = y_step ? (y_forward ? ECHO_FWD : ECHO_BWD) : ECHO_NONE);
        if (++tail == shaping_echoes) tail = 0;
        TERN_(INPUT_SHAPING_X, _free_count_x--);
        TERN_(INPUT_SHAPING_Y, _free_count_y--);
        TERN_(INPUT_SHAPING_X, if (echo_axes[head_x].x == ECHO_NONE) dequeue_x());
        TERN_(INPUT_SHAPING_Y, if (echo_axes[head_y].y == ECHO_NONE) dequeue_y());
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
    bool enabled;
    int16_t delta_error = 0;    // delta_error for seconday bresenham mod 128
    uint8_t factor1;
    uint8_t factor2;
    bool forward;
    int32_t last_block_end_pos = 0;
  };

#endif // HAS_SHAPING

//
// Stepper class definition
//
class Stepper {
  friend class KinematicSystem;
  friend class DeltaKinematicSystem;
  friend void stepperTask(void *);

  public:

    #if EITHER(HAS_EXTRA_ENDSTOPS, Z_STEPPER_AUTO_ALIGN)
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
      static bool frozen;                   // Set this flag to instantly freeze motion
    #endif

  private:

    static block_t* current_block;          // A pointer to the block currently being traced

    static axis_bits_t last_direction_bits, // The next stepping-bits to be output
                       axis_did_move;       // Last Movement in the given direction is not null, as computed when the last movement was fetched from planner

    static bool abort_current_block;        // Signals to the stepper that current block should be aborted

    #if ENABLED(X_DUAL_ENDSTOPS)
      static bool locked_X_motor, locked_X2_motor;
    #endif
    #if ENABLED(Y_DUAL_ENDSTOPS)
      static bool locked_Y_motor, locked_Y2_motor;
    #endif
    #if EITHER(Z_MULTI_ENDSTOPS, Z_STEPPER_AUTO_ALIGN)
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
    static uint8_t steps_per_isr;         // Count of steps to perform per Stepper ISR call

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

    #if EITHER(HAS_MULTI_EXTRUDER, MIXING_EXTRUDER)
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

    #if HAS_SHAPING
      #if ENABLED(INPUT_SHAPING_X)
        static ShapeParams shaping_x;
      #endif
      #if ENABLED(INPUT_SHAPING_Y)
        static ShapeParams shaping_y;
      #endif
    #endif

    #if ENABLED(LIN_ADVANCE)
      static constexpr uint32_t LA_ADV_NEVER = 0xFFFFFFFF;
      static uint32_t nextAdvanceISR,
                      la_interval;      // Interval between ISR calls for LA
      static int32_t  la_delta_error,   // Analogue of delta_error.e for E steps in LA ISR
                      la_dividend,      // Analogue of advance_dividend.e for E steps in LA ISR
                      la_advance_steps; // Count of steps added to increase nozzle pressure
    #endif

    #if ENABLED(INTEGRATED_BABYSTEPPING)
      static constexpr uint32_t BABYSTEP_NEVER = 0xFFFFFFFF;
      static uint32_t nextBabystepISR;
    #endif

    #if ENABLED(DIRECT_STEPPING)
      static page_step_state_t page_step_state;
    #endif

    static int32_t ticks_nominal;
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
    static uint32_t block_phase_isr();

    #if HAS_SHAPING
      static void shaping_isr();
    #endif

    #if ENABLED(LIN_ADVANCE)
      // The Linear advance ISR phase
      static void advance_isr();
    #endif

    #if ENABLED(INTEGRATED_BABYSTEPPING)
      // The Babystepping ISR phase
      static uint32_t babystepping_isr();
      FORCE_INLINE static void initiateBabystepping() {
        if (nextBabystepISR == BABYSTEP_NEVER) {
          nextBabystepISR = 0;
          wake_up();
        }
      }
    #endif

    // Check if the given block is busy or not - Must not be called from ISR contexts
    static bool is_block_busy(const block_t * const block);

    #if HAS_SHAPING
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
      axis_did_move = 0;
      planner.release_current_block();
      TERN_(LIN_ADVANCE, la_interval = nextAdvanceISR = LA_ADV_NEVER);
    }

    // Quickly stop all steppers
    FORCE_INLINE static void quick_stop() { abort_current_block = true; }

    // The direction of a single motor
    FORCE_INLINE static bool motor_direction(const AxisEnum axis) { return TEST(last_direction_bits, axis); }

    // The last movement direction was not null on the specified axis. Note that motor direction is not necessarily the same.
    FORCE_INLINE static bool axis_is_moving(const AxisEnum axis) { return TEST(axis_did_move, axis); }

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

    #if EITHER(HAS_EXTRA_ENDSTOPS, Z_STEPPER_AUTO_ALIGN)
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
    #if EITHER(Z_MULTI_ENDSTOPS, Z_STEPPER_AUTO_ALIGN)
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
    static void set_directions();

    // Set direction bits and update all stepper DIR states
    static void set_directions(const axis_bits_t bits) {
      last_direction_bits = bits;
      set_directions();
    }

    #if HAS_SHAPING
      static void set_shaping_damping_ratio(const AxisEnum axis, const float zeta);
      static float get_shaping_damping_ratio(const AxisEnum axis);
      static void set_shaping_frequency(const AxisEnum axis, const float freq);
      static float get_shaping_frequency(const AxisEnum axis);
    #endif

  private:

    // Set the current position in steps
    static void _set_position(const abce_long_t &spos);

    // Calculate timing interval for the given step rate
    static uint32_t calc_timer_interval(uint32_t step_rate);
    static uint32_t calc_timer_interval(uint32_t step_rate, uint8_t &loops);

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

};

extern Stepper stepper;
