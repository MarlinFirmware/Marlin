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

/**
 * planner.cpp
 *
 * Buffer movement commands and manage the acceleration profile plan
 *
 * Derived from Grbl
 * Copyright (c) 2009-2011 Simen Svale Skogsrud
 *
 * The ring buffer implementation gleaned from the wiring_serial library by David A. Mellis.
 *
 *
 * Reasoning behind the mathematics in this module (in the key of 'Mathematica'):
 *
 * s == speed, a == acceleration, t == time, d == distance
 *
 * Basic definitions:
 *   Speed[s_, a_, t_] := s + (a*t)
 *   Travel[s_, a_, t_] := Integrate[Speed[s, a, t], t]
 *
 * Distance to reach a specific speed with a constant acceleration:
 *   Solve[{Speed[s, a, t] == m, Travel[s, a, t] == d}, d, t]
 *   d -> (m^2 - s^2)/(2 a) --> estimate_acceleration_distance()
 *
 * Speed after a given distance of travel with constant acceleration:
 *   Solve[{Speed[s, a, t] == m, Travel[s, a, t] == d}, m, t]
 *   m -> Sqrt[2 a d + s^2]
 *
 * DestinationSpeed[s_, a_, d_] := Sqrt[2 a d + s^2]
 *
 * When to start braking (di) to reach a specified destination speed (s2) after accelerating
 * from initial speed s1 without ever stopping at a plateau:
 *   Solve[{DestinationSpeed[s1, a, di] == DestinationSpeed[s2, a, d - di]}, di]
 *   di -> (2 a d - s1^2 + s2^2)/(4 a) --> intersection_distance()
 *
 * IntersectionDistance[s1_, s2_, a_, d_] := (2 a d - s1^2 + s2^2)/(4 a)
 *
 * --
 *
 * The fast inverse function needed for Bézier interpolation for AVR
 * was designed, written and tested by Eduardo José Tagle on April/2018
 */

#include "planner.h"
#include "stepper.h"
#include "motion.h"
#include "temperature.h"
#include "../lcd/ultralcd.h"
#include "../gcode/parser.h"

#include "../MarlinCore.h"

#if HAS_LEVELING
  #include "../feature/bedlevel/bedlevel.h"
#endif

#if ENABLED(FILAMENT_WIDTH_SENSOR)
  #include "../feature/filwidth.h"
#endif

#if ENABLED(BARICUDA)
  #include "../feature/baricuda.h"
#endif

#if ENABLED(MIXING_EXTRUDER)
  #include "../feature/mixing.h"
#endif

#if ENABLED(AUTO_POWER_CONTROL)
  #include "../feature/power.h"
#endif

#if ENABLED(EXTERNAL_CLOSED_LOOP_CONTROLLER)
  #include "../feature/closedloop.h"
#endif

#if ENABLED(BACKLASH_COMPENSATION)
  #include "../feature/backlash.h"
#endif

#if ENABLED(CANCEL_OBJECTS)
  #include "../feature/cancel_object.h"
#endif

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../feature/powerloss.h"
#endif

#if HAS_CUTTER
  #include "../feature/spindle_laser.h"
#endif

// Delay for delivery of first block to the stepper ISR, if the queue contains 2 or
// fewer movements. The delay is measured in milliseconds, and must be less than 250ms
#define BLOCK_DELAY_FOR_1ST_MOVE 100

Planner planner;

// public:

/**
 * A ring buffer of moves described in steps
 */
block_t Planner::block_buffer[BLOCK_BUFFER_SIZE];
volatile uint8_t Planner::block_buffer_head,    // Index of the next block to be pushed
                 Planner::block_buffer_nonbusy, // Index of the first non-busy block
                 Planner::block_buffer_planned, // Index of the optimally planned block
                 Planner::block_buffer_tail;    // Index of the busy block, if any
uint16_t Planner::cleaning_buffer_counter;      // A counter to disable queuing of blocks
uint8_t Planner::delay_before_delivering;       // This counter delays delivery of blocks when queue becomes empty to allow the opportunity of merging blocks

planner_settings_t Planner::settings;           // Initialized by settings.load()

#if ENABLED(LASER_POWER_INLINE)
  laser_state_t Planner::laser_inline;          // Current state for blocks
#endif

uint32_t Planner::max_acceleration_steps_per_s2[XYZE_N]; // (steps/s^2) Derived from mm_per_s2

float Planner::steps_to_mm[XYZE_N];             // (mm) Millimeters per step

#if HAS_JUNCTION_DEVIATION
  float Planner::junction_deviation_mm;         // (mm) M205 J
  #if HAS_LINEAR_E_JERK
    float Planner::max_e_jerk[DISTINCT_E];      // Calculated from junction_deviation_mm
  #endif
#endif

#if HAS_CLASSIC_JERK
  TERN(HAS_LINEAR_E_JERK, xyz_pos_t, xyze_pos_t) Planner::max_jerk;
#endif

#if ENABLED(SD_ABORT_ON_ENDSTOP_HIT)
  bool Planner::abort_on_endstop_hit = false;
#endif

#if ENABLED(DISTINCT_E_FACTORS)
  uint8_t Planner::last_extruder = 0;     // Respond to extruder change
#endif

#if ENABLED(DIRECT_STEPPING)
  uint32_t Planner::last_page_step_rate = 0;
  xyze_bool_t Planner::last_page_dir{0};
#endif

#if EXTRUDERS
  int16_t Planner::flow_percentage[EXTRUDERS] = ARRAY_BY_EXTRUDERS1(100); // Extrusion factor for each extruder
  float Planner::e_factor[EXTRUDERS] = ARRAY_BY_EXTRUDERS1(1.0f); // The flow percentage and volumetric multiplier combine to scale E movement
#endif

#if DISABLED(NO_VOLUMETRICS)
  float Planner::filament_size[EXTRUDERS],          // diameter of filament (in millimeters), typically around 1.75 or 2.85, 0 disables the volumetric calculations for the extruder
        Planner::volumetric_area_nominal = CIRCLE_AREA(float(DEFAULT_NOMINAL_FILAMENT_DIA) * 0.5f), // Nominal cross-sectional area
        Planner::volumetric_multiplier[EXTRUDERS];  // Reciprocal of cross-sectional area of filament (in mm^2). Pre-calculated to reduce computation in the planner
#endif

#if ENABLED(VOLUMETRIC_EXTRUDER_LIMIT)
  float Planner::volumetric_extruder_limit[EXTRUDERS],          // max mm^3/sec the extruder is able to handle
        Planner::volumetric_extruder_feedrate_limit[EXTRUDERS]; // pre calculated extruder feedrate limit based on volumetric_extruder_limit; pre-calculated to reduce computation in the planner
#endif

#if HAS_LEVELING
  bool Planner::leveling_active = false; // Flag that auto bed leveling is enabled
  #if ABL_PLANAR
    matrix_3x3 Planner::bed_level_matrix; // Transform to compensate for bed level
  #endif
  #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
    float Planner::z_fade_height,      // Initialized by settings.load()
          Planner::inverse_z_fade_height,
          Planner::last_fade_z;
  #endif
#else
  constexpr bool Planner::leveling_active;
#endif

skew_factor_t Planner::skew_factor; // Initialized by settings.load()

#if ENABLED(AUTOTEMP)
  float Planner::autotemp_max = 250,
        Planner::autotemp_min = 210,
        Planner::autotemp_factor = 0.1f;
  bool Planner::autotemp_enabled = false;
#endif

// private:

xyze_long_t Planner::position{0};

uint32_t Planner::cutoff_long;

xyze_float_t Planner::previous_speed;
float Planner::previous_nominal_speed_sqr;

#if ENABLED(DISABLE_INACTIVE_EXTRUDER)
  uint8_t Planner::g_uc_extruder_last_move[EXTRUDERS] = { 0 };
#endif

#ifdef XY_FREQUENCY_LIMIT
  int8_t Planner::xy_freq_limit_hz = XY_FREQUENCY_LIMIT;
  float Planner::xy_freq_min_speed_factor = (XY_FREQUENCY_MIN_PERCENT) * 0.01f;
  int32_t Planner::xy_freq_min_interval_us = LROUND(1000000.0 / (XY_FREQUENCY_LIMIT));
#endif

#if ENABLED(LIN_ADVANCE)
  float Planner::extruder_advance_K[EXTRUDERS]; // Initialized by settings.load()
#endif

#if HAS_POSITION_FLOAT
  xyze_pos_t Planner::position_float; // Needed for accurate maths. Steps cannot be used!
#endif

#if IS_KINEMATIC
  xyze_pos_t Planner::position_cart;
#endif

#if HAS_SPI_LCD
  volatile uint32_t Planner::block_buffer_runtime_us = 0;
#endif

/**
 * Class and Instance Methods
 */

Planner::Planner() { init(); }

void Planner::init() {
  position.reset();
  TERN_(HAS_POSITION_FLOAT, position_float.reset());
  TERN_(IS_KINEMATIC, position_cart.reset());
  previous_speed.reset();
  previous_nominal_speed_sqr = 0;
  TERN_(ABL_PLANAR, bed_level_matrix.set_to_identity());
  clear_block_buffer();
  delay_before_delivering = 0;
  #if ENABLED(DIRECT_STEPPING)
    last_page_step_rate = 0;
    last_page_dir.reset();
  #endif
}

