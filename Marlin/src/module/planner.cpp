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
#include "../module/temperature.h"
#include "../lcd/ultralcd.h"
#include "../core/language.h"
#include "../gcode/parser.h"

#include "../Marlin.h"

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

Planner planner;

  // public:

/**
 * A ring buffer of moves described in steps
 */
block_t Planner::block_buffer[BLOCK_BUFFER_SIZE];
volatile uint8_t Planner::block_buffer_head, // Index of the next block to be pushed
                 Planner::block_buffer_tail;

float Planner::max_feedrate_mm_s[XYZE_N], // Max speeds in mm per second
      Planner::axis_steps_per_mm[XYZE_N],
      Planner::steps_to_mm[XYZE_N];

#if ENABLED(DISTINCT_E_FACTORS)
  uint8_t Planner::last_extruder = 0;     // Respond to extruder change
#endif

int16_t Planner::flow_percentage[EXTRUDERS] = ARRAY_BY_EXTRUDERS1(100); // Extrusion factor for each extruder

float Planner::e_factor[EXTRUDERS] = ARRAY_BY_EXTRUDERS1(1.0); // The flow percentage and volumetric multiplier combine to scale E movement

#if DISABLED(NO_VOLUMETRICS)
  float Planner::filament_size[EXTRUDERS],          // diameter of filament (in millimeters), typically around 1.75 or 2.85, 0 disables the volumetric calculations for the extruder
        Planner::volumetric_area_nominal = CIRCLE_AREA((DEFAULT_NOMINAL_FILAMENT_DIA) * 0.5), // Nominal cross-sectional area
        Planner::volumetric_multiplier[EXTRUDERS];  // Reciprocal of cross-sectional area of filament (in mm^2). Pre-calculated to reduce computation in the planner
#endif

uint32_t Planner::max_acceleration_steps_per_s2[XYZE_N],
         Planner::max_acceleration_mm_per_s2[XYZE_N]; // Use M201 to override by software

uint32_t Planner::min_segment_time_us;

// Initialized by settings.load()
float Planner::min_feedrate_mm_s,
      Planner::acceleration,         // Normal acceleration mm/s^2  DEFAULT ACCELERATION for all printing moves. M204 SXXXX
      Planner::retract_acceleration, // Retract acceleration mm/s^2 filament pull-back and push-forward while standing still in the other axes M204 TXXXX
      Planner::travel_acceleration,  // Travel acceleration mm/s^2  DEFAULT ACCELERATION for all NON printing moves. M204 MXXXX
      Planner::max_jerk[XYZE],       // The largest speed change requiring no acceleration
      Planner::min_travel_feedrate_mm_s;

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

#if ENABLED(SKEW_CORRECTION)
  #if ENABLED(SKEW_CORRECTION_GCODE)
    float Planner::xy_skew_factor;
  #else
    constexpr float Planner::xy_skew_factor;
  #endif
  #if ENABLED(SKEW_CORRECTION_FOR_Z) && ENABLED(SKEW_CORRECTION_GCODE)
    float Planner::xz_skew_factor, Planner::yz_skew_factor;
  #else
    constexpr float Planner::xz_skew_factor, Planner::yz_skew_factor;
  #endif
#endif

#if ENABLED(AUTOTEMP)
  float Planner::autotemp_max = 250,
        Planner::autotemp_min = 210,
        Planner::autotemp_factor = 0.1;
  bool Planner::autotemp_enabled = false;
#endif

// private:

int32_t Planner::position[NUM_AXIS] = { 0 };

uint32_t Planner::cutoff_long;

float Planner::previous_speed[NUM_AXIS],
      Planner::previous_nominal_speed;

#if ENABLED(DISABLE_INACTIVE_EXTRUDER)
  uint8_t Planner::g_uc_extruder_last_move[EXTRUDERS] = { 0 };
#endif

#ifdef XY_FREQUENCY_LIMIT
  // Old direction bits. Used for speed calculations
  unsigned char Planner::old_direction_bits = 0;
  // Segment times (in µs). Used for speed calculations
  uint32_t Planner::axis_segment_time_us[2][3] = { { MAX_FREQ_TIME_US + 1, 0, 0 }, { MAX_FREQ_TIME_US + 1, 0, 0 } };
#endif

#if ENABLED(LIN_ADVANCE)
  float Planner::extruder_advance_K; // Initialized by settings.load()
#endif

#if HAS_POSITION_FLOAT
  float Planner::position_float[XYZE]; // Needed for accurate maths. Steps cannot be used!
#endif

#if ENABLED(ULTRA_LCD)
  volatile uint32_t Planner::block_buffer_runtime_us = 0;
#endif

/**
 * Class and Instance Methods
 */

Planner::Planner() { init(); }

void Planner::init() {
  ZERO(position);
  #if HAS_POSITION_FLOAT
    ZERO(position_float);
  #endif
  ZERO(previous_speed);
  previous_nominal_speed = 0.0;
  #if ABL_PLANAR
    bed_level_matrix.set_to_identity();
  #endif
  clear_block_buffer();
}

