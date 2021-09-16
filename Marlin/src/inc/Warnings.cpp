/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * Warnings.cpp
 * Test configuration values and give warnings at compile-time.
 */

#include "MarlinConfig.h"

//
// Warnings! Located here so they will appear just once in the build output.
//

#if ENABLED(MARLIN_DEV_MODE)
  #warning "WARNING! Disable MARLIN_DEV_MODE for the final build!"
#endif

#if NONE(HAS_RESUME_CONTINUE, HOST_PROMPT_SUPPORT)
  #warning "Your Configuration provides no method to acquire user feedback!"
#endif

#if AUTO_ASSIGNED_X2_STEPPER
  #warning "Auto-assigned X2 STEP/DIR/ENABLE_PINs to unused En_STEP/DIR/ENABLE_PINs."
#endif
#if AUTO_ASSIGNED_X2_MS1
  #warning "Auto-assigned X2_MS1_PIN to an unused En_MS1_PIN."
#endif
#if AUTO_ASSIGNED_X2_MS2
  #warning "Auto-assigned X2_MS2_PIN to an unused En_MS2_PIN."
#endif
#if AUTO_ASSIGNED_X2_MS3
  #warning "Auto-assigned X2_MS3_PIN to an unused En_MS3_PIN."
#endif
#if AUTO_ASSIGNED_X2_CS
  #warning "Auto-assigned X2_CS_PIN to an unused En_CS_PIN."
#endif
#if AUTO_ASSIGNED_X2_DIAG
  #if X2_USE_ENDSTOP == _XMIN_
    #warning "Auto-assigned X2_DIAG_PIN to X_MIN_PIN."
  #elif X2_USE_ENDSTOP == _XMAX_
    #warning "Auto-assigned X2_DIAG_PIN to X_MAX_PIN."
  #elif X2_USE_ENDSTOP == _YMIN_
    #warning "Auto-assigned X2_DIAG_PIN to Y_MIN_PIN."
  #elif X2_USE_ENDSTOP == _YMAX_
    #warning "Auto-assigned X2_DIAG_PIN to Y_MAX_PIN."
  #elif X2_USE_ENDSTOP == _ZMIN_
    #warning "Auto-assigned X2_DIAG_PIN to Z_MIN_PIN."
  #elif X2_USE_ENDSTOP == _ZMAX_
    #warning "Auto-assigned X2_DIAG_PIN to Z_MAX_PIN."
  #elif X2_USE_ENDSTOP == _XDIAG_
    #warning "Auto-assigned X2_DIAG_PIN to X_DIAG_PIN."
  #elif X2_USE_ENDSTOP == _YDIAG_
    #warning "Auto-assigned X2_DIAG_PIN to Y_DIAG_PIN."
  #elif X2_USE_ENDSTOP == _ZDIAG_
    #warning "Auto-assigned X2_DIAG_PIN to Z_DIAG_PIN."
  #elif X2_USE_ENDSTOP == _E0DIAG_
    #warning "Auto-assigned X2_DIAG_PIN to E0_DIAG_PIN."
  #elif X2_USE_ENDSTOP == _E1DIAG_
    #warning "Auto-assigned X2_DIAG_PIN to E1_DIAG_PIN."
  #elif X2_USE_ENDSTOP == _E2DIAG_
    #warning "Auto-assigned X2_DIAG_PIN to E2_DIAG_PIN."
  #elif X2_USE_ENDSTOP == _E3DIAG_
    #warning "Auto-assigned X2_DIAG_PIN to E3_DIAG_PIN."
  #elif X2_USE_ENDSTOP == _E4DIAG_
    #warning "Auto-assigned X2_DIAG_PIN to E4_DIAG_PIN."
  #elif X2_USE_ENDSTOP == _E5DIAG_
    #warning "Auto-assigned X2_DIAG_PIN to E5_DIAG_PIN."
  #elif X2_USE_ENDSTOP == _E6DIAG_
    #warning "Auto-assigned X2_DIAG_PIN to E6_DIAG_PIN."
  #elif X2_USE_ENDSTOP == _E7DIAG_
    #warning "Auto-assigned X2_DIAG_PIN to E7_DIAG_PIN."
  #endif
