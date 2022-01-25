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

/**
 * Conditionals_adv.h
 * Defines that depend on advanced configuration.
 */

#ifndef AXIS_RELATIVE_MODES
  #define AXIS_RELATIVE_MODES {}
#endif

#ifdef SWITCHING_NOZZLE_E1_SERVO_NR
  #define SWITCHING_NOZZLE_TWO_SERVOS 1
#endif

// Determine NUM_SERVOS if none was supplied
#ifndef NUM_SERVOS
  #define NUM_SERVOS 0
  #if ANY(HAS_Z_SERVO_PROBE, CHAMBER_VENT, SWITCHING_TOOLHEAD, SWITCHING_EXTRUDER, SWITCHING_NOZZLE, SPINDLE_SERVO)
    #if NUM_SERVOS <= Z_PROBE_SERVO_NR
      #undef NUM_SERVOS
      #define NUM_SERVOS (Z_PROBE_SERVO_NR + 1)
    #endif
    #if NUM_SERVOS <= CHAMBER_VENT_SERVO_NR
      #undef NUM_SERVOS
      #define NUM_SERVOS (CHAMBER_VENT_SERVO_NR + 1)
    #endif
    #if NUM_SERVOS <= SWITCHING_TOOLHEAD_SERVO_NR
      #undef NUM_SERVOS
      #define NUM_SERVOS (SWITCHING_TOOLHEAD_SERVO_NR + 1)
    #endif
    #if NUM_SERVOS <= SWITCHING_NOZZLE_SERVO_NR
      #undef NUM_SERVOS
      #define NUM_SERVOS (SWITCHING_NOZZLE_SERVO_NR + 1)
    #endif
    #if NUM_SERVOS <= SWITCHING_NOZZLE_E1_SERVO_NR
      #undef NUM_SERVOS
      #define NUM_SERVOS (SWITCHING_NOZZLE_E1_SERVO_NR + 1)
    #endif
    #if NUM_SERVOS <= SWITCHING_EXTRUDER_SERVO_NR
      #undef NUM_SERVOS
      #define NUM_SERVOS (SWITCHING_EXTRUDER_SERVO_NR + 1)
    #endif
    #if NUM_SERVOS <= SWITCHING_EXTRUDER_E23_SERVO_NR
      #undef NUM_SERVOS
      #define NUM_SERVOS (SWITCHING_EXTRUDER_E23_SERVO_NR + 1)
    #endif
    #if NUM_SERVOS <= SPINDLE_SERVO_NR
      #undef NUM_SERVOS
      #define NUM_SERVOS (SPINDLE_SERVO_NR + 1)
    #endif
  #endif
#endif

// Convenience override for a BLTouch alone
#if ENABLED(BLTOUCH) && NUM_SERVOS == 1
  #undef SERVO_DELAY
  #define SERVO_DELAY { 50 }
#endif

#if !HAS_EXTRUDERS
  #define NO_VOLUMETRICS
  #undef TEMP_SENSOR_0
  #undef TEMP_SENSOR_1
  #undef TEMP_SENSOR_2
  #undef TEMP_SENSOR_3
  #undef TEMP_SENSOR_4
  #undef TEMP_SENSOR_5
  #undef TEMP_SENSOR_6
  #undef TEMP_SENSOR_7
  #undef FWRETRACT
  #undef PIDTEMP
  #undef AUTOTEMP
  #undef PID_EXTRUSION_SCALING
  #undef LIN_ADVANCE
  #undef FILAMENT_RUNOUT_SENSOR
  #undef ADVANCED_PAUSE_FEATURE
  #undef FILAMENT_RUNOUT_DISTANCE_MM
  #undef FILAMENT_LOAD_UNLOAD_GCODES
  #undef DISABLE_INACTIVE_EXTRUDER
  #undef FILAMENT_LOAD_UNLOAD_GCODES
  #undef EXTRUDER_RUNOUT_PREVENT
  #undef PREVENT_COLD_EXTRUSION
  #undef PREVENT_LENGTHY_EXTRUDE
  #undef THERMAL_PROTECTION_HOTENDS
  #undef THERMAL_PROTECTION_PERIOD
  #undef WATCH_TEMP_PERIOD
  #undef SHOW_TEMP_ADC_VALUES
  #undef LCD_SHOW_E_TOTAL
  #undef MANUAL_E_MOVES_RELATIVE
  #undef STEALTHCHOP_E
#endif

/**
 * Temperature Sensors; define what sensor(s) we have.
 */

// Temperature sensor IDs
#define HID_REDUNDANT -6
#define HID_BOARD     -5
#define HID_COOLER    -4
#define HID_PROBE     -3
#define HID_CHAMBER   -2
#define HID_BED       -1
#define HID_E0         0
#define HID_E1         1
#define HID_E2         2
#define HID_E3         3
#define HID_E4         4
#define HID_E5         5
#define HID_E6         6
#define HID_E7         7

#define _SENSOR_IS(I,N) || (TEMP_SENSOR_##N == I)
#define _E_SENSOR_IS(I,N) _SENSOR_IS(N,I)
#define ANY_THERMISTOR_IS(N) (0 REPEAT2(HOTENDS, _E_SENSOR_IS, N) \
  _SENSOR_IS(N,BED) _SENSOR_IS(N,PROBE) _SENSOR_IS(N,CHAMBER) \
  _SENSOR_IS(N,COOLER) _SENSOR_IS(N,BOARD) _SENSOR_IS(N,REDUNDANT) )

#if ANY_THERMISTOR_IS(1000)
  #define HAS_USER_THERMISTORS 1
#endif

#if TEMP_SENSOR_REDUNDANT
  #define _HEATER_ID(M) HID_##M
  #define HEATER_ID(M)  _HEATER_ID(M)
  #define REDUNDANT_TEMP_MATCH(M,N) (HEATER_ID(TEMP_SENSOR_REDUNDANT_##M) == _HEATER_ID(N))
#else
  #define REDUNDANT_TEMP_MATCH(...) 0
#endif