#if ENABLED(BEZIER_JERK_CONTROL)

  #ifdef __AVR__
    // This routine, for AVR, returns 0x1000000 / d, but trying to get the inverse as
    //  fast as possible. A fast converging iterative Newton-Raphson method is able to
    //  reach full precision in just 1 iteration, and takes 211 cycles (worst case, mean
    //  case is less, up to 30 cycles for small divisors), instead of the 500 cycles a
    //  normal division would take.
    //
    // Inspired by the following page,
    //  https://stackoverflow.com/questions/27801397/newton-raphson-division-with-big-integers
    //
    // Suppose we want to calculate
    //  floor(2 ^ k / B)    where B is a positive integer
    // Then
    //  B must be <= 2^k, otherwise, the quotient is 0.
    //
    // The Newton - Raphson iteration for x = B / 2 ^ k yields:
    //  q[n + 1] = q[n] * (2 - q[n] * B / 2 ^ k)
    //
    // We can rearrange it as:
    //  q[n + 1] = q[n] * (2 ^ (k + 1) - q[n] * B) >> k
    //
    //  Each iteration of this kind requires only integer multiplications
    // and bit shifts.
    //  Does it converge to floor(2 ^ k / B) ?:  Not necessarily, but, in
    // the worst case, it eventually alternates between floor(2 ^ k / B)
    // and ceiling(2 ^ k / B)).
    //  So we can use some not-so-clever test to see if we are in this
    // case, and extract floor(2 ^ k / B).
    //  Lastly, a simple but important optimization for this approach is to
    // truncate multiplications (i.e.calculate only the higher bits of the
    // product) in the early iterations of the Newton - Raphson method.The
    // reason to do so, is that the results of the early iterations are far
    // from the quotient, and it doesn't matter to perform them inaccurately.
    //  Finally, we should pick a good starting value for x. Knowing how many
    // digits the divisor has, we can estimate it:
    //
    // 2^k / x = 2 ^ log2(2^k / x)
    // 2^k / x = 2 ^(log2(2^k)-log2(x))
    // 2^k / x = 2 ^(k*log2(2)-log2(x))
    // 2^k / x = 2 ^ (k-log2(x))
    // 2^k / x >= 2 ^ (k-floor(log2(x)))
    // floor(log2(x)) simply is the index of the most significant bit set.
    //
    //  If we could improve this estimation even further, then the number of
    // iterations can be dropped quite a bit, thus saving valuable execution time.
    //  The paper "Software Integer Division" by Thomas L.Rodeheffer, Microsoft
    // Research, Silicon Valley,August 26, 2008, that is available at
    // https://www.microsoft.com/en-us/research/wp-content/uploads/2008/08/tr-2008-141.pdf
    // suggests , for its integer division algorithm, that using a table to supply the
    // first 8 bits of precision, and due to the quadratic convergence nature of the
    // Newton-Raphon iteration, then just 2 iterations should be enough to get
    // maximum precision of the division.
    //  If we precompute values of inverses for small denominator values, then
    // just one Newton-Raphson iteration is enough to reach full precision
    //  We will use the top 9 bits of the denominator as index.
    //
    //  The AVR assembly function is implementing the following C code, included
    // here as reference:
    //
    // uint32_t get_period_inverse(uint32_t d) {
    //  static const uint8_t inv_tab[256] = {
    //    255,253,252,250,248,246,244,242,240,238,236,234,233,231,229,227,
    //    225,224,222,220,218,217,215,213,212,210,208,207,205,203,202,200,
    //    199,197,195,194,192,191,189,188,186,185,183,182,180,179,178,176,
    //    175,173,172,170,169,168,166,165,164,162,161,160,158,157,156,154,
    //    153,152,151,149,148,147,146,144,143,142,141,139,138,137,136,135,
    //    134,132,131,130,129,128,127,126,125,123,122,121,120,119,118,117,
    //    116,115,114,113,112,111,110,109,108,107,106,105,104,103,102,101,
    //    100,99,98,97,96,95,94,93,92,91,90,89,88,88,87,86,
    //    85,84,83,82,81,80,80,79,78,77,76,75,74,74,73,72,
    //    71,70,70,69,68,67,66,66,65,64,63,62,62,61,60,59,
    //    59,58,57,56,56,55,54,53,53,52,51,50,50,49,48,48,
    //    47,46,46,45,44,43,43,42,41,41,40,39,39,38,37,37,
    //    36,35,35,34,33,33,32,32,31,30,30,29,28,28,27,27,
    //    26,25,25,24,24,23,22,22,21,21,20,19,19,18,18,17,
    //    17,16,15,15,14,14,13,13,12,12,11,10,10,9,9,8,
    //    8,7,7,6,6,5,5,4,4,3,3,2,2,1,0,0
    //  };
    //
    //  // For small denominators, it is cheaper to directly store the result,
    //  //  because those denominators would require 2 Newton-Raphson iterations
    //  //  to converge to the required result precision. For bigger ones, just
    //  //  ONE Newton-Raphson iteration is enough to get maximum precision!
    //  static const uint32_t small_inv_tab[111] PROGMEM = {
    //    16777216,16777216,8388608,5592405,4194304,3355443,2796202,2396745,2097152,1864135,1677721,1525201,1398101,1290555,1198372,1118481,
    //    1048576,986895,932067,883011,838860,798915,762600,729444,699050,671088,645277,621378,599186,578524,559240,541200,
    //    524288,508400,493447,479349,466033,453438,441505,430185,419430,409200,399457,390167,381300,372827,364722,356962,
    //    349525,342392,335544,328965,322638,316551,310689,305040,299593,294337,289262,284359,279620,275036,270600,266305,
    //    262144,258111,254200,250406,246723,243148,239674,236298,233016,229824,226719,223696,220752,217885,215092,212369,
    //    209715,207126,204600,202135,199728,197379,195083,192841,190650,188508,186413,184365,182361,180400,178481,176602,
    //    174762,172960,171196,169466,167772,166111,164482,162885,161319,159783,158275,156796,155344,153919,152520
    //  };
    //
    //  // For small divisors, it is best to directly retrieve the results
    //  if (d <= 110)
    //    return pgm_read_dword(&small_inv_tab[d]);
    //
    //  // Compute initial estimation of 0x1000000/x -
    //  // Get most significant bit set on divider
    //  uint8_t idx = 0;
    //  uint32_t nr = d;
    //  if (!(nr & 0xff0000)) {
    //    nr <<= 8;
    //    idx += 8;
    //    if (!(nr & 0xff0000)) {
    //      nr <<= 8;
    //      idx += 8;
    //    }
    //  }
    //  if (!(nr & 0xf00000)) {
    //    nr <<= 4;
    //    idx += 4;
    //  }
    //  if (!(nr & 0xc00000)) {
    //    nr <<= 2;
    //    idx += 2;
    //  }
    //  if (!(nr & 0x800000)) {
    //    nr <<= 1;
    //    idx += 1;
    //  }
    //
    //  // Isolate top 9 bits of the denominator, to be used as index into the initial estimation table
    //  uint32_t tidx = nr >> 15;         // top 9 bits. bit8 is always set
    //  uint32_t ie = inv_tab[tidx & 0xFF] + 256; // Get the table value. bit9 is always set
    //  uint32_t x = idx <= 8 ? (ie >> (8 - idx)) : (ie << (idx - 8)); // Position the estimation at the proper place
    //
    //  // Now, refine estimation by newton-raphson. 1 iteration is enough
    //  x = uint32_t((x * uint64_t((1 << 25) - x * d)) >> 24);
    //
    //  // Estimate remainder
    //  uint32_t r = (1 << 24) - x * d;
    //
    //  // Check if we must adjust result
    //  if (r >= d) x++;
    //
    //  // x holds the proper estimation
    //  return uint32_t(x);
    // }
    //
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
      if (d <= 110)
        return pgm_read_dword(&small_inv_tab[d]);

      register uint8_t r8 = d & 0xFF;
      register uint8_t r9 = (d >> 8) & 0xFF;
      register uint8_t r10 = (d >> 16) & 0xFF;
      register uint8_t r2,r3,r4,r5,r6,r7,r11,r12,r13,r14,r15,r16,r17,r18;
      register const uint8_t* ptab = inv_tab;

      __asm__ __volatile__(
        /*  %8:%7:%6 = interval*/
        /*  r31:r30: MUST be those registers, and they must point to the inv_tab */

        " clr %13" "\n\t"                 /* %13 = 0 */

        /*  Now we must compute */
        /*   result = 0xFFFFFF / d */
        /*  %8:%7:%6 = interval*/
        /*  %16:%15:%14 = nr */
        /*  %13 = 0*/

        /*  A plain division of 24x24 bits should take 388 cycles to complete. We will */
        /*  use Newton-Raphson for the calculation, and will strive to get way less cycles*/
        /*  for the same result - Using C division, it takes 500cycles to complete .*/

        " clr %3" "\n\t"                  /* idx = 0 */
        " mov %14,%6" "\n\t"
        " mov %15,%7" "\n\t"
        " mov %16,%8" "\n\t"              /* nr = interval */
        " tst %16" "\n\t"                 /* nr & 0xFF0000 == 0 ? */
        " brne 2f" "\n\t"                 /* No, skip this */
        " mov %16,%15" "\n\t"
        " mov %15,%14" "\n\t"             /* nr <<= 8, %14 not needed */
        " subi %3,-8" "\n\t"              /* idx += 8 */
        " tst %16" "\n\t"                 /* nr & 0xFF0000 == 0 ? */
        " brne 2f" "\n\t"                 /* No, skip this */
        " mov %16,%15" "\n\t"             /* nr <<= 8, %14 not needed */
        " clr %15" "\n\t"                 /* We clear %14 */
        " subi %3,-8" "\n\t"              /* idx += 8 */

        /*  here %16 != 0 and %16:%15 contains at least 9 MSBits, or both %16:%15 are 0 */
        "2:" "\n\t"
        " cpi %16,0x10" "\n\t"            /* (nr & 0xf00000) == 0 ? */
        " brcc 3f" "\n\t"                 /* No, skip this */
        " swap %15" "\n\t"                /* Swap nibbles */
        " swap %16" "\n\t"                /* Swap nibbles. Low nibble is 0 */
        " mov %14, %15" "\n\t"
        " andi %14,0x0f" "\n\t"           /* Isolate low nibble */
        " andi %15,0xf0" "\n\t"           /* Keep proper nibble in %15 */
        " or %16, %14" "\n\t"             /* %16:%15 <<= 4 */
        " subi %3,-4" "\n\t"              /* idx += 4 */

        "3:" "\n\t"
        " cpi %16,0x40" "\n\t"            /* (nr & 0xc00000) == 0 ? */
        " brcc 4f" "\n\t"                 /* No, skip this*/
        " add %15,%15" "\n\t"
        " adc %16,%16" "\n\t"
        " add %15,%15" "\n\t"
        " adc %16,%16" "\n\t"             /* %16:%15 <<= 2 */
        " subi %3,-2" "\n\t"              /* idx += 2 */

        "4:" "\n\t"
        " cpi %16,0x80" "\n\t"            /* (nr & 0x800000) == 0 ? */
        " brcc 5f" "\n\t"                 /* No, skip this */
        " add %15,%15" "\n\t"
        " adc %16,%16" "\n\t"             /* %16:%15 <<= 1 */
        " inc %3" "\n\t"                  /* idx += 1 */

        /*  Now %16:%15 contains its MSBit set to 1, or %16:%15 is == 0. We are now absolutely sure*/
        /*  we have at least 9 MSBits available to enter the initial estimation table*/
        "5:" "\n\t"
        " add %15,%15" "\n\t"
        " adc %16,%16" "\n\t"             /* %16:%15 = tidx = (nr <<= 1), we lose the top MSBit (always set to 1, %16 is the index into the inverse table)*/
        " add r30,%16" "\n\t"             /* Only use top 8 bits */
        " adc r31,%13" "\n\t"             /* r31:r30 = inv_tab + (tidx) */
        " lpm %14, Z" "\n\t"              /* %14 = inv_tab[tidx] */
        " ldi %15, 1" "\n\t"              /* %15 = 1  %15:%14 = inv_tab[tidx] + 256 */

        /*  We must scale the approximation to the proper place*/
        " clr %16" "\n\t"                 /* %16 will always be 0 here */
        " subi %3,8" "\n\t"               /* idx == 8 ? */
        " breq 6f" "\n\t"                 /* yes, no need to scale*/
        " brcs 7f" "\n\t"                 /* If C=1, means idx < 8, result was negative!*/

        /*  idx > 8, now %3 = idx - 8. We must perform a left shift. idx range:[1-8]*/
        " sbrs %3,0" "\n\t"               /* shift by 1bit position?*/
        " rjmp 8f" "\n\t"                 /* No*/
        " add %14,%14" "\n\t"
        " adc %15,%15" "\n\t"             /* %15:16 <<= 1*/
        "8:" "\n\t"
        " sbrs %3,1" "\n\t"               /* shift by 2bit position?*/
        " rjmp 9f" "\n\t"                 /* No*/
        " add %14,%14" "\n\t"
        " adc %15,%15" "\n\t"
        " add %14,%14" "\n\t"
        " adc %15,%15" "\n\t"             /* %15:16 <<= 1*/
        "9:" "\n\t"
        " sbrs %3,2" "\n\t"               /* shift by 4bits position?*/
        " rjmp 16f" "\n\t"                /* No*/
        " swap %15" "\n\t"                /* Swap nibbles. lo nibble of %15 will always be 0*/
        " swap %14" "\n\t"                /* Swap nibbles*/
        " mov %12,%14" "\n\t"
        " andi %12,0x0f" "\n\t"           /* isolate low nibble*/
        " andi %14,0xf0" "\n\t"           /* and clear it*/
        " or %15,%12" "\n\t"              /* %15:%16 <<= 4*/
        "16:" "\n\t"
        " sbrs %3,3" "\n\t"               /* shift by 8bits position?*/
        " rjmp 6f" "\n\t"                 /* No, we are done */
        " mov %16,%15" "\n\t"
        " mov %15,%14" "\n\t"
        " clr %14" "\n\t"
        " jmp 6f" "\n\t"

        /*  idx < 8, now %3 = idx - 8. Get the count of bits */
        "7:" "\n\t"
        " neg %3" "\n\t"                  /* %3 = -idx = count of bits to move right. idx range:[1...8]*/
        " sbrs %3,0" "\n\t"               /* shift by 1 bit position ?*/
        " rjmp 10f" "\n\t"                /* No, skip it*/
        " asr %15" "\n\t"                 /* (bit7 is always 0 here)*/
        " ror %14" "\n\t"
        "10:" "\n\t"
        " sbrs %3,1" "\n\t"               /* shift by 2 bit position ?*/
        " rjmp 11f" "\n\t"                /* No, skip it*/
        " asr %15" "\n\t"                 /* (bit7 is always 0 here)*/
        " ror %14" "\n\t"
        " asr %15" "\n\t"                 /* (bit7 is always 0 here)*/
        " ror %14" "\n\t"
        "11:" "\n\t"
        " sbrs %3,2" "\n\t"               /* shift by 4 bit position ?*/
        " rjmp 12f" "\n\t"                /* No, skip it*/
        " swap %15" "\n\t"                /* Swap nibbles*/
        " andi %14, 0xf0" "\n\t"          /* Lose the lowest nibble*/
        " swap %14" "\n\t"                /* Swap nibbles. Upper nibble is 0*/
        " or %14,%15" "\n\t"              /* Pass nibble from upper byte*/
        " andi %15, 0x0f" "\n\t"          /* And get rid of that nibble*/
        "12:" "\n\t"
        " sbrs %3,3" "\n\t"               /* shift by 8 bit position ?*/
        " rjmp 6f" "\n\t"                 /* No, skip it*/
        " mov %14,%15" "\n\t"
        " clr %15" "\n\t"
        "6:" "\n\t"                       /* %16:%15:%14 = initial estimation of 0x1000000 / d*/

        /*  Now, we must refine the estimation present on %16:%15:%14 using 1 iteration*/
        /*   of Newton-Raphson. As it has a quadratic convergence, 1 iteration is enough*/
        /*   to get more than 18bits of precision (the initial table lookup gives 9 bits of*/
        /*   precision to start from). 18bits of precision is all what is needed here for result */

        /*  %8:%7:%6 = d = interval*/
        /*  %16:%15:%14 = x = initial estimation of 0x1000000 / d*/
        /*  %13 = 0*/
        /*  %3:%2:%1:%0 = working accumulator*/

        /*  Compute 1<<25 - x*d. Result should never exceed 25 bits and should always be positive*/
        " clr %0" "\n\t"
        " clr %1" "\n\t"
        " clr %2" "\n\t"
        " ldi %3,2" "\n\t"                /* %3:%2:%1:%0 = 0x2000000*/
        " mul %6,%14" "\n\t"              /* r1:r0 = LO(d) * LO(x)*/
        " sub %0,r0" "\n\t"
        " sbc %1,r1" "\n\t"
        " sbc %2,%13" "\n\t"
        " sbc %3,%13" "\n\t"              /* %3:%2:%1:%0 -= LO(d) * LO(x)*/
        " mul %7,%14" "\n\t"              /* r1:r0 = MI(d) * LO(x)*/
        " sub %1,r0" "\n\t"
        " sbc %2,r1"  "\n\t"
        " sbc %3,%13" "\n\t"              /* %3:%2:%1:%0 -= MI(d) * LO(x) << 8*/
        " mul %8,%14" "\n\t"              /* r1:r0 = HI(d) * LO(x)*/
        " sub %2,r0" "\n\t"
        " sbc %3,r1" "\n\t"               /* %3:%2:%1:%0 -= MIL(d) * LO(x) << 16*/
        " mul %6,%15" "\n\t"              /* r1:r0 = LO(d) * MI(x)*/
        " sub %1,r0" "\n\t"
        " sbc %2,r1" "\n\t"
        " sbc %3,%13" "\n\t"              /* %3:%2:%1:%0 -= LO(d) * MI(x) << 8*/
        " mul %7,%15" "\n\t"              /* r1:r0 = MI(d) * MI(x)*/
        " sub %2,r0" "\n\t"
        " sbc %3,r1" "\n\t"               /* %3:%2:%1:%0 -= MI(d) * MI(x) << 16*/
        " mul %8,%15" "\n\t"              /* r1:r0 = HI(d) * MI(x)*/
        " sub %3,r0" "\n\t"               /* %3:%2:%1:%0 -= MIL(d) * MI(x) << 24*/
        " mul %6,%16" "\n\t"              /* r1:r0 = LO(d) * HI(x)*/
        " sub %2,r0" "\n\t"
        " sbc %3,r1" "\n\t"               /* %3:%2:%1:%0 -= LO(d) * HI(x) << 16*/
        " mul %7,%16" "\n\t"              /* r1:r0 = MI(d) * HI(x)*/
        " sub %3,r0" "\n\t"               /* %3:%2:%1:%0 -= MI(d) * HI(x) << 24*/
        /*  %3:%2:%1:%0 = (1<<25) - x*d     [169]*/

        /*  We need to multiply that result by x, and we are only interested in the top 24bits of that multiply*/

        /*  %16:%15:%14 = x = initial estimation of 0x1000000 / d*/
        /*  %3:%2:%1:%0 = (1<<25) - x*d = acc*/
        /*  %13 = 0 */

        /*  result = %11:%10:%9:%5:%4*/
        " mul %14,%0" "\n\t"              /* r1:r0 = LO(x) * LO(acc)*/
        " mov %4,r1" "\n\t"
        " clr %5" "\n\t"
        " clr %9" "\n\t"
        " clr %10" "\n\t"
        " clr %11" "\n\t"                 /* %11:%10:%9:%5:%4 = LO(x) * LO(acc) >> 8*/
        " mul %15,%0" "\n\t"              /* r1:r0 = MI(x) * LO(acc)*/
        " add %4,r0" "\n\t"
        " adc %5,r1" "\n\t"
        " adc %9,%13" "\n\t"
        " adc %10,%13" "\n\t"
        " adc %11,%13" "\n\t"             /* %11:%10:%9:%5:%4 += MI(x) * LO(acc) */
        " mul %16,%0" "\n\t"              /* r1:r0 = HI(x) * LO(acc)*/
        " add %5,r0" "\n\t"
        " adc %9,r1" "\n\t"
        " adc %10,%13" "\n\t"
        " adc %11,%13" "\n\t"             /* %11:%10:%9:%5:%4 += MI(x) * LO(acc) << 8*/

        " mul %14,%1" "\n\t"              /* r1:r0 = LO(x) * MIL(acc)*/
        " add %4,r0" "\n\t"
        " adc %5,r1" "\n\t"
        " adc %9,%13" "\n\t"
        " adc %10,%13" "\n\t"
        " adc %11,%13" "\n\t"             /* %11:%10:%9:%5:%4 = LO(x) * MIL(acc)*/
        " mul %15,%1" "\n\t"              /* r1:r0 = MI(x) * MIL(acc)*/
        " add %5,r0" "\n\t"
        " adc %9,r1" "\n\t"
        " adc %10,%13" "\n\t"
        " adc %11,%13" "\n\t"             /* %11:%10:%9:%5:%4 += MI(x) * MIL(acc) << 8*/
        " mul %16,%1" "\n\t"              /* r1:r0 = HI(x) * MIL(acc)*/
        " add %9,r0" "\n\t"
        " adc %10,r1" "\n\t"
        " adc %11,%13" "\n\t"             /* %11:%10:%9:%5:%4 += MI(x) * MIL(acc) << 16*/

        " mul %14,%2" "\n\t"              /* r1:r0 = LO(x) * MIH(acc)*/
        " add %5,r0" "\n\t"
        " adc %9,r1" "\n\t"
        " adc %10,%13" "\n\t"
        " adc %11,%13" "\n\t"             /* %11:%10:%9:%5:%4 = LO(x) * MIH(acc) << 8*/
        " mul %15,%2" "\n\t"              /* r1:r0 = MI(x) * MIH(acc)*/
        " add %9,r0" "\n\t"
        " adc %10,r1" "\n\t"
        " adc %11,%13" "\n\t"             /* %11:%10:%9:%5:%4 += MI(x) * MIH(acc) << 16*/
        " mul %16,%2" "\n\t"              /* r1:r0 = HI(x) * MIH(acc)*/
        " add %10,r0" "\n\t"
        " adc %11,r1" "\n\t"              /* %11:%10:%9:%5:%4 += MI(x) * MIH(acc) << 24*/

        " mul %14,%3" "\n\t"              /* r1:r0 = LO(x) * HI(acc)*/
        " add %9,r0" "\n\t"
        " adc %10,r1" "\n\t"
        " adc %11,%13" "\n\t"             /* %11:%10:%9:%5:%4 = LO(x) * HI(acc) << 16*/
        " mul %15,%3" "\n\t"              /* r1:r0 = MI(x) * HI(acc)*/
        " add %10,r0" "\n\t"
        " adc %11,r1" "\n\t"              /* %11:%10:%9:%5:%4 += MI(x) * HI(acc) << 24*/
        " mul %16,%3" "\n\t"              /* r1:r0 = HI(x) * HI(acc)*/
        " add %11,r0" "\n\t"              /* %11:%10:%9:%5:%4 += MI(x) * HI(acc) << 32*/

        /*  At this point, %11:%10:%9 contains the new estimation of x. */

        /*  Finally, we must correct the result. Estimate remainder as*/
        /*  (1<<24) - x*d*/
        /*  %11:%10:%9 = x*/
        /*  %8:%7:%6 = d = interval" "\n\t" /*  */
        " ldi %3,1" "\n\t"
        " clr %2" "\n\t"
        " clr %1" "\n\t"
        " clr %0" "\n\t"                  /* %3:%2:%1:%0 = 0x1000000*/
        " mul %6,%9" "\n\t"              /* r1:r0 = LO(d) * LO(x)*/
        " sub %0,r0" "\n\t"
        " sbc %1,r1" "\n\t"
        " sbc %2,%13" "\n\t"
        " sbc %3,%13" "\n\t"              /* %3:%2:%1:%0 -= LO(d) * LO(x)*/
        " mul %7,%9" "\n\t"              /* r1:r0 = MI(d) * LO(x)*/
        " sub %1,r0" "\n\t"
        " sbc %2,r1" "\n\t"
        " sbc %3,%13" "\n\t"              /* %3:%2:%1:%0 -= MI(d) * LO(x) << 8*/
        " mul %8,%9" "\n\t"              /* r1:r0 = HI(d) * LO(x)*/
        " sub %2,r0" "\n\t"
        " sbc %3,r1" "\n\t"               /* %3:%2:%1:%0 -= MIL(d) * LO(x) << 16*/
        " mul %6,%10" "\n\t"              /* r1:r0 = LO(d) * MI(x)*/
        " sub %1,r0" "\n\t"
        " sbc %2,r1" "\n\t"
        " sbc %3,%13" "\n\t"              /* %3:%2:%1:%0 -= LO(d) * MI(x) << 8*/
        " mul %7,%10" "\n\t"              /* r1:r0 = MI(d) * MI(x)*/
        " sub %2,r0" "\n\t"
        " sbc %3,r1" "\n\t"               /* %3:%2:%1:%0 -= MI(d) * MI(x) << 16*/
        " mul %8,%10" "\n\t"              /* r1:r0 = HI(d) * MI(x)*/
        " sub %3,r0" "\n\t"               /* %3:%2:%1:%0 -= MIL(d) * MI(x) << 24*/
        " mul %6,%11" "\n\t"              /* r1:r0 = LO(d) * HI(x)*/
        " sub %2,r0" "\n\t"
        " sbc %3,r1" "\n\t"               /* %3:%2:%1:%0 -= LO(d) * HI(x) << 16*/
        " mul %7,%11" "\n\t"              /* r1:r0 = MI(d) * HI(x)*/
        " sub %3,r0" "\n\t"               /* %3:%2:%1:%0 -= MI(d) * HI(x) << 24*/
        /*  %3:%2:%1:%0 = r = (1<<24) - x*d*/
        /*  %8:%7:%6 = d = interval */

        /*  Perform the final correction*/
        " sub %0,%6" "\n\t"
        " sbc %1,%7" "\n\t"
        " sbc %2,%8" "\n\t"              /* r -= d*/
        " brcs 14f" "\n\t"                /* if ( r >= d) */

        /*  %11:%10:%9 = x */
        " ldi %3,1" "\n\t"
        " add %9,%3" "\n\t"
        " adc %10,%13" "\n\t"
        " adc %11,%13" "\n\t"             /* x++*/
        "14:" "\n\t"

        /*  Estimation is done. %11:%10:%9 = x */
        " clr __zero_reg__" "\n\t"        /* Make C runtime happy */
        /*  [211 cycles total]*/
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
    // All the other 32 CPUs can easily perform the inverse using hardware division,
    // so we don´t need to reduce precision or to use assembly language at all.

    // This routine, for all the other archs, returns 0x100000000 / d ~= 0xFFFFFFFF / d
    static FORCE_INLINE uint32_t get_period_inverse(uint32_t d) {
      return 0xFFFFFFFF / d;
    }
  #endif
#endif

#define MINIMAL_STEP_RATE 120

/**
 * Calculate trapezoid parameters, multiplying the entry- and exit-speeds
 * by the provided factors.
 */
void Planner::calculate_trapezoid_for_block(block_t* const block, const float &entry_factor, const float &exit_factor) {
  uint32_t initial_rate = CEIL(block->nominal_rate * entry_factor),
           final_rate = CEIL(block->nominal_rate * exit_factor); // (steps per second)

  // Limit minimal step rate (Otherwise the timer will overflow.)
  NOLESS(initial_rate, MINIMAL_STEP_RATE);
  NOLESS(final_rate, MINIMAL_STEP_RATE);

  #if ENABLED(BEZIER_JERK_CONTROL)
    uint32_t cruise_rate = initial_rate;
  #endif

  const int32_t accel = block->acceleration_steps_per_s2;

          // Steps required for acceleration, deceleration to/from nominal rate
  int32_t accelerate_steps = CEIL(estimate_acceleration_distance(initial_rate, block->nominal_rate, accel)),
          decelerate_steps = FLOOR(estimate_acceleration_distance(block->nominal_rate, final_rate, -accel)),
          // Steps between acceleration and deceleration, if any
          plateau_steps = block->step_event_count - accelerate_steps - decelerate_steps;

  // Does accelerate_steps + decelerate_steps exceed step_event_count?
  // Then we can't possibly reach the nominal rate, there will be no cruising.
  // Use intersection_distance() to calculate accel / braking time in order to
  // reach the final_rate exactly at the end of this block.
  if (plateau_steps < 0) {
    accelerate_steps = CEIL(intersection_distance(initial_rate, final_rate, accel, block->step_event_count));
    NOLESS(accelerate_steps, 0); // Check limits due to numerical round-off
    accelerate_steps = min((uint32_t)accelerate_steps, block->step_event_count);//(We can cast here to unsigned, because the above line ensures that we are above zero)
    plateau_steps = 0;

    #if ENABLED(BEZIER_JERK_CONTROL)
      // We won't reach the cruising rate. Let's calculate the speed we will reach
      cruise_rate = final_speed(initial_rate, accel, accelerate_steps);
    #endif
  }
  #if ENABLED(BEZIER_JERK_CONTROL)
    else // We have some plateau time, so the cruise rate will be the nominal rate
      cruise_rate = block->nominal_rate;
  #endif

  // block->accelerate_until = accelerate_steps;
  // block->decelerate_after = accelerate_steps+plateau_steps;

  #if ENABLED(BEZIER_JERK_CONTROL)
    // Jerk controlled speed requires to express speed versus time, NOT steps
    uint32_t acceleration_time = ((float)(cruise_rate - initial_rate) / accel) * HAL_STEPPER_TIMER_RATE,
             deceleration_time = ((float)(cruise_rate - final_rate) / accel) * HAL_STEPPER_TIMER_RATE;

    // And to offload calculations from the ISR, we also calculate the inverse of those times here
    uint32_t acceleration_time_inverse = get_period_inverse(acceleration_time);
    uint32_t deceleration_time_inverse = get_period_inverse(deceleration_time);

  #endif

  CRITICAL_SECTION_START;  // Fill variables used by the stepper in a critical section
  if (!TEST(block->flag, BLOCK_BIT_BUSY)) { // Don't update variables if block is busy.
    block->accelerate_until = accelerate_steps;
    block->decelerate_after = accelerate_steps + plateau_steps;
    block->initial_rate = initial_rate;
    #if ENABLED(BEZIER_JERK_CONTROL)
      block->acceleration_time = acceleration_time;
      block->deceleration_time = deceleration_time;
      block->acceleration_time_inverse = acceleration_time_inverse;
      block->deceleration_time_inverse = deceleration_time_inverse;
      block->cruise_rate = cruise_rate;
    #endif
    block->final_rate = final_rate;
  }
  CRITICAL_SECTION_END;
}

// "Junction jerk" in this context is the immediate change in speed at the junction of two blocks.
// This method will calculate the junction jerk as the euclidean distance between the nominal
// velocities of the respective blocks.
//inline float junction_jerk(block_t *before, block_t *after) {
//  return SQRT(
//    POW((before->speed_x-after->speed_x), 2)+POW((before->speed_y-after->speed_y), 2));
//}


// The kernel called by recalculate() when scanning the plan from last to first entry.
void Planner::reverse_pass_kernel(block_t* const current, const block_t * const next) {
  if (!current || !next) return;
  // If entry speed is already at the maximum entry speed, no need to recheck. Block is cruising.
  // If not, block in state of acceleration or deceleration. Reset entry speed to maximum and
  // check for maximum allowable speed reductions to ensure maximum possible planned speed.
  float max_entry_speed = current->max_entry_speed;
  if (current->entry_speed != max_entry_speed) {
    // If nominal length true, max junction speed is guaranteed to be reached. Only compute
    // for max allowable speed if block is decelerating and nominal length is false.
    current->entry_speed = (TEST(current->flag, BLOCK_BIT_NOMINAL_LENGTH) || max_entry_speed <= next->entry_speed)
      ? max_entry_speed
      : min(max_entry_speed, max_allowable_speed(-current->acceleration, next->entry_speed, current->millimeters));
    SBI(current->flag, BLOCK_BIT_RECALCULATE);
  }
}

/**
 * recalculate() needs to go over the current plan twice.
 * Once in reverse and once forward. This implements the reverse pass.
 */
void Planner::reverse_pass() {
  if (movesplanned() > 2) {
    const uint8_t endnr = BLOCK_MOD(block_buffer_tail + 1); // tail is running. tail+1 shouldn't be altered because it's connected to the running block.
    uint8_t blocknr = prev_block_index(block_buffer_head);
    block_t* current = &block_buffer[blocknr];

    // Last/newest block in buffer:
    const float max_entry_speed = current->max_entry_speed;
    if (current->entry_speed != max_entry_speed) {
      // If nominal length true, max junction speed is guaranteed to be reached. Only compute
      // for max allowable speed if block is decelerating and nominal length is false.
      current->entry_speed = TEST(current->flag, BLOCK_BIT_NOMINAL_LENGTH)
        ? max_entry_speed
        : min(max_entry_speed, max_allowable_speed(-current->acceleration, MINIMUM_PLANNER_SPEED, current->millimeters));
      SBI(current->flag, BLOCK_BIT_RECALCULATE);
    }

    do {
      const block_t * const next = current;
      blocknr = prev_block_index(blocknr);
      current = &block_buffer[blocknr];
      reverse_pass_kernel(current, next);
    } while (blocknr != endnr);
  }
}

// The kernel called by recalculate() when scanning the plan from first to last entry.
void Planner::forward_pass_kernel(const block_t * const previous, block_t* const current) {
  if (!previous) return;

  // If the previous block is an acceleration block, but it is not long enough to complete the
  // full speed change within the block, we need to adjust the entry speed accordingly. Entry
  // speeds have already been reset, maximized, and reverse planned by reverse planner.
  // If nominal length is true, max junction speed is guaranteed to be reached. No need to recheck.
  if (!TEST(previous->flag, BLOCK_BIT_NOMINAL_LENGTH)) {
    if (previous->entry_speed < current->entry_speed) {
      float entry_speed = min(current->entry_speed,
                               max_allowable_speed(-previous->acceleration, previous->entry_speed, previous->millimeters));
      // Check for junction speed change
      if (current->entry_speed != entry_speed) {
        current->entry_speed = entry_speed;
        SBI(current->flag, BLOCK_BIT_RECALCULATE);
      }
    }
  }
}

/**
 * recalculate() needs to go over the current plan twice.
 * Once in reverse and once forward. This implements the forward pass.
 */
void Planner::forward_pass() {
  block_t* block[3] = { NULL, NULL, NULL };

  for (uint8_t b = block_buffer_tail; b != block_buffer_head; b = next_block_index(b)) {
    block[0] = block[1];
    block[1] = block[2];
    block[2] = &block_buffer[b];
    forward_pass_kernel(block[0], block[1]);
  }
  forward_pass_kernel(block[1], block[2]);
}

/**
 * Recalculate the trapezoid speed profiles for all blocks in the plan
 * according to the entry_factor for each junction. Must be called by
 * recalculate() after updating the blocks.
 */
void Planner::recalculate_trapezoids() {
  int8_t block_index = block_buffer_tail;
  block_t *current, *next = NULL;

  while (block_index != block_buffer_head) {
    current = next;
    next = &block_buffer[block_index];
    if (current) {
      // Recalculate if current block entry or exit junction speed has changed.
      if (TEST(current->flag, BLOCK_BIT_RECALCULATE) || TEST(next->flag, BLOCK_BIT_RECALCULATE)) {
        // NOTE: Entry and exit factors always > 0 by all previous logic operations.
        const float nomr = 1.0 / current->nominal_speed;
        calculate_trapezoid_for_block(current, current->entry_speed * nomr, next->entry_speed * nomr);
        #if ENABLED(LIN_ADVANCE)
          if (current->use_advance_lead) {
            const float comp = current->e_D_ratio * extruder_advance_K * axis_steps_per_mm[E_AXIS];
            current->max_adv_steps = current->nominal_speed * comp;
            current->final_adv_steps = next->entry_speed * comp;
          }
        #endif
        CBI(current->flag, BLOCK_BIT_RECALCULATE); // Reset current only to ensure next trapezoid is computed
      }
    }
    block_index = next_block_index(block_index);
  }
  // Last/newest block in buffer. Exit speed is set with MINIMUM_PLANNER_SPEED. Always recalculated.
  if (next) {
    const float nomr = 1.0 / next->nominal_speed;
    calculate_trapezoid_for_block(next, next->entry_speed * nomr, (MINIMUM_PLANNER_SPEED) * nomr);
    #if ENABLED(LIN_ADVANCE)
      if (next->use_advance_lead) {
        const float comp = next->e_D_ratio * extruder_advance_K * axis_steps_per_mm[E_AXIS];
        next->max_adv_steps = next->nominal_speed * comp;
        next->final_adv_steps = (MINIMUM_PLANNER_SPEED) * comp;
      }
    #endif
    CBI(next->flag, BLOCK_BIT_RECALCULATE);
  }
}

/**
 * Recalculate the motion plan according to the following algorithm:
 *
 *   1. Go over every block in reverse order...
 *
 *      Calculate a junction speed reduction (block_t.entry_factor) so:
 *
 *      a. The junction jerk is within the set limit, and
 *
 *      b. No speed reduction within one block requires faster
 *         deceleration than the one, true constant acceleration.
 *
 *   2. Go over every block in chronological order...
 *
 *      Dial down junction speed reduction values if:
 *      a. The speed increase within one block would require faster
 *         acceleration than the one, true constant acceleration.
 *
 * After that, all blocks will have an entry_factor allowing all speed changes to
 * be performed using only the one, true constant acceleration, and where no junction
 * jerk is jerkier than the set limit, Jerky. Finally it will:
 *
 *   3. Recalculate "trapezoids" for all blocks.
 */
void Planner::recalculate() {
  reverse_pass();
  forward_pass();
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
      if (block->steps[X_AXIS] || block->steps[Y_AXIS] || block->steps[Z_AXIS]) {
        float se = (float)block->steps[E_AXIS] / block->step_event_count * block->nominal_speed; // mm/sec;
        NOLESS(high, se);
      }
    }

    float t = autotemp_min + high * autotemp_factor;
    t = constrain(t, autotemp_min, autotemp_max);
    if (t < oldt) t = t * (1 - (AUTOTEMP_OLDWEIGHT)) + oldt * (AUTOTEMP_OLDWEIGHT);
    oldt = t;
    thermalManager.setTargetHotend(t, 0);
  }

