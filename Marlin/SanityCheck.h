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
 * SanityCheck.h
 *
 * Test configuration values for errors at compile-time.
 */

/**
 * Due to the high number of issues related with old versions of Arduino IDE
 * we now prevent Marlin from compiling with older toolkits.
 */
#if !defined(ARDUINO) || ARDUINO < 10608
  #error "Versions of Arduino IDE prior to 1.6.8 are no longer supported, please update your toolkit."
#endif

/**
 * We try our best to include sanity checks for all the changes configuration
 * directives because people have a tendency to use outdated config files with
 * the bleding edge source code, but sometimes this is not enough. This check
 * will force a minimum config file revision, otherwise Marlin will not build.
 */
#if ! defined(CONFIGURATION_H_VERSION) || CONFIGURATION_H_VERSION < REQUIRED_CONFIGURATION_H_VERSION
  #error "You are using an old Configuration.h file, update it before building Marlin."
#endif

#if ! defined(CONFIGURATION_ADV_H_VERSION) || CONFIGURATION_ADV_H_VERSION < REQUIRED_CONFIGURATION_ADV_H_VERSION
  #error "You are using an old Configuration_adv.h file, update it before building Marlin."
#endif

/**
 * Warnings for old configurations
 */
#if WATCH_TEMP_PERIOD > 500
  #error "WATCH_TEMP_PERIOD now uses seconds instead of milliseconds."
#elif DISABLED(THERMAL_PROTECTION_HOTENDS) && (defined(WATCH_TEMP_PERIOD) || defined(THERMAL_PROTECTION_PERIOD))
  #error "Thermal Runaway Protection for hotends is now enabled with THERMAL_PROTECTION_HOTENDS."
#elif DISABLED(THERMAL_PROTECTION_BED) && defined(THERMAL_PROTECTION_BED_PERIOD)
  #error "Thermal Runaway Protection for the bed is now enabled with THERMAL_PROTECTION_BED."
#elif ENABLED(COREXZ) && ENABLED(Z_LATE_ENABLE)
  #error "Z_LATE_ENABLE can't be used with COREXZ."
#elif defined(X_HOME_RETRACT_MM)
  #error "[XYZ]_HOME_RETRACT_MM settings have been renamed [XYZ]_HOME_BUMP_MM."
#elif defined(SDCARDDETECTINVERTED)
  #error "SDCARDDETECTINVERTED is now SD_DETECT_INVERTED. Please update your configuration."
#elif defined(BTENABLED)
  #error "BTENABLED is now BLUETOOTH. Please update your configuration."
#elif defined(CUSTOM_MENDEL_NAME)
  #error "CUSTOM_MENDEL_NAME is now CUSTOM_MACHINE_NAME. Please update your configuration."
#elif defined(HAS_AUTOMATIC_VERSIONING)
  #error "HAS_AUTOMATIC_VERSIONING is now USE_AUTOMATIC_VERSIONING. Please update your configuration."
#elif defined(SDSLOW)
  #error "SDSLOW deprecated. Set SPI_SPEED to SPI_HALF_SPEED instead."
#elif defined(SDEXTRASLOW)
  #error "SDEXTRASLOW deprecated. Set SPI_SPEED to SPI_QUARTER_SPEED instead."
#elif defined(FILAMENT_SENSOR)
  #error "FILAMENT_SENSOR is deprecated. Use FILAMENT_WIDTH_SENSOR instead."
#elif defined(DISABLE_MAX_ENDSTOPS) || defined(DISABLE_MIN_ENDSTOPS)
  #error "DISABLE_MAX_ENDSTOPS and DISABLE_MIN_ENDSTOPS deprecated. Use individual USE_*_PLUG options instead."
#elif ENABLED(Z_DUAL_ENDSTOPS) && !defined(Z2_USE_ENDSTOP)
  #error "Z_DUAL_ENDSTOPS settings are simplified. Just set Z2_USE_ENDSTOP to the endstop you want to repurpose for Z2"
#elif defined(LANGUAGE_INCLUDE)
  #error "LANGUAGE_INCLUDE has been replaced by LCD_LANGUAGE. Please update your configuration."
#elif defined(EXTRUDER_OFFSET_X) || defined(EXTRUDER_OFFSET_Y)
  #error "EXTRUDER_OFFSET_[XY] is deprecated. Use HOTEND_OFFSET_[XY] instead."
#elif defined(PID_PARAMS_PER_EXTRUDER)
  #error "PID_PARAMS_PER_EXTRUDER is deprecated. Use PID_PARAMS_PER_HOTEND instead."
#elif defined(EXTRUDER_WATTS) || defined(BED_WATTS)
  #error "EXTRUDER_WATTS and BED_WATTS are deprecated. Remove them from your configuration."
#elif defined(SERVO_ENDSTOP_ANGLES)
  #error "SERVO_ENDSTOP_ANGLES is deprecated. Use Z_SERVO_ANGLES instead."
#elif defined(X_ENDSTOP_SERVO_NR) || defined(Y_ENDSTOP_SERVO_NR)
  #error "X_ENDSTOP_SERVO_NR and Y_ENDSTOP_SERVO_NR are deprecated and should be removed."
#elif defined(DEFAULT_XYJERK)
  #error "DEFAULT_XYJERK is deprecated. Use DEFAULT_XJERK and DEFAULT_YJERK instead."
#elif defined(XY_TRAVEL_SPEED)
  #error "XY_TRAVEL_SPEED is deprecated. Use XY_PROBE_SPEED instead."
#elif defined(PROBE_SERVO_DEACTIVATION_DELAY)
  #error "PROBE_SERVO_DEACTIVATION_DELAY is deprecated. Use DEACTIVATE_SERVOS_AFTER_MOVE instead."
#elif defined(SERVO_DEACTIVATION_DELAY)
  #error "SERVO_DEACTIVATION_DELAY is deprecated. Use SERVO_DELAY instead."
#elif ENABLED(FILAMENTCHANGEENABLE)
  #error "FILAMENTCHANGEENABLE is now FILAMENT_CHANGE_FEATURE. Please update your configuration."
#elif defined(PLA_PREHEAT_HOTEND_TEMP)
  #error "PLA_PREHEAT_HOTEND_TEMP is now PREHEAT_1_TEMP_HOTEND. Please update your configuration."
#elif defined(PLA_PREHEAT_HPB_TEMP)
  #error "PLA_PREHEAT_HPB_TEMP is now PREHEAT_1_TEMP_BED. Please update your configuration."
#elif defined(PLA_PREHEAT_FAN_SPEED)
  #error "PLA_PREHEAT_FAN_SPEED is now PREHEAT_1_FAN_SPEED. Please update your configuration."
