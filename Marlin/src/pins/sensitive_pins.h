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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include <stdint.h>

//
// Define unsorted and arrays of pins which may have duplicstes,
// but don't allocate any runtime memory for these arrays.
// We'll use constexpr functions to get the Nth distinct value,
// and return -1 if we go past the end of the array.
//
constexpr pin_t sensitive_dio[] PROGMEM = {

  #if HAS_X_AXIS
    #if PIN_EXISTS(X_MIN)
      X_MIN_PIN,
    #endif
    #if PIN_EXISTS(X_MAX)
      X_MAX_PIN,
    #endif
    #if PIN_EXISTS(X_CS) && AXIS_HAS_SPI(X)
      X_CS_PIN,
    #endif
    #if PIN_EXISTS(X_MS1)
      X_MS1_PIN,
    #endif
    #if PIN_EXISTS(X_MS2)
      X_MS2_PIN,
    #endif
    #if PIN_EXISTS(X_MS3)
      X_MS3_PIN,
    #endif
    #if PIN_EXISTS(X_ENABLE)
      X_ENABLE_PIN,
    #endif
  #endif

  #if HAS_Y_AXIS
    #if PIN_EXISTS(Y_MIN)
      Y_MIN_PIN,
    #endif
    #if PIN_EXISTS(Y_MAX)
      Y_MAX_PIN,
    #endif
    #if PIN_EXISTS(Y_CS) && AXIS_HAS_SPI(Y)
      Y_CS_PIN,
    #endif
    #if PIN_EXISTS(Y_MS1)
      Y_MS1_PIN,
    #endif
    #if PIN_EXISTS(Y_MS2)
      Y_MS2_PIN,
    #endif
    #if PIN_EXISTS(Y_MS3)
      Y_MS3_PIN,
    #endif
    #if PIN_EXISTS(Y_ENABLE)
      Y_ENABLE_PIN,
    #endif
  #endif

  #if HAS_Z_AXIS
    #if PIN_EXISTS(Z_MIN)
      Z_MIN_PIN,
    #endif
    #if PIN_EXISTS(Z_MAX)
      Z_MAX_PIN,
    #endif
    #if PIN_EXISTS(Z_CS) && AXIS_HAS_SPI(Z)
      Z_CS_PIN,
    #endif
    #if PIN_EXISTS(Z_MS1)
      Z_MS1_PIN,
    #endif
    #if PIN_EXISTS(Z_MS2)
      Z_MS2_PIN,
    #endif
    #if PIN_EXISTS(Z_MS3)
      Z_MS3_PIN,
    #endif
    #if PIN_EXISTS(Z_ENABLE)
      Z_ENABLE_PIN,
    #endif
  #endif

  #if HAS_I_AXIS
    #if PIN_EXISTS(I_MIN)
      I_MIN_PIN,
    #endif
    #if PIN_EXISTS(I_MAX)
      I_MAX_PIN,
    #endif
    #if PIN_EXISTS(I_CS) && AXIS_HAS_SPI(I)
      I_CS_PIN,
    #endif
    #if PIN_EXISTS(I_MS1)
      I_MS1_PIN,
    #endif
    #if PIN_EXISTS(I_MS2)
      I_MS2_PIN,
    #endif
    #if PIN_EXISTS(I_MS3)
      I_MS3_PIN,
    #endif
    #if PIN_EXISTS(I_ENABLE)
      I_ENABLE_PIN,
    #endif
  #endif

  #if HAS_J_AXIS
    #if PIN_EXISTS(J_MIN)
      J_MIN_PIN,
    #endif
    #if PIN_EXISTS(J_MAX)
      J_MAX_PIN,
    #endif
    #if PIN_EXISTS(J_CS) && AXIS_HAS_SPI(J)
      J_CS_PIN,
    #endif
    #if PIN_EXISTS(J_MS1)
      J_MS1_PIN,
    #endif
    #if PIN_EXISTS(J_MS2)
      J_MS2_PIN,
    #endif
    #if PIN_EXISTS(J_MS3)
      J_MS3_PIN,
    #endif
    #if PIN_EXISTS(J_ENABLE)
      J_ENABLE_PIN,
    #endif
  #endif

  #if HAS_K_AXIS
    #if PIN_EXISTS(K_MIN)
      K_MIN_PIN,
    #endif
    #if PIN_EXISTS(K_MAX)
      K_MAX_PIN,
    #endif
    #if PIN_EXISTS(K_CS) && AXIS_HAS_SPI(K)
      K_CS_PIN,
    #endif
    #if PIN_EXISTS(K_MS1)
      K_MS1_PIN,
    #endif
    #if PIN_EXISTS(K_MS2)
      K_MS2_PIN,
    #endif
    #if PIN_EXISTS(K_MS3)
      K_MS3_PIN,
    #endif
    #if PIN_EXISTS(K_ENABLE)
      K_ENABLE_PIN,
    #endif
  #endif

  #if HAS_U_AXIS
    #if PIN_EXISTS(U_MIN)
      U_MIN_PIN,
    #endif
    #if PIN_EXISTS(U_MAX)
      U_MAX_PIN,
    #endif
    #if PIN_EXISTS(U_CS) && AXIS_HAS_SPI(U)
      U_CS_PIN,
    #endif
    #if PIN_EXISTS(U_MS1)
      U_MS1_PIN,
    #endif
    #if PIN_EXISTS(U_MS2)
      U_MS2_PIN,
    #endif
    #if PIN_EXISTS(U_MS3)
      U_MS3_PIN,
    #endif
  #endif

  #if HAS_V_AXIS
    #if PIN_EXISTS(V_MIN)
      V_MIN_PIN,
    #endif
    #if PIN_EXISTS(V_MAX)
      V_MAX_PIN,
    #endif
    #if PIN_EXISTS(V_CS) && AXIS_HAS_SPI(V)
      V_CS_PIN,
    #endif
    #if PIN_EXISTS(V_MS1)
      V_MS1_PIN,
    #endif
    #if PIN_EXISTS(V_MS2)
      V_MS2_PIN,
    #endif
    #if PIN_EXISTS(V_MS3)
      V_MS3_PIN,
    #endif
  #endif

  #if HAS_W_AXIS
    #if PIN_EXISTS(W_MIN)
      W_MIN_PIN,
    #endif
    #if PIN_EXISTS(W_MAX)
      W_MAX_PIN,
    #endif
    #if PIN_EXISTS(W_CS) && AXIS_HAS_SPI(W)
      W_CS_PIN,
    #endif
    #if PIN_EXISTS(W_MS1)
      W_MS1_PIN,
    #endif
    #if PIN_EXISTS(W_MS2)
      W_MS2_PIN,
    #endif
    #if PIN_EXISTS(W_MS3)
      W_MS3_PIN,
    #endif
  #endif

  //
  // Extruder Chip Select, Digital Micro-steps
  //

  // Mixing stepper, Switching stepper, or regular stepper
  #define E_NEEDED(N) (ENABLED(MIXING_EXTRUDER) && MIXING_STEPPERS > N) \
                   || (HAS_SWITCHING_EXTRUDER && E_STEPPERS > N) \
                   || (NONE(SWITCHING_EXTRUDER, MIXING_EXTRUDER) && EXTRUDERS > N)

  #if E_NEEDED(0)
    E0_STEP_PIN, E0_DIR_PIN, E0_ENABLE_PIN,
    #if PIN_EXISTS(E0_CS) && AXIS_HAS_SPI(E0)
      E0_CS_PIN,
    #endif
    #if PIN_EXISTS(E0_MS1)
      E0_MS1_PIN,
    #endif
    #if PIN_EXISTS(E0_MS2)
      E0_MS2_PIN,
    #endif
    #if PIN_EXISTS(E0_MS3)
      E0_MS3_PIN,
    #endif
  #endif

  #if E_NEEDED(1)
    E1_STEP_PIN, E1_DIR_PIN, E1_ENABLE_PIN,
    #if PIN_EXISTS(E1_CS) && AXIS_HAS_SPI(E1)
      E1_CS_PIN,
    #endif
    #if PIN_EXISTS(E1_MS1)
      E1_MS1_PIN,
    #endif
    #if PIN_EXISTS(E1_MS2)
      E1_MS2_PIN,
    #endif
    #if PIN_EXISTS(E1_MS3)
      E1_MS3_PIN,
    #endif
  #endif

  #if E_NEEDED(2)
    E2_STEP_PIN, E2_DIR_PIN, E2_ENABLE_PIN,
    #if PIN_EXISTS(E2_CS) && AXIS_HAS_SPI(E2)
      E2_CS_PIN,
    #endif
    #if PIN_EXISTS(E2_MS1)
      E2_MS1_PIN,
    #endif
    #if PIN_EXISTS(E2_MS2)
      E2_MS2_PIN,
    #endif
    #if PIN_EXISTS(E2_MS3)
      E2_MS3_PIN,
    #endif
  #endif

  #if E_NEEDED(3)
    E3_STEP_PIN, E3_DIR_PIN, E3_ENABLE_PIN,
    #if PIN_EXISTS(E3_CS) && AXIS_HAS_SPI(E3)
      E3_CS_PIN,
    #endif
    #if PIN_EXISTS(E3_MS1)
      E3_MS1_PIN,
    #endif
    #if PIN_EXISTS(E3_MS2)
      E3_MS2_PIN,
    #endif
    #if PIN_EXISTS(E3_MS3)
      E3_MS3_PIN,
    #endif
  #endif

  #if E_NEEDED(4)
    E4_STEP_PIN, E4_DIR_PIN, E4_ENABLE_PIN,
    #if PIN_EXISTS(E4_CS) && AXIS_HAS_SPI(E4)
      E4_CS_PIN,
    #endif
    #if PIN_EXISTS(E4_MS1)
      E4_MS1_PIN,
    #endif
    #if PIN_EXISTS(E4_MS2)
      E4_MS2_PIN,
    #endif
    #if PIN_EXISTS(E4_MS3)
      E4_MS3_PIN,
    #endif
  #endif

  #if E_NEEDED(5)
    E5_STEP_PIN, E5_DIR_PIN, E5_ENABLE_PIN,
    #if PIN_EXISTS(E5_CS) && AXIS_HAS_SPI(E5)
      E5_CS_PIN,
    #endif
    #if PIN_EXISTS(E5_MS1)
      E5_MS1_PIN,
    #endif
    #if PIN_EXISTS(E5_MS2)
      E5_MS2_PIN,
    #endif
    #if PIN_EXISTS(E5_MS3)
      E5_MS3_PIN,
    #endif
  #endif

  #if E_NEEDED(6)
    E6_STEP_PIN, E6_DIR_PIN, E6_ENABLE_PIN,
    #if PIN_EXISTS(E6_CS) && AXIS_HAS_SPI(E6)
      E6_CS_PIN,
    #endif
    #if PIN_EXISTS(E6_MS2)
      E6_MS2_PIN,
    #endif
    #if PIN_EXISTS(E6_MS3)
      E6_MS3_PIN,
    #endif
    #if PIN_EXISTS(E6_MS4)
      E6_MS4_PIN,
    #endif
  #endif

  #if E_NEEDED(7)
    E7_STEP_PIN, E7_DIR_PIN, E7_ENABLE_PIN,
    #if PIN_EXISTS(E7_CS) && AXIS_HAS_SPI(E7)
      E7_CS_PIN,
    #endif
    #if PIN_EXISTS(E7_MS3)
      E7_MS3_PIN,
    #endif
    #if PIN_EXISTS(E7_MS4)
      E7_MS4_PIN,
    #endif
    #if PIN_EXISTS(E7_MS5)
      E7_MS5_PIN,
    #endif
  #endif

  //
  // Hotend Heaters and Fans
  //

  #if HAS_HOTEND
    HEATER_0_PIN,
    #if PIN_EXISTS(E0_AUTO_FAN)
      E0_AUTO_FAN_PIN,
    #endif
  #endif
  #if HAS_MULTI_HOTEND
    HEATER_1_PIN,
    #if PIN_EXISTS(E1_AUTO_FAN)
      E1_AUTO_FAN_PIN,
    #endif
  #endif
  #if HOTENDS > 2
    HEATER_2_PIN,
    #if PIN_EXISTS(E2_AUTO_FAN)
      E2_AUTO_FAN_PIN,
    #endif
  #endif
  #if HOTENDS > 3
    HEATER_3_PIN,
    #if PIN_EXISTS(E3_AUTO_FAN)
      E3_AUTO_FAN_PIN,
    #endif
  #endif
  #if HOTENDS > 4
    HEATER_4_PIN,
    #if PIN_EXISTS(E4_AUTO_FAN)
      E4_AUTO_FAN_PIN,
    #endif
  #endif
  #if HOTENDS > 5
    HEATER_5_PIN,
    #if PIN_EXISTS(E5_AUTO_FAN)
      E5_AUTO_FAN_PIN,
    #endif
  #endif
  #if HOTENDS > 6
    HEATER_6_PIN,
    #if PIN_EXISTS(E6_AUTO_FAN)
      E6_AUTO_FAN_PIN,
    #endif
  #endif
  #if HOTENDS > 7
    HEATER_7_PIN,
    #if PIN_EXISTS(E7_AUTO_FAN)
      E7_AUTO_FAN_PIN,
    #endif
  #endif

  //
  // Dual X, Dual Y, Multi-Z
  // Chip Select and Digital Micro-stepping
  //

  #if HAS_X2_STEPPER
    #if PIN_EXISTS(X2_CS) && AXIS_HAS_SPI(X2)
      X2_CS_PIN,
    #endif
    #if PIN_EXISTS(X2_MS1)
      X2_MS1_PIN,
    #endif
    #if PIN_EXISTS(X2_MS2)
      X2_MS2_PIN,
    #endif
    #if PIN_EXISTS(X2_MS3)
      X2_MS3_PIN,
    #endif
  #endif

  #if HAS_Y2_STEPPER
    #if PIN_EXISTS(Y2_CS) && AXIS_HAS_SPI(Y2)
      Y2_CS_PIN,
    #endif
    #if PIN_EXISTS(Y2_MS1)
      Y2_MS1_PIN,
    #endif
    #if PIN_EXISTS(Y2_MS2)
      Y2_MS2_PIN,
    #endif
    #if PIN_EXISTS(Y2_MS3)
      Y2_MS3_PIN,
    #endif
  #endif

  #if NUM_Z_STEPPERS >= 2
    #if PIN_EXISTS(Z2_CS) && AXIS_HAS_SPI(Z2)
      Z2_CS_PIN,
    #endif
    #if PIN_EXISTS(Z2_MS1)
      Z2_MS1_PIN,
    #endif
    #if PIN_EXISTS(Z2_MS2)
      Z2_MS2_PIN,
    #endif
    #if PIN_EXISTS(Z2_MS3)
      Z2_MS3_PIN,
    #endif
  #endif

  #if NUM_Z_STEPPERS >= 3
    #if PIN_EXISTS(Z3_CS) && AXIS_HAS_SPI(Z3)
      Z3_CS_PIN,
    #endif
    #if PIN_EXISTS(Z3_MS1)
      Z3_MS1_PIN,
    #endif
    #if PIN_EXISTS(Z3_MS2)
      Z3_MS2_PIN,
    #endif
    #if PIN_EXISTS(Z3_MS3)
      Z3_MS3_PIN,
    #endif
  #endif

  #if NUM_Z_STEPPERS >= 4
    #if PIN_EXISTS(Z4_CS) && AXIS_HAS_SPI(Z4)
      Z4_CS_PIN,
    #endif
    #if PIN_EXISTS(Z4_MS1)
      Z4_MS1_PIN,
    #endif
    #if PIN_EXISTS(Z4_MS2)
      Z4_MS2_PIN,
    #endif
    #if PIN_EXISTS(Z4_MS3)
      Z4_MS3_PIN,
    #endif
  #endif

  #if PIN_EXISTS(PS_ON)
    PS_ON_PIN,
  #endif

  #if HAS_BED_PROBE && PIN_EXISTS(Z_MIN_PROBE)
    Z_MIN_PROBE_PIN,
  #endif

  #if PIN_EXISTS(FAN0)
    FAN0_PIN,
  #endif
  #if PIN_EXISTS(FAN1)
    FAN1_PIN,
  #endif
  #if PIN_EXISTS(FAN2)
    FAN2_PIN,
  #endif
  #if PIN_EXISTS(FAN3)
    FAN3_PIN,
  #endif
  #if PIN_EXISTS(FAN4)
    FAN4_PIN,
  #endif
  #if PIN_EXISTS(FAN5)
    FAN5_PIN,
  #endif
  #if PIN_EXISTS(FAN6)
    FAN6_PIN,
  #endif
  #if PIN_EXISTS(FAN7)
    FAN7_PIN,
  #endif
  #if PIN_EXISTS(CONTROLLER_FAN)
    CONTROLLER_FAN_PIN,
  #endif

  #if TEMP_SENSOR_BED && PINS_EXIST(TEMP_BED, HEATER_BED)
    HEATER_BED_PIN,
  #endif

  #if TEMP_SENSOR_CHAMBER && PIN_EXISTS(TEMP_CHAMBER)
    #if PIN_EXISTS(HEATER_CHAMBER)
      HEATER_CHAMBER_PIN,
    #endif
    #if PIN_EXISTS(CHAMBER_AUTO_FAN)
      CHAMBER_AUTO_FAN_PIN,
    #endif
  #endif

  #if TEMP_SENSOR_COOLER && PIN_EXISTS(TEMP_COOLER)
    #if PIN_EXISTS(COOLER_AUTO_FAN)
      COOLER_AUTO_FAN_PIN,
    #endif
    #if PIN_EXISTS(COOLER)
      COOLER_PIN,
    #endif
  #endif

  #ifdef HAL_SENSITIVE_PINS
    HAL_SENSITIVE_PINS
  #endif
};

constexpr pin_t sensitive_aio[] PROGMEM = {
  #if HAS_HOTEND
    TEMP_0_PIN,
  #endif
  #if HAS_MULTI_HOTEND
    TEMP_1_PIN,
  #endif
  #if HOTENDS > 2
    TEMP_2_PIN,
  #endif
  #if HOTENDS > 3
    TEMP_3_PIN,
  #endif
  #if HOTENDS > 4
    TEMP_4_PIN,
  #endif
  #if HOTENDS > 5
    TEMP_5_PIN,
  #endif
  #if HOTENDS > 6
    TEMP_6_PIN,
  #endif
  #if HOTENDS > 7
    TEMP_7_PIN,
  #endif
  #if TEMP_SENSOR_BED && PINS_EXIST(TEMP_BED, HEATER_BED)
    TEMP_BED_PIN,
  #endif
  #if TEMP_SENSOR_CHAMBER && PIN_EXISTS(TEMP_CHAMBER)
    TEMP_CHAMBER_PIN,
  #endif
  #if TEMP_SENSOR_COOLER && PIN_EXISTS(TEMP_COOLER)
    TEMP_COOLER_PIN,
  #endif
};