#endif
#if AUTO_ASSIGNED_Y2_STEPPER
  #warning "Auto-assigned Y2 STEP/DIR/ENABLE_PINs to unused En_STEP/DIR/ENABLE_PINs."
#endif
#if AUTO_ASSIGNED_Y2_MS1
  #warning "Auto-assigned Y2_MS1_PIN to an unused En_MS1_PIN."
#endif
#if AUTO_ASSIGNED_Y2_MS2
  #warning "Auto-assigned Y2_MS2_PIN to an unused En_MS2_PIN."
#endif
#if AUTO_ASSIGNED_Y2_MS3
  #warning "Auto-assigned Y2_MS3_PIN to an unused En_MS3_PIN."
#endif
#if AUTO_ASSIGNED_Y2_CS
  #warning "Auto-assigned Y2_CS_PIN to an unused En_CS_PIN."
#endif
#if AUTO_ASSIGNED_Y2_DIAG
  #if Y2_USE_ENDSTOP == _XMIN_
    #warning "Auto-assigned Y2_DIAG_PIN to X_MIN_PIN."
  #elif Y2_USE_ENDSTOP == _XMAX_
    #warning "Auto-assigned Y2_DIAG_PIN to X_MAX_PIN."
  #elif Y2_USE_ENDSTOP == _YMIN_
    #warning "Auto-assigned Y2_DIAG_PIN to Y_MIN_PIN."
  #elif Y2_USE_ENDSTOP == _YMAX_
    #warning "Auto-assigned Y2_DIAG_PIN to Y_MAX_PIN."
  #elif Y2_USE_ENDSTOP == _ZMIN_
    #warning "Auto-assigned Y2_DIAG_PIN to Z_MIN_PIN."
  #elif Y2_USE_ENDSTOP == _ZMAX_
    #warning "Auto-assigned Y2_DIAG_PIN to Z_MAX_PIN."
  #elif Y2_USE_ENDSTOP == _XDIAG_
    #warning "Auto-assigned Y2_DIAG_PIN to X_DIAG_PIN."
  #elif Y2_USE_ENDSTOP == _YDIAG_
    #warning "Auto-assigned Y2_DIAG_PIN to Y_DIAG_PIN."
  #elif Y2_USE_ENDSTOP == _ZDIAG_
    #warning "Auto-assigned Y2_DIAG_PIN to Z_DIAG_PIN."
  #elif Y2_USE_ENDSTOP == _E0DIAG_
    #warning "Auto-assigned Y2_DIAG_PIN to E0_DIAG_PIN."
  #elif Y2_USE_ENDSTOP == _E1DIAG_
    #warning "Auto-assigned Y2_DIAG_PIN to E1_DIAG_PIN."
  #elif Y2_USE_ENDSTOP == _E2DIAG_
    #warning "Auto-assigned Y2_DIAG_PIN to E2_DIAG_PIN."
  #elif Y2_USE_ENDSTOP == _E3DIAG_
    #warning "Auto-assigned Y2_DIAG_PIN to E3_DIAG_PIN."
  #elif Y2_USE_ENDSTOP == _E4DIAG_
    #warning "Auto-assigned Y2_DIAG_PIN to E4_DIAG_PIN."
  #elif Y2_USE_ENDSTOP == _E5DIAG_
    #warning "Auto-assigned Y2_DIAG_PIN to E5_DIAG_PIN."
  #elif Y2_USE_ENDSTOP == _E6DIAG_
    #warning "Auto-assigned Y2_DIAG_PIN to E6_DIAG_PIN."
  #elif Y2_USE_ENDSTOP == _E7DIAG_
    #warning "Auto-assigned Y2_DIAG_PIN to E7_DIAG_PIN."
  #endif
#endif
#if AUTO_ASSIGNED_Z2_STEPPER
  #warning "Auto-assigned Z2 STEP/DIR/ENABLE_PINs to unused En_STEP/DIR/ENABLE_PINs."
#endif
#if AUTO_ASSIGNED_Z2_MS1
  #warning "Auto-assigned Z2_MS1_PIN to an unused En_MS1_PIN."