#elif defined(ABS_PREHEAT_HOTEND_TEMP)
  #error "ABS_PREHEAT_HOTEND_TEMP is now PREHEAT_2_TEMP_HOTEND. Please update your configuration."
#elif defined(ABS_PREHEAT_HPB_TEMP)
  #error "ABS_PREHEAT_HPB_TEMP is now PREHEAT_2_TEMP_BED. Please update your configuration."
#elif defined(ABS_PREHEAT_FAN_SPEED)
  #error "ABS_PREHEAT_FAN_SPEED is now PREHEAT_2_FAN_SPEED. Please update your configuration."
#elif defined(ENDSTOPS_ONLY_FOR_HOMING)
  #error "ENDSTOPS_ONLY_FOR_HOMING is deprecated. Use (disable) ENDSTOPS_ALWAYS_ON_DEFAULT instead."
#elif defined(HOMING_FEEDRATE)
  #error "HOMING_FEEDRATE is deprecated. Set individual rates with HOMING_FEEDRATE_(XY|Z|E) instead."
#elif defined(MANUAL_HOME_POSITIONS)
  #error "MANUAL_HOME_POSITIONS is deprecated. Set MANUAL_[XYZ]_HOME_POS as-needed instead."
#elif defined(PID_ADD_EXTRUSION_RATE)
  #error "PID_ADD_EXTRUSION_RATE is now PID_EXTRUSION_SCALING and is DISABLED by default. Are you sure you want to use this option? Please update your configuration."
#elif defined(Z_RAISE_BEFORE_HOMING)
  #error "Z_RAISE_BEFORE_HOMING is now Z_HOMING_HEIGHT. Please update your configuration."
#elif defined(MIN_Z_HEIGHT_FOR_HOMING)
  #error "MIN_Z_HEIGHT_FOR_HOMING is now Z_HOMING_HEIGHT. Please update your configuration."
#elif defined(Z_RAISE_BEFORE_PROBING) || defined(Z_RAISE_AFTER_PROBING)
  #error "Z_RAISE_(BEFORE|AFTER)_PROBING are deprecated. Use Z_CLEARANCE_DEPLOY_PROBE instead."
#elif defined(Z_RAISE_PROBE_DEPLOY_STOW) || defined(Z_RAISE_BETWEEN_PROBINGS)
  #error "Z_RAISE_PROBE_DEPLOY_STOW and Z_RAISE_BETWEEN_PROBINGS are now Z_CLEARANCE_DEPLOY_PROBE and Z_CLEARANCE_BETWEEN_PROBES. Please update your configuration."
#elif defined(Z_PROBE_DEPLOY_HEIGHT) || defined(Z_PROBE_TRAVEL_HEIGHT)
  #error "Z_PROBE_DEPLOY_HEIGHT and Z_PROBE_TRAVEL_HEIGHT are now Z_CLEARANCE_DEPLOY_PROBE and Z_CLEARANCE_BETWEEN_PROBES. Please update your configuration."
#elif !defined(MIN_STEPS_PER_SEGMENT)
  #error Please replace "const int dropsegments" with "#define MIN_STEPS_PER_SEGMENT" (and increase by 1) in Configuration_adv.h.
#elif defined(PREVENT_DANGEROUS_EXTRUDE)
  #error "PREVENT_DANGEROUS_EXTRUDE is now PREVENT_COLD_EXTRUSION. Please update your configuration."
#elif defined(SCARA)
  #error "SCARA is now MORGAN_SCARA. Please update your configuration."
#elif defined(ENABLE_AUTO_BED_LEVELING)
  #error "ENABLE_AUTO_BED_LEVELING is deprecated. Specify AUTO_BED_LEVELING_LINEAR, AUTO_BED_LEVELING_BILINEAR, or AUTO_BED_LEVELING_3POINT."
#elif defined(AUTO_BED_LEVELING_FEATURE)
  #error "AUTO_BED_LEVELING_FEATURE is deprecated. Specify AUTO_BED_LEVELING_LINEAR, AUTO_BED_LEVELING_BILINEAR, or AUTO_BED_LEVELING_3POINT."
#elif defined(ABL_GRID_POINTS)
  #error "ABL_GRID_POINTS is now ABL_GRID_POINTS_X and ABL_GRID_POINTS_Y. Please update your configuration."
#elif defined(BEEPER)
  #error "BEEPER is now BEEPER_PIN. Please update your pins definitions."
#elif defined(SDCARDDETECT)
  #error "SDCARDDETECT is now SD_DETECT_PIN. Please update your pins definitions."
#elif defined(STAT_LED_RED) || defined(STAT_LED_BLUE)
  #error "STAT_LED_RED/STAT_LED_BLUE are now STAT_LED_RED_PIN/STAT_LED_BLUE_PIN. Please update your pins definitions."
#elif defined(LCD_PIN_BL)
  #error "LCD_PIN_BL is now LCD_BACKLIGHT_PIN. Please update your pins definitions."
#elif defined(LCD_PIN_RESET)
  #error "LCD_PIN_RESET is now LCD_RESET_PIN. Please update your pins definitions."
#endif

/**
 * Marlin release, version and default string
 */
#ifndef SHORT_BUILD_VERSION
  #error "SHORT_BUILD_VERSION must be specified."
#elif !defined(DETAILED_BUILD_VERSION)
  #error "BUILD_VERSION must be specified."
#elif !defined(STRING_DISTRIBUTION_DATE)
  #error "STRING_DISTRIBUTION_DATE must be specified."
#elif !defined(PROTOCOL_VERSION)
  #error "PROTOCOL_VERSION must be specified."
#elif !defined(MACHINE_NAME)
  #error "MACHINE_NAME must be specified."
#elif !defined(SOURCE_CODE_URL)
  #error "SOURCE_CODE_URL must be specified."
#elif !defined(DEFAULT_MACHINE_UUID)
  #error "DEFAULT_MACHINE_UUID must be specified."
#elif !defined(WEBSITE_URL)
  #error "WEBSITE_URL must be specified."
#endif

/**
 * Dual Stepper Drivers
 */
#if ENABLED(X_DUAL_STEPPER_DRIVERS) && ENABLED(DUAL_X_CARRIAGE)
  #error "DUAL_X_CARRIAGE is not compatible with X_DUAL_STEPPER_DRIVERS."
#elif ENABLED(X_DUAL_STEPPER_DRIVERS) && (!HAS_X2_ENABLE || !HAS_X2_STEP || !HAS_X2_DIR)
  #error "X_DUAL_STEPPER_DRIVERS requires X2 pins (and an extra E plug)."
