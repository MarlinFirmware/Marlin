/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2024 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * Conditionals-6-type.h
 * Internal defines that depend on Configurations and Pins but are not user-editable.
 * Define conditionals in this file if they depend on core/types.h.
 */

//========================================================
// Get requirements for the benefit of IntelliSense, etc.
//
#include "MarlinConfigPre-6-type.h"
//========================================================

#ifdef GITHUB_ACTIONS
  // Extras for CI testing
#endif

// If an axis's Homing Current differs from standard current...
#define HAS_HOME_CURRENT(N) TERN0(EDITABLE_HOMING_CURRENT, N##_IS_TRINAMIC && N##_HOME_DIR != 0) || (N##_CURRENT_HOME > 0 && N##_CURRENT_HOME != N##_CURRENT)
#if HAS_HOME_CURRENT(X)
  #define X_HAS_HOME_CURRENT 1
#endif
#if HAS_HOME_CURRENT(Y)
  #define Y_HAS_HOME_CURRENT 1
#endif
#if HAS_HOME_CURRENT(Z)
  #define Z_HAS_HOME_CURRENT 1
#endif
#if HAS_HOME_CURRENT(I)
  #define I_HAS_HOME_CURRENT 1
#endif
#if HAS_HOME_CURRENT(J)
  #define J_HAS_HOME_CURRENT 1
#endif
#if HAS_HOME_CURRENT(K)
  #define K_HAS_HOME_CURRENT 1
#endif
#if HAS_HOME_CURRENT(U)
  #define U_HAS_HOME_CURRENT 1
#endif
#if HAS_HOME_CURRENT(V)
  #define V_HAS_HOME_CURRENT 1
#endif
#if HAS_HOME_CURRENT(W)
  #define W_HAS_HOME_CURRENT 1
#endif
#if HAS_HOME_CURRENT(X2)
  #define X2_HAS_HOME_CURRENT 1
#endif
#if HAS_HOME_CURRENT(Y2)
  #define Y2_HAS_HOME_CURRENT 1
#endif
#if HAS_HOME_CURRENT(Z2)
  #define Z2_HAS_HOME_CURRENT 1
#endif
#if HAS_HOME_CURRENT(Z3)
  #define Z3_HAS_HOME_CURRENT 1
#endif
#if HAS_HOME_CURRENT(Z4)
  #define Z4_HAS_HOME_CURRENT 1
#endif
#undef HAS_HOME_CURRENT

// Does any axis have homing current?
#define _OR_HAS_CURR_HOME(N) N##_HAS_HOME_CURRENT ||
#if MAIN_AXIS_MAP(_OR_HAS_CURR_HOME) MAP(_OR_HAS_CURR_HOME, X2, Y2, Z2, Z3, Z4) 0
  #define HAS_HOMING_CURRENT 1
#endif
#undef _OR_HAS_CURR_HOME
