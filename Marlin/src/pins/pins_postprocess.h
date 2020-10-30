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
#ifndef SUICIDE_PIN_INVERTING
  #define SUICIDE_PIN_INVERTING false
#endif

#ifndef NUM_SERVO_PLUGS
  #define NUM_SERVO_PLUGS 4
#endif

//
// Assign endstop pins for boards with only 3 connectors
//
#ifdef X_STOP_PIN
  #if X_HOME_DIR < 0
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
#elif X_HOME_DIR < 0
  #define X_STOP_PIN X_MIN_PIN
#else
  #define X_STOP_PIN X_MAX_PIN
#endif

#ifdef Y_STOP_PIN
  #if Y_HOME_DIR < 0
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
#elif Y_HOME_DIR < 0
  #define Y_STOP_PIN Y_MIN_PIN
#else
  #define Y_STOP_PIN Y_MAX_PIN
#endif

#ifdef Z_STOP_PIN
  #if Z_HOME_DIR < 0
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
#elif Z_HOME_DIR < 0
  #define Z_STOP_PIN Z_MIN_PIN
#else
  #define Z_STOP_PIN Z_MAX_PIN
#endif

//
// Disable unused endstop / probe pins
//
#if !HAS_CUSTOM_PROBE_PIN
  #undef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN    -1
#endif

#if DISABLED(USE_XMAX_PLUG)
  #undef X_MAX_PIN
  #define X_MAX_PIN          -1
#endif

#if DISABLED(USE_YMAX_PLUG)
  #undef Y_MAX_PIN
  #define Y_MAX_PIN          -1
#endif

#if DISABLED(USE_ZMAX_PLUG)
  #undef Z_MAX_PIN
  #define Z_MAX_PIN          -1
#endif

#if DISABLED(USE_XMIN_PLUG)
  #undef X_MIN_PIN
  #define X_MIN_PIN          -1
#endif

#if DISABLED(USE_YMIN_PLUG)
  #undef Y_MIN_PIN
  #define Y_MIN_PIN          -1
#endif

#if DISABLED(USE_ZMIN_PLUG)
  #undef Z_MIN_PIN
  #define Z_MIN_PIN          -1
#endif

#if HAS_FILAMENT_SENSOR
  #define FIL_RUNOUT1_PIN FIL_RUNOUT_PIN
#else
  #undef FIL_RUNOUT_PIN
  #undef FIL_RUNOUT1_PIN
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

// The X2 axis, if any, should be the next open extruder port
#define X2_E_INDEX E_STEPPERS

#if EITHER(DUAL_X_CARRIAGE, X_DUAL_STEPPER_DRIVERS)
  #ifndef X2_STEP_PIN
    #define X2_STEP_PIN   _EPIN(X2_E_INDEX, STEP)
    #define X2_DIR_PIN    _EPIN(X2_E_INDEX, DIR)
    #define X2_ENABLE_PIN _EPIN(X2_E_INDEX, ENABLE)
    #if X2_E_INDEX >= MAX_EXTRUDERS || !PIN_EXISTS(X2_STEP)
      #error "No E stepper plug left for X2!"
    #endif
  #endif
  #ifndef X2_MS1_PIN
    #define X2_MS1_PIN    _EPIN(X2_E_INDEX, MS1)
  #endif
  #ifndef X2_MS2_PIN
    #define X2_MS2_PIN    _EPIN(X2_E_INDEX, MS2)
  #endif
  #ifndef X2_MS3_PIN
    #define X2_MS3_PIN    _EPIN(X2_E_INDEX, MS3)
  #endif
  #if AXIS_HAS_SPI(X2) && !defined(X2_CS_PIN)
    #define X2_CS_PIN     _EPIN(X2_E_INDEX, CS)
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
  #if defined(X2_STALL_SENSITIVITY) && ENABLED(X_DUAL_ENDSTOPS) && _PEXI(X2_E_INDEX, DIAG)
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
    #else                               // Otherwise use the driver DIAG_PIN directly
      #define _X2_USE_ENDSTOP(P) _E##P##_DIAG_
      #define X2_USE_ENDSTOP _X2_USE_ENDSTOP(X2_E_INDEX)
    #endif
    #undef X2_DIAG_PIN
  #endif

  #define Y2_E_INDEX INCREMENT(X2_E_INDEX)
