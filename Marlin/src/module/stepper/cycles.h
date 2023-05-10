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

/**
 * cycles.h - Cycle counting for the Stepper ISR
 *
 * Estimate the amount of time the Stepper ISR will take to execute.
 *
 * These cycle counts are rough estimates used to determine whether the ISR
 * has enough time to do all its work before it should yield back to userland.
 * These constants may be updated as data is gathered from a variety of MCUs.
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
  #if HAS_ZV_SHAPING
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
  #define ISR_BASE_CYCLES  882UL

  // Linear advance base time is 32 cycles
  #if ENABLED(LIN_ADVANCE)
    #define ISR_LA_BASE_CYCLES 30UL
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
  #if HAS_ZV_SHAPING
    #define ISR_SHAPING_BASE_CYCLES 290UL
  #else
    #define ISR_SHAPING_BASE_CYCLES 0UL
  #endif

  // Stepper Loop base cycles
  #define ISR_LOOP_BASE_CYCLES 32UL

  // And each stepper (start + stop pulse) takes in worst case
  #define ISR_STEPPER_CYCLES 60UL

#endif

// If linear advance is disabled, the loop also handles them
#if DISABLED(LIN_ADVANCE) && ENABLED(MIXING_EXTRUDER)
  #define ISR_MIXING_STEPPER_CYCLES ((MIXING_STEPPERS) * (ISR_STEPPER_CYCLES))
#else
  #define ISR_MIXING_STEPPER_CYCLES  0UL
#endif

// Add time for each stepper
#if HAS_X_STEP
  #define ISR_X_STEPPER_CYCLES ISR_STEPPER_CYCLES
#endif
#if HAS_Y_STEP
  #define ISR_Y_STEPPER_CYCLES ISR_STEPPER_CYCLES
#endif
#if HAS_Z_STEP
  #define ISR_Z_STEPPER_CYCLES ISR_STEPPER_CYCLES
#endif
#if HAS_I_STEP
  #define ISR_I_STEPPER_CYCLES ISR_STEPPER_CYCLES
#endif
#if HAS_J_STEP
  #define ISR_J_STEPPER_CYCLES ISR_STEPPER_CYCLES
#endif
#if HAS_K_STEP
  #define ISR_K_STEPPER_CYCLES ISR_STEPPER_CYCLES
#endif
#if HAS_U_STEP
  #define ISR_U_STEPPER_CYCLES ISR_STEPPER_CYCLES
#endif
#if HAS_V_STEP
  #define ISR_V_STEPPER_CYCLES ISR_STEPPER_CYCLES
#endif
#if HAS_W_STEP
  #define ISR_W_STEPPER_CYCLES ISR_STEPPER_CYCLES
#endif
#if HAS_EXTRUDERS
  #define ISR_E_STEPPER_CYCLES ISR_STEPPER_CYCLES // E is always interpolated, even for mixing extruders
#endif

// And the total minimum loop time, not including the base
#define _PLUS_AXIS_CYCLES(A) + (ISR_##A##_STEPPER_CYCLES)
#define MIN_ISR_LOOP_CYCLES (ISR_MIXING_STEPPER_CYCLES LOGICAL_AXIS_MAP(_PLUS_AXIS_CYCLES))

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

// The loop takes the base time plus the time for all the bresenham logic for 1 << R pulses plus the time
// between pulses for ((1 << R) - 1) pulses. But the user could be enforcing a minimum time so the loop time is:
#define ISR_LOOP_CYCLES(R) ((ISR_LOOP_BASE_CYCLES + MIN_ISR_LOOP_CYCLES + MIN_STEPPER_PULSE_CYCLES) * ((1UL << R) - 1) + _MAX(MIN_ISR_LOOP_CYCLES, MIN_STEPPER_PULSE_CYCLES))

// Model input shaping as an extra loop call
#define ISR_SHAPING_LOOP_CYCLES(R) (TERN0(HAS_ZV_SHAPING, (ISR_LOOP_BASE_CYCLES + TERN0(INPUT_SHAPING_X, ISR_X_STEPPER_CYCLES) + TERN0(INPUT_SHAPING_Y, ISR_Y_STEPPER_CYCLES)) << R))

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

// Estimate the total ISR execution time in cycles given a step-per-ISR shift multiplier
#define ISR_EXECUTION_CYCLES(R) ((ISR_BASE_CYCLES + ISR_S_CURVE_CYCLES + ISR_SHAPING_BASE_CYCLES + ISR_LOOP_CYCLES(R) + ISR_SHAPING_LOOP_CYCLES(R) + ISR_LA_BASE_CYCLES + ISR_LA_LOOP_CYCLES) >> R)

// The maximum allowable stepping frequency when doing 1x stepping (in Hz)
#define MAX_STEP_ISR_FREQUENCY_1X ((F_CPU) / ISR_EXECUTION_CYCLES(0))

// The minimum step ISR rate used by ADAPTIVE_STEP_SMOOTHING to target 50% CPU usage
// This does not account for the possibility of multi-stepping.
#define MIN_STEP_ISR_FREQUENCY (MAX_STEP_ISR_FREQUENCY_1X >> 1)