#if TEMP_SENSOR_0 == -5 || TEMP_SENSOR_0 == -3 || TEMP_SENSOR_0 == -2
  #define TEMP_SENSOR_0_IS_MAX_TC 1
  #if TEMP_SENSOR_0 == -5
    #define TEMP_SENSOR_0_IS_MAX31865 1
    #define TEMP_SENSOR_0_MAX_TC_TMIN    0
    #define TEMP_SENSOR_0_MAX_TC_TMAX 1024
    #ifndef MAX31865_SENSOR_WIRES_0
      #define MAX31865_SENSOR_WIRES_0 2
    #endif
    #ifndef MAX31865_WIRE_OHMS_0
      #define MAX31865_WIRE_OHMS_0 0.0f
    #endif
  #elif TEMP_SENSOR_0 == -3
    #define TEMP_SENSOR_0_IS_MAX31855 1
    #define TEMP_SENSOR_0_MAX_TC_TMIN -270
    #define TEMP_SENSOR_0_MAX_TC_TMAX 1800
  #elif TEMP_SENSOR_0 == -2
    #define TEMP_SENSOR_0_IS_MAX6675 1
    #define TEMP_SENSOR_0_MAX_TC_TMIN    0
    #define TEMP_SENSOR_0_MAX_TC_TMAX 1024
  #endif
#elif TEMP_SENSOR_0 == -4
  #define TEMP_SENSOR_0_IS_AD8495 1
#elif TEMP_SENSOR_0 == -1
  #define TEMP_SENSOR_0_IS_AD595 1
#elif TEMP_SENSOR_0 > 0
  #define TEMP_SENSOR_0_IS_THERMISTOR 1
  #if TEMP_SENSOR_0 == 1000
    #define TEMP_SENSOR_0_IS_CUSTOM 1
  #elif TEMP_SENSOR_0 == 998 || TEMP_SENSOR_0 == 999
    #define TEMP_SENSOR_0_IS_DUMMY 1
  #endif
#else
  #undef HEATER_0_MINTEMP
  #undef HEATER_0_MAXTEMP
#endif

#if TEMP_SENSOR_1 == -5 || TEMP_SENSOR_1 == -3 || TEMP_SENSOR_1 == -2
  #define TEMP_SENSOR_1_IS_MAX_TC 1
  #if TEMP_SENSOR_1 == -5
    #define TEMP_SENSOR_1_IS_MAX31865 1
    #define TEMP_SENSOR_1_MAX_TC_TMIN    0
    #define TEMP_SENSOR_1_MAX_TC_TMAX 1024
    #ifndef MAX31865_SENSOR_WIRES_1
      #define MAX31865_SENSOR_WIRES_1 2
    #endif
    #ifndef MAX31865_WIRE_OHMS_1
      #define MAX31865_WIRE_OHMS_1 0.0f
    #endif
  #elif TEMP_SENSOR_1 == -3
    #define TEMP_SENSOR_1_IS_MAX31855 1
    #define TEMP_SENSOR_1_MAX_TC_TMIN -270
    #define TEMP_SENSOR_1_MAX_TC_TMAX 1800
  #elif TEMP_SENSOR_1 == -2
    #define TEMP_SENSOR_1_IS_MAX6675 1
    #define TEMP_SENSOR_1_MAX_TC_TMIN    0
    #define TEMP_SENSOR_1_MAX_TC_TMAX 1024
  #endif

  #if TEMP_SENSOR_1 != TEMP_SENSOR_0
    #if   TEMP_SENSOR_1 == -5
      #error "If MAX31865 Thermocouple (-5) is used for TEMP_SENSOR_1 then TEMP_SENSOR_0 must match."
    #elif TEMP_SENSOR_1 == -3
      #error "If MAX31855 Thermocouple (-3) is used for TEMP_SENSOR_1 then TEMP_SENSOR_0 must match."
    #elif TEMP_SENSOR_1 == -2
      #error "If MAX6675 Thermocouple (-2) is used for TEMP_SENSOR_1 then TEMP_SENSOR_0 must match."
    #endif
  #endif
#elif TEMP_SENSOR_1 == -4
  #define TEMP_SENSOR_1_IS_AD8495 1
#elif TEMP_SENSOR_1 == -1
  #define TEMP_SENSOR_1_IS_AD595 1
#elif TEMP_SENSOR_1 > 0
  #define TEMP_SENSOR_1_IS_THERMISTOR 1
  #if TEMP_SENSOR_1 == 1000
    #define TEMP_SENSOR_1_IS_CUSTOM 1
  #elif TEMP_SENSOR_1 == 998 || TEMP_SENSOR_1 == 999
    #define TEMP_SENSOR_1_IS_DUMMY 1
  #endif
#else
  #undef HEATER_1_MINTEMP
  #undef HEATER_1_MAXTEMP
#endif