#else
  #define Y2_E_INDEX X2_E_INDEX
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
#if ENABLED(Y_DUAL_STEPPER_DRIVERS)
  #ifndef Y2_STEP_PIN
    #define Y2_STEP_PIN   _EPIN(Y2_E_INDEX, STEP)
    #define Y2_DIR_PIN    _EPIN(Y2_E_INDEX, DIR)
    #define Y2_ENABLE_PIN _EPIN(Y2_E_INDEX, ENABLE)
    #if Y2_E_INDEX >= MAX_EXTRUDERS || !PIN_EXISTS(Y2_STEP)
      #error "No E stepper plug left for Y2!"
    #endif
  #endif
  #ifndef Y2_MS1_PIN
    #define Y2_MS1_PIN    _EPIN(Y2_E_INDEX, MS1)
  #endif
  #ifndef Y2_MS2_PIN
    #define Y2_MS2_PIN    _EPIN(Y2_E_INDEX, MS2)
  #endif
  #ifndef Y2_MS3_PIN
    #define Y2_MS3_PIN    _EPIN(Y2_E_INDEX, MS3)
  #endif
  #if AXIS_HAS_SPI(Y2) && !defined(Y2_CS_PIN)
    #define Y2_CS_PIN     _EPIN(Y2_E_INDEX, CS)
  #endif
  #if AXIS_HAS_UART(Y2)
    #ifndef Y2_SERIAL_TX_PIN
      #define Y2_SERIAL_TX_PIN _EPIN(Y2_E_INDEX, SERIAL_TX)
    #endif
    #ifndef Y2_SERIAL_RX_PIN
      #define Y2_SERIAL_RX_PIN _EPIN(Y2_E_INDEX, SERIAL_RX)
    #endif
  #endif
  #if defined(Y2_STALL_SENSITIVITY) && ENABLED(Y_DUAL_ENDSTOPS) && _PEXI(Y2_E_INDEX, DIAG)
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
      #define _Y2_USE_ENDSTOP(P) _E##P##_DIAG_
      #define Y2_USE_ENDSTOP _Y2_USE_ENDSTOP(Y2_E_INDEX)
    #endif
    #undef Y2_DIAG_PIN
  #endif
  #define Z2_E_INDEX INCREMENT(Y2_E_INDEX)
#else
  #define Z2_E_INDEX Y2_E_INDEX
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
#if NUM_Z_STEPPER_DRIVERS >= 2
  #ifndef Z2_STEP_PIN
    #define Z2_STEP_PIN   _EPIN(Z2_E_INDEX, STEP)
    #define Z2_DIR_PIN    _EPIN(Z2_E_INDEX, DIR)
    #define Z2_ENABLE_PIN _EPIN(Z2_E_INDEX, ENABLE)
    #if Z2_E_INDEX >= MAX_EXTRUDERS || !PIN_EXISTS(Z2_STEP)
      #error "No E stepper plug left for Z2!"
    #endif
  #endif
  #ifndef Z2_MS1_PIN
    #define Z2_MS1_PIN    _EPIN(Z2_E_INDEX, MS1)
  #endif
  #ifndef Z2_MS2_PIN
    #define Z2_MS2_PIN    _EPIN(Z2_E_INDEX, MS2)
  #endif
  #ifndef Z2_MS3_PIN
    #define Z2_MS3_PIN    _EPIN(Z2_E_INDEX, MS3)
  #endif
  #if AXIS_HAS_SPI(Z2) && !defined(Z2_CS_PIN)
    #define Z2_CS_PIN     _EPIN(Z2_E_INDEX, CS)
  #endif
  #if AXIS_HAS_UART(Z2)
    #ifndef Z2_SERIAL_TX_PIN
      #define Z2_SERIAL_TX_PIN _EPIN(Z2_E_INDEX, SERIAL_TX)
    #endif
    #ifndef Z2_SERIAL_RX_PIN
      #define Z2_SERIAL_RX_PIN _EPIN(Z2_E_INDEX, SERIAL_RX)
    #endif
  #endif
  #if defined(Z2_STALL_SENSITIVITY) && ENABLED(Z_MULTI_ENDSTOPS) && NUM_Z_STEPPER_DRIVERS >= 2 && _PEXI(Z2_E_INDEX, DIAG)
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
      #define _Z2_USE_ENDSTOP(P) _E##P##_DIAG_
      #define Z2_USE_ENDSTOP _Z2_USE_ENDSTOP(Z2_E_INDEX)
    #endif
    #undef Z2_DIAG_PIN
  #endif
  #define Z3_E_INDEX INCREMENT(Z2_E_INDEX)
#else
  #define Z3_E_INDEX Z2_E_INDEX
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

