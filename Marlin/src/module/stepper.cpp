/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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
 * along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
 */

/* The timer calculations of this module informed by the 'RepRap cartesian firmware' by Zack Smith
   and Philipp Tiefenbacher. */

/* Jerk controlled movements planner added by Eduardo José Tagle in April
   2018, Equations based on Synthethos TinyG2 sources, but the fixed-point
   implementation is a complete new one, as we are running the ISR with a
   variable period.
   Also implemented the Bézier velocity curve evaluation in ARM assembler,
   to avoid impacting ISR speed. */

#include "stepper.h"

#ifdef __AVR__
  #include "speed_lookuptable.h"
#endif

#include "endstops.h"
#include "planner.h"
#include "motion.h"

#include "../module/temperature.h"
#include "../lcd/ultralcd.h"
#include "../core/language.h"
#include "../gcode/queue.h"
#include "../sd/cardreader.h"
#include "../Marlin.h"

#if MB(ALLIGATOR)
  #include "../feature/dac/dac_dac084s085.h"
#endif

#if HAS_DIGIPOTSS
  #include <SPI.h>
#endif

Stepper stepper; // Singleton

// public:

block_t* Stepper::current_block = NULL;  // A pointer to the block currently being traced

#if ENABLED(ABORT_ON_ENDSTOP_HIT_FEATURE_ENABLED)
  bool Stepper::abort_on_endstop_hit = false;
#endif

#if ENABLED(X_DUAL_ENDSTOPS) || ENABLED(Y_DUAL_ENDSTOPS) || ENABLED(Z_DUAL_ENDSTOPS)
  bool Stepper::performing_homing = false;
#endif

#if HAS_MOTOR_CURRENT_PWM
  uint32_t Stepper::motor_current_setting[3]; // Initialized by settings.load()
#endif

// private:

uint8_t Stepper::last_direction_bits = 0;        // The next stepping-bits to be output
int16_t Stepper::cleaning_buffer_counter = 0;

#if ENABLED(X_DUAL_ENDSTOPS)
  bool Stepper::locked_x_motor = false, Stepper::locked_x2_motor = false;
#endif
#if ENABLED(Y_DUAL_ENDSTOPS)
  bool Stepper::locked_y_motor = false, Stepper::locked_y2_motor = false;
#endif
#if ENABLED(Z_DUAL_ENDSTOPS)
  bool Stepper::locked_z_motor = false, Stepper::locked_z2_motor = false;
#endif

long Stepper::counter_X = 0,
     Stepper::counter_Y = 0,
     Stepper::counter_Z = 0,
     Stepper::counter_E = 0;

volatile uint32_t Stepper::step_events_completed = 0; // The number of step events executed in the current block

#if ENABLED(BEZIER_JERK_CONTROL)
  int32_t __attribute__((used)) Stepper::bezier_A __asm__("bezier_A");    // A coefficient in Bézier speed curve with alias for assembler
  int32_t __attribute__((used)) Stepper::bezier_B __asm__("bezier_B");    // B coefficient in Bézier speed curve with alias for assembler
  int32_t __attribute__((used)) Stepper::bezier_C __asm__("bezier_C");    // C coefficient in Bézier speed curve with alias for assembler
  uint32_t __attribute__((used)) Stepper::bezier_F __asm__("bezier_F");   // F coefficient in Bézier speed curve with alias for assembler
  uint32_t __attribute__((used)) Stepper::bezier_AV __asm__("bezier_AV"); // AV coefficient in Bézier speed curve with alias for assembler
  #ifdef __AVR__
    bool __attribute__((used)) Stepper::A_negative __asm__("A_negative");   // If A coefficient was negative
  #endif
  bool Stepper::bezier_2nd_half;    // =false If Bézier curve has been initialized or not
#endif

#if ENABLED(LIN_ADVANCE)

  uint32_t Stepper::LA_decelerate_after;

  constexpr hal_timer_t ADV_NEVER = HAL_TIMER_TYPE_MAX;

  hal_timer_t Stepper::nextMainISR = 0,
              Stepper::nextAdvanceISR = ADV_NEVER,
              Stepper::eISR_Rate = ADV_NEVER;
  uint16_t Stepper::current_adv_steps = 0,
           Stepper::final_adv_steps,
           Stepper::max_adv_steps;

  int8_t Stepper::e_steps = 0;

  #if E_STEPPERS > 1
    int8_t Stepper::LA_active_extruder; // Copy from current executed block. Needed because current_block is set to NULL "too early".
  #else
    constexpr int8_t Stepper::LA_active_extruder;
  #endif

  bool Stepper::use_advance_lead;

#endif // LIN_ADVANCE

int32_t Stepper::acceleration_time, Stepper::deceleration_time;

volatile int32_t Stepper::count_position[NUM_AXIS] = { 0 };
volatile signed char Stepper::count_direction[NUM_AXIS] = { 1, 1, 1, 1 };

#if ENABLED(MIXING_EXTRUDER)
  long Stepper::counter_m[MIXING_STEPPERS];
#endif

uint8_t Stepper::step_loops, Stepper::step_loops_nominal;

hal_timer_t Stepper::OCR1A_nominal;
#if DISABLED(BEZIER_JERK_CONTROL)
  hal_timer_t Stepper::acc_step_rate; // needed for deceleration start point
#endif

volatile long Stepper::endstops_trigsteps[XYZ];