#endif
#if AUTO_ASSIGNED_Z2_MS2
  #warning "Auto-assigned Z2_MS2_PIN to an unused En_MS2_PIN."
#endif
#if AUTO_ASSIGNED_Z2_MS3
  #warning "Auto-assigned Z2_MS3_PIN to an unused En_MS3_PIN."
#endif
#if AUTO_ASSIGNED_Z2_CS
  #warning "Auto-assigned Z2_CS_PIN to an unused En_CS_PIN."
#endif
#if AUTO_ASSIGNED_Z2_DIAG
  #if Z2_USE_ENDSTOP == _XMIN_
    #warning "Auto-assigned Z2_DIAG_PIN to X_MIN_PIN."
  #elif Z2_USE_ENDSTOP == _XMAX_
    #warning "Auto-assigned Z2_DIAG_PIN to X_MAX_PIN."
  #elif Z2_USE_ENDSTOP == _YMIN_
    #warning "Auto-assigned Z2_DIAG_PIN to Y_MIN_PIN."
  #elif Z2_USE_ENDSTOP == _YMAX_
    #warning "Auto-assigned Z2_DIAG_PIN to Y_MAX_PIN."
  #elif Z2_USE_ENDSTOP == _ZMIN_
    #warning "Auto-assigned Z2_DIAG_PIN to Z_MIN_PIN."
  #elif Z2_USE_ENDSTOP == _ZMAX_
    #warning "Auto-assigned Z2_DIAG_PIN to Z_MAX_PIN."
  #elif Z2_USE_ENDSTOP == _XDIAG_
    #warning "Auto-assigned Z2_DIAG_PIN to X_DIAG_PIN."
  #elif Z2_USE_ENDSTOP == _YDIAG_
    #warning "Auto-assigned Z2_DIAG_PIN to Y_DIAG_PIN."
  #elif Z2_USE_ENDSTOP == _ZDIAG_
    #warning "Auto-assigned Z2_DIAG_PIN to Z_DIAG_PIN."
  #elif Z2_USE_ENDSTOP == _E0DIAG_
    #warning "Auto-assigned Z2_DIAG_PIN to E0_DIAG_PIN."
  #elif Z2_USE_ENDSTOP == _E1DIAG_
    #warning "Auto-assigned Z2_DIAG_PIN to E1_DIAG_PIN."
  #elif Z2_USE_ENDSTOP == _E2DIAG_
    #warning "Auto-assigned Z2_DIAG_PIN to E2_DIAG_PIN."
  #elif Z2_USE_ENDSTOP == _E3DIAG_
    #warning "Auto-assigned Z2_DIAG_PIN to E3_DIAG_PIN."
  #elif Z2_USE_ENDSTOP == _E4DIAG_
    #warning "Auto-assigned Z2_DIAG_PIN to E4_DIAG_PIN."
  #elif Z2_USE_ENDSTOP == _E5DIAG_
    #warning "Auto-assigned Z2_DIAG_PIN to E5_DIAG_PIN."
  #elif Z2_USE_ENDSTOP == _E6DIAG_
    #warning "Auto-assigned Z2_DIAG_PIN to E6_DIAG_PIN."
  #elif Z2_USE_ENDSTOP == _E7DIAG_
    #warning "Auto-assigned Z2_DIAG_PIN to E7_DIAG_PIN."
  #endif
#endif
#if AUTO_ASSIGNED_Z3_STEPPER
  #warning "Auto-assigned Z3 STEP/DIR/ENABLE_PINs to unused En_STEP/DIR/ENABLE_PINs."
#endif
#if AUTO_ASSIGNED_Z3_CS
  #warning "Auto-assigned Z3_CS_PIN to an unused En_CS_PIN."
#endif
#if AUTO_ASSIGNED_Z3_MS1
  #warning "Auto-assigned Z3_MS1_PIN to an unused En_MS1_PIN."
#endif
#if AUTO_ASSIGNED_Z3_MS2
  #warning "Auto-assigned Z3_MS2_PIN to an unused En_MS2_PIN."
#endif
#if AUTO_ASSIGNED_Z3_MS3
  #warning "Auto-assigned Z3_MS3_PIN to an unused En_MS3_PIN."