#endif // AUTOTEMP

/**
 * Maintain fans, paste extruder pressure,
 */
void Planner::check_axes_activity() {
  unsigned char axis_active[NUM_AXIS] = { 0 },
                tail_fan_speed[FAN_COUNT];

  #if ENABLED(BARICUDA)
    #if HAS_HEATER_1
      uint8_t tail_valve_pressure;
    #endif
    #if HAS_HEATER_2
      uint8_t tail_e_to_p_pressure;
    #endif
  #endif

  if (has_blocks_queued()) {

    #if FAN_COUNT > 0
      for (uint8_t i = 0; i < FAN_COUNT; i++)
        tail_fan_speed[i] = block_buffer[block_buffer_tail].fan_speed[i];
    #endif

    block_t* block;

    #if ENABLED(BARICUDA)
      block = &block_buffer[block_buffer_tail];
      #if HAS_HEATER_1
        tail_valve_pressure = block->valve_pressure;
      #endif
      #if HAS_HEATER_2
        tail_e_to_p_pressure = block->e_to_p_pressure;
      #endif
    #endif

    for (uint8_t b = block_buffer_tail; b != block_buffer_head; b = next_block_index(b)) {
      block = &block_buffer[b];
      LOOP_XYZE(i) if (block->steps[i]) axis_active[i]++;
    }
  }
  else {
    #if FAN_COUNT > 0
      for (uint8_t i = 0; i < FAN_COUNT; i++) tail_fan_speed[i] = fanSpeeds[i];
    #endif

    #if ENABLED(BARICUDA)
      #if HAS_HEATER_1
        tail_valve_pressure = baricuda_valve_pressure;
      #endif
      #if HAS_HEATER_2
        tail_e_to_p_pressure = baricuda_e_to_p_pressure;
      #endif
    #endif
  }

  #if ENABLED(DISABLE_X)
    if (!axis_active[X_AXIS]) disable_X();
  #endif
  #if ENABLED(DISABLE_Y)
    if (!axis_active[Y_AXIS]) disable_Y();
  #endif
  #if ENABLED(DISABLE_Z)
    if (!axis_active[Z_AXIS]) disable_Z();
  #endif
  #if ENABLED(DISABLE_E)
    if (!axis_active[E_AXIS]) disable_e_steppers();
  #endif

  #if FAN_COUNT > 0

    #if FAN_KICKSTART_TIME > 0

      static millis_t fan_kick_end[FAN_COUNT] = { 0 };

      #define KICKSTART_FAN(f) \
        if (tail_fan_speed[f]) { \
          millis_t ms = millis(); \
          if (fan_kick_end[f] == 0) { \
            fan_kick_end[f] = ms + FAN_KICKSTART_TIME; \
            tail_fan_speed[f] = 255; \
          } else if (PENDING(ms, fan_kick_end[f])) \
            tail_fan_speed[f] = 255; \
        } else fan_kick_end[f] = 0

      #if HAS_FAN0
        KICKSTART_FAN(0);
      #endif
      #if HAS_FAN1
        KICKSTART_FAN(1);
      #endif
      #if HAS_FAN2
        KICKSTART_FAN(2);
      #endif

    #endif // FAN_KICKSTART_TIME > 0

    #ifdef FAN_MIN_PWM
      #define CALC_FAN_SPEED(f) (tail_fan_speed[f] ? ( FAN_MIN_PWM + (tail_fan_speed[f] * (255 - FAN_MIN_PWM)) / 255 ) : 0)
    #else
      #define CALC_FAN_SPEED(f) tail_fan_speed[f]
    #endif

    #if ENABLED(FAN_SOFT_PWM)
      #if HAS_FAN0
        thermalManager.soft_pwm_amount_fan[0] = CALC_FAN_SPEED(0);
      #endif
      #if HAS_FAN1
        thermalManager.soft_pwm_amount_fan[1] = CALC_FAN_SPEED(1);
      #endif
      #if HAS_FAN2
        thermalManager.soft_pwm_amount_fan[2] = CALC_FAN_SPEED(2);
      #endif
    #else
      #if HAS_FAN0
        analogWrite(FAN_PIN, CALC_FAN_SPEED(0));
      #endif
      #if HAS_FAN1
        analogWrite(FAN1_PIN, CALC_FAN_SPEED(1));
      #endif
      #if HAS_FAN2
        analogWrite(FAN2_PIN, CALC_FAN_SPEED(2));
      #endif
    #endif

  #endif // FAN_COUNT > 0

  #if ENABLED(AUTOTEMP)
    getHighESpeed();
  #endif

  #if ENABLED(BARICUDA)
    #if HAS_HEATER_1
      analogWrite(HEATER_1_PIN, tail_valve_pressure);
    #endif
    #if HAS_HEATER_2
      analogWrite(HEATER_2_PIN, tail_e_to_p_pressure);
    #endif
  #endif
}

