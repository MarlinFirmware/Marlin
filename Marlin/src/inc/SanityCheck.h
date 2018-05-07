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

#ifndef _SANITYCHECK_H_
#define _SANITYCHECK_H_

/**
 * Require gcc 4.7 or newer (first included with Arduino 1.6.8) for C++11 features.
 */
#if __cplusplus < 201103L
  #error "Marlin requires C++11 support (gcc >= 4.7, Arduino IDE >= 1.6.8). Please upgrade your toolchain."
#endif

/**
 * We try our best to include sanity checks for all changed configuration
 * directives because users have a tendency to use outdated config files with
 * the bleeding-edge source code, but sometimes this is not enough. This check
 * forces a minimum config file revision. Otherwise Marlin will not build.
 */
#define HEXIFY(H) _CAT(0x,H)
#if !defined(CONFIGURATION_H_VERSION) || HEXIFY(CONFIGURATION_H_VERSION) < HEXIFY(REQUIRED_CONFIGURATION_H_VERSION)
  #error "You are using an old Configuration.h file, update it before building Marlin."
#endif

#if !defined(CONFIGURATION_ADV_H_VERSION) || HEXIFY(CONFIGURATION_ADV_H_VERSION) < HEXIFY(REQUIRED_CONFIGURATION_ADV_H_VERSION)
  #error "You are using an old Configuration_adv.h file, update it before building Marlin."
#endif

/**
 * Warnings for old configurations
 */
#if !defined(X_BED_SIZE) || !defined(Y_BED_SIZE)
  #error "X_BED_SIZE and Y_BED_SIZE are now required! Please update your configuration."
#elif WATCH_TEMP_PERIOD > 500
  #error "WATCH_TEMP_PERIOD now uses seconds instead of milliseconds."
#elif DISABLED(THERMAL_PROTECTION_HOTENDS) && (defined(WATCH_TEMP_PERIOD) || defined(THERMAL_PROTECTION_PERIOD))
  #error "Thermal Runaway Protection for hotends is now enabled with THERMAL_PROTECTION_HOTENDS."
#elif DISABLED(THERMAL_PROTECTION_BED) && defined(THERMAL_PROTECTION_BED_PERIOD)
  #error "Thermal Runaway Protection for the bed is now enabled with THERMAL_PROTECTION_BED."
#elif (CORE_IS_XZ || CORE_IS_YZ) && ENABLED(Z_LATE_ENABLE)
  #error "Z_LATE_ENABLE can't be used with COREXZ, COREZX, COREYZ, or COREZY."
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
  #error "FILAMENT_SENSOR is now FILAMENT_WIDTH_SENSOR. Please update your configuration."
#elif defined(ENDSTOPPULLUP_FIL_RUNOUT)
  #error "ENDSTOPPULLUP_FIL_RUNOUT is now FIL_RUNOUT_PULLUP. Please update your configuration."
#elif defined(DISABLE_MAX_ENDSTOPS) || defined(DISABLE_MIN_ENDSTOPS)
  #error "DISABLE_MAX_ENDSTOPS and DISABLE_MIN_ENDSTOPS deprecated. Use individual USE_*_PLUG options instead."
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
#elif defined(Z_ENDSTOP_SERVO_NR)
  #error "Z_ENDSTOP_SERVO_NR is now Z_PROBE_SERVO_NR. Please update your configuration."
#elif defined(DEFAULT_XYJERK)
  #error "DEFAULT_XYJERK is deprecated. Use DEFAULT_XJERK and DEFAULT_YJERK instead."
#elif defined(XY_TRAVEL_SPEED)
  #error "XY_TRAVEL_SPEED is deprecated. Use XY_PROBE_SPEED instead."
#elif defined(PROBE_SERVO_DEACTIVATION_DELAY)
  #error "PROBE_SERVO_DEACTIVATION_DELAY is deprecated. Use DEACTIVATE_SERVOS_AFTER_MOVE instead."
#elif defined(SERVO_DEACTIVATION_DELAY)
  #error "SERVO_DEACTIVATION_DELAY is deprecated. Use SERVO_DELAY instead."
#elif ENABLED(FILAMENTCHANGEENABLE)
  #error "FILAMENTCHANGEENABLE is now ADVANCED_PAUSE_FEATURE. Please update your configuration."
#elif ENABLED(FILAMENT_CHANGE_FEATURE)
  #error "FILAMENT_CHANGE_FEATURE is now ADVANCED_PAUSE_FEATURE. Please update your configuration."
#elif defined(FILAMENT_CHANGE_X_POS) || defined(FILAMENT_CHANGE_Y_POS)
  #error "FILAMENT_CHANGE_[XY]_POS is now set with NOZZLE_PARK_POINT. Please update your configuration."
#elif defined(FILAMENT_CHANGE_Z_ADD)
  #error "FILAMENT_CHANGE_Z_ADD is now set with NOZZLE_PARK_POINT. Please update your configuration."
#elif defined(FILAMENT_CHANGE_XY_FEEDRATE)
  #error "FILAMENT_CHANGE_XY_FEEDRATE is now NOZZLE_PARK_XY_FEEDRATE. Please update your configuration."
#elif defined(FILAMENT_CHANGE_Z_FEEDRATE)
  #error "FILAMENT_CHANGE_Z_FEEDRATE is now NOZZLE_PARK_Z_FEEDRATE. Please update your configuration."
#elif defined(PAUSE_PARK_X_POS) || defined(PAUSE_PARK_Y_POS)
  #error "PAUSE_PARK_[XY]_POS is now set with NOZZLE_PARK_POINT. Please update your configuration."
#elif defined(PAUSE_PARK_Z_ADD)
  #error "PAUSE_PARK_Z_ADD is now set with NOZZLE_PARK_POINT. Please update your configuration."
#elif defined(PAUSE_PARK_XY_FEEDRATE)
  #error "PAUSE_PARK_XY_FEEDRATE is now NOZZLE_PARK_XY_FEEDRATE. Please update your configuration."
#elif defined(PAUSE_PARK_Z_FEEDRATE)
  #error "PAUSE_PARK_Z_FEEDRATE is now NOZZLE_PARK_Z_FEEDRATE. Please update your configuration."
#elif defined(FILAMENT_CHANGE_RETRACT_FEEDRATE)
  #error "FILAMENT_CHANGE_RETRACT_FEEDRATE is now PAUSE_PARK_RETRACT_FEEDRATE. Please update your configuration."
#elif defined(FILAMENT_CHANGE_RETRACT_LENGTH)
  #error "FILAMENT_CHANGE_RETRACT_LENGTH is now PAUSE_PARK_RETRACT_LENGTH. Please update your configuration."
#elif defined(FILAMENT_CHANGE_EXTRUDE_FEEDRATE)
  #error "FILAMENT_CHANGE_EXTRUDE_FEEDRATE is now ADVANCED_PAUSE_PURGE_FEEDRATE. Please update your configuration."
#elif defined(ADVANCED_PAUSE_EXTRUDE_FEEDRATE)
  #error "ADVANCED_PAUSE_EXTRUDE_FEEDRATE is now ADVANCED_PAUSE_PURGE_FEEDRATE. Please update your configuration."
#elif defined(FILAMENT_CHANGE_EXTRUDE_LENGTH)
  #error "FILAMENT_CHANGE_EXTRUDE_LENGTH is now ADVANCED_PAUSE_PURGE_LENGTH. Please update your configuration."
#elif defined(ADVANCED_PAUSE_EXTRUDE_LENGTH)
  #error "ADVANCED_PAUSE_EXTRUDE_LENGTH is now ADVANCED_PAUSE_PURGE_LENGTH. Please update your configuration."
#elif defined(FILAMENT_CHANGE_NOZZLE_TIMEOUT)
  #error "FILAMENT_CHANGE_NOZZLE_TIMEOUT is now PAUSE_PARK_NOZZLE_TIMEOUT. Please update your configuration."
#elif defined(FILAMENT_CHANGE_NUMBER_OF_ALERT_BEEPS)
  #error "FILAMENT_CHANGE_NUMBER_OF_ALERT_BEEPS is now FILAMENT_CHANGE_ALERT_BEEPS. Please update your configuration."
#elif ENABLED(FILAMENT_CHANGE_NO_STEPPER_TIMEOUT)
  #error "FILAMENT_CHANGE_NO_STEPPER_TIMEOUT is now PAUSE_PARK_NO_STEPPER_TIMEOUT. Please update your configuration."
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
  #error "Z_RAISE_(BEFORE|AFTER)_PROBING are deprecated. Use Z_CLEARANCE_DEPLOY_PROBE and Z_AFTER_PROBING instead."
#elif defined(Z_RAISE_PROBE_DEPLOY_STOW) || defined(Z_RAISE_BETWEEN_PROBINGS)
  #error "Z_RAISE_PROBE_DEPLOY_STOW and Z_RAISE_BETWEEN_PROBINGS are now Z_CLEARANCE_DEPLOY_PROBE and Z_CLEARANCE_BETWEEN_PROBES. Please update your configuration."
#elif defined(Z_PROBE_DEPLOY_HEIGHT) || defined(Z_PROBE_TRAVEL_HEIGHT)
  #error "Z_PROBE_DEPLOY_HEIGHT and Z_PROBE_TRAVEL_HEIGHT are now Z_CLEARANCE_DEPLOY_PROBE and Z_CLEARANCE_BETWEEN_PROBES. Please update your configuration."
#elif defined(MANUAL_BED_LEVELING)
  #error "MANUAL_BED_LEVELING is now LCD_BED_LEVELING. Please update your configuration."
#elif defined(MESH_HOME_SEARCH_Z)
  #error "MESH_HOME_SEARCH_Z is now LCD_PROBE_Z_RANGE. Please update your configuration."
#elif defined(MANUAL_PROBE_Z_RANGE)
  #error "MANUAL_PROBE_Z_RANGE is now LCD_PROBE_Z_RANGE. Please update your configuration."
#elif !defined(MIN_STEPS_PER_SEGMENT)
  #error Please replace "const int dropsegments" with "#define MIN_STEPS_PER_SEGMENT" (and increase by 1) in Configuration_adv.h.
#elif MIN_STEPS_PER_SEGMENT <= 0
  #error "MIN_STEPS_PER_SEGMENT must be at least 1. Please update your Configuration_adv.h."
#elif defined(PREVENT_DANGEROUS_EXTRUDE)
  #error "PREVENT_DANGEROUS_EXTRUDE is now PREVENT_COLD_EXTRUSION. Please update your configuration."
#elif defined(SCARA)
  #error "SCARA is now MORGAN_SCARA. Please update your configuration."
#elif defined(ENABLE_AUTO_BED_LEVELING)
  #error "ENABLE_AUTO_BED_LEVELING is deprecated. Specify AUTO_BED_LEVELING_LINEAR, AUTO_BED_LEVELING_BILINEAR, or AUTO_BED_LEVELING_3POINT."
#elif defined(AUTO_BED_LEVELING_FEATURE)
  #error "AUTO_BED_LEVELING_FEATURE is deprecated. Specify AUTO_BED_LEVELING_LINEAR, AUTO_BED_LEVELING_BILINEAR, or AUTO_BED_LEVELING_3POINT."
#elif defined(ABL_GRID_POINTS)
  #error "ABL_GRID_POINTS is now GRID_MAX_POINTS_X and GRID_MAX_POINTS_Y. Please update your configuration."
#elif defined(ABL_GRID_POINTS_X) || defined(ABL_GRID_POINTS_Y)
  #error "ABL_GRID_POINTS_[XY] is now GRID_MAX_POINTS_[XY]. Please update your configuration."
#elif defined(ABL_GRID_MAX_POINTS_X) || defined(ABL_GRID_MAX_POINTS_Y)
  #error "ABL_GRID_MAX_POINTS_[XY] is now GRID_MAX_POINTS_[XY]. Please update your configuration."
#elif defined(MESH_NUM_X_POINTS) || defined(MESH_NUM_Y_POINTS)
  #error "MESH_NUM_[XY]_POINTS is now GRID_MAX_POINTS_[XY]. Please update your configuration."
