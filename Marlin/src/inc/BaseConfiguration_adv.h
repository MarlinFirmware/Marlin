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
 * BaseConfiguration_adv.h - Generated using makeBaseConfigs.py
 */

#ifndef CONFIGURATION_ADV_H_VERSION
  #define CONFIGURATION_ADV_H_VERSION 02010300
#endif
#ifndef THERMOCOUPLE_MAX_ERRORS
  #define THERMOCOUPLE_MAX_ERRORS 15
#endif
#if TEMP_SENSOR_0 == 1000
  #ifndef HOTEND0_PULLUP_RESISTOR_OHMS
    #define HOTEND0_PULLUP_RESISTOR_OHMS 4700
  #endif
  #ifndef HOTEND0_RESISTANCE_25C_OHMS
    #define HOTEND0_RESISTANCE_25C_OHMS 100000
  #endif
  #ifndef HOTEND0_BETA
    #define HOTEND0_BETA 3950
  #endif
  #ifndef HOTEND0_SH_C_COEFF
    #define HOTEND0_SH_C_COEFF 0
  #endif
#endif
#if TEMP_SENSOR_1 == 1000
  #ifndef HOTEND1_PULLUP_RESISTOR_OHMS
    #define HOTEND1_PULLUP_RESISTOR_OHMS 4700
  #endif
  #ifndef HOTEND1_RESISTANCE_25C_OHMS
    #define HOTEND1_RESISTANCE_25C_OHMS 100000
  #endif
  #ifndef HOTEND1_BETA
    #define HOTEND1_BETA 3950
  #endif
  #ifndef HOTEND1_SH_C_COEFF
    #define HOTEND1_SH_C_COEFF 0
  #endif
#endif
#if TEMP_SENSOR_2 == 1000
  #ifndef HOTEND2_PULLUP_RESISTOR_OHMS
    #define HOTEND2_PULLUP_RESISTOR_OHMS 4700
  #endif
  #ifndef HOTEND2_RESISTANCE_25C_OHMS
    #define HOTEND2_RESISTANCE_25C_OHMS 100000
  #endif
  #ifndef HOTEND2_BETA
    #define HOTEND2_BETA 3950
  #endif
  #ifndef HOTEND2_SH_C_COEFF
    #define HOTEND2_SH_C_COEFF 0
  #endif
#endif
#if TEMP_SENSOR_3 == 1000
  #ifndef HOTEND3_PULLUP_RESISTOR_OHMS
    #define HOTEND3_PULLUP_RESISTOR_OHMS 4700
  #endif
  #ifndef HOTEND3_RESISTANCE_25C_OHMS
    #define HOTEND3_RESISTANCE_25C_OHMS 100000
  #endif
  #ifndef HOTEND3_BETA
    #define HOTEND3_BETA 3950
  #endif
  #ifndef HOTEND3_SH_C_COEFF
    #define HOTEND3_SH_C_COEFF 0
  #endif
#endif
#if TEMP_SENSOR_4 == 1000
  #ifndef HOTEND4_PULLUP_RESISTOR_OHMS
    #define HOTEND4_PULLUP_RESISTOR_OHMS 4700
  #endif
  #ifndef HOTEND4_RESISTANCE_25C_OHMS
    #define HOTEND4_RESISTANCE_25C_OHMS 100000
  #endif
  #ifndef HOTEND4_BETA
    #define HOTEND4_BETA 3950
  #endif
  #ifndef HOTEND4_SH_C_COEFF
    #define HOTEND4_SH_C_COEFF 0
  #endif
#endif
#if TEMP_SENSOR_5 == 1000
  #ifndef HOTEND5_PULLUP_RESISTOR_OHMS
    #define HOTEND5_PULLUP_RESISTOR_OHMS 4700
  #endif
  #ifndef HOTEND5_RESISTANCE_25C_OHMS
    #define HOTEND5_RESISTANCE_25C_OHMS 100000
  #endif
  #ifndef HOTEND5_BETA
    #define HOTEND5_BETA 3950
  #endif
  #ifndef HOTEND5_SH_C_COEFF
    #define HOTEND5_SH_C_COEFF 0
  #endif
#endif
#if TEMP_SENSOR_6 == 1000
  #ifndef HOTEND6_PULLUP_RESISTOR_OHMS
    #define HOTEND6_PULLUP_RESISTOR_OHMS 4700
  #endif
  #ifndef HOTEND6_RESISTANCE_25C_OHMS
    #define HOTEND6_RESISTANCE_25C_OHMS 100000
  #endif
  #ifndef HOTEND6_BETA
    #define HOTEND6_BETA 3950
  #endif
  #ifndef HOTEND6_SH_C_COEFF
    #define HOTEND6_SH_C_COEFF 0
  #endif
#endif
#if TEMP_SENSOR_7 == 1000
  #ifndef HOTEND7_PULLUP_RESISTOR_OHMS
    #define HOTEND7_PULLUP_RESISTOR_OHMS 4700
  #endif
  #ifndef HOTEND7_RESISTANCE_25C_OHMS
    #define HOTEND7_RESISTANCE_25C_OHMS 100000
  #endif
  #ifndef HOTEND7_BETA
    #define HOTEND7_BETA 3950
  #endif
  #ifndef HOTEND7_SH_C_COEFF
    #define HOTEND7_SH_C_COEFF 0
  #endif
#endif
#if TEMP_SENSOR_BED == 1000
  #ifndef BED_PULLUP_RESISTOR_OHMS
    #define BED_PULLUP_RESISTOR_OHMS 4700
  #endif
  #ifndef BED_RESISTANCE_25C_OHMS
    #define BED_RESISTANCE_25C_OHMS 100000
  #endif
  #ifndef BED_BETA
    #define BED_BETA 3950
  #endif
  #ifndef BED_SH_C_COEFF
    #define BED_SH_C_COEFF 0
  #endif
#endif
#if TEMP_SENSOR_CHAMBER == 1000
  #ifndef CHAMBER_PULLUP_RESISTOR_OHMS
    #define CHAMBER_PULLUP_RESISTOR_OHMS 4700
  #endif
  #ifndef CHAMBER_RESISTANCE_25C_OHMS
    #define CHAMBER_RESISTANCE_25C_OHMS 100000
  #endif
  #ifndef CHAMBER_BETA
    #define CHAMBER_BETA 3950
  #endif
  #ifndef CHAMBER_SH_C_COEFF
    #define CHAMBER_SH_C_COEFF 0
  #endif
#endif
#if TEMP_SENSOR_COOLER == 1000
  #ifndef COOLER_PULLUP_RESISTOR_OHMS
    #define COOLER_PULLUP_RESISTOR_OHMS 4700
  #endif
  #ifndef COOLER_RESISTANCE_25C_OHMS
    #define COOLER_RESISTANCE_25C_OHMS 100000
  #endif
  #ifndef COOLER_BETA
    #define COOLER_BETA 3950
  #endif
  #ifndef COOLER_SH_C_COEFF
    #define COOLER_SH_C_COEFF 0
  #endif
#endif
#if TEMP_SENSOR_PROBE == 1000
  #ifndef PROBE_PULLUP_RESISTOR_OHMS
    #define PROBE_PULLUP_RESISTOR_OHMS 4700
  #endif
  #ifndef PROBE_RESISTANCE_25C_OHMS
    #define PROBE_RESISTANCE_25C_OHMS 100000
  #endif
  #ifndef PROBE_BETA
    #define PROBE_BETA 3950
  #endif
  #ifndef PROBE_SH_C_COEFF
    #define PROBE_SH_C_COEFF 0
  #endif
#endif
#if TEMP_SENSOR_BOARD == 1000
  #ifndef BOARD_PULLUP_RESISTOR_OHMS
    #define BOARD_PULLUP_RESISTOR_OHMS 4700
  #endif
  #ifndef BOARD_RESISTANCE_25C_OHMS
    #define BOARD_RESISTANCE_25C_OHMS 100000
  #endif
  #ifndef BOARD_BETA
    #define BOARD_BETA 3950
  #endif
  #ifndef BOARD_SH_C_COEFF
    #define BOARD_SH_C_COEFF 0
  #endif
#endif
#if TEMP_SENSOR_REDUNDANT == 1000
  #ifndef REDUNDANT_PULLUP_RESISTOR_OHMS
    #define REDUNDANT_PULLUP_RESISTOR_OHMS 4700
  #endif
  #ifndef REDUNDANT_RESISTANCE_25C_OHMS
    #define REDUNDANT_RESISTANCE_25C_OHMS 100000
  #endif
  #ifndef REDUNDANT_BETA
    #define REDUNDANT_BETA 3950
  #endif
  #ifndef REDUNDANT_SH_C_COEFF
    #define REDUNDANT_SH_C_COEFF 0
  #endif
#endif
#if ENABLED(HEPHESTOS2_HEATED_BED_KIT)
  #ifndef HEATER_BED_INVERTING
    #define HEATER_BED_INVERTING true
  #endif
#endif
#if DISABLED(PIDTEMPBED)
  #ifndef BED_CHECK_INTERVAL
    #define BED_CHECK_INTERVAL 5000
  #endif
  #if ENABLED(BED_LIMIT_SWITCHING)
    #ifndef BED_HYSTERESIS
      #define BED_HYSTERESIS 2
    #endif
  #endif
#endif
#if DISABLED(PIDTEMPCHAMBER)
  #ifndef CHAMBER_CHECK_INTERVAL
    #define CHAMBER_CHECK_INTERVAL 5000
  #endif
  #if ENABLED(CHAMBER_LIMIT_SWITCHING)
    #ifndef CHAMBER_HYSTERESIS
      #define CHAMBER_HYSTERESIS 2
    #endif
  #endif
#endif
#if TEMP_SENSOR_CHAMBER
  #if ENABLED(CHAMBER_FAN)
    #ifndef CHAMBER_FAN_MODE
      #define CHAMBER_FAN_MODE 2
    #endif
    #if CHAMBER_FAN_MODE == 0
      #ifndef CHAMBER_FAN_BASE
        #define CHAMBER_FAN_BASE 255
      #endif
    #elif CHAMBER_FAN_MODE == 1
      #ifndef CHAMBER_FAN_BASE
        #define CHAMBER_FAN_BASE 128
      #endif
      #ifndef CHAMBER_FAN_FACTOR
        #define CHAMBER_FAN_FACTOR 25
      #endif
    #elif CHAMBER_FAN_MODE == 2
      #ifndef CHAMBER_FAN_BASE
        #define CHAMBER_FAN_BASE 128
      #endif
      #ifndef CHAMBER_FAN_FACTOR
        #define CHAMBER_FAN_FACTOR 25
      #endif
    #elif CHAMBER_FAN_MODE == 3
      #ifndef CHAMBER_FAN_BASE
        #define CHAMBER_FAN_BASE 128
      #endif
      #ifndef CHAMBER_FAN_FACTOR
        #define CHAMBER_FAN_FACTOR 25
      #endif
    #endif
  #endif
  #if ENABLED(CHAMBER_VENT)
    #ifndef CHAMBER_VENT_SERVO_NR
      #define CHAMBER_VENT_SERVO_NR 1
    #endif
    #ifndef HIGH_EXCESS_HEAT_LIMIT
      #define HIGH_EXCESS_HEAT_LIMIT 5
    #endif
    #ifndef LOW_EXCESS_HEAT_LIMIT
      #define LOW_EXCESS_HEAT_LIMIT 3
    #endif
    #ifndef MIN_COOLING_SLOPE_TIME_CHAMBER_VENT
      #define MIN_COOLING_SLOPE_TIME_CHAMBER_VENT 20
    #endif
    #ifndef MIN_COOLING_SLOPE_DEG_CHAMBER_VENT
      #define MIN_COOLING_SLOPE_DEG_CHAMBER_VENT 1.5
    #endif
  #endif
#endif
#if TEMP_SENSOR_COOLER
  #ifndef COOLER_MINTEMP
    #define COOLER_MINTEMP 8
  #endif
  #ifndef COOLER_MAXTEMP
    #define COOLER_MAXTEMP 26
  #endif
  #ifndef COOLER_DEFAULT_TEMP
    #define COOLER_DEFAULT_TEMP 16
  #endif
  #ifndef TEMP_COOLER_HYSTERESIS
    #define TEMP_COOLER_HYSTERESIS 1
  #endif
  #ifndef COOLER_PIN
    #define COOLER_PIN 8
  #endif
  #ifndef COOLER_INVERTING
    #define COOLER_INVERTING false
  #endif
  #ifndef TEMP_COOLER_PIN
    #define TEMP_COOLER_PIN 15
  #endif
  #ifndef COOLER_FAN
    #define COOLER_FAN
  #endif
  #ifndef COOLER_FAN_INDEX
    #define COOLER_FAN_INDEX 0
  #endif
  #if ENABLED(COOLER_FAN)
    #ifndef COOLER_FAN_BASE
      #define COOLER_FAN_BASE 100
    #endif
    #ifndef COOLER_FAN_FACTOR
      #define COOLER_FAN_FACTOR 25
    #endif
  #endif
#endif
#if TEMP_SENSOR_BOARD
  #ifndef THERMAL_PROTECTION_BOARD
    #define THERMAL_PROTECTION_BOARD
  #endif
  #ifndef BOARD_MINTEMP
    #define BOARD_MINTEMP 8
  #endif
  #ifndef BOARD_MAXTEMP
    #define BOARD_MAXTEMP 70
  #endif
#endif
#if TEMP_SENSOR_SOC
  #ifndef THERMAL_PROTECTION_SOC
    #define THERMAL_PROTECTION_SOC
  #endif
  #ifndef SOC_MAXTEMP
    #define SOC_MAXTEMP 85
  #endif
#endif
#if ALL(HAS_HOTEND, THERMAL_PROTECTION_HOTENDS)
  #ifndef THERMAL_PROTECTION_PERIOD
    #define THERMAL_PROTECTION_PERIOD 40
  #endif
  #ifndef THERMAL_PROTECTION_HYSTERESIS
    #define THERMAL_PROTECTION_HYSTERESIS 4
  #endif
  #ifndef WATCH_TEMP_PERIOD
    #define WATCH_TEMP_PERIOD 40
  #endif
  #ifndef WATCH_TEMP_INCREASE
    #define WATCH_TEMP_INCREASE 2
  #endif
