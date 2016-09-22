/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * Conditionals_post.h
 * Defines that depend on configuration but are not editable.
 */

#ifndef CONDITIONALS_POST_H
#define CONDITIONALS_POST_H

  #if ENABLED(EMERGENCY_PARSER)
    #define EMERGENCY_PARSER_CAPABILITIES " EMERGENCY_CODES:M108,M112,M410"
  #else
    #define EMERGENCY_PARSER_CAPABILITIES ""
  #endif

  /**
   * Axis lengths and center
   */
  #define X_MAX_LENGTH (X_MAX_POS - (X_MIN_POS))
  #define Y_MAX_LENGTH (Y_MAX_POS - (Y_MIN_POS))
  #define Z_MAX_LENGTH (Z_MAX_POS - (Z_MIN_POS))
  #define X_CENTER float((X_MIN_POS + X_MAX_POS) * 0.5)
  #define Y_CENTER float((Y_MIN_POS + Y_MAX_POS) * 0.5)
  #define Z_CENTER float((Z_MIN_POS + Z_MAX_POS) * 0.5)

  /**
   * CoreXY and CoreXZ
   */
  #if ENABLED(COREXY)
    #define CORE_AXIS_1 A_AXIS // XY from A + B
    #define CORE_AXIS_2 B_AXIS
    #define NORMAL_AXIS Z_AXIS
  #elif ENABLED(COREXZ)
    #define CORE_AXIS_1 A_AXIS // XZ from A + C
    #define CORE_AXIS_2 C_AXIS
    #define NORMAL_AXIS Y_AXIS
  #elif ENABLED(COREYZ)
    #define CORE_AXIS_1 B_AXIS // YZ from B + C
    #define CORE_AXIS_2 C_AXIS
    #define NORMAL_AXIS X_AXIS
  #endif

  #define IS_SCARA (ENABLED(MORGAN_SCARA) || ENABLED(MAKERARM_SCARA))
  #define IS_KINEMATIC (ENABLED(DELTA) || IS_SCARA)
  #define IS_CARTESIAN !IS_KINEMATIC

  /**
   * SCARA cannot use SLOWDOWN and requires QUICKHOME
   */
  #if IS_SCARA
    #undef SLOWDOWN
    #define QUICK_HOME
  #endif

  /**
   * Set the home position based on settings or manual overrides
   */
  #ifdef MANUAL_X_HOME_POS
    #define X_HOME_POS MANUAL_X_HOME_POS
  #elif ENABLED(BED_CENTER_AT_0_0)
    #if ENABLED(DELTA)
      #define X_HOME_POS 0
    #else
      #define X_HOME_POS ((X_MAX_LENGTH) * (X_HOME_DIR) * 0.5)
    #endif
  #else
    #if ENABLED(DELTA)
      #define X_HOME_POS (X_MIN_POS + (X_MAX_LENGTH) * 0.5)
    #else
      #define X_HOME_POS (X_HOME_DIR < 0 ? X_MIN_POS : X_MAX_POS)
    #endif
  #endif

  #ifdef MANUAL_Y_HOME_POS
    #define Y_HOME_POS MANUAL_Y_HOME_POS
  #elif ENABLED(BED_CENTER_AT_0_0)
    #if ENABLED(DELTA)
      #define Y_HOME_POS 0
    #else
      #define Y_HOME_POS ((Y_MAX_LENGTH) * (Y_HOME_DIR) * 0.5)
    #endif
  #else
    #if ENABLED(DELTA)
      #define Y_HOME_POS (Y_MIN_POS + (Y_MAX_LENGTH) * 0.5)
    #else
      #define Y_HOME_POS (Y_HOME_DIR < 0 ? Y_MIN_POS : Y_MAX_POS)
    #endif
  #endif

  #ifdef MANUAL_Z_HOME_POS
    #define Z_HOME_POS MANUAL_Z_HOME_POS
  #else
    #define Z_HOME_POS (Z_HOME_DIR < 0 ? Z_MIN_POS : Z_MAX_POS)
  #endif

  /**
   * Auto Bed Leveling and Z Probe Repeatability Test
   */
  #define HAS_PROBING_PROCEDURE (ENABLED(AUTO_BED_LEVELING_FEATURE) || ENABLED(Z_MIN_PROBE_REPEATABILITY_TEST))

  #define HOMING_Z_WITH_PROBE (HAS_BED_PROBE && Z_HOME_DIR < 0 && ENABLED(Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN))

  /**
   * Z Sled Probe requires Z_SAFE_HOMING
   */
  #if ENABLED(Z_PROBE_SLED)
    #define Z_SAFE_HOMING
  #endif

  /**
   * DELTA should ignore Z_SAFE_HOMING and SLOWDOWN
   */
  #if ENABLED(DELTA)
    #undef Z_SAFE_HOMING
    #undef SLOWDOWN
  #endif

  /**
   * Safe Homing Options
   */
  #if ENABLED(Z_SAFE_HOMING)
    #ifndef Z_SAFE_HOMING_X_POINT
      #define Z_SAFE_HOMING_X_POINT ((X_MIN_POS + X_MAX_POS) / 2)
    #endif
    #ifndef Z_SAFE_HOMING_Y_POINT
      #define Z_SAFE_HOMING_Y_POINT ((Y_MIN_POS + Y_MAX_POS) / 2)
    #endif
    #define X_TILT_FULCRUM Z_SAFE_HOMING_X_POINT
    #define Y_TILT_FULCRUM Z_SAFE_HOMING_Y_POINT
  #else
    #define X_TILT_FULCRUM X_HOME_POS
    #define Y_TILT_FULCRUM Y_HOME_POS
  #endif

  /**
   * Host keep alive
   */
  #ifndef DEFAULT_KEEPALIVE_INTERVAL
    #define DEFAULT_KEEPALIVE_INTERVAL 2
  #endif

  /**
   * MAX_STEP_FREQUENCY differs for TOSHIBA
   */
  #if ENABLED(CONFIG_STEPPERS_TOSHIBA)
    #define MAX_STEP_FREQUENCY 10000 // Max step frequency for Toshiba Stepper Controllers
  #else
    #define MAX_STEP_FREQUENCY 40000 // Max step frequency for Ultimaker (5000 pps / half step)
  #endif

  // MS1 MS2 Stepper Driver Microstepping mode table
  #define MICROSTEP1 LOW,LOW
  #define MICROSTEP2 HIGH,LOW
  #define MICROSTEP4 LOW,HIGH
  #define MICROSTEP8 HIGH,HIGH
  #define MICROSTEP16 HIGH,HIGH

  /**
   * Advance calculated values
   */
  #if ENABLED(ADVANCE)
    #define EXTRUSION_AREA (0.25 * (D_FILAMENT) * (D_FILAMENT) * M_PI)
    #define STEPS_PER_CUBIC_MM_E (axis_steps_per_mm[E_AXIS] / (EXTRUSION_AREA))
  #endif

  #if ENABLED(ULTIPANEL) && DISABLED(ELB_FULL_GRAPHIC_CONTROLLER)
    #undef SD_DETECT_INVERTED
  #endif

  /**
   * Set defaults for missing (newer) options
   */
  #ifndef DISABLE_INACTIVE_X
    #define DISABLE_INACTIVE_X DISABLE_X
  #endif
  #ifndef DISABLE_INACTIVE_Y
    #define DISABLE_INACTIVE_Y DISABLE_Y
  #endif
  #ifndef DISABLE_INACTIVE_Z
    #define DISABLE_INACTIVE_Z DISABLE_Z
  #endif
  #ifndef DISABLE_INACTIVE_E
    #define DISABLE_INACTIVE_E DISABLE_E
  #endif

  // Power Signal Control Definitions
  // By default use ATX definition
  #ifndef POWER_SUPPLY
    #define POWER_SUPPLY 1
  #endif
  #if (POWER_SUPPLY == 1)     // 1 = ATX
    #define PS_ON_AWAKE  LOW
    #define PS_ON_ASLEEP HIGH
  #elif (POWER_SUPPLY == 2)   // 2 = X-Box 360 203W
    #define PS_ON_AWAKE  HIGH
    #define PS_ON_ASLEEP LOW
  #endif
  #define HAS_POWER_SWITCH (POWER_SUPPLY > 0 && PIN_EXISTS(PS_ON))

  /**
   * Temp Sensor defines
   */
  #if TEMP_SENSOR_0 == -3
    #define HEATER_0_USES_MAX6675
    #define MAX6675_IS_MAX31855
  #elif TEMP_SENSOR_0 == -2
    #define HEATER_0_USES_MAX6675
  #elif TEMP_SENSOR_0 == -1
    #define HEATER_0_USES_AD595
  #elif TEMP_SENSOR_0 == 0
    #undef HEATER_0_MINTEMP
    #undef HEATER_0_MAXTEMP
  #elif TEMP_SENSOR_0 > 0
    #define THERMISTORHEATER_0 TEMP_SENSOR_0
    #define HEATER_0_USES_THERMISTOR
  #endif

  #if TEMP_SENSOR_1 <= -2
    #error "MAX6675 / MAX31855 Thermocouples not supported for TEMP_SENSOR_1"
  #elif TEMP_SENSOR_1 == -1
    #define HEATER_1_USES_AD595
  #elif TEMP_SENSOR_1 == 0
    #undef HEATER_1_MINTEMP
    #undef HEATER_1_MAXTEMP
  #elif TEMP_SENSOR_1 > 0
    #define THERMISTORHEATER_1 TEMP_SENSOR_1
    #define HEATER_1_USES_THERMISTOR
  #endif

  #if TEMP_SENSOR_2 <= -2
    #error "MAX6675 / MAX31855 Thermocouples not supported for TEMP_SENSOR_2"
  #elif TEMP_SENSOR_2 == -1
    #define HEATER_2_USES_AD595
  #elif TEMP_SENSOR_2 == 0
    #undef HEATER_2_MINTEMP
    #undef HEATER_2_MAXTEMP
  #elif TEMP_SENSOR_2 > 0
    #define THERMISTORHEATER_2 TEMP_SENSOR_2
    #define HEATER_2_USES_THERMISTOR
  #endif

  #if TEMP_SENSOR_3 <= -2
    #error "MAX6675 / MAX31855 Thermocouples not supported for TEMP_SENSOR_3"
  #elif TEMP_SENSOR_3 == -1
    #define HEATER_3_USES_AD595
  #elif TEMP_SENSOR_3 == 0
    #undef HEATER_3_MINTEMP
    #undef HEATER_3_MAXTEMP
  #elif TEMP_SENSOR_3 > 0
    #define THERMISTORHEATER_3 TEMP_SENSOR_3
    #define HEATER_3_USES_THERMISTOR
  #endif

  #if TEMP_SENSOR_BED <= -2
    #error "MAX6675 / MAX31855 Thermocouples not supported for TEMP_SENSOR_BED"
  #elif TEMP_SENSOR_BED == -1
    #define BED_USES_AD595
  #elif TEMP_SENSOR_BED == 0
    #undef BED_MINTEMP
    #undef BED_MAXTEMP
  #elif TEMP_SENSOR_BED > 0
    #define THERMISTORBED TEMP_SENSOR_BED
    #define BED_USES_THERMISTOR
  #endif

  /**
   * Flags for PID handling
   */
  #define HAS_PID_HEATING (ENABLED(PIDTEMP) || ENABLED(PIDTEMPBED))
  #define HAS_PID_FOR_BOTH (ENABLED(PIDTEMP) && ENABLED(PIDTEMPBED))

  /**
   * Default hotend offsets, if not defined
   */
  #if HOTENDS > 1
    #ifndef HOTEND_OFFSET_X
      #define HOTEND_OFFSET_X { 0 } // X offsets for each extruder
    #endif
    #ifndef HOTEND_OFFSET_Y
      #define HOTEND_OFFSET_Y { 0 } // Y offsets for each extruder
    #endif
    #if !defined(HOTEND_OFFSET_Z) && (ENABLED(DUAL_X_CARRIAGE) || ENABLED(SWITCHING_EXTRUDER))
      #define HOTEND_OFFSET_Z { 0 }
    #endif
  #endif

  /**
   * ARRAY_BY_EXTRUDERS based on EXTRUDERS
   */
  #define ARRAY_BY_EXTRUDERS(args...) ARRAY_N(EXTRUDERS, args)
  #define ARRAY_BY_EXTRUDERS1(v1) ARRAY_BY_EXTRUDERS(v1, v1, v1, v1, v1, v1)

  /**
   * ARRAY_BY_HOTENDS based on HOTENDS
   */
  #define ARRAY_BY_HOTENDS(args...) ARRAY_N(HOTENDS, args)
  #define ARRAY_BY_HOTENDS1(v1) ARRAY_BY_HOTENDS(v1, v1, v1, v1, v1, v1)

  /**
   * Z_DUAL_ENDSTOPS endstop reassignment
   */
  #if ENABLED(Z_DUAL_ENDSTOPS)
    #define _XMIN_ 100
    #define _YMIN_ 200
    #define _ZMIN_ 300
    #define _XMAX_ 101
    #define _YMAX_ 201
    #define _ZMAX_ 301
    #if Z2_USE_ENDSTOP == _XMAX_
      #define USE_XMAX_PLUG
    #elif Z2_USE_ENDSTOP == _YMAX_
      #define USE_YMAX_PLUG
    #elif Z2_USE_ENDSTOP == _ZMAX_
      #define USE_ZMAX_PLUG
    #elif Z2_USE_ENDSTOP == _XMIN_
      #define USE_XMIN_PLUG
    #elif Z2_USE_ENDSTOP == _YMIN_
      #define USE_YMIN_PLUG
    #elif Z2_USE_ENDSTOP == _ZMIN_
      #define USE_ZMIN_PLUG
    #endif
    #if Z_HOME_DIR > 0
      #if Z2_USE_ENDSTOP == _XMAX_
        #define Z2_MAX_ENDSTOP_INVERTING X_MAX_ENDSTOP_INVERTING
        #define Z2_MAX_PIN X_MAX_PIN
      #elif Z2_USE_ENDSTOP == _YMAX_
        #define Z2_MAX_ENDSTOP_INVERTING Y_MAX_ENDSTOP_INVERTING
        #define Z2_MAX_PIN Y_MAX_PIN
      #elif Z2_USE_ENDSTOP == _ZMAX_
        #define Z2_MAX_ENDSTOP_INVERTING Z_MAX_ENDSTOP_INVERTING
        #define Z2_MAX_PIN Z_MAX_PIN
      #elif Z2_USE_ENDSTOP == _XMIN_
        #define Z2_MAX_ENDSTOP_INVERTING X_MIN_ENDSTOP_INVERTING
        #define Z2_MAX_PIN X_MIN_PIN
      #elif Z2_USE_ENDSTOP == _YMIN_
        #define Z2_MAX_ENDSTOP_INVERTING Y_MIN_ENDSTOP_INVERTING
        #define Z2_MAX_PIN Y_MIN_PIN
      #elif Z2_USE_ENDSTOP == _ZMIN_
        #define Z2_MAX_ENDSTOP_INVERTING Z_MIN_ENDSTOP_INVERTING
        #define Z2_MAX_PIN Z_MIN_PIN
      #else
        #define Z2_MAX_ENDSTOP_INVERTING false
      #endif
    #else
      #if Z2_USE_ENDSTOP == _XMAX_
        #define Z2_MIN_ENDSTOP_INVERTING X_MAX_ENDSTOP_INVERTING
        #define Z2_MIN_PIN X_MAX_PIN
      #elif Z2_USE_ENDSTOP == _YMAX_
        #define Z2_MIN_ENDSTOP_INVERTING Y_MAX_ENDSTOP_INVERTING
        #define Z2_MIN_PIN Y_MAX_PIN
      #elif Z2_USE_ENDSTOP == _ZMAX_
        #define Z2_MIN_ENDSTOP_INVERTING Z_MAX_ENDSTOP_INVERTING
        #define Z2_MIN_PIN Z_MAX_PIN
      #elif Z2_USE_ENDSTOP == _XMIN_
        #define Z2_MIN_ENDSTOP_INVERTING X_MIN_ENDSTOP_INVERTING
        #define Z2_MIN_PIN X_MIN_PIN
      #elif Z2_USE_ENDSTOP == _YMIN_
        #define Z2_MIN_ENDSTOP_INVERTING Y_MIN_ENDSTOP_INVERTING
        #define Z2_MIN_PIN Y_MIN_PIN
      #elif Z2_USE_ENDSTOP == _ZMIN_
        #define Z2_MIN_ENDSTOP_INVERTING Z_MIN_ENDSTOP_INVERTING
        #define Z2_MIN_PIN Z_MIN_PIN
      #else
        #define Z2_MIN_ENDSTOP_INVERTING false
      #endif
    #endif
  #endif

  /**
   * Set ENDSTOPPULLUPS for active endstop switches
   */
  #if ENABLED(ENDSTOPPULLUPS)
    #if ENABLED(USE_XMAX_PLUG)
      #define ENDSTOPPULLUP_XMAX
    #endif
    #if ENABLED(USE_YMAX_PLUG)
      #define ENDSTOPPULLUP_YMAX
    #endif
    #if ENABLED(USE_ZMAX_PLUG)
      #define ENDSTOPPULLUP_ZMAX
    #endif
    #if ENABLED(USE_XMIN_PLUG)
      #define ENDSTOPPULLUP_XMIN
    #endif
    #if ENABLED(USE_YMIN_PLUG)
      #define ENDSTOPPULLUP_YMIN
    #endif
    #if ENABLED(USE_ZMIN_PLUG)
      #define ENDSTOPPULLUP_ZMIN
    #endif
  #endif

  /**
   * Shorthand for pin tests, used wherever needed
   */
  #define HAS_TEMP_0 (PIN_EXISTS(TEMP_0) && TEMP_SENSOR_0 != 0 && TEMP_SENSOR_0 > -2)
  #define HAS_TEMP_1 (PIN_EXISTS(TEMP_1) && TEMP_SENSOR_1 != 0 && TEMP_SENSOR_1 > -2)
  #define HAS_TEMP_2 (PIN_EXISTS(TEMP_2) && TEMP_SENSOR_2 != 0 && TEMP_SENSOR_2 > -2)
  #define HAS_TEMP_3 (PIN_EXISTS(TEMP_3) && TEMP_SENSOR_3 != 0 && TEMP_SENSOR_3 > -2)
  #define HAS_TEMP_BED (PIN_EXISTS(TEMP_BED) && TEMP_SENSOR_BED != 0 && TEMP_SENSOR_BED > -2)
  #define HAS_HEATER_0 (PIN_EXISTS(HEATER_0))
  #define HAS_HEATER_1 (PIN_EXISTS(HEATER_1))
  #define HAS_HEATER_2 (PIN_EXISTS(HEATER_2))
  #define HAS_HEATER_3 (PIN_EXISTS(HEATER_3))
  #define HAS_HEATER_BED (PIN_EXISTS(HEATER_BED))
  #define HAS_AUTO_FAN_0 (PIN_EXISTS(EXTRUDER_0_AUTO_FAN))
  #define HAS_AUTO_FAN_1 (PIN_EXISTS(EXTRUDER_1_AUTO_FAN))
  #define HAS_AUTO_FAN_2 (PIN_EXISTS(EXTRUDER_2_AUTO_FAN))
  #define HAS_AUTO_FAN_3 (PIN_EXISTS(EXTRUDER_3_AUTO_FAN))
  #define AUTO_1_IS_0 (EXTRUDER_1_AUTO_FAN_PIN == EXTRUDER_0_AUTO_FAN_PIN)
  #define AUTO_2_IS_0 (EXTRUDER_2_AUTO_FAN_PIN == EXTRUDER_0_AUTO_FAN_PIN)
  #define AUTO_2_IS_1 (EXTRUDER_2_AUTO_FAN_PIN == EXTRUDER_1_AUTO_FAN_PIN)
  #define AUTO_3_IS_0 (EXTRUDER_3_AUTO_FAN_PIN == EXTRUDER_0_AUTO_FAN_PIN)
  #define AUTO_3_IS_1 (EXTRUDER_3_AUTO_FAN_PIN == EXTRUDER_1_AUTO_FAN_PIN)
  #define AUTO_3_IS_2 (EXTRUDER_3_AUTO_FAN_PIN == EXTRUDER_2_AUTO_FAN_PIN)
  #define HAS_AUTO_FAN (HAS_AUTO_FAN_0 || HAS_AUTO_FAN_1 || HAS_AUTO_FAN_2 || HAS_AUTO_FAN_3)
  #define HAS_FAN0 (PIN_EXISTS(FAN))
  #define HAS_FAN1 (PIN_EXISTS(FAN1) && CONTROLLERFAN_PIN != FAN1_PIN && EXTRUDER_0_AUTO_FAN_PIN != FAN1_PIN && EXTRUDER_1_AUTO_FAN_PIN != FAN1_PIN && EXTRUDER_2_AUTO_FAN_PIN != FAN1_PIN && EXTRUDER_3_AUTO_FAN_PIN != FAN1_PIN)
  #define HAS_FAN2 (PIN_EXISTS(FAN2) && CONTROLLERFAN_PIN != FAN2_PIN && EXTRUDER_0_AUTO_FAN_PIN != FAN2_PIN && EXTRUDER_1_AUTO_FAN_PIN != FAN2_PIN && EXTRUDER_2_AUTO_FAN_PIN != FAN2_PIN && EXTRUDER_3_AUTO_FAN_PIN != FAN2_PIN)
  #define HAS_CONTROLLERFAN (PIN_EXISTS(CONTROLLERFAN))
  #define HAS_SERVOS (defined(NUM_SERVOS) && NUM_SERVOS > 0)
  #define HAS_SERVO_0 (PIN_EXISTS(SERVO0))
  #define HAS_SERVO_1 (PIN_EXISTS(SERVO1))
  #define HAS_SERVO_2 (PIN_EXISTS(SERVO2))
  #define HAS_SERVO_3 (PIN_EXISTS(SERVO3))
  #define HAS_FILAMENT_WIDTH_SENSOR (PIN_EXISTS(FILWIDTH))
  #define HAS_FIL_RUNOUT (PIN_EXISTS(FIL_RUNOUT))
  #define HAS_HOME (PIN_EXISTS(HOME))
  #define HAS_KILL (PIN_EXISTS(KILL))
  #define HAS_SUICIDE (PIN_EXISTS(SUICIDE))
  #define HAS_PHOTOGRAPH (PIN_EXISTS(PHOTOGRAPH))
  #define HAS_X_MIN (PIN_EXISTS(X_MIN))
  #define HAS_X_MAX (PIN_EXISTS(X_MAX))
  #define HAS_Y_MIN (PIN_EXISTS(Y_MIN))
  #define HAS_Y_MAX (PIN_EXISTS(Y_MAX))
  #define HAS_Z_MIN (PIN_EXISTS(Z_MIN))
  #define HAS_Z_MAX (PIN_EXISTS(Z_MAX))
  #define HAS_Z2_MIN (PIN_EXISTS(Z2_MIN))
  #define HAS_Z2_MAX (PIN_EXISTS(Z2_MAX))
  #define HAS_Z_MIN_PROBE_PIN (PIN_EXISTS(Z_MIN_PROBE))
  #define HAS_SOLENOID_1 (PIN_EXISTS(SOL1))
  #define HAS_SOLENOID_2 (PIN_EXISTS(SOL2))
  #define HAS_SOLENOID_3 (PIN_EXISTS(SOL3))
  #define HAS_MICROSTEPS (PIN_EXISTS(X_MS1))
  #define HAS_MICROSTEPS_E0 (PIN_EXISTS(E0_MS1))
  #define HAS_MICROSTEPS_E1 (PIN_EXISTS(E1_MS1))
  #define HAS_MICROSTEPS_E2 (PIN_EXISTS(E2_MS1))
  #define HAS_STEPPER_RESET (PIN_EXISTS(STEPPER_RESET))
  #define HAS_X_ENABLE (PIN_EXISTS(X_ENABLE))
  #define HAS_X2_ENABLE (PIN_EXISTS(X2_ENABLE))
  #define HAS_Y_ENABLE (PIN_EXISTS(Y_ENABLE))
  #define HAS_Y2_ENABLE (PIN_EXISTS(Y2_ENABLE))
  #define HAS_Z_ENABLE (PIN_EXISTS(Z_ENABLE))
  #define HAS_Z2_ENABLE (PIN_EXISTS(Z2_ENABLE))
  #define HAS_E0_ENABLE (PIN_EXISTS(E0_ENABLE))
  #define HAS_E1_ENABLE (PIN_EXISTS(E1_ENABLE))
  #define HAS_E2_ENABLE (PIN_EXISTS(E2_ENABLE))
  #define HAS_E3_ENABLE (PIN_EXISTS(E3_ENABLE))
  #define HAS_E4_ENABLE (PIN_EXISTS(E4_ENABLE))
  #define HAS_X_DIR (PIN_EXISTS(X_DIR))
  #define HAS_X2_DIR (PIN_EXISTS(X2_DIR))
  #define HAS_Y_DIR (PIN_EXISTS(Y_DIR))
  #define HAS_Y2_DIR (PIN_EXISTS(Y2_DIR))
  #define HAS_Z_DIR (PIN_EXISTS(Z_DIR))
  #define HAS_Z2_DIR (PIN_EXISTS(Z2_DIR))
  #define HAS_E0_DIR (PIN_EXISTS(E0_DIR))
  #define HAS_E1_DIR (PIN_EXISTS(E1_DIR))
  #define HAS_E2_DIR (PIN_EXISTS(E2_DIR))
  #define HAS_E3_DIR (PIN_EXISTS(E3_DIR))
  #define HAS_E4_DIR (PIN_EXISTS(E4_DIR))
  #define HAS_X_STEP (PIN_EXISTS(X_STEP))
  #define HAS_X2_STEP (PIN_EXISTS(X2_STEP))
  #define HAS_Y_STEP (PIN_EXISTS(Y_STEP))
  #define HAS_Y2_STEP (PIN_EXISTS(Y2_STEP))
  #define HAS_Z_STEP (PIN_EXISTS(Z_STEP))
  #define HAS_Z2_STEP (PIN_EXISTS(Z2_STEP))
  #define HAS_E0_STEP (PIN_EXISTS(E0_STEP))
  #define HAS_E1_STEP (PIN_EXISTS(E1_STEP))
  #define HAS_E2_STEP (PIN_EXISTS(E2_STEP))
  #define HAS_E3_STEP (PIN_EXISTS(E3_STEP))
  #define HAS_E4_STEP (PIN_EXISTS(E4_STEP))
  #define HAS_DIGIPOTSS (PIN_EXISTS(DIGIPOTSS))
  #define HAS_BUZZER (PIN_EXISTS(BEEPER) || ENABLED(LCD_USE_I2C_BUZZER))

  #define HAS_MOTOR_CURRENT_PWM (PIN_EXISTS(MOTOR_CURRENT_PWM_XY) || PIN_EXISTS(MOTOR_CURRENT_PWM_Z) || PIN_EXISTS(MOTOR_CURRENT_PWM_E))

  #define HAS_TEMP_HOTEND (HAS_TEMP_0 || ENABLED(HEATER_0_USES_MAX6675))

  #define HAS_THERMALLY_PROTECTED_BED (HAS_TEMP_BED && HAS_HEATER_BED && ENABLED(THERMAL_PROTECTION_BED))

  /**
   * This setting is also used by M109 when trying to calculate
   * a ballpark safe margin to prevent wait-forever situation.
   */
  #ifndef EXTRUDE_MINTEMP
    #define EXTRUDE_MINTEMP 170
  #endif

  /**
   * Helper Macros for heaters and extruder fan
   */
  #define WRITE_HEATER_0P(v) WRITE(HEATER_0_PIN, v)
  #if HOTENDS > 1 || ENABLED(HEATERS_PARALLEL)
    #define WRITE_HEATER_1(v) WRITE(HEATER_1_PIN, v)
    #if HOTENDS > 2
      #define WRITE_HEATER_2(v) WRITE(HEATER_2_PIN, v)
      #if HOTENDS > 3
        #define WRITE_HEATER_3(v) WRITE(HEATER_3_PIN, v)
      #endif
    #endif
  #endif
  #if ENABLED(HEATERS_PARALLEL)
    #define WRITE_HEATER_0(v) { WRITE_HEATER_0P(v); WRITE_HEATER_1(v); }
  #else
    #define WRITE_HEATER_0(v) WRITE_HEATER_0P(v)
  #endif
  #if HAS_HEATER_BED
    #define WRITE_HEATER_BED(v) WRITE(HEATER_BED_PIN, v)
  #endif

  /**
   * Up to 3 PWM fans
   */
  #if HAS_FAN2
    #define FAN_COUNT 3
  #elif HAS_FAN1
    #define FAN_COUNT 2
  #elif HAS_FAN0
    #define FAN_COUNT 1
  #else
    #define FAN_COUNT 0
  #endif

  #if HAS_FAN0
    #define WRITE_FAN(v) WRITE(FAN_PIN, v)
    #define WRITE_FAN0(v) WRITE_FAN(v)
  #endif
  #if HAS_FAN1
    #define WRITE_FAN1(v) WRITE(FAN1_PIN, v)
  #endif
  #if HAS_FAN2
    #define WRITE_FAN2(v) WRITE(FAN2_PIN, v)
  #endif
  #define WRITE_FAN_N(n, v) WRITE_FAN##n(v)

  /**
   * Servos and probes
   */

  #if HAS_SERVOS
    #ifndef Z_ENDSTOP_SERVO_NR
      #define Z_ENDSTOP_SERVO_NR -1
    #endif
  #endif

  #define PROBE_PIN_CONFIGURED (HAS_Z_MIN_PROBE_PIN || (HAS_Z_MIN && ENABLED(Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN)))

  #define HAS_BED_PROBE (PROBE_SELECTED && PROBE_PIN_CONFIGURED)

  #if ENABLED(Z_PROBE_ALLEN_KEY)
    #define PROBE_IS_TRIGGERED_WHEN_STOWED_TEST
  #endif

  /**
   * Bed Probe dependencies
   */
  #if HAS_BED_PROBE
    #if ENABLED(ENDSTOPPULLUPS) && HAS_Z_MIN_PROBE_PIN
      #define ENDSTOPPULLUP_ZMIN_PROBE
    #endif
    #ifndef Z_PROBE_OFFSET_RANGE_MIN
      #define Z_PROBE_OFFSET_RANGE_MIN -20
    #endif
    #ifndef Z_PROBE_OFFSET_RANGE_MAX
      #define Z_PROBE_OFFSET_RANGE_MAX 20
    #endif
    #ifndef XY_PROBE_SPEED
      #ifdef HOMING_FEEDRATE_XY
        #define XY_PROBE_SPEED HOMING_FEEDRATE_XY
      #else
        #define XY_PROBE_SPEED 4000
      #endif
    #endif
    #if Z_CLEARANCE_BETWEEN_PROBES > Z_CLEARANCE_DEPLOY_PROBE
      #define _Z_CLEARANCE_DEPLOY_PROBE Z_CLEARANCE_BETWEEN_PROBES
    #else
      #define _Z_CLEARANCE_DEPLOY_PROBE Z_CLEARANCE_DEPLOY_PROBE
    #endif
  #else
    #undef X_PROBE_OFFSET_FROM_EXTRUDER
    #undef Y_PROBE_OFFSET_FROM_EXTRUDER
    #undef Z_PROBE_OFFSET_FROM_EXTRUDER
    #define X_PROBE_OFFSET_FROM_EXTRUDER 0
    #define Y_PROBE_OFFSET_FROM_EXTRUDER 0
    #define Z_PROBE_OFFSET_FROM_EXTRUDER 0
  #endif

  /**
   * Delta radius/rod trimmers
   */
  #if ENABLED(DELTA)
    #ifndef DELTA_RADIUS_TRIM_TOWER_1
      #define DELTA_RADIUS_TRIM_TOWER_1 0.0
    #endif
    #ifndef DELTA_RADIUS_TRIM_TOWER_2
      #define DELTA_RADIUS_TRIM_TOWER_2 0.0
    #endif
    #ifndef DELTA_RADIUS_TRIM_TOWER_3
      #define DELTA_RADIUS_TRIM_TOWER_3 0.0
    #endif
    #ifndef DELTA_DIAGONAL_ROD_TRIM_TOWER_1
      #define DELTA_DIAGONAL_ROD_TRIM_TOWER_1 0.0
    #endif
    #ifndef DELTA_DIAGONAL_ROD_TRIM_TOWER_2
      #define DELTA_DIAGONAL_ROD_TRIM_TOWER_2 0.0
    #endif
    #ifndef DELTA_DIAGONAL_ROD_TRIM_TOWER_3
      #define DELTA_DIAGONAL_ROD_TRIM_TOWER_3 0.0
    #endif
  #endif

  /**
   * Specify the exact style of auto bed leveling
   *
   *  3POINT    - 3 Point Probing with the least-squares solution.
   *  LINEAR    - Grid Probing with the least-squares solution.
   *  NONLINEAR - Grid Probing with a mesh solution. Best for large beds.
   */
  #if ENABLED(AUTO_BED_LEVELING_FEATURE)
    #if DISABLED(AUTO_BED_LEVELING_GRID)
      #define AUTO_BED_LEVELING_LINEAR
      #define AUTO_BED_LEVELING_3POINT
    #elif IS_KINEMATIC
      #define AUTO_BED_LEVELING_NONLINEAR
    #else
      #define AUTO_BED_LEVELING_LINEAR
      #define AUTO_BED_LEVELING_LINEAR_GRID
    #endif
  #endif

  #define PLANNER_LEVELING (ENABLED(MESH_BED_LEVELING) || ENABLED(AUTO_BED_LEVELING_FEATURE))

  /**
   * Buzzer/Speaker
   */
  #if ENABLED(LCD_USE_I2C_BUZZER)
    #ifndef LCD_FEEDBACK_FREQUENCY_HZ
      #define LCD_FEEDBACK_FREQUENCY_HZ 1000
    #endif
    #ifndef LCD_FEEDBACK_FREQUENCY_DURATION_MS
      #define LCD_FEEDBACK_FREQUENCY_DURATION_MS 100
    #endif
  #else
    #ifndef LCD_FEEDBACK_FREQUENCY_HZ
      #define LCD_FEEDBACK_FREQUENCY_HZ 5000
    #endif
    #ifndef LCD_FEEDBACK_FREQUENCY_DURATION_MS
      #define LCD_FEEDBACK_FREQUENCY_DURATION_MS 2
    #endif
  #endif

  /**
   * Z_HOMING_HEIGHT / Z_CLEARANCE_BETWEEN_PROBES
   */
  #ifndef Z_HOMING_HEIGHT
    #ifndef Z_CLEARANCE_BETWEEN_PROBES
      #define Z_HOMING_HEIGHT 0
    #else
      #define Z_HOMING_HEIGHT Z_CLEARANCE_BETWEEN_PROBES
    #endif
  #endif
  #ifndef Z_CLEARANCE_BETWEEN_PROBES
    #define Z_CLEARANCE_BETWEEN_PROBES Z_HOMING_HEIGHT
  #endif

  #if IS_KINEMATIC
    // Check for this in the code instead
    #define MIN_PROBE_X X_MIN_POS
    #define MAX_PROBE_X X_MAX_POS
    #define MIN_PROBE_Y Y_MIN_POS
    #define MAX_PROBE_Y Y_MAX_POS
  #else
    // Boundaries for probing based on set limits
    #define MIN_PROBE_X (max(X_MIN_POS, X_MIN_POS + X_PROBE_OFFSET_FROM_EXTRUDER))
    #define MAX_PROBE_X (min(X_MAX_POS, X_MAX_POS + X_PROBE_OFFSET_FROM_EXTRUDER))
    #define MIN_PROBE_Y (max(Y_MIN_POS, Y_MIN_POS + Y_PROBE_OFFSET_FROM_EXTRUDER))
    #define MAX_PROBE_Y (min(Y_MAX_POS, Y_MAX_POS + Y_PROBE_OFFSET_FROM_EXTRUDER))
  #endif

#endif // CONDITIONALS_POST_H
