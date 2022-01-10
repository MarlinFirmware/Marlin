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

//
// File: pins/pins_postprocess.h
// Post-process pins according to configured settings
//

// Define certain undefined pins
#ifndef X_MS1_PIN
  #define X_MS1_PIN -1
#endif
#ifndef X_MS2_PIN
  #define X_MS2_PIN -1
#endif
#ifndef X_MS3_PIN
  #define X_MS3_PIN -1
#endif
#ifndef Y_MS1_PIN
  #define Y_MS1_PIN -1
#endif
#ifndef Y_MS2_PIN
  #define Y_MS2_PIN -1
#endif
#ifndef Y_MS3_PIN
  #define Y_MS3_PIN -1
#endif
#ifndef Z_MS1_PIN
  #define Z_MS1_PIN -1
#endif
#ifndef Z_MS2_PIN
  #define Z_MS2_PIN -1
#endif
#ifndef Z_MS3_PIN
  #define Z_MS3_PIN -1
#endif
#ifndef E0_MS1_PIN
  #define E0_MS1_PIN -1
#endif
#ifndef E0_MS2_PIN
  #define E0_MS2_PIN -1
#endif
#ifndef E0_MS3_PIN
  #define E0_MS3_PIN -1
#endif
#ifndef E1_MS1_PIN
  #define E1_MS1_PIN -1
#endif
#ifndef E1_MS2_PIN
  #define E1_MS2_PIN -1
#endif
#ifndef E1_MS3_PIN
  #define E1_MS3_PIN -1
#endif
#ifndef E2_MS1_PIN
  #define E2_MS1_PIN -1
#endif
#ifndef E2_MS2_PIN
  #define E2_MS2_PIN -1
#endif
#ifndef E2_MS3_PIN
  #define E2_MS3_PIN -1
#endif
#ifndef E3_MS1_PIN
  #define E3_MS1_PIN -1
#endif
#ifndef E3_MS2_PIN
  #define E3_MS2_PIN -1
#endif
#ifndef E3_MS3_PIN
  #define E3_MS3_PIN -1
#endif
#ifndef E4_MS1_PIN
  #define E4_MS1_PIN -1
#endif
#ifndef E4_MS2_PIN
  #define E4_MS2_PIN -1
#endif
#ifndef E4_MS3_PIN
  #define E4_MS3_PIN -1
#endif
#ifndef E5_MS1_PIN
  #define E5_MS1_PIN -1
#endif
#ifndef E5_MS2_PIN
  #define E5_MS2_PIN -1
#endif
#ifndef E5_MS3_PIN
  #define E5_MS3_PIN -1
#endif
#ifndef E6_MS1_PIN
  #define E6_MS1_PIN -1
#endif
#ifndef E6_MS2_PIN
  #define E6_MS2_PIN -1
#endif
#ifndef E6_MS3_PIN
  #define E6_MS3_PIN -1
#endif
#ifndef E7_MS1_PIN
  #define E7_MS1_PIN -1
#endif
#ifndef E7_MS2_PIN
  #define E7_MS2_PIN -1
#endif
#ifndef E7_MS3_PIN
  #define E7_MS3_PIN -1
#endif

#ifndef E0_STEP_PIN
  #define E0_STEP_PIN -1
#endif
#ifndef E0_DIR_PIN
  #define E0_DIR_PIN -1
#endif
#ifndef E0_ENABLE_PIN
  #define E0_ENABLE_PIN -1
#endif
#ifndef E1_STEP_PIN
  #define E1_STEP_PIN -1
#endif
#ifndef E1_DIR_PIN
  #define E1_DIR_PIN -1
#endif
#ifndef E1_ENABLE_PIN
  #define E1_ENABLE_PIN -1
#endif
#ifndef E2_STEP_PIN
  #define E2_STEP_PIN -1
#endif
#ifndef E2_DIR_PIN
  #define E2_DIR_PIN -1
#endif
#ifndef E2_ENABLE_PIN
  #define E2_ENABLE_PIN -1
#endif
#ifndef E3_STEP_PIN
  #define E3_STEP_PIN -1
#endif
#ifndef E3_DIR_PIN
  #define E3_DIR_PIN -1
#endif
#ifndef E3_ENABLE_PIN
  #define E3_ENABLE_PIN -1
#endif
#ifndef E4_STEP_PIN
  #define E4_STEP_PIN -1
#endif
#ifndef E4_DIR_PIN
  #define E4_DIR_PIN -1
#endif
#ifndef E4_ENABLE_PIN
  #define E4_ENABLE_PIN -1
#endif
#ifndef E5_STEP_PIN
  #define E5_STEP_PIN -1
#endif
#ifndef E5_DIR_PIN
  #define E5_DIR_PIN -1
#endif
#ifndef E5_ENABLE_PIN
  #define E5_ENABLE_PIN -1
#endif
#ifndef E6_STEP_PIN
  #define E6_STEP_PIN -1
#endif
#ifndef E6_DIR_PIN
  #define E6_DIR_PIN -1
#endif
#ifndef E6_ENABLE_PIN
  #define E6_ENABLE_PIN -1
#endif
#ifndef E7_STEP_PIN
  #define E7_STEP_PIN -1
#endif
#ifndef E7_DIR_PIN
  #define E7_DIR_PIN -1
#endif
#ifndef E7_ENABLE_PIN
  #define E7_ENABLE_PIN -1
#endif

//
// Destroy unused CS pins
//
#if !AXIS_HAS_SPI(X)
  #undef X_CS_PIN
#endif
#if !AXIS_HAS_SPI(Y)
  #undef Y_CS_PIN
#endif
#if !AXIS_HAS_SPI(Z)
  #undef Z_CS_PIN
#endif
#if !AXIS_HAS_SPI(I)
  #undef I_CS_PIN
#endif
#if !AXIS_HAS_SPI(J)
  #undef J_CS_PIN
#endif
#if !AXIS_HAS_SPI(K)
  #undef K_CS_PIN
#endif
#if E_STEPPERS && !AXIS_HAS_SPI(E0)
  #undef E0_CS_PIN
#endif
#if E_STEPPERS > 1 && !AXIS_HAS_SPI(E1)
  #undef E1_CS_PIN
#endif
#if E_STEPPERS > 2 && !AXIS_HAS_SPI(E2)
  #undef E2_CS_PIN
#endif
#if E_STEPPERS > 3 && !AXIS_HAS_SPI(E3)
  #undef E3_CS_PIN
#endif
#if E_STEPPERS > 4 && !AXIS_HAS_SPI(E4)
  #undef E4_CS_PIN
#endif
#if E_STEPPERS > 5 && !AXIS_HAS_SPI(E5)
  #undef E5_CS_PIN
#endif
#if E_STEPPERS > 6 && !AXIS_HAS_SPI(E6)
  #undef E6_CS_PIN
#endif
#if E_STEPPERS > 7 && !AXIS_HAS_SPI(E7)
  #undef E7_CS_PIN
#endif

#ifndef X_CS_PIN
  #define X_CS_PIN -1
#endif
#ifndef Y_CS_PIN
  #define Y_CS_PIN -1
#endif
#ifndef Z_CS_PIN
  #define Z_CS_PIN -1
#endif
#ifndef I_CS_PIN
  #define I_CS_PIN -1
#endif
#ifndef J_CS_PIN
  #define J_CS_PIN -1