#if ENABLED(S_CURVE_ACCELERATION)
  #ifdef __AVR__
    /**
     * This routine returns 0x1000000 / d, getting the inverse as fast as possible.
     * A fast-converging iterative Newton-Raphson method can reach full precision in
     * just 1 iteration, and takes 211 cycles (worst case; the mean case is less, up
     * to 30 cycles for small divisors), instead of the 500 cycles a normal division
     * would take.
     *
     * Inspired by the following page:
     *  https://stackoverflow.com/questions/27801397/newton-raphson-division-with-big-integers
     *
     * Suppose we want to calculate  floor(2 ^ k / B)  where B is a positive integer
     * Then, B must be <= 2^k, otherwise, the quotient is 0.
     *
     * The Newton - Raphson iteration for x = B / 2 ^ k yields:
     *  q[n + 1] = q[n] * (2 - q[n] * B / 2 ^ k)
     *
     * This can be rearranged to:
     *  q[n + 1] = q[n] * (2 ^ (k + 1) - q[n] * B) >> k
     *
     * Each iteration requires only integer multiplications and bit shifts.
     * It doesn't necessarily converge to floor(2 ^ k / B) but in the worst case
     * it eventually alternates between floor(2 ^ k / B) and ceil(2 ^ k / B).
     * So it checks for this case and extracts floor(2 ^ k / B).
     *
     * A simple but important optimization for this approach is to truncate
     * multiplications (i.e., calculate only the higher bits of the product) in the
     * early iterations of the Newton - Raphson method. This is done so the results
     * of the early iterations are far from the quotient. Then it doesn't matter if
     * they are done inaccurately.
     * It's important to pick a good starting value for x. Knowing how many
     * digits the divisor has, it can be estimated:
     *
     *   2^k / x = 2 ^ log2(2^k / x)
     *   2^k / x = 2 ^(log2(2^k)-log2(x))
     *   2^k / x = 2 ^(k*log2(2)-log2(x))
     *   2^k / x = 2 ^ (k-log2(x))
     *   2^k / x >= 2 ^ (k-floor(log2(x)))
     *   floor(log2(x)) is simply the index of the most significant bit set.
     *
     * If this estimation can be improved even further the number of iterations can be
     * reduced a lot, saving valuable execution time.
     * The paper "Software Integer Division" by Thomas L.Rodeheffer, Microsoft
     * Research, Silicon Valley,August 26, 2008, available at
     * https://www.microsoft.com/en-us/research/wp-content/uploads/2008/08/tr-2008-141.pdf
     * suggests, for its integer division algorithm, using a table to supply the first
     * 8 bits of precision, then, due to the quadratic convergence nature of the
     * Newton-Raphon iteration, just 2 iterations should be enough to get maximum
     * precision of the division.
     * By precomputing values of inverses for small denominator values, just one
     * Newton-Raphson iteration is enough to reach full precision.
     * This code uses the top 9 bits of the denominator as index.
     *
     * The AVR assembly function implements this C code using the data below:
     *
     *  // For small divisors, it is best to directly retrieve the results
     *  if (d <= 110) return pgm_read_dword(&small_inv_tab[d]);
     *
     *  // Compute initial estimation of 0x1000000/x -
     *  // Get most significant bit set on divider
     *  uint8_t idx = 0;
     *  uint32_t nr = d;
     *  if (!(nr & 0xFF0000)) {
     *    nr <<= 8; idx += 8;
     *    if (!(nr & 0xFF0000)) { nr <<= 8; idx += 8; }
     *  }
     *  if (!(nr & 0xF00000)) { nr <<= 4; idx += 4; }
     *  if (!(nr & 0xC00000)) { nr <<= 2; idx += 2; }
     *  if (!(nr & 0x800000)) { nr <<= 1; idx += 1; }
     *
     *  // Isolate top 9 bits of the denominator, to be used as index into the initial estimation table
     *  uint32_t tidx = nr >> 15,                                       // top 9 bits. bit8 is always set
     *           ie = inv_tab[tidx & 0xFF] + 256,                       // Get the table value. bit9 is always set
     *           x = idx <= 8 ? (ie >> (8 - idx)) : (ie << (idx - 8));  // Position the estimation at the proper place
     *
     *  x = uint32_t((x * uint64_t(_BV(25) - x * d)) >> 24);            // Refine estimation by newton-raphson. 1 iteration is enough
     *  const uint32_t r = _BV(24) - x * d;                             // Estimate remainder
     *  if (r >= d) x++;                                                // Check whether to adjust result
     *  return uint32_t(x);                                             // x holds the proper estimation
     *
     */
    static uint32_t get_period_inverse(uint32_t d) {

      static const uint8_t inv_tab[256] PROGMEM = {
        255,253,252,250,248,246,244,242,240,238,236,234,233,231,229,227,
        225,224,222,220,218,217,215,213,212,210,208,207,205,203,202,200,
        199,197,195,194,192,191,189,188,186,185,183,182,180,179,178,176,
        175,173,172,170,169,168,166,165,164,162,161,160,158,157,156,154,
        153,152,151,149,148,147,146,144,143,142,141,139,138,137,136,135,
        134,132,131,130,129,128,127,126,125,123,122,121,120,119,118,117,
        116,115,114,113,112,111,110,109,108,107,106,105,104,103,102,101,
        100,99,98,97,96,95,94,93,92,91,90,89,88,88,87,86,
        85,84,83,82,81,80,80,79,78,77,76,75,74,74,73,72,
        71,70,70,69,68,67,66,66,65,64,63,62,62,61,60,59,
        59,58,57,56,56,55,54,53,53,52,51,50,50,49,48,48,
        47,46,46,45,44,43,43,42,41,41,40,39,39,38,37,37,
        36,35,35,34,33,33,32,32,31,30,30,29,28,28,27,27,
        26,25,25,24,24,23,22,22,21,21,20,19,19,18,18,17,
        17,16,15,15,14,14,13,13,12,12,11,10,10,9,9,8,
        8,7,7,6,6,5,5,4,4,3,3,2,2,1,0,0
      };

      // For small denominators, it is cheaper to directly store the result.
      //  For bigger ones, just ONE Newton-Raphson iteration is enough to get
      //  maximum precision we need
      static const uint32_t small_inv_tab[111] PROGMEM = {
        16777216,16777216,8388608,5592405,4194304,3355443,2796202,2396745,2097152,1864135,1677721,1525201,1398101,1290555,1198372,1118481,
        1048576,986895,932067,883011,838860,798915,762600,729444,699050,671088,645277,621378,599186,578524,559240,541200,
        524288,508400,493447,479349,466033,453438,441505,430185,419430,409200,399457,390167,381300,372827,364722,356962,
        349525,342392,335544,328965,322638,316551,310689,305040,299593,294337,289262,284359,279620,275036,270600,266305,
        262144,258111,254200,250406,246723,243148,239674,236298,233016,229824,226719,223696,220752,217885,215092,212369,
        209715,207126,204600,202135,199728,197379,195083,192841,190650,188508,186413,184365,182361,180400,178481,176602,
        174762,172960,171196,169466,167772,166111,164482,162885,161319,159783,158275,156796,155344,153919,152520
      };

      // For small divisors, it is best to directly retrieve the results
      if (d <= 110) return pgm_read_dword(&small_inv_tab[d]);

      uint8_t r8 = d & 0xFF,
              r9 = (d >> 8) & 0xFF,
              r10 = (d >> 16) & 0xFF,
              r2,r3,r4,r5,r6,r7,r11,r12,r13,r14,r15,r16,r17,r18;
      const uint8_t* ptab = inv_tab;

      __asm__ __volatile__(
        // %8:%7:%6 = interval
        // r31:r30: MUST be those registers, and they must point to the inv_tab

        A("clr %13")                       // %13 = 0

        // Now we must compute
        // result = 0xFFFFFF / d
        // %8:%7:%6 = interval
        // %16:%15:%14 = nr
        // %13 = 0

        // A plain division of 24x24 bits should take 388 cycles to complete. We will
        // use Newton-Raphson for the calculation, and will strive to get way less cycles
        // for the same result - Using C division, it takes 500cycles to complete .

        A("clr %3")                       // idx = 0
        A("mov %14,%6")
        A("mov %15,%7")
        A("mov %16,%8")                   // nr = interval
        A("tst %16")                      // nr & 0xFF0000 == 0 ?
        A("brne 2f")                      // No, skip this
        A("mov %16,%15")
        A("mov %15,%14")                  // nr <<= 8, %14 not needed
        A("subi %3,-8")                   // idx += 8
        A("tst %16")                      // nr & 0xFF0000 == 0 ?
        A("brne 2f")                      // No, skip this
        A("mov %16,%15")                  // nr <<= 8, %14 not needed
        A("clr %15")                      // We clear %14
        A("subi %3,-8")                   // idx += 8

        // here %16 != 0 and %16:%15 contains at least 9 MSBits, or both %16:%15 are 0
        L("2")
        A("cpi %16,0x10")                 // (nr & 0xF00000) == 0 ?
        A("brcc 3f")                      // No, skip this
        A("swap %15")                     // Swap nibbles
        A("swap %16")                     // Swap nibbles. Low nibble is 0
        A("mov %14, %15")
        A("andi %14,0x0F")                // Isolate low nibble
        A("andi %15,0xF0")                // Keep proper nibble in %15
        A("or %16, %14")                  // %16:%15 <<= 4
        A("subi %3,-4")                   // idx += 4

        L("3")
        A("cpi %16,0x40")                 // (nr & 0xC00000) == 0 ?
        A("brcc 4f")                      // No, skip this
        A("add %15,%15")
        A("adc %16,%16")
        A("add %15,%15")
        A("adc %16,%16")                  // %16:%15 <<= 2
        A("subi %3,-2")                   // idx += 2

        L("4")
        A("cpi %16,0x80")                 // (nr & 0x800000) == 0 ?
        A("brcc 5f")                      // No, skip this
        A("add %15,%15")
        A("adc %16,%16")                  // %16:%15 <<= 1
        A("inc %3")                       // idx += 1

        // Now %16:%15 contains its MSBit set to 1, or %16:%15 is == 0. We are now absolutely sure
        // we have at least 9 MSBits available to enter the initial estimation table
        L("5")
        A("add %15,%15")
        A("adc %16,%16")                  // %16:%15 = tidx = (nr <<= 1), we lose the top MSBit (always set to 1, %16 is the index into the inverse table)
        A("add r30,%16")                  // Only use top 8 bits
        A("adc r31,%13")                  // r31:r30 = inv_tab + (tidx)
        A("lpm %14, Z")                   // %14 = inv_tab[tidx]
        A("ldi %15, 1")                   // %15 = 1  %15:%14 = inv_tab[tidx] + 256

        // We must scale the approximation to the proper place
        A("clr %16")                      // %16 will always be 0 here
        A("subi %3,8")                    // idx == 8 ?
        A("breq 6f")                      // yes, no need to scale
        A("brcs 7f")                      // If C=1, means idx < 8, result was negative!

        // idx > 8, now %3 = idx - 8. We must perform a left shift. idx range:[1-8]
        A("sbrs %3,0")                    // shift by 1bit position?
        A("rjmp 8f")                      // No
        A("add %14,%14")
        A("adc %15,%15")                  // %15:16 <<= 1
        L("8")
        A("sbrs %3,1")                    // shift by 2bit position?
        A("rjmp 9f")                      // No
        A("add %14,%14")
        A("adc %15,%15")
        A("add %14,%14")
        A("adc %15,%15")                  // %15:16 <<= 1
        L("9")
        A("sbrs %3,2")                    // shift by 4bits position?
        A("rjmp 16f")                     // No
        A("swap %15")                     // Swap nibbles. lo nibble of %15 will always be 0
        A("swap %14")                     // Swap nibbles
        A("mov %12,%14")
        A("andi %12,0x0F")                // isolate low nibble
        A("andi %14,0xF0")                // and clear it
        A("or %15,%12")                   // %15:%16 <<= 4
        L("16")
        A("sbrs %3,3")                    // shift by 8bits position?
        A("rjmp 6f")                      // No, we are done
        A("mov %16,%15")
        A("mov %15,%14")
        A("clr %14")
        A("jmp 6f")

        // idx < 8, now %3 = idx - 8. Get the count of bits
        L("7")
        A("neg %3")                       // %3 = -idx = count of bits to move right. idx range:[1...8]
        A("sbrs %3,0")                    // shift by 1 bit position ?
        A("rjmp 10f")                     // No, skip it
        A("asr %15")                      // (bit7 is always 0 here)
        A("ror %14")
        L("10")
        A("sbrs %3,1")                    // shift by 2 bit position ?
        A("rjmp 11f")                     // No, skip it
        A("asr %15")                      // (bit7 is always 0 here)
        A("ror %14")
        A("asr %15")                      // (bit7 is always 0 here)
        A("ror %14")
        L("11")
        A("sbrs %3,2")                    // shift by 4 bit position ?
        A("rjmp 12f")                     // No, skip it
        A("swap %15")                     // Swap nibbles
        A("andi %14, 0xF0")               // Lose the lowest nibble
        A("swap %14")                     // Swap nibbles. Upper nibble is 0
        A("or %14,%15")                   // Pass nibble from upper byte
        A("andi %15, 0x0F")               // And get rid of that nibble
        L("12")
        A("sbrs %3,3")                    // shift by 8 bit position ?
        A("rjmp 6f")                      // No, skip it
        A("mov %14,%15")
        A("clr %15")
        L("6")                            // %16:%15:%14 = initial estimation of 0x1000000 / d

        // Now, we must refine the estimation present on %16:%15:%14 using 1 iteration
        // of Newton-Raphson. As it has a quadratic convergence, 1 iteration is enough
        // to get more than 18bits of precision (the initial table lookup gives 9 bits of
        // precision to start from). 18bits of precision is all what is needed here for result

        // %8:%7:%6 = d = interval
        // %16:%15:%14 = x = initial estimation of 0x1000000 / d
        // %13 = 0
        // %3:%2:%1:%0 = working accumulator

        // Compute 1<<25 - x*d. Result should never exceed 25 bits and should always be positive
        A("clr %0")
        A("clr %1")
        A("clr %2")
        A("ldi %3,2")                     // %3:%2:%1:%0 = 0x2000000
        A("mul %6,%14")                   // r1:r0 = LO(d) * LO(x)
        A("sub %0,r0")
        A("sbc %1,r1")
        A("sbc %2,%13")
        A("sbc %3,%13")                   // %3:%2:%1:%0 -= LO(d) * LO(x)
        A("mul %7,%14")                   // r1:r0 = MI(d) * LO(x)
        A("sub %1,r0")
        A("sbc %2,r1" )
        A("sbc %3,%13")                   // %3:%2:%1:%0 -= MI(d) * LO(x) << 8
        A("mul %8,%14")                   // r1:r0 = HI(d) * LO(x)
        A("sub %2,r0")
        A("sbc %3,r1")                    // %3:%2:%1:%0 -= MIL(d) * LO(x) << 16
        A("mul %6,%15")                   // r1:r0 = LO(d) * MI(x)
        A("sub %1,r0")
        A("sbc %2,r1")
        A("sbc %3,%13")                   // %3:%2:%1:%0 -= LO(d) * MI(x) << 8
        A("mul %7,%15")                   // r1:r0 = MI(d) * MI(x)
        A("sub %2,r0")
        A("sbc %3,r1")                    // %3:%2:%1:%0 -= MI(d) * MI(x) << 16
        A("mul %8,%15")                   // r1:r0 = HI(d) * MI(x)
        A("sub %3,r0")                    // %3:%2:%1:%0 -= MIL(d) * MI(x) << 24
        A("mul %6,%16")                   // r1:r0 = LO(d) * HI(x)
        A("sub %2,r0")
        A("sbc %3,r1")                    // %3:%2:%1:%0 -= LO(d) * HI(x) << 16
        A("mul %7,%16")                   // r1:r0 = MI(d) * HI(x)
        A("sub %3,r0")                    // %3:%2:%1:%0 -= MI(d) * HI(x) << 24
        // %3:%2:%1:%0 = (1<<25) - x*d     [169]

        // We need to multiply that result by x, and we are only interested in the top 24bits of that multiply

        // %16:%15:%14 = x = initial estimation of 0x1000000 / d
        // %3:%2:%1:%0 = (1<<25) - x*d = acc
        // %13 = 0

        // result = %11:%10:%9:%5:%4
        A("mul %14,%0")                   // r1:r0 = LO(x) * LO(acc)
        A("mov %4,r1")
        A("clr %5")
        A("clr %9")
        A("clr %10")
        A("clr %11")                      // %11:%10:%9:%5:%4 = LO(x) * LO(acc) >> 8
        A("mul %15,%0")                   // r1:r0 = MI(x) * LO(acc)
        A("add %4,r0")
        A("adc %5,r1")
        A("adc %9,%13")
        A("adc %10,%13")
        A("adc %11,%13")                  // %11:%10:%9:%5:%4 += MI(x) * LO(acc)
        A("mul %16,%0")                   // r1:r0 = HI(x) * LO(acc)
        A("add %5,r0")
        A("adc %9,r1")
        A("adc %10,%13")
        A("adc %11,%13")                  // %11:%10:%9:%5:%4 += MI(x) * LO(acc) << 8

        A("mul %14,%1")                   // r1:r0 = LO(x) * MIL(acc)
        A("add %4,r0")
        A("adc %5,r1")
        A("adc %9,%13")
        A("adc %10,%13")
        A("adc %11,%13")                  // %11:%10:%9:%5:%4 = LO(x) * MIL(acc)
        A("mul %15,%1")                   // r1:r0 = MI(x) * MIL(acc)
        A("add %5,r0")
        A("adc %9,r1")
        A("adc %10,%13")
        A("adc %11,%13")                  // %11:%10:%9:%5:%4 += MI(x) * MIL(acc) << 8
        A("mul %16,%1")                   // r1:r0 = HI(x) * MIL(acc)
        A("add %9,r0")
        A("adc %10,r1")
        A("adc %11,%13")                  // %11:%10:%9:%5:%4 += MI(x) * MIL(acc) << 16

        A("mul %14,%2")                   // r1:r0 = LO(x) * MIH(acc)
        A("add %5,r0")
        A("adc %9,r1")
        A("adc %10,%13")
        A("adc %11,%13")                  // %11:%10:%9:%5:%4 = LO(x) * MIH(acc) << 8
        A("mul %15,%2")                   // r1:r0 = MI(x) * MIH(acc)
        A("add %9,r0")
        A("adc %10,r1")
        A("adc %11,%13")                  // %11:%10:%9:%5:%4 += MI(x) * MIH(acc) << 16
        A("mul %16,%2")                   // r1:r0 = HI(x) * MIH(acc)
        A("add %10,r0")
        A("adc %11,r1")                   // %11:%10:%9:%5:%4 += MI(x) * MIH(acc) << 24

        A("mul %14,%3")                   // r1:r0 = LO(x) * HI(acc)
        A("add %9,r0")
        A("adc %10,r1")
        A("adc %11,%13")                  // %11:%10:%9:%5:%4 = LO(x) * HI(acc) << 16
        A("mul %15,%3")                   // r1:r0 = MI(x) * HI(acc)
        A("add %10,r0")
        A("adc %11,r1")                   // %11:%10:%9:%5:%4 += MI(x) * HI(acc) << 24
        A("mul %16,%3")                   // r1:r0 = HI(x) * HI(acc)
        A("add %11,r0")                   // %11:%10:%9:%5:%4 += MI(x) * HI(acc) << 32

        // At this point, %11:%10:%9 contains the new estimation of x.

        // Finally, we must correct the result. Estimate remainder as
        // (1<<24) - x*d
        // %11:%10:%9 = x
        // %8:%7:%6 = d = interval" "\n\t"
        A("ldi %3,1")
        A("clr %2")
        A("clr %1")
        A("clr %0")                       // %3:%2:%1:%0 = 0x1000000
        A("mul %6,%9")                    // r1:r0 = LO(d) * LO(x)
        A("sub %0,r0")
        A("sbc %1,r1")
        A("sbc %2,%13")
        A("sbc %3,%13")                   // %3:%2:%1:%0 -= LO(d) * LO(x)
        A("mul %7,%9")                    // r1:r0 = MI(d) * LO(x)
        A("sub %1,r0")
        A("sbc %2,r1")
        A("sbc %3,%13")                   // %3:%2:%1:%0 -= MI(d) * LO(x) << 8
        A("mul %8,%9")                    // r1:r0 = HI(d) * LO(x)
        A("sub %2,r0")
        A("sbc %3,r1")                    // %3:%2:%1:%0 -= MIL(d) * LO(x) << 16
        A("mul %6,%10")                   // r1:r0 = LO(d) * MI(x)
        A("sub %1,r0")
        A("sbc %2,r1")
        A("sbc %3,%13")                   // %3:%2:%1:%0 -= LO(d) * MI(x) << 8
        A("mul %7,%10")                   // r1:r0 = MI(d) * MI(x)
        A("sub %2,r0")
        A("sbc %3,r1")                    // %3:%2:%1:%0 -= MI(d) * MI(x) << 16
        A("mul %8,%10")                   // r1:r0 = HI(d) * MI(x)
        A("sub %3,r0")                    // %3:%2:%1:%0 -= MIL(d) * MI(x) << 24
        A("mul %6,%11")                   // r1:r0 = LO(d) * HI(x)
        A("sub %2,r0")
        A("sbc %3,r1")                    // %3:%2:%1:%0 -= LO(d) * HI(x) << 16
        A("mul %7,%11")                   // r1:r0 = MI(d) * HI(x)
        A("sub %3,r0")                    // %3:%2:%1:%0 -= MI(d) * HI(x) << 24
        // %3:%2:%1:%0 = r = (1<<24) - x*d
        // %8:%7:%6 = d = interval

        // Perform the final correction
        A("sub %0,%6")
        A("sbc %1,%7")
        A("sbc %2,%8")                    // r -= d
        A("brcs 14f")                     // if ( r >= d)

        // %11:%10:%9 = x
        A("ldi %3,1")
        A("add %9,%3")
        A("adc %10,%13")
        A("adc %11,%13")                  // x++
        L("14")

        // Estimation is done. %11:%10:%9 = x
        A("clr __zero_reg__")              // Make C runtime happy
        // [211 cycles total]
        : "=r" (r2),
          "=r" (r3),
          "=r" (r4),
          "=d" (r5),
          "=r" (r6),
          "=r" (r7),
          "+r" (r8),
          "+r" (r9),
          "+r" (r10),
          "=d" (r11),
          "=r" (r12),
          "=r" (r13),
          "=d" (r14),
          "=d" (r15),
          "=d" (r16),
          "=d" (r17),
          "=d" (r18),
          "+z" (ptab)
        :
        : "r0", "r1", "cc"
      );

      // Return the result
      return r11 | (uint16_t(r12) << 8) | (uint32_t(r13) << 16);
    }
  #else
    // All other 32-bit MPUs can easily do inverse using hardware division,
    // so we don't need to reduce precision or to use assembly language at all.
    // This routine, for all other archs, returns 0x100000000 / d ~= 0xFFFFFFFF / d
    static FORCE_INLINE uint32_t get_period_inverse(const uint32_t d) {
      return d ? 0xFFFFFFFF / d : 0xFFFFFFFF;
    }
  #endif
#endif

#define MINIMAL_STEP_RATE 120

/**
 * Get the current block for processing
 * and mark the block as busy.
 * Return nullptr if the buffer is empty
 * or if there is a first-block delay.
 *
 * WARNING: Called from Stepper ISR context!
 */
