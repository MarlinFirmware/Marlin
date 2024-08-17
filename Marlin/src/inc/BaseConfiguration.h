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
 * BaseConfiguration.h - Generated using makeBaseConfigs.py
 */

#ifndef CONFIGURATION_H_VERSION
  #define CONFIGURATION_H_VERSION 02010300
#endif
#ifndef STRING_CONFIG_H_AUTHOR
  #define STRING_CONFIG_H_AUTHOR "(none, default config)"
#endif
#ifndef MOTHERBOARD
  #define MOTHERBOARD BOARD_RAMPS_14_EFB
#endif
#ifndef SERIAL_PORT
  #define SERIAL_PORT 0
#endif
#ifndef BAUDRATE
  #define BAUDRATE 250000
#endif
#ifndef X_DRIVER_TYPE
  #define X_DRIVER_TYPE A4988
#endif
#ifndef Y_DRIVER_TYPE
  #define Y_DRIVER_TYPE A4988
#endif
#ifndef Z_DRIVER_TYPE
  #define Z_DRIVER_TYPE A4988
#endif
#ifndef E0_DRIVER_TYPE
  #define E0_DRIVER_TYPE A4988
#endif
#ifdef I_DRIVER_TYPE
  #ifndef AXIS4_NAME
    #define AXIS4_NAME 'A'
  #endif
  #ifndef AXIS4_ROTATES
    #define AXIS4_ROTATES
  #endif
#endif
#ifdef J_DRIVER_TYPE
  #ifndef AXIS5_NAME
    #define AXIS5_NAME 'B'
  #endif
  #ifndef AXIS5_ROTATES
    #define AXIS5_ROTATES
  #endif
#endif
#ifdef K_DRIVER_TYPE
  #ifndef AXIS6_NAME
    #define AXIS6_NAME 'C'
  #endif
  #ifndef AXIS6_ROTATES
    #define AXIS6_ROTATES
  #endif
#endif
#ifdef U_DRIVER_TYPE
  #ifndef AXIS7_NAME
    #define AXIS7_NAME 'U'
  #endif
#endif
#ifdef V_DRIVER_TYPE
  #ifndef AXIS8_NAME
    #define AXIS8_NAME 'V'
  #endif
#endif
#ifdef W_DRIVER_TYPE
  #ifndef AXIS9_NAME
    #define AXIS9_NAME 'W'
  #endif
#endif
#ifndef EXTRUDERS
  #define EXTRUDERS 1
#endif
#ifndef DEFAULT_NOMINAL_FILAMENT_DIA
  #define DEFAULT_NOMINAL_FILAMENT_DIA 1.75
#endif
#if ENABLED(SWITCHING_EXTRUDER)
  #ifndef SWITCHING_EXTRUDER_SERVO_NR
    #define SWITCHING_EXTRUDER_SERVO_NR 0
  #endif
  #ifndef SWITCHING_EXTRUDER_SERVO_ANGLES
    #define SWITCHING_EXTRUDER_SERVO_ANGLES { 0, 90 }
  #endif
  #if EXTRUDERS > 3
    #ifndef SWITCHING_EXTRUDER_E23_SERVO_NR
      #define SWITCHING_EXTRUDER_E23_SERVO_NR 1
    #endif
  #endif
#endif
#if ENABLED(SWITCHING_NOZZLE)
  #ifndef SWITCHING_NOZZLE_SERVO_NR
    #define SWITCHING_NOZZLE_SERVO_NR 0
  #endif
  #ifndef SWITCHING_NOZZLE_SERVO_ANGLES
    #define SWITCHING_NOZZLE_SERVO_ANGLES { 0, 90 }
  #endif
  #ifndef SWITCHING_NOZZLE_SERVO_DWELL
    #define SWITCHING_NOZZLE_SERVO_DWELL 2500
  #endif
#endif
#if ANY(PARKING_EXTRUDER, MAGNETIC_PARKING_EXTRUDER)
  #ifndef PARKING_EXTRUDER_PARKING_X
    #define PARKING_EXTRUDER_PARKING_X { -78, 184 }
  #endif
  #ifndef PARKING_EXTRUDER_GRAB_DISTANCE
    #define PARKING_EXTRUDER_GRAB_DISTANCE 1
  #endif
  #if ENABLED(PARKING_EXTRUDER)
    #ifndef PARKING_EXTRUDER_SOLENOIDS_INVERT
      #define PARKING_EXTRUDER_SOLENOIDS_INVERT
    #endif
    #ifndef PARKING_EXTRUDER_SOLENOIDS_PINS_ACTIVE
      #define PARKING_EXTRUDER_SOLENOIDS_PINS_ACTIVE LOW
    #endif
    #ifndef PARKING_EXTRUDER_SOLENOIDS_DELAY
      #define PARKING_EXTRUDER_SOLENOIDS_DELAY 250
    #endif
  #elif ENABLED(MAGNETIC_PARKING_EXTRUDER)
    #ifndef MPE_FAST_SPEED
      #define MPE_FAST_SPEED 9000
    #endif
    #ifndef MPE_SLOW_SPEED
      #define MPE_SLOW_SPEED 4500
    #endif
    #ifndef MPE_TRAVEL_DISTANCE
      #define MPE_TRAVEL_DISTANCE 10
    #endif
    #ifndef MPE_COMPENSATION
      #define MPE_COMPENSATION 0
    #endif
  #endif
#endif
#if ANY(SWITCHING_TOOLHEAD, MAGNETIC_SWITCHING_TOOLHEAD, ELECTROMAGNETIC_SWITCHING_TOOLHEAD)
  #ifndef SWITCHING_TOOLHEAD_Y_POS
    #define SWITCHING_TOOLHEAD_Y_POS 235
  #endif
  #ifndef SWITCHING_TOOLHEAD_Y_SECURITY
    #define SWITCHING_TOOLHEAD_Y_SECURITY 10
  #endif
  #ifndef SWITCHING_TOOLHEAD_Y_CLEAR
    #define SWITCHING_TOOLHEAD_Y_CLEAR 60
  #endif
  #ifndef SWITCHING_TOOLHEAD_X_POS
    #define SWITCHING_TOOLHEAD_X_POS { 215, 0 }
  #endif
  #if ENABLED(SWITCHING_TOOLHEAD)
    #ifndef SWITCHING_TOOLHEAD_SERVO_NR
      #define SWITCHING_TOOLHEAD_SERVO_NR 2
    #endif
    #ifndef SWITCHING_TOOLHEAD_SERVO_ANGLES
      #define SWITCHING_TOOLHEAD_SERVO_ANGLES { 0, 180 }
    #endif
  #elif ENABLED(MAGNETIC_SWITCHING_TOOLHEAD)
    #ifndef SWITCHING_TOOLHEAD_Y_RELEASE
      #define SWITCHING_TOOLHEAD_Y_RELEASE 5
    #endif
    #ifndef SWITCHING_TOOLHEAD_X_SECURITY
      #define SWITCHING_TOOLHEAD_X_SECURITY { 90, 150 }
    #endif
    #if ENABLED(PRIME_BEFORE_REMOVE)
      #ifndef SWITCHING_TOOLHEAD_PRIME_MM
        #define SWITCHING_TOOLHEAD_PRIME_MM 20
      #endif
      #ifndef SWITCHING_TOOLHEAD_RETRACT_MM
        #define SWITCHING_TOOLHEAD_RETRACT_MM 10
      #endif
      #ifndef SWITCHING_TOOLHEAD_PRIME_FEEDRATE
        #define SWITCHING_TOOLHEAD_PRIME_FEEDRATE 300
      #endif
      #ifndef SWITCHING_TOOLHEAD_RETRACT_FEEDRATE
        #define SWITCHING_TOOLHEAD_RETRACT_FEEDRATE 2400
      #endif
    #endif
  #elif ENABLED(ELECTROMAGNETIC_SWITCHING_TOOLHEAD)
    #ifndef SWITCHING_TOOLHEAD_Z_HOP
      #define SWITCHING_TOOLHEAD_Z_HOP 2
    #endif
  #endif
#endif
#if ENABLED(MIXING_EXTRUDER)
  #ifndef MIXING_STEPPERS
    #define MIXING_STEPPERS 2
  #endif
  #ifndef MIXING_VIRTUAL_TOOLS
    #define MIXING_VIRTUAL_TOOLS 16
  #endif