#elif defined(UBL_MESH_NUM_X_POINTS) || defined(UBL_MESH_NUM_Y_POINTS)
  #error "UBL_MESH_NUM_[XY]_POINTS is now GRID_MAX_POINTS_[XY]. Please update your configuration."
#elif defined(UBL_G26_MESH_VALIDATION)
  #error "UBL_G26_MESH_VALIDATION is now G26_MESH_VALIDATION. Please update your configuration."
#elif defined(UBL_MESH_EDIT_ENABLED)
  #error "UBL_MESH_EDIT_ENABLED is now G26_MESH_VALIDATION. Please update your configuration."
#elif defined(UBL_MESH_EDITING)
  #error "UBL_MESH_EDITING is now G26_MESH_VALIDATION. Please update your configuration."
#elif defined(BLTOUCH_HEATERS_OFF)
  #error "BLTOUCH_HEATERS_OFF is now PROBING_HEATERS_OFF. Please update your configuration."
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
#elif defined(EXTRUDER_0_AUTO_FAN_PIN) || defined(EXTRUDER_1_AUTO_FAN_PIN) || defined(EXTRUDER_2_AUTO_FAN_PIN) || defined(EXTRUDER_3_AUTO_FAN_PIN)
  #error "EXTRUDER_[0123]_AUTO_FAN_PIN is now E[0123]_AUTO_FAN_PIN. Please update your Configuration_adv.h."
#elif defined(min_software_endstops) || defined(max_software_endstops)
  #error "(min|max)_software_endstops are now (MIN|MAX)_SOFTWARE_ENDSTOPS. Please update your configuration."
#elif ENABLED(Z_PROBE_SLED) && defined(SLED_PIN)
  #error "Replace SLED_PIN with SOL1_PIN (applies to both Z_PROBE_SLED and SOLENOID_PROBE)."
#elif defined(CONTROLLERFAN_PIN)
  #error "CONTROLLERFAN_PIN is now CONTROLLER_FAN_PIN, enabled with USE_CONTROLLER_FAN. Please update your Configuration_adv.h."
#elif defined(MIN_RETRACT)
  #error "MIN_RETRACT is now MIN_AUTORETRACT and MAX_AUTORETRACT. Please update your Configuration_adv.h."
#elif defined(ADVANCE)
  #error "ADVANCE was removed in Marlin 1.1.6. Please use LIN_ADVANCE."
#elif defined(LIN_ADVANCE_E_D_RATIO)
  #error "LIN_ADVANCE (1.5) no longer uses LIN_ADVANCE_E_D_RATIO. Check your configuration."
#elif defined(NEOPIXEL_RGBW_LED)
  #error "NEOPIXEL_RGBW_LED is now NEOPIXEL_LED. Please update your configuration."
#elif ENABLED(DELTA) && defined(DELTA_PROBEABLE_RADIUS)
  #error "Remove DELTA_PROBEABLE_RADIUS and use MIN_PROBE_EDGE to inset the probe area instead."
#elif defined(UBL_MESH_INSET)
  #error "UBL_MESH_INSET is now just MESH_INSET. Please update your configuration."
#elif defined(UBL_MESH_MIN_X) || defined(UBL_MESH_MIN_Y) || defined(UBL_MESH_MAX_X)  || defined(UBL_MESH_MAX_Y)
  #error "UBL_MESH_(MIN|MAX)_[XY] is now just MESH_(MIN|MAX)_[XY]. Please update your configuration."
#elif defined(ABL_PROBE_PT_1_X) || defined(ABL_PROBE_PT_1_Y) || defined(ABL_PROBE_PT_2_X) || defined(ABL_PROBE_PT_2_Y) || defined(ABL_PROBE_PT_3_X) || defined(ABL_PROBE_PT_3_Y)
  #error "ABL_PROBE_PT_[123]_[XY] is now PROBE_PT_[123]_[XY]. Please update your configuration."
#elif defined(UBL_PROBE_PT_1_X) || defined(UBL_PROBE_PT_1_Y) || defined(UBL_PROBE_PT_2_X) || defined(UBL_PROBE_PT_2_Y) || defined(UBL_PROBE_PT_3_X) || defined(UBL_PROBE_PT_3_Y)
  #error "UBL_PROBE_PT_[123]_[XY] is now PROBE_PT_[123]_[XY]. Please update your configuration."
#elif defined(ENABLE_MESH_EDIT_GFX_OVERLAY)
  #error "ENABLE_MESH_EDIT_GFX_OVERLAY is now MESH_EDIT_GFX_OVERLAY. Please update your configuration."
#elif defined(BABYSTEP_ZPROBE_GFX_REVERSE)
  #error "BABYSTEP_ZPROBE_GFX_REVERSE is now set by OVERLAY_GFX_REVERSE. Please update your configurations."
#elif defined(UBL_GRANULAR_SEGMENTATION_FOR_CARTESIAN)
  #error "UBL_GRANULAR_SEGMENTATION_FOR_CARTESIAN is now SEGMENT_LEVELED_MOVES. Please update your configuration."
#elif HAS_PID_HEATING && (defined(K1) || !defined(PID_K1))
  #error "K1 is now PID_K1. Please update your configuration."
#elif defined(PROBE_DOUBLE_TOUCH)
  #error "PROBE_DOUBLE_TOUCH is now MULTIPLE_PROBING. Please update your configuration."
#elif defined(ANET_KEYPAD_LCD)
  #error "ANET_KEYPAD_LCD is now ZONESTAR_LCD. Please update your configuration."
#elif defined(LCD_I2C_SAINSMART_YWROBOT)
  #error "LCD_I2C_SAINSMART_YWROBOT is now LCD_SAINSMART_I2C_(1602|2004). Please update your configuration."
#elif defined(MEASURED_LOWER_LIMIT) || defined(MEASURED_UPPER_LIMIT)
  #error "MEASURED_(UPPER|LOWER)_LIMIT is now FILWIDTH_ERROR_MARGIN. Please update your configuration."
#elif defined(HAVE_TMCDRIVER)
  #error "HAVE_TMCDRIVER is now HAVE_TMC26X. Please update your Configuration_adv.h."
#elif defined(X_IS_TMC) || defined(X2_IS_TMC) || defined(Y_IS_TMC) || defined(Y2_IS_TMC) || defined(Z_IS_TMC) || defined(Z2_IS_TMC) \
   || defined(E0_IS_TMC) || defined(E1_IS_TMC) || defined(E2_IS_TMC) || defined(E3_IS_TMC) || defined(E4_IS_TMC)
  #error "[AXIS]_IS_TMC is now [AXIS]_IS_TMC26X. Please update your Configuration_adv.h."
#elif defined(AUTOMATIC_CURRENT_CONTROL)
  #error "AUTOMATIC_CURRENT_CONTROL is now MONITOR_DRIVER_STATUS. Please update your configuration."
#elif defined(FILAMENT_CHANGE_LOAD_LENGTH)
  #error "FILAMENT_CHANGE_LOAD_LENGTH is now FILAMENT_CHANGE_FAST_LOAD_LENGTH. Please update your configuration."
#elif ENABLED(LEVEL_BED_CORNERS) && !defined(LEVEL_CORNERS_INSET)
  #error "LEVEL_BED_CORNERS requires a LEVEL_CORNERS_INSET value. Please update your Configuration.h."
#endif

#define BOARD_MKS_13     -47
#define BOARD_TRIGORILLA -343
#if MB(MKS_13)
  #error "BOARD_MKS_13 has been renamed BOARD_MKS_GEN_13. Please update your configuration."
#elif MB(BOARD_TRIGORILLA)
  #error "BOARD_TRIGORILLA has been renamed BOARD_TRIGORILLA_13. Please update your configuration."
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
 * Serial
 */
#if !(defined(__AVR__) && defined(USBCON))
  #if ENABLED(SERIAL_XON_XOFF) && RX_BUFFER_SIZE < 1024
    #error "SERIAL_XON_XOFF requires RX_BUFFER_SIZE >= 1024 for reliable transfers without drops."
  #elif RX_BUFFER_SIZE && (RX_BUFFER_SIZE < 2 || !IS_POWER_OF_2(RX_BUFFER_SIZE))
    #error "RX_BUFFER_SIZE must be a power of 2 greater than 1."
  #elif TX_BUFFER_SIZE && (TX_BUFFER_SIZE < 2 || TX_BUFFER_SIZE > 256 || !IS_POWER_OF_2(TX_BUFFER_SIZE))
    #error "TX_BUFFER_SIZE must be 0, a power of 2 greater than 1, and no greater than 256."
  #endif
#elif ENABLED(SERIAL_XON_XOFF) || ENABLED(SERIAL_STATS_MAX_RX_QUEUED) || ENABLED(SERIAL_STATS_DROPPED_RX)
  #error "SERIAL_XON_XOFF and SERIAL_STATS_* features not supported on USB-native AVR devices."
#endif

#if SERIAL_PORT > 7
  #error "Set SERIAL_PORT to the port on your board. Usually this is 0."
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
 * Validate that the bed size fits
 */
static_assert(X_MAX_LENGTH >= X_BED_SIZE && Y_MAX_LENGTH >= Y_BED_SIZE,
  "Movement bounds ([XY]_MIN_POS, [XY]_MAX_POS) are too narrow to contain [XY]_BED_SIZE.");

/**
 * Granular software endstops (Marlin >= 1.1.7)
 */
#if ENABLED(MIN_SOFTWARE_ENDSTOPS) && DISABLED(MIN_SOFTWARE_ENDSTOP_Z)
  #if IS_KINEMATIC
    #error "MIN_SOFTWARE_ENDSTOPS on DELTA/SCARA also requires MIN_SOFTWARE_ENDSTOP_Z."
  #elif DISABLED(MIN_SOFTWARE_ENDSTOP_X) && DISABLED(MIN_SOFTWARE_ENDSTOP_Y)
    #error "MIN_SOFTWARE_ENDSTOPS requires at least one of the MIN_SOFTWARE_ENDSTOP_[XYZ] options."
  #endif
#endif

#if ENABLED(MAX_SOFTWARE_ENDSTOPS) && DISABLED(MAX_SOFTWARE_ENDSTOP_Z)
  #if IS_KINEMATIC
    #error "MAX_SOFTWARE_ENDSTOPS on DELTA/SCARA also requires MAX_SOFTWARE_ENDSTOP_Z."
  #elif DISABLED(MAX_SOFTWARE_ENDSTOP_X) && DISABLED(MAX_SOFTWARE_ENDSTOP_Y)
    #error "MAX_SOFTWARE_ENDSTOPS requires at least one of the MAX_SOFTWARE_ENDSTOP_[XYZ] options."
  #endif
#endif

#if !defined(TARGET_LPC1768) && ( \
     ENABLED(ENDSTOPPULLDOWNS) \
  || ENABLED(ENDSTOPPULLDOWN_XMAX) \
  || ENABLED(ENDSTOPPULLDOWN_YMAX) \
  || ENABLED(ENDSTOPPULLDOWN_ZMAX) \
  || ENABLED(ENDSTOPPULLDOWN_XMIN) \
  || ENABLED(ENDSTOPPULLDOWN_YMIN) \
  || ENABLED(ENDSTOPPULLDOWN_ZMIN) )
  #error "PULLDOWN pin mode is not available on the selected board."
#endif

#if ENABLED(ENDSTOPPULLUPS) && ENABLED(ENDSTOPPULLDOWNS)
  #error "Enable only one of ENDSTOPPULLUPS or ENDSTOPPULLDOWNS."
#elif ENABLED(FIL_RUNOUT_PULLUP) && ENABLED(FIL_RUNOUT_PULLDOWN)
  #error "Enable only one of FIL_RUNOUT_PULLUP or FIL_RUNOUT_PULLDOWN."
#elif ENABLED(ENDSTOPPULLUP_XMAX) && ENABLED(ENDSTOPPULLDOWN_XMAX)
  #error "Enable only one of ENDSTOPPULLUP_X_MAX or ENDSTOPPULLDOWN_X_MAX."
