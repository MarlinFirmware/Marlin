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

/**
 * stepper.cpp - A singleton object to execute motion plans using stepper motors
 * Marlin Firmware
 *
 * Derived from Grbl
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

/**
 * Timer calculations informed by the 'RepRap cartesian firmware' by Zack Smith
 * and Philipp Tiefenbacher.
 */

/**
 *         __________________________
 *        /|                        |\     _________________         ^
 *       / |                        | \   /|               |\        |
 *      /  |                        |  \ / |               | \       s
 *     /   |                        |   |  |               |  \      p
 *    /    |                        |   |  |               |   \     e
 *   +-----+------------------------+---+--+---------------+----+    e
 *   |               BLOCK 1            |      BLOCK 2          |    d
 *
 *                           time ----->
 *
 *  The trapezoid is the shape the speed curve over time. It starts at block->initial_rate, accelerates
 *  first block->accelerate_until step_events_completed, then keeps going at constant speed until
 *  step_events_completed reaches block->decelerate_after after which it decelerates until the trapezoid generator is reset.
 *  The slope of acceleration is calculated using v = u + at where t is the accumulated timer values of the steps so far.
 */

/**
 * Marlin uses the Bresenham algorithm. For a detailed explanation of theory and
 * method see https://www.cs.helsinki.fi/group/goa/mallinnus/lines/bresenh.html
 */

/**
 * Jerk controlled movements planner added Apr 2018 by Eduardo José Tagle.
 * Equations based on Synthethos TinyG2 sources, but the fixed-point
 * implementation is new, as we are running the ISR with a variable period.
 * Also implemented the Bézier velocity curve evaluation in ARM assembler,
 * to avoid impacting ISR speed.
 */

#include "stepper.h"

Stepper stepper; // Singleton

#define BABYSTEPPING_EXTRA_DIR_WAIT

#ifdef __AVR__
  #include "speed_lookuptable.h"
#endif

#include "endstops.h"
#include "planner.h"
#include "motion.h"

#include "../lcd/marlinui.h"
#include "../gcode/queue.h"
#include "../sd/cardreader.h"
#include "../MarlinCore.h"
#include "../HAL/shared/Delay.h"

#if ENABLED(INTEGRATED_BABYSTEPPING)
  #include "../feature/babystep.h"
#endif

#if MB(ALLIGATOR)
  #include "../feature/dac/dac_dac084s085.h"
#endif

#if HAS_MOTOR_CURRENT_SPI
  #include <SPI.h>
#endif

#if ENABLED(MIXING_EXTRUDER)
  #include "../feature/mixing.h"
#endif

#if HAS_FILAMENT_RUNOUT_DISTANCE
  #include "../feature/runout.h"
#endif

#if HAS_L64XX
  #include "../libs/L64XX/L64XX_Marlin.h"
  uint8_t L6470_buf[MAX_L64XX + 1];   // chip command sequence - element 0 not used
  bool L64XX_OK_to_power_up = false;  // flag to keep L64xx steppers powered down after a reset or power up
#endif

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../feature/powerloss.h"
#endif

#if HAS_CUTTER
  #include "../feature/spindle_laser.h"
#endif

// public:

#if EITHER(HAS_EXTRA_ENDSTOPS, Z_STEPPER_AUTO_ALIGN)
  bool Stepper::separate_multi_axis = false;
#endif

#if HAS_MOTOR_CURRENT_SPI || HAS_MOTOR_CURRENT_PWM
  bool Stepper::initialized; // = false
  uint32_t Stepper::motor_current_setting[MOTOR_CURRENT_COUNT]; // Initialized by settings.load()
  #if HAS_MOTOR_CURRENT_SPI
    constexpr uint32_t Stepper::digipot_count[];
  #endif
#endif

// private:

block_t* Stepper::current_block; // (= nullptr) A pointer to the block currently being traced

uint8_t Stepper::last_direction_bits, // = 0
        Stepper::axis_did_move; // = 0

bool Stepper::abort_current_block;

#if DISABLED(MIXING_EXTRUDER) && HAS_MULTI_EXTRUDER
  uint8_t Stepper::last_moved_extruder = 0xFF;
#endif

#if ENABLED(X_DUAL_ENDSTOPS)
  bool Stepper::locked_X_motor = false, Stepper::locked_X2_motor = false;
#endif
#if ENABLED(Y_DUAL_ENDSTOPS)
  bool Stepper::locked_Y_motor = false, Stepper::locked_Y2_motor = false;
#endif

#if EITHER(Z_MULTI_ENDSTOPS, Z_STEPPER_AUTO_ALIGN)
  bool Stepper::locked_Z_motor = false, Stepper::locked_Z2_motor = false
    #if NUM_Z_STEPPER_DRIVERS >= 3
      , Stepper::locked_Z3_motor = false
      #if NUM_Z_STEPPER_DRIVERS >= 4
        , Stepper::locked_Z4_motor = false
      #endif
    #endif
  ;
#endif

uint32_t Stepper::acceleration_time, Stepper::deceleration_time;
uint8_t Stepper::steps_per_isr;

#if HAS_FREEZE_PIN
  bool Stepper::frozen; // = false
#endif

IF_DISABLED(ADAPTIVE_STEP_SMOOTHING, constexpr) uint8_t Stepper::oversampling_factor;

xyze_long_t Stepper::delta_error{0};

xyze_ulong_t Stepper::advance_dividend{0};
uint32_t Stepper::advance_divisor = 0,
         Stepper::step_events_completed = 0, // The number of step events executed in the current block
         Stepper::accelerate_until,          // The count at which to stop accelerating
         Stepper::decelerate_after,          // The count at which to start decelerating
         Stepper::step_event_count;          // The total event count for the current block

#if EITHER(HAS_MULTI_EXTRUDER, MIXING_EXTRUDER)
  uint8_t Stepper::stepper_extruder;
#else
  constexpr uint8_t Stepper::stepper_extruder;
#endif

#if ENABLED(S_CURVE_ACCELERATION)
  int32_t __attribute__((used)) Stepper::bezier_A __asm__("bezier_A");    // A coefficient in Bézier speed curve with alias for assembler
  int32_t __attribute__((used)) Stepper::bezier_B __asm__("bezier_B");    // B coefficient in Bézier speed curve with alias for assembler
  int32_t __attribute__((used)) Stepper::bezier_C __asm__("bezier_C");    // C coefficient in Bézier speed curve with alias for assembler
  uint32_t __attribute__((used)) Stepper::bezier_F __asm__("bezier_F");   // F coefficient in Bézier speed curve with alias for assembler
  uint32_t __attribute__((used)) Stepper::bezier_AV __asm__("bezier_AV"); // AV coefficient in Bézier speed curve with alias for assembler
  #ifdef __AVR__
    bool __attribute__((used)) Stepper::A_negative __asm__("A_negative"); // If A coefficient was negative
  #endif
  bool Stepper::bezier_2nd_half;    // =false If Bézier curve has been initialized or not
#endif

#if ENABLED(LIN_ADVANCE)

  uint32_t Stepper::nextAdvanceISR = LA_ADV_NEVER,
           Stepper::LA_isr_rate = LA_ADV_NEVER;
  uint16_t Stepper::LA_current_adv_steps = 0,
           Stepper::LA_final_adv_steps,
           Stepper::LA_max_adv_steps;

  int8_t   Stepper::LA_steps = 0;

  bool Stepper::LA_use_advance_lead;

#endif // LIN_ADVANCE

#if ENABLED(INTEGRATED_BABYSTEPPING)
  uint32_t Stepper::nextBabystepISR = BABYSTEP_NEVER;
#endif

#if ENABLED(DIRECT_STEPPING)
  page_step_state_t Stepper::page_step_state;
#endif

int32_t Stepper::ticks_nominal = -1;
#if DISABLED(S_CURVE_ACCELERATION)
  uint32_t Stepper::acc_step_rate; // needed for deceleration start point
#endif

xyz_long_t Stepper::endstops_trigsteps;
xyze_long_t Stepper::count_position{0};
xyze_int8_t Stepper::count_direction{0};

#if ENABLED(LASER_POWER_INLINE_TRAPEZOID)
  Stepper::stepper_laser_t Stepper::laser_trap = {
    .enabled = false,
    .cur_power = 0,
    .cruise_set = false,
    #if DISABLED(LASER_POWER_INLINE_TRAPEZOID_CONT)
      .last_step_count = 0,
      .acc_step_count = 0
    #else
      .till_update = 0
    #endif
  };
#endif