#if NUM_Z_STEPPER_DRIVERS >= 3
  #ifndef Z3_STEP_PIN
    #define Z3_STEP_PIN   _EPIN(Z3_E_INDEX, STEP)
    #define Z3_DIR_PIN    _EPIN(Z3_E_INDEX, DIR)
    #define Z3_ENABLE_PIN _EPIN(Z3_E_INDEX, ENABLE)
    #if Z3_E_INDEX >= MAX_EXTRUDERS || !PIN_EXISTS(Z3_STEP)
      #error "No E stepper plug left for Z3!"
    #endif
  #endif
  #if AXIS_HAS_SPI(Z3)
    #ifndef Z3_CS_PIN
      #define Z3_CS_PIN   _EPIN(Z3_E_INDEX, CS)
    #endif
  #endif
  #ifndef Z3_MS1_PIN
    #define Z3_MS1_PIN    _EPIN(Z3_E_INDEX, MS1)
  #endif
  #ifndef Z3_MS2_PIN
    #define Z3_MS2_PIN    _EPIN(Z3_E_INDEX, MS2)
  #endif
  #ifndef Z3_MS3_PIN
    #define Z3_MS3_PIN    _EPIN(Z3_E_INDEX, MS3)
  #endif
  #if AXIS_HAS_UART(Z3)
    #ifndef Z3_SERIAL_TX_PIN
      #define Z3_SERIAL_TX_PIN _EPIN(Z3_E_INDEX, SERIAL_TX)
    #endif
    #ifndef Z3_SERIAL_RX_PIN
      #define Z3_SERIAL_RX_PIN _EPIN(Z3_E_INDEX, SERIAL_RX)
    #endif
  #endif
  #if defined(Z3_STALL_SENSITIVITY) && ENABLED(Z_MULTI_ENDSTOPS) && NUM_Z_STEPPER_DRIVERS >= 3 && _PEXI(Z3_E_INDEX, DIAG)
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
      #define _Z3_USE_ENDSTOP(P) _E##P##_DIAG_
      #define Z3_USE_ENDSTOP _Z3_USE_ENDSTOP(Z3_E_INDEX)
    #endif
    #undef Z3_DIAG_PIN
  #endif
  #define Z4_E_INDEX INCREMENT(Z3_E_INDEX)
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

#if NUM_Z_STEPPER_DRIVERS >= 4
  #ifndef Z4_STEP_PIN
    #define Z4_STEP_PIN   _EPIN(Z4_E_INDEX, STEP)
    #define Z4_DIR_PIN    _EPIN(Z4_E_INDEX, DIR)
    #define Z4_ENABLE_PIN _EPIN(Z4_E_INDEX, ENABLE)
    #if Z4_E_INDEX >= MAX_EXTRUDERS || !PIN_EXISTS(Z4_STEP)
      #error "No E stepper plug left for Z4!"
    #endif
  #endif
  #if AXIS_HAS_SPI(Z4)
    #ifndef Z4_CS_PIN
      #define Z4_CS_PIN     _EPIN(Z4_E_INDEX, CS)
    #endif
  #endif
  #ifndef Z4_MS1_PIN
    #define Z4_MS1_PIN    _EPIN(Z4_E_INDEX, MS1)
  #endif
  #ifndef Z4_MS2_PIN
    #define Z4_MS2_PIN    _EPIN(Z4_E_INDEX, MS2)
  #endif
  #ifndef Z4_MS3_PIN
    #define Z4_MS3_PIN    _EPIN(Z4_E_INDEX, MS3)
  #endif
  #if AXIS_HAS_UART(Z4)
    #ifndef Z4_SERIAL_TX_PIN
      #define Z4_SERIAL_TX_PIN _EPIN(Z4_E_INDEX, SERIAL_TX)
    #endif
    #ifndef Z4_SERIAL_RX_PIN
      #define Z4_SERIAL_RX_PIN _EPIN(Z4_E_INDEX, SERIAL_RX)
    #endif
  #endif
  #if defined(Z4_STALL_SENSITIVITY) && ENABLED(Z_MULTI_ENDSTOPS) && NUM_Z_STEPPER_DRIVERS >= 4 && _PEXI(Z4_E_INDEX, DIAG)
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
      #define _Z4_USE_ENDSTOP(P) _E##P##_DIAG_
      #define Z4_USE_ENDSTOP _Z4_USE_ENDSTOP(Z4_E_INDEX)
    #endif
    #undef Z4_DIAG_PIN
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

#if HAS_MARLINUI_U8GLIB
  #if !defined(ST7920_DELAY_1) && defined(BOARD_ST7920_DELAY_1)
    #define ST7920_DELAY_1 BOARD_ST7920_DELAY_1
  #endif
  #if !defined(ST7920_DELAY_2) && defined(BOARD_ST7920_DELAY_2)
    #define ST7920_DELAY_2 BOARD_ST7920_DELAY_2
  #endif
  #if !defined(ST7920_DELAY_3) && defined(BOARD_ST7920_DELAY_3)
    #define ST7920_DELAY_3 BOARD_ST7920_DELAY_3
  #endif
#else
  #undef ST7920_DELAY_1
  #undef ST7920_DELAY_2
  #undef ST7920_DELAY_3
  #undef BOARD_ST7920_DELAY_1
  #undef BOARD_ST7920_DELAY_2
  #undef BOARD_ST7920_DELAY_3
#endif

#undef HAS_FREE_AUX2_PINS
#undef DIAG_REMAPPED