#if TEMP_SENSOR_REDUNDANT == -5 || TEMP_SENSOR_REDUNDANT == -3 || TEMP_SENSOR_REDUNDANT == -2
  #define TEMP_SENSOR_REDUNDANT_IS_MAX_TC 1

  #if TEMP_SENSOR_REDUNDANT == -5
    #if !REDUNDANT_TEMP_MATCH(SOURCE, E0) && !REDUNDANT_TEMP_MATCH(SOURCE, E1)
      #error "MAX31865 Thermocouples (-5) not supported for TEMP_SENSOR_REDUNDANT_SOURCE other than TEMP_SENSOR_0/TEMP_SENSOR_1 (0/1)."
    #endif

    #define TEMP_SENSOR_REDUNDANT_IS_MAX31865    1
    #define TEMP_SENSOR_REDUNDANT_MAX_TC_TMIN    0
    #define TEMP_SENSOR_REDUNDANT_MAX_TC_TMAX 1024
  #elif TEMP_SENSOR_REDUNDANT == -3
    #if !REDUNDANT_TEMP_MATCH(SOURCE, E0) && !REDUNDANT_TEMP_MATCH(SOURCE, E1)
      #error "MAX31855 Thermocouples (-3) not supported for TEMP_SENSOR_REDUNDANT_SOURCE other than TEMP_SENSOR_0/TEMP_SENSOR_1 (0/1)."
    #endif

    #define TEMP_SENSOR_REDUNDANT_IS_MAX31855    1
    #define TEMP_SENSOR_REDUNDANT_MAX_TC_TMIN -270
    #define TEMP_SENSOR_REDUNDANT_MAX_TC_TMAX 1800
  #elif TEMP_SENSOR_REDUNDANT == -2
    #if !REDUNDANT_TEMP_MATCH(SOURCE, E0) && !REDUNDANT_TEMP_MATCH(SOURCE, E1)
      #error "MAX6675 Thermocouples (-2) not supported for TEMP_SENSOR_REDUNDANT_SOURCE other than TEMP_SENSOR_0/TEMP_SENSOR_1 (0/1)."
    #endif

    #define TEMP_SENSOR_REDUNDANT_IS_MAX6675     1
    #define TEMP_SENSOR_REDUNDANT_MAX_TC_TMIN    0
    #define TEMP_SENSOR_REDUNDANT_MAX_TC_TMAX 1024
  #endif

  // mimic setting up the source TEMP_SENSOR
  #if REDUNDANT_TEMP_MATCH(SOURCE, E0)
    #define TEMP_SENSOR_0_MAX_TC_TMIN TEMP_SENSOR_REDUNDANT_MAX_TC_TMIN
    #define TEMP_SENSOR_0_MAX_TC_TMAX TEMP_SENSOR_REDUNDANT_MAX_TC_TMAX
    #ifndef MAX31865_SENSOR_WIRES_0
      #define MAX31865_SENSOR_WIRES_0 2
    #endif
  #elif REDUNDANT_TEMP_MATCH(SOURCE, E1)
    #define TEMP_SENSOR_1_MAX_TC_TMIN TEMP_SENSOR_REDUNDANT_MAX_TC_TMIN
    #define TEMP_SENSOR_1_MAX_TC_TMAX TEMP_SENSOR_REDUNDANT_MAX_TC_TMAX
    #ifndef MAX31865_SENSOR_WIRES_1
      #define MAX31865_SENSOR_WIRES_1 2
    #endif
  #endif

  #if (TEMP_SENSOR_0_IS_MAX_TC && TEMP_SENSOR_REDUNDANT != TEMP_SENSOR_0) || (TEMP_SENSOR_1_IS_MAX_TC && TEMP_SENSOR_REDUNDANT != TEMP_SENSOR_1)
    #if   TEMP_SENSOR_REDUNDANT == -5
      #error "If MAX31865 Thermocouple (-5) is used for TEMP_SENSOR_0/TEMP_SENSOR_1 then TEMP_SENSOR_REDUNDANT must match."
    #elif TEMP_SENSOR_REDUNDANT == -3
      #error "If MAX31855 Thermocouple (-3) is used for TEMP_SENSOR_0/TEMP_SENSOR_1 then TEMP_SENSOR_REDUNDANT must match."
    #elif TEMP_SENSOR_REDUNDANT == -2
      #error "If MAX6675 Thermocouple (-2) is used for TEMP_SENSOR_0/TEMP_SENSOR_1 then TEMP_SENSOR_REDUNDANT must match."
    #endif
  #endif
#elif TEMP_SENSOR_REDUNDANT == -4
  #define TEMP_SENSOR_REDUNDANT_IS_AD8495 1
#elif TEMP_SENSOR_REDUNDANT == -1
  #define TEMP_SENSOR_REDUNDANT_IS_AD595 1
#elif TEMP_SENSOR_REDUNDANT > 0
  #define TEMP_SENSOR_REDUNDANT_IS_THERMISTOR 1
  #if TEMP_SENSOR_REDUNDANT == 1000
    #define TEMP_SENSOR_REDUNDANT_IS_CUSTOM 1
  #elif TEMP_SENSOR_REDUNDANT == 998 || TEMP_SENSOR_REDUNDANT == 999
    #error "Dummy sensors are not supported for TEMP_SENSOR_REDUNDANT."
  #endif
#endif

#if TEMP_SENSOR_0_IS_MAX_TC || TEMP_SENSOR_1_IS_MAX_TC || TEMP_SENSOR_REDUNDANT_IS_MAX_TC
  #define HAS_MAX_TC 1
#endif
#if TEMP_SENSOR_0_IS_MAX6675 || TEMP_SENSOR_1_IS_MAX6675 || TEMP_SENSOR_REDUNDANT_IS_MAX6675
  #define HAS_MAX6675 1
#endif
#if TEMP_SENSOR_0_IS_MAX31855 || TEMP_SENSOR_1_IS_MAX31855 || TEMP_SENSOR_REDUNDANT_IS_MAX31855
  #define HAS_MAX31855 1
#endif
#if TEMP_SENSOR_0_IS_MAX31865 || TEMP_SENSOR_1_IS_MAX31865 || TEMP_SENSOR_REDUNDANT_IS_MAX31865
  #define HAS_MAX31865 1
#endif

#if TEMP_SENSOR_2 == -4
  #define TEMP_SENSOR_2_IS_AD8495 1
#elif TEMP_SENSOR_2 == -3
  #error "MAX31855 Thermocouples (-3) not supported for TEMP_SENSOR_2."
#elif TEMP_SENSOR_2 == -2
  #error "MAX6675 Thermocouples (-2) not supported for TEMP_SENSOR_2."
#elif TEMP_SENSOR_2 == -1
  #define TEMP_SENSOR_2_IS_AD595 1
#elif TEMP_SENSOR_2 > 0
  #define TEMP_SENSOR_2_IS_THERMISTOR 1
  #if TEMP_SENSOR_2 == 1000
    #define TEMP_SENSOR_2_IS_CUSTOM 1
  #elif TEMP_SENSOR_2 == 998 || TEMP_SENSOR_2 == 999
    #define TEMP_SENSOR_2_IS_DUMMY 1
  #endif
#else
  #undef HEATER_2_MINTEMP
  #undef HEATER_2_MAXTEMP
#endif

#if TEMP_SENSOR_3 == -4
  #define TEMP_SENSOR_3_IS_AD8495 1
#elif TEMP_SENSOR_3 == -3
  #error "MAX31855 Thermocouples (-3) not supported for TEMP_SENSOR_3."
#elif TEMP_SENSOR_3 == -2
  #error "MAX6675 Thermocouples (-2) not supported for TEMP_SENSOR_3."
#elif TEMP_SENSOR_3 == -1
  #define TEMP_SENSOR_3_IS_AD595 1