#define DUAL_ENDSTOP_APPLY_STEP(A,V)                                                                                        \
  if (separate_multi_axis) {                                                                                                \
    if (A##_HOME_DIR < 0) {                                                                                                 \
      if (!(TEST(endstops.state(), A##_MIN) && count_direction[_AXIS(A)] < 0) && !locked_##A##_motor) A##_STEP_WRITE(V);    \
      if (!(TEST(endstops.state(), A##2_MIN) && count_direction[_AXIS(A)] < 0) && !locked_##A##2_motor) A##2_STEP_WRITE(V); \
    }                                                                                                                       \
    else {                                                                                                                  \
      if (!(TEST(endstops.state(), A##_MAX) && count_direction[_AXIS(A)] > 0) && !locked_##A##_motor) A##_STEP_WRITE(V);    \
      if (!(TEST(endstops.state(), A##2_MAX) && count_direction[_AXIS(A)] > 0) && !locked_##A##2_motor) A##2_STEP_WRITE(V); \
    }                                                                                                                       \
  }                                                                                                                         \
  else {                                                                                                                    \
    A##_STEP_WRITE(V);                                                                                                      \
    A##2_STEP_WRITE(V);                                                                                                     \
  }

#define DUAL_SEPARATE_APPLY_STEP(A,V)             \
  if (separate_multi_axis) {                      \
    if (!locked_##A##_motor) A##_STEP_WRITE(V);   \
    if (!locked_##A##2_motor) A##2_STEP_WRITE(V); \
  }                                               \
  else {                                          \
    A##_STEP_WRITE(V);                            \
    A##2_STEP_WRITE(V);                           \
  }

#define TRIPLE_ENDSTOP_APPLY_STEP(A,V)                                                                                      \
  if (separate_multi_axis) {                                                                                                \
    if (A##_HOME_DIR < 0) {                                                                                                 \
      if (!(TEST(endstops.state(), A##_MIN) && count_direction[_AXIS(A)] < 0) && !locked_##A##_motor) A##_STEP_WRITE(V);    \
      if (!(TEST(endstops.state(), A##2_MIN) && count_direction[_AXIS(A)] < 0) && !locked_##A##2_motor) A##2_STEP_WRITE(V); \
      if (!(TEST(endstops.state(), A##3_MIN) && count_direction[_AXIS(A)] < 0) && !locked_##A##3_motor) A##3_STEP_WRITE(V); \
    }                                                                                                                       \
    else {                                                                                                                  \
      if (!(TEST(endstops.state(), A##_MAX) && count_direction[_AXIS(A)] > 0) && !locked_##A##_motor) A##_STEP_WRITE(V);    \
      if (!(TEST(endstops.state(), A##2_MAX) && count_direction[_AXIS(A)] > 0) && !locked_##A##2_motor) A##2_STEP_WRITE(V); \
      if (!(TEST(endstops.state(), A##3_MAX) && count_direction[_AXIS(A)] > 0) && !locked_##A##3_motor) A##3_STEP_WRITE(V); \
    }                                                                                                                       \
  }                                                                                                                         \
  else {                                                                                                                    \
    A##_STEP_WRITE(V);                                                                                                      \
    A##2_STEP_WRITE(V);                                                                                                     \
    A##3_STEP_WRITE(V);                                                                                                     \
  }

#define TRIPLE_SEPARATE_APPLY_STEP(A,V)           \
  if (separate_multi_axis) {                      \
    if (!locked_##A##_motor) A##_STEP_WRITE(V);   \
    if (!locked_##A##2_motor) A##2_STEP_WRITE(V); \
    if (!locked_##A##3_motor) A##3_STEP_WRITE(V); \
  }                                               \
  else {                                          \
    A##_STEP_WRITE(V);                            \
    A##2_STEP_WRITE(V);                           \
    A##3_STEP_WRITE(V);                           \
  }

#define QUAD_ENDSTOP_APPLY_STEP(A,V)                                                                                        \
  if (separate_multi_axis) {                                                                                                \
    if (A##_HOME_DIR < 0) {                                                                                                 \
      if (!(TEST(endstops.state(), A##_MIN) && count_direction[_AXIS(A)] < 0) && !locked_##A##_motor) A##_STEP_WRITE(V);    \
      if (!(TEST(endstops.state(), A##2_MIN) && count_direction[_AXIS(A)] < 0) && !locked_##A##2_motor) A##2_STEP_WRITE(V); \
      if (!(TEST(endstops.state(), A##3_MIN) && count_direction[_AXIS(A)] < 0) && !locked_##A##3_motor) A##3_STEP_WRITE(V); \
      if (!(TEST(endstops.state(), A##4_MIN) && count_direction[_AXIS(A)] < 0) && !locked_##A##4_motor) A##4_STEP_WRITE(V); \
    }                                                                                                                       \
    else {                                                                                                                  \
      if (!(TEST(endstops.state(), A##_MAX) && count_direction[_AXIS(A)] > 0) && !locked_##A##_motor) A##_STEP_WRITE(V);    \
      if (!(TEST(endstops.state(), A##2_MAX) && count_direction[_AXIS(A)] > 0) && !locked_##A##2_motor) A##2_STEP_WRITE(V); \
      if (!(TEST(endstops.state(), A##3_MAX) && count_direction[_AXIS(A)] > 0) && !locked_##A##3_motor) A##3_STEP_WRITE(V); \
      if (!(TEST(endstops.state(), A##4_MAX) && count_direction[_AXIS(A)] > 0) && !locked_##A##4_motor) A##4_STEP_WRITE(V); \
    }                                                                                                                       \
  }                                                                                                                         \
  else {                                                                                                                    \
    A##_STEP_WRITE(V);                                                                                                      \
    A##2_STEP_WRITE(V);                                                                                                     \
    A##3_STEP_WRITE(V);                                                                                                     \
    A##4_STEP_WRITE(V);                                                                                                     \
  }

#define QUAD_SEPARATE_APPLY_STEP(A,V)             \
  if (separate_multi_axis) {                      \
    if (!locked_##A##_motor) A##_STEP_WRITE(V);   \
    if (!locked_##A##2_motor) A##2_STEP_WRITE(V); \
    if (!locked_##A##3_motor) A##3_STEP_WRITE(V); \
    if (!locked_##A##4_motor) A##4_STEP_WRITE(V); \
  }                                               \
  else {                                          \
    A##_STEP_WRITE(V);                            \
    A##2_STEP_WRITE(V);                           \
    A##3_STEP_WRITE(V);                           \
    A##4_STEP_WRITE(V);                           \
  }

#if ENABLED(X_DUAL_STEPPER_DRIVERS)
  #define X_APPLY_DIR(v,Q) do{ X_DIR_WRITE(v); X2_DIR_WRITE((v) ^ ENABLED(INVERT_X2_VS_X_DIR)); }while(0)
  #if ENABLED(X_DUAL_ENDSTOPS)
    #define X_APPLY_STEP(v,Q) DUAL_ENDSTOP_APPLY_STEP(X,v)
  #else
    #define X_APPLY_STEP(v,Q) do{ X_STEP_WRITE(v); X2_STEP_WRITE(v); }while(0)
  #endif
#elif ENABLED(DUAL_X_CARRIAGE)
  #define X_APPLY_DIR(v,ALWAYS) do{ \
    if (extruder_duplication_enabled || ALWAYS) { X_DIR_WRITE(v); X2_DIR_WRITE((v) ^ idex_mirrored_mode); } \
    else if (last_moved_extruder) X2_DIR_WRITE(v); else X_DIR_WRITE(v); \
  }while(0)
  #define X_APPLY_STEP(v,ALWAYS) do{ \
    if (extruder_duplication_enabled || ALWAYS) { X_STEP_WRITE(v); X2_STEP_WRITE(v); } \
    else if (last_moved_extruder) X2_STEP_WRITE(v); else X_STEP_WRITE(v); \
  }while(0)
#else
  #define X_APPLY_DIR(v,Q) X_DIR_WRITE(v)
  #define X_APPLY_STEP(v,Q) X_STEP_WRITE(v)
#endif

#if ENABLED(Y_DUAL_STEPPER_DRIVERS)
  #define Y_APPLY_DIR(v,Q) do{ Y_DIR_WRITE(v); Y2_DIR_WRITE((v) ^ ENABLED(INVERT_Y2_VS_Y_DIR)); }while(0)
  #if ENABLED(Y_DUAL_ENDSTOPS)
    #define Y_APPLY_STEP(v,Q) DUAL_ENDSTOP_APPLY_STEP(Y,v)
  #else
    #define Y_APPLY_STEP(v,Q) do{ Y_STEP_WRITE(v); Y2_STEP_WRITE(v); }while(0)
  #endif
#else
  #define Y_APPLY_DIR(v,Q) Y_DIR_WRITE(v)
  #define Y_APPLY_STEP(v,Q) Y_STEP_WRITE(v)
#endif

#if NUM_Z_STEPPER_DRIVERS == 4
  #define Z_APPLY_DIR(v,Q) do{ \
    Z_DIR_WRITE(v); Z2_DIR_WRITE((v) ^ ENABLED(INVERT_Z2_VS_Z_DIR)); \
    Z3_DIR_WRITE((v) ^ ENABLED(INVERT_Z3_VS_Z_DIR)); Z4_DIR_WRITE((v) ^ ENABLED(INVERT_Z4_VS_Z_DIR)); \
  }while(0)
  #if ENABLED(Z_MULTI_ENDSTOPS)
    #define Z_APPLY_STEP(v,Q) QUAD_ENDSTOP_APPLY_STEP(Z,v)
  #elif ENABLED(Z_STEPPER_AUTO_ALIGN)
    #define Z_APPLY_STEP(v,Q) QUAD_SEPARATE_APPLY_STEP(Z,v)
  #else
    #define Z_APPLY_STEP(v,Q) do{ Z_STEP_WRITE(v); Z2_STEP_WRITE(v); Z3_STEP_WRITE(v); Z4_STEP_WRITE(v); }while(0)
  #endif
#elif NUM_Z_STEPPER_DRIVERS == 3
  #define Z_APPLY_DIR(v,Q) do{ \
    Z_DIR_WRITE(v); Z2_DIR_WRITE((v) ^ ENABLED(INVERT_Z2_VS_Z_DIR)); Z3_DIR_WRITE((v) ^ ENABLED(INVERT_Z3_VS_Z_DIR)); \
  }while(0)
  #if ENABLED(Z_MULTI_ENDSTOPS)
    #define Z_APPLY_STEP(v,Q) TRIPLE_ENDSTOP_APPLY_STEP(Z,v)
  #elif ENABLED(Z_STEPPER_AUTO_ALIGN)
    #define Z_APPLY_STEP(v,Q) TRIPLE_SEPARATE_APPLY_STEP(Z,v)
  #else
    #define Z_APPLY_STEP(v,Q) do{ Z_STEP_WRITE(v); Z2_STEP_WRITE(v); Z3_STEP_WRITE(v); }while(0)
  #endif
#elif NUM_Z_STEPPER_DRIVERS == 2
  #define Z_APPLY_DIR(v,Q) do{ Z_DIR_WRITE(v); Z2_DIR_WRITE((v) ^ ENABLED(INVERT_Z2_VS_Z_DIR)); }while(0)
  #if ENABLED(Z_MULTI_ENDSTOPS)
    #define Z_APPLY_STEP(v,Q) DUAL_ENDSTOP_APPLY_STEP(Z,v)
  #elif ENABLED(Z_STEPPER_AUTO_ALIGN)
    #define Z_APPLY_STEP(v,Q) DUAL_SEPARATE_APPLY_STEP(Z,v)
  #else
    #define Z_APPLY_STEP(v,Q) do{ Z_STEP_WRITE(v); Z2_STEP_WRITE(v); }while(0)
  #endif
#else
  #define Z_APPLY_DIR(v,Q) Z_DIR_WRITE(v)
  #define Z_APPLY_STEP(v,Q) Z_STEP_WRITE(v)
#endif

#if DISABLED(MIXING_EXTRUDER)
  #define E_APPLY_STEP(v,Q) E_STEP_WRITE(stepper_extruder, v)
#endif

#define CYCLES_TO_NS(CYC) (1000UL * (CYC) / ((F_CPU) / 1000000))
#define NS_PER_PULSE_TIMER_TICK (1000000000UL / (STEPPER_TIMER_RATE))

// Round up when converting from ns to timer ticks
#define NS_TO_PULSE_TIMER_TICKS(NS) (((NS) + (NS_PER_PULSE_TIMER_TICK) / 2) / (NS_PER_PULSE_TIMER_TICK))

#define TIMER_SETUP_NS (CYCLES_TO_NS(TIMER_READ_ADD_AND_STORE_CYCLES))

#define PULSE_HIGH_TICK_COUNT hal_timer_t(NS_TO_PULSE_TIMER_TICKS(_MIN_PULSE_HIGH_NS - _MIN(_MIN_PULSE_HIGH_NS, TIMER_SETUP_NS)))
#define PULSE_LOW_TICK_COUNT hal_timer_t(NS_TO_PULSE_TIMER_TICKS(_MIN_PULSE_LOW_NS - _MIN(_MIN_PULSE_LOW_NS, TIMER_SETUP_NS)))

#define USING_TIMED_PULSE() hal_timer_t start_pulse_count = 0
#define START_TIMED_PULSE(DIR) (start_pulse_count = HAL_timer_get_count(PULSE_TIMER_NUM))
#define AWAIT_TIMED_PULSE(DIR) while (PULSE_##DIR##_TICK_COUNT > HAL_timer_get_count(PULSE_TIMER_NUM) - start_pulse_count) { }
#define START_HIGH_PULSE()  START_TIMED_PULSE(HIGH)
#define AWAIT_HIGH_PULSE()  AWAIT_TIMED_PULSE(HIGH)
#define START_LOW_PULSE()   START_TIMED_PULSE(LOW)
#define AWAIT_LOW_PULSE()   AWAIT_TIMED_PULSE(LOW)

#if MINIMUM_STEPPER_PRE_DIR_DELAY > 0
  #define DIR_WAIT_BEFORE() DELAY_NS(MINIMUM_STEPPER_PRE_DIR_DELAY)
#else
  #define DIR_WAIT_BEFORE()
#endif

#if MINIMUM_STEPPER_POST_DIR_DELAY > 0
  #define DIR_WAIT_AFTER() DELAY_NS(MINIMUM_STEPPER_POST_DIR_DELAY)
#else
  #define DIR_WAIT_AFTER()
#endif

/**
 * Set the stepper direction of each axis
 *
 *   COREXY: X_AXIS=A_AXIS and Y_AXIS=B_AXIS
 *   COREXZ: X_AXIS=A_AXIS and Z_AXIS=C_AXIS
 *   COREYZ: Y_AXIS=B_AXIS and Z_AXIS=C_AXIS
 */
void Stepper::set_directions() {

  DIR_WAIT_BEFORE();

  #define SET_STEP_DIR(A)                       \
    if (motor_direction(_AXIS(A))) {            \
      A##_APPLY_DIR(INVERT_##A##_DIR, false);   \
      count_direction[_AXIS(A)] = -1;           \
    }                                           \
    else {                                      \
      A##_APPLY_DIR(!INVERT_##A##_DIR, false);  \
      count_direction[_AXIS(A)] = 1;            \
    }

  #if HAS_X_DIR
    SET_STEP_DIR(X); // A
  #endif
  #if HAS_Y_DIR
    SET_STEP_DIR(Y); // B
  #endif
  #if HAS_Z_DIR
    SET_STEP_DIR(Z); // C
  #endif

  #if DISABLED(LIN_ADVANCE)
    #if ENABLED(MIXING_EXTRUDER)
       // Because this is valid for the whole block we don't know
       // what e-steppers will step. Likely all. Set all.
      if (motor_direction(E_AXIS)) {
        MIXER_STEPPER_LOOP(j) REV_E_DIR(j);
        count_direction.e = -1;
      }
      else {
        MIXER_STEPPER_LOOP(j) NORM_E_DIR(j);
        count_direction.e = 1;
      }
    #else
      if (motor_direction(E_AXIS)) {
        REV_E_DIR(stepper_extruder);
        count_direction.e = -1;
      }
      else {
        NORM_E_DIR(stepper_extruder);
        count_direction.e = 1;
      }
    #endif
  #endif // !LIN_ADVANCE

  #if HAS_L64XX
    if (L64XX_OK_to_power_up) { // OK to send the direction commands (which powers up the L64XX steppers)
      if (L64xxManager.spi_active) {
        L64xxManager.spi_abort = true;                    // Interrupted SPI transfer needs to shut down gracefully
        for (uint8_t j = 1; j <= L64XX::chain[0]; j++)
          L6470_buf[j] = dSPIN_NOP;                         // Fill buffer with NOOPs
        L64xxManager.transfer(L6470_buf, L64XX::chain[0]);  // Send enough NOOPs to complete any command
        L64xxManager.transfer(L6470_buf, L64XX::chain[0]);
        L64xxManager.transfer(L6470_buf, L64XX::chain[0]);
      }

      // L64xxManager.dir_commands[] is an array that holds direction command for each stepper

      // Scan command array, copy matches into L64xxManager.transfer
      for (uint8_t j = 1; j <= L64XX::chain[0]; j++)
        L6470_buf[j] = L64xxManager.dir_commands[L64XX::chain[j]];

      L64xxManager.transfer(L6470_buf, L64XX::chain[0]);  // send the command stream to the drivers
    }
  #endif

  DIR_WAIT_AFTER();
}

#if ENABLED(S_CURVE_ACCELERATION)
  /**
   *  This uses a quintic (fifth-degree) Bézier polynomial for the velocity curve, giving
   *  a "linear pop" velocity curve; with pop being the sixth derivative of position:
   *  velocity - 1st, acceleration - 2nd, jerk - 3rd, snap - 4th, crackle - 5th, pop - 6th
   *
   *  The Bézier curve takes the form:
   *
   *  V(t) = P_0 * B_0(t) + P_1 * B_1(t) + P_2 * B_2(t) + P_3 * B_3(t) + P_4 * B_4(t) + P_5 * B_5(t)
   *
   *  Where 0 <= t <= 1, and V(t) is the velocity. P_0 through P_5 are the control points, and B_0(t)
   *  through B_5(t) are the Bernstein basis as follows:
   *
   *        B_0(t) =   (1-t)^5        =   -t^5 +  5t^4 - 10t^3 + 10t^2 -  5t   +   1
   *        B_1(t) =  5(1-t)^4 * t    =   5t^5 - 20t^4 + 30t^3 - 20t^2 +  5t
   *        B_2(t) = 10(1-t)^3 * t^2  = -10t^5 + 30t^4 - 30t^3 + 10t^2
   *        B_3(t) = 10(1-t)^2 * t^3  =  10t^5 - 20t^4 + 10t^3
   *        B_4(t) =  5(1-t)   * t^4  =  -5t^5 +  5t^4
   *        B_5(t) =             t^5  =    t^5
   *                                      ^       ^       ^       ^       ^       ^
   *                                      |       |       |       |       |       |
   *                                      A       B       C       D       E       F
   *
   *  Unfortunately, we cannot use forward-differencing to calculate each position through
   *  the curve, as Marlin uses variable timer periods. So, we require a formula of the form:
   *
   *        V_f(t) = A*t^5 + B*t^4 + C*t^3 + D*t^2 + E*t + F
   *
   *  Looking at the above B_0(t) through B_5(t) expanded forms, if we take the coefficients of t^5
   *  through t of the Bézier form of V(t), we can determine that:
   *
   *        A =    -P_0 +  5*P_1 - 10*P_2 + 10*P_3 -  5*P_4 +  P_5
   *        B =   5*P_0 - 20*P_1 + 30*P_2 - 20*P_3 +  5*P_4
   *        C = -10*P_0 + 30*P_1 - 30*P_2 + 10*P_3
   *        D =  10*P_0 - 20*P_1 + 10*P_2
   *        E = - 5*P_0 +  5*P_1
   *        F =     P_0
   *
   *  Now, since we will (currently) *always* want the initial acceleration and jerk values to be 0,
   *  We set P_i = P_0 = P_1 = P_2 (initial velocity), and P_t = P_3 = P_4 = P_5 (target velocity),
   *  which, after simplification, resolves to:
   *
   *        A = - 6*P_i +  6*P_t =  6*(P_t - P_i)
   *        B =  15*P_i - 15*P_t = 15*(P_i - P_t)
   *        C = -10*P_i + 10*P_t = 10*(P_t - P_i)
   *        D = 0
   *        E = 0
   *        F = P_i
   *
   *  As the t is evaluated in non uniform steps here, there is no other way rather than evaluating
   *  the Bézier curve at each point:
   *
   *        V_f(t) = A*t^5 + B*t^4 + C*t^3 + F          [0 <= t <= 1]
   *
   * Floating point arithmetic execution time cost is prohibitive, so we will transform the math to
   * use fixed point values to be able to evaluate it in realtime. Assuming a maximum of 250000 steps
   * per second (driver pulses should at least be 2µS hi/2µS lo), and allocating 2 bits to avoid
   * overflows on the evaluation of the Bézier curve, means we can use
   *
   *   t: unsigned Q0.32 (0 <= t < 1) |range 0 to 0xFFFFFFFF unsigned
   *   A:   signed Q24.7 ,            |range = +/- 250000 * 6 * 128 = +/- 192000000 = 0x0B71B000 | 28 bits + sign
   *   B:   signed Q24.7 ,            |range = +/- 250000 *15 * 128 = +/- 480000000 = 0x1C9C3800 | 29 bits + sign
   *   C:   signed Q24.7 ,            |range = +/- 250000 *10 * 128 = +/- 320000000 = 0x1312D000 | 29 bits + sign
   *   F:   signed Q24.7 ,            |range = +/- 250000     * 128 =      32000000 = 0x01E84800 | 25 bits + sign
   *
   * The trapezoid generator state contains the following information, that we will use to create and evaluate
   * the Bézier curve:
   *
   *  blk->step_event_count [TS] = The total count of steps for this movement. (=distance)
   *  blk->initial_rate     [VI] = The initial steps per second (=velocity)
   *  blk->final_rate       [VF] = The ending steps per second  (=velocity)
   *  and the count of events completed (step_events_completed) [CS] (=distance until now)
   *
   *  Note the abbreviations we use in the following formulae are between []s
   *
   *  For Any 32bit CPU:
   *
   *    At the start of each trapezoid, calculate the coefficients A,B,C,F and Advance [AV], as follows:
   *
   *      A =  6*128*(VF - VI) =  768*(VF - VI)
   *      B = 15*128*(VI - VF) = 1920*(VI - VF)
   *      C = 10*128*(VF - VI) = 1280*(VF - VI)
   *      F =    128*VI        =  128*VI
   *     AV = (1<<32)/TS      ~= 0xFFFFFFFF / TS (To use ARM UDIV, that is 32 bits) (this is computed at the planner, to offload expensive calculations from the ISR)
   *
   *    And for each point, evaluate the curve with the following sequence:
   *
   *      void lsrs(uint32_t& d, uint32_t s, int cnt) {
   *        d = s >> cnt;
   *      }
   *      void lsls(uint32_t& d, uint32_t s, int cnt) {
   *        d = s << cnt;
   *      }
   *      void lsrs(int32_t& d, uint32_t s, int cnt) {
   *        d = uint32_t(s) >> cnt;
   *      }
   *      void lsls(int32_t& d, uint32_t s, int cnt) {
   *        d = uint32_t(s) << cnt;
   *      }
   *      void umull(uint32_t& rlo, uint32_t& rhi, uint32_t op1, uint32_t op2) {
   *        uint64_t res = uint64_t(op1) * op2;
   *        rlo = uint32_t(res & 0xFFFFFFFF);
   *        rhi = uint32_t((res >> 32) & 0xFFFFFFFF);
   *      }
   *      void smlal(int32_t& rlo, int32_t& rhi, int32_t op1, int32_t op2) {
   *        int64_t mul = int64_t(op1) * op2;
   *        int64_t s = int64_t(uint32_t(rlo) | ((uint64_t(uint32_t(rhi)) << 32U)));
   *        mul += s;
   *        rlo = int32_t(mul & 0xFFFFFFFF);
   *        rhi = int32_t((mul >> 32) & 0xFFFFFFFF);
   *      }
   *      int32_t _eval_bezier_curve_arm(uint32_t curr_step) {
   *        uint32_t flo = 0;
   *        uint32_t fhi = bezier_AV * curr_step;
   *        uint32_t t = fhi;
   *        int32_t alo = bezier_F;
   *        int32_t ahi = 0;
   *        int32_t A = bezier_A;
   *        int32_t B = bezier_B;
   *        int32_t C = bezier_C;
   *
   *        lsrs(ahi, alo, 1);          // a  = F << 31
   *        lsls(alo, alo, 31);         //
   *        umull(flo, fhi, fhi, t);    // f *= t
   *        umull(flo, fhi, fhi, t);    // f>>=32; f*=t
   *        lsrs(flo, fhi, 1);          //
   *        smlal(alo, ahi, flo, C);    // a+=(f>>33)*C
   *        umull(flo, fhi, fhi, t);    // f>>=32; f*=t
   *        lsrs(flo, fhi, 1);          //
   *        smlal(alo, ahi, flo, B);    // a+=(f>>33)*B
   *        umull(flo, fhi, fhi, t);    // f>>=32; f*=t
   *        lsrs(flo, fhi, 1);          // f>>=33;
   *        smlal(alo, ahi, flo, A);    // a+=(f>>33)*A;
   *        lsrs(alo, ahi, 6);          // a>>=38
   *
   *        return alo;
   *      }
   *
   *  This is rewritten in ARM assembly for optimal performance (43 cycles to execute).
   *
   *  For AVR, the precision of coefficients is scaled so the Bézier curve can be evaluated in real-time:
   *  Let's reduce precision as much as possible. After some experimentation we found that:
   *
   *    Assume t and AV with 24 bits is enough
   *       A =  6*(VF - VI)
   *       B = 15*(VI - VF)
   *       C = 10*(VF - VI)
   *       F =     VI
   *      AV = (1<<24)/TS   (this is computed at the planner, to offload expensive calculations from the ISR)
   *
   *    Instead of storing sign for each coefficient, we will store its absolute value,
   *    and flag the sign of the A coefficient, so we can save to store the sign bit.
   *    It always holds that sign(A) = - sign(B) = sign(C)
   *
   *     So, the resulting range of the coefficients are:
   *
   *       t: unsigned (0 <= t < 1) |range 0 to 0xFFFFFF unsigned
   *       A:   signed Q24 , range = 250000 * 6 = 1500000 = 0x16E360 | 21 bits
   *       B:   signed Q24 , range = 250000 *15 = 3750000 = 0x393870 | 22 bits
   *       C:   signed Q24 , range = 250000 *10 = 2500000 = 0x1312D0 | 21 bits
   *       F:   signed Q24 , range = 250000     =  250000 = 0x0ED090 | 20 bits
   *
   *    And for each curve, estimate its coefficients with:
   *
   *      void _calc_bezier_curve_coeffs(int32_t v0, int32_t v1, uint32_t av) {
   *       // Calculate the Bézier coefficients
   *       if (v1 < v0) {
   *         A_negative = true;
   *         bezier_A = 6 * (v0 - v1);
   *         bezier_B = 15 * (v0 - v1);
   *         bezier_C = 10 * (v0 - v1);
   *       }
   *       else {
   *         A_negative = false;
   *         bezier_A = 6 * (v1 - v0);
   *         bezier_B = 15 * (v1 - v0);
   *         bezier_C = 10 * (v1 - v0);
   *       }
   *       bezier_F = v0;
   *      }
   *
   *    And for each point, evaluate the curve with the following sequence:
   *
   *      // unsigned multiplication of 24 bits x 24bits, return upper 16 bits
   *      void umul24x24to16hi(uint16_t& r, uint24_t op1, uint24_t op2) {
   *        r = (uint64_t(op1) * op2) >> 8;
   *      }
   *      // unsigned multiplication of 16 bits x 16bits, return upper 16 bits
   *      void umul16x16to16hi(uint16_t& r, uint16_t op1, uint16_t op2) {
   *        r = (uint32_t(op1) * op2) >> 16;
   *      }
   *      // unsigned multiplication of 16 bits x 24bits, return upper 24 bits
   *      void umul16x24to24hi(uint24_t& r, uint16_t op1, uint24_t op2) {
   *        r = uint24_t((uint64_t(op1) * op2) >> 16);
   *      }
   *
   *      int32_t _eval_bezier_curve(uint32_t curr_step) {
   *        // To save computing, the first step is always the initial speed
   *        if (!curr_step)
   *          return bezier_F;
   *
   *        uint16_t t;
   *        umul24x24to16hi(t, bezier_AV, curr_step);   // t: Range 0 - 1^16 = 16 bits
   *        uint16_t f = t;
   *        umul16x16to16hi(f, f, t);                   // Range 16 bits (unsigned)
   *        umul16x16to16hi(f, f, t);                   // Range 16 bits : f = t^3  (unsigned)
   *        uint24_t acc = bezier_F;                    // Range 20 bits (unsigned)
   *        if (A_negative) {
   *          uint24_t v;
   *          umul16x24to24hi(v, f, bezier_C);          // Range 21bits
   *          acc -= v;
   *          umul16x16to16hi(f, f, t);                 // Range 16 bits : f = t^4  (unsigned)
   *          umul16x24to24hi(v, f, bezier_B);          // Range 22bits
   *          acc += v;
   *          umul16x16to16hi(f, f, t);                 // Range 16 bits : f = t^5  (unsigned)
   *          umul16x24to24hi(v, f, bezier_A);          // Range 21bits + 15 = 36bits (plus sign)
   *          acc -= v;
   *        }
   *        else {
   *          uint24_t v;
   *          umul16x24to24hi(v, f, bezier_C);          // Range 21bits
   *          acc += v;
   *          umul16x16to16hi(f, f, t);                 // Range 16 bits : f = t^4  (unsigned)
   *          umul16x24to24hi(v, f, bezier_B);          // Range 22bits
   *          acc -= v;
   *          umul16x16to16hi(f, f, t);                 // Range 16 bits : f = t^5  (unsigned)
   *          umul16x24to24hi(v, f, bezier_A);          // Range 21bits + 15 = 36bits (plus sign)
   *          acc += v;
   *        }
   *        return acc;
   *      }
   *    These functions are translated to assembler for optimal performance.
   *    Coefficient calculation takes 70 cycles. Bezier point evaluation takes 150 cycles.
   */

  #ifdef __AVR__

    // For AVR we use assembly to maximize speed
    void Stepper::_calc_bezier_curve_coeffs(const int32_t v0, const int32_t v1, const uint32_t av) {

      // Store advance
      bezier_AV = av;

      // Calculate the rest of the coefficients
      uint8_t r2 = v0 & 0xFF;
      uint8_t r3 = (v0 >> 8) & 0xFF;
      uint8_t r12 = (v0 >> 16) & 0xFF;
      uint8_t r5 = v1 & 0xFF;
      uint8_t r6 = (v1 >> 8) & 0xFF;
      uint8_t r7 = (v1 >> 16) & 0xFF;
      uint8_t r4,r8,r9,r10,r11;

      __asm__ __volatile__(
        /* Calculate the Bézier coefficients */
        /*  %10:%1:%0 = v0*/
        /*  %5:%4:%3 = v1*/
        /*  %7:%6:%10 = temporary*/
        /*  %9 = val (must be high register!)*/
        /*  %10 (must be high register!)*/

        /* Store initial velocity*/
        A("sts bezier_F, %0")
        A("sts bezier_F+1, %1")
        A("sts bezier_F+2, %10")    /* bezier_F = %10:%1:%0 = v0 */

        /* Get delta speed */
        A("ldi %2,-1")              /* %2 = 0xFF, means A_negative = true */
        A("clr %8")                 /* %8 = 0 */
        A("sub %0,%3")
        A("sbc %1,%4")
        A("sbc %10,%5")             /*  v0 -= v1, C=1 if result is negative */
        A("brcc 1f")                /* branch if result is positive (C=0), that means v0 >= v1 */

        /*  Result was negative, get the absolute value*/
        A("com %10")
        A("com %1")
        A("neg %0")
        A("sbc %1,%2")
        A("sbc %10,%2")             /* %10:%1:%0 +1  -> %10:%1:%0 = -(v0 - v1) = (v1 - v0) */
        A("clr %2")                 /* %2 = 0, means A_negative = false */

        /*  Store negative flag*/
        L("1")
        A("sts A_negative, %2")     /* Store negative flag */

        /*  Compute coefficients A,B and C   [20 cycles worst case]*/
        A("ldi %9,6")               /* %9 = 6 */
        A("mul %0,%9")              /* r1:r0 = 6*LO(v0-v1) */
        A("sts bezier_A, r0")
        A("mov %6,r1")
        A("clr %7")                 /* %7:%6:r0 = 6*LO(v0-v1) */
        A("mul %1,%9")              /* r1:r0 = 6*MI(v0-v1) */
        A("add %6,r0")
        A("adc %7,r1")              /* %7:%6:?? += 6*MI(v0-v1) << 8 */
        A("mul %10,%9")             /* r1:r0 = 6*HI(v0-v1) */
        A("add %7,r0")              /* %7:%6:?? += 6*HI(v0-v1) << 16 */
        A("sts bezier_A+1, %6")
        A("sts bezier_A+2, %7")     /* bezier_A = %7:%6:?? = 6*(v0-v1) [35 cycles worst] */

        A("ldi %9,15")              /* %9 = 15 */
        A("mul %0,%9")              /* r1:r0 = 5*LO(v0-v1) */
        A("sts bezier_B, r0")
        A("mov %6,r1")
        A("clr %7")                 /* %7:%6:?? = 5*LO(v0-v1) */
        A("mul %1,%9")              /* r1:r0 = 5*MI(v0-v1) */
        A("add %6,r0")
        A("adc %7,r1")              /* %7:%6:?? += 5*MI(v0-v1) << 8 */
        A("mul %10,%9")             /* r1:r0 = 5*HI(v0-v1) */
        A("add %7,r0")              /* %7:%6:?? += 5*HI(v0-v1) << 16 */
        A("sts bezier_B+1, %6")
        A("sts bezier_B+2, %7")     /* bezier_B = %7:%6:?? = 5*(v0-v1) [50 cycles worst] */

        A("ldi %9,10")              /* %9 = 10 */
        A("mul %0,%9")              /* r1:r0 = 10*LO(v0-v1) */
        A("sts bezier_C, r0")
        A("mov %6,r1")
        A("clr %7")                 /* %7:%6:?? = 10*LO(v0-v1) */
        A("mul %1,%9")              /* r1:r0 = 10*MI(v0-v1) */
        A("add %6,r0")
        A("adc %7,r1")              /* %7:%6:?? += 10*MI(v0-v1) << 8 */
        A("mul %10,%9")             /* r1:r0 = 10*HI(v0-v1) */
        A("add %7,r0")              /* %7:%6:?? += 10*HI(v0-v1) << 16 */
        A("sts bezier_C+1, %6")
        " sts bezier_C+2, %7"       /* bezier_C = %7:%6:?? = 10*(v0-v1) [65 cycles worst] */
        : "+r" (r2),
          "+d" (r3),
          "=r" (r4),
          "+r" (r5),
          "+r" (r6),
          "+r" (r7),
          "=r" (r8),
          "=r" (r9),
          "=r" (r10),
          "=d" (r11),
          "+r" (r12)
        :
        : "r0", "r1", "cc", "memory"
      );
    }

    FORCE_INLINE int32_t Stepper::_eval_bezier_curve(const uint32_t curr_step) {

      // If dealing with the first step, save expensive computing and return the initial speed
      if (!curr_step)
        return bezier_F;

      uint8_t r0 = 0; /* Zero register */
      uint8_t r2 = (curr_step) & 0xFF;
      uint8_t r3 = (curr_step >> 8) & 0xFF;
      uint8_t r4 = (curr_step >> 16) & 0xFF;
      uint8_t r1,r5,r6,r7,r8,r9,r10,r11; /* Temporary registers */

      __asm__ __volatile(
        /* umul24x24to16hi(t, bezier_AV, curr_step);  t: Range 0 - 1^16 = 16 bits*/
        A("lds %9,bezier_AV")       /* %9 = LO(AV)*/
        A("mul %9,%2")              /* r1:r0 = LO(bezier_AV)*LO(curr_step)*/
        A("mov %7,r1")              /* %7 = LO(bezier_AV)*LO(curr_step) >> 8*/
        A("clr %8")                 /* %8:%7  = LO(bezier_AV)*LO(curr_step) >> 8*/
        A("lds %10,bezier_AV+1")    /* %10 = MI(AV)*/
        A("mul %10,%2")             /* r1:r0  = MI(bezier_AV)*LO(curr_step)*/
        A("add %7,r0")
        A("adc %8,r1")              /* %8:%7 += MI(bezier_AV)*LO(curr_step)*/
        A("lds r1,bezier_AV+2")     /* r11 = HI(AV)*/
        A("mul r1,%2")              /* r1:r0  = HI(bezier_AV)*LO(curr_step)*/
        A("add %8,r0")              /* %8:%7 += HI(bezier_AV)*LO(curr_step) << 8*/
        A("mul %9,%3")              /* r1:r0 =  LO(bezier_AV)*MI(curr_step)*/
        A("add %7,r0")
        A("adc %8,r1")              /* %8:%7 += LO(bezier_AV)*MI(curr_step)*/
        A("mul %10,%3")             /* r1:r0 =  MI(bezier_AV)*MI(curr_step)*/
        A("add %8,r0")              /* %8:%7 += LO(bezier_AV)*MI(curr_step) << 8*/
        A("mul %9,%4")              /* r1:r0 =  LO(bezier_AV)*HI(curr_step)*/
        A("add %8,r0")              /* %8:%7 += LO(bezier_AV)*HI(curr_step) << 8*/
        /* %8:%7 = t*/

        /* uint16_t f = t;*/
        A("mov %5,%7")              /* %6:%5 = f*/
        A("mov %6,%8")
        /* %6:%5 = f*/

        /* umul16x16to16hi(f, f, t); / Range 16 bits (unsigned) [17] */
        A("mul %5,%7")              /* r1:r0 = LO(f) * LO(t)*/
        A("mov %9,r1")              /* store MIL(LO(f) * LO(t)) in %9, we need it for rounding*/
        A("clr %10")                /* %10 = 0*/
        A("clr %11")                /* %11 = 0*/
        A("mul %5,%8")              /* r1:r0 = LO(f) * HI(t)*/
        A("add %9,r0")              /* %9 += LO(LO(f) * HI(t))*/
        A("adc %10,r1")             /* %10 = HI(LO(f) * HI(t))*/
        A("adc %11,%0")             /* %11 += carry*/
        A("mul %6,%7")              /* r1:r0 = HI(f) * LO(t)*/
        A("add %9,r0")              /* %9 += LO(HI(f) * LO(t))*/
        A("adc %10,r1")             /* %10 += HI(HI(f) * LO(t)) */
        A("adc %11,%0")             /* %11 += carry*/
        A("mul %6,%8")              /* r1:r0 = HI(f) * HI(t)*/
        A("add %10,r0")             /* %10 += LO(HI(f) * HI(t))*/
        A("adc %11,r1")             /* %11 += HI(HI(f) * HI(t))*/
        A("mov %5,%10")             /* %6:%5 = */
        A("mov %6,%11")             /* f = %10:%11*/

        /* umul16x16to16hi(f, f, t); / Range 16 bits : f = t^3  (unsigned) [17]*/
        A("mul %5,%7")              /* r1:r0 = LO(f) * LO(t)*/
        A("mov %1,r1")              /* store MIL(LO(f) * LO(t)) in %1, we need it for rounding*/
        A("clr %10")                /* %10 = 0*/
        A("clr %11")                /* %11 = 0*/
        A("mul %5,%8")              /* r1:r0 = LO(f) * HI(t)*/
        A("add %1,r0")              /* %1 += LO(LO(f) * HI(t))*/
        A("adc %10,r1")             /* %10 = HI(LO(f) * HI(t))*/
        A("adc %11,%0")             /* %11 += carry*/
        A("mul %6,%7")              /* r1:r0 = HI(f) * LO(t)*/
        A("add %1,r0")              /* %1 += LO(HI(f) * LO(t))*/
        A("adc %10,r1")             /* %10 += HI(HI(f) * LO(t))*/
        A("adc %11,%0")             /* %11 += carry*/
        A("mul %6,%8")              /* r1:r0 = HI(f) * HI(t)*/
        A("add %10,r0")             /* %10 += LO(HI(f) * HI(t))*/
        A("adc %11,r1")             /* %11 += HI(HI(f) * HI(t))*/
        A("mov %5,%10")             /* %6:%5 =*/
        A("mov %6,%11")             /* f = %10:%11*/
        /* [15 +17*2] = [49]*/

        /* %4:%3:%2 will be acc from now on*/

        /* uint24_t acc = bezier_F; / Range 20 bits (unsigned)*/
        A("clr %9")                 /* "decimal place we get for free"*/
        A("lds %2,bezier_F")
        A("lds %3,bezier_F+1")
        A("lds %4,bezier_F+2")      /* %4:%3:%2 = acc*/

        /* if (A_negative) {*/
        A("lds r0,A_negative")
        A("or r0,%0")               /* Is flag signalling negative? */
        A("brne 3f")                /* If yes, Skip next instruction if A was negative*/
        A("rjmp 1f")                /* Otherwise, jump */

        /* uint24_t v; */
        /* umul16x24to24hi(v, f, bezier_C); / Range 21bits [29] */
        /* acc -= v; */
        L("3")
        A("lds %10, bezier_C")      /* %10 = LO(bezier_C)*/
        A("mul %10,%5")             /* r1:r0 = LO(bezier_C) * LO(f)*/
        A("sub %9,r1")
        A("sbc %2,%0")
        A("sbc %3,%0")
        A("sbc %4,%0")              /* %4:%3:%2:%9 -= HI(LO(bezier_C) * LO(f))*/
        A("lds %11, bezier_C+1")    /* %11 = MI(bezier_C)*/
        A("mul %11,%5")             /* r1:r0 = MI(bezier_C) * LO(f)*/
        A("sub %9,r0")
        A("sbc %2,r1")
        A("sbc %3,%0")
        A("sbc %4,%0")              /* %4:%3:%2:%9 -= MI(bezier_C) * LO(f)*/
        A("lds %1, bezier_C+2")     /* %1 = HI(bezier_C)*/
        A("mul %1,%5")              /* r1:r0 = MI(bezier_C) * LO(f)*/
        A("sub %2,r0")
        A("sbc %3,r1")
        A("sbc %4,%0")              /* %4:%3:%2:%9 -= HI(bezier_C) * LO(f) << 8*/
        A("mul %10,%6")             /* r1:r0 = LO(bezier_C) * MI(f)*/
        A("sub %9,r0")
        A("sbc %2,r1")
        A("sbc %3,%0")
        A("sbc %4,%0")              /* %4:%3:%2:%9 -= LO(bezier_C) * MI(f)*/
        A("mul %11,%6")             /* r1:r0 = MI(bezier_C) * MI(f)*/
        A("sub %2,r0")
        A("sbc %3,r1")
        A("sbc %4,%0")              /* %4:%3:%2:%9 -= MI(bezier_C) * MI(f) << 8*/
        A("mul %1,%6")              /* r1:r0 = HI(bezier_C) * LO(f)*/
        A("sub %3,r0")
        A("sbc %4,r1")              /* %4:%3:%2:%9 -= HI(bezier_C) * LO(f) << 16*/

        /* umul16x16to16hi(f, f, t); / Range 16 bits : f = t^3  (unsigned) [17]*/
        A("mul %5,%7")              /* r1:r0 = LO(f) * LO(t)*/
        A("mov %1,r1")              /* store MIL(LO(f) * LO(t)) in %1, we need it for rounding*/
        A("clr %10")                /* %10 = 0*/
        A("clr %11")                /* %11 = 0*/
        A("mul %5,%8")              /* r1:r0 = LO(f) * HI(t)*/
        A("add %1,r0")              /* %1 += LO(LO(f) * HI(t))*/
        A("adc %10,r1")             /* %10 = HI(LO(f) * HI(t))*/
        A("adc %11,%0")             /* %11 += carry*/
        A("mul %6,%7")              /* r1:r0 = HI(f) * LO(t)*/
        A("add %1,r0")              /* %1 += LO(HI(f) * LO(t))*/
        A("adc %10,r1")             /* %10 += HI(HI(f) * LO(t))*/
        A("adc %11,%0")             /* %11 += carry*/
        A("mul %6,%8")              /* r1:r0 = HI(f) * HI(t)*/
        A("add %10,r0")             /* %10 += LO(HI(f) * HI(t))*/
        A("adc %11,r1")             /* %11 += HI(HI(f) * HI(t))*/
        A("mov %5,%10")             /* %6:%5 =*/
        A("mov %6,%11")             /* f = %10:%11*/

        /* umul16x24to24hi(v, f, bezier_B); / Range 22bits [29]*/
        /* acc += v; */
        A("lds %10, bezier_B")      /* %10 = LO(bezier_B)*/
        A("mul %10,%5")             /* r1:r0 = LO(bezier_B) * LO(f)*/
        A("add %9,r1")
        A("adc %2,%0")
        A("adc %3,%0")
        A("adc %4,%0")              /* %4:%3:%2:%9 += HI(LO(bezier_B) * LO(f))*/
        A("lds %11, bezier_B+1")    /* %11 = MI(bezier_B)*/
        A("mul %11,%5")             /* r1:r0 = MI(bezier_B) * LO(f)*/
        A("add %9,r0")
        A("adc %2,r1")
        A("adc %3,%0")
        A("adc %4,%0")              /* %4:%3:%2:%9 += MI(bezier_B) * LO(f)*/
        A("lds %1, bezier_B+2")     /* %1 = HI(bezier_B)*/
        A("mul %1,%5")              /* r1:r0 = MI(bezier_B) * LO(f)*/
        A("add %2,r0")
        A("adc %3,r1")
        A("adc %4,%0")              /* %4:%3:%2:%9 += HI(bezier_B) * LO(f) << 8*/
        A("mul %10,%6")             /* r1:r0 = LO(bezier_B) * MI(f)*/
        A("add %9,r0")
        A("adc %2,r1")
        A("adc %3,%0")
        A("adc %4,%0")              /* %4:%3:%2:%9 += LO(bezier_B) * MI(f)*/
        A("mul %11,%6")             /* r1:r0 = MI(bezier_B) * MI(f)*/
        A("add %2,r0")
        A("adc %3,r1")
        A("adc %4,%0")              /* %4:%3:%2:%9 += MI(bezier_B) * MI(f) << 8*/
        A("mul %1,%6")              /* r1:r0 = HI(bezier_B) * LO(f)*/
        A("add %3,r0")
        A("adc %4,r1")              /* %4:%3:%2:%9 += HI(bezier_B) * LO(f) << 16*/

        /* umul16x16to16hi(f, f, t); / Range 16 bits : f = t^5  (unsigned) [17]*/
        A("mul %5,%7")              /* r1:r0 = LO(f) * LO(t)*/
        A("mov %1,r1")              /* store MIL(LO(f) * LO(t)) in %1, we need it for rounding*/
        A("clr %10")                /* %10 = 0*/
        A("clr %11")                /* %11 = 0*/
        A("mul %5,%8")              /* r1:r0 = LO(f) * HI(t)*/
        A("add %1,r0")              /* %1 += LO(LO(f) * HI(t))*/
        A("adc %10,r1")             /* %10 = HI(LO(f) * HI(t))*/
        A("adc %11,%0")             /* %11 += carry*/
        A("mul %6,%7")              /* r1:r0 = HI(f) * LO(t)*/
        A("add %1,r0")              /* %1 += LO(HI(f) * LO(t))*/
        A("adc %10,r1")             /* %10 += HI(HI(f) * LO(t))*/
        A("adc %11,%0")             /* %11 += carry*/
        A("mul %6,%8")              /* r1:r0 = HI(f) * HI(t)*/
        A("add %10,r0")             /* %10 += LO(HI(f) * HI(t))*/
        A("adc %11,r1")             /* %11 += HI(HI(f) * HI(t))*/
        A("mov %5,%10")             /* %6:%5 =*/
        A("mov %6,%11")             /* f = %10:%11*/

        /* umul16x24to24hi(v, f, bezier_A); / Range 21bits [29]*/
        /* acc -= v; */
        A("lds %10, bezier_A")      /* %10 = LO(bezier_A)*/
        A("mul %10,%5")             /* r1:r0 = LO(bezier_A) * LO(f)*/
        A("sub %9,r1")
        A("sbc %2,%0")
        A("sbc %3,%0")
        A("sbc %4,%0")              /* %4:%3:%2:%9 -= HI(LO(bezier_A) * LO(f))*/
        A("lds %11, bezier_A+1")    /* %11 = MI(bezier_A)*/
        A("mul %11,%5")             /* r1:r0 = MI(bezier_A) * LO(f)*/
        A("sub %9,r0")
        A("sbc %2,r1")
        A("sbc %3,%0")
        A("sbc %4,%0")              /* %4:%3:%2:%9 -= MI(bezier_A) * LO(f)*/
        A("lds %1, bezier_A+2")     /* %1 = HI(bezier_A)*/
        A("mul %1,%5")              /* r1:r0 = MI(bezier_A) * LO(f)*/
        A("sub %2,r0")
        A("sbc %3,r1")
        A("sbc %4,%0")              /* %4:%3:%2:%9 -= HI(bezier_A) * LO(f) << 8*/
        A("mul %10,%6")             /* r1:r0 = LO(bezier_A) * MI(f)*/
        A("sub %9,r0")
        A("sbc %2,r1")
        A("sbc %3,%0")
        A("sbc %4,%0")              /* %4:%3:%2:%9 -= LO(bezier_A) * MI(f)*/
        A("mul %11,%6")             /* r1:r0 = MI(bezier_A) * MI(f)*/
        A("sub %2,r0")
        A("sbc %3,r1")
        A("sbc %4,%0")              /* %4:%3:%2:%9 -= MI(bezier_A) * MI(f) << 8*/
        A("mul %1,%6")              /* r1:r0 = HI(bezier_A) * LO(f)*/
        A("sub %3,r0")
        A("sbc %4,r1")              /* %4:%3:%2:%9 -= HI(bezier_A) * LO(f) << 16*/
        A("jmp 2f")                 /* Done!*/

        L("1")

        /* uint24_t v; */
        /* umul16x24to24hi(v, f, bezier_C); / Range 21bits [29]*/
        /* acc += v; */
        A("lds %10, bezier_C")      /* %10 = LO(bezier_C)*/
        A("mul %10,%5")             /* r1:r0 = LO(bezier_C) * LO(f)*/
        A("add %9,r1")
        A("adc %2,%0")
        A("adc %3,%0")
        A("adc %4,%0")              /* %4:%3:%2:%9 += HI(LO(bezier_C) * LO(f))*/
        A("lds %11, bezier_C+1")    /* %11 = MI(bezier_C)*/
        A("mul %11,%5")             /* r1:r0 = MI(bezier_C) * LO(f)*/
        A("add %9,r0")
        A("adc %2,r1")
        A("adc %3,%0")
        A("adc %4,%0")              /* %4:%3:%2:%9 += MI(bezier_C) * LO(f)*/
        A("lds %1, bezier_C+2")     /* %1 = HI(bezier_C)*/
        A("mul %1,%5")              /* r1:r0 = MI(bezier_C) * LO(f)*/
        A("add %2,r0")
        A("adc %3,r1")
        A("adc %4,%0")              /* %4:%3:%2:%9 += HI(bezier_C) * LO(f) << 8*/
        A("mul %10,%6")             /* r1:r0 = LO(bezier_C) * MI(f)*/
        A("add %9,r0")
        A("adc %2,r1")
        A("adc %3,%0")
        A("adc %4,%0")              /* %4:%3:%2:%9 += LO(bezier_C) * MI(f)*/
        A("mul %11,%6")             /* r1:r0 = MI(bezier_C) * MI(f)*/
        A("add %2,r0")
        A("adc %3,r1")
        A("adc %4,%0")              /* %4:%3:%2:%9 += MI(bezier_C) * MI(f) << 8*/
        A("mul %1,%6")              /* r1:r0 = HI(bezier_C) * LO(f)*/
        A("add %3,r0")
        A("adc %4,r1")              /* %4:%3:%2:%9 += HI(bezier_C) * LO(f) << 16*/

        /* umul16x16to16hi(f, f, t); / Range 16 bits : f = t^3  (unsigned) [17]*/
        A("mul %5,%7")              /* r1:r0 = LO(f) * LO(t)*/
        A("mov %1,r1")              /* store MIL(LO(f) * LO(t)) in %1, we need it for rounding*/
        A("clr %10")                /* %10 = 0*/
        A("clr %11")                /* %11 = 0*/
        A("mul %5,%8")              /* r1:r0 = LO(f) * HI(t)*/
        A("add %1,r0")              /* %1 += LO(LO(f) * HI(t))*/
        A("adc %10,r1")             /* %10 = HI(LO(f) * HI(t))*/
        A("adc %11,%0")             /* %11 += carry*/
        A("mul %6,%7")              /* r1:r0 = HI(f) * LO(t)*/
        A("add %1,r0")              /* %1 += LO(HI(f) * LO(t))*/
        A("adc %10,r1")             /* %10 += HI(HI(f) * LO(t))*/
        A("adc %11,%0")             /* %11 += carry*/
        A("mul %6,%8")              /* r1:r0 = HI(f) * HI(t)*/
        A("add %10,r0")             /* %10 += LO(HI(f) * HI(t))*/
        A("adc %11,r1")             /* %11 += HI(HI(f) * HI(t))*/
        A("mov %5,%10")             /* %6:%5 =*/
        A("mov %6,%11")             /* f = %10:%11*/

        /* umul16x24to24hi(v, f, bezier_B); / Range 22bits [29]*/
        /* acc -= v;*/
        A("lds %10, bezier_B")      /* %10 = LO(bezier_B)*/
        A("mul %10,%5")             /* r1:r0 = LO(bezier_B) * LO(f)*/
        A("sub %9,r1")
        A("sbc %2,%0")
        A("sbc %3,%0")
        A("sbc %4,%0")              /* %4:%3:%2:%9 -= HI(LO(bezier_B) * LO(f))*/
        A("lds %11, bezier_B+1")    /* %11 = MI(bezier_B)*/
        A("mul %11,%5")             /* r1:r0 = MI(bezier_B) * LO(f)*/
        A("sub %9,r0")
        A("sbc %2,r1")
        A("sbc %3,%0")
        A("sbc %4,%0")              /* %4:%3:%2:%9 -= MI(bezier_B) * LO(f)*/
        A("lds %1, bezier_B+2")     /* %1 = HI(bezier_B)*/
        A("mul %1,%5")              /* r1:r0 = MI(bezier_B) * LO(f)*/
        A("sub %2,r0")
        A("sbc %3,r1")
        A("sbc %4,%0")              /* %4:%3:%2:%9 -= HI(bezier_B) * LO(f) << 8*/
        A("mul %10,%6")             /* r1:r0 = LO(bezier_B) * MI(f)*/
        A("sub %9,r0")
        A("sbc %2,r1")
        A("sbc %3,%0")
        A("sbc %4,%0")              /* %4:%3:%2:%9 -= LO(bezier_B) * MI(f)*/
        A("mul %11,%6")             /* r1:r0 = MI(bezier_B) * MI(f)*/
        A("sub %2,r0")
        A("sbc %3,r1")
        A("sbc %4,%0")              /* %4:%3:%2:%9 -= MI(bezier_B) * MI(f) << 8*/
        A("mul %1,%6")              /* r1:r0 = HI(bezier_B) * LO(f)*/
        A("sub %3,r0")
        A("sbc %4,r1")              /* %4:%3:%2:%9 -= HI(bezier_B) * LO(f) << 16*/

        /* umul16x16to16hi(f, f, t); / Range 16 bits : f = t^5  (unsigned) [17]*/
        A("mul %5,%7")              /* r1:r0 = LO(f) * LO(t)*/
        A("mov %1,r1")              /* store MIL(LO(f) * LO(t)) in %1, we need it for rounding*/
        A("clr %10")                /* %10 = 0*/
        A("clr %11")                /* %11 = 0*/
        A("mul %5,%8")              /* r1:r0 = LO(f) * HI(t)*/
        A("add %1,r0")              /* %1 += LO(LO(f) * HI(t))*/
        A("adc %10,r1")             /* %10 = HI(LO(f) * HI(t))*/
        A("adc %11,%0")             /* %11 += carry*/
        A("mul %6,%7")              /* r1:r0 = HI(f) * LO(t)*/
        A("add %1,r0")              /* %1 += LO(HI(f) * LO(t))*/
        A("adc %10,r1")             /* %10 += HI(HI(f) * LO(t))*/
        A("adc %11,%0")             /* %11 += carry*/
        A("mul %6,%8")              /* r1:r0 = HI(f) * HI(t)*/
        A("add %10,r0")             /* %10 += LO(HI(f) * HI(t))*/
        A("adc %11,r1")             /* %11 += HI(HI(f) * HI(t))*/
        A("mov %5,%10")             /* %6:%5 =*/
        A("mov %6,%11")             /* f = %10:%11*/

        /* umul16x24to24hi(v, f, bezier_A); / Range 21bits [29]*/
        /* acc += v; */
        A("lds %10, bezier_A")      /* %10 = LO(bezier_A)*/
        A("mul %10,%5")             /* r1:r0 = LO(bezier_A) * LO(f)*/
        A("add %9,r1")
        A("adc %2,%0")
        A("adc %3,%0")
        A("adc %4,%0")              /* %4:%3:%2:%9 += HI(LO(bezier_A) * LO(f))*/
        A("lds %11, bezier_A+1")    /* %11 = MI(bezier_A)*/
        A("mul %11,%5")             /* r1:r0 = MI(bezier_A) * LO(f)*/
        A("add %9,r0")
        A("adc %2,r1")
        A("adc %3,%0")
        A("adc %4,%0")              /* %4:%3:%2:%9 += MI(bezier_A) * LO(f)*/
        A("lds %1, bezier_A+2")     /* %1 = HI(bezier_A)*/
        A("mul %1,%5")              /* r1:r0 = MI(bezier_A) * LO(f)*/
        A("add %2,r0")
        A("adc %3,r1")
        A("adc %4,%0")              /* %4:%3:%2:%9 += HI(bezier_A) * LO(f) << 8*/
        A("mul %10,%6")             /* r1:r0 = LO(bezier_A) * MI(f)*/
        A("add %9,r0")
        A("adc %2,r1")
        A("adc %3,%0")
        A("adc %4,%0")              /* %4:%3:%2:%9 += LO(bezier_A) * MI(f)*/
        A("mul %11,%6")             /* r1:r0 = MI(bezier_A) * MI(f)*/
        A("add %2,r0")
        A("adc %3,r1")
        A("adc %4,%0")              /* %4:%3:%2:%9 += MI(bezier_A) * MI(f) << 8*/
        A("mul %1,%6")              /* r1:r0 = HI(bezier_A) * LO(f)*/
        A("add %3,r0")
        A("adc %4,r1")              /* %4:%3:%2:%9 += HI(bezier_A) * LO(f) << 16*/
        L("2")
        " clr __zero_reg__"         /* C runtime expects r1 = __zero_reg__ = 0 */
        : "+r"(r0),
          "+r"(r1),
          "+r"(r2),
          "+r"(r3),
          "+r"(r4),
          "+r"(r5),
          "+r"(r6),
          "+r"(r7),
          "+r"(r8),
          "+r"(r9),
          "+r"(r10),
          "+r"(r11)
        :
        :"cc","r0","r1"
      );
      return (r2 | (uint16_t(r3) << 8)) | (uint32_t(r4) << 16);
    }

  #else

    // For all the other 32bit CPUs
    FORCE_INLINE void Stepper::_calc_bezier_curve_coeffs(const int32_t v0, const int32_t v1, const uint32_t av) {
      // Calculate the Bézier coefficients
      bezier_A =  768 * (v1 - v0);
      bezier_B = 1920 * (v0 - v1);
      bezier_C = 1280 * (v1 - v0);
      bezier_F =  128 * v0;
      bezier_AV = av;
    }

    FORCE_INLINE int32_t Stepper::_eval_bezier_curve(const uint32_t curr_step) {
      #if defined(__arm__) || defined(__thumb__)

        // For ARM Cortex M3/M4 CPUs, we have the optimized assembler version, that takes 43 cycles to execute
        uint32_t flo = 0;
        uint32_t fhi = bezier_AV * curr_step;
        uint32_t t = fhi;
        int32_t alo = bezier_F;
        int32_t ahi = 0;
        int32_t A = bezier_A;
        int32_t B = bezier_B;
        int32_t C = bezier_C;

         __asm__ __volatile__(
          ".syntax unified" "\n\t"              // is to prevent CM0,CM1 non-unified syntax
          A("lsrs  %[ahi],%[alo],#1")           // a  = F << 31      1 cycles
          A("lsls  %[alo],%[alo],#31")          //                   1 cycles
          A("umull %[flo],%[fhi],%[fhi],%[t]")  // f *= t            5 cycles [fhi:flo=64bits]
          A("umull %[flo],%[fhi],%[fhi],%[t]")  // f>>=32; f*=t      5 cycles [fhi:flo=64bits]
          A("lsrs  %[flo],%[fhi],#1")           //                   1 cycles [31bits]
          A("smlal %[alo],%[ahi],%[flo],%[C]")  // a+=(f>>33)*C;     5 cycles
          A("umull %[flo],%[fhi],%[fhi],%[t]")  // f>>=32; f*=t      5 cycles [fhi:flo=64bits]
          A("lsrs  %[flo],%[fhi],#1")           //                   1 cycles [31bits]
          A("smlal %[alo],%[ahi],%[flo],%[B]")  // a+=(f>>33)*B;     5 cycles
          A("umull %[flo],%[fhi],%[fhi],%[t]")  // f>>=32; f*=t      5 cycles [fhi:flo=64bits]
          A("lsrs  %[flo],%[fhi],#1")           // f>>=33;           1 cycles [31bits]
          A("smlal %[alo],%[ahi],%[flo],%[A]")  // a+=(f>>33)*A;     5 cycles
          A("lsrs  %[alo],%[ahi],#6")           // a>>=38            1 cycles
          : [alo]"+r"( alo ) ,
            [flo]"+r"( flo ) ,
            [fhi]"+r"( fhi ) ,
            [ahi]"+r"( ahi ) ,
            [A]"+r"( A ) ,  // <== Note: Even if A, B, C, and t registers are INPUT ONLY
            [B]"+r"( B ) ,  //  GCC does bad optimizations on the code if we list them as
            [C]"+r"( C ) ,  //  such, breaking this function. So, to avoid that problem,
            [t]"+r"( t )    //  we list all registers as input-outputs.
          :
          : "cc"
        );
        return alo;

      #else

        // For non ARM targets, we provide a fallback implementation. Really doubt it
        // will be useful, unless the processor is fast and 32bit

        uint32_t t = bezier_AV * curr_step;               // t: Range 0 - 1^32 = 32 bits
        uint64_t f = t;
        f *= t;                                           // Range 32*2 = 64 bits (unsigned)
        f >>= 32;                                         // Range 32 bits  (unsigned)
        f *= t;                                           // Range 32*2 = 64 bits  (unsigned)
        f >>= 32;                                         // Range 32 bits : f = t^3  (unsigned)
        int64_t acc = (int64_t) bezier_F << 31;           // Range 63 bits (signed)
        acc += ((uint32_t) f >> 1) * (int64_t) bezier_C;  // Range 29bits + 31 = 60bits (plus sign)
        f *= t;                                           // Range 32*2 = 64 bits
        f >>= 32;                                         // Range 32 bits : f = t^3  (unsigned)
        acc += ((uint32_t) f >> 1) * (int64_t) bezier_B;  // Range 29bits + 31 = 60bits (plus sign)
        f *= t;                                           // Range 32*2 = 64 bits
        f >>= 32;                                         // Range 32 bits : f = t^3  (unsigned)
        acc += ((uint32_t) f >> 1) * (int64_t) bezier_A;  // Range 28bits + 31 = 59bits (plus sign)
        acc >>= (31 + 7);                                 // Range 24bits (plus sign)
        return (int32_t) acc;

      #endif
    }
  #endif
#endif // S_CURVE_ACCELERATION

/**
 * Stepper Driver Interrupt
 *
 * Directly pulses the stepper motors at high frequency.
 */

HAL_STEP_TIMER_ISR() {
  HAL_timer_isr_prologue(STEP_TIMER_NUM);

  Stepper::isr();

  HAL_timer_isr_epilogue(STEP_TIMER_NUM);
}

#ifdef CPU_32_BIT
  #define STEP_MULTIPLY(A,B) MultiU32X24toH32(A, B)
#else
  #define STEP_MULTIPLY(A,B) MultiU24X32toH16(A, B)
#endif

void Stepper::isr() {

  static uint32_t nextMainISR = 0;  // Interval until the next main Stepper Pulse phase (0 = Now)

  #ifndef __AVR__
    // Disable interrupts, to avoid ISR preemption while we reprogram the period
    // (AVR enters the ISR with global interrupts disabled, so no need to do it here)
    DISABLE_ISRS();
  #endif

  // Program timer compare for the maximum period, so it does NOT
  // flag an interrupt while this ISR is running - So changes from small
  // periods to big periods are respected and the timer does not reset to 0
  HAL_timer_set_compare(STEP_TIMER_NUM, hal_timer_t(HAL_TIMER_TYPE_MAX));

  // Count of ticks for the next ISR
  hal_timer_t next_isr_ticks = 0;

  // Limit the amount of iterations
  uint8_t max_loops = 10;

  // We need this variable here to be able to use it in the following loop
  hal_timer_t min_ticks;
  do {
    // Enable ISRs to reduce USART processing latency
    ENABLE_ISRS();

    if (!nextMainISR) pulse_phase_isr();                            // 0 = Do coordinated axes Stepper pulses

    #if ENABLED(LIN_ADVANCE)
      if (!nextAdvanceISR) nextAdvanceISR = advance_isr();          // 0 = Do Linear Advance E Stepper pulses
    #endif

    #if ENABLED(INTEGRATED_BABYSTEPPING)
      const bool is_babystep = (nextBabystepISR == 0);              // 0 = Do Babystepping (XY)Z pulses
      if (is_babystep) nextBabystepISR = babystepping_isr();
    #endif

    // ^== Time critical. NOTHING besides pulse generation should be above here!!!

    if (!nextMainISR) nextMainISR = block_phase_isr();  // Manage acc/deceleration, get next block

    #if ENABLED(INTEGRATED_BABYSTEPPING)
      if (is_babystep)                                  // Avoid ANY stepping too soon after baby-stepping
        NOLESS(nextMainISR, (BABYSTEP_TICKS) / 8);      // FULL STOP for 125µs after a baby-step

      if (nextBabystepISR != BABYSTEP_NEVER)            // Avoid baby-stepping too close to axis Stepping
        NOLESS(nextBabystepISR, nextMainISR / 2);       // TODO: Only look at axes enabled for baby-stepping
    #endif

    // Get the interval to the next ISR call
    const uint32_t interval = _MIN(
      nextMainISR                                       // Time until the next Pulse / Block phase
      #if ENABLED(LIN_ADVANCE)
        , nextAdvanceISR                                // Come back early for Linear Advance?
      #endif
      #if ENABLED(INTEGRATED_BABYSTEPPING)
        , nextBabystepISR                               // Come back early for Babystepping?
      #endif
      , uint32_t(HAL_TIMER_TYPE_MAX)                    // Come back in a very long time
    );

    //
    // Compute remaining time for each ISR phase
    //     NEVER : The phase is idle
    //      Zero : The phase will occur on the next ISR call
    //  Non-zero : The phase will occur on a future ISR call
    //

    nextMainISR -= interval;

    #if ENABLED(LIN_ADVANCE)
      if (nextAdvanceISR != LA_ADV_NEVER) nextAdvanceISR -= interval;
    #endif

    #if ENABLED(INTEGRATED_BABYSTEPPING)
      if (nextBabystepISR != BABYSTEP_NEVER) nextBabystepISR -= interval;
    #endif

    /**
     * This needs to avoid a race-condition caused by interleaving
     * of interrupts required by both the LA and Stepper algorithms.
     *
     * Assume the following tick times for stepper pulses:
     *   Stepper ISR (S):  1 1000 2000 3000 4000
     *   Linear Adv. (E): 10 1010 2010 3010 4010
     *
     * The current algorithm tries to interleave them, giving:
     *  1:S 10:E 1000:S 1010:E 2000:S 2010:E 3000:S 3010:E 4000:S 4010:E
     *
     * Ideal timing would yield these delta periods:
     *  1:S  9:E  990:S   10:E  990:S   10:E  990:S   10:E  990:S   10:E
     *
     * But, since each event must fire an ISR with a minimum duration, the
     * minimum delta might be 900, so deltas under 900 get rounded up:
     *  900:S d900:E d990:S d900:E d990:S d900:E d990:S d900:E d990:S d900:E
     *
     * It works, but divides the speed of all motors by half, leading to a sudden
     * reduction to 1/2 speed! Such jumps in speed lead to lost steps (not even
     * accounting for double/quad stepping, which makes it even worse).
     */

    // Compute the tick count for the next ISR
    next_isr_ticks += interval;

    /**
     * The following section must be done with global interrupts disabled.
     * We want nothing to interrupt it, as that could mess the calculations
     * we do for the next value to program in the period register of the
     * stepper timer and lead to skipped ISRs (if the value we happen to program
     * is less than the current count due to something preempting between the
     * read and the write of the new period value).
     */
    DISABLE_ISRS();

    /**
     * Get the current tick value + margin
     * Assuming at least 6µs between calls to this ISR...
     * On AVR the ISR epilogue+prologue is estimated at 100 instructions - Give 8µs as margin
     * On ARM the ISR epilogue+prologue is estimated at 20 instructions - Give 1µs as margin
     */
    min_ticks = HAL_timer_get_count(STEP_TIMER_NUM) + hal_timer_t(
      #ifdef __AVR__
        8
      #else
        1
      #endif
      * (STEPPER_TIMER_TICKS_PER_US)
    );

    /**
     * NB: If for some reason the stepper monopolizes the MPU, eventually the
     * timer will wrap around (and so will 'next_isr_ticks'). So, limit the
     * loop to 10 iterations. Beyond that, there's no way to ensure correct pulse
     * timing, since the MCU isn't fast enough.
     */
    if (!--max_loops) next_isr_ticks = min_ticks;

    // Advance pulses if not enough time to wait for the next ISR
  } while (next_isr_ticks < min_ticks);

  // Now 'next_isr_ticks' contains the period to the next Stepper ISR - And we are
  // sure that the time has not arrived yet - Warrantied by the scheduler

  // Set the next ISR to fire at the proper time
  HAL_timer_set_compare(STEP_TIMER_NUM, hal_timer_t(next_isr_ticks));

  // Don't forget to finally reenable interrupts
  ENABLE_ISRS();
}

#if MINIMUM_STEPPER_PULSE || MAXIMUM_STEPPER_RATE
  #define ISR_PULSE_CONTROL 1
#endif
#if ISR_PULSE_CONTROL && DISABLED(I2S_STEPPER_STREAM)
  #define ISR_MULTI_STEPS 1
#endif

/**
 * This phase of the ISR should ONLY create the pulses for the steppers.
 * This prevents jitter caused by the interval between the start of the
 * interrupt and the start of the pulses. DON'T add any logic ahead of the
 * call to this method that might cause variation in the timing. The aim
 * is to keep pulse timing as regular as possible.
 */
void Stepper::pulse_phase_isr() {

  // If we must abort the current block, do so!
  if (abort_current_block) {
    abort_current_block = false;
    if (current_block) discard_current_block();
  }

  // If there is no current block, do nothing
  if (!current_block) return;

  // Skipping step processing causes motion to freeze
  if (TERN0(HAS_FREEZE_PIN, frozen)) return;

  // Count of pending loops and events for this iteration
  const uint32_t pending_events = step_event_count - step_events_completed;
  uint8_t events_to_do = _MIN(pending_events, steps_per_isr);

  // Just update the value we will get at the end of the loop
  step_events_completed += events_to_do;

  // Take multiple steps per interrupt (For high speed moves)
  #if ISR_MULTI_STEPS
    bool firstStep = true;
    USING_TIMED_PULSE();
  #endif
  xyze_bool_t step_needed{0};

  do {
    #define _APPLY_STEP(AXIS, INV, ALWAYS) AXIS ##_APPLY_STEP(INV, ALWAYS)
    #define _INVERT_STEP_PIN(AXIS) INVERT_## AXIS ##_STEP_PIN

    // Determine if a pulse is needed using Bresenham
    #define PULSE_PREP(AXIS) do{ \
      delta_error[_AXIS(AXIS)] += advance_dividend[_AXIS(AXIS)]; \
      step_needed[_AXIS(AXIS)] = (delta_error[_AXIS(AXIS)] >= 0); \
      if (step_needed[_AXIS(AXIS)]) { \
        count_position[_AXIS(AXIS)] += count_direction[_AXIS(AXIS)]; \
        delta_error[_AXIS(AXIS)] -= advance_divisor; \
      } \
    }while(0)

    // Start an active pulse if needed
    #define PULSE_START(AXIS) do{ \
      if (step_needed[_AXIS(AXIS)]) { \
        _APPLY_STEP(AXIS, !_INVERT_STEP_PIN(AXIS), 0); \
      } \
    }while(0)

    // Stop an active pulse if needed
    #define PULSE_STOP(AXIS) do { \
      if (step_needed[_AXIS(AXIS)]) { \
        _APPLY_STEP(AXIS, _INVERT_STEP_PIN(AXIS), 0); \
      } \
    }while(0)

    // Direct Stepping page?
    const bool is_page = IS_PAGE(current_block);

    #if ENABLED(DIRECT_STEPPING)

      if (is_page) {

        #if STEPPER_PAGE_FORMAT == SP_4x4D_128

          #define PAGE_SEGMENT_UPDATE(AXIS, VALUE) do{   \
                 if ((VALUE) <  7) SBI(dm, _AXIS(AXIS)); \
            else if ((VALUE) >  7) CBI(dm, _AXIS(AXIS)); \
            page_step_state.sd[_AXIS(AXIS)] = VALUE;     \
            page_step_state.bd[_AXIS(AXIS)] += VALUE;    \
          }while(0)

          #define PAGE_PULSE_PREP(AXIS) do{ \
            step_needed[_AXIS(AXIS)] =      \
              pgm_read_byte(&segment_table[page_step_state.sd[_AXIS(AXIS)]][page_step_state.segment_steps & 0x7]); \
          }while(0)

          switch (page_step_state.segment_steps) {
            case DirectStepping::Config::SEGMENT_STEPS:
              page_step_state.segment_idx += 2;
              page_step_state.segment_steps = 0;
              // fallthru
            case 0: {
              const uint8_t low = page_step_state.page[page_step_state.segment_idx],
                           high = page_step_state.page[page_step_state.segment_idx + 1];
              uint8_t dm = last_direction_bits;

              PAGE_SEGMENT_UPDATE(X, low >> 4);
              PAGE_SEGMENT_UPDATE(Y, low & 0xF);
              PAGE_SEGMENT_UPDATE(Z, high >> 4);
              PAGE_SEGMENT_UPDATE(E, high & 0xF);

              if (dm != last_direction_bits)
                set_directions(dm);

            } break;

            default: break;
          }

          PAGE_PULSE_PREP(X);
          PAGE_PULSE_PREP(Y);
          PAGE_PULSE_PREP(Z);
          PAGE_PULSE_PREP(E);

          page_step_state.segment_steps++;

        #elif STEPPER_PAGE_FORMAT == SP_4x2_256

          #define PAGE_SEGMENT_UPDATE(AXIS, VALUE) \
            page_step_state.sd[_AXIS(AXIS)] = VALUE; \
            page_step_state.bd[_AXIS(AXIS)] += VALUE;

          #define PAGE_PULSE_PREP(AXIS) do{ \
            step_needed[_AXIS(AXIS)] =      \
              pgm_read_byte(&segment_table[page_step_state.sd[_AXIS(AXIS)]][page_step_state.segment_steps & 0x3]); \
          }while(0)

          switch (page_step_state.segment_steps) {
            case DirectStepping::Config::SEGMENT_STEPS:
              page_step_state.segment_idx++;
              page_step_state.segment_steps = 0;
              // fallthru
            case 0: {
              const uint8_t b = page_step_state.page[page_step_state.segment_idx];
              PAGE_SEGMENT_UPDATE(X, (b >> 6) & 0x3);
              PAGE_SEGMENT_UPDATE(Y, (b >> 4) & 0x3);
              PAGE_SEGMENT_UPDATE(Z, (b >> 2) & 0x3);
              PAGE_SEGMENT_UPDATE(E, (b >> 0) & 0x3);
            } break;
            default: break;
          }

          PAGE_PULSE_PREP(X);
          PAGE_PULSE_PREP(Y);
          PAGE_PULSE_PREP(Z);
          PAGE_PULSE_PREP(E);

          page_step_state.segment_steps++;

        #elif STEPPER_PAGE_FORMAT == SP_4x1_512

          #define PAGE_PULSE_PREP(AXIS, BITS) do{             \
            step_needed[_AXIS(AXIS)] = (steps >> BITS) & 0x1; \
            if (step_needed[_AXIS(AXIS)])                     \
              page_step_state.bd[_AXIS(AXIS)]++;              \
          }while(0)

          uint8_t steps = page_step_state.page[page_step_state.segment_idx >> 1];
          if (page_step_state.segment_idx & 0x1) steps >>= 4;

          PAGE_PULSE_PREP(X, 3);
          PAGE_PULSE_PREP(Y, 2);
          PAGE_PULSE_PREP(Z, 1);
          PAGE_PULSE_PREP(E, 0);

          page_step_state.segment_idx++;

        #else
          #error "Unknown direct stepping page format!"
        #endif
      }

    #endif // DIRECT_STEPPING

    if (!is_page) {
      // Determine if pulses are needed
      #if HAS_X_STEP
        PULSE_PREP(X);
      #endif
      #if HAS_Y_STEP
        PULSE_PREP(Y);
      #endif
      #if HAS_Z_STEP
        PULSE_PREP(Z);
      #endif

      #if EITHER(LIN_ADVANCE, MIXING_EXTRUDER)
        delta_error.e += advance_dividend.e;
        if (delta_error.e >= 0) {
          #if ENABLED(LIN_ADVANCE)
            delta_error.e -= advance_divisor;
            // Don't step E here - But remember the number of steps to perform
            motor_direction(E_AXIS) ? --LA_steps : ++LA_steps;
          #else
            count_position.e += count_direction.e;
            step_needed.e = true;
          #endif
        }
      #elif HAS_E0_STEP
        PULSE_PREP(E);
      #endif
    }

    #if ISR_MULTI_STEPS
      if (firstStep)
        firstStep = false;
      else
        AWAIT_LOW_PULSE();
    #endif

    // Pulse start
    #if HAS_X_STEP
      PULSE_START(X);
    #endif
    #if HAS_Y_STEP
      PULSE_START(Y);
    #endif
    #if HAS_Z_STEP
      PULSE_START(Z);
    #endif

    #if DISABLED(LIN_ADVANCE)
      #if ENABLED(MIXING_EXTRUDER)
        if (step_needed.e) E_STEP_WRITE(mixer.get_next_stepper(), !INVERT_E_STEP_PIN);
      #elif HAS_E0_STEP
        PULSE_START(E);
      #endif
    #endif

    #if ENABLED(I2S_STEPPER_STREAM)
      i2s_push_sample();
    #endif

    // TODO: need to deal with MINIMUM_STEPPER_PULSE over i2s
    #if ISR_MULTI_STEPS
      START_HIGH_PULSE();
      AWAIT_HIGH_PULSE();
    #endif

    // Pulse stop
    #if HAS_X_STEP
      PULSE_STOP(X);
    #endif
    #if HAS_Y_STEP
      PULSE_STOP(Y);
    #endif
    #if HAS_Z_STEP
      PULSE_STOP(Z);
    #endif

    #if DISABLED(LIN_ADVANCE)
      #if ENABLED(MIXING_EXTRUDER)
        if (delta_error.e >= 0) {
          delta_error.e -= advance_divisor;
          E_STEP_WRITE(mixer.get_stepper(), INVERT_E_STEP_PIN);
        }
      #elif HAS_E0_STEP
        PULSE_STOP(E);
      #endif
    #endif

    #if ISR_MULTI_STEPS
      if (events_to_do) START_LOW_PULSE();
    #endif

  } while (--events_to_do);
}

// This is the last half of the stepper interrupt: This one processes and
// properly schedules blocks from the planner. This is executed after creating
// the step pulses, so it is not time critical, as pulses are already done.

uint32_t Stepper::block_phase_isr() {

  // If no queued movements, just wait 1ms for the next block
  uint32_t interval = (STEPPER_TIMER_RATE) / 1000UL;

  // If there is a current block
  if (current_block) {

    // If current block is finished, reset pointer and finalize state
    if (step_events_completed >= step_event_count) {
      #if ENABLED(DIRECT_STEPPING)
        #if STEPPER_PAGE_FORMAT == SP_4x4D_128
          #define PAGE_SEGMENT_UPDATE_POS(AXIS) \
            count_position[_AXIS(AXIS)] += page_step_state.bd[_AXIS(AXIS)] - 128 * 7;
        #elif STEPPER_PAGE_FORMAT == SP_4x1_512 || STEPPER_PAGE_FORMAT == SP_4x2_256
          #define PAGE_SEGMENT_UPDATE_POS(AXIS) \
            count_position[_AXIS(AXIS)] += page_step_state.bd[_AXIS(AXIS)] * count_direction[_AXIS(AXIS)];
        #endif

        if (IS_PAGE(current_block)) {
          PAGE_SEGMENT_UPDATE_POS(X);
          PAGE_SEGMENT_UPDATE_POS(Y);
          PAGE_SEGMENT_UPDATE_POS(Z);
          PAGE_SEGMENT_UPDATE_POS(E);
        }
      #endif
      TERN_(HAS_FILAMENT_RUNOUT_DISTANCE, runout.block_completed(current_block));
      discard_current_block();
    }
    else {
      // Step events not completed yet...

      // Are we in acceleration phase ?
      if (step_events_completed <= accelerate_until) { // Calculate new timer value

        #if ENABLED(S_CURVE_ACCELERATION)
          // Get the next speed to use (Jerk limited!)
          uint32_t acc_step_rate = acceleration_time < current_block->acceleration_time
                                   ? _eval_bezier_curve(acceleration_time)
                                   : current_block->cruise_rate;
        #else
          acc_step_rate = STEP_MULTIPLY(acceleration_time, current_block->acceleration_rate) + current_block->initial_rate;
          NOMORE(acc_step_rate, current_block->nominal_rate);
        #endif

        // acc_step_rate is in steps/second

        // step_rate to timer interval and steps per stepper isr
        interval = calc_timer_interval(acc_step_rate, &steps_per_isr);
        acceleration_time += interval;

        #if ENABLED(LIN_ADVANCE)
          if (LA_use_advance_lead) {
            // Fire ISR if final adv_rate is reached
            if (LA_steps && LA_isr_rate != current_block->advance_speed) nextAdvanceISR = 0;
          }
          else if (LA_steps) nextAdvanceISR = 0;
        #endif

        // Update laser - Accelerating
        #if ENABLED(LASER_POWER_INLINE_TRAPEZOID)
          if (laser_trap.enabled) {
            #if DISABLED(LASER_POWER_INLINE_TRAPEZOID_CONT)
              if (current_block->laser.entry_per) {
                laser_trap.acc_step_count -= step_events_completed - laser_trap.last_step_count;
                laser_trap.last_step_count = step_events_completed;

                // Should be faster than a divide, since this should trip just once
                if (laser_trap.acc_step_count < 0) {
                  while (laser_trap.acc_step_count < 0) {
                    laser_trap.acc_step_count += current_block->laser.entry_per;
                    if (laser_trap.cur_power < current_block->laser.power) laser_trap.cur_power++;
                  }
                  cutter.set_ocr_power(laser_trap.cur_power);
                }
              }
            #else
              if (laser_trap.till_update)
                laser_trap.till_update--;
              else {
                laser_trap.till_update = LASER_POWER_INLINE_TRAPEZOID_CONT_PER;
                laser_trap.cur_power = (current_block->laser.power * acc_step_rate) / current_block->nominal_rate;
                cutter.set_ocr_power(laser_trap.cur_power); // Cycle efficiency is irrelevant it the last line was many cycles
              }
            #endif
          }
        #endif
      }
      // Are we in Deceleration phase ?
      else if (step_events_completed > decelerate_after) {
        uint32_t step_rate;

        #if ENABLED(S_CURVE_ACCELERATION)
          // If this is the 1st time we process the 2nd half of the trapezoid...
          if (!bezier_2nd_half) {
            // Initialize the Bézier speed curve
            _calc_bezier_curve_coeffs(current_block->cruise_rate, current_block->final_rate, current_block->deceleration_time_inverse);
            bezier_2nd_half = true;
            // The first point starts at cruise rate. Just save evaluation of the Bézier curve
            step_rate = current_block->cruise_rate;
          }
          else {
            // Calculate the next speed to use
            step_rate = deceleration_time < current_block->deceleration_time
              ? _eval_bezier_curve(deceleration_time)
              : current_block->final_rate;
          }
        #else

          // Using the old trapezoidal control
          step_rate = STEP_MULTIPLY(deceleration_time, current_block->acceleration_rate);
          if (step_rate < acc_step_rate) { // Still decelerating?
            step_rate = acc_step_rate - step_rate;
            NOLESS(step_rate, current_block->final_rate);
          }
          else
            step_rate = current_block->final_rate;
        #endif

        // step_rate is in steps/second

        // step_rate to timer interval and steps per stepper isr
        interval = calc_timer_interval(step_rate, &steps_per_isr);
        deceleration_time += interval;

        #if ENABLED(LIN_ADVANCE)
          if (LA_use_advance_lead) {
            // Wake up eISR on first deceleration loop and fire ISR if final adv_rate is reached
            if (step_events_completed <= decelerate_after + steps_per_isr || (LA_steps && LA_isr_rate != current_block->advance_speed)) {
              initiateLA();
              LA_isr_rate = current_block->advance_speed;
            }
          }
          else if (LA_steps) nextAdvanceISR = 0;
        #endif // LIN_ADVANCE

        // Update laser - Decelerating
        #if ENABLED(LASER_POWER_INLINE_TRAPEZOID)
          if (laser_trap.enabled) {
            #if DISABLED(LASER_POWER_INLINE_TRAPEZOID_CONT)
              if (current_block->laser.exit_per) {
                laser_trap.acc_step_count -= step_events_completed - laser_trap.last_step_count;
                laser_trap.last_step_count = step_events_completed;

                // Should be faster than a divide, since this should trip just once
                if (laser_trap.acc_step_count < 0) {
                  while (laser_trap.acc_step_count < 0) {
                    laser_trap.acc_step_count += current_block->laser.exit_per;
                    if (laser_trap.cur_power > current_block->laser.power_exit) laser_trap.cur_power--;
                  }
                  cutter.set_ocr_power(laser_trap.cur_power);
                }
              }
            #else
              if (laser_trap.till_update)
                laser_trap.till_update--;
              else {
                laser_trap.till_update = LASER_POWER_INLINE_TRAPEZOID_CONT_PER;
                laser_trap.cur_power = (current_block->laser.power * step_rate) / current_block->nominal_rate;
                cutter.set_ocr_power(laser_trap.cur_power); // Cycle efficiency isn't relevant when the last line was many cycles
              }
            #endif
          }
        #endif
      }
      // Must be in cruise phase otherwise
      else {

        #if ENABLED(LIN_ADVANCE)
          // If there are any esteps, fire the next advance_isr "now"
          if (LA_steps && LA_isr_rate != current_block->advance_speed) initiateLA();
        #endif

        // Calculate the ticks_nominal for this nominal speed, if not done yet
        if (ticks_nominal < 0) {
          // step_rate to timer interval and loops for the nominal speed
          ticks_nominal = calc_timer_interval(current_block->nominal_rate, &steps_per_isr);
        }

        // The timer interval is just the nominal value for the nominal speed
        interval = ticks_nominal;

        // Update laser - Cruising
        #if ENABLED(LASER_POWER_INLINE_TRAPEZOID)
          if (laser_trap.enabled) {
            if (!laser_trap.cruise_set) {
              laser_trap.cur_power = current_block->laser.power;
              cutter.set_ocr_power(laser_trap.cur_power);
              laser_trap.cruise_set = true;
            }
            #if ENABLED(LASER_POWER_INLINE_TRAPEZOID_CONT)
              laser_trap.till_update = LASER_POWER_INLINE_TRAPEZOID_CONT_PER;
            #else
              laser_trap.last_step_count = step_events_completed;
            #endif
          }
        #endif
      }
    }
  }

  // If there is no current block at this point, attempt to pop one from the buffer
  // and prepare its movement
  if (!current_block) {

    // Anything in the buffer?
    if ((current_block = planner.get_current_block())) {

      // Sync block? Sync the stepper counts or fan speeds and return
      while (current_block->flag & BLOCK_MASK_SYNC) {

        #if ENABLED(LASER_SYNCHRONOUS_M106_M107)
          const bool is_sync_fans = TEST(current_block->flag, BLOCK_BIT_SYNC_FANS);
          if (is_sync_fans) planner.sync_fan_speeds(current_block->fan_speed);
        #else
          constexpr bool is_sync_fans = false;
        #endif

        if (!is_sync_fans) _set_position(current_block->position);

        discard_current_block();

        // Try to get a new block
        if (!(current_block = planner.get_current_block()))
          return interval; // No more queued movements!
      }

      // For non-inline cutter, grossly apply power
      #if ENABLED(LASER_FEATURE) && DISABLED(LASER_POWER_INLINE)
        cutter.apply_power(current_block->cutter_power);
      #endif

      TERN_(POWER_LOSS_RECOVERY, recovery.info.sdpos = current_block->sdpos);

      #if ENABLED(DIRECT_STEPPING)
        if (IS_PAGE(current_block)) {
          page_step_state.segment_steps = 0;
          page_step_state.segment_idx = 0;
          page_step_state.page = page_manager.get_page(current_block->page_idx);
          page_step_state.bd.reset();

          if (DirectStepping::Config::DIRECTIONAL)
            current_block->direction_bits = last_direction_bits;

          if (!page_step_state.page) {
            discard_current_block();
            return interval;
          }
        }
      #endif

      // Flag all moving axes for proper endstop handling

      #if IS_CORE
        // Define conditions for checking endstops
        #define S_(N) current_block->steps[CORE_AXIS_##N]
        #define D_(N) TEST(current_block->direction_bits, CORE_AXIS_##N)
      #endif

      #if CORE_IS_XY || CORE_IS_XZ
        /**
         * Head direction in -X axis for CoreXY and CoreXZ bots.
         *
         * If steps differ, both axes are moving.
         * If DeltaA == -DeltaB, the movement is only in the 2nd axis (Y or Z, handled below)
         * If DeltaA ==  DeltaB, the movement is only in the 1st axis (X)
         */
        #if EITHER(COREXY, COREXZ)
          #define X_CMP(A,B) ((A)==(B))
        #else
          #define X_CMP(A,B) ((A)!=(B))
        #endif
        #define X_MOVE_TEST ( S_(1) != S_(2) || (S_(1) > 0 && X_CMP(D_(1),D_(2))) )
      #elif ENABLED(MARKFORGED_XY)
        #define X_MOVE_TEST (current_block->steps.a != current_block->steps.b)
      #else
        #define X_MOVE_TEST !!current_block->steps.a
      #endif

      #if CORE_IS_XY || CORE_IS_YZ
        /**
         * Head direction in -Y axis for CoreXY / CoreYZ bots.
         *
         * If steps differ, both axes are moving
         * If DeltaA ==  DeltaB, the movement is only in the 1st axis (X or Y)
         * If DeltaA == -DeltaB, the movement is only in the 2nd axis (Y or Z)
         */
        #if EITHER(COREYX, COREYZ)
          #define Y_CMP(A,B) ((A)==(B))
        #else
          #define Y_CMP(A,B) ((A)!=(B))
        #endif
        #define Y_MOVE_TEST ( S_(1) != S_(2) || (S_(1) > 0 && Y_CMP(D_(1),D_(2))) )
      #else
        #define Y_MOVE_TEST !!current_block->steps.b
      #endif

      #if CORE_IS_XZ || CORE_IS_YZ
        /**
         * Head direction in -Z axis for CoreXZ or CoreYZ bots.
         *
         * If steps differ, both axes are moving
         * If DeltaA ==  DeltaB, the movement is only in the 1st axis (X or Y, already handled above)
         * If DeltaA == -DeltaB, the movement is only in the 2nd axis (Z)
         */
        #if EITHER(COREZX, COREZY)
          #define Z_CMP(A,B) ((A)==(B))
        #else
          #define Z_CMP(A,B) ((A)!=(B))
        #endif
        #define Z_MOVE_TEST ( S_(1) != S_(2) || (S_(1) > 0 && Z_CMP(D_(1),D_(2))) )
      #else
        #define Z_MOVE_TEST !!current_block->steps.c
      #endif

      uint8_t axis_bits = 0;
      if (X_MOVE_TEST) SBI(axis_bits, A_AXIS);
      if (Y_MOVE_TEST) SBI(axis_bits, B_AXIS);
      if (Z_MOVE_TEST) SBI(axis_bits, C_AXIS);
      //if (!!current_block->steps.e) SBI(axis_bits, E_AXIS);
      //if (!!current_block->steps.a) SBI(axis_bits, X_HEAD);
      //if (!!current_block->steps.b) SBI(axis_bits, Y_HEAD);
      //if (!!current_block->steps.c) SBI(axis_bits, Z_HEAD);
      axis_did_move = axis_bits;

      // No acceleration / deceleration time elapsed so far
      acceleration_time = deceleration_time = 0;

      #if ENABLED(ADAPTIVE_STEP_SMOOTHING)
        uint8_t oversampling = 0;                           // Assume no axis smoothing (via oversampling)
        // Decide if axis smoothing is possible
        uint32_t max_rate = current_block->nominal_rate;    // Get the step event rate
        while (max_rate < MIN_STEP_ISR_FREQUENCY) {         // As long as more ISRs are possible...
          max_rate <<= 1;                                   // Try to double the rate
          if (max_rate < MIN_STEP_ISR_FREQUENCY)            // Don't exceed the estimated ISR limit
            ++oversampling;                                 // Increase the oversampling (used for left-shift)
        }
        oversampling_factor = oversampling;                 // For all timer interval calculations
      #else
        constexpr uint8_t oversampling = 0;
      #endif

      // Based on the oversampling factor, do the calculations
      step_event_count = current_block->step_event_count << oversampling;

      // Initialize Bresenham delta errors to 1/2
      delta_error = -int32_t(step_event_count);

      // Calculate Bresenham dividends and divisors
      advance_dividend = current_block->steps << 1;
      advance_divisor = step_event_count << 1;

      // No step events completed so far
      step_events_completed = 0;

      // Compute the acceleration and deceleration points
      accelerate_until = current_block->accelerate_until << oversampling;
      decelerate_after = current_block->decelerate_after << oversampling;

      TERN_(MIXING_EXTRUDER, mixer.stepper_setup(current_block->b_color))

      TERN_(HAS_MULTI_EXTRUDER, stepper_extruder = current_block->extruder);

      // Initialize the trapezoid generator from the current block.
      #if ENABLED(LIN_ADVANCE)
        #if DISABLED(MIXING_EXTRUDER) && E_STEPPERS > 1
          // If the now active extruder wasn't in use during the last move, its pressure is most likely gone.
          if (stepper_extruder != last_moved_extruder) LA_current_adv_steps = 0;
        #endif

        if ((LA_use_advance_lead = current_block->use_advance_lead)) {
          LA_final_adv_steps = current_block->final_adv_steps;
          LA_max_adv_steps = current_block->max_adv_steps;
          initiateLA(); // Start the ISR
          LA_isr_rate = current_block->advance_speed;
        }
        else LA_isr_rate = LA_ADV_NEVER;
      #endif

      if ( ENABLED(HAS_L64XX)       // Always set direction for L64xx (Also enables the chips)
        || ENABLED(DUAL_X_CARRIAGE) // TODO: Find out why this fixes "jittery" small circles
        || current_block->direction_bits != last_direction_bits
        || TERN(MIXING_EXTRUDER, false, stepper_extruder != last_moved_extruder)
      ) {
        TERN_(HAS_MULTI_EXTRUDER, last_moved_extruder = stepper_extruder);
        TERN_(HAS_L64XX, L64XX_OK_to_power_up = true);
        set_directions(current_block->direction_bits);
      }

      #if ENABLED(LASER_POWER_INLINE)
        const power_status_t stat = current_block->laser.status;
        #if ENABLED(LASER_POWER_INLINE_TRAPEZOID)
          laser_trap.enabled = stat.isPlanned && stat.isEnabled;
          laser_trap.cur_power = current_block->laser.power_entry; // RESET STATE
          laser_trap.cruise_set = false;
          #if DISABLED(LASER_POWER_INLINE_TRAPEZOID_CONT)
            laser_trap.last_step_count = 0;
            laser_trap.acc_step_count = current_block->laser.entry_per / 2;
          #else
            laser_trap.till_update = 0;
          #endif
          // Always have PWM in this case
          if (stat.isPlanned) {                        // Planner controls the laser
            cutter.set_ocr_power(
              stat.isEnabled ? laser_trap.cur_power : 0 // ON with power or OFF
            );
          }
        #else
          if (stat.isPlanned) {                        // Planner controls the laser
            #if ENABLED(SPINDLE_LASER_PWM)
              cutter.set_ocr_power(
                stat.isEnabled ? current_block->laser.power : 0 // ON with power or OFF
              );
            #else
              cutter.set_enabled(stat.isEnabled);
            #endif
          }
        #endif
      #endif // LASER_POWER_INLINE

      // At this point, we must ensure the movement about to execute isn't
      // trying to force the head against a limit switch. If using interrupt-
      // driven change detection, and already against a limit then no call to
      // the endstop_triggered method will be done and the movement will be
      // done against the endstop. So, check the limits here: If the movement
      // is against the limits, the block will be marked as to be killed, and
      // on the next call to this ISR, will be discarded.
      endstops.update();

      #if ENABLED(Z_LATE_ENABLE)
        // If delayed Z enable, enable it now. This option will severely interfere with
        // timing between pulses when chaining motion between blocks, and it could lead
        // to lost steps in both X and Y axis, so avoid using it unless strictly necessary!!
        if (current_block->steps.z) ENABLE_AXIS_Z();
      #endif

      // Mark the time_nominal as not calculated yet
      ticks_nominal = -1;

      #if ENABLED(S_CURVE_ACCELERATION)
        // Initialize the Bézier speed curve
        _calc_bezier_curve_coeffs(current_block->initial_rate, current_block->cruise_rate, current_block->acceleration_time_inverse);
        // We haven't started the 2nd half of the trapezoid
        bezier_2nd_half = false;
      #else
        // Set as deceleration point the initial rate of the block
        acc_step_rate = current_block->initial_rate;
      #endif

      // Calculate the initial timer interval
      interval = calc_timer_interval(current_block->initial_rate, &steps_per_isr);
    }
    #if ENABLED(LASER_POWER_INLINE_CONTINUOUS)
      else { // No new block found; so apply inline laser parameters
        // This should mean ending file with 'M5 I' will stop the laser; thus the inline flag isn't needed
        const power_status_t stat = planner.laser_inline.status;
        if (stat.isPlanned) {             // Planner controls the laser
          #if ENABLED(SPINDLE_LASER_PWM)
            cutter.set_ocr_power(
              stat.isEnabled ? planner.laser_inline.power : 0 // ON with power or OFF
            );
          #else
            cutter.set_enabled(stat.isEnabled);
          #endif
        }
      }
    #endif
  }

  // Return the interval to wait
  return interval;
}

#if ENABLED(LIN_ADVANCE)

  // Timer interrupt for E. LA_steps is set in the main routine
  uint32_t Stepper::advance_isr() {
    uint32_t interval;

    if (LA_use_advance_lead) {
      if (step_events_completed > decelerate_after && LA_current_adv_steps > LA_final_adv_steps) {
        LA_steps--;
        LA_current_adv_steps--;
        interval = LA_isr_rate;
      }
      else if (step_events_completed < decelerate_after && LA_current_adv_steps < LA_max_adv_steps) {
        LA_steps++;
        LA_current_adv_steps++;
        interval = LA_isr_rate;
      }
      else
        interval = LA_isr_rate = LA_ADV_NEVER;
    }
    else
      interval = LA_ADV_NEVER;

    if (!LA_steps) return interval; // Leave pins alone if there are no steps!

    DIR_WAIT_BEFORE();

    #if ENABLED(MIXING_EXTRUDER)
      // We don't know which steppers will be stepped because LA loop follows,
      // with potentially multiple steps. Set all.
      if (LA_steps > 0) {
        MIXER_STEPPER_LOOP(j) NORM_E_DIR(j);
        count_direction.e = 1;
      }
      else if (LA_steps < 0) {
        MIXER_STEPPER_LOOP(j) REV_E_DIR(j);
        count_direction.e = -1;
      }
    #else
      if (LA_steps > 0) {
        NORM_E_DIR(stepper_extruder);
        count_direction.e = 1;
      }
      else if (LA_steps < 0) {
        REV_E_DIR(stepper_extruder);
        count_direction.e = -1;
      }
    #endif

    DIR_WAIT_AFTER();

    //const hal_timer_t added_step_ticks = hal_timer_t(ADDED_STEP_TICKS);

    // Step E stepper if we have steps
    #if ISR_MULTI_STEPS
      bool firstStep = true;
      USING_TIMED_PULSE();
    #endif

    while (LA_steps) {
      #if ISR_MULTI_STEPS
        if (firstStep)
          firstStep = false;
        else
          AWAIT_LOW_PULSE();
      #endif

      count_position.e += count_direction.e;

      // Set the STEP pulse ON
      #if ENABLED(MIXING_EXTRUDER)
        E_STEP_WRITE(mixer.get_next_stepper(), !INVERT_E_STEP_PIN);
      #else
        E_STEP_WRITE(stepper_extruder, !INVERT_E_STEP_PIN);
      #endif

      // Enforce a minimum duration for STEP pulse ON
      #if ISR_PULSE_CONTROL
        START_HIGH_PULSE();
      #endif

      LA_steps < 0 ? ++LA_steps : --LA_steps;

      #if ISR_PULSE_CONTROL
        AWAIT_HIGH_PULSE();
      #endif

      // Set the STEP pulse OFF
      #if ENABLED(MIXING_EXTRUDER)
        E_STEP_WRITE(mixer.get_stepper(), INVERT_E_STEP_PIN);
      #else
        E_STEP_WRITE(stepper_extruder, INVERT_E_STEP_PIN);
      #endif

      // For minimum pulse time wait before looping
      // Just wait for the requested pulse duration
      #if ISR_PULSE_CONTROL
        if (LA_steps) START_LOW_PULSE();
      #endif
    } // LA_steps

    return interval;
  }

#endif // LIN_ADVANCE

#if ENABLED(INTEGRATED_BABYSTEPPING)

  // Timer interrupt for baby-stepping
  uint32_t Stepper::babystepping_isr() {
    babystep.task();
    return babystep.has_steps() ? BABYSTEP_TICKS : BABYSTEP_NEVER;
  }

#endif

// Check if the given block is busy or not - Must not be called from ISR contexts
// The current_block could change in the middle of the read by an Stepper ISR, so
// we must explicitly prevent that!
bool Stepper::is_block_busy(const block_t * const block) {
  #ifdef __AVR__
    // A SW memory barrier, to ensure GCC does not overoptimize loops
    #define sw_barrier() asm volatile("": : :"memory");

    // Keep reading until 2 consecutive reads return the same value,
    // meaning there was no update in-between caused by an interrupt.
    // This works because stepper ISRs happen at a slower rate than
    // successive reads of a variable, so 2 consecutive reads with
    // the same value means no interrupt updated it.
    block_t *vold, *vnew = current_block;
    sw_barrier();
    do {
      vold = vnew;
      vnew = current_block;
      sw_barrier();
    } while (vold != vnew);
  #else
    block_t *vnew = current_block;
  #endif

  // Return if the block is busy or not
  return block == vnew;
}

void Stepper::init() {

  #if MB(ALLIGATOR)
    const float motor_current[] = MOTOR_CURRENT;
    unsigned int digipot_motor = 0;
    LOOP_L_N(i, 3 + EXTRUDERS) {
      digipot_motor = 255 * (motor_current[i] / 2.5);
      dac084s085::setValue(i, digipot_motor);
    }
  #endif

  // Init Microstepping Pins
  TERN_(HAS_MICROSTEPS, microstep_init());

  // Init Dir Pins
  TERN_(HAS_X_DIR, X_DIR_INIT());
  TERN_(HAS_X2_DIR, X2_DIR_INIT());
  #if HAS_Y_DIR
    Y_DIR_INIT();
    #if BOTH(Y_DUAL_STEPPER_DRIVERS, HAS_Y2_DIR)
      Y2_DIR_INIT();
    #endif
  #endif
  #if HAS_Z_DIR
    Z_DIR_INIT();
    #if NUM_Z_STEPPER_DRIVERS >= 2 && HAS_Z2_DIR
      Z2_DIR_INIT();
    #endif
    #if NUM_Z_STEPPER_DRIVERS >= 3 && HAS_Z3_DIR
      Z3_DIR_INIT();
    #endif
    #if NUM_Z_STEPPER_DRIVERS >= 4 && HAS_Z4_DIR
      Z4_DIR_INIT();
    #endif
  #endif
  #if HAS_E0_DIR
    E0_DIR_INIT();
  #endif
  #if HAS_E1_DIR
    E1_DIR_INIT();
  #endif
  #if HAS_E2_DIR
    E2_DIR_INIT();
  #endif
  #if HAS_E3_DIR
    E3_DIR_INIT();
  #endif
  #if HAS_E4_DIR
    E4_DIR_INIT();
  #endif
  #if HAS_E5_DIR
    E5_DIR_INIT();
  #endif
  #if HAS_E6_DIR
    E6_DIR_INIT();
  #endif
  #if HAS_E7_DIR
    E7_DIR_INIT();
  #endif

  // Init Enable Pins - steppers default to disabled.
  #if HAS_X_ENABLE
    X_ENABLE_INIT();
    if (!X_ENABLE_ON) X_ENABLE_WRITE(HIGH);
    #if EITHER(DUAL_X_CARRIAGE, X_DUAL_STEPPER_DRIVERS) && HAS_X2_ENABLE
      X2_ENABLE_INIT();
      if (!X_ENABLE_ON) X2_ENABLE_WRITE(HIGH);
    #endif
  #endif
  #if HAS_Y_ENABLE
    Y_ENABLE_INIT();
    if (!Y_ENABLE_ON) Y_ENABLE_WRITE(HIGH);
    #if BOTH(Y_DUAL_STEPPER_DRIVERS, HAS_Y2_ENABLE)
      Y2_ENABLE_INIT();
      if (!Y_ENABLE_ON) Y2_ENABLE_WRITE(HIGH);
    #endif
  #endif
  #if HAS_Z_ENABLE
    Z_ENABLE_INIT();
    if (!Z_ENABLE_ON) Z_ENABLE_WRITE(HIGH);
    #if NUM_Z_STEPPER_DRIVERS >= 2 && HAS_Z2_ENABLE
      Z2_ENABLE_INIT();
      if (!Z_ENABLE_ON) Z2_ENABLE_WRITE(HIGH);
    #endif
    #if NUM_Z_STEPPER_DRIVERS >= 3 && HAS_Z3_ENABLE
      Z3_ENABLE_INIT();
      if (!Z_ENABLE_ON) Z3_ENABLE_WRITE(HIGH);
    #endif
    #if NUM_Z_STEPPER_DRIVERS >= 4 && HAS_Z4_ENABLE
      Z4_ENABLE_INIT();
      if (!Z_ENABLE_ON) Z4_ENABLE_WRITE(HIGH);
    #endif
  #endif
  #if HAS_E0_ENABLE
    E0_ENABLE_INIT();
    if (!E_ENABLE_ON) E0_ENABLE_WRITE(HIGH);
  #endif
  #if HAS_E1_ENABLE
    E1_ENABLE_INIT();
    if (!E_ENABLE_ON) E1_ENABLE_WRITE(HIGH);
  #endif
  #if HAS_E2_ENABLE
    E2_ENABLE_INIT();
    if (!E_ENABLE_ON) E2_ENABLE_WRITE(HIGH);
  #endif
  #if HAS_E3_ENABLE
    E3_ENABLE_INIT();
    if (!E_ENABLE_ON) E3_ENABLE_WRITE(HIGH);
  #endif
  #if HAS_E4_ENABLE
    E4_ENABLE_INIT();
    if (!E_ENABLE_ON) E4_ENABLE_WRITE(HIGH);
  #endif
  #if HAS_E5_ENABLE
    E5_ENABLE_INIT();
    if (!E_ENABLE_ON) E5_ENABLE_WRITE(HIGH);
  #endif
  #if HAS_E6_ENABLE
    E6_ENABLE_INIT();
    if (!E_ENABLE_ON) E6_ENABLE_WRITE(HIGH);
  #endif
  #if HAS_E7_ENABLE
    E7_ENABLE_INIT();
    if (!E_ENABLE_ON) E7_ENABLE_WRITE(HIGH);
  #endif

  #define _STEP_INIT(AXIS) AXIS ##_STEP_INIT()
  #define _WRITE_STEP(AXIS, HIGHLOW) AXIS ##_STEP_WRITE(HIGHLOW)
  #define _DISABLE_AXIS(AXIS) DISABLE_AXIS_## AXIS()

  #define AXIS_INIT(AXIS, PIN) \
    _STEP_INIT(AXIS); \
    _WRITE_STEP(AXIS, _INVERT_STEP_PIN(PIN)); \
    _DISABLE_AXIS(AXIS)

  #define E_AXIS_INIT(NUM) AXIS_INIT(E## NUM, E)

  // Init Step Pins
  #if HAS_X_STEP
    #if EITHER(X_DUAL_STEPPER_DRIVERS, DUAL_X_CARRIAGE)
      X2_STEP_INIT();
      X2_STEP_WRITE(INVERT_X_STEP_PIN);
    #endif
    AXIS_INIT(X, X);
  #endif

  #if HAS_Y_STEP
    #if ENABLED(Y_DUAL_STEPPER_DRIVERS)
      Y2_STEP_INIT();
      Y2_STEP_WRITE(INVERT_Y_STEP_PIN);
    #endif
    AXIS_INIT(Y, Y);
  #endif

  #if HAS_Z_STEP
    #if NUM_Z_STEPPER_DRIVERS >= 2
      Z2_STEP_INIT();
      Z2_STEP_WRITE(INVERT_Z_STEP_PIN);
    #endif
    #if NUM_Z_STEPPER_DRIVERS >= 3
      Z3_STEP_INIT();
      Z3_STEP_WRITE(INVERT_Z_STEP_PIN);
    #endif
    #if NUM_Z_STEPPER_DRIVERS >= 4
      Z4_STEP_INIT();
      Z4_STEP_WRITE(INVERT_Z_STEP_PIN);
    #endif
    AXIS_INIT(Z, Z);
  #endif

  #if E_STEPPERS && HAS_E0_STEP
    E_AXIS_INIT(0);
  #endif
  #if E_STEPPERS > 1 && HAS_E1_STEP
    E_AXIS_INIT(1);
  #endif
  #if E_STEPPERS > 2 && HAS_E2_STEP
    E_AXIS_INIT(2);
  #endif
  #if E_STEPPERS > 3 && HAS_E3_STEP
    E_AXIS_INIT(3);
  #endif
  #if E_STEPPERS > 4 && HAS_E4_STEP
    E_AXIS_INIT(4);
  #endif
  #if E_STEPPERS > 5 && HAS_E5_STEP
    E_AXIS_INIT(5);
  #endif
  #if E_STEPPERS > 6 && HAS_E6_STEP
    E_AXIS_INIT(6);
  #endif
  #if E_STEPPERS > 7 && HAS_E7_STEP
    E_AXIS_INIT(7);
  #endif

  #if DISABLED(I2S_STEPPER_STREAM)
    HAL_timer_start(STEP_TIMER_NUM, 122); // Init Stepper ISR to 122 Hz for quick starting
    wake_up();
    sei();
  #endif

  // Init direction bits for first moves
  set_directions((INVERT_X_DIR ? _BV(X_AXIS) : 0)
               | (INVERT_Y_DIR ? _BV(Y_AXIS) : 0)
               | (INVERT_Z_DIR ? _BV(Z_AXIS) : 0));

  #if HAS_MOTOR_CURRENT_SPI || HAS_MOTOR_CURRENT_PWM
    initialized = true;
    digipot_init();
  #endif
}

/**
 * Set the stepper positions directly in steps
 *
 * The input is based on the typical per-axis XYZ steps.
 * For CORE machines XYZ needs to be translated to ABC.
 *
 * This allows get_axis_position_mm to correctly
 * derive the current XYZ position later on.
 */
void Stepper::_set_position(const int32_t &a, const int32_t &b, const int32_t &c, const int32_t &e) {
  #if CORE_IS_XY
    // corexy positioning
    // these equations follow the form of the dA and dB equations on https://www.corexy.com/theory.html
    count_position.set(a + b, CORESIGN(a - b), c);
  #elif CORE_IS_XZ
    // corexz planning
    count_position.set(a + c, b, CORESIGN(a - c));
  #elif CORE_IS_YZ
    // coreyz planning
    count_position.set(a, b + c, CORESIGN(b - c));
  #elif ENABLED(MARKFORGED_XY)
    count_position.set(a - b, b, c);
  #else
    // default non-h-bot planning
    count_position.set(a, b, c);
  #endif
  count_position.e = e;
}

/**
 * Get a stepper's position in steps.
 */
int32_t Stepper::position(const AxisEnum axis) {
  #ifdef __AVR__
    // Protect the access to the position. Only required for AVR, as
    //  any 32bit CPU offers atomic access to 32bit variables
    const bool was_enabled = suspend();
  #endif

  const int32_t v = count_position[axis];

  #ifdef __AVR__
    // Reenable Stepper ISR
    if (was_enabled) wake_up();
  #endif
  return v;
}

// Set the current position in steps
void Stepper::set_position(const int32_t &a, const int32_t &b, const int32_t &c, const int32_t &e) {
  planner.synchronize();
  const bool was_enabled = suspend();
  _set_position(a, b, c, e);
  if (was_enabled) wake_up();
}

void Stepper::set_axis_position(const AxisEnum a, const int32_t &v) {
  planner.synchronize();

  #ifdef __AVR__
    // Protect the access to the position. Only required for AVR, as
    //  any 32bit CPU offers atomic access to 32bit variables
    const bool was_enabled = suspend();
  #endif

  count_position[a] = v;

  #ifdef __AVR__
    // Reenable Stepper ISR
    if (was_enabled) wake_up();
  #endif
}

// Signal endstops were triggered - This function can be called from
// an ISR context  (Temperature, Stepper or limits ISR), so we must
// be very careful here. If the interrupt being preempted was the
// Stepper ISR (this CAN happen with the endstop limits ISR) then
// when the stepper ISR resumes, we must be very sure that the movement
// is properly canceled
void Stepper::endstop_triggered(const AxisEnum axis) {

  const bool was_enabled = suspend();
  endstops_trigsteps[axis] = (
    #if IS_CORE
      (axis == CORE_AXIS_2
        ? CORESIGN(count_position[CORE_AXIS_1] - count_position[CORE_AXIS_2])
        : count_position[CORE_AXIS_1] + count_position[CORE_AXIS_2]
      ) * double(0.5)
    #elif ENABLED(MARKFORGED_XY)
      axis == CORE_AXIS_1
        ? count_position[CORE_AXIS_1] - count_position[CORE_AXIS_2]
        : count_position[CORE_AXIS_2]
    #else // !IS_CORE
      count_position[axis]
    #endif
  );

  // Discard the rest of the move if there is a current block
  quick_stop();

  if (was_enabled) wake_up();
}

int32_t Stepper::triggered_position(const AxisEnum axis) {
  #ifdef __AVR__
    // Protect the access to the position. Only required for AVR, as
    //  any 32bit CPU offers atomic access to 32bit variables
    const bool was_enabled = suspend();
  #endif

  const int32_t v = endstops_trigsteps[axis];

  #ifdef __AVR__
    // Reenable Stepper ISR
    if (was_enabled) wake_up();
  #endif

  return v;
}

void Stepper::report_a_position(const xyz_long_t &pos) {
  #if ANY(CORE_IS_XY, CORE_IS_XZ, MARKFORGED_XY, DELTA, IS_SCARA)
    SERIAL_ECHOPAIR(STR_COUNT_A, pos.x, " B:", pos.y);
  #else
    SERIAL_ECHOPAIR_P(PSTR(STR_COUNT_X), pos.x, SP_Y_LBL, pos.y);
  #endif
  #if ANY(CORE_IS_XZ, CORE_IS_YZ, DELTA)
    SERIAL_ECHOLNPAIR(" C:", pos.z);
  #else
    SERIAL_ECHOLNPAIR_P(SP_Z_LBL, pos.z);
  #endif
}

void Stepper::report_positions() {

  #ifdef __AVR__
    // Protect the access to the position.
    const bool was_enabled = suspend();
  #endif

  const xyz_long_t pos = count_position;

  #ifdef __AVR__
    if (was_enabled) wake_up();
  #endif

  report_a_position(pos);
}

#if ENABLED(BABYSTEPPING)

  #define _ENABLE_AXIS(AXIS) ENABLE_AXIS_## AXIS()
  #define _READ_DIR(AXIS) AXIS ##_DIR_READ()
  #define _INVERT_DIR(AXIS) INVERT_## AXIS ##_DIR
  #define _APPLY_DIR(AXIS, INVERT) AXIS ##_APPLY_DIR(INVERT, true)

  #if MINIMUM_STEPPER_PULSE
    #define STEP_PULSE_CYCLES ((MINIMUM_STEPPER_PULSE) * CYCLES_PER_MICROSECOND)
  #else
    #define STEP_PULSE_CYCLES 0
  #endif

  #if ENABLED(DELTA)
    #define CYCLES_EATEN_BABYSTEP (2 * 15)
  #else
    #define CYCLES_EATEN_BABYSTEP 0
  #endif
  #define EXTRA_CYCLES_BABYSTEP (STEP_PULSE_CYCLES - (CYCLES_EATEN_BABYSTEP))

  #if EXTRA_CYCLES_BABYSTEP > 20
    #define _SAVE_START() const hal_timer_t pulse_start = HAL_timer_get_count(PULSE_TIMER_NUM)
    #define _PULSE_WAIT() while (EXTRA_CYCLES_BABYSTEP > (uint32_t)(HAL_timer_get_count(PULSE_TIMER_NUM) - pulse_start) * (PULSE_TIMER_PRESCALE)) { /* nada */ }
  #else
    #define _SAVE_START() NOOP
    #if EXTRA_CYCLES_BABYSTEP > 0
      #define _PULSE_WAIT() DELAY_NS(EXTRA_CYCLES_BABYSTEP * NANOSECONDS_PER_CYCLE)
    #elif ENABLED(DELTA)
      #define _PULSE_WAIT() DELAY_US(2);
    #elif STEP_PULSE_CYCLES > 0
      #define _PULSE_WAIT() NOOP
    #else
      #define _PULSE_WAIT() DELAY_US(4);
    #endif
  #endif

  #if ENABLED(BABYSTEPPING_EXTRA_DIR_WAIT)
    #define EXTRA_DIR_WAIT_BEFORE DIR_WAIT_BEFORE
    #define EXTRA_DIR_WAIT_AFTER  DIR_WAIT_AFTER
  #else
    #define EXTRA_DIR_WAIT_BEFORE()
    #define EXTRA_DIR_WAIT_AFTER()
  #endif

  #if DISABLED(DELTA)

    #define BABYSTEP_AXIS(AXIS, INV, DIR) do{           \
      const uint8_t old_dir = _READ_DIR(AXIS);          \
      _ENABLE_AXIS(AXIS);                               \
      DIR_WAIT_BEFORE();                                \
      _APPLY_DIR(AXIS, _INVERT_DIR(AXIS)^DIR^INV);      \
      DIR_WAIT_AFTER();                                 \
      _SAVE_START();                                    \
      _APPLY_STEP(AXIS, !_INVERT_STEP_PIN(AXIS), true); \
      _PULSE_WAIT();                                    \
      _APPLY_STEP(AXIS, _INVERT_STEP_PIN(AXIS), true);  \
      EXTRA_DIR_WAIT_BEFORE();                          \
      _APPLY_DIR(AXIS, old_dir);                        \
      EXTRA_DIR_WAIT_AFTER();                           \
    }while(0)

  #endif

  #if IS_CORE

    #define BABYSTEP_CORE(A, B, INV, DIR, ALT) do{              \
      const xy_byte_t old_dir = { _READ_DIR(A), _READ_DIR(B) }; \
      _ENABLE_AXIS(A); _ENABLE_AXIS(B);                         \
      DIR_WAIT_BEFORE();                                        \
      _APPLY_DIR(A, _INVERT_DIR(A)^DIR^INV);                    \
      _APPLY_DIR(B, _INVERT_DIR(B)^DIR^INV^ALT);                \
      DIR_WAIT_AFTER();                                         \
      _SAVE_START();                                            \
      _APPLY_STEP(A, !_INVERT_STEP_PIN(A), true);               \
      _APPLY_STEP(B, !_INVERT_STEP_PIN(B), true);               \
      _PULSE_WAIT();                                            \
      _APPLY_STEP(A, _INVERT_STEP_PIN(A), true);                \
      _APPLY_STEP(B, _INVERT_STEP_PIN(B), true);                \
      EXTRA_DIR_WAIT_BEFORE();                                  \
      _APPLY_DIR(A, old_dir.a); _APPLY_DIR(B, old_dir.b);       \
      EXTRA_DIR_WAIT_AFTER();                                   \
    }while(0)

  #endif

  // MUST ONLY BE CALLED BY AN ISR,
  // No other ISR should ever interrupt this!
  void Stepper::do_babystep(const AxisEnum axis, const bool direction) {

    #if DISABLED(INTEGRATED_BABYSTEPPING)
      cli();
    #endif

    switch (axis) {

      #if ENABLED(BABYSTEP_XY)

        case X_AXIS:
          #if CORE_IS_XY
            BABYSTEP_CORE(X, Y, 0, direction, 0);
          #elif CORE_IS_XZ
            BABYSTEP_CORE(X, Z, 0, direction, 0);
          #else
            BABYSTEP_AXIS(X, 0, direction);
          #endif
          break;

        case Y_AXIS:
          #if CORE_IS_XY
            BABYSTEP_CORE(X, Y, 1, !direction, (CORESIGN(1)>0));
          #elif CORE_IS_YZ
            BABYSTEP_CORE(Y, Z, 0, direction, (CORESIGN(1)<0));
          #else
            BABYSTEP_AXIS(Y, 0, direction);
          #endif
          break;

      #endif

      case Z_AXIS: {

        #if CORE_IS_XZ
          BABYSTEP_CORE(X, Z, BABYSTEP_INVERT_Z, direction, (CORESIGN(1)<0));
        #elif CORE_IS_YZ
          BABYSTEP_CORE(Y, Z, BABYSTEP_INVERT_Z, direction, (CORESIGN(1)<0));
        #elif DISABLED(DELTA)
          BABYSTEP_AXIS(Z, BABYSTEP_INVERT_Z, direction);

        #else // DELTA

          const bool z_direction = direction ^ BABYSTEP_INVERT_Z;

          ENABLE_AXIS_X();
          ENABLE_AXIS_Y();
          ENABLE_AXIS_Z();

          DIR_WAIT_BEFORE();

          const xyz_byte_t old_dir = { X_DIR_READ(), Y_DIR_READ(), Z_DIR_READ() };

          X_DIR_WRITE(INVERT_X_DIR ^ z_direction);
          Y_DIR_WRITE(INVERT_Y_DIR ^ z_direction);
          Z_DIR_WRITE(INVERT_Z_DIR ^ z_direction);

          DIR_WAIT_AFTER();

          _SAVE_START();

          X_STEP_WRITE(!INVERT_X_STEP_PIN);
          Y_STEP_WRITE(!INVERT_Y_STEP_PIN);
          Z_STEP_WRITE(!INVERT_Z_STEP_PIN);

          _PULSE_WAIT();

          X_STEP_WRITE(INVERT_X_STEP_PIN);
          Y_STEP_WRITE(INVERT_Y_STEP_PIN);
          Z_STEP_WRITE(INVERT_Z_STEP_PIN);

          // Restore direction bits
          EXTRA_DIR_WAIT_BEFORE();

          X_DIR_WRITE(old_dir.x);
          Y_DIR_WRITE(old_dir.y);
          Z_DIR_WRITE(old_dir.z);

          EXTRA_DIR_WAIT_AFTER();

        #endif

      } break;

      default: break;
    }

    #if DISABLED(INTEGRATED_BABYSTEPPING)
      sei();
    #endif
  }

#endif // BABYSTEPPING

/**
 * Software-controlled Stepper Motor Current
 */

#if HAS_MOTOR_CURRENT_SPI

  // From Arduino DigitalPotControl example
  void Stepper::set_digipot_value_spi(const int16_t address, const int16_t value) {
    WRITE(DIGIPOTSS_PIN, LOW);  // Take the SS pin low to select the chip
    SPI.transfer(address);      // Send the address and value via SPI
    SPI.transfer(value);
    WRITE(DIGIPOTSS_PIN, HIGH); // Take the SS pin high to de-select the chip
    //delay(10);
  }

#endif // HAS_MOTOR_CURRENT_SPI

#if HAS_MOTOR_CURRENT_PWM

  void Stepper::refresh_motor_power() {
    if (!initialized) return;
    LOOP_L_N(i, COUNT(motor_current_setting)) {
      switch (i) {
        #if ANY_PIN(MOTOR_CURRENT_PWM_XY, MOTOR_CURRENT_PWM_X, MOTOR_CURRENT_PWM_Y)
          case 0:
        #endif
        #if PIN_EXISTS(MOTOR_CURRENT_PWM_Z)
          case 1:
        #endif
        #if ANY_PIN(MOTOR_CURRENT_PWM_E, MOTOR_CURRENT_PWM_E0, MOTOR_CURRENT_PWM_E1)
          case 2:
        #endif
            set_digipot_current(i, motor_current_setting[i]);
        default: break;
      }
    }
  }

#endif // HAS_MOTOR_CURRENT_PWM

#if !MB(PRINTRBOARD_G2)

  #if HAS_MOTOR_CURRENT_SPI || HAS_MOTOR_CURRENT_PWM

    void Stepper::set_digipot_current(const uint8_t driver, const int16_t current) {
      if (WITHIN(driver, 0, MOTOR_CURRENT_COUNT - 1))
        motor_current_setting[driver] = current; // update motor_current_setting

      if (!initialized) return;

      #if HAS_MOTOR_CURRENT_SPI

        //SERIAL_ECHOLNPAIR("Digipotss current ", current);

        const uint8_t digipot_ch[] = DIGIPOT_CHANNELS;
        set_digipot_value_spi(digipot_ch[driver], current);

      #elif HAS_MOTOR_CURRENT_PWM

        #define _WRITE_CURRENT_PWM(P) analogWrite(pin_t(MOTOR_CURRENT_PWM_## P ##_PIN), 255L * current / (MOTOR_CURRENT_PWM_RANGE))
        switch (driver) {
          case 0:
            #if PIN_EXISTS(MOTOR_CURRENT_PWM_X)
              _WRITE_CURRENT_PWM(X);
            #endif
            #if PIN_EXISTS(MOTOR_CURRENT_PWM_Y)
              _WRITE_CURRENT_PWM(Y);
            #endif
            #if PIN_EXISTS(MOTOR_CURRENT_PWM_XY)
              _WRITE_CURRENT_PWM(XY);
            #endif
            break;
          case 1:
            #if PIN_EXISTS(MOTOR_CURRENT_PWM_Z)
              _WRITE_CURRENT_PWM(Z);
            #endif
            break;
          case 2:
            #if PIN_EXISTS(MOTOR_CURRENT_PWM_E)
              _WRITE_CURRENT_PWM(E);
            #endif
            #if PIN_EXISTS(MOTOR_CURRENT_PWM_E0)
              _WRITE_CURRENT_PWM(E0);
            #endif
            #if PIN_EXISTS(MOTOR_CURRENT_PWM_E1)
              _WRITE_CURRENT_PWM(E1);
            #endif
            break;
        }
      #endif
    }

    void Stepper::digipot_init() {

      #if HAS_MOTOR_CURRENT_SPI

        SPI.begin();
        SET_OUTPUT(DIGIPOTSS_PIN);

        LOOP_L_N(i, COUNT(motor_current_setting))
          set_digipot_current(i, motor_current_setting[i]);

      #elif HAS_MOTOR_CURRENT_PWM

        #if PIN_EXISTS(MOTOR_CURRENT_PWM_X)
          SET_PWM(MOTOR_CURRENT_PWM_X_PIN);
        #endif
        #if PIN_EXISTS(MOTOR_CURRENT_PWM_Y)
          SET_PWM(MOTOR_CURRENT_PWM_Y_PIN);
        #endif
        #if PIN_EXISTS(MOTOR_CURRENT_PWM_XY)
          SET_PWM(MOTOR_CURRENT_PWM_XY_PIN);
        #endif
        #if PIN_EXISTS(MOTOR_CURRENT_PWM_Z)
          SET_PWM(MOTOR_CURRENT_PWM_Z_PIN);
        #endif
        #if PIN_EXISTS(MOTOR_CURRENT_PWM_E)
          SET_PWM(MOTOR_CURRENT_PWM_E_PIN);
        #endif
        #if PIN_EXISTS(MOTOR_CURRENT_PWM_E0)
          SET_PWM(MOTOR_CURRENT_PWM_E0_PIN);
        #endif
        #if PIN_EXISTS(MOTOR_CURRENT_PWM_E1)
          SET_PWM(MOTOR_CURRENT_PWM_E1_PIN);
        #endif

        refresh_motor_power();

        // Set Timer5 to 31khz so the PWM of the motor power is as constant as possible. (removes a buzzing noise)
        #ifdef __AVR__
          SET_CS5(PRESCALER_1);
        #endif
      #endif
    }

  #endif

#else // PRINTRBOARD_G2

  #include HAL_PATH(../HAL, fastio/G2_PWM.h)

#endif

#if HAS_MICROSTEPS

  /**
   * Software-controlled Microstepping
   */

  void Stepper::microstep_init() {
    #if HAS_X_MS_PINS
      SET_OUTPUT(X_MS1_PIN);
      SET_OUTPUT(X_MS2_PIN);
      #if PIN_EXISTS(X_MS3)
        SET_OUTPUT(X_MS3_PIN);
      #endif
    #endif
    #if HAS_X2_MS_PINS
      SET_OUTPUT(X2_MS1_PIN);
      SET_OUTPUT(X2_MS2_PIN);
      #if PIN_EXISTS(X2_MS3)
        SET_OUTPUT(X2_MS3_PIN);
      #endif
    #endif
    #if HAS_Y_MS_PINS
      SET_OUTPUT(Y_MS1_PIN);
      SET_OUTPUT(Y_MS2_PIN);
      #if PIN_EXISTS(Y_MS3)
        SET_OUTPUT(Y_MS3_PIN);
      #endif
    #endif
    #if HAS_Y2_MS_PINS
      SET_OUTPUT(Y2_MS1_PIN);
      SET_OUTPUT(Y2_MS2_PIN);
      #if PIN_EXISTS(Y2_MS3)
        SET_OUTPUT(Y2_MS3_PIN);
      #endif
    #endif
    #if HAS_Z_MS_PINS
      SET_OUTPUT(Z_MS1_PIN);
      SET_OUTPUT(Z_MS2_PIN);
      #if PIN_EXISTS(Z_MS3)
        SET_OUTPUT(Z_MS3_PIN);
      #endif
    #endif
    #if HAS_Z2_MS_PINS
      SET_OUTPUT(Z2_MS1_PIN);
      SET_OUTPUT(Z2_MS2_PIN);
      #if PIN_EXISTS(Z2_MS3)
        SET_OUTPUT(Z2_MS3_PIN);
      #endif
    #endif
    #if HAS_Z3_MS_PINS
      SET_OUTPUT(Z3_MS1_PIN);
      SET_OUTPUT(Z3_MS2_PIN);
      #if PIN_EXISTS(Z3_MS3)
        SET_OUTPUT(Z3_MS3_PIN);
      #endif
    #endif
    #if HAS_Z4_MS_PINS
      SET_OUTPUT(Z4_MS1_PIN);
      SET_OUTPUT(Z4_MS2_PIN);
      #if PIN_EXISTS(Z4_MS3)
        SET_OUTPUT(Z4_MS3_PIN);
      #endif
    #endif
    #if HAS_E0_MS_PINS
      SET_OUTPUT(E0_MS1_PIN);
      SET_OUTPUT(E0_MS2_PIN);
      #if PIN_EXISTS(E0_MS3)
        SET_OUTPUT(E0_MS3_PIN);
      #endif
    #endif
    #if HAS_E1_MS_PINS
      SET_OUTPUT(E1_MS1_PIN);
      SET_OUTPUT(E1_MS2_PIN);
      #if PIN_EXISTS(E1_MS3)
        SET_OUTPUT(E1_MS3_PIN);
      #endif
    #endif
    #if HAS_E2_MS_PINS
      SET_OUTPUT(E2_MS1_PIN);
      SET_OUTPUT(E2_MS2_PIN);
      #if PIN_EXISTS(E2_MS3)
        SET_OUTPUT(E2_MS3_PIN);
      #endif
    #endif
    #if HAS_E3_MS_PINS
      SET_OUTPUT(E3_MS1_PIN);
      SET_OUTPUT(E3_MS2_PIN);
      #if PIN_EXISTS(E3_MS3)
        SET_OUTPUT(E3_MS3_PIN);
      #endif
    #endif
    #if HAS_E4_MS_PINS
      SET_OUTPUT(E4_MS1_PIN);
      SET_OUTPUT(E4_MS2_PIN);
      #if PIN_EXISTS(E4_MS3)
        SET_OUTPUT(E4_MS3_PIN);
      #endif
    #endif
    #if HAS_E5_MS_PINS
      SET_OUTPUT(E5_MS1_PIN);
      SET_OUTPUT(E5_MS2_PIN);
      #if PIN_EXISTS(E5_MS3)
        SET_OUTPUT(E5_MS3_PIN);
      #endif
    #endif
    #if HAS_E6_MS_PINS
      SET_OUTPUT(E6_MS1_PIN);
      SET_OUTPUT(E6_MS2_PIN);
      #if PIN_EXISTS(E6_MS3)
        SET_OUTPUT(E6_MS3_PIN);
      #endif
    #endif
    #if HAS_E7_MS_PINS
      SET_OUTPUT(E7_MS1_PIN);
      SET_OUTPUT(E7_MS2_PIN);
      #if PIN_EXISTS(E7_MS3)
        SET_OUTPUT(E7_MS3_PIN);
      #endif
    #endif

    static const uint8_t microstep_modes[] = MICROSTEP_MODES;
    for (uint16_t i = 0; i < COUNT(microstep_modes); i++)
      microstep_mode(i, microstep_modes[i]);
  }

  void Stepper::microstep_ms(const uint8_t driver, const int8_t ms1, const int8_t ms2, const int8_t ms3) {
    if (ms1 >= 0) switch (driver) {
      #if HAS_X_MS_PINS || HAS_X2_MS_PINS
        case 0:
          #if HAS_X_MS_PINS
            WRITE(X_MS1_PIN, ms1);
          #endif
          #if HAS_X2_MS_PINS
            WRITE(X2_MS1_PIN, ms1);
          #endif
          break;
      #endif
      #if HAS_Y_MS_PINS || HAS_Y2_MS_PINS
        case 1:
          #if HAS_Y_MS_PINS
            WRITE(Y_MS1_PIN, ms1);
          #endif
          #if HAS_Y2_MS_PINS
            WRITE(Y2_MS1_PIN, ms1);
          #endif
          break;
      #endif
      #if HAS_SOME_Z_MS_PINS
        case 2:
          #if HAS_Z_MS_PINS
            WRITE(Z_MS1_PIN, ms1);
          #endif
          #if HAS_Z2_MS_PINS
            WRITE(Z2_MS1_PIN, ms1);
          #endif
          #if HAS_Z3_MS_PINS
            WRITE(Z3_MS1_PIN, ms1);
          #endif
          #if HAS_Z4_MS_PINS
            WRITE(Z4_MS1_PIN, ms1);
          #endif
          break;
      #endif
      #if HAS_E0_MS_PINS
        case  3: WRITE(E0_MS1_PIN, ms1); break;
      #endif
      #if HAS_E1_MS_PINS
        case  4: WRITE(E1_MS1_PIN, ms1); break;
      #endif
      #if HAS_E2_MS_PINS
        case  5: WRITE(E2_MS1_PIN, ms1); break;
      #endif
      #if HAS_E3_MS_PINS
        case  6: WRITE(E3_MS1_PIN, ms1); break;
      #endif
      #if HAS_E4_MS_PINS
        case  7: WRITE(E4_MS1_PIN, ms1); break;
      #endif
      #if HAS_E5_MS_PINS
        case  8: WRITE(E5_MS1_PIN, ms1); break;
      #endif
      #if HAS_E6_MS_PINS
        case  9: WRITE(E6_MS1_PIN, ms1); break;
      #endif
      #if HAS_E7_MS_PINS
        case 10: WRITE(E7_MS1_PIN, ms1); break;
      #endif
    }
    if (ms2 >= 0) switch (driver) {
      #if HAS_X_MS_PINS || HAS_X2_MS_PINS
        case 0:
          #if HAS_X_MS_PINS
            WRITE(X_MS2_PIN, ms2);
          #endif
          #if HAS_X2_MS_PINS
            WRITE(X2_MS2_PIN, ms2);
          #endif
          break;
      #endif
      #if HAS_Y_MS_PINS || HAS_Y2_MS_PINS
        case 1:
          #if HAS_Y_MS_PINS
            WRITE(Y_MS2_PIN, ms2);
          #endif
          #if HAS_Y2_MS_PINS
            WRITE(Y2_MS2_PIN, ms2);
          #endif
          break;
      #endif
      #if HAS_SOME_Z_MS_PINS
        case 2:
          #if HAS_Z_MS_PINS
            WRITE(Z_MS2_PIN, ms2);
          #endif
          #if HAS_Z2_MS_PINS
            WRITE(Z2_MS2_PIN, ms2);
          #endif
          #if HAS_Z3_MS_PINS
            WRITE(Z3_MS2_PIN, ms2);
          #endif
          #if HAS_Z4_MS_PINS
            WRITE(Z4_MS2_PIN, ms2);
          #endif
          break;
      #endif
      #if HAS_E0_MS_PINS
        case  3: WRITE(E0_MS2_PIN, ms2); break;
      #endif
      #if HAS_E1_MS_PINS
        case  4: WRITE(E1_MS2_PIN, ms2); break;
      #endif
      #if HAS_E2_MS_PINS
        case  5: WRITE(E2_MS2_PIN, ms2); break;
      #endif
      #if HAS_E3_MS_PINS
        case  6: WRITE(E3_MS2_PIN, ms2); break;
      #endif
      #if HAS_E4_MS_PINS
        case  7: WRITE(E4_MS2_PIN, ms2); break;
      #endif
      #if HAS_E5_MS_PINS
        case  8: WRITE(E5_MS2_PIN, ms2); break;
      #endif
      #if HAS_E6_MS_PINS
        case  9: WRITE(E6_MS2_PIN, ms2); break;
      #endif
      #if HAS_E7_MS_PINS
        case 10: WRITE(E7_MS2_PIN, ms2); break;
      #endif
    }
    if (ms3 >= 0) switch (driver) {
      #if HAS_X_MS_PINS || HAS_X2_MS_PINS
        case 0:
          #if HAS_X_MS_PINS && PIN_EXISTS(X_MS3)
            WRITE(X_MS3_PIN, ms3);
          #endif
          #if HAS_X2_MS_PINS && PIN_EXISTS(X2_MS3)
            WRITE(X2_MS3_PIN, ms3);
          #endif
          break;
      #endif
      #if HAS_Y_MS_PINS || HAS_Y2_MS_PINS
        case 1:
          #if HAS_Y_MS_PINS && PIN_EXISTS(Y_MS3)
            WRITE(Y_MS3_PIN, ms3);
          #endif
          #if HAS_Y2_MS_PINS && PIN_EXISTS(Y2_MS3)
            WRITE(Y2_MS3_PIN, ms3);
          #endif
          break;
      #endif
      #if HAS_SOME_Z_MS_PINS
        case 2:
          #if HAS_Z_MS_PINS && PIN_EXISTS(Z_MS3)
            WRITE(Z_MS3_PIN, ms3);
          #endif
          #if HAS_Z2_MS_PINS && PIN_EXISTS(Z2_MS3)
            WRITE(Z2_MS3_PIN, ms3);
          #endif
          #if HAS_Z3_MS_PINS && PIN_EXISTS(Z3_MS3)
            WRITE(Z3_MS3_PIN, ms3);
          #endif
          #if HAS_Z4_MS_PINS && PIN_EXISTS(Z4_MS3)
            WRITE(Z4_MS3_PIN, ms3);
          #endif
          break;
      #endif
      #if HAS_E0_MS_PINS && PIN_EXISTS(E0_MS3)
        case  3: WRITE(E0_MS3_PIN, ms3); break;
      #endif
      #if HAS_E1_MS_PINS && PIN_EXISTS(E1_MS3)
        case  4: WRITE(E1_MS3_PIN, ms3); break;
      #endif
      #if HAS_E2_MS_PINS && PIN_EXISTS(E2_MS3)
        case  5: WRITE(E2_MS3_PIN, ms3); break;
      #endif
      #if HAS_E3_MS_PINS && PIN_EXISTS(E3_MS3)
        case  6: WRITE(E3_MS3_PIN, ms3); break;
      #endif
      #if HAS_E4_MS_PINS && PIN_EXISTS(E4_MS3)
        case  7: WRITE(E4_MS3_PIN, ms3); break;
      #endif
      #if HAS_E5_MS_PINS && PIN_EXISTS(E5_MS3)
        case  8: WRITE(E5_MS3_PIN, ms3); break;
      #endif
      #if HAS_E6_MS_PINS && PIN_EXISTS(E6_MS3)
        case  9: WRITE(E6_MS3_PIN, ms3); break;
      #endif
      #if HAS_E7_MS_PINS && PIN_EXISTS(E7_MS3)
        case 10: WRITE(E7_MS3_PIN, ms3); break;
      #endif
    }
  }

  void Stepper::microstep_mode(const uint8_t driver, const uint8_t stepping_mode) {
    switch (stepping_mode) {
      #if HAS_MICROSTEP1
        case 1: microstep_ms(driver, MICROSTEP1); break;
      #endif
      #if HAS_MICROSTEP2
        case 2: microstep_ms(driver, MICROSTEP2); break;
      #endif
      #if HAS_MICROSTEP4
        case 4: microstep_ms(driver, MICROSTEP4); break;
      #endif
      #if HAS_MICROSTEP8
        case 8: microstep_ms(driver, MICROSTEP8); break;
      #endif
      #if HAS_MICROSTEP16
        case 16: microstep_ms(driver, MICROSTEP16); break;
      #endif
      #if HAS_MICROSTEP32
        case 32: microstep_ms(driver, MICROSTEP32); break;
      #endif
      #if HAS_MICROSTEP64
        case 64: microstep_ms(driver, MICROSTEP64); break;
      #endif
      #if HAS_MICROSTEP128
        case 128: microstep_ms(driver, MICROSTEP128); break;
      #endif

      default: SERIAL_ERROR_MSG("Microsteps unavailable"); break;
    }
  }

  void Stepper::microstep_readings() {
    #define PIN_CHAR(P) SERIAL_CHAR('0' + READ(P##_PIN))
    #define MS_LINE(A)  do{ SERIAL_ECHOPGM(" " STRINGIFY(A) ":"); PIN_CHAR(A##_MS1); PIN_CHAR(A##_MS2); }while(0)
    SERIAL_ECHOPGM("MS1|2|3 Pins");
    #if HAS_X_MS_PINS
      MS_LINE(X);
      #if PIN_EXISTS(X_MS3)
        PIN_CHAR(X_MS3);
      #endif
    #endif
    #if HAS_Y_MS_PINS
      MS_LINE(Y);
      #if PIN_EXISTS(Y_MS3)
        PIN_CHAR(Y_MS3);
      #endif
    #endif
    #if HAS_Z_MS_PINS
      MS_LINE(Z);
      #if PIN_EXISTS(Z_MS3)
        PIN_CHAR(Z_MS3);
      #endif
    #endif
    #if HAS_E0_MS_PINS
      MS_LINE(E0);
      #if PIN_EXISTS(E0_MS3)
        PIN_CHAR(E0_MS3);
      #endif
    #endif
    #if HAS_E1_MS_PINS
      MS_LINE(E1);
      #if PIN_EXISTS(E1_MS3)
        PIN_CHAR(E1_MS3);
      #endif
    #endif
    #if HAS_E2_MS_PINS
      MS_LINE(E2);
      #if PIN_EXISTS(E2_MS3)
        PIN_CHAR(E2_MS3);
      #endif
    #endif
    #if HAS_E3_MS_PINS
      MS_LINE(E3);
      #if PIN_EXISTS(E3_MS3)
        PIN_CHAR(E3_MS3);
      #endif
    #endif
    #if HAS_E4_MS_PINS
      MS_LINE(E4);
      #if PIN_EXISTS(E4_MS3)
        PIN_CHAR(E4_MS3);
      #endif
    #endif
    #if HAS_E5_MS_PINS
      MS_LINE(E5);
      #if PIN_EXISTS(E5_MS3)
        PIN_CHAR(E5_MS3);
      #endif
    #endif
    #if HAS_E6_MS_PINS
      MS_LINE(E6);
      #if PIN_EXISTS(E6_MS3)
        PIN_CHAR(E6_MS3);
      #endif
    #endif
    #if HAS_E7_MS_PINS
      MS_LINE(E7);
      #if PIN_EXISTS(E7_MS3)
        PIN_CHAR(E7_MS3);
      #endif
    #endif
    SERIAL_EOL();
  }

#endif // HAS_MICROSTEPS