#endif
#ifndef K_CS_PIN
  #define K_CS_PIN -1
#endif
#ifndef E0_CS_PIN
  #define E0_CS_PIN -1
#endif
#ifndef E1_CS_PIN
  #define E1_CS_PIN -1
#endif
#ifndef E2_CS_PIN
  #define E2_CS_PIN -1
#endif
#ifndef E3_CS_PIN
  #define E3_CS_PIN -1
#endif
#ifndef E4_CS_PIN
  #define E4_CS_PIN -1
#endif
#ifndef E5_CS_PIN
  #define E5_CS_PIN -1
#endif
#ifndef E6_CS_PIN
  #define E6_CS_PIN -1
#endif
#ifndef E7_CS_PIN
  #define E7_CS_PIN -1
#endif

#ifndef FAN_PIN
  #define FAN_PIN -1
#endif
#define FAN0_PIN FAN_PIN
#ifndef FAN1_PIN
  #define FAN1_PIN -1
#endif
#ifndef FAN2_PIN
  #define FAN2_PIN -1
#endif
#ifndef CONTROLLER_FAN_PIN
  #define CONTROLLER_FAN_PIN  -1
#endif

#ifndef FANMUX0_PIN
  #define FANMUX0_PIN -1
#endif
#ifndef FANMUX1_PIN
  #define FANMUX1_PIN -1
#endif
#ifndef FANMUX2_PIN
  #define FANMUX2_PIN -1
#endif

#ifndef HEATER_0_PIN
  #define HEATER_0_PIN -1
#endif
#ifndef HEATER_1_PIN
  #define HEATER_1_PIN -1
#endif
#ifndef HEATER_2_PIN
  #define HEATER_2_PIN -1
#endif
#ifndef HEATER_3_PIN
  #define HEATER_3_PIN -1
#endif
#ifndef HEATER_4_PIN
  #define HEATER_4_PIN -1
#endif
#ifndef HEATER_5_PIN
  #define HEATER_5_PIN -1
#endif
#ifndef HEATER_6_PIN
  #define HEATER_6_PIN -1
#endif
#ifndef HEATER_7_PIN
  #define HEATER_7_PIN -1
#endif
#ifndef HEATER_BED_PIN
  #define HEATER_BED_PIN -1
#endif

#ifndef TEMP_0_PIN
  #define TEMP_0_PIN -1
#endif
#ifndef TEMP_1_PIN
  #define TEMP_1_PIN -1
#endif
#ifndef TEMP_2_PIN
  #define TEMP_2_PIN -1
#endif
#ifndef TEMP_3_PIN
  #define TEMP_3_PIN -1
#endif
#ifndef TEMP_4_PIN
  #define TEMP_4_PIN -1
#endif
#ifndef TEMP_5_PIN
  #define TEMP_5_PIN -1
#endif
#ifndef TEMP_6_PIN
  #define TEMP_6_PIN -1
#endif
#ifndef TEMP_7_PIN
  #define TEMP_7_PIN -1
#endif
#ifndef TEMP_BED_PIN
  #define TEMP_BED_PIN -1
#endif

#ifndef SD_DETECT_PIN
  #define SD_DETECT_PIN -1
#endif
#ifndef SDPOWER_PIN
  #define SDPOWER_PIN -1
#endif
#ifndef SDSS
  #define SDSS -1
#endif
#ifndef LED_PIN
  #define LED_PIN -1
#endif
#if DISABLED(PSU_CONTROL) || !defined(PS_ON_PIN)
  #undef PS_ON_PIN
  #define PS_ON_PIN -1
#endif
#ifndef KILL_PIN
  #define KILL_PIN -1
#endif
#ifndef SUICIDE_PIN
  #define SUICIDE_PIN -1
#endif
#ifndef SUICIDE_PIN_STATE
  #define SUICIDE_PIN_STATE LOW
#endif

#ifndef NUM_SERVO_PLUGS
  #define NUM_SERVO_PLUGS 4
#endif

//
// Assign endstop pins for boards with only 3 connectors
//
#ifdef X_STOP_PIN
  #if X_HOME_TO_MIN
    #define X_MIN_PIN X_STOP_PIN
    #ifndef X_MAX_PIN
      #define X_MAX_PIN -1
    #endif
  #else
    #define X_MAX_PIN X_STOP_PIN
    #ifndef X_MIN_PIN
      #define X_MIN_PIN -1
    #endif
  #endif
#elif X_HOME_TO_MIN
  #define X_STOP_PIN X_MIN_PIN
#else
  #define X_STOP_PIN X_MAX_PIN
#endif
#if !defined(X2_USE_ENDSTOP) && ENABLED(X_DUAL_ENDSTOPS) && PIN_EXISTS(X_STOP)
  #define X2_USE_ENDSTOP _XSTOP_
#endif

#if HAS_Y_AXIS
  #ifdef Y_STOP_PIN
    #if Y_HOME_TO_MIN
      #define Y_MIN_PIN Y_STOP_PIN
      #ifndef Y_MAX_PIN
        #define Y_MAX_PIN -1
      #endif
    #else
      #define Y_MAX_PIN Y_STOP_PIN
      #ifndef Y_MIN_PIN
        #define Y_MIN_PIN -1
      #endif
    #endif
  #elif Y_HOME_TO_MIN
    #define Y_STOP_PIN Y_MIN_PIN
  #else
    #define Y_STOP_PIN Y_MAX_PIN
  #endif
  #if !defined(Y2_USE_ENDSTOP) && ENABLED(Y_DUAL_ENDSTOPS) && PIN_EXISTS(Y_STOP)
    #define Y2_USE_ENDSTOP _YSTOP_
  #endif
#endif

#if HAS_Z_AXIS
  #ifdef Z_STOP_PIN
    #if Z_HOME_TO_MIN
      #define Z_MIN_PIN Z_STOP_PIN
      #ifndef Z_MAX_PIN
        #define Z_MAX_PIN -1
      #endif
    #else
      #define Z_MAX_PIN Z_STOP_PIN
      #ifndef Z_MIN_PIN
        #define Z_MIN_PIN -1
      #endif
    #endif
  #elif Z_HOME_TO_MIN
    #define Z_STOP_PIN Z_MIN_PIN
  #else
    #define Z_STOP_PIN Z_MAX_PIN
  #endif
  #if ENABLED(Z_MULTI_ENDSTOPS) && PIN_EXISTS(Z_STOP)
    #ifndef Z2_USE_ENDSTOP
      #define Z2_USE_ENDSTOP _ZSTOP_
    #endif
    #if NUM_Z_STEPPER_DRIVERS >= 3 && !defined(Z3_USE_ENDSTOP)
      #define Z3_USE_ENDSTOP _ZSTOP_
    #endif
    #if NUM_Z_STEPPER_DRIVERS >= 4 && !defined(Z4_USE_ENDSTOP)
      #define Z4_USE_ENDSTOP _ZSTOP_
    #endif
  #endif
#endif

#if HAS_I_AXIS
  #ifdef I_STOP_PIN
    #if I_HOME_TO_MIN
      #define I_MIN_PIN I_STOP_PIN
      #ifndef I_MAX_PIN
        #define I_MAX_PIN -1
      #endif
    #else
      #define I_MAX_PIN I_STOP_PIN
      #ifndef I_MIN_PIN
        #define I_MIN_PIN -1
      #endif
    #endif
  #elif I_HOME_TO_MIN
    #define I_STOP_PIN I_MIN_PIN
  #else
    #define I_STOP_PIN I_MAX_PIN
  #endif