#elif ENABLED(Y_DUAL_STEPPER_DRIVERS) && (!HAS_Y2_ENABLE || !HAS_Y2_STEP || !HAS_Y2_DIR)
  #error "Y_DUAL_STEPPER_DRIVERS requires Y2 pins (and an extra E plug)."
#elif ENABLED(Z_DUAL_STEPPER_DRIVERS) && (!HAS_Z2_ENABLE || !HAS_Z2_STEP || !HAS_Z2_DIR)
  #error "Z_DUAL_STEPPER_DRIVERS requires Z2 pins (and an extra E plug)."
#endif

/**
 * Progress Bar
 */
#if ENABLED(LCD_PROGRESS_BAR)
  #if DISABLED(SDSUPPORT)
    #error "LCD_PROGRESS_BAR requires SDSUPPORT."
  #elif ENABLED(DOGLCD)
    #error "LCD_PROGRESS_BAR does not apply to graphical displays."
  #elif ENABLED(FILAMENT_LCD_DISPLAY)
    #error "LCD_PROGRESS_BAR and FILAMENT_LCD_DISPLAY are not fully compatible. Comment out this line to use both."
  #endif
#endif

/**
 * Delta requirements
 */
#if ENABLED(DELTA)
  #if DISABLED(USE_XMAX_PLUG) && DISABLED(USE_YMAX_PLUG) && DISABLED(USE_ZMAX_PLUG)
    #error "You probably want to use Max Endstops for DELTA!"
  #endif
  #if ABL_GRID
    #if (ABL_GRID_POINTS_X & 1) == 0 || (ABL_GRID_POINTS_Y & 1) == 0
      #error "DELTA requires ABL_GRID_POINTS_X and ABL_GRID_POINTS_Y to be odd numbers."
    #elif ABL_GRID_POINTS_X < 3
      #error "DELTA requires ABL_GRID_POINTS_X and ABL_GRID_POINTS_Y to be 3 or higher."
    #endif
  #endif
#endif

/**
 * Babystepping
 */
#if ENABLED(BABYSTEPPING)
  #if DISABLED(ULTRA_LCD)
    #error "BABYSTEPPING requires an LCD controller."
  #endif
  #if ENABLED(SCARA)
    #error "BABYSTEPPING is not implemented for SCARA yet."
  #endif
  #if ENABLED(DELTA) && ENABLED(BABYSTEP_XY)
    #error "BABYSTEPPING only implemented for Z axis on deltabots."
  #endif
#endif

/**
 * Filament Runout needs a pin and either SD Support or Auto print start detection
 */
#if ENABLED(FILAMENT_RUNOUT_SENSOR)
  #if !HAS_FIL_RUNOUT
    #error "FILAMENT_RUNOUT_SENSOR requires FIL_RUNOUT_PIN."
  #elif DISABLED(SDSUPPORT) && DISABLED(PRINTJOB_TIMER_AUTOSTART)
    #error "FILAMENT_RUNOUT_SENSOR requires SDSUPPORT or PRINTJOB_TIMER_AUTOSTART."
  #endif
#endif

/**
 * Filament Change with Extruder Runout Prevention
 */
#if ENABLED(FILAMENT_CHANGE_FEATURE) && ENABLED(EXTRUDER_RUNOUT_PREVENT)
  #error "EXTRUDER_RUNOUT_PREVENT is incompatible with FILAMENT_CHANGE_FEATURE."
#endif

/**
 * Individual axis homing is useless for DELTAS
 */
#if ENABLED(INDIVIDUAL_AXIS_HOMING_MENU) && ENABLED(DELTA)
  #error "INDIVIDUAL_AXIS_HOMING_MENU is incompatible with DELTA kinematics."
#endif

/**
 * Options only for EXTRUDERS > 1
 */
#if EXTRUDERS > 1

  #if EXTRUDERS > 4
    #error "The maximum number of EXTRUDERS in Marlin is 4."
  #endif

  #if ENABLED(TEMP_SENSOR_1_AS_REDUNDANT)
    #error "EXTRUDERS must be 1 with TEMP_SENSOR_1_AS_REDUNDANT."
  #endif

  #if ENABLED(HEATERS_PARALLEL)
    #error "EXTRUDERS must be 1 with HEATERS_PARALLEL."
  #endif

#elif ENABLED(SINGLENOZZLE)
  #error "SINGLENOZZLE requires 2 or more EXTRUDERS."
#endif

/**
 * Only one type of extruder allowed
 */
#if (ENABLED(SWITCHING_EXTRUDER) && (ENABLED(SINGLENOZZLE) || ENABLED(MIXING_EXTRUDER))) \
  || (ENABLED(SINGLENOZZLE) && ENABLED(MIXING_EXTRUDER))
    #error "Please define only one type of extruder: SINGLENOZZLE, SWITCHING_EXTRUDER, or MIXING_EXTRUDER."
#endif

/**
 * Single Stepper Dual Extruder with switching servo
 */
#if ENABLED(SWITCHING_EXTRUDER)
  #if ENABLED(DUAL_X_CARRIAGE)
    #error "SWITCHING_EXTRUDER and DUAL_X_CARRIAGE are incompatible."
  #elif EXTRUDERS != 2
    #error "SWITCHING_EXTRUDER requires exactly 2 EXTRUDERS."
  #elif NUM_SERVOS < 1
    #error "SWITCHING_EXTRUDER requires NUM_SERVOS >= 1."
  #endif
#endif

/**
 * Mixing Extruder requirements
 */
#if ENABLED(MIXING_EXTRUDER)
  #if EXTRUDERS > 1
    #error "MIXING_EXTRUDER currently only supports one extruder."
  #endif
  #if MIXING_STEPPERS < 2
    #error "You must set MIXING_STEPPERS >= 2 for a mixing extruder."
  #endif
  #if ENABLED(FILAMENT_SENSOR)
    #error "MIXING_EXTRUDER is incompatible with FILAMENT_SENSOR. Comment out this line to use it anyway."
  #endif
#endif

/**
 * Limited number of servos
 */
#if NUM_SERVOS > 4
  #error "The maximum number of SERVOS in Marlin is 4."
#endif

/**
 * Servo deactivation depends on servo endstops
 */
#if ENABLED(DEACTIVATE_SERVOS_AFTER_MOVE) && !HAS_Z_SERVO_ENDSTOP
  #error "Z_ENDSTOP_SERVO_NR is required for DEACTIVATE_SERVOS_AFTER_MOVE."
#endif

/**
 * Required LCD language
 */
#if DISABLED(DOGLCD) && ENABLED(ULTRA_LCD) && !defined(DISPLAY_CHARSET_HD44780)
  #error "You must set DISPLAY_CHARSET_HD44780 to JAPANESE, WESTERN or CYRILLIC for your LCD controller."