#endif
#if AUTO_ASSIGNED_Z3_DIAG
  #if Z3_USE_ENDSTOP == _XMIN_
    #warning "Auto-assigned Z3_DIAG_PIN to X_MIN_PIN."
  #elif Z3_USE_ENDSTOP == _XMAX_
    #warning "Auto-assigned Z3_DIAG_PIN to X_MAX_PIN."
  #elif Z3_USE_ENDSTOP == _YMIN_
    #warning "Auto-assigned Z3_DIAG_PIN to Y_MIN_PIN."
  #elif Z3_USE_ENDSTOP == _YMAX_
    #warning "Auto-assigned Z3_DIAG_PIN to Y_MAX_PIN."
  #elif Z3_USE_ENDSTOP == _ZMIN_
    #warning "Auto-assigned Z3_DIAG_PIN to Z_MIN_PIN."
  #elif Z3_USE_ENDSTOP == _ZMAX_
    #warning "Auto-assigned Z3_DIAG_PIN to Z_MAX_PIN."
  #elif Z3_USE_ENDSTOP == _XDIAG_
    #warning "Auto-assigned Z3_DIAG_PIN to X_DIAG_PIN."
  #elif Z3_USE_ENDSTOP == _YDIAG_
    #warning "Auto-assigned Z3_DIAG_PIN to Y_DIAG_PIN."
  #elif Z3_USE_ENDSTOP == _ZDIAG_
    #warning "Auto-assigned Z3_DIAG_PIN to Z_DIAG_PIN."
  #elif Z3_USE_ENDSTOP == _E0DIAG_
    #warning "Auto-assigned Z3_DIAG_PIN to E0_DIAG_PIN."
  #elif Z3_USE_ENDSTOP == _E1DIAG_
    #warning "Auto-assigned Z3_DIAG_PIN to E1_DIAG_PIN."
  #elif Z3_USE_ENDSTOP == _E2DIAG_
    #warning "Auto-assigned Z3_DIAG_PIN to E2_DIAG_PIN."
  #elif Z3_USE_ENDSTOP == _E3DIAG_
    #warning "Auto-assigned Z3_DIAG_PIN to E3_DIAG_PIN."
  #elif Z3_USE_ENDSTOP == _E4DIAG_
    #warning "Auto-assigned Z3_DIAG_PIN to E4_DIAG_PIN."
  #elif Z3_USE_ENDSTOP == _E5DIAG_
    #warning "Auto-assigned Z3_DIAG_PIN to E5_DIAG_PIN."
  #elif Z3_USE_ENDSTOP == _E6DIAG_
    #warning "Auto-assigned Z3_DIAG_PIN to E6_DIAG_PIN."
  #elif Z3_USE_ENDSTOP == _E7DIAG_
    #warning "Auto-assigned Z3_DIAG_PIN to E7_DIAG_PIN."
  #endif
#endif
#if AUTO_ASSIGNED_Z4_STEPPER
  #warning "Auto-assigned Z4 STEP/DIR/ENABLE_PINs to unused En_STEP/DIR/ENABLE_PINs."
#endif
#if AUTO_ASSIGNED_Z4_CS
  #warning "Auto-assigned Z4_CS_PIN to an unused En_CS_PIN."
#endif
#if AUTO_ASSIGNED_Z4_MS1
  #warning "Auto-assigned Z4_MS1_PIN to an unused En_MS1_PIN."
#endif
#if AUTO_ASSIGNED_Z4_MS2
  #warning "Auto-assigned Z4_MS2_PIN to an unused En_MS2_PIN."
#endif
#if AUTO_ASSIGNED_Z4_MS3
  #warning "Auto-assigned Z4_MS3_PIN to an unused En_MS3_PIN."