#if DISABLED(NO_VOLUMETRICS)

  /**
   * Get a volumetric multiplier from a filament diameter.
   * This is the reciprocal of the circular cross-section area.
   * Return 1.0 with volumetric off or a diameter of 0.0.
   */
  inline float calculate_volumetric_multiplier(const float &diameter) {
    return (parser.volumetric_enabled && diameter) ? 1.0 / CIRCLE_AREA(diameter * 0.5) : 1.0;
  }

  /**
   * Convert the filament sizes into volumetric multipliers.
   * The multiplier converts a given E value into a length.
   */
  void Planner::calculate_volumetric_multipliers() {
    for (uint8_t i = 0; i < COUNT(filament_size); i++) {
      volumetric_multiplier[i] = calculate_volumetric_multiplier(filament_size[i]);
      refresh_e_factor(i);
    }
  }

#endif // !NO_VOLUMETRICS

#if ENABLED(FILAMENT_WIDTH_SENSOR)
  /**
   * Convert the ratio value given by the filament width sensor
   * into a volumetric multiplier. Conversion differs when using
   * linear extrusion vs volumetric extrusion.
   */
  void Planner::calculate_volumetric_for_width_sensor(const int8_t encoded_ratio) {
    // Reconstitute the nominal/measured ratio
    const float nom_meas_ratio = 1.0 + 0.01 * encoded_ratio,
                ratio_2 = sq(nom_meas_ratio);

    volumetric_multiplier[FILAMENT_SENSOR_EXTRUDER_NUM] = parser.volumetric_enabled
      ? ratio_2 / CIRCLE_AREA(filament_width_nominal * 0.5) // Volumetric uses a true volumetric multiplier
      : ratio_2;                                            // Linear squares the ratio, which scales the volume

    refresh_e_factor(FILAMENT_SENSOR_EXTRUDER_NUM);
  }