block_t* Planner::get_current_block() {
  // Get the number of moves in the planner queue so far
  const uint8_t nr_moves = movesplanned();

  // If there are any moves queued ...
  if (nr_moves) {

    // If there is still delay of delivery of blocks running, decrement it
    if (delay_before_delivering) {
      --delay_before_delivering;
      // If the number of movements queued is less than 3, and there is still time
      //  to wait, do not deliver anything
      if (nr_moves < 3 && delay_before_delivering) return nullptr;
      delay_before_delivering = 0;
    }

    // If we are here, there is no excuse to deliver the block
    block_t * const block = &block_buffer[block_buffer_tail];

    // No trapezoid calculated? Don't execute yet.
    if (TEST(block->flag, BLOCK_BIT_RECALCULATE)) return nullptr;

    // We can't be sure how long an active block will take, so don't count it.
    TERN_(HAS_SPI_LCD, block_buffer_runtime_us -= block->segment_time_us);

    // As this block is busy, advance the nonbusy block pointer
    block_buffer_nonbusy = next_block_index(block_buffer_tail);

    // Push block_buffer_planned pointer, if encountered.
    if (block_buffer_tail == block_buffer_planned)
      block_buffer_planned = block_buffer_nonbusy;

    // Return the block
    return block;
  }

  // The queue became empty
  TERN_(HAS_SPI_LCD, clear_block_buffer_runtime()); // paranoia. Buffer is empty now - so reset accumulated time to zero.

  return nullptr;
}

/**
 * Calculate trapezoid parameters, multiplying the entry- and exit-speeds
 * by the provided factors.
 **
 * ############ VERY IMPORTANT ############
 * NOTE that the PRECONDITION to call this function is that the block is
 * NOT BUSY and it is marked as RECALCULATE. That WARRANTIES the Stepper ISR
 * is not and will not use the block while we modify it, so it is safe to
 * alter its values.
 */
void Planner::calculate_trapezoid_for_block(block_t* const block, const float &entry_factor, const float &exit_factor) {

  uint32_t initial_rate = CEIL(block->nominal_rate * entry_factor),
           final_rate = CEIL(block->nominal_rate * exit_factor); // (steps per second)

  // Limit minimal step rate (Otherwise the timer will overflow.)
  NOLESS(initial_rate, uint32_t(MINIMAL_STEP_RATE));
  NOLESS(final_rate, uint32_t(MINIMAL_STEP_RATE));

  #if ENABLED(S_CURVE_ACCELERATION)
    uint32_t cruise_rate = initial_rate;
  #endif

  const int32_t accel = block->acceleration_steps_per_s2;

          // Steps required for acceleration, deceleration to/from nominal rate
  uint32_t accelerate_steps = CEIL(estimate_acceleration_distance(initial_rate, block->nominal_rate, accel)),
           decelerate_steps = FLOOR(estimate_acceleration_distance(block->nominal_rate, final_rate, -accel));
          // Steps between acceleration and deceleration, if any
  int32_t plateau_steps = block->step_event_count - accelerate_steps - decelerate_steps;

  // Does accelerate_steps + decelerate_steps exceed step_event_count?
  // Then we can't possibly reach the nominal rate, there will be no cruising.
  // Use intersection_distance() to calculate accel / braking time in order to
  // reach the final_rate exactly at the end of this block.
  if (plateau_steps < 0) {
    const float accelerate_steps_float = CEIL(intersection_distance(initial_rate, final_rate, accel, block->step_event_count));
    accelerate_steps = _MIN(uint32_t(_MAX(accelerate_steps_float, 0)), block->step_event_count);
    plateau_steps = 0;

    #if ENABLED(S_CURVE_ACCELERATION)
      // We won't reach the cruising rate. Let's calculate the speed we will reach
      cruise_rate = final_speed(initial_rate, accel, accelerate_steps);
    #endif
  }
  #if ENABLED(S_CURVE_ACCELERATION)
    else // We have some plateau time, so the cruise rate will be the nominal rate
      cruise_rate = block->nominal_rate;
  #endif

  #if ENABLED(S_CURVE_ACCELERATION)
    // Jerk controlled speed requires to express speed versus time, NOT steps
    uint32_t acceleration_time = ((float)(cruise_rate - initial_rate) / accel) * (STEPPER_TIMER_RATE),
             deceleration_time = ((float)(cruise_rate - final_rate) / accel) * (STEPPER_TIMER_RATE),
    // And to offload calculations from the ISR, we also calculate the inverse of those times here
             acceleration_time_inverse = get_period_inverse(acceleration_time),
             deceleration_time_inverse = get_period_inverse(deceleration_time);
  #endif

  // Store new block parameters
  block->accelerate_until = accelerate_steps;
  block->decelerate_after = accelerate_steps + plateau_steps;
  block->initial_rate = initial_rate;
  #if ENABLED(S_CURVE_ACCELERATION)
    block->acceleration_time = acceleration_time;
    block->deceleration_time = deceleration_time;
    block->acceleration_time_inverse = acceleration_time_inverse;
    block->deceleration_time_inverse = deceleration_time_inverse;
    block->cruise_rate = cruise_rate;
  #endif
  block->final_rate = final_rate;

  /**
   * Laser trapezoid calculations
   *
   * Approximate the trapezoid with the laser, incrementing the power every `entry_per` while accelerating
   * and decrementing it every `exit_power_per` while decelerating, thus ensuring power is related to feedrate.
   *
   * LASER_POWER_INLINE_TRAPEZOID_CONT doesn't need this as it continuously approximates
   *
   * Note this may behave unreliably when running with S_CURVE_ACCELERATION
   */
  #if ENABLED(LASER_POWER_INLINE_TRAPEZOID)
    if (block->laser.power > 0) { // No need to care if power == 0
      const uint8_t entry_power = block->laser.power * entry_factor; // Power on block entry
      #if DISABLED(LASER_POWER_INLINE_TRAPEZOID_CONT)
        // Speedup power
        const uint8_t entry_power_diff = block->laser.power - entry_power;
        if (entry_power_diff) {
          block->laser.entry_per = accelerate_steps / entry_power_diff;
          block->laser.power_entry = entry_power;
        }
        else {
          block->laser.entry_per = 0;
          block->laser.power_entry = block->laser.power;
        }
        // Slowdown power
        const uint8_t exit_power = block->laser.power * exit_factor, // Power on block entry
                      exit_power_diff = block->laser.power - exit_power;
        if (exit_power_diff) {
          block->laser.exit_per = (block->step_event_count - block->decelerate_after) / exit_power_diff;
          block->laser.power_exit = exit_power;
        }
        else {
          block->laser.exit_per = 0;
          block->laser.power_exit = block->laser.power;
        }
      #else
        block->laser.power_entry = entry_power;
      #endif
    }
  #endif
}

/*                            PLANNER SPEED DEFINITION
                                     +--------+   <- current->nominal_speed
                                    /          \
         current->entry_speed ->   +            \
                                   |             + <- next->entry_speed (aka exit speed)
                                   +-------------+
                                       time -->

  Recalculates the motion plan according to the following basic guidelines:

    1. Go over every feasible block sequentially in reverse order and calculate the junction speeds
        (i.e. current->entry_speed) such that:
      a. No junction speed exceeds the pre-computed maximum junction speed limit or nominal speeds of
         neighboring blocks.
      b. A block entry speed cannot exceed one reverse-computed from its exit speed (next->entry_speed)
         with a maximum allowable deceleration over the block travel distance.
      c. The last (or newest appended) block is planned from a complete stop (an exit speed of zero).
    2. Go over every block in chronological (forward) order and dial down junction speed values if
      a. The exit speed exceeds the one forward-computed from its entry speed with the maximum allowable
         acceleration over the block travel distance.

  When these stages are complete, the planner will have maximized the velocity profiles throughout the all
  of the planner blocks, where every block is operating at its maximum allowable acceleration limits. In
  other words, for all of the blocks in the planner, the plan is optimal and no further speed improvements
  are possible. If a new block is added to the buffer, the plan is recomputed according to the said
  guidelines for a new optimal plan.

  To increase computational efficiency of these guidelines, a set of planner block pointers have been
  created to indicate stop-compute points for when the planner guidelines cannot logically make any further
  changes or improvements to the plan when in normal operation and new blocks are streamed and added to the
  planner buffer. For example, if a subset of sequential blocks in the planner have been planned and are
  bracketed by junction velocities at their maximums (or by the first planner block as well), no new block
  added to the planner buffer will alter the velocity profiles within them. So we no longer have to compute
  them. Or, if a set of sequential blocks from the first block in the planner (or a optimal stop-compute
  point) are all accelerating, they are all optimal and can not be altered by a new block added to the
  planner buffer, as this will only further increase the plan speed to chronological blocks until a maximum
  junction velocity is reached. However, if the operational conditions of the plan changes from infrequently
  used feed holds or feedrate overrides, the stop-compute pointers will be reset and the entire plan is
  recomputed as stated in the general guidelines.

  Planner buffer index mapping:
  - block_buffer_tail: Points to the beginning of the planner buffer. First to be executed or being executed.
  - block_buffer_head: Points to the buffer block after the last block in the buffer. Used to indicate whether
      the buffer is full or empty. As described for standard ring buffers, this block is always empty.
  - block_buffer_planned: Points to the first buffer block after the last optimally planned block for normal
      streaming operating conditions. Use for planning optimizations by avoiding recomputing parts of the
      planner buffer that don't change with the addition of a new block, as describe above. In addition,
      this block can never be less than block_buffer_tail and will always be pushed forward and maintain
      this requirement when encountered by the Planner::release_current_block() routine during a cycle.

  NOTE: Since the planner only computes on what's in the planner buffer, some motions with lots of short
  line segments, like G2/3 arcs or complex curves, may seem to move slow. This is because there simply isn't
  enough combined distance traveled in the entire buffer to accelerate up to the nominal speed and then
  decelerate to a complete stop at the end of the buffer, as stated by the guidelines. If this happens and
  becomes an annoyance, there are a few simple solutions: (1) Maximize the machine acceleration. The planner
  will be able to compute higher velocity profiles within the same combined distance. (2) Maximize line
  motion(s) distance per block to a desired tolerance. The more combined distance the planner has to use,
  the faster it can go. (3) Maximize the planner buffer size. This also will increase the combined distance
  for the planner to compute over. It also increases the number of computations the planner has to perform
  to compute an optimal plan, so select carefully.
*/

// The kernel called by recalculate() when scanning the plan from last to first entry.
void Planner::reverse_pass_kernel(block_t* const current, const block_t * const next) {
  if (current) {
    // If entry speed is already at the maximum entry speed, and there was no change of speed
    // in the next block, there is no need to recheck. Block is cruising and there is no need to
    // compute anything for this block,
    // If not, block entry speed needs to be recalculated to ensure maximum possible planned speed.
    const float max_entry_speed_sqr = current->max_entry_speed_sqr;

    // Compute maximum entry speed decelerating over the current block from its exit speed.
    // If not at the maximum entry speed, or the previous block entry speed changed
    if (current->entry_speed_sqr != max_entry_speed_sqr || (next && TEST(next->flag, BLOCK_BIT_RECALCULATE))) {

      // If nominal length true, max junction speed is guaranteed to be reached.
      // If a block can de/ac-celerate from nominal speed to zero within the length of the block, then
      // the current block and next block junction speeds are guaranteed to always be at their maximum
      // junction speeds in deceleration and acceleration, respectively. This is due to how the current
      // block nominal speed limits both the current and next maximum junction speeds. Hence, in both
      // the reverse and forward planners, the corresponding block junction speed will always be at the
      // the maximum junction speed and may always be ignored for any speed reduction checks.

      const float new_entry_speed_sqr = TEST(current->flag, BLOCK_BIT_NOMINAL_LENGTH)
        ? max_entry_speed_sqr
        : _MIN(max_entry_speed_sqr, max_allowable_speed_sqr(-current->acceleration, next ? next->entry_speed_sqr : sq(float(MINIMUM_PLANNER_SPEED)), current->millimeters));
      if (current->entry_speed_sqr != new_entry_speed_sqr) {

        // Need to recalculate the block speed - Mark it now, so the stepper
        // ISR does not consume the block before being recalculated
        SBI(current->flag, BLOCK_BIT_RECALCULATE);

        // But there is an inherent race condition here, as the block may have
        // become BUSY just before being marked RECALCULATE, so check for that!
        if (stepper.is_block_busy(current)) {
          // Block became busy. Clear the RECALCULATE flag (no point in
          // recalculating BUSY blocks). And don't set its speed, as it can't
          // be updated at this time.
          CBI(current->flag, BLOCK_BIT_RECALCULATE);
        }
        else {
          // Block is not BUSY so this is ahead of the Stepper ISR:
          // Just Set the new entry speed.
          current->entry_speed_sqr = new_entry_speed_sqr;
        }
      }
    }
  }
}

/**
 * recalculate() needs to go over the current plan twice.
 * Once in reverse and once forward. This implements the reverse pass.
 */
void Planner::reverse_pass() {
  // Initialize block index to the last block in the planner buffer.
  uint8_t block_index = prev_block_index(block_buffer_head);

  // Read the index of the last buffer planned block.
  // The ISR may change it so get a stable local copy.
  uint8_t planned_block_index = block_buffer_planned;

  // If there was a race condition and block_buffer_planned was incremented
  //  or was pointing at the head (queue empty) break loop now and avoid
  //  planning already consumed blocks
  if (planned_block_index == block_buffer_head) return;

  // Reverse Pass: Coarsely maximize all possible deceleration curves back-planning from the last
  // block in buffer. Cease planning when the last optimal planned or tail pointer is reached.
  // NOTE: Forward pass will later refine and correct the reverse pass to create an optimal plan.
  const block_t *next = nullptr;
  while (block_index != planned_block_index) {

    // Perform the reverse pass
    block_t *current = &block_buffer[block_index];

    // Only consider non sync and page blocks
    if (!TEST(current->flag, BLOCK_BIT_SYNC_POSITION) && !IS_PAGE(current)) {
      reverse_pass_kernel(current, next);
      next = current;
    }

    // Advance to the next
    block_index = prev_block_index(block_index);

    // The ISR could advance the block_buffer_planned while we were doing the reverse pass.
    // We must try to avoid using an already consumed block as the last one - So follow
    // changes to the pointer and make sure to limit the loop to the currently busy block
    while (planned_block_index != block_buffer_planned) {

      // If we reached the busy block or an already processed block, break the loop now
      if (block_index == planned_block_index) return;

      // Advance the pointer, following the busy block
      planned_block_index = next_block_index(planned_block_index);
    }
  }
}

// The kernel called by recalculate() when scanning the plan from first to last entry.
void Planner::forward_pass_kernel(const block_t* const previous, block_t* const current, const uint8_t block_index) {
  if (previous) {
    // If the previous block is an acceleration block, too short to complete the full speed
    // change, adjust the entry speed accordingly. Entry speeds have already been reset,
    // maximized, and reverse-planned. If nominal length is set, max junction speed is
    // guaranteed to be reached. No need to recheck.
    if (!TEST(previous->flag, BLOCK_BIT_NOMINAL_LENGTH) &&
      previous->entry_speed_sqr < current->entry_speed_sqr) {

      // Compute the maximum allowable speed
      const float new_entry_speed_sqr = max_allowable_speed_sqr(-previous->acceleration, previous->entry_speed_sqr, previous->millimeters);

      // If true, current block is full-acceleration and we can move the planned pointer forward.
      if (new_entry_speed_sqr < current->entry_speed_sqr) {

        // Mark we need to recompute the trapezoidal shape, and do it now,
        // so the stepper ISR does not consume the block before being recalculated
        SBI(current->flag, BLOCK_BIT_RECALCULATE);

        // But there is an inherent race condition here, as the block maybe
        // became BUSY, just before it was marked as RECALCULATE, so check
        // if that is the case!
        if (stepper.is_block_busy(current)) {
          // Block became busy. Clear the RECALCULATE flag (no point in
          //  recalculating BUSY blocks and don't set its speed, as it can't
          //  be updated at this time.
          CBI(current->flag, BLOCK_BIT_RECALCULATE);
        }
        else {
          // Block is not BUSY, we won the race against the Stepper ISR:

          // Always <= max_entry_speed_sqr. Backward pass sets this.
          current->entry_speed_sqr = new_entry_speed_sqr; // Always <= max_entry_speed_sqr. Backward pass sets this.

          // Set optimal plan pointer.
          block_buffer_planned = block_index;
        }
      }
    }

    // Any block set at its maximum entry speed also creates an optimal plan up to this
    // point in the buffer. When the plan is bracketed by either the beginning of the
    // buffer and a maximum entry speed or two maximum entry speeds, every block in between
    // cannot logically be further improved. Hence, we don't have to recompute them anymore.
    if (current->entry_speed_sqr == current->max_entry_speed_sqr)
      block_buffer_planned = block_index;
  }
}

/**
 * recalculate() needs to go over the current plan twice.
 * Once in reverse and once forward. This implements the forward pass.
 */