#elif TEMP_SENSOR_3 > 0
  #define TEMP_SENSOR_3_IS_THERMISTOR 1
  #if TEMP_SENSOR_3 == 1000
    #define TEMP_SENSOR_3_IS_CUSTOM 1
  #elif TEMP_SENSOR_3 == 998 || TEMP_SENSOR_3 == 999
    #define TEMP_SENSOR_3_IS_DUMMY 1
  #endif
#else
  #undef HEATER_3_MINTEMP
  #undef HEATER_3_MAXTEMP
#endif

#if TEMP_SENSOR_4 == -4
  #define TEMP_SENSOR_4_IS_AD8495 1
#elif TEMP_SENSOR_4 == -3
  #error "MAX31855 Thermocouples (-3) not supported for TEMP_SENSOR_4."
#elif TEMP_SENSOR_4 == -2
  #error "MAX6675 Thermocouples (-2) not supported for TEMP_SENSOR_4."
#elif TEMP_SENSOR_4 == -1
  #define TEMP_SENSOR_4_IS_AD595 1
#elif TEMP_SENSOR_4 > 0
  #define TEMP_SENSOR_4_IS_THERMISTOR 1
  #if TEMP_SENSOR_4 == 1000
    #define TEMP_SENSOR_4_IS_CUSTOM 1
  #elif TEMP_SENSOR_4 == 998 || TEMP_SENSOR_4 == 999
    #define TEMP_SENSOR_4_IS_DUMMY 1
  #endif
#else
  #undef HEATER_4_MINTEMP
  #undef HEATER_4_MAXTEMP
#endif

#if TEMP_SENSOR_5 == -4
  #define TEMP_SENSOR_5_IS_AD8495 1
#elif TEMP_SENSOR_5 == -3
  #error "MAX31855 Thermocouples (-3) not supported for TEMP_SENSOR_5."
#elif TEMP_SENSOR_5 == -2
  #error "MAX6675 Thermocouples (-2) not supported for TEMP_SENSOR_5."
#elif TEMP_SENSOR_5 == -1
  #define TEMP_SENSOR_5_IS_AD595 1
#elif TEMP_SENSOR_5 > 0
  #define TEMP_SENSOR_5_IS_THERMISTOR 1
  #if TEMP_SENSOR_5 == 1000
    #define TEMP_SENSOR_5_IS_CUSTOM 1
  #elif TEMP_SENSOR_5 == 998 || TEMP_SENSOR_5 == 999
    #define TEMP_SENSOR_5_IS_DUMMY 1
  #endif
#else
  #undef HEATER_5_MINTEMP
  #undef HEATER_5_MAXTEMP
#endif

#if TEMP_SENSOR_6 == -4
  #define TEMP_SENSOR_6_IS_AD8495 1
#elif TEMP_SENSOR_6 == -3
  #error "MAX31855 Thermocouples (-3) not supported for TEMP_SENSOR_6."
#elif TEMP_SENSOR_6 == -2
  #error "MAX6675 Thermocouples (-2) not supported for TEMP_SENSOR_6."
#elif TEMP_SENSOR_6 == -1
  #define TEMP_SENSOR_6_IS_AD595 1
#elif TEMP_SENSOR_6 > 0
  #define TEMP_SENSOR_6_IS_THERMISTOR 1
  #if TEMP_SENSOR_6 == 1000
    #define TEMP_SENSOR_6_IS_CUSTOM 1
  #elif TEMP_SENSOR_6 == 998 || TEMP_SENSOR_6 == 999
    #define TEMP_SENSOR_6_IS_DUMMY 1
  #endif
#else
  #undef HEATER_6_MINTEMP
  #undef HEATER_6_MAXTEMP
#endif

#if TEMP_SENSOR_7 == -4
  #define TEMP_SENSOR_7_IS_AD8495 1
#elif TEMP_SENSOR_7 == -3
  #error "MAX31855 Thermocouples (-3) not supported for TEMP_SENSOR_7."
#elif TEMP_SENSOR_7 == -2
  #error "MAX7775 Thermocouples (-2) not supported for TEMP_SENSOR_7."
#elif TEMP_SENSOR_7 == -1
  #define TEMP_SENSOR_7_IS_AD595 1
#elif TEMP_SENSOR_7 > 0
  #define TEMP_SENSOR_7_IS_THERMISTOR 1
  #if TEMP_SENSOR_7 == 1000
    #define TEMP_SENSOR_7_IS_CUSTOM 1
  #elif TEMP_SENSOR_7 == 998 || TEMP_SENSOR_7 == 999
    #define TEMP_SENSOR_7_IS_DUMMY 1
  #endif
#else
  #undef HEATER_7_MINTEMP
  #undef HEATER_7_MAXTEMP
#endif

#if TEMP_SENSOR_BED == -4
  #define TEMP_SENSOR_BED_IS_AD8495 1
#elif TEMP_SENSOR_BED == -3
  #error "MAX31855 Thermocouples (-3) not supported for TEMP_SENSOR_BED."
#elif TEMP_SENSOR_BED == -2
  #error "MAX6675 Thermocouples (-2) not supported for TEMP_SENSOR_BED."
#elif TEMP_SENSOR_BED == -1
  #define TEMP_SENSOR_BED_IS_AD595 1
#elif TEMP_SENSOR_BED > 0
  #define TEMP_SENSOR_BED_IS_THERMISTOR 1
  #if TEMP_SENSOR_BED == 1000
    #define TEMP_SENSOR_BED_IS_CUSTOM 1
  #elif TEMP_SENSOR_BED == 998 || TEMP_SENSOR_BED == 999
    #define TEMP_SENSOR_BED_IS_DUMMY 1
  #endif
#else
  #undef THERMAL_PROTECTION_BED
  #undef THERMAL_PROTECTION_BED_PERIOD
  #undef BED_MINTEMP
  #undef BED_MAXTEMP
#endif

#if TEMP_SENSOR_CHAMBER == -4
  #define TEMP_SENSOR_CHAMBER_IS_AD8495 1
#elif TEMP_SENSOR_CHAMBER == -3
  #error "MAX31855 Thermocouples (-3) not supported for TEMP_SENSOR_CHAMBER."
