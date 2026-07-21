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
 * runtime_endstop_pins.h
 *
 * Runtime endstop pin mapping for RUNTIME_HOMING_DIRECTION feature.
 *
 * When both MIN and MAX endstop pins are defined for an axis, changing
 * homing direction at runtime (via M671) should switch which physical
 * pin is used for homing.
 *
 * This header provides inline accessor functions that return the appropriate
 * endstop pin based on current runtime homing direction.
 */

#if ENABLED(RUNTIME_HOMING_DIRECTION)

#include "../module/motion.h"

// Store physical pin numbers as compile-time constants
// These preserve the original pin assignments from the board definition

#if HAS_X_AXIS
  #if defined(X_MIN_PIN) && defined(X_MAX_PIN)
    constexpr pin_t X_MIN_ENDSTOP_PIN = X_MIN_PIN;
    constexpr pin_t X_MAX_ENDSTOP_PIN = X_MAX_PIN;
    #define HAS_X_RUNTIME_ENDSTOP_SWAP 1
  #endif
#endif

#if HAS_Y_AXIS
  #if defined(Y_MIN_PIN) && defined(Y_MAX_PIN)
    constexpr pin_t Y_MIN_ENDSTOP_PIN = Y_MIN_PIN;
    constexpr pin_t Y_MAX_ENDSTOP_PIN = Y_MAX_PIN;
    #define HAS_Y_RUNTIME_ENDSTOP_SWAP 1
  #endif
#endif

#if HAS_Z_AXIS
  #if defined(Z_MIN_PIN) && defined(Z_MAX_PIN)
    constexpr pin_t Z_MIN_ENDSTOP_PIN = Z_MIN_PIN;
    constexpr pin_t Z_MAX_ENDSTOP_PIN = Z_MAX_PIN;
    #define HAS_Z_RUNTIME_ENDSTOP_SWAP 1
  #endif
#endif

#if HAS_I_AXIS
  #if defined(I_MIN_PIN) && defined(I_MAX_PIN)
    constexpr pin_t I_MIN_ENDSTOP_PIN = I_MIN_PIN;
    constexpr pin_t I_MAX_ENDSTOP_PIN = I_MAX_PIN;
    #define HAS_I_RUNTIME_ENDSTOP_SWAP 1
  #endif
#endif

#if HAS_J_AXIS
  #if defined(J_MIN_PIN) && defined(J_MAX_PIN)
    constexpr pin_t J_MIN_ENDSTOP_PIN = J_MIN_PIN;
    constexpr pin_t J_MAX_ENDSTOP_PIN = J_MAX_PIN;
    #define HAS_J_RUNTIME_ENDSTOP_SWAP 1
  #endif
#endif

#if HAS_K_AXIS
  #if defined(K_MIN_PIN) && defined(K_MAX_PIN)
    constexpr pin_t K_MIN_ENDSTOP_PIN = K_MIN_PIN;
    constexpr pin_t K_MAX_ENDSTOP_PIN = K_MAX_PIN;
    #define HAS_K_RUNTIME_ENDSTOP_SWAP 1
  #endif
#endif

#if HAS_U_AXIS
  #if defined(U_MIN_PIN) && defined(U_MAX_PIN)
    constexpr pin_t U_MIN_ENDSTOP_PIN = U_MIN_PIN;
    constexpr pin_t U_MAX_ENDSTOP_PIN = U_MAX_PIN;
    #define HAS_U_RUNTIME_ENDSTOP_SWAP 1
  #endif
#endif

#if HAS_V_AXIS
  #if defined(V_MIN_PIN) && defined(V_MAX_PIN)
    constexpr pin_t V_MIN_ENDSTOP_PIN = V_MIN_PIN;
    constexpr pin_t V_MAX_ENDSTOP_PIN = V_MAX_PIN;
    #define HAS_V_RUNTIME_ENDSTOP_SWAP 1
  #endif
#endif

#if HAS_W_AXIS
  #if defined(W_MIN_PIN) && defined(W_MAX_PIN)
    constexpr pin_t W_MIN_ENDSTOP_PIN = W_MIN_PIN;
    constexpr pin_t W_MAX_ENDSTOP_PIN = W_MAX_PIN;
    #define HAS_W_RUNTIME_ENDSTOP_SWAP 1
  #endif
#endif

// Runtime endstop pin accessors
// These return the appropriate physical pin based on current homing direction