#elif ENABLED(ENDSTOPPULLUP_YMAX) && ENABLED(ENDSTOPPULLDOWN_YMAX)
  #error "Enable only one of ENDSTOPPULLUP_Y_MAX or ENDSTOPPULLDOWN_Y_MAX."
#elif ENABLED(ENDSTOPPULLUP_ZMAX) && ENABLED(ENDSTOPPULLDOWN_ZMAX)
  #error "Enable only one of ENDSTOPPULLUP_Z_MAX or ENDSTOPPULLDOWN_Z_MAX."
#elif ENABLED(ENDSTOPPULLUP_XMIN) && ENABLED(ENDSTOPPULLDOWN_XMIN)
  #error "Enable only one of ENDSTOPPULLUP_X_MIN or ENDSTOPPULLDOWN_X_MIN."
#elif ENABLED(ENDSTOPPULLUP_YMIN) && ENABLED(ENDSTOPPULLDOWN_YMIN)
  #error "Enable only one of ENDSTOPPULLUP_Y_MIN or ENDSTOPPULLDOWN_Y_MIN."
#elif ENABLED(ENDSTOPPULLUP_ZMIN) && ENABLED(ENDSTOPPULLDOWN_ZMIN)
  #error "Enable only one of ENDSTOPPULLUP_Z_MIN or ENDSTOPPULLDOWN_Z_MIN."
#endif

/**
 * Progress Bar
 */
#if ENABLED(LCD_PROGRESS_BAR)
  #if DISABLED(SDSUPPORT) && DISABLED(LCD_SET_PROGRESS_MANUALLY)
    #error "LCD_PROGRESS_BAR requires SDSUPPORT or LCD_SET_PROGRESS_MANUALLY."
  #elif DISABLED(ULTRA_LCD)
    #error "LCD_PROGRESS_BAR requires a character LCD."
  #elif ENABLED(DOGLCD)
    #error "LCD_PROGRESS_BAR does not apply to graphical displays."
  #elif ENABLED(FILAMENT_LCD_DISPLAY)
    #error "LCD_PROGRESS_BAR and FILAMENT_LCD_DISPLAY are not fully compatible. Comment out this line to use both."
  #endif
#elif ENABLED(LCD_SET_PROGRESS_MANUALLY) && DISABLED(DOGLCD)
  #error "LCD_SET_PROGRESS_MANUALLY requires LCD_PROGRESS_BAR or Graphical LCD."
#endif

/**
 * Custom Boot and Status screens
 */
#if DISABLED(DOGLCD) && (ENABLED(SHOW_CUSTOM_BOOTSCREEN) || ENABLED(CUSTOM_STATUS_SCREEN_IMAGE))
  #error "Graphical LCD is required for SHOW_CUSTOM_BOOTSCREEN and CUSTOM_STATUS_SCREEN_IMAGE."
#endif

/**
 * SD File Sorting
 */
#if ENABLED(SDCARD_SORT_ALPHA)
  #if SDSORT_LIMIT > 256
    #error "SDSORT_LIMIT must be 256 or smaller."
  #elif SDSORT_LIMIT < 10
    #error "SDSORT_LIMIT should be greater than 9 to be useful."
  #elif DISABLED(SDSORT_USES_RAM)
    #if ENABLED(SDSORT_DYNAMIC_RAM)
      #error "SDSORT_DYNAMIC_RAM requires SDSORT_USES_RAM (which reads the directory into RAM)."
    #elif ENABLED(SDSORT_CACHE_NAMES)
      #error "SDSORT_CACHE_NAMES requires SDSORT_USES_RAM (which reads the directory into RAM)."
    #endif
  #endif

  #if ENABLED(SDSORT_CACHE_NAMES) && DISABLED(SDSORT_DYNAMIC_RAM)
    #if SDSORT_CACHE_VFATS < 2
      #error "SDSORT_CACHE_VFATS must be 2 or greater!"
    #elif SDSORT_CACHE_VFATS > MAX_VFAT_ENTRIES
      #undef SDSORT_CACHE_VFATS
      #define SDSORT_CACHE_VFATS MAX_VFAT_ENTRIES
      #warning "SDSORT_CACHE_VFATS was reduced to MAX_VFAT_ENTRIES!"
    #endif
  #endif
#endif

/**
 * I2C Position Encoders
 */
#if ENABLED(I2C_POSITION_ENCODERS)
  #if DISABLED(BABYSTEPPING) || DISABLED(BABYSTEP_XY)
    #error "I2C_POSITION_ENCODERS requires BABYSTEPPING and BABYSTEP_XY."
  #elif !WITHIN(I2CPE_ENCODER_CNT, 1, 5)
    #error "I2CPE_ENCODER_CNT must be between 1 and 5."
  #endif
#endif

/**
 * Babystepping
 */
#if ENABLED(BABYSTEPPING)
  #if ENABLED(SCARA)
    #error "BABYSTEPPING is not implemented for SCARA yet."
  #elif ENABLED(DELTA) && ENABLED(BABYSTEP_XY)
    #error "BABYSTEPPING only implemented for Z axis on deltabots."
  #elif ENABLED(BABYSTEP_ZPROBE_OFFSET) && ENABLED(MESH_BED_LEVELING)
    #error "MESH_BED_LEVELING and BABYSTEP_ZPROBE_OFFSET is not a valid combination"
  #elif ENABLED(BABYSTEP_ZPROBE_OFFSET) && !HAS_BED_PROBE
    #error "BABYSTEP_ZPROBE_OFFSET requires a probe."
  #elif ENABLED(BABYSTEP_ZPROBE_GFX_OVERLAY) && !ENABLED(DOGLCD)
    #error "BABYSTEP_ZPROBE_GFX_OVERLAY requires a Graphical LCD."
  #elif ENABLED(BABYSTEP_ZPROBE_GFX_OVERLAY) && !ENABLED(BABYSTEP_ZPROBE_OFFSET)
    #error "BABYSTEP_ZPROBE_GFX_OVERLAY requires a BABYSTEP_ZPROBE_OFFSET."
  #endif
#endif

/**
 * Filament Runout needs one or more pins and either SD Support or Auto print start detection
 */
#if ENABLED(FILAMENT_RUNOUT_SENSOR)
  #if !PIN_EXISTS(FIL_RUNOUT)
    #error "FILAMENT_RUNOUT_SENSOR requires FIL_RUNOUT_PIN."
  #elif NUM_RUNOUT_SENSORS > E_STEPPERS
    #error "NUM_RUNOUT_SENSORS cannot exceed the number of E steppers."
  #elif NUM_RUNOUT_SENSORS > 1 && !PIN_EXISTS(FIL_RUNOUT2)
    #error "FILAMENT_RUNOUT_SENSOR with NUM_RUNOUT_SENSORS > 1 requires FIL_RUNOUT2_PIN."
  #elif NUM_RUNOUT_SENSORS > 2 && !PIN_EXISTS(FIL_RUNOUT3)
    #error "FILAMENT_RUNOUT_SENSOR with NUM_RUNOUT_SENSORS > 2 requires FIL_RUNOUT3_PIN."
  #elif NUM_RUNOUT_SENSORS > 3 && !PIN_EXISTS(FIL_RUNOUT4)
    #error "FILAMENT_RUNOUT_SENSOR with NUM_RUNOUT_SENSORS > 3 requires FIL_RUNOUT4_PIN."
  #elif NUM_RUNOUT_SENSORS > 4 && !PIN_EXISTS(FIL_RUNOUT5)
    #error "FILAMENT_RUNOUT_SENSOR with NUM_RUNOUT_SENSORS > 4 requires FIL_RUNOUT5_PIN."
  #elif DISABLED(SDSUPPORT) && DISABLED(PRINTJOB_TIMER_AUTOSTART)
    #error "FILAMENT_RUNOUT_SENSOR requires SDSUPPORT or PRINTJOB_TIMER_AUTOSTART."
  #elif DISABLED(ADVANCED_PAUSE_FEATURE)
    static_assert(NULL == strstr(FILAMENT_RUNOUT_SCRIPT, "M600"), "ADVANCED_PAUSE_FEATURE is required to use M600 with FILAMENT_RUNOUT_SENSOR.");
  #endif
#endif

/**
 * Advanced Pause
 */
#if ENABLED(ADVANCED_PAUSE_FEATURE)
  #if !HAS_RESUME_CONTINUE
    #error "ADVANCED_PAUSE_FEATURE currently requires an LCD controller or EMERGENCY_PARSER."
  #elif ENABLED(EXTRUDER_RUNOUT_PREVENT)
    #error "EXTRUDER_RUNOUT_PREVENT is incompatible with ADVANCED_PAUSE_FEATURE."
  #elif ENABLED(PARK_HEAD_ON_PAUSE) && DISABLED(SDSUPPORT) && DISABLED(NEWPANEL) && DISABLED(EMERGENCY_PARSER)
    #error "PARK_HEAD_ON_PAUSE requires SDSUPPORT, EMERGENCY_PARSER, or an LCD controller."
  #elif ENABLED(HOME_BEFORE_FILAMENT_CHANGE) && DISABLED(PAUSE_PARK_NO_STEPPER_TIMEOUT)
    #error "HOME_BEFORE_FILAMENT_CHANGE requires PAUSE_PARK_NO_STEPPER_TIMEOUT."
  #elif DISABLED(NOZZLE_PARK_FEATURE)
    #error "ADVANCED_PAUSE_FEATURE requires NOZZLE_PARK_FEATURE."
  #elif ENABLED(PREVENT_LENGTHY_EXTRUDE) && FILAMENT_CHANGE_UNLOAD_LENGTH > EXTRUDE_MAXLENGTH
    #error "FILAMENT_CHANGE_UNLOAD_LENGTH must be less than or equal to EXTRUDE_MAXLENGTH."
  #elif ENABLED(PREVENT_LENGTHY_EXTRUDE) && FILAMENT_CHANGE_SLOW_LOAD_LENGTH > EXTRUDE_MAXLENGTH
    #error "FILAMENT_CHANGE_SLOW_LOAD_LENGTH must be less than or equal to EXTRUDE_MAXLENGTH."
  #elif ENABLED(PREVENT_LENGTHY_EXTRUDE) && FILAMENT_CHANGE_FAST_LOAD_LENGTH > EXTRUDE_MAXLENGTH
    #error "FILAMENT_CHANGE_FAST_LOAD_LENGTH must be less than or equal to EXTRUDE_MAXLENGTH."
  #endif
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

  #if EXTRUDERS > 5
    #error "Marlin supports a maximum of 5 EXTRUDERS."
  #endif

  #if ENABLED(TEMP_SENSOR_1_AS_REDUNDANT)
    #error "EXTRUDERS must be 1 with TEMP_SENSOR_1_AS_REDUNDANT."
  #endif

  #if ENABLED(HEATERS_PARALLEL)
    #error "EXTRUDERS must be 1 with HEATERS_PARALLEL."
  #endif

#elif ENABLED(MK2_MULTIPLEXER)
  #error "MK2_MULTIPLEXER requires 2 or more EXTRUDERS."
#elif ENABLED(SINGLENOZZLE)
  #error "SINGLENOZZLE requires 2 or more EXTRUDERS."
#endif

/**
 * Sanity checking for the Průša MK2 Multiplexer
 */
#ifdef SNMM
  #error "SNMM is now MK2_MULTIPLEXER. Please update your configuration."
#endif

/**
 * A Dual Nozzle carriage with switching servo
 */
#if ENABLED(SWITCHING_NOZZLE)
  #if ENABLED(DUAL_X_CARRIAGE)
    #error "SWITCHING_NOZZLE and DUAL_X_CARRIAGE are incompatible."
  #elif ENABLED(SINGLENOZZLE)
    #error "SWITCHING_NOZZLE and SINGLENOZZLE are incompatible."
  #elif EXTRUDERS != 2
    #error "SWITCHING_NOZZLE requires exactly 2 EXTRUDERS."
  #elif NUM_SERVOS < 1
    #error "SWITCHING_NOZZLE requires NUM_SERVOS >= 1."
  #endif
#endif

/**
 * Single Stepper Dual Extruder with switching servo
 */