#elif TEMP_SENSOR_CHAMBER == -2
  #error "MAX6675 Thermocouples (-2) not supported for TEMP_SENSOR_CHAMBER."
#elif TEMP_SENSOR_CHAMBER == -1
  #define TEMP_SENSOR_CHAMBER_IS_AD595 1
#elif TEMP_SENSOR_CHAMBER > 0
  #define TEMP_SENSOR_CHAMBER_IS_THERMISTOR 1
  #if TEMP_SENSOR_CHAMBER == 1000
    #define TEMP_SENSOR_CHAMBER_IS_CUSTOM 1
  #elif TEMP_SENSOR_CHAMBER == 998 || TEMP_SENSOR_CHAMBER == 999
    #define TEMP_SENSOR_CHAMBER_IS_DUMMY 1
  #endif
#else
  #undef THERMAL_PROTECTION_CHAMBER
  #undef CHAMBER_MINTEMP
  #undef CHAMBER_MAXTEMP
#endif

#if TEMP_SENSOR_COOLER == -4
  #define TEMP_SENSOR_COOLER_IS_AD8495 1
#elif TEMP_SENSOR_COOLER == -3
  #error "MAX31855 Thermocouples (-3) not supported for TEMP_SENSOR_COOLER."
#elif TEMP_SENSOR_COOLER == -2
  #error "MAX6675 Thermocouples (-2) not supported for TEMP_SENSOR_COOLER."
#elif TEMP_SENSOR_COOLER == -1
  #define TEMP_SENSOR_COOLER_IS_AD595 1
#elif TEMP_SENSOR_COOLER > 0
  #define TEMP_SENSOR_COOLER_IS_THERMISTOR 1
  #if TEMP_SENSOR_COOLER == 1000
    #define TEMP_SENSOR_COOLER_IS_CUSTOM 1
  #elif TEMP_SENSOR_COOLER == 998 || TEMP_SENSOR_COOLER == 999
    #define TEMP_SENSOR_COOLER_IS_DUMMY 1
  #endif
#else
  #undef THERMAL_PROTECTION_COOLER
  #undef COOLER_MINTEMP
  #undef COOLER_MAXTEMP
#endif

#if TEMP_SENSOR_PROBE == -4
  #define TEMP_SENSOR_PROBE_IS_AD8495 1
#elif TEMP_SENSOR_PROBE == -3
  #error "MAX31855 Thermocouples (-3) not supported for TEMP_SENSOR_PROBE."
#elif TEMP_SENSOR_PROBE == -2
  #error "MAX6675 Thermocouples (-2) not supported for TEMP_SENSOR_PROBE."
#elif TEMP_SENSOR_PROBE == -1
  #define TEMP_SENSOR_PROBE_IS_AD595 1
#elif TEMP_SENSOR_PROBE > 0
  #define TEMP_SENSOR_PROBE_IS_THERMISTOR 1
  #if TEMP_SENSOR_PROBE == 1000
    #define TEMP_SENSOR_PROBE_IS_CUSTOM 1
  #elif TEMP_SENSOR_PROBE == 998 || TEMP_SENSOR_PROBE == 999
    #define TEMP_SENSOR_PROBE_IS_DUMMY 1
  #endif
#endif

#if TEMP_SENSOR_BOARD == -4
  #define TEMP_SENSOR_BOARD_IS_AD8495 1
#elif TEMP_SENSOR_BOARD == -3
  #error "MAX31855 Thermocouples (-3) not supported for TEMP_SENSOR_BOARD."
#elif TEMP_SENSOR_BOARD == -2
  #error "MAX6675 Thermocouples (-2) not supported for TEMP_SENSOR_BOARD."
#elif TEMP_SENSOR_BOARD == -1
  #define TEMP_SENSOR_BOARD_IS_AD595 1
#elif TEMP_SENSOR_BOARD > 0
  #define TEMP_SENSOR_BOARD_IS_THERMISTOR 1
  #if TEMP_SENSOR_BOARD == 1000
    #define TEMP_SENSOR_BOARD_IS_CUSTOM 1
  #elif TEMP_SENSOR_BOARD == 998 || TEMP_SENSOR_BOARD == 999
    #define TEMP_SENSOR_BOARD_IS_DUMMY 1
  #endif
#endif

#if ENABLED(MIXING_EXTRUDER) && (ENABLED(RETRACT_SYNC_MIXING) || BOTH(FILAMENT_LOAD_UNLOAD_GCODES, FILAMENT_UNLOAD_ALL_EXTRUDERS))
  #define HAS_MIXER_SYNC_CHANNEL 1
#endif

#if EITHER(DUAL_X_CARRIAGE, MULTI_NOZZLE_DUPLICATION)
  #define HAS_DUPLICATION_MODE 1
#endif

#if ENABLED(PRINTCOUNTER) && (SERVICE_INTERVAL_1 > 0 || SERVICE_INTERVAL_2 > 0 || SERVICE_INTERVAL_3 > 0)
  #define HAS_SERVICE_INTERVALS 1
#endif

#if ENABLED(FILAMENT_RUNOUT_SENSOR)
  #define HAS_FILAMENT_SENSOR 1
  #if NUM_RUNOUT_SENSORS > 1
    #define MULTI_FILAMENT_SENSOR 1
  #endif
  #ifdef FILAMENT_RUNOUT_DISTANCE_MM
    #define HAS_FILAMENT_RUNOUT_DISTANCE 1
  #endif
  #if ENABLED(MIXING_EXTRUDER)
    #define WATCH_ALL_RUNOUT_SENSORS
  #endif
#endif

// Probe Temperature Compensation
#if !TEMP_SENSOR_PROBE
  #undef PTC_PROBE
#endif
#if !TEMP_SENSOR_BED
  #undef PTC_BED
#endif
#if !HAS_EXTRUDERS
  #undef PTC_HOTEND
#endif
#if ANY(PTC_PROBE, PTC_BED, PTC_HOTEND)
  #define HAS_PTC 1
#endif

// Let SD_FINISHED_RELEASECOMMAND stand in for SD_FINISHED_STEPPERRELEASE
#if ENABLED(SD_FINISHED_STEPPERRELEASE)
  #ifndef SD_FINISHED_RELEASECOMMAND
    #define SD_FINISHED_RELEASECOMMAND "M84" // planner.finish_and_disable()
  #endif