#if HAS_X_AXIS
  FORCE_INLINE pin_t X_HOME_ENDSTOP_PIN() {
    #if HAS_X_RUNTIME_ENDSTOP_SWAP
      return (motion.home_dir(X_AXIS) < 0) ? X_MIN_ENDSTOP_PIN : X_MAX_ENDSTOP_PIN;
    #elif defined(X_MIN_PIN)
      return X_MIN_PIN;
    #elif defined(X_MAX_PIN)
      return X_MAX_PIN;
    #else
      return -1;
    #endif
  }
#endif

#if HAS_Y_AXIS
  FORCE_INLINE pin_t Y_HOME_ENDSTOP_PIN() {
    #if HAS_Y_RUNTIME_ENDSTOP_SWAP
      return (motion.home_dir(Y_AXIS) < 0) ? Y_MIN_ENDSTOP_PIN : Y_MAX_ENDSTOP_PIN;
    #elif defined(Y_MIN_PIN)
      return Y_MIN_PIN;
    #elif defined(Y_MAX_PIN)
      return Y_MAX_PIN;
    #else
      return -1;
    #endif
  }
#endif

#if HAS_Z_AXIS
  FORCE_INLINE pin_t Z_HOME_ENDSTOP_PIN() {
    #if HAS_Z_RUNTIME_ENDSTOP_SWAP
      return (motion.home_dir(Z_AXIS) < 0) ? Z_MIN_ENDSTOP_PIN : Z_MAX_ENDSTOP_PIN;
    #elif defined(Z_MIN_PIN)
      return Z_MIN_PIN;
    #elif defined(Z_MAX_PIN)
      return Z_MAX_PIN;
    #else
      return -1;
    #endif
  }
#endif

#if HAS_I_AXIS
  FORCE_INLINE pin_t I_HOME_ENDSTOP_PIN() {
    #if HAS_I_RUNTIME_ENDSTOP_SWAP
      return (motion.home_dir(I_AXIS) < 0) ? I_MIN_ENDSTOP_PIN : I_MAX_ENDSTOP_PIN;
    #elif defined(I_MIN_PIN)
      return I_MIN_PIN;
    #elif defined(I_MAX_PIN)
      return I_MAX_PIN;
    #else
      return -1;
    #endif
  }
#endif

#if HAS_J_AXIS
  FORCE_INLINE pin_t J_HOME_ENDSTOP_PIN() {
    #if HAS_J_RUNTIME_ENDSTOP_SWAP
      return (motion.home_dir(J_AXIS) < 0) ? J_MIN_ENDSTOP_PIN : J_MAX_ENDSTOP_PIN;
    #elif defined(J_MIN_PIN)
      return J_MIN_PIN;
    #elif defined(J_MAX_PIN)
      return J_MAX_PIN;
    #else
      return -1;
    #endif
  }
#endif

#if HAS_K_AXIS
  FORCE_INLINE pin_t K_HOME_ENDSTOP_PIN() {
    #if HAS_K_RUNTIME_ENDSTOP_SWAP
      return (motion.home_dir(K_AXIS) < 0) ? K_MIN_ENDSTOP_PIN : K_MAX_ENDSTOP_PIN;
    #elif defined(K_MIN_PIN)
      return K_MIN_PIN;
    #elif defined(K_MAX_PIN)
      return K_MAX_PIN;
    #else
      return -1;
    #endif
  }
#endif

#if HAS_U_AXIS
  FORCE_INLINE pin_t U_HOME_ENDSTOP_PIN() {
    #if HAS_U_RUNTIME_ENDSTOP_SWAP
      return (motion.home_dir(U_AXIS) < 0) ? U_MIN_ENDSTOP_PIN : U_MAX_ENDSTOP_PIN;
    #elif defined(U_MIN_PIN)
      return U_MIN_PIN;
    #elif defined(U_MAX_PIN)
      return U_MAX_PIN;
    #else
      return -1;
    #endif
  }
#endif

#if HAS_V_AXIS
  FORCE_INLINE pin_t V_HOME_ENDSTOP_PIN() {
    #if HAS_V_RUNTIME_ENDSTOP_SWAP
      return (motion.home_dir(V_AXIS) < 0) ? V_MIN_ENDSTOP_PIN : V_MAX_ENDSTOP_PIN;
    #elif defined(V_MIN_PIN)
      return V_MIN_PIN;
    #elif defined(V_MAX_PIN)
      return V_MAX_PIN;
    #else
      return -1;
    #endif
  }