#if ENABLED(X_DUAL_ENDSTOPS) || ENABLED(Y_DUAL_ENDSTOPS) || ENABLED(Z_DUAL_ENDSTOPS)
  #define LOCKED_X_MOTOR  locked_x_motor
  #define LOCKED_Y_MOTOR  locked_y_motor
  #define LOCKED_Z_MOTOR  locked_z_motor
  #define LOCKED_X2_MOTOR locked_x2_motor
  #define LOCKED_Y2_MOTOR locked_y2_motor
  #define LOCKED_Z2_MOTOR locked_z2_motor
  #define DUAL_ENDSTOP_APPLY_STEP(AXIS,v)                                                                                                           \
    if (performing_homing) {                                                                                                                        \
      if (AXIS##_HOME_DIR < 0) {                                                                                                                    \
        if (!(TEST(endstops.old_endstop_bits, AXIS##_MIN) && count_direction[AXIS##_AXIS] < 0) && !LOCKED_##AXIS##_MOTOR) AXIS##_STEP_WRITE(v);     \
        if (!(TEST(endstops.old_endstop_bits, AXIS##2_MIN) && count_direction[AXIS##_AXIS] < 0) && !LOCKED_##AXIS##2_MOTOR) AXIS##2_STEP_WRITE(v);  \
      }                                                                                                                                             \
      else {                                                                                                                                        \
        if (!(TEST(endstops.old_endstop_bits, AXIS##_MAX) && count_direction[AXIS##_AXIS] > 0) && !LOCKED_##AXIS##_MOTOR) AXIS##_STEP_WRITE(v);     \
        if (!(TEST(endstops.old_endstop_bits, AXIS##2_MAX) && count_direction[AXIS##_AXIS] > 0) && !LOCKED_##AXIS##2_MOTOR) AXIS##2_STEP_WRITE(v);  \
      }                                                                                                                                             \
    }                                                                                                                                               \
    else {                                                                                                                                          \
      AXIS##_STEP_WRITE(v);                                                                                                                         \
      AXIS##2_STEP_WRITE(v);                                                                                                                        \
    }
#endif

#if ENABLED(X_DUAL_STEPPER_DRIVERS)
  #define X_APPLY_DIR(v,Q) do{ X_DIR_WRITE(v); X2_DIR_WRITE((v) != INVERT_X2_VS_X_DIR); }while(0)
  #if ENABLED(X_DUAL_ENDSTOPS)
    #define X_APPLY_STEP(v,Q) DUAL_ENDSTOP_APPLY_STEP(X,v)
  #else
    #define X_APPLY_STEP(v,Q) do{ X_STEP_WRITE(v); X2_STEP_WRITE(v); }while(0)
  #endif
#elif ENABLED(DUAL_X_CARRIAGE)
  #define X_APPLY_DIR(v,ALWAYS) \
    if (extruder_duplication_enabled || ALWAYS) { \
      X_DIR_WRITE(v); \
      X2_DIR_WRITE(v); \
    } \
    else { \
      if (current_block->active_extruder) X2_DIR_WRITE(v); else X_DIR_WRITE(v); \
    }
  #define X_APPLY_STEP(v,ALWAYS) \
    if (extruder_duplication_enabled || ALWAYS) { \
      X_STEP_WRITE(v); \
      X2_STEP_WRITE(v); \
    } \
    else { \
      if (current_block->active_extruder) X2_STEP_WRITE(v); else X_STEP_WRITE(v); \
    }
#else
  #define X_APPLY_DIR(v,Q) X_DIR_WRITE(v)
  #define X_APPLY_STEP(v,Q) X_STEP_WRITE(v)
#endif

#if ENABLED(Y_DUAL_STEPPER_DRIVERS)
  #define Y_APPLY_DIR(v,Q) do{ Y_DIR_WRITE(v); Y2_DIR_WRITE((v) != INVERT_Y2_VS_Y_DIR); }while(0)
  #if ENABLED(Y_DUAL_ENDSTOPS)
    #define Y_APPLY_STEP(v,Q) DUAL_ENDSTOP_APPLY_STEP(Y,v)
  #else
    #define Y_APPLY_STEP(v,Q) do{ Y_STEP_WRITE(v); Y2_STEP_WRITE(v); }while(0)
  #endif
#else
  #define Y_APPLY_DIR(v,Q) Y_DIR_WRITE(v)
  #define Y_APPLY_STEP(v,Q) Y_STEP_WRITE(v)
#endif

#if ENABLED(Z_DUAL_STEPPER_DRIVERS)
  #define Z_APPLY_DIR(v,Q) do{ Z_DIR_WRITE(v); Z2_DIR_WRITE(v); }while(0)
  #if ENABLED(Z_DUAL_ENDSTOPS)
    #define Z_APPLY_STEP(v,Q) DUAL_ENDSTOP_APPLY_STEP(Z,v)
  #else
    #define Z_APPLY_STEP(v,Q) do{ Z_STEP_WRITE(v); Z2_STEP_WRITE(v); }while(0)
  #endif
#else
  #define Z_APPLY_DIR(v,Q) Z_DIR_WRITE(v)
  #define Z_APPLY_STEP(v,Q) Z_STEP_WRITE(v)
#endif

#if DISABLED(MIXING_EXTRUDER)
  #define E_APPLY_STEP(v,Q) E_STEP_WRITE(v)
#endif

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
void Stepper::wake_up() {
  // TCNT1 = 0;
  ENABLE_STEPPER_DRIVER_INTERRUPT();
}

/**
 * Set the stepper direction of each axis
 *
 *   COREXY: X_AXIS=A_AXIS and Y_AXIS=B_AXIS
 *   COREXZ: X_AXIS=A_AXIS and Z_AXIS=C_AXIS
 *   COREYZ: Y_AXIS=B_AXIS and Z_AXIS=C_AXIS
 */
void Stepper::set_directions() {

  #define SET_STEP_DIR(AXIS) \
    if (motor_direction(AXIS ##_AXIS)) { \
      AXIS ##_APPLY_DIR(INVERT_## AXIS ##_DIR, false); \
      count_direction[AXIS ##_AXIS] = -1; \
    } \
    else { \
      AXIS ##_APPLY_DIR(!INVERT_## AXIS ##_DIR, false); \
      count_direction[AXIS ##_AXIS] = 1; \
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
    if (motor_direction(E_AXIS)) {
      REV_E_DIR();
      count_direction[E_AXIS] = -1;
    }
    else {
      NORM_E_DIR();
      count_direction[E_AXIS] = 1;
    }
  #endif // !LIN_ADVANCE
}

#if ENABLED(ENDSTOP_INTERRUPTS_FEATURE)
  extern volatile uint8_t e_hit;
#endif

#if ENABLED(BEZIER_JERK_CONTROL)
  /**
   *   We are using a quintic (fifth-degree) Bézier polynomial for the velocity curve.
   *  This gives us a "linear pop" velocity curve; with pop being the sixth derivative of position:
   *  velocity - 1st, acceleration - 2nd, jerk - 3rd, snap - 4th, crackle - 5th, pop - 6th
   *
   *  The Bézier curve takes the form:
   *
   *  V(t) = P_0 * B_0(t) + P_1 * B_1(t) + P_2 * B_2(t) + P_3 * B_3(t) + P_4 * B_4(t) + P_5 * B_5(t)
   *
   *   Where 0 <= t <= 1, and V(t) is the velocity. P_0 through P_5 are the control points, and B_0(t)
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
   *   Unfortunately, we cannot use forward-differencing to calculate each position through
   *  the curve, as Marlin uses variable timer periods. So, we require a formula of the form:
   *
   *        V_f(t) = A*t^5 + B*t^4 + C*t^3 + D*t^2 + E*t + F
   *
   *   Looking at the above B_0(t) through B_5(t) expanded forms, if we take the coefficients of t^5
   *  through t of the Bézier form of V(t), we can determine that:
   *
   *        A =    -P_0 +  5*P_1 - 10*P_2 + 10*P_3 -  5*P_4 +  P_5
   *        B =   5*P_0 - 20*P_1 + 30*P_2 - 20*P_3 +  5*P_4
   *        C = -10*P_0 + 30*P_1 - 30*P_2 + 10*P_3
   *        D =  10*P_0 - 20*P_1 + 10*P_2
   *        E = - 5*P_0 +  5*P_1
   *        F =     P_0
   *
   *   Now, since we will (currently) *always* want the initial acceleration and jerk values to be 0,
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
   *   As the t is evaluated in non uniform steps here, there is no other way rather than evaluating
   *  the Bézier curve at each point:
   *
   *        V_f(t) = A*t^5 + B*t^4 + C*t^3 + F          [0 <= t <= 1]
   *
   *   Floating point arithmetic execution time cost is prohibitive, so we will transform the math to
   * use fixed point values to be able to evaluate it in realtime. Assuming a maximum of 250000 steps
   * per second (driver pulses should at least be 2uS hi/2uS lo), and allocating 2 bits to avoid
   * overflows on the evaluation of the Bézier curve, means we can use
   *
   *   t: unsigned Q0.32 (0 <= t < 1) |range 0 to 0xFFFFFFFF unsigned
   *   A:   signed Q24.7 ,            |range = +/- 250000 * 6 * 128 = +/- 192000000 = 0x0B71B000 | 28 bits + sign
   *   B:   signed Q24.7 ,            |range = +/- 250000 *15 * 128 = +/- 480000000 = 0x1C9C3800 | 29 bits + sign
   *   C:   signed Q24.7 ,            |range = +/- 250000 *10 * 128 = +/- 320000000 = 0x1312D000 | 29 bits + sign
   *   F:   signed Q24.7 ,            |range = +/- 250000     * 128 =      32000000 = 0x01E84800 | 25 bits + sign
   *
   *  The trapezoid generator state contains the following information, that we will use to create and evaluate
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
   *    At the start of each trapezoid, we calculate the coefficients A,B,C,F and Advance [AV], as follows:
   *
   *      A =  6*128*(VF - VI) =  768*(VF - VI)
   *      B = 15*128*(VI - VF) = 1920*(VI - VF)
   *      C = 10*128*(VF - VI) = 1280*(VF - VI)
   *      F =    128*VI        =  128*VI
   *     AV = (1<<32)/TS      ~= 0xFFFFFFFF / TS (To use ARM UDIV, that is 32 bits) (this is computed at the planner, to offload expensive calculations from the ISR)
   *
   *   And for each point, we will evaluate the curve with the following sequence:
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
   *        register uint32_t flo = 0;
   *        register uint32_t fhi = bezier_AV * curr_step;
   *        register uint32_t t = fhi;
   *        register int32_t alo = bezier_F;
   *        register int32_t ahi = 0;
   *        register int32_t A = bezier_A;
   *        register int32_t B = bezier_B;
   *        register int32_t C = bezier_C;
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
   *    This will be rewritten in ARM assembly to get peak performance and will take 43 cycles to execute
   *
   *  For AVR, we scale precision of coefficients to make it possible to evaluate the Bézier curve in
   *    realtime: Let's reduce precision as much as possible. After some experimentation we found that:
   *
   *    Assume t and AV with 24 bits is enough
   *       A =  6*(VF - VI)
   *       B = 15*(VI - VF)
   *       C = 10*(VF - VI)
   *       F =     VI
   *      AV = (1<<24)/TS   (this is computed at the planner, to offload expensive calculations from the ISR)
   *
   *     Instead of storing sign for each coefficient, we will store its absolute value,
   *    and flag the sign of the A coefficient, so we can save to store the sign bit.
   *     It always holds that sign(A) = - sign(B) = sign(C)
   *
   *     So, the resulting range of the coefficients are:
   *
   *       t: unsigned (0 <= t < 1) |range 0 to 0xFFFFFF unsigned
   *       A:   signed Q24 , range = 250000 * 6 = 1500000 = 0x16E360 | 21 bits
   *       B:   signed Q24 , range = 250000 *15 = 3750000 = 0x393870 | 22 bits
   *       C:   signed Q24 , range = 250000 *10 = 2500000 = 0x1312D0 | 21 bits
   *       F:   signed Q24 , range = 250000     =  250000 = 0x0ED090 | 20 bits
   *
   *    And for each curve, we estimate its coefficients with:
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
   *    And for each point, we will evaluate the curve with the following sequence:
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
   *        umul16x16to16hi(f, f, t);           // Range 16 bits (unsigned)
   *        umul16x16to16hi(f, f, t);           // Range 16 bits : f = t^3  (unsigned)
   *        uint24_t acc = bezier_F;          // Range 20 bits (unsigned)
   *        if (A_negative) {
   *          uint24_t v;
   *          umul16x24to24hi(v, f, bezier_C);    // Range 21bits
   *          acc -= v;
   *          umul16x16to16hi(f, f, t);         // Range 16 bits : f = t^4  (unsigned)
   *          umul16x24to24hi(v, f, bezier_B);    // Range 22bits
   *          acc += v;
   *          umul16x16to16hi(f, f, t);         // Range 16 bits : f = t^5  (unsigned)
   *          umul16x24to24hi(v, f, bezier_A);    // Range 21bits + 15 = 36bits (plus sign)
   *          acc -= v;
   *        }
   *        else {
   *          uint24_t v;
   *          umul16x24to24hi(v, f, bezier_C);    // Range 21bits
   *          acc += v;
   *          umul16x16to16hi(f, f, t);       // Range 16 bits : f = t^4  (unsigned)
   *          umul16x24to24hi(v, f, bezier_B);    // Range 22bits
   *          acc -= v;
   *          umul16x16to16hi(f, f, t);               // Range 16 bits : f = t^5  (unsigned)
   *          umul16x24to24hi(v, f, bezier_A);    // Range 21bits + 15 = 36bits (plus sign)
   *          acc += v;
   *        }
   *        return acc;
   *      }
   *    Those functions will be translated into assembler to get peak performance. coefficient calculations takes 70 cycles,
   *    Bezier point evaluation takes 150 cycles
   *
   */

  #ifdef __AVR__

    // For AVR we use assembly to maximize speed
    void Stepper::_calc_bezier_curve_coeffs(const int32_t v0, const int32_t v1, const uint32_t av) {

      // Store advance
      bezier_AV = av;

      // Calculate the rest of the coefficients
      register uint8_t r2 = v0 & 0xFF;
      register uint8_t r3 = (v0 >> 8) & 0xFF;
      register uint8_t r12 = (v0 >> 16) & 0xFF;
      register uint8_t r5 = v1 & 0xFF;
      register uint8_t r6 = (v1 >> 8) & 0xFF;
      register uint8_t r7 = (v1 >> 16) & 0xFF;
      register uint8_t r4,r8,r9,r10,r11;

      __asm__ __volatile__(
        /* Calculate the Bézier coefficients */
        /*  %10:%1:%0 = v0*/
        /*  %5:%4:%3 = v1*/
        /*  %7:%6:%10 = temporary*/
        /*  %9 = val (must be high register!)*/
        /*  %10 (must be high register!)*/

        /* Store initial velocity*/
        " sts bezier_F, %0" "\n\t"
        " sts bezier_F+1, %1" "\n\t"
        " sts bezier_F+2, %10" "\n\t"    /* bezier_F = %10:%1:%0 = v0 */

        /* Get delta speed */
        " ldi %2,-1" "\n\t"              /* %2 = 0xff, means A_negative = true */
        " clr %8" "\n\t"                 /* %8 = 0 */
        " sub %0,%3" "\n\t"
        " sbc %1,%4" "\n\t"
        " sbc %10,%5" "\n\t"             /*  v0 -= v1, C=1 if result is negative */
        " brcc 1f" "\n\t"                /* branch if result is positive (C=0), that means v0 >= v1 */

        /*  Result was negative, get the absolute value*/
        " com %10" "\n\t"
        " com %1" "\n\t"
        " neg %0" "\n\t"
        " sbc %1,%2" "\n\t"
        " sbc %10,%2" "\n\t"             /* %10:%1:%0 +1  -> %10:%1:%0 = -(v0 - v1) = (v1 - v0) */
        " clr %2" "\n\t"                 /* %2 = 0, means A_negative = false */

        /*  Store negative flag*/
        "1:" "\n\t"
        " sts A_negative, %2" "\n\t"     /* Store negative flag */

        /*  Compute coefficients A,B and C   [20 cycles worst case]*/
        " ldi %9,6" "\n\t"               /* %9 = 6 */
        " mul %0,%9" "\n\t"              /* r1:r0 = 6*LO(v0-v1) */
        " sts bezier_A, r0" "\n\t"
        " mov %6,r1" "\n\t"
        " clr %7" "\n\t"                 /* %7:%6:r0 = 6*LO(v0-v1) */
        " mul %1,%9" "\n\t"              /* r1:r0 = 6*MI(v0-v1) */
        " add %6,r0" "\n\t"
        " adc %7,r1" "\n\t"              /* %7:%6:?? += 6*MI(v0-v1) << 8 */
        " mul %10,%9" "\n\t"             /* r1:r0 = 6*HI(v0-v1) */
        " add %7,r0" "\n\t"              /* %7:%6:?? += 6*HI(v0-v1) << 16 */
        " sts bezier_A+1, %6" "\n\t"
        " sts bezier_A+2, %7" "\n\t"     /* bezier_A = %7:%6:?? = 6*(v0-v1) [35 cycles worst] */

        " ldi %9,15" "\n\t"              /* %9 = 15 */
        " mul %0,%9" "\n\t"              /* r1:r0 = 5*LO(v0-v1) */
        " sts bezier_B, r0" "\n\t"
        " mov %6,r1" "\n\t"
        " clr %7" "\n\t"                 /* %7:%6:?? = 5*LO(v0-v1) */
        " mul %1,%9" "\n\t"              /* r1:r0 = 5*MI(v0-v1) */
        " add %6,r0" "\n\t"
        " adc %7,r1" "\n\t"              /* %7:%6:?? += 5*MI(v0-v1) << 8 */
        " mul %10,%9" "\n\t"             /* r1:r0 = 5*HI(v0-v1) */
        " add %7,r0" "\n\t"              /* %7:%6:?? += 5*HI(v0-v1) << 16 */
        " sts bezier_B+1, %6" "\n\t"
        " sts bezier_B+2, %7" "\n\t"     /* bezier_B = %7:%6:?? = 5*(v0-v1) [50 cycles worst] */

        " ldi %9,10" "\n\t"              /* %9 = 10 */
        " mul %0,%9" "\n\t"              /* r1:r0 = 10*LO(v0-v1) */
        " sts bezier_C, r0" "\n\t"
        " mov %6,r1" "\n\t"
        " clr %7" "\n\t"                 /* %7:%6:?? = 10*LO(v0-v1) */
        " mul %1,%9" "\n\t"              /* r1:r0 = 10*MI(v0-v1) */
        " add %6,r0" "\n\t"
        " adc %7,r1" "\n\t"              /* %7:%6:?? += 10*MI(v0-v1) << 8 */
        " mul %10,%9" "\n\t"             /* r1:r0 = 10*HI(v0-v1) */
        " add %7,r0" "\n\t"              /* %7:%6:?? += 10*HI(v0-v1) << 16 */
        " sts bezier_C+1, %6" "\n\t"
        " sts bezier_C+2, %7"            /* bezier_C = %7:%6:?? = 10*(v0-v1) [65 cycles worst] */
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

      register uint8_t r0 = 0; /* Zero register */
      register uint8_t r2 = (curr_step) & 0xFF;
      register uint8_t r3 = (curr_step >> 8) & 0xFF;
      register uint8_t r4 = (curr_step >> 16) & 0xFF;
      register uint8_t r1,r5,r6,r7,r8,r9,r10,r11; /* Temporary registers */

      __asm__ __volatile(
        /* umul24x24to16hi(t, bezier_AV, curr_step);  t: Range 0 - 1^16 = 16 bits*/
        " lds %9,bezier_AV" "\n\t"       /* %9 = LO(AV)*/
        " mul %9,%2" "\n\t"              /* r1:r0 = LO(bezier_AV)*LO(curr_step)*/
        " mov %7,r1" "\n\t"              /* %7 = LO(bezier_AV)*LO(curr_step) >> 8*/
        " clr %8" "\n\t"                 /* %8:%7  = LO(bezier_AV)*LO(curr_step) >> 8*/
        " lds %10,bezier_AV+1" "\n\t"    /* %10 = MI(AV)*/
        " mul %10,%2" "\n\t"             /* r1:r0  = MI(bezier_AV)*LO(curr_step)*/
        " add %7,r0" "\n\t"
        " adc %8,r1" "\n\t"              /* %8:%7 += MI(bezier_AV)*LO(curr_step)*/
        " lds r1,bezier_AV+2" "\n\t"     /* r11 = HI(AV)*/
        " mul r1,%2" "\n\t"              /* r1:r0  = HI(bezier_AV)*LO(curr_step)*/
        " add %8,r0" "\n\t"              /* %8:%7 += HI(bezier_AV)*LO(curr_step) << 8*/
        " mul %9,%3" "\n\t"              /* r1:r0 =  LO(bezier_AV)*MI(curr_step)*/
        " add %7,r0" "\n\t"
        " adc %8,r1" "\n\t"              /* %8:%7 += LO(bezier_AV)*MI(curr_step)*/
        " mul %10,%3" "\n\t"             /* r1:r0 =  MI(bezier_AV)*MI(curr_step)*/
        " add %8,r0" "\n\t"              /* %8:%7 += LO(bezier_AV)*MI(curr_step) << 8*/
        " mul %9,%4" "\n\t"              /* r1:r0 =  LO(bezier_AV)*HI(curr_step)*/
        " add %8,r0" "\n\t"              /* %8:%7 += LO(bezier_AV)*HI(curr_step) << 8*/
        /* %8:%7 = t*/

        /* uint16_t f = t;*/
        " mov %5,%7" "\n\t"              /* %6:%5 = f*/
        " mov %6,%8" "\n\t"
        /* %6:%5 = f*/

        /* umul16x16to16hi(f, f, t); / Range 16 bits (unsigned) [17] */
        " mul %5,%7" "\n\t"              /* r1:r0 = LO(f) * LO(t)*/
        " mov %9,r1" "\n\t"              /* store MIL(LO(f) * LO(t)) in %9, we need it for rounding*/
        " clr %10" "\n\t"                /* %10 = 0*/
        " clr %11" "\n\t"                /* %11 = 0*/
        " mul %5,%8" "\n\t"              /* r1:r0 = LO(f) * HI(t)*/
        " add %9,r0" "\n\t"              /* %9 += LO(LO(f) * HI(t))*/
        " adc %10,r1" "\n\t"             /* %10 = HI(LO(f) * HI(t))*/
        " adc %11,%0" "\n\t"             /* %11 += carry*/
        " mul %6,%7" "\n\t"              /* r1:r0 = HI(f) * LO(t)*/
        " add %9,r0" "\n\t"              /* %9 += LO(HI(f) * LO(t))*/
        " adc %10,r1" "\n\t"             /* %10 += HI(HI(f) * LO(t)) */
        " adc %11,%0" "\n\t"             /* %11 += carry*/
        " mul %6,%8" "\n\t"              /* r1:r0 = HI(f) * HI(t)*/
        " add %10,r0" "\n\t"             /* %10 += LO(HI(f) * HI(t))*/
        " adc %11,r1" "\n\t"             /* %11 += HI(HI(f) * HI(t))*/
        " mov %5,%10" "\n\t"             /* %6:%5 = */
        " mov %6,%11" "\n\t"             /* f = %10:%11*/

        /* umul16x16to16hi(f, f, t); / Range 16 bits : f = t^3  (unsigned) [17]*/
        " mul %5,%7" "\n\t"              /* r1:r0 = LO(f) * LO(t)*/
        " mov %1,r1" "\n\t"              /* store MIL(LO(f) * LO(t)) in %1, we need it for rounding*/
        " clr %10" "\n\t"                /* %10 = 0*/
        " clr %11" "\n\t"                /* %11 = 0*/
        " mul %5,%8" "\n\t"              /* r1:r0 = LO(f) * HI(t)*/
        " add %1,r0" "\n\t"              /* %1 += LO(LO(f) * HI(t))*/
        " adc %10,r1" "\n\t"             /* %10 = HI(LO(f) * HI(t))*/
        " adc %11,%0" "\n\t"             /* %11 += carry*/
        " mul %6,%7" "\n\t"              /* r1:r0 = HI(f) * LO(t)*/
        " add %1,r0" "\n\t"              /* %1 += LO(HI(f) * LO(t))*/
        " adc %10,r1" "\n\t"             /* %10 += HI(HI(f) * LO(t))*/
        " adc %11,%0" "\n\t"             /* %11 += carry*/
        " mul %6,%8" "\n\t"              /* r1:r0 = HI(f) * HI(t)*/
        " add %10,r0" "\n\t"             /* %10 += LO(HI(f) * HI(t))*/
        " adc %11,r1" "\n\t"             /* %11 += HI(HI(f) * HI(t))*/
        " mov %5,%10" "\n\t"             /* %6:%5 =*/
        " mov %6,%11" "\n\t"             /* f = %10:%11*/
        /* [15 +17*2] = [49]*/

        /* %4:%3:%2 will be acc from now on*/

        /* uint24_t acc = bezier_F; / Range 20 bits (unsigned)*/
        " clr %9" "\n\t"                 /* "decimal place we get for free"*/
        " lds %2,bezier_F" "\n\t"
        " lds %3,bezier_F+1" "\n\t"
        " lds %4,bezier_F+2" "\n\t"      /* %4:%3:%2 = acc*/

        /* if (A_negative) {*/
        " lds r0,A_negative" "\n\t"
        " or r0,%0" "\n\t"               /* Is flag signalling negative? */
        " brne 3f" "\n\t"                /* If yes, Skip next instruction if A was negative*/
        " rjmp 1f" "\n\t"                /* Otherwise, jump */

        /* uint24_t v; */
        /* umul16x24to24hi(v, f, bezier_C); / Range 21bits [29] */
        /* acc -= v; */
        "3:" "\n\t"
        " lds %10, bezier_C" "\n\t"      /* %10 = LO(bezier_C)*/
        " mul %10,%5" "\n\t"             /* r1:r0 = LO(bezier_C) * LO(f)*/
        " sub %9,r1" "\n\t"
        " sbc %2,%0" "\n\t"
        " sbc %3,%0" "\n\t"
        " sbc %4,%0" "\n\t"              /* %4:%3:%2:%9 -= HI(LO(bezier_C) * LO(f))*/
        " lds %11, bezier_C+1" "\n\t"    /* %11 = MI(bezier_C)*/
        " mul %11,%5" "\n\t"             /* r1:r0 = MI(bezier_C) * LO(f)*/
        " sub %9,r0" "\n\t"
        " sbc %2,r1" "\n\t"
        " sbc %3,%0" "\n\t"
        " sbc %4,%0" "\n\t"              /* %4:%3:%2:%9 -= MI(bezier_C) * LO(f)*/
        " lds %1, bezier_C+2" "\n\t"     /* %1 = HI(bezier_C)*/
        " mul %1,%5" "\n\t"              /* r1:r0 = MI(bezier_C) * LO(f)*/
        " sub %2,r0" "\n\t"
        " sbc %3,r1" "\n\t"
        " sbc %4,%0" "\n\t"              /* %4:%3:%2:%9 -= HI(bezier_C) * LO(f) << 8*/
        " mul %10,%6" "\n\t"             /* r1:r0 = LO(bezier_C) * MI(f)*/
        " sub %9,r0" "\n\t"
        " sbc %2,r1" "\n\t"
        " sbc %3,%0" "\n\t"
        " sbc %4,%0" "\n\t"              /* %4:%3:%2:%9 -= LO(bezier_C) * MI(f)*/
        " mul %11,%6" "\n\t"             /* r1:r0 = MI(bezier_C) * MI(f)*/
        " sub %2,r0" "\n\t"
        " sbc %3,r1" "\n\t"
        " sbc %4,%0" "\n\t"              /* %4:%3:%2:%9 -= MI(bezier_C) * MI(f) << 8*/
        " mul %1,%6" "\n\t"              /* r1:r0 = HI(bezier_C) * LO(f)*/
        " sub %3,r0" "\n\t"
        " sbc %4,r1" "\n\t"              /* %4:%3:%2:%9 -= HI(bezier_C) * LO(f) << 16*/

        /* umul16x16to16hi(f, f, t); / Range 16 bits : f = t^3  (unsigned) [17]*/
        " mul %5,%7" "\n\t"              /* r1:r0 = LO(f) * LO(t)*/
        " mov %1,r1" "\n\t"              /* store MIL(LO(f) * LO(t)) in %1, we need it for rounding*/
        " clr %10" "\n\t"                /* %10 = 0*/
        " clr %11" "\n\t"                /* %11 = 0*/
        " mul %5,%8" "\n\t"              /* r1:r0 = LO(f) * HI(t)*/
        " add %1,r0" "\n\t"              /* %1 += LO(LO(f) * HI(t))*/
        " adc %10,r1" "\n\t"             /* %10 = HI(LO(f) * HI(t))*/
        " adc %11,%0" "\n\t"             /* %11 += carry*/
        " mul %6,%7" "\n\t"              /* r1:r0 = HI(f) * LO(t)*/
        " add %1,r0" "\n\t"              /* %1 += LO(HI(f) * LO(t))*/
        " adc %10,r1" "\n\t"             /* %10 += HI(HI(f) * LO(t))*/
        " adc %11,%0" "\n\t"             /* %11 += carry*/
        " mul %6,%8" "\n\t"              /* r1:r0 = HI(f) * HI(t)*/
        " add %10,r0" "\n\t"             /* %10 += LO(HI(f) * HI(t))*/
        " adc %11,r1" "\n\t"             /* %11 += HI(HI(f) * HI(t))*/
        " mov %5,%10" "\n\t"             /* %6:%5 =*/
        " mov %6,%11" "\n\t"             /* f = %10:%11*/

        /* umul16x24to24hi(v, f, bezier_B); / Range 22bits [29]*/
        /* acc += v; */
        " lds %10, bezier_B" "\n\t"      /* %10 = LO(bezier_B)*/
        " mul %10,%5" "\n\t"             /* r1:r0 = LO(bezier_B) * LO(f)*/
        " add %9,r1" "\n\t"
        " adc %2,%0" "\n\t"
        " adc %3,%0" "\n\t"
        " adc %4,%0" "\n\t"              /* %4:%3:%2:%9 += HI(LO(bezier_B) * LO(f))*/
        " lds %11, bezier_B+1" "\n\t"    /* %11 = MI(bezier_B)*/
        " mul %11,%5" "\n\t"             /* r1:r0 = MI(bezier_B) * LO(f)*/
        " add %9,r0" "\n\t"
        " adc %2,r1" "\n\t"
        " adc %3,%0" "\n\t"
        " adc %4,%0" "\n\t"              /* %4:%3:%2:%9 += MI(bezier_B) * LO(f)*/
        " lds %1, bezier_B+2" "\n\t"     /* %1 = HI(bezier_B)*/
        " mul %1,%5" "\n\t"              /* r1:r0 = MI(bezier_B) * LO(f)*/
        " add %2,r0" "\n\t"
        " adc %3,r1" "\n\t"
        " adc %4,%0" "\n\t"              /* %4:%3:%2:%9 += HI(bezier_B) * LO(f) << 8*/
        " mul %10,%6" "\n\t"             /* r1:r0 = LO(bezier_B) * MI(f)*/
        " add %9,r0" "\n\t"
        " adc %2,r1" "\n\t"
        " adc %3,%0" "\n\t"
        " adc %4,%0" "\n\t"              /* %4:%3:%2:%9 += LO(bezier_B) * MI(f)*/
        " mul %11,%6" "\n\t"             /* r1:r0 = MI(bezier_B) * MI(f)*/
        " add %2,r0" "\n\t"
        " adc %3,r1" "\n\t"
        " adc %4,%0" "\n\t"              /* %4:%3:%2:%9 += MI(bezier_B) * MI(f) << 8*/
        " mul %1,%6" "\n\t"              /* r1:r0 = HI(bezier_B) * LO(f)*/
        " add %3,r0" "\n\t"
        " adc %4,r1" "\n\t"              /* %4:%3:%2:%9 += HI(bezier_B) * LO(f) << 16*/

        /* umul16x16to16hi(f, f, t); / Range 16 bits : f = t^5  (unsigned) [17]*/
        " mul %5,%7" "\n\t"              /* r1:r0 = LO(f) * LO(t)*/
        " mov %1,r1" "\n\t"              /* store MIL(LO(f) * LO(t)) in %1, we need it for rounding*/
        " clr %10" "\n\t"                /* %10 = 0*/
        " clr %11" "\n\t"                /* %11 = 0*/
        " mul %5,%8" "\n\t"              /* r1:r0 = LO(f) * HI(t)*/
        " add %1,r0" "\n\t"              /* %1 += LO(LO(f) * HI(t))*/
        " adc %10,r1" "\n\t"             /* %10 = HI(LO(f) * HI(t))*/
        " adc %11,%0" "\n\t"             /* %11 += carry*/
        " mul %6,%7" "\n\t"              /* r1:r0 = HI(f) * LO(t)*/
        " add %1,r0" "\n\t"              /* %1 += LO(HI(f) * LO(t))*/
        " adc %10,r1" "\n\t"             /* %10 += HI(HI(f) * LO(t))*/
        " adc %11,%0" "\n\t"             /* %11 += carry*/
        " mul %6,%8" "\n\t"              /* r1:r0 = HI(f) * HI(t)*/
        " add %10,r0" "\n\t"             /* %10 += LO(HI(f) * HI(t))*/
        " adc %11,r1" "\n\t"             /* %11 += HI(HI(f) * HI(t))*/
        " mov %5,%10" "\n\t"             /* %6:%5 =*/
        " mov %6,%11" "\n\t"             /* f = %10:%11*/

        /* umul16x24to24hi(v, f, bezier_A); / Range 21bits [29]*/
        /* acc -= v; */
        " lds %10, bezier_A" "\n\t"      /* %10 = LO(bezier_A)*/
        " mul %10,%5" "\n\t"             /* r1:r0 = LO(bezier_A) * LO(f)*/
        " sub %9,r1" "\n\t"
        " sbc %2,%0" "\n\t"
        " sbc %3,%0" "\n\t"
        " sbc %4,%0" "\n\t"              /* %4:%3:%2:%9 -= HI(LO(bezier_A) * LO(f))*/
        " lds %11, bezier_A+1" "\n\t"    /* %11 = MI(bezier_A)*/
        " mul %11,%5" "\n\t"             /* r1:r0 = MI(bezier_A) * LO(f)*/
        " sub %9,r0" "\n\t"
        " sbc %2,r1" "\n\t"
        " sbc %3,%0" "\n\t"
        " sbc %4,%0" "\n\t"              /* %4:%3:%2:%9 -= MI(bezier_A) * LO(f)*/
        " lds %1, bezier_A+2" "\n\t"     /* %1 = HI(bezier_A)*/
        " mul %1,%5" "\n\t"              /* r1:r0 = MI(bezier_A) * LO(f)*/
        " sub %2,r0" "\n\t"
        " sbc %3,r1" "\n\t"
        " sbc %4,%0" "\n\t"              /* %4:%3:%2:%9 -= HI(bezier_A) * LO(f) << 8*/
        " mul %10,%6" "\n\t"             /* r1:r0 = LO(bezier_A) * MI(f)*/
        " sub %9,r0" "\n\t"
        " sbc %2,r1" "\n\t"
        " sbc %3,%0" "\n\t"
        " sbc %4,%0" "\n\t"              /* %4:%3:%2:%9 -= LO(bezier_A) * MI(f)*/
        " mul %11,%6" "\n\t"             /* r1:r0 = MI(bezier_A) * MI(f)*/
        " sub %2,r0" "\n\t"
        " sbc %3,r1" "\n\t"
        " sbc %4,%0" "\n\t"              /* %4:%3:%2:%9 -= MI(bezier_A) * MI(f) << 8*/
        " mul %1,%6" "\n\t"              /* r1:r0 = HI(bezier_A) * LO(f)*/
        " sub %3,r0" "\n\t"
        " sbc %4,r1" "\n\t"              /* %4:%3:%2:%9 -= HI(bezier_A) * LO(f) << 16*/
        " jmp 2f" "\n\t"                 /* Done!*/

        "1:" "\n\t"

        /* uint24_t v; */
        /* umul16x24to24hi(v, f, bezier_C); / Range 21bits [29]*/
        /* acc += v; */
        " lds %10, bezier_C" "\n\t"      /* %10 = LO(bezier_C)*/
        " mul %10,%5" "\n\t"             /* r1:r0 = LO(bezier_C) * LO(f)*/
        " add %9,r1" "\n\t"
        " adc %2,%0" "\n\t"
        " adc %3,%0" "\n\t"
        " adc %4,%0" "\n\t"              /* %4:%3:%2:%9 += HI(LO(bezier_C) * LO(f))*/
        " lds %11, bezier_C+1" "\n\t"    /* %11 = MI(bezier_C)*/
        " mul %11,%5" "\n\t"             /* r1:r0 = MI(bezier_C) * LO(f)*/
        " add %9,r0" "\n\t"
        " adc %2,r1" "\n\t"
        " adc %3,%0" "\n\t"
        " adc %4,%0" "\n\t"              /* %4:%3:%2:%9 += MI(bezier_C) * LO(f)*/
        " lds %1, bezier_C+2" "\n\t"     /* %1 = HI(bezier_C)*/
        " mul %1,%5" "\n\t"              /* r1:r0 = MI(bezier_C) * LO(f)*/
        " add %2,r0" "\n\t"
        " adc %3,r1" "\n\t"
        " adc %4,%0" "\n\t"              /* %4:%3:%2:%9 += HI(bezier_C) * LO(f) << 8*/
        " mul %10,%6" "\n\t"             /* r1:r0 = LO(bezier_C) * MI(f)*/
        " add %9,r0" "\n\t"
        " adc %2,r1" "\n\t"
        " adc %3,%0" "\n\t"
        " adc %4,%0" "\n\t"              /* %4:%3:%2:%9 += LO(bezier_C) * MI(f)*/
        " mul %11,%6" "\n\t"             /* r1:r0 = MI(bezier_C) * MI(f)*/
        " add %2,r0" "\n\t"
        " adc %3,r1" "\n\t"
        " adc %4,%0" "\n\t"              /* %4:%3:%2:%9 += MI(bezier_C) * MI(f) << 8*/
        " mul %1,%6" "\n\t"              /* r1:r0 = HI(bezier_C) * LO(f)*/
        " add %3,r0" "\n\t"
        " adc %4,r1" "\n\t"              /* %4:%3:%2:%9 += HI(bezier_C) * LO(f) << 16*/

        /* umul16x16to16hi(f, f, t); / Range 16 bits : f = t^3  (unsigned) [17]*/
        " mul %5,%7" "\n\t"              /* r1:r0 = LO(f) * LO(t)*/
        " mov %1,r1" "\n\t"              /* store MIL(LO(f) * LO(t)) in %1, we need it for rounding*/
        " clr %10" "\n\t"                /* %10 = 0*/
        " clr %11" "\n\t"                /* %11 = 0*/
        " mul %5,%8" "\n\t"              /* r1:r0 = LO(f) * HI(t)*/
        " add %1,r0" "\n\t"              /* %1 += LO(LO(f) * HI(t))*/
        " adc %10,r1" "\n\t"             /* %10 = HI(LO(f) * HI(t))*/
        " adc %11,%0" "\n\t"             /* %11 += carry*/
        " mul %6,%7" "\n\t"              /* r1:r0 = HI(f) * LO(t)*/
        " add %1,r0" "\n\t"              /* %1 += LO(HI(f) * LO(t))*/
        " adc %10,r1" "\n\t"             /* %10 += HI(HI(f) * LO(t))*/
        " adc %11,%0" "\n\t"             /* %11 += carry*/
        " mul %6,%8" "\n\t"              /* r1:r0 = HI(f) * HI(t)*/
        " add %10,r0" "\n\t"             /* %10 += LO(HI(f) * HI(t))*/
        " adc %11,r1" "\n\t"             /* %11 += HI(HI(f) * HI(t))*/
        " mov %5,%10" "\n\t"             /* %6:%5 =*/
        " mov %6,%11" "\n\t"             /* f = %10:%11*/

        /* umul16x24to24hi(v, f, bezier_B); / Range 22bits [29]*/
        /* acc -= v;*/
        " lds %10, bezier_B" "\n\t"      /* %10 = LO(bezier_B)*/
        " mul %10,%5" "\n\t"             /* r1:r0 = LO(bezier_B) * LO(f)*/
        " sub %9,r1" "\n\t"
        " sbc %2,%0" "\n\t"
        " sbc %3,%0" "\n\t"
        " sbc %4,%0" "\n\t"              /* %4:%3:%2:%9 -= HI(LO(bezier_B) * LO(f))*/
        " lds %11, bezier_B+1" "\n\t"    /* %11 = MI(bezier_B)*/
        " mul %11,%5" "\n\t"             /* r1:r0 = MI(bezier_B) * LO(f)*/
        " sub %9,r0" "\n\t"
        " sbc %2,r1" "\n\t"
        " sbc %3,%0" "\n\t"
        " sbc %4,%0" "\n\t"              /* %4:%3:%2:%9 -= MI(bezier_B) * LO(f)*/
        " lds %1, bezier_B+2" "\n\t"     /* %1 = HI(bezier_B)*/
        " mul %1,%5" "\n\t"              /* r1:r0 = MI(bezier_B) * LO(f)*/
        " sub %2,r0" "\n\t"
        " sbc %3,r1" "\n\t"
        " sbc %4,%0" "\n\t"              /* %4:%3:%2:%9 -= HI(bezier_B) * LO(f) << 8*/
        " mul %10,%6" "\n\t"             /* r1:r0 = LO(bezier_B) * MI(f)*/
        " sub %9,r0" "\n\t"
        " sbc %2,r1" "\n\t"
        " sbc %3,%0" "\n\t"
        " sbc %4,%0" "\n\t"              /* %4:%3:%2:%9 -= LO(bezier_B) * MI(f)*/
        " mul %11,%6" "\n\t"             /* r1:r0 = MI(bezier_B) * MI(f)*/
        " sub %2,r0" "\n\t"
        " sbc %3,r1" "\n\t"
        " sbc %4,%0" "\n\t"              /* %4:%3:%2:%9 -= MI(bezier_B) * MI(f) << 8*/
        " mul %1,%6" "\n\t"              /* r1:r0 = HI(bezier_B) * LO(f)*/
        " sub %3,r0" "\n\t"
        " sbc %4,r1" "\n\t"              /* %4:%3:%2:%9 -= HI(bezier_B) * LO(f) << 16*/

        /* umul16x16to16hi(f, f, t); / Range 16 bits : f = t^5  (unsigned) [17]*/
        " mul %5,%7" "\n\t"              /* r1:r0 = LO(f) * LO(t)*/
        " mov %1,r1" "\n\t"              /* store MIL(LO(f) * LO(t)) in %1, we need it for rounding*/
        " clr %10" "\n\t"                /* %10 = 0*/
        " clr %11" "\n\t"                /* %11 = 0*/
        " mul %5,%8" "\n\t"              /* r1:r0 = LO(f) * HI(t)*/
        " add %1,r0" "\n\t"              /* %1 += LO(LO(f) * HI(t))*/
        " adc %10,r1" "\n\t"             /* %10 = HI(LO(f) * HI(t))*/
        " adc %11,%0" "\n\t"             /* %11 += carry*/
        " mul %6,%7" "\n\t"              /* r1:r0 = HI(f) * LO(t)*/
        " add %1,r0" "\n\t"              /* %1 += LO(HI(f) * LO(t))*/
        " adc %10,r1" "\n\t"             /* %10 += HI(HI(f) * LO(t))*/
        " adc %11,%0" "\n\t"             /* %11 += carry*/
        " mul %6,%8" "\n\t"              /* r1:r0 = HI(f) * HI(t)*/
        " add %10,r0" "\n\t"             /* %10 += LO(HI(f) * HI(t))*/
        " adc %11,r1" "\n\t"             /* %11 += HI(HI(f) * HI(t))*/
        " mov %5,%10" "\n\t"             /* %6:%5 =*/
        " mov %6,%11" "\n\t"             /* f = %10:%11*/

        /* umul16x24to24hi(v, f, bezier_A); / Range 21bits [29]*/
        /* acc += v; */
        " lds %10, bezier_A" "\n\t"      /* %10 = LO(bezier_A)*/
        " mul %10,%5" "\n\t"             /* r1:r0 = LO(bezier_A) * LO(f)*/
        " add %9,r1" "\n\t"
        " adc %2,%0" "\n\t"
        " adc %3,%0" "\n\t"
        " adc %4,%0" "\n\t"              /* %4:%3:%2:%9 += HI(LO(bezier_A) * LO(f))*/
        " lds %11, bezier_A+1" "\n\t"    /* %11 = MI(bezier_A)*/
        " mul %11,%5" "\n\t"             /* r1:r0 = MI(bezier_A) * LO(f)*/
        " add %9,r0" "\n\t"
        " adc %2,r1" "\n\t"
        " adc %3,%0" "\n\t"
        " adc %4,%0" "\n\t"              /* %4:%3:%2:%9 += MI(bezier_A) * LO(f)*/
        " lds %1, bezier_A+2" "\n\t"     /* %1 = HI(bezier_A)*/
        " mul %1,%5" "\n\t"              /* r1:r0 = MI(bezier_A) * LO(f)*/
        " add %2,r0" "\n\t"
        " adc %3,r1" "\n\t"
        " adc %4,%0" "\n\t"              /* %4:%3:%2:%9 += HI(bezier_A) * LO(f) << 8*/
        " mul %10,%6" "\n\t"             /* r1:r0 = LO(bezier_A) * MI(f)*/
        " add %9,r0" "\n\t"
        " adc %2,r1" "\n\t"
        " adc %3,%0" "\n\t"
        " adc %4,%0" "\n\t"              /* %4:%3:%2:%9 += LO(bezier_A) * MI(f)*/
        " mul %11,%6" "\n\t"             /* r1:r0 = MI(bezier_A) * MI(f)*/
        " add %2,r0" "\n\t"
        " adc %3,r1" "\n\t"
        " adc %4,%0" "\n\t"              /* %4:%3:%2:%9 += MI(bezier_A) * MI(f) << 8*/
        " mul %1,%6" "\n\t"              /* r1:r0 = HI(bezier_A) * LO(f)*/
        " add %3,r0" "\n\t"
        " adc %4,r1" "\n\t"              /* %4:%3:%2:%9 += HI(bezier_A) * LO(f) << 16*/
        "2:" "\n\t"
        " clr __zero_reg__"              /* C runtime expects r1 = __zero_reg__ = 0 */
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
      #if defined(__ARM__) || defined(__thumb__)

        // For ARM Cortex M3/M4 CPUs, we have the optimized assembler version, that takes 43 cycles to execute
        register uint32_t flo = 0;
        register uint32_t fhi = bezier_AV * curr_step;
        register uint32_t t = fhi;
        register int32_t alo = bezier_F;
        register int32_t ahi = 0;
        register int32_t A = bezier_A;
        register int32_t B = bezier_B;
        register int32_t C = bezier_C;

         __asm__ __volatile__(
          ".syntax unified"                   "\n\t"  // is to prevent CM0,CM1 non-unified syntax
          " lsrs  %[ahi],%[alo],#1"           "\n\t"  // a  = F << 31      1 cycles
          " lsls  %[alo],%[alo],#31"          "\n\t"  //                   1 cycles
          " umull %[flo],%[fhi],%[fhi],%[t]"  "\n\t"  // f *= t            5 cycles [fhi:flo=64bits]
          " umull %[flo],%[fhi],%[fhi],%[t]"  "\n\t"  // f>>=32; f*=t      5 cycles [fhi:flo=64bits]
          " lsrs  %[flo],%[fhi],#1"           "\n\t"  //                   1 cycles [31bits]
          " smlal %[alo],%[ahi],%[flo],%[C]"  "\n\t"  // a+=(f>>33)*C;     5 cycles
          " umull %[flo],%[fhi],%[fhi],%[t]"  "\n\t"  // f>>=32; f*=t      5 cycles [fhi:flo=64bits]
          " lsrs  %[flo],%[fhi],#1"           "\n\t"  //                   1 cycles [31bits]
          " smlal %[alo],%[ahi],%[flo],%[B]"  "\n\t"  // a+=(f>>33)*B;     5 cycles
          " umull %[flo],%[fhi],%[fhi],%[t]"  "\n\t"  // f>>=32; f*=t      5 cycles [fhi:flo=64bits]
          " lsrs  %[flo],%[fhi],#1"           "\n\t"  // f>>=33;           1 cycles [31bits]
          " smlal %[alo],%[ahi],%[flo],%[A]"  "\n\t"  // a+=(f>>33)*A;     5 cycles
          " lsrs  %[alo],%[ahi],#6"           "\n\t"  // a>>=38            1 cycles
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
#endif // BEZIER_JERK_CONTROL

/**
 * Stepper Driver Interrupt
 *
 * Directly pulses the stepper motors at high frequency.
 *
 * AVR :
 * Timer 1 runs at a base frequency of 2MHz, with this ISR using OCR1A compare mode.
 *
 * OCR1A   Frequency
 *     1     2 MHz
 *    50    40 KHz
 *   100    20 KHz - capped max rate
 *   200    10 KHz - nominal max rate
 *  2000     1 KHz - sleep rate
 *  4000   500  Hz - init rate
 */

HAL_STEP_TIMER_ISR {
  HAL_timer_isr_prologue(STEP_TIMER_NUM);

  #if ENABLED(LIN_ADVANCE)
    Stepper::advance_isr_scheduler();
  #else
    Stepper::isr();
  #endif

  HAL_timer_isr_epilogue(STEP_TIMER_NUM);
}

void Stepper::isr() {

  #define ENDSTOP_NOMINAL_OCR_VAL 1500 * HAL_TICKS_PER_US // Check endstops every 1.5ms to guarantee two stepper ISRs within 5ms for BLTouch
  #define OCR_VAL_TOLERANCE        500 * HAL_TICKS_PER_US // First max delay is 2.0ms, last min delay is 0.5ms, all others 1.5ms

  hal_timer_t ocr_val;
  static uint32_t step_remaining = 0;  // SPLIT function always runs.  This allows 16 bit timers to be
                                       // used to generate the stepper ISR.
  #define SPLIT(L) do { \
    if (L > ENDSTOP_NOMINAL_OCR_VAL) { \
      const uint32_t remainder = (uint32_t)L % (ENDSTOP_NOMINAL_OCR_VAL); \
      ocr_val = (remainder < OCR_VAL_TOLERANCE) ? ENDSTOP_NOMINAL_OCR_VAL + remainder : ENDSTOP_NOMINAL_OCR_VAL; \
      step_remaining = (uint32_t)L - ocr_val; \
    } \
    else \
      ocr_val = L;\
  }while(0)

  // Time remaining before the next step?
  if (step_remaining) {

    // Make sure endstops are updated
    if (ENDSTOPS_ENABLED) endstops.update();

    // Next ISR either for endstops or stepping
    ocr_val = step_remaining <= ENDSTOP_NOMINAL_OCR_VAL ? step_remaining : ENDSTOP_NOMINAL_OCR_VAL;
    step_remaining -= ocr_val;
    _NEXT_ISR(ocr_val);

    #if DISABLED(LIN_ADVANCE)
      HAL_timer_restrain(STEP_TIMER_NUM, STEP_TIMER_MIN_INTERVAL * HAL_TICKS_PER_US);
    #endif

    return;
  }

  //
  // When cleaning, discard the current block and run fast
  //
  if (cleaning_buffer_counter) {
    if (cleaning_buffer_counter < 0) {          // Count up for endstop hit
      if (current_block) planner.discard_current_block(); // Discard the active block that led to the trigger
      if (!planner.discard_continued_block())   // Discard next CONTINUED block
        cleaning_buffer_counter = 0;            // Keep discarding until non-CONTINUED
    }
    else {
      planner.discard_current_block();
      --cleaning_buffer_counter;                // Count down for abort print
      #if ENABLED(SD_FINISHED_STEPPERRELEASE) && defined(SD_FINISHED_RELEASECOMMAND)
        if (!cleaning_buffer_counter) enqueue_and_echo_commands_P(PSTR(SD_FINISHED_RELEASECOMMAND));
      #endif
    }
    current_block = NULL;                       // Prep to get a new block after cleaning
    _NEXT_ISR(HAL_STEPPER_TIMER_RATE / 10000);  // Run at max speed - 10 KHz
    return;
  }

  // If there is no current block, attempt to pop one from the buffer
  if (!current_block) {

    // Anything in the buffer?
    if ((current_block = planner.get_current_block())) {

      // Initialize the trapezoid generator from the current block.
      static int8_t last_extruder = -1;

      #if ENABLED(LIN_ADVANCE)
        #if E_STEPPERS > 1
          if (current_block->active_extruder != last_extruder) {
            current_adv_steps = 0; // If the now active extruder wasn't in use during the last move, its pressure is most likely gone.
            LA_active_extruder = current_block->active_extruder;
          }
        #endif

        if ((use_advance_lead = current_block->use_advance_lead)) {
          LA_decelerate_after = current_block->decelerate_after;
          final_adv_steps = current_block->final_adv_steps;
          max_adv_steps = current_block->max_adv_steps;
        }
      #endif

      if (current_block->direction_bits != last_direction_bits || current_block->active_extruder != last_extruder) {
        last_direction_bits = current_block->direction_bits;
        last_extruder = current_block->active_extruder;
        set_directions();
      }

      // No acceleration / deceleration time elapsed so far
      acceleration_time = deceleration_time = 0;

      // No step events completed so far
      step_events_completed = 0;

      // step_rate to timer interval
      OCR1A_nominal = calc_timer_interval(current_block->nominal_rate);

      // make a note of the number of step loops required at nominal speed
      step_loops_nominal = step_loops;

      #if DISABLED(BEZIER_JERK_CONTROL)
        // Set as deceleration point the initial rate of the block
        acc_step_rate = current_block->initial_rate;
      #endif

      #if ENABLED(BEZIER_JERK_CONTROL)
        // Initialize the Bézier speed curve
        _calc_bezier_curve_coeffs(current_block->initial_rate, current_block->cruise_rate, current_block->acceleration_time_inverse);

        // We have not started the 2nd half of the trapezoid
        bezier_2nd_half = false;
      #endif

      // Initialize Bresenham counters to 1/2 the ceiling
      counter_X = counter_Y = counter_Z = counter_E = -(current_block->step_event_count >> 1);
      #if ENABLED(MIXING_EXTRUDER)
        MIXING_STEPPERS_LOOP(i)
          counter_m[i] = -(current_block->mix_event_count[i] >> 1);
      #endif

      #if ENABLED(ENDSTOP_INTERRUPTS_FEATURE)
        e_hit = 2; // Needed for the case an endstop is already triggered before the new move begins.
                   // No 'change' can be detected.
      #endif

      #if ENABLED(Z_LATE_ENABLE)
        // If delayed Z enable, postpone move for 1mS
        if (current_block->steps[Z_AXIS] > 0) {
          enable_Z();
          _NEXT_ISR(HAL_STEPPER_TIMER_RATE / 1000); // Run at slow speed - 1 KHz
          return;
        }
      #endif
    }
    else {
      // If no more queued moves, postpone next check for 1mS
      _NEXT_ISR(HAL_STEPPER_TIMER_RATE / 1000); // Run at slow speed - 1 KHz
      return;
    }
  }

  // Update endstops state, if enabled
  #if ENABLED(ENDSTOP_INTERRUPTS_FEATURE)
    if (e_hit && ENDSTOPS_ENABLED) {
      endstops.update();
      e_hit--;
    }
  #else
    if (ENDSTOPS_ENABLED) endstops.update();
  #endif

  // Take multiple steps per interrupt (For high speed moves)
  bool all_steps_done = false;
  for (uint8_t i = step_loops; i--;) {

    #define _COUNTER(AXIS) counter_## AXIS
    #define _APPLY_STEP(AXIS) AXIS ##_APPLY_STEP
    #define _INVERT_STEP_PIN(AXIS) INVERT_## AXIS ##_STEP_PIN

    // Advance the Bresenham counter; start a pulse if the axis needs a step
    #define PULSE_START(AXIS) do{ \
      _COUNTER(AXIS) += current_block->steps[_AXIS(AXIS)]; \
      if (_COUNTER(AXIS) > 0) { _APPLY_STEP(AXIS)(!_INVERT_STEP_PIN(AXIS), 0); } \
    }while(0)

    // Advance the Bresenham counter; start a pulse if the axis needs a step
    #define STEP_TICK(AXIS) do { \
      if (_COUNTER(AXIS) > 0) { \
        _COUNTER(AXIS) -= current_block->step_event_count; \
        count_position[_AXIS(AXIS)] += count_direction[_AXIS(AXIS)]; \
      } \
    }while(0)

    // Stop an active pulse, if any
    #define PULSE_STOP(AXIS) _APPLY_STEP(AXIS)(_INVERT_STEP_PIN(AXIS), 0)

    /**
     * Estimate the number of cycles that the stepper logic already takes
     * up between the start and stop of the X stepper pulse.
     *
     * Currently this uses very modest estimates of around 5 cycles.
     * True values may be derived by careful testing.
     *
     * Once any delay is added, the cost of the delay code itself
     * may be subtracted from this value to get a more accurate delay.
     * Delays under 20 cycles (1.25µs) will be very accurate, using NOPs.
     * Longer delays use a loop. The resolution is 8 cycles.
     */
    #if HAS_X_STEP
      #define _CYCLE_APPROX_1 5
    #else
      #define _CYCLE_APPROX_1 0
    #endif
    #if ENABLED(X_DUAL_STEPPER_DRIVERS)
      #define _CYCLE_APPROX_2 _CYCLE_APPROX_1 + 4
    #else
      #define _CYCLE_APPROX_2 _CYCLE_APPROX_1
    #endif
    #if HAS_Y_STEP
      #define _CYCLE_APPROX_3 _CYCLE_APPROX_2 + 5
    #else
      #define _CYCLE_APPROX_3 _CYCLE_APPROX_2
    #endif
    #if ENABLED(Y_DUAL_STEPPER_DRIVERS)
      #define _CYCLE_APPROX_4 _CYCLE_APPROX_3 + 4
    #else
      #define _CYCLE_APPROX_4 _CYCLE_APPROX_3
    #endif
    #if HAS_Z_STEP
      #define _CYCLE_APPROX_5 _CYCLE_APPROX_4 + 5
    #else
      #define _CYCLE_APPROX_5 _CYCLE_APPROX_4
    #endif
    #if ENABLED(Z_DUAL_STEPPER_DRIVERS)
      #define _CYCLE_APPROX_6 _CYCLE_APPROX_5 + 4
    #else
      #define _CYCLE_APPROX_6 _CYCLE_APPROX_5
    #endif
    #if DISABLED(LIN_ADVANCE)
      #if ENABLED(MIXING_EXTRUDER)
        #define _CYCLE_APPROX_7 _CYCLE_APPROX_6 + (MIXING_STEPPERS) * 6
      #else
        #define _CYCLE_APPROX_7 _CYCLE_APPROX_6 + 5
      #endif
    #else
      #define _CYCLE_APPROX_7 _CYCLE_APPROX_6
    #endif

    #define CYCLES_EATEN_XYZE _CYCLE_APPROX_7
    #define EXTRA_CYCLES_XYZE (STEP_PULSE_CYCLES - (CYCLES_EATEN_XYZE))

    /**
     * If a minimum pulse time was specified get the timer 0 value.
     *
     * On AVR the TCNT0 timer has an 8x prescaler, so it increments every 8 cycles.
     * That's every 0.5µs on 16MHz and every 0.4µs on 20MHz.
     * 20 counts of TCNT0 -by itself- is a good pulse delay.
     * 10µs = 160 or 200 cycles.
     */
    #if EXTRA_CYCLES_XYZE > 20
      hal_timer_t pulse_start = HAL_timer_get_count(PULSE_TIMER_NUM);
    #endif

    #if HAS_X_STEP
      PULSE_START(X);
    #endif
    #if HAS_Y_STEP
      PULSE_START(Y);
    #endif
    #if HAS_Z_STEP
      PULSE_START(Z);
    #endif

    #if ENABLED(LIN_ADVANCE)
      counter_E += current_block->steps[E_AXIS];
      if (counter_E > 0) {
        #if DISABLED(MIXING_EXTRUDER)
          // Don't step E here for mixing extruder
          motor_direction(E_AXIS) ? --e_steps : ++e_steps;
        #endif
      }

      #if ENABLED(MIXING_EXTRUDER)
        // Step mixing steppers proportionally
        const bool dir = motor_direction(E_AXIS);
        MIXING_STEPPERS_LOOP(j) {
          counter_m[j] += current_block->steps[E_AXIS];
          if (counter_m[j] > 0) {
            counter_m[j] -= current_block->mix_event_count[j];
            dir ? --e_steps[j] : ++e_steps[j];
          }
        }
      #endif

    #else // !LIN_ADVANCE - use linear interpolation for E also

      #if ENABLED(MIXING_EXTRUDER)
        // Keep updating the single E axis
        counter_E += current_block->steps[E_AXIS];
        // Tick the counters used for this mix
        MIXING_STEPPERS_LOOP(j) {
          // Step mixing steppers (proportionally)
          counter_m[j] += current_block->steps[E_AXIS];
          // Step when the counter goes over zero
          if (counter_m[j] > 0) En_STEP_WRITE(j, !INVERT_E_STEP_PIN);
        }
      #else // !MIXING_EXTRUDER
        PULSE_START(E);
      #endif
    #endif // !LIN_ADVANCE

    #if HAS_X_STEP
      STEP_TICK(X);
    #endif
    #if HAS_Y_STEP
      STEP_TICK(Y);
    #endif
    #if HAS_Z_STEP
      STEP_TICK(Z);
    #endif

    STEP_TICK(E); // Always tick the single E axis

    // For minimum pulse time wait before stopping pulses
    #if EXTRA_CYCLES_XYZE > 20
      while (EXTRA_CYCLES_XYZE > (uint32_t)(HAL_timer_get_count(PULSE_TIMER_NUM) - pulse_start) * (PULSE_TIMER_PRESCALE)) { /* nada */ }
      pulse_start = HAL_timer_get_count(PULSE_TIMER_NUM);
    #elif EXTRA_CYCLES_XYZE > 0
      DELAY_NOPS(EXTRA_CYCLES_XYZE);
    #endif

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
        MIXING_STEPPERS_LOOP(j) {
          if (counter_m[j] > 0) {
            counter_m[j] -= current_block->mix_event_count[j];
            En_STEP_WRITE(j, INVERT_E_STEP_PIN);
          }
        }
      #else // !MIXING_EXTRUDER
        PULSE_STOP(E);
      #endif
    #endif // !LIN_ADVANCE

    if (++step_events_completed >= current_block->step_event_count) {
      all_steps_done = true;
      break;
    }

    // For minimum pulse time wait after stopping pulses also
    #if EXTRA_CYCLES_XYZE > 20
      if (i) while (EXTRA_CYCLES_XYZE > (uint32_t)(HAL_timer_get_count(PULSE_TIMER_NUM) - pulse_start) * (PULSE_TIMER_PRESCALE)) { /* nada */ }
    #elif EXTRA_CYCLES_XYZE > 0
      if (i) DELAY_NOPS(EXTRA_CYCLES_XYZE);
    #endif

  } // steps_loop

  // Calculate new timer value
  if (step_events_completed <= (uint32_t)current_block->accelerate_until) {

    #if ENABLED(BEZIER_JERK_CONTROL)
      // Get the next speed to use (Jerk limited!)
      hal_timer_t acc_step_rate =
        acceleration_time < current_block->acceleration_time
          ? _eval_bezier_curve(acceleration_time)
          : current_block->cruise_rate;
    #else
      #ifdef CPU_32_BIT
        MultiU32X24toH32(acc_step_rate, acceleration_time, current_block->acceleration_rate);
      #else
        MultiU24X32toH16(acc_step_rate, acceleration_time, current_block->acceleration_rate);
      #endif
      acc_step_rate += current_block->initial_rate;

      // upper limit
      NOMORE(acc_step_rate, current_block->nominal_rate);
    #endif

    // step_rate to timer interval
    const hal_timer_t interval = calc_timer_interval(acc_step_rate);

    SPLIT(interval);  // split step into multiple ISRs if larger than ENDSTOP_NOMINAL_OCR_VAL
    _NEXT_ISR(ocr_val);

    acceleration_time += interval;

    #if ENABLED(LIN_ADVANCE)
      if (current_block->use_advance_lead) {
        if (step_events_completed == step_loops || (e_steps && eISR_Rate != current_block->advance_speed)) {
          nextAdvanceISR = 0; // Wake up eISR on first acceleration loop and fire ISR if final adv_rate is reached
          eISR_Rate = current_block->advance_speed;
        }
      }
      else {
        eISR_Rate = ADV_NEVER;
        if (e_steps) nextAdvanceISR = 0;
      }
    #endif // LIN_ADVANCE
  }
  else if (step_events_completed > (uint32_t)current_block->decelerate_after) {
    hal_timer_t step_rate;

    #if ENABLED(BEZIER_JERK_CONTROL)
      // If this is the 1st time we process the 2nd half of the trapezoid...
      if (!bezier_2nd_half) {

        // Initialize the Bézier speed curve
        _calc_bezier_curve_coeffs(current_block->cruise_rate, current_block->final_rate, current_block->deceleration_time_inverse);
        bezier_2nd_half = true;
      }

      // Calculate the next speed to use
      step_rate = deceleration_time < current_block->deceleration_time
        ? _eval_bezier_curve(deceleration_time)
        : current_block->final_rate;
    #else

      // Using the old trapezoidal control
      #ifdef CPU_32_BIT
        MultiU32X24toH32(step_rate, deceleration_time, current_block->acceleration_rate);
      #else
        MultiU24X32toH16(step_rate, deceleration_time, current_block->acceleration_rate);
      #endif

      if (step_rate < acc_step_rate) { // Still decelerating?
        step_rate = acc_step_rate - step_rate;
        NOLESS(step_rate, current_block->final_rate);
      }
      else
        step_rate = current_block->final_rate;
    #endif

    // step_rate to timer interval
    const hal_timer_t interval = calc_timer_interval(step_rate);

    SPLIT(interval);  // split step into multiple ISRs if larger than ENDSTOP_NOMINAL_OCR_VAL
    _NEXT_ISR(ocr_val);

    deceleration_time += interval;

    #if ENABLED(LIN_ADVANCE)
      if (current_block->use_advance_lead) {
        if (step_events_completed <= (uint32_t)current_block->decelerate_after + step_loops || (e_steps && eISR_Rate != current_block->advance_speed)) {
          nextAdvanceISR = 0; // Wake up eISR on first deceleration loop
          eISR_Rate = current_block->advance_speed;
        }
      }
      else {
        eISR_Rate = ADV_NEVER;
        if (e_steps) nextAdvanceISR = 0;
      }
    #endif // LIN_ADVANCE
  }
  else {

    #if ENABLED(LIN_ADVANCE)
      // If we have esteps to execute, fire the next advance_isr "now"
      if (e_steps && eISR_Rate != current_block->advance_speed) nextAdvanceISR = 0;
    #endif

    SPLIT(OCR1A_nominal);  // split step into multiple ISRs if larger than ENDSTOP_NOMINAL_OCR_VAL
    _NEXT_ISR(ocr_val);

    // ensure we're running at the correct step rate, even if we just came off an acceleration
    step_loops = step_loops_nominal;
  }

  #if DISABLED(LIN_ADVANCE)
    // Make sure stepper ISR doesn't monopolize the CPU
    HAL_timer_restrain(STEP_TIMER_NUM, STEP_TIMER_MIN_INTERVAL * HAL_TICKS_PER_US);
  #endif

  // If current block is finished, reset pointer
  if (all_steps_done) {
    current_block = NULL;
    planner.discard_current_block();
  }
}

#if ENABLED(LIN_ADVANCE)

  #define CYCLES_EATEN_E (E_STEPPERS * 5)
  #define EXTRA_CYCLES_E (STEP_PULSE_CYCLES - (CYCLES_EATEN_E))

  // Timer interrupt for E. e_steps is set in the main routine;

  void Stepper::advance_isr() {

    #if ENABLED(MK2_MULTIPLEXER) // For SNMM even-numbered steppers are reversed
      #define SET_E_STEP_DIR(INDEX) do{ if (e_steps) E0_DIR_WRITE(e_steps < 0 ? !INVERT_E## INDEX ##_DIR ^ TEST(INDEX, 0) : INVERT_E## INDEX ##_DIR ^ TEST(INDEX, 0)); }while(0)
    #elif ENABLED(DUAL_X_CARRIAGE) || ENABLED(DUAL_NOZZLE_DUPLICATION_MODE)
      #define SET_E_STEP_DIR(INDEX) do{ if (e_steps) { if (e_steps < 0) REV_E_DIR(); else NORM_E_DIR(); } }while(0)
    #else
      #define SET_E_STEP_DIR(INDEX) do{ if (e_steps) E## INDEX ##_DIR_WRITE(e_steps < 0 ? INVERT_E## INDEX ##_DIR : !INVERT_E## INDEX ##_DIR); }while(0)
    #endif

    #if ENABLED(DUAL_X_CARRIAGE) || ENABLED(DUAL_NOZZLE_DUPLICATION_MODE)
      #define START_E_PULSE(INDEX) do{ if (e_steps) E_STEP_WRITE(!INVERT_E_STEP_PIN); }while(0)
      #define STOP_E_PULSE(INDEX) do{ if (e_steps) { E_STEP_WRITE(INVERT_E_STEP_PIN); e_steps < 0 ? ++e_steps : --e_steps; } }while(0)
    #else
      #define START_E_PULSE(INDEX) do{ if (e_steps) E## INDEX ##_STEP_WRITE(!INVERT_E_STEP_PIN); }while(0)
      #define STOP_E_PULSE(INDEX) do { if (e_steps) { e_steps < 0 ? ++e_steps : --e_steps; E## INDEX ##_STEP_WRITE(INVERT_E_STEP_PIN); } }while(0)
    #endif

    if (use_advance_lead) {
      if (step_events_completed > LA_decelerate_after && current_adv_steps > final_adv_steps) {
        e_steps--;
        current_adv_steps--;
        nextAdvanceISR = eISR_Rate;
      }
      else if (step_events_completed < LA_decelerate_after && current_adv_steps < max_adv_steps) {
             //step_events_completed <= (uint32_t)current_block->accelerate_until) {
        e_steps++;
        current_adv_steps++;
        nextAdvanceISR = eISR_Rate;
      }
      else {
        nextAdvanceISR = ADV_NEVER;
        eISR_Rate = ADV_NEVER;
      }
    }
    else
      nextAdvanceISR = ADV_NEVER;

    switch (LA_active_extruder) {
      case 0: SET_E_STEP_DIR(0); break;
      #if EXTRUDERS > 1
        case 1: SET_E_STEP_DIR(1); break;
        #if EXTRUDERS > 2
          case 2: SET_E_STEP_DIR(2); break;
          #if EXTRUDERS > 3
            case 3: SET_E_STEP_DIR(3); break;
            #if EXTRUDERS > 4
              case 4: SET_E_STEP_DIR(4); break;
            #endif // EXTRUDERS > 4
          #endif // EXTRUDERS > 3
        #endif // EXTRUDERS > 2
      #endif // EXTRUDERS > 1
    }

    // Step E stepper if we have steps
    while (e_steps) {

      #if EXTRA_CYCLES_E > 20
        hal_timer_t pulse_start = HAL_timer_get_count(PULSE_TIMER_NUM);
      #endif

      switch (LA_active_extruder) {
        case 0: START_E_PULSE(0); break;
        #if EXTRUDERS > 1
          case 1: START_E_PULSE(1); break;
          #if EXTRUDERS > 2
            case 2: START_E_PULSE(2); break;
            #if EXTRUDERS > 3
              case 3: START_E_PULSE(3); break;
              #if EXTRUDERS > 4
                case 4: START_E_PULSE(4); break;
              #endif // EXTRUDERS > 4
            #endif // EXTRUDERS > 3
          #endif // EXTRUDERS > 2
        #endif // EXTRUDERS > 1
      }

      // For minimum pulse time wait before stopping pulses
      #if EXTRA_CYCLES_E > 20
        while (EXTRA_CYCLES_E > (hal_timer_t)(HAL_timer_get_count(PULSE_TIMER_NUM) - pulse_start) * (PULSE_TIMER_PRESCALE)) { /* nada */ }
        pulse_start = HAL_timer_get_count(PULSE_TIMER_NUM);
      #elif EXTRA_CYCLES_E > 0
        DELAY_NOPS(EXTRA_CYCLES_E);
      #endif

      switch (LA_active_extruder) {
        case 0: STOP_E_PULSE(0); break;
        #if EXTRUDERS > 1
          case 1: STOP_E_PULSE(1); break;
          #if EXTRUDERS > 2
            case 2: STOP_E_PULSE(2); break;
            #if EXTRUDERS > 3
              case 3: STOP_E_PULSE(3); break;
              #if EXTRUDERS > 4
                case 4: STOP_E_PULSE(4); break;
              #endif // EXTRUDERS > 4
            #endif // EXTRUDERS > 3
          #endif // EXTRUDERS > 2
        #endif // EXTRUDERS > 1
      }

      // For minimum pulse time wait before looping
      #if EXTRA_CYCLES_E > 20
        if (e_steps) while (EXTRA_CYCLES_E > (hal_timer_t)(HAL_timer_get_count(PULSE_TIMER_NUM) - pulse_start) * (PULSE_TIMER_PRESCALE)) { /* nada */ }
      #elif EXTRA_CYCLES_E > 0
        if (e_steps) DELAY_NOPS(EXTRA_CYCLES_E);
      #endif

    } // e_steps
  }

  void Stepper::advance_isr_scheduler() {

    // Run main stepping ISR if flagged
    if (!nextMainISR) isr();

    // Run Advance stepping ISR if flagged
    if (!nextAdvanceISR) advance_isr();

    // Is the next advance ISR scheduled before the next main ISR?
    if (nextAdvanceISR <= nextMainISR) {
      // Set up the next interrupt
      HAL_timer_set_compare(STEP_TIMER_NUM, nextAdvanceISR);
      // New interval for the next main ISR
      if (nextMainISR) nextMainISR -= nextAdvanceISR;
      // Will call Stepper::advance_isr on the next interrupt
      nextAdvanceISR = 0;
    }
    else {
      // The next main ISR comes first
      HAL_timer_set_compare(STEP_TIMER_NUM, nextMainISR);
      // New interval for the next advance ISR, if any
      if (nextAdvanceISR && nextAdvanceISR != ADV_NEVER)
        nextAdvanceISR -= nextMainISR;
      // Will call Stepper::isr on the next interrupt
      nextMainISR = 0;
    }

    // Make sure stepper ISR doesn't monopolize the CPU
    HAL_timer_restrain(STEP_TIMER_NUM, STEP_TIMER_MIN_INTERVAL * HAL_TICKS_PER_US);
  }

#endif // LIN_ADVANCE

void Stepper::init() {

  // Init Digipot Motor Current
  #if HAS_DIGIPOTSS || HAS_MOTOR_CURRENT_PWM
    digipot_init();
  #endif

  #if MB(ALLIGATOR)
    const float motor_current[] = MOTOR_CURRENT;
    unsigned int digipot_motor = 0;
    for (uint8_t i = 0; i < 3 + EXTRUDERS; i++) {
      digipot_motor = 255 * (motor_current[i] / 2.5);
      dac084s085::setValue(i, digipot_motor);
    }
  #endif//MB(ALLIGATOR)

  // Init Microstepping Pins
  #if HAS_MICROSTEPS
    microstep_init();
  #endif

  // Init Dir Pins
  #if HAS_X_DIR
    X_DIR_INIT;
  #endif
  #if HAS_X2_DIR
    X2_DIR_INIT;
  #endif
  #if HAS_Y_DIR
    Y_DIR_INIT;
    #if ENABLED(Y_DUAL_STEPPER_DRIVERS) && HAS_Y2_DIR
      Y2_DIR_INIT;
    #endif
  #endif
  #if HAS_Z_DIR
    Z_DIR_INIT;
    #if ENABLED(Z_DUAL_STEPPER_DRIVERS) && HAS_Z2_DIR
      Z2_DIR_INIT;
    #endif
  #endif
  #if HAS_E0_DIR
    E0_DIR_INIT;
  #endif
  #if HAS_E1_DIR
    E1_DIR_INIT;
  #endif
  #if HAS_E2_DIR
    E2_DIR_INIT;
  #endif
  #if HAS_E3_DIR
    E3_DIR_INIT;
  #endif
  #if HAS_E4_DIR
    E4_DIR_INIT;
  #endif

  // Init Enable Pins - steppers default to disabled.
  #if HAS_X_ENABLE
    X_ENABLE_INIT;
    if (!X_ENABLE_ON) X_ENABLE_WRITE(HIGH);
    #if (ENABLED(DUAL_X_CARRIAGE) || ENABLED(X_DUAL_STEPPER_DRIVERS)) && HAS_X2_ENABLE
      X2_ENABLE_INIT;
      if (!X_ENABLE_ON) X2_ENABLE_WRITE(HIGH);
    #endif
  #endif
  #if HAS_Y_ENABLE
    Y_ENABLE_INIT;
    if (!Y_ENABLE_ON) Y_ENABLE_WRITE(HIGH);
    #if ENABLED(Y_DUAL_STEPPER_DRIVERS) && HAS_Y2_ENABLE
      Y2_ENABLE_INIT;
      if (!Y_ENABLE_ON) Y2_ENABLE_WRITE(HIGH);
    #endif
  #endif
  #if HAS_Z_ENABLE
    Z_ENABLE_INIT;
    if (!Z_ENABLE_ON) Z_ENABLE_WRITE(HIGH);
    #if ENABLED(Z_DUAL_STEPPER_DRIVERS) && HAS_Z2_ENABLE
      Z2_ENABLE_INIT;
      if (!Z_ENABLE_ON) Z2_ENABLE_WRITE(HIGH);
    #endif
  #endif
  #if HAS_E0_ENABLE
    E0_ENABLE_INIT;
    if (!E_ENABLE_ON) E0_ENABLE_WRITE(HIGH);
  #endif
  #if HAS_E1_ENABLE
    E1_ENABLE_INIT;
    if (!E_ENABLE_ON) E1_ENABLE_WRITE(HIGH);
  #endif
  #if HAS_E2_ENABLE
    E2_ENABLE_INIT;
    if (!E_ENABLE_ON) E2_ENABLE_WRITE(HIGH);
  #endif
  #if HAS_E3_ENABLE
    E3_ENABLE_INIT;
    if (!E_ENABLE_ON) E3_ENABLE_WRITE(HIGH);
  #endif
  #if HAS_E4_ENABLE
    E4_ENABLE_INIT;
    if (!E_ENABLE_ON) E4_ENABLE_WRITE(HIGH);
  #endif

  // Init endstops and pullups
  endstops.init();

  #define _STEP_INIT(AXIS) AXIS ##_STEP_INIT
  #define _WRITE_STEP(AXIS, HIGHLOW) AXIS ##_STEP_WRITE(HIGHLOW)
  #define _DISABLE(AXIS) disable_## AXIS()

  #define AXIS_INIT(AXIS, PIN) \
    _STEP_INIT(AXIS); \
    _WRITE_STEP(AXIS, _INVERT_STEP_PIN(PIN)); \
    _DISABLE(AXIS)

  #define E_AXIS_INIT(NUM) AXIS_INIT(E## NUM, E)

  // Init Step Pins
  #if HAS_X_STEP
    #if ENABLED(X_DUAL_STEPPER_DRIVERS) || ENABLED(DUAL_X_CARRIAGE)
      X2_STEP_INIT;
      X2_STEP_WRITE(INVERT_X_STEP_PIN);
    #endif
    AXIS_INIT(X, X);
  #endif

  #if HAS_Y_STEP
    #if ENABLED(Y_DUAL_STEPPER_DRIVERS)
      Y2_STEP_INIT;
      Y2_STEP_WRITE(INVERT_Y_STEP_PIN);
    #endif
    AXIS_INIT(Y, Y);
  #endif

  #if HAS_Z_STEP
    #if ENABLED(Z_DUAL_STEPPER_DRIVERS)
      Z2_STEP_INIT;
      Z2_STEP_WRITE(INVERT_Z_STEP_PIN);
    #endif
    AXIS_INIT(Z, Z);
  #endif

  #if HAS_E0_STEP
    E_AXIS_INIT(0);
  #endif
  #if HAS_E1_STEP
    E_AXIS_INIT(1);
  #endif
  #if HAS_E2_STEP
    E_AXIS_INIT(2);
  #endif
  #if HAS_E3_STEP
    E_AXIS_INIT(3);
  #endif
  #if HAS_E4_STEP
    E_AXIS_INIT(4);
  #endif

  #ifdef __AVR__
    // waveform generation = 0100 = CTC
    SET_WGM(1, CTC_OCRnA);

    // output mode = 00 (disconnected)
    SET_COMA(1, NORMAL);

    // Set the timer pre-scaler
    // Generally we use a divider of 8, resulting in a 2MHz timer
    // frequency on a 16MHz MCU. If you are going to change this, be
    // sure to regenerate speed_lookuptable.h with
    // create_speed_lookuptable.py
    SET_CS(1, PRESCALER_8);  //  CS 2 = 1/8 prescaler

    // Init Stepper ISR to 122 Hz for quick starting
    OCR1A = 0x4000;
    TCNT1 = 0;
  #else
    // Init Stepper ISR to 122 Hz for quick starting
    HAL_timer_start(STEP_TIMER_NUM, 122);
  #endif

  ENABLE_STEPPER_DRIVER_INTERRUPT();

  endstops.enable(true); // Start with endstops active. After homing they can be disabled
  sei();

  set_directions(); // Init directions to last_direction_bits = 0
}


/**
 * Block until all buffered steps are executed / cleaned
 */
void Stepper::synchronize() { while (planner.has_blocks_queued() || cleaning_buffer_counter) idle(); }

/**
 * Set the stepper positions directly in steps
 *
 * The input is based on the typical per-axis XYZ steps.
 * For CORE machines XYZ needs to be translated to ABC.
 *
 * This allows get_axis_position_mm to correctly
 * derive the current XYZ position later on.
 */
void Stepper::set_position(const long &a, const long &b, const long &c, const long &e) {

  synchronize(); // Bad to set stepper counts in the middle of a move

  CRITICAL_SECTION_START;

  #if CORE_IS_XY
    // corexy positioning
    // these equations follow the form of the dA and dB equations on http://www.corexy.com/theory.html
    count_position[A_AXIS] = a + b;
    count_position[B_AXIS] = CORESIGN(a - b);
    count_position[Z_AXIS] = c;
  #elif CORE_IS_XZ
    // corexz planning
    count_position[A_AXIS] = a + c;
    count_position[Y_AXIS] = b;
    count_position[C_AXIS] = CORESIGN(a - c);
  #elif CORE_IS_YZ
    // coreyz planning
    count_position[X_AXIS] = a;
    count_position[B_AXIS] = b + c;
    count_position[C_AXIS] = CORESIGN(b - c);
  #else
    // default non-h-bot planning
    count_position[X_AXIS] = a;
    count_position[Y_AXIS] = b;
    count_position[Z_AXIS] = c;
  #endif

  count_position[E_AXIS] = e;
  CRITICAL_SECTION_END;
}

void Stepper::set_position(const AxisEnum &axis, const long &v) {
  CRITICAL_SECTION_START;
  count_position[axis] = v;
  CRITICAL_SECTION_END;
}

void Stepper::set_e_position(const long &e) {
  CRITICAL_SECTION_START;
  count_position[E_AXIS] = e;
  CRITICAL_SECTION_END;
}

/**
 * Get a stepper's position in steps.
 */
long Stepper::position(const AxisEnum axis) {
  CRITICAL_SECTION_START;
  const long count_pos = count_position[axis];
  CRITICAL_SECTION_END;
  return count_pos;
}

/**
 * Get an axis position according to stepper position(s)
 * For CORE machines apply translation from ABC to XYZ.
 */
float Stepper::get_axis_position_mm(const AxisEnum axis) {
  float axis_steps;
  #if IS_CORE
    // Requesting one of the "core" axes?
    if (axis == CORE_AXIS_1 || axis == CORE_AXIS_2) {
      CRITICAL_SECTION_START;
      // ((a1+a2)+(a1-a2))/2 -> (a1+a2+a1-a2)/2 -> (a1+a1)/2 -> a1
      // ((a1+a2)-(a1-a2))/2 -> (a1+a2-a1+a2)/2 -> (a2+a2)/2 -> a2
      axis_steps = 0.5f * (
        axis == CORE_AXIS_2 ? CORESIGN(count_position[CORE_AXIS_1] - count_position[CORE_AXIS_2])
                            : count_position[CORE_AXIS_1] + count_position[CORE_AXIS_2]
      );
      CRITICAL_SECTION_END;
    }
    else
      axis_steps = position(axis);
  #else
    axis_steps = position(axis);
  #endif
  return axis_steps * planner.steps_to_mm[axis];
}

void Stepper::finish_and_disable() {
  synchronize();
  disable_all_steppers();
}

void Stepper::quick_stop() {
  DISABLE_STEPPER_DRIVER_INTERRUPT();
  kill_current_block();
  current_block = NULL;
  cleaning_buffer_counter = 5000;
  planner.clear_block_buffer();
  ENABLE_STEPPER_DRIVER_INTERRUPT();
  #if ENABLED(ULTRA_LCD)
    planner.clear_block_buffer_runtime();
  #endif
}

void Stepper::endstop_triggered(const AxisEnum axis) {

  #if IS_CORE

    endstops_trigsteps[axis] = 0.5f * (
      axis == CORE_AXIS_2 ? CORESIGN(count_position[CORE_AXIS_1] - count_position[CORE_AXIS_2])
                          : count_position[CORE_AXIS_1] + count_position[CORE_AXIS_2]
    );

  #else // !COREXY && !COREXZ && !COREYZ

    endstops_trigsteps[axis] = count_position[axis];

  #endif // !COREXY && !COREXZ && !COREYZ

  kill_current_block();
  cleaning_buffer_counter = -1; // Discard the rest of the move
}

void Stepper::report_positions() {
  CRITICAL_SECTION_START;
  const long xpos = count_position[X_AXIS],
             ypos = count_position[Y_AXIS],
             zpos = count_position[Z_AXIS];
  CRITICAL_SECTION_END;

  #if CORE_IS_XY || CORE_IS_XZ || IS_DELTA || IS_SCARA
    SERIAL_PROTOCOLPGM(MSG_COUNT_A);
  #else
    SERIAL_PROTOCOLPGM(MSG_COUNT_X);
  #endif
  SERIAL_PROTOCOL(xpos);

  #if CORE_IS_XY || CORE_IS_YZ || IS_DELTA || IS_SCARA
    SERIAL_PROTOCOLPGM(" B:");
  #else
    SERIAL_PROTOCOLPGM(" Y:");
  #endif
  SERIAL_PROTOCOL(ypos);

  #if CORE_IS_XZ || CORE_IS_YZ || IS_DELTA
    SERIAL_PROTOCOLPGM(" C:");
  #else
    SERIAL_PROTOCOLPGM(" Z:");
  #endif
  SERIAL_PROTOCOL(zpos);

  SERIAL_EOL();
}

#if ENABLED(BABYSTEPPING)

  #if ENABLED(DELTA)
    #define CYCLES_EATEN_BABYSTEP (2 * 15)
  #else
    #define CYCLES_EATEN_BABYSTEP 0
  #endif
  #define EXTRA_CYCLES_BABYSTEP (STEP_PULSE_CYCLES - (CYCLES_EATEN_BABYSTEP))

  #define _ENABLE(AXIS) enable_## AXIS()
  #define _READ_DIR(AXIS) AXIS ##_DIR_READ
  #define _INVERT_DIR(AXIS) INVERT_## AXIS ##_DIR
  #define _APPLY_DIR(AXIS, INVERT) AXIS ##_APPLY_DIR(INVERT, true)

  #if EXTRA_CYCLES_BABYSTEP > 20
    #define _SAVE_START const hal_timer_t pulse_start = HAL_timer_get_count(STEP_TIMER_NUM)
    #define _PULSE_WAIT while (EXTRA_CYCLES_BABYSTEP > (uint32_t)(HAL_timer_get_count(STEP_TIMER_NUM) - pulse_start) * (PULSE_TIMER_PRESCALE)) { /* nada */ }
  #else
    #define _SAVE_START NOOP
    #if EXTRA_CYCLES_BABYSTEP > 0
      #define _PULSE_WAIT DELAY_NOPS(EXTRA_CYCLES_BABYSTEP)
    #elif STEP_PULSE_CYCLES > 0
      #define _PULSE_WAIT NOOP
    #elif ENABLED(DELTA)
      #define _PULSE_WAIT delayMicroseconds(2);
    #else
      #define _PULSE_WAIT delayMicroseconds(4);
    #endif
  #endif

  #define BABYSTEP_AXIS(AXIS, INVERT, DIR) {            \
      const uint8_t old_dir = _READ_DIR(AXIS);          \
      _ENABLE(AXIS);                                    \
      _SAVE_START;                                      \
      _APPLY_DIR(AXIS, _INVERT_DIR(AXIS)^DIR^INVERT);   \
      _PULSE_WAIT;                                      \
      _APPLY_STEP(AXIS)(!_INVERT_STEP_PIN(AXIS), true); \
      _PULSE_WAIT;                                      \
      _APPLY_STEP(AXIS)(_INVERT_STEP_PIN(AXIS), true);  \
      _APPLY_DIR(AXIS, old_dir);                        \
    }

  // MUST ONLY BE CALLED BY AN ISR,
  // No other ISR should ever interrupt this!
  void Stepper::babystep(const AxisEnum axis, const bool direction) {
    cli();

    switch (axis) {

      #if ENABLED(BABYSTEP_XY)

        case X_AXIS:
          #if CORE_IS_XY
            BABYSTEP_AXIS(X, false, direction);
            BABYSTEP_AXIS(Y, false, direction);
          #elif CORE_IS_XZ
            BABYSTEP_AXIS(X, false, direction);
            BABYSTEP_AXIS(Z, false, direction);
          #else
            BABYSTEP_AXIS(X, false, direction);
          #endif
          break;

        case Y_AXIS:
          #if CORE_IS_XY
            BABYSTEP_AXIS(X, false, direction);
            BABYSTEP_AXIS(Y, false, direction^(CORESIGN(1)<0));
          #elif CORE_IS_YZ
            BABYSTEP_AXIS(Y, false, direction);
            BABYSTEP_AXIS(Z, false, direction^(CORESIGN(1)<0));
          #else
            BABYSTEP_AXIS(Y, false, direction);
          #endif
          break;

      #endif

      case Z_AXIS: {

        #if CORE_IS_XZ
          BABYSTEP_AXIS(X, BABYSTEP_INVERT_Z, direction);
          BABYSTEP_AXIS(Z, BABYSTEP_INVERT_Z, direction^(CORESIGN(1)<0));

        #elif CORE_IS_YZ
          BABYSTEP_AXIS(Y, BABYSTEP_INVERT_Z, direction);
          BABYSTEP_AXIS(Z, BABYSTEP_INVERT_Z, direction^(CORESIGN(1)<0));

        #elif DISABLED(DELTA)
          BABYSTEP_AXIS(Z, BABYSTEP_INVERT_Z, direction);

        #else // DELTA

          const bool z_direction = direction ^ BABYSTEP_INVERT_Z;

          enable_X();
          enable_Y();
          enable_Z();

          const uint8_t old_x_dir_pin = X_DIR_READ,
                        old_y_dir_pin = Y_DIR_READ,
                        old_z_dir_pin = Z_DIR_READ;

          X_DIR_WRITE(INVERT_X_DIR ^ z_direction);
          Y_DIR_WRITE(INVERT_Y_DIR ^ z_direction);
          Z_DIR_WRITE(INVERT_Z_DIR ^ z_direction);

          _SAVE_START;

          X_STEP_WRITE(!INVERT_X_STEP_PIN);
          Y_STEP_WRITE(!INVERT_Y_STEP_PIN);
          Z_STEP_WRITE(!INVERT_Z_STEP_PIN);

          _PULSE_WAIT;

          X_STEP_WRITE(INVERT_X_STEP_PIN);
          Y_STEP_WRITE(INVERT_Y_STEP_PIN);
          Z_STEP_WRITE(INVERT_Z_STEP_PIN);

          // Restore direction bits
          X_DIR_WRITE(old_x_dir_pin);
          Y_DIR_WRITE(old_y_dir_pin);
          Z_DIR_WRITE(old_z_dir_pin);

        #endif

      } break;

      default: break;
    }
    sei();
  }

#endif // BABYSTEPPING

/**
 * Software-controlled Stepper Motor Current
 */

#if HAS_DIGIPOTSS

  // From Arduino DigitalPotControl example
  void Stepper::digitalPotWrite(const int16_t address, const int16_t value) {
    WRITE(DIGIPOTSS_PIN, LOW);  // Take the SS pin low to select the chip
    SPI.transfer(address);      // Send the address and value via SPI
    SPI.transfer(value);
    WRITE(DIGIPOTSS_PIN, HIGH); // Take the SS pin high to de-select the chip
    //delay(10);
  }

#endif // HAS_DIGIPOTSS

#if HAS_MOTOR_CURRENT_PWM

  void Stepper::refresh_motor_power() {
    for (uint8_t i = 0; i < COUNT(motor_current_setting); ++i) {
      switch (i) {
        #if PIN_EXISTS(MOTOR_CURRENT_PWM_XY)
          case 0:
        #endif
        #if PIN_EXISTS(MOTOR_CURRENT_PWM_Z)
          case 1:
        #endif
        #if PIN_EXISTS(MOTOR_CURRENT_PWM_E)
          case 2:
        #endif
            digipot_current(i, motor_current_setting[i]);
        default: break;
      }
    }
  }

#endif // HAS_MOTOR_CURRENT_PWM

#if HAS_DIGIPOTSS || HAS_MOTOR_CURRENT_PWM

  void Stepper::digipot_current(const uint8_t driver, const int current) {

    #if HAS_DIGIPOTSS

      const uint8_t digipot_ch[] = DIGIPOT_CHANNELS;
      digitalPotWrite(digipot_ch[driver], current);

    #elif HAS_MOTOR_CURRENT_PWM

      if (WITHIN(driver, 0, 2))
        motor_current_setting[driver] = current; // update motor_current_setting

      #define _WRITE_CURRENT_PWM(P) analogWrite(MOTOR_CURRENT_PWM_## P ##_PIN, 255L * current / (MOTOR_CURRENT_PWM_RANGE))
      switch (driver) {
        #if PIN_EXISTS(MOTOR_CURRENT_PWM_XY)
          case 0: _WRITE_CURRENT_PWM(XY); break;
        #endif
        #if PIN_EXISTS(MOTOR_CURRENT_PWM_Z)
          case 1: _WRITE_CURRENT_PWM(Z); break;
        #endif
        #if PIN_EXISTS(MOTOR_CURRENT_PWM_E)
          case 2: _WRITE_CURRENT_PWM(E); break;
        #endif
      }
    #endif
  }

  void Stepper::digipot_init() {

    #if HAS_DIGIPOTSS

      static const uint8_t digipot_motor_current[] = DIGIPOT_MOTOR_CURRENT;

      SPI.begin();
      SET_OUTPUT(DIGIPOTSS_PIN);

      for (uint8_t i = 0; i < COUNT(digipot_motor_current); i++) {
        //digitalPotWrite(digipot_ch[i], digipot_motor_current[i]);
        digipot_current(i, digipot_motor_current[i]);
      }

    #elif HAS_MOTOR_CURRENT_PWM

      #if PIN_EXISTS(MOTOR_CURRENT_PWM_XY)
        SET_OUTPUT(MOTOR_CURRENT_PWM_XY_PIN);
      #endif
      #if PIN_EXISTS(MOTOR_CURRENT_PWM_Z)
        SET_OUTPUT(MOTOR_CURRENT_PWM_Z_PIN);
      #endif
      #if PIN_EXISTS(MOTOR_CURRENT_PWM_E)
        SET_OUTPUT(MOTOR_CURRENT_PWM_E_PIN);
      #endif

      refresh_motor_power();

      // Set Timer5 to 31khz so the PWM of the motor power is as constant as possible. (removes a buzzing noise)
      SET_CS5(PRESCALER_1);

    #endif
  }

#endif

#if HAS_MICROSTEPS

  /**
   * Software-controlled Microstepping
   */

  void Stepper::microstep_init() {
    SET_OUTPUT(X_MS1_PIN);
    SET_OUTPUT(X_MS2_PIN);
    #if HAS_Y_MICROSTEPS
      SET_OUTPUT(Y_MS1_PIN);
      SET_OUTPUT(Y_MS2_PIN);
    #endif
    #if HAS_Z_MICROSTEPS
      SET_OUTPUT(Z_MS1_PIN);
      SET_OUTPUT(Z_MS2_PIN);
    #endif
    #if HAS_E0_MICROSTEPS
      SET_OUTPUT(E0_MS1_PIN);
      SET_OUTPUT(E0_MS2_PIN);
    #endif
    #if HAS_E1_MICROSTEPS
      SET_OUTPUT(E1_MS1_PIN);
      SET_OUTPUT(E1_MS2_PIN);
    #endif
    #if HAS_E2_MICROSTEPS
      SET_OUTPUT(E2_MS1_PIN);
      SET_OUTPUT(E2_MS2_PIN);
    #endif
    #if HAS_E3_MICROSTEPS
      SET_OUTPUT(E3_MS1_PIN);
      SET_OUTPUT(E3_MS2_PIN);
    #endif
    #if HAS_E4_MICROSTEPS
      SET_OUTPUT(E4_MS1_PIN);
      SET_OUTPUT(E4_MS2_PIN);
    #endif
    static const uint8_t microstep_modes[] = MICROSTEP_MODES;
    for (uint16_t i = 0; i < COUNT(microstep_modes); i++)
      microstep_mode(i, microstep_modes[i]);
  }

  void Stepper::microstep_ms(const uint8_t driver, const int8_t ms1, const int8_t ms2) {
    if (ms1 >= 0) switch (driver) {
      case 0: WRITE(X_MS1_PIN, ms1); break;
      #if HAS_Y_MICROSTEPS
        case 1: WRITE(Y_MS1_PIN, ms1); break;
      #endif
      #if HAS_Z_MICROSTEPS
        case 2: WRITE(Z_MS1_PIN, ms1); break;
      #endif
      #if HAS_E0_MICROSTEPS
        case 3: WRITE(E0_MS1_PIN, ms1); break;
      #endif
      #if HAS_E1_MICROSTEPS
        case 4: WRITE(E1_MS1_PIN, ms1); break;
      #endif
      #if HAS_E2_MICROSTEPS
        case 5: WRITE(E2_MS1_PIN, ms1); break;
      #endif
      #if HAS_E3_MICROSTEPS
        case 6: WRITE(E3_MS1_PIN, ms1); break;
      #endif
      #if HAS_E4_MICROSTEPS
        case 7: WRITE(E4_MS1_PIN, ms1); break;
      #endif
    }
    if (ms2 >= 0) switch (driver) {
      case 0: WRITE(X_MS2_PIN, ms2); break;
      #if HAS_Y_MICROSTEPS
        case 1: WRITE(Y_MS2_PIN, ms2); break;
      #endif
      #if HAS_Z_MICROSTEPS
        case 2: WRITE(Z_MS2_PIN, ms2); break;
      #endif
      #if HAS_E0_MICROSTEPS
        case 3: WRITE(E0_MS2_PIN, ms2); break;
      #endif
      #if HAS_E1_MICROSTEPS
        case 4: WRITE(E1_MS2_PIN, ms2); break;
      #endif
      #if HAS_E2_MICROSTEPS
        case 5: WRITE(E2_MS2_PIN, ms2); break;
      #endif
      #if HAS_E3_MICROSTEPS
        case 6: WRITE(E3_MS2_PIN, ms2); break;
      #endif
      #if HAS_E4_MICROSTEPS
        case 7: WRITE(E4_MS2_PIN, ms2); break;
      #endif
    }
  }

  void Stepper::microstep_mode(const uint8_t driver, const uint8_t stepping_mode) {
    switch (stepping_mode) {
      case 1: microstep_ms(driver, MICROSTEP1); break;
      #if ENABLED(HEROIC_STEPPER_DRIVERS)
        case 128: microstep_ms(driver, MICROSTEP128); break;
      #else
        case 2: microstep_ms(driver, MICROSTEP2); break;
        case 4: microstep_ms(driver, MICROSTEP4); break;
      #endif
      case 8: microstep_ms(driver, MICROSTEP8); break;
      case 16: microstep_ms(driver, MICROSTEP16); break;
      #if MB(ALLIGATOR)
        case 32: microstep_ms(driver, MICROSTEP32); break;
      #endif
      default: SERIAL_ERROR_START(); SERIAL_ERRORLNPGM("Microsteps unavailable"); break;
    }
  }

  void Stepper::microstep_readings() {
    SERIAL_PROTOCOLLNPGM("MS1,MS2 Pins");
    SERIAL_PROTOCOLPGM("X: ");
    SERIAL_PROTOCOL(READ(X_MS1_PIN));
    SERIAL_PROTOCOLLN(READ(X_MS2_PIN));
    #if HAS_Y_MICROSTEPS
      SERIAL_PROTOCOLPGM("Y: ");
      SERIAL_PROTOCOL(READ(Y_MS1_PIN));
      SERIAL_PROTOCOLLN(READ(Y_MS2_PIN));
    #endif
    #if HAS_Z_MICROSTEPS
      SERIAL_PROTOCOLPGM("Z: ");
      SERIAL_PROTOCOL(READ(Z_MS1_PIN));
      SERIAL_PROTOCOLLN(READ(Z_MS2_PIN));
    #endif
    #if HAS_E0_MICROSTEPS
      SERIAL_PROTOCOLPGM("E0: ");
      SERIAL_PROTOCOL(READ(E0_MS1_PIN));
      SERIAL_PROTOCOLLN(READ(E0_MS2_PIN));
    #endif
    #if HAS_E1_MICROSTEPS
      SERIAL_PROTOCOLPGM("E1: ");
      SERIAL_PROTOCOL(READ(E1_MS1_PIN));
      SERIAL_PROTOCOLLN(READ(E1_MS2_PIN));
    #endif
    #if HAS_E2_MICROSTEPS
      SERIAL_PROTOCOLPGM("E2: ");
      SERIAL_PROTOCOL(READ(E2_MS1_PIN));
      SERIAL_PROTOCOLLN(READ(E2_MS2_PIN));
    #endif
    #if HAS_E3_MICROSTEPS
      SERIAL_PROTOCOLPGM("E3: ");
      SERIAL_PROTOCOL(READ(E3_MS1_PIN));
      SERIAL_PROTOCOLLN(READ(E3_MS2_PIN));
    #endif
    #if HAS_E4_MICROSTEPS
      SERIAL_PROTOCOLPGM("E4: ");
      SERIAL_PROTOCOL(READ(E4_MS1_PIN));
      SERIAL_PROTOCOLLN(READ(E4_MS2_PIN));
    #endif
  }

#endif // HAS_MICROSTEPS