#if ENABLED(SWITCHING_EXTRUDER) && NUM_SERVOS < 1
  #error "SWITCHING_EXTRUDER requires NUM_SERVOS >= 1."
#endif

/**
 * Mixing Extruder requirements
 */
#if ENABLED(MIXING_EXTRUDER)
  #if EXTRUDERS > 1
    #error "MIXING_EXTRUDER currently only supports one extruder."
  #elif MIXING_STEPPERS < 2
    #error "You must set MIXING_STEPPERS >= 2 for a mixing extruder."
  #elif ENABLED(FILAMENT_SENSOR)
    #error "MIXING_EXTRUDER is incompatible with FILAMENT_SENSOR. Comment out this line to use it anyway."
  #elif ENABLED(SWITCHING_EXTRUDER)
    #error "Please select either MIXING_EXTRUDER or SWITCHING_EXTRUDER, not both."
  #elif ENABLED(SINGLENOZZLE)
    #error "MIXING_EXTRUDER is incompatible with SINGLENOZZLE."
  #elif ENABLED(LIN_ADVANCE)
    #error "MIXING_EXTRUDER is incompatible with LIN_ADVANCE."
  #endif
#endif

/**
 * Linear Advance 1.5 - Check K value range
 */
#if ENABLED(LIN_ADVANCE)
  static_assert(
    WITHIN(LIN_ADVANCE_K, 0, 10),
    "LIN_ADVANCE_K must be a value from 0 to 10 (Changed in LIN_ADVANCE v1.5, Marlin 1.1.9)."
  );
#endif

/**
 * Parking Extruder requirements
 */
#if ENABLED(PARKING_EXTRUDER)
  #if ENABLED(DUAL_X_CARRIAGE)
    #error "PARKING_EXTRUDER and DUAL_X_CARRIAGE are incompatible."
  #elif ENABLED(SINGLENOZZLE)
    #error "PARKING_EXTRUDER and SINGLENOZZLE are incompatible."
  #elif ENABLED(EXT_SOLENOID)
    #error "PARKING_EXTRUDER and EXT_SOLENOID are incompatible. (Pins are used twice.)"
  #elif EXTRUDERS != 2
    #error "PARKING_EXTRUDER requires exactly 2 EXTRUDERS."
  #elif !PIN_EXISTS(SOL0) || !PIN_EXISTS(SOL1)
    #error "PARKING_EXTRUDER requires SOL0_PIN and SOL1_PIN."
  #elif !defined(PARKING_EXTRUDER_PARKING_X)
    #error "PARKING_EXTRUDER requires PARKING_EXTRUDER_PARKING_X."
  #elif !defined(PARKING_EXTRUDER_SECURITY_RAISE)
    #error "PARKING_EXTRUDER requires PARKING_EXTRUDER_SECURITY_RAISE."
  #elif PARKING_EXTRUDER_SECURITY_RAISE < 0
    #error "PARKING_EXTRUDER_SECURITY_RAISE must be 0 or higher."
  #elif !defined(PARKING_EXTRUDER_SOLENOIDS_PINS_ACTIVE) || !WITHIN(PARKING_EXTRUDER_SOLENOIDS_PINS_ACTIVE, LOW, HIGH)
    #error "PARKING_EXTRUDER_SOLENOIDS_PINS_ACTIVE must be defined as HIGH or LOW."
  #elif !defined(PARKING_EXTRUDER_SOLENOIDS_DELAY) || !WITHIN(PARKING_EXTRUDER_SOLENOIDS_DELAY, 0, 2000)
    #error "PARKING_EXTRUDER_SOLENOIDS_DELAY must be between 0 and 2000 (ms)."
  #endif
#endif

/**
 * Part-Cooling Fan Multiplexer requirements
 */
#if PIN_EXISTS(FANMUX1)
  #if !HAS_FANMUX
    #error "FANMUX0_PIN must be set before FANMUX1_PIN can be set."
  #endif
#elif PIN_EXISTS(FANMUX2)
  #error "FANMUX0_PIN and FANMUX1_PIN must be set before FANMUX2_PIN can be set."
#endif

/**
 * Limited number of servos
 */
#if NUM_SERVOS > 4
  #error "The maximum number of SERVOS in Marlin is 4."
#endif

/**
 * Servo deactivation depends on servo endstops, switching nozzle, or switching extruder
 */
#if ENABLED(DEACTIVATE_SERVOS_AFTER_MOVE) && !HAS_Z_SERVO_PROBE && !defined(SWITCHING_NOZZLE_SERVO_NR) && !defined(SWITCHING_EXTRUDER_SERVO_NR)
  #error "Z_PROBE_SERVO_NR, switching nozzle, or switching extruder is required for DEACTIVATE_SERVOS_AFTER_MOVE."
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
 * Kinematics
 */

/**
 * Allow only one kinematic type to be defined
 */
#if 1 < 0 \
  + ENABLED(DELTA) \
  + ENABLED(MORGAN_SCARA) \
  + ENABLED(MAKERARM_SCARA) \
  + ENABLED(COREXY) \
  + ENABLED(COREXZ) \
  + ENABLED(COREYZ) \
  + ENABLED(COREYX) \
  + ENABLED(COREZX) \
  + ENABLED(COREZY)
  #error "Please enable only one of DELTA, MORGAN_SCARA, MAKERARM_SCARA, COREXY, COREYX, COREXZ, COREZX, COREYZ, or COREZY."
#endif

/**
 * Delta requirements
 */
#if ENABLED(DELTA)
  #if DISABLED(USE_XMAX_PLUG) && DISABLED(USE_YMAX_PLUG) && DISABLED(USE_ZMAX_PLUG)
    #error "You probably want to use Max Endstops for DELTA!"
  #elif ENABLED(ENABLE_LEVELING_FADE_HEIGHT) && DISABLED(AUTO_BED_LEVELING_BILINEAR) && !UBL_SEGMENTED
    #error "ENABLE_LEVELING_FADE_HEIGHT on DELTA requires AUTO_BED_LEVELING_BILINEAR or AUTO_BED_LEVELING_UBL."
  #elif ENABLED(DELTA_AUTO_CALIBRATION) && !(HAS_BED_PROBE || ENABLED(ULTIPANEL))
    #error "DELTA_AUTO_CALIBRATION requires a probe or LCD Controller."
  #elif ABL_GRID
    #if (GRID_MAX_POINTS_X & 1) == 0 || (GRID_MAX_POINTS_Y & 1) == 0
      #error "DELTA requires GRID_MAX_POINTS_X and GRID_MAX_POINTS_Y to be odd numbers."
    #elif GRID_MAX_POINTS_X < 3
      #error "DELTA requires GRID_MAX_POINTS_X and GRID_MAX_POINTS_Y to be 3 or higher."
    #endif
  #endif
#endif

/**
 * Probes
 */

/**
 * Allow only one probe option to be defined
 */
#if 1 < 0 \
  + ENABLED(PROBE_MANUALLY) \
  + ENABLED(FIX_MOUNTED_PROBE) \
  + (HAS_Z_SERVO_PROBE && DISABLED(BLTOUCH)) \
  + ENABLED(BLTOUCH) \
  + ENABLED(SOLENOID_PROBE) \
  + ENABLED(Z_PROBE_ALLEN_KEY) \
  + ENABLED(Z_PROBE_SLED)
  #error "Please enable only one probe option: PROBE_MANUALLY, FIX_MOUNTED_PROBE, BLTOUCH, SOLENOID_PROBE, Z_PROBE_ALLEN_KEY, Z_PROBE_SLED, or Z Servo."
#endif

#if HAS_BED_PROBE

  /**
   * Z_PROBE_SLED is incompatible with DELTA
   */
  #if ENABLED(Z_PROBE_SLED) && ENABLED(DELTA)
    #error "You cannot use Z_PROBE_SLED with DELTA."
  #endif

  /**
   * SOLENOID_PROBE requirements
   */
  #if ENABLED(SOLENOID_PROBE)
    #if ENABLED(EXT_SOLENOID)
      #error "SOLENOID_PROBE is incompatible with EXT_SOLENOID."
    #elif !HAS_SOLENOID_1
      #error "SOLENOID_PROBE requires SOL1_PIN. It can be added to your Configuration.h."
    #endif
  #endif

  /**
   * NUM_SERVOS is required for a Z servo probe
   */
  #if HAS_Z_SERVO_PROBE
    #ifndef NUM_SERVOS
      #error "You must set NUM_SERVOS for a Z servo probe (Z_PROBE_SERVO_NR)."
    #elif Z_PROBE_SERVO_NR >= NUM_SERVOS
      #error "Z_PROBE_SERVO_NR must be smaller than NUM_SERVOS."
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
    #elif ENABLED(Z_MIN_PROBE_ENDSTOP_INVERTING) != ENABLED(Z_MIN_ENDSTOP_INVERTING)
      #error "Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN requires Z_MIN_ENDSTOP_INVERTING to match Z_MIN_PROBE_ENDSTOP_INVERTING."
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
  #ifndef Z_CLEARANCE_DEPLOY_PROBE
    #error "You must define Z_CLEARANCE_DEPLOY_PROBE in your configuration."
  #elif !defined(Z_CLEARANCE_BETWEEN_PROBES)
    #error "You must define Z_CLEARANCE_BETWEEN_PROBES in your configuration."
  #elif Z_CLEARANCE_DEPLOY_PROBE < 0
    #error "Probes need Z_CLEARANCE_DEPLOY_PROBE >= 0."
  #elif Z_CLEARANCE_BETWEEN_PROBES < 0
    #error "Probes need Z_CLEARANCE_BETWEEN_PROBES >= 0."
  #elif Z_AFTER_PROBING < 0
    #error "Probes need Z_AFTER_PROBING >= 0."
  #endif

  #if MULTIPLE_PROBING && MULTIPLE_PROBING < 2
    #error "MULTIPLE_PROBING must be >= 2."
  #endif

  #if Z_PROBE_LOW_POINT > 0
    #error "Z_PROBE_LOW_POINT must be less than or equal to 0."
  #endif

#else

  /**
   * Require some kind of probe for bed leveling and probe testing
   */
  #if OLDSCHOOL_ABL && !PROBE_SELECTED
    #error "Auto Bed Leveling requires one of these: PROBE_MANUALLY, FIX_MOUNTED_PROBE, BLTOUCH, SOLENOID_PROBE, Z_PROBE_ALLEN_KEY, Z_PROBE_SLED, or a Z Servo."
  #endif

  #if ENABLED(Z_MIN_PROBE_REPEATABILITY_TEST)
    #error "Z_MIN_PROBE_REPEATABILITY_TEST requires a probe: FIX_MOUNTED_PROBE, BLTOUCH, SOLENOID_PROBE, Z_PROBE_ALLEN_KEY, Z_PROBE_SLED, or Z Servo."
  #endif

#endif

/**
 * Allow only one bed leveling option to be defined
 */
#if 1 < 0 \
  + ENABLED(AUTO_BED_LEVELING_LINEAR) \
  + ENABLED(AUTO_BED_LEVELING_3POINT) \
  + ENABLED(AUTO_BED_LEVELING_BILINEAR) \
  + ENABLED(AUTO_BED_LEVELING_UBL) \
  + ENABLED(MESH_BED_LEVELING)
  #error "Select only one of: MESH_BED_LEVELING, AUTO_BED_LEVELING_LINEAR, AUTO_BED_LEVELING_3POINT, AUTO_BED_LEVELING_BILINEAR or AUTO_BED_LEVELING_UBL."
#endif

/**
 * Bed Leveling Requirements
 */