void Planner::forward_pass() {

  // Forward Pass: Forward plan the acceleration curve from the planned pointer onward.
  // Also scans for optimal plan breakpoints and appropriately updates the planned pointer.

  // Begin at buffer planned pointer. Note that block_buffer_planned can be modified
  //  by the stepper ISR,  so read it ONCE. It it guaranteed that block_buffer_planned
  //  will never lead head, so the loop is safe to execute. Also note that the forward
  //  pass will never modify the values at the tail.
  uint8_t block_index = block_buffer_planned;

  block_t *block;
  const block_t * previous = nullptr;
  while (block_index != block_buffer_head) {

    // Perform the forward pass
    block = &block_buffer[block_index];

    // Skip SYNC and page blocks
    if (!TEST(block->flag, BLOCK_BIT_SYNC_POSITION) && !IS_PAGE(block)) {
      // If there's no previous block or the previous block is not
      // BUSY (thus, modifiable) run the forward_pass_kernel. Otherwise,
      // the previous block became BUSY, so assume the current block's
      // entry speed can't be altered (since that would also require
      // updating the exit speed of the previous block).
      if (!previous || !stepper.is_block_busy(previous))
        forward_pass_kernel(previous, block, block_index);
      previous = block;
    }
    // Advance to the previous
    block_index = next_block_index(block_index);
  }
}

/**
 * Recalculate the trapezoid speed profiles for all blocks in the plan
 * according to the entry_factor for each junction. Must be called by
 * recalculate() after updating the blocks.
 */
void Planner::recalculate_trapezoids() {
  // The tail may be changed by the ISR so get a local copy.
  uint8_t block_index = block_buffer_tail,
          head_block_index = block_buffer_head;
  // Since there could be a sync block in the head of the queue, and the
  // next loop must not recalculate the head block (as it needs to be
  // specially handled), scan backwards to the first non-SYNC block.
  while (head_block_index != block_index) {

    // Go back (head always point to the first free block)
    const uint8_t prev_index = prev_block_index(head_block_index);

    // Get the pointer to the block
    block_t *prev = &block_buffer[prev_index];

    // If not dealing with a sync block, we are done. The last block is not a SYNC block
    if (!TEST(prev->flag, BLOCK_BIT_SYNC_POSITION)) break;

    // Examine the previous block. This and all following are SYNC blocks
    head_block_index = prev_index;
  }

  // Go from the tail (currently executed block) to the first block, without including it)
  block_t *block = nullptr, *next = nullptr;
  float current_entry_speed = 0.0, next_entry_speed = 0.0;
  while (block_index != head_block_index) {

    next = &block_buffer[block_index];

    // Skip sync and page blocks
    if (!TEST(next->flag, BLOCK_BIT_SYNC_POSITION) && !IS_PAGE(next)) {
      next_entry_speed = SQRT(next->entry_speed_sqr);

      if (block) {
        // Recalculate if current block entry or exit junction speed has changed.
        if (TEST(block->flag, BLOCK_BIT_RECALCULATE) || TEST(next->flag, BLOCK_BIT_RECALCULATE)) {

          // Mark the current block as RECALCULATE, to protect it from the Stepper ISR running it.
          // Note that due to the above condition, there's a chance the current block isn't marked as
          // RECALCULATE yet, but the next one is. That's the reason for the following line.
          SBI(block->flag, BLOCK_BIT_RECALCULATE);

          // But there is an inherent race condition here, as the block maybe
          // became BUSY, just before it was marked as RECALCULATE, so check
          // if that is the case!
          if (!stepper.is_block_busy(block)) {
            // Block is not BUSY, we won the race against the Stepper ISR:

            // NOTE: Entry and exit factors always > 0 by all previous logic operations.
            const float current_nominal_speed = SQRT(block->nominal_speed_sqr),
                        nomr = 1.0f / current_nominal_speed;
            calculate_trapezoid_for_block(block, current_entry_speed * nomr, next_entry_speed * nomr);
            #if ENABLED(LIN_ADVANCE)
              if (block->use_advance_lead) {
                const float comp = block->e_D_ratio * extruder_advance_K[active_extruder] * settings.axis_steps_per_mm[E_AXIS];
                block->max_adv_steps = current_nominal_speed * comp;
                block->final_adv_steps = next_entry_speed * comp;
              }
            #endif
          }

          // Reset current only to ensure next trapezoid is computed - The
          // stepper is free to use the block from now on.
          CBI(block->flag, BLOCK_BIT_RECALCULATE);
        }
      }

      block = next;
      current_entry_speed = next_entry_speed;
    }

    block_index = next_block_index(block_index);
  }

  // Last/newest block in buffer. Exit speed is set with MINIMUM_PLANNER_SPEED. Always recalculated.
  if (next) {

    // Mark the next(last) block as RECALCULATE, to prevent the Stepper ISR running it.
    // As the last block is always recalculated here, there is a chance the block isn't
    // marked as RECALCULATE yet. That's the reason for the following line.
    SBI(next->flag, BLOCK_BIT_RECALCULATE);

    // But there is an inherent race condition here, as the block maybe
    // became BUSY, just before it was marked as RECALCULATE, so check
    // if that is the case!
    if (!stepper.is_block_busy(block)) {
      // Block is not BUSY, we won the race against the Stepper ISR:

      const float next_nominal_speed = SQRT(next->nominal_speed_sqr),
                  nomr = 1.0f / next_nominal_speed;
      calculate_trapezoid_for_block(next, next_entry_speed * nomr, float(MINIMUM_PLANNER_SPEED) * nomr);
      #if ENABLED(LIN_ADVANCE)
        if (next->use_advance_lead) {
          const float comp = next->e_D_ratio * extruder_advance_K[active_extruder] * settings.axis_steps_per_mm[E_AXIS];
          next->max_adv_steps = next_nominal_speed * comp;
          next->final_adv_steps = (MINIMUM_PLANNER_SPEED) * comp;
        }
      #endif
    }

    // Reset next only to ensure its trapezoid is computed - The stepper is free to use
    // the block from now on.
    CBI(next->flag, BLOCK_BIT_RECALCULATE);
  }
}

void Planner::recalculate() {
  // Initialize block index to the last block in the planner buffer.
  const uint8_t block_index = prev_block_index(block_buffer_head);
  // If there is just one block, no planning can be done. Avoid it!
  if (block_index != block_buffer_planned) {
    reverse_pass();
    forward_pass();
  }
  recalculate_trapezoids();
}

#if ENABLED(AUTOTEMP)

  void Planner::getHighESpeed() {
    static float oldt = 0;

    if (!autotemp_enabled) return;
    if (thermalManager.degTargetHotend(0) + 2 < autotemp_min) return; // probably temperature set to zero.

    float high = 0.0;
    for (uint8_t b = block_buffer_tail; b != block_buffer_head; b = next_block_index(b)) {
      block_t* block = &block_buffer[b];
      if (block->steps.x || block->steps.y || block->steps.z) {
        const float se = (float)block->steps.e / block->step_event_count * SQRT(block->nominal_speed_sqr); // mm/sec;
        NOLESS(high, se);
      }
    }

    float t = autotemp_min + high * autotemp_factor;
    LIMIT(t, autotemp_min, autotemp_max);
    if (t < oldt) t = t * (1 - float(AUTOTEMP_OLDWEIGHT)) + oldt * float(AUTOTEMP_OLDWEIGHT);
    oldt = t;
    thermalManager.setTargetHotend(t, 0);
  }

#endif // AUTOTEMP

/**
 * Maintain fans, paste extruder pressure,
 */
void Planner::check_axes_activity() {

  #if ANY(DISABLE_X, DISABLE_Y, DISABLE_Z, DISABLE_E)
    xyze_bool_t axis_active = { false };
  #endif

  #if HAS_FAN
    uint8_t tail_fan_speed[FAN_COUNT];
  #endif

  #if ENABLED(BARICUDA)
    #if HAS_HEATER_1
      uint8_t tail_valve_pressure;
    #endif
    #if HAS_HEATER_2
      uint8_t tail_e_to_p_pressure;
    #endif
  #endif

  if (has_blocks_queued()) {

    #if HAS_FAN || ENABLED(BARICUDA)
      block_t *block = &block_buffer[block_buffer_tail];
    #endif

    #if HAS_FAN
      FANS_LOOP(i)
        tail_fan_speed[i] = thermalManager.scaledFanSpeed(i, block->fan_speed[i]);
    #endif

    #if ENABLED(BARICUDA)
      TERN_(HAS_HEATER_1, tail_valve_pressure = block->valve_pressure);
      TERN_(HAS_HEATER_2, tail_e_to_p_pressure = block->e_to_p_pressure);
    #endif

    #if ANY(DISABLE_X, DISABLE_Y, DISABLE_Z, DISABLE_E)
      for (uint8_t b = block_buffer_tail; b != block_buffer_head; b = next_block_index(b)) {
        block_t *block = &block_buffer[b];
        LOOP_XYZE(i) if (block->steps[i]) axis_active[i] = true;
      }
    #endif
  }
  else {

    TERN_(HAS_CUTTER, cutter.refresh());

    #if HAS_FAN
      FANS_LOOP(i)
        tail_fan_speed[i] = thermalManager.scaledFanSpeed(i);
    #endif

    #if ENABLED(BARICUDA)
      TERN_(HAS_HEATER_1, tail_valve_pressure = baricuda_valve_pressure);
      TERN_(HAS_HEATER_2, tail_e_to_p_pressure = baricuda_e_to_p_pressure);
    #endif
  }

  //
  // Disable inactive axes
  //
  if (TERN0(DISABLE_X, !axis_active.x)) DISABLE_AXIS_X();
  if (TERN0(DISABLE_Y, !axis_active.y)) DISABLE_AXIS_Y();
  if (TERN0(DISABLE_Z, !axis_active.z)) DISABLE_AXIS_Z();
  if (TERN0(DISABLE_E, !axis_active.e)) disable_e_steppers();

  //
  // Update Fan speeds
  //
  #if HAS_FAN

    #if FAN_KICKSTART_TIME > 0
      static millis_t fan_kick_end[FAN_COUNT] = { 0 };
      #define KICKSTART_FAN(f)                         \
        if (tail_fan_speed[f]) {                       \
          millis_t ms = millis();                      \
          if (fan_kick_end[f] == 0) {                  \
            fan_kick_end[f] = ms + FAN_KICKSTART_TIME; \
            tail_fan_speed[f] = 255;                   \
          } else if (PENDING(ms, fan_kick_end[f]))     \
            tail_fan_speed[f] = 255;                   \
        } else fan_kick_end[f] = 0
    #else
      #define KICKSTART_FAN(f) NOOP
    #endif

    #if FAN_MIN_PWM != 0 || FAN_MAX_PWM != 255
      #define CALC_FAN_SPEED(f) (tail_fan_speed[f] ? map(tail_fan_speed[f], 1, 255, FAN_MIN_PWM, FAN_MAX_PWM) : FAN_OFF_PWM)
    #else
      #define CALC_FAN_SPEED(f) (tail_fan_speed[f] ?: FAN_OFF_PWM)
    #endif

    #if ENABLED(FAN_SOFT_PWM)
      #define _FAN_SET(F) thermalManager.soft_pwm_amount_fan[F] = CALC_FAN_SPEED(F);
    #elif ENABLED(FAST_PWM_FAN)
      #define _FAN_SET(F) set_pwm_duty(FAN##F##_PIN, CALC_FAN_SPEED(F));
    #else
      #define _FAN_SET(F) analogWrite(pin_t(FAN##F##_PIN), CALC_FAN_SPEED(F));
    #endif
    #define FAN_SET(F) do{ KICKSTART_FAN(F); _FAN_SET(F); }while(0)

    TERN_(HAS_FAN0, FAN_SET(0));
    TERN_(HAS_FAN1, FAN_SET(1));
    TERN_(HAS_FAN2, FAN_SET(2));
    TERN_(HAS_FAN3, FAN_SET(3));
    TERN_(HAS_FAN4, FAN_SET(4));
    TERN_(HAS_FAN5, FAN_SET(5));
    TERN_(HAS_FAN6, FAN_SET(6));
    TERN_(HAS_FAN7, FAN_SET(7));
  #endif // HAS_FAN

  TERN_(AUTOTEMP, getHighESpeed());

  #if ENABLED(BARICUDA)
    TERN_(HAS_HEATER_1, analogWrite(pin_t(HEATER_1_PIN), tail_valve_pressure));
    TERN_(HAS_HEATER_2, analogWrite(pin_t(HEATER_2_PIN), tail_e_to_p_pressure));
  #endif
}

#if DISABLED(NO_VOLUMETRICS)

  /**
   * Get a volumetric multiplier from a filament diameter.
   * This is the reciprocal of the circular cross-section area.
   * Return 1.0 with volumetric off or a diameter of 0.0.
   */
  inline float calculate_volumetric_multiplier(const float &diameter) {
    return (parser.volumetric_enabled && diameter) ? 1.0f / CIRCLE_AREA(diameter * 0.5f) : 1;
  }

  /**
   * Convert the filament sizes into volumetric multipliers.
   * The multiplier converts a given E value into a length.
   */
  void Planner::calculate_volumetric_multipliers() {
    LOOP_L_N(i, COUNT(filament_size)) {
      volumetric_multiplier[i] = calculate_volumetric_multiplier(filament_size[i]);
      refresh_e_factor(i);
    }
    #if ENABLED(VOLUMETRIC_EXTRUDER_LIMIT)
      calculate_volumetric_extruder_limits(); // update volumetric_extruder_limits as well.
    #endif
  }

#endif // !NO_VOLUMETRICS

#if ENABLED(VOLUMETRIC_EXTRUDER_LIMIT)

  /**
   * Convert volumetric based limits into pre calculated extruder feedrate limits.
   */
  void Planner::calculate_volumetric_extruder_limit(const uint8_t e) {
    const float &lim = volumetric_extruder_limit[e], &siz = filament_size[e];
    volumetric_extruder_feedrate_limit[e] = (lim && siz) ? lim / CIRCLE_AREA(siz * 0.5f) : 0;
  }
  void Planner::calculate_volumetric_extruder_limits() {
    LOOP_L_N(e, EXTRUDERS) calculate_volumetric_extruder_limit(e);
  }

#endif

#if ENABLED(FILAMENT_WIDTH_SENSOR)
  /**
   * Convert the ratio value given by the filament width sensor
   * into a volumetric multiplier. Conversion differs when using
   * linear extrusion vs volumetric extrusion.
   */
  void Planner::apply_filament_width_sensor(const int8_t encoded_ratio) {
    // Reconstitute the nominal/measured ratio
    const float nom_meas_ratio = 1 + 0.01f * encoded_ratio,
                ratio_2 = sq(nom_meas_ratio);

    volumetric_multiplier[FILAMENT_SENSOR_EXTRUDER_NUM] = parser.volumetric_enabled
      ? ratio_2 / CIRCLE_AREA(filwidth.nominal_mm * 0.5f) // Volumetric uses a true volumetric multiplier
      : ratio_2;                                          // Linear squares the ratio, which scales the volume

    refresh_e_factor(FILAMENT_SENSOR_EXTRUDER_NUM);
  }
#endif

#if HAS_LEVELING

  constexpr xy_pos_t level_fulcrum = {
    TERN(Z_SAFE_HOMING, Z_SAFE_HOMING_X_POINT, X_HOME_POS),
    TERN(Z_SAFE_HOMING, Z_SAFE_HOMING_Y_POINT, Y_HOME_POS)
  };

  /**
   * rx, ry, rz - Cartesian positions in mm
   *              Leveled XYZ on completion
   */
  void Planner::apply_leveling(xyz_pos_t &raw) {
    if (!leveling_active) return;

    #if ABL_PLANAR

      xy_pos_t d = raw - level_fulcrum;
      apply_rotation_xyz(bed_level_matrix, d.x, d.y, raw.z);
      raw = d + level_fulcrum;

    #elif HAS_MESH

      #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
        const float fade_scaling_factor = fade_scaling_factor_for_z(raw.z);
      #elif DISABLED(MESH_BED_LEVELING)
        constexpr float fade_scaling_factor = 1.0;
      #endif

      raw.z += (
        #if ENABLED(MESH_BED_LEVELING)
          mbl.get_z(raw
            #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
              , fade_scaling_factor
            #endif
          )
        #elif ENABLED(AUTO_BED_LEVELING_UBL)
          fade_scaling_factor ? fade_scaling_factor * ubl.get_z_correction(raw) : 0.0
        #elif ENABLED(AUTO_BED_LEVELING_BILINEAR)
          fade_scaling_factor ? fade_scaling_factor * bilinear_z_offset(raw) : 0.0
        #endif
      );

    #endif
  }

  void Planner::unapply_leveling(xyz_pos_t &raw) {

    if (leveling_active) {

      #if ABL_PLANAR

        matrix_3x3 inverse = matrix_3x3::transpose(bed_level_matrix);

        xy_pos_t d = raw - level_fulcrum;
        apply_rotation_xyz(inverse, d.x, d.y, raw.z);
        raw = d + level_fulcrum;

      #elif HAS_MESH

        #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
          const float fade_scaling_factor = fade_scaling_factor_for_z(raw.z);
        #elif DISABLED(MESH_BED_LEVELING)
          constexpr float fade_scaling_factor = 1.0;
        #endif

        raw.z -= (
          #if ENABLED(MESH_BED_LEVELING)
            mbl.get_z(raw
              #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
                , fade_scaling_factor
              #endif
            )
          #elif ENABLED(AUTO_BED_LEVELING_UBL)
            fade_scaling_factor ? fade_scaling_factor * ubl.get_z_correction(raw) : 0.0
          #elif ENABLED(AUTO_BED_LEVELING_BILINEAR)
            fade_scaling_factor ? fade_scaling_factor * bilinear_z_offset(raw) : 0.0
          #endif
        );

      #endif
    }
  }