#endif
#if AUTO_ASSIGNED_Z4_DIAG
  #if Z4_USE_ENDSTOP == _XMIN_
    #warning "Auto-assigned Z4_DIAG_PIN to X_MIN_PIN."
  #elif Z4_USE_ENDSTOP == _XMAX_
    #warning "Auto-assigned Z4_DIAG_PIN to X_MAX_PIN."
  #elif Z4_USE_ENDSTOP == _YMIN_
    #warning "Auto-assigned Z4_DIAG_PIN to Y_MIN_PIN."
  #elif Z4_USE_ENDSTOP == _YMAX_
    #warning "Auto-assigned Z4_DIAG_PIN to Y_MAX_PIN."
  #elif Z4_USE_ENDSTOP == _ZMIN_
    #warning "Auto-assigned Z4_DIAG_PIN to Z_MIN_PIN."
  #elif Z4_USE_ENDSTOP == _ZMAX_
    #warning "Auto-assigned Z4_DIAG_PIN to Z_MAX_PIN."
  #elif Z4_USE_ENDSTOP == _XDIAG_
    #warning "Auto-assigned Z4_DIAG_PIN to X_DIAG_PIN."
  #elif Z4_USE_ENDSTOP == _YDIAG_
    #warning "Auto-assigned Z4_DIAG_PIN to Y_DIAG_PIN."
  #elif Z4_USE_ENDSTOP == _ZDIAG_
    #warning "Auto-assigned Z4_DIAG_PIN to Z_DIAG_PIN."
  #elif Z4_USE_ENDSTOP == _E0DIAG_
    #warning "Auto-assigned Z4_DIAG_PIN to E0_DIAG_PIN."
  #elif Z4_USE_ENDSTOP == _E1DIAG_
    #warning "Auto-assigned Z4_DIAG_PIN to E1_DIAG_PIN."
  #elif Z4_USE_ENDSTOP == _E2DIAG_
    #warning "Auto-assigned Z4_DIAG_PIN to E2_DIAG_PIN."
  #elif Z4_USE_ENDSTOP == _E3DIAG_
    #warning "Auto-assigned Z4_DIAG_PIN to E3_DIAG_PIN."
  #elif Z4_USE_ENDSTOP == _E4DIAG_
    #warning "Auto-assigned Z4_DIAG_PIN to E4_DIAG_PIN."
  #elif Z4_USE_ENDSTOP == _E5DIAG_
    #warning "Auto-assigned Z4_DIAG_PIN to E5_DIAG_PIN."
  #elif Z4_USE_ENDSTOP == _E6DIAG_
    #warning "Auto-assigned Z4_DIAG_PIN to E6_DIAG_PIN."
  #elif Z4_USE_ENDSTOP == _E7DIAG_
    #warning "Auto-assigned Z4_DIAG_PIN to E7_DIAG_PIN."
  #endif
#endif
#if AUTO_ASSIGNED_I_STEPPER
  #warning "Auto-assigned I STEP/DIR/ENABLE_PINs to unused En_STEP/DIR/ENABLE_PINs."
#endif
#if AUTO_ASSIGNED_I_CS
  #warning "Auto-assigned I_CS_PIN to an unused En_CS_PIN."
#endif
#if AUTO_ASSIGNED_I_MS1
  #warning "Auto-assigned I_MS1_PIN to an unused En_MS1_PIN."
#endif
#if AUTO_ASSIGNED_I_MS2
  #warning "Auto-assigned I_MS2_PIN to an unused En_MS2_PIN."
#endif
#if AUTO_ASSIGNED_I_MS3
  #warning "Auto-assigned I_MS3_PIN to an unused En_MS3_PIN."