#else
  #undef SD_FINISHED_RELEASECOMMAND
#endif

#if ENABLED(NO_SD_AUTOSTART)
  #undef MENU_ADDAUTOSTART
#endif

#if EITHER(SDSUPPORT, LCD_SET_PROGRESS_MANUALLY)
  #define HAS_PRINT_PROGRESS 1
#endif

#if ENABLED(SDSUPPORT) && SD_PROCEDURE_DEPTH
  #define HAS_MEDIA_SUBCALLS 1
#endif

#if HAS_PRINT_PROGRESS && EITHER(PRINT_PROGRESS_SHOW_DECIMALS, SHOW_REMAINING_TIME)
  #define HAS_PRINT_PROGRESS_PERMYRIAD 1
#endif

#if ANY(MARLIN_BRICKOUT, MARLIN_INVADERS, MARLIN_SNAKE, MARLIN_MAZE)
  #define HAS_GAMES 1
  #if MANY(MARLIN_BRICKOUT, MARLIN_INVADERS, MARLIN_SNAKE, MARLIN_MAZE)
    #define HAS_GAME_MENU 1
  #endif
#endif

#if ANY(FWRETRACT, HAS_LEVELING, SKEW_CORRECTION)
  #define HAS_POSITION_MODIFIERS 1
#endif

#if ANY(X_DUAL_ENDSTOPS, Y_DUAL_ENDSTOPS, Z_MULTI_ENDSTOPS)
  #define HAS_EXTRA_ENDSTOPS 1
#endif
#if EITHER(MIN_SOFTWARE_ENDSTOPS, MAX_SOFTWARE_ENDSTOPS)
  #define HAS_SOFTWARE_ENDSTOPS 1
#endif
#if ANY(EXTENSIBLE_UI, IS_NEWPANEL, EMERGENCY_PARSER, HAS_ADC_BUTTONS, HAS_DWIN_E3V2)
  #define HAS_RESUME_CONTINUE 1
#endif

#if ANY(BLINKM, RGB_LED, RGBW_LED, PCA9632, PCA9533, NEOPIXEL_LED)
  #define HAS_COLOR_LEDS 1
#endif
#if ALL(HAS_RESUME_CONTINUE, PRINTER_EVENT_LEDS, SDSUPPORT)
  #define HAS_LEDS_OFF_FLAG 1
#endif

#if EITHER(DIGIPOT_MCP4018, DIGIPOT_MCP4451)
  #define HAS_MOTOR_CURRENT_I2C 1
#endif

#if ENABLED(Z_STEPPER_AUTO_ALIGN)
  #if ENABLED(Z_STEPPER_ALIGN_KNOWN_STEPPER_POSITIONS)
    #undef Z_STEPPER_ALIGN_AMP
  #endif
  #ifndef Z_STEPPER_ALIGN_AMP
    #define Z_STEPPER_ALIGN_AMP 1.0
  #endif
#endif

// Multiple Z steppers
#ifndef NUM_Z_STEPPER_DRIVERS
  #define NUM_Z_STEPPER_DRIVERS 1
#endif

// Fallback Stepper Driver types that depend on Configuration_adv.h
#if EITHER(DUAL_X_CARRIAGE, X_DUAL_STEPPER_DRIVERS)
  #define HAS_X2_STEPPER 1
#else
  #undef X2_DRIVER_TYPE
#endif
#if DISABLED(Y_DUAL_STEPPER_DRIVERS)
  #undef Y2_DRIVER_TYPE
#endif

#if NUM_Z_STEPPER_DRIVERS < 4
  #undef Z4_DRIVER_TYPE
  #undef INVERT_Z4_VS_Z_DIR
  #if NUM_Z_STEPPER_DRIVERS < 3
    #undef Z3_DRIVER_TYPE
    #undef INVERT_Z3_VS_Z_DIR
    #if NUM_Z_STEPPER_DRIVERS < 2
      #undef Z2_DRIVER_TYPE
      #undef INVERT_Z2_VS_Z_DIR
    #endif
  #endif
#endif

//
// Spindle/Laser power display types
// Defined here so sanity checks can use them
//
#if EITHER(SPINDLE_FEATURE, LASER_FEATURE)
  #define HAS_CUTTER 1
  #define _CUTTER_POWER_PWM255  1
  #define _CUTTER_POWER_PERCENT 2
  #define _CUTTER_POWER_RPM     3
  #define _CUTTER_POWER(V)      _CAT(_CUTTER_POWER_, V)
  #define CUTTER_UNIT_IS(V)    (_CUTTER_POWER(CUTTER_POWER_UNIT) == _CUTTER_POWER(V))
#endif

#if !defined(__AVR__) || !defined(USBCON)
  // Define constants and variables for buffering serial data.
  // Use only 0 or powers of 2 greater than 1
  // : [0, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, ...]
  #ifndef RX_BUFFER_SIZE
    #define RX_BUFFER_SIZE 128
  #endif
  // 256 is the max TX buffer limit due to uint8_t head and tail
  // : [0, 4, 8, 16, 32, 64, 128, 256]
  #ifndef TX_BUFFER_SIZE
    #define TX_BUFFER_SIZE 32
  #endif
#else
  // SERIAL_XON_XOFF not supported on USB-native devices
  #undef SERIAL_XON_XOFF
#endif