#endif
#if ENABLED(PSU_CONTROL)
  #ifndef PSU_ACTIVE_STATE
    #define PSU_ACTIVE_STATE LOW
  #endif
  #ifndef PS_EDM_RESPONSE
    #define PS_EDM_RESPONSE 250
  #endif
  #if ENABLED(AUTO_POWER_CONTROL)
    #ifndef AUTO_POWER_FANS
      #define AUTO_POWER_FANS
    #endif
    #ifndef AUTO_POWER_E_FANS
      #define AUTO_POWER_E_FANS
    #endif
    #ifndef AUTO_POWER_CONTROLLERFAN
      #define AUTO_POWER_CONTROLLERFAN
    #endif
    #ifndef AUTO_POWER_CHAMBER_FAN
      #define AUTO_POWER_CHAMBER_FAN
    #endif
    #ifndef AUTO_POWER_COOLER_FAN
      #define AUTO_POWER_COOLER_FAN
    #endif
    #ifndef AUTO_POWER_SPINDLE_LASER
      #define AUTO_POWER_SPINDLE_LASER
    #endif
    #ifndef POWER_TIMEOUT
      #define POWER_TIMEOUT 30
    #endif
  #endif
#endif
#ifndef TEMP_SENSOR_0
  #define TEMP_SENSOR_0 1
#endif
#ifndef TEMP_SENSOR_1
  #define TEMP_SENSOR_1 0
#endif
#ifndef TEMP_SENSOR_2
  #define TEMP_SENSOR_2 0
#endif
#ifndef TEMP_SENSOR_3
  #define TEMP_SENSOR_3 0
#endif
#ifndef TEMP_SENSOR_4
  #define TEMP_SENSOR_4 0
#endif
#ifndef TEMP_SENSOR_5
  #define TEMP_SENSOR_5 0
#endif
#ifndef TEMP_SENSOR_6
  #define TEMP_SENSOR_6 0
#endif
#ifndef TEMP_SENSOR_7
  #define TEMP_SENSOR_7 0
#endif
#ifndef TEMP_SENSOR_BED
  #define TEMP_SENSOR_BED 1
#endif
#ifndef TEMP_SENSOR_PROBE
  #define TEMP_SENSOR_PROBE 0
#endif
#ifndef TEMP_SENSOR_CHAMBER
  #define TEMP_SENSOR_CHAMBER 0
#endif
#ifndef TEMP_SENSOR_COOLER
  #define TEMP_SENSOR_COOLER 0
#endif
#ifndef TEMP_SENSOR_BOARD
  #define TEMP_SENSOR_BOARD 0
#endif
#ifndef TEMP_SENSOR_SOC
  #define TEMP_SENSOR_SOC 0
#endif
#ifndef TEMP_SENSOR_REDUNDANT
  #define TEMP_SENSOR_REDUNDANT 0
#endif
#ifndef DUMMY_THERMISTOR_998_VALUE
  #define DUMMY_THERMISTOR_998_VALUE 25
#endif
#ifndef DUMMY_THERMISTOR_999_VALUE
  #define DUMMY_THERMISTOR_999_VALUE 100
#endif
#if TEMP_SENSOR_IS_MAX_TC(0)
  #ifndef MAX31865_SENSOR_OHMS_0
    #define MAX31865_SENSOR_OHMS_0 100
  #endif
  #ifndef MAX31865_CALIBRATION_OHMS_0
    #define MAX31865_CALIBRATION_OHMS_0 430
  #endif
#endif
#if TEMP_SENSOR_IS_MAX_TC(1)
  #ifndef MAX31865_SENSOR_OHMS_1
    #define MAX31865_SENSOR_OHMS_1 100
  #endif
  #ifndef MAX31865_CALIBRATION_OHMS_1
    #define MAX31865_CALIBRATION_OHMS_1 430
  #endif
#endif
#if TEMP_SENSOR_IS_MAX_TC(2)
  #ifndef MAX31865_SENSOR_OHMS_2
    #define MAX31865_SENSOR_OHMS_2 100
  #endif
  #ifndef MAX31865_CALIBRATION_OHMS_2
    #define MAX31865_CALIBRATION_OHMS_2 430
  #endif
#endif
#if HAS_E_TEMP_SENSOR
  #ifndef TEMP_RESIDENCY_TIME
    #define TEMP_RESIDENCY_TIME 10
  #endif
  #ifndef TEMP_WINDOW
    #define TEMP_WINDOW 1
  #endif
  #ifndef TEMP_HYSTERESIS
    #define TEMP_HYSTERESIS 3
  #endif
#endif
#if TEMP_SENSOR_BED
  #ifndef TEMP_BED_RESIDENCY_TIME
    #define TEMP_BED_RESIDENCY_TIME 10
  #endif
  #ifndef TEMP_BED_WINDOW
    #define TEMP_BED_WINDOW 1
  #endif
  #ifndef TEMP_BED_HYSTERESIS
    #define TEMP_BED_HYSTERESIS 3
  #endif
#endif
#if TEMP_SENSOR_CHAMBER
  #ifndef TEMP_CHAMBER_RESIDENCY_TIME
    #define TEMP_CHAMBER_RESIDENCY_TIME 10
  #endif
  #ifndef TEMP_CHAMBER_WINDOW
    #define TEMP_CHAMBER_WINDOW 1
  #endif
  #ifndef TEMP_CHAMBER_HYSTERESIS
    #define TEMP_CHAMBER_HYSTERESIS 3
  #endif
#endif
#if TEMP_SENSOR_REDUNDANT
  #ifndef TEMP_SENSOR_REDUNDANT_SOURCE
    #define TEMP_SENSOR_REDUNDANT_SOURCE E1
  #endif
  #ifndef TEMP_SENSOR_REDUNDANT_TARGET
    #define TEMP_SENSOR_REDUNDANT_TARGET E0
  #endif
  #ifndef TEMP_SENSOR_REDUNDANT_MAX_DIFF
    #define TEMP_SENSOR_REDUNDANT_MAX_DIFF 10
  #endif
#endif
#ifndef HEATER_0_MINTEMP
  #define HEATER_0_MINTEMP 5
#endif
#ifndef HEATER_1_MINTEMP
  #define HEATER_1_MINTEMP 5
#endif
#ifndef HEATER_2_MINTEMP
  #define HEATER_2_MINTEMP 5
#endif
#ifndef HEATER_3_MINTEMP
  #define HEATER_3_MINTEMP 5
#endif
#ifndef HEATER_4_MINTEMP
  #define HEATER_4_MINTEMP 5
#endif
#ifndef HEATER_5_MINTEMP
  #define HEATER_5_MINTEMP 5
#endif
#ifndef HEATER_6_MINTEMP
  #define HEATER_6_MINTEMP 5
#endif
#ifndef HEATER_7_MINTEMP
  #define HEATER_7_MINTEMP 5
#endif
#ifndef BED_MINTEMP
  #define BED_MINTEMP 5
#endif
#ifndef CHAMBER_MINTEMP
  #define CHAMBER_MINTEMP 5
#endif
#ifndef HEATER_0_MAXTEMP
  #define HEATER_0_MAXTEMP 275
#endif
#ifndef HEATER_1_MAXTEMP
  #define HEATER_1_MAXTEMP 275
#endif
#ifndef HEATER_2_MAXTEMP
  #define HEATER_2_MAXTEMP 275
#endif
#ifndef HEATER_3_MAXTEMP
  #define HEATER_3_MAXTEMP 275
#endif
#ifndef HEATER_4_MAXTEMP
  #define HEATER_4_MAXTEMP 275
#endif
#ifndef HEATER_5_MAXTEMP
  #define HEATER_5_MAXTEMP 275
#endif
#ifndef HEATER_6_MAXTEMP
  #define HEATER_6_MAXTEMP 275
#endif
#ifndef HEATER_7_MAXTEMP
  #define HEATER_7_MAXTEMP 275
#endif
#ifndef BED_MAXTEMP
  #define BED_MAXTEMP 150
#endif
#ifndef CHAMBER_MAXTEMP
  #define CHAMBER_MAXTEMP 60
#endif
#ifndef HOTEND_OVERSHOOT
  #define HOTEND_OVERSHOOT 15
#endif
#ifndef BED_OVERSHOOT
  #define BED_OVERSHOOT 10
#endif
#ifndef COOLER_OVERSHOOT
  #define COOLER_OVERSHOOT 2
