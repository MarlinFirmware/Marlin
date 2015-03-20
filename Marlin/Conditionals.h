/**
 * Conditionals.h
 * Defines that depend on configuration but are not editable.
 */
#ifndef CONDITIONALS_H
  #define CONDITIONALS_H

  #ifndef AT90USB
    #define HardwareSerial_h // trick to disable the standard HWserial
  #endif

  #if (ARDUINO >= 100)
    #include "Arduino.h"
  #else
    #include "WProgram.h"
  #endif

  #include "pins.h"

  /**
   * ENDSTOPPULLUPS
   */
  #ifdef ENDSTOPPULLUPS
    #ifndef DISABLE_MAX_ENDSTOPS
      #define ENDSTOPPULLUP_XMAX
      #define ENDSTOPPULLUP_YMAX
      #define ENDSTOPPULLUP_ZMAX
    #endif
    #ifndef DISABLE_MIN_ENDSTOPS
      #define ENDSTOPPULLUP_XMIN
      #define ENDSTOPPULLUP_YMIN
      #define ENDSTOPPULLUP_ZMIN
    #endif
  #endif

  /**
   * Axis lengths
   */
  #define X_MAX_LENGTH (X_MAX_POS - X_MIN_POS)
  #define Y_MAX_LENGTH (Y_MAX_POS - Y_MIN_POS)
  #define Z_MAX_LENGTH (Z_MAX_POS - Z_MIN_POS)

  /**
   * SCARA
   */
  #ifdef SCARA
    #undef SLOWDOWN
    #define QUICK_HOME //SCARA needs Quickhome
  #endif

  /**
   * AUTOSET LOCATIONS OF LIMIT SWITCHES
   * Added by ZetaPhoenix 09-15-2012
   */
  #ifdef MANUAL_HOME_POSITIONS  // Use manual limit switch locations
    #define X_HOME_POS MANUAL_X_HOME_POS
    #define Y_HOME_POS MANUAL_Y_HOME_POS
    #define Z_HOME_POS MANUAL_Z_HOME_POS
  #else //!MANUAL_HOME_POSITIONS – Use home switch positions based on homing direction and travel limits
    #ifdef BED_CENTER_AT_0_0
      #define X_HOME_POS X_MAX_LENGTH * X_HOME_DIR * 0.5
      #define Y_HOME_POS Y_MAX_LENGTH * Y_HOME_DIR * 0.5
    #else
      #define X_HOME_POS (X_HOME_DIR < 0 ? X_MIN_POS : X_MAX_POS)
      #define Y_HOME_POS (Y_HOME_DIR < 0 ? Y_MIN_POS : Y_MAX_POS)
    #endif
    #define Z_HOME_POS (Z_HOME_DIR < 0 ? Z_MIN_POS : Z_MAX_POS)
  #endif //!MANUAL_HOME_POSITIONS

  /**
   * Auto Bed Leveling
   */
  #ifdef ENABLE_AUTO_BED_LEVELING
    // Boundaries for probing based on set limits
    #define MIN_PROBE_X (max(X_MIN_POS, X_MIN_POS + X_PROBE_OFFSET_FROM_EXTRUDER))
    #define MAX_PROBE_X (min(X_MAX_POS, X_MAX_POS + X_PROBE_OFFSET_FROM_EXTRUDER))
    #define MIN_PROBE_Y (max(Y_MIN_POS, Y_MIN_POS + Y_PROBE_OFFSET_FROM_EXTRUDER))
    #define MAX_PROBE_Y (min(Y_MAX_POS, Y_MAX_POS + Y_PROBE_OFFSET_FROM_EXTRUDER))
  #endif

  /**
   * MAX_STEP_FREQUENCY differs for TOSHIBA
   */
  #ifdef CONFIG_STEPPERS_TOSHIBA
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
  #ifdef ADVANCE
    #define EXTRUSION_AREA (0.25 * D_FILAMENT * D_FILAMENT * 3.14159)
    #define STEPS_PER_CUBIC_MM_E (axis_steps_per_unit[E_AXIS] / EXTRUSION_AREA)
  #endif

  #ifdef ULTIPANEL
   #undef SDCARDDETECTINVERTED
  #endif

  // Power Signal Control Definitions
  // By default use ATX definition
  #ifndef POWER_SUPPLY
    #define POWER_SUPPLY 1
  #endif
  // 1 = ATX
  #if (POWER_SUPPLY == 1)
    #define PS_ON_AWAKE  LOW
    #define PS_ON_ASLEEP HIGH
  #endif
  // 2 = X-Box 360 203W
  #if (POWER_SUPPLY == 2)
    #define PS_ON_AWAKE  HIGH
    #define PS_ON_ASLEEP LOW
  #endif

  /**
   * Temp Sensor defines
   */
  #if TEMP_SENSOR_0 == -2
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

  #if TEMP_SENSOR_1 == -1
    #define HEATER_1_USES_AD595
  #elif TEMP_SENSOR_1 == 0
    #undef HEATER_1_MINTEMP
    #undef HEATER_1_MAXTEMP
  #elif TEMP_SENSOR_1 > 0
    #define THERMISTORHEATER_1 TEMP_SENSOR_1
    #define HEATER_1_USES_THERMISTOR
  #endif

  #if TEMP_SENSOR_2 == -1
    #define HEATER_2_USES_AD595
  #elif TEMP_SENSOR_2 == 0
    #undef HEATER_2_MINTEMP
    #undef HEATER_2_MAXTEMP
  #elif TEMP_SENSOR_2 > 0
    #define THERMISTORHEATER_2 TEMP_SENSOR_2
    #define HEATER_2_USES_THERMISTOR
  #endif

  #if TEMP_SENSOR_3 == -1
    #define HEATER_3_USES_AD595
  #elif TEMP_SENSOR_3 == 0
    #undef HEATER_3_MINTEMP
    #undef HEATER_3_MAXTEMP
  #elif TEMP_SENSOR_3 > 0
    #define THERMISTORHEATER_3 TEMP_SENSOR_3
    #define HEATER_3_USES_THERMISTOR
  #endif

  #if TEMP_SENSOR_BED == -1
    #define BED_USES_AD595
  #elif TEMP_SENSOR_BED == 0
    #undef BED_MINTEMP
    #undef BED_MAXTEMP
  #elif TEMP_SENSOR_BED > 0
    #define THERMISTORBED TEMP_SENSOR_BED
    #define BED_USES_THERMISTOR
  #endif

  /**
   * ARRAY_BY_EXTRUDERS based on EXTRUDERS
   */
  #if EXTRUDERS > 3
    #define ARRAY_BY_EXTRUDERS(v1, v2, v3, v4) { v1, v2, v3, v4 }
  #elif EXTRUDERS > 2
    #define ARRAY_BY_EXTRUDERS(v1, v2, v3, v4) { v1, v2, v3 }
  #elif EXTRUDERS > 1
    #define ARRAY_BY_EXTRUDERS(v1, v2, v3, v4) { v1, v2 }
  #else
    #define ARRAY_BY_EXTRUDERS(v1, v2, v3, v4) { v1 }
  #endif

  /**
   * Shorthand for pin tests, for temperature.cpp
   */
  #define HAS_TEMP_0 (defined(TEMP_0_PIN) && TEMP_0_PIN >= 0)
  #define HAS_TEMP_1 (defined(TEMP_1_PIN) && TEMP_1_PIN >= 0)
  #define HAS_TEMP_2 (defined(TEMP_2_PIN) && TEMP_2_PIN >= 0)
  #define HAS_TEMP_3 (defined(TEMP_3_PIN) && TEMP_3_PIN >= 0)
  #define HAS_TEMP_BED (defined(TEMP_BED_PIN) && TEMP_BED_PIN >= 0)
  #define HAS_FILAMENT_SENSOR (defined(FILAMENT_SENSOR) && defined(FILWIDTH_PIN) && FILWIDTH_PIN >= 0)
  #define HAS_HEATER_0 (defined(HEATER_0_PIN) && HEATER_0_PIN >= 0)
  #define HAS_HEATER_1 (defined(HEATER_1_PIN) && HEATER_1_PIN >= 0)
  #define HAS_HEATER_2 (defined(HEATER_2_PIN) && HEATER_2_PIN >= 0)
  #define HAS_HEATER_3 (defined(HEATER_3_PIN) && HEATER_3_PIN >= 0)
  #define HAS_HEATER_BED (defined(HEATER_BED_PIN) && HEATER_BED_PIN >= 0)
  #define HAS_AUTO_FAN_0 (defined(EXTRUDER_0_AUTO_FAN_PIN) && EXTRUDER_0_AUTO_FAN_PIN >= 0)
  #define HAS_AUTO_FAN_1 (defined(EXTRUDER_1_AUTO_FAN_PIN) && EXTRUDER_1_AUTO_FAN_PIN >= 0)
  #define HAS_AUTO_FAN_2 (defined(EXTRUDER_2_AUTO_FAN_PIN) && EXTRUDER_2_AUTO_FAN_PIN >= 0)
  #define HAS_AUTO_FAN_3 (defined(EXTRUDER_3_AUTO_FAN_PIN) && EXTRUDER_3_AUTO_FAN_PIN >= 0)
  #define HAS_AUTO_FAN HAS_AUTO_FAN_0 || HAS_AUTO_FAN_1 || HAS_AUTO_FAN_2 || HAS_AUTO_FAN_3
  #define HAS_FAN (defined(FAN_PIN) && FAN_PIN >= 0)

  /**
   * Helper Macros for heaters and extruder fan
   */
  #define WRITE_HEATER_0P(v) WRITE(HEATER_0_PIN, v)
  #if EXTRUDERS > 1 || defined(HEATERS_PARALLEL)
    #define WRITE_HEATER_1(v) WRITE(HEATER_1_PIN, v)
    #if EXTRUDERS > 2
      #define WRITE_HEATER_2(v) WRITE(HEATER_2_PIN, v)
      #if EXTRUDERS > 3
        #define WRITE_HEATER_3(v) WRITE(HEATER_3_PIN, v)
      #endif
    #endif
  #endif
  #ifdef HEATERS_PARALLEL
    #define WRITE_HEATER_0(v) { WRITE_HEATER_0P(v); WRITE_HEATER_1(v); }
  #else
    #define WRITE_HEATER_0(v) WRITE_HEATER_0P(v)
  #endif
  #if HAS_HEATER_BED
    #define WRITE_HEATER_BED(v) WRITE(HEATER_BED_PIN, v)
  #endif
  #if HAS_FAN
    #define WRITE_FAN(v) WRITE(FAN_PIN, v)
  #endif

  /**
   * Sampling period of the temperature routine
   * This override comes originally from temperature.cpp
   * The Configuration.h option is basically ignored.
   */
  #ifdef PID_dT
    #undef PID_dT
  #endif
  #define PID_dT ((OVERSAMPLENR * 12.0)/(F_CPU / 64.0 / 256.0))

#endif //CONDITIONALS_H