#endif // HAS_LEVELING

#if ENABLED(FWRETRACT)
  /**
   * rz, e - Cartesian positions in mm
   */
  void Planner::apply_retract(float &rz, float &e) {
    rz += fwretract.current_hop;
    e -= fwretract.current_retract[active_extruder];
  }

  void Planner::unapply_retract(float &rz, float &e) {
    rz -= fwretract.current_hop;
    e += fwretract.current_retract[active_extruder];
  }

#endif

void Planner::quick_stop() {

  // Remove all the queued blocks. Note that this function is NOT
  // called from the Stepper ISR, so we must consider tail as readonly!
  // that is why we set head to tail - But there is a race condition that
  // must be handled: The tail could change between the read and the assignment
  // so this must be enclosed in a critical section

  const bool was_enabled = stepper.suspend();

  // Drop all queue entries
  block_buffer_nonbusy = block_buffer_planned = block_buffer_head = block_buffer_tail;

  // Restart the block delay for the first movement - As the queue was
  // forced to empty, there's no risk the ISR will touch this.
  delay_before_delivering = BLOCK_DELAY_FOR_1ST_MOVE;

  #if HAS_SPI_LCD
    // Clear the accumulated runtime
    clear_block_buffer_runtime();
  #endif

  // Make sure to drop any attempt of queuing moves for 1 second
  cleaning_buffer_counter = TEMP_TIMER_FREQUENCY;

  // Reenable Stepper ISR
  if (was_enabled) stepper.wake_up();

  // And stop the stepper ISR
  stepper.quick_stop();
}

void Planner::endstop_triggered(const AxisEnum axis) {
  // Record stepper position and discard the current block
  stepper.endstop_triggered(axis);
}

float Planner::triggered_position_mm(const AxisEnum axis) {
  return stepper.triggered_position(axis) * steps_to_mm[axis];
}

void Planner::finish_and_disable() {
  while (has_blocks_queued() || cleaning_buffer_counter) idle();
  disable_all_steppers();
}

/**
 * Get an axis position according to stepper position(s)
 * For CORE machines apply translation from ABC to XYZ.
 */
float Planner::get_axis_position_mm(const AxisEnum axis) {
  float axis_steps;
  #if IS_CORE
    // Requesting one of the "core" axes?
    if (axis == CORE_AXIS_1 || axis == CORE_AXIS_2) {

      // Protect the access to the position.
      const bool was_enabled = stepper.suspend();

      const int32_t p1 = stepper.position(CORE_AXIS_1),
                    p2 = stepper.position(CORE_AXIS_2);

      if (was_enabled) stepper.wake_up();

      // ((a1+a2)+(a1-a2))/2 -> (a1+a2+a1-a2)/2 -> (a1+a1)/2 -> a1
      // ((a1+a2)-(a1-a2))/2 -> (a1+a2-a1+a2)/2 -> (a2+a2)/2 -> a2
      axis_steps = (axis == CORE_AXIS_2 ? CORESIGN(p1 - p2) : p1 + p2) * 0.5f;
    }
    else
      axis_steps = stepper.position(axis);
  #else
    axis_steps = stepper.position(axis);
  #endif
  return axis_steps * steps_to_mm[axis];
}

/**
 * Block until all buffered steps are executed / cleaned
 */
void Planner::synchronize() {
  while (has_blocks_queued() || cleaning_buffer_counter
      || TERN0(EXTERNAL_CLOSED_LOOP_CONTROLLER, CLOSED_LOOP_WAITING())
  ) idle();
}

/**
 * Planner::_buffer_steps
 *
 * Add a new linear movement to the planner queue (in terms of steps).
 *
 *  target        - target position in steps units
 *  target_float  - target position in direct (mm, degrees) units. optional
 *  fr_mm_s       - (target) speed of the move
 *  extruder      - target extruder
 *  millimeters   - the length of the movement, if known
 *
 * Returns true if movement was properly queued, false otherwise
 */
bool Planner::_buffer_steps(const xyze_long_t &target
  #if HAS_POSITION_FLOAT
    , const xyze_pos_t &target_float
  #endif
  #if HAS_DIST_MM_ARG
    , const xyze_float_t &cart_dist_mm
  #endif
  , feedRate_t fr_mm_s, const uint8_t extruder, const float &millimeters
) {

  // If we are cleaning, do not accept queuing of movements
  if (cleaning_buffer_counter) return false;

  // Wait for the next available block
  uint8_t next_buffer_head;
  block_t * const block = get_next_free_block(next_buffer_head);

  // Fill the block with the specified movement
  if (!_populate_block(block, false, target
    #if HAS_POSITION_FLOAT
      , target_float
    #endif
    #if HAS_DIST_MM_ARG
      , cart_dist_mm
    #endif
    , fr_mm_s, extruder, millimeters
  )) {
    // Movement was not queued, probably because it was too short.
    //  Simply accept that as movement queued and done
    return true;
  }

  // If this is the first added movement, reload the delay, otherwise, cancel it.
  if (block_buffer_head == block_buffer_tail) {
    // If it was the first queued block, restart the 1st block delivery delay, to
    // give the planner an opportunity to queue more movements and plan them
    // As there are no queued movements, the Stepper ISR will not touch this
    // variable, so there is no risk setting this here (but it MUST be done
    // before the following line!!)
    delay_before_delivering = BLOCK_DELAY_FOR_1ST_MOVE;
  }

  // Move buffer head
  block_buffer_head = next_buffer_head;

  // Recalculate and optimize trapezoidal speed profiles
  recalculate();

  // Movement successfully queued!
  return true;
}

/**
 * Planner::_populate_block
 *
 * Fills a new linear movement in the block (in terms of steps).
 *
 *  target      - target position in steps units
 *  fr_mm_s     - (target) speed of the move
 *  extruder    - target extruder
 *
 * Returns true if movement is acceptable, false otherwise
 */