#endif
#ifndef PIDTEMP
  #define PIDTEMP
#endif
#ifndef PID_MAX
  #define PID_MAX 255
#endif
#ifndef PID_K1
  #define PID_K1 0.95
#endif
#if ENABLED(PIDTEMP)
  #if ENABLED(PID_PARAMS_PER_HOTEND)
    #ifndef DEFAULT_Kp_LIST
      #define DEFAULT_Kp_LIST {  22.20,  22.20 }
    #endif
    #ifndef DEFAULT_Ki_LIST
      #define DEFAULT_Ki_LIST {   1.08,   1.08 }
    #endif
    #ifndef DEFAULT_Kd_LIST
      #define DEFAULT_Kd_LIST { 114.00, 114.00 }
    #endif
  #else
    #ifndef DEFAULT_Kp
      #define DEFAULT_Kp 22.20
    #endif
    #ifndef DEFAULT_Ki
      #define DEFAULT_Ki 1.08
    #endif
    #ifndef DEFAULT_Kd
      #define DEFAULT_Kd 114.00
    #endif
  #endif
#else
  #ifndef BANG_MAX
    #define BANG_MAX 255
  #endif
#endif
#if ENABLED(MPCTEMP)
  #ifndef MPC_AUTOTUNE
    #define MPC_AUTOTUNE
  #endif
  #ifndef MPC_MAX
    #define MPC_MAX 255
  #endif
  #ifndef MPC_HEATER_POWER
    #define MPC_HEATER_POWER { 40.0f }
  #endif
  #ifndef MPC_INCLUDE_FAN
    #define MPC_INCLUDE_FAN
  #endif
  #ifndef MPC_BLOCK_HEAT_CAPACITY
    #define MPC_BLOCK_HEAT_CAPACITY { 16.7f }
  #endif
  #ifndef MPC_SENSOR_RESPONSIVENESS
    #define MPC_SENSOR_RESPONSIVENESS { 0.22f }
  #endif
  #ifndef MPC_AMBIENT_XFER_COEFF
    #define MPC_AMBIENT_XFER_COEFF { 0.068f }
  #endif
  #if ENABLED(MPC_INCLUDE_FAN)
    #ifndef MPC_AMBIENT_XFER_COEFF_FAN255
      #define MPC_AMBIENT_XFER_COEFF_FAN255 { 0.097f }
    #endif
  #endif
  #ifndef FILAMENT_HEAT_CAPACITY_PERMM
    #define FILAMENT_HEAT_CAPACITY_PERMM { 5.6e-3f }
  #endif
  #ifndef MPC_SMOOTHING_FACTOR
    #define MPC_SMOOTHING_FACTOR 0.5f
  #endif
  #ifndef MPC_MIN_AMBIENT_CHANGE
    #define MPC_MIN_AMBIENT_CHANGE 1.0f
  #endif
  #ifndef MPC_STEADYSTATE
    #define MPC_STEADYSTATE 0.5f
  #endif
  #ifndef MPC_TUNING_POS
    #define MPC_TUNING_POS { X_CENTER, Y_CENTER, 1.0f }
  #endif
  #ifndef MPC_TUNING_END_Z
    #define MPC_TUNING_END_Z 10.0f
  #endif
#endif
#ifndef MAX_BED_POWER
  #define MAX_BED_POWER 255
#endif
#if ENABLED(PIDTEMPBED)
  #ifndef DEFAULT_bedKp
    #define DEFAULT_bedKp 10.00
  #endif
  #ifndef DEFAULT_bedKi
    #define DEFAULT_bedKi .023
  #endif
  #ifndef DEFAULT_bedKd
    #define DEFAULT_bedKd 305.4
  #endif
#else
#endif
#ifndef MAX_CHAMBER_POWER
  #define MAX_CHAMBER_POWER 255
#endif
#if ENABLED(PIDTEMPCHAMBER)
  #ifndef MIN_CHAMBER_POWER
    #define MIN_CHAMBER_POWER 0
  #endif
  #ifndef DEFAULT_chamberKp
    #define DEFAULT_chamberKp 37.04
  #endif
  #ifndef DEFAULT_chamberKi
    #define DEFAULT_chamberKi 1.40
  #endif
  #ifndef DEFAULT_chamberKd
    #define DEFAULT_chamberKd 655.17
  #endif
#endif
#if ANY(PIDTEMP, PIDTEMPBED, PIDTEMPCHAMBER)
  #ifndef PID_FUNCTIONAL_RANGE
    #define PID_FUNCTIONAL_RANGE 10
  #endif
#endif
#ifndef PREVENT_COLD_EXTRUSION
  #define PREVENT_COLD_EXTRUSION
#endif
#ifndef EXTRUDE_MINTEMP
  #define EXTRUDE_MINTEMP 170
#endif
#ifndef PREVENT_LENGTHY_EXTRUDE
  #define PREVENT_LENGTHY_EXTRUDE
#endif
#ifndef EXTRUDE_MAXLENGTH
  #define EXTRUDE_MAXLENGTH 200
#endif
#ifndef THERMAL_PROTECTION_HOTENDS
  #define THERMAL_PROTECTION_HOTENDS
#endif
#ifndef THERMAL_PROTECTION_BED
  #define THERMAL_PROTECTION_BED
#endif
#ifndef THERMAL_PROTECTION_CHAMBER
  #define THERMAL_PROTECTION_CHAMBER
#endif
#ifndef THERMAL_PROTECTION_COOLER
  #define THERMAL_PROTECTION_COOLER
#endif
#if ENABLED(POLARGRAPH)
  #ifndef POLARGRAPH_MAX_BELT_LEN
    #define POLARGRAPH_MAX_BELT_LEN 1035.0
  #endif
  #ifndef DEFAULT_SEGMENTS_PER_SECOND
    #define DEFAULT_SEGMENTS_PER_SECOND 5
  #endif
  #ifndef PEN_UP_DOWN_MENU
    #define PEN_UP_DOWN_MENU
  #endif
#endif
#if ENABLED(DELTA)
  #ifndef DEFAULT_SEGMENTS_PER_SECOND
    #define DEFAULT_SEGMENTS_PER_SECOND 200
  #endif
  #if ENABLED(DELTA_AUTO_CALIBRATION)
    #ifndef DELTA_CALIBRATION_DEFAULT_POINTS
      #define DELTA_CALIBRATION_DEFAULT_POINTS 4
    #endif
  #endif
  #if ANY(DELTA_AUTO_CALIBRATION, DELTA_CALIBRATION_MENU)
    #ifndef PROBE_MANUALLY_STEP
      #define PROBE_MANUALLY_STEP 0.05
    #endif
  #endif
  #ifndef PRINTABLE_RADIUS
    #define PRINTABLE_RADIUS 140.0
  #endif
  #ifndef DELTA_MAX_RADIUS
    #define DELTA_MAX_RADIUS 140.0
  #endif
  #ifndef DELTA_DIAGONAL_ROD
    #define DELTA_DIAGONAL_ROD 250.0
  #endif
  #ifndef DELTA_HEIGHT
    #define DELTA_HEIGHT 250.00
  #endif
  #ifndef DELTA_ENDSTOP_ADJ
    #define DELTA_ENDSTOP_ADJ { 0.0, 0.0, 0.0 }
  #endif
  #ifndef DELTA_RADIUS
    #define DELTA_RADIUS 124.0
  #endif
  #ifndef DELTA_TOWER_ANGLE_TRIM
    #define DELTA_TOWER_ANGLE_TRIM { 0.0, 0.0, 0.0 }
  #endif
#endif
#if ANY(MORGAN_SCARA, MP_SCARA)
  #ifndef DEFAULT_SEGMENTS_PER_SECOND
    #define DEFAULT_SEGMENTS_PER_SECOND 200
  #endif
  #ifndef SCARA_LINKAGE_1
    #define SCARA_LINKAGE_1 150
  #endif
  #ifndef SCARA_LINKAGE_2
    #define SCARA_LINKAGE_2 150
  #endif
  #ifndef SCARA_OFFSET_X
    #define SCARA_OFFSET_X 100
  #endif
  #ifndef SCARA_OFFSET_Y
    #define SCARA_OFFSET_Y -56
  #endif
  #if ENABLED(MORGAN_SCARA)
    #ifndef FEEDRATE_SCALING
      #define FEEDRATE_SCALING
    #endif
    #ifndef MIDDLE_DEAD_ZONE_R
      #define MIDDLE_DEAD_ZONE_R 0
    #endif
  #elif ENABLED(MP_SCARA)
    #ifndef SCARA_OFFSET_THETA1
      #define SCARA_OFFSET_THETA1 12
    #endif
    #ifndef SCARA_OFFSET_THETA2
      #define SCARA_OFFSET_THETA2 131
    #endif
  #endif