#endif
#if AUTO_ASSIGNED_I_DIAG
  #if I_USE_ENDSTOP == _XMIN_
    #warning "Auto-assigned I_DIAG_PIN to X_MIN_PIN."
  #elif I_USE_ENDSTOP == _XMAX_
    #warning "Auto-assigned I_DIAG_PIN to X_MAX_PIN."
  #elif I_USE_ENDSTOP == _YMIN_
    #warning "Auto-assigned I_DIAG_PIN to Y_MIN_PIN."
  #elif I_USE_ENDSTOP == _YMAX_
    #warning "Auto-assigned I_DIAG_PIN to Y_MAX_PIN."
  #elif I_USE_ENDSTOP == _ZMIN_
    #warning "Auto-assigned I_DIAG_PIN to Z_MIN_PIN."
  #elif I_USE_ENDSTOP == _ZMAX_
    #warning "Auto-assigned I_DIAG_PIN to Z_MAX_PIN."
  #elif I_USE_ENDSTOP == _XDIAG_
    #warning "Auto-assigned I_DIAG_PIN to X_DIAG_PIN."
  #elif I_USE_ENDSTOP == _YDIAG_
    #warning "Auto-assigned I_DIAG_PIN to Y_DIAG_PIN."
  #elif I_USE_ENDSTOP == _ZDIAG_
    #warning "Auto-assigned I_DIAG_PIN to Z_DIAG_PIN."
  #elif I_USE_ENDSTOP == _E0DIAG_
    #warning "Auto-assigned I_DIAG_PIN to E0_DIAG_PIN."
  #elif I_USE_ENDSTOP == _E1DIAG_
    #warning "Auto-assigned I_DIAG_PIN to E1_DIAG_PIN."
  #elif I_USE_ENDSTOP == _E2DIAG_
    #warning "Auto-assigned I_DIAG_PIN to E2_DIAG_PIN."
  #elif I_USE_ENDSTOP == _E3DIAG_
    #warning "Auto-assigned I_DIAG_PIN to E3_DIAG_PIN."
  #elif I_USE_ENDSTOP == _E4DIAG_
    #warning "Auto-assigned I_DIAG_PIN to E4_DIAG_PIN."
  #elif I_USE_ENDSTOP == _E5DIAG_
    #warning "Auto-assigned I_DIAG_PIN to E5_DIAG_PIN."
  #elif I_USE_ENDSTOP == _E6DIAG_
    #warning "Auto-assigned I_DIAG_PIN to E6_DIAG_PIN."
  #elif I_USE_ENDSTOP == _E7DIAG_
    #warning "Auto-assigned I_DIAG_PIN to E7_DIAG_PIN."
  #endif
#endif
#if AUTO_ASSIGNED_J_STEPPER
  #warning "Auto-assigned J STEP/DIR/ENABLE_PINs to unused En_STEP/DIR/ENABLE_PINs."
#endif
#if AUTO_ASSIGNED_J_CS
  #warning "Auto-assigned J_CS_PIN to an unused En_CS_PIN."
#endif
#if AUTO_ASSIGNED_J_MS1
  #warning "Auto-assigned J_MS1_PIN to an unused En_MS1_PIN."
#endif
#if AUTO_ASSIGNED_J_MS2
  #warning "Auto-assigned J_MS2_PIN to an unused En_MS2_PIN."
#endif
#if AUTO_ASSIGNED_J_MS3
  #warning "Auto-assigned J_MS3_PIN to an unused En_MS3_PIN."
#endif
#if AUTO_ASSIGNED_J_DIAG
  #if J_USE_ENDSTOP == _XMIN_
    #warning "Auto-assigned J_DIAG_PIN to X_MIN_PIN."
  #elif J_USE_ENDSTOP == _XMAX_
    #warning "Auto-assigned J_DIAG_PIN to X_MAX_PIN."
  #elif J_USE_ENDSTOP == _YMIN_
    #warning "Auto-assigned J_DIAG_PIN to Y_MIN_PIN."
  #elif J_USE_ENDSTOP == _YMAX_
    #warning "Auto-assigned J_DIAG_PIN to Y_MAX_PIN."
  #elif J_USE_ENDSTOP == _ZMIN_
    #warning "Auto-assigned J_DIAG_PIN to Z_MIN_PIN."
  #elif J_USE_ENDSTOP == _ZMAX_
    #warning "Auto-assigned J_DIAG_PIN to Z_MAX_PIN."
  #elif J_USE_ENDSTOP == _XDIAG_
    #warning "Auto-assigned J_DIAG_PIN to X_DIAG_PIN."
  #elif J_USE_ENDSTOP == _YDIAG_
    #warning "Auto-assigned J_DIAG_PIN to Y_DIAG_PIN."
  #elif J_USE_ENDSTOP == _ZDIAG_
    #warning "Auto-assigned J_DIAG_PIN to Z_DIAG_PIN."
  #elif J_USE_ENDSTOP == _E0DIAG_
    #warning "Auto-assigned J_DIAG_PIN to E0_DIAG_PIN."
  #elif J_USE_ENDSTOP == _E1DIAG_
    #warning "Auto-assigned J_DIAG_PIN to E1_DIAG_PIN."
  #elif J_USE_ENDSTOP == _E2DIAG_
    #warning "Auto-assigned J_DIAG_PIN to E2_DIAG_PIN."
  #elif J_USE_ENDSTOP == _E3DIAG_
    #warning "Auto-assigned J_DIAG_PIN to E3_DIAG_PIN."
  #elif J_USE_ENDSTOP == _E4DIAG_
    #warning "Auto-assigned J_DIAG_PIN to E4_DIAG_PIN."
  #elif J_USE_ENDSTOP == _E5DIAG_
    #warning "Auto-assigned J_DIAG_PIN to E5_DIAG_PIN."
  #elif J_USE_ENDSTOP == _E6DIAG_
    #warning "Auto-assigned J_DIAG_PIN to E6_DIAG_PIN."
  #elif J_USE_ENDSTOP == _E7DIAG_
    #warning "Auto-assigned J_DIAG_PIN to E7_DIAG_PIN."
  #endif
