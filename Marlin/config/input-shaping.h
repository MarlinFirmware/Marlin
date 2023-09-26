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
 * Input Shaping -- EXPERIMENTAL
 *
 * Zero Vibration (ZV) Input Shaping for X and/or Y movements.
 *
 * This option uses a lot of SRAM for the step buffer. The buffer size is
 * calculated automatically from SHAPING_FREQ_[XY], DEFAULT_AXIS_STEPS_PER_UNIT,
 * DEFAULT_MAX_FEEDRATE and ADAPTIVE_STEP_SMOOTHING. The default calculation can
 * be overridden by setting SHAPING_MIN_FREQ and/or SHAPING_MAX_FEEDRATE.
 * The higher the frequency and the lower the feedrate, the smaller the buffer.
 * If the buffer is too small at runtime, input shaping will have reduced
 * effectiveness during high speed movements.
 *
 * Tune with M593 D<factor> F<frequency>:
 *
 *  D<factor>    Set the zeta/damping factor. If axes (X, Y, etc.) are not specified, set for all axes.
 *  F<frequency> Set the frequency. If axes (X, Y, etc.) are not specified, set for all axes.
 *  T[map]       Input Shaping type, 0:ZV, 1:EI, 2:2H EI (not implemented yet)
 *  X<1>         Set the given parameters only for the X axis.
 *  Y<1>         Set the given parameters only for the Y axis.
 */
//#define INPUT_SHAPING_X
//#define INPUT_SHAPING_Y
#if ANY(INPUT_SHAPING_X, INPUT_SHAPING_Y)
  #if ENABLED(INPUT_SHAPING_X)
    #define SHAPING_FREQ_X  40          // (Hz) The default dominant resonant frequency on the X axis.
    #define SHAPING_ZETA_X  0.15f       // Damping ratio of the X axis (range: 0.0 = no damping to 1.0 = critical damping).
  #endif
  #if ENABLED(INPUT_SHAPING_Y)
    #define SHAPING_FREQ_Y  40          // (Hz) The default dominant resonant frequency on the Y axis.
    #define SHAPING_ZETA_Y  0.15f       // Damping ratio of the Y axis (range: 0.0 = no damping to 1.0 = critical damping).
  #endif
  //#define SHAPING_MIN_FREQ  20        // By default the minimum of the shaping frequencies. Override to affect SRAM usage.
  //#define SHAPING_MAX_STEPRATE 10000  // By default the maximum total step rate of the shaped axes. Override to affect SRAM usage.
  //#define SHAPING_MENU                // Add a menu to the LCD to set shaping parameters.
#endif
