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

  #define IS_SCARA (ENABLED(MORGAN_SCARA) || ENABLED(MAKERARM_SCARA))
  #define IS_KINEMATIC (ENABLED(DELTA) || IS_SCARA)
  #define IS_CARTESIAN !IS_KINEMATIC

  /**
   * Axis lengths and center
   */
  #define X_MAX_LENGTH (X_MAX_POS - (X_MIN_POS))
  #define Y_MAX_LENGTH (Y_MAX_POS - (Y_MIN_POS))
  #define Z_MAX_LENGTH (Z_MAX_POS - (Z_MIN_POS))

  // Defined only if the sanity-check is bypassed
  #ifndef X_BED_SIZE
    #define X_BED_SIZE X_MAX_LENGTH
  #endif
  #ifndef Y_BED_SIZE
    #define Y_BED_SIZE Y_MAX_LENGTH
  #endif

  // Require 0,0 bed center for Delta and SCARA
  #if IS_KINEMATIC
    #define BED_CENTER_AT_0_0
  #endif

  // Define center values for future use
  #if ENABLED(BED_CENTER_AT_0_0)
    #define X_CENTER 0
    #define Y_CENTER 0
  #else
    #define X_CENTER ((X_BED_SIZE) / 2)
    #define Y_CENTER ((Y_BED_SIZE) / 2)
  #endif
  #define Z_CENTER ((Z_MIN_POS + Z_MAX_POS) / 2)

  // Get the linear boundaries of the bed
  #define X_MIN_BED (X_CENTER - (X_BED_SIZE) / 2)
  #define X_MAX_BED (X_CENTER + (X_BED_SIZE) / 2)
  #define Y_MIN_BED (Y_CENTER - (Y_BED_SIZE) / 2)
  #define Y_MAX_BED (Y_CENTER + (Y_BED_SIZE) / 2)

  /**
   * CoreXY, CoreXZ, and CoreYZ - and their reverse
   */
  #define CORE_IS_XY (ENABLED(COREXY) || ENABLED(COREYX))
  #define CORE_IS_XZ (ENABLED(COREXZ) || ENABLED(COREZX))
  #define CORE_IS_YZ (ENABLED(COREYZ) || ENABLED(COREZY))
  #define IS_CORE (CORE_IS_XY || CORE_IS_XZ || CORE_IS_YZ)
  #if IS_CORE
    #if CORE_IS_XY
      #define CORE_AXIS_1 A_AXIS
      #define CORE_AXIS_2 B_AXIS
      #define NORMAL_AXIS Z_AXIS
    #elif CORE_IS_XZ
      #define CORE_AXIS_1 A_AXIS
      #define NORMAL_AXIS Y_AXIS
      #define CORE_AXIS_2 C_AXIS
    #elif CORE_IS_YZ
      #define NORMAL_AXIS X_AXIS
      #define CORE_AXIS_1 B_AXIS
      #define CORE_AXIS_2 C_AXIS
    #endif
    #if ENABLED(COREYX) || ENABLED(COREZX) || ENABLED(COREZY)
      #define CORESIGN(n) (-(n))
    #else
      #define CORESIGN(n) (n)
    #endif
  #endif

  /**
   * No adjustable bed on non-cartesians
   */
  #if IS_KINEMATIC
    #undef LEVEL_BED_CORNERS
  #endif

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
      #define X_HOME_POS ((X_BED_SIZE) * (X_HOME_DIR) * 0.5)
    #endif
  #else
    #if ENABLED(DELTA)
      #define X_HOME_POS (X_MIN_POS + (X_BED_SIZE) * 0.5)
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
      #define Y_HOME_POS ((Y_BED_SIZE) * (Y_HOME_DIR) * 0.5)
    #endif
  #else
    #if ENABLED(DELTA)
      #define Y_HOME_POS (Y_MIN_POS + (Y_BED_SIZE) * 0.5)
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
   * If DELTA_HEIGHT isn't defined use the old setting
   */
  #if ENABLED(DELTA) && !defined(DELTA_HEIGHT)
    #define DELTA_HEIGHT Z_HOME_POS
  #endif

  /**
   * Auto Bed Leveling and Z Probe Repeatability Test
   */
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
      #define Z_SAFE_HOMING_X_POINT X_CENTER
    #endif
    #ifndef Z_SAFE_HOMING_Y_POINT
      #define Z_SAFE_HOMING_Y_POINT Y_CENTER
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
   * Provide a MAX_AUTORETRACT for older configs
   */
  #if ENABLED(FWRETRACT) && !defined(MAX_AUTORETRACT)
    #define MAX_AUTORETRACT 99
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
   * Override here because this is set in Configuration_adv.h
   */
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
    #define MAX6675_TMIN -270
    #define MAX6675_TMAX 1800
  #elif TEMP_SENSOR_0 == -2
    #define HEATER_0_USES_MAX6675
    #define MAX6675_TMIN 0
    #define MAX6675_TMAX 1024
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

  #if TEMP_SENSOR_4 <= -2
    #error "MAX6675 / MAX31855 Thermocouples not supported for TEMP_SENSOR_4"
  #elif TEMP_SENSOR_4 == -1
    #define HEATER_4_USES_AD595
  #elif TEMP_SENSOR_4 == 0
    #undef HEATER_4_MINTEMP
    #undef HEATER_4_MAXTEMP
  #elif TEMP_SENSOR_4 > 0
    #define THERMISTORHEATER_4 TEMP_SENSOR_4
    #define HEATER_4_USES_THERMISTOR
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
    #if !defined(HOTEND_OFFSET_Z) && (ENABLED(DUAL_X_CARRIAGE) || ENABLED(SWITCHING_NOZZLE))
      #define HOTEND_OFFSET_Z { 0 }
    #endif
  #endif

  /**
   * ARRAY_BY_EXTRUDERS based on EXTRUDERS
   */
  #define ARRAY_BY_EXTRUDERS(...) ARRAY_N(EXTRUDERS, __VA_ARGS__)
  #define ARRAY_BY_EXTRUDERS1(v1) ARRAY_BY_EXTRUDERS(v1, v1, v1, v1, v1, v1)

  /**
   * ARRAY_BY_HOTENDS based on HOTENDS
   */
  #define ARRAY_BY_HOTENDS(...) ARRAY_N(HOTENDS, __VA_ARGS__)
  #define ARRAY_BY_HOTENDS1(v1) ARRAY_BY_HOTENDS(v1, v1, v1, v1, v1, v1)

  /**
   * X_DUAL_ENDSTOPS endstop reassignment
   */
  #if ENABLED(X_DUAL_ENDSTOPS)
    #if X_HOME_DIR > 0
      #if X2_USE_ENDSTOP == _XMIN_
        #define X2_MAX_ENDSTOP_INVERTING X_MIN_ENDSTOP_INVERTING
        #define X2_MAX_PIN X_MIN_PIN
      #elif X2_USE_ENDSTOP == _XMAX_
        #define X2_MAX_ENDSTOP_INVERTING X_MAX_ENDSTOP_INVERTING
        #define X2_MAX_PIN X_MAX_PIN
      #elif X2_USE_ENDSTOP == _YMIN_
        #define X2_MAX_ENDSTOP_INVERTING Y_MIN_ENDSTOP_INVERTING
        #define X2_MAX_PIN Y_MIN_PIN
      #elif X2_USE_ENDSTOP == _YMAX_
        #define X2_MAX_ENDSTOP_INVERTING Y_MAX_ENDSTOP_INVERTING
        #define X2_MAX_PIN Y_MAX_PIN
      #elif X2_USE_ENDSTOP == _ZMIN_
        #define X2_MAX_ENDSTOP_INVERTING Z_MIN_ENDSTOP_INVERTING
        #define X2_MAX_PIN Z_MIN_PIN
      #elif X2_USE_ENDSTOP == _ZMAX_
        #define X2_MAX_ENDSTOP_INVERTING Z_MAX_ENDSTOP_INVERTING
        #define X2_MAX_PIN Z_MAX_PIN
      #else
        #define X2_MAX_ENDSTOP_INVERTING false
      #endif
      #define X2_MIN_ENDSTOP_INVERTING false
    #else
      #if X2_USE_ENDSTOP == _XMIN_
        #define X2_MIN_ENDSTOP_INVERTING X_MIN_ENDSTOP_INVERTING
        #define X2_MIN_PIN X_MIN_PIN
      #elif X2_USE_ENDSTOP == _XMAX_
        #define X2_MIN_ENDSTOP_INVERTING X_MAX_ENDSTOP_INVERTING
        #define X2_MIN_PIN X_MAX_PIN
      #elif X2_USE_ENDSTOP == _YMIN_
        #define X2_MIN_ENDSTOP_INVERTING Y_MIN_ENDSTOP_INVERTING
        #define X2_MIN_PIN Y_MIN_PIN
      #elif X2_USE_ENDSTOP == _YMAX_
        #define X2_MIN_ENDSTOP_INVERTING Y_MAX_ENDSTOP_INVERTING
        #define X2_MIN_PIN Y_MAX_PIN
      #elif X2_USE_ENDSTOP == _ZMIN_
        #define X2_MIN_ENDSTOP_INVERTING Z_MIN_ENDSTOP_INVERTING
        #define X2_MIN_PIN Z_MIN_PIN
      #elif X2_USE_ENDSTOP == _ZMAX_
        #define X2_MIN_ENDSTOP_INVERTING Z_MAX_ENDSTOP_INVERTING
        #define X2_MIN_PIN Z_MAX_PIN
      #else
        #define X2_MIN_ENDSTOP_INVERTING false
      #endif
      #define X2_MAX_ENDSTOP_INVERTING false
    #endif
  #endif

  // Is an endstop plug used for the X2 endstop?
  #define IS_X2_ENDSTOP(A,M) (ENABLED(X_DUAL_ENDSTOPS) && X2_USE_ENDSTOP == _##A##M##_)

  /**
   * Y_DUAL_ENDSTOPS endstop reassignment
   */
  #if ENABLED(Y_DUAL_ENDSTOPS)
    #if Y_HOME_DIR > 0
      #if Y2_USE_ENDSTOP == _XMIN_
        #define Y2_MAX_ENDSTOP_INVERTING X_MIN_ENDSTOP_INVERTING
        #define Y2_MAX_PIN X_MIN_PIN
      #elif Y2_USE_ENDSTOP == _XMAX_
        #define Y2_MAX_ENDSTOP_INVERTING X_MAX_ENDSTOP_INVERTING
        #define Y2_MAX_PIN X_MAX_PIN
      #elif Y2_USE_ENDSTOP == _YMIN_
        #define Y2_MAX_ENDSTOP_INVERTING Y_MIN_ENDSTOP_INVERTING
        #define Y2_MAX_PIN Y_MIN_PIN
      #elif Y2_USE_ENDSTOP == _YMAX_
        #define Y2_MAX_ENDSTOP_INVERTING Y_MAX_ENDSTOP_INVERTING
        #define Y2_MAX_PIN Y_MAX_PIN
      #elif Y2_USE_ENDSTOP == _ZMIN_
        #define Y2_MAX_ENDSTOP_INVERTING Z_MIN_ENDSTOP_INVERTING
        #define Y2_MAX_PIN Z_MIN_PIN
      #elif Y2_USE_ENDSTOP == _ZMAX_
        #define Y2_MAX_ENDSTOP_INVERTING Z_MAX_ENDSTOP_INVERTING
        #define Y2_MAX_PIN Z_MAX_PIN
      #else
        #define Y2_MAX_ENDSTOP_INVERTING false
      #endif
      #define Y2_MIN_ENDSTOP_INVERTING false
    #else
      #if Y2_USE_ENDSTOP == _XMIN_
        #define Y2_MIN_ENDSTOP_INVERTING X_MIN_ENDSTOP_INVERTING
        #define Y2_MIN_PIN X_MIN_PIN
      #elif Y2_USE_ENDSTOP == _XMAX_
        #define Y2_MIN_ENDSTOP_INVERTING X_MAX_ENDSTOP_INVERTING
        #define Y2_MIN_PIN X_MAX_PIN
      #elif Y2_USE_ENDSTOP == _YMIN_
        #define Y2_MIN_ENDSTOP_INVERTING Y_MIN_ENDSTOP_INVERTING
        #define Y2_MIN_PIN Y_MIN_PIN
      #elif Y2_USE_ENDSTOP == _YMAX_
        #define Y2_MIN_ENDSTOP_INVERTING Y_MAX_ENDSTOP_INVERTING
        #define Y2_MIN_PIN Y_MAX_PIN
      #elif Y2_USE_ENDSTOP == _ZMIN_
        #define Y2_MIN_ENDSTOP_INVERTING Z_MIN_ENDSTOP_INVERTING
        #define Y2_MIN_PIN Z_MIN_PIN
      #elif Y2_USE_ENDSTOP == _ZMAX_
        #define Y2_MIN_ENDSTOP_INVERTING Z_MAX_ENDSTOP_INVERTING
        #define Y2_MIN_PIN Z_MAX_PIN
      #else
        #define Y2_MIN_ENDSTOP_INVERTING false
      #endif
      #define Y2_MAX_ENDSTOP_INVERTING false
    #endif
  #endif

  // Is an endstop plug used for the Y2 endstop or the bed probe?
  #define IS_Y2_ENDSTOP(A,M) (ENABLED(Y_DUAL_ENDSTOPS) && Y2_USE_ENDSTOP == _##A##M##_)

  /**
   * Z_DUAL_ENDSTOPS endstop reassignment
   */
  #if ENABLED(Z_DUAL_ENDSTOPS)
    #if Z_HOME_DIR > 0
      #if Z2_USE_ENDSTOP == _XMIN_
        #define Z2_MAX_ENDSTOP_INVERTING X_MIN_ENDSTOP_INVERTING
        #define Z2_MAX_PIN X_MIN_PIN
      #elif Z2_USE_ENDSTOP == _XMAX_
        #define Z2_MAX_ENDSTOP_INVERTING X_MAX_ENDSTOP_INVERTING
        #define Z2_MAX_PIN X_MAX_PIN
      #elif Z2_USE_ENDSTOP == _YMIN_
        #define Z2_MAX_ENDSTOP_INVERTING Y_MIN_ENDSTOP_INVERTING
        #define Z2_MAX_PIN Y_MIN_PIN
      #elif Z2_USE_ENDSTOP == _YMAX_
        #define Z2_MAX_ENDSTOP_INVERTING Y_MAX_ENDSTOP_INVERTING
        #define Z2_MAX_PIN Y_MAX_PIN
      #elif Z2_USE_ENDSTOP == _ZMIN_
        #define Z2_MAX_ENDSTOP_INVERTING Z_MIN_ENDSTOP_INVERTING
        #define Z2_MAX_PIN Z_MIN_PIN
      #elif Z2_USE_ENDSTOP == _ZMAX_
        #define Z2_MAX_ENDSTOP_INVERTING Z_MAX_ENDSTOP_INVERTING
        #define Z2_MAX_PIN Z_MAX_PIN
      #else
        #define Z2_MAX_ENDSTOP_INVERTING false
      #endif
      #define Z2_MIN_ENDSTOP_INVERTING false
    #else
      #if Z2_USE_ENDSTOP == _XMIN_
        #define Z2_MIN_ENDSTOP_INVERTING X_MIN_ENDSTOP_INVERTING
        #define Z2_MIN_PIN X_MIN_PIN
      #elif Z2_USE_ENDSTOP == _XMAX_
        #define Z2_MIN_ENDSTOP_INVERTING X_MAX_ENDSTOP_INVERTING
        #define Z2_MIN_PIN X_MAX_PIN
      #elif Z2_USE_ENDSTOP == _YMIN_
        #define Z2_MIN_ENDSTOP_INVERTING Y_MIN_ENDSTOP_INVERTING
        #define Z2_MIN_PIN Y_MIN_PIN
      #elif Z2_USE_ENDSTOP == _YMAX_
        #define Z2_MIN_ENDSTOP_INVERTING Y_MAX_ENDSTOP_INVERTING
        #define Z2_MIN_PIN Y_MAX_PIN
      #elif Z2_USE_ENDSTOP == _ZMIN_
        #define Z2_MIN_ENDSTOP_INVERTING Z_MIN_ENDSTOP_INVERTING
        #define Z2_MIN_PIN Z_MIN_PIN
      #elif Z2_USE_ENDSTOP == _ZMAX_
        #define Z2_MIN_ENDSTOP_INVERTING Z_MAX_ENDSTOP_INVERTING
        #define Z2_MIN_PIN Z_MAX_PIN
      #else
        #define Z2_MIN_ENDSTOP_INVERTING false
      #endif
      #define Z2_MAX_ENDSTOP_INVERTING false
    #endif
  #endif

  // Is an endstop plug used for the Z2 endstop or the bed probe?
  #define IS_Z2_OR_PROBE(A,M) ( \
       (ENABLED(Z_DUAL_ENDSTOPS) && Z2_USE_ENDSTOP == _##A##M##_) \
    || (ENABLED(Z_MIN_PROBE_ENDSTOP) && Z_MIN_PROBE_PIN == A##_##M##_PIN ) )

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

  // Steppers
  #define HAS_X_ENABLE      (PIN_EXISTS(X_ENABLE))
  #define HAS_X_DIR         (PIN_EXISTS(X_DIR))
  #define HAS_X_STEP        (PIN_EXISTS(X_STEP))
  #define HAS_X_MICROSTEPS  (PIN_EXISTS(X_MS1))

  #define HAS_X2_ENABLE     (PIN_EXISTS(X2_ENABLE))
  #define HAS_X2_DIR        (PIN_EXISTS(X2_DIR))
  #define HAS_X2_STEP       (PIN_EXISTS(X2_STEP))
  #define HAS_Y_MICROSTEPS  (PIN_EXISTS(Y_MS1))

  #define HAS_Y_ENABLE      (PIN_EXISTS(Y_ENABLE))
  #define HAS_Y_DIR         (PIN_EXISTS(Y_DIR))
  #define HAS_Y_STEP        (PIN_EXISTS(Y_STEP))
  #define HAS_Z_MICROSTEPS  (PIN_EXISTS(Z_MS1))

  #define HAS_Y2_ENABLE     (PIN_EXISTS(Y2_ENABLE))
  #define HAS_Y2_DIR        (PIN_EXISTS(Y2_DIR))
  #define HAS_Y2_STEP       (PIN_EXISTS(Y2_STEP))

  #define HAS_Z_ENABLE      (PIN_EXISTS(Z_ENABLE))
  #define HAS_Z_DIR         (PIN_EXISTS(Z_DIR))
  #define HAS_Z_STEP        (PIN_EXISTS(Z_STEP))

  #define HAS_Z2_ENABLE     (PIN_EXISTS(Z2_ENABLE))
  #define HAS_Z2_DIR        (PIN_EXISTS(Z2_DIR))
  #define HAS_Z2_STEP       (PIN_EXISTS(Z2_STEP))

  // Extruder steppers and solenoids
  #define HAS_E0_ENABLE     (PIN_EXISTS(E0_ENABLE))
  #define HAS_E0_DIR        (PIN_EXISTS(E0_DIR))
  #define HAS_E0_STEP       (PIN_EXISTS(E0_STEP))
  #define HAS_E0_MICROSTEPS (PIN_EXISTS(E0_MS1))
  #define HAS_SOLENOID_0    (PIN_EXISTS(SOL0))

  #define HAS_E1_ENABLE     (PIN_EXISTS(E1_ENABLE))
  #define HAS_E1_DIR        (PIN_EXISTS(E1_DIR))
  #define HAS_E1_STEP       (PIN_EXISTS(E1_STEP))
  #define HAS_E1_MICROSTEPS (PIN_EXISTS(E1_MS1))
  #define HAS_SOLENOID_1    (PIN_EXISTS(SOL1))

  #define HAS_E2_ENABLE     (PIN_EXISTS(E2_ENABLE))
  #define HAS_E2_DIR        (PIN_EXISTS(E2_DIR))
  #define HAS_E2_STEP       (PIN_EXISTS(E2_STEP))
  #define HAS_E2_MICROSTEPS (PIN_EXISTS(E2_MS1))
  #define HAS_SOLENOID_2    (PIN_EXISTS(SOL2))

  #define HAS_E3_ENABLE     (PIN_EXISTS(E3_ENABLE))
  #define HAS_E3_DIR        (PIN_EXISTS(E3_DIR))
  #define HAS_E3_STEP       (PIN_EXISTS(E3_STEP))
  #define HAS_E3_MICROSTEPS (PIN_EXISTS(E3_MS1))
  #define HAS_SOLENOID_3    (PIN_EXISTS(SOL3))

  #define HAS_E4_ENABLE     (PIN_EXISTS(E4_ENABLE))
  #define HAS_E4_DIR        (PIN_EXISTS(E4_DIR))
  #define HAS_E4_STEP       (PIN_EXISTS(E4_STEP))
  #define HAS_E4_MICROSTEPS (PIN_EXISTS(E4_MS1))
  #define HAS_SOLENOID_4    (PIN_EXISTS(SOL4))

  // Trinamic Stepper Drivers
  #define HAS_TRINAMIC (ENABLED(HAVE_TMC2130) || ENABLED(HAVE_TMC2208) || ENABLED(IS_TRAMS))
  #define  X_IS_TRINAMIC (ENABLED( X_IS_TMC2130) || ENABLED( X_IS_TMC2208) || ENABLED(IS_TRAMS))
  #define X2_IS_TRINAMIC (ENABLED(X2_IS_TMC2130) || ENABLED(X2_IS_TMC2208))
  #define  Y_IS_TRINAMIC (ENABLED( Y_IS_TMC2130) || ENABLED( Y_IS_TMC2208) || ENABLED(IS_TRAMS))
  #define Y2_IS_TRINAMIC (ENABLED(Y2_IS_TMC2130) || ENABLED(Y2_IS_TMC2208))
  #define  Z_IS_TRINAMIC (ENABLED( Z_IS_TMC2130) || ENABLED( Z_IS_TMC2208) || ENABLED(IS_TRAMS))
  #define Z2_IS_TRINAMIC (ENABLED(Z2_IS_TMC2130) || ENABLED(Z2_IS_TMC2208))
  #define E0_IS_TRINAMIC (ENABLED(E0_IS_TMC2130) || ENABLED(E0_IS_TMC2208) || ENABLED(IS_TRAMS))
  #define E1_IS_TRINAMIC (ENABLED(E1_IS_TMC2130) || ENABLED(E1_IS_TMC2208))
  #define E2_IS_TRINAMIC (ENABLED(E2_IS_TMC2130) || ENABLED(E2_IS_TMC2208))
  #define E3_IS_TRINAMIC (ENABLED(E3_IS_TMC2130) || ENABLED(E3_IS_TMC2208))
  #define E4_IS_TRINAMIC (ENABLED(E4_IS_TMC2130) || ENABLED(E4_IS_TMC2208))

  // Disable Z axis sensorless homing if a probe is used to home the Z axis
  #if ENABLED(SENSORLESS_HOMING) && HOMING_Z_WITH_PROBE
    #undef Z_HOMING_SENSITIVITY
  #endif

  // Endstops and bed probe
  #define HAS_X_MIN (PIN_EXISTS(X_MIN) && !IS_X2_ENDSTOP(X,MIN) && !IS_Y2_ENDSTOP(X,MIN) && !IS_Z2_OR_PROBE(X,MIN))
  #define HAS_X_MAX (PIN_EXISTS(X_MAX) && !IS_X2_ENDSTOP(X,MAX) && !IS_Y2_ENDSTOP(X,MAX) && !IS_Z2_OR_PROBE(X,MAX))
  #define HAS_Y_MIN (PIN_EXISTS(Y_MIN) && !IS_X2_ENDSTOP(Y,MIN) && !IS_Y2_ENDSTOP(Y,MIN) && !IS_Z2_OR_PROBE(Y,MIN))
  #define HAS_Y_MAX (PIN_EXISTS(Y_MAX) && !IS_X2_ENDSTOP(Y,MAX) && !IS_Y2_ENDSTOP(Y,MAX) && !IS_Z2_OR_PROBE(Y,MAX))
  #define HAS_Z_MIN (PIN_EXISTS(Z_MIN) && !IS_X2_ENDSTOP(Z,MIN) && !IS_Y2_ENDSTOP(Z,MIN) && !IS_Z2_OR_PROBE(Z,MIN))
  #define HAS_Z_MAX (PIN_EXISTS(Z_MAX) && !IS_X2_ENDSTOP(Z,MAX) && !IS_Y2_ENDSTOP(Z,MAX) && !IS_Z2_OR_PROBE(Z,MAX))
  #define HAS_X2_MIN (PIN_EXISTS(X2_MIN))
  #define HAS_X2_MAX (PIN_EXISTS(X2_MAX))
  #define HAS_Y2_MIN (PIN_EXISTS(Y2_MIN))
  #define HAS_Y2_MAX (PIN_EXISTS(Y2_MAX))
  #define HAS_Z2_MIN (PIN_EXISTS(Z2_MIN))
  #define HAS_Z2_MAX (PIN_EXISTS(Z2_MAX))
  #define HAS_Z_MIN_PROBE_PIN (PIN_EXISTS(Z_MIN_PROBE))

  // Thermistors
  #define HAS_TEMP_0 (PIN_EXISTS(TEMP_0) && TEMP_SENSOR_0 != 0 && TEMP_SENSOR_0 > -2)
  #define HAS_TEMP_1 (PIN_EXISTS(TEMP_1) && TEMP_SENSOR_1 != 0 && TEMP_SENSOR_1 > -2)
  #define HAS_TEMP_2 (PIN_EXISTS(TEMP_2) && TEMP_SENSOR_2 != 0 && TEMP_SENSOR_2 > -2)
  #define HAS_TEMP_3 (PIN_EXISTS(TEMP_3) && TEMP_SENSOR_3 != 0 && TEMP_SENSOR_3 > -2)
  #define HAS_TEMP_4 (PIN_EXISTS(TEMP_4) && TEMP_SENSOR_4 != 0 && TEMP_SENSOR_4 > -2)
  #define HAS_TEMP_HOTEND (HAS_TEMP_0 || ENABLED(HEATER_0_USES_MAX6675))
  #define HAS_TEMP_BED (PIN_EXISTS(TEMP_BED) && TEMP_SENSOR_BED != 0 && TEMP_SENSOR_BED > -2)

  // Heaters
  #define HAS_HEATER_0 (PIN_EXISTS(HEATER_0))
  #define HAS_HEATER_1 (PIN_EXISTS(HEATER_1))
  #define HAS_HEATER_2 (PIN_EXISTS(HEATER_2))
  #define HAS_HEATER_3 (PIN_EXISTS(HEATER_3))
  #define HAS_HEATER_4 (PIN_EXISTS(HEATER_4))
  #define HAS_HEATER_BED (PIN_EXISTS(HEATER_BED))

  // Thermal protection
  #define HAS_THERMALLY_PROTECTED_BED (ENABLED(THERMAL_PROTECTION_BED) && HAS_TEMP_BED && HAS_HEATER_BED)
  #define WATCH_HOTENDS (ENABLED(THERMAL_PROTECTION_HOTENDS) && WATCH_TEMP_PERIOD > 0)
  #define WATCH_THE_BED (HAS_THERMALLY_PROTECTED_BED && WATCH_BED_TEMP_PERIOD > 0)

  // Auto fans
  #define HAS_AUTO_FAN_0 (PIN_EXISTS(E0_AUTO_FAN))
  #define HAS_AUTO_FAN_1 (HOTENDS > 1 && PIN_EXISTS(E1_AUTO_FAN))
  #define HAS_AUTO_FAN_2 (HOTENDS > 2 && PIN_EXISTS(E2_AUTO_FAN))
  #define HAS_AUTO_FAN_3 (HOTENDS > 3 && PIN_EXISTS(E3_AUTO_FAN))
  #define HAS_AUTO_FAN_4 (HOTENDS > 4 && PIN_EXISTS(E4_AUTO_FAN))
  #define HAS_AUTO_FAN (HAS_AUTO_FAN_0 || HAS_AUTO_FAN_1 || HAS_AUTO_FAN_2 || HAS_AUTO_FAN_3)
  #define AUTO_1_IS_0 (E1_AUTO_FAN_PIN == E0_AUTO_FAN_PIN)
  #define AUTO_2_IS_0 (E2_AUTO_FAN_PIN == E0_AUTO_FAN_PIN)
  #define AUTO_2_IS_1 (E2_AUTO_FAN_PIN == E1_AUTO_FAN_PIN)
  #define AUTO_3_IS_0 (E3_AUTO_FAN_PIN == E0_AUTO_FAN_PIN)
  #define AUTO_3_IS_1 (E3_AUTO_FAN_PIN == E1_AUTO_FAN_PIN)
  #define AUTO_3_IS_2 (E3_AUTO_FAN_PIN == E2_AUTO_FAN_PIN)
  #define AUTO_4_IS_0 (E4_AUTO_FAN_PIN == E0_AUTO_FAN_PIN)
  #define AUTO_4_IS_1 (E4_AUTO_FAN_PIN == E1_AUTO_FAN_PIN)
  #define AUTO_4_IS_2 (E4_AUTO_FAN_PIN == E2_AUTO_FAN_PIN)
  #define AUTO_4_IS_3 (E4_AUTO_FAN_PIN == E3_AUTO_FAN_PIN)

  // Other fans
  #define HAS_FAN0 (PIN_EXISTS(FAN))
  #define HAS_FAN1 (PIN_EXISTS(FAN1) && CONTROLLER_FAN_PIN != FAN1_PIN && E0_AUTO_FAN_PIN != FAN1_PIN && E1_AUTO_FAN_PIN != FAN1_PIN && E2_AUTO_FAN_PIN != FAN1_PIN && E3_AUTO_FAN_PIN != FAN1_PIN)
  #define HAS_FAN2 (PIN_EXISTS(FAN2) && CONTROLLER_FAN_PIN != FAN2_PIN && E0_AUTO_FAN_PIN != FAN2_PIN && E1_AUTO_FAN_PIN != FAN2_PIN && E2_AUTO_FAN_PIN != FAN2_PIN && E3_AUTO_FAN_PIN != FAN2_PIN)
  #define HAS_CONTROLLER_FAN (PIN_EXISTS(CONTROLLER_FAN))

  // Servos
  #define HAS_SERVOS (defined(NUM_SERVOS) && NUM_SERVOS > 0)
  #define HAS_SERVO_0 (PIN_EXISTS(SERVO0))
  #define HAS_SERVO_1 (PIN_EXISTS(SERVO1))
  #define HAS_SERVO_2 (PIN_EXISTS(SERVO2))
  #define HAS_SERVO_3 (PIN_EXISTS(SERVO3))

  // Sensors
  #define HAS_FILAMENT_WIDTH_SENSOR (PIN_EXISTS(FILWIDTH))
  #define HAS_FIL_RUNOUT (PIN_EXISTS(FIL_RUNOUT))

  // User Interface
  #define HAS_HOME (PIN_EXISTS(HOME))
  #define HAS_KILL (PIN_EXISTS(KILL))
  #define HAS_SUICIDE (PIN_EXISTS(SUICIDE))
  #define HAS_PHOTOGRAPH (PIN_EXISTS(PHOTOGRAPH))
  #define HAS_BUZZER (PIN_EXISTS(BEEPER) || ENABLED(LCD_USE_I2C_BUZZER))
  #define HAS_CASE_LIGHT (PIN_EXISTS(CASE_LIGHT) && ENABLED(CASE_LIGHT_ENABLE))

  // Digital control
  #define HAS_MICROSTEPS (HAS_X_MICROSTEPS || HAS_Y_MICROSTEPS || HAS_Z_MICROSTEPS || HAS_E0_MICROSTEPS || HAS_E1_MICROSTEPS || HAS_E2_MICROSTEPS || HAS_E3_MICROSTEPS || HAS_E4_MICROSTEPS)
  #define HAS_STEPPER_RESET (PIN_EXISTS(STEPPER_RESET))
  #define HAS_DIGIPOTSS (PIN_EXISTS(DIGIPOTSS))
  #define HAS_MOTOR_CURRENT_PWM (PIN_EXISTS(MOTOR_CURRENT_PWM_XY) || PIN_EXISTS(MOTOR_CURRENT_PWM_Z) || PIN_EXISTS(MOTOR_CURRENT_PWM_E))

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
        #if HOTENDS > 4
          #define WRITE_HEATER_4(v) WRITE(HEATER_4_PIN, v)
        #endif // HOTENDS > 4
      #endif // HOTENDS > 3
    #endif // HOTENDS > 2
  #endif // HOTENDS > 1
  #if ENABLED(HEATERS_PARALLEL)
    #define WRITE_HEATER_0(v) { WRITE_HEATER_0P(v); WRITE_HEATER_1(v); }
  #else
    #define WRITE_HEATER_0(v) WRITE_HEATER_0P(v)
  #endif

  /**
   * Heated bed requires settings
   */
  #if HAS_HEATER_BED
    #ifndef MAX_BED_POWER
      #define MAX_BED_POWER 255
    #endif
    #ifndef HEATER_BED_INVERTING
      #define HEATER_BED_INVERTING false
    #endif
    #define WRITE_HEATER_BED(v) WRITE(HEATER_BED_PIN, (v) ^ HEATER_BED_INVERTING)
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
   * Part Cooling fan multipliexer
   */
  #define HAS_FANMUX PIN_EXISTS(FANMUX0)

  /**
   * Servos and probes
   */

  #if HAS_SERVOS
    #ifndef Z_ENDSTOP_SERVO_NR
      #define Z_ENDSTOP_SERVO_NR -1
    #endif
  #endif

  #define HAS_BED_PROBE (PROBE_SELECTED && DISABLED(PROBE_MANUALLY))

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
   * XYZ Bed Skew Correction
   */
  #if ENABLED(SKEW_CORRECTION)
    #define SKEW_FACTOR_MIN -1
    #define SKEW_FACTOR_MAX 1

    #define _GET_SIDE(a,b,c) (SQRT(2*sq(a)+2*sq(b)-4*sq(c))*0.5)
    #define _SKEW_SIDE(a,b,c) tan(M_PI*0.5-acos((sq(a)-sq(b)-sq(c))/(2*c*b)))
    #define _SKEW_FACTOR(a,b,c) _SKEW_SIDE(float(a),_GET_SIDE(float(a),float(b),float(c)),float(c))

    #ifndef XY_SKEW_FACTOR
      constexpr float XY_SKEW_FACTOR = (
        #if defined(XY_DIAG_AC) && defined(XY_DIAG_BD) && defined(XY_SIDE_AD)
          _SKEW_FACTOR(XY_DIAG_AC, XY_DIAG_BD, XY_SIDE_AD)
        #else
          0.0
        #endif
      );
    #endif
    #ifndef XZ_SKEW_FACTOR
      #if defined(XY_SIDE_AD) && !defined(XZ_SIDE_AD)
        #define XZ_SIDE_AD XY_SIDE_AD
      #endif
      constexpr float XZ_SKEW_FACTOR = (
        #if defined(XZ_DIAG_AC) && defined(XZ_DIAG_BD) && defined(XZ_SIDE_AD)
          _SKEW_FACTOR(XZ_DIAG_AC, XZ_DIAG_BD, XZ_SIDE_AD)
        #else
          0.0
        #endif
      );
    #endif
    #ifndef YZ_SKEW_FACTOR
      constexpr float YZ_SKEW_FACTOR = (
        #if defined(YZ_DIAG_AC) && defined(YZ_DIAG_BD) && defined(YZ_SIDE_AD)
          _SKEW_FACTOR(YZ_DIAG_AC, YZ_DIAG_BD, YZ_SIDE_AD)
        #else
          0.0
        #endif
      );
    #endif
  #endif // SKEW_CORRECTION

  /**
   * Heater & Fan Pausing
   */
  #if FAN_COUNT == 0
    #undef PROBING_FANS_OFF
  #endif
  #define QUIET_PROBING (HAS_BED_PROBE && (ENABLED(PROBING_HEATERS_OFF) || ENABLED(PROBING_FANS_OFF) || DELAY_BEFORE_PROBING > 0))
  #define HEATER_IDLE_HANDLER (ENABLED(ADVANCED_PAUSE_FEATURE) || ENABLED(PROBING_HEATERS_OFF))

  /**
   * Only constrain Z on DELTA / SCARA machines
   */
  #if IS_KINEMATIC
    #undef MIN_SOFTWARE_ENDSTOP_X
    #undef MIN_SOFTWARE_ENDSTOP_Y
    #undef MAX_SOFTWARE_ENDSTOP_X
    #undef MAX_SOFTWARE_ENDSTOP_Y
  #endif

  /**
   * Delta radius/rod trimmers/angle trimmers
   */
  #if ENABLED(DELTA)
    #ifndef DELTA_PROBEABLE_RADIUS
      #define DELTA_PROBEABLE_RADIUS DELTA_PRINTABLE_RADIUS
    #endif
    #ifndef DELTA_CALIBRATION_RADIUS
      #define DELTA_CALIBRATION_RADIUS DELTA_PRINTABLE_RADIUS - 10
    #endif
    #ifndef DELTA_ENDSTOP_ADJ
      #define DELTA_ENDSTOP_ADJ { 0, 0, 0 }
    #endif
    #ifndef DELTA_TOWER_ANGLE_TRIM
      #define DELTA_TOWER_ANGLE_TRIM {0, 0, 0}
    #endif
    #ifndef DELTA_RADIUS_TRIM_TOWER
      #define DELTA_RADIUS_TRIM_TOWER {0, 0, 0}
    #endif
    #ifndef DELTA_DIAGONAL_ROD_TRIM_TOWER
      #define DELTA_DIAGONAL_ROD_TRIM_TOWER {0, 0, 0}
    #endif
  #endif

  /**
   * Set granular options based on the specific type of leveling
   */
  #define UBL_SEGMENTED  (ENABLED(AUTO_BED_LEVELING_UBL) && (ENABLED(DELTA) || ENABLED(SEGMENT_LEVELED_MOVES)))
  #define ABL_PLANAR     (ENABLED(AUTO_BED_LEVELING_LINEAR) || ENABLED(AUTO_BED_LEVELING_3POINT))
  #define ABL_GRID       (ENABLED(AUTO_BED_LEVELING_LINEAR) || ENABLED(AUTO_BED_LEVELING_BILINEAR))
  #define OLDSCHOOL_ABL  (ABL_PLANAR || ABL_GRID)
  #define HAS_ABL        (OLDSCHOOL_ABL || ENABLED(AUTO_BED_LEVELING_UBL))
  #define HAS_LEVELING   (HAS_ABL || ENABLED(MESH_BED_LEVELING))
  #define HAS_AUTOLEVEL  (HAS_ABL && DISABLED(PROBE_MANUALLY))
  #define HAS_MESH       (ENABLED(AUTO_BED_LEVELING_BILINEAR) || ENABLED(AUTO_BED_LEVELING_UBL) || ENABLED(MESH_BED_LEVELING))
  #define PLANNER_LEVELING      (OLDSCHOOL_ABL || ENABLED(MESH_BED_LEVELING) || UBL_SEGMENTED || ENABLED(SKEW_CORRECTION))
  #define HAS_PROBING_PROCEDURE (HAS_ABL || ENABLED(Z_MIN_PROBE_REPEATABILITY_TEST))
  #if HAS_PROBING_PROCEDURE
    #define PROBE_BED_WIDTH abs(RIGHT_PROBE_BED_POSITION - (LEFT_PROBE_BED_POSITION))
    #define PROBE_BED_HEIGHT abs(BACK_PROBE_BED_POSITION - (FRONT_PROBE_BED_POSITION))
  #endif

  #if ENABLED(SEGMENT_LEVELED_MOVES) && !defined(LEVELED_SEGMENT_LENGTH)
    #define LEVELED_SEGMENT_LENGTH 5
  #endif

  /**
   * Bed Probing rectangular bounds
   * These can be further constrained in code for Delta and SCARA
   */
  #if ENABLED(DELTA)
    // Probing points may be verified at compile time within the radius
    // using static_assert(HYPOT2(X2-X1,Y2-Y1)<=sq(DELTA_PRINTABLE_RADIUS),"bad probe point!")
    // so that may be added to SanityCheck.h in the future.
    #define _MIN_PROBE_X (X_CENTER - DELTA_PRINTABLE_RADIUS)
    #define _MIN_PROBE_Y (Y_CENTER - DELTA_PRINTABLE_RADIUS)
    #define _MAX_PROBE_X (X_CENTER + DELTA_PRINTABLE_RADIUS)
    #define _MAX_PROBE_Y (Y_CENTER + DELTA_PRINTABLE_RADIUS)
  #elif IS_SCARA
    #define SCARA_PRINTABLE_RADIUS (SCARA_LINKAGE_1 + SCARA_LINKAGE_2)
    #define _MIN_PROBE_X (X_CENTER - (SCARA_PRINTABLE_RADIUS))
    #define _MIN_PROBE_Y (Y_CENTER - (SCARA_PRINTABLE_RADIUS))
    #define _MAX_PROBE_X (X_CENTER +  SCARA_PRINTABLE_RADIUS)
    #define _MAX_PROBE_Y (Y_CENTER +  SCARA_PRINTABLE_RADIUS)
  #else
    // Boundaries for Cartesian probing based on bed limits
    #define _MIN_PROBE_X (max(X_MIN_BED, X_MIN_POS + X_PROBE_OFFSET_FROM_EXTRUDER))
    #define _MIN_PROBE_Y (max(Y_MIN_BED, Y_MIN_POS + Y_PROBE_OFFSET_FROM_EXTRUDER))
    #define _MAX_PROBE_X (min(X_MAX_BED, X_MAX_POS + X_PROBE_OFFSET_FROM_EXTRUDER))
    #define _MAX_PROBE_Y (min(Y_MAX_BED, Y_MAX_POS + Y_PROBE_OFFSET_FROM_EXTRUDER))
  #endif

  // Allow configuration to override these for special purposes
  #ifndef MIN_PROBE_X
    #define MIN_PROBE_X _MIN_PROBE_X
  #endif
  #ifndef MIN_PROBE_Y
    #define MIN_PROBE_Y _MIN_PROBE_Y
  #endif
  #ifndef MAX_PROBE_X
    #define MAX_PROBE_X _MAX_PROBE_X
  #endif
  #ifndef MAX_PROBE_Y
    #define MAX_PROBE_Y _MAX_PROBE_Y
  #endif

  /**
   * Default mesh area is an area with an inset margin on the print area.
   */
  #if ENABLED(MESH_BED_LEVELING) || ENABLED(AUTO_BED_LEVELING_UBL)
    #if IS_KINEMATIC
      // Probing points may be verified at compile time within the radius
      // using static_assert(HYPOT2(X2-X1,Y2-Y1)<=sq(DELTA_PRINTABLE_RADIUS),"bad probe point!")
      // so that may be added to SanityCheck.h in the future.
      #define _MESH_MIN_X (MIN_PROBE_X + MESH_INSET)
      #define _MESH_MIN_Y (MIN_PROBE_Y + MESH_INSET)
      #define _MESH_MAX_X (MAX_PROBE_X - (MESH_INSET))
      #define _MESH_MAX_Y (MAX_PROBE_Y - (MESH_INSET))
    #else
      // Boundaries for Cartesian probing based on set limits
      #if ENABLED(AUTO_BED_LEVELING_UBL)
        #define _MESH_MIN_X (max(X_MIN_BED + MESH_INSET, X_MIN_POS))  // UBL is careful not to probe off the bed.  It does not
        #define _MESH_MIN_Y (max(Y_MIN_BED + MESH_INSET, Y_MIN_POS))  // need *_PROBE_OFFSET_FROM_EXTRUDER in the mesh dimensions
        #define _MESH_MAX_X (min(X_MAX_BED - (MESH_INSET), X_MAX_POS))
        #define _MESH_MAX_Y (min(Y_MAX_BED - (MESH_INSET), Y_MAX_POS))
      #else
        #define _MESH_MIN_X (max(X_MIN_BED + MESH_INSET, X_MIN_POS + X_PROBE_OFFSET_FROM_EXTRUDER))
        #define _MESH_MIN_Y (max(Y_MIN_BED + MESH_INSET, Y_MIN_POS + Y_PROBE_OFFSET_FROM_EXTRUDER))
        #define _MESH_MAX_X (min(X_MAX_BED - (MESH_INSET), X_MAX_POS + X_PROBE_OFFSET_FROM_EXTRUDER))
        #define _MESH_MAX_Y (min(Y_MAX_BED - (MESH_INSET), Y_MAX_POS + Y_PROBE_OFFSET_FROM_EXTRUDER))
      #endif

    #endif
    /**
     * These may be overridden in Configuration if a smaller area is wanted
     */
    #if ENABLED(MESH_BED_LEVELING) || ENABLED(AUTO_BED_LEVELING_UBL)
      #ifndef MESH_MIN_X
        #define MESH_MIN_X _MESH_MIN_X
      #endif
      #ifndef MESH_MIN_Y
        #define MESH_MIN_Y _MESH_MIN_Y
      #endif
      #ifndef MESH_MAX_X
        #define MESH_MAX_X _MESH_MAX_X
      #endif
      #ifndef MESH_MAX_Y
        #define MESH_MAX_Y _MESH_MAX_Y
      #endif
    #endif
  #endif // MESH_BED_LEVELING || AUTO_BED_LEVELING_UBL

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
   * VIKI2, miniVIKI, and AZSMZ_12864 require DOGLCD_SCK and DOGLCD_MOSI to be defined.
   */
  #if ENABLED(VIKI2) || ENABLED(miniVIKI) || ENABLED(AZSMZ_12864)
    #ifndef DOGLCD_SCK
      #define DOGLCD_SCK  SCK_PIN
    #endif
    #ifndef DOGLCD_MOSI
      #define DOGLCD_MOSI MOSI_PIN
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
  #if Z_CLEARANCE_BETWEEN_PROBES > Z_HOMING_HEIGHT
    #define MANUAL_PROBE_HEIGHT Z_CLEARANCE_BETWEEN_PROBES
  #else
    #define MANUAL_PROBE_HEIGHT Z_HOMING_HEIGHT
  #endif

  // Stepper pulse duration, in cycles
  #define STEP_PULSE_CYCLES ((MINIMUM_STEPPER_PULSE) * CYCLES_PER_MICROSECOND)

  #if ENABLED(SDCARD_SORT_ALPHA)
    #define HAS_FOLDER_SORTING (FOLDER_SORTING || ENABLED(SDSORT_GCODE))
  #endif

  // Updated G92 behavior shifts the workspace
  #define HAS_POSITION_SHIFT DISABLED(NO_WORKSPACE_OFFSETS)
  // The home offset also shifts the coordinate space
  #define HAS_HOME_OFFSET (DISABLED(NO_WORKSPACE_OFFSETS) && DISABLED(DELTA))
  // Either offset yields extra calculations on all moves
  #define HAS_WORKSPACE_OFFSET (HAS_POSITION_SHIFT || HAS_HOME_OFFSET)
  // M206 doesn't apply to DELTA
  #define HAS_M206_COMMAND (HAS_HOME_OFFSET && DISABLED(DELTA))

  // LCD timeout to status screen default is 15s
  #ifndef LCD_TIMEOUT_TO_STATUS
    #define LCD_TIMEOUT_TO_STATUS 15000
  #endif

  // Shorthand
  #define GRID_MAX_POINTS ((GRID_MAX_POINTS_X) * (GRID_MAX_POINTS_Y))

  // Add commands that need sub-codes to this list
  #define USE_GCODE_SUBCODES ENABLED(G38_PROBE_TARGET) || ENABLED(CNC_COORDINATE_SYSTEMS)

  // Parking Extruder
  #if ENABLED(PARKING_EXTRUDER)
    #ifndef PARKING_EXTRUDER_GRAB_DISTANCE
      #define PARKING_EXTRUDER_GRAB_DISTANCE 0
    #endif
    #ifndef PARKING_EXTRUDER_SOLENOIDS_PINS_ACTIVE
      #define PARKING_EXTRUDER_SOLENOIDS_PINS_ACTIVE HIGH
    #endif
  #endif

  // Number of VFAT entries used. Each entry has 13 UTF-16 characters
  #if ENABLED(SCROLL_LONG_FILENAMES)
    #define MAX_VFAT_ENTRIES (5)
  #else
    #define MAX_VFAT_ENTRIES (2)
  #endif

  // Set defaults for unspecified LED user colors
  #if ENABLED(LED_CONTROL_MENU)
    #ifndef LED_USER_PRESET_RED
      #define LED_USER_PRESET_RED       255
    #endif
    #ifndef LED_USER_PRESET_GREEN
      #define LED_USER_PRESET_GREEN     255
    #endif
    #ifndef LED_USER_PRESET_BLUE
      #define LED_USER_PRESET_BLUE      255
    #endif
    #ifndef LED_USER_PRESET_WHITE
      #define LED_USER_PRESET_WHITE     0
    #endif
    #ifndef LED_USER_PRESET_BRIGHTNESS
      #ifdef NEOPIXEL_BRIGHTNESS
        #define LED_USER_PRESET_BRIGHTNESS NEOPIXEL_BRIGHTNESS
      #else
        #define LED_USER_PRESET_BRIGHTNESS 255
      #endif
    #endif
  #endif

  // Nozzle park
  #if ENABLED(NOZZLE_PARK_FEATURE) && ENABLED(DELTA)
    #undef NOZZLE_PARK_Z_FEEDRATE
    #define NOZZLE_PARK_Z_FEEDRATE NOZZLE_PARK_XY_FEEDRATE
  #endif

#endif // CONDITIONALS_POST_H