#endif

#if HAS_W_AXIS
  FORCE_INLINE pin_t W_HOME_ENDSTOP_PIN() {
    #if HAS_W_RUNTIME_ENDSTOP_SWAP
      return (motion.home_dir(W_AXIS) < 0) ? W_MIN_ENDSTOP_PIN : W_MAX_ENDSTOP_PIN;
    #elif defined(W_MIN_PIN)
      return W_MIN_PIN;
    #elif defined(W_MAX_PIN)
      return W_MAX_PIN;
    #else
      return -1;
    #endif
  }
#endif

// Convenience macro for accessing runtime endstop pins
#define GET_HOME_ENDSTOP_PIN(A) A##_HOME_ENDSTOP_PIN()

// Runtime-aware endstop pin READ macros
// These read the appropriate physical pin based on current homing direction

#if HAS_X_AXIS
  #if HAS_X_RUNTIME_ENDSTOP_SWAP
    #define READ_X_HOME_ENDSTOP() extDigitalRead(X_HOME_ENDSTOP_PIN())
    #define X_HOME_ENDSTOP_HIT_STATE() (motion.home_dir(X_AXIS) < 0 ? X_MIN_ENDSTOP_HIT_STATE : X_MAX_ENDSTOP_HIT_STATE)
  #else
    #define READ_X_HOME_ENDSTOP() READ(TERN(X_MIN_PIN, X_MIN_PIN, X_MAX_PIN))
    #define X_HOME_ENDSTOP_HIT_STATE() TERN(X_MIN_PIN, X_MIN_ENDSTOP_HIT_STATE, X_MAX_ENDSTOP_HIT_STATE)
  #endif
#endif

#if HAS_Y_AXIS
  #if HAS_Y_RUNTIME_ENDSTOP_SWAP
    #define READ_Y_HOME_ENDSTOP() extDigitalRead(Y_HOME_ENDSTOP_PIN())
    #define Y_HOME_ENDSTOP_HIT_STATE() (motion.home_dir(Y_AXIS) < 0 ? Y_MIN_ENDSTOP_HIT_STATE : Y_MAX_ENDSTOP_HIT_STATE)
  #else
    #define READ_Y_HOME_ENDSTOP() READ(TERN(Y_MIN_PIN, Y_MIN_PIN, Y_MAX_PIN))
    #define Y_HOME_ENDSTOP_HIT_STATE() TERN(Y_MIN_PIN, Y_MIN_ENDSTOP_HIT_STATE, Y_MAX_ENDSTOP_HIT_STATE)
  #endif
#endif

#if HAS_Z_AXIS
  #if HAS_Z_RUNTIME_ENDSTOP_SWAP
    #define READ_Z_HOME_ENDSTOP() extDigitalRead(Z_HOME_ENDSTOP_PIN())
    #define Z_HOME_ENDSTOP_HIT_STATE() (motion.home_dir(Z_AXIS) < 0 ? Z_MIN_ENDSTOP_HIT_STATE : Z_MAX_ENDSTOP_HIT_STATE)
  #else
    #define READ_Z_HOME_ENDSTOP() READ(TERN(Z_MIN_PIN, Z_MIN_PIN, Z_MAX_PIN))
    #define Z_HOME_ENDSTOP_HIT_STATE() TERN(Z_MIN_PIN, Z_MIN_ENDSTOP_HIT_STATE, Z_MAX_ENDSTOP_HIT_STATE)
  #endif
#endif

#if HAS_I_AXIS
  #if HAS_I_RUNTIME_ENDSTOP_SWAP
    #define READ_I_HOME_ENDSTOP() extDigitalRead(I_HOME_ENDSTOP_PIN())
    #define I_HOME_ENDSTOP_HIT_STATE() (motion.home_dir(I_AXIS) < 0 ? I_MIN_ENDSTOP_HIT_STATE : I_MAX_ENDSTOP_HIT_STATE)
  #else
    #define READ_I_HOME_ENDSTOP() READ(TERN(I_MIN_PIN, I_MIN_PIN, I_MAX_PIN))
    #define I_HOME_ENDSTOP_HIT_STATE() TERN(I_MIN_PIN, I_MIN_ENDSTOP_HIT_STATE, I_MAX_ENDSTOP_HIT_STATE)
  #endif