#endif

/**
 * Bed Heating Options - PID vs Limit Switching
 */
#if ENABLED(PIDTEMPBED) && ENABLED(BED_LIMIT_SWITCHING)
  #error "To use BED_LIMIT_SWITCHING you must disable PIDTEMPBED."
#endif

/**
 * Allow only one bed leveling option to be defined
 */
#if HAS_ABL
  #define COUNT_LEV_1 0
  #if ENABLED(AUTO_BED_LEVELING_LINEAR)
    #define COUNT_LEV_2 INCREMENT(COUNT_LEV_1)
  #else
    #define COUNT_LEV_2 COUNT_LEV_1
  #endif
  #if ENABLED(AUTO_BED_LEVELING_3POINT)
    #define COUNT_LEV_3 INCREMENT(COUNT_LEV_2)
  #else
    #define COUNT_LEV_3 COUNT_LEV_2
  #endif
  #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
    #define COUNT_LEV_4 INCREMENT(COUNT_LEV_3)
  #else
    #define COUNT_LEV_4 COUNT_LEV_3
  #endif
  #if ENABLED(MESH_BED_LEVELING)
    #define COUNT_LEV_5 INCREMENT(COUNT_LEV_4)
  #else
    #define COUNT_LEV_5 COUNT_LEV_4
  #endif
  #if COUNT_LEV_5 > 1
    #error "Select only one of: MESH_BED_LEVELING, AUTO_BED_LEVELING_LINEAR, AUTO_BED_LEVELING_3POINT, or AUTO_BED_LEVELING_BILINEAR."
  #endif
#endif

/**
 * Mesh Bed Leveling
 */
#if ENABLED(MESH_BED_LEVELING)
  #if ENABLED(DELTA)
    #error "MESH_BED_LEVELING does not yet support DELTA printers."
  #elif MESH_NUM_X_POINTS > 9 || MESH_NUM_Y_POINTS > 9
    #error "MESH_NUM_X_POINTS and MESH_NUM_Y_POINTS must be less than 10."
  #endif
#elif ENABLED(MANUAL_BED_LEVELING)
  #error "MANUAL_BED_LEVELING only applies to MESH_BED_LEVELING."
#endif

/**
 * Probes
 */
#if PROBE_SELECTED

  /**
   * Allow only one probe option to be defined
   */
  #define COUNT_PROBE_1 0
  #if ENABLED(FIX_MOUNTED_PROBE)
    #define COUNT_PROBE_2 INCREMENT(COUNT_PROBE_1)
  #else
    #define COUNT_PROBE_2 COUNT_PROBE_1
  #endif
  #if HAS_Z_SERVO_ENDSTOP && DISABLED(BLTOUCH)
    #define COUNT_PROBE_3 INCREMENT(COUNT_PROBE_2)
  #else
    #define COUNT_PROBE_3 COUNT_PROBE_2
  #endif
  #if ENABLED(BLTOUCH)
    #define COUNT_PROBE_4 INCREMENT(COUNT_PROBE_3)
  #else
    #define COUNT_PROBE_4 COUNT_PROBE_3
  #endif
  #if ENABLED(Z_PROBE_ALLEN_KEY)
    #define COUNT_PROBE_5 INCREMENT(COUNT_PROBE_4)
  #else
    #define COUNT_PROBE_5 COUNT_PROBE_4
  #endif
  #if ENABLED(Z_PROBE_SLED)
    #define COUNT_PROBE_6 INCREMENT(COUNT_PROBE_5)
  #else
    #define COUNT_PROBE_6 COUNT_PROBE_5
  #endif
  #if COUNT_PROBE_6 > 1
    #error "Please enable only one probe: FIX_MOUNTED_PROBE, Z Servo, BLTOUCH, Z_PROBE_ALLEN_KEY, or Z_PROBE_SLED."
  #endif

  /**
   * Z_PROBE_SLED is incompatible with DELTA
   */
  #if ENABLED(Z_PROBE_SLED) && ENABLED(DELTA)
    #error "You cannot use Z_PROBE_SLED with DELTA."
  #endif

  /**
   * NUM_SERVOS is required for a Z servo probe
   */
  #if HAS_Z_SERVO_ENDSTOP
    #ifndef NUM_SERVOS
      #error "You must set NUM_SERVOS for a Z servo probe (Z_ENDSTOP_SERVO_NR)."
    #elif Z_ENDSTOP_SERVO_NR >= NUM_SERVOS
      #error "Z_ENDSTOP_SERVO_NR must be smaller than NUM_SERVOS."
    #endif
  #endif

  /**
   * Require pin options and pins to be defined
   */
  #if ENABLED(Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN)
    #if ENABLED(Z_MIN_PROBE_ENDSTOP)
      #error "Enable only one option: Z_MIN_PROBE_ENDSTOP or Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN."
    #elif DISABLED(USE_ZMIN_PLUG)
      #error "Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN requires USE_ZMIN_PLUG to be enabled."
    #elif !HAS_Z_MIN
      #error "Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN requires the Z_MIN_PIN to be defined."
    #endif
  #elif ENABLED(Z_MIN_PROBE_ENDSTOP)
    #if !HAS_Z_MIN_PROBE_PIN
      #error "Z_MIN_PROBE_ENDSTOP requires the Z_MIN_PROBE_PIN to be defined."
    #endif
  #else
    #error "You must enable either Z_MIN_PROBE_ENDSTOP or Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN to use a probe."
  #endif

  /**
   * Make sure Z raise values are set
   */
  #if !defined(Z_CLEARANCE_DEPLOY_PROBE)
    #error "You must define Z_CLEARANCE_DEPLOY_PROBE in your configuration."
  #elif !defined(Z_CLEARANCE_BETWEEN_PROBES)
    #error "You must define Z_CLEARANCE_BETWEEN_PROBES in your configuration."
  #elif Z_CLEARANCE_DEPLOY_PROBE < 0
    #error "Probes need Z_CLEARANCE_DEPLOY_PROBE >= 0."
  #elif Z_CLEARANCE_BETWEEN_PROBES < 0
    #error "Probes need Z_CLEARANCE_BETWEEN_PROBES >= 0."
  #endif

#else

  /**
   * Require some kind of probe for bed leveling and probe testing
   */
  #if HAS_ABL
    #error "Auto Bed Leveling requires a probe! Define a Z Servo, Z_PROBE_ALLEN_KEY, Z_PROBE_SLED, or FIX_MOUNTED_PROBE."
  #elif ENABLED(Z_MIN_PROBE_REPEATABILITY_TEST)
    #error "Z_MIN_PROBE_REPEATABILITY_TEST requires a probe! Define a Z Servo, Z_PROBE_ALLEN_KEY, Z_PROBE_SLED, or FIX_MOUNTED_PROBE."
  #endif