bool Planner::_populate_block(block_t * const block, bool split_move,
  const abce_long_t &target
  #if HAS_POSITION_FLOAT
    , const xyze_pos_t &target_float
  #endif
  #if HAS_DIST_MM_ARG
    , const xyze_float_t &cart_dist_mm
  #endif
  , feedRate_t fr_mm_s, const uint8_t extruder, const float &millimeters/*=0.0*/
) {

  const int32_t da = target.a - position.a,
                db = target.b - position.b,
                dc = target.c - position.c;

  #if EXTRUDERS
    int32_t de = target.e - position.e;
  #else
    constexpr int32_t de = 0;
  #endif

  /* <-- add a slash to enable
    SERIAL_ECHOLNPAIR("  _populate_block FR:", fr_mm_s,
                      " A:", target.a, " (", da, " steps)"
                      " B:", target.b, " (", db, " steps)"
                      " C:", target.c, " (", dc, " steps)"
                      #if EXTRUDERS
                        " E:", target.e, " (", de, " steps)"
                      #endif
                    );
  //*/

  #if EITHER(PREVENT_COLD_EXTRUSION, PREVENT_LENGTHY_EXTRUDE)
    if (de) {
      #if ENABLED(PREVENT_COLD_EXTRUSION)
        if (thermalManager.tooColdToExtrude(extruder)) {
          position.e = target.e; // Behave as if the move really took place, but ignore E part
          TERN_(HAS_POSITION_FLOAT, position_float.e = target_float.e);
          de = 0; // no difference
          SERIAL_ECHO_MSG(STR_ERR_COLD_EXTRUDE_STOP);
        }
      #endif // PREVENT_COLD_EXTRUSION
      #if ENABLED(PREVENT_LENGTHY_EXTRUDE)
        const float e_steps = ABS(de * e_factor[extruder]);
        const float max_e_steps = settings.axis_steps_per_mm[E_AXIS_N(extruder)] * (EXTRUDE_MAXLENGTH);
        if (e_steps > max_e_steps) {
          #if ENABLED(MIXING_EXTRUDER)
            bool ignore_e = false;
            float collector[MIXING_STEPPERS];
            mixer.refresh_collector(1.0, mixer.get_current_vtool(), collector);
            MIXER_STEPPER_LOOP(e)
              if (e_steps * collector[e] > max_e_steps) { ignore_e = true; break; }
          #else
            constexpr bool ignore_e = true;
          #endif
          if (ignore_e) {
            position.e = target.e; // Behave as if the move really took place, but ignore E part
            TERN_(HAS_POSITION_FLOAT, position_float.e = target_float.e);
            de = 0; // no difference
            SERIAL_ECHO_MSG(STR_ERR_LONG_EXTRUDE_STOP);
          }
        }
      #endif // PREVENT_LENGTHY_EXTRUDE
    }
  #endif // PREVENT_COLD_EXTRUSION || PREVENT_LENGTHY_EXTRUDE

  // Compute direction bit-mask for this block
  uint8_t dm = 0;
  #if CORE_IS_XY
    if (da < 0) SBI(dm, X_HEAD);                // Save the real Extruder (head) direction in X Axis
    if (db < 0) SBI(dm, Y_HEAD);                // ...and Y
    if (dc < 0) SBI(dm, Z_AXIS);
    if (da + db < 0) SBI(dm, A_AXIS);           // Motor A direction
    if (CORESIGN(da - db) < 0) SBI(dm, B_AXIS); // Motor B direction
  #elif CORE_IS_XZ
    if (da < 0) SBI(dm, X_HEAD);                // Save the real Extruder (head) direction in X Axis
    if (db < 0) SBI(dm, Y_AXIS);
    if (dc < 0) SBI(dm, Z_HEAD);                // ...and Z
    if (da + dc < 0) SBI(dm, A_AXIS);           // Motor A direction
    if (CORESIGN(da - dc) < 0) SBI(dm, C_AXIS); // Motor C direction
  #elif CORE_IS_YZ
    if (da < 0) SBI(dm, X_AXIS);
    if (db < 0) SBI(dm, Y_HEAD);                // Save the real Extruder (head) direction in Y Axis
    if (dc < 0) SBI(dm, Z_HEAD);                // ...and Z
    if (db + dc < 0) SBI(dm, B_AXIS);           // Motor B direction
    if (CORESIGN(db - dc) < 0) SBI(dm, C_AXIS); // Motor C direction
  #else
    if (da < 0) SBI(dm, X_AXIS);
    if (db < 0) SBI(dm, Y_AXIS);
    if (dc < 0) SBI(dm, Z_AXIS);
  #endif
  if (de < 0) SBI(dm, E_AXIS);

  #if EXTRUDERS
    const float esteps_float = de * e_factor[extruder];
    const uint32_t esteps = ABS(esteps_float) + 0.5f;
  #else
    constexpr uint32_t esteps = 0;
  #endif

  // Clear all flags, including the "busy" bit
  block->flag = 0x00;

  // Set direction bits
  block->direction_bits = dm;

  // Update block laser power
  #if ENABLED(LASER_POWER_INLINE)
    laser_inline.status.isPlanned = true;
    block->laser.status = laser_inline.status;
    block->laser.power = laser_inline.power;
  #endif

  // Number of steps for each axis
  // See http://www.corexy.com/theory.html
  #if CORE_IS_XY
    block->steps.set(ABS(da + db), ABS(da - db), ABS(dc));
  #elif CORE_IS_XZ
    block->steps.set(ABS(da + dc), ABS(db), ABS(da - dc));
  #elif CORE_IS_YZ
    block->steps.set(ABS(da), ABS(db + dc), ABS(db - dc));
  #elif IS_SCARA
    block->steps.set(ABS(da), ABS(db), ABS(dc));
  #else
    // default non-h-bot planning
    block->steps.set(ABS(da), ABS(db), ABS(dc));
  #endif

  /**
   * This part of the code calculates the total length of the movement.
   * For cartesian bots, the X_AXIS is the real X movement and same for Y_AXIS.
   * But for corexy bots, that is not true. The "X_AXIS" and "Y_AXIS" motors (that should be named to A_AXIS
   * and B_AXIS) cannot be used for X and Y length, because A=X+Y and B=X-Y.
   * So we need to create other 2 "AXIS", named X_HEAD and Y_HEAD, meaning the real displacement of the Head.
   * Having the real displacement of the head, we can calculate the total movement length and apply the desired speed.
   */
  struct DistanceMM : abce_float_t {
    TERN_(IS_CORE, xyz_pos_t head);
  } steps_dist_mm;
  #if IS_CORE
    #if CORE_IS_XY
      steps_dist_mm.head.x = da * steps_to_mm[A_AXIS];
      steps_dist_mm.head.y = db * steps_to_mm[B_AXIS];
      steps_dist_mm.z      = dc * steps_to_mm[Z_AXIS];
      steps_dist_mm.a      = (da + db) * steps_to_mm[A_AXIS];
      steps_dist_mm.b      = CORESIGN(da - db) * steps_to_mm[B_AXIS];
    #elif CORE_IS_XZ
      steps_dist_mm.head.x = da * steps_to_mm[A_AXIS];
      steps_dist_mm.y      = db * steps_to_mm[Y_AXIS];
      steps_dist_mm.head.z = dc * steps_to_mm[C_AXIS];
      steps_dist_mm.a      = (da + dc) * steps_to_mm[A_AXIS];
      steps_dist_mm.c      = CORESIGN(da - dc) * steps_to_mm[C_AXIS];
    #elif CORE_IS_YZ
      steps_dist_mm.x      = da * steps_to_mm[X_AXIS];
      steps_dist_mm.head.y = db * steps_to_mm[B_AXIS];
      steps_dist_mm.head.z = dc * steps_to_mm[C_AXIS];
      steps_dist_mm.b      = (db + dc) * steps_to_mm[B_AXIS];
      steps_dist_mm.c      = CORESIGN(db - dc) * steps_to_mm[C_AXIS];
    #endif
  #else
    steps_dist_mm.a = da * steps_to_mm[A_AXIS];
    steps_dist_mm.b = db * steps_to_mm[B_AXIS];
    steps_dist_mm.c = dc * steps_to_mm[C_AXIS];
  #endif

  #if EXTRUDERS
    steps_dist_mm.e = esteps_float * steps_to_mm[E_AXIS_N(extruder)];
  #else
    steps_dist_mm.e = 0.0f;
  #endif

  TERN_(LCD_SHOW_E_TOTAL, e_move_accumulator += steps_dist_mm.e);

  if (block->steps.a < MIN_STEPS_PER_SEGMENT && block->steps.b < MIN_STEPS_PER_SEGMENT && block->steps.c < MIN_STEPS_PER_SEGMENT) {
    block->millimeters = (0
      #if EXTRUDERS
        + ABS(steps_dist_mm.e)
      #endif
    );
  }
  else {
    if (millimeters)
      block->millimeters = millimeters;
    else
      block->millimeters = SQRT(
        #if CORE_IS_XY
          sq(steps_dist_mm.head.x) + sq(steps_dist_mm.head.y) + sq(steps_dist_mm.z)
        #elif CORE_IS_XZ
          sq(steps_dist_mm.head.x) + sq(steps_dist_mm.y) + sq(steps_dist_mm.head.z)
        #elif CORE_IS_YZ
          sq(steps_dist_mm.x) + sq(steps_dist_mm.head.y) + sq(steps_dist_mm.head.z)
        #else
          sq(steps_dist_mm.x) + sq(steps_dist_mm.y) + sq(steps_dist_mm.z)
        #endif
      );

    /**
     * At this point at least one of the axes has more steps than
     * MIN_STEPS_PER_SEGMENT, ensuring the segment won't get dropped as
     * zero-length. It's important to not apply corrections
     * to blocks that would get dropped!
     *
     * A correction function is permitted to add steps to an axis, it
     * should *never* remove steps!
     */
    TERN_(BACKLASH_COMPENSATION, backlash.add_correction_steps(da, db, dc, dm, block));
  }

  #if EXTRUDERS
    block->steps.e = esteps;
  #endif

  block->step_event_count = _MAX(block->steps.a, block->steps.b, block->steps.c, esteps);

  // Bail if this is a zero-length block
  if (block->step_event_count < MIN_STEPS_PER_SEGMENT) return false;

  #if ENABLED(MIXING_EXTRUDER)
    MIXER_POPULATE_BLOCK();
  #endif

  TERN_(HAS_CUTTER, block->cutter_power = cutter.power);

  #if HAS_FAN
    FANS_LOOP(i) block->fan_speed[i] = thermalManager.fan_speed[i];
  #endif

  #if ENABLED(BARICUDA)
    block->valve_pressure = baricuda_valve_pressure;
    block->e_to_p_pressure = baricuda_e_to_p_pressure;
  #endif

  #if EXTRUDERS > 1
    block->extruder = extruder;
  #endif

  #if ENABLED(AUTO_POWER_CONTROL)
    if (block->steps.x || block->steps.y || block->steps.z)
      powerManager.power_on();
  #endif

  // Enable active axes
  #if CORE_IS_XY
    if (block->steps.a || block->steps.b) {
      ENABLE_AXIS_X();
      ENABLE_AXIS_Y();
    }
    #if DISABLED(Z_LATE_ENABLE)
      if (block->steps.z) ENABLE_AXIS_Z();
    #endif
  #elif CORE_IS_XZ
    if (block->steps.a || block->steps.c) {
      ENABLE_AXIS_X();
      ENABLE_AXIS_Z();
    }
    if (block->steps.y) ENABLE_AXIS_Y();
  #elif CORE_IS_YZ
    if (block->steps.b || block->steps.c) {
      ENABLE_AXIS_Y();
      ENABLE_AXIS_Z();
    }
    if (block->steps.x) ENABLE_AXIS_X();
  #else
    if (block->steps.x) ENABLE_AXIS_X();
    if (block->steps.y) ENABLE_AXIS_Y();
    #if DISABLED(Z_LATE_ENABLE)
      if (block->steps.z) ENABLE_AXIS_Z();
    #endif
  #endif

  // Enable extruder(s)
  #if EXTRUDERS
    if (esteps) {
      TERN_(AUTO_POWER_CONTROL, powerManager.power_on());

      #if ENABLED(DISABLE_INACTIVE_EXTRUDER) // Enable only the selected extruder

        LOOP_L_N(i, EXTRUDERS)
          if (g_uc_extruder_last_move[i] > 0) g_uc_extruder_last_move[i]--;

        #if HAS_DUPLICATION_MODE
          if (extruder_duplication_enabled && extruder == 0) {
            ENABLE_AXIS_E1();
            g_uc_extruder_last_move[1] = (BLOCK_BUFFER_SIZE) * 2;
          }
        #endif

        #define ENABLE_ONE_E(N) do{ \
          if (extruder == N) { \
            ENABLE_AXIS_E##N(); \
            g_uc_extruder_last_move[N] = (BLOCK_BUFFER_SIZE) * 2; \
          } \
          else if (!g_uc_extruder_last_move[N]) \
            DISABLE_AXIS_E##N(); \
        }while(0);

      #else

        #define ENABLE_ONE_E(N) ENABLE_AXIS_E##N();

      #endif

      REPEAT(EXTRUDERS, ENABLE_ONE_E); // (ENABLE_ONE_E must end with semicolon)
    }
  #endif // EXTRUDERS

  if (esteps)
    NOLESS(fr_mm_s, settings.min_feedrate_mm_s);
  else
    NOLESS(fr_mm_s, settings.min_travel_feedrate_mm_s);

  const float inverse_millimeters = 1.0f / block->millimeters;  // Inverse millimeters to remove multiple divides

  // Calculate inverse time for this move. No divide by zero due to previous checks.
  // Example: At 120mm/s a 60mm move takes 0.5s. So this will give 2.0.
  float inverse_secs = fr_mm_s * inverse_millimeters;

  // Get the number of non busy movements in queue (non busy means that they can be altered)
  const uint8_t moves_queued = nonbusy_movesplanned();

  // Slow down when the buffer starts to empty, rather than wait at the corner for a buffer refill
  #if EITHER(SLOWDOWN, HAS_SPI_LCD) || defined(XY_FREQUENCY_LIMIT)
    // Segment time im micro seconds
    int32_t segment_time_us = LROUND(1000000.0f / inverse_secs);
  #endif

  #if ENABLED(SLOWDOWN)
    #ifndef SLOWDOWN_DIVISOR
      #define SLOWDOWN_DIVISOR 2
    #endif
    if (WITHIN(moves_queued, 2, (BLOCK_BUFFER_SIZE) / (SLOWDOWN_DIVISOR) - 1)) {
      const int32_t time_diff = settings.min_segment_time_us - segment_time_us;
      if (time_diff > 0) {
        // Buffer is draining so add extra time. The amount of time added increases if the buffer is still emptied more.
        const int32_t nst = segment_time_us + LROUND(2 * time_diff / moves_queued);
        inverse_secs = 1000000.0f / nst;
        #if defined(XY_FREQUENCY_LIMIT) || HAS_SPI_LCD
          segment_time_us = nst;
        #endif
      }
    }
  #endif

  #if HAS_SPI_LCD
    // Protect the access to the position.
    const bool was_enabled = stepper.suspend();

    block_buffer_runtime_us += segment_time_us;
    block->segment_time_us = segment_time_us;

    if (was_enabled) stepper.wake_up();
  #endif

  block->nominal_speed_sqr = sq(block->millimeters * inverse_secs);   // (mm/sec)^2 Always > 0
  block->nominal_rate = CEIL(block->step_event_count * inverse_secs); // (step/sec) Always > 0

  #if ENABLED(FILAMENT_WIDTH_SENSOR)
    if (extruder == FILAMENT_SENSOR_EXTRUDER_NUM)   // Only for extruder with filament sensor
      filwidth.advance_e(steps_dist_mm.e);
  #endif

  // Calculate and limit speed in mm/sec

  xyze_float_t current_speed;
  float speed_factor = 1.0f; // factor <1 decreases speed

  // Linear axes first with less logic
  LOOP_XYZ(i) {
    current_speed[i] = steps_dist_mm[i] * inverse_secs;
    const feedRate_t cs = ABS(current_speed[i]),
                 max_fr = settings.max_feedrate_mm_s[i];
    if (cs > max_fr) NOMORE(speed_factor, max_fr / cs);
  }

  // Limit speed on extruders, if any
  #if EXTRUDERS
    {
      current_speed.e = steps_dist_mm.e * inverse_secs;
      #if HAS_MIXER_SYNC_CHANNEL
        // Move all mixing extruders at the specified rate
        if (mixer.get_current_vtool() == MIXER_AUTORETRACT_TOOL)
          current_speed.e *= MIXING_STEPPERS;
      #endif

      const feedRate_t cs = ABS(current_speed.e),
                   max_fr = settings.max_feedrate_mm_s[E_AXIS_N(extruder)]
                            * TERN(HAS_MIXER_SYNC_CHANNEL, MIXING_STEPPERS, 1);

      if (cs > max_fr) NOMORE(speed_factor, max_fr / cs); //respect max feedrate on any movement (doesn't matter if E axes only or not)

      #if ENABLED(VOLUMETRIC_EXTRUDER_LIMIT)
        const feedRate_t max_vfr = volumetric_extruder_feedrate_limit[extruder]
                                   * TERN(HAS_MIXER_SYNC_CHANNEL, MIXING_STEPPERS, 1);

        // TODO: Doesn't work properly for joined segments. Set MIN_STEPS_PER_SEGMENT 1 as workaround.

        if (block->steps.a || block->steps.b || block->steps.c) {

          if (max_vfr > 0 && cs > max_vfr) {
            NOMORE(speed_factor, max_vfr / cs); // respect volumetric extruder limit (if any)
            /* <-- add a slash to enable
            SERIAL_ECHOPAIR("volumetric extruder limit enforced: ", (cs * CIRCLE_AREA(filament_size[extruder] * 0.5f)));
            SERIAL_ECHOPAIR(" mm^3/s (", cs);
            SERIAL_ECHOPAIR(" mm/s) limited to ", (max_vfr * CIRCLE_AREA(filament_size[extruder] * 0.5f)));
            SERIAL_ECHOPAIR(" mm^3/s (", max_vfr);
            SERIAL_ECHOLNPGM(" mm/s)");
            //*/
          }
        }
      #endif
    }
  #endif

  #ifdef XY_FREQUENCY_LIMIT

    static uint8_t old_direction_bits; // = 0

    if (xy_freq_limit_hz) {
      // Check and limit the xy direction change frequency
      const uint8_t direction_change = block->direction_bits ^ old_direction_bits;
      old_direction_bits = block->direction_bits;
      segment_time_us = LROUND(float(segment_time_us) / speed_factor);

      static int32_t xs0, xs1, xs2, ys0, ys1, ys2;
      if (segment_time_us > xy_freq_min_interval_us)
        xs2 = xs1 = ys2 = ys1 = xy_freq_min_interval_us;
      else {
        xs2 = xs1; xs1 = xs0;
        ys2 = ys1; ys1 = ys0;
      }
      xs0 = TEST(direction_change, X_AXIS) ? segment_time_us : xy_freq_min_interval_us;
      ys0 = TEST(direction_change, Y_AXIS) ? segment_time_us : xy_freq_min_interval_us;

      if (segment_time_us < xy_freq_min_interval_us) {
        const int32_t least_xy_segment_time = _MIN(_MAX(xs0, xs1, xs2), _MAX(ys0, ys1, ys2));
        if (least_xy_segment_time < xy_freq_min_interval_us) {
          float freq_xy_feedrate = (speed_factor * least_xy_segment_time) / xy_freq_min_interval_us;
          NOLESS(freq_xy_feedrate, xy_freq_min_speed_factor);
          NOMORE(speed_factor, freq_xy_feedrate);
        }
      }
    }

  #endif // XY_FREQUENCY_LIMIT

  // Correct the speed
  if (speed_factor < 1.0f) {
    current_speed *= speed_factor;
    block->nominal_rate *= speed_factor;
    block->nominal_speed_sqr = block->nominal_speed_sqr * sq(speed_factor);
  }

  // Compute and limit the acceleration rate for the trapezoid generator.
  const float steps_per_mm = block->step_event_count * inverse_millimeters;
  uint32_t accel;
  if (!block->steps.a && !block->steps.b && !block->steps.c) {
    // convert to: acceleration steps/sec^2
    accel = CEIL(settings.retract_acceleration * steps_per_mm);
    TERN_(LIN_ADVANCE, block->use_advance_lead = false);
  }
  else {
    #define LIMIT_ACCEL_LONG(AXIS,INDX) do{ \
      if (block->steps[AXIS] && max_acceleration_steps_per_s2[AXIS+INDX] < accel) { \
        const uint32_t comp = max_acceleration_steps_per_s2[AXIS+INDX] * block->step_event_count; \
        if (accel * block->steps[AXIS] > comp) accel = comp / block->steps[AXIS]; \
      } \
    }while(0)

    #define LIMIT_ACCEL_FLOAT(AXIS,INDX) do{ \
      if (block->steps[AXIS] && max_acceleration_steps_per_s2[AXIS+INDX] < accel) { \
        const float comp = (float)max_acceleration_steps_per_s2[AXIS+INDX] * (float)block->step_event_count; \
        if ((float)accel * (float)block->steps[AXIS] > comp) accel = comp / (float)block->steps[AXIS]; \
      } \
    }while(0)

    // Start with print or travel acceleration
    accel = CEIL((esteps ? settings.acceleration : settings.travel_acceleration) * steps_per_mm);

    #if ENABLED(LIN_ADVANCE)

      #define MAX_E_JERK(N) TERN(HAS_LINEAR_E_JERK, max_e_jerk[E_INDEX_N(N)], max_jerk.e)

      /**
       *
       * Use LIN_ADVANCE for blocks if all these are true:
       *
       * esteps             : This is a print move, because we checked for A, B, C steps before.
       *
       * extruder_advance_K[active_extruder] : There is an advance factor set for this extruder.
       *
       * de > 0             : Extruder is running forward (e.g., for "Wipe while retracting" (Slic3r) or "Combing" (Cura) moves)
       */
      block->use_advance_lead =  esteps
                              && extruder_advance_K[active_extruder]
                              && de > 0;

      if (block->use_advance_lead) {
        block->e_D_ratio = (target_float.e - position_float.e) /
          #if IS_KINEMATIC
            block->millimeters
          #else
            SQRT(sq(target_float.x - position_float.x)
               + sq(target_float.y - position_float.y)
               + sq(target_float.z - position_float.z))
          #endif
        ;

        // Check for unusual high e_D ratio to detect if a retract move was combined with the last print move due to min. steps per segment. Never execute this with advance!
        // This assumes no one will use a retract length of 0mm < retr_length < ~0.2mm and no one will print 100mm wide lines using 3mm filament or 35mm wide lines using 1.75mm filament.
        if (block->e_D_ratio > 3.0f)
          block->use_advance_lead = false;
        else {
          const uint32_t max_accel_steps_per_s2 = MAX_E_JERK(extruder) / (extruder_advance_K[active_extruder] * block->e_D_ratio) * steps_per_mm;
          if (TERN0(LA_DEBUG, accel > max_accel_steps_per_s2))
            SERIAL_ECHOLNPGM("Acceleration limited.");
          NOMORE(accel, max_accel_steps_per_s2);
        }
      }
    #endif

    // Limit acceleration per axis
    if (block->step_event_count <= cutoff_long) {
      LIMIT_ACCEL_LONG(A_AXIS, 0);
      LIMIT_ACCEL_LONG(B_AXIS, 0);
      LIMIT_ACCEL_LONG(C_AXIS, 0);
      LIMIT_ACCEL_LONG(E_AXIS, E_INDEX_N(extruder));
    }
    else {
      LIMIT_ACCEL_FLOAT(A_AXIS, 0);
      LIMIT_ACCEL_FLOAT(B_AXIS, 0);
      LIMIT_ACCEL_FLOAT(C_AXIS, 0);
      LIMIT_ACCEL_FLOAT(E_AXIS, E_INDEX_N(extruder));
    }
  }
  block->acceleration_steps_per_s2 = accel;
  block->acceleration = accel / steps_per_mm;
  #if DISABLED(S_CURVE_ACCELERATION)
    block->acceleration_rate = (uint32_t)(accel * (4096.0f * 4096.0f / (STEPPER_TIMER_RATE)));
  #endif
  #if ENABLED(LIN_ADVANCE)
    if (block->use_advance_lead) {
      block->advance_speed = (STEPPER_TIMER_RATE) / (extruder_advance_K[active_extruder] * block->e_D_ratio * block->acceleration * settings.axis_steps_per_mm[E_AXIS_N(extruder)]);
      #if ENABLED(LA_DEBUG)
        if (extruder_advance_K[active_extruder] * block->e_D_ratio * block->acceleration * 2 < SQRT(block->nominal_speed_sqr) * block->e_D_ratio)
          SERIAL_ECHOLNPGM("More than 2 steps per eISR loop executed.");
        if (block->advance_speed < 200)
          SERIAL_ECHOLNPGM("eISR running at > 10kHz.");
      #endif
    }
  #endif

  float vmax_junction_sqr; // Initial limit on the segment entry velocity (mm/s)^2

  #if HAS_JUNCTION_DEVIATION
    /**
     * Compute maximum allowable entry speed at junction by centripetal acceleration approximation.
     * Let a circle be tangent to both previous and current path line segments, where the junction
     * deviation is defined as the distance from the junction to the closest edge of the circle,
     * colinear with the circle center. The circular segment joining the two paths represents the
     * path of centripetal acceleration. Solve for max velocity based on max acceleration about the
     * radius of the circle, defined indirectly by junction deviation. This may be also viewed as
     * path width or max_jerk in the previous Grbl version. This approach does not actually deviate
     * from path, but used as a robust way to compute cornering speeds, as it takes into account the
     * nonlinearities of both the junction angle and junction velocity.
     *
     * NOTE: If the junction deviation value is finite, Grbl executes the motions in an exact path
     * mode (G61). If the junction deviation value is zero, Grbl will execute the motion in an exact
     * stop mode (G61.1) manner. In the future, if continuous mode (G64) is desired, the math here
     * is exactly the same. Instead of motioning all the way to junction point, the machine will
     * just follow the arc circle defined here. The Arduino doesn't have the CPU cycles to perform
     * a continuous mode path, but ARM-based microcontrollers most certainly do.
     *
     * NOTE: The max junction speed is a fixed value, since machine acceleration limits cannot be
     * changed dynamically during operation nor can the line move geometry. This must be kept in
     * memory in the event of a feedrate override changing the nominal speeds of blocks, which can
     * change the overall maximum entry speed conditions of all blocks.
     *
     * #######
     * https://github.com/MarlinFirmware/Marlin/issues/10341#issuecomment-388191754
     *
     * hoffbaked: on May 10 2018 tuned and improved the GRBL algorithm for Marlin:
          Okay! It seems to be working good. I somewhat arbitrarily cut it off at 1mm
          on then on anything with less sides than an octagon. With this, and the
          reverse pass actually recalculating things, a corner acceleration value
          of 1000 junction deviation of .05 are pretty reasonable. If the cycles
          can be spared, a better acos could be used. For all I know, it may be
          already calculated in a different place. */

    // Unit vector of previous path line segment
    static xyze_float_t prev_unit_vec;

    xyze_float_t unit_vec =
      #if HAS_DIST_MM_ARG
        cart_dist_mm
      #else
        { steps_dist_mm.x, steps_dist_mm.y, steps_dist_mm.z, steps_dist_mm.e }
      #endif
    ;

    /**
     * On CoreXY the length of the vector [A,B] is SQRT(2) times the length of the head movement vector [X,Y].
     * So taking Z and E into account, we cannot scale to a unit vector with "inverse_millimeters".
     * => normalize the complete junction vector.
     * Elsewise, when needed JD factors in the E component
     */
    if (ENABLED(IS_CORE) || esteps > 0)
      normalize_junction_vector(unit_vec);  // Normalize with XYZE components
    else
      unit_vec *= inverse_millimeters;      // Use pre-calculated (1 / SQRT(x^2 + y^2 + z^2))

    // Skip first block or when previous_nominal_speed is used as a flag for homing and offset cycles.
    if (moves_queued && !UNEAR_ZERO(previous_nominal_speed_sqr)) {
      // Compute cosine of angle between previous and current path. (prev_unit_vec is negative)
      // NOTE: Max junction velocity is computed without sin() or acos() by trig half angle identity.
      float junction_cos_theta = (-prev_unit_vec.x * unit_vec.x) + (-prev_unit_vec.y * unit_vec.y)
                               + (-prev_unit_vec.z * unit_vec.z) + (-prev_unit_vec.e * unit_vec.e);

      // NOTE: Computed without any expensive trig, sin() or acos(), by trig half angle identity of cos(theta).
      if (junction_cos_theta > 0.999999f) {
        // For a 0 degree acute junction, just set minimum junction speed.
        vmax_junction_sqr = sq(float(MINIMUM_PLANNER_SPEED));
      }
      else {
        NOLESS(junction_cos_theta, -0.999999f); // Check for numerical round-off to avoid divide by zero.

        // Convert delta vector to unit vector
        xyze_float_t junction_unit_vec = unit_vec - prev_unit_vec;
        normalize_junction_vector(junction_unit_vec);

        const float junction_acceleration = limit_value_by_axis_maximum(block->acceleration, junction_unit_vec),
                    sin_theta_d2 = SQRT(0.5f * (1.0f - junction_cos_theta)); // Trig half angle identity. Always positive.

        vmax_junction_sqr = junction_acceleration * junction_deviation_mm * sin_theta_d2 / (1.0f - sin_theta_d2);

        #if ENABLED(JD_HANDLE_SMALL_SEGMENTS)

          // For small moves with >135° junction (octagon) find speed for approximate arc
          if (block->millimeters < 1 && junction_cos_theta < -0.7071067812f) {

            #if ENABLED(JD_USE_MATH_ACOS)

              #error "TODO: Inline maths with the MCU / FPU."

            #elif ENABLED(JD_USE_LOOKUP_TABLE)

              // Fast acos approximation (max. error +-0.01 rads)
              // Based on LUT table and linear interpolation

              /**
               *  // Generate the JD Lookup Table
               *  constexpr float c = 1.00751495f; // Correction factor to center error around 0
               *  for (int i = 0; i < jd_lut_count - 1; ++i) {
               *    const float x0 = (sq(i) - 1) / sq(i),
               *                y0 = acos(x0) * (i == 0 ? 1 : c),
               *                x1 = i < jd_lut_count - 1 ?  0.5 * x0 + 0.5 : 0.999999f,
               *                y1 = acos(x1) * (i < jd_lut_count - 1 ? c : 1);
               *    jd_lut_k[i] = (y0 - y1) / (x0 - x1);
               *    jd_lut_b[i] = (y1 * x0 - y0 * x1) / (x0 - x1);
               *  }
               *
               *  // Compute correction factor (Set c to 1.0f first!)
               *  float min = INFINITY, max = -min;
               *  for (float t = 0; t <= 1; t += 0.0003f) {
               *    const float e = acos(t) / approx(t);
               *    if (isfinite(e)) {
               *      if (e < min) min = e;
               *      if (e > max) max = e;
               *    }
               *  }
               *  fprintf(stderr, "%.9gf, ", (min + max) / 2);
               */
              static constexpr int16_t  jd_lut_count = 16;
              static constexpr uint16_t jd_lut_tll   = _BV(jd_lut_count - 1);
              static constexpr int16_t  jd_lut_tll0  = __builtin_clz(jd_lut_tll) + 1; // i.e., 16 - jd_lut_count + 1
              static constexpr float jd_lut_k[jd_lut_count] PROGMEM = {
                -1.03145837f, -1.30760646f, -1.75205851f, -2.41705704f,
                -3.37769222f, -4.74888992f, -6.69649887f, -9.45661736f,
                -13.3640480f, -18.8928222f, -26.7136841f, -37.7754593f,
                -53.4201813f, -75.5458374f, -106.836761f, -218.532821f };
              static constexpr float jd_lut_b[jd_lut_count] PROGMEM = {
                 1.57079637f,  1.70887053f,  2.04220939f,  2.62408352f,
                 3.52467871f,  4.85302639f,  6.77020454f,  9.50875854f,
                 13.4009285f,  18.9188995f,  26.7321243f,  37.7885055f,
                 53.4293975f,  75.5523529f,  106.841369f,  218.534011f };

              const float neg = junction_cos_theta < 0 ? -1 : 1,
                          t = neg * junction_cos_theta;

              const int16_t idx = (t < 0.00000003f) ? 0 : __builtin_clz(uint16_t((1.0f - t) * jd_lut_tll)) - jd_lut_tll0;

              float junction_theta = t * pgm_read_float(&jd_lut_k[idx]) + pgm_read_float(&jd_lut_b[idx]);
              if (neg > 0) junction_theta = RADIANS(180) - junction_theta; // acos(-t)

            #else

              // Fast acos(-t) approximation (max. error +-0.033rad = 1.89°)
              // Based on MinMax polynomial published by W. Randolph Franklin, see
              // https://wrf.ecse.rpi.edu/Research/Short_Notes/arcsin/onlyelem.html
              //  acos( t) = pi / 2 - asin(x)
              //  acos(-t) = pi - acos(t) ... pi / 2 + asin(x)

              const float neg = junction_cos_theta < 0 ? -1 : 1,
                          t = neg * junction_cos_theta,
                          asinx =       0.032843707f
                                + t * (-1.451838349f
                                + t * ( 29.66153956f
                                + t * (-131.1123477f
                                + t * ( 262.8130562f
                                + t * (-242.7199627f
                                + t * ( 84.31466202f ) ))))),
                          junction_theta = RADIANS(90) + neg * asinx; // acos(-t)

              // NOTE: junction_theta bottoms out at 0.033 which avoids divide by 0.

            #endif

            const float limit_sqr = (block->millimeters * junction_acceleration) / junction_theta;
            NOMORE(vmax_junction_sqr, limit_sqr);
          }

        #endif // JD_HANDLE_SMALL_SEGMENTS
      }

      // Get the lowest speed
      vmax_junction_sqr = _MIN(vmax_junction_sqr, block->nominal_speed_sqr, previous_nominal_speed_sqr);
    }
    else // Init entry speed to zero. Assume it starts from rest. Planner will correct this later.
      vmax_junction_sqr = 0;

    prev_unit_vec = unit_vec;

  #endif

  #ifdef USE_CACHED_SQRT
    #define CACHED_SQRT(N, V) \
      static float saved_V, N; \
      if (V != saved_V) { N = SQRT(V); saved_V = V; }
  #else
    #define CACHED_SQRT(N, V) const float N = SQRT(V)
  #endif

  #if HAS_CLASSIC_JERK

    /**
     * Adapted from Průša MKS firmware
     * https://github.com/prusa3d/Prusa-Firmware
     */
    CACHED_SQRT(nominal_speed, block->nominal_speed_sqr);

    // Exit speed limited by a jerk to full halt of a previous last segment
    static float previous_safe_speed;

    // Start with a safe speed (from which the machine may halt to stop immediately).
    float safe_speed = nominal_speed;

    #ifndef TRAVEL_EXTRA_XYJERK
      #define TRAVEL_EXTRA_XYJERK 0
    #endif
    const float extra_xyjerk = (de <= 0) ? TRAVEL_EXTRA_XYJERK : 0;

    uint8_t limited = 0;
    TERN(HAS_LINEAR_E_JERK, LOOP_XYZ, LOOP_XYZE)(i) {
      const float jerk = ABS(current_speed[i]),   // cs : Starting from zero, change in speed for this axis
                  maxj = (max_jerk[i] + (i == X_AXIS || i == Y_AXIS ? extra_xyjerk : 0.0f)); // mj : The max jerk setting for this axis
      if (jerk > maxj) {                          // cs > mj : New current speed too fast?
        if (limited) {                            // limited already?
          const float mjerk = nominal_speed * maxj; // ns*mj
          if (jerk * safe_speed > mjerk) safe_speed = mjerk / jerk; // ns*mj/cs
        }
        else {
          safe_speed *= maxj / jerk;              // Initial limit: ns*mj/cs
          ++limited;                              // Initially limited
        }
      }
    }

    float vmax_junction;
    if (moves_queued && !UNEAR_ZERO(previous_nominal_speed_sqr)) {
      // Estimate a maximum velocity allowed at a joint of two successive segments.
      // If this maximum velocity allowed is lower than the minimum of the entry / exit safe velocities,
      // then the machine is not coasting anymore and the safe entry / exit velocities shall be used.

      // Factor to multiply the previous / current nominal velocities to get componentwise limited velocities.
      float v_factor = 1;
      limited = 0;

      // The junction velocity will be shared between successive segments. Limit the junction velocity to their minimum.
      // Pick the smaller of the nominal speeds. Higher speed shall not be achieved at the junction during coasting.
      CACHED_SQRT(previous_nominal_speed, previous_nominal_speed_sqr);

      float smaller_speed_factor = 1.0f;
      if (nominal_speed < previous_nominal_speed) {
        vmax_junction = nominal_speed;
        smaller_speed_factor = vmax_junction / previous_nominal_speed;
      }
      else
        vmax_junction = previous_nominal_speed;

      // Now limit the jerk in all axes.
      TERN(HAS_LINEAR_E_JERK, LOOP_XYZ, LOOP_XYZE)(axis) {
        // Limit an axis. We have to differentiate: coasting, reversal of an axis, full stop.
        float v_exit = previous_speed[axis] * smaller_speed_factor,
              v_entry = current_speed[axis];
        if (limited) {
          v_exit *= v_factor;
          v_entry *= v_factor;
        }

        // Calculate jerk depending on whether the axis is coasting in the same direction or reversing.
        const float jerk = (v_exit > v_entry)
            ? //                                  coasting             axis reversal
              ( (v_entry > 0 || v_exit < 0) ? (v_exit - v_entry) : _MAX(v_exit, -v_entry) )
            : // v_exit <= v_entry                coasting             axis reversal
              ( (v_entry < 0 || v_exit > 0) ? (v_entry - v_exit) : _MAX(-v_exit, v_entry) );

        const float maxj = (max_jerk[axis] + (axis == X_AXIS || axis == Y_AXIS ? extra_xyjerk : 0.0f));

        if (jerk > maxj) {
          v_factor *= maxj / jerk;
          ++limited;
        }
      }
      if (limited) vmax_junction *= v_factor;
      // Now the transition velocity is known, which maximizes the shared exit / entry velocity while
      // respecting the jerk factors, it may be possible, that applying separate safe exit / entry velocities will achieve faster prints.
      const float vmax_junction_threshold = vmax_junction * 0.99f;
      if (previous_safe_speed > vmax_junction_threshold && safe_speed > vmax_junction_threshold)
        vmax_junction = safe_speed;
    }
    else
      vmax_junction = safe_speed;

    previous_safe_speed = safe_speed;

    #if HAS_JUNCTION_DEVIATION
      NOMORE(vmax_junction_sqr, sq(vmax_junction));   // Throttle down to max speed
    #else
      vmax_junction_sqr = sq(vmax_junction);          // Go up or down to the new speed
    #endif

  #endif // Classic Jerk Limiting

  // Max entry speed of this block equals the max exit speed of the previous block.
  block->max_entry_speed_sqr = vmax_junction_sqr;

  // Initialize block entry speed. Compute based on deceleration to user-defined MINIMUM_PLANNER_SPEED.
  const float v_allowable_sqr = max_allowable_speed_sqr(-block->acceleration, sq(float(MINIMUM_PLANNER_SPEED)), block->millimeters);

  // If we are trying to add a split block, start with the
  // max. allowed speed to avoid an interrupted first move.
  block->entry_speed_sqr = !split_move ? sq(float(MINIMUM_PLANNER_SPEED)) : _MIN(vmax_junction_sqr, v_allowable_sqr);

  // Initialize planner efficiency flags
  // Set flag if block will always reach maximum junction speed regardless of entry/exit speeds.
  // If a block can de/ac-celerate from nominal speed to zero within the length of the block, then
  // the current block and next block junction speeds are guaranteed to always be at their maximum
  // junction speeds in deceleration and acceleration, respectively. This is due to how the current
  // block nominal speed limits both the current and next maximum junction speeds. Hence, in both
  // the reverse and forward planners, the corresponding block junction speed will always be at the
  // the maximum junction speed and may always be ignored for any speed reduction checks.
  block->flag |= block->nominal_speed_sqr <= v_allowable_sqr ? BLOCK_FLAG_RECALCULATE | BLOCK_FLAG_NOMINAL_LENGTH : BLOCK_FLAG_RECALCULATE;

  // Update previous path unit_vector and nominal speed
  previous_speed = current_speed;
  previous_nominal_speed_sqr = block->nominal_speed_sqr;

  position = target;  // Update the position

  TERN_(HAS_POSITION_FLOAT, position_float = target_float);
  TERN_(GRADIENT_MIX, mixer.gradient_control(target_float.z));
  TERN_(POWER_LOSS_RECOVERY, block->sdpos = recovery.command_sdpos());

  return true;        // Movement was accepted

} // _populate_block()