#endif
#if ENABLED(AXEL_TPARA)
  #ifndef DEBUG_TPARA_KINEMATICS
    #define DEBUG_TPARA_KINEMATICS
  #endif
  #ifndef DEFAULT_SEGMENTS_PER_SECOND
    #define DEFAULT_SEGMENTS_PER_SECOND 200
  #endif
  #ifndef TPARA_LINKAGE_1
    #define TPARA_LINKAGE_1 120
  #endif
  #ifndef TPARA_LINKAGE_2
    #define TPARA_LINKAGE_2 120
  #endif
  #ifndef TPARA_OFFSET_X
    #define TPARA_OFFSET_X 0
  #endif
  #ifndef TPARA_OFFSET_Y
    #define TPARA_OFFSET_Y 0
  #endif
  #ifndef TPARA_OFFSET_Z
    #define TPARA_OFFSET_Z 0
  #endif
  #ifndef FEEDRATE_SCALING
    #define FEEDRATE_SCALING
  #endif
  #ifndef MIDDLE_DEAD_ZONE_R
    #define MIDDLE_DEAD_ZONE_R 0
  #endif
#endif
#if ENABLED(POLAR)
  #ifndef DEFAULT_SEGMENTS_PER_SECOND
    #define DEFAULT_SEGMENTS_PER_SECOND 180
  #endif
  #ifndef PRINTABLE_RADIUS
    #define PRINTABLE_RADIUS 82.0f
  #endif
  #ifndef POLAR_FAST_RADIUS
    #define POLAR_FAST_RADIUS 3.0f
  #endif
  #ifndef POLAR_CENTER_OFFSET
    #define POLAR_CENTER_OFFSET 0.0f
  #endif
  #ifndef FEEDRATE_SCALING
    #define FEEDRATE_SCALING
  #endif
#endif
#ifndef ENDSTOPPULLUPS
  #define ENDSTOPPULLUPS
#endif
#ifndef X_MIN_ENDSTOP_HIT_STATE
  #define X_MIN_ENDSTOP_HIT_STATE HIGH
#endif
#ifndef X_MAX_ENDSTOP_HIT_STATE
  #define X_MAX_ENDSTOP_HIT_STATE HIGH
#endif
#ifndef Y_MIN_ENDSTOP_HIT_STATE
  #define Y_MIN_ENDSTOP_HIT_STATE HIGH
#endif
#ifndef Y_MAX_ENDSTOP_HIT_STATE
  #define Y_MAX_ENDSTOP_HIT_STATE HIGH
#endif
#ifndef Z_MIN_ENDSTOP_HIT_STATE
  #define Z_MIN_ENDSTOP_HIT_STATE HIGH
#endif
#ifndef Z_MAX_ENDSTOP_HIT_STATE
  #define Z_MAX_ENDSTOP_HIT_STATE HIGH
#endif
#ifndef I_MIN_ENDSTOP_HIT_STATE
  #define I_MIN_ENDSTOP_HIT_STATE HIGH
#endif
#ifndef I_MAX_ENDSTOP_HIT_STATE
  #define I_MAX_ENDSTOP_HIT_STATE HIGH
#endif
#ifndef J_MIN_ENDSTOP_HIT_STATE
  #define J_MIN_ENDSTOP_HIT_STATE HIGH
#endif
#ifndef J_MAX_ENDSTOP_HIT_STATE
  #define J_MAX_ENDSTOP_HIT_STATE HIGH
#endif
#ifndef K_MIN_ENDSTOP_HIT_STATE
  #define K_MIN_ENDSTOP_HIT_STATE HIGH
#endif
#ifndef K_MAX_ENDSTOP_HIT_STATE
  #define K_MAX_ENDSTOP_HIT_STATE HIGH
#endif
#ifndef U_MIN_ENDSTOP_HIT_STATE
  #define U_MIN_ENDSTOP_HIT_STATE HIGH
#endif
#ifndef U_MAX_ENDSTOP_HIT_STATE
  #define U_MAX_ENDSTOP_HIT_STATE HIGH
#endif
#ifndef V_MIN_ENDSTOP_HIT_STATE
  #define V_MIN_ENDSTOP_HIT_STATE HIGH
#endif
#ifndef V_MAX_ENDSTOP_HIT_STATE
  #define V_MAX_ENDSTOP_HIT_STATE HIGH
#endif
#ifndef W_MIN_ENDSTOP_HIT_STATE
  #define W_MIN_ENDSTOP_HIT_STATE HIGH
#endif
#ifndef W_MAX_ENDSTOP_HIT_STATE
  #define W_MAX_ENDSTOP_HIT_STATE HIGH
#endif
#ifndef Z_MIN_PROBE_ENDSTOP_HIT_STATE
  #define Z_MIN_PROBE_ENDSTOP_HIT_STATE HIGH
#endif
#ifndef DEFAULT_AXIS_STEPS_PER_UNIT
  #define DEFAULT_AXIS_STEPS_PER_UNIT { 80, 80, 400, 500 }
#endif
#ifndef EDITABLE_STEPS_PER_UNIT
  #define EDITABLE_STEPS_PER_UNIT
#endif
#ifndef DEFAULT_MAX_FEEDRATE
  #define DEFAULT_MAX_FEEDRATE { 300, 300, 5, 25 }
#endif
#if ENABLED(LIMITED_MAX_FR_EDITING)
  #ifndef MAX_FEEDRATE_EDIT_VALUES
    #define MAX_FEEDRATE_EDIT_VALUES { 600, 600, 10, 50 }
  #endif
#endif
#ifndef DEFAULT_MAX_ACCELERATION
  #define DEFAULT_MAX_ACCELERATION { 3000, 3000, 100, 10000 }
#endif
#if ENABLED(LIMITED_MAX_ACCEL_EDITING)
  #ifndef MAX_ACCEL_EDIT_VALUES
    #define MAX_ACCEL_EDIT_VALUES { 6000, 6000, 200, 20000 }
  #endif
#endif
#ifndef DEFAULT_ACCELERATION
  #define DEFAULT_ACCELERATION 3000
#endif
#ifndef DEFAULT_RETRACT_ACCELERATION
  #define DEFAULT_RETRACT_ACCELERATION 3000
#endif
#ifndef DEFAULT_TRAVEL_ACCELERATION
  #define DEFAULT_TRAVEL_ACCELERATION 3000
#endif
#if ENABLED(CLASSIC_JERK)
  #ifndef DEFAULT_XJERK
    #define DEFAULT_XJERK 10.0
  #endif
  #ifndef DEFAULT_YJERK
    #define DEFAULT_YJERK 10.0
  #endif
  #ifndef DEFAULT_ZJERK
    #define DEFAULT_ZJERK 0.3
  #endif
  #ifndef DEFAULT_EJERK
    #define DEFAULT_EJERK 5.0
  #endif
  #if ENABLED(LIMITED_JERK_EDITING)
    #ifndef MAX_JERK_EDIT_VALUES
      #define MAX_JERK_EDIT_VALUES { 20, 20, 0.6, 10 }
    #endif
  #endif
#endif
#if DISABLED(CLASSIC_JERK)
  #ifndef JUNCTION_DEVIATION_MM
    #define JUNCTION_DEVIATION_MM 0.013
  #endif
  #ifndef JD_HANDLE_SMALL_SEGMENTS
    #define JD_HANDLE_SMALL_SEGMENTS
  #endif
#endif
#ifndef Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN
  #define Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN
#endif
#if ENABLED(MAGLEV4)
  #ifndef MAGLEV_TRIGGER_DELAY
    #define MAGLEV_TRIGGER_DELAY 15
  #endif
#endif
#if ENABLED(TOUCH_MI_PROBE)
  #ifndef TOUCH_MI_RETRACT_Z
    #define TOUCH_MI_RETRACT_Z 0.5
  #endif