#endif

/**
 * Homing Bump
 */
#if X_HOME_BUMP_MM < 0 || Y_HOME_BUMP_MM < 0 || Z_HOME_BUMP_MM < 0
  #error "[XYZ]_HOME_BUMP_MM must be greater than or equal to 0."
#endif

/**
 * Make sure Z_SAFE_HOMING point is reachable
 */
#if ENABLED(Z_SAFE_HOMING)
  #if Z_SAFE_HOMING_X_POINT < MIN_PROBE_X || Z_SAFE_HOMING_X_POINT > MAX_PROBE_X
    #if HAS_BED_PROBE
      #error "Z_SAFE_HOMING_X_POINT can't be reached by the Z probe."
    #else
      #error "Z_SAFE_HOMING_X_POINT can't be reached by the nozzle."
    #endif
  #elif Z_SAFE_HOMING_Y_POINT < MIN_PROBE_Y || Z_SAFE_HOMING_Y_POINT > MAX_PROBE_Y
    #if HAS_BED_PROBE
      #error "Z_SAFE_HOMING_Y_POINT can't be reached by the Z probe."
    #else
      #error "Z_SAFE_HOMING_Y_POINT can't be reached by the nozzle."
    #endif
  #endif
#endif // Z_SAFE_HOMING

/**
 * Auto Bed Leveling
 */
#if HAS_ABL

  /**
   * Delta and SCARA have limited bed leveling options
   */
  #if DISABLED(AUTO_BED_LEVELING_BILINEAR)
    #if ENABLED(DELTA)
      #error "Only AUTO_BED_LEVELING_BILINEAR is supported for DELTA bed leveling."
    #elif ENABLED(SCARA)
      #error "Only AUTO_BED_LEVELING_BILINEAR is supported for SCARA bed leveling."
    #endif
  #endif

  /**
   * Check if Probe_Offset * Grid Points is greater than Probing Range
   */
  #if ABL_GRID
    #ifndef DELTA_PROBEABLE_RADIUS
      // Be sure points are in the right order
      #if LEFT_PROBE_BED_POSITION > RIGHT_PROBE_BED_POSITION
        #error "LEFT_PROBE_BED_POSITION must be less than RIGHT_PROBE_BED_POSITION."
      #elif FRONT_PROBE_BED_POSITION > BACK_PROBE_BED_POSITION
        #error "FRONT_PROBE_BED_POSITION must be less than BACK_PROBE_BED_POSITION."
      #endif
      // Make sure probing points are reachable
      #if LEFT_PROBE_BED_POSITION < MIN_PROBE_X
        #error "The given LEFT_PROBE_BED_POSITION can't be reached by the Z probe."
      #elif RIGHT_PROBE_BED_POSITION > MAX_PROBE_X
        #error "The given RIGHT_PROBE_BED_POSITION can't be reached by the Z probe."
      #elif FRONT_PROBE_BED_POSITION < MIN_PROBE_Y
        #error "The given FRONT_PROBE_BED_POSITION can't be reached by the Z probe."
      #elif BACK_PROBE_BED_POSITION > MAX_PROBE_Y
        #error "The given BACK_PROBE_BED_POSITION can't be reached by the Z probe."
      #endif
    #endif
  #else // !ABL_GRID

    // Check the triangulation points
    #if ABL_PROBE_PT_1_X < MIN_PROBE_X || ABL_PROBE_PT_1_X > MAX_PROBE_X
      #error "The given ABL_PROBE_PT_1_X can't be reached by the Z probe."
    #elif ABL_PROBE_PT_2_X < MIN_PROBE_X || ABL_PROBE_PT_2_X > MAX_PROBE_X
      #error "The given ABL_PROBE_PT_2_X can't be reached by the Z probe."
    #elif ABL_PROBE_PT_3_X < MIN_PROBE_X || ABL_PROBE_PT_3_X > MAX_PROBE_X
      #error "The given ABL_PROBE_PT_3_X can't be reached by the Z probe."
    #elif ABL_PROBE_PT_1_Y < MIN_PROBE_Y || ABL_PROBE_PT_1_Y > MAX_PROBE_Y
      #error "The given ABL_PROBE_PT_1_Y can't be reached by the Z probe."
    #elif ABL_PROBE_PT_2_Y < MIN_PROBE_Y || ABL_PROBE_PT_2_Y > MAX_PROBE_Y
      #error "The given ABL_PROBE_PT_2_Y can't be reached by the Z probe."
    #elif ABL_PROBE_PT_3_Y < MIN_PROBE_Y || ABL_PROBE_PT_3_Y > MAX_PROBE_Y
      #error "The given ABL_PROBE_PT_3_Y can't be reached by the Z probe."
    #endif

  #endif // !ABL_GRID

#endif // HAS_ABL

/**
 * Advance Extrusion
 */
#if ENABLED(ADVANCE) && ENABLED(LIN_ADVANCE)
  #error "You can enable ADVANCE or LIN_ADVANCE, but not both."
#endif

/**
 * Filament Width Sensor
 */
#if ENABLED(FILAMENT_WIDTH_SENSOR) && !HAS_FILAMENT_WIDTH_SENSOR
  #error "FILAMENT_WIDTH_SENSOR requires a FILWIDTH_PIN to be defined."
#endif

/**
 * ULTIPANEL encoder
 */
#if ENABLED(ULTIPANEL) && DISABLED(NEWPANEL) && DISABLED(SR_LCD_2W_NL) && !defined(SHIFT_CLK)
  #error "ULTIPANEL requires some kind of encoder."
#endif

#if ENCODER_PULSES_PER_STEP < 0
  #error "ENCODER_PULSES_PER_STEP should not be negative, use REVERSE_MENU_DIRECTION instead."
#endif

/**
 * SAV_3DGLCD display options
 */
#if ENABLED(U8GLIB_SSD1306) && ENABLED(U8GLIB_SH1106)
  #error "Only enable one SAV_3DGLCD display type: U8GLIB_SSD1306 or U8GLIB_SH1106."
#endif

/**
 * Don't set more than one kinematic type
 */
#define COUNT_KIN_1 0
#if ENABLED(DELTA)
  #define COUNT_KIN_2 INCREMENT(COUNT_KIN_1)
#else
  #define COUNT_KIN_2 COUNT_KIN_1
#endif
#if ENABLED(MORGAN_SCARA)
  #define COUNT_KIN_3 INCREMENT(COUNT_KIN_2)
#else
  #define COUNT_KIN_3 COUNT_KIN_2