#endif
#if TEMP_SENSOR_BED && ENABLED(THERMAL_PROTECTION_BED)
  #ifndef THERMAL_PROTECTION_BED_PERIOD
    #define THERMAL_PROTECTION_BED_PERIOD 20
  #endif
  #ifndef THERMAL_PROTECTION_BED_HYSTERESIS
    #define THERMAL_PROTECTION_BED_HYSTERESIS 2
  #endif
  #ifndef WATCH_BED_TEMP_PERIOD
    #define WATCH_BED_TEMP_PERIOD 60
  #endif
  #ifndef WATCH_BED_TEMP_INCREASE
    #define WATCH_BED_TEMP_INCREASE 2
  #endif
#endif
#if TEMP_SENSOR_CHAMBER && ENABLED(THERMAL_PROTECTION_CHAMBER)
  #ifndef THERMAL_PROTECTION_CHAMBER_PERIOD
    #define THERMAL_PROTECTION_CHAMBER_PERIOD 20
  #endif
  #ifndef THERMAL_PROTECTION_CHAMBER_HYSTERESIS
    #define THERMAL_PROTECTION_CHAMBER_HYSTERESIS 2
  #endif
  #ifndef WATCH_CHAMBER_TEMP_PERIOD
    #define WATCH_CHAMBER_TEMP_PERIOD 60
  #endif
  #ifndef WATCH_CHAMBER_TEMP_INCREASE
    #define WATCH_CHAMBER_TEMP_INCREASE 2
  #endif
#endif
#if TEMP_SENSOR_COOLER && ENABLED(THERMAL_PROTECTION_COOLER)
  #ifndef THERMAL_PROTECTION_COOLER_PERIOD
    #define THERMAL_PROTECTION_COOLER_PERIOD 10
  #endif
  #ifndef THERMAL_PROTECTION_COOLER_HYSTERESIS
    #define THERMAL_PROTECTION_COOLER_HYSTERESIS 3
  #endif
  #ifndef WATCH_COOLER_TEMP_PERIOD
    #define WATCH_COOLER_TEMP_PERIOD 60
  #endif
  #ifndef WATCH_COOLER_TEMP_INCREASE
    #define WATCH_COOLER_TEMP_INCREASE 3
  #endif
#endif
#if ENABLED(PIDTEMP)
  #if ENABLED(PID_EXTRUSION_SCALING)
    #ifndef DEFAULT_Kc
      #define DEFAULT_Kc (100)
    #endif
    #ifndef LPQ_MAX_LEN
      #define LPQ_MAX_LEN 50
    #endif
  #endif
  #if ENABLED(PID_FAN_SCALING)
    #if ENABLED(PID_FAN_SCALING_ALTERNATIVE_DEFINITION)
      #ifndef PID_FAN_SCALING_AT_FULL_SPEED
        #define PID_FAN_SCALING_AT_FULL_SPEED 13.0
      #endif
      #ifndef PID_FAN_SCALING_AT_MIN_SPEED
        #define PID_FAN_SCALING_AT_MIN_SPEED 6.0
      #endif
      #ifndef PID_FAN_SCALING_MIN_SPEED
        #define PID_FAN_SCALING_MIN_SPEED 10.0
      #endif
      #ifndef DEFAULT_Kf
        #define DEFAULT_Kf (255.0*PID_FAN_SCALING_AT_MIN_SPEED-PID_FAN_SCALING_AT_FULL_SPEED*PID_FAN_SCALING_MIN_SPEED)/(255.0-PID_FAN_SCALING_MIN_SPEED)
      #endif
      #ifndef PID_FAN_SCALING_LIN_FACTOR
        #define PID_FAN_SCALING_LIN_FACTOR (PID_FAN_SCALING_AT_FULL_SPEED-DEFAULT_Kf)/255.0
      #endif
    #else
      #ifndef PID_FAN_SCALING_LIN_FACTOR
        #define PID_FAN_SCALING_LIN_FACTOR (0)
      #endif
      #ifndef DEFAULT_Kf
        #define DEFAULT_Kf 10
      #endif
      #ifndef PID_FAN_SCALING_MIN_SPEED
        #define PID_FAN_SCALING_MIN_SPEED 10
      #endif
    #endif
  #endif
#endif
#ifndef AUTOTEMP
  #define AUTOTEMP
#endif
#if ENABLED(AUTOTEMP)
  #ifndef AUTOTEMP_OLDWEIGHT
    #define AUTOTEMP_OLDWEIGHT 0.98
  #endif
  #ifndef AUTOTEMP_MIN
    #define AUTOTEMP_MIN 210
  #endif
  #ifndef AUTOTEMP_MAX
    #define AUTOTEMP_MAX 250
  #endif
  #ifndef AUTOTEMP_FACTOR
    #define AUTOTEMP_FACTOR 0.1f
  #endif
  #if ENABLED(AUTOTEMP_PROPORTIONAL)
    #ifndef AUTOTEMP_MIN_P
      #define AUTOTEMP_MIN_P 0
    #endif
    #ifndef AUTOTEMP_MAX_P
      #define AUTOTEMP_MAX_P 5
    #endif
    #ifndef AUTOTEMP_FACTOR_P
      #define AUTOTEMP_FACTOR_P 1
    #endif
  #endif
#endif
#if ENABLED(EXTRUDER_RUNOUT_PREVENT)
  #ifndef EXTRUDER_RUNOUT_MINTEMP
    #define EXTRUDER_RUNOUT_MINTEMP 190
  #endif
  #ifndef EXTRUDER_RUNOUT_SECONDS
    #define EXTRUDER_RUNOUT_SECONDS 30
  #endif
  #ifndef EXTRUDER_RUNOUT_SPEED
    #define EXTRUDER_RUNOUT_SPEED 1500
  #endif
  #ifndef EXTRUDER_RUNOUT_EXTRUDE
    #define EXTRUDER_RUNOUT_EXTRUDE 5
  #endif
#endif
#if ENABLED(HOTEND_IDLE_TIMEOUT)
  #ifndef HOTEND_IDLE_TIMEOUT_SEC
    #define HOTEND_IDLE_TIMEOUT_SEC (5*60)
  #endif
  #ifndef HOTEND_IDLE_MIN_TRIGGER
    #define HOTEND_IDLE_MIN_TRIGGER 180
  #endif
  #ifndef HOTEND_IDLE_NOZZLE_TARGET
    #define HOTEND_IDLE_NOZZLE_TARGET 0
  #endif
  #ifndef HOTEND_IDLE_BED_TARGET
    #define HOTEND_IDLE_BED_TARGET 0
  #endif
#endif
#ifndef TEMP_SENSOR_AD595_OFFSET
  #define TEMP_SENSOR_AD595_OFFSET 0.0
#endif
#ifndef TEMP_SENSOR_AD595_GAIN
  #define TEMP_SENSOR_AD595_GAIN 1.0
#endif
#ifndef TEMP_SENSOR_AD8495_OFFSET
  #define TEMP_SENSOR_AD8495_OFFSET 0.0
#endif
#ifndef TEMP_SENSOR_AD8495_GAIN
  #define TEMP_SENSOR_AD8495_GAIN 1.0
#endif
#if ENABLED(USE_CONTROLLER_FAN)
  #ifndef CONTROLLERFAN_SPEED_MIN
    #define CONTROLLERFAN_SPEED_MIN 0
  #endif
  #ifndef CONTROLLERFAN_SPEED_ACTIVE
    #define CONTROLLERFAN_SPEED_ACTIVE 255
  #endif
  #ifndef CONTROLLERFAN_SPEED_IDLE
    #define CONTROLLERFAN_SPEED_IDLE 0
  #endif
  #ifndef CONTROLLERFAN_IDLE_TIME
    #define CONTROLLERFAN_IDLE_TIME 60
  #endif
  #if ENABLED(CONTROLLER_FAN_EDITABLE)
    #ifndef CONTROLLER_FAN_MENU
      #define CONTROLLER_FAN_MENU
    #endif
  #endif
#endif
#if ENABLED(FAST_PWM_FAN)
  #ifndef FAST_PWM_FAN_FREQUENCY
    #ifdef __AVR__
      #ifndef FAST_PWM_FAN_FREQUENCY
        #define FAST_PWM_FAN_FREQUENCY ((F_CPU) / (2 * 255 * 1))
      #endif
    #else
      #ifndef FAST_PWM_FAN_FREQUENCY
        #define FAST_PWM_FAN_FREQUENCY 1000U
      #endif
    #endif
  #endif
#endif
#ifndef E0_AUTO_FAN_PIN
  #define E0_AUTO_FAN_PIN -1
#endif
#ifndef E1_AUTO_FAN_PIN
  #define E1_AUTO_FAN_PIN -1
#endif
#ifndef E2_AUTO_FAN_PIN
  #define E2_AUTO_FAN_PIN -1
#endif
#ifndef E3_AUTO_FAN_PIN
  #define E3_AUTO_FAN_PIN -1
#endif
#ifndef E4_AUTO_FAN_PIN
  #define E4_AUTO_FAN_PIN -1
#endif
#ifndef E5_AUTO_FAN_PIN
  #define E5_AUTO_FAN_PIN -1
#endif
#ifndef E6_AUTO_FAN_PIN
  #define E6_AUTO_FAN_PIN -1
#endif
#ifndef E7_AUTO_FAN_PIN
  #define E7_AUTO_FAN_PIN -1
#endif
#ifndef CHAMBER_AUTO_FAN_PIN
  #define CHAMBER_AUTO_FAN_PIN -1
#endif
#ifndef COOLER_AUTO_FAN_PIN
  #define COOLER_AUTO_FAN_PIN -1
#endif
#ifndef EXTRUDER_AUTO_FAN_TEMPERATURE
  #define EXTRUDER_AUTO_FAN_TEMPERATURE 50
#endif
#ifndef EXTRUDER_AUTO_FAN_SPEED
  #define EXTRUDER_AUTO_FAN_SPEED 255
#endif
#ifndef CHAMBER_AUTO_FAN_TEMPERATURE
  #define CHAMBER_AUTO_FAN_TEMPERATURE 30
#endif
#ifndef CHAMBER_AUTO_FAN_SPEED
  #define CHAMBER_AUTO_FAN_SPEED 255
#endif
#ifndef COOLER_AUTO_FAN_TEMPERATURE
  #define COOLER_AUTO_FAN_TEMPERATURE 18
#endif
#ifndef COOLER_AUTO_FAN_SPEED
  #define COOLER_AUTO_FAN_SPEED 255
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
#if ENABLED(CASE_LIGHT_ENABLE)
  #ifndef INVERT_CASE_LIGHT
    #define INVERT_CASE_LIGHT false
  #endif
  #ifndef CASE_LIGHT_DEFAULT_ON
    #define CASE_LIGHT_DEFAULT_ON true
  #endif
  #ifndef CASE_LIGHT_DEFAULT_BRIGHTNESS
    #define CASE_LIGHT_DEFAULT_BRIGHTNESS 105
  #endif
  #if ANY(CASE_LIGHT_USE_NEOPIXEL, CASE_LIGHT_USE_RGB_LED)
    #ifndef CASE_LIGHT_DEFAULT_COLOR
      #define CASE_LIGHT_DEFAULT_COLOR { 255, 255, 255, 255 }
    #endif
  #endif
#endif
#if ENABLED(DUAL_X_CARRIAGE)
  #ifndef X1_MIN_POS
    #define X1_MIN_POS X_MIN_POS
  #endif
  #ifndef X1_MAX_POS
    #define X1_MAX_POS X_BED_SIZE
  #endif
  #ifndef X2_MIN_POS
    #define X2_MIN_POS 80
  #endif
  #ifndef X2_MAX_POS
    #define X2_MAX_POS 353
  #endif
  #ifndef X2_HOME_POS
    #define X2_HOME_POS X2_MAX_POS
  #endif
  #ifndef DEFAULT_DUAL_X_CARRIAGE_MODE
    #define DEFAULT_DUAL_X_CARRIAGE_MODE DXC_AUTO_PARK_MODE
  #endif
  #ifndef DEFAULT_DUPLICATION_X_OFFSET
    #define DEFAULT_DUPLICATION_X_OFFSET 100
  #endif
#endif
#if HAS_X2_STEPPER && DISABLED(DUAL_X_CARRIAGE)
  #if ENABLED(X_DUAL_ENDSTOPS)
    #ifndef X2_ENDSTOP_ADJUSTMENT
      #define X2_ENDSTOP_ADJUSTMENT 0
    #endif
  #endif
#endif
#if HAS_Y2_STEPPER
  #if ENABLED(Y_DUAL_ENDSTOPS)
    #ifndef Y2_ENDSTOP_ADJUSTMENT
      #define Y2_ENDSTOP_ADJUSTMENT 0
    #endif
  #endif
#endif
#ifdef Z2_DRIVER_TYPE
  #if ENABLED(Z_MULTI_ENDSTOPS)
    #ifndef Z2_ENDSTOP_ADJUSTMENT
      #define Z2_ENDSTOP_ADJUSTMENT 0
    #endif
  #endif
  #ifdef Z3_DRIVER_TYPE
    #if ENABLED(Z_MULTI_ENDSTOPS)
      #ifndef Z3_ENDSTOP_ADJUSTMENT
        #define Z3_ENDSTOP_ADJUSTMENT 0
      #endif
    #endif
  #endif
  #ifdef Z4_DRIVER_TYPE
    #if ENABLED(Z_MULTI_ENDSTOPS)
      #ifndef Z4_ENDSTOP_ADJUSTMENT
        #define Z4_ENDSTOP_ADJUSTMENT 0
      #endif
    #endif
  #endif
#endif
#ifndef HOMING_BUMP_MM
  #define HOMING_BUMP_MM { 5, 5, 2 }
#endif
#ifndef HOMING_BUMP_DIVISOR
  #define HOMING_BUMP_DIVISOR { 2, 2, 4 }
#endif
#if ENABLED(BLTOUCH)
  #ifdef BLTOUCH_HS_MODE
    #ifndef BLTOUCH_HS_EXTRA_CLEARANCE
      #define BLTOUCH_HS_EXTRA_CLEARANCE 7
    #endif
  #endif