#endif

#if PLANNER_LEVELING
  /**
   * rx, ry, rz - Cartesian positions in mm
   *              Leveled XYZ on completion
   */
  void Planner::apply_leveling(float &rx, float &ry, float &rz) {

    #if ENABLED(SKEW_CORRECTION)
      skew(rx, ry, rz);
    #endif

    if (!leveling_active) return;

    #if ABL_PLANAR

      float dx = rx - (X_TILT_FULCRUM),
            dy = ry - (Y_TILT_FULCRUM);

      apply_rotation_xyz(bed_level_matrix, dx, dy, rz);

      rx = dx + X_TILT_FULCRUM;
      ry = dy + Y_TILT_FULCRUM;

    #elif HAS_MESH

      #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
        const float fade_scaling_factor = fade_scaling_factor_for_z(rz);
      #else
        constexpr float fade_scaling_factor = 1.0;
      #endif

      #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
        const float raw[XYZ] = { rx, ry, 0 };
      #endif

      rz += (
        #if ENABLED(MESH_BED_LEVELING)
          mbl.get_z(rx, ry
            #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
              , fade_scaling_factor
            #endif
          )
        #elif ENABLED(AUTO_BED_LEVELING_UBL)
          fade_scaling_factor ? fade_scaling_factor * ubl.get_z_correction(rx, ry) : 0.0
        #elif ENABLED(AUTO_BED_LEVELING_BILINEAR)
          fade_scaling_factor ? fade_scaling_factor * bilinear_z_offset(raw) : 0.0
        #endif
      );

    #endif
  }

  void Planner::unapply_leveling(float raw[XYZ]) {

    if (leveling_active) {

      #if ABL_PLANAR

        matrix_3x3 inverse = matrix_3x3::transpose(bed_level_matrix);

        float dx = raw[X_AXIS] - (X_TILT_FULCRUM),
              dy = raw[Y_AXIS] - (Y_TILT_FULCRUM);

        apply_rotation_xyz(inverse, dx, dy, raw[Z_AXIS]);

        raw[X_AXIS] = dx + X_TILT_FULCRUM;
        raw[Y_AXIS] = dy + Y_TILT_FULCRUM;

      #elif HAS_MESH

        #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
          const float fade_scaling_factor = fade_scaling_factor_for_z(raw[Z_AXIS]);
        #else
          constexpr float fade_scaling_factor = 1.0;
        #endif

        raw[Z_AXIS] -= (
          #if ENABLED(MESH_BED_LEVELING)
            mbl.get_z(raw[X_AXIS], raw[Y_AXIS]
              #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
                , fade_scaling_factor
              #endif
            )
          #elif ENABLED(AUTO_BED_LEVELING_UBL)
            fade_scaling_factor ? fade_scaling_factor * ubl.get_z_correction(raw[X_AXIS], raw[Y_AXIS]) : 0.0
          #elif ENABLED(AUTO_BED_LEVELING_BILINEAR)
            fade_scaling_factor ? fade_scaling_factor * bilinear_z_offset(raw) : 0.0
          #endif
        );

      #endif
    }

    #if ENABLED(SKEW_CORRECTION)
      unskew(raw[X_AXIS], raw[Y_AXIS], raw[Z_AXIS]);
    #endif
  }

#endif // PLANNER_LEVELING

/**
 * Planner::_buffer_steps
 *
 * Add a new linear movement to the buffer (in terms of steps).
 *
 *  target      - target position in steps units
 *  fr_mm_s     - (target) speed of the move
 *  extruder    - target extruder
 */