#endif
#if ENABLED(RACK_AND_PINION_PROBE)
  #ifndef Z_PROBE_DEPLOY_X
    #define Z_PROBE_DEPLOY_X X_MIN_POS
  #endif
  #ifndef Z_PROBE_RETRACT_X
    #define Z_PROBE_RETRACT_X X_MAX_POS
  #endif
#endif
#if ENABLED(MAG_MOUNTED_PROBE)
  #ifndef PROBE_DEPLOY_FEEDRATE
    #define PROBE_DEPLOY_FEEDRATE (133*60)
  #endif
  #ifndef PROBE_STOW_FEEDRATE
    #define PROBE_STOW_FEEDRATE (133*60)
  #endif
  #ifndef MAG_MOUNTED_DEPLOY_1
    #define MAG_MOUNTED_DEPLOY_1 { PROBE_DEPLOY_FEEDRATE, { 245, 114, 30 } }
  #endif
  #ifndef MAG_MOUNTED_DEPLOY_2
    #define MAG_MOUNTED_DEPLOY_2 { PROBE_DEPLOY_FEEDRATE, { 210, 114, 30 } }
  #endif
  #ifndef MAG_MOUNTED_DEPLOY_3
    #define MAG_MOUNTED_DEPLOY_3 { PROBE_DEPLOY_FEEDRATE, {   0,   0,  0 } }
  #endif
  #ifndef MAG_MOUNTED_DEPLOY_4
    #define MAG_MOUNTED_DEPLOY_4 { PROBE_DEPLOY_FEEDRATE, {   0,   0,  0 } }
  #endif
  #ifndef MAG_MOUNTED_DEPLOY_5
    #define MAG_MOUNTED_DEPLOY_5 { PROBE_DEPLOY_FEEDRATE, {   0,   0,  0 } }
  #endif
  #ifndef MAG_MOUNTED_STOW_1
    #define MAG_MOUNTED_STOW_1 { PROBE_STOW_FEEDRATE,   { 245, 114, 20 } }
  #endif
  #ifndef MAG_MOUNTED_STOW_2
    #define MAG_MOUNTED_STOW_2 { PROBE_STOW_FEEDRATE,   { 245, 114,  0 } }
  #endif
  #ifndef MAG_MOUNTED_STOW_3
    #define MAG_MOUNTED_STOW_3 { PROBE_STOW_FEEDRATE,   { 230, 114,  0 } }
  #endif
  #ifndef MAG_MOUNTED_STOW_4
    #define MAG_MOUNTED_STOW_4 { PROBE_STOW_FEEDRATE,   { 210, 114, 20 } }
  #endif
  #ifndef MAG_MOUNTED_STOW_5
    #define MAG_MOUNTED_STOW_5 { PROBE_STOW_FEEDRATE,   {   0,   0,  0 } }
  #endif
#endif
#if ENABLED(DUET_SMART_EFFECTOR)
  #ifndef SMART_EFFECTOR_MOD_PIN
    #define SMART_EFFECTOR_MOD_PIN -1
  #endif
#endif
#if ENABLED(Z_PROBE_ALLEN_KEY)
  #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_1
    #define Z_PROBE_ALLEN_KEY_DEPLOY_1 { 30.0, PRINTABLE_RADIUS, 100.0 }
  #endif
  #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_1_FEEDRATE
    #define Z_PROBE_ALLEN_KEY_DEPLOY_1_FEEDRATE XY_PROBE_FEEDRATE
  #endif
  #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_2
    #define Z_PROBE_ALLEN_KEY_DEPLOY_2 { 0.0, PRINTABLE_RADIUS, 100.0 }
  #endif
  #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_2_FEEDRATE
    #define Z_PROBE_ALLEN_KEY_DEPLOY_2_FEEDRATE (XY_PROBE_FEEDRATE)/10
  #endif
  #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_3
    #define Z_PROBE_ALLEN_KEY_DEPLOY_3 { 0.0, (PRINTABLE_RADIUS) * 0.75, 100.0 }
  #endif
  #ifndef Z_PROBE_ALLEN_KEY_DEPLOY_3_FEEDRATE
    #define Z_PROBE_ALLEN_KEY_DEPLOY_3_FEEDRATE XY_PROBE_FEEDRATE
  #endif
  #ifndef Z_PROBE_ALLEN_KEY_STOW_1
    #define Z_PROBE_ALLEN_KEY_STOW_1 { -64.0, 56.0, 23.0 }
  #endif
  #ifndef Z_PROBE_ALLEN_KEY_STOW_1_FEEDRATE
    #define Z_PROBE_ALLEN_KEY_STOW_1_FEEDRATE XY_PROBE_FEEDRATE
  #endif
  #ifndef Z_PROBE_ALLEN_KEY_STOW_2
    #define Z_PROBE_ALLEN_KEY_STOW_2 { -64.0, 56.0, 3.0 }
  #endif
  #ifndef Z_PROBE_ALLEN_KEY_STOW_2_FEEDRATE
    #define Z_PROBE_ALLEN_KEY_STOW_2_FEEDRATE (XY_PROBE_FEEDRATE)/10
  #endif
  #ifndef Z_PROBE_ALLEN_KEY_STOW_3
    #define Z_PROBE_ALLEN_KEY_STOW_3 { -64.0, 56.0, 50.0 }
  #endif
  #ifndef Z_PROBE_ALLEN_KEY_STOW_3_FEEDRATE
    #define Z_PROBE_ALLEN_KEY_STOW_3_FEEDRATE XY_PROBE_FEEDRATE
  #endif
  #ifndef Z_PROBE_ALLEN_KEY_STOW_4
    #define Z_PROBE_ALLEN_KEY_STOW_4 { 0.0, 0.0, 50.0 }
  #endif
  #ifndef Z_PROBE_ALLEN_KEY_STOW_4_FEEDRATE
    #define Z_PROBE_ALLEN_KEY_STOW_4_FEEDRATE XY_PROBE_FEEDRATE
  #endif
#endif
#ifndef NOZZLE_TO_PROBE_OFFSET
  #define NOZZLE_TO_PROBE_OFFSET { 10, 10, 0 }
#endif
#ifndef PROBING_TOOL
  #define PROBING_TOOL 0
#endif
#ifndef PROBING_MARGIN
  #define PROBING_MARGIN 10
#endif
#ifndef XY_PROBE_FEEDRATE
  #define XY_PROBE_FEEDRATE (133*60)
#endif
#ifndef Z_PROBE_FEEDRATE_FAST
  #define Z_PROBE_FEEDRATE_FAST (4*60)
#endif
#ifndef Z_PROBE_FEEDRATE_SLOW
  #define Z_PROBE_FEEDRATE_SLOW (Z_PROBE_FEEDRATE_FAST / 2)
#endif
#if ENABLED(PROBE_ACTIVATION_SWITCH)
  #ifndef PROBE_ACTIVATION_SWITCH_STATE
    #define PROBE_ACTIVATION_SWITCH_STATE LOW
  #endif
#endif
#if ENABLED(PROBE_TARE)
  #ifndef PROBE_TARE_TIME
    #define PROBE_TARE_TIME 200
  #endif
  #ifndef PROBE_TARE_DELAY
    #define PROBE_TARE_DELAY 200
  #endif
  #ifndef PROBE_TARE_STATE
    #define PROBE_TARE_STATE HIGH
  #endif
#endif
#ifndef Z_CLEARANCE_DEPLOY_PROBE
  #define Z_CLEARANCE_DEPLOY_PROBE 10
#endif
#ifndef Z_CLEARANCE_BETWEEN_PROBES
  #define Z_CLEARANCE_BETWEEN_PROBES 5
#endif
#ifndef Z_CLEARANCE_MULTI_PROBE
  #define Z_CLEARANCE_MULTI_PROBE 5
#endif
#ifndef Z_PROBE_ERROR_TOLERANCE
  #define Z_PROBE_ERROR_TOLERANCE 3
#endif
#ifndef Z_PROBE_LOW_POINT
  #define Z_PROBE_LOW_POINT -2
#endif
#if ENABLED(PREHEAT_BEFORE_PROBING)
  #ifndef PROBING_NOZZLE_TEMP
    #define PROBING_NOZZLE_TEMP 120
  #endif
  #ifndef PROBING_BED_TEMP
    #define PROBING_BED_TEMP 50
  #endif
#endif
#ifndef X_ENABLE_ON
  #define X_ENABLE_ON 0
#endif
#ifndef Y_ENABLE_ON
  #define Y_ENABLE_ON 0