#else
  #undef I_MIN_PIN
  #undef I_MAX_PIN
#endif

#if HAS_J_AXIS
  #ifdef J_STOP_PIN
    #if J_HOME_TO_MIN
      #define J_MIN_PIN J_STOP_PIN
      #ifndef J_MAX_PIN
        #define J_MAX_PIN -1
      #endif
    #else
      #define J_MAX_PIN J_STOP_PIN
      #ifndef J_MIN_PIN
        #define J_MIN_PIN -1
      #endif
    #endif
  #elif J_HOME_TO_MIN
    #define J_STOP_PIN J_MIN_PIN
  #else
    #define J_STOP_PIN J_MAX_PIN
  #endif
#else
  #undef J_MIN_PIN
  #undef J_MAX_PIN
#endif

#if HAS_K_AXIS
  #ifdef K_STOP_PIN
    #if K_HOME_TO_MIN
      #define K_MIN_PIN K_STOP_PIN
      #ifndef K_MAX_PIN
        #define K_MAX_PIN -1
      #endif
    #else
      #define K_MAX_PIN K_STOP_PIN
      #ifndef K_MIN_PIN
        #define K_MIN_PIN -1
      #endif
    #endif
  #elif K_HOME_TO_MIN
    #define K_STOP_PIN K_MIN_PIN
  #else
    #define K_STOP_PIN K_MAX_PIN
  #endif
#else
  #undef K_MIN_PIN
  #undef K_MAX_PIN
#endif

#if HAS_FILAMENT_SENSOR
  #define FIL_RUNOUT1_PIN FIL_RUNOUT_PIN  // Filament Sensor first pin alias
#else
  #undef FIL_RUNOUT_PIN
  #undef FIL_RUNOUT1_PIN
#endif

#if NUM_RUNOUT_SENSORS < 2
  #undef FIL_RUNOUT2_PIN
#endif

#ifndef LCD_PINS_D4
  #define LCD_PINS_D4 -1
#endif

#if HAS_MARLINUI_HD44780 || TOUCH_UI_ULTIPANEL
  #ifndef LCD_PINS_D5
    #define LCD_PINS_D5 -1
  #endif
  #ifndef LCD_PINS_D6
    #define LCD_PINS_D6 -1
  #endif
  #ifndef LCD_PINS_D7
    #define LCD_PINS_D7 -1
  #endif
#endif

/**
 * Auto-Assignment for Dual X, Dual Y, Multi-Z Steppers
 *
 * By default X2 is assigned to the next open E plug
 * on the board, then in order, Y2, Z2, Z3. These can be
 * overridden in Configuration.h or Configuration_adv.h.
 */

#define __PEXI(p,q) PIN_EXISTS(E##p##_##q)
#define _PEXI(p,q) __PEXI(p,q)
#define __EPIN(p,q) E##p##_##q##_PIN
#define _EPIN(p,q) __EPIN(p,q)
#define DIAG_REMAPPED(p,q) (PIN_EXISTS(q) && _EPIN(p##_E_INDEX, DIAG) == q##_PIN)
#define _En_DIAG_(p) _E##p##_DIAG_

// The E0/E1 steppers are always used for Dual E
#if ENABLED(E_DUAL_STEPPER_DRIVERS)
  #ifndef E1_STEP_PIN
    #error "No E1 stepper available for E_DUAL_STEPPER_DRIVERS!"
  #endif
  #define X2_E_INDEX INCREMENT(E_STEPPERS)
#else
  #define X2_E_INDEX E_STEPPERS
#endif

// The X2 axis, if any, should be the next open extruder port
#if HAS_X2_STEPPER && !defined(X2_DIAG_PIN) && !defined(X2_STEP_PIN) && !PIN_EXISTS(X2_CS_PIN)
  #define Y2_E_INDEX INCREMENT(X2_E_INDEX)
#else
  #define Y2_E_INDEX X2_E_INDEX
#endif
#if HAS_X2_STEPPER
  #ifndef X2_STEP_PIN
    #define X2_STEP_PIN   _EPIN(X2_E_INDEX, STEP)
    #define X2_DIR_PIN    _EPIN(X2_E_INDEX, DIR)
    #define X2_ENABLE_PIN _EPIN(X2_E_INDEX, ENABLE)
    #if X2_E_INDEX >= MAX_E_STEPPERS || !PIN_EXISTS(X2_STEP)
      #error "No E stepper plug left for X2!"
    #else
      #define AUTO_ASSIGNED_X2_STEPPER 1
    #endif
  #endif
  #ifndef X2_MS1_PIN
    #define X2_MS1_PIN    _EPIN(X2_E_INDEX, MS1)
    #if PIN_EXISTS(X2_MS1)
      #define AUTO_ASSIGNED_X2_MS1 1
    #endif
  #endif
  #ifndef X2_MS2_PIN
    #define X2_MS2_PIN    _EPIN(X2_E_INDEX, MS2)
    #if PIN_EXISTS(X2_MS2)
      #define AUTO_ASSIGNED_X2_MS2 1
    #endif
  #endif
  #ifndef X2_MS3_PIN
    #define X2_MS3_PIN    _EPIN(X2_E_INDEX, MS3)
    #if PIN_EXISTS(X2_MS3)
      #define AUTO_ASSIGNED_X2_MS3 1
    #endif
  #endif
  #if AXIS_HAS_SPI(X2) && !defined(X2_CS_PIN)
    #define X2_CS_PIN     _EPIN(X2_E_INDEX, CS)
    #if PIN_EXISTS(X2_CS)
      #define AUTO_ASSIGNED_X2_CS 1
    #endif
  #endif
  #if AXIS_HAS_UART(X2)
    #ifndef X2_SERIAL_TX_PIN
      #define X2_SERIAL_TX_PIN _EPIN(X2_E_INDEX, SERIAL_TX)
    #endif
    #ifndef X2_SERIAL_RX_PIN
      #define X2_SERIAL_RX_PIN _EPIN(X2_E_INDEX, SERIAL_RX)
    #endif
  #endif

  //
  // Auto-assign pins for stallGuard sensorless homing
  //
  #if !defined(X2_DIAG_PIN) && !defined(X2_USE_ENDSTOP) && defined(X2_STALL_SENSITIVITY) && ENABLED(X_DUAL_ENDSTOPS) && _PEXI(X2_E_INDEX, DIAG)
    #define X2_DIAG_PIN _EPIN(X2_E_INDEX, DIAG)
    #if   DIAG_REMAPPED(X2, X_MIN)      // If already remapped in the pins file...
      #define X2_USE_ENDSTOP _XMIN_
    #elif DIAG_REMAPPED(X2, Y_MIN)
      #define X2_USE_ENDSTOP _YMIN_
    #elif DIAG_REMAPPED(X2, Z_MIN)
      #define X2_USE_ENDSTOP _ZMIN_
    #elif DIAG_REMAPPED(X2, X_MAX)
      #define X2_USE_ENDSTOP _XMAX_
    #elif DIAG_REMAPPED(X2, Y_MAX)
      #define X2_USE_ENDSTOP _YMAX_
    #elif DIAG_REMAPPED(X2, Z_MAX)
      #define X2_USE_ENDSTOP _ZMAX_
    #else                               // Otherwise pick the next free En_DIAG_PIN directly
      #define X2_USE_ENDSTOP _En_DIAG_(X2_E_INDEX)
    #endif
    #define AUTO_ASSIGNED_X2_DIAG 1
    #undef X2_DIAG_PIN // Defined in Conditionals_post.h based on X2_USE_ENDSTOP
  #endif