#endif
#if ENABLED(Z_STEPPER_AUTO_ALIGN)
  #ifndef Z_STEPPER_ALIGN_STEPPER_XY
    #ifndef Z_STEPPER_ALIGN_AMP
      #define Z_STEPPER_ALIGN_AMP 1.0
    #endif
  #endif
  #ifndef G34_MAX_GRADE
    #define G34_MAX_GRADE 5
  #endif
  #ifndef Z_STEPPER_ALIGN_ITERATIONS
    #define Z_STEPPER_ALIGN_ITERATIONS 5
  #endif
  #ifndef Z_STEPPER_ALIGN_ACC
    #define Z_STEPPER_ALIGN_ACC 0.02
  #endif
  #ifndef RESTORE_LEVELING_AFTER_G34
    #define RESTORE_LEVELING_AFTER_G34
  #endif
  #ifndef HOME_AFTER_G34
    #define HOME_AFTER_G34
  #endif
#endif
#if ENABLED(ASSISTED_TRAMMING)
  #ifndef TRAMMING_POINT_XY
    #define TRAMMING_POINT_XY { {  20, 20 }, { 180,  20 }, { 180, 180 }, { 20, 180 } }
  #endif
  #ifndef TRAMMING_POINT_NAME_1
    #define TRAMMING_POINT_NAME_1 "Front-Left"
  #endif
  #ifndef TRAMMING_POINT_NAME_2
    #define TRAMMING_POINT_NAME_2 "Front-Right"
  #endif
  #ifndef TRAMMING_POINT_NAME_3
    #define TRAMMING_POINT_NAME_3 "Back-Right"
  #endif
  #ifndef TRAMMING_POINT_NAME_4
    #define TRAMMING_POINT_NAME_4 "Back-Left"
  #endif
  #ifndef RESTORE_LEVELING_AFTER_G35
    #define RESTORE_LEVELING_AFTER_G35
  #endif
  #ifndef TRAMMING_SCREW_THREAD
    #define TRAMMING_SCREW_THREAD 30
  #endif
#endif
#if ENABLED(FT_MOTION)
  #ifndef FTM_DEFAULT_DYNFREQ_MODE
    #define FTM_DEFAULT_DYNFREQ_MODE dynFreqMode_DISABLED
  #endif
  #ifndef FTM_DEFAULT_SHAPER_X
    #define FTM_DEFAULT_SHAPER_X ftMotionShaper_NONE
  #endif
  #ifndef FTM_DEFAULT_SHAPER_Y
    #define FTM_DEFAULT_SHAPER_Y ftMotionShaper_NONE
  #endif
  #ifndef FTM_SHAPING_DEFAULT_X_FREQ
    #define FTM_SHAPING_DEFAULT_X_FREQ 37.0f
  #endif
  #ifndef FTM_SHAPING_DEFAULT_Y_FREQ
    #define FTM_SHAPING_DEFAULT_Y_FREQ 37.0f
  #endif
  #ifndef FTM_LINEAR_ADV_DEFAULT_ENA
    #define FTM_LINEAR_ADV_DEFAULT_ENA false
  #endif
  #ifndef FTM_LINEAR_ADV_DEFAULT_K
    #define FTM_LINEAR_ADV_DEFAULT_K 0
  #endif
  #ifndef FTM_SHAPING_ZETA_X
    #define FTM_SHAPING_ZETA_X 0.1f
  #endif
  #ifndef FTM_SHAPING_ZETA_Y
    #define FTM_SHAPING_ZETA_Y 0.1f
  #endif
  #ifndef FTM_SHAPING_V_TOL_X
    #define FTM_SHAPING_V_TOL_X 0.05f
  #endif
  #ifndef FTM_SHAPING_V_TOL_Y
    #define FTM_SHAPING_V_TOL_Y 0.05f
  #endif
  #ifndef FTM_UNIFIED_BWS
    #define FTM_UNIFIED_BWS
  #endif
  #if ENABLED(FTM_UNIFIED_BWS)
    #ifndef FTM_BW_SIZE
      #define FTM_BW_SIZE 100
    #endif
  #else
    #ifndef FTM_WINDOW_SIZE
      #define FTM_WINDOW_SIZE 200
    #endif
    #ifndef FTM_BATCH_SIZE
      #define FTM_BATCH_SIZE 100
    #endif
  #endif
  #ifndef FTM_FS
    #define FTM_FS 1000
  #endif
  #ifndef FTM_TS
    #define FTM_TS 0.001f
  #endif
  #if DISABLED(COREXY)
    #ifndef FTM_STEPPER_FS
      #define FTM_STEPPER_FS 20000
    #endif
    #ifndef FTM_STEPPERCMD_BUFF_SIZE
      #define FTM_STEPPERCMD_BUFF_SIZE 3000
    #endif
  #else
    #ifndef FTM_STEPPER_FS
      #define FTM_STEPPER_FS 30000
    #endif
    #ifndef FTM_STEPPERCMD_BUFF_SIZE
      #define FTM_STEPPERCMD_BUFF_SIZE 6000
    #endif
  #endif
  #ifndef FTM_STEPS_PER_UNIT_TIME
    #define FTM_STEPS_PER_UNIT_TIME (FTM_STEPPER_FS / FTM_FS)
  #endif
  #ifndef FTM_CTS_COMPARE_VAL
    #define FTM_CTS_COMPARE_VAL (FTM_STEPS_PER_UNIT_TIME / 2)
  #endif
  #ifndef FTM_MIN_TICKS
    #define FTM_MIN_TICKS ((STEPPER_TIMER_RATE) / (FTM_STEPPER_FS))
  #endif
  #ifndef FTM_MIN_SHAPE_FREQ
    #define FTM_MIN_SHAPE_FREQ 10
  #endif
  #ifndef FTM_RATIO
    #define FTM_RATIO (FTM_FS / FTM_MIN_SHAPE_FREQ)
  #endif
  #ifndef FTM_ZMAX
    #define FTM_ZMAX (FTM_RATIO * 2)
  #endif
#endif
#if ANY(INPUT_SHAPING_X, INPUT_SHAPING_Y, INPUT_SHAPING_Z)
  #if ENABLED(INPUT_SHAPING_X)
    #ifndef SHAPING_FREQ_X
      #define SHAPING_FREQ_X 40.0
    #endif
    #ifndef SHAPING_ZETA_X
      #define SHAPING_ZETA_X 0.15
    #endif
  #endif
  #if ENABLED(INPUT_SHAPING_Y)
    #ifndef SHAPING_FREQ_Y
      #define SHAPING_FREQ_Y 40.0
    #endif
    #ifndef SHAPING_ZETA_Y
      #define SHAPING_ZETA_Y 0.15
    #endif
  #endif
  #if ENABLED(INPUT_SHAPING_Z)
    #ifndef SHAPING_FREQ_Z
      #define SHAPING_FREQ_Z 40.0
    #endif
    #ifndef SHAPING_ZETA_Z
      #define SHAPING_ZETA_Z 0.15
    #endif
  #endif
#endif
#ifndef AXIS_RELATIVE_MODES
  #define AXIS_RELATIVE_MODES { false, false, false, false }
#endif
#ifndef STEP_STATE_X
  #define STEP_STATE_X HIGH
#endif
#ifndef STEP_STATE_Y
  #define STEP_STATE_Y HIGH
#endif
#ifndef STEP_STATE_Z
  #define STEP_STATE_Z HIGH
#endif
#ifndef STEP_STATE_I
  #define STEP_STATE_I HIGH
#endif
#ifndef STEP_STATE_J
  #define STEP_STATE_J HIGH
#endif
#ifndef STEP_STATE_K
  #define STEP_STATE_K HIGH
#endif
#ifndef STEP_STATE_U
  #define STEP_STATE_U HIGH
#endif
#ifndef STEP_STATE_V
  #define STEP_STATE_V HIGH
#endif
#ifndef STEP_STATE_W
  #define STEP_STATE_W HIGH
#endif
#ifndef STEP_STATE_E
  #define STEP_STATE_E HIGH
#endif
#ifndef DEFAULT_STEPPER_TIMEOUT_SEC
  #define DEFAULT_STEPPER_TIMEOUT_SEC 120
#endif
#ifndef DISABLE_IDLE_X
  #define DISABLE_IDLE_X
#endif
#ifndef DISABLE_IDLE_Y
  #define DISABLE_IDLE_Y
#endif
#ifndef DISABLE_IDLE_Z
  #define DISABLE_IDLE_Z
#endif
#ifndef DISABLE_IDLE_E
  #define DISABLE_IDLE_E
#endif
#ifndef DEFAULT_MINIMUMFEEDRATE
  #define DEFAULT_MINIMUMFEEDRATE 0.0
#endif
#ifndef DEFAULT_MINTRAVELFEEDRATE
  #define DEFAULT_MINTRAVELFEEDRATE 0.0
#endif
#ifndef DEFAULT_MINSEGMENTTIME
  #define DEFAULT_MINSEGMENTTIME 20000
#endif
#ifndef SLOWDOWN
  #define SLOWDOWN
#endif
#if ENABLED(SLOWDOWN)
  #ifndef SLOWDOWN_DIVISOR
    #define SLOWDOWN_DIVISOR 2
  #endif
#endif
#ifdef XY_FREQUENCY_LIMIT
  #ifndef XY_FREQUENCY_MIN_PERCENT
    #define XY_FREQUENCY_MIN_PERCENT 5
  #endif
#endif
#if ENABLED(BACKLASH_COMPENSATION)
  #ifndef BACKLASH_DISTANCE_MM
    #define BACKLASH_DISTANCE_MM { 0, 0, 0 }
  #endif
  #ifndef BACKLASH_CORRECTION
    #define BACKLASH_CORRECTION 0.0
  #endif
  #if ENABLED(BACKLASH_GCODE)
    #ifndef MEASURE_BACKLASH_WHEN_PROBING
      #define MEASURE_BACKLASH_WHEN_PROBING
    #endif
    #if ENABLED(MEASURE_BACKLASH_WHEN_PROBING)
      #ifndef BACKLASH_MEASUREMENT_LIMIT
        #define BACKLASH_MEASUREMENT_LIMIT 0.5
      #endif
      #ifndef BACKLASH_MEASUREMENT_RESOLUTION
        #define BACKLASH_MEASUREMENT_RESOLUTION 0.005
      #endif
      #ifndef BACKLASH_MEASUREMENT_FEEDRATE
        #define BACKLASH_MEASUREMENT_FEEDRATE Z_PROBE_FEEDRATE_SLOW
      #endif
    #endif
  #endif
#endif
#if ENABLED(CALIBRATION_GCODE)
  #ifndef CALIBRATION_FEEDRATE_SLOW
    #define CALIBRATION_FEEDRATE_SLOW 60
  #endif
  #ifndef CALIBRATION_FEEDRATE_FAST
    #define CALIBRATION_FEEDRATE_FAST 1200
  #endif
  #ifndef CALIBRATION_FEEDRATE_TRAVEL
    #define CALIBRATION_FEEDRATE_TRAVEL 3000
  #endif
  #ifndef CALIBRATION_NOZZLE_TIP_HEIGHT
    #define CALIBRATION_NOZZLE_TIP_HEIGHT 1.0
  #endif
  #ifndef CALIBRATION_NOZZLE_OUTER_DIAMETER
    #define CALIBRATION_NOZZLE_OUTER_DIAMETER 2.0
  #endif
  #ifndef CALIBRATION_OBJECT_CENTER
    #define CALIBRATION_OBJECT_CENTER { 264.0, -22.0,  -2.0 }
  #endif
  #ifndef CALIBRATION_OBJECT_DIMENSIONS
    #define CALIBRATION_OBJECT_DIMENSIONS {  10.0,  10.0,  10.0 }
  #endif
  #ifndef CALIBRATION_MEASURE_RIGHT
    #define CALIBRATION_MEASURE_RIGHT
  #endif
  #ifndef CALIBRATION_MEASURE_FRONT
    #define CALIBRATION_MEASURE_FRONT
  #endif
  #ifndef CALIBRATION_MEASURE_LEFT
    #define CALIBRATION_MEASURE_LEFT
  #endif
  #ifndef CALIBRATION_MEASURE_BACK
    #define CALIBRATION_MEASURE_BACK
  #endif
  #ifndef CALIBRATION_PIN
    #ifndef CALIBRATION_PIN_INVERTING
      #define CALIBRATION_PIN_INVERTING false
    #endif
    #ifndef CALIBRATION_PIN_PULLUP
      #define CALIBRATION_PIN_PULLUP
    #endif
  #endif
#endif
#ifndef MULTISTEPPING_LIMIT
  #define MULTISTEPPING_LIMIT 16
#endif
#ifndef MICROSTEP_MODES
  #define MICROSTEP_MODES { 16, 16, 16, 16, 16, 16 }
#endif
#if ANY(DIGIPOT_MCP4018, DIGIPOT_MCP4451)
  #ifndef DIGIPOT_I2C_NUM_CHANNELS
    #define DIGIPOT_I2C_NUM_CHANNELS 8
  #endif
  #ifndef DIGIPOT_I2C_MOTOR_CURRENTS
    #define DIGIPOT_I2C_MOTOR_CURRENTS { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 }
  #endif
#endif
#if HAS_MANUAL_MOVE_MENU
  #ifndef MANUAL_FEEDRATE
    #define MANUAL_FEEDRATE { 50*60, 50*60, 4*60, 2*60 }
  #endif
  #ifndef FINE_MANUAL_MOVE
    #define FINE_MANUAL_MOVE 0.025
  #endif
  #if IS_ULTIPANEL
    #ifndef MANUAL_E_MOVES_RELATIVE
      #define MANUAL_E_MOVES_RELATIVE
    #endif
    #ifndef ULTIPANEL_FEEDMULTIPLY
      #define ULTIPANEL_FEEDMULTIPLY
    #endif
  #endif
#endif
#ifndef ENCODER_RATE_MULTIPLIER
  #define ENCODER_RATE_MULTIPLIER
#endif
#if ENABLED(ENCODER_RATE_MULTIPLIER)
  #ifndef ENCODER_10X_STEPS_PER_SEC
    #define ENCODER_10X_STEPS_PER_SEC 30
  #endif
  #ifndef ENCODER_100X_STEPS_PER_SEC
    #define ENCODER_100X_STEPS_PER_SEC 80
  #endif