#if ENABLED(AUTO_BED_LEVELING_UBL) || ENABLED(AUTO_BED_LEVELING_3POINT)
  static_assert(WITHIN(PROBE_PT_1_X, MIN_PROBE_X, MAX_PROBE_X), "PROBE_PT_1_X is outside the probe region.");
  static_assert(WITHIN(PROBE_PT_2_X, MIN_PROBE_X, MAX_PROBE_X), "PROBE_PT_2_X is outside the probe region.");
  static_assert(WITHIN(PROBE_PT_3_X, MIN_PROBE_X, MAX_PROBE_X), "PROBE_PT_3_X is outside the probe region.");
  static_assert(WITHIN(PROBE_PT_1_Y, MIN_PROBE_Y, MAX_PROBE_Y), "PROBE_PT_1_Y is outside the probe region.");
  static_assert(WITHIN(PROBE_PT_2_Y, MIN_PROBE_Y, MAX_PROBE_Y), "PROBE_PT_2_Y is outside the probe region.");
  static_assert(WITHIN(PROBE_PT_3_Y, MIN_PROBE_Y, MAX_PROBE_Y), "PROBE_PT_3_Y is outside the probe region.");
#endif

#if ENABLED(AUTO_BED_LEVELING_UBL)

  /**
   * Unified Bed Leveling
   */

  // Hide PROBE_MANUALLY from the rest of the code
  #undef PROBE_MANUALLY

  #if IS_SCARA
    #error "AUTO_BED_LEVELING_UBL does not yet support SCARA printers."
  #elif DISABLED(EEPROM_SETTINGS)
    #error "AUTO_BED_LEVELING_UBL requires EEPROM_SETTINGS. Please update your configuration."
  #elif !WITHIN(GRID_MAX_POINTS_X, 3, 15) || !WITHIN(GRID_MAX_POINTS_Y, 3, 15)
    #error "GRID_MAX_POINTS_[XY] must be a whole number between 3 and 15."
  #elif !defined(RESTORE_LEVELING_AFTER_G28)
    #error "AUTO_BED_LEVELING_UBL used to enable RESTORE_LEVELING_AFTER_G28. To keep this behavior enable RESTORE_LEVELING_AFTER_G28. Otherwise define it as 'false'."
  #endif

#elif OLDSCHOOL_ABL

  /**
   * Auto Bed Leveling
   */

  /**
   * Delta and SCARA have limited bed leveling options
   */
  #if IS_SCARA && DISABLED(AUTO_BED_LEVELING_BILINEAR)
    #error "SCARA machines can only use the AUTO_BED_LEVELING_BILINEAR leveling option."
  #endif

  /**
   * Check auto bed leveling probe points
   */
  #if ABL_GRID

    static_assert(LEFT_PROBE_BED_POSITION < RIGHT_PROBE_BED_POSITION, "LEFT_PROBE_BED_POSITION must be less than RIGHT_PROBE_BED_POSITION.");
    static_assert(FRONT_PROBE_BED_POSITION < BACK_PROBE_BED_POSITION, "FRONT_PROBE_BED_POSITION must be less than BACK_PROBE_BED_POSITION.");
    static_assert(LEFT_PROBE_BED_POSITION >= MIN_PROBE_X, "LEFT_PROBE_BED_POSITION is outside the probe region.");
    static_assert(RIGHT_PROBE_BED_POSITION <= MAX_PROBE_X, "RIGHT_PROBE_BED_POSITION is outside the probe region.");
    static_assert(FRONT_PROBE_BED_POSITION >= MIN_PROBE_Y, "FRONT_PROBE_BED_POSITION is outside the probe region.");
    static_assert(BACK_PROBE_BED_POSITION <= MAX_PROBE_Y, "BACK_PROBE_BED_POSITION is outside the probe region.");

  #endif // AUTO_BED_LEVELING_3POINT

#elif ENABLED(MESH_BED_LEVELING)

  // Hide PROBE_MANUALLY from the rest of the code
  #undef PROBE_MANUALLY

  /**
   * Mesh Bed Leveling
   */

  #if ENABLED(DELTA)
    #error "MESH_BED_LEVELING is not compatible with DELTA printers."
  #elif GRID_MAX_POINTS_X > 9 || GRID_MAX_POINTS_Y > 9
    #error "GRID_MAX_POINTS_X and GRID_MAX_POINTS_Y must be less than 10 for MBL."
  #endif

#endif

#if HAS_MESH
  static_assert(DEFAULT_ZJERK > 0.1, "Low DEFAULT_ZJERK values are incompatible with mesh-based leveling.");
#elif ENABLED(G26_MESH_VALIDATION)
  #error "G26_MESH_VALIDATION requires MESH_BED_LEVELING, AUTO_BED_LEVELING_BILINEAR, or AUTO_BED_LEVELING_UBL."
#endif

#if ENABLED(MESH_EDIT_GFX_OVERLAY) && (DISABLED(AUTO_BED_LEVELING_UBL) || DISABLED(DOGLCD))
  #error "MESH_EDIT_GFX_OVERLAY requires AUTO_BED_LEVELING_UBL and a Graphical LCD."
#endif

/**
 * LCD_BED_LEVELING requirements
 */
#if ENABLED(LCD_BED_LEVELING)
  #if DISABLED(ULTIPANEL)
    #error "LCD_BED_LEVELING requires an LCD controller."
  #elif !(ENABLED(MESH_BED_LEVELING) || OLDSCHOOL_ABL)
    #error "LCD_BED_LEVELING requires MESH_BED_LEVELING or AUTO_BED_LEVELING."
  #endif
#endif

/**
 * Homing
 */
#if X_HOME_BUMP_MM < 0 || Y_HOME_BUMP_MM < 0 || Z_HOME_BUMP_MM < 0
  #error "[XYZ]_HOME_BUMP_MM must be greater than or equal to 0."
#endif

#if ENABLED(CODEPENDENT_XY_HOMING)
  #if ENABLED(QUICK_HOME)
    #error "QUICK_HOME is incompatible with CODEPENDENT_XY_HOMING."
  #elif IS_KINEMATIC
    #error "CODEPENDENT_XY_HOMING requires a Cartesian setup."
  #endif
#endif

/**
 * Make sure Z_SAFE_HOMING point is reachable
 */
#if ENABLED(Z_SAFE_HOMING)
  #if HAS_BED_PROBE
    static_assert(WITHIN(Z_SAFE_HOMING_X_POINT, MIN_PROBE_X, MAX_PROBE_X),
      "Z_SAFE_HOMING_X_POINT is outside the probe region.");
    static_assert(WITHIN(Z_SAFE_HOMING_Y_POINT, MIN_PROBE_Y, MAX_PROBE_Y),
      "Z_SAFE_HOMING_Y_POINT is outside the probe region.");
  #else
    static_assert(WITHIN(Z_SAFE_HOMING_X_POINT, X_MIN_POS, X_MAX_POS),
      "Z_SAFE_HOMING_X_POINT can't be reached by the nozzle.");
    static_assert(WITHIN(Z_SAFE_HOMING_Y_POINT, Y_MIN_POS, Y_MAX_POS),
      "Z_SAFE_HOMING_Y_POINT can't be reached by the nozzle.");
  #endif
#endif // Z_SAFE_HOMING

/**
 * Make sure DISABLE_[XYZ] compatible with selected homing options
 */
#if ENABLED(DISABLE_X) || ENABLED(DISABLE_Y) || ENABLED(DISABLE_Z)
  #if ENABLED(HOME_AFTER_DEACTIVATE) || ENABLED(Z_SAFE_HOMING)
    #error "DISABLE_[XYZ] is not compatible with HOME_AFTER_DEACTIVATE or Z_SAFE_HOMING."
  #endif
#endif // DISABLE_[XYZ]

/**
 * Filament Width Sensor
 */
#if ENABLED(FILAMENT_WIDTH_SENSOR)
  #if !HAS_FILAMENT_WIDTH_SENSOR
    #error "FILAMENT_WIDTH_SENSOR requires a FILWIDTH_PIN to be defined."
  #elif ENABLED(NO_VOLUMETRICS)
    #error "FILAMENT_WIDTH_SENSOR requires NO_VOLUMETRICS to be disabled."
  #endif
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
#if ENABLED(SAV_3DGLCD)
  #if DISABLED(U8GLIB_SSD1306) && DISABLED(U8GLIB_SH1106)
    #error "Enable a SAV_3DGLCD display type: U8GLIB_SSD1306 or U8GLIB_SH1106."
  #elif ENABLED(U8GLIB_SSD1306) && ENABLED(U8GLIB_SH1106)
    #error "Only enable one SAV_3DGLCD display type: U8GLIB_SSD1306 or U8GLIB_SH1106."
  #endif
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
  #elif CORE_IS_XY || CORE_IS_XZ
    #error "DUAL_X_CARRIAGE cannot be used with COREXY, COREYX, COREXZ, or COREZX."
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
    #if E0_AUTO_FAN_PIN == FAN_PIN
      #error "You cannot set E0_AUTO_FAN_PIN equal to FAN_PIN."
    #elif E1_AUTO_FAN_PIN == FAN_PIN
      #error "You cannot set E1_AUTO_FAN_PIN equal to FAN_PIN."
    #elif E2_AUTO_FAN_PIN == FAN_PIN
      #error "You cannot set E2_AUTO_FAN_PIN equal to FAN_PIN."
    #elif E3_AUTO_FAN_PIN == FAN_PIN
      #error "You cannot set E3_AUTO_FAN_PIN equal to FAN_PIN."
    #endif
  #endif
#endif

#if HAS_FAN0 && CONTROLLER_FAN_PIN == FAN_PIN
  #error "You cannot set CONTROLLER_FAN_PIN equal to FAN_PIN."
#endif

#if ENABLED(USE_CONTROLLER_FAN)
  #if !HAS_CONTROLLER_FAN
    #error "USE_CONTROLLER_FAN requires a CONTROLLER_FAN_PIN. Define in Configuration_adv.h."
  #elif E0_AUTO_FAN_PIN == CONTROLLER_FAN_PIN
    #error "You cannot set E0_AUTO_FAN_PIN equal to CONTROLLER_FAN_PIN."
  #elif E1_AUTO_FAN_PIN == CONTROLLER_FAN_PIN
    #error "You cannot set E1_AUTO_FAN_PIN equal to CONTROLLER_FAN_PIN."
  #elif E2_AUTO_FAN_PIN == CONTROLLER_FAN_PIN
    #error "You cannot set E2_AUTO_FAN_PIN equal to CONTROLLER_FAN_PIN."
  #elif E3_AUTO_FAN_PIN == CONTROLLER_FAN_PIN
    #error "You cannot set E3_AUTO_FAN_PIN equal to CONTROLLER_FAN_PIN."
  #endif
#endif

/**
 * Test Heater, Temp Sensor, and Extruder Pins; Sensor Type must also be set.
 */
#if !HAS_HEATER_0
  #error "HEATER_0_PIN not defined for this board."
#elif !PIN_EXISTS(TEMP_0) && !(defined(MAX6675_SS) && MAX6675_SS >= 0)
  #error "TEMP_0_PIN not defined for this board."
#elif ((defined(__AVR_ATmega644P__) || defined(__AVR_ATmega1284P__)) && (!PIN_EXISTS(E0_STEP) || !PIN_EXISTS(E0_DIR)))
  #error "E0_STEP_PIN or E0_DIR_PIN not defined for this board."
#elif ( !(defined(__AVR_ATmega644P__) || defined(__AVR_ATmega1284P__)) && (!PIN_EXISTS(E0_STEP) || !PIN_EXISTS(E0_DIR) || !PIN_EXISTS(E0_ENABLE)))
  #error "E0_STEP_PIN, E0_DIR_PIN, or E0_ENABLE_PIN not defined for this board."
#elif TEMP_SENSOR_0 == 0
  #error "TEMP_SENSOR_0 is required."
#endif

// Pins are required for heaters
#if ENABLED(HEATER_0_USES_MAX6675) && !(defined(MAX6675_SS) && MAX6675_SS >= 0)
  #error "MAX6675_SS (required for TEMP_SENSOR_0) not defined for this board."