#endif

#ifndef X2_CS_PIN
  #define X2_CS_PIN  -1
#endif
#ifndef X2_MS1_PIN
  #define X2_MS1_PIN -1
#endif
#ifndef X2_MS2_PIN
  #define X2_MS2_PIN -1
#endif
#ifndef X2_MS3_PIN
  #define X2_MS3_PIN -1
#endif

// The Y2 axis, if any, should be the next open extruder port
#if ENABLED(Y_DUAL_STEPPER_DRIVERS) && !defined(Y2_DIAG_PIN) && !defined(Y2_STEP_PIN) && !PIN_EXISTS(Y2_CS_PIN)
  #define Z2_E_INDEX INCREMENT(Y2_E_INDEX)
#else
  #define Z2_E_INDEX Y2_E_INDEX
#endif
#if ENABLED(Y_DUAL_STEPPER_DRIVERS)
  #ifndef Y2_STEP_PIN
    #define Y2_STEP_PIN   _EPIN(Y2_E_INDEX, STEP)
    #define Y2_DIR_PIN    _EPIN(Y2_E_INDEX, DIR)
    #define Y2_ENABLE_PIN _EPIN(Y2_E_INDEX, ENABLE)
    #if Y2_E_INDEX >= MAX_E_STEPPERS || !PIN_EXISTS(Y2_STEP)
      #error "No E stepper plug left for Y2!"
    #else
      #define AUTO_ASSIGNED_Y2_STEPPER 1
    #endif
  #endif
  #ifndef Y2_MS1_PIN
    #define Y2_MS1_PIN    _EPIN(Y2_E_INDEX, MS1)
    #if PIN_EXISTS(Y2_MS1)
      #define AUTO_ASSIGNED_Y2_MS1 1
    #endif
  #endif
  #ifndef Y2_MS2_PIN
    #define Y2_MS2_PIN    _EPIN(Y2_E_INDEX, MS2)
    #if PIN_EXISTS(Y2_MS2)
      #define AUTO_ASSIGNED_Y2_MS2 1
    #endif
  #endif
  #ifndef Y2_MS3_PIN
    #define Y2_MS3_PIN    _EPIN(Y2_E_INDEX, MS3)
    #if PIN_EXISTS(Y2_MS3)
      #define AUTO_ASSIGNED_Y2_MS3 1
    #endif
  #endif
  #if AXIS_HAS_SPI(Y2) && !defined(Y2_CS_PIN)
    #define Y2_CS_PIN     _EPIN(Y2_E_INDEX, CS)
    #if PIN_EXISTS(Y2_CS)
      #define AUTO_ASSIGNED_Y2_CS 1
    #endif
  #endif
  #if AXIS_HAS_UART(Y2)
    #ifndef Y2_SERIAL_TX_PIN
      #define Y2_SERIAL_TX_PIN _EPIN(Y2_E_INDEX, SERIAL_TX)
    #endif
    #ifndef Y2_SERIAL_RX_PIN
      #define Y2_SERIAL_RX_PIN _EPIN(Y2_E_INDEX, SERIAL_RX)
    #endif
  #endif
  // Auto-assign pins for stallGuard sensorless homing
  #if !defined(Y2_DIAG_PIN) && !defined(Y2_USE_ENDSTOP) && defined(Y2_STALL_SENSITIVITY) && ENABLED(Y_DUAL_ENDSTOPS) && _PEXI(Y2_E_INDEX, DIAG)
    #define Y2_DIAG_PIN _EPIN(Y2_E_INDEX, DIAG)
    #if   DIAG_REMAPPED(Y2, X_MIN)
      #define Y2_USE_ENDSTOP _XMIN_
    #elif DIAG_REMAPPED(Y2, Y_MIN)
      #define Y2_USE_ENDSTOP _YMIN_
    #elif DIAG_REMAPPED(Y2, Z_MIN)
      #define Y2_USE_ENDSTOP _ZMIN_
    #elif DIAG_REMAPPED(Y2, X_MAX)
      #define Y2_USE_ENDSTOP _XMAX_
    #elif DIAG_REMAPPED(Y2, Y_MAX)
      #define Y2_USE_ENDSTOP _YMAX_
    #elif DIAG_REMAPPED(Y2, Z_MAX)
      #define Y2_USE_ENDSTOP _ZMAX_
    #else
      #define Y2_USE_ENDSTOP _En_DIAG_(Y2_E_INDEX)
    #endif
    #define AUTO_ASSIGNED_Y2_DIAG 1
    #undef Y2_DIAG_PIN // Defined in Conditionals_post.h based on Y2_USE_ENDSTOP
  #endif
#endif

#ifndef Y2_CS_PIN
  #define Y2_CS_PIN  -1
#endif
#ifndef Y2_MS1_PIN
  #define Y2_MS1_PIN -1
#endif
#ifndef Y2_MS2_PIN
  #define Y2_MS2_PIN -1
#endif
#ifndef Y2_MS3_PIN
  #define Y2_MS3_PIN -1
#endif

// The Z2 axis, if any, should be the next open extruder port
#if NUM_Z_STEPPER_DRIVERS >= 2 && !defined(Z2_DIAG_PIN) && !defined(Z2_STEP_PIN) && !PIN_EXISTS(Z2_CS_PIN)
  #define Z3_E_INDEX INCREMENT(Z2_E_INDEX)
#else
  #define Z3_E_INDEX Z2_E_INDEX