#endif
#ifndef Z_ENABLE_ON
  #define Z_ENABLE_ON 0
#endif
#ifndef E_ENABLE_ON
  #define E_ENABLE_ON 0
#endif
#ifndef DISABLE_OTHER_EXTRUDERS
  #define DISABLE_OTHER_EXTRUDERS
#endif
#ifndef INVERT_X_DIR
  #define INVERT_X_DIR false
#endif
#ifndef INVERT_Y_DIR
  #define INVERT_Y_DIR true
#endif
#ifndef INVERT_Z_DIR
  #define INVERT_Z_DIR false
#endif
#ifndef INVERT_E0_DIR
  #define INVERT_E0_DIR false
#endif
#ifndef INVERT_E1_DIR
  #define INVERT_E1_DIR false
#endif
#ifndef INVERT_E2_DIR
  #define INVERT_E2_DIR false
#endif
#ifndef INVERT_E3_DIR
  #define INVERT_E3_DIR false
#endif
#ifndef INVERT_E4_DIR
  #define INVERT_E4_DIR false
#endif
#ifndef INVERT_E5_DIR
  #define INVERT_E5_DIR false
#endif
#ifndef INVERT_E6_DIR
  #define INVERT_E6_DIR false
#endif
#ifndef INVERT_E7_DIR
  #define INVERT_E7_DIR false
#endif
#ifndef X_HOME_DIR
  #define X_HOME_DIR -1
#endif
#ifndef Y_HOME_DIR
  #define Y_HOME_DIR -1
#endif
#ifndef Z_HOME_DIR
  #define Z_HOME_DIR -1
#endif
#ifndef X_BED_SIZE
  #define X_BED_SIZE 200
#endif
#ifndef Y_BED_SIZE
  #define Y_BED_SIZE 200
#endif
#ifndef X_MIN_POS
  #define X_MIN_POS 0
#endif
#ifndef Y_MIN_POS
  #define Y_MIN_POS 0
#endif
#ifndef Z_MIN_POS
  #define Z_MIN_POS 0
#endif
#ifndef X_MAX_POS
  #define X_MAX_POS X_BED_SIZE
#endif
#ifndef Y_MAX_POS
  #define Y_MAX_POS Y_BED_SIZE
#endif
#ifndef Z_MAX_POS
  #define Z_MAX_POS 200
#endif
#ifndef MIN_SOFTWARE_ENDSTOPS
  #define MIN_SOFTWARE_ENDSTOPS
#endif
#if ENABLED(MIN_SOFTWARE_ENDSTOPS)
  #ifndef MIN_SOFTWARE_ENDSTOP_X
    #define MIN_SOFTWARE_ENDSTOP_X
  #endif
  #ifndef MIN_SOFTWARE_ENDSTOP_Y
    #define MIN_SOFTWARE_ENDSTOP_Y
  #endif
  #ifndef MIN_SOFTWARE_ENDSTOP_Z
    #define MIN_SOFTWARE_ENDSTOP_Z
  #endif
  #ifndef MIN_SOFTWARE_ENDSTOP_I
    #define MIN_SOFTWARE_ENDSTOP_I
  #endif
  #ifndef MIN_SOFTWARE_ENDSTOP_J
    #define MIN_SOFTWARE_ENDSTOP_J
  #endif
  #ifndef MIN_SOFTWARE_ENDSTOP_K
    #define MIN_SOFTWARE_ENDSTOP_K
  #endif
  #ifndef MIN_SOFTWARE_ENDSTOP_U
    #define MIN_SOFTWARE_ENDSTOP_U
  #endif
  #ifndef MIN_SOFTWARE_ENDSTOP_V
    #define MIN_SOFTWARE_ENDSTOP_V
  #endif
  #ifndef MIN_SOFTWARE_ENDSTOP_W
    #define MIN_SOFTWARE_ENDSTOP_W
  #endif
#endif
#ifndef MAX_SOFTWARE_ENDSTOPS
  #define MAX_SOFTWARE_ENDSTOPS
#endif
#if ENABLED(MAX_SOFTWARE_ENDSTOPS)
  #ifndef MAX_SOFTWARE_ENDSTOP_X
    #define MAX_SOFTWARE_ENDSTOP_X
  #endif
  #ifndef MAX_SOFTWARE_ENDSTOP_Y
    #define MAX_SOFTWARE_ENDSTOP_Y
  #endif
  #ifndef MAX_SOFTWARE_ENDSTOP_Z
    #define MAX_SOFTWARE_ENDSTOP_Z
  #endif
  #ifndef MAX_SOFTWARE_ENDSTOP_I
    #define MAX_SOFTWARE_ENDSTOP_I
  #endif
  #ifndef MAX_SOFTWARE_ENDSTOP_J
    #define MAX_SOFTWARE_ENDSTOP_J
  #endif
  #ifndef MAX_SOFTWARE_ENDSTOP_K
    #define MAX_SOFTWARE_ENDSTOP_K
  #endif
  #ifndef MAX_SOFTWARE_ENDSTOP_U
    #define MAX_SOFTWARE_ENDSTOP_U
  #endif
  #ifndef MAX_SOFTWARE_ENDSTOP_V
    #define MAX_SOFTWARE_ENDSTOP_V
  #endif
  #ifndef MAX_SOFTWARE_ENDSTOP_W
    #define MAX_SOFTWARE_ENDSTOP_W
  #endif
#endif
#if ENABLED(FILAMENT_RUNOUT_SENSOR)
  #ifndef FIL_RUNOUT_ENABLED_DEFAULT
    #define FIL_RUNOUT_ENABLED_DEFAULT true
  #endif
  #ifndef NUM_RUNOUT_SENSORS
    #define NUM_RUNOUT_SENSORS 1
  #endif
  #ifndef FIL_RUNOUT_STATE
    #define FIL_RUNOUT_STATE LOW
  #endif
  #ifndef FIL_RUNOUT_PULLUP
    #define FIL_RUNOUT_PULLUP
  #endif
  #ifndef FILAMENT_RUNOUT_SCRIPT
    #define FILAMENT_RUNOUT_SCRIPT "M600"
  #endif
  #ifdef FILAMENT_RUNOUT_DISTANCE_MM
    #if ENABLED(FILAMENT_MOTION_SENSOR)
      #if ENABLED(FILAMENT_SWITCH_AND_MOTION)
        #ifndef NUM_MOTION_SENSORS
          #define NUM_MOTION_SENSORS 1
        #endif
      #endif
    #endif
  #endif
#endif
#if ENABLED(PREHEAT_BEFORE_LEVELING)
  #ifndef LEVELING_NOZZLE_TEMP
    #define LEVELING_NOZZLE_TEMP 120
  #endif
  #ifndef LEVELING_BED_TEMP
    #define LEVELING_BED_TEMP 50
  #endif
#endif
#if ANY(MESH_BED_LEVELING, AUTO_BED_LEVELING_UBL, PROBE_MANUALLY)
  #ifndef MANUAL_PROBE_START_Z
    #define MANUAL_PROBE_START_Z 0.2
  #endif
#endif
#if ANY(MESH_BED_LEVELING, AUTO_BED_LEVELING_BILINEAR, AUTO_BED_LEVELING_UBL)
  #ifndef ENABLE_LEVELING_FADE_HEIGHT
    #define ENABLE_LEVELING_FADE_HEIGHT
  #endif
  #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
    #ifndef DEFAULT_LEVELING_FADE_HEIGHT
      #define DEFAULT_LEVELING_FADE_HEIGHT 10.0
    #endif
  #endif
  #ifndef SEGMENT_LEVELED_MOVES
    #define SEGMENT_LEVELED_MOVES
  #endif
  #ifndef LEVELED_SEGMENT_LENGTH
    #define LEVELED_SEGMENT_LENGTH 5.0
  #endif
  #if ENABLED(G26_MESH_VALIDATION)
    #ifndef MESH_TEST_NOZZLE_SIZE
      #define MESH_TEST_NOZZLE_SIZE 0.4
    #endif
    #ifndef MESH_TEST_LAYER_HEIGHT
      #define MESH_TEST_LAYER_HEIGHT 0.2
    #endif
    #ifndef MESH_TEST_HOTEND_TEMP
      #define MESH_TEST_HOTEND_TEMP 205
    #endif
    #ifndef MESH_TEST_BED_TEMP
      #define MESH_TEST_BED_TEMP 60
    #endif
    #ifndef G26_XY_FEEDRATE
      #define G26_XY_FEEDRATE 20
    #endif
    #ifndef G26_XY_FEEDRATE_TRAVEL
      #define G26_XY_FEEDRATE_TRAVEL 100
    #endif
    #ifndef G26_RETRACT_MULTIPLIER
      #define G26_RETRACT_MULTIPLIER 1.0
    #endif
  #endif