#elif (HOTENDS > 1 || ENABLED(HEATERS_PARALLEL)) && !HAS_HEATER_1
  #error "HEATER_1_PIN not defined for this board."
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
        #error "TEMP_SENSOR_3 is required with 4 or more HOTENDS."
      #elif !HAS_HEATER_3
        #error "HEATER_3_PIN not defined for this board."
      #elif !PIN_EXISTS(TEMP_3)
        #error "TEMP_3_PIN not defined for this board."
      #endif
      #if HOTENDS > 4
        #if TEMP_SENSOR_4 == 0
          #error "TEMP_SENSOR_4 is required with 5 HOTENDS."
        #elif !HAS_HEATER_4
          #error "HEATER_4_PIN not defined for this board."
        #elif !PIN_EXISTS(TEMP_4)
          #error "TEMP_4_PIN not defined for this board."
        #endif
      #elif TEMP_SENSOR_4 != 0
        #error "TEMP_SENSOR_4 shouldn't be set with only 4 HOTENDS."
      #endif
    #elif TEMP_SENSOR_3 != 0
      #error "TEMP_SENSOR_3 shouldn't be set with only 3 HOTENDS."
    #elif TEMP_SENSOR_4 != 0
      #error "TEMP_SENSOR_4 shouldn't be set with only 3 HOTENDS."
    #endif
  #elif TEMP_SENSOR_2 != 0
    #error "TEMP_SENSOR_2 shouldn't be set with only 2 HOTENDS."
  #elif TEMP_SENSOR_3 != 0
    #error "TEMP_SENSOR_3 shouldn't be set with only 2 HOTENDS."
  #elif TEMP_SENSOR_4 != 0
    #error "TEMP_SENSOR_4 shouldn't be set with only 2 HOTENDS."
  #endif
#elif TEMP_SENSOR_1 != 0 && DISABLED(TEMP_SENSOR_1_AS_REDUNDANT)
  #error "TEMP_SENSOR_1 shouldn't be set with only 1 HOTEND."
#elif TEMP_SENSOR_2 != 0
  #error "TEMP_SENSOR_2 shouldn't be set with only 1 HOTEND."
#elif TEMP_SENSOR_3 != 0
  #error "TEMP_SENSOR_3 shouldn't be set with only 1 HOTEND."
#elif TEMP_SENSOR_4 != 0
  #error "TEMP_SENSOR_4 shouldn't be set with only 1 HOTEND."
#endif

#if ENABLED(TEMP_SENSOR_1_AS_REDUNDANT) && TEMP_SENSOR_1 == 0
  #error "TEMP_SENSOR_1 is required with TEMP_SENSOR_1_AS_REDUNDANT."
#endif

/**
 * Temperature status LEDs
 */
#if ENABLED(TEMP_STAT_LEDS) && !PIN_EXISTS(STAT_LED_RED) && !PIN_EXISTS(STAT_LED_BLUE)
  #error "TEMP_STAT_LEDS requires STAT_LED_RED_PIN or STAT_LED_BLUE_PIN, preferably both."
#endif

/**
 * LED Control Menu
 */
#if ENABLED(LED_CONTROL_MENU) && !HAS_COLOR_LEDS
  #error "LED_CONTROL_MENU requires BLINKM, RGB_LED, RGBW_LED, PCA9632, or NEOPIXEL_LED."
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
 * Test Extruder Stepper Pins
 */
#if DISABLED(MK2_MULTIPLEXER) // MK2_MULTIPLEXER uses E0 stepper only
  #if E_STEPPERS > 4
    #if !PIN_EXISTS(E4_STEP) || !PIN_EXISTS(E4_DIR) || !PIN_EXISTS(E4_ENABLE)
      #error "E4_STEP_PIN, E4_DIR_PIN, or E4_ENABLE_PIN not defined for this board."
    #endif
  #elif E_STEPPERS > 3
    #if !PIN_EXISTS(E3_STEP) || !PIN_EXISTS(E3_DIR) || !PIN_EXISTS(E3_ENABLE)
      #error "E3_STEP_PIN, E3_DIR_PIN, or E3_ENABLE_PIN not defined for this board."
    #endif
  #elif E_STEPPERS > 2
    #if !PIN_EXISTS(E2_STEP) || !PIN_EXISTS(E2_DIR) || !PIN_EXISTS(E2_ENABLE)
      #error "E2_STEP_PIN, E2_DIR_PIN, or E2_ENABLE_PIN not defined for this board."
    #endif
  #elif E_STEPPERS > 1
    #if !PIN_EXISTS(E1_STEP) || !PIN_EXISTS(E1_DIR) || !PIN_EXISTS(E1_ENABLE)
      #error "E1_STEP_PIN, E1_DIR_PIN, or E1_ENABLE_PIN not defined for this board."
    #endif
  #endif
#endif
/**
 * Endstop Tests
 */

#define _PLUG_UNUSED_TEST(AXIS,PLUG) (DISABLED(USE_##PLUG##MIN_PLUG) && DISABLED(USE_##PLUG##MAX_PLUG) && !(ENABLED(AXIS##_DUAL_ENDSTOPS) && WITHIN(AXIS##2_USE_ENDSTOP, _##PLUG##MAX_, _##PLUG##MIN_)))
#define _AXIS_PLUG_UNUSED_TEST(AXIS) (_PLUG_UNUSED_TEST(AXIS,X) && _PLUG_UNUSED_TEST(AXIS,Y) && _PLUG_UNUSED_TEST(AXIS,Z))

// At least 3 endstop plugs must be used
#if _AXIS_PLUG_UNUSED_TEST(X)
  #error "You must enable USE_XMIN_PLUG or USE_XMAX_PLUG."
#endif
#if _AXIS_PLUG_UNUSED_TEST(Y)
  #error "You must enable USE_YMIN_PLUG or USE_YMAX_PLUG."
#endif
#if _AXIS_PLUG_UNUSED_TEST(Z)
  #error "You must enable USE_ZMIN_PLUG or USE_ZMAX_PLUG."
#endif

// Delta and Cartesian use 3 homing endstops
#if !IS_SCARA
  #if X_HOME_DIR < 0 && DISABLED(USE_XMIN_PLUG)
    #error "Enable USE_XMIN_PLUG when homing X to MIN."
  #elif X_HOME_DIR > 0 && DISABLED(USE_XMAX_PLUG)
    #error "Enable USE_XMAX_PLUG when homing X to MAX."
  #elif Y_HOME_DIR < 0 && DISABLED(USE_YMIN_PLUG)
    #error "Enable USE_YMIN_PLUG when homing Y to MIN."
  #elif Y_HOME_DIR > 0 && DISABLED(USE_YMAX_PLUG)
    #error "Enable USE_YMAX_PLUG when homing Y to MAX."
  #endif
#endif
#if Z_HOME_DIR < 0 && DISABLED(USE_ZMIN_PLUG)
  #error "Enable USE_ZMIN_PLUG when homing Z to MIN."
#elif Z_HOME_DIR > 0 && DISABLED(USE_ZMAX_PLUG)
  #error "Enable USE_ZMAX_PLUG when homing Z to MAX."
#endif

// Dual endstops requirements
#if ENABLED(X_DUAL_ENDSTOPS)
  #if !X2_USE_ENDSTOP
    #error "You must set X2_USE_ENDSTOP with X_DUAL_ENDSTOPS."
  #elif X2_USE_ENDSTOP == _X_MIN_ && DISABLED(USE_XMIN_PLUG)
    #error "USE_XMIN_PLUG is required when X2_USE_ENDSTOP is _X_MIN_."
  #elif X2_USE_ENDSTOP == _X_MAX_ && DISABLED(USE_XMAX_PLUG)
    #error "USE_XMAX_PLUG is required when X2_USE_ENDSTOP is _X_MAX_."
  #elif X2_USE_ENDSTOP == _Y_MIN_ && DISABLED(USE_YMIN_PLUG)
    #error "USE_YMIN_PLUG is required when X2_USE_ENDSTOP is _Y_MIN_."
  #elif X2_USE_ENDSTOP == _Y_MAX_ && DISABLED(USE_YMAX_PLUG)
    #error "USE_YMAX_PLUG is required when X2_USE_ENDSTOP is _Y_MAX_."
  #elif X2_USE_ENDSTOP == _Z_MIN_ && DISABLED(USE_ZMIN_PLUG)
    #error "USE_ZMIN_PLUG is required when X2_USE_ENDSTOP is _Z_MIN_."
  #elif X2_USE_ENDSTOP == _Z_MAX_ && DISABLED(USE_ZMAX_PLUG)
    #error "USE_ZMAX_PLUG is required when X2_USE_ENDSTOP is _Z_MAX_."
  #elif !HAS_X2_MIN && !HAS_X2_MAX
    #error "X2_USE_ENDSTOP has been assigned to a nonexistent endstop!"
  #elif ENABLED(DELTA)
    #error "X_DUAL_ENDSTOPS is not compatible with DELTA."
  #endif
#endif
#if ENABLED(Y_DUAL_ENDSTOPS)
  #if !Y2_USE_ENDSTOP
    #error "You must set Y2_USE_ENDSTOP with Y_DUAL_ENDSTOPS."
  #elif Y2_USE_ENDSTOP == _X_MIN_ && DISABLED(USE_XMIN_PLUG)
    #error "USE_XMIN_PLUG is required when Y2_USE_ENDSTOP is _X_MIN_."
  #elif Y2_USE_ENDSTOP == _X_MAX_ && DISABLED(USE_XMAX_PLUG)
    #error "USE_XMAX_PLUG is required when Y2_USE_ENDSTOP is _X_MAX_."
  #elif Y2_USE_ENDSTOP == _Y_MIN_ && DISABLED(USE_YMIN_PLUG)
    #error "USE_YMIN_PLUG is required when Y2_USE_ENDSTOP is _Y_MIN_."
  #elif Y2_USE_ENDSTOP == _Y_MAX_ && DISABLED(USE_YMAX_PLUG)
    #error "USE_YMAX_PLUG is required when Y2_USE_ENDSTOP is _Y_MAX_."
  #elif Y2_USE_ENDSTOP == _Z_MIN_ && DISABLED(USE_ZMIN_PLUG)
    #error "USE_ZMIN_PLUG is required when Y2_USE_ENDSTOP is _Z_MIN_."
  #elif Y2_USE_ENDSTOP == _Z_MAX_ && DISABLED(USE_ZMAX_PLUG)
    #error "USE_ZMAX_PLUG is required when Y2_USE_ENDSTOP is _Z_MAX_."
  #elif !HAS_Y2_MIN && !HAS_Y2_MAX
    #error "Y2_USE_ENDSTOP has been assigned to a nonexistent endstop!"
  #elif ENABLED(DELTA)
    #error "Y_DUAL_ENDSTOPS is not compatible with DELTA."
  #endif
#endif
#if ENABLED(Z_DUAL_ENDSTOPS)
  #if !Z2_USE_ENDSTOP
    #error "You must set Z2_USE_ENDSTOP with Z_DUAL_ENDSTOPS."
  #elif Z2_USE_ENDSTOP == _X_MIN_ && DISABLED(USE_XMIN_PLUG)
    #error "USE_XMIN_PLUG is required when Z2_USE_ENDSTOP is _X_MIN_."
  #elif Z2_USE_ENDSTOP == _X_MAX_ && DISABLED(USE_XMAX_PLUG)
    #error "USE_XMAX_PLUG is required when Z2_USE_ENDSTOP is _X_MAX_."
  #elif Z2_USE_ENDSTOP == _Y_MIN_ && DISABLED(USE_YMIN_PLUG)
    #error "USE_YMIN_PLUG is required when Z2_USE_ENDSTOP is _Y_MIN_."
  #elif Z2_USE_ENDSTOP == _Y_MAX_ && DISABLED(USE_YMAX_PLUG)
    #error "USE_YMAX_PLUG is required when Z2_USE_ENDSTOP is _Y_MAX_."
  #elif Z2_USE_ENDSTOP == _Z_MIN_ && DISABLED(USE_ZMIN_PLUG)
    #error "USE_ZMIN_PLUG is required when Z2_USE_ENDSTOP is _Z_MIN_."
  #elif Z2_USE_ENDSTOP == _Z_MAX_ && DISABLED(USE_ZMAX_PLUG)
    #error "USE_ZMAX_PLUG is required when Z2_USE_ENDSTOP is _Z_MAX_."
  #elif !HAS_Z2_MIN && !HAS_Z2_MAX
    #error "Z2_USE_ENDSTOP has been assigned to a nonexistent endstop!"
  #elif ENABLED(DELTA)
    #error "Z_DUAL_ENDSTOPS is not compatible with DELTA."
  #endif