#endif
#if NUM_Z_STEPPER_DRIVERS >= 2
  #ifndef Z2_STEP_PIN
    #define Z2_STEP_PIN   _EPIN(Z2_E_INDEX, STEP)
    #define Z2_DIR_PIN    _EPIN(Z2_E_INDEX, DIR)
    #define Z2_ENABLE_PIN _EPIN(Z2_E_INDEX, ENABLE)
    #if Z2_E_INDEX >= MAX_E_STEPPERS || !PIN_EXISTS(Z2_STEP)
      #error "No E stepper plug left for Z2!"
    #else
      #define AUTO_ASSIGNED_Z2_STEPPER 1
    #endif
  #endif
  #ifndef Z2_MS1_PIN
    #define Z2_MS1_PIN    _EPIN(Z2_E_INDEX, MS1)
    #if PIN_EXISTS(Z2_MS1)
      #define AUTO_ASSIGNED_Z2_MS1 1
    #endif
  #endif
  #ifndef Z2_MS2_PIN
    #define Z2_MS2_PIN    _EPIN(Z2_E_INDEX, MS2)
    #if PIN_EXISTS(Z2_MS2)
      #define AUTO_ASSIGNED_Z2_MS2 1
    #endif
  #endif
  #ifndef Z2_MS3_PIN
    #define Z2_MS3_PIN    _EPIN(Z2_E_INDEX, MS3)
    #if PIN_EXISTS(Z2_MS3)
      #define AUTO_ASSIGNED_Z2_MS3 1
    #endif
  #endif
  #if AXIS_HAS_SPI(Z2) && !defined(Z2_CS_PIN)
    #define Z2_CS_PIN     _EPIN(Z2_E_INDEX, CS)
    #if PIN_EXISTS(Z2_CS)
      #define AUTO_ASSIGNED_Z2_CS 1
    #endif
  #endif
  #if AXIS_HAS_UART(Z2)
    #ifndef Z2_SERIAL_TX_PIN
      #define Z2_SERIAL_TX_PIN _EPIN(Z2_E_INDEX, SERIAL_TX)
    #endif
    #ifndef Z2_SERIAL_RX_PIN
      #define Z2_SERIAL_RX_PIN _EPIN(Z2_E_INDEX, SERIAL_RX)
    #endif
  #endif
  // Auto-assign pins for stallGuard sensorless homing
  #if !defined(Z2_DIAG_PIN) && !defined(Z2_USE_ENDSTOP) && defined(Z2_STALL_SENSITIVITY) && ENABLED(Z_MULTI_ENDSTOPS) && _PEXI(Z2_E_INDEX, DIAG)
    #define Z2_DIAG_PIN _EPIN(Z2_E_INDEX, DIAG)
    #if   DIAG_REMAPPED(Z2, X_MIN)
      #define Z2_USE_ENDSTOP _XMIN_
    #elif DIAG_REMAPPED(Z2, Y_MIN)
      #define Z2_USE_ENDSTOP _YMIN_
    #elif DIAG_REMAPPED(Z2, Z_MIN)
      #define Z2_USE_ENDSTOP _ZMIN_
    #elif DIAG_REMAPPED(Z2, X_MAX)
      #define Z2_USE_ENDSTOP _XMAX_
    #elif DIAG_REMAPPED(Z2, Y_MAX)
      #define Z2_USE_ENDSTOP _YMAX_
    #elif DIAG_REMAPPED(Z2, Z_MAX)
      #define Z2_USE_ENDSTOP _ZMAX_
    #else
      #define Z2_USE_ENDSTOP _En_DIAG_(Z2_E_INDEX)
    #endif
    #define AUTO_ASSIGNED_Z2_DIAG 1
    #undef Z2_DIAG_PIN // Defined in Conditionals_post.h based on Z2_USE_ENDSTOP
  #endif
#endif

#ifndef Z2_CS_PIN
  #define Z2_CS_PIN  -1
#endif
#ifndef Z2_MS1_PIN
  #define Z2_MS1_PIN -1
#endif
#ifndef Z2_MS2_PIN
  #define Z2_MS2_PIN -1
#endif
#ifndef Z2_MS3_PIN
  #define Z2_MS3_PIN -1
#endif

// The Z3 axis, if any, should be the next open extruder port
#if NUM_Z_STEPPER_DRIVERS >= 3 && !defined(Z3_DIAG_PIN) && !defined(Z3_STEP_PIN) && !PIN_EXISTS(Z3_CS_PIN)
  #define Z4_E_INDEX INCREMENT(Z3_E_INDEX)
#else
  #define Z4_E_INDEX Z3_E_INDEX
#endif
#if NUM_Z_STEPPER_DRIVERS >= 3
  #ifndef Z3_STEP_PIN
    #define Z3_STEP_PIN   _EPIN(Z3_E_INDEX, STEP)
    #define Z3_DIR_PIN    _EPIN(Z3_E_INDEX, DIR)
    #define Z3_ENABLE_PIN _EPIN(Z3_E_INDEX, ENABLE)
    #if Z3_E_INDEX >= MAX_E_STEPPERS || !PIN_EXISTS(Z3_STEP)
      #error "No E stepper plug left for Z3!"
    #else
      #define AUTO_ASSIGNED_Z3_STEPPER 1
    #endif
  #endif
  #if AXIS_HAS_SPI(Z3) && !defined(Z3_CS_PIN)
    #define Z3_CS_PIN     _EPIN(Z3_E_INDEX, CS)
    #if PIN_EXISTS(Z3_CS)
      #define AUTO_ASSIGNED_Z3_CS 1
    #endif
  #endif
  #ifndef Z3_MS1_PIN
    #define Z3_MS1_PIN    _EPIN(Z3_E_INDEX, MS1)
    #if PIN_EXISTS(Z3_MS1)
      #define AUTO_ASSIGNED_Z3_MS1 1
    #endif
  #endif
  #ifndef Z3_MS2_PIN
    #define Z3_MS2_PIN    _EPIN(Z3_E_INDEX, MS2)
    #if PIN_EXISTS(Z3_MS2)
      #define AUTO_ASSIGNED_Z3_MS2 1
    #endif
  #endif
  #ifndef Z3_MS3_PIN
    #define Z3_MS3_PIN    _EPIN(Z3_E_INDEX, MS3)
    #if PIN_EXISTS(Z3_MS3)
      #define AUTO_ASSIGNED_Z3_MS3 1
    #endif
  #endif
  #if AXIS_HAS_UART(Z3)
    #ifndef Z3_SERIAL_TX_PIN
      #define Z3_SERIAL_TX_PIN _EPIN(Z3_E_INDEX, SERIAL_TX)
    #endif
    #ifndef Z3_SERIAL_RX_PIN
      #define Z3_SERIAL_RX_PIN _EPIN(Z3_E_INDEX, SERIAL_RX)
    #endif
  #endif
  // Auto-assign pins for stallGuard sensorless homing
  #if !defined(Z3_DIAG_PIN) && !defined(Z3_USE_ENDSTOP) && defined(Z3_STALL_SENSITIVITY) && ENABLED(Z_MULTI_ENDSTOPS) && _PEXI(Z3_E_INDEX, DIAG)
    #define Z3_DIAG_PIN _EPIN(Z3_E_INDEX, DIAG)
    #if   DIAG_REMAPPED(Z3, X_MIN)
      #define Z3_USE_ENDSTOP _XMIN_
    #elif DIAG_REMAPPED(Z3, Y_MIN)
      #define Z3_USE_ENDSTOP _YMIN_
    #elif DIAG_REMAPPED(Z3, Z_MIN)
      #define Z3_USE_ENDSTOP _ZMIN_
    #elif DIAG_REMAPPED(Z3, X_MAX)
      #define Z3_USE_ENDSTOP _XMAX_
    #elif DIAG_REMAPPED(Z3, Y_MAX)
      #define Z3_USE_ENDSTOP _YMAX_
    #elif DIAG_REMAPPED(Z3, Z_MAX)
      #define Z3_USE_ENDSTOP _ZMAX_
    #else
      #define Z3_USE_ENDSTOP _En_DIAG_(Z3_E_INDEX)
    #endif
    #define AUTO_ASSIGNED_Z3_DIAG 1
    #undef Z3_DIAG_PIN // Defined in Conditionals_post.h based on Z3_USE_ENDSTOP
  #endif
#endif

#ifndef Z3_CS_PIN
  #define Z3_CS_PIN  -1
#endif
#ifndef Z3_MS1_PIN
  #define Z3_MS1_PIN -1
#endif
#ifndef Z3_MS2_PIN
  #define Z3_MS2_PIN -1
#endif
#ifndef Z3_MS3_PIN
  #define Z3_MS3_PIN -1
#endif