#endif
#if ANY(AUTO_BED_LEVELING_LINEAR, AUTO_BED_LEVELING_BILINEAR)
  #ifndef GRID_MAX_POINTS_X
    #define GRID_MAX_POINTS_X 3
  #endif
  #ifndef GRID_MAX_POINTS_Y
    #define GRID_MAX_POINTS_Y GRID_MAX_POINTS_X
  #endif
  #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
    #if ENABLED(ABL_BILINEAR_SUBDIVISION)
      #ifndef BILINEAR_SUBDIVISIONS
        #define BILINEAR_SUBDIVISIONS 3
      #endif
    #endif
  #endif
#elif ENABLED(AUTO_BED_LEVELING_UBL)
  #ifndef MESH_INSET
    #define MESH_INSET 1
  #endif
  #ifndef GRID_MAX_POINTS_X
    #define GRID_MAX_POINTS_X 10
  #endif
  #ifndef GRID_MAX_POINTS_Y
    #define GRID_MAX_POINTS_Y GRID_MAX_POINTS_X
  #endif
  #ifndef UBL_MESH_EDIT_MOVES_Z
    #define UBL_MESH_EDIT_MOVES_Z
  #endif
  #ifndef UBL_SAVE_ACTIVE_ON_M500
    #define UBL_SAVE_ACTIVE_ON_M500
  #endif
  #if ENABLED(AVOID_OBSTACLES)
    #ifndef CLIP_W
      #define CLIP_W 23
    #endif
    #ifndef CLIP_H
      #define CLIP_H 14
    #endif
    #ifndef OBSTACLE1
      #define OBSTACLE1 { (X_BED_SIZE) / 4     - (CLIP_W) / 2,                       0, (X_BED_SIZE) / 4     + (CLIP_W) / 2, CLIP_H }
    #endif
    #ifndef OBSTACLE2
      #define OBSTACLE2 { (X_BED_SIZE) * 3 / 4 - (CLIP_W) / 2,                       0, (X_BED_SIZE) * 3 / 4 + (CLIP_W) / 2, CLIP_H }
    #endif
    #ifndef OBSTACLE3
      #define OBSTACLE3 { (X_BED_SIZE) / 4     - (CLIP_W) / 2, (Y_BED_SIZE) - (CLIP_H), (X_BED_SIZE) / 4     + (CLIP_W) / 2, Y_BED_SIZE }
    #endif
    #ifndef OBSTACLE4
      #define OBSTACLE4 { (X_BED_SIZE) * 3 / 4 - (CLIP_W) / 2, (Y_BED_SIZE) - (CLIP_H), (X_BED_SIZE) * 3 / 4 + (CLIP_W) / 2, Y_BED_SIZE }
    #endif
    #ifndef G29J_MESH_TILT_MARGIN
      #define G29J_MESH_TILT_MARGIN ((CLIP_H) + 1)
    #endif
  #endif
#elif ENABLED(MESH_BED_LEVELING)
  #ifndef MESH_INSET
    #define MESH_INSET 10
  #endif
  #ifndef GRID_MAX_POINTS_X
    #define GRID_MAX_POINTS_X 3
  #endif
  #ifndef GRID_MAX_POINTS_Y
    #define GRID_MAX_POINTS_Y GRID_MAX_POINTS_X
  #endif
#endif
#if ENABLED(LCD_BED_LEVELING)
  #ifndef MESH_EDIT_Z_STEP
    #define MESH_EDIT_Z_STEP 0.025
  #endif
  #ifndef LCD_PROBE_Z_RANGE
    #define LCD_PROBE_Z_RANGE 4
  #endif
#endif
#if ENABLED(LCD_BED_TRAMMING)
  #ifndef BED_TRAMMING_INSET_LFRB
    #define BED_TRAMMING_INSET_LFRB { 30, 30, 30, 30 }
  #endif
  #ifndef BED_TRAMMING_HEIGHT
    #define BED_TRAMMING_HEIGHT 0.0
  #endif
  #ifndef BED_TRAMMING_Z_HOP
    #define BED_TRAMMING_Z_HOP 4.0
  #endif
  #if ENABLED(BED_TRAMMING_USE_PROBE)
    #ifndef BED_TRAMMING_PROBE_TOLERANCE
      #define BED_TRAMMING_PROBE_TOLERANCE 0.1
    #endif
    #ifndef BED_TRAMMING_VERIFY_RAISED
      #define BED_TRAMMING_VERIFY_RAISED
    #endif
  #endif
  #ifndef BED_TRAMMING_LEVELING_ORDER
    #define BED_TRAMMING_LEVELING_ORDER { LF, RF, RB, LB }
  #endif
#endif
#if ENABLED(Z_SAFE_HOMING)
  #ifndef Z_SAFE_HOMING_X_POINT
    #define Z_SAFE_HOMING_X_POINT X_CENTER
  #endif
  #ifndef Z_SAFE_HOMING_Y_POINT
    #define Z_SAFE_HOMING_Y_POINT Y_CENTER
  #endif
#endif
#ifndef HOMING_FEEDRATE_MM_M
  #define HOMING_FEEDRATE_MM_M { (50*60), (50*60), (4*60) }
#endif
#ifndef VALIDATE_HOMING_ENDSTOPS
  #define VALIDATE_HOMING_ENDSTOPS
#endif
#if ENABLED(SKEW_CORRECTION)
  #ifndef XY_DIAG_AC
    #define XY_DIAG_AC 282.8427124746
  #endif
  #ifndef XY_DIAG_BD
    #define XY_DIAG_BD 282.8427124746
  #endif
  #ifndef XY_SIDE_AD
    #define XY_SIDE_AD 200
  #endif
  #if ENABLED(SKEW_CORRECTION_FOR_Z)
    #ifndef XZ_DIAG_AC
      #define XZ_DIAG_AC 282.8427124746
    #endif
    #ifndef XZ_DIAG_BD
      #define XZ_DIAG_BD 282.8427124746
    #endif
    #ifndef YZ_DIAG_AC
      #define YZ_DIAG_AC 282.8427124746
    #endif
    #ifndef YZ_DIAG_BD
      #define YZ_DIAG_BD 282.8427124746
    #endif
    #ifndef YZ_SIDE_AD
      #define YZ_SIDE_AD 200
    #endif
  #endif
#endif
#ifndef EEPROM_CHITCHAT
  #define EEPROM_CHITCHAT
#endif
#ifndef EEPROM_BOOT_SILENT
  #define EEPROM_BOOT_SILENT
#endif
#ifndef HOST_KEEPALIVE_FEATURE
  #define HOST_KEEPALIVE_FEATURE
#endif
#ifndef DEFAULT_KEEPALIVE_INTERVAL
  #define DEFAULT_KEEPALIVE_INTERVAL 2
#endif
#ifndef BUSY_WHILE_HEATING
  #define BUSY_WHILE_HEATING
#endif
#ifndef PREHEAT_1_LABEL
  #define PREHEAT_1_LABEL "PLA"
#endif
#ifndef PREHEAT_1_TEMP_HOTEND
  #define PREHEAT_1_TEMP_HOTEND 180
#endif
#ifndef PREHEAT_1_TEMP_BED
  #define PREHEAT_1_TEMP_BED 70
#endif
#ifndef PREHEAT_1_TEMP_CHAMBER
  #define PREHEAT_1_TEMP_CHAMBER 35
#endif
#ifndef PREHEAT_1_FAN_SPEED
  #define PREHEAT_1_FAN_SPEED 0
#endif
#ifndef PREHEAT_2_LABEL
  #define PREHEAT_2_LABEL "ABS"
#endif
#ifndef PREHEAT_2_TEMP_HOTEND
  #define PREHEAT_2_TEMP_HOTEND 240
#endif
#ifndef PREHEAT_2_TEMP_BED
  #define PREHEAT_2_TEMP_BED 110
#endif
#ifndef PREHEAT_2_TEMP_CHAMBER
  #define PREHEAT_2_TEMP_CHAMBER 35