#endif
#if AUTO_ASSIGNED_K_STEPPER
  #warning "Auto-assigned K STEP/DIR/ENABLE_PINs to unused En_STEP/DIR/ENABLE_PINs."
#endif
#if AUTO_ASSIGNED_K_CS
  #warning "Auto-assigned K_CS_PIN to an unused En_CS_PIN."
#endif
#if AUTO_ASSIGNED_K_MS1
  #warning "Auto-assigned K_MS1_PIN to an unused En_MS1_PIN."
#endif
#if AUTO_ASSIGNED_K_MS2
  #warning "Auto-assigned K_MS2_PIN to an unused En_MS2_PIN."
#endif
#if AUTO_ASSIGNED_K_MS3
  #warning "Auto-assigned K_MS3_PIN to an unused En_MS3_PIN."
#endif
#if AUTO_ASSIGNED_K_DIAG
  #if K_USE_ENDSTOP == _XMIN_
    #warning "Auto-assigned K_DIAG_PIN to X_MIN_PIN."
  #elif K_USE_ENDSTOP == _XMAX_
    #warning "Auto-assigned K_DIAG_PIN to X_MAX_PIN."
  #elif K_USE_ENDSTOP == _YMIN_
    #warning "Auto-assigned K_DIAG_PIN to Y_MIN_PIN."
  #elif K_USE_ENDSTOP == _YMAX_
    #warning "Auto-assigned K_DIAG_PIN to Y_MAX_PIN."
  #elif K_USE_ENDSTOP == _ZMIN_
    #warning "Auto-assigned K_DIAG_PIN to Z_MIN_PIN."
  #elif K_USE_ENDSTOP == _ZMAX_
    #warning "Auto-assigned K_DIAG_PIN to Z_MAX_PIN."
  #elif K_USE_ENDSTOP == _XDIAG_
    #warning "Auto-assigned K_DIAG_PIN to X_DIAG_PIN."
  #elif K_USE_ENDSTOP == _YDIAG_
    #warning "Auto-assigned K_DIAG_PIN to Y_DIAG_PIN."
  #elif K_USE_ENDSTOP == _ZDIAG_
    #warning "Auto-assigned K_DIAG_PIN to Z_DIAG_PIN."
  #elif K_USE_ENDSTOP == _E0DIAG_
    #warning "Auto-assigned K_DIAG_PIN to E0_DIAG_PIN."
  #elif K_USE_ENDSTOP == _E1DIAG_
    #warning "Auto-assigned K_DIAG_PIN to E1_DIAG_PIN."
  #elif K_USE_ENDSTOP == _E2DIAG_
    #warning "Auto-assigned K_DIAG_PIN to E2_DIAG_PIN."
  #elif K_USE_ENDSTOP == _E3DIAG_
    #warning "Auto-assigned K_DIAG_PIN to E3_DIAG_PIN."
  #elif K_USE_ENDSTOP == _E4DIAG_
    #warning "Auto-assigned K_DIAG_PIN to E4_DIAG_PIN."
  #elif K_USE_ENDSTOP == _E5DIAG_
    #warning "Auto-assigned K_DIAG_PIN to E5_DIAG_PIN."
  #elif K_USE_ENDSTOP == _E6DIAG_
    #warning "Auto-assigned K_DIAG_PIN to E6_DIAG_PIN."
  #elif K_USE_ENDSTOP == _E7DIAG_
    #warning "Auto-assigned K_DIAG_PIN to E7_DIAG_PIN."
  #endif