// The Z4 axis, if any, should be the next open extruder port
#if NUM_Z_STEPPER_DRIVERS >= 4 && !defined(Z4_DIAG_PIN) && !defined(Z4_STEP_PIN) && !PIN_EXISTS(Z4_CS_PIN)
  #define I_E_INDEX INCREMENT(Z4_E_INDEX)
#else
  #define I_E_INDEX Z4_E_INDEX
#endif
#if NUM_Z_STEPPER_DRIVERS >= 4
  #ifndef Z4_STEP_PIN
    #define Z4_STEP_PIN   _EPIN(Z4_E_INDEX, STEP)
    #define Z4_DIR_PIN    _EPIN(Z4_E_INDEX, DIR)
    #define Z4_ENABLE_PIN _EPIN(Z4_E_INDEX, ENABLE)
    #if Z4_E_INDEX >= MAX_E_STEPPERS || !PIN_EXISTS(Z4_STEP)
      #error "No E stepper plug left for Z4!"
    #else
      #define AUTO_ASSIGNED_Z4_STEPPER 1
    #endif
  #endif
  #if AXIS_HAS_SPI(Z4) && !defined(Z4_CS_PIN)
    #define Z4_CS_PIN     _EPIN(Z4_E_INDEX, CS)
    #if PIN_EXISTS(Z4_CS)
      #define AUTO_ASSIGNED_Z4_CS 1
    #endif
  #endif
  #ifndef Z4_MS1_PIN
    #define Z4_MS1_PIN    _EPIN(Z4_E_INDEX, MS1)
    #if PIN_EXISTS(Z4_MS1)
      #define AUTO_ASSIGNED_Z4_MS1 1
    #endif
  #endif
  #ifndef Z4_MS2_PIN
    #define Z4_MS2_PIN    _EPIN(Z4_E_INDEX, MS2)
    #if PIN_EXISTS(Z4_MS2)
      #define AUTO_ASSIGNED_Z4_MS2 1
    #endif
  #endif
  #ifndef Z4_MS3_PIN
    #define Z4_MS3_PIN    _EPIN(Z4_E_INDEX, MS3)
    #if PIN_EXISTS(Z4_MS3)
      #define AUTO_ASSIGNED_Z4_MS3 1
    #endif
  #endif
  #if AXIS_HAS_UART(Z4)
    #ifndef Z4_SERIAL_TX_PIN
      #define Z4_SERIAL_TX_PIN _EPIN(Z4_E_INDEX, SERIAL_TX)
    #endif
    #ifndef Z4_SERIAL_RX_PIN
      #define Z4_SERIAL_RX_PIN _EPIN(Z4_E_INDEX, SERIAL_RX)
    #endif
  #endif
  // Auto-assign pins for stallGuard sensorless homing
  #if !defined(Z4_DIAG_PIN) && !defined(Z4_USE_ENDSTOP) && defined(Z4_STALL_SENSITIVITY) && ENABLED(Z_MULTI_ENDSTOPS) && _PEXI(Z4_E_INDEX, DIAG)
    #define Z4_DIAG_PIN _EPIN(Z4_E_INDEX, DIAG)
    #if   DIAG_REMAPPED(Z4, X_MIN)
      #define Z4_USE_ENDSTOP _XMIN_
    #elif DIAG_REMAPPED(Z4, Y_MIN)
      #define Z4_USE_ENDSTOP _YMIN_
    #elif DIAG_REMAPPED(Z4, Z_MIN)
      #define Z4_USE_ENDSTOP _ZMIN_
    #elif DIAG_REMAPPED(Z4, X_MAX)
      #define Z4_USE_ENDSTOP _XMAX_
    #elif DIAG_REMAPPED(Z4, Y_MAX)
      #define Z4_USE_ENDSTOP _YMAX_
    #elif DIAG_REMAPPED(Z4, Z_MAX)
      #define Z4_USE_ENDSTOP _ZMAX_
    #else
      #define Z4_USE_ENDSTOP _En_DIAG_(Z4_E_INDEX)
    #endif
    #define AUTO_ASSIGNED_Z4_DIAG 1
    #undef Z4_DIAG_PIN // Defined in Conditionals_post.h based on Z4_USE_ENDSTOP
  #endif
#endif

#ifndef Z4_CS_PIN
  #define Z4_CS_PIN  -1
#endif
#ifndef Z4_MS1_PIN
  #define Z4_MS1_PIN -1
#endif
#ifndef Z4_MS2_PIN
  #define Z4_MS2_PIN -1
#endif
#ifndef Z4_MS3_PIN
  #define Z4_MS3_PIN -1
#endif

// The I axis, if any, should be the next open extruder port
#if HAS_I_AXIS && !defined(I_DIAG_PIN) && !defined(I_STEP_PIN) && !PIN_EXISTS(I_CS_PIN)
  #define J_E_INDEX INCREMENT(I_E_INDEX)
#else
  #define J_E_INDEX I_E_INDEX
#endif
#if HAS_I_AXIS
  #ifndef I_STEP_PIN
    #define I_STEP_PIN   _EPIN(I_E_INDEX, STEP)
    #define I_DIR_PIN    _EPIN(I_E_INDEX, DIR)
    #define I_ENABLE_PIN _EPIN(I_E_INDEX, ENABLE)
    #if I_E_INDEX >= MAX_E_STEPPERS || !PIN_EXISTS(I_STEP)
      #error "No E stepper plug left for I!"
    #else
      #define AUTO_ASSIGNED_I_STEPPER 1
    #endif
  #endif
  #if AXIS_HAS_SPI(I) && !defined(I_CS_PIN)
    #define I_CS_PIN     _EPIN(I_E_INDEX, CS)
    #if PIN_EXISTS(I_CS)
      #define AUTO_ASSIGNED_I_CS 1
    #endif
  #endif
  #ifndef I_MS1_PIN
    #define I_MS1_PIN    _EPIN(I_E_INDEX, MS1)
    #if PIN_EXISTS(I_MS1)
      #define AUTO_ASSIGNED_I_MS1 1
    #endif
  #endif
  #ifndef I_MS2_PIN
    #define I_MS2_PIN    _EPIN(I_E_INDEX, MS2)
    #if PIN_EXISTS(I_MS2)
      #define AUTO_ASSIGNED_I_MS2 1
    #endif
  #endif
  #ifndef I_MS3_PIN
    #define I_MS3_PIN    _EPIN(I_E_INDEX, MS3)
    #if PIN_EXISTS(I_MS3)
      #define AUTO_ASSIGNED_I_MS3 1
    #endif
  #endif
  #if AXIS_HAS_UART(I)
    #ifndef I_SERIAL_TX_PIN
      #define I_SERIAL_TX_PIN _EPIN(I_E_INDEX, SERIAL_TX)
    #endif
    #ifndef I_SERIAL_RX_PIN
      #define I_SERIAL_RX_PIN _EPIN(I_E_INDEX, SERIAL_RX)
    #endif
  #endif
  // Auto-assign pins for stallGuard sensorless homing
  #if !defined(I_DIAG_PIN) && !defined(I_USE_ENDSTOP) && defined(I_STALL_SENSITIVITY) && _PEXI(I_E_INDEX, DIAG)
    #define I_DIAG_PIN _EPIN(I_E_INDEX, DIAG)
    #if   DIAG_REMAPPED(I, X_MIN)
      #define I_USE_ENDSTOP _XMIN_
    #elif DIAG_REMAPPED(I, Y_MIN)
      #define I_USE_ENDSTOP _YMIN_
    #elif DIAG_REMAPPED(I, Z_MIN)
      #define I_USE_ENDSTOP _ZMIN_
    #elif DIAG_REMAPPED(I, X_MAX)
      #define I_USE_ENDSTOP _XMAX_
    #elif DIAG_REMAPPED(I, Y_MAX)
      #define I_USE_ENDSTOP _YMAX_
    #elif DIAG_REMAPPED(I, Z_MAX)
      #define I_USE_ENDSTOP _ZMAX_
    #else
      #define I_USE_ENDSTOP _En_DIAG_(I_E_INDEX)
    #endif
    #define AUTO_ASSIGNED_I_DIAG 1
    #undef I_DIAG_PIN // Defined in Conditionals_post.h based on I_USE_ENDSTOP
  #endif
