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
CONFIG_VERSION(02020000)

/**
 * Fixed-time-based Motion Control -- EXPERIMENTAL
 * Enable/disable and set parameters with G-code M493.
 */
//#define FT_MOTION
#if ENABLED(FT_MOTION)
  #define FTM_DEFAULT_MODE        ftMotionMode_DISABLED // Default mode of fixed time control. (Enums in ft_types.h)
  #define FTM_DEFAULT_DYNFREQ_MODE dynFreqMode_DISABLED // Default mode of dynamic frequency calculation. (Enums in ft_types.h)
  #define FTM_SHAPING_DEFAULT_X_FREQ 37.0f              // (Hz) Default peak frequency used by input shapers.
  #define FTM_SHAPING_DEFAULT_Y_FREQ 37.0f              // (Hz) Default peak frequency used by input shapers.
  #define FTM_LINEAR_ADV_DEFAULT_ENA false              // Default linear advance enable (true) or disable (false).
  #define FTM_LINEAR_ADV_DEFAULT_K    0.0f              // Default linear advance gain.
  #define FTM_SHAPING_ZETA            0.1f              // Zeta used by input shapers.
  #define FTM_SHAPING_V_TOL           0.05f             // Vibration tolerance used by EI input shapers.

  /**
   * Advanced configuration
   */
  #define FTM_BATCH_SIZE            100                 // Batch size for trajectory generation;
  #define FTM_WINDOW_SIZE           200                 // Window size for trajectory generation.
  #define FTM_FS                   1000                 // (Hz) Frequency for trajectory generation. (1 / FTM_TS)
  #define FTM_TS                      0.001f            // (s) Time step for trajectory generation. (1 / FTM_FS)
  #define FTM_STEPPER_FS          20000                 // (Hz) Frequency for stepper I/O update.
  #define FTM_MIN_TICKS ((STEPPER_TIMER_RATE) / (FTM_STEPPER_FS)) // Minimum stepper ticks between steps.
  #define FTM_MIN_SHAPE_FREQ         10                 // Minimum shaping frequency.
  #define FTM_ZMAX                  100                 // Maximum delays for shaping functions (even numbers only!).
                                                        // Calculate as:
                                                        //    1/2 * (FTM_FS / FTM_MIN_SHAPE_FREQ) for ZV.
                                                        //    (FTM_FS / FTM_MIN_SHAPE_FREQ) for ZVD, MZV.
                                                        //    3/2 * (FTM_FS / FTM_MIN_SHAPE_FREQ) for 2HEI.
                                                        //    2 * (FTM_FS / FTM_MIN_SHAPE_FREQ) for 3HEI.
  #define FTM_STEPS_PER_UNIT_TIME    20                 // Interpolated stepper commands per unit time.
                                                        // Calculate as (FTM_STEPPER_FS / FTM_FS).
  #define FTM_CTS_COMPARE_VAL        10                 // Comparison value used in interpolation algorithm.
                                                        // Calculate as (FTM_STEPS_PER_UNIT_TIME / 2).
  // These values may be configured to adjust duration of loop().
  #define FTM_STEPS_PER_LOOP         60                 // Number of stepper commands to generate each loop().
  #define FTM_POINTS_PER_LOOP       100                 // Number of trajectory points to generate each loop().

  // This value may be configured to adjust duration to consume the command buffer.
  // Try increasing this value if stepper motion is not smooth.
  #define FTM_STEPPERCMD_BUFF_SIZE 1000                 // Size of the stepper command buffers.

  //#define FT_MOTION_MENU                              // Provide a MarlinUI menu to set M493 parameters.
#endif