#endif
#if ENABLED(BEEP_ON_FEEDRATE_CHANGE)
  #ifndef FEEDRATE_CHANGE_BEEP_DURATION
    #define FEEDRATE_CHANGE_BEEP_DURATION 10
  #endif
  #ifndef FEEDRATE_CHANGE_BEEP_FREQUENCY
    #define FEEDRATE_CHANGE_BEEP_FREQUENCY 440
  #endif
#endif
#if HAS_MARLINUI_MENU
  #if HAS_BED_PROBE
    #if ENABLED(X_AXIS_TWIST_COMPENSATION)
      #ifndef XATC_START_Z
        #define XATC_START_Z 0.0
      #endif
      #ifndef XATC_MAX_POINTS
        #define XATC_MAX_POINTS 3
      #endif
      #ifndef XATC_Y_POSITION
        #define XATC_Y_POSITION Y_CENTER
      #endif
      #ifndef XATC_Z_OFFSETS
        #define XATC_Z_OFFSETS { 0, 0, 0 }
      #endif
    #endif
    #ifndef PROBE_DEPLOY_STOW_MENU
      #define PROBE_DEPLOY_STOW_MENU
    #endif
  #endif
  #ifndef MANUAL_MOVE_DISTANCE_MM
    #define MANUAL_MOVE_DISTANCE_MM 10, 1.0, 0.1
  #endif
  #ifndef MANUAL_MOVE_DISTANCE_IN
    #define MANUAL_MOVE_DISTANCE_IN 0.100, 0.010, 0.001
  #endif
  #ifndef MANUAL_MOVE_DISTANCE_DEG
    #define MANUAL_MOVE_DISTANCE_DEG 90, 45, 22.5, 5, 1
  #endif
#endif
#if HAS_DISPLAY
  #ifndef SHOW_BOOTSCREEN
    #define SHOW_BOOTSCREEN
  #endif
  #if ENABLED(SHOW_BOOTSCREEN)
    #ifndef BOOTSCREEN_TIMEOUT
      #define BOOTSCREEN_TIMEOUT 3000
    #endif
    #if ANY(HAS_MARLINUI_U8GLIB, TFT_COLOR_UI)
      #ifndef BOOT_MARLIN_LOGO_SMALL
        #define BOOT_MARLIN_LOGO_SMALL
      #endif
    #endif
  #endif
  #ifndef SOUND_ON_DEFAULT
    #define SOUND_ON_DEFAULT
  #endif
  #if ENABLED(LED_CONTROL_MENU)
    #ifndef LED_COLOR_PRESETS
      #define LED_COLOR_PRESETS
    #endif
    #if ENABLED(LED_COLOR_PRESETS)
      #ifndef LED_USER_PRESET_RED
        #define LED_USER_PRESET_RED 255
      #endif
      #ifndef LED_USER_PRESET_GREEN
        #define LED_USER_PRESET_GREEN 128
      #endif
      #ifndef LED_USER_PRESET_BLUE
        #define LED_USER_PRESET_BLUE 0
      #endif
      #ifndef LED_USER_PRESET_WHITE
        #define LED_USER_PRESET_WHITE 255
      #endif
      #ifndef LED_USER_PRESET_BRIGHTNESS
        #define LED_USER_PRESET_BRIGHTNESS 255
      #endif
    #endif
    #if ENABLED(NEO2_COLOR_PRESETS)
      #ifndef NEO2_USER_PRESET_RED
        #define NEO2_USER_PRESET_RED 255
      #endif
      #ifndef NEO2_USER_PRESET_GREEN
        #define NEO2_USER_PRESET_GREEN 128
      #endif
      #ifndef NEO2_USER_PRESET_BLUE
        #define NEO2_USER_PRESET_BLUE 0
      #endif
      #ifndef NEO2_USER_PRESET_WHITE
        #define NEO2_USER_PRESET_WHITE 255
      #endif
      #ifndef NEO2_USER_PRESET_BRIGHTNESS
        #define NEO2_USER_PRESET_BRIGHTNESS 255
      #endif
    #endif
  #endif
#endif
#if HAS_FEEDRATE_EDIT
  #ifndef SPEED_EDIT_MIN
    #define SPEED_EDIT_MIN 10
  #endif
  #ifndef SPEED_EDIT_MAX
    #define SPEED_EDIT_MAX 999
  #endif
#endif
#if HAS_FLOW_EDIT
  #ifndef FLOW_EDIT_MIN
    #define FLOW_EDIT_MIN 10
  #endif
  #ifndef FLOW_EDIT_MAX
    #define FLOW_EDIT_MAX 999
  #endif
#endif
#if ENABLED(SET_PROGRESS_MANUALLY)
  #ifndef SET_PROGRESS_PERCENT
    #define SET_PROGRESS_PERCENT
  #endif
  #ifndef SET_REMAINING_TIME
    #define SET_REMAINING_TIME
  #endif
  #if ALL(M73_REPORT, HAS_MEDIA)
    #ifndef M73_REPORT_SD_ONLY
      #define M73_REPORT_SD_ONLY
    #endif
  #endif
#endif
#if HAS_DISPLAY && ANY(HAS_MEDIA, SET_PROGRESS_MANUALLY)
  #ifndef SHOW_PROGRESS_PERCENT
    #define SHOW_PROGRESS_PERCENT
  #endif
  #ifndef SHOW_ELAPSED_TIME
    #define SHOW_ELAPSED_TIME
  #endif
  #if ENABLED(SET_INTERACTION_TIME)
    #ifndef SHOW_INTERACTION_TIME
      #define SHOW_INTERACTION_TIME
    #endif
  #endif
  #if ANY(HAS_MARLINUI_HD44780, IS_TFTGLCD_PANEL)
    #if ENABLED(LCD_PROGRESS_BAR)
      #ifndef PROGRESS_BAR_BAR_TIME
        #define PROGRESS_BAR_BAR_TIME 2000
      #endif
      #ifndef PROGRESS_BAR_MSG_TIME
        #define PROGRESS_BAR_MSG_TIME 3000
      #endif
      #ifndef PROGRESS_MSG_EXPIRE
        #define PROGRESS_MSG_EXPIRE 0
      #endif
    #endif
  #endif
#endif
#if HAS_MEDIA
  #ifndef SD_PROCEDURE_DEPTH
    #define SD_PROCEDURE_DEPTH 1
  #endif
  #ifndef SD_FINISHED_STEPPERRELEASE
    #define SD_FINISHED_STEPPERRELEASE true
  #endif
  #ifndef SD_FINISHED_RELEASECOMMAND
    #define SD_FINISHED_RELEASECOMMAND "M84"
  #endif
  #ifndef SDCARD_RATHERRECENTFIRST
    #define SDCARD_RATHERRECENTFIRST
  #endif
  #ifndef SD_MENU_CONFIRM_START
    #define SD_MENU_CONFIRM_START
  #endif
  #ifndef EVENT_GCODE_SD_ABORT
    #define EVENT_GCODE_SD_ABORT "G28XY"
  #endif
  #if ENABLED(PRINTER_EVENT_LEDS)
    #ifndef PE_LEDS_COMPLETED_TIME
      #define PE_LEDS_COMPLETED_TIME (30*60)
    #endif
  #endif
  #if ENABLED(POWER_LOSS_RECOVERY)
    #ifndef PLR_ENABLED_DEFAULT
      #define PLR_ENABLED_DEFAULT false
    #endif
    #ifndef POWER_LOSS_MIN_Z_CHANGE
      #define POWER_LOSS_MIN_Z_CHANGE 0.05
    #endif
  #endif
  #if ENABLED(SDCARD_SORT_ALPHA)
    #ifndef SDSORT_REVERSE
      #define SDSORT_REVERSE false
    #endif
    #ifndef SDSORT_LIMIT
      #define SDSORT_LIMIT 40
    #endif
    #ifndef SDSORT_FOLDERS
      #define SDSORT_FOLDERS -1
    #endif
    #ifndef SDSORT_GCODE
      #define SDSORT_GCODE false
    #endif
    #ifndef SDSORT_USES_RAM
      #define SDSORT_USES_RAM false
    #endif
    #ifndef SDSORT_USES_STACK
      #define SDSORT_USES_STACK false
    #endif
    #ifndef SDSORT_CACHE_NAMES
      #define SDSORT_CACHE_NAMES false
    #endif
    #ifndef SDSORT_DYNAMIC_RAM
      #define SDSORT_DYNAMIC_RAM false
    #endif
    #ifndef SDSORT_CACHE_VFATS
      #define SDSORT_CACHE_VFATS 2
    #endif
  #endif
  #if ENABLED(USB_FLASH_DRIVE_SUPPORT)
    #ifndef DISABLE_DUE_SD_MMC
      #define DISABLE_DUE_SD_MMC
    #endif
    #if DISABLED(USE_OTG_USB_HOST)
      #ifndef USB_CS_PIN
        #define USB_CS_PIN SDSS
      #endif
      #ifndef USB_INTR_PIN
        #define USB_INTR_PIN SD_DETECT_PIN
      #endif
    #endif
  #endif
  #if ENABLED(SD_FIRMWARE_UPDATE)
    #ifndef SD_FIRMWARE_UPDATE_EEPROM_ADDR
      #define SD_FIRMWARE_UPDATE_EEPROM_ADDR 0x1FF
    #endif
    #ifndef SD_FIRMWARE_UPDATE_ACTIVE_VALUE
      #define SD_FIRMWARE_UPDATE_ACTIVE_VALUE 0xF0
    #endif
    #ifndef SD_FIRMWARE_UPDATE_INACTIVE_VALUE
      #define SD_FIRMWARE_UPDATE_INACTIVE_VALUE 0xFF
    #endif
  #endif
  #if ENABLED(MULTI_VOLUME)
    #ifndef VOLUME_SD_ONBOARD
      #define VOLUME_SD_ONBOARD
    #endif
    #ifndef VOLUME_USB_FLASH_DRIVE
      #define VOLUME_USB_FLASH_DRIVE
    #endif
    #ifndef DEFAULT_VOLUME
      #define DEFAULT_VOLUME SV_SD_ONBOARD
    #endif
    #ifndef DEFAULT_SHARED_VOLUME
      #define DEFAULT_SHARED_VOLUME SV_USB_FLASH_DRIVE
    #endif
  #endif
#endif
#if HAS_MARLINUI_U8GLIB
  #ifndef XYZ_HOLLOW_FRAME
    #define XYZ_HOLLOW_FRAME
  #endif
  #if IS_U8GLIB_ST7920
    #if ENABLED(LIGHTWEIGHT_UI)
      #ifndef STATUS_EXPIRE_SECONDS
        #define STATUS_EXPIRE_SECONDS 20
      #endif
    #endif
  #endif
  #ifndef STATUS_HOTEND_INVERTED
    #define STATUS_HOTEND_INVERTED
  #endif
  #ifndef STATUS_HOTEND_ANIM
    #define STATUS_HOTEND_ANIM
  #endif
  #ifndef STATUS_BED_ANIM
    #define STATUS_BED_ANIM
  #endif
  #ifndef STATUS_CHAMBER_ANIM
    #define STATUS_CHAMBER_ANIM
  #endif
#endif
#if HAS_MARLINUI_U8GLIB || IS_DWIN_MARLINUI
  #ifndef MENU_HOLLOW_FRAME
    #define MENU_HOLLOW_FRAME
  #endif
#endif
#if HAS_DGUS_LCD
  #ifndef LCD_BAUDRATE
    #define LCD_BAUDRATE 115200
  #endif
  #ifndef DGUS_RX_BUFFER_SIZE
    #define DGUS_RX_BUFFER_SIZE 128
  #endif
  #ifndef DGUS_TX_BUFFER_SIZE
    #define DGUS_TX_BUFFER_SIZE 48
  #endif
  #ifndef DGUS_UPDATE_INTERVAL_MS
    #define DGUS_UPDATE_INTERVAL_MS 500
  #endif
  #if DGUS_UI_IS(FYSETC, MKS, HIPRECY)
    #ifndef DGUS_PRINT_FILENAME
      #define DGUS_PRINT_FILENAME
    #endif
    #ifndef DGUS_PREHEAT_UI
      #define DGUS_PREHEAT_UI
    #endif
    #if !(DGUS_UI_IS(FYSETC, MKS))
      #ifndef DGUS_UI_MOVE_DIS_OPTION
        #define DGUS_UI_MOVE_DIS_OPTION
      #endif
    #endif
    #ifndef DGUS_FILAMENT_LOADUNLOAD
      #define DGUS_FILAMENT_LOADUNLOAD
    #endif
    #if ENABLED(DGUS_FILAMENT_LOADUNLOAD)
      #ifndef DGUS_FILAMENT_PURGE_LENGTH
        #define DGUS_FILAMENT_PURGE_LENGTH 10
      #endif
      #ifndef DGUS_FILAMENT_LOAD_LENGTH_PER_TIME
        #define DGUS_FILAMENT_LOAD_LENGTH_PER_TIME 0.5
      #endif
    #endif
    #ifndef DGUS_UI_WAITING
      #define DGUS_UI_WAITING
    #endif
    #if ENABLED(DGUS_UI_WAITING)
      #ifndef DGUS_UI_WAITING_STATUS
        #define DGUS_UI_WAITING_STATUS 10
      #endif
      #ifndef DGUS_UI_WAITING_STATUS_PERIOD
        #define DGUS_UI_WAITING_STATUS_PERIOD 8
      #endif
    #endif
  #elif DGUS_UI_IS(E3S1PRO)
    #ifndef DGUS_SOFTWARE_AUTOSCROLL
      #define DGUS_SOFTWARE_AUTOSCROLL
    #endif
    #ifndef DGUS_AUTOSCROLL_START_CYCLES
      #define DGUS_AUTOSCROLL_START_CYCLES 1
    #endif
    #ifndef DGUS_AUTOSCROLL_END_CYCLES
      #define DGUS_AUTOSCROLL_END_CYCLES 1
    #endif
    #ifndef DGUS_ADVANCED_SDCARD
      #define DGUS_ADVANCED_SDCARD
    #endif
    #ifndef DGUS_USERCONFIRM
      #define DGUS_USERCONFIRM
    #endif
  #endif
#endif
#if ENABLED(ANYCUBIC_LCD_CHIRON)
  #ifndef AC_SD_FOLDER_VIEW
    #define AC_SD_FOLDER_VIEW
  #endif