#endif

#ifndef I_CS_PIN
  #define I_CS_PIN  -1
#endif
#ifndef I_MS1_PIN
  #define I_MS1_PIN -1
#endif
#ifndef I_MS2_PIN
  #define I_MS2_PIN -1
#endif
#ifndef I_MS3_PIN
  #define I_MS3_PIN -1
#endif

// The J axis, if any, should be the next open extruder port
#if HAS_J_AXIS && !defined(J_DIAG_PIN) && !defined(J_STEP_PIN) && !PIN_EXISTS(J_CS_PIN)
  #define K_E_INDEX INCREMENT(J_E_INDEX)
#else
  #define K_E_INDEX J_E_INDEX
#endif
#if HAS_J_AXIS
  #ifndef J_STEP_PIN
    #define J_STEP_PIN   _EPIN(J_E_INDEX, STEP)
    #define J_DIR_PIN    _EPIN(J_E_INDEX, DIR)
    #define J_ENABLE_PIN _EPIN(J_E_INDEX, ENABLE)
    #if I_E_INDEX >= MAX_E_STEPPERS || !PIN_EXISTS(J_STEP)
      #error "No E stepper plug left for J!"
    #else
      #define AUTO_ASSIGNED_J_STEPPER 1
    #endif
  #endif
  #if AXIS_HAS_SPI(J) && !defined(J_CS_PIN)
    #define J_CS_PIN     _EPIN(J_E_INDEX, CS)
    #if PIN_EXISTS(J_CS)
      #define AUTO_ASSIGNED_J_CS 1
    #endif
  #endif
  #ifndef J_MS1_PIN
    #define J_MS1_PIN    _EPIN(J_E_INDEX, MS1)
    #if PIN_EXISTS(J_MS1)
      #define AUTO_ASSIGNED_J_MS1 1
    #endif
  #endif
  #ifndef J_MS2_PIN
    #define J_MS2_PIN    _EPIN(J_E_INDEX, MS2)
    #if PIN_EXISTS(J_MS2)
      #define AUTO_ASSIGNED_J_MS2 1
    #endif
  #endif
  #ifndef J_MS3_PIN
    #define J_MS3_PIN    _EPIN(J_E_INDEX, MS3)
    #if PIN_EXISTS(J_MS3)
      #define AUTO_ASSIGNED_J_MS3 1
    #endif
  #endif
  #if AXIS_HAS_UART(J)
    #ifndef J_SERIAL_TX_PIN
      #define J_SERIAL_TX_PIN _EPIN(J_E_INDEX, SERIAL_TX)
    #endif
    #ifndef J_SERIAL_RX_PIN
      #define J_SERIAL_RX_PIN _EPIN(J_E_INDEX, SERIAL_RX)
    #endif
  #endif
  // Auto-assign pins for stallGuard sensorless homing
  #if !defined(J_DIAG_PIN) && !defined(J_USE_ENDSTOP) && defined(J_STALL_SENSITIVITY) && _PEXI(J_E_INDEX, DIAG)
    #define J_DIAG_PIN _EPIN(J_E_INDEX, DIAG)
    #if   DIAG_REMAPPED(J, X_MIN)
      #define J_USE_ENDSTOP _XMIN_
    #elif DIAG_REMAPPED(J, Y_MIN)
      #define J_USE_ENDSTOP _YMIN_
    #elif DIAG_REMAPPED(J, Z_MIN)
      #define J_USE_ENDSTOP _ZMIN_
    #elif DIAG_REMAPPED(J, X_MAX)
      #define J_USE_ENDSTOP _XMAX_
    #elif DIAG_REMAPPED(J, Y_MAX)
      #define J_USE_ENDSTOP _YMAX_
    #elif DIAG_REMAPPED(I, Z_MAX)
      #define J_USE_ENDSTOP _ZMAX_
    #else
      #define J_USE_ENDSTOP _En_DIAG_(J_E_INDEX)
    #endif
    #define AUTO_ASSIGNED_J_DIAG 1
    #undef J_DIAG_PIN // Defined in Conditionals_post.h based on J_USE_ENDSTOP
  #endif
#endif

#ifndef J_CS_PIN
  #define J_CS_PIN  -1
#endif
#ifndef J_MS1_PIN
  #define J_MS1_PIN -1
#endif
#ifndef J_MS2_PIN
  #define J_MS2_PIN -1
#endif
#ifndef J_MS3_PIN
  #define J_MS3_PIN -1
#endif

// The K axis, if any, should be the next open extruder port
#if HAS_K_AXIS
  #ifndef K_STEP_PIN
    #define K_STEP_PIN   _EPIN(K_E_INDEX, STEP)
    #define K_DIR_PIN    _EPIN(K_E_INDEX, DIR)
    #define K_ENABLE_PIN _EPIN(K_E_INDEX, ENABLE)
    #if K_E_INDEX >= MAX_E_STEPPERS || !PIN_EXISTS(K_STEP)
      #error "No E stepper plug left for K!"
    #else
      #define AUTO_ASSIGNED_K_STEPPER 1
    #endif
  #endif
  #if AXIS_HAS_SPI(K) && !defined(K_CS_PIN)
    #define K_CS_PIN     _EPIN(K_E_INDEX, CS)
    #if PIN_EXISTS(K_CS)
      #define AUTO_ASSIGNED_K_CS 1
    #endif
  #endif
  #ifndef K_MS1_PIN
    #define K_MS1_PIN    _EPIN(K_E_INDEX, MS1)
    #if PIN_EXISTS(K_MS1)
      #define AUTO_ASSIGNED_K_MS1 1
    #endif
  #endif
  #ifndef K_MS2_PIN
    #define K_MS2_PIN    _EPIN(K_E_INDEX, MS2)
    #if PIN_EXISTS(K_MS2)
      #define AUTO_ASSIGNED_K_MS2 1
    #endif
  #endif
  #ifndef K_MS3_PIN
    #define K_MS3_PIN    _EPIN(K_E_INDEX, MS3)
    #if PIN_EXISTS(K_MS3)
      #define AUTO_ASSIGNED_K_MS3 1
    #endif
  #endif
  #if AXIS_HAS_UART(K)
    #ifndef K_SERIAL_TX_PIN
      #define K_SERIAL_TX_PIN _EPIN(K_E_INDEX, SERIAL_TX)
    #endif
    #ifndef K_SERIAL_RX_PIN
      #define K_SERIAL_RX_PIN _EPIN(K_E_INDEX, SERIAL_RX)
    #endif
  #endif
  // Auto-assign pins for stallGuard sensorless homing
  #if !defined(K_DIAG_PIN) && !defined(K_USE_ENDSTOP) && defined(K_STALL_SENSITIVITY) && _PEXI(K_E_INDEX, DIAG)
    #define K_DIAG_PIN _EPIN(K_E_INDEX, DIAG)
    #if   DIAG_REMAPPED(K, X_MIN)
      #define K_USE_ENDSTOP _XMIN_
    #elif DIAG_REMAPPED(K, Y_MIN)
      #define K_USE_ENDSTOP _YMIN_
    #elif DIAG_REMAPPED(K, Z_MIN)
      #define K_USE_ENDSTOP _ZMIN_
    #elif DIAG_REMAPPED(K, X_MAX)
      #define K_USE_ENDSTOP _XMAX_
    #elif DIAG_REMAPPED(K, Y_MAX)
      #define K_USE_ENDSTOP _YMAX_
    #elif DIAG_REMAPPED(K, Z_MAX)
      #define K_USE_ENDSTOP _ZMAX_
    #else
      #define K_USE_ENDSTOP _En_DIAG_(K_E_INDEX)
    #endif
    #define AUTO_ASSIGNED_K_DIAG 1
    #undef K_DIAG_PIN // Defined in Conditionals_post.h based on K_USE_ENDSTOP
  #endif