void Planner::_buffer_steps(const int32_t (&target)[XYZE]
  #if HAS_POSITION_FLOAT
    , const float (&target_float)[XYZE]
  #endif
  , float fr_mm_s, const uint8_t extruder, const float &millimeters/*=0.0*/
) {

  const int32_t da = target[A_AXIS] - position[A_AXIS],
                db = target[B_AXIS] - position[B_AXIS],
                dc = target[C_AXIS] - position[C_AXIS];

  int32_t de = target[E_AXIS] - position[E_AXIS];

  /* <-- add a slash to enable
    SERIAL_ECHOPAIR("  _buffer_steps FR:", fr_mm_s);
    SERIAL_ECHOPAIR(" A:", target[A_AXIS]);
    SERIAL_ECHOPAIR(" (", da);
    SERIAL_ECHOPAIR(" steps) B:", target[B_AXIS]);
    SERIAL_ECHOPAIR(" (", db);
    SERIAL_ECHOPAIR(" steps) C:", target[C_AXIS]);
    SERIAL_ECHOPAIR(" (", dc);
    SERIAL_ECHOPAIR(" steps) E:", target[E_AXIS]);
    SERIAL_ECHOPAIR(" (", de);
    SERIAL_ECHOLNPGM(" steps)");
  //*/

  #if ENABLED(PREVENT_COLD_EXTRUSION) || ENABLED(PREVENT_LENGTHY_EXTRUDE)
    if (de) {
      #if ENABLED(PREVENT_COLD_EXTRUSION)
        if (thermalManager.tooColdToExtrude(extruder)) {
          position[E_AXIS] = target[E_AXIS]; // Behave as if the move really took place, but ignore E part
          #if HAS_POSITION_FLOAT
            position_float[E_AXIS] = target_float[E_AXIS];
          #endif
          de = 0; // no difference
          SERIAL_ECHO_START();
          SERIAL_ECHOLNPGM(MSG_ERR_COLD_EXTRUDE_STOP);
        }
      #endif // PREVENT_COLD_EXTRUSION
      #if ENABLED(PREVENT_LENGTHY_EXTRUDE)
        if (labs(de * e_factor[extruder]) > (int32_t)axis_steps_per_mm[E_AXIS_N] * (EXTRUDE_MAXLENGTH)) { // It's not important to get max. extrusion length in a precision < 1mm, so save some cycles and cast to int
          position[E_AXIS] = target[E_AXIS]; // Behave as if the move really took place, but ignore E part
          #if HAS_POSITION_FLOAT
            position_float[E_AXIS] = target_float[E_AXIS];
          #endif
          de = 0; // no difference
          SERIAL_ECHO_START();
          SERIAL_ECHOLNPGM(MSG_ERR_LONG_EXTRUDE_STOP);
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

  const float esteps_float = de * e_factor[extruder];
  const int32_t esteps = abs(esteps_float) + 0.5;

  // Calculate the buffer head after we push this byte
  const uint8_t next_buffer_head = next_block_index(block_buffer_head);

  // If the buffer is full: good! That means we are well ahead of the robot.
  // Rest here until there is room in the buffer.
  while (block_buffer_tail == next_buffer_head) idle();

  // Prepare to set up new block
  block_t* block = &block_buffer[block_buffer_head];

  // Clear all flags, including the "busy" bit
  block->flag = 0x00;

  // Set direction bits
  block->direction_bits = dm;

  // Number of steps for each axis
  // See http://www.corexy.com/theory.html
  #if CORE_IS_XY
    block->steps[A_AXIS] = labs(da + db);
    block->steps[B_AXIS] = labs(da - db);
    block->steps[Z_AXIS] = labs(dc);
  #elif CORE_IS_XZ
    block->steps[A_AXIS] = labs(da + dc);
    block->steps[Y_AXIS] = labs(db);
    block->steps[C_AXIS] = labs(da - dc);
  #elif CORE_IS_YZ
    block->steps[X_AXIS] = labs(da);
    block->steps[B_AXIS] = labs(db + dc);
    block->steps[C_AXIS] = labs(db - dc);
  #elif IS_SCARA
    block->steps[A_AXIS] = labs(da);
    block->steps[B_AXIS] = labs(db);
    block->steps[Z_AXIS] = labs(dc);
  #else
    // default non-h-bot planning
    block->steps[A_AXIS] = labs(da);
    block->steps[B_AXIS] = labs(db);
    block->steps[C_AXIS] = labs(dc);
  #endif

  block->steps[E_AXIS] = esteps;
  block->step_event_count = MAX4(block->steps[A_AXIS], block->steps[B_AXIS], block->steps[C_AXIS], esteps);

  // Bail if this is a zero-length block
  if (block->step_event_count < MIN_STEPS_PER_SEGMENT) return;

  // For a mixing extruder, get a magnified step_event_count for each
  #if ENABLED(MIXING_EXTRUDER)
    for (uint8_t i = 0; i < MIXING_STEPPERS; i++)
      block->mix_event_count[i] = mixing_factor[i] * block->step_event_count;
  #endif

  #if FAN_COUNT > 0
    for (uint8_t i = 0; i < FAN_COUNT; i++) block->fan_speed[i] = fanSpeeds[i];
  #endif

  #if ENABLED(BARICUDA)
    block->valve_pressure = baricuda_valve_pressure;
    block->e_to_p_pressure = baricuda_e_to_p_pressure;
  #endif

  block->active_extruder = extruder;

  #if ENABLED(AUTO_POWER_CONTROL)
    if (block->steps[X_AXIS] || block->steps[Y_AXIS] || block->steps[Z_AXIS])
      powerManager.power_on();
  #endif

  // Enable active axes
  #if CORE_IS_XY
    if (block->steps[A_AXIS] || block->steps[B_AXIS]) {
      enable_X();
      enable_Y();
    }
    #if DISABLED(Z_LATE_ENABLE)
      if (block->steps[Z_AXIS]) enable_Z();
    #endif
  #elif CORE_IS_XZ
    if (block->steps[A_AXIS] || block->steps[C_AXIS]) {
      enable_X();
      enable_Z();
    }
    if (block->steps[Y_AXIS]) enable_Y();
  #elif CORE_IS_YZ
    if (block->steps[B_AXIS] || block->steps[C_AXIS]) {
      enable_Y();
      enable_Z();
    }
    if (block->steps[X_AXIS]) enable_X();
  #else
    if (block->steps[X_AXIS]) enable_X();
    if (block->steps[Y_AXIS]) enable_Y();
    #if DISABLED(Z_LATE_ENABLE)
      if (block->steps[Z_AXIS]) enable_Z();
    #endif
  #endif

  // Enable extruder(s)
  if (esteps) {
    #if ENABLED(AUTO_POWER_CONTROL)
      powerManager.power_on();
    #endif

    #if ENABLED(DISABLE_INACTIVE_EXTRUDER) // Enable only the selected extruder

      #define DISABLE_IDLE_E(N) if (!g_uc_extruder_last_move[N]) disable_E##N();

      for (uint8_t i = 0; i < EXTRUDERS; i++)
        if (g_uc_extruder_last_move[i] > 0) g_uc_extruder_last_move[i]--;

      switch (extruder) {
        case 0:
          #if EXTRUDERS > 1
            DISABLE_IDLE_E(1);
            #if EXTRUDERS > 2
              DISABLE_IDLE_E(2);
              #if EXTRUDERS > 3
                DISABLE_IDLE_E(3);
                #if EXTRUDERS > 4
                  DISABLE_IDLE_E(4);
                #endif // EXTRUDERS > 4
              #endif // EXTRUDERS > 3
            #endif // EXTRUDERS > 2
          #endif // EXTRUDERS > 1
          enable_E0();
          g_uc_extruder_last_move[0] = (BLOCK_BUFFER_SIZE) * 2;
          #if ENABLED(DUAL_X_CARRIAGE) || ENABLED(DUAL_NOZZLE_DUPLICATION_MODE)
            if (extruder_duplication_enabled) {
              enable_E1();
              g_uc_extruder_last_move[1] = (BLOCK_BUFFER_SIZE) * 2;
            }
          #endif
        break;
        #if EXTRUDERS > 1
          case 1:
            DISABLE_IDLE_E(0);
            #if EXTRUDERS > 2
              DISABLE_IDLE_E(2);
              #if EXTRUDERS > 3
                DISABLE_IDLE_E(3);
                #if EXTRUDERS > 4
                  DISABLE_IDLE_E(4);
                #endif // EXTRUDERS > 4
              #endif // EXTRUDERS > 3
            #endif // EXTRUDERS > 2
            enable_E1();
            g_uc_extruder_last_move[1] = (BLOCK_BUFFER_SIZE) * 2;
          break;
          #if EXTRUDERS > 2
            case 2:
              DISABLE_IDLE_E(0);
              DISABLE_IDLE_E(1);
              #if EXTRUDERS > 3
                DISABLE_IDLE_E(3);
                #if EXTRUDERS > 4
                  DISABLE_IDLE_E(4);
                #endif
              #endif
              enable_E2();
              g_uc_extruder_last_move[2] = (BLOCK_BUFFER_SIZE) * 2;
            break;
            #if EXTRUDERS > 3
              case 3:
                DISABLE_IDLE_E(0);
                DISABLE_IDLE_E(1);
                DISABLE_IDLE_E(2);
                #if EXTRUDERS > 4
                  DISABLE_IDLE_E(4);
                #endif
                enable_E3();
                g_uc_extruder_last_move[3] = (BLOCK_BUFFER_SIZE) * 2;
              break;
              #if EXTRUDERS > 4
                case 4:
                  DISABLE_IDLE_E(0);
                  DISABLE_IDLE_E(1);
                  DISABLE_IDLE_E(2);
                  DISABLE_IDLE_E(3);
                  enable_E4();
                  g_uc_extruder_last_move[4] = (BLOCK_BUFFER_SIZE) * 2;
                break;
              #endif // EXTRUDERS > 4
            #endif // EXTRUDERS > 3
          #endif // EXTRUDERS > 2
        #endif // EXTRUDERS > 1
      }
    #else
      enable_E0();
      enable_E1();
      enable_E2();
      enable_E3();
      enable_E4();
    #endif
  }

  if (esteps)
    NOLESS(fr_mm_s, min_feedrate_mm_s);
  else
    NOLESS(fr_mm_s, min_travel_feedrate_mm_s);

  /**
   * This part of the code calculates the total length of the movement.
   * For cartesian bots, the X_AXIS is the real X movement and same for Y_AXIS.
   * But for corexy bots, that is not true. The "X_AXIS" and "Y_AXIS" motors (that should be named to A_AXIS
   * and B_AXIS) cannot be used for X and Y length, because A=X+Y and B=X-Y.
   * So we need to create other 2 "AXIS", named X_HEAD and Y_HEAD, meaning the real displacement of the Head.
   * Having the real displacement of the head, we can calculate the total movement length and apply the desired speed.
   */
  #if IS_CORE
    float delta_mm[Z_HEAD + 1];
    #if CORE_IS_XY
      delta_mm[X_HEAD] = da * steps_to_mm[A_AXIS];
      delta_mm[Y_HEAD] = db * steps_to_mm[B_AXIS];
      delta_mm[Z_AXIS] = dc * steps_to_mm[Z_AXIS];
      delta_mm[A_AXIS] = (da + db) * steps_to_mm[A_AXIS];
      delta_mm[B_AXIS] = CORESIGN(da - db) * steps_to_mm[B_AXIS];
    #elif CORE_IS_XZ
      delta_mm[X_HEAD] = da * steps_to_mm[A_AXIS];
      delta_mm[Y_AXIS] = db * steps_to_mm[Y_AXIS];
      delta_mm[Z_HEAD] = dc * steps_to_mm[C_AXIS];
      delta_mm[A_AXIS] = (da + dc) * steps_to_mm[A_AXIS];
      delta_mm[C_AXIS] = CORESIGN(da - dc) * steps_to_mm[C_AXIS];
    #elif CORE_IS_YZ
      delta_mm[X_AXIS] = da * steps_to_mm[X_AXIS];
      delta_mm[Y_HEAD] = db * steps_to_mm[B_AXIS];
      delta_mm[Z_HEAD] = dc * steps_to_mm[C_AXIS];
      delta_mm[B_AXIS] = (db + dc) * steps_to_mm[B_AXIS];
      delta_mm[C_AXIS] = CORESIGN(db - dc) * steps_to_mm[C_AXIS];
    #endif
  #else
    float delta_mm[ABCE];
    delta_mm[A_AXIS] = da * steps_to_mm[A_AXIS];
    delta_mm[B_AXIS] = db * steps_to_mm[B_AXIS];
    delta_mm[C_AXIS] = dc * steps_to_mm[C_AXIS];
  #endif
  delta_mm[E_AXIS] = esteps_float * steps_to_mm[E_AXIS_N];

  if (block->steps[A_AXIS] < MIN_STEPS_PER_SEGMENT && block->steps[B_AXIS] < MIN_STEPS_PER_SEGMENT && block->steps[C_AXIS] < MIN_STEPS_PER_SEGMENT) {
    block->millimeters = FABS(delta_mm[E_AXIS]);
  }
  else if (!millimeters) {
    block->millimeters = SQRT(
      #if CORE_IS_XY
        sq(delta_mm[X_HEAD]) + sq(delta_mm[Y_HEAD]) + sq(delta_mm[Z_AXIS])
      #elif CORE_IS_XZ
        sq(delta_mm[X_HEAD]) + sq(delta_mm[Y_AXIS]) + sq(delta_mm[Z_HEAD])
      #elif CORE_IS_YZ
        sq(delta_mm[X_AXIS]) + sq(delta_mm[Y_HEAD]) + sq(delta_mm[Z_HEAD])
      #else
        sq(delta_mm[X_AXIS]) + sq(delta_mm[Y_AXIS]) + sq(delta_mm[Z_AXIS])
      #endif
    );
  }
  else
    block->millimeters = millimeters;

  const float inverse_millimeters = 1.0 / block->millimeters;  // Inverse millimeters to remove multiple divides

  // Calculate inverse time for this move. No divide by zero due to previous checks.
  // Example: At 120mm/s a 60mm move takes 0.5s. So this will give 2.0.
  float inverse_secs = fr_mm_s * inverse_millimeters;

  const uint8_t moves_queued = movesplanned();

  // Slow down when the buffer starts to empty, rather than wait at the corner for a buffer refill
  #if ENABLED(SLOWDOWN) || ENABLED(ULTRA_LCD) || defined(XY_FREQUENCY_LIMIT)
    // Segment time im micro seconds
    uint32_t segment_time_us = LROUND(1000000.0 / inverse_secs);
  #endif

  #if ENABLED(SLOWDOWN)
    if (WITHIN(moves_queued, 2, (BLOCK_BUFFER_SIZE) / 2 - 1)) {
      if (segment_time_us < min_segment_time_us) {
        // buffer is draining, add extra time.  The amount of time added increases if the buffer is still emptied more.
        const uint32_t nst = segment_time_us + LROUND(2 * (min_segment_time_us - segment_time_us) / moves_queued);
        inverse_secs = 1000000.0 / nst;
        #if defined(XY_FREQUENCY_LIMIT) || ENABLED(ULTRA_LCD)
          segment_time_us = nst;
        #endif
      }
    }
  #endif

  #if ENABLED(ULTRA_LCD)
    CRITICAL_SECTION_START
      block_buffer_runtime_us += segment_time_us;
    CRITICAL_SECTION_END
  #endif

  block->nominal_speed = block->millimeters * inverse_secs;           //   (mm/sec) Always > 0
  block->nominal_rate = CEIL(block->step_event_count * inverse_secs); // (step/sec) Always > 0

  #if ENABLED(FILAMENT_WIDTH_SENSOR)
    static float filwidth_e_count = 0, filwidth_delay_dist = 0;

    //FMM update ring buffer used for delay with filament measurements
    if (extruder == FILAMENT_SENSOR_EXTRUDER_NUM && filwidth_delay_index[1] >= 0) {  //only for extruder with filament sensor and if ring buffer is initialized

      constexpr int MMD_CM = MAX_MEASUREMENT_DELAY + 1, MMD_MM = MMD_CM * 10;

      // increment counters with next move in e axis
      filwidth_e_count += delta_mm[E_AXIS];
      filwidth_delay_dist += delta_mm[E_AXIS];

      // Only get new measurements on forward E movement
      if (!UNEAR_ZERO(filwidth_e_count)) {

        // Loop the delay distance counter (modulus by the mm length)
        while (filwidth_delay_dist >= MMD_MM) filwidth_delay_dist -= MMD_MM;

        // Convert into an index into the measurement array
        filwidth_delay_index[0] = int8_t(filwidth_delay_dist * 0.1);

        // If the index has changed (must have gone forward)...
        if (filwidth_delay_index[0] != filwidth_delay_index[1]) {
          filwidth_e_count = 0; // Reset the E movement counter
          const int8_t meas_sample = thermalManager.widthFil_to_size_ratio();
          do {
            filwidth_delay_index[1] = (filwidth_delay_index[1] + 1) % MMD_CM; // The next unused slot
            measurement_delay[filwidth_delay_index[1]] = meas_sample;         // Store the measurement
          } while (filwidth_delay_index[0] != filwidth_delay_index[1]);       // More slots to fill?
        }
      }
    }
  #endif

  // Calculate and limit speed in mm/sec for each axis
  float current_speed[NUM_AXIS], speed_factor = 1.0; // factor <1 decreases speed
  LOOP_XYZE(i) {
    const float cs = FABS((current_speed[i] = delta_mm[i] * inverse_secs));
    #if ENABLED(DISTINCT_E_FACTORS)
      if (i == E_AXIS) i += extruder;
    #endif
    if (cs > max_feedrate_mm_s[i]) NOMORE(speed_factor, max_feedrate_mm_s[i] / cs);
  }

  // Max segment time in µs.
  #ifdef XY_FREQUENCY_LIMIT

    // Check and limit the xy direction change frequency
    const unsigned char direction_change = block->direction_bits ^ old_direction_bits;
    old_direction_bits = block->direction_bits;
    segment_time_us = LROUND((float)segment_time_us / speed_factor);

    uint32_t xs0 = axis_segment_time_us[X_AXIS][0],
             xs1 = axis_segment_time_us[X_AXIS][1],
             xs2 = axis_segment_time_us[X_AXIS][2],
             ys0 = axis_segment_time_us[Y_AXIS][0],
             ys1 = axis_segment_time_us[Y_AXIS][1],
             ys2 = axis_segment_time_us[Y_AXIS][2];

    if (TEST(direction_change, X_AXIS)) {
      xs2 = axis_segment_time_us[X_AXIS][2] = xs1;
      xs1 = axis_segment_time_us[X_AXIS][1] = xs0;
      xs0 = 0;
    }
    xs0 = axis_segment_time_us[X_AXIS][0] = xs0 + segment_time_us;

    if (TEST(direction_change, Y_AXIS)) {
      ys2 = axis_segment_time_us[Y_AXIS][2] = axis_segment_time_us[Y_AXIS][1];
      ys1 = axis_segment_time_us[Y_AXIS][1] = axis_segment_time_us[Y_AXIS][0];
      ys0 = 0;
    }
    ys0 = axis_segment_time_us[Y_AXIS][0] = ys0 + segment_time_us;

    const uint32_t max_x_segment_time = MAX3(xs0, xs1, xs2),
                   max_y_segment_time = MAX3(ys0, ys1, ys2),
                   min_xy_segment_time = min(max_x_segment_time, max_y_segment_time);
    if (min_xy_segment_time < MAX_FREQ_TIME_US) {
      const float low_sf = speed_factor * min_xy_segment_time / (MAX_FREQ_TIME_US);
      NOMORE(speed_factor, low_sf);
    }
  #endif // XY_FREQUENCY_LIMIT

  // Correct the speed
  if (speed_factor < 1.0) {
    LOOP_XYZE(i) current_speed[i] *= speed_factor;
    block->nominal_speed *= speed_factor;
    block->nominal_rate *= speed_factor;
  }

  // Compute and limit the acceleration rate for the trapezoid generator.
  const float steps_per_mm = block->step_event_count * inverse_millimeters;
  uint32_t accel;
  if (!block->steps[A_AXIS] && !block->steps[B_AXIS] && !block->steps[C_AXIS]) {
    // convert to: acceleration steps/sec^2
    accel = CEIL(retract_acceleration * steps_per_mm);
    #if ENABLED(LIN_ADVANCE)
      block->use_advance_lead = false;
    #endif
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
    accel = CEIL((esteps ? acceleration : travel_acceleration) * steps_per_mm);

    #if ENABLED(LIN_ADVANCE)
      /**
       *
       * Use LIN_ADVANCE for blocks if all these are true:
       *
       * esteps             : This is a print move, because we checked for A, B, C steps before.
       *
       * extruder_advance_K : There is an advance factor set.
       *
       * de > 0             : Extruder is running forward (e.g., for "Wipe while retracting" (Slic3r) or "Combing" (Cura) moves)
       */
      block->use_advance_lead =  esteps
                              && extruder_advance_K
                              && de > 0;

      if (block->use_advance_lead) {
        block->e_D_ratio = (target_float[E_AXIS] - position_float[E_AXIS]) /
          #if IS_KINEMATIC
            block->millimeters
          #else
            SQRT(sq(target_float[X_AXIS] - position_float[X_AXIS])
               + sq(target_float[Y_AXIS] - position_float[Y_AXIS])
               + sq(target_float[Z_AXIS] - position_float[Z_AXIS]))
          #endif
        ;

        // Check for unusual high e_D ratio to detect if a retract move was combined with the last print move due to min. steps per segment. Never execute this with advance!
        // This assumes no one will use a retract length of 0mm < retr_length < ~0.2mm and no one will print 100mm wide lines using 3mm filament or 35mm wide lines using 1.75mm filament.
        if (block->e_D_ratio > 3.0)
          block->use_advance_lead = false;
        else {
          const uint32_t max_accel_steps_per_s2 = max_jerk[E_AXIS] / (extruder_advance_K * block->e_D_ratio) * steps_per_mm;
          #if ENABLED(LA_DEBUG)
            if (accel > max_accel_steps_per_s2)
              SERIAL_ECHOLNPGM("Acceleration limited.");
          #endif
          NOMORE(accel, max_accel_steps_per_s2);
        }
      }
    #endif

    #if ENABLED(DISTINCT_E_FACTORS)
      #define ACCEL_IDX extruder
    #else
      #define ACCEL_IDX 0
    #endif

    // Limit acceleration per axis
    if (block->step_event_count <= cutoff_long) {
      LIMIT_ACCEL_LONG(A_AXIS, 0);
      LIMIT_ACCEL_LONG(B_AXIS, 0);
      LIMIT_ACCEL_LONG(C_AXIS, 0);
      LIMIT_ACCEL_LONG(E_AXIS, ACCEL_IDX);
    }
    else {
      LIMIT_ACCEL_FLOAT(A_AXIS, 0);
      LIMIT_ACCEL_FLOAT(B_AXIS, 0);
      LIMIT_ACCEL_FLOAT(C_AXIS, 0);
      LIMIT_ACCEL_FLOAT(E_AXIS, ACCEL_IDX);
    }
  }
  block->acceleration_steps_per_s2 = accel;
  block->acceleration = accel / steps_per_mm;
  #if DISABLED(BEZIER_JERK_CONTROL)
    block->acceleration_rate = (long)(accel * (4096.0 * 4096.0 / (HAL_STEPPER_TIMER_RATE)));
  #endif
  #if ENABLED(LIN_ADVANCE)
    if (block->use_advance_lead) {
      block->advance_speed = (HAL_STEPPER_TIMER_RATE) / (extruder_advance_K * block->e_D_ratio * block->acceleration * axis_steps_per_mm[E_AXIS_N]);
      #if ENABLED(LA_DEBUG)
        if (extruder_advance_K * block->e_D_ratio * block->acceleration * 2 < block->nominal_speed * block->e_D_ratio)
          SERIAL_ECHOLNPGM("More than 2 steps per eISR loop executed.");
        if (block->advance_speed < 200)
          SERIAL_ECHOLNPGM("eISR running at > 10kHz.");
      #endif
    }
  #endif

  // Initial limit on the segment entry velocity
  float vmax_junction;

  #if 0  // Use old jerk for now

    float junction_deviation = 0.1;

    // Compute path unit vector
    double unit_vec[XYZ] = {
      delta_mm[A_AXIS] * inverse_millimeters,
      delta_mm[B_AXIS] * inverse_millimeters,
      delta_mm[C_AXIS] * inverse_millimeters
    };

    /*
       Compute maximum allowable entry speed at junction by centripetal acceleration approximation.

       Let a circle be tangent to both previous and current path line segments, where the junction
       deviation is defined as the distance from the junction to the closest edge of the circle,
       collinear with the circle center.

       The circular segment joining the two paths represents the path of centripetal acceleration.
       Solve for max velocity based on max acceleration about the radius of the circle, defined
       indirectly by junction deviation.

       This may be also viewed as path width or max_jerk in the previous grbl version. This approach
       does not actually deviate from path, but used as a robust way to compute cornering speeds, as
       it takes into account the nonlinearities of both the junction angle and junction velocity.
     */

    vmax_junction = MINIMUM_PLANNER_SPEED; // Set default max junction speed

    // Skip first block or when previous_nominal_speed is used as a flag for homing and offset cycles.
    if (moves_queued && !UNEAR_ZERO(previous_nominal_speed)) {
      // Compute cosine of angle between previous and current path. (prev_unit_vec is negative)
      // NOTE: Max junction velocity is computed without sin() or acos() by trig half angle identity.
      const float cos_theta = - previous_unit_vec[X_AXIS] * unit_vec[X_AXIS]
                              - previous_unit_vec[Y_AXIS] * unit_vec[Y_AXIS]
                              - previous_unit_vec[Z_AXIS] * unit_vec[Z_AXIS];
      // Skip and use default max junction speed for 0 degree acute junction.
      if (cos_theta < 0.95) {
        vmax_junction = min(previous_nominal_speed, block->nominal_speed);
        // Skip and avoid divide by zero for straight junctions at 180 degrees. Limit to min() of nominal speeds.
        if (cos_theta > -0.95) {
          // Compute maximum junction velocity based on maximum acceleration and junction deviation
          float sin_theta_d2 = SQRT(0.5 * (1.0 - cos_theta)); // Trig half angle identity. Always positive.
          NOMORE(vmax_junction, SQRT(block->acceleration * junction_deviation * sin_theta_d2 / (1.0 - sin_theta_d2)));
        }
      }
    }
  #endif

  /**
   * Adapted from Průša MKS firmware
   * https://github.com/prusa3d/Prusa-Firmware
   *
   * Start with a safe speed (from which the machine may halt to stop immediately).
   */

  // Exit speed limited by a jerk to full halt of a previous last segment
  static float previous_safe_speed;

  float safe_speed = block->nominal_speed;
  uint8_t limited = 0;
  LOOP_XYZE(i) {
    const float jerk = FABS(current_speed[i]), maxj = max_jerk[i];
    if (jerk > maxj) {
      if (limited) {
        const float mjerk = maxj * block->nominal_speed;
        if (jerk * safe_speed > mjerk) safe_speed = mjerk / jerk;
      }
      else {
        ++limited;
        safe_speed = maxj;
      }
    }
  }

  if (moves_queued && !UNEAR_ZERO(previous_nominal_speed)) {
    // Estimate a maximum velocity allowed at a joint of two successive segments.
    // If this maximum velocity allowed is lower than the minimum of the entry / exit safe velocities,
    // then the machine is not coasting anymore and the safe entry / exit velocities shall be used.

    // The junction velocity will be shared between successive segments. Limit the junction velocity to their minimum.
    // Pick the smaller of the nominal speeds. Higher speed shall not be achieved at the junction during coasting.
    vmax_junction = min(block->nominal_speed, previous_nominal_speed);

    // Factor to multiply the previous / current nominal velocities to get componentwise limited velocities.
    float v_factor = 1;
    limited = 0;

    // Now limit the jerk in all axes.
    const float smaller_speed_factor = vmax_junction / previous_nominal_speed;
    LOOP_XYZE(axis) {
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
            ( (v_entry > 0 || v_exit < 0) ? (v_exit - v_entry) : max(v_exit, -v_entry) )
          : // v_exit <= v_entry                coasting             axis reversal
            ( (v_entry < 0 || v_exit > 0) ? (v_entry - v_exit) : max(-v_exit, v_entry) );

      if (jerk > max_jerk[axis]) {
        v_factor *= max_jerk[axis] / jerk;
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

  // Max entry speed of this block equals the max exit speed of the previous block.
  block->max_entry_speed = vmax_junction;

  // Initialize block entry speed. Compute based on deceleration to user-defined MINIMUM_PLANNER_SPEED.
  const float v_allowable = max_allowable_speed(-block->acceleration, MINIMUM_PLANNER_SPEED, block->millimeters);
  // If stepper ISR is disabled, this indicates buffer_segment wants to add a split block.
  // In this case start with the max. allowed speed to avoid an interrupted first move.
  block->entry_speed = STEPPER_ISR_ENABLED() ? MINIMUM_PLANNER_SPEED : min(vmax_junction, v_allowable);

  // Initialize planner efficiency flags
  // Set flag if block will always reach maximum junction speed regardless of entry/exit speeds.
  // If a block can de/ac-celerate from nominal speed to zero within the length of the block, then
  // the current block and next block junction speeds are guaranteed to always be at their maximum
  // junction speeds in deceleration and acceleration, respectively. This is due to how the current
  // block nominal speed limits both the current and next maximum junction speeds. Hence, in both
  // the reverse and forward planners, the corresponding block junction speed will always be at the
  // the maximum junction speed and may always be ignored for any speed reduction checks.
  block->flag |= block->nominal_speed <= v_allowable ? BLOCK_FLAG_RECALCULATE | BLOCK_FLAG_NOMINAL_LENGTH : BLOCK_FLAG_RECALCULATE;

  // Update previous path unit_vector and nominal speed
  COPY(previous_speed, current_speed);
  previous_nominal_speed = block->nominal_speed;
  previous_safe_speed = safe_speed;

  // Move buffer head
  block_buffer_head = next_buffer_head;

  // Update the position (only when a move was queued)
  static_assert(COUNT(target) > 1, "Parameter to _buffer_steps must be (&target)[XYZE]!");
  COPY(position, target);
  #if HAS_POSITION_FLOAT
    COPY(position_float, target_float);
  #endif

  recalculate();

} // _buffer_steps()

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
void Planner::buffer_segment(const float &a, const float &b, const float &c, const float &e, const float &fr_mm_s, const uint8_t extruder, const float &millimeters/*=0.0*/) {
  // When changing extruders recalculate steps corresponding to the E position
  #if ENABLED(DISTINCT_E_FACTORS)
    if (last_extruder != extruder && axis_steps_per_mm[E_AXIS_N] != axis_steps_per_mm[E_AXIS + last_extruder]) {
      position[E_AXIS] = LROUND(position[E_AXIS] * axis_steps_per_mm[E_AXIS_N] * steps_to_mm[E_AXIS + last_extruder]);
      last_extruder = extruder;
    }
  #endif

  // The target position of the tool in absolute steps
  // Calculate target position in absolute steps
  const int32_t target[ABCE] = {
    LROUND(a * axis_steps_per_mm[A_AXIS]),
    LROUND(b * axis_steps_per_mm[B_AXIS]),
    LROUND(c * axis_steps_per_mm[C_AXIS]),
    LROUND(e * axis_steps_per_mm[E_AXIS_N])
  };

  #if HAS_POSITION_FLOAT
    const float target_float[XYZE] = { a, b, c, e };
  #endif

  // DRYRUN prevents E moves from taking place
  if (DEBUGGING(DRYRUN)) {
    position[E_AXIS] = target[E_AXIS];
    #if HAS_POSITION_FLOAT
      position_float[E_AXIS] = e;
    #endif
  }

  /* <-- add a slash to enable
    SERIAL_ECHOPAIR("  buffer_segment FR:", fr_mm_s);
    #if IS_KINEMATIC
      SERIAL_ECHOPAIR(" A:", a);
      SERIAL_ECHOPAIR(" (", position[A_AXIS]);
      SERIAL_ECHOPAIR("->", target[A_AXIS]);
      SERIAL_ECHOPAIR(") B:", b);
    #else
      SERIAL_ECHOPAIR(" X:", a);
      SERIAL_ECHOPAIR(" (", position[X_AXIS]);
      SERIAL_ECHOPAIR("->", target[X_AXIS]);
      SERIAL_ECHOPAIR(") Y:", b);
    #endif
    SERIAL_ECHOPAIR(" (", position[Y_AXIS]);
    SERIAL_ECHOPAIR("->", target[Y_AXIS]);
    #if ENABLED(DELTA)
      SERIAL_ECHOPAIR(") C:", c);
    #else
      SERIAL_ECHOPAIR(") Z:", c);
    #endif
    SERIAL_ECHOPAIR(" (", position[Z_AXIS]);
    SERIAL_ECHOPAIR("->", target[Z_AXIS]);
    SERIAL_ECHOPAIR(") E:", e);
    SERIAL_ECHOPAIR(" (", position[E_AXIS]);
    SERIAL_ECHOPAIR("->", target[E_AXIS]);
    SERIAL_ECHOLNPGM(")");
  //*/

  // Always split the first move into two (if not homing or probing)
  if (!has_blocks_queued()) {

    #define _BETWEEN(A) (position[A##_AXIS] + target[A##_AXIS]) >> 1
    const int32_t between[ABCE] = { _BETWEEN(A), _BETWEEN(B), _BETWEEN(C), _BETWEEN(E) };

    #if HAS_POSITION_FLOAT
      #define _BETWEEN_F(A) (position_float[A##_AXIS] + target_float[A##_AXIS]) * 0.5
      const float between_float[ABCE] = { _BETWEEN_F(A), _BETWEEN_F(B), _BETWEEN_F(C), _BETWEEN_F(E) };
    #endif

    DISABLE_STEPPER_DRIVER_INTERRUPT();

    _buffer_steps(between
      #if HAS_POSITION_FLOAT
        , between_float
      #endif
      , fr_mm_s, extruder, millimeters * 0.5
    );

    const uint8_t next = block_buffer_head;

    _buffer_steps(target
      #if HAS_POSITION_FLOAT
        , target_float
      #endif
      , fr_mm_s, extruder, millimeters * 0.5
    );

    SBI(block_buffer[next].flag, BLOCK_BIT_CONTINUED);
    ENABLE_STEPPER_DRIVER_INTERRUPT();
  }
  else
    _buffer_steps(target
      #if HAS_POSITION_FLOAT
        , target_float
      #endif
      , fr_mm_s, extruder, millimeters
    );

  stepper.wake_up();

} // buffer_segment()

/**
 * Directly set the planner XYZ position (and stepper positions)
 * converting mm (or angles for SCARA) into steps.
 *
 * On CORE machines stepper ABC will be translated from the given XYZ.
 */

void Planner::_set_position_mm(const float &a, const float &b, const float &c, const float &e) {
  #if ENABLED(DISTINCT_E_FACTORS)
    #define _EINDEX (E_AXIS + active_extruder)
    last_extruder = active_extruder;
  #else
    #define _EINDEX E_AXIS
  #endif
  const int32_t na = position[A_AXIS] = LROUND(a * axis_steps_per_mm[A_AXIS]),
                nb = position[B_AXIS] = LROUND(b * axis_steps_per_mm[B_AXIS]),
                nc = position[C_AXIS] = LROUND(c * axis_steps_per_mm[C_AXIS]),
                ne = position[E_AXIS] = LROUND(e * axis_steps_per_mm[_EINDEX]);
  #if HAS_POSITION_FLOAT
    position_float[X_AXIS] = a;
    position_float[Y_AXIS] = b;
    position_float[Z_AXIS] = c;
    position_float[E_AXIS] = e;
  #endif
  stepper.set_position(na, nb, nc, ne);
  previous_nominal_speed = 0.0; // Resets planner junction speeds. Assumes start from rest.
  ZERO(previous_speed);
}

void Planner::set_position_mm_kinematic(const float (&cart)[XYZE]) {
  #if PLANNER_LEVELING
    float raw[XYZ] = { cart[X_AXIS], cart[Y_AXIS], cart[Z_AXIS] };
    apply_leveling(raw);
  #else
    const float (&raw)[XYZE] = cart;
  #endif
  #if IS_KINEMATIC
    inverse_kinematics(raw);
    _set_position_mm(delta[A_AXIS], delta[B_AXIS], delta[C_AXIS], cart[E_AXIS]);
  #else
    _set_position_mm(raw[X_AXIS], raw[Y_AXIS], raw[Z_AXIS], cart[E_AXIS]);
  #endif
}

/**
 * Sync from the stepper positions. (e.g., after an interrupted move)
 */
void Planner::sync_from_steppers() {
  LOOP_XYZE(i) {
    position[i] = stepper.position((AxisEnum)i);
    #if HAS_POSITION_FLOAT
      position_float[i] = position[i] * steps_to_mm[i
        #if ENABLED(DISTINCT_E_FACTORS)
          + (i == E_AXIS ? active_extruder : 0)
        #endif
      ];
    #endif
  }
}

/**
 * Setters for planner position (also setting stepper position).
 */
void Planner::set_position_mm(const AxisEnum axis, const float &v) {
  #if ENABLED(DISTINCT_E_FACTORS)
    const uint8_t axis_index = axis + (axis == E_AXIS ? active_extruder : 0);
    last_extruder = active_extruder;
  #else
    const uint8_t axis_index = axis;
  #endif
  position[axis] = LROUND(v * axis_steps_per_mm[axis_index]);
  #if HAS_POSITION_FLOAT
    position_float[axis] = v;
  #endif
  stepper.set_position(axis, v);
  previous_speed[axis] = 0.0;
}

// Recalculate the steps/s^2 acceleration rates, based on the mm/s^2
void Planner::reset_acceleration_rates() {
  #if ENABLED(DISTINCT_E_FACTORS)
    #define HIGHEST_CONDITION (i < E_AXIS || i == E_AXIS + active_extruder)
  #else
    #define HIGHEST_CONDITION true
  #endif
  uint32_t highest_rate = 1;
  LOOP_XYZE_N(i) {
    max_acceleration_steps_per_s2[i] = max_acceleration_mm_per_s2[i] * axis_steps_per_mm[i];
    if (HIGHEST_CONDITION) NOLESS(highest_rate, max_acceleration_steps_per_s2[i]);
  }
  cutoff_long = 4294967295UL / highest_rate;
}

// Recalculate position, steps_to_mm if axis_steps_per_mm changes!
void Planner::refresh_positioning() {
  LOOP_XYZE_N(i) steps_to_mm[i] = 1.0 / axis_steps_per_mm[i];
  set_position_mm_kinematic(current_position);
  reset_acceleration_rates();
}

#if ENABLED(AUTOTEMP)

  void Planner::autotemp_M104_M109() {
    if ((autotemp_enabled = parser.seen('F'))) autotemp_factor = parser.value_float();
    if (parser.seen('S')) autotemp_min = parser.value_celsius();
    if (parser.seen('B')) autotemp_max = parser.value_celsius();
  }

#endif