#endif
#if ENABLED(TOUCH_UI_FTDI_EVE)
  #if ENABLED(OTHER_PIN_LAYOUT)
    #ifndef CLCD_MOD_RESET
      #define CLCD_MOD_RESET 9
    #endif
    #ifndef CLCD_SPI_CS
      #define CLCD_SPI_CS 10
    #endif
    #if ENABLED(CLCD_USE_SOFT_SPI)
      #ifndef CLCD_SOFT_SPI_MOSI
        #define CLCD_SOFT_SPI_MOSI 11
      #endif
      #ifndef CLCD_SOFT_SPI_MISO
        #define CLCD_SOFT_SPI_MISO 12
      #endif
      #ifndef CLCD_SOFT_SPI_SCLK
        #define CLCD_SOFT_SPI_SCLK 13
      #endif
    #endif
  #endif
  #if ENABLED(TOUCH_UI_USE_UTF8)
    #ifndef TOUCH_UI_UTF8_WESTERN_CHARSET
      #define TOUCH_UI_UTF8_WESTERN_CHARSET
    #endif
  #endif
  #ifndef TOUCH_UI_FIT_TEXT
    #define TOUCH_UI_FIT_TEXT
  #endif
#endif
#if defined(DISPLAY_SLEEP_MINUTES) || defined(LCD_BACKLIGHT_TIMEOUT_MINS)
  #ifndef EDITABLE_DISPLAY_TIMEOUT
    #define EDITABLE_DISPLAY_TIMEOUT
  #endif
#endif
#if HAS_ADC_BUTTONS
  #ifndef ADC_BUTTON_DEBOUNCE_DELAY
    #define ADC_BUTTON_DEBOUNCE_DELAY 16
  #endif
#endif
#ifndef USE_WATCHDOG
  #define USE_WATCHDOG
#endif
#if ENABLED(BABYSTEPPING)
  #ifndef BABYSTEP_MULTIPLICATOR_Z
    #define BABYSTEP_MULTIPLICATOR_Z 1
  #endif
  #ifndef BABYSTEP_MULTIPLICATOR_XY
    #define BABYSTEP_MULTIPLICATOR_XY 1
  #endif
  #if ENABLED(DOUBLECLICK_FOR_Z_BABYSTEPPING)
    #ifndef DOUBLECLICK_MAX_INTERVAL
      #define DOUBLECLICK_MAX_INTERVAL 1250
    #endif
    #if ENABLED(MOVE_Z_WHEN_IDLE)
      #ifndef MOVE_Z_IDLE_MULTIPLICATOR
        #define MOVE_Z_IDLE_MULTIPLICATOR 1
      #endif
    #endif
  #endif
#endif
#if ENABLED(LIN_ADVANCE)
  #if ENABLED(DISTINCT_E_FACTORS)
    #ifndef ADVANCE_K
      #define ADVANCE_K { 0.22 }
    #endif
  #else
    #ifndef ADVANCE_K
      #define ADVANCE_K 0.22
    #endif
  #endif
#endif
#if ENABLED(G29_RETRY_AND_RECOVER)
  #ifndef G29_MAX_RETRIES
    #define G29_MAX_RETRIES 3
  #endif
  #ifndef G29_HALT_ON_FAILURE
    #define G29_HALT_ON_FAILURE
  #endif
  #ifndef G29_SUCCESS_COMMANDS
    #define G29_SUCCESS_COMMANDS "M117 Bed leveling done."
  #endif
  #ifndef G29_RECOVER_COMMANDS
    #define G29_RECOVER_COMMANDS "M117 Probe failed. Rewiping.\nG28\nG12 P0 S12 T0"
  #endif
  #ifndef G29_FAILURE_COMMANDS
    #define G29_FAILURE_COMMANDS "M117 Bed leveling failed.\nG0 Z10\nM300 P25 S880\nM300 P50 S0\nM300 P25 S880\nM300 P50 S0\nM300 P25 S880\nM300 P50 S0\nG4 S1"
  #endif
#endif
#if ANY(PTC_PROBE, PTC_BED, PTC_HOTEND)
  #if ENABLED(PTC_PROBE)
    #ifndef PTC_PROBE_START
      #define PTC_PROBE_START 30
    #endif
    #ifndef PTC_PROBE_RES
      #define PTC_PROBE_RES 5
    #endif
    #ifndef PTC_PROBE_COUNT
      #define PTC_PROBE_COUNT 10
    #endif
    #ifndef PTC_PROBE_ZOFFS
      #define PTC_PROBE_ZOFFS { 0 }
    #endif
  #endif
  #if ENABLED(PTC_BED)
    #ifndef PTC_BED_START
      #define PTC_BED_START 60
    #endif
    #ifndef PTC_BED_RES
      #define PTC_BED_RES 5
    #endif
    #ifndef PTC_BED_COUNT
      #define PTC_BED_COUNT 10
    #endif
    #ifndef PTC_BED_ZOFFS
      #define PTC_BED_ZOFFS { 0 }
    #endif
  #endif
  #if ENABLED(PTC_HOTEND)
    #ifndef PTC_HOTEND_START
      #define PTC_HOTEND_START 180
    #endif
    #ifndef PTC_HOTEND_RES
      #define PTC_HOTEND_RES 5
    #endif
    #ifndef PTC_HOTEND_COUNT
      #define PTC_HOTEND_COUNT 20
    #endif
    #ifndef PTC_HOTEND_ZOFFS
      #define PTC_HOTEND_ZOFFS { 0 }
    #endif
  #endif
  #if ALL(PTC_PROBE, PTC_BED)
    #ifndef PTC_PARK_POS
      #define PTC_PARK_POS { 0, 0, 100 }
    #endif
    #ifndef PTC_PROBE_POS
      #define PTC_PROBE_POS { 90, 100 }
    #endif
    #ifndef PTC_PROBE_TEMP
      #define PTC_PROBE_TEMP 30
    #endif
    #ifndef PTC_PROBE_HEATING_OFFSET
      #define PTC_PROBE_HEATING_OFFSET 0.5
    #endif
  #endif
#endif
#ifndef ARC_SUPPORT
  #define ARC_SUPPORT
#endif
#if ENABLED(ARC_SUPPORT)
  #ifndef MIN_ARC_SEGMENT_MM
    #define MIN_ARC_SEGMENT_MM 0.1
  #endif
  #ifndef MAX_ARC_SEGMENT_MM
    #define MAX_ARC_SEGMENT_MM 1.0
  #endif
  #ifndef MIN_CIRCLE_SEGMENTS
    #define MIN_CIRCLE_SEGMENTS 72
  #endif
  #ifndef N_ARC_CORRECTION
    #define N_ARC_CORRECTION 25
  #endif
#endif
#if ENABLED(G38_PROBE_TARGET)
  #ifndef G38_MINIMUM_MOVE
    #define G38_MINIMUM_MOVE 0.0275
  #endif
#endif
#ifndef MIN_STEPS_PER_SEGMENT
  #define MIN_STEPS_PER_SEGMENT 6
#endif
#if ALL(HAS_MEDIA, DIRECT_STEPPING)
  #ifndef BLOCK_BUFFER_SIZE
    #define BLOCK_BUFFER_SIZE 8
  #endif
#elif HAS_MEDIA
  #ifndef BLOCK_BUFFER_SIZE
    #define BLOCK_BUFFER_SIZE 16
  #endif
#else
  #ifndef BLOCK_BUFFER_SIZE
    #define BLOCK_BUFFER_SIZE 16
  #endif
#endif
#ifndef MAX_CMD_SIZE
  #define MAX_CMD_SIZE 96
#endif
#ifndef BUFSIZE
  #define BUFSIZE 4
#endif
#ifndef TX_BUFFER_SIZE
  #define TX_BUFFER_SIZE 0
#endif
#ifndef SERIAL_OVERRUN_PROTECTION
  #define SERIAL_OVERRUN_PROTECTION
#endif
#ifndef PROPORTIONAL_FONT_RATIO
  #define PROPORTIONAL_FONT_RATIO 1.0
#endif
#if ENABLED(FWRETRACT)
  #ifndef FWRETRACT_AUTORETRACT
    #define FWRETRACT_AUTORETRACT
  #endif
  #if ENABLED(FWRETRACT_AUTORETRACT)
    #ifndef MIN_AUTORETRACT
      #define MIN_AUTORETRACT 0.1
    #endif
    #ifndef MAX_AUTORETRACT
      #define MAX_AUTORETRACT 10.0
    #endif
  #endif
  #ifndef RETRACT_LENGTH
    #define RETRACT_LENGTH 3
  #endif
  #ifndef RETRACT_LENGTH_SWAP
    #define RETRACT_LENGTH_SWAP 13
  #endif
  #ifndef RETRACT_FEEDRATE
    #define RETRACT_FEEDRATE 45
  #endif
  #ifndef RETRACT_ZRAISE
    #define RETRACT_ZRAISE 0
  #endif
  #ifndef RETRACT_RECOVER_LENGTH
    #define RETRACT_RECOVER_LENGTH 0
  #endif
  #ifndef RETRACT_RECOVER_LENGTH_SWAP
    #define RETRACT_RECOVER_LENGTH_SWAP 0
  #endif
  #ifndef RETRACT_RECOVER_FEEDRATE
    #define RETRACT_RECOVER_FEEDRATE 8
  #endif
  #ifndef RETRACT_RECOVER_FEEDRATE_SWAP
    #define RETRACT_RECOVER_FEEDRATE_SWAP 8
  #endif
#endif
#if HAS_MULTI_EXTRUDER
  #ifndef TOOLCHANGE_ZRAISE
    #define TOOLCHANGE_ZRAISE 2
  #endif
  #if ENABLED(TOOLCHANGE_FILAMENT_SWAP)
    #ifndef TOOLCHANGE_FS_LENGTH
      #define TOOLCHANGE_FS_LENGTH 12
    #endif
    #ifndef TOOLCHANGE_FS_EXTRA_RESUME_LENGTH
      #define TOOLCHANGE_FS_EXTRA_RESUME_LENGTH 0
    #endif
    #ifndef TOOLCHANGE_FS_RETRACT_SPEED
      #define TOOLCHANGE_FS_RETRACT_SPEED (50*60)
    #endif
    #ifndef TOOLCHANGE_FS_UNRETRACT_SPEED
      #define TOOLCHANGE_FS_UNRETRACT_SPEED (25*60)
    #endif
    #ifndef TOOLCHANGE_FS_EXTRA_PRIME
      #define TOOLCHANGE_FS_EXTRA_PRIME 0
    #endif
    #ifndef TOOLCHANGE_FS_PRIME_SPEED
      #define TOOLCHANGE_FS_PRIME_SPEED (4.6*60)
    #endif
    #ifndef TOOLCHANGE_FS_WIPE_RETRACT
      #define TOOLCHANGE_FS_WIPE_RETRACT 0
    #endif
    #ifndef TOOLCHANGE_FS_FAN
      #define TOOLCHANGE_FS_FAN -1
    #endif
    #ifndef TOOLCHANGE_FS_FAN_SPEED
      #define TOOLCHANGE_FS_FAN_SPEED 255
    #endif
    #ifndef TOOLCHANGE_FS_FAN_TIME
      #define TOOLCHANGE_FS_FAN_TIME 10
    #endif
    #ifndef TOOLCHANGE_MIGRATION_FEATURE
      #define TOOLCHANGE_MIGRATION_FEATURE
    #endif
  #endif
  #if ENABLED(TOOLCHANGE_PARK)
    #ifndef TOOLCHANGE_PARK_XY
      #define TOOLCHANGE_PARK_XY { X_MIN_POS + 10, Y_MIN_POS + 10 }
    #endif
    #ifndef TOOLCHANGE_PARK_XY_FEEDRATE
      #define TOOLCHANGE_PARK_XY_FEEDRATE 6000
    #endif
  #endif
#endif
#if ENABLED(ADVANCED_PAUSE_FEATURE)
  #ifndef PAUSE_PARK_RETRACT_FEEDRATE
    #define PAUSE_PARK_RETRACT_FEEDRATE 60
  #endif
  #ifndef PAUSE_PARK_RETRACT_LENGTH
    #define PAUSE_PARK_RETRACT_LENGTH 2
  #endif
  #ifndef FILAMENT_CHANGE_UNLOAD_FEEDRATE
    #define FILAMENT_CHANGE_UNLOAD_FEEDRATE 10
  #endif
  #ifndef FILAMENT_CHANGE_UNLOAD_ACCEL
    #define FILAMENT_CHANGE_UNLOAD_ACCEL 25
  #endif
  #ifndef FILAMENT_CHANGE_UNLOAD_LENGTH
    #define FILAMENT_CHANGE_UNLOAD_LENGTH 100
  #endif
  #ifndef FILAMENT_CHANGE_SLOW_LOAD_FEEDRATE
    #define FILAMENT_CHANGE_SLOW_LOAD_FEEDRATE 6
  #endif
  #ifndef FILAMENT_CHANGE_SLOW_LOAD_LENGTH
    #define FILAMENT_CHANGE_SLOW_LOAD_LENGTH 0
  #endif
  #ifndef FILAMENT_CHANGE_FAST_LOAD_FEEDRATE
    #define FILAMENT_CHANGE_FAST_LOAD_FEEDRATE 6
  #endif
  #ifndef FILAMENT_CHANGE_FAST_LOAD_ACCEL
    #define FILAMENT_CHANGE_FAST_LOAD_ACCEL 25
  #endif
  #ifndef FILAMENT_CHANGE_FAST_LOAD_LENGTH
    #define FILAMENT_CHANGE_FAST_LOAD_LENGTH 0
  #endif
  #ifndef ADVANCED_PAUSE_PURGE_FEEDRATE
    #define ADVANCED_PAUSE_PURGE_FEEDRATE 3
  #endif
  #ifndef ADVANCED_PAUSE_PURGE_LENGTH
    #define ADVANCED_PAUSE_PURGE_LENGTH 50
  #endif
  #ifndef ADVANCED_PAUSE_RESUME_PRIME
    #define ADVANCED_PAUSE_RESUME_PRIME 0
  #endif
  #ifndef FILAMENT_UNLOAD_PURGE_RETRACT
    #define FILAMENT_UNLOAD_PURGE_RETRACT 13
  #endif
  #ifndef FILAMENT_UNLOAD_PURGE_DELAY
    #define FILAMENT_UNLOAD_PURGE_DELAY 5000
  #endif
  #ifndef FILAMENT_UNLOAD_PURGE_LENGTH
    #define FILAMENT_UNLOAD_PURGE_LENGTH 8
  #endif
  #ifndef FILAMENT_UNLOAD_PURGE_FEEDRATE
    #define FILAMENT_UNLOAD_PURGE_FEEDRATE 25
  #endif
  #ifndef PAUSE_PARK_NOZZLE_TIMEOUT
    #define PAUSE_PARK_NOZZLE_TIMEOUT 45
  #endif
  #ifndef FILAMENT_CHANGE_ALERT_BEEPS
    #define FILAMENT_CHANGE_ALERT_BEEPS 10
  #endif
  #ifndef PAUSE_PARK_NO_STEPPER_TIMEOUT
    #define PAUSE_PARK_NO_STEPPER_TIMEOUT
  #endif
  #ifndef CONFIGURE_FILAMENT_CHANGE
    #define CONFIGURE_FILAMENT_CHANGE
  #endif