#endif
#if ENABLED(MAKERARM_SCARA)
  #define COUNT_KIN_4 INCREMENT(COUNT_KIN_3)
#else
  #define COUNT_KIN_4 COUNT_KIN_3
#endif
#if ENABLED(COREXY)
  #define COUNT_KIN_5 INCREMENT(COUNT_KIN_4)
#else
  #define COUNT_KIN_5 COUNT_KIN_4
#endif
#if ENABLED(COREXZ)
  #define COUNT_KIN_6 INCREMENT(COUNT_KIN_5)
#else
  #define COUNT_KIN_6 COUNT_KIN_5
#endif
#if ENABLED(COREYZ)
  #define COUNT_KIN_7 INCREMENT(COUNT_KIN_6)
#else
  #define COUNT_KIN_7 COUNT_KIN_6
#endif
#if COUNT_KIN_7 > 1
  #error "Please enable only one of DELTA, MORGAN_SCARA, MAKERARM_SCARA, COREXY, COREXZ, or COREYZ."
#endif

/**
 * Allen Key
 * Deploying the Allen Key probe uses big moves in z direction. Too dangerous for an unhomed z-axis.
 */
#if ENABLED(Z_PROBE_ALLEN_KEY) && (Z_HOME_DIR < 0) && ENABLED(Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN)
  #error "You can't home to a z min endstop with a Z_PROBE_ALLEN_KEY"
#endif

/**
 * Dual X Carriage requirements
 */
#if ENABLED(DUAL_X_CARRIAGE)
  #if EXTRUDERS == 1
    #error "DUAL_X_CARRIAGE requires 2 (or more) extruders."
  #elif ENABLED(COREXY) || ENABLED(COREXZ)
    #error "DUAL_X_CARRIAGE cannot be used with COREXY or COREXZ."
  #elif !HAS_X2_ENABLE || !HAS_X2_STEP || !HAS_X2_DIR
    #error "DUAL_X_CARRIAGE requires X2 stepper pins to be defined."
  #elif !HAS_X_MAX
    #error "DUAL_X_CARRIAGE requires USE_XMAX_PLUG and an X Max Endstop."
  #elif !defined(X2_HOME_POS) || !defined(X2_MIN_POS) || !defined(X2_MAX_POS)
    #error "DUAL_X_CARRIAGE requires X2_HOME_POS, X2_MIN_POS, and X2_MAX_POS."
  #elif X_HOME_DIR != -1 || X2_HOME_DIR != 1
    #error "DUAL_X_CARRIAGE requires X_HOME_DIR -1 and X2_HOME_DIR 1."
  #endif
#endif // DUAL_X_CARRIAGE

/**
 * Make sure auto fan pins don't conflict with the fan pin
 */
#if HAS_AUTO_FAN
  #if HAS_FAN0
    #if EXTRUDER_0_AUTO_FAN_PIN == FAN_PIN
      #error "You cannot set EXTRUDER_0_AUTO_FAN_PIN equal to FAN_PIN."
    #elif EXTRUDER_1_AUTO_FAN_PIN == FAN_PIN
      #error "You cannot set EXTRUDER_1_AUTO_FAN_PIN equal to FAN_PIN."
    #elif EXTRUDER_2_AUTO_FAN_PIN == FAN_PIN
      #error "You cannot set EXTRUDER_2_AUTO_FAN_PIN equal to FAN_PIN."
    #elif EXTRUDER_3_AUTO_FAN_PIN == FAN_PIN
      #error "You cannot set EXTRUDER_3_AUTO_FAN_PIN equal to FAN_PIN."
    #endif
  #endif
#endif

#if HAS_FAN0 && CONTROLLERFAN_PIN == FAN_PIN
  #error "You cannot set CONTROLLERFAN_PIN equal to FAN_PIN."
#endif

#if HAS_CONTROLLERFAN
  #if EXTRUDER_0_AUTO_FAN_PIN == CONTROLLERFAN_PIN
    #error "You cannot set EXTRUDER_0_AUTO_FAN_PIN equal to CONTROLLERFAN_PIN."
  #elif EXTRUDER_1_AUTO_FAN_PIN == CONTROLLERFAN_PIN
    #error "You cannot set EXTRUDER_1_AUTO_FAN_PIN equal to CONTROLLERFAN_PIN."
  #elif EXTRUDER_2_AUTO_FAN_PIN == CONTROLLERFAN_PIN
    #error "You cannot set EXTRUDER_2_AUTO_FAN_PIN equal to CONTROLLERFAN_PIN."
  #elif EXTRUDER_3_AUTO_FAN_PIN == CONTROLLERFAN_PIN
    #error "You cannot set EXTRUDER_3_AUTO_FAN_PIN equal to CONTROLLERFAN_PIN."
  #endif
#endif

/**
 * Test Heater, Temp Sensor, and Extruder Pins; Sensor Type must also be set.
 */
#if !HAS_HEATER_0
  #error "HEATER_0_PIN not defined for this board."
#elif !PIN_EXISTS(TEMP_0)
  #error "TEMP_0_PIN not defined for this board."
#elif !PIN_EXISTS(E0_STEP) || !PIN_EXISTS(E0_DIR) || !PIN_EXISTS(E0_ENABLE)
  #error "E0_STEP_PIN, E0_DIR_PIN, or E0_ENABLE_PIN not defined for this board."
#elif TEMP_SENSOR_0 == 0
  #error "TEMP_SENSOR_0 is required."
#endif

#if HOTENDS > 1 || ENABLED(HEATERS_PARALLEL)
  #if !HAS_HEATER_1
    #error "HEATER_1_PIN not defined for this board."
  #endif
#endif

#if HOTENDS > 1
  #if TEMP_SENSOR_1 == 0
    #error "TEMP_SENSOR_1 is required with 2 or more HOTENDS."
  #elif !PIN_EXISTS(TEMP_1)
    #error "TEMP_1_PIN not defined for this board."
  #endif
  #if HOTENDS > 2
    #if TEMP_SENSOR_2 == 0
      #error "TEMP_SENSOR_2 is required with 3 or more HOTENDS."
    #elif !HAS_HEATER_2
      #error "HEATER_2_PIN not defined for this board."
    #elif !PIN_EXISTS(TEMP_2)
      #error "TEMP_2_PIN not defined for this board."
    #endif
    #if HOTENDS > 3
      #if TEMP_SENSOR_3 == 0
        #error "TEMP_SENSOR_3 is required with 4 HOTENDS."
      #elif !HAS_HEATER_3
        #error "HEATER_3_PIN not defined for this board."
      #elif !PIN_EXISTS(TEMP_3)
        #error "TEMP_3_PIN not defined for this board."
      #endif
    #elif TEMP_SENSOR_3 != 0
      #error "TEMP_SENSOR_3 shouldn't be set with only 3 extruders."
    #endif
  #elif TEMP_SENSOR_2 != 0
    #error "TEMP_SENSOR_2 shouldn't be set with only 2 extruders."
  #elif TEMP_SENSOR_3 != 0
    #error "TEMP_SENSOR_3 shouldn't be set with only 2 extruders."
  #endif
