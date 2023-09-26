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
 * Multi-Stepper / Multi-Endstop
 *
 * When X2_DRIVER_TYPE is defined, this indicates that the X and X2 motors work in tandem.
 * The following explanations for X also apply to Y and Z multi-stepper setups.
 * Endstop offsets may be changed by 'M666 X<offset> Y<offset> Z<offset>' and stored to EEPROM.
 *
 * - Enable INVERT_X2_VS_X_DIR if the X2 motor requires an opposite DIR signal from X.
 *
 * - Enable X_DUAL_ENDSTOPS if the second motor has its own endstop, with adjustable offset.
 *
 *   - Extra endstops are included in the output of 'M119'.
 *
 *   - Set X_DUAL_ENDSTOP_ADJUSTMENT to the known error in the X2 endstop.
 *     Applied to the X2 motor on 'G28' / 'G28 X'.
 *     Get the offset by homing X and measuring the error.
 *     Also set with 'M666 X<offset>' and stored to EEPROM with 'M500'.
 *
 *   - Define the extra endstop pins here to override defaults. No auto-assignment.
 */
#if HAS_X2_STEPPER && DISABLED(DUAL_X_CARRIAGE)
  //#define INVERT_X2_VS_X_DIR        // X2 direction signal is the opposite of X
  //#define X_DUAL_ENDSTOPS           // X2 has its own endstop
  #if ENABLED(X_DUAL_ENDSTOPS)
    //#define X2_STOP_PIN X_MAX_PIN   // X2 endstop pin override
    #define X2_ENDSTOP_ADJUSTMENT  0  // X2 offset relative to X endstop
  #endif
#endif

#if HAS_Y2_STEPPER
  //#define INVERT_Y2_VS_Y_DIR        // Y2 direction signal is the opposite of Y
  //#define Y_DUAL_ENDSTOPS           // Y2 has its own endstop
  #if ENABLED(Y_DUAL_ENDSTOPS)
    //#define Y2_STOP_PIN Y_MAX_PIN   // Y2 endstop pin override
    #define Y2_ENDSTOP_ADJUSTMENT  0  // Y2 offset relative to Y endstop
  #endif
#endif

//
// Multi-Z steppers
//
#ifdef Z2_DRIVER_TYPE
  //#define INVERT_Z2_VS_Z_DIR        // Z2 direction signal is the opposite of Z

  //#define Z_MULTI_ENDSTOPS          // Other Z axes have their own endstops
  #if ENABLED(Z_MULTI_ENDSTOPS)
    //#define Z2_STOP_PIN X_MAX_PIN   // Z2 endstop pin override
    #define Z2_ENDSTOP_ADJUSTMENT 0   // Z2 offset relative to Z endstop
  #endif
  #ifdef Z3_DRIVER_TYPE
    //#define INVERT_Z3_VS_Z_DIR      // Z3 direction signal is the opposite of Z
    #if ENABLED(Z_MULTI_ENDSTOPS)
      //#define Z3_STOP_PIN Y_MAX_PIN // Z3 endstop pin override
      #define Z3_ENDSTOP_ADJUSTMENT 0 // Z3 offset relative to Z endstop
    #endif
  #endif
  #ifdef Z4_DRIVER_TYPE
    //#define INVERT_Z4_VS_Z_DIR      // Z4 direction signal is the opposite of Z
    #if ENABLED(Z_MULTI_ENDSTOPS)
      //#define Z4_STOP_PIN Z_MAX_PIN // Z4 endstop pin override
      #define Z4_ENDSTOP_ADJUSTMENT 0 // Z4 offset relative to Z endstop
    #endif
  #endif
#endif