#endif
#if HAS_TRINAMIC_CONFIG
  #ifndef HOLD_MULTIPLIER
    #define HOLD_MULTIPLIER 0.5
  #endif
  #ifndef INTERPOLATE
    #define INTERPOLATE true
  #endif
  #if AXIS_IS_TMC_CONFIG(X)
    #ifndef X_CURRENT
      #define X_CURRENT 800
    #endif
    #ifndef X_CURRENT_HOME
      #define X_CURRENT_HOME X_CURRENT
    #endif
    #ifndef X_MICROSTEPS
      #define X_MICROSTEPS 16
    #endif
    #ifndef X_RSENSE
      #define X_RSENSE 0.11
    #endif
    #ifndef X_CHAIN_POS
      #define X_CHAIN_POS -1
    #endif
  #endif
  #if AXIS_IS_TMC_CONFIG(X2)
    #ifndef X2_CURRENT
      #define X2_CURRENT X_CURRENT
    #endif
    #ifndef X2_CURRENT_HOME
      #define X2_CURRENT_HOME X_CURRENT_HOME
    #endif
    #ifndef X2_MICROSTEPS
      #define X2_MICROSTEPS X_MICROSTEPS
    #endif
    #ifndef X2_RSENSE
      #define X2_RSENSE X_RSENSE
    #endif
    #ifndef X2_CHAIN_POS
      #define X2_CHAIN_POS -1
    #endif
  #endif
  #if AXIS_IS_TMC_CONFIG(Y)
    #ifndef Y_CURRENT
      #define Y_CURRENT 800
    #endif
    #ifndef Y_CURRENT_HOME
      #define Y_CURRENT_HOME Y_CURRENT
    #endif
    #ifndef Y_MICROSTEPS
      #define Y_MICROSTEPS 16
    #endif
    #ifndef Y_RSENSE
      #define Y_RSENSE 0.11
    #endif
    #ifndef Y_CHAIN_POS
      #define Y_CHAIN_POS -1
    #endif
  #endif
  #if AXIS_IS_TMC_CONFIG(Y2)
    #ifndef Y2_CURRENT
      #define Y2_CURRENT Y_CURRENT
    #endif
    #ifndef Y2_CURRENT_HOME
      #define Y2_CURRENT_HOME Y_CURRENT_HOME
    #endif
    #ifndef Y2_MICROSTEPS
      #define Y2_MICROSTEPS Y_MICROSTEPS
    #endif
    #ifndef Y2_RSENSE
      #define Y2_RSENSE Y_RSENSE
    #endif
    #ifndef Y2_CHAIN_POS
      #define Y2_CHAIN_POS -1
    #endif
  #endif
  #if AXIS_IS_TMC_CONFIG(Z)
    #ifndef Z_CURRENT
      #define Z_CURRENT 800
    #endif
    #ifndef Z_CURRENT_HOME
      #define Z_CURRENT_HOME Z_CURRENT
    #endif
    #ifndef Z_MICROSTEPS
      #define Z_MICROSTEPS 16
    #endif
    #ifndef Z_RSENSE
      #define Z_RSENSE 0.11
    #endif
    #ifndef Z_CHAIN_POS
      #define Z_CHAIN_POS -1
    #endif
  #endif
  #if AXIS_IS_TMC_CONFIG(Z2)
    #ifndef Z2_CURRENT
      #define Z2_CURRENT Z_CURRENT
    #endif
    #ifndef Z2_CURRENT_HOME
      #define Z2_CURRENT_HOME Z_CURRENT_HOME
    #endif
    #ifndef Z2_MICROSTEPS
      #define Z2_MICROSTEPS Z_MICROSTEPS
    #endif
    #ifndef Z2_RSENSE
      #define Z2_RSENSE Z_RSENSE
    #endif
    #ifndef Z2_CHAIN_POS
      #define Z2_CHAIN_POS -1
    #endif
  #endif
  #if AXIS_IS_TMC_CONFIG(Z3)
    #ifndef Z3_CURRENT
      #define Z3_CURRENT Z_CURRENT
    #endif
    #ifndef Z3_CURRENT_HOME
      #define Z3_CURRENT_HOME Z_CURRENT_HOME
    #endif
    #ifndef Z3_MICROSTEPS
      #define Z3_MICROSTEPS Z_MICROSTEPS
    #endif
    #ifndef Z3_RSENSE
      #define Z3_RSENSE Z_RSENSE
    #endif
    #ifndef Z3_CHAIN_POS
      #define Z3_CHAIN_POS -1
    #endif
  #endif
  #if AXIS_IS_TMC_CONFIG(Z4)
    #ifndef Z4_CURRENT
      #define Z4_CURRENT Z_CURRENT
    #endif
    #ifndef Z4_CURRENT_HOME
      #define Z4_CURRENT_HOME Z_CURRENT_HOME
    #endif
    #ifndef Z4_MICROSTEPS
      #define Z4_MICROSTEPS Z_MICROSTEPS
    #endif
    #ifndef Z4_RSENSE
      #define Z4_RSENSE Z_RSENSE
    #endif
    #ifndef Z4_CHAIN_POS
      #define Z4_CHAIN_POS -1
    #endif
  #endif
  #if AXIS_IS_TMC_CONFIG(I)
    #ifndef I_CURRENT
      #define I_CURRENT 800
    #endif
    #ifndef I_CURRENT_HOME
      #define I_CURRENT_HOME I_CURRENT
    #endif
    #ifndef I_MICROSTEPS
      #define I_MICROSTEPS 16
    #endif
    #ifndef I_RSENSE
      #define I_RSENSE 0.11
    #endif
    #ifndef I_CHAIN_POS
      #define I_CHAIN_POS -1
    #endif
  #endif
  #if AXIS_IS_TMC_CONFIG(J)
    #ifndef J_CURRENT
      #define J_CURRENT 800
    #endif
    #ifndef J_CURRENT_HOME
      #define J_CURRENT_HOME J_CURRENT
    #endif
    #ifndef J_MICROSTEPS
      #define J_MICROSTEPS 16
    #endif
    #ifndef J_RSENSE
      #define J_RSENSE 0.11
    #endif
    #ifndef J_CHAIN_POS
      #define J_CHAIN_POS -1
    #endif
  #endif
  #if AXIS_IS_TMC_CONFIG(K)
    #ifndef K_CURRENT
      #define K_CURRENT 800
    #endif
    #ifndef K_CURRENT_HOME
      #define K_CURRENT_HOME K_CURRENT
    #endif
    #ifndef K_MICROSTEPS
      #define K_MICROSTEPS 16
    #endif
    #ifndef K_RSENSE
      #define K_RSENSE 0.11
    #endif
    #ifndef K_CHAIN_POS
      #define K_CHAIN_POS -1
    #endif
  #endif
  #if AXIS_IS_TMC_CONFIG(U)
    #ifndef U_CURRENT
      #define U_CURRENT 800
    #endif
    #ifndef U_CURRENT_HOME
      #define U_CURRENT_HOME U_CURRENT
    #endif
    #ifndef U_MICROSTEPS
      #define U_MICROSTEPS 8
    #endif
    #ifndef U_RSENSE
      #define U_RSENSE 0.11
    #endif
    #ifndef U_CHAIN_POS
      #define U_CHAIN_POS -1
    #endif
  #endif
  #if AXIS_IS_TMC_CONFIG(V)
    #ifndef V_CURRENT
      #define V_CURRENT 800
    #endif
    #ifndef V_CURRENT_HOME
      #define V_CURRENT_HOME V_CURRENT
    #endif
    #ifndef V_MICROSTEPS
      #define V_MICROSTEPS 8
    #endif
    #ifndef V_RSENSE
      #define V_RSENSE 0.11
    #endif
    #ifndef V_CHAIN_POS
      #define V_CHAIN_POS -1
    #endif
  #endif
  #if AXIS_IS_TMC_CONFIG(W)
    #ifndef W_CURRENT
      #define W_CURRENT 800
    #endif
    #ifndef W_CURRENT_HOME
      #define W_CURRENT_HOME W_CURRENT
    #endif
    #ifndef W_MICROSTEPS
      #define W_MICROSTEPS 8
    #endif
    #ifndef W_RSENSE
      #define W_RSENSE 0.11
    #endif
    #ifndef W_CHAIN_POS
      #define W_CHAIN_POS -1
    #endif
  #endif
  #if AXIS_IS_TMC_CONFIG(E0)
    #ifndef E0_CURRENT
      #define E0_CURRENT 800
    #endif
    #ifndef E0_MICROSTEPS
      #define E0_MICROSTEPS 16
    #endif
    #ifndef E0_RSENSE
      #define E0_RSENSE 0.11
    #endif
    #ifndef E0_CHAIN_POS
      #define E0_CHAIN_POS -1
    #endif
  #endif
  #if AXIS_IS_TMC_CONFIG(E1)
    #ifndef E1_CURRENT
      #define E1_CURRENT E0_CURRENT
    #endif
    #ifndef E1_MICROSTEPS
      #define E1_MICROSTEPS E0_MICROSTEPS
    #endif
    #ifndef E1_RSENSE
      #define E1_RSENSE E0_RSENSE
    #endif
    #ifndef E1_CHAIN_POS
      #define E1_CHAIN_POS -1
    #endif
  #endif
  #if AXIS_IS_TMC_CONFIG(E2)
    #ifndef E2_CURRENT
      #define E2_CURRENT E0_CURRENT
    #endif
    #ifndef E2_MICROSTEPS
      #define E2_MICROSTEPS E0_MICROSTEPS
    #endif
    #ifndef E2_RSENSE
      #define E2_RSENSE E0_RSENSE
    #endif
    #ifndef E2_CHAIN_POS
      #define E2_CHAIN_POS -1
    #endif
  #endif
  #if AXIS_IS_TMC_CONFIG(E3)
    #ifndef E3_CURRENT
      #define E3_CURRENT E0_CURRENT
    #endif
    #ifndef E3_MICROSTEPS
      #define E3_MICROSTEPS E0_MICROSTEPS
    #endif
    #ifndef E3_RSENSE
      #define E3_RSENSE E0_RSENSE
    #endif
    #ifndef E3_CHAIN_POS
      #define E3_CHAIN_POS -1
    #endif
  #endif
  #if AXIS_IS_TMC_CONFIG(E4)
    #ifndef E4_CURRENT
      #define E4_CURRENT E0_CURRENT
    #endif
    #ifndef E4_MICROSTEPS
      #define E4_MICROSTEPS E0_MICROSTEPS
    #endif
    #ifndef E4_RSENSE
      #define E4_RSENSE E0_RSENSE
    #endif
    #ifndef E4_CHAIN_POS
      #define E4_CHAIN_POS -1
    #endif
  #endif
  #if AXIS_IS_TMC_CONFIG(E5)
    #ifndef E5_CURRENT
      #define E5_CURRENT E0_CURRENT
    #endif
    #ifndef E5_MICROSTEPS
      #define E5_MICROSTEPS E0_MICROSTEPS
    #endif
    #ifndef E5_RSENSE
      #define E5_RSENSE E0_RSENSE
    #endif
    #ifndef E5_CHAIN_POS
      #define E5_CHAIN_POS -1
    #endif
  #endif
  #if AXIS_IS_TMC_CONFIG(E6)
    #ifndef E6_CURRENT
      #define E6_CURRENT E0_CURRENT
    #endif
    #ifndef E6_MICROSTEPS
      #define E6_MICROSTEPS E0_MICROSTEPS
    #endif
    #ifndef E6_RSENSE
      #define E6_RSENSE E0_RSENSE
    #endif
    #ifndef E6_CHAIN_POS
      #define E6_CHAIN_POS -1
    #endif
  #endif
  #if AXIS_IS_TMC_CONFIG(E7)
    #ifndef E7_CURRENT
      #define E7_CURRENT E0_CURRENT
    #endif
    #ifndef E7_MICROSTEPS
      #define E7_MICROSTEPS E0_MICROSTEPS
    #endif
    #ifndef E7_RSENSE
      #define E7_RSENSE E0_RSENSE
    #endif
    #ifndef E7_CHAIN_POS
      #define E7_CHAIN_POS -1
    #endif
  #endif
  #if HAS_STEALTHCHOP
    #ifndef STEALTHCHOP_XY
      #define STEALTHCHOP_XY
    #endif
    #ifndef STEALTHCHOP_Z
      #define STEALTHCHOP_Z
    #endif
    #ifndef STEALTHCHOP_I
      #define STEALTHCHOP_I
    #endif
    #ifndef STEALTHCHOP_J
      #define STEALTHCHOP_J
    #endif
    #ifndef STEALTHCHOP_K
      #define STEALTHCHOP_K
    #endif
    #ifndef STEALTHCHOP_U
      #define STEALTHCHOP_U
    #endif
    #ifndef STEALTHCHOP_V
      #define STEALTHCHOP_V
    #endif
    #ifndef STEALTHCHOP_W
      #define STEALTHCHOP_W
    #endif
    #ifndef STEALTHCHOP_E
      #define STEALTHCHOP_E
    #endif
  #endif
  #ifndef CHOPPER_TIMING
    #define CHOPPER_TIMING CHOPPER_DEFAULT_12V
  #endif
  #if ENABLED(MONITOR_DRIVER_STATUS)
    #ifndef CURRENT_STEP_DOWN
      #define CURRENT_STEP_DOWN 50
    #endif
    #ifndef REPORT_CURRENT_CHANGE
      #define REPORT_CURRENT_CHANGE
    #endif
    #ifndef STOP_ON_ERROR
      #define STOP_ON_ERROR
    #endif
  #endif
  #ifndef X_HYBRID_THRESHOLD
    #define X_HYBRID_THRESHOLD 100
  #endif
  #ifndef X2_HYBRID_THRESHOLD
    #define X2_HYBRID_THRESHOLD 100
  #endif
  #ifndef Y_HYBRID_THRESHOLD
    #define Y_HYBRID_THRESHOLD 100
  #endif
  #ifndef Y2_HYBRID_THRESHOLD
    #define Y2_HYBRID_THRESHOLD 100
  #endif
  #ifndef Z_HYBRID_THRESHOLD
    #define Z_HYBRID_THRESHOLD 3
  #endif
  #ifndef Z2_HYBRID_THRESHOLD
    #define Z2_HYBRID_THRESHOLD 3
  #endif
  #ifndef Z3_HYBRID_THRESHOLD
    #define Z3_HYBRID_THRESHOLD 3
  #endif
  #ifndef Z4_HYBRID_THRESHOLD
    #define Z4_HYBRID_THRESHOLD 3
  #endif
  #ifndef I_HYBRID_THRESHOLD
    #define I_HYBRID_THRESHOLD 3
  #endif
  #ifndef J_HYBRID_THRESHOLD
    #define J_HYBRID_THRESHOLD 3
  #endif
  #ifndef K_HYBRID_THRESHOLD
    #define K_HYBRID_THRESHOLD 3
  #endif
  #ifndef U_HYBRID_THRESHOLD
    #define U_HYBRID_THRESHOLD 3
  #endif
  #ifndef V_HYBRID_THRESHOLD
    #define V_HYBRID_THRESHOLD 3
  #endif
  #ifndef W_HYBRID_THRESHOLD
    #define W_HYBRID_THRESHOLD 3
  #endif
  #ifndef E0_HYBRID_THRESHOLD
    #define E0_HYBRID_THRESHOLD 30
  #endif
  #ifndef E1_HYBRID_THRESHOLD
    #define E1_HYBRID_THRESHOLD 30
  #endif
  #ifndef E2_HYBRID_THRESHOLD
    #define E2_HYBRID_THRESHOLD 30
  #endif
  #ifndef E3_HYBRID_THRESHOLD
    #define E3_HYBRID_THRESHOLD 30
  #endif
  #ifndef E4_HYBRID_THRESHOLD
    #define E4_HYBRID_THRESHOLD 30
  #endif
  #ifndef E5_HYBRID_THRESHOLD
    #define E5_HYBRID_THRESHOLD 30
  #endif
  #ifndef E6_HYBRID_THRESHOLD
    #define E6_HYBRID_THRESHOLD 30
  #endif
  #ifndef E7_HYBRID_THRESHOLD
    #define E7_HYBRID_THRESHOLD 30
  #endif
  #if ANY(SENSORLESS_HOMING, SENSORLESS_PROBING)
    #ifndef X_STALL_SENSITIVITY
      #define X_STALL_SENSITIVITY 8
    #endif
    #ifndef X2_STALL_SENSITIVITY
      #define X2_STALL_SENSITIVITY X_STALL_SENSITIVITY
    #endif
    #ifndef Y_STALL_SENSITIVITY
      #define Y_STALL_SENSITIVITY 8
    #endif
    #ifndef Y2_STALL_SENSITIVITY
      #define Y2_STALL_SENSITIVITY Y_STALL_SENSITIVITY
    #endif
  #endif
  #ifndef EDGE_STEPPING
    #define EDGE_STEPPING
  #endif
  #ifndef TMC_ADV
    #define TMC_ADV () {  }
  #endif