/**
 * Planner::buffer_sync_block
 * Add a block to the buffer that just updates the position
 */
void Planner::buffer_sync_block() {
  // Wait for the next available block
  uint8_t next_buffer_head;
  block_t * const block = get_next_free_block(next_buffer_head);

  // Clear block
  memset(block, 0, sizeof(block_t));

  block->flag = BLOCK_FLAG_SYNC_POSITION;

  block->position = position;

  // If this is the first added movement, reload the delay, otherwise, cancel it.
  if (block_buffer_head == block_buffer_tail) {
    // If it was the first queued block, restart the 1st block delivery delay, to
    // give the planner an opportunity to queue more movements and plan them
    // As there are no queued movements, the Stepper ISR will not touch this
    // variable, so there is no risk setting this here (but it MUST be done
    // before the following line!!)
    delay_before_delivering = BLOCK_DELAY_FOR_1ST_MOVE;
  }

  block_buffer_head = next_buffer_head;

  stepper.wake_up();
} // buffer_sync_block()

/**
 * Planner::buffer_segment
 *
 * Add a new linear movement to the buffer in axis units.
 *
 * Leveling and kinematics should be applied ahead of calling this.
 *
 *  a,b,c,e     - target positions in mm and/or degrees
 *  fr_mm_s     - (target) speed of the move
 *  extruder    - target extruder
 *  millimeters - the length of the movement, if known
 */