#if ENABLED(HOST_ACTION_COMMANDS)
  #ifndef ACTION_ON_PAUSE
    #define ACTION_ON_PAUSE   "pause"
  #endif
  #ifndef ACTION_ON_PAUSED
    #define ACTION_ON_PAUSED  "paused"
  #endif
  #ifndef ACTION_ON_RESUME
    #define ACTION_ON_RESUME  "resume"
  #endif
  #ifndef ACTION_ON_RESUMED
    #define ACTION_ON_RESUMED "resumed"
  #endif
  #ifndef ACTION_ON_CANCEL
    #define ACTION_ON_CANCEL  "cancel"
  #endif
  #ifndef ACTION_ON_START
    #define ACTION_ON_START   "start"
  #endif
  #ifndef ACTION_ON_KILL
    #define ACTION_ON_KILL    "poweroff"
  #endif
  #ifndef SHUTDOWN_ACTION
    #define SHUTDOWN_ACTION   "shutdown"
  #endif
  #if HAS_FILAMENT_SENSOR
    #ifndef ACTION_ON_FILAMENT_RUNOUT
      #define ACTION_ON_FILAMENT_RUNOUT "filament_runout"
    #endif
    #ifndef ACTION_REASON_ON_FILAMENT_RUNOUT
      #define ACTION_REASON_ON_FILAMENT_RUNOUT "filament_runout"
    #endif
  #endif
  #if ENABLED(G29_RETRY_AND_RECOVER)
    #ifndef ACTION_ON_G29_RECOVER
      #define ACTION_ON_G29_RECOVER "probe_rewipe"
    #endif
    #ifndef ACTION_ON_G29_FAILURE
      #define ACTION_ON_G29_FAILURE "probe_failed"
    #endif
  #endif
#endif

#if EITHER(FYSETC_MINI_12864_2_1, FYSETC_242_OLED_12864)
  #define LED_CONTROL_MENU
  #define LED_USER_PRESET_STARTUP
  #define LED_COLOR_PRESETS
  #ifndef LED_USER_PRESET_GREEN
    #define LED_USER_PRESET_GREEN      128
  #endif
  #ifndef LED_USER_PRESET_BLUE
    #define LED_USER_PRESET_BLUE         0
  #endif
  #ifndef LED_USER_PRESET_BRIGHTNESS
    #define LED_USER_PRESET_BRIGHTNESS 255
  #endif
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

#if BOTH(LED_CONTROL_MENU, NEOPIXEL2_SEPARATE)
  #ifndef LED2_USER_PRESET_RED
    #define LED2_USER_PRESET_RED       255
  #endif
  #ifndef LED2_USER_PRESET_GREEN
    #define LED2_USER_PRESET_GREEN     255
  #endif
  #ifndef LED2_USER_PRESET_BLUE
    #define LED2_USER_PRESET_BLUE      255
  #endif
  #ifndef LED2_USER_PRESET_WHITE
    #define LED2_USER_PRESET_WHITE     0
  #endif
  #ifndef LED2_USER_PRESET_BRIGHTNESS
    #ifdef NEOPIXEL2_BRIGHTNESS
      #define LED2_USER_PRESET_BRIGHTNESS NEOPIXEL2_BRIGHTNESS
    #else
      #define LED2_USER_PRESET_BRIGHTNESS 255
    #endif
  #endif
#endif

// Full Touch Screen needs 'tft/xpt2046'
#if EITHER(TFT_TOUCH_DEVICE_XPT2046, HAS_TFT_LVGL_UI)
  #define HAS_TFT_XPT2046 1
#endif

// Touch Screen or "Touch Buttons" need XPT2046 pins
// but they use different components
#if HAS_TFT_XPT2046 || HAS_RES_TOUCH_BUTTONS
  #define NEED_TOUCH_PINS 1
#endif

// Extensible UI pin mapping for RepRapDiscount
#if ENABLED(TOUCH_UI_FTDI_EVE) && ANY(AO_EXP1_PINMAP, AO_EXP2_PINMAP, CR10_TFT_PINMAP)
  #define TOUCH_UI_ULTIPANEL 1
#endif

// Poll-based jogging for joystick and other devices
#if ENABLED(JOYSTICK)
  #define POLL_JOG
#endif

#if X2_HOME_DIR > 0
  #define X2_HOME_TO_MAX 1
#elif X2_HOME_DIR < 0
  #define X2_HOME_TO_MIN 1
#endif

#ifndef HOMING_BUMP_MM
  #define HOMING_BUMP_MM { 0, 0, 0 }
#endif

#if ENABLED(USB_FLASH_DRIVE_SUPPORT) && NONE(USE_OTG_USB_HOST, USE_UHS3_USB)
  #define USE_UHS2_USB
#endif

/**
 * Driver Timings (in nanoseconds)
 * NOTE: Driver timing order is longest-to-shortest duration.
 *       Preserve this ordering when adding new drivers.
 */
#ifndef MINIMUM_STEPPER_POST_DIR_DELAY
  #if HAS_DRIVER(TB6560)
    #define MINIMUM_STEPPER_POST_DIR_DELAY 15000
  #elif HAS_DRIVER(TB6600)
    #define MINIMUM_STEPPER_POST_DIR_DELAY 1500
  #elif HAS_DRIVER(DRV8825)
    #define MINIMUM_STEPPER_POST_DIR_DELAY 650
  #elif HAS_DRIVER(LV8729)
    #define MINIMUM_STEPPER_POST_DIR_DELAY 500
  #elif HAS_DRIVER(A5984)
    #define MINIMUM_STEPPER_POST_DIR_DELAY 400
  #elif HAS_DRIVER(A4988)
    #define MINIMUM_STEPPER_POST_DIR_DELAY 200
  #elif HAS_TRINAMIC_CONFIG || HAS_TRINAMIC_STANDALONE
    #define MINIMUM_STEPPER_POST_DIR_DELAY 60
  #else
    #define MINIMUM_STEPPER_POST_DIR_DELAY 0   // Expect at least 10µS since one Stepper ISR must transpire
  #endif
#endif

#ifndef MINIMUM_STEPPER_PRE_DIR_DELAY
  #define MINIMUM_STEPPER_PRE_DIR_DELAY MINIMUM_STEPPER_POST_DIR_DELAY
#endif

#ifndef MINIMUM_STEPPER_PULSE
  #if HAS_DRIVER(TB6560)
    #define MINIMUM_STEPPER_PULSE 30
  #elif HAS_DRIVER(TB6600)
    #define MINIMUM_STEPPER_PULSE 3
  #elif HAS_DRIVER(DRV8825)
    #define MINIMUM_STEPPER_PULSE 2
  #elif HAS_DRIVER(A4988) || HAS_DRIVER(A5984)
    #define MINIMUM_STEPPER_PULSE 1
  #elif HAS_TRINAMIC_CONFIG || HAS_TRINAMIC_STANDALONE
    #define MINIMUM_STEPPER_PULSE 0
  #elif HAS_DRIVER(LV8729)
    #define MINIMUM_STEPPER_PULSE 0
  #else
    #define MINIMUM_STEPPER_PULSE 2
  #endif