#endif

/**
 * emergency-command parser
 */
#if ENABLED(EMERGENCY_PARSER) && defined(__AVR__) && defined(USBCON)
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
 * RGB_LED Requirements
 */
#define _RGB_TEST (PIN_EXISTS(RGB_LED_R) && PIN_EXISTS(RGB_LED_G) && PIN_EXISTS(RGB_LED_B))
#if ENABLED(RGB_LED)
  #if !_RGB_TEST
    #error "RGB_LED requires RGB_LED_R_PIN, RGB_LED_G_PIN, and RGB_LED_B_PIN."
  #elif ENABLED(RGBW_LED)
    #error "Please enable only one of RGB_LED and RGBW_LED."
  #endif
#elif ENABLED(RGBW_LED)
  #if !(_RGB_TEST && PIN_EXISTS(RGB_LED_W))
    #error "RGBW_LED requires RGB_LED_R_PIN, RGB_LED_G_PIN, RGB_LED_B_PIN, and RGB_LED_W_PIN."
  #endif
#elif ENABLED(NEOPIXEL_LED)
  #if !(PIN_EXISTS(NEOPIXEL) && NEOPIXEL_PIXELS > 0)
    #error "NEOPIXEL_LED requires NEOPIXEL_PIN and NEOPIXEL_PIXELS."
  #endif
#elif ENABLED(PRINTER_EVENT_LEDS) && DISABLED(BLINKM) && DISABLED(PCA9632) && DISABLED(NEOPIXEL_LED)
  #error "PRINTER_EVENT_LEDS requires BLINKM, PCA9632, RGB_LED, RGBW_LED or NEOPIXEL_LED."
#endif

/**
 * Auto Fan check for PWM pins
 */
#if HAS_AUTO_FAN && EXTRUDER_AUTO_FAN_SPEED != 255
  #define AF_ERR_SUFF "_AUTO_FAN_PIN is not a PWM pin. Set EXTRUDER_AUTO_FAN_SPEED to 255."
  #if HAS_AUTO_FAN_0
    static_assert(GET_TIMER(E0_AUTO_FAN_PIN), "E0" AF_ERR_SUFF);
  #elif HAS_AUTO_FAN_1
    static_assert(GET_TIMER(E1_AUTO_FAN_PIN), "E1" AF_ERR_SUFF);
  #elif HAS_AUTO_FAN_2
    static_assert(GET_TIMER(E2_AUTO_FAN_PIN), "E2" AF_ERR_SUFF);
  #elif HAS_AUTO_FAN_3
    static_assert(GET_TIMER(E3_AUTO_FAN_PIN), "E3" AF_ERR_SUFF);
  #endif
#endif

/**
 * Make sure only one display is enabled
 *
 * Note: BQ_LCD_SMART_CONTROLLER => REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER
 *       REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER => REPRAP_DISCOUNT_SMART_CONTROLLER
 *       SAV_3DGLCD => U8GLIB_SH1106 => ULTIMAKERCONTROLLER
 *       MKS_12864OLED => U8GLIB_SH1106 => ULTIMAKERCONTROLLER
 *       MKS_12864OLED_SSD1306 => U8GLIB_SSD1306 => ULTIMAKERCONTROLLER
 *       MKS_MINI_12864 => MINIPANEL
 *       miniVIKI => ULTIMAKERCONTROLLER
 *       VIKI2 => ULTIMAKERCONTROLLER
 *       ELB_FULL_GRAPHIC_CONTROLLER => ULTIMAKERCONTROLLER
 *       AZSMZ_12864 => ULTIMAKERCONTROLLER
 *       PANEL_ONE => ULTIMAKERCONTROLLER
 */
#if 1 < 0 \
  + (     ENABLED(ULTIMAKERCONTROLLER) \
      && DISABLED(SAV_3DGLCD) \
      && DISABLED(miniVIKI) \
      && DISABLED(VIKI2) \
      && DISABLED(ELB_FULL_GRAPHIC_CONTROLLER) \
      && DISABLED(AZSMZ_12864) \
      && DISABLED(PANEL_ONE) \
      && DISABLED(MKS_12864OLED) \
      && DISABLED(MKS_12864OLED_SSD1306) ) \
  + (     ENABLED(REPRAP_DISCOUNT_SMART_CONTROLLER) \
      && DISABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER) \
      && DISABLED(LCD_FOR_MELZI) \
      && DISABLED(MAKEBOARD_MINI_2_LINE_DISPLAY_1602) \
      && DISABLED(MKS_12864OLED) \
      && DISABLED(MKS_12864OLED_SSD1306) ) \
  + (ENABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER) && DISABLED(BQ_LCD_SMART_CONTROLLER)) \
  + ENABLED(LCD_FOR_MELZI) \
  + ENABLED(MKS_12864OLED) \
  + ENABLED(MKS_12864OLED_SSD1306) \
  + ENABLED(MAKEBOARD_MINI_2_LINE_DISPLAY_1602) \
  + ENABLED(CARTESIO_UI) \
  + ENABLED(PANEL_ONE) \
  + ENABLED(MAKRPANEL) \
  + ENABLED(REPRAPWORLD_GRAPHICAL_LCD) \
  + ENABLED(VIKI2) \
  + ENABLED(miniVIKI) \
  + ENABLED(ELB_FULL_GRAPHIC_CONTROLLER) \
  + ENABLED(AZSMZ_12864) \
  + ENABLED(G3D_PANEL) \
  + (ENABLED(MINIPANEL) && DISABLED(MKS_MINI_12864)) \
  + ENABLED(MKS_MINI_12864) \
  + (ENABLED(REPRAPWORLD_KEYPAD) && DISABLED(CARTESIO_UI) && DISABLED(ZONESTAR_LCD)) \
  + ENABLED(RIGIDBOT_PANEL) \
  + ENABLED(RA_CONTROL_PANEL) \
  + ENABLED(LCD_SAINSMART_I2C_1602) \
  + ENABLED(LCD_SAINSMART_I2C_2004) \
  + ENABLED(LCM1602) \
  + ENABLED(LCD_I2C_PANELOLU2) \
  + ENABLED(LCD_I2C_VIKI) \
  + (ENABLED(U8GLIB_SSD1306) && DISABLED(OLED_PANEL_TINYBOY2) && DISABLED(MKS_12864OLED_SSD1306)) \
  + ENABLED(SAV_3DLCD) \
  + ENABLED(BQ_LCD_SMART_CONTROLLER) \
  + ENABLED(SAV_3DGLCD) \
  + ENABLED(OLED_PANEL_TINYBOY2) \
  + ENABLED(ZONESTAR_LCD) \
  + ENABLED(ULTI_CONTROLLER)
  #error "Please select no more than one LCD controller option."
#endif

/**
 * Make sure HAVE_TMC26X is warranted
 */
#if ENABLED(HAVE_TMC26X) && !( \
         ENABLED(  X_IS_TMC26X ) \
      || ENABLED( X2_IS_TMC26X ) \
      || ENABLED(  Y_IS_TMC26X ) \
      || ENABLED( Y2_IS_TMC26X ) \
      || ENABLED(  Z_IS_TMC26X ) \
      || ENABLED( Z2_IS_TMC26X ) \
      || ENABLED( E0_IS_TMC26X ) \
      || ENABLED( E1_IS_TMC26X ) \
      || ENABLED( E2_IS_TMC26X ) \
      || ENABLED( E3_IS_TMC26X ) \
      || ENABLED( E4_IS_TMC26X ) \
  )
  #error "HAVE_TMC26X requires at least one TMC26X stepper to be set."
#endif

/**
 * Make sure HAVE_TMC2130 is warranted
 */
#if ENABLED(HAVE_TMC2130)
  #if !( ENABLED(  X_IS_TMC2130 ) \
      || ENABLED( X2_IS_TMC2130 ) \
      || ENABLED(  Y_IS_TMC2130 ) \
      || ENABLED( Y2_IS_TMC2130 ) \
      || ENABLED(  Z_IS_TMC2130 ) \
      || ENABLED( Z2_IS_TMC2130 ) \
      || ENABLED( E0_IS_TMC2130 ) \
      || ENABLED( E1_IS_TMC2130 ) \
      || ENABLED( E2_IS_TMC2130 ) \
      || ENABLED( E3_IS_TMC2130 ) \
      || ENABLED( E4_IS_TMC2130 ) )
    #error "HAVE_TMC2130 requires at least one TMC2130 stepper to be set."
  #elif ENABLED(HYBRID_THRESHOLD) && DISABLED(STEALTHCHOP)
    #error "Enable STEALTHCHOP to use HYBRID_THRESHOLD."
  #endif

  #if ENABLED(X_IS_TMC2130) && !PIN_EXISTS(X_CS)
    #error "X_CS_PIN is required for X_IS_TMC2130. Define X_CS_PIN in Configuration_adv.h."
  #elif ENABLED(X2_IS_TMC2130) && !PIN_EXISTS(X2_CS)
    #error "X2_CS_PIN is required for X2_IS_TMC2130. Define X2_CS_PIN in Configuration_adv.h."
  #elif ENABLED(Y_IS_TMC2130) && !PIN_EXISTS(Y_CS)
    #error "Y_CS_PIN is required for Y_IS_TMC2130. Define Y_CS_PIN in Configuration_adv.h."
  #elif ENABLED(Y2_IS_TMC2130) && !PIN_EXISTS(Y2_CS)
    #error "Y2_CS_PIN is required for Y2_IS_TMC2130. Define Y2_CS_PIN in Configuration_adv.h."
  #elif ENABLED(Z_IS_TMC2130) && !PIN_EXISTS(Z_CS)
    #error "Z_CS_PIN is required for Z_IS_TMC2130. Define Z_CS_PIN in Configuration_adv.h."
  #elif ENABLED(Z2_IS_TMC2130) && !PIN_EXISTS(Z2_CS)
    #error "Z2_CS_PIN is required for Z2_IS_TMC2130. Define Z2_CS_PIN in Configuration_adv.h."
  #elif ENABLED(E0_IS_TMC2130) && !PIN_EXISTS(E0_CS)
    #error "E0_CS_PIN is required for E0_IS_TMC2130. Define E0_CS_PIN in Configuration_adv.h."
  #elif ENABLED(E1_IS_TMC2130) && !PIN_EXISTS(E1_CS)
    #error "E1_CS_PIN is required for E1_IS_TMC2130. Define E1_CS_PIN in Configuration_adv.h."
  #elif ENABLED(E2_IS_TMC2130) && !PIN_EXISTS(E2_CS)
    #error "E2_CS_PIN is required for E2_IS_TMC2130. Define E2_CS_PIN in Configuration_adv.h."
  #elif ENABLED(E3_IS_TMC2130) && !PIN_EXISTS(E3_CS)
    #error "E3_CS_PIN is required for E3_IS_TMC2130. Define E3_CS_PIN in Configuration_adv.h."
  #elif ENABLED(E4_IS_TMC2130) && !PIN_EXISTS(E4_CS)
    #error "E4_CS_PIN is required for E4_IS_TMC2130. Define E4_CS_PIN in Configuration_adv.h."
  #endif

  #if ENABLED(SENSORLESS_HOMING)
    // Require STEALTHCHOP for SENSORLESS_HOMING on DELTA as the transition from spreadCycle to stealthChop
    // is necessary in order to reset the stallGuard indication between the initial movement of all three
    // towers to +Z and the individual homing of each tower. This restriction can be removed once a means of
    // clearing the stallGuard activated status is found.
    #if ENABLED(DELTA) && !ENABLED(STEALTHCHOP)
      #error "SENSORLESS_HOMING on DELTA currently requires STEALTHCHOP."
    #elif X_HOME_DIR == -1 && DISABLED(X_MIN_ENDSTOP_INVERTING)
      #error "SENSORLESS_HOMING requires X_MIN_ENDSTOP_INVERTING when homing to X_MIN."
    #elif X_HOME_DIR ==  1 && DISABLED(X_MAX_ENDSTOP_INVERTING)
      #error "SENSORLESS_HOMING requires X_MAX_ENDSTOP_INVERTING when homing to X_MAX."
    #elif Y_HOME_DIR == -1 && DISABLED(Y_MIN_ENDSTOP_INVERTING)
      #error "SENSORLESS_HOMING requires Y_MIN_ENDSTOP_INVERTING when homing to Y_MIN."
    #elif Y_HOME_DIR ==  1 && DISABLED(Y_MAX_ENDSTOP_INVERTING)
      #error "SENSORLESS_HOMING requires Y_MAX_ENDSTOP_INVERTING when homing to Y_MAX."
    #endif
  #endif

  // Sensorless homing is required for both combined steppers in an H-bot
  #if CORE_IS_XY && X_SENSORLESS != Y_SENSORLESS
    #error "CoreXY requires both X and Y to use sensorless homing if either does."
  #elif CORE_IS_XZ && X_SENSORLESS != Z_SENSORLESS
    #error "CoreXZ requires both X and Z to use sensorless homing if either does."
  #elif CORE_IS_YZ && Y_SENSORLESS != Z_SENSORLESS
    #error "CoreYZ requires both Y and Z to use sensorless homing if either does."
  #endif