bool Planner::buffer_segment(const float &a, const float &b, const float &c, const float &e
  #if HAS_DIST_MM_ARG
    , const xyze_float_t &cart_dist_mm
  #endif
  , const feedRate_t &fr_mm_s, const uint8_t extruder, const float &millimeters/*=0.0*/
) {

  // If we are cleaning, do not accept queuing of movements
  if (cleaning_buffer_counter) return false;

  // When changing extruders recalculate steps corresponding to the E position
  #if ENABLED(DISTINCT_E_FACTORS)
    if (last_extruder != extruder && settings.axis_steps_per_mm[E_AXIS_N(extruder)] != settings.axis_steps_per_mm[E_AXIS_N(last_extruder)]) {
      position.e = LROUND(position.e * settings.axis_steps_per_mm[E_AXIS_N(extruder)] * steps_to_mm[E_AXIS_N(last_extruder)]);
      last_extruder = extruder;
    }
  #endif

  // The target position of the tool in absolute steps
  // Calculate target position in absolute steps
  const abce_long_t target = {
    int32_t(LROUND(a * settings.axis_steps_per_mm[A_AXIS])),
    int32_t(LROUND(b * settings.axis_steps_per_mm[B_AXIS])),
    int32_t(LROUND(c * settings.axis_steps_per_mm[C_AXIS])),
    int32_t(LROUND(e * settings.axis_steps_per_mm[E_AXIS_N(extruder)]))
  };

  #if HAS_POSITION_FLOAT
    const xyze_pos_t target_float = { a, b, c, e };
  #endif

  // DRYRUN prevents E moves from taking place
  if (DEBUGGING(DRYRUN) || TERN0(CANCEL_OBJECTS, cancelable.skipping)) {
    position.e = target.e;
    TERN_(HAS_POSITION_FLOAT, position_float.e = e);
  }

  /* <-- add a slash to enable
    SERIAL_ECHOPAIR("  buffer_segment FR:", fr_mm_s);
    #if IS_KINEMATIC
      SERIAL_ECHOPAIR(" A:", a);
      SERIAL_ECHOPAIR(" (", position.a);
      SERIAL_ECHOPAIR("->", target.a);
      SERIAL_ECHOPAIR(") B:", b);
    #else
      SERIAL_ECHOPAIR_P(SP_X_LBL, a);
      SERIAL_ECHOPAIR(" (", position.x);
      SERIAL_ECHOPAIR("->", target.x);
      SERIAL_CHAR(')');
      SERIAL_ECHOPAIR_P(SP_Y_LBL, b);
    #endif
    SERIAL_ECHOPAIR(" (", position.y);
    SERIAL_ECHOPAIR("->", target.y);
    #if ENABLED(DELTA)
      SERIAL_ECHOPAIR(") C:", c);
    #else
      SERIAL_CHAR(')');
      SERIAL_ECHOPAIR_P(SP_Z_LBL, c);
    #endif
    SERIAL_ECHOPAIR(" (", position.z);
    SERIAL_ECHOPAIR("->", target.z);
    SERIAL_CHAR(')');
    SERIAL_ECHOPAIR_P(SP_E_LBL, e);
    SERIAL_ECHOPAIR(" (", position.e);
    SERIAL_ECHOPAIR("->", target.e);
    SERIAL_ECHOLNPGM(")");
  //*/

  // Queue the movement
  if (!_buffer_steps(target
      #if HAS_POSITION_FLOAT
        , target_float
      #endif
      #if HAS_DIST_MM_ARG
        , cart_dist_mm
      #endif
      , fr_mm_s, extruder, millimeters)
  ) return false;

  stepper.wake_up();
  return true;
} // buffer_segment()

/**
 * Add a new linear movement to the buffer.
 * The target is cartesian. It's translated to
 * delta/scara if needed.
 *
 *  rx,ry,rz,e   - target position in mm or degrees
 *  fr_mm_s      - (target) speed of the move (mm/s)
 *  extruder     - target extruder
 *  millimeters  - the length of the movement, if known
 *  inv_duration - the reciprocal if the duration of the movement, if known (kinematic only if feeedrate scaling is enabled)
 */
bool Planner::buffer_line(const float &rx, const float &ry, const float &rz, const float &e, const feedRate_t &fr_mm_s, const uint8_t extruder, const float millimeters
  #if ENABLED(SCARA_FEEDRATE_SCALING)
    , const float &inv_duration
  #endif
) {
  xyze_pos_t machine = { rx, ry, rz, e };
  TERN_(HAS_POSITION_MODIFIERS, apply_modifiers(machine));

  #if IS_KINEMATIC

    #if HAS_JUNCTION_DEVIATION
      const xyze_pos_t cart_dist_mm = {
        rx - position_cart.x, ry - position_cart.y,
        rz - position_cart.z, e  - position_cart.e
      };
    #else
      const xyz_pos_t cart_dist_mm = { rx - position_cart.x, ry - position_cart.y, rz - position_cart.z };
    #endif

    float mm = millimeters;
    if (mm == 0.0)
      mm = (cart_dist_mm.x != 0.0 || cart_dist_mm.y != 0.0) ? cart_dist_mm.magnitude() : ABS(cart_dist_mm.z);

    // Cartesian XYZ to kinematic ABC, stored in global 'delta'
    inverse_kinematics(machine);

    #if ENABLED(SCARA_FEEDRATE_SCALING)
      // For SCARA scale the feed rate from mm/s to degrees/s
      // i.e., Complete the angular vector in the given time.
      const float duration_recip = inv_duration ?: fr_mm_s / mm;
      const xyz_pos_t diff = delta - position_float;
      const feedRate_t feedrate = diff.magnitude() * duration_recip;
    #else
      const feedRate_t feedrate = fr_mm_s;
    #endif
    if (buffer_segment(delta.a, delta.b, delta.c, machine.e
      #if HAS_JUNCTION_DEVIATION
        , cart_dist_mm
      #endif
      , feedrate, extruder, mm
    )) {
      position_cart.set(rx, ry, rz, e);
      return true;
    }
    else
      return false;
  #else
    return buffer_segment(machine, fr_mm_s, extruder, millimeters);
  #endif
} // buffer_line()

#if ENABLED(DIRECT_STEPPING)

  void Planner::buffer_page(const page_idx_t page_idx, const uint8_t extruder, const uint16_t num_steps) {
    if (!last_page_step_rate) {
      kill(GET_TEXT(MSG_BAD_PAGE_SPEED));
      return;
    }

    uint8_t next_buffer_head;
    block_t * const block = get_next_free_block(next_buffer_head);

    block->flag = BLOCK_FLAG_IS_PAGE;

    #if FAN_COUNT > 0
      FANS_LOOP(i) block->fan_speed[i] = thermalManager.fan_speed[i];
    #endif

    #if EXTRUDERS > 1
      block->extruder = extruder;
    #endif

    block->page_idx = page_idx;

    block->step_event_count = num_steps;
    block->initial_rate =
      block->final_rate =
      block->nominal_rate = last_page_step_rate; // steps/s

    block->accelerate_until = 0;
    block->decelerate_after = block->step_event_count;

    // Will be set to last direction later if directional format.
    block->direction_bits = 0;

    #define PAGE_UPDATE_DIR(AXIS) \
      if (!last_page_dir[_AXIS(AXIS)]) SBI(block->direction_bits, _AXIS(AXIS));

    if (!DirectStepping::Config::DIRECTIONAL) {
      PAGE_UPDATE_DIR(X);
      PAGE_UPDATE_DIR(Y);
      PAGE_UPDATE_DIR(Z);
      PAGE_UPDATE_DIR(E);
    }

    // If this is the first added movement, reload the delay, otherwise, cancel it.
    if (block_buffer_head == block_buffer_tail) {
      // If it was the first queued block, restart the 1st block delivery delay, to
      // give the planner an opportunity to queue more movements and plan them
      // As there are no queued movements, the Stepper ISR will not touch this
      // variable, so there is no risk setting this here (but it MUST be done
      // before the following line!!)
      delay_before_delivering = BLOCK_DELAY_FOR_1ST_MOVE;
    }

    // Move buffer head
    block_buffer_head = next_buffer_head;

    enable_all_steppers();
    stepper.wake_up();
  }

#endif // DIRECT_STEPPING

/**
 * Directly set the planner ABC position (and stepper positions)
 * converting mm (or angles for SCARA) into steps.
 *
 * The provided ABC position is in machine units.
 */

void Planner::set_machine_position_mm(const float &a, const float &b, const float &c, const float &e) {
  TERN_(DISTINCT_E_FACTORS, last_extruder = active_extruder);
  TERN_(HAS_POSITION_FLOAT, position_float.set(a, b, c, e));
  position.set(LROUND(a * settings.axis_steps_per_mm[A_AXIS]),
               LROUND(b * settings.axis_steps_per_mm[B_AXIS]),
               LROUND(c * settings.axis_steps_per_mm[C_AXIS]),
               LROUND(e * settings.axis_steps_per_mm[E_AXIS_N(active_extruder)]));
  if (has_blocks_queued()) {
    //previous_nominal_speed_sqr = 0.0; // Reset planner junction speeds. Assume start from rest.
    //previous_speed.reset();
    buffer_sync_block();
  }
  else
    stepper.set_position(position);
}

void Planner::set_position_mm(const float &rx, const float &ry, const float &rz, const float &e) {
  xyze_pos_t machine = { rx, ry, rz, e };
  #if HAS_POSITION_MODIFIERS
    apply_modifiers(machine, true);
  #endif
  #if IS_KINEMATIC
    position_cart.set(rx, ry, rz, e);
    inverse_kinematics(machine);
    set_machine_position_mm(delta.a, delta.b, delta.c, machine.e);
  #else
    set_machine_position_mm(machine);
  #endif
}

/**
 * Setters for planner position (also setting stepper position).
 */
void Planner::set_e_position_mm(const float &e) {
  const uint8_t axis_index = E_AXIS_N(active_extruder);
  TERN_(DISTINCT_E_FACTORS, last_extruder = active_extruder);

  const float e_new = e - TERN0(FWRETRACT, fwretract.current_retract[active_extruder]);
  position.e = LROUND(settings.axis_steps_per_mm[axis_index] * e_new);
  TERN_(HAS_POSITION_FLOAT, position_float.e = e_new);
  TERN_(IS_KINEMATIC, position_cart.e = e);

  if (has_blocks_queued())
    buffer_sync_block();
  else
    stepper.set_axis_position(E_AXIS, position.e);
}

// Recalculate the steps/s^2 acceleration rates, based on the mm/s^2
void Planner::reset_acceleration_rates() {
  #if ENABLED(DISTINCT_E_FACTORS)
    #define AXIS_CONDITION (i < E_AXIS || i == E_AXIS_N(active_extruder))
  #else
    #define AXIS_CONDITION true
  #endif
  uint32_t highest_rate = 1;
  LOOP_XYZE_N(i) {
    max_acceleration_steps_per_s2[i] = settings.max_acceleration_mm_per_s2[i] * settings.axis_steps_per_mm[i];
    if (AXIS_CONDITION) NOLESS(highest_rate, max_acceleration_steps_per_s2[i]);
  }
  cutoff_long = 4294967295UL / highest_rate; // 0xFFFFFFFFUL
  TERN_(HAS_LINEAR_E_JERK, recalculate_max_e_jerk());
}

// Recalculate position, steps_to_mm if settings.axis_steps_per_mm changes!
void Planner::refresh_positioning() {
  LOOP_XYZE_N(i) steps_to_mm[i] = 1.0f / settings.axis_steps_per_mm[i];
  set_position_mm(current_position);
  reset_acceleration_rates();
}

inline void limit_and_warn(float &val, const uint8_t axis, PGM_P const setting_name, const xyze_float_t &max_limit) {
  const uint8_t lim_axis = axis > E_AXIS ? E_AXIS : axis;
  const float before = val;
  LIMIT(val, 0.1, max_limit[lim_axis]);
  if (before != val) {
    SERIAL_CHAR(axis_codes[lim_axis]);
    SERIAL_ECHOPGM(" Max ");
    serialprintPGM(setting_name);
    SERIAL_ECHOLNPAIR(" limited to ", val);
  }
}

void Planner::set_max_acceleration(const uint8_t axis, float targetValue) {
  #if ENABLED(LIMITED_MAX_ACCEL_EDITING)
    #ifdef MAX_ACCEL_EDIT_VALUES
      constexpr xyze_float_t max_accel_edit = MAX_ACCEL_EDIT_VALUES;
      const xyze_float_t &max_acc_edit_scaled = max_accel_edit;
    #else
      constexpr xyze_float_t max_accel_edit = DEFAULT_MAX_ACCELERATION;
      const xyze_float_t max_acc_edit_scaled = max_accel_edit * 2;
    #endif
    limit_and_warn(targetValue, axis, PSTR("Acceleration"), max_acc_edit_scaled);
  #endif
  settings.max_acceleration_mm_per_s2[axis] = targetValue;

  // Update steps per s2 to agree with the units per s2 (since they are used in the planner)
  reset_acceleration_rates();
}

void Planner::set_max_feedrate(const uint8_t axis, float targetValue) {
  #if ENABLED(LIMITED_MAX_FR_EDITING)
    #ifdef MAX_FEEDRATE_EDIT_VALUES
      constexpr xyze_float_t max_fr_edit = MAX_FEEDRATE_EDIT_VALUES;
      const xyze_float_t &max_fr_edit_scaled = max_fr_edit;
    #else
      constexpr xyze_float_t max_fr_edit = DEFAULT_MAX_FEEDRATE;
      const xyze_float_t max_fr_edit_scaled = max_fr_edit * 2;
    #endif
    limit_and_warn(targetValue, axis, PSTR("Feedrate"), max_fr_edit_scaled);
  #endif
  settings.max_feedrate_mm_s[axis] = targetValue;
}

void Planner::set_max_jerk(const AxisEnum axis, float targetValue) {
  #if HAS_CLASSIC_JERK
    #if ENABLED(LIMITED_JERK_EDITING)
      constexpr xyze_float_t max_jerk_edit =
        #ifdef MAX_JERK_EDIT_VALUES
          MAX_JERK_EDIT_VALUES
        #else
          { (DEFAULT_XJERK) * 2, (DEFAULT_YJERK) * 2,
            (DEFAULT_ZJERK) * 2, (DEFAULT_EJERK) * 2 }
        #endif
      ;
      limit_and_warn(targetValue, axis, PSTR("Jerk"), max_jerk_edit);
    #endif
    max_jerk[axis] = targetValue;
  #else
    UNUSED(axis); UNUSED(targetValue);
  #endif
}

#if HAS_SPI_LCD

  uint16_t Planner::block_buffer_runtime() {
    #ifdef __AVR__
      // Protect the access to the variable. Only required for AVR, as
      //  any 32bit CPU offers atomic access to 32bit variables
      const bool was_enabled = stepper.suspend();
    #endif

    uint32_t bbru = block_buffer_runtime_us;

    #ifdef __AVR__
      // Reenable Stepper ISR
      if (was_enabled) stepper.wake_up();
    #endif

    // To translate µs to ms a division by 1000 would be required.
    // We introduce 2.4% error here by dividing by 1024.
    // Doesn't matter because block_buffer_runtime_us is already too small an estimation.
    bbru >>= 10;
    // limit to about a minute.
    NOMORE(bbru, 0x0000FFFFUL);
    return bbru;
  }

  void Planner::clear_block_buffer_runtime() {
    #ifdef __AVR__
      // Protect the access to the variable. Only required for AVR, as
      //  any 32bit CPU offers atomic access to 32bit variables
      const bool was_enabled = stepper.suspend();
    #endif

    block_buffer_runtime_us = 0;

    #ifdef __AVR__
      // Reenable Stepper ISR
      if (was_enabled) stepper.wake_up();
    #endif
  }

#endif

#if ENABLED(AUTOTEMP)

void Planner::autotemp_update() {
  #if ENABLED(AUTOTEMP_PROPORTIONAL)
    const int16_t target = thermalManager.degTargetHotend(active_extruder);
    autotemp_min = target + AUTOTEMP_MIN_P;
    autotemp_max = target + AUTOTEMP_MAX_P;
  #endif
  autotemp_factor = TERN(AUTOTEMP_PROPORTIONAL, AUTOTEMP_FACTOR_P, 0);
  autotemp_enabled = autotemp_factor != 0;
}

  void Planner::autotemp_M104_M109() {

    #if ENABLED(AUTOTEMP_PROPORTIONAL)
      const int16_t target = thermalManager.degTargetHotend(active_extruder);
      autotemp_min = target + AUTOTEMP_MIN_P;
      autotemp_max = target + AUTOTEMP_MAX_P;
    #endif

    if (parser.seenval('S')) autotemp_min = parser.value_celsius();
    if (parser.seenval('B')) autotemp_max = parser.value_celsius();

    // When AUTOTEMP_PROPORTIONAL is enabled, F0 disables autotemp.
    // Normally, leaving off F also disables autotemp.
    autotemp_factor = parser.seen('F') ? parser.value_float() : TERN(AUTOTEMP_PROPORTIONAL, AUTOTEMP_FACTOR_P, 0);
    autotemp_enabled = autotemp_factor != 0;
  }
#endif