#endif

#ifndef MAXIMUM_STEPPER_RATE
  #if HAS_DRIVER(TB6560)
    #define MAXIMUM_STEPPER_RATE 15000
  #elif HAS_DRIVER(TB6600)
    #define MAXIMUM_STEPPER_RATE 150000
  #elif HAS_DRIVER(DRV8825)
    #define MAXIMUM_STEPPER_RATE 250000
  #elif HAS_DRIVER(A4988)
    #define MAXIMUM_STEPPER_RATE 500000
  #elif HAS_DRIVER(LV8729)
    #define MAXIMUM_STEPPER_RATE 1000000
  #elif HAS_TRINAMIC_CONFIG || HAS_TRINAMIC_STANDALONE
    #define MAXIMUM_STEPPER_RATE 5000000
  #else
    #define MAXIMUM_STEPPER_RATE 250000
  #endif
#endif

#if ENABLED(DIRECT_STEPPING)
  #ifndef STEPPER_PAGES
    #define STEPPER_PAGES 16
  #endif
  #ifndef STEPPER_PAGE_FORMAT
    #define STEPPER_PAGE_FORMAT SP_4x2_256
  #endif
  #ifndef PAGE_MANAGER
    #define PAGE_MANAGER SerialPageManager
  #endif
#endif

// Remove unused STEALTHCHOP flags
#if LINEAR_AXES < 6
  #undef STEALTHCHOP_K
  #undef CALIBRATION_MEASURE_KMIN
  #undef CALIBRATION_MEASURE_KMAX
  #if LINEAR_AXES < 5
    #undef STEALTHCHOP_J
    #undef CALIBRATION_MEASURE_JMIN
    #undef CALIBRATION_MEASURE_JMAX
    #if LINEAR_AXES < 4
      #undef STEALTHCHOP_I
      #undef CALIBRATION_MEASURE_IMIN
      #undef CALIBRATION_MEASURE_IMAX
      #if LINEAR_AXES < 3
        #undef Z_IDLE_HEIGHT
        #undef STEALTHCHOP_Z
        #undef Z_PROBE_SLED
        #undef Z_SAFE_HOMING
        #undef HOME_Z_FIRST
        #undef HOMING_Z_WITH_PROBE
        #undef ENABLE_LEVELING_FADE_HEIGHT
        #undef NUM_Z_STEPPER_DRIVERS
        #undef CNC_WORKSPACE_PLANES
        #if LINEAR_AXES < 2
          #undef STEALTHCHOP_Y
        #endif
      #endif
    #endif
  #endif
#endif

//
// SD Card connection methods
// Defined here so pins and sanity checks can use them
//
#if ENABLED(SDSUPPORT)
  #define _SDCARD_LCD          1
  #define _SDCARD_ONBOARD      2
  #define _SDCARD_CUSTOM_CABLE 3
  #define _SDCARD_ID(V) _CAT(_SDCARD_, V)
  #define SD_CONNECTION_IS(V) (_SDCARD_ID(SDCARD_CONNECTION) == _SDCARD_ID(V))
#else
  #define SD_CONNECTION_IS(...) 0
#endif

// Power Monitor sensors
#if EITHER(POWER_MONITOR_CURRENT, POWER_MONITOR_VOLTAGE)
  #define HAS_POWER_MONITOR 1
  #if ENABLED(POWER_MONITOR_CURRENT) && (ENABLED(POWER_MONITOR_VOLTAGE) || defined(POWER_MONITOR_FIXED_VOLTAGE))
    #define HAS_POWER_MONITOR_WATTS 1
  #endif
#endif

// Flag if an EEPROM type is pre-selected
#if ENABLED(EEPROM_SETTINGS) && NONE(I2C_EEPROM, SPI_EEPROM, QSPI_EEPROM, FLASH_EEPROM_EMULATION, SRAM_EEPROM_EMULATION, SDCARD_EEPROM_EMULATION)
  #define NO_EEPROM_SELECTED 1
#endif

// Flag whether hex_print.cpp is used
#if ANY(AUTO_BED_LEVELING_UBL, M100_FREE_MEMORY_WATCHER, DEBUG_GCODE_PARSER, TMC_DEBUG, MARLIN_DEV_MODE)
  #define NEED_HEX_PRINT 1
#endif

// Flags for Case Light having a color property or a single pin
#if ENABLED(CASE_LIGHT_ENABLE)
  #if EITHER(CASE_LIGHT_USE_NEOPIXEL, CASE_LIGHT_USE_RGB_LED)
    #define CASE_LIGHT_IS_COLOR_LED 1
  #else
    #define NEED_CASE_LIGHT_PIN 1
  #endif
#endif

// Flag whether least_squares_fit.cpp is used
#if ANY(AUTO_BED_LEVELING_UBL, AUTO_BED_LEVELING_LINEAR, Z_STEPPER_ALIGN_KNOWN_STEPPER_POSITIONS)
  #define NEED_LSF 1
#endif

#if BOTH(HAS_TFT_LVGL_UI, CUSTOM_MENU_MAIN)
  #define _HAS_1(N) (defined(MAIN_MENU_ITEM_##N##_DESC) && defined(MAIN_MENU_ITEM_##N##_GCODE))
  #define HAS_USER_ITEM(V...) DO(HAS,||,V)
#else
  #define HAS_USER_ITEM(N) 0
#endif

#if !HAS_MULTI_SERIAL
  #undef MEATPACK_ON_SERIAL_PORT_2
#endif
#if EITHER(MEATPACK_ON_SERIAL_PORT_1, MEATPACK_ON_SERIAL_PORT_2)
  #define HAS_MEATPACK 1
#endif

// AVR are (usually) too limited in resources to store the configuration into the binary
#if ENABLED(CONFIGURATION_EMBEDDING) && !defined(FORCE_CONFIG_EMBED) && (defined(__AVR__) || DISABLED(SDSUPPORT) || EITHER(SDCARD_READONLY, DISABLE_M503))
  #undef CONFIGURATION_EMBEDDING
  #define CANNOT_EMBED_CONFIGURATION defined(__AVR__)
#endif