#endif
#ifndef PREHEAT_2_FAN_SPEED
  #define PREHEAT_2_FAN_SPEED 0
#endif
#if ENABLED(NOZZLE_PARK_FEATURE)
  #ifndef NOZZLE_PARK_POINT
    #define NOZZLE_PARK_POINT { (X_MIN_POS + 10), (Y_MAX_POS - 10), 20 }
  #endif
  #ifndef NOZZLE_PARK_MOVE
    #define NOZZLE_PARK_MOVE 0
  #endif
  #ifndef NOZZLE_PARK_Z_RAISE_MIN
    #define NOZZLE_PARK_Z_RAISE_MIN 2
  #endif
  #ifndef NOZZLE_PARK_XY_FEEDRATE
    #define NOZZLE_PARK_XY_FEEDRATE 100
  #endif
  #ifndef NOZZLE_PARK_Z_FEEDRATE
    #define NOZZLE_PARK_Z_FEEDRATE 5
  #endif
#endif
#if ENABLED(NOZZLE_CLEAN_FEATURE)
  #ifndef NOZZLE_CLEAN_PATTERN_LINE
    #define NOZZLE_CLEAN_PATTERN_LINE
  #endif
  #ifndef NOZZLE_CLEAN_PATTERN_ZIGZAG
    #define NOZZLE_CLEAN_PATTERN_ZIGZAG
  #endif
  #ifndef NOZZLE_CLEAN_PATTERN_CIRCLE
    #define NOZZLE_CLEAN_PATTERN_CIRCLE
  #endif
  #ifndef NOZZLE_CLEAN_DEFAULT_PATTERN
    #define NOZZLE_CLEAN_DEFAULT_PATTERN 0
  #endif
  #ifndef NOZZLE_CLEAN_STROKES
    #define NOZZLE_CLEAN_STROKES 12
  #endif
  #if ENABLED(NOZZLE_CLEAN_PATTERN_ZIGZAG)
    #ifndef NOZZLE_CLEAN_TRIANGLES
      #define NOZZLE_CLEAN_TRIANGLES 3
    #endif
  #endif
  #ifndef NOZZLE_CLEAN_START_POINT
    #define NOZZLE_CLEAN_START_POINT { {  30, 30, (Z_MIN_POS + 1) } }
  #endif
  #ifndef NOZZLE_CLEAN_END_POINT
    #define NOZZLE_CLEAN_END_POINT { { 100, 60, (Z_MIN_POS + 1) } }
  #endif
  #if ENABLED(NOZZLE_CLEAN_PATTERN_CIRCLE)
    #ifndef NOZZLE_CLEAN_CIRCLE_RADIUS
      #define NOZZLE_CLEAN_CIRCLE_RADIUS 6.5
    #endif
    #ifndef NOZZLE_CLEAN_CIRCLE_FN
      #define NOZZLE_CLEAN_CIRCLE_FN 10
    #endif
    #ifndef NOZZLE_CLEAN_CIRCLE_MIDDLE
      #define NOZZLE_CLEAN_CIRCLE_MIDDLE NOZZLE_CLEAN_START_POINT
    #endif
  #endif
  #ifndef NOZZLE_CLEAN_GOBACK
    #define NOZZLE_CLEAN_GOBACK
  #endif
  #ifndef NOZZLE_CLEAN_MIN_TEMP
    #define NOZZLE_CLEAN_MIN_TEMP 170
  #endif
#endif
#ifndef PRINTJOB_TIMER_AUTOSTART
  #define PRINTJOB_TIMER_AUTOSTART
#endif
#if ENABLED(PRINTCOUNTER)
  #ifndef PRINTCOUNTER_SAVE_INTERVAL
    #define PRINTCOUNTER_SAVE_INTERVAL 60
  #endif
#endif
#if ENABLED(PASSWORD_FEATURE)
  #ifndef PASSWORD_LENGTH
    #define PASSWORD_LENGTH 4
  #endif
  #ifndef PASSWORD_ON_STARTUP
    #define PASSWORD_ON_STARTUP
  #endif
  #ifndef PASSWORD_UNLOCK_GCODE
    #define PASSWORD_UNLOCK_GCODE
  #endif
  #ifndef PASSWORD_CHANGE_GCODE
    #define PASSWORD_CHANGE_GCODE
  #endif
#endif
#ifndef LCD_LANGUAGE
  #define LCD_LANGUAGE en
#endif
#ifndef DISPLAY_CHARSET_HD44780
  #define DISPLAY_CHARSET_HD44780 JAPANESE
#endif
#ifndef LCD_INFO_SCREEN_STYLE
  #define LCD_INFO_SCREEN_STYLE 0
#endif
#if ENABLED(ENCODER_NOISE_FILTER)
  #ifndef ENCODER_SAMPLES
    #define ENCODER_SAMPLES 10
  #endif
#endif
#if ENABLED(SAV_3DGLCD)
  #ifndef U8GLIB_SSD1306
    #define U8GLIB_SSD1306
  #endif
#endif
#if DGUS_UI_IS(MKS)
  #ifndef USE_MKS_GREEN_UI
    #define USE_MKS_GREEN_UI
  #endif
#elif DGUS_UI_IS(IA_CREALITY)
#endif
#if ENABLED(TFT_GENERIC)
  #ifndef TFT_DRIVER
    #define TFT_DRIVER AUTO
  #endif
#endif
#if ENABLED(TFT_COLOR_UI)
  #ifndef TFT_FONT
    #define TFT_FONT NOTOSANS
  #endif
  #ifndef TFT_THEME
    #define TFT_THEME BLACK_MARLIN
  #endif
  #ifndef COMPACT_MARLIN_BOOT_LOGO
    #define COMPACT_MARLIN_BOOT_LOGO
  #endif
#endif
#if ENABLED(TOUCH_SCREEN)
  #ifndef BUTTON_DELAY_EDIT
    #define BUTTON_DELAY_EDIT 50
  #endif
  #ifndef BUTTON_DELAY_MENU
    #define BUTTON_DELAY_MENU 250
  #endif
  #ifndef TOUCH_SCREEN_CALIBRATION
    #define TOUCH_SCREEN_CALIBRATION
  #endif
  #if ALL(TOUCH_SCREEN_CALIBRATION, EEPROM_SETTINGS)
    #ifndef TOUCH_CALIBRATION_AUTO_SAVE
      #define TOUCH_CALIBRATION_AUTO_SAVE
    #endif
  #endif
#endif
#ifndef SOFT_PWM_SCALE
  #define SOFT_PWM_SCALE 0
#endif
#if ANY(RGB_LED, RGBW_LED, PCA9632)
  #if ENABLED(RGB_STARTUP_TEST)
    #ifndef RGB_STARTUP_TEST_INNER_MS
      #define RGB_STARTUP_TEST_INNER_MS 10
    #endif
  #endif
#endif
#if ENABLED(NEOPIXEL_LED)
  #ifndef NEOPIXEL_TYPE
    #define NEOPIXEL_TYPE NEO_GRBW
  #endif
  #ifndef NEOPIXEL_PIXELS
    #define NEOPIXEL_PIXELS 30
  #endif
  #ifndef NEOPIXEL_IS_SEQUENTIAL
    #define NEOPIXEL_IS_SEQUENTIAL
  #endif
  #ifndef NEOPIXEL_BRIGHTNESS
    #define NEOPIXEL_BRIGHTNESS 127
  #endif
  #if ENABLED(NEOPIXEL2_SEPARATE)
    #ifndef NEOPIXEL2_PIXELS
      #define NEOPIXEL2_PIXELS 15
    #endif
    #ifndef NEOPIXEL2_BRIGHTNESS
      #define NEOPIXEL2_BRIGHTNESS 127
    #endif
    #ifndef NEOPIXEL2_STARTUP_TEST
      #define NEOPIXEL2_STARTUP_TEST
    #endif
    #ifndef NEOPIXEL_M150_DEFAULT
      #define NEOPIXEL_M150_DEFAULT -1
    #endif
  #else
  #endif
#endif
#if ANY(BLINKM, RGB_LED, RGBW_LED, PCA9632, PCA9533, NEOPIXEL_LED)
  #ifndef PRINTER_EVENT_LEDS
    #define PRINTER_EVENT_LEDS
  #endif
#endif
#ifndef SERVO_DELAY
  #define SERVO_DELAY { 300 }
#endif