#endif
#if ENABLED(EXPERIMENTAL_I2CBUS)
  #ifndef I2C_SLAVE_ADDRESS
    #define I2C_SLAVE_ADDRESS 0
  #endif
#endif
#if ENABLED(PHOTO_GCODE)
  #ifdef PHOTO_PULSES_US
    #ifndef PHOTO_PULSE_DELAY_US
      #define PHOTO_PULSE_DELAY_US 13
    #endif
  #endif
#endif
#if ANY(SPINDLE_FEATURE, LASER_FEATURE)
  #ifndef SPINDLE_LASER_ACTIVE_STATE
    #define SPINDLE_LASER_ACTIVE_STATE LOW
  #endif
  #ifndef SPINDLE_LASER_USE_PWM
    #define SPINDLE_LASER_USE_PWM
  #endif
  #if ENABLED(SPINDLE_LASER_USE_PWM)
    #ifndef SPINDLE_LASER_PWM_INVERT
      #define SPINDLE_LASER_PWM_INVERT false
    #endif
    #ifndef SPINDLE_LASER_FREQUENCY
      #define SPINDLE_LASER_FREQUENCY 2500
    #endif
  #endif
  #if ENABLED(AIR_EVACUATION)
    #ifndef AIR_EVACUATION_ACTIVE
      #define AIR_EVACUATION_ACTIVE LOW
    #endif
  #endif
  #if ENABLED(AIR_ASSIST)
    #ifndef AIR_ASSIST_ACTIVE
      #define AIR_ASSIST_ACTIVE LOW
    #endif
  #endif
  #ifdef SPINDLE_SERVO
    #ifndef SPINDLE_SERVO_NR
      #define SPINDLE_SERVO_NR 0
    #endif
    #ifndef SPINDLE_SERVO_MIN
      #define SPINDLE_SERVO_MIN 10
    #endif
  #endif
  #ifndef CUTTER_POWER_UNIT
    #define CUTTER_POWER_UNIT PWM255
  #endif
  #if ENABLED(SPINDLE_FEATURE)
    #ifndef SPINDLE_CHANGE_DIR_STOP
      #define SPINDLE_CHANGE_DIR_STOP
    #endif
    #ifndef SPINDLE_INVERT_DIR
      #define SPINDLE_INVERT_DIR false
    #endif
    #ifndef SPINDLE_LASER_POWERUP_DELAY
      #define SPINDLE_LASER_POWERUP_DELAY 5000
    #endif
    #ifndef SPINDLE_LASER_POWERDOWN_DELAY
      #define SPINDLE_LASER_POWERDOWN_DELAY 5000
    #endif
    #if ENABLED(SPINDLE_LASER_USE_PWM)
      #ifndef SPEED_POWER_INTERCEPT
        #define SPEED_POWER_INTERCEPT 0
      #endif
      #ifndef SPEED_POWER_MIN
        #define SPEED_POWER_MIN 5000
      #endif
      #ifndef SPEED_POWER_MAX
        #define SPEED_POWER_MAX 30000
      #endif
      #ifndef SPEED_POWER_STARTUP
        #define SPEED_POWER_STARTUP 25000
      #endif
    #endif
  #else
    #if ENABLED(SPINDLE_LASER_USE_PWM)
      #ifndef SPEED_POWER_INTERCEPT
        #define SPEED_POWER_INTERCEPT 0
      #endif
      #ifndef SPEED_POWER_MIN
        #define SPEED_POWER_MIN 0
      #endif
      #ifndef SPEED_POWER_MAX
        #define SPEED_POWER_MAX 100
      #endif
      #ifndef SPEED_POWER_STARTUP
        #define SPEED_POWER_STARTUP 80
      #endif
    #endif
    #ifndef LASER_TEST_PULSE_MIN
      #define LASER_TEST_PULSE_MIN 1
    #endif
    #ifndef LASER_TEST_PULSE_MAX
      #define LASER_TEST_PULSE_MAX 999
    #endif
    #ifndef SPINDLE_LASER_POWERUP_DELAY
      #define SPINDLE_LASER_POWERUP_DELAY 50
    #endif
    #ifndef SPINDLE_LASER_POWERDOWN_DELAY
      #define SPINDLE_LASER_POWERDOWN_DELAY 50
    #endif
    #ifndef LASER_SAFETY_TIMEOUT_MS
      #define LASER_SAFETY_TIMEOUT_MS 1000
    #endif
    #if ENABLED(I2C_AMMETER)
      #ifndef I2C_AMMETER_IMAX
        #define I2C_AMMETER_IMAX 0.1
      #endif
      #ifndef I2C_AMMETER_SHUNT_RESISTOR
        #define I2C_AMMETER_SHUNT_RESISTOR 0.1
      #endif
    #endif
    #if ENABLED(LASER_COOLANT_FLOW_METER)
      #ifndef FLOWMETER_PIN
        #define FLOWMETER_PIN 20
      #endif
      #ifndef FLOWMETER_PPL
        #define FLOWMETER_PPL 5880
      #endif
      #ifndef FLOWMETER_INTERVAL
        #define FLOWMETER_INTERVAL 1000
      #endif
      #ifndef FLOWMETER_SAFETY
        #define FLOWMETER_SAFETY
      #endif
      #if ENABLED(FLOWMETER_SAFETY)
        #ifndef FLOWMETER_MIN_LITERS_PER_MINUTE
          #define FLOWMETER_MIN_LITERS_PER_MINUTE 1.5
        #endif
      #endif
    #endif
  #endif
#endif
#if ENABLED(COOLANT_CONTROL)
  #ifndef COOLANT_MIST
    #define COOLANT_MIST
  #endif
  #ifndef COOLANT_FLOOD
    #define COOLANT_FLOOD
  #endif
  #ifndef COOLANT_MIST_INVERT
    #define COOLANT_MIST_INVERT false
  #endif
  #ifndef COOLANT_FLOOD_INVERT
    #define COOLANT_FLOOD_INVERT false
  #endif
#endif
#if ENABLED(FILAMENT_WIDTH_SENSOR)
  #ifndef FILAMENT_SENSOR_EXTRUDER_NUM
    #define FILAMENT_SENSOR_EXTRUDER_NUM 0
  #endif
  #ifndef MEASUREMENT_DELAY_CM
    #define MEASUREMENT_DELAY_CM 14
  #endif
  #ifndef FILWIDTH_ERROR_MARGIN
    #define FILWIDTH_ERROR_MARGIN 1.0
  #endif
  #ifndef MAX_MEASUREMENT_DELAY
    #define MAX_MEASUREMENT_DELAY 20
  #endif
  #ifndef DEFAULT_MEASURED_FILAMENT_DIA
    #define DEFAULT_MEASURED_FILAMENT_DIA DEFAULT_NOMINAL_FILAMENT_DIA
  #endif
#endif
#if ENABLED(POWER_MONITOR_CURRENT)
  #ifndef POWER_MONITOR_VOLTS_PER_AMP
    #define POWER_MONITOR_VOLTS_PER_AMP 0.05000
  #endif
  #ifndef POWER_MONITOR_CURRENT_OFFSET
    #define POWER_MONITOR_CURRENT_OFFSET 0
  #endif
  #ifndef POWER_MONITOR_FIXED_VOLTAGE
    #define POWER_MONITOR_FIXED_VOLTAGE 13.6
  #endif
#endif
#if ENABLED(POWER_MONITOR_VOLTAGE)
  #ifndef POWER_MONITOR_VOLTS_PER_VOLT
    #define POWER_MONITOR_VOLTS_PER_VOLT 0.077933
  #endif
  #ifndef POWER_MONITOR_VOLTAGE_OFFSET
    #define POWER_MONITOR_VOLTAGE_OFFSET 0
  #endif
#endif
#if DISABLED(NO_VOLUMETRICS)
  #if ENABLED(VOLUMETRIC_EXTRUDER_LIMIT)
    #ifndef DEFAULT_VOLUMETRIC_EXTRUDER_LIMIT
      #define DEFAULT_VOLUMETRIC_EXTRUDER_LIMIT 0.00
    #endif
    #ifndef VOLUMETRIC_EXTRUDER_LIMIT_MAX
      #define VOLUMETRIC_EXTRUDER_LIMIT_MAX 20
    #endif
  #endif
#endif
#ifndef AUTO_REPORT_TEMPERATURES
  #define AUTO_REPORT_TEMPERATURES
#endif
#ifndef CAPABILITIES_REPORT
  #define CAPABILITIES_REPORT
#endif
#if ENABLED(CAPABILITIES_REPORT)
  #ifndef EXTENDED_CAPABILITIES_REPORT
    #define EXTENDED_CAPABILITIES_REPORT
  #endif
#endif
#ifndef FASTER_GCODE_PARSER
  #define FASTER_GCODE_PARSER
#endif
#ifndef DEBUG_FLAGS_GCODE
  #define DEBUG_FLAGS_GCODE
#endif
#if ENABLED(GCODE_MACROS)
  #ifndef GCODE_MACROS_SLOTS
    #define GCODE_MACROS_SLOTS 5
  #endif
  #ifndef GCODE_MACROS_SLOT_SIZE
    #define GCODE_MACROS_SLOT_SIZE 50
  #endif
#endif
#if ENABLED(CUSTOM_MENU_MAIN)
  #ifndef CUSTOM_MENU_MAIN_SCRIPT_DONE
    #define CUSTOM_MENU_MAIN_SCRIPT_DONE "M117 User Script Done"
  #endif
  #ifndef CUSTOM_MENU_MAIN_SCRIPT_AUDIBLE_FEEDBACK
    #define CUSTOM_MENU_MAIN_SCRIPT_AUDIBLE_FEEDBACK
  #endif
  #ifndef CUSTOM_MENU_MAIN_ONLY_IDLE
    #define CUSTOM_MENU_MAIN_ONLY_IDLE
  #endif
  #ifndef MAIN_MENU_ITEM_1_DESC
    #define MAIN_MENU_ITEM_1_DESC "Home & UBL Info"
  #endif
  #ifndef MAIN_MENU_ITEM_1_GCODE
    #define MAIN_MENU_ITEM_1_GCODE "G28\nG29 W"
  #endif
  #ifndef MAIN_MENU_ITEM_2_DESC
    #define MAIN_MENU_ITEM_2_DESC "Preheat for " PREHEAT_1_LABEL
  #endif
  #ifndef MAIN_MENU_ITEM_2_GCODE
    #define MAIN_MENU_ITEM_2_GCODE "M140 S" STRINGIFY(PREHEAT_1_TEMP_BED) "\nM104 S" STRINGIFY(PREHEAT_1_TEMP_HOTEND)
  #endif