#endif

#if SHARED_ENABLE_XY
  #warning "X and Y enable pins are shared. M17/M18/M84 can not be used to disable just X or Y"
#endif
#if SHARED_ENABLE_XZ
  #warning "X and Z enable pins are shared. M17/M18/M84 can not be used to disable just X or Z"
#endif
#if SHARED_ENABLE_XE
  #warning "X and E enable pins are shared. M17/M18/M84 can not be used to disable just X or E"
#endif
#if SHARED_ENABLE_XI
  #warning "X and I enable pins are shared. M17/M18/M84 can not be used to disable just X or I"
#endif
#if SHARED_ENABLE_XJ
  #warning "X and J enable pins are shared. M17/M18/M84 can not be used to disable just X or J"
#endif
#if SHARED_ENABLE_XK
  #warning "X and K enable pins are shared. M17/M18/M84 can not be used to disable just X or K"
#endif
#if SHARED_ENABLE_YZ
  #warning "Y and Z enable pins are shared. M17/M18/M84 cqn not be used to disable just Y or Z"
#endif
#if SHARED_ENABLE_YE
  #warning "Y and E enable pins are shared. M17/M18/M84 can not be used to disable just Y or E"
#endif
#if SHARED_ENABLE_YI
  #warning "Y and I enable pins are shared. M17/M18/M84 can not be used to disable just Y or I"
#endif
#if SHARED_ENABLE_YJ
  #warning "Y and J enable pins are shared. M17/M18/M84 can not be used to disable just Y or J"
#endif
#if SHARED_ENABLE_YK
  #warning "Y and K enable pins are shared. M17/M18/M84 can not be used to disable just Y or K"
#endif
#if SHARED_ENABLE_ZE
  #warning "Z and E enable pins are shared. M17/M18/M84 can not be used to disable just Z or E"
#endif
#if SHARED_ENABLE_ZI
  #warning "Z and I enable pins are shared. M17/M18/M84 can not be used to disable just Z or I"
#endif
#if SHARED_ENABLE_ZJ
  #warning "Z and J enable pins are shared. M17/M18/M84 can not be used to disable just Z or J"
#endif
#if SHARED_ENABLE_ZK
  #warning "Z and K enable pins are shared. M17/M18/M84 can not be used to disable just Z or K"
#endif
#if SHARED_ENABLE_IE
  #warning "I and E enable pins are shared. M17/M18/M84 can not be used to disable just I or E"
#endif
#if SHARED_ENABLE_JE
  #warning "J and E enable pins are shared. M17/M18/M84 can not be used to disable just J or E"
#endif
#if SHARED_ENABLE_KE
  #warning "K and E enable pins are shared. M17/M18/M84 can not be used to disable just K or E"
#endif
#if SHARED_ENABLE_IJ
  #warning "I and J enable pins are shared. M17/M18/M84 can not be used to disable just I or J"
#endif
#if SHARED_ENABLE_IK
  #warning "I and K enable pins are shared. M17/M18/M84 can not be used to disable just I or K"
#endif
#if SHARED_ENABLE_JK
  #warning "J and K enable pins are shared. M17/M18/M84 can not be used to disable just J or K"
#endif
#if SHARED_ENABLE_XYZE
  #warning "X,Y,Z and E enable pins are shared. M17/M18/M84 can not be used to disable just X,Y,Z or E"
#endif
#if SHARED_ENABLE_XYZ
  #warning "X,Y and Z enable pins are shared. M17/M18/M84 can not be used to disable just X,Y or Z"
#endif
#if SHARED_ENABLE_XYE
  #warning "X,Y and E enable pins are shared. M17/M18/M84 can not be used to disable just X,Y or E"
#endif