#endif

#ifndef K_CS_PIN
  #define K_CS_PIN  -1
#endif
#ifndef K_MS1_PIN
  #define K_MS1_PIN -1
#endif
#ifndef K_MS2_PIN
  #define K_MS2_PIN -1
#endif
#ifndef K_MS3_PIN
  #define K_MS3_PIN -1
#endif

//
// Disable unused endstop / probe pins
//
#define _STOP_IN_USE(N) (X2_USE_ENDSTOP == N || Y2_USE_ENDSTOP == N || Z2_USE_ENDSTOP == N || Z3_USE_ENDSTOP == N || Z4_USE_ENDSTOP == N)
#if !defined(USE_XMAX_PLUG) && _STOP_IN_USE(_XMAX_)
  #define USE_XMAX_PLUG
#endif
#if !defined(USE_YMAX_PLUG) && _STOP_IN_USE(_YMAX_)
  #define USE_YMAX_PLUG
#endif
#if !defined(USE_ZMAX_PLUG) && _STOP_IN_USE(_ZMAX_)
  #define USE_ZMAX_PLUG
#endif
#if !defined(USE_XMIN_PLUG) && _STOP_IN_USE(_XMIN_)
  #define USE_XMIN_PLUG
#endif
#if !defined(USE_YMIN_PLUG) && _STOP_IN_USE(_YMIN_)
  #define USE_YMIN_PLUG
#endif
#if !defined(USE_ZMIN_PLUG) && _STOP_IN_USE(_ZMIN_)
  #define USE_ZMIN_PLUG
#endif
#undef _STOP_IN_USE
#if !USES_Z_MIN_PROBE_PIN
  #undef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN    -1
#endif
#if DISABLED(USE_XMIN_PLUG)
  #undef X_MIN_PIN
  #define X_MIN_PIN          -1
#endif
#if DISABLED(USE_XMAX_PLUG)
  #undef X_MAX_PIN
  #define X_MAX_PIN          -1
#endif
#if DISABLED(USE_YMIN_PLUG)
  #undef Y_MIN_PIN
  #define Y_MIN_PIN          -1
#endif
#if DISABLED(USE_YMAX_PLUG)
  #undef Y_MAX_PIN
  #define Y_MAX_PIN          -1
#endif
#if DISABLED(USE_ZMIN_PLUG)
  #undef Z_MIN_PIN
  #define Z_MIN_PIN          -1
#endif
#if DISABLED(USE_ZMAX_PLUG)
  #undef Z_MAX_PIN
  #define Z_MAX_PIN          -1
#endif
#if DISABLED(USE_IMIN_PLUG)
  #undef I_MIN_PIN
  #define I_MIN_PIN          -1
#endif
#if DISABLED(USE_IMAX_PLUG)
  #undef I_MAX_PIN
  #define I_MAX_PIN          -1
#endif
#if DISABLED(USE_JMIN_PLUG)
  #undef J_MIN_PIN
  #define J_MIN_PIN          -1
#endif
#if DISABLED(USE_JMAX_PLUG)
  #undef J_MAX_PIN
  #define J_MAX_PIN          -1
#endif
#if DISABLED(USE_KMIN_PLUG)
  #undef K_MIN_PIN
  #define K_MIN_PIN          -1
#endif
#if DISABLED(USE_KMAX_PLUG)
  #undef K_MAX_PIN
  #define K_MAX_PIN          -1
#endif

#if DISABLED(X_DUAL_ENDSTOPS) || X_HOME_TO_MAX
  #undef X2_MIN_PIN
#endif
#if DISABLED(X_DUAL_ENDSTOPS) || X_HOME_TO_MIN
  #undef X2_MAX_PIN
#endif
#if DISABLED(Y_DUAL_ENDSTOPS) || Y_HOME_TO_MAX
  #undef Y2_MIN_PIN
#endif
#if DISABLED(Y_DUAL_ENDSTOPS) || Y_HOME_TO_MIN
  #undef Y2_MAX_PIN
#endif
#if DISABLED(Z_MULTI_ENDSTOPS) || Z_HOME_TO_MAX
  #undef Z2_MIN_PIN
#endif
#if DISABLED(Z_MULTI_ENDSTOPS) || Z_HOME_TO_MIN
  #undef Z2_MAX_PIN
#endif
#if DISABLED(Z_MULTI_ENDSTOPS) || NUM_Z_STEPPER_DRIVERS < 3 || Z_HOME_TO_MAX
  #undef Z3_MIN_PIN
#endif
#if DISABLED(Z_MULTI_ENDSTOPS) || NUM_Z_STEPPER_DRIVERS < 3 || Z_HOME_TO_MIN
  #undef Z3_MAX_PIN
#endif
#if DISABLED(Z_MULTI_ENDSTOPS) || NUM_Z_STEPPER_DRIVERS < 4 || Z_HOME_TO_MAX
  #undef Z4_MIN_PIN
#endif
#if DISABLED(Z_MULTI_ENDSTOPS) || NUM_Z_STEPPER_DRIVERS < 4 || Z_HOME_TO_MIN
  #undef Z4_MAX_PIN
#endif

//
// Default DOGLCD SPI delays
//
#if !IS_U8GLIB_ST7920
  #undef ST7920_DELAY_1
  #undef ST7920_DELAY_2
  #undef ST7920_DELAY_3
  #undef LCD_ST7920_DELAY_1
  #undef LCD_ST7920_DELAY_2
  #undef LCD_ST7920_DELAY_3
  #undef BOARD_ST7920_DELAY_1
  #undef BOARD_ST7920_DELAY_2
  #undef BOARD_ST7920_DELAY_3
  #undef CPU_ST7920_DELAY_1
  #undef CPU_ST7920_DELAY_2
  #undef CPU_ST7920_DELAY_3
#endif

#if !NEED_CASE_LIGHT_PIN
  #undef CASE_LIGHT_PIN
#endif

#undef HAS_FREE_AUX2_PINS
#undef DIAG_REMAPPED