#endif
#if ENABLED(CUSTOM_MENU_CONFIG)
  #ifndef CUSTOM_MENU_CONFIG_SCRIPT_DONE
    #define CUSTOM_MENU_CONFIG_SCRIPT_DONE "M117 Wireless Script Done"
  #endif
  #ifndef CUSTOM_MENU_CONFIG_SCRIPT_AUDIBLE_FEEDBACK
    #define CUSTOM_MENU_CONFIG_SCRIPT_AUDIBLE_FEEDBACK
  #endif
  #ifndef CUSTOM_MENU_CONFIG_ONLY_IDLE
    #define CUSTOM_MENU_CONFIG_ONLY_IDLE
  #endif
  #ifndef CONFIG_MENU_ITEM_1_DESC
    #define CONFIG_MENU_ITEM_1_DESC "Wifi ON"
  #endif
  #ifndef CONFIG_MENU_ITEM_1_GCODE
    #define CONFIG_MENU_ITEM_1_GCODE "M118 [ESP110] WIFI-STA pwd=12345678"
  #endif
  #ifndef CONFIG_MENU_ITEM_2_DESC
    #define CONFIG_MENU_ITEM_2_DESC "Bluetooth ON"
  #endif
  #ifndef CONFIG_MENU_ITEM_2_GCODE
    #define CONFIG_MENU_ITEM_2_GCODE "M118 [ESP110] BT pwd=12345678"
  #endif
#endif
#if ENABLED(CUSTOM_USER_BUTTONS)
  #if PIN_EXISTS(BUTTON1)
    #ifndef BUTTON1_HIT_STATE
      #define BUTTON1_HIT_STATE LOW
    #endif
    #ifndef BUTTON1_WHEN_PRINTING
      #define BUTTON1_WHEN_PRINTING false
    #endif
    #ifndef BUTTON1_GCODE
      #define BUTTON1_GCODE "G28"
    #endif
    #ifndef BUTTON1_DESC
      #define BUTTON1_DESC "Homing"
    #endif
  #endif
  #if PIN_EXISTS(BUTTON2)
    #ifndef BUTTON2_HIT_STATE
      #define BUTTON2_HIT_STATE LOW
    #endif
    #ifndef BUTTON2_WHEN_PRINTING
      #define BUTTON2_WHEN_PRINTING false
    #endif
    #ifndef BUTTON2_GCODE
      #define BUTTON2_GCODE "M140 S" STRINGIFY(PREHEAT_1_TEMP_BED) "\nM104 S" STRINGIFY(PREHEAT_1_TEMP_HOTEND)
    #endif
    #ifndef BUTTON2_DESC
      #define BUTTON2_DESC "Preheat for " PREHEAT_1_LABEL
    #endif
  #endif
  #if PIN_EXISTS(BUTTON3)
    #ifndef BUTTON3_HIT_STATE
      #define BUTTON3_HIT_STATE LOW
    #endif
    #ifndef BUTTON3_WHEN_PRINTING
      #define BUTTON3_WHEN_PRINTING false
    #endif
    #ifndef BUTTON3_GCODE
      #define BUTTON3_GCODE "M140 S" STRINGIFY(PREHEAT_2_TEMP_BED) "\nM104 S" STRINGIFY(PREHEAT_2_TEMP_HOTEND)
    #endif
    #ifndef BUTTON3_DESC
      #define BUTTON3_DESC "Preheat for " PREHEAT_2_LABEL
    #endif
  #endif
#endif
#if ENABLED(CANCEL_OBJECTS)
  #ifndef CANCEL_OBJECTS_REPORTING
    #define CANCEL_OBJECTS_REPORTING
  #endif
#endif
#if ENABLED(I2C_POSITION_ENCODERS)
  #ifndef I2CPE_ENCODER_CNT
    #define I2CPE_ENCODER_CNT 1
  #endif
  #ifndef I2CPE_ENC_1_ADDR
    #define I2CPE_ENC_1_ADDR I2CPE_PRESET_ADDR_X
  #endif
  #ifndef I2CPE_ENC_1_AXIS
    #define I2CPE_ENC_1_AXIS X_AXIS
  #endif
  #ifndef I2CPE_ENC_1_TYPE
    #define I2CPE_ENC_1_TYPE I2CPE_ENC_TYPE_LINEAR
  #endif
  #ifndef I2CPE_ENC_1_TICKS_UNIT
    #define I2CPE_ENC_1_TICKS_UNIT 2048
  #endif
  #ifndef I2CPE_ENC_1_EC_METHOD
    #define I2CPE_ENC_1_EC_METHOD I2CPE_ECM_MICROSTEP
  #endif
  #ifndef I2CPE_ENC_1_EC_THRESH
    #define I2CPE_ENC_1_EC_THRESH 0.10
  #endif
  #ifndef I2CPE_ENC_2_ADDR
    #define I2CPE_ENC_2_ADDR I2CPE_PRESET_ADDR_Y
  #endif
  #ifndef I2CPE_ENC_2_AXIS
    #define I2CPE_ENC_2_AXIS Y_AXIS
  #endif
  #ifndef I2CPE_ENC_2_TYPE
    #define I2CPE_ENC_2_TYPE I2CPE_ENC_TYPE_LINEAR
  #endif
  #ifndef I2CPE_ENC_2_TICKS_UNIT
    #define I2CPE_ENC_2_TICKS_UNIT 2048
  #endif
  #ifndef I2CPE_ENC_2_EC_METHOD
    #define I2CPE_ENC_2_EC_METHOD I2CPE_ECM_MICROSTEP
  #endif
  #ifndef I2CPE_ENC_2_EC_THRESH
    #define I2CPE_ENC_2_EC_THRESH 0.10
  #endif
  #ifndef I2CPE_ENC_3_ADDR
    #define I2CPE_ENC_3_ADDR I2CPE_PRESET_ADDR_Z
  #endif
  #ifndef I2CPE_ENC_3_AXIS
    #define I2CPE_ENC_3_AXIS Z_AXIS
  #endif
  #ifndef I2CPE_ENC_4_ADDR
    #define I2CPE_ENC_4_ADDR I2CPE_PRESET_ADDR_E
  #endif
  #ifndef I2CPE_ENC_4_AXIS
    #define I2CPE_ENC_4_AXIS E_AXIS
  #endif
  #ifndef I2CPE_ENC_5_ADDR
    #define I2CPE_ENC_5_ADDR 34
  #endif
  #ifndef I2CPE_ENC_5_AXIS
    #define I2CPE_ENC_5_AXIS E_AXIS
  #endif
  #ifndef I2CPE_DEF_TYPE
    #define I2CPE_DEF_TYPE I2CPE_ENC_TYPE_LINEAR
  #endif
  #ifndef I2CPE_DEF_ENC_TICKS_UNIT
    #define I2CPE_DEF_ENC_TICKS_UNIT 2048
  #endif
  #ifndef I2CPE_DEF_TICKS_REV
    #define I2CPE_DEF_TICKS_REV (16 * 200)
  #endif
  #ifndef I2CPE_DEF_EC_METHOD
    #define I2CPE_DEF_EC_METHOD I2CPE_ECM_NONE
  #endif
  #ifndef I2CPE_DEF_EC_THRESH
    #define I2CPE_DEF_EC_THRESH 0.1
  #endif
  #ifndef I2CPE_TIME_TRUSTED
    #define I2CPE_TIME_TRUSTED 10000
  #endif
  #ifndef I2CPE_MIN_UPD_TIME_MS
    #define I2CPE_MIN_UPD_TIME_MS 4
  #endif
  #ifndef I2CPE_ERR_ROLLING_AVERAGE
    #define I2CPE_ERR_ROLLING_AVERAGE
  #endif
#endif
#if ENABLED(JOYSTICK)
  #ifndef JOY_X_PIN
    #define JOY_X_PIN 5
  #endif
  #ifndef JOY_Y_PIN
    #define JOY_Y_PIN 10
  #endif
  #ifndef JOY_Z_PIN
    #define JOY_Z_PIN 12
  #endif
  #ifndef JOY_EN_PIN
    #define JOY_EN_PIN 44
  #endif
  #ifndef JOY_X_LIMITS
    #define JOY_X_LIMITS { 5600, 8190-100, 8190+100, 10800 }
  #endif
  #ifndef JOY_Y_LIMITS
    #define JOY_Y_LIMITS { 5600, 8250-100, 8250+100, 11000 }
  #endif
  #ifndef JOY_Z_LIMITS
    #define JOY_Z_LIMITS { 4800, 8080-100, 8080+100, 11550 }
  #endif
#endif
#if ENABLED(MECHANICAL_GANTRY_CALIBRATION)
  #ifndef GANTRY_CALIBRATION_CURRENT
    #define GANTRY_CALIBRATION_CURRENT 600
  #endif
  #ifndef GANTRY_CALIBRATION_EXTRA_HEIGHT
    #define GANTRY_CALIBRATION_EXTRA_HEIGHT 15
  #endif
  #ifndef GANTRY_CALIBRATION_FEEDRATE
    #define GANTRY_CALIBRATION_FEEDRATE 500
  #endif
  #ifndef GANTRY_CALIBRATION_COMMANDS_POST
    #define GANTRY_CALIBRATION_COMMANDS_POST "G28"
  #endif
#endif
#if ENABLED(FREEZE_FEATURE)
  #ifndef FREEZE_STATE
    #define FREEZE_STATE LOW
  #endif
#endif
#if ENABLED(MAX7219_DEBUG)
  #ifndef MAX7219_CLK_PIN
    #define MAX7219_CLK_PIN 64
  #endif
  #ifndef MAX7219_DIN_PIN
    #define MAX7219_DIN_PIN 57
  #endif
  #ifndef MAX7219_LOAD_PIN
    #define MAX7219_LOAD_PIN 44
  #endif
  #ifndef MAX7219_INIT_TEST
    #define MAX7219_INIT_TEST 2
  #endif
  #ifndef MAX7219_NUMBER_UNITS
    #define MAX7219_NUMBER_UNITS 1
  #endif
  #ifndef MAX7219_ROTATE
    #define MAX7219_ROTATE 0
  #endif
  #ifndef MAX7219_DEBUG_PRINTER_ALIVE
    #define MAX7219_DEBUG_PRINTER_ALIVE
  #endif
  #ifndef MAX7219_DEBUG_PLANNER_HEAD
    #define MAX7219_DEBUG_PLANNER_HEAD 2
  #endif
  #ifndef MAX7219_DEBUG_PLANNER_TAIL
    #define MAX7219_DEBUG_PLANNER_TAIL 4
  #endif
  #ifndef MAX7219_DEBUG_PLANNER_QUEUE
    #define MAX7219_DEBUG_PLANNER_QUEUE 0
  #endif
  #ifndef MAX7219_DEBUG_PROFILE
    #define MAX7219_DEBUG_PROFILE 6
  #endif
#endif
#if HAS_ETHERNET
  #ifndef MAC_ADDRESS
    #define MAC_ADDRESS { 0xDE, 0xAD, 0xBE, 0xEF, 0xF0, 0x0D }
  #endif
#endif
#if !(HAS_PRUSA_MMU1) && (HAS_PRUSA_MMU2)

  #ifndef MMU2_SERIAL_PORT
    #define MMU2_SERIAL_PORT 2
  #endif
  #ifndef MMU2_FILAMENT_RUNOUT_SCRIPT
    #define MMU2_FILAMENT_RUNOUT_SCRIPT "M600"
  #endif
  #ifndef MMU2_FILAMENTCHANGE_EJECT_FEED
    #define MMU2_FILAMENTCHANGE_EJECT_FEED 80.0
  #endif
  #ifndef MMU2_LOAD_TO_NOZZLE_SEQUENCE
    #define MMU2_LOAD_TO_NOZZLE_SEQUENCE {  7.2, 1145 },  { 14.4,  871 },  { 36.0, 1393 },  { 14.4,  871 },  { 50.0,  198 }
  #endif
  #ifndef MMU2_RAMMING_SEQUENCE
    #define MMU2_RAMMING_SEQUENCE {   1.0, 1000 },  {   1.0, 1500 },  {   2.0, 2000 },  {   1.5, 3000 },  {   2.5, 4000 },  { -15.0, 5000 },  { -14.0, 1200 },  {  -6.0,  600 },  {  10.0,  700 },  { -10.0,  400 },  { -50.0, 2000 }
  #endif
  #if HAS_PRUSA_MMU2S
    #ifndef MMU2_C0_RETRY
      #define MMU2_C0_RETRY 5
    #endif
    #ifndef MMU2_CAN_LOAD_FEEDRATE
      #define MMU2_CAN_LOAD_FEEDRATE 800
    #endif
    #ifndef MMU2_CAN_LOAD_SEQUENCE
      #define MMU2_CAN_LOAD_SEQUENCE {  0.1, MMU2_CAN_LOAD_FEEDRATE },  {  60.0, MMU2_CAN_LOAD_FEEDRATE },  { -52.0, MMU2_CAN_LOAD_FEEDRATE }
    #endif
    #ifndef MMU2_CAN_LOAD_RETRACT
      #define MMU2_CAN_LOAD_RETRACT 6.0
    #endif
    #ifndef MMU2_CAN_LOAD_DEVIATION
      #define MMU2_CAN_LOAD_DEVIATION 0.8
    #endif
    #ifndef MMU2_CAN_LOAD_INCREMENT
      #define MMU2_CAN_LOAD_INCREMENT 0.2
    #endif
    #ifndef MMU2_CAN_LOAD_INCREMENT_SEQUENCE
      #define MMU2_CAN_LOAD_INCREMENT_SEQUENCE { -MMU2_CAN_LOAD_INCREMENT, MMU2_CAN_LOAD_FEEDRATE }
    #endif
  #else
    #if ENABLED(MMU_EXTRUDER_SENSOR)
      #ifndef MMU_LOADING_ATTEMPTS_NR
        #define MMU_LOADING_ATTEMPTS_NR 5
      #endif
    #endif
  #endif
#endif
#if ENABLED(PRINTCOUNTER)
  #ifndef SERVICE_WARNING_BUZZES
    #define SERVICE_WARNING_BUZZES 3
  #endif
#endif