#elif ENABLED(SENSORLESS_HOMING)

  #error "SENSORLESS_HOMING requires TMC2130 stepper drivers."

#endif

/**
 * Make sure HAVE_TMC2208 is warranted
 */
#if ENABLED(HAVE_TMC2208) && !( \
       ENABLED(  X_IS_TMC2208 ) \
    || ENABLED( X2_IS_TMC2208 ) \
    || ENABLED(  Y_IS_TMC2208 ) \
    || ENABLED( Y2_IS_TMC2208 ) \
    || ENABLED(  Z_IS_TMC2208 ) \
    || ENABLED( Z2_IS_TMC2208 ) \
    || ENABLED( E0_IS_TMC2208 ) \
    || ENABLED( E1_IS_TMC2208 ) \
    || ENABLED( E2_IS_TMC2208 ) \
    || ENABLED( E3_IS_TMC2208 ) )
  #error "HAVE_TMC2208 requires at least one TMC2208 stepper to be set."
#endif

/**
 * TMC2208 software UART and ENDSTOP_INTERRUPTS both use pin change interrupts (PCI)
 */
#if ENABLED(HAVE_TMC2208) && ENABLED(ENDSTOP_INTERRUPTS_FEATURE) && !( \
       defined(X_HARDWARE_SERIAL ) \
    || defined(X2_HARDWARE_SERIAL) \
    || defined(Y_HARDWARE_SERIAL ) \
    || defined(Y2_HARDWARE_SERIAL) \
    || defined(Z_HARDWARE_SERIAL ) \
    || defined(Z2_HARDWARE_SERIAL) \
    || defined(E0_HARDWARE_SERIAL) \
    || defined(E1_HARDWARE_SERIAL) \
    || defined(E2_HARDWARE_SERIAL) \
    || defined(E3_HARDWARE_SERIAL) \
    || defined(E4_HARDWARE_SERIAL) )
  #error "select hardware UART for TMC2208 to use both TMC2208 and ENDSTOP_INTERRUPTS_FEATURE."
#endif

#if ENABLED(HYBRID_THRESHOLD) && DISABLED(STEALTHCHOP)
  #error "Enable STEALTHCHOP to use HYBRID_THRESHOLD."
#endif

#if ENABLED(TMC_Z_CALIBRATION) && !Z_IS_TRINAMIC && !Z2_IS_TRINAMIC
  #error "TMC_Z_CALIBRATION requires at least one TMC driver on Z axis"
#endif

/**
 * Make sure HAVE_L6470DRIVER is warranted
 */
#if ENABLED(HAVE_L6470DRIVER) && !( \
         ENABLED(  X_IS_L6470 ) \
      || ENABLED( X2_IS_L6470 ) \
      || ENABLED(  Y_IS_L6470 ) \
      || ENABLED( Y2_IS_L6470 ) \
      || ENABLED(  Z_IS_L6470 ) \
      || ENABLED( Z2_IS_L6470 ) \
      || ENABLED( E0_IS_L6470 ) \
      || ENABLED( E1_IS_L6470 ) \
      || ENABLED( E2_IS_L6470 ) \
      || ENABLED( E3_IS_L6470 ) \
      || ENABLED( E4_IS_L6470 ) \
  )
  #error "HAVE_L6470DRIVER requires at least one L6470 stepper to be set."
#endif

/**
 * Check that each axis has only one driver selected
 */
#if 1 < 0 \
  + ENABLED(X_IS_TMC26X) \
  + ENABLED(X_IS_TMC2130) \
  + ENABLED(X_IS_TMC2208) \
  + ENABLED(X_IS_L6470)
  #error "Please enable only one stepper driver for the X axis."
#endif
#if 1 < 0 \
  + ENABLED(X2_IS_TMC26X) \
  + ENABLED(X2_IS_TMC2130) \
  + ENABLED(X2_IS_TMC2208) \
  + ENABLED(X2_IS_L6470)
  #error "Please enable only one stepper driver for the X2 axis."
#endif
#if 1 < 0 \
  + ENABLED(Y_IS_TMC26X) \
  + ENABLED(Y_IS_TMC2130) \
  + ENABLED(Y_IS_TMC2208) \
  + ENABLED(Y_IS_L6470)
  #error "Please enable only one stepper driver for the Y axis."
#endif
#if 1 < 0 \
  + ENABLED(Y2_IS_TMC26X) \
  + ENABLED(Y2_IS_TMC2130) \
  + ENABLED(Y2_IS_TMC2208) \
  + ENABLED(Y2_IS_L6470)
  #error "Please enable only one stepper driver for the Y2 axis."
#endif
#if 1 < 0 \
  + ENABLED(Z_IS_TMC26X) \
  + ENABLED(Z_IS_TMC2130) \
  + ENABLED(Z_IS_TMC2208) \
  + ENABLED(Z_IS_L6470)
  #error "Please enable only one stepper driver for the Z axis."
#endif
#if 1 < 0 \
  + ENABLED(Z2_IS_TMC26X) \
  + ENABLED(Z2_IS_TMC2130) \
  + ENABLED(Z2_IS_TMC2208) \
  + ENABLED(Z2_IS_L6470)
  #error "Please enable only one stepper driver for the Z2 axis."
#endif
#if 1 < 0 \
  + ENABLED(E0_IS_TMC26X) \
  + ENABLED(E0_IS_TMC2130) \
  + ENABLED(E0_IS_TMC2208) \
  + ENABLED(E0_IS_L6470)
  #error "Please enable only one stepper driver for the E0 axis."
#endif
#if 1 < 0 \
  + ENABLED(E1_IS_TMC26X) \
  + ENABLED(E1_IS_TMC2130) \
  + ENABLED(E1_IS_TMC2208) \
  + ENABLED(E1_IS_L6470)
  #error "Please enable only one stepper driver for the E1 axis."
#endif
#if 1 < 0 \
  + ENABLED(E2_IS_TMC26X) \
  + ENABLED(E2_IS_TMC2130) \
  + ENABLED(E2_IS_TMC2208) \
  + ENABLED(E2_IS_L6470)
  #error "Please enable only one stepper driver for the E2 axis."
#endif
#if 1 < 0 \
  + ENABLED(E3_IS_TMC26X) \
  + ENABLED(E3_IS_TMC2130) \
  + ENABLED(E3_IS_TMC2208) \
  + ENABLED(E3_IS_L6470)
  #error "Please enable only one stepper driver for the E3 axis."
#endif
#if 1 < 0 \
  + ENABLED(E4_IS_TMC26X) \
  + ENABLED(E4_IS_TMC2130) \
  + ENABLED(E4_IS_TMC2208) \
  + ENABLED(E4_IS_L6470)
  #error "Please enable only one stepper driver for the E4 axis."
#endif

/**
 * Digipot requirement
 */
#if ENABLED(DIGIPOT_MCP4018)
  #if !defined(DIGIPOTS_I2C_SDA_X) || !defined(DIGIPOTS_I2C_SDA_Y) || !defined(DIGIPOTS_I2C_SDA_Z) \
    || !defined(DIGIPOTS_I2C_SDA_E0) || !defined(DIGIPOTS_I2C_SDA_E1)
      #error "DIGIPOT_MCP4018 requires DIGIPOTS_I2C_SDA_* pins to be defined."
  #endif
#endif

/**
 * Require 4 or more elements in per-axis initializers
 */
constexpr float sanity_arr_1[] = DEFAULT_AXIS_STEPS_PER_UNIT,
                sanity_arr_2[] = DEFAULT_MAX_FEEDRATE,
                sanity_arr_3[] = DEFAULT_MAX_ACCELERATION;
static_assert(COUNT(sanity_arr_1) >= XYZE, "DEFAULT_AXIS_STEPS_PER_UNIT requires 4 (or more) elements.");
static_assert(COUNT(sanity_arr_2) >= XYZE, "DEFAULT_MAX_FEEDRATE requires 4 (or more) elements.");
static_assert(COUNT(sanity_arr_3) >= XYZE, "DEFAULT_MAX_ACCELERATION requires 4 (or more) elements.");
static_assert(COUNT(sanity_arr_1) <= XYZE_N, "DEFAULT_AXIS_STEPS_PER_UNIT has too many elements.");
static_assert(COUNT(sanity_arr_2) <= XYZE_N, "DEFAULT_MAX_FEEDRATE has too many elements.");
static_assert(COUNT(sanity_arr_3) <= XYZE_N, "DEFAULT_MAX_ACCELERATION has too many elements.");

#if ENABLED(CNC_COORDINATE_SYSTEMS) && ENABLED(NO_WORKSPACE_OFFSETS)
  #error "CNC_COORDINATE_SYSTEMS is incompatible with NO_WORKSPACE_OFFSETS."
#endif

#if !BLOCK_BUFFER_SIZE || !IS_POWER_OF_2(BLOCK_BUFFER_SIZE)
  #error "BLOCK_BUFFER_SIZE must be a power of 2."
#endif

#if ENABLED(LED_CONTROL_MENU) && DISABLED(ULTIPANEL)
  #error "LED_CONTROL_MENU requires an LCD controller."
#endif

#if ENABLED(CASE_LIGHT_USE_NEOPIXEL) && DISABLED(NEOPIXEL_LED)
  #error "CASE_LIGHT_USE_NEOPIXEL requires NEOPIXEL_LED."
#endif

#if ENABLED(SKEW_CORRECTION)
  #if !defined(XY_SKEW_FACTOR) && !(defined(XY_DIAG_AC) && defined(XY_DIAG_BD) && defined(XY_SIDE_AD))
    #error "SKEW_CORRECTION requires XY_SKEW_FACTOR or XY_DIAG_AC, XY_DIAG_BD, XY_SIDE_AD."
  #endif
  #if ENABLED(SKEW_CORRECTION_FOR_Z)
    #if !defined(XZ_SKEW_FACTOR) && !(defined(XZ_DIAG_AC) && defined(XZ_DIAG_BD) && defined(XZ_SIDE_AD))
      #error "SKEW_CORRECTION requires XZ_SKEW_FACTOR or XZ_DIAG_AC, XZ_DIAG_BD, XZ_SIDE_AD."
    #endif
    #if !defined(YZ_SKEW_FACTOR) && !(defined(YZ_DIAG_AC) && defined(YZ_DIAG_BD) && defined(YZ_SIDE_AD))
      #error "SKEW_CORRECTION requires YZ_SKEW_FACTOR or YZ_DIAG_AC, YZ_DIAG_BD, YZ_SIDE_AD."
    #endif
  #endif
#endif

#if ENABLED(POWER_LOSS_RECOVERY) && !ENABLED(ULTIPANEL)
  #error "POWER_LOSS_RECOVERY currently requires an LCD Controller."
#endif

#endif // _SANITYCHECK_H_