#elif TEMP_SENSOR_1 != 0 && DISABLED(TEMP_SENSOR_1_AS_REDUNDANT)
  #error "TEMP_SENSOR_1 shouldn't be set with only 1 extruder."
#elif TEMP_SENSOR_2 != 0
  #error "TEMP_SENSOR_2 shouldn't be set with only 1 extruder."
#elif TEMP_SENSOR_3 != 0
  #error "TEMP_SENSOR_3 shouldn't be set with only 1 extruder."
#endif

#if ENABLED(TEMP_SENSOR_1_AS_REDUNDANT) && TEMP_SENSOR_1 == 0
  #error "TEMP_SENSOR_1 is required with TEMP_SENSOR_1_AS_REDUNDANT."
#endif

/**
 * Temperature status LEDs
 */
#if ENABLED(TEMP_STAT_LEDS) && !(PIN_EXISTS(STAT_LED_RED) && PIN_EXISTS(STAT_LED_BLUE))
  #error "TEMP_STAT_LEDS requires STAT_LED_RED_PIN and STAT_LED_BLUE_PIN."
#endif

/**
 * Basic 2-nozzle duplication mode
 */
#if ENABLED(DUAL_NOZZLE_DUPLICATION_MODE)
  #if HOTENDS != 2
    #error "DUAL_NOZZLE_DUPLICATION_MODE requires exactly 2 hotends."
  #elif ENABLED(DUAL_X_CARRIAGE)
    #error "DUAL_NOZZLE_DUPLICATION_MODE is incompatible with DUAL_X_CARRIAGE."
  #elif ENABLED(SINGLENOZZLE)
    #error "DUAL_NOZZLE_DUPLICATION_MODE is incompatible with SINGLENOZZLE."
  #elif ENABLED(MIXING_EXTRUDER)
    #error "DUAL_NOZZLE_DUPLICATION_MODE is incompatible with MIXING_EXTRUDER."
  #elif ENABLED(SWITCHING_EXTRUDER)
    #error "DUAL_NOZZLE_DUPLICATION_MODE is incompatible with SWITCHING_EXTRUDER."
  #endif
#endif

/**
 * Test Extruder Pins
 */
#if EXTRUDERS > 3
  #if !PIN_EXISTS(E3_STEP) || !PIN_EXISTS(E3_DIR) || !PIN_EXISTS(E3_ENABLE)
    #error "E3_STEP_PIN, E3_DIR_PIN, or E3_ENABLE_PIN not defined for this board."
  #endif
#elif EXTRUDERS > 2
  #if !PIN_EXISTS(E2_STEP) || !PIN_EXISTS(E2_DIR) || !PIN_EXISTS(E2_ENABLE)
    #error "E2_STEP_PIN, E2_DIR_PIN, or E2_ENABLE_PIN not defined for this board."
  #endif
#elif EXTRUDERS > 1
  #if !PIN_EXISTS(E1_STEP) || !PIN_EXISTS(E1_DIR) || !PIN_EXISTS(E1_ENABLE)
    #error "E1_STEP_PIN, E1_DIR_PIN, or E1_ENABLE_PIN not defined for this board."
  #endif
#endif

/**
 * Endstops
 */
#if DISABLED(USE_XMIN_PLUG) && DISABLED(USE_XMAX_PLUG) && !(ENABLED(Z_DUAL_ENDSTOPS) && Z2_USE_ENDSTOP >= _XMAX_ && Z2_USE_ENDSTOP <= _XMIN_)
 #error "You must enable USE_XMIN_PLUG or USE_XMAX_PLUG."
#elif DISABLED(USE_YMIN_PLUG) && DISABLED(USE_YMAX_PLUG) && !(ENABLED(Z_DUAL_ENDSTOPS) && Z2_USE_ENDSTOP >= _YMAX_ && Z2_USE_ENDSTOP <= _YMIN_)
 #error "You must enable USE_YMIN_PLUG or USE_YMAX_PLUG."
#elif DISABLED(USE_ZMIN_PLUG) && DISABLED(USE_ZMAX_PLUG) && !(ENABLED(Z_DUAL_ENDSTOPS) && Z2_USE_ENDSTOP >= _ZMAX_ && Z2_USE_ENDSTOP <= _ZMIN_)
 #error "You must enable USE_ZMIN_PLUG or USE_ZMAX_PLUG."
#elif ENABLED(Z_DUAL_ENDSTOPS)
  #if !Z2_USE_ENDSTOP
    #error "You must set Z2_USE_ENDSTOP with Z_DUAL_ENDSTOPS."
  #elif ENABLED(DELTA)
    #error "Z_DUAL_ENDSTOPS is not compatible with DELTA."
  #endif
#elif !IS_SCARA
  #if X_HOME_DIR < 0 && DISABLED(USE_XMIN_PLUG)
    #error "Enable USE_XMIN_PLUG when homing X to MIN."
  #elif X_HOME_DIR > 0 && DISABLED(USE_XMAX_PLUG)
    #error "Enable USE_XMAX_PLUG when homing X to MAX."
  #elif Y_HOME_DIR < 0 && DISABLED(USE_YMIN_PLUG)
    #error "Enable USE_YMIN_PLUG when homing Y to MIN."
  #elif Y_HOME_DIR > 0 && DISABLED(USE_YMAX_PLUG)
    #error "Enable USE_YMAX_PLUG when homing Y to MAX."
  #elif Z_HOME_DIR < 0 && DISABLED(USE_ZMIN_PLUG)
    #error "Enable USE_ZMIN_PLUG when homing Z to MIN."
  #elif Z_HOME_DIR > 0 && DISABLED(USE_ZMAX_PLUG)
    #error "Enable USE_ZMAX_PLUG when homing Z to MAX."
  #endif
#endif

/**
 * emergency-command parser
 */
#if ENABLED(EMERGENCY_PARSER) && defined(USBCON)
  #error "EMERGENCY_PARSER does not work on boards with AT90USB processors (USBCON)."
#endif

/**
 * I2C bus
 */
#if ENABLED(EXPERIMENTAL_I2CBUS) && I2C_SLAVE_ADDRESS > 0
  #if I2C_SLAVE_ADDRESS < 8
    #error "I2C_SLAVE_ADDRESS can't be less than 8. (Addresses 0 - 7 are reserved.)"
  #elif I2C_SLAVE_ADDRESS > 127
    #error "I2C_SLAVE_ADDRESS can't be over 127. (Only 7 bits allowed.)"
  #endif