#endif

#if HAS_J_AXIS
  #if HAS_J_RUNTIME_ENDSTOP_SWAP
    #define READ_J_HOME_ENDSTOP() extDigitalRead(J_HOME_ENDSTOP_PIN())
    #define J_HOME_ENDSTOP_HIT_STATE() (motion.home_dir(J_AXIS) < 0 ? J_MIN_ENDSTOP_HIT_STATE : J_MAX_ENDSTOP_HIT_STATE)
  #else
    #define READ_J_HOME_ENDSTOP() READ(TERN(J_MIN_PIN, J_MIN_PIN, J_MAX_PIN))
    #define J_HOME_ENDSTOP_HIT_STATE() TERN(J_MIN_PIN, J_MIN_ENDSTOP_HIT_STATE, J_MAX_ENDSTOP_HIT_STATE)
  #endif
#endif

#if HAS_K_AXIS
  #if HAS_K_RUNTIME_ENDSTOP_SWAP
    #define READ_K_HOME_ENDSTOP() extDigitalRead(K_HOME_ENDSTOP_PIN())
    #define K_HOME_ENDSTOP_HIT_STATE() (motion.home_dir(K_AXIS) < 0 ? K_MIN_ENDSTOP_HIT_STATE : K_MAX_ENDSTOP_HIT_STATE)
  #else
    #define READ_K_HOME_ENDSTOP() READ(TERN(K_MIN_PIN, K_MIN_PIN, K_MAX_PIN))
    #define K_HOME_ENDSTOP_HIT_STATE() TERN(K_MIN_PIN, K_MIN_ENDSTOP_HIT_STATE, K_MAX_ENDSTOP_HIT_STATE)
  #endif
#endif

#if HAS_U_AXIS
  #if HAS_U_RUNTIME_ENDSTOP_SWAP
    #define READ_U_HOME_ENDSTOP() extDigitalRead(U_HOME_ENDSTOP_PIN())
    #define U_HOME_ENDSTOP_HIT_STATE() (motion.home_dir(U_AXIS) < 0 ? U_MIN_ENDSTOP_HIT_STATE : U_MAX_ENDSTOP_HIT_STATE)
  #else
    #define READ_U_HOME_ENDSTOP() READ(TERN(U_MIN_PIN, U_MIN_PIN, U_MAX_PIN))
    #define U_HOME_ENDSTOP_HIT_STATE() TERN(U_MIN_PIN, U_MIN_ENDSTOP_HIT_STATE, U_MAX_ENDSTOP_HIT_STATE)
  #endif
#endif

#if HAS_V_AXIS
  #if HAS_V_RUNTIME_ENDSTOP_SWAP
    #define READ_V_HOME_ENDSTOP() extDigitalRead(V_HOME_ENDSTOP_PIN())
    #define V_HOME_ENDSTOP_HIT_STATE() (motion.home_dir(V_AXIS) < 0 ? V_MIN_ENDSTOP_HIT_STATE : V_MAX_ENDSTOP_HIT_STATE)
  #else
    #define READ_V_HOME_ENDSTOP() READ(TERN(V_MIN_PIN, V_MIN_PIN, V_MAX_PIN))
    #define V_HOME_ENDSTOP_HIT_STATE() TERN(V_MIN_PIN, V_MIN_ENDSTOP_HIT_STATE, V_MAX_ENDSTOP_HIT_STATE)
  #endif
#endif

#if HAS_W_AXIS
  #if HAS_W_RUNTIME_ENDSTOP_SWAP
    #define READ_W_HOME_ENDSTOP() extDigitalRead(W_HOME_ENDSTOP_PIN())
    #define W_HOME_ENDSTOP_HIT_STATE() (motion.home_dir(W_AXIS) < 0 ? W_MIN_ENDSTOP_HIT_STATE : W_MAX_ENDSTOP_HIT_STATE)
  #else
    #define READ_W_HOME_ENDSTOP() READ(TERN(W_MIN_PIN, W_MIN_PIN, W_MAX_PIN))
    #define W_HOME_ENDSTOP_HIT_STATE() TERN(W_MIN_PIN, W_MIN_ENDSTOP_HIT_STATE, W_MAX_ENDSTOP_HIT_STATE)
  #endif
#endif

#endif // RUNTIME_HOMING_DIRECTION