#endif

/**
 * G38 Probe Target
 */
#if ENABLED(G38_PROBE_TARGET)
  #if !HAS_BED_PROBE
    #error "G38_PROBE_TARGET requires a bed probe."
  #elif !IS_CARTESIAN
    #error "G38_PROBE_TARGET requires a Cartesian machine."
  #endif
#endif

/**
 * Make sure only one display is enabled
 *
 * Note: BQ_LCD_SMART_CONTROLLER => REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER
 *       REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER => REPRAP_DISCOUNT_SMART_CONTROLLER
 *       SAV_3DGLCD => U8GLIB_SH1106 => ULTIMAKERCONTROLLER
 *       miniVIKI => ULTIMAKERCONTROLLER
 *       VIKI2 => ULTIMAKERCONTROLLER
 *       ELB_FULL_GRAPHIC_CONTROLLER => ULTIMAKERCONTROLLER
 *       PANEL_ONE => ULTIMAKERCONTROLLER
 */
#define COUNT_LCD_1 0
#if ENABLED(ULTIMAKERCONTROLLER) \
    && DISABLED(SAV_3DGLCD) && DISABLED(miniVIKI) && DISABLED(VIKI2) \
    && DISABLED(ELB_FULL_GRAPHIC_CONTROLLER) && DISABLED(PANEL_ONE)
  #define COUNT_LCD_2 INCREMENT(COUNT_LCD_1)
#else
  #define COUNT_LCD_2 COUNT_LCD_1
#endif
#if ENABLED(REPRAP_DISCOUNT_SMART_CONTROLLER) && DISABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER)
  #define COUNT_LCD_3 INCREMENT(COUNT_LCD_2)
#else
  #define COUNT_LCD_3 COUNT_LCD_2
#endif
#if ENABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER) && DISABLED(BQ_LCD_SMART_CONTROLLER)
  #define COUNT_LCD_4 INCREMENT(COUNT_LCD_3)
#else
  #define COUNT_LCD_4 COUNT_LCD_3
#endif
#if ENABLED(CARTESIO_UI)
  #define COUNT_LCD_5 INCREMENT(COUNT_LCD_4)
#else
  #define COUNT_LCD_5 COUNT_LCD_4
#endif
#if ENABLED(PANEL_ONE)
  #define COUNT_LCD_6 INCREMENT(COUNT_LCD_5)
#else
  #define COUNT_LCD_6 COUNT_LCD_5
#endif
#if ENABLED(MAKRPANEL)
  #define COUNT_LCD_7 INCREMENT(COUNT_LCD_6)
#else
  #define COUNT_LCD_7 COUNT_LCD_6
#endif
#if ENABLED(REPRAPWORLD_GRAPHICAL_LCD)
  #define COUNT_LCD_8 INCREMENT(COUNT_LCD_7)
#else
  #define COUNT_LCD_8 COUNT_LCD_7
#endif
#if ENABLED(VIKI2)
  #define COUNT_LCD_9 INCREMENT(COUNT_LCD_8)
#else
  #define COUNT_LCD_9 COUNT_LCD_8
#endif
#if ENABLED(miniVIKI)
  #define COUNT_LCD_10 INCREMENT(COUNT_LCD_9)
#else
  #define COUNT_LCD_10 COUNT_LCD_9
#endif
#if ENABLED(ELB_FULL_GRAPHIC_CONTROLLER)
  #define COUNT_LCD_11 INCREMENT(COUNT_LCD_10)
#else
  #define COUNT_LCD_11 COUNT_LCD_10
#endif
#if ENABLED(G3D_PANEL)
  #define COUNT_LCD_12 INCREMENT(COUNT_LCD_11)
#else
  #define COUNT_LCD_12 COUNT_LCD_11
#endif
#if ENABLED(MINIPANEL)
  #define COUNT_LCD_13 INCREMENT(COUNT_LCD_12)
#else
  #define COUNT_LCD_13 COUNT_LCD_12
#endif
#if ENABLED(REPRAPWORLD_KEYPAD)
  #define COUNT_LCD_14 INCREMENT(COUNT_LCD_13)
#else
  #define COUNT_LCD_14 COUNT_LCD_13
#endif
#if ENABLED(RIGIDBOT_PANEL)
  #define COUNT_LCD_15 INCREMENT(COUNT_LCD_14)
#else
  #define COUNT_LCD_15 COUNT_LCD_14
#endif
#if ENABLED(RA_CONTROL_PANEL)
  #define COUNT_LCD_16 INCREMENT(COUNT_LCD_15)
#else
  #define COUNT_LCD_16 COUNT_LCD_15
#endif
#if ENABLED(LCD_I2C_SAINSMART_YWROBOT)
  #define COUNT_LCD_17 INCREMENT(COUNT_LCD_16)
#else
  #define COUNT_LCD_17 COUNT_LCD_16
#endif
#if ENABLED(LCM1602)
  #define COUNT_LCD_18 INCREMENT(COUNT_LCD_17)
#else
  #define COUNT_LCD_18 COUNT_LCD_17
#endif
#if ENABLED(LCD_I2C_PANELOLU2)
  #define COUNT_LCD_19 INCREMENT(COUNT_LCD_18)
#else
  #define COUNT_LCD_19 COUNT_LCD_18
#endif
#if ENABLED(LCD_I2C_VIKI)
  #define COUNT_LCD_20 INCREMENT(COUNT_LCD_19)
#else
  #define COUNT_LCD_20 COUNT_LCD_19
#endif
#if ENABLED(U8GLIB_SSD1306)
  #define COUNT_LCD_21 INCREMENT(COUNT_LCD_20)
#else
  #define COUNT_LCD_21 COUNT_LCD_20
#endif
#if ENABLED(SAV_3DLCD)
  #define COUNT_LCD_22 INCREMENT(COUNT_LCD_21)
#else
  #define COUNT_LCD_22 COUNT_LCD_21
#endif
#if ENABLED(BQ_LCD_SMART_CONTROLLER)
  #define COUNT_LCD_23 INCREMENT(COUNT_LCD_22)
#else
  #define COUNT_LCD_23 COUNT_LCD_22
#endif
#if ENABLED(SAV_3DGLCD)
  #define COUNT_LCD_24 INCREMENT(COUNT_LCD_23)
#else
  #define COUNT_LCD_24 COUNT_LCD_23
#endif
#if COUNT_LCD_24 > 1
  #error "Please select no more than one LCD controller option."
#endif
