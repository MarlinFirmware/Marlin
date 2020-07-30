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
 * SanityCheck.h
 *
 * Test configuration values for errors at compile-time.
 */

/**
 * Require gcc 4.7 or newer (first included with Arduino 1.6.8) for C++11 features.
 */
#if __cplusplus < 201103L
  #error "Marlin requires C++11 support (gcc >= 4.7, Arduino IDE >= 1.6.8). Please upgrade your toolchain."
#endif

// Make sure macros aren't borked
#define TEST1
#define TEST2 1
#define TEST3 0
#define TEST4 true
#if ENABLED(TEST0)
  #error "ENABLED is borked!"
#endif
#if DISABLED(TEST1)
  #error "DISABLED is borked!"
#endif
#if !ENABLED(TEST2)
  #error "ENABLED is borked!"
#endif
#if ENABLED(TEST3)
  #error "ENABLED is borked!"
#endif
#if DISABLED(TEST4)
  #error "DISABLED is borked!"
#endif
#if !ANY(TEST1, TEST2, TEST3, TEST4) || ANY(TEST0, TEST3)
  #error "ANY is borked!"
#endif
#if DISABLED(TEST0, TEST1, TEST2, TEST4)
  #error "DISABLED is borked!"
#endif
#undef TEST1
#undef TEST2
#undef TEST3
#undef TEST4

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
#undef HEXIFY

/**
 * Warnings for old configurations
 */
#ifndef MOTHERBOARD
  #error "MOTHERBOARD is required. Please update your configuration."
#elif !defined(X_BED_SIZE) || !defined(Y_BED_SIZE)
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
  #error "SDCARDDETECTINVERTED is now SD_DETECT_STATE (HIGH). Please update your configuration."
#elif defined(SD_DETECT_INVERTED)
  #error "SD_DETECT_INVERTED is now SD_DETECT_STATE (HIGH). Please update your configuration."
#elif defined(BTENABLED)
  #error "BTENABLED is now BLUETOOTH. Please update your configuration."
#elif defined(CUSTOM_MENDEL_NAME)
  #error "CUSTOM_MENDEL_NAME is now CUSTOM_MACHINE_NAME. Please update your configuration."
#elif defined(HAS_AUTOMATIC_VERSIONING)
  #error "HAS_AUTOMATIC_VERSIONING is now CUSTOM_VERSION_FILE. Please update your configuration."
#elif defined(USE_AUTOMATIC_VERSIONING)
  #error "USE_AUTOMATIC_VERSIONING is now CUSTOM_VERSION_FILE. Please update your configuration."
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
#elif defined(FILAMENT_CHANGE_NO_STEPPER_TIMEOUT)
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
#elif defined(BLTOUCH_V3)
  #error "BLTOUCH_V3 is obsolete. Please update your configuration."
#elif defined(BLTOUCH_FORCE_OPEN_DRAIN_MODE)
  #error "BLTOUCH_FORCE_OPEN_DRAIN_MODE is obsolete. Please update your configuration."
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
#elif defined(PID_FAN_SCALING) && !HAS_FAN
  #error "PID_FAN_SCALING needs at least one fan enabled."
#elif defined(min_software_endstops) || defined(max_software_endstops)
  #error "(min|max)_software_endstops are now (MIN|MAX)_SOFTWARE_ENDSTOPS. Please update your configuration."
#elif ENABLED(Z_PROBE_SLED) && defined(SLED_PIN)
  #error "Replace SLED_PIN with SOL1_PIN (applies to both Z_PROBE_SLED and SOLENOID_PROBE)."
#elif defined(CONTROLLERFAN_PIN)
  #error "CONTROLLERFAN_PIN is now CONTROLLER_FAN_PIN, enabled with USE_CONTROLLER_FAN. Please update your Configuration_adv.h."
#elif defined(CONTROLLERFAN_SPEED)
  #error "CONTROLLERFAN_SPEED is now CONTROLLERFAN_SPEED_ACTIVE. Please update your Configuration_adv.h."
#elif defined(CONTROLLERFAN_SECS)
  #error "CONTROLLERFAN_SECS is now CONTROLLERFAN_IDLE_TIME. Please update your Configuration_adv.h."
#elif defined(MIN_RETRACT)
  #error "MIN_RETRACT is now MIN_AUTORETRACT and MAX_AUTORETRACT. Please update your Configuration_adv.h."
#elif defined(ADVANCE)
  #error "ADVANCE was removed in Marlin 1.1.6. Please use LIN_ADVANCE."
#elif defined(LIN_ADVANCE_E_D_RATIO)
  #error "LIN_ADVANCE (1.5) no longer uses LIN_ADVANCE_E_D_RATIO. Check your configuration."
#elif defined(NEOPIXEL_RGBW_LED)
  #error "NEOPIXEL_RGBW_LED is now NEOPIXEL_LED. Please update your configuration."
#elif ENABLED(DELTA) && defined(DELTA_PROBEABLE_RADIUS)
  #error "Remove DELTA_PROBEABLE_RADIUS and use PROBING_MARGIN to inset the probe area instead."
#elif ENABLED(DELTA) && defined(DELTA_CALIBRATION_RADIUS)
  #error "Remove DELTA_CALIBRATION_RADIUS and use PROBING_MARGIN to inset the probe area instead."
#elif defined(UBL_MESH_INSET)
  #error "UBL_MESH_INSET is now just MESH_INSET. Please update your configuration."
#elif defined(UBL_MESH_MIN_X) || defined(UBL_MESH_MIN_Y) || defined(UBL_MESH_MAX_X) || defined(UBL_MESH_MAX_Y)
  #error "UBL_MESH_(MIN|MAX)_[XY] is now just MESH_(MIN|MAX)_[XY]. Please update your configuration."
#elif defined(ABL_PROBE_PT_1_X) || defined(ABL_PROBE_PT_1_Y) || defined(ABL_PROBE_PT_2_X) || defined(ABL_PROBE_PT_2_Y) || defined(ABL_PROBE_PT_3_X) || defined(ABL_PROBE_PT_3_Y)
  #error "ABL_PROBE_PT_[123]_[XY] is no longer required. Please remove it from Configuration.h."
#elif defined(UBL_PROBE_PT_1_X) || defined(UBL_PROBE_PT_1_Y) || defined(UBL_PROBE_PT_2_X) || defined(UBL_PROBE_PT_2_Y) || defined(UBL_PROBE_PT_3_X) || defined(UBL_PROBE_PT_3_Y)
  #error "UBL_PROBE_PT_[123]_[XY] is no longer required. Please remove it from Configuration.h."
#elif defined(MIN_PROBE_EDGE)
  #error "MIN_PROBE_EDGE is now called PROBING_MARGIN. Please update your configuration."
#elif defined(MIN_PROBE_EDGE_LEFT)
  #error "MIN_PROBE_EDGE_LEFT is now called PROBING_MARGIN_LEFT. Please update your configuration."
#elif defined(MIN_PROBE_EDGE_RIGHT)
  #error "MIN_PROBE_EDGE_RIGHT is now called PROBING_MARGIN_RIGHT. Please update your configuration."
#elif defined(MIN_PROBE_EDGE_FRONT)
  #error "MIN_PROBE_EDGE_FRONT is now called PROBING_MARGIN_FRONT. Please update your configuration."
#elif defined(MIN_PROBE_EDGE_BACK)
  #error "MIN_PROBE_EDGE_BACK is now called PROBING_MARGIN_BACK. Please update your configuration."
#elif defined(LEFT_PROBE_BED_POSITION)
  #error "LEFT_PROBE_BED_POSITION is obsolete. Set a margin with PROBING_MARGIN or PROBING_MARGIN_LEFT instead."
#elif defined(RIGHT_PROBE_BED_POSITION)
  #error "RIGHT_PROBE_BED_POSITION is obsolete. Set a margin with PROBING_MARGIN or PROBING_MARGIN_RIGHT instead."
#elif defined(FRONT_PROBE_BED_POSITION)
  #error "FRONT_PROBE_BED_POSITION is obsolete. Set a margin with PROBING_MARGIN or PROBING_MARGIN_FRONT instead."
#elif defined(BACK_PROBE_BED_POSITION)
  #error "BACK_PROBE_BED_POSITION is obsolete. Set a margin with PROBING_MARGIN or PROBING_MARGIN_BACK instead."
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
  #error "HAVE_TMCDRIVER is now [AXIS]_DRIVER_TYPE TMC26X. Please update your Configuration.h."
#elif defined(STEALTHCHOP)
  #error "STEALTHCHOP is now STEALTHCHOP_(XY|Z|E). Please update your Configuration_adv.h."
#elif defined(HAVE_TMC26X)
  #error "HAVE_TMC26X is now [AXIS]_DRIVER_TYPE TMC26X. Please update your Configuration.h."
#elif defined(HAVE_TMC2130)
  #error "HAVE_TMC2130 is now [AXIS]_DRIVER_TYPE TMC2130. Please update your Configuration.h."
#elif defined(HAVE_TMC2208)
  #error "HAVE_TMC2208 is now [AXIS]_DRIVER_TYPE TMC2208. Please update your Configuration.h."
#elif defined(HAVE_L6470DRIVER)
  #error "HAVE_L6470DRIVER is now [AXIS]_DRIVER_TYPE L6470. Please update your Configuration.h."
#elif defined(X_IS_TMC) || defined(X2_IS_TMC) || defined(Y_IS_TMC) || defined(Y2_IS_TMC) || defined(Z_IS_TMC) || defined(Z2_IS_TMC) || defined(Z3_IS_TMC) \
   || defined(E0_IS_TMC) || defined(E1_IS_TMC) || defined(E2_IS_TMC) || defined(E3_IS_TMC) || defined(E4_IS_TMC) || defined(E5_IS_TMC) || defined(E6_IS_TMC) || defined(E7_IS_TMC)
  #error "[AXIS]_IS_TMC is now [AXIS]_DRIVER_TYPE TMC26X. Please update your Configuration.h."
#elif defined(X_IS_TMC26X) || defined(X2_IS_TMC26X) || defined(Y_IS_TMC26X) || defined(Y2_IS_TMC26X) || defined(Z_IS_TMC26X) || defined(Z2_IS_TMC26X) || defined(Z3_IS_TMC26X) \
   || defined(E0_IS_TMC26X) || defined(E1_IS_TMC26X) || defined(E2_IS_TMC26X) || defined(E3_IS_TMC26X) || defined(E4_IS_TMC26X) || defined(E5_IS_TMC26X) || defined(E6_IS_TMC26X) || defined(E7_IS_TMC26X)
  #error "[AXIS]_IS_TMC26X is now [AXIS]_DRIVER_TYPE TMC26X. Please update your Configuration.h."
#elif defined(X_IS_TMC2130) || defined(X2_IS_TMC2130) || defined(Y_IS_TMC2130) || defined(Y2_IS_TMC2130) || defined(Z_IS_TMC2130) || defined(Z2_IS_TMC2130) || defined(Z3_IS_TMC2130) \
   || defined(E0_IS_TMC2130) || defined(E1_IS_TMC2130) || defined(E2_IS_TMC2130) || defined(E3_IS_TMC2130) || defined(E4_IS_TMC2130) || defined(E5_IS_TMC2130) || defined(E6_IS_TMC2130) || defined(E7_IS_TMC2130)
  #error "[AXIS]_IS_TMC2130 is now [AXIS]_DRIVER_TYPE TMC2130. Please update your Configuration.h."
#elif defined(X_IS_TMC2208) || defined(X2_IS_TMC2208) || defined(Y_IS_TMC2208) || defined(Y2_IS_TMC2208) || defined(Z_IS_TMC2208) || defined(Z2_IS_TMC2208) || defined(Z3_IS_TMC2208) \
   || defined(E0_IS_TMC2208) || defined(E1_IS_TMC2208) || defined(E2_IS_TMC2208) || defined(E3_IS_TMC2208) || defined(E4_IS_TMC2208) || defined(E5_IS_TMC2208) || defined(E6_IS_TMC2208) || defined(E7_IS_TMC2208)
  #error "[AXIS]_IS_TMC2208 is now [AXIS]_DRIVER_TYPE TMC2208. Please update your Configuration.h."
#elif defined(X_IS_L6470) || defined(X2_IS_L6470) || defined(Y_IS_L6470) || defined(Y2_IS_L6470) || defined(Z_IS_L6470) || defined(Z2_IS_L6470) || defined(Z3_IS_L6470) \
   || defined(E0_IS_L6470) || defined(E1_IS_L6470) || defined(E2_IS_L6470) || defined(E3_IS_L6470) || defined(E4_IS_L6470) || defined(E5_IS_L6470) || defined(E6_IS_L6470) || defined(E7_IS_L6470)
  #error "[AXIS]_IS_L6470 is now [AXIS]_DRIVER_TYPE L6470. Please update your Configuration.h."
#elif defined(AUTOMATIC_CURRENT_CONTROL)
  #error "AUTOMATIC_CURRENT_CONTROL is now MONITOR_DRIVER_STATUS. Please update your configuration."
#elif defined(FILAMENT_CHANGE_LOAD_LENGTH)
  #error "FILAMENT_CHANGE_LOAD_LENGTH is now FILAMENT_CHANGE_FAST_LOAD_LENGTH. Please update your configuration."
#elif defined(LEVEL_CORNERS_INSET)
  #error "LEVEL_CORNERS_INSET is now LEVEL_CORNERS_INSET_LFRB . Please update your Configuration.h."
#elif ENABLED(LEVEL_BED_CORNERS) && !defined(LEVEL_CORNERS_INSET_LFRB)
  #error "LEVEL_BED_CORNERS requires LEVEL_CORNERS_INSET_LFRB values. Please update your Configuration.h."
#elif defined(BEZIER_JERK_CONTROL)
  #error "BEZIER_JERK_CONTROL is now S_CURVE_ACCELERATION. Please update your configuration."
#elif HAS_JUNCTION_DEVIATION && defined(JUNCTION_DEVIATION_FACTOR)
  #error "JUNCTION_DEVIATION_FACTOR is now JUNCTION_DEVIATION_MM. Please update your configuration."
#elif defined(JUNCTION_ACCELERATION_FACTOR)
  #error "JUNCTION_ACCELERATION_FACTOR is obsolete. Delete it from Configuration_adv.h."
#elif defined(JUNCTION_ACCELERATION)
  #error "JUNCTION_ACCELERATION is obsolete. Delete it from Configuration_adv.h."
#elif defined(MAX7219_DEBUG_STEPPER_HEAD)
  #error "MAX7219_DEBUG_STEPPER_HEAD is now MAX7219_DEBUG_PLANNER_HEAD. Please update your configuration."
#elif defined(MAX7219_DEBUG_STEPPER_TAIL)
  #error "MAX7219_DEBUG_STEPPER_TAIL is now MAX7219_DEBUG_PLANNER_TAIL. Please update your configuration."
#elif defined(MAX7219_DEBUG_STEPPER_QUEUE)
  #error "MAX7219_DEBUG_STEPPER_QUEUE is now MAX7219_DEBUG_PLANNER_QUEUE. Please update your configuration."
#elif defined(ENDSTOP_NOISE_FILTER)
  #error "ENDSTOP_NOISE_FILTER is now ENDSTOP_NOISE_THRESHOLD [2-7]. Please update your configuration."
#elif defined(RETRACT_ZLIFT)
  #error "RETRACT_ZLIFT is now RETRACT_ZRAISE. Please update your Configuration_adv.h."
#elif defined(TOOLCHANGE_PARK_ZLIFT) || defined(TOOLCHANGE_UNPARK_ZLIFT)
  #error "TOOLCHANGE_PARK_ZLIFT and TOOLCHANGE_UNPARK_ZLIFT are now TOOLCHANGE_ZRAISE. Please update your configuration."
#elif defined(SINGLENOZZLE_TOOLCHANGE_ZRAISE)
  #error "SINGLENOZZLE_TOOLCHANGE_ZRAISE is now TOOLCHANGE_ZRAISE. Please update your configuration."
#elif defined(SINGLENOZZLE_SWAP_LENGTH)
  #error "SINGLENOZZLE_SWAP_LENGTH is now TOOLCHANGE_FIL_SWAP_LENGTH. Please update your configuration."
#elif defined(SINGLENOZZLE_SWAP_RETRACT_SPEED)
  #error "SINGLENOZZLE_SWAP_RETRACT_SPEED is now TOOLCHANGE_FIL_SWAP_RETRACT_SPEED. Please update your configuration."
#elif defined(SINGLENOZZLE_SWAP_PRIME_SPEED)
  #error "SINGLENOZZLE_SWAP_PRIME_SPEED is now TOOLCHANGE_FIL_SWAP_PRIME_SPEED. Please update your configuration."
#elif defined(SINGLENOZZLE_SWAP_PARK)
  #error "SINGLENOZZLE_SWAP_PARK is now TOOLCHANGE_PARK. Please update your configuration."
#elif defined(SINGLENOZZLE_TOOLCHANGE_XY)
  #error "SINGLENOZZLE_TOOLCHANGE_XY is now TOOLCHANGE_PARK_XY. Please update your configuration."
#elif defined(SINGLENOZZLE_PARK_XY_FEEDRATE)
  #error "SINGLENOZZLE_PARK_XY_FEEDRATE is now TOOLCHANGE_PARK_XY_FEEDRATE. Please update your configuration."
#elif defined(PARKING_EXTRUDER_SECURITY_RAISE)
  #error "PARKING_EXTRUDER_SECURITY_RAISE is now TOOLCHANGE_ZRAISE. Please update your configuration."
#elif defined(SWITCHING_TOOLHEAD_SECURITY_RAISE)
  #error "SWITCHING_TOOLHEAD_SECURITY_RAISE is now TOOLCHANGE_ZRAISE. Please update your configuration."
#elif defined(G0_FEEDRATE) && G0_FEEDRATE == 0
  #error "G0_FEEDRATE is now used to set the G0 feedrate. Please update your configuration."
#elif defined(MBL_Z_STEP)
  #error "MBL_Z_STEP is now MESH_EDIT_Z_STEP. Please update your configuration."
#elif defined(CHDK)
  #error "CHDK is now CHDK_PIN. Please update your Configuration_adv.h."
#elif defined(MAX6675_SS)
  #error "MAX6675_SS is now MAX6675_SS_PIN. Please update your configuration and/or pins."
#elif defined(MAX6675_SS2)
  #error "MAX6675_SS2 is now MAX6675_SS2_PIN. Please update your configuration and/or pins."
#elif defined(SPINDLE_LASER_ENABLE)
  #error "SPINDLE_LASER_ENABLE is now SPINDLE_FEATURE or LASER_FEATURE. Please update your Configuration_adv.h."
#elif defined(SPINDLE_LASER_ENABLE_PIN)
  #error "SPINDLE_LASER_ENABLE_PIN is now SPINDLE_LASER_ENA_PIN. Please update your Configuration_adv.h and/or pins."
#elif defined(SPINDLE_DIR_CHANGE)
  #error "SPINDLE_DIR_CHANGE is now SPINDLE_CHANGE_DIR. Please update your Configuration_adv.h."
#elif defined(SPINDLE_STOP_ON_DIR_CHANGE)
  #error "SPINDLE_STOP_ON_DIR_CHANGE is now SPINDLE_CHANGE_DIR_STOP. Please update your Configuration_adv.h."
#elif defined(SPINDLE_LASER_ENABLE_INVERT)
  #error "SPINDLE_LASER_ENABLE_INVERT is now SPINDLE_LASER_ACTIVE_HIGH. Please update your Configuration_adv.h."
#elif defined(CUTTER_POWER_DISPLAY)
  #error "CUTTER_POWER_DISPLAY is now CUTTER_POWER_UNIT. Please update your Configuration_adv.h."
#elif defined(CHAMBER_HEATER_PIN)
  #error "CHAMBER_HEATER_PIN is now HEATER_CHAMBER_PIN. Please update your configuration and/or pins."
#elif defined(TMC_Z_CALIBRATION)
  #error "TMC_Z_CALIBRATION has been deprecated in favor of Z_STEPPER_AUTO_ALIGN. Please update your configuration."
#elif defined(Z_MIN_PROBE_ENDSTOP)
  #error "Z_MIN_PROBE_ENDSTOP is no longer required. Please remove it from Configuration.h."
#elif defined(DUAL_NOZZLE_DUPLICATION_MODE)
  #error "DUAL_NOZZLE_DUPLICATION_MODE is now MULTI_NOZZLE_DUPLICATION. Please update your configuration."
#elif defined(MENU_ITEM_CASE_LIGHT)
  #error "MENU_ITEM_CASE_LIGHT is now CASE_LIGHT_MENU. Please update your configuration."
#elif defined(ABORT_ON_ENDSTOP_HIT_FEATURE_ENABLED)
  #error "ABORT_ON_ENDSTOP_HIT_FEATURE_ENABLED is now SD_ABORT_ON_ENDSTOP_HIT. Please update your Configuration_adv.h."
#elif defined(LPC_SD_LCD) || defined(LPC_SD_ONBOARD) || defined(LPC_SD_CUSTOM_CABLE)
  #error "LPC_SD_(LCD|ONBOARD|CUSTOM_CABLE) are now SDCARD_CONNECTION. Please update your Configuration_adv.h."
#elif defined(USB_SD_DISABLED)
  #error "USB_SD_DISABLED is now NO_SD_HOST_DRIVE. Please update your Configuration_adv.h."
#elif defined(USB_SD_ONBOARD)
  #error "USB_SD_ONBOARD is obsolete. Disable NO_SD_HOST_DRIVE instead."
#elif POWER_SUPPLY == 1
  #error "Replace POWER_SUPPLY 1 by enabling PSU_CONTROL and setting PSU_ACTIVE_HIGH to 'false'."
#elif POWER_SUPPLY == 2
  #error "Replace POWER_SUPPLY 2 by enabling PSU_CONTROL and setting PSU_ACTIVE_HIGH to 'true'."
#elif defined(POWER_SUPPLY)
  #error "POWER_SUPPLY is now obsolete. Please remove it from Configuration.h."
#elif defined(MKS_ROBIN_TFT)
  #error "MKS_ROBIN_TFT is now FSMC_GRAPHICAL_TFT. Please update your configuration."
#elif defined(TFT_LVGL_UI)
  #error "TFT_LVGL_UI is now TFT_LVGL_UI_FSMC. Please update your configuration."
#elif defined(SDPOWER)
  #error "SDPOWER is now SDPOWER_PIN. Please update your configuration and/or pins."
#elif defined(STRING_SPLASH_LINE1) || defined(STRING_SPLASH_LINE2)
  #error "STRING_SPLASH_LINE[12] are now obsolete. Please remove them from Configuration.h."
#elif defined(Z_PROBE_ALLEN_KEY_DEPLOY_1_X) || defined(Z_PROBE_ALLEN_KEY_STOW_1_X)
  #error "Z_PROBE_ALLEN_KEY_(DEPLOY|STOW) coordinates are now a single setting. Please update your configuration."
#elif defined(X_PROBE_OFFSET_FROM_EXTRUDER) || defined(Y_PROBE_OFFSET_FROM_EXTRUDER) || defined(Z_PROBE_OFFSET_FROM_EXTRUDER)
  #error "[XYZ]_PROBE_OFFSET_FROM_EXTRUDER is now NOZZLE_TO_PROBE_OFFSET. Please update your configuration."
#elif defined(MIN_PROBE_X) || defined(MIN_PROBE_Y) || defined(MAX_PROBE_X) || defined(MAX_PROBE_Y)
  #error "(MIN|MAX)_PROBE_[XY] are now calculated at runtime. Please remove them from Configuration.h."
#elif defined(Z_STEPPER_ALIGN_X) || defined(Z_STEPPER_ALIGN_X)
  #error "Z_STEPPER_ALIGN_X and Z_STEPPER_ALIGN_Y are now combined as Z_STEPPER_ALIGN_XY. Please update your Configuration_adv.h."
#elif defined(JUNCTION_DEVIATION)
  #error "JUNCTION_DEVIATION is no longer required. (See CLASSIC_JERK). Please remove it from Configuration.h."
#elif defined(BABYSTEP_MULTIPLICATOR)
  #error "BABYSTEP_MULTIPLICATOR is now BABYSTEP_MULTIPLICATOR_[XY|Z]. Please update Configuration_adv.h."
#elif defined(LULZBOT_TOUCH_UI)
  #error "LULZBOT_TOUCH_UI is now TOUCH_UI_FTDI_EVE. Please update your configuration."
#elif defined(PS_DEFAULT_OFF)
  #error "PS_DEFAULT_OFF is now PSU_DEFAULT_OFF. Please update your configuration."
#elif defined(FILAMENT_UNLOAD_RETRACT_LENGTH)
  #error "FILAMENT_UNLOAD_RETRACT_LENGTH is now FILAMENT_UNLOAD_PURGE_RETRACT. Please update Configuration_adv.h."
#elif defined(FILAMENT_UNLOAD_DELAY)
  #error "FILAMENT_UNLOAD_DELAY is now FILAMENT_UNLOAD_PURGE_DELAY. Please update Configuration_adv.h."
#elif defined(HOME_USING_SPREADCYCLE)
  #error "HOME_USING_SPREADCYCLE is now obsolete. Please remove it from Configuration_adv.h."
#elif defined(DGUS_LCD)
  #error "DGUS_LCD is now DGUS_LCD_UI_(ORIGIN|FYSETC|HIPRECY). Please update your configuration."
#elif defined(X_DUAL_ENDSTOPS_ADJUSTMENT)
  #error "X_DUAL_ENDSTOPS_ADJUSTMENT is now X2_ENDSTOP_ADJUSTMENT. Please update Configuration_adv.h."
#elif defined(Y_DUAL_ENDSTOPS_ADJUSTMENT)
  #error "Y_DUAL_ENDSTOPS_ADJUSTMENT is now Y2_ENDSTOP_ADJUSTMENT. Please update Configuration_adv.h."
#elif defined(Z_DUAL_ENDSTOPS_ADJUSTMENT)
  #error "Z_DUAL_ENDSTOPS_ADJUSTMENT is now Z2_ENDSTOP_ADJUSTMENT. Please update Configuration_adv.h."
#elif defined(Z_TRIPLE_ENDSTOPS_ADJUSTMENT2) || defined(Z_TRIPLE_ENDSTOPS_ADJUSTMENT3)
  #error "Z_TRIPLE_ENDSTOPS_ADJUSTMENT[23] is now Z[23]_ENDSTOP_ADJUSTMENT. Please update Configuration_adv.h."
#elif defined(Z_QUAD_ENDSTOPS_ADJUSTMENT2) || defined(Z_QUAD_ENDSTOPS_ADJUSTMENT3) || defined(Z_QUAD_ENDSTOPS_ADJUSTMENT4)
  #error "Z_QUAD_ENDSTOPS_ADJUSTMENT[234] is now Z[234]_ENDSTOP_ADJUSTMENT. Please update Configuration_adv.h."
#elif defined(Z_DUAL_STEPPER_DRIVERS)
  #error "Z_DUAL_STEPPER_DRIVERS is now NUM_Z_STEPPER_DRIVERS with a value of 2. Please update Configuration_adv.h."
#elif defined(Z_TRIPLE_STEPPER_DRIVERS)
  #error "Z_TRIPLE_STEPPER_DRIVERS is now NUM_Z_STEPPER_DRIVERS with a value of 3. Please update Configuration_adv.h."
#elif defined(Z_QUAD_STEPPER_DRIVERS)
  #error "Z_QUAD_STEPPER_DRIVERS is now NUM_Z_STEPPER_DRIVERS with a value of 4. Please update Configuration_adv.h."
#elif defined(Z_DUAL_ENDSTOPS)
  #error "Z_DUAL_ENDSTOPS is now Z_MULTI_ENDSTOPS. Please update Configuration_adv.h."
#elif defined(Z_TRIPLE_ENDSTOPS)
  #error "Z_TRIPLE_ENDSTOPS is now Z_MULTI_ENDSTOPS. Please update Configuration_adv.h."
#elif defined(Z_QUAD_ENDSTOPS)
  #error "Z_QUAD_ENDSTOPS is now Z_MULTI_ENDSTOPS. Please update Configuration_adv.h."
#elif defined(DUGS_UI_MOVE_DIS_OPTION)
  #error "DUGS_UI_MOVE_DIS_OPTION is spelled DGUS_UI_MOVE_DIS_OPTION. Please update Configuration_adv.h."
#elif defined(ORIG_E0_AUTO_FAN_PIN) || defined(ORIG_E1_AUTO_FAN_PIN) || defined(ORIG_E2_AUTO_FAN_PIN) || defined(ORIG_E3_AUTO_FAN_PIN) || defined(ORIG_E4_AUTO_FAN_PIN) || defined(ORIG_E5_AUTO_FAN_PIN) || defined(ORIG_E6_AUTO_FAN_PIN) || defined(ORIG_E7_AUTO_FAN_PIN)
  #error "ORIG_Ex_AUTO_FAN_PIN is now just Ex_AUTO_FAN_PIN. Make sure your pins are up to date."
#elif defined(ORIG_CHAMBER_AUTO_FAN_PIN)
  #error "ORIG_CHAMBER_AUTO_FAN_PIN is now just CHAMBER_AUTO_FAN_PIN. Make sure your pins are up to date."
#elif defined(HOMING_BACKOFF_MM)
  #error "HOMING_BACKOFF_MM is now HOMING_BACKOFF_POST_MM. Please update Configuration_adv.h."
#elif defined(X_HOME_BUMP_MM) || defined(Y_HOME_BUMP_MM) || defined(Z_HOME_BUMP_MM)
  #error "[XYZ]_HOME_BUMP_MM is now HOMING_BUMP_MM. Please update Configuration_adv.h."
#elif defined(DIGIPOT_I2C)
  #error "DIGIPOT_I2C is now DIGIPOT_MCP4451 (or DIGIPOT_MCP4018). Please update Configuration_adv.h."
#elif defined(TOUCH_BUTTONS)
  #error "TOUCH_BUTTONS is now TOUCH_SCREEN. Please update your Configuration.h."
#endif

#ifdef FIL_RUNOUT_INVERTING
  #if FIL_RUNOUT_INVERTING
    #warning "FIL_RUNOUT_INVERTING true is now FIL_RUNOUT_STATE HIGH. Please update Configuration.h."
  #else
    #warning "FIL_RUNOUT_INVERTING false is now FIL_RUNOUT_STATE LOW. Please update Configuration.h."
  #endif
  #ifndef FIL_RUNOUT_STATE
    #define FIL_RUNOUT_STATE ((FIL_RUNOUT_INVERTING) ? HIGH : LOW)
  #endif
#endif

/**
 * Probe temp compensation requirements
 */
#if ENABLED(PROBE_TEMP_COMPENSATION)
  #if defined(PTC_PARK_POS_X) || defined(PTC_PARK_POS_Y) || defined(PTC_PARK_POS_Z)
    #error "PTC_PARK_POS_[XYZ] is now PTC_PARK_POS (array). Please update Configuration_adv.h."
  #elif !defined(PTC_PARK_POS)
    #error "PROBE_TEMP_COMPENSATION requires PTC_PARK_POS."
  #elif defined(PTC_PROBE_POS_X) || defined(PTC_PROBE_POS_Y)
    #error "PTC_PROBE_POS_[XY] is now PTC_PROBE_POS (array). Please update Configuration_adv.h."
  #elif !defined(PTC_PROBE_POS)
    #error "PROBE_TEMP_COMPENSATION requires PTC_PROBE_POS."
  #endif
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
    #error "TX_BUFFER_SIZE must be 0 or a power of 2 between 1 and 256."
  #endif
#elif ANY(SERIAL_XON_XOFF, SERIAL_STATS_MAX_RX_QUEUED, SERIAL_STATS_DROPPED_RX)
  #error "SERIAL_XON_XOFF and SERIAL_STATS_* features not supported on USB-native AVR devices."
#endif

#if SERIAL_PORT > 7
  #error "Set SERIAL_PORT to the port on your board. Usually this is 0."
#endif

#if defined(SERIAL_PORT_2) && NUM_SERIAL < 2
  #error "SERIAL_PORT_2 is not supported for your MOTHERBOARD. Disable it to continue."
#endif

/**
 * Multiple Stepper Drivers Per Axis
 */
#define GOOD_AXIS_PINS(A) (HAS_##A##_ENABLE && HAS_##A##_STEP && HAS_##A##_DIR)
#if ENABLED(X_DUAL_STEPPER_DRIVERS)
  #if ENABLED(DUAL_X_CARRIAGE)
    #error "DUAL_X_CARRIAGE is not compatible with X_DUAL_STEPPER_DRIVERS."
  #elif !GOOD_AXIS_PINS(X)
    #error "X_DUAL_STEPPER_DRIVERS requires X2 pins to be defined."
  #endif
#endif

#if ENABLED(Y_DUAL_STEPPER_DRIVERS) && !GOOD_AXIS_PINS(Y)
  #error "Y_DUAL_STEPPER_DRIVERS requires Y2 pins to be defined."
#elif !WITHIN(NUM_Z_STEPPER_DRIVERS, 1, 4)
  #error "NUM_Z_STEPPER_DRIVERS must be an integer from 1 to 4."
#elif NUM_Z_STEPPER_DRIVERS == 2 && !GOOD_AXIS_PINS(Z2)
  #error "If NUM_Z_STEPPER_DRIVERS is 2, you must define stepper pins for Z2."
#elif NUM_Z_STEPPER_DRIVERS == 3 && !(GOOD_AXIS_PINS(Z2) && GOOD_AXIS_PINS(Z3))
  #error "If NUM_Z_STEPPER_DRIVERS is 3, you must define stepper pins for Z2 and Z3."
#elif NUM_Z_STEPPER_DRIVERS == 4 && !(GOOD_AXIS_PINS(Z2) && GOOD_AXIS_PINS(Z3) && GOOD_AXIS_PINS(Z4))
  #error "If NUM_Z_STEPPER_DRIVERS is 4, you must define stepper pins for Z2, Z3, and Z4."
#endif

/**
 * Validate that the bed size fits
 */
static_assert(X_MAX_LENGTH >= X_BED_SIZE, "Movement bounds (X_MIN_POS, X_MAX_POS) are too narrow to contain X_BED_SIZE.");
static_assert(Y_MAX_LENGTH >= Y_BED_SIZE, "Movement bounds (Y_MIN_POS, Y_MAX_POS) are too narrow to contain Y_BED_SIZE.");

/**
 * Granular software endstops (Marlin >= 1.1.7)
 */
#if ENABLED(MIN_SOFTWARE_ENDSTOPS) && DISABLED(MIN_SOFTWARE_ENDSTOP_Z)
  #if IS_KINEMATIC
    #error "MIN_SOFTWARE_ENDSTOPS on DELTA/SCARA also requires MIN_SOFTWARE_ENDSTOP_Z."
  #elif NONE(MIN_SOFTWARE_ENDSTOP_X, MIN_SOFTWARE_ENDSTOP_Y)
    #error "MIN_SOFTWARE_ENDSTOPS requires at least one of the MIN_SOFTWARE_ENDSTOP_[XYZ] options."
  #endif
#endif

#if ENABLED(MAX_SOFTWARE_ENDSTOPS) && DISABLED(MAX_SOFTWARE_ENDSTOP_Z)
  #if IS_KINEMATIC
    #error "MAX_SOFTWARE_ENDSTOPS on DELTA/SCARA also requires MAX_SOFTWARE_ENDSTOP_Z."
  #elif NONE(MAX_SOFTWARE_ENDSTOP_X, MAX_SOFTWARE_ENDSTOP_Y)
    #error "MAX_SOFTWARE_ENDSTOPS requires at least one of the MAX_SOFTWARE_ENDSTOP_[XYZ] options."
  #endif
#endif

#if !defined(TARGET_LPC1768) && ANY( \
    ENDSTOPPULLDOWNS, \
    ENDSTOPPULLDOWN_XMAX, ENDSTOPPULLDOWN_YMAX, \
    ENDSTOPPULLDOWN_ZMAX, ENDSTOPPULLDOWN_XMIN, \
    ENDSTOPPULLDOWN_YMIN, ENDSTOPPULLDOWN_ZMIN \
  )
  #error "PULLDOWN pin mode is not available on the selected board."
#endif

#if BOTH(ENDSTOPPULLUPS, ENDSTOPPULLDOWNS)
  #error "Enable only one of ENDSTOPPULLUPS or ENDSTOPPULLDOWNS."
#elif BOTH(FIL_RUNOUT_PULLUP, FIL_RUNOUT_PULLDOWN)
  #error "Enable only one of FIL_RUNOUT_PULLUP or FIL_RUNOUT_PULLDOWN."
#elif BOTH(ENDSTOPPULLUP_XMAX, ENDSTOPPULLDOWN_XMAX)
  #error "Enable only one of ENDSTOPPULLUP_X_MAX or ENDSTOPPULLDOWN_X_MAX."
#elif BOTH(ENDSTOPPULLUP_YMAX, ENDSTOPPULLDOWN_YMAX)
  #error "Enable only one of ENDSTOPPULLUP_Y_MAX or ENDSTOPPULLDOWN_Y_MAX."
#elif BOTH(ENDSTOPPULLUP_ZMAX, ENDSTOPPULLDOWN_ZMAX)
  #error "Enable only one of ENDSTOPPULLUP_Z_MAX or ENDSTOPPULLDOWN_Z_MAX."
#elif BOTH(ENDSTOPPULLUP_XMIN, ENDSTOPPULLDOWN_XMIN)
  #error "Enable only one of ENDSTOPPULLUP_X_MIN or ENDSTOPPULLDOWN_X_MIN."
#elif BOTH(ENDSTOPPULLUP_YMIN, ENDSTOPPULLDOWN_YMIN)
  #error "Enable only one of ENDSTOPPULLUP_Y_MIN or ENDSTOPPULLDOWN_Y_MIN."
#elif BOTH(ENDSTOPPULLUP_ZMIN, ENDSTOPPULLDOWN_ZMIN)
  #error "Enable only one of ENDSTOPPULLUP_Z_MIN or ENDSTOPPULLDOWN_Z_MIN."
#endif

/**
 * LCD Info Screen Style
 */
#if LCD_INFO_SCREEN_STYLE > 0
  #if HAS_GRAPHICAL_LCD || LCD_WIDTH < 20 || LCD_HEIGHT < 4
    #error "Alternative LCD_INFO_SCREEN_STYLE requires 20x4 Character LCD."
  #elif LCD_INFO_SCREEN_STYLE > 1
    #error "LCD_INFO_SCREEN_STYLE only has options 0 and 1 at this time."
  #endif
#endif

/**
 * Progress Bar
 */
#if ENABLED(LCD_PROGRESS_BAR)
  #if NONE(SDSUPPORT, LCD_SET_PROGRESS_MANUALLY)
    #error "LCD_PROGRESS_BAR requires SDSUPPORT or LCD_SET_PROGRESS_MANUALLY."
  #elif !HAS_CHARACTER_LCD
    #error "LCD_PROGRESS_BAR requires a character LCD."
  #elif HAS_GRAPHICAL_LCD
    #error "LCD_PROGRESS_BAR does not apply to graphical displays."
  #elif ENABLED(FILAMENT_LCD_DISPLAY)
    #error "LCD_PROGRESS_BAR and FILAMENT_LCD_DISPLAY are not fully compatible. Comment out this line to use both."
  #elif PROGRESS_MSG_EXPIRE < 0
    #error "PROGRESS_MSG_EXPIRE must be greater than or equal to 0."
  #endif
#elif ENABLED(LCD_SET_PROGRESS_MANUALLY) && NONE(HAS_GRAPHICAL_LCD, HAS_GRAPHICAL_TFT, EXTENSIBLE_UI)
  #error "LCD_SET_PROGRESS_MANUALLY requires LCD_PROGRESS_BAR, Graphical LCD, TFT, or EXTENSIBLE_UI."
#endif

#if !HAS_LCD_MENU && ENABLED(SD_REPRINT_LAST_SELECTED_FILE)
  #error "SD_REPRINT_LAST_SELECTED_FILE currently requires a Marlin-native LCD menu."
#endif

/**
 * Custom Boot and Status screens
 */
#if ENABLED(SHOW_CUSTOM_BOOTSCREEN) && NONE(HAS_GRAPHICAL_LCD, TOUCH_UI_FTDI_EVE)
  #error "SHOW_CUSTOM_BOOTSCREEN requires Graphical LCD or TOUCH_UI_FTDI_EVE."
#elif ENABLED(CUSTOM_STATUS_SCREEN_IMAGE) && !HAS_GRAPHICAL_LCD
  #error "CUSTOM_STATUS_SCREEN_IMAGE requires a Graphical LCD."
#endif

/**
 * LCD Lightweight Screen Style
 */
#if ENABLED(LIGHTWEIGHT_UI) && DISABLED(U8GLIB_ST7920)
  #error "LIGHTWEIGHT_UI requires a U8GLIB_ST7920-based display."
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

#if defined(EVENT_GCODE_SD_STOP) && DISABLED(NOZZLE_PARK_FEATURE)
  static_assert(nullptr == strstr(EVENT_GCODE_SD_STOP, "G27"), "NOZZLE_PARK_FEATURE is required to use G27 in EVENT_GCODE_SD_STOP.");
#endif

/**
 * I2C Position Encoders
 */
#if ENABLED(I2C_POSITION_ENCODERS)
  #if !BOTH(BABYSTEPPING, BABYSTEP_XY)
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
  #elif BOTH(DELTA, BABYSTEP_XY)
    #error "BABYSTEPPING only implemented for Z axis on deltabots."
  #elif BOTH(BABYSTEP_ZPROBE_OFFSET, MESH_BED_LEVELING)
    #error "MESH_BED_LEVELING and BABYSTEP_ZPROBE_OFFSET is not a valid combination"
  #elif ENABLED(BABYSTEP_ZPROBE_OFFSET) && !HAS_BED_PROBE
    #error "BABYSTEP_ZPROBE_OFFSET requires a probe."
  #elif ENABLED(BABYSTEP_ZPROBE_GFX_OVERLAY) && !HAS_GRAPHICAL_LCD
    #error "BABYSTEP_ZPROBE_GFX_OVERLAY requires a Graphical LCD."
  #elif ENABLED(BABYSTEP_ZPROBE_GFX_OVERLAY) && DISABLED(BABYSTEP_ZPROBE_OFFSET)
    #error "BABYSTEP_ZPROBE_GFX_OVERLAY requires a BABYSTEP_ZPROBE_OFFSET."
  #elif ENABLED(BABYSTEP_HOTEND_Z_OFFSET) && !HAS_HOTEND_OFFSET
    #error "BABYSTEP_HOTEND_Z_OFFSET requires 2 or more HOTENDS."
  #elif BOTH(BABYSTEP_ALWAYS_AVAILABLE, MOVE_Z_WHEN_IDLE)
    #error "BABYSTEP_ALWAYS_AVAILABLE and MOVE_Z_WHEN_IDLE are incompatible."
  #elif !defined(BABYSTEP_MULTIPLICATOR_Z)
    #error "BABYSTEPPING requires BABYSTEP_MULTIPLICATOR_Z."
  #elif ENABLED(BABYSTEP_XY) && !defined(BABYSTEP_MULTIPLICATOR_XY)
    #error "BABYSTEP_XY requires BABYSTEP_MULTIPLICATOR_XY."
  #elif ENABLED(BABYSTEP_MILLIMETER_UNITS)
    static_assert(BABYSTEP_MULTIPLICATOR_Z <= 0.1f, "BABYSTEP_MULTIPLICATOR_Z must be less or equal to 0.1mm.");
    #if ENABLED(BABYSTEP_XY)
      static_assert(BABYSTEP_MULTIPLICATOR_XY <= 0.25f, "BABYSTEP_MULTIPLICATOR_XY must be less than or equal to 0.25mm.");
    #endif
  #endif
#endif

/**
 * Filament Runout needs one or more pins and either SD Support or Auto print start detection
 */
#if HAS_FILAMENT_SENSOR
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
  #elif NUM_RUNOUT_SENSORS > 5 && !PIN_EXISTS(FIL_RUNOUT6)
    #error "FILAMENT_RUNOUT_SENSOR with NUM_RUNOUT_SENSORS > 5 requires FIL_RUNOUT6_PIN."
  #elif NONE(SDSUPPORT, PRINTJOB_TIMER_AUTOSTART)
    #error "FILAMENT_RUNOUT_SENSOR requires SDSUPPORT or PRINTJOB_TIMER_AUTOSTART."
  #elif FILAMENT_RUNOUT_DISTANCE_MM < 0
    #error "FILAMENT_RUNOUT_DISTANCE_MM must be greater than or equal to zero."
  #elif DISABLED(ADVANCED_PAUSE_FEATURE)
    static_assert(nullptr == strstr(FILAMENT_RUNOUT_SCRIPT, "M600"), "ADVANCED_PAUSE_FEATURE is required to use M600 with FILAMENT_RUNOUT_SENSOR.");
  #endif
#endif

/**
 * Advanced Pause
 */
#if ENABLED(ADVANCED_PAUSE_FEATURE)
  #if !HAS_RESUME_CONTINUE
    #error "ADVANCED_PAUSE_FEATURE currently requires an LCD controller or EMERGENCY_PARSER."
  #elif DISABLED(NOZZLE_PARK_FEATURE)
    #error "ADVANCED_PAUSE_FEATURE requires NOZZLE_PARK_FEATURE."
  #elif !defined(FILAMENT_UNLOAD_PURGE_FEEDRATE)
    #error "ADVANCED_PAUSE_FEATURE requires FILAMENT_UNLOAD_PURGE_FEEDRATE. Please add it to Configuration_adv.h."
  #elif ENABLED(EXTRUDER_RUNOUT_PREVENT)
    #error "EXTRUDER_RUNOUT_PREVENT is incompatible with ADVANCED_PAUSE_FEATURE."
  #elif ENABLED(PARK_HEAD_ON_PAUSE) && NONE(SDSUPPORT, NEWPANEL, EMERGENCY_PARSER)
    #error "PARK_HEAD_ON_PAUSE requires SDSUPPORT, EMERGENCY_PARSER, or an LCD controller."
  #elif ENABLED(HOME_BEFORE_FILAMENT_CHANGE) && DISABLED(PAUSE_PARK_NO_STEPPER_TIMEOUT)
    #error "HOME_BEFORE_FILAMENT_CHANGE requires PAUSE_PARK_NO_STEPPER_TIMEOUT."
  #elif ENABLED(PREVENT_LENGTHY_EXTRUDE) && FILAMENT_CHANGE_UNLOAD_LENGTH > EXTRUDE_MAXLENGTH
    #error "FILAMENT_CHANGE_UNLOAD_LENGTH must be less than or equal to EXTRUDE_MAXLENGTH."
  #elif ENABLED(PREVENT_LENGTHY_EXTRUDE) && FILAMENT_CHANGE_SLOW_LOAD_LENGTH > EXTRUDE_MAXLENGTH
    #error "FILAMENT_CHANGE_SLOW_LOAD_LENGTH must be less than or equal to EXTRUDE_MAXLENGTH."
  #elif ENABLED(PREVENT_LENGTHY_EXTRUDE) && FILAMENT_CHANGE_FAST_LOAD_LENGTH > EXTRUDE_MAXLENGTH
    #error "FILAMENT_CHANGE_FAST_LOAD_LENGTH must be less than or equal to EXTRUDE_MAXLENGTH."
  #endif
#endif

#if ENABLED(NOZZLE_PARK_FEATURE)
  constexpr float npp[] = NOZZLE_PARK_POINT;
  static_assert(COUNT(npp) == XYZ, "NOZZLE_PARK_POINT requires X, Y, and Z values.");
  constexpr xyz_pos_t npp_xyz = NOZZLE_PARK_POINT;
  static_assert(WITHIN(npp_xyz.x, X_MIN_POS, X_MAX_POS), "NOZZLE_PARK_POINT.X is out of bounds (X_MIN_POS, X_MAX_POS).");
  static_assert(WITHIN(npp_xyz.y, Y_MIN_POS, Y_MAX_POS), "NOZZLE_PARK_POINT.Y is out of bounds (Y_MIN_POS, Y_MAX_POS).");
  static_assert(WITHIN(npp_xyz.z, Z_MIN_POS, Z_MAX_POS), "NOZZLE_PARK_POINT.Z is out of bounds (Z_MIN_POS, Z_MAX_POS).");
#endif

/**
 * Individual axis homing is useless for DELTAS
 */
#if BOTH(INDIVIDUAL_AXIS_HOMING_MENU, DELTA)
  #error "INDIVIDUAL_AXIS_HOMING_MENU is incompatible with DELTA kinematics."
#endif

/**
 * Options only for EXTRUDERS > 1
 */
#if EXTRUDERS > 1

  #if EXTRUDERS > 8
    #error "Marlin supports a maximum of 8 EXTRUDERS."
  #endif

  #if ENABLED(HEATERS_PARALLEL)
    #error "EXTRUDERS must be 1 with HEATERS_PARALLEL."
  #endif

  #if ENABLED(TOOLCHANGE_FILAMENT_SWAP)
    #ifndef TOOLCHANGE_FS_LENGTH
      #error "TOOLCHANGE_FILAMENT_SWAP requires TOOLCHANGE_FS_LENGTH. Please update your Configuration_adv.h."
    #elif !defined(TOOLCHANGE_FS_RETRACT_SPEED)
      #error "TOOLCHANGE_FILAMENT_SWAP requires TOOLCHANGE_FS_RETRACT_SPEED. Please update your Configuration_adv.h."
    #elif !defined(TOOLCHANGE_FS_PRIME_SPEED)
      #error "TOOLCHANGE_FILAMENT_SWAP requires TOOLCHANGE_FS_PRIME_SPEED. Please update your Configuration_adv.h."
    #endif
  #endif

  #if ENABLED(TOOLCHANGE_PARK)
    #ifndef TOOLCHANGE_PARK_XY
      #error "TOOLCHANGE_PARK requires TOOLCHANGE_PARK_XY. Please update your Configuration."
    #elif !defined(TOOLCHANGE_PARK_XY_FEEDRATE)
      #error "TOOLCHANGE_PARK requires TOOLCHANGE_PARK_XY_FEEDRATE. Please update your Configuration."
    #endif
  #endif

  #ifndef TOOLCHANGE_ZRAISE
    #error "TOOLCHANGE_ZRAISE required for EXTRUDERS > 1. Please update your Configuration_adv.h."
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

  #ifndef SWITCHING_NOZZLE_SERVO_NR
    #error "SWITCHING_NOZZLE requires SWITCHING_NOZZLE_SERVO_NR."
  #elif SWITCHING_NOZZLE_SERVO_NR == 0 && !PIN_EXISTS(SERVO0)
    #error "SERVO0_PIN must be defined for your SWITCHING_NOZZLE."
  #elif SWITCHING_NOZZLE_SERVO_NR == 1 && !PIN_EXISTS(SERVO1)
    #error "SERVO1_PIN must be defined for your SWITCHING_NOZZLE."
  #elif SWITCHING_NOZZLE_SERVO_NR == 2 && !PIN_EXISTS(SERVO2)
    #error "SERVO2_PIN must be defined for your SWITCHING_NOZZLE."
  #elif SWITCHING_NOZZLE_SERVO_NR == 3 && !PIN_EXISTS(SERVO3)
    #error "SERVO3_PIN must be defined for your SWITCHING_NOZZLE."
  #endif

  #ifdef SWITCHING_NOZZLE_E1_SERVO_NR
    #if SWITCHING_NOZZLE_E1_SERVO_NR == SWITCHING_NOZZLE_SERVO_NR
      #error "SWITCHING_NOZZLE_E1_SERVO_NR must be different from SWITCHING_NOZZLE_SERVO_NR."
    #elif SWITCHING_NOZZLE_E1_SERVO_NR == 0 && !PIN_EXISTS(SERVO0)
      #error "SERVO0_PIN must be defined for your SWITCHING_NOZZLE."
    #elif SWITCHING_NOZZLE_E1_SERVO_NR == 1 && !PIN_EXISTS(SERVO1)
      #error "SERVO1_PIN must be defined for your SWITCHING_NOZZLE."
    #elif SWITCHING_NOZZLE_E1_SERVO_NR == 2 && !PIN_EXISTS(SERVO2)
      #error "SERVO2_PIN must be defined for your SWITCHING_NOZZLE."
    #elif SWITCHING_NOZZLE_E1_SERVO_NR == 3 && !PIN_EXISTS(SERVO3)
      #error "SERVO3_PIN must be defined for your SWITCHING_NOZZLE."
    #endif
  #endif
#endif

/**
 * Single Stepper Dual Extruder with switching servo
 */
#if ENABLED(SWITCHING_EXTRUDER)
  #if NUM_SERVOS < 1
    #error "SWITCHING_EXTRUDER requires NUM_SERVOS >= 1."
  #elif SWITCHING_EXTRUDER_SERVO_NR == 0 && !PIN_EXISTS(SERVO0)
    #error "SERVO0_PIN must be defined for your SWITCHING_EXTRUDER."
  #elif SWITCHING_EXTRUDER_SERVO_NR == 1 && !PIN_EXISTS(SERVO1)
    #error "SERVO1_PIN must be defined for your SWITCHING_EXTRUDER."
  #elif SWITCHING_EXTRUDER_SERVO_NR == 2 && !PIN_EXISTS(SERVO2)
    #error "SERVO2_PIN must be defined for your SWITCHING_EXTRUDER."
  #elif SWITCHING_EXTRUDER_SERVO_NR == 3 && !PIN_EXISTS(SERVO3)
    #error "SERVO3_PIN must be defined for your SWITCHING_EXTRUDER."
  #endif
  #if EXTRUDERS > 3
    #if NUM_SERVOS < 2
      #error "SWITCHING_EXTRUDER with 4 extruders requires NUM_SERVOS >= 2."
    #elif SWITCHING_EXTRUDER_E23_SERVO_NR == 0 && !PIN_EXISTS(SERVO0)
      #error "SERVO0_PIN must be defined for your SWITCHING_EXTRUDER."
    #elif SWITCHING_EXTRUDER_E23_SERVO_NR == 1 && !PIN_EXISTS(SERVO1)
      #error "SERVO1_PIN must be defined for your SWITCHING_EXTRUDER."
    #elif SWITCHING_EXTRUDER_E23_SERVO_NR == 2 && !PIN_EXISTS(SERVO2)
      #error "SERVO2_PIN must be defined for your SWITCHING_EXTRUDER."
    #elif SWITCHING_EXTRUDER_E23_SERVO_NR == 3 && !PIN_EXISTS(SERVO3)
      #error "SERVO3_PIN must be defined for your SWITCHING_EXTRUDER."
    #elif SWITCHING_EXTRUDER_E23_SERVO_NR == SWITCHING_EXTRUDER_SERVO_NR
      #error "SWITCHING_EXTRUDER_E23_SERVO_NR should be a different extruder from SWITCHING_EXTRUDER_SERVO_NR."
    #endif
  #endif
#endif

/**
 * Mixing Extruder requirements
 */
#if ENABLED(MIXING_EXTRUDER)
  #if EXTRUDERS > 1
    #error "For MIXING_EXTRUDER set MIXING_STEPPERS > 1 instead of EXTRUDERS > 1."
  #elif MIXING_STEPPERS < 2
    #error "You must set MIXING_STEPPERS >= 2 for a mixing extruder."
  #elif ENABLED(FILAMENT_SENSOR)
    #error "MIXING_EXTRUDER is incompatible with FILAMENT_SENSOR. Comment out this line to use it anyway."
  #elif ENABLED(SWITCHING_EXTRUDER)
    #error "Please select either MIXING_EXTRUDER or SWITCHING_EXTRUDER, not both."
  #elif ENABLED(SINGLENOZZLE)
    #error "MIXING_EXTRUDER is incompatible with SINGLENOZZLE."
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
  #if ENABLED(S_CURVE_ACCELERATION) && DISABLED(EXPERIMENTAL_SCURVE)
    #error "LIN_ADVANCE and S_CURVE_ACCELERATION may not play well together! Enable EXPERIMENTAL_SCURVE to continue."
  #endif
#endif

/**
 * Special tool-changing options
 */
#if 1 < 0 \
  + ENABLED(SINGLENOZZLE) \
  + ENABLED(DUAL_X_CARRIAGE) \
  + ENABLED(PARKING_EXTRUDER) \
  + ENABLED(MAGNETIC_PARKING_EXTRUDER) \
  + ENABLED(SWITCHING_TOOLHEAD) \
  + ENABLED(MAGNETIC_SWITCHING_TOOLHEAD) \
  + ENABLED(ELECTROMAGNETIC_SWITCHING_TOOLHEAD)
  #error "Please select only one of SINGLENOZZLE, DUAL_X_CARRIAGE, PARKING_EXTRUDER, SWITCHING_TOOLHEAD, MAGNETIC_SWITCHING_TOOLHEAD, or ELECTROMAGNETIC_SWITCHING_TOOLHEAD."
#endif

/**
 * (Magnetic) Parking Extruder requirements
 */
#if ANY(PARKING_EXTRUDER, MAGNETIC_PARKING_EXTRUDER)
  #if ENABLED(EXT_SOLENOID)
    #error "(MAGNETIC_)PARKING_EXTRUDER and EXT_SOLENOID are incompatible. (Pins are used twice.)"
  #elif EXTRUDERS != 2
    #error "(MAGNETIC_)PARKING_EXTRUDER requires exactly 2 EXTRUDERS."
  #elif !defined(PARKING_EXTRUDER_PARKING_X)
    #error "(MAGNETIC_)PARKING_EXTRUDER requires PARKING_EXTRUDER_PARKING_X."
  #elif !defined(TOOLCHANGE_ZRAISE)
    #error "(MAGNETIC_)PARKING_EXTRUDER requires TOOLCHANGE_ZRAISE."
  #elif TOOLCHANGE_ZRAISE < 0
    #error "TOOLCHANGE_ZRAISE must be 0 or higher."
  #elif ENABLED(PARKING_EXTRUDER)
    #if !PINS_EXIST(SOL0, SOL1)
      #error "PARKING_EXTRUDER requires SOL0_PIN and SOL1_PIN."
    #elif !defined(PARKING_EXTRUDER_SOLENOIDS_PINS_ACTIVE) || !WITHIN(PARKING_EXTRUDER_SOLENOIDS_PINS_ACTIVE, LOW, HIGH)
      #error "PARKING_EXTRUDER_SOLENOIDS_PINS_ACTIVE must be defined as HIGH or LOW."
    #elif !defined(PARKING_EXTRUDER_SOLENOIDS_DELAY) || !WITHIN(PARKING_EXTRUDER_SOLENOIDS_DELAY, 0, 2000)
      #error "PARKING_EXTRUDER_SOLENOIDS_DELAY must be between 0 and 2000 (ms)."
    #endif
  #endif
#endif

/**
 * Switching Toolhead requirements
 */
#if ENABLED(SWITCHING_TOOLHEAD)
  #ifndef SWITCHING_TOOLHEAD_SERVO_NR
    #error "SWITCHING_TOOLHEAD requires SWITCHING_TOOLHEAD_SERVO_NR."
  #elif EXTRUDERS < 2
    #error "SWITCHING_TOOLHEAD requires at least 2 EXTRUDERS."
  #elif NUM_SERVOS < (SWITCHING_TOOLHEAD_SERVO_NR - 1)
    #if SWITCHING_TOOLHEAD_SERVO_NR == 0
      #error "A SWITCHING_TOOLHEAD_SERVO_NR of 0 requires NUM_SERVOS >= 1."
    #elif SWITCHING_TOOLHEAD_SERVO_NR == 1
      #error "A SWITCHING_TOOLHEAD_SERVO_NR of 1 requires NUM_SERVOS >= 2."
    #elif SWITCHING_TOOLHEAD_SERVO_NR == 2
      #error "A SWITCHING_TOOLHEAD_SERVO_NR of 2 requires NUM_SERVOS >= 3."
    #elif SWITCHING_TOOLHEAD_SERVO_NR == 3
      #error "A SWITCHING_TOOLHEAD_SERVO_NR of 3 requires NUM_SERVOS >= 4."
    #endif
  #elif !defined(TOOLCHANGE_ZRAISE)
    #error "SWITCHING_TOOLHEAD requires TOOLCHANGE_ZRAISE."
  #elif TOOLCHANGE_ZRAISE < 0
    #error "TOOLCHANGE_ZRAISE must be 0 or higher."
  #endif
#endif

/**
 * (Electro)magnetic Switching Toolhead requirements
 */
#if EITHER(MAGNETIC_SWITCHING_TOOLHEAD, ELECTROMAGNETIC_SWITCHING_TOOLHEAD)
  #ifndef SWITCHING_TOOLHEAD_Y_POS
    #error "(ELECTRO)MAGNETIC_SWITCHING_TOOLHEAD requires SWITCHING_TOOLHEAD_Y_POS"
  #elif !defined(SWITCHING_TOOLHEAD_X_POS)
    #error "(ELECTRO)MAGNETIC_SWITCHING_TOOLHEAD requires SWITCHING_TOOLHEAD_X_POS"
  #elif !defined(SWITCHING_TOOLHEAD_Z_HOP)
    #error "(ELECTRO)MAGNETIC_SWITCHING_TOOLHEAD requires SWITCHING_TOOLHEAD_Z_HOP."
  #elif !defined(SWITCHING_TOOLHEAD_Y_CLEAR)
    #error "(ELECTRO)MAGNETIC_SWITCHING_TOOLHEAD requires SWITCHING_TOOLHEAD_Y_CLEAR."
  #elif ENABLED(ELECTROMAGNETIC_SWITCHING_TOOLHEAD)
    #if ENABLED(EXT_SOLENOID)
      #error "(ELECTRO)MAGNETIC_SWITCHING_TOOLHEAD and EXT_SOLENOID are incompatible. (Pins are used twice.)"
    #elif !PIN_EXISTS(SOL0)
      #error "(ELECTRO)MAGNETIC_SWITCHING_TOOLHEAD requires SOL0_PIN."
    #endif
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
#if NUM_SERVOS > NUM_SERVO_PLUGS
  #error "The selected board doesn't support enough servos for your configuration. Reduce NUM_SERVOS."
#endif

/**
 * Servo deactivation depends on servo endstops, switching nozzle, or switching extruder
 */
#if ENABLED(DEACTIVATE_SERVOS_AFTER_MOVE) && !HAS_Z_SERVO_PROBE && !defined(SWITCHING_NOZZLE_SERVO_NR) && !defined(SWITCHING_EXTRUDER_SERVO_NR) && !defined(SWITCHING_TOOLHEAD_SERVO_NR)
  #error "Z_PROBE_SERVO_NR, switching nozzle, switching toolhead or switching extruder is required for DEACTIVATE_SERVOS_AFTER_MOVE."
#endif

/**
 * Required LCD language
 */
#if HAS_CHARACTER_LCD && !defined(DISPLAY_CHARSET_HD44780)
  #error "You must set DISPLAY_CHARSET_HD44780 to JAPANESE, WESTERN or CYRILLIC for your LCD controller."
#endif

/**
 * Bed Heating Options - PID vs Limit Switching
 */
#if BOTH(PIDTEMPBED, BED_LIMIT_SWITCHING)
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
  + ENABLED(COREXY) \
  + ENABLED(COREXZ) \
  + ENABLED(COREYZ) \
  + ENABLED(COREYX) \
  + ENABLED(COREZX) \
  + ENABLED(COREZY)
  #error "Please enable only one of DELTA, MORGAN_SCARA, COREXY, COREYX, COREXZ, COREZX, COREYZ, or COREZY."
#endif

/**
 * Delta requirements
 */
#if ENABLED(DELTA)
  #if NONE(USE_XMAX_PLUG, USE_YMAX_PLUG, USE_ZMAX_PLUG)
    #error "You probably want to use Max Endstops for DELTA!"
  #elif ENABLED(ENABLE_LEVELING_FADE_HEIGHT) && DISABLED(AUTO_BED_LEVELING_BILINEAR) && !UBL_SEGMENTED
    #error "ENABLE_LEVELING_FADE_HEIGHT on DELTA requires AUTO_BED_LEVELING_BILINEAR or AUTO_BED_LEVELING_UBL."
  #elif ENABLED(DELTA_AUTO_CALIBRATION) && !(HAS_BED_PROBE || HAS_LCD_MENU)
    #error "DELTA_AUTO_CALIBRATION requires a probe or LCD Controller."
  #elif ENABLED(DELTA_CALIBRATION_MENU) && !HAS_LCD_MENU
    #error "DELTA_CALIBRATION_MENU requires an LCD Controller."
  #elif ABL_GRID
    #if (GRID_MAX_POINTS_X & 1) == 0 || (GRID_MAX_POINTS_Y & 1) == 0
      #error "DELTA requires GRID_MAX_POINTS_X and GRID_MAX_POINTS_Y to be odd numbers."
    #elif GRID_MAX_POINTS_X < 3
      #error "DELTA requires GRID_MAX_POINTS_X and GRID_MAX_POINTS_Y to be 3 or higher."
    #endif
  #endif
#endif

/**
 * Junction deviation is incompatible with kinematic systems.
 */
#if HAS_JUNCTION_DEVIATION && IS_KINEMATIC
  #error "CLASSIC_JERK is required for DELTA and SCARA."
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
  + ENABLED(NOZZLE_AS_PROBE) \
  + (HAS_Z_SERVO_PROBE && DISABLED(BLTOUCH)) \
  + ENABLED(BLTOUCH) && DISABLED(CREALITY_TOUCH) \
  + ENABLED(CREALITY_TOUCH) \
  + ENABLED(TOUCH_MI_PROBE) \
  + ENABLED(SOLENOID_PROBE) \
  + ENABLED(Z_PROBE_ALLEN_KEY) \
  + ENABLED(Z_PROBE_SLED) \
  + ENABLED(RACK_AND_PINION_PROBE) \
  + ENABLED(SENSORLESS_PROBING)
  #error "Please enable only one probe option: PROBE_MANUALLY, SENSORLESS_PROBING, BLTOUCH, FIX_MOUNTED_PROBE, NOZZLE_AS_PROBE, TOUCH_MI_PROBE, SOLENOID_PROBE, Z_PROBE_ALLEN_KEY, Z_PROBE_SLED, or Z Servo."
#endif

#if HAS_BED_PROBE

  /**
   * Z_PROBE_SLED is incompatible with DELTA
   */
  #if BOTH(Z_PROBE_SLED, DELTA)
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
    #elif Z_PROBE_SERVO_NR == 0 && !PIN_EXISTS(SERVO0)
      #error "SERVO0_PIN must be defined for your servo or BLTOUCH probe."
    #elif Z_PROBE_SERVO_NR == 1 && !PIN_EXISTS(SERVO1)
      #error "SERVO1_PIN must be defined for your servo or BLTOUCH probe."
    #elif Z_PROBE_SERVO_NR == 2 && !PIN_EXISTS(SERVO2)
      #error "SERVO2_PIN must be defined for your servo or BLTOUCH probe."
    #elif Z_PROBE_SERVO_NR == 3 && !PIN_EXISTS(SERVO3)
      #error "SERVO3_PIN must be defined for your servo or BLTOUCH probe."
    #elif Z_PROBE_SERVO_NR >= NUM_SERVOS
      #error "Z_PROBE_SERVO_NR must be smaller than NUM_SERVOS."
    #endif
  #endif

  #if ENABLED(BLTOUCH)
    #if BLTOUCH_DELAY < 200
      #error "BLTOUCH_DELAY less than 200 is unsafe and is not supported."
    #elif DISABLED(BLTOUCH_SET_5V_MODE) && NONE(ONBOARD_ENDSTOPPULLUPS, ENDSTOPPULLUPS, ENDSTOPPULLUP_ZMIN, ENDSTOPPULLUP_ZMIN_PROBE)
      #error "BLTOUCH without BLTOUCH_SET_5V_MODE requires ENDSTOPPULLUPS, ENDSTOPPULLUP_ZMIN or ENDSTOPPULLUP_ZMIN_PROBE."
    #endif
  #endif

  #if ENABLED(RACK_AND_PINION_PROBE) && !(defined(Z_PROBE_DEPLOY_X) && defined(Z_PROBE_RETRACT_X))
    #error "RACK_AND_PINION_PROBE requires Z_PROBE_DEPLOY_X and Z_PROBE_RETRACT_X."
  #endif

  /**
   * Touch-MI probe requirements
   */
  #if ENABLED(TOUCH_MI_PROBE)
    #if DISABLED(Z_SAFE_HOMING)
      #error "TOUCH_MI_PROBE requires Z_SAFE_HOMING."
    #elif !defined(TOUCH_MI_RETRACT_Z)
      #error "TOUCH_MI_PROBE requires TOUCH_MI_RETRACT_Z."
    #elif defined(Z_AFTER_PROBING)
      #error "TOUCH_MI_PROBE requires Z_AFTER_PROBING to be disabled."
    #elif Z_HOMING_HEIGHT < 10
      #error "TOUCH_MI_PROBE requires Z_HOMING_HEIGHT >= 10."
    #elif Z_MIN_PROBE_ENDSTOP_INVERTING
      #error "TOUCH_MI_PROBE requires Z_MIN_PROBE_ENDSTOP_INVERTING to be set to false."
    #elif DISABLED(BABYSTEP_ZPROBE_OFFSET)
      #error "TOUCH_MI_PROBE requires BABYSTEPPING with BABYSTEP_ZPROBE_OFFSET."
    #elif !HAS_RESUME_CONTINUE
      #error "TOUCH_MI_PROBE currently requires an LCD controller or EMERGENCY_PARSER."
    #endif
  #endif

  /**
   * Require pin options and pins to be defined
   */
  #if ENABLED(SENSORLESS_PROBING)
    #if ENABLED(DELTA) && !(AXIS_HAS_STALLGUARD(X) && AXIS_HAS_STALLGUARD(Y) && AXIS_HAS_STALLGUARD(Z))
      #error "SENSORLESS_PROBING requires TMC2130/2160/2209/5130/5160 drivers on X, Y, and Z."
    #elif !AXIS_HAS_STALLGUARD(Z)
      #error "SENSORLESS_PROBING requires a TMC2130/2160/2209/5130/5160 driver on Z."
    #endif
  #elif ENABLED(Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN)
    #if DISABLED(USE_ZMIN_PLUG)
      #error "Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN requires USE_ZMIN_PLUG to be enabled."
    #elif !HAS_Z_MIN
      #error "Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN requires the Z_MIN_PIN to be defined."
    #elif Z_MIN_PROBE_ENDSTOP_INVERTING != Z_MIN_ENDSTOP_INVERTING
      #error "Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN requires Z_MIN_ENDSTOP_INVERTING to match Z_MIN_PROBE_ENDSTOP_INVERTING."
    #endif
  #elif !HAS_Z_MIN_PROBE_PIN
    #error "Z_MIN_PROBE_PIN must be defined if Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN is not enabled."
  #endif

  #if ENABLED(NOZZLE_AS_PROBE)
    constexpr float sanity_nozzle_to_probe_offset[] = NOZZLE_TO_PROBE_OFFSET;
    static_assert(sanity_nozzle_to_probe_offset[0] == 0.0 && sanity_nozzle_to_probe_offset[1] == 0.0,
                  "NOZZLE_AS_PROBE requires the X,Y offsets in NOZZLE_TO_PROBE_OFFSET to be 0,0.");
  #endif

  #if DISABLED(NOZZLE_AS_PROBE)
    static_assert(PROBING_MARGIN >= 0, "PROBING_MARGIN must be >= 0.");
    static_assert(PROBING_MARGIN_BACK >= 0, "PROBING_MARGIN_BACK must be >= 0.");
    static_assert(PROBING_MARGIN_FRONT >= 0, "PROBING_MARGIN_FRONT must be >= 0.");
    static_assert(PROBING_MARGIN_LEFT >= 0, "PROBING_MARGIN_LEFT must be >= 0.");
    static_assert(PROBING_MARGIN_RIGHT >= 0, "PROBING_MARGIN_RIGHT must be >= 0.");
  #endif

  #define _MARGIN(A) TERN(IS_SCARA, SCARA_PRINTABLE_RADIUS, TERN(DELTA, DELTA_PRINTABLE_RADIUS, A##_CENTER))
  static_assert(PROBING_MARGIN < _MARGIN(X), "PROBING_MARGIN is too large.");
  static_assert(PROBING_MARGIN_BACK < _MARGIN(Y), "PROBING_MARGIN_BACK is too large.");
  static_assert(PROBING_MARGIN_FRONT < _MARGIN(Y), "PROBING_MARGIN_FRONT is too large.");
  static_assert(PROBING_MARGIN_LEFT < _MARGIN(X), "PROBING_MARGIN_LEFT is too large.");
  static_assert(PROBING_MARGIN_RIGHT < _MARGIN(X), "PROBING_MARGIN_RIGHT is too large.");
  #undef _MARGIN

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

  #if MULTIPLE_PROBING > 0 || EXTRA_PROBING > 0
    #if MULTIPLE_PROBING == 0
      #error "EXTRA_PROBING requires MULTIPLE_PROBING."
    #elif MULTIPLE_PROBING < 2
      #error "MULTIPLE_PROBING must be 2 or more."
    #elif MULTIPLE_PROBING <= EXTRA_PROBING
      #error "EXTRA_PROBING must be less than MULTIPLE_PROBING."
    #endif
  #endif

  #if Z_PROBE_LOW_POINT > 0
    #error "Z_PROBE_LOW_POINT must be less than or equal to 0."
  #endif

  #if HOMING_Z_WITH_PROBE && IS_CARTESIAN && DISABLED(Z_SAFE_HOMING)
    #error "Z_SAFE_HOMING is recommended when homing with a probe. Enable it or comment out this line to continue."
  #endif

#else

  /**
   * Require some kind of probe for bed leveling and probe testing
   */
  #if HAS_ABL_NOT_UBL && !PROBE_SELECTED
    #error "Auto Bed Leveling requires one of these: PROBE_MANUALLY, SENSORLESS_PROBING, BLTOUCH, FIX_MOUNTED_PROBE, NOZZLE_AS_PROBE, TOUCH_MI_PROBE, SOLENOID_PROBE, Z_PROBE_ALLEN_KEY, Z_PROBE_SLED, or a Z Servo."
  #endif

  #if ENABLED(Z_MIN_PROBE_REPEATABILITY_TEST)
    #error "Z_MIN_PROBE_REPEATABILITY_TEST requires a probe: FIX_MOUNTED_PROBE, NOZZLE_AS_PROBE, BLTOUCH, SOLENOID_PROBE, Z_PROBE_ALLEN_KEY, Z_PROBE_SLED, or Z Servo."
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

#elif HAS_ABL_NOT_UBL

  /**
   * Auto Bed Leveling
   */

  /**
   * Delta and SCARA have limited bed leveling options
   */
  #if IS_SCARA && DISABLED(AUTO_BED_LEVELING_BILINEAR)
    #error "SCARA machines can only use the AUTO_BED_LEVELING_BILINEAR leveling option."
  #endif

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

#if HAS_MESH && HAS_CLASSIC_JERK
  static_assert(DEFAULT_ZJERK > 0.1, "Low DEFAULT_ZJERK values are incompatible with mesh-based leveling.");
#endif

#if ENABLED(G26_MESH_VALIDATION)
  #if !EXTRUDERS
    #error "G26_MESH_VALIDATION requires at least one extruder."
  #elif !HAS_MESH
    #error "G26_MESH_VALIDATION requires MESH_BED_LEVELING, AUTO_BED_LEVELING_BILINEAR, or AUTO_BED_LEVELING_UBL."
  #endif
#endif

#if ENABLED(MESH_EDIT_GFX_OVERLAY) && !BOTH(AUTO_BED_LEVELING_UBL, HAS_GRAPHICAL_LCD)
  #error "MESH_EDIT_GFX_OVERLAY requires AUTO_BED_LEVELING_UBL and a Graphical LCD."
#endif

#if ENABLED(G29_RETRY_AND_RECOVER)
  #if ENABLED(AUTO_BED_LEVELING_UBL)
    #error "G29_RETRY_AND_RECOVER is not compatible with UBL."
  #elif ENABLED(MESH_BED_LEVELING)
    #error "G29_RETRY_AND_RECOVER is not compatible with MESH_BED_LEVELING."
  #endif
#endif

/**
 * LCD_BED_LEVELING requirements
 */
#if ENABLED(LCD_BED_LEVELING)
  #if !HAS_LCD_MENU
    #error "LCD_BED_LEVELING requires a programmable LCD controller."
  #elif !(ENABLED(MESH_BED_LEVELING) || HAS_ABL_NOT_UBL)
    #error "LCD_BED_LEVELING requires MESH_BED_LEVELING or AUTO_BED_LEVELING."
  #endif
#endif

/**
 * Homing
 */
constexpr float hbm[] = HOMING_BUMP_MM;
static_assert(COUNT(hbm) == XYZ, "HOMING_BUMP_MM requires X, Y, and Z elements.");
static_assert(hbm[X_AXIS] >= 0, "HOMING_BUMP_MM.X must be greater than or equal to 0.");
static_assert(hbm[Y_AXIS] >= 0, "HOMING_BUMP_MM.Y must be greater than or equal to 0.");
static_assert(hbm[Z_AXIS] >= 0, "HOMING_BUMP_MM.Z must be greater than or equal to 0.");

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
  static_assert(WITHIN(Z_SAFE_HOMING_X_POINT, X_MIN_POS, X_MAX_POS), "Z_SAFE_HOMING_X_POINT can't be reached by the nozzle.");
  static_assert(WITHIN(Z_SAFE_HOMING_Y_POINT, Y_MIN_POS, Y_MAX_POS), "Z_SAFE_HOMING_Y_POINT can't be reached by the nozzle.");
#endif

/**
 * Make sure DISABLE_[XYZ] compatible with selected homing options
 */
#if ANY(DISABLE_X, DISABLE_Y, DISABLE_Z)
  #if EITHER(HOME_AFTER_DEACTIVATE, Z_SAFE_HOMING)
    #error "DISABLE_[XYZ] is not compatible with HOME_AFTER_DEACTIVATE or Z_SAFE_HOMING."
  #endif
#endif

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
 * System Power Sensor
 */
#if ENABLED(POWER_MONITOR_CURRENT) && !PIN_EXISTS(POWER_MONITOR_CURRENT)
  #error "POWER_MONITOR_CURRENT requires a valid POWER_MONITOR_CURRENT_PIN."
#elif ENABLED(POWER_MONITOR_VOLTAGE) && !PIN_EXISTS(POWER_MONITOR_VOLTAGE)
  #error "POWER_MONITOR_VOLTAGE requires POWER_MONITOR_VOLTAGE_PIN to be defined."
#elif BOTH(POWER_MONITOR_CURRENT, POWER_MONITOR_VOLTAGE) && POWER_MONITOR_CURRENT_PIN == POWER_MONITOR_VOLTAGE_PIN
  #error "POWER_MONITOR_CURRENT_PIN and POWER_MONITOR_VOLTAGE_PIN must be different."
#endif

/**
 * Volumetric Extruder Limit
 */
#if ENABLED(VOLUMETRIC_EXTRUDER_LIMIT)
  #if ENABLED(NO_VOLUMETRICS)
    #error "VOLUMETRIC_EXTRUDER_LIMIT requires NO_VOLUMETRICS to be disabled."
  #elif MIN_STEPS_PER_SEGMENT > 1
    #error "VOLUMETRIC_EXTRUDER_LIMIT is not compatible with MIN_STEPS_PER_SEGMENT greater than 1."
  #endif
#endif

/**
 * ULTIPANEL encoder
 */
#if ENABLED(ULTIPANEL) && NONE(NEWPANEL, SR_LCD_2W_NL) && !defined(SHIFT_CLK)
  #error "ULTIPANEL requires some kind of encoder."
#endif

#if ENCODER_PULSES_PER_STEP < 0
  #error "ENCODER_PULSES_PER_STEP should not be negative, use REVERSE_MENU_DIRECTION instead."
#endif

/**
 * SAV_3DGLCD display options
 */
#if ENABLED(SAV_3DGLCD)
  #if NONE(U8GLIB_SSD1306, U8GLIB_SH1106)
    #error "Enable a SAV_3DGLCD display type: U8GLIB_SSD1306 or U8GLIB_SH1106."
  #elif BOTH(U8GLIB_SSD1306, U8GLIB_SH1106)
    #error "Only enable one SAV_3DGLCD display type: U8GLIB_SSD1306 or U8GLIB_SH1106."
  #endif
#endif

/**
 * Allen Key
 * Deploying the Allen Key probe uses big moves in z direction. Too dangerous for an unhomed z-axis.
 */
#if ENABLED(Z_PROBE_ALLEN_KEY) && (Z_HOME_DIR < 0) && ENABLED(Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN)
  #error "You can't home to a z min endstop with a Z_PROBE_ALLEN_KEY."
#endif

/**
 * Dual X Carriage requirements
 */
#if ENABLED(DUAL_X_CARRIAGE)
  #if EXTRUDERS < 2
    #error "DUAL_X_CARRIAGE requires 2 (or more) extruders."
  #elif CORE_IS_XY || CORE_IS_XZ
    #error "DUAL_X_CARRIAGE cannot be used with COREXY, COREYX, COREXZ, or COREZX."
  #elif !GOOD_AXIS_PINS(X2)
    #error "DUAL_X_CARRIAGE requires X2 stepper pins to be defined."
  #elif !HAS_X_MAX
    #error "DUAL_X_CARRIAGE requires USE_XMAX_PLUG and an X Max Endstop."
  #elif !defined(X2_HOME_POS) || !defined(X2_MIN_POS) || !defined(X2_MAX_POS)
    #error "DUAL_X_CARRIAGE requires X2_HOME_POS, X2_MIN_POS, and X2_MAX_POS."
  #elif X_HOME_DIR != -1 || X2_HOME_DIR != 1
    #error "DUAL_X_CARRIAGE requires X_HOME_DIR -1 and X2_HOME_DIR 1."
  #endif
#endif

#undef GOOD_AXIS_PINS

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
 * Case Light requirements
 */
#if ENABLED(CASE_LIGHT_ENABLE)
  #if !PIN_EXISTS(CASE_LIGHT)
    #error "CASE_LIGHT_ENABLE requires CASE_LIGHT_PIN to be defined."
  #elif CASE_LIGHT_PIN == FAN_PIN
    #error "CASE_LIGHT_PIN conflicts with FAN_PIN. Resolve before continuing."
  #endif
#endif

/**
 * Required custom thermistor settings
 */
#if   ENABLED(HEATER_0_USER_THERMISTOR) && !(defined(HOTEND0_PULLUP_RESISTOR_OHMS) && defined(HOTEND0_RESISTANCE_25C_OHMS) && defined(HOTEND0_BETA))
  #error "TEMP_SENSOR_0 1000 requires HOTEND0_PULLUP_RESISTOR_OHMS, HOTEND0_RESISTANCE_25C_OHMS and HOTEND0_BETA in Configuration_adv.h."
#elif ENABLED(HEATER_1_USER_THERMISTOR) && !(defined(HOTEND1_PULLUP_RESISTOR_OHMS) && defined(HOTEND1_RESISTANCE_25C_OHMS) && defined(HOTEND1_BETA))
  #error "TEMP_SENSOR_1 1000 requires HOTEND1_PULLUP_RESISTOR_OHMS, HOTEND1_RESISTANCE_25C_OHMS and HOTEND1_BETA in Configuration_adv.h."
#elif ENABLED(HEATER_2_USER_THERMISTOR) && !(defined(HOTEND2_PULLUP_RESISTOR_OHMS) && defined(HOTEND2_RESISTANCE_25C_OHMS) && defined(HOTEND2_BETA))
  #error "TEMP_SENSOR_2 1000 requires HOTEND2_PULLUP_RESISTOR_OHMS, HOTEND2_RESISTANCE_25C_OHMS and HOTEND2_BETA in Configuration_adv.h."
#elif ENABLED(HEATER_3_USER_THERMISTOR) && !(defined(HOTEND3_PULLUP_RESISTOR_OHMS) && defined(HOTEND3_RESISTANCE_25C_OHMS) && defined(HOTEND3_BETA))
  #error "TEMP_SENSOR_3 1000 requires HOTEND3_PULLUP_RESISTOR_OHMS, HOTEND3_RESISTANCE_25C_OHMS and HOTEND3_BETA in Configuration_adv.h."
#elif ENABLED(HEATER_4_USER_THERMISTOR) && !(defined(HOTEND4_PULLUP_RESISTOR_OHMS) && defined(HOTEND4_RESISTANCE_25C_OHMS) && defined(HOTEND4_BETA))
  #error "TEMP_SENSOR_4 1000 requires HOTEND4_PULLUP_RESISTOR_OHMS, HOTEND4_RESISTANCE_25C_OHMS and HOTEND4_BETA in Configuration_adv.h."
#elif ENABLED(HEATER_5_USER_THERMISTOR) && !(defined(HOTEND5_PULLUP_RESISTOR_OHMS) && defined(HOTEND5_RESISTANCE_25C_OHMS) && defined(HOTEND5_BETA))
  #error "TEMP_SENSOR_5 1000 requires HOTEND5_PULLUP_RESISTOR_OHMS, HOTEND5_RESISTANCE_25C_OHMS and HOTEND5_BETA in Configuration_adv.h."
#elif ENABLED(HEATER_6_USER_THERMISTOR) && !(defined(HOTEND6_PULLUP_RESISTOR_OHMS) && defined(HOTEND6_RESISTANCE_25C_OHMS) && defined(HOTEND6_BETA))
  #error "TEMP_SENSOR_6 1000 requires HOTEND6_PULLUP_RESISTOR_OHMS, HOTEND6_RESISTANCE_25C_OHMS and HOTEND6_BETA in Configuration_adv.h."
#elif ENABLED(HEATER_7_USER_THERMISTOR) && !(defined(HOTEND7_PULLUP_RESISTOR_OHMS) && defined(HOTEND7_RESISTANCE_25C_OHMS) && defined(HOTEND7_BETA))
  #error "TEMP_SENSOR_7 1000 requires HOTEND7_PULLUP_RESISTOR_OHMS, HOTEND7_RESISTANCE_25C_OHMS and HOTEND7_BETA in Configuration_adv.h."
#elif ENABLED(HEATER_BED_USER_THERMISTOR) && !(defined(BED_PULLUP_RESISTOR_OHMS) && defined(BED_RESISTANCE_25C_OHMS) && defined(BED_BETA))
  #error "TEMP_SENSOR_BED 1000 requires BED_PULLUP_RESISTOR_OHMS, BED_RESISTANCE_25C_OHMS and BED_BETA in Configuration_adv.h."
#elif ENABLED(HEATER_CHAMBER_USER_THERMISTOR) && !(defined(CHAMBER_PULLUP_RESISTOR_OHMS) && defined(CHAMBER_RESISTANCE_25C_OHMS) && defined(CHAMBER_BETA))
  #error "TEMP_SENSOR_CHAMBER 1000 requires CHAMBER_PULLUP_RESISTOR_OHMS, CHAMBER_RESISTANCE_25C_OHMS and CHAMBER_BETA in Configuration_adv.h."
#endif

/**
 * A Sensor ID has to be set for each heater
 */

#if HAS_MULTI_HOTEND
  #if ENABLED(HEATER_1_USES_MAX6675) && !PIN_EXISTS(MAX6675_SS2)
    #error "MAX6675_SS2_PIN (required for TEMP_SENSOR_1) not defined for this board."
  #elif TEMP_SENSOR_1 == 0
    #error "TEMP_SENSOR_1 is required with 2 or more HOTENDS."
  #elif !ANY_PIN(TEMP_1, MAX6675_SS2)
    #error "TEMP_1_PIN not defined for this board."
  #elif ENABLED(TEMP_SENSOR_1_AS_REDUNDANT)
    #error "HOTENDS must be 1 with TEMP_SENSOR_1_AS_REDUNDANT."
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
          #error "TEMP_SENSOR_4 is required with 5 or more HOTENDS."
        #elif !HAS_HEATER_4
          #error "HEATER_4_PIN not defined for this board."
        #elif !PIN_EXISTS(TEMP_4)
          #error "TEMP_4_PIN not defined for this board."
        #endif
        #if HOTENDS > 5
          #if TEMP_SENSOR_5 == 0
            #error "TEMP_SENSOR_5 is required with 6 HOTENDS."
          #elif !HAS_HEATER_5
            #error "HEATER_5_PIN not defined for this board."
          #elif !PIN_EXISTS(TEMP_5)
            #error "TEMP_5_PIN not defined for this board."
          #endif
          #if HOTENDS > 6
            #if TEMP_SENSOR_6 == 0
              #error "TEMP_SENSOR_6 is required with 6 HOTENDS."
            #elif !HAS_HEATER_6
              #error "HEATER_6_PIN not defined for this board."
            #elif !PIN_EXISTS(TEMP_6)
              #error "TEMP_6_PIN not defined for this board."
            #endif
            #if HOTENDS > 7
              #if TEMP_SENSOR_7 == 0
                #error "TEMP_SENSOR_7 is required with 7 HOTENDS."
              #elif !HAS_HEATER_7
                #error "HEATER_7_PIN not defined for this board."
              #elif !PIN_EXISTS(TEMP_7)
                #error "TEMP_7_PIN not defined for this board."
              #endif
            #elif TEMP_SENSOR_7 != 0
              #error "TEMP_SENSOR_7 shouldn't be set with only 7 HOTENDS."
            #endif
          #elif TEMP_SENSOR_6 != 0
            #error "TEMP_SENSOR_6 shouldn't be set with only 6 HOTENDS."
          #elif TEMP_SENSOR_7 != 0
            #error "TEMP_SENSOR_7 shouldn't be set with only 6 HOTENDS."
          #endif
        #elif TEMP_SENSOR_5 != 0
          #error "TEMP_SENSOR_5 shouldn't be set with only 5 HOTENDS."
        #elif TEMP_SENSOR_6 != 0
          #error "TEMP_SENSOR_6 shouldn't be set with only 5 HOTENDS."
        #elif TEMP_SENSOR_7 != 0
          #error "TEMP_SENSOR_7 shouldn't be set with only 5 HOTENDS."
        #endif
      #elif TEMP_SENSOR_4 != 0
        #error "TEMP_SENSOR_4 shouldn't be set with only 4 HOTENDS."
      #elif TEMP_SENSOR_5 != 0
        #error "TEMP_SENSOR_5 shouldn't be set with only 4 HOTENDS."
      #elif TEMP_SENSOR_6 != 0
        #error "TEMP_SENSOR_6 shouldn't be set with only 4 HOTENDS."
      #elif TEMP_SENSOR_7 != 0
        #error "TEMP_SENSOR_7 shouldn't be set with only 4 HOTENDS."
      #endif
    #elif TEMP_SENSOR_3 != 0
      #error "TEMP_SENSOR_3 shouldn't be set with only 3 HOTENDS."
    #elif TEMP_SENSOR_4 != 0
      #error "TEMP_SENSOR_4 shouldn't be set with only 3 HOTENDS."
    #elif TEMP_SENSOR_5 != 0
      #error "TEMP_SENSOR_5 shouldn't be set with only 3 HOTENDS."
    #elif TEMP_SENSOR_6 != 0
      #error "TEMP_SENSOR_6 shouldn't be set with only 3 HOTENDS."
    #elif TEMP_SENSOR_7 != 0
      #error "TEMP_SENSOR_7 shouldn't be set with only 3 HOTENDS."
    #endif
  #elif TEMP_SENSOR_2 != 0
    #error "TEMP_SENSOR_2 shouldn't be set with only 2 HOTENDS."
  #elif TEMP_SENSOR_3 != 0
    #error "TEMP_SENSOR_3 shouldn't be set with only 2 HOTENDS."
  #elif TEMP_SENSOR_4 != 0
    #error "TEMP_SENSOR_4 shouldn't be set with only 2 HOTENDS."
  #elif TEMP_SENSOR_5 != 0
    #error "TEMP_SENSOR_5 shouldn't be set with only 2 HOTENDS."
  #elif TEMP_SENSOR_6 != 0
    #error "TEMP_SENSOR_6 shouldn't be set with only 2 HOTENDS."
  #elif TEMP_SENSOR_7 != 0
    #error "TEMP_SENSOR_7 shouldn't be set with only 2 HOTENDS."
  #endif
#elif TEMP_SENSOR_1 != 0 && DISABLED(TEMP_SENSOR_1_AS_REDUNDANT)
  #error "TEMP_SENSOR_1 shouldn't be set with only 1 HOTEND."
#elif TEMP_SENSOR_2 != 0
  #error "TEMP_SENSOR_2 shouldn't be set with only 1 HOTEND."
#elif TEMP_SENSOR_3 != 0
  #error "TEMP_SENSOR_3 shouldn't be set with only 1 HOTEND."
#elif TEMP_SENSOR_4 != 0
  #error "TEMP_SENSOR_4 shouldn't be set with only 1 HOTEND."
#elif TEMP_SENSOR_5 != 0
  #error "TEMP_SENSOR_5 shouldn't be set with only 1 HOTEND."
#elif TEMP_SENSOR_6 != 0
  #error "TEMP_SENSOR_6 shouldn't be set with only 1 HOTEND."
#elif TEMP_SENSOR_7 != 0
  #error "TEMP_SENSOR_7 shouldn't be set with only 1 HOTEND."
#endif

#if TEMP_SENSOR_CHAMBER && !PIN_EXISTS(TEMP_CHAMBER)
  #error "TEMP_SENSOR_CHAMBER requires TEMP_CHAMBER_PIN. Please add it to your configuration."
#endif

#if TEMP_SENSOR_PROBE
  #if !PIN_EXISTS(TEMP_PROBE)
    #error "TEMP_SENSOR_PROBE requires TEMP_PROBE_PIN. Please add it to your configuration."
  #elif !HAS_TEMP_ADC_PROBE
    #error "TEMP_PROBE_PIN must be an ADC pin."
  #elif !ENABLED(FIX_MOUNTED_PROBE)
    #error "TEMP_SENSOR_PROBE shouldn't be set without FIX_MOUNTED_PROBE."
  #endif
#endif

#if ENABLED(TEMP_SENSOR_1_AS_REDUNDANT) && TEMP_SENSOR_1 == 0
  #error "TEMP_SENSOR_1 is required with TEMP_SENSOR_1_AS_REDUNDANT."
#endif

/**
 * Test Heater, Temp Sensor, and Extruder Pins
 */
#if !HAS_HEATER_0 && EXTRUDERS
  #error "HEATER_0_PIN not defined for this board."
#elif !ANY_PIN(TEMP_0, MAX6675_SS)
  #error "TEMP_0_PIN not defined for this board."
#elif ((defined(__AVR_ATmega644P__) || defined(__AVR_ATmega1284P__)) && !PINS_EXIST(E0_STEP, E0_DIR))
  #error "E0_STEP_PIN or E0_DIR_PIN not defined for this board."
#elif ( !(defined(__AVR_ATmega644P__) || defined(__AVR_ATmega1284P__)) && (!PINS_EXIST(E0_STEP, E0_DIR) || !HAS_E0_ENABLE))
  #error "E0_STEP_PIN, E0_DIR_PIN, or E0_ENABLE_PIN not defined for this board."
#elif EXTRUDERS && TEMP_SENSOR_0 == 0
  #error "TEMP_SENSOR_0 is required if there are any extruders."
#endif

// Pins are required for heaters
#if ENABLED(HEATER_0_USES_MAX6675) && !PIN_EXISTS(MAX6675_SS)
  #error "MAX6675_SS_PIN (required for TEMP_SENSOR_0) not defined for this board."
#elif HAS_HOTEND && !HAS_TEMP_HOTEND
  #error "TEMP_0_PIN (required for TEMP_SENSOR_0) not defined for this board."
#elif EITHER(HAS_MULTI_HOTEND, HEATERS_PARALLEL) && !HAS_HEATER_1
  #error "HEATER_1_PIN is not defined. TEMP_SENSOR_1 might not be set, or the board (not EEB / EEF?) doesn't define a pin."
#endif

/**
 * Temperature status LEDs
 */
#if ENABLED(TEMP_STAT_LEDS) && !ANY_PIN(STAT_LED_RED, STAT_LED_BLUE)
  #error "TEMP_STAT_LEDS requires STAT_LED_RED_PIN or STAT_LED_BLUE_PIN, preferably both."
#endif

/**
 * LED Control Menu
 */
#if ENABLED(LED_CONTROL_MENU) && !HAS_COLOR_LEDS
  #error "LED_CONTROL_MENU requires BLINKM, RGB_LED, RGBW_LED, PCA9533, PCA9632, or NEOPIXEL_LED."
#endif

/**
 * LED Backlight Timeout
 */
#if defined(LED_BACKLIGHT_TIMEOUT) && !(ENABLED(PSU_CONTROL) && EITHER(FYSETC_MINI_12864_2_0, FYSETC_MINI_12864_2_1))
  #error "LED_BACKLIGHT_TIMEOUT requires a FYSETC Mini Panel and a Power Switch."
#endif

/**
 * Basic multi hotend duplication mode
 */
#if ENABLED(MULTI_NOZZLE_DUPLICATION)
  #if HOTENDS < 2
    #error "MULTI_NOZZLE_DUPLICATION requires 2 or more hotends."
  #elif ENABLED(DUAL_X_CARRIAGE)
    #error "MULTI_NOZZLE_DUPLICATION is incompatible with DUAL_X_CARRIAGE."
  #elif ENABLED(SINGLENOZZLE)
    #error "MULTI_NOZZLE_DUPLICATION is incompatible with SINGLENOZZLE."
  #elif ENABLED(MIXING_EXTRUDER)
    #error "MULTI_NOZZLE_DUPLICATION is incompatible with MIXING_EXTRUDER."
  #elif ENABLED(SWITCHING_EXTRUDER)
    #error "MULTI_NOZZLE_DUPLICATION is incompatible with SWITCHING_EXTRUDER."
  #endif
#endif

/**
 * Test Extruder Stepper Pins
 */
#if DISABLED(MK2_MULTIPLEXER) // MK2_MULTIPLEXER uses E0 stepper only
  #if E_STEPPERS
    #if !(PINS_EXIST(E0_STEP, E0_DIR) && HAS_E0_ENABLE)
      #error "E0_STEP_PIN, E0_DIR_PIN, or E0_ENABLE_PIN not defined for this board."
    #endif
    #if E_STEPPERS > 1
      #if !(PINS_EXIST(E1_STEP, E1_DIR) && HAS_E1_ENABLE)
        #error "E1_STEP_PIN, E1_DIR_PIN, or E1_ENABLE_PIN not defined for this board."
      #endif
      #if E_STEPPERS > 2
        #if !(PINS_EXIST(E2_STEP, E2_DIR) && HAS_E2_ENABLE)
          #error "E2_STEP_PIN, E2_DIR_PIN, or E2_ENABLE_PIN not defined for this board."
        #endif
        #if E_STEPPERS > 3
          #if !(PINS_EXIST(E3_STEP, E3_DIR) && HAS_E3_ENABLE)
            #error "E3_STEP_PIN, E3_DIR_PIN, or E3_ENABLE_PIN not defined for this board."
          #endif
          #if E_STEPPERS > 4
            #if !(PINS_EXIST(E4_STEP, E4_DIR) && HAS_E4_ENABLE)
              #error "E4_STEP_PIN, E4_DIR_PIN, or E4_ENABLE_PIN not defined for this board."
            #endif
            #if E_STEPPERS > 5
              #if !(PINS_EXIST(E5_STEP, E5_DIR) && HAS_E5_ENABLE)
                #error "E5_STEP_PIN, E5_DIR_PIN, or E5_ENABLE_PIN not defined for this board."
              #endif
              #if E_STEPPERS > 6
                #if !(PINS_EXIST(E6_STEP, E6_DIR) && HAS_E6_ENABLE)
                  #error "E6_STEP_PIN, E6_DIR_PIN, or E6_ENABLE_PIN not defined for this board."
                #endif
                #if E_STEPPERS > 7
                  #if !(PINS_EXIST(E7_STEP, E7_DIR) && HAS_E7_ENABLE)
                    #error "E7_STEP_PIN, E7_DIR_PIN, or E7_ENABLE_PIN not defined for this board."
                  #endif
                #endif // E_STEPPERS > 7
              #endif // E_STEPPERS > 6
            #endif // E_STEPPERS > 5
          #endif // E_STEPPERS > 4
        #endif // E_STEPPERS > 3
      #endif // E_STEPPERS > 2
    #endif // E_STEPPERS > 1
  #endif // E_STEPPERS
#endif

/**
 * Endstop Tests
 */

#define _PLUG_UNUSED_TEST(A,P) (DISABLED(USE_##P##MIN_PLUG, USE_##P##MAX_PLUG) \
  && !(ENABLED(A##_DUAL_ENDSTOPS) && WITHIN(A##2_USE_ENDSTOP, _##P##MAX_, _##P##MIN_)) \
  && !(ENABLED(A##_MULTI_ENDSTOPS) && WITHIN(A##2_USE_ENDSTOP, _##P##MAX_, _##P##MIN_)) )
#define _AXIS_PLUG_UNUSED_TEST(A) (_PLUG_UNUSED_TEST(A,X) && _PLUG_UNUSED_TEST(A,Y) && _PLUG_UNUSED_TEST(A,Z))

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

// Z homing direction and plug usage flags
#if Z_HOME_DIR < 0 && NONE(USE_ZMIN_PLUG, HOMING_Z_WITH_PROBE)
  #error "Enable USE_ZMIN_PLUG when homing Z to MIN."
#elif Z_HOME_DIR > 0 && ENABLED(USE_PROBE_FOR_Z_HOMING)
  #error "Z_HOME_DIR must be -1 when homing Z with the probe."
#elif Z_HOME_DIR > 0 && DISABLED(USE_ZMAX_PLUG)
  #error "Enable USE_ZMAX_PLUG when homing Z to MAX."
#endif

// Dual/multiple endstops requirements
#if ENABLED(X_DUAL_ENDSTOPS)
  #if !X2_USE_ENDSTOP
    #error "You must set X2_USE_ENDSTOP with X_DUAL_ENDSTOPS."
  #elif X2_USE_ENDSTOP == _XMIN_ && DISABLED(USE_XMIN_PLUG)
    #error "USE_XMIN_PLUG is required when X2_USE_ENDSTOP is _XMIN_."
  #elif X2_USE_ENDSTOP == _XMAX_ && DISABLED(USE_XMAX_PLUG)
    #error "USE_XMAX_PLUG is required when X2_USE_ENDSTOP is _XMAX_."
  #elif X2_USE_ENDSTOP == _YMIN_ && DISABLED(USE_YMIN_PLUG)
    #error "USE_YMIN_PLUG is required when X2_USE_ENDSTOP is _YMIN_."
  #elif X2_USE_ENDSTOP == _YMAX_ && DISABLED(USE_YMAX_PLUG)
    #error "USE_YMAX_PLUG is required when X2_USE_ENDSTOP is _YMAX_."
  #elif X2_USE_ENDSTOP == _ZMIN_ && DISABLED(USE_ZMIN_PLUG)
    #error "USE_ZMIN_PLUG is required when X2_USE_ENDSTOP is _ZMIN_."
  #elif X2_USE_ENDSTOP == _ZMAX_ && DISABLED(USE_ZMAX_PLUG)
    #error "USE_ZMAX_PLUG is required when X2_USE_ENDSTOP is _ZMAX_."
  #elif !HAS_X2_MIN && !HAS_X2_MAX
    #error "X2_USE_ENDSTOP has been assigned to a nonexistent endstop!"
  #elif ENABLED(DELTA)
    #error "X_DUAL_ENDSTOPS is not compatible with DELTA."
  #endif
#endif
#if ENABLED(Y_DUAL_ENDSTOPS)
  #if !Y2_USE_ENDSTOP
    #error "You must set Y2_USE_ENDSTOP with Y_DUAL_ENDSTOPS."
  #elif Y2_USE_ENDSTOP == _XMIN_ && DISABLED(USE_XMIN_PLUG)
    #error "USE_XMIN_PLUG is required when Y2_USE_ENDSTOP is _XMIN_."
  #elif Y2_USE_ENDSTOP == _XMAX_ && DISABLED(USE_XMAX_PLUG)
    #error "USE_XMAX_PLUG is required when Y2_USE_ENDSTOP is _XMAX_."
  #elif Y2_USE_ENDSTOP == _YMIN_ && DISABLED(USE_YMIN_PLUG)
    #error "USE_YMIN_PLUG is required when Y2_USE_ENDSTOP is _YMIN_."
  #elif Y2_USE_ENDSTOP == _YMAX_ && DISABLED(USE_YMAX_PLUG)
    #error "USE_YMAX_PLUG is required when Y2_USE_ENDSTOP is _YMAX_."
  #elif Y2_USE_ENDSTOP == _ZMIN_ && DISABLED(USE_ZMIN_PLUG)
    #error "USE_ZMIN_PLUG is required when Y2_USE_ENDSTOP is _ZMIN_."
  #elif Y2_USE_ENDSTOP == _ZMAX_ && DISABLED(USE_ZMAX_PLUG)
    #error "USE_ZMAX_PLUG is required when Y2_USE_ENDSTOP is _ZMAX_."
  #elif !HAS_Y2_MIN && !HAS_Y2_MAX
    #error "Y2_USE_ENDSTOP has been assigned to a nonexistent endstop!"
  #elif ENABLED(DELTA)
    #error "Y_DUAL_ENDSTOPS is not compatible with DELTA."
  #endif
#endif

#if ENABLED(Z_MULTI_ENDSTOPS)
  #if !Z2_USE_ENDSTOP
    #error "You must set Z2_USE_ENDSTOP with Z_MULTI_ENDSTOPS when NUM_Z_STEPPER_DRIVERS >= 2."
  #elif Z2_USE_ENDSTOP == _XMIN_ && DISABLED(USE_XMIN_PLUG)
    #error "USE_XMIN_PLUG is required when Z2_USE_ENDSTOP is _XMIN_."
  #elif Z2_USE_ENDSTOP == _XMAX_ && DISABLED(USE_XMAX_PLUG)
    #error "USE_XMAX_PLUG is required when Z2_USE_ENDSTOP is _XMAX_."
  #elif Z2_USE_ENDSTOP == _YMIN_ && DISABLED(USE_YMIN_PLUG)
    #error "USE_YMIN_PLUG is required when Z2_USE_ENDSTOP is _YMIN_."
  #elif Z2_USE_ENDSTOP == _YMAX_ && DISABLED(USE_YMAX_PLUG)
    #error "USE_YMAX_PLUG is required when Z2_USE_ENDSTOP is _YMAX_."
  #elif Z2_USE_ENDSTOP == _ZMIN_ && DISABLED(USE_ZMIN_PLUG)
    #error "USE_ZMIN_PLUG is required when Z2_USE_ENDSTOP is _ZMIN_."
  #elif Z2_USE_ENDSTOP == _ZMAX_ && DISABLED(USE_ZMAX_PLUG)
    #error "USE_ZMAX_PLUG is required when Z2_USE_ENDSTOP is _ZMAX_."
  #elif !HAS_Z2_MIN && !HAS_Z2_MAX
    #error "Z2_USE_ENDSTOP has been assigned to a nonexistent endstop!"
  #elif ENABLED(DELTA)
    #error "Z_MULTI_ENDSTOPS is not compatible with DELTA."
  #endif
  #if NUM_Z_STEPPER_DRIVERS >= 3
    #if !Z3_USE_ENDSTOP
      #error "You must set Z3_USE_ENDSTOP with Z_MULTI_ENDSTOPS when NUM_Z_STEPPER_DRIVERS >= 3."
    #elif Z3_USE_ENDSTOP == _XMIN_ && DISABLED(USE_XMIN_PLUG)
      #error "USE_XMIN_PLUG is required when Z3_USE_ENDSTOP is _XMIN_."
    #elif Z3_USE_ENDSTOP == _XMAX_ && DISABLED(USE_XMAX_PLUG)
      #error "USE_XMAX_PLUG is required when Z3_USE_ENDSTOP is _XMAX_."
    #elif Z3_USE_ENDSTOP == _YMIN_ && DISABLED(USE_YMIN_PLUG)
      #error "USE_YMIN_PLUG is required when Z3_USE_ENDSTOP is _YMIN_."
    #elif Z3_USE_ENDSTOP == _YMAX_ && DISABLED(USE_YMAX_PLUG)
      #error "USE_YMAX_PLUG is required when Z3_USE_ENDSTOP is _YMAX_."
    #elif Z3_USE_ENDSTOP == _ZMIN_ && DISABLED(USE_ZMIN_PLUG)
      #error "USE_ZMIN_PLUG is required when Z3_USE_ENDSTOP is _ZMIN_."
    #elif Z3_USE_ENDSTOP == _ZMAX_ && DISABLED(USE_ZMAX_PLUG)
      #error "USE_ZMAX_PLUG is required when Z3_USE_ENDSTOP is _ZMAX_."
    #elif !HAS_Z3_MIN && !HAS_Z3_MAX
      #error "Z3_USE_ENDSTOP has been assigned to a nonexistent endstop!"
    #endif
  #endif
  #if NUM_Z_STEPPER_DRIVERS >= 4
    #if !Z4_USE_ENDSTOP
      #error "You must set Z4_USE_ENDSTOP with Z_MULTI_ENDSTOPS when NUM_Z_STEPPER_DRIVERS >= 4."
    #elif Z4_USE_ENDSTOP == _XMIN_ && DISABLED(USE_XMIN_PLUG)
      #error "USE_XMIN_PLUG is required when Z4_USE_ENDSTOP is _XMIN_."
    #elif Z4_USE_ENDSTOP == _XMAX_ && DISABLED(USE_XMAX_PLUG)
      #error "USE_XMAX_PLUG is required when Z4_USE_ENDSTOP is _XMAX_."
    #elif Z4_USE_ENDSTOP == _YMIN_ && DISABLED(USE_YMIN_PLUG)
      #error "USE_YMIN_PLUG is required when Z4_USE_ENDSTOP is _YMIN_."
    #elif Z4_USE_ENDSTOP == _YMAX_ && DISABLED(USE_YMAX_PLUG)
      #error "USE_YMAX_PLUG is required when Z4_USE_ENDSTOP is _YMAX_."
    #elif Z4_USE_ENDSTOP == _ZMIN_ && DISABLED(USE_ZMIN_PLUG)
      #error "USE_ZMIN_PLUG is required when Z4_USE_ENDSTOP is _ZMIN_."
    #elif Z4_USE_ENDSTOP == _ZMAX_ && DISABLED(USE_ZMAX_PLUG)
      #error "USE_ZMAX_PLUG is required when Z4_USE_ENDSTOP is _ZMAX_."
    #elif !HAS_Z4_MIN && !HAS_Z4_MAX
      #error "Z4_USE_ENDSTOP has been assigned to a nonexistent endstop!"
    #endif
  #endif
#endif

#if defined(ENDSTOP_NOISE_THRESHOLD) && !WITHIN(ENDSTOP_NOISE_THRESHOLD, 2, 7)
  #error "ENDSTOP_NOISE_THRESHOLD must be an integer from 2 to 7."
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
#define _RGB_TEST (PINS_EXIST(RGB_LED_R, RGB_LED_G, RGB_LED_B))
#if ENABLED(PRINTER_EVENT_LEDS) && !HAS_COLOR_LEDS
  #error "PRINTER_EVENT_LEDS requires BLINKM, PCA9533, PCA9632, RGB_LED, RGBW_LED or NEOPIXEL_LED."
#elif ENABLED(RGB_LED)
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
#endif
#undef _RGB_TEST

#if DISABLED(NO_COMPILE_TIME_PWM)
  #define _TEST_PWM(P) PWM_PIN(P)
#else
  #define _TEST_PWM(P) 1 // pass
#endif

/**
 * Auto Fan check for PWM pins
 */
#if HAS_AUTO_FAN && EXTRUDER_AUTO_FAN_SPEED != 255
  #define AF_ERR_SUFF "_AUTO_FAN_PIN is not a PWM pin. Set EXTRUDER_AUTO_FAN_SPEED to 255."
  #if HAS_AUTO_FAN_0
    static_assert(_TEST_PWM(E0_AUTO_FAN_PIN), "E0" AF_ERR_SUFF);
  #elif HAS_AUTO_FAN_1
    static_assert(_TEST_PWM(E1_AUTO_FAN_PIN), "E1" AF_ERR_SUFF);
  #elif HAS_AUTO_FAN_2
    static_assert(_TEST_PWM(E2_AUTO_FAN_PIN), "E2" AF_ERR_SUFF);
  #elif HAS_AUTO_FAN_3
    static_assert(_TEST_PWM(E3_AUTO_FAN_PIN), "E3" AF_ERR_SUFF);
  #elif HAS_AUTO_FAN_4
    static_assert(_TEST_PWM(E4_AUTO_FAN_PIN), "E4" AF_ERR_SUFF);
  #elif HAS_AUTO_FAN_5
    static_assert(_TEST_PWM(E5_AUTO_FAN_PIN), "E5" AF_ERR_SUFF);
  #elif HAS_AUTO_FAN_6
    static_assert(_TEST_PWM(E6_AUTO_FAN_PIN), "E6" AF_ERR_SUFF);
  #elif HAS_AUTO_FAN_7
    static_assert(_TEST_PWM(E7_AUTO_FAN_PIN), "E7" AF_ERR_SUFF);
  #endif
#endif

/**
 * Make sure only one EEPROM type is enabled
 */
#if ENABLED(EEPROM_SETTINGS)
  #if 1 < 0 \
    + ENABLED(I2C_EEPROM) \
    + ENABLED(SPI_EEPROM) \
    + ENABLED(QSPI_EEPROM) \
    + ENABLED(SDCARD_EEPROM_EMULATION) \
    + ENABLED(FLASH_EEPROM_EMULATION) \
    + ENABLED(SRAM_EEPROM_EMULATION) \
    + ENABLED(IIC_BL24CXX_EEPROM)
    #error "Please select only one method of EEPROM Persistent Storage."
  #endif
#endif

/**
 * Make sure features that need to write to the SD card are
 * disabled unless write support is enabled.
 */
#if ENABLED(SDCARD_READONLY)
  #if ENABLED(POWER_LOSS_RECOVERY)
    #error "POWER_LOSS_RECOVERY is incompatible with SDCARD_READONLY."
  #elif ENABLED(BINARY_FILE_TRANSFER)
    #error "BINARY_FILE_TRANSFER is incompatible with SDCARD_READONLY."
  #elif ENABLED(SDCARD_EEPROM_EMULATION)
    #error "SDCARD_EEPROM_EMULATION is incompatible with SDCARD_READONLY."
  #endif
#endif

/**
 * Make sure only one display is enabled
 */
#if 1 < 0 \
  + (ENABLED(REPRAP_DISCOUNT_SMART_CONTROLLER) && DISABLED(IS_RRD_SC)) \
  + (ENABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER) && DISABLED(IS_RRD_FG_SC)) \
  + (ENABLED(ULTRA_LCD) && DISABLED(IS_ULTRA_LCD)) \
  + (ENABLED(U8GLIB_SSD1306) && DISABLED(IS_U8GLIB_SSD1306)) \
  + (ENABLED(MINIPANEL) && DISABLED(MKS_MINI_12864, ENDER2_STOCKDISPLAY)) \
  + (ENABLED(MKS_MINI_12864) && DISABLED(MKS_LCD12864)) \
  + (ENABLED(EXTENSIBLE_UI) && DISABLED(IS_EXTUI)) \
  + (ENABLED(ULTIPANEL) && DISABLED(IS_ULTIPANEL)) \
  + ENABLED(RADDS_DISPLAY) \
  + ENABLED(ULTIMAKERCONTROLLER) \
  + ENABLED(PANEL_ONE) \
  + ENABLED(G3D_PANEL) \
  + ENABLED(RIGIDBOT_PANEL) \
  + ENABLED(MAKEBOARD_MINI_2_LINE_DISPLAY_1602) \
  + ENABLED(ZONESTAR_LCD) \
  + ENABLED(RA_CONTROL_PANEL) \
  + ENABLED(LCD_SAINSMART_I2C_1602) \
  + ENABLED(LCD_SAINSMART_I2C_2004) \
  + ENABLED(LCM1602) \
  + ENABLED(LCD_I2C_PANELOLU2) \
  + ENABLED(LCD_I2C_VIKI) \
  + ENABLED(SAV_3DLCD) \
  + ENABLED(FF_INTERFACEBOARD) \
  + ENABLED(REPRAPWORLD_GRAPHICAL_LCD) \
  + ENABLED(VIKI2) \
  + ENABLED(miniVIKI) \
  + ENABLED(MAKRPANEL) \
  + ENABLED(ELB_FULL_GRAPHIC_CONTROLLER) \
  + ENABLED(BQ_LCD_SMART_CONTROLLER) \
  + ENABLED(CARTESIO_UI) \
  + ENABLED(LCD_FOR_MELZI) \
  + ENABLED(ULTI_CONTROLLER) \
  + ENABLED(MKS_LCD12864) \
  + ENABLED(ENDER2_STOCKDISPLAY) \
  + ENABLED(FYSETC_MINI_12864_X_X) \
  + ENABLED(FYSETC_MINI_12864_1_2) \
  + ENABLED(FYSETC_MINI_12864_2_0) \
  + ENABLED(FYSETC_MINI_12864_2_1) \
  + ENABLED(FYSETC_GENERIC_12864_1_1) \
  + ENABLED(CR10_STOCKDISPLAY) \
  + ENABLED(DWIN_CREALITY_LCD) \
  + ENABLED(ANET_FULL_GRAPHICS_LCD) \
  + ENABLED(AZSMZ_12864) \
  + ENABLED(SILVER_GATE_GLCD_CONTROLLER) \
  + ENABLED(SAV_3DGLCD) \
  + ENABLED(OLED_PANEL_TINYBOY2) \
  + ENABLED(MKS_12864OLED) \
  + ENABLED(MKS_12864OLED_SSD1306) \
  + ENABLED(U8GLIB_SH1106_EINSTART) \
  + ENABLED(OVERLORD_OLED) \
  + ENABLED(FYSETC_242_OLED_12864) \
  + ENABLED(DGUS_LCD_UI_ORIGIN) \
  + ENABLED(DGUS_LCD_UI_FYSETC) \
  + ENABLED(DGUS_LCD_UI_HIPRECY) \
  + ENABLED(MALYAN_LCD) \
  + ENABLED(TOUCH_UI_FTDI_EVE) \
  + ENABLED(TFT_320x240) \
  + ENABLED(TFT_320x240_SPI) \
  + ENABLED(FSMC_GRAPHICAL_TFT) \
  + ENABLED(TFT_LVGL_UI_FSMC) \
  + ENABLED(TFT_LVGL_UI_SPI)
  #error "Please select no more than one LCD controller option."
#endif

#undef IS_RRD_SC
#undef IS_RRD_FG_SC
#undef IS_ULTRA_LCD
#undef IS_U8GLIB_SSD1306
#undef IS_RRW_KEYPAD
#undef IS_EXTUI
#undef IS_ULTIPANEL

#if 1 < ENABLED(LCD_SCREEN_ROT_0) + ENABLED(LCD_SCREEN_ROT_90) + ENABLED(LCD_SCREEN_ROT_180) + ENABLED(LCD_SCREEN_ROT_270)
  #error "Please enable only one LCD_SCREEN_ROT_* option: 0, 90, 180, or 270."
#endif

/**
 * FYSETC Mini 12864 RGB backlighting required
 */
#if EITHER(FYSETC_MINI_12864_1_2, FYSETC_MINI_12864_2_0) && DISABLED(RGB_LED)
  #error "RGB_LED is required for FYSETC_MINI_12864 1.2 and 2.0."
#elif EITHER(FYSETC_MINI_12864_2_0, FYSETC_MINI_12864_2_1) && DISABLED(LED_USER_PRESET_STARTUP)
  #error "LED_USER_PRESET_STARTUP is required for FYSETC_MINI_12864 2.x displays."
#endif

/**
 * Check existing CS pins against enabled TMC SPI drivers.
 */
#define INVALID_TMC_SPI(ST) (AXIS_HAS_SPI(ST) && !PIN_EXISTS(ST##_CS))
#if INVALID_TMC_SPI(X)
  #error "An SPI driven TMC driver on X requires X_CS_PIN."
#elif INVALID_TMC_SPI(X2)
  #error "An SPI driven TMC driver on X2 requires X2_CS_PIN."
#elif INVALID_TMC_SPI(Y)
  #error "An SPI driven TMC driver on Y requires Y_CS_PIN."
#elif INVALID_TMC_SPI(Y2)
  #error "An SPI driven TMC driver on Y2 requires Y2_CS_PIN."
#elif INVALID_TMC_SPI(Z)
  #error "An SPI driven TMC driver on Z requires Z_CS_PIN."
#elif INVALID_TMC_SPI(Z2)
  #error "An SPI driven TMC driver on Z2 requires Z2_CS_PIN."
#elif INVALID_TMC_SPI(Z3)
  #error "An SPI driven TMC driver on Z3 requires Z3_CS_PIN."
#elif INVALID_TMC_SPI(Z4)
  #error "An SPI driven TMC driver on Z4 requires Z4_CS_PIN."
#elif INVALID_TMC_SPI(E0)
  #error "An SPI driven TMC driver on E0 requires E0_CS_PIN."
#elif INVALID_TMC_SPI(E1)
  #error "An SPI driven TMC driver on E1 requires E1_CS_PIN."
#elif INVALID_TMC_SPI(E2)
  #error "An SPI driven TMC driver on E2 requires E2_CS_PIN."
#elif INVALID_TMC_SPI(E3)
  #error "An SPI driven TMC driver on E3 requires E3_CS_PIN."
#elif INVALID_TMC_SPI(E4)
  #error "An SPI driven TMC driver on E4 requires E4_CS_PIN."
#elif INVALID_TMC_SPI(E5)
  #error "An SPI driven TMC driver on E5 requires E5_CS_PIN."
#elif INVALID_TMC_SPI(E6)
  #error "An SPI driven TMC driver on E6 requires E6_CS_PIN."
#elif INVALID_TMC_SPI(E7)
  #error "An SPI driven TMC driver on E7 requires E7_CS_PIN."
#endif
#undef INVALID_TMC_SPI

/**
 * Check existing RX/TX pins against enable TMC UART drivers.
 */
#define INVALID_TMC_UART(ST) (AXIS_HAS_UART(ST) && !(defined(ST##_HARDWARE_SERIAL) || (PINS_EXIST(ST##_SERIAL_RX, ST##_SERIAL_TX))))
#if INVALID_TMC_UART(X)
  #error "TMC2208 or TMC2209 on X requires X_HARDWARE_SERIAL or X_SERIAL_(RX|TX)_PIN."
#elif INVALID_TMC_UART(X2)
  #error "TMC2208 or TMC2209 on X2 requires X2_HARDWARE_SERIAL or X2_SERIAL_(RX|TX)_PIN."
#elif INVALID_TMC_UART(Y)
  #error "TMC2208 or TMC2209 on Y requires Y_HARDWARE_SERIAL or Y_SERIAL_(RX|TX)_PIN."
#elif INVALID_TMC_UART(Y2)
  #error "TMC2208 or TMC2209 on Y2 requires Y2_HARDWARE_SERIAL or Y2_SERIAL_(RX|TX)_PIN."
#elif INVALID_TMC_UART(Z)
  #error "TMC2208 or TMC2209 on Z requires Z_HARDWARE_SERIAL or Z_SERIAL_(RX|TX)_PIN."
#elif INVALID_TMC_UART(Z2)
  #error "TMC2208 or TMC2209 on Z2 requires Z2_HARDWARE_SERIAL or Z2_SERIAL_(RX|TX)_PIN."
#elif INVALID_TMC_UART(Z3)
  #error "TMC2208 or TMC2209 on Z3 requires Z3_HARDWARE_SERIAL or Z3_SERIAL_(RX|TX)_PIN."
#elif INVALID_TMC_UART(Z4)
  #error "TMC2208 or TMC2209 on Z4 requires Z4_HARDWARE_SERIAL or Z4_SERIAL_(RX|TX)_PIN."
#elif INVALID_TMC_UART(E0)
  #error "TMC2208 or TMC2209 on E0 requires E0_HARDWARE_SERIAL or E0_SERIAL_(RX|TX)_PIN."
#elif INVALID_TMC_UART(E1)
  #error "TMC2208 or TMC2209 on E1 requires E1_HARDWARE_SERIAL or E1_SERIAL_(RX|TX)_PIN."
#elif INVALID_TMC_UART(E2)
  #error "TMC2208 or TMC2209 on E2 requires E2_HARDWARE_SERIAL or E2_SERIAL_(RX|TX)_PIN."
#elif INVALID_TMC_UART(E3)
  #error "TMC2208 or TMC2209 on E3 requires E3_HARDWARE_SERIAL or E3_SERIAL_(RX|TX)_PIN."
#elif INVALID_TMC_UART(E4)
  #error "TMC2208 or TMC2209 on E4 requires E4_HARDWARE_SERIAL or E4_SERIAL_(RX|TX)_PIN."
#elif INVALID_TMC_UART(E5)
  #error "TMC2208 or TMC2209 on E5 requires E5_HARDWARE_SERIAL or E5_SERIAL_(RX|TX)_PIN."
#elif INVALID_TMC_UART(E6)
  #error "TMC2208 or TMC2209 on E6 requires E6_HARDWARE_SERIAL or E6_SERIAL_(RX|TX)_PIN."
#elif INVALID_TMC_UART(E7)
  #error "TMC2208 or TMC2209 on E7 requires E7_HARDWARE_SERIAL or E7_SERIAL_(RX|TX)_PIN."
#endif
#undef INVALID_TMC_UART

/**
 * TMC2209 slave address values
 */
#define INVALID_TMC_ADDRESS(ST) static_assert(0 <= ST##_SLAVE_ADDRESS && ST##_SLAVE_ADDRESS <= 3, "TMC2209 slave address must be 0, 1, 2 or 3")
#if AXIS_DRIVER_TYPE_X(TMC2209)
  INVALID_TMC_ADDRESS(X);
#elif AXIS_DRIVER_TYPE_X2(TMC2209)
  INVALID_TMC_ADDRESS(X2);
#elif AXIS_DRIVER_TYPE_Y(TMC2209)
  INVALID_TMC_ADDRESS(Y);
#elif AXIS_DRIVER_TYPE_Y2(TMC2209)
  INVALID_TMC_ADDRESS(Y2);
#elif AXIS_DRIVER_TYPE_Z(TMC2209)
  INVALID_TMC_ADDRESS(Z);
#elif AXIS_DRIVER_TYPE_Z2(TMC2209)
  INVALID_TMC_ADDRESS(Z2);
#elif AXIS_DRIVER_TYPE_Z3(TMC2209)
  INVALID_TMC_ADDRESS(Z3);
#elif AXIS_DRIVER_TYPE_Z4(TMC2209)
  INVALID_TMC_ADDRESS(Z4);
#elif AXIS_DRIVER_TYPE_E0(TMC2209)
  INVALID_TMC_ADDRESS(E0);
#elif AXIS_DRIVER_TYPE_E1(TMC2209)
  INVALID_TMC_ADDRESS(E1);
#elif AXIS_DRIVER_TYPE_E2(TMC2209)
  INVALID_TMC_ADDRESS(E2);
#elif AXIS_DRIVER_TYPE_E3(TMC2209)
  INVALID_TMC_ADDRESS(E3);
#elif AXIS_DRIVER_TYPE_E4(TMC2209)
  INVALID_TMC_ADDRESS(E4);
#elif AXIS_DRIVER_TYPE_E5(TMC2209)
  INVALID_TMC_ADDRESS(E5);
#elif AXIS_DRIVER_TYPE_E6(TMC2209)
  INVALID_TMC_ADDRESS(E6);
#elif AXIS_DRIVER_TYPE_E7(TMC2209)
  INVALID_TMC_ADDRESS(E7);
#endif
#undef INVALID_TMC_ADDRESS

#define _TMC_MICROSTEP_IS_VALID(MS) (MS == 0 || MS == 2 || MS == 4 || MS == 8 || MS == 16 || MS == 32 || MS == 64 || MS == 128 || MS == 256)
#define TMC_MICROSTEP_IS_VALID(M) (!AXIS_IS_TMC(M) || _TMC_MICROSTEP_IS_VALID(M##_MICROSTEPS))
#define INVALID_TMC_MS(ST) static_assert(0, "Invalid " STRINGIFY(ST) "_MICROSTEPS. Valid values are 0, 2, 4, 8, 16, 32, 64, 128, and 256.")

#if !TMC_MICROSTEP_IS_VALID(X)
  INVALID_TMC_MS(X);
#elif !TMC_MICROSTEP_IS_VALID(Y)
  INVALID_TMC_MS(Y)
#elif !TMC_MICROSTEP_IS_VALID(Z)
  INVALID_TMC_MS(Z)
#elif !TMC_MICROSTEP_IS_VALID(X2)
  INVALID_TMC_MS(X2);
#elif !TMC_MICROSTEP_IS_VALID(Y2)
  INVALID_TMC_MS(Y2)
#elif !TMC_MICROSTEP_IS_VALID(Z2)
  INVALID_TMC_MS(Z2)
#elif !TMC_MICROSTEP_IS_VALID(Z3)
  INVALID_TMC_MS(Z3)
#elif !TMC_MICROSTEP_IS_VALID(Z4)
  INVALID_TMC_MS(Z4)
#elif !TMC_MICROSTEP_IS_VALID(E0)
  INVALID_TMC_MS(E0)
#elif !TMC_MICROSTEP_IS_VALID(E1)
  INVALID_TMC_MS(E1)
#elif !TMC_MICROSTEP_IS_VALID(E2)
  INVALID_TMC_MS(E2)
#elif !TMC_MICROSTEP_IS_VALID(E3)
  INVALID_TMC_MS(E3)
#elif !TMC_MICROSTEP_IS_VALID(E4)
  INVALID_TMC_MS(E4)
#elif !TMC_MICROSTEP_IS_VALID(E5)
  INVALID_TMC_MS(E5)
#elif !TMC_MICROSTEP_IS_VALID(E6)
  INVALID_TMC_MS(E6)
#elif !TMC_MICROSTEP_IS_VALID(E7)
  INVALID_TMC_MS(E7)
#endif
#undef INVALID_TMC_MS
#undef TMC_MICROSTEP_IS_VALID
#undef _TMC_MICROSTEP_IS_VALID

#if ENABLED(DELTA) && (ENABLED(STEALTHCHOP_XY) != ENABLED(STEALTHCHOP_Z))
  #error "STEALTHCHOP_XY and STEALTHCHOP_Z must be the same on DELTA."
#endif

#if ENABLED(SENSORLESS_HOMING)
  // Require STEALTHCHOP for SENSORLESS_HOMING on DELTA as the transition from spreadCycle to stealthChop
  // is necessary in order to reset the stallGuard indication between the initial movement of all three
  // towers to +Z and the individual homing of each tower. This restriction can be removed once a means of
  // clearing the stallGuard activated status is found.

  // Stall detection DIAG = HIGH : TMC2209
  // Stall detection DIAG = LOW  : TMC2130/TMC2160/TMC2660/TMC5130/TMC5160
  #define X_ENDSTOP_INVERTING !AXIS_DRIVER_TYPE(X,TMC2209)
  #define Y_ENDSTOP_INVERTING !AXIS_DRIVER_TYPE(Y,TMC2209)
  #define Z_ENDSTOP_INVERTING !AXIS_DRIVER_TYPE(Z,TMC2209)

  #if ENABLED(DELTA) && !BOTH(STEALTHCHOP_XY, STEALTHCHOP_Z)
    #error "SENSORLESS_HOMING on DELTA currently requires STEALTHCHOP_XY and STEALTHCHOP_Z."
  #elif X_SENSORLESS && X_HOME_DIR < 0 && NONE(ONBOARD_ENDSTOPPULLUPS, ENDSTOPPULLUPS, ENDSTOPPULLUP_XMIN)
    #error "SENSORLESS_HOMING requires ENDSTOPPULLUP_XMIN (or ENDSTOPPULLUPS) when homing to X_MIN."
  #elif X_SENSORLESS && X_HOME_DIR > 0 && NONE(ONBOARD_ENDSTOPPULLUPS, ENDSTOPPULLUPS, ENDSTOPPULLUP_XMAX)
    #error "SENSORLESS_HOMING requires ENDSTOPPULLUP_XMAX (or ENDSTOPPULLUPS) when homing to X_MAX."
  #elif Y_SENSORLESS && Y_HOME_DIR < 0 && NONE(ONBOARD_ENDSTOPPULLUPS, ENDSTOPPULLUPS, ENDSTOPPULLUP_YMIN)
    #error "SENSORLESS_HOMING requires ENDSTOPPULLUP_YMIN (or ENDSTOPPULLUPS) when homing to Y_MIN."
  #elif Y_SENSORLESS && Y_HOME_DIR > 0 && NONE(ONBOARD_ENDSTOPPULLUPS, ENDSTOPPULLUPS, ENDSTOPPULLUP_YMAX)
    #error "SENSORLESS_HOMING requires ENDSTOPPULLUP_YMAX (or ENDSTOPPULLUPS) when homing to Y_MAX."
  #elif Z_SENSORLESS && Z_HOME_DIR < 0 && NONE(ONBOARD_ENDSTOPPULLUPS, ENDSTOPPULLUPS, ENDSTOPPULLUP_ZMIN)
    #error "SENSORLESS_HOMING requires ENDSTOPPULLUP_ZMIN (or ENDSTOPPULLUPS) when homing to Z_MIN."
  #elif Z_SENSORLESS && Z_HOME_DIR > 0 && NONE(ONBOARD_ENDSTOPPULLUPS, ENDSTOPPULLUPS, ENDSTOPPULLUP_ZMAX)
    #error "SENSORLESS_HOMING requires ENDSTOPPULLUP_ZMAX (or ENDSTOPPULLUPS) when homing to Z_MAX."
  #elif X_SENSORLESS && X_HOME_DIR < 0 && X_MIN_ENDSTOP_INVERTING != X_ENDSTOP_INVERTING
    #if X_ENDSTOP_INVERTING
      #error "SENSORLESS_HOMING requires X_MIN_ENDSTOP_INVERTING = true when homing to X_MIN."
    #else
      #error "SENSORLESS_HOMING requires X_MIN_ENDSTOP_INVERTING = false when homing TMC2209 to X_MIN."
    #endif
  #elif X_SENSORLESS && X_HOME_DIR > 0 && X_MAX_ENDSTOP_INVERTING != X_ENDSTOP_INVERTING
    #if X_ENDSTOP_INVERTING
      #error "SENSORLESS_HOMING requires X_MAX_ENDSTOP_INVERTING = true when homing to X_MAX."
    #else
      #error "SENSORLESS_HOMING requires X_MAX_ENDSTOP_INVERTING = false when homing TMC2209 to X_MAX."
    #endif
  #elif Y_SENSORLESS && Y_HOME_DIR < 0 && Y_MIN_ENDSTOP_INVERTING != Y_ENDSTOP_INVERTING
    #if Y_ENDSTOP_INVERTING
      #error "SENSORLESS_HOMING requires Y_MIN_ENDSTOP_INVERTING = true when homing to Y_MIN."
    #else
      #error "SENSORLESS_HOMING requires Y_MIN_ENDSTOP_INVERTING = false when homing TMC2209 to Y_MIN."
    #endif
  #elif Y_SENSORLESS && Y_HOME_DIR > 0 && Y_MAX_ENDSTOP_INVERTING != Y_ENDSTOP_INVERTING
    #if Y_ENDSTOP_INVERTING
      #error "SENSORLESS_HOMING requires Y_MAX_ENDSTOP_INVERTING = true when homing to Y_MAX."
    #else
      #error "SENSORLESS_HOMING requires Y_MAX_ENDSTOP_INVERTING = false when homing TMC2209 to Y_MAX."
    #endif
  #elif Z_SENSORLESS && Z_HOME_DIR < 0 && Z_MIN_ENDSTOP_INVERTING != Z_ENDSTOP_INVERTING
    #if Z_ENDSTOP_INVERTING
      #error "SENSORLESS_HOMING requires Z_MIN_ENDSTOP_INVERTING = true when homing to Z_MIN."
    #else
      #error "SENSORLESS_HOMING requires Z_MIN_ENDSTOP_INVERTING = false when homing TMC2209 to Z_MIN."
    #endif
  #elif Z_SENSORLESS && Z_HOME_DIR > 0 && Z_MAX_ENDSTOP_INVERTING != Z_ENDSTOP_INVERTING
    #if Z_ENDSTOP_INVERTING
      #error "SENSORLESS_HOMING requires Z_MAX_ENDSTOP_INVERTING = true when homing to Z_MAX."
    #else
      #error "SENSORLESS_HOMING requires Z_MAX_ENDSTOP_INVERTING = false when homing TMC2209 to Z_MAX."
    #endif
  #elif ENDSTOP_NOISE_THRESHOLD
    #error "SENSORLESS_HOMING is incompatible with ENDSTOP_NOISE_THRESHOLD."
  #elif !(X_SENSORLESS || Y_SENSORLESS || Z_SENSORLESS)
    #error "SENSORLESS_HOMING requires a TMC stepper driver with StallGuard on X, Y, or Z axes."
  #endif

  #undef X_ENDSTOP_INVERTING
  #undef Y_ENDSTOP_INVERTING
  #undef Z_ENDSTOP_INVERTING
#endif

// Sensorless probing requirements
#if ENABLED(SENSORLESS_PROBING)
  #if ENABLED(DELTA) && !(X_SENSORLESS && Y_SENSORLESS && Z_SENSORLESS)
    #error "SENSORLESS_PROBING for DELTA requires TMC stepper drivers with StallGuard on X, Y, and Z axes."
  #elif !Z_SENSORLESS
    #error "SENSORLESS_PROBING requires a TMC stepper driver with StallGuard on Z."
  #endif
#endif

// Sensorless homing is required for both combined steppers in an H-bot
#if CORE_IS_XY && X_SENSORLESS != Y_SENSORLESS
  #error "CoreXY requires both X and Y to use sensorless homing if either one does."
#elif CORE_IS_XZ && X_SENSORLESS != Z_SENSORLESS && !HOMING_Z_WITH_PROBE
  #error "CoreXZ requires both X and Z to use sensorless homing if either one does."
#elif CORE_IS_YZ && Y_SENSORLESS != Z_SENSORLESS && !HOMING_Z_WITH_PROBE
  #error "CoreYZ requires both Y and Z to use sensorless homing if either one does."
#endif

// Other TMC feature requirements
#if ENABLED(HYBRID_THRESHOLD) && !STEALTHCHOP_ENABLED
  #error "Enable STEALTHCHOP_(XY|Z|E) to use HYBRID_THRESHOLD."
#elif ENABLED(SENSORLESS_HOMING) && !HAS_STALLGUARD
  #error "SENSORLESS_HOMING requires TMC2130, TMC2160, TMC2209, TMC2660, or TMC5160 stepper drivers."
#elif ENABLED(SENSORLESS_PROBING) && !HAS_STALLGUARD
  #error "SENSORLESS_PROBING requires TMC2130, TMC2160, TMC2209, TMC2660, or TMC5160 stepper drivers."
#elif STEALTHCHOP_ENABLED && !HAS_STEALTHCHOP
  #error "STEALTHCHOP requires TMC2130, TMC2160, TMC2208, TMC2209, or TMC5160 stepper drivers."
#endif

/**
 * TMC SPI Chaining
 */
#define IN_CHAIN(A) ((A##_CHAIN_POS > 0) && !HAS_L64XX)
#if  IN_CHAIN(X ) || IN_CHAIN(Y ) || IN_CHAIN(Z ) || IN_CHAIN(X2) || IN_CHAIN(Y2) || IN_CHAIN(Z2) || IN_CHAIN(Z3) || IN_CHAIN(Z4) \
  || IN_CHAIN(E0) || IN_CHAIN(E1) || IN_CHAIN(E2) || IN_CHAIN(E3) || IN_CHAIN(E4) || IN_CHAIN(E5) || IN_CHAIN(E6) || IN_CHAIN(E7)
  #define BAD_CHAIN(A) (IN_CHAIN(A) && !PIN_EXISTS(A##_CS))
  #if  BAD_CHAIN(X ) || BAD_CHAIN(Y ) || BAD_CHAIN(Z ) || BAD_CHAIN(X2) || BAD_CHAIN(Y2) || BAD_CHAIN(Z2) || BAD_CHAIN(Z3) || BAD_CHAIN(Z4) \
    || BAD_CHAIN(E0) || BAD_CHAIN(E1) || BAD_CHAIN(E2) || BAD_CHAIN(E3) || BAD_CHAIN(E4) || BAD_CHAIN(E5) || BAD_CHAIN(E6) || BAD_CHAIN(E7)
    #error "All chained TMC drivers need a CS pin."
  #else
    #if IN_CHAIN(X)
      #define CS_COMPARE X_CS_PIN
    #elif IN_CHAIN(Y)
      #define CS_COMPARE Y_CS_PIN
    #elif IN_CHAIN(Z)
      #define CS_COMPARE Z_CS_PIN
    #elif IN_CHAIN(X2)
      #define CS_COMPARE X2_CS_PIN
    #elif IN_CHAIN(Y2)
      #define CS_COMPARE Y2_CS_PIN
    #elif IN_CHAIN(Z2)
      #define CS_COMPARE Z2_CS_PIN
    #elif IN_CHAIN(Z3)
      #define CS_COMPARE Z3_CS_PIN
    #elif IN_CHAIN(E0)
      #define CS_COMPARE E0_CS_PIN
    #elif IN_CHAIN(E1)
      #define CS_COMPARE E1_CS_PIN
    #elif IN_CHAIN(E2)
      #define CS_COMPARE E2_CS_PIN
    #elif IN_CHAIN(E3)
      #define CS_COMPARE E3_CS_PIN
    #elif IN_CHAIN(E4)
      #define CS_COMPARE E4_CS_PIN
    #elif IN_CHAIN(E5)
      #define CS_COMPARE E5_CS_PIN
    #elif IN_CHAIN(E6)
      #define CS_COMPARE E6_CS_PIN
    #elif IN_CHAIN(E7)
      #define CS_COMPARE E7_CS_PIN
    #endif
    #define BAD_CS_PIN(A) (IN_CHAIN(A) && A##_CS_PIN != CS_COMPARE)
    #if  BAD_CS_PIN(X ) || BAD_CS_PIN(Y ) || BAD_CS_PIN(Z ) || BAD_CS_PIN(X2) || BAD_CS_PIN(Y2) || BAD_CS_PIN(Z2) || BAD_CS_PIN(Z3) || BAD_CS_PIN(Z4) \
      || BAD_CS_PIN(E0) || BAD_CS_PIN(E1) || BAD_CS_PIN(E2) || BAD_CS_PIN(E3) || BAD_CS_PIN(E4) || BAD_CS_PIN(E5) || BAD_CS_PIN(E6) || BAD_CS_PIN(E7)
      #error "All chained TMC drivers must use the same CS pin."
    #endif
    #undef BAD_CS_PIN
    #undef CS_COMPARE
  #endif
  #undef BAD_CHAIN
#endif
#undef IN_CHAIN

/**
 * Digipot requirement
 */
#if HAS_I2C_DIGIPOT
  #if BOTH(DIGIPOT_MCP4018, DIGIPOT_MCP4451)
    #error "Enable only one of DIGIPOT_MCP4018 or DIGIPOT_MCP4451."
  #elif !MB(MKS_SBASE) \
    && (!defined(DIGIPOTS_I2C_SDA_X) || !defined(DIGIPOTS_I2C_SDA_Y) || !defined(DIGIPOTS_I2C_SDA_Z) || !defined(DIGIPOTS_I2C_SDA_E0) || !defined(DIGIPOTS_I2C_SDA_E1))
      #error "DIGIPOT_MCP4018/4451 requires DIGIPOTS_I2C_SDA_* pins to be defined."
  #endif
#endif

/**
 * Check per-axis initializers for errors
 */
constexpr float sanity_arr_1[] = DEFAULT_AXIS_STEPS_PER_UNIT,
                sanity_arr_2[] = DEFAULT_MAX_FEEDRATE,
                sanity_arr_3[] = DEFAULT_MAX_ACCELERATION;

#define _ARR_TEST(N,I) (sanity_arr_##N[_MIN(I,int(COUNT(sanity_arr_##N))-1)] > 0)

static_assert(COUNT(sanity_arr_1) >= XYZE,   "DEFAULT_AXIS_STEPS_PER_UNIT requires X, Y, Z and E elements.");
static_assert(COUNT(sanity_arr_1) <= XYZE_N, "DEFAULT_AXIS_STEPS_PER_UNIT has too many elements. (Did you forget to enable DISTINCT_E_FACTORS?)");
static_assert(   _ARR_TEST(1,0) && _ARR_TEST(1,1) && _ARR_TEST(1,2)
              && _ARR_TEST(1,3) && _ARR_TEST(1,4) && _ARR_TEST(1,5)
              && _ARR_TEST(1,6) && _ARR_TEST(1,7) && _ARR_TEST(1,8),
              "DEFAULT_AXIS_STEPS_PER_UNIT values must be positive.");

static_assert(COUNT(sanity_arr_2) >= XYZE,   "DEFAULT_MAX_FEEDRATE requires X, Y, Z and E elements.");
static_assert(COUNT(sanity_arr_2) <= XYZE_N, "DEFAULT_MAX_FEEDRATE has too many elements. (Did you forget to enable DISTINCT_E_FACTORS?)");
static_assert(   _ARR_TEST(2,0) && _ARR_TEST(2,1) && _ARR_TEST(2,2)
              && _ARR_TEST(2,3) && _ARR_TEST(2,4) && _ARR_TEST(2,5)
              && _ARR_TEST(2,6) && _ARR_TEST(2,7) && _ARR_TEST(2,8),
              "DEFAULT_MAX_FEEDRATE values must be positive.");

static_assert(COUNT(sanity_arr_3) >= XYZE,   "DEFAULT_MAX_ACCELERATION requires X, Y, Z and E elements.");
static_assert(COUNT(sanity_arr_3) <= XYZE_N, "DEFAULT_MAX_ACCELERATION has too many elements. (Did you forget to enable DISTINCT_E_FACTORS?)");
static_assert(   _ARR_TEST(3,0) && _ARR_TEST(3,1) && _ARR_TEST(3,2)
              && _ARR_TEST(3,3) && _ARR_TEST(3,4) && _ARR_TEST(3,5)
              && _ARR_TEST(3,6) && _ARR_TEST(3,7) && _ARR_TEST(3,8),
              "DEFAULT_MAX_ACCELERATION values must be positive.");

#if ENABLED(LIMITED_MAX_ACCEL_EDITING)
  #ifdef MAX_ACCEL_EDIT_VALUES
    constexpr float sanity_arr_4[] = MAX_ACCEL_EDIT_VALUES;
    static_assert(COUNT(sanity_arr_4) >= XYZE, "MAX_ACCEL_EDIT_VALUES requires X, Y, Z and E elements.");
    static_assert(COUNT(sanity_arr_4) <= XYZE, "MAX_ACCEL_EDIT_VALUES has too many elements. X, Y, Z and E elements only.");
    static_assert(   _ARR_TEST(4,0) && _ARR_TEST(4,1) && _ARR_TEST(4,2)
                  && _ARR_TEST(4,3) && _ARR_TEST(4,4) && _ARR_TEST(4,5)
                  && _ARR_TEST(4,6) && _ARR_TEST(4,7) && _ARR_TEST(4,8),
                  "MAX_ACCEL_EDIT_VALUES values must be positive.");
  #endif
#endif

#if ENABLED(LIMITED_MAX_FR_EDITING)
  #ifdef MAX_FEEDRATE_EDIT_VALUES
    constexpr float sanity_arr_5[] = MAX_FEEDRATE_EDIT_VALUES;
    static_assert(COUNT(sanity_arr_5) >= XYZE, "MAX_FEEDRATE_EDIT_VALUES requires X, Y, Z and E elements.");
    static_assert(COUNT(sanity_arr_5) <= XYZE, "MAX_FEEDRATE_EDIT_VALUES has too many elements. X, Y, Z and E elements only.");
    static_assert(   _ARR_TEST(5,0) && _ARR_TEST(5,1) && _ARR_TEST(5,2)
                  && _ARR_TEST(5,3) && _ARR_TEST(5,4) && _ARR_TEST(5,5)
                  && _ARR_TEST(5,6) && _ARR_TEST(5,7) && _ARR_TEST(5,8),
                  "MAX_FEEDRATE_EDIT_VALUES values must be positive.");
  #endif
#endif

#if ENABLED(LIMITED_JERK_EDITING)
  #ifdef MAX_JERK_EDIT_VALUES
    constexpr float sanity_arr_6[] = MAX_JERK_EDIT_VALUES;
    static_assert(COUNT(sanity_arr_6) >= XYZE, "MAX_JERK_EDIT_VALUES requires X, Y, Z and E elements.");
    static_assert(COUNT(sanity_arr_6) <= XYZE, "MAX_JERK_EDIT_VALUES has too many elements. X, Y, Z and E elements only.");
    static_assert(   _ARR_TEST(6,0) && _ARR_TEST(6,1) && _ARR_TEST(6,2)
                  && _ARR_TEST(6,3) && _ARR_TEST(6,4) && _ARR_TEST(6,5)
                  && _ARR_TEST(6,6) && _ARR_TEST(6,7) && _ARR_TEST(6,8),
                  "MAX_JERK_EDIT_VALUES values must be positive.");
  #endif
#endif

#undef _ARR_TEST

#if BOTH(CNC_COORDINATE_SYSTEMS, NO_WORKSPACE_OFFSETS)
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

#if ENABLED(BACKUP_POWER_SUPPLY) && !PIN_EXISTS(POWER_LOSS)
  #error "BACKUP_POWER_SUPPLY requires a POWER_LOSS_PIN."
#endif

#if ENABLED(Z_STEPPER_AUTO_ALIGN)
  #if NUM_Z_STEPPER_DRIVERS <= 1
    #error "Z_STEPPER_AUTO_ALIGN requires NUM_Z_STEPPER_DRIVERS greater than 1."
  #elif !HAS_BED_PROBE
    #error "Z_STEPPER_AUTO_ALIGN requires a Z-bed probe."
  #elif ENABLED(Z_STEPPER_ALIGN_KNOWN_STEPPER_POSITIONS) && NUM_Z_STEPPER_DRIVERS < 3
    #error "Z_STEPPER_ALIGN_KNOWN_STEPPER_POSITIONS requires NUM_Z_STEPPER_DRIVERS to be 3 or 4."
  #endif
#endif

#if ENABLED(PRINTCOUNTER) && DISABLED(EEPROM_SETTINGS)
  #error "PRINTCOUNTER requires EEPROM_SETTINGS. Please update your Configuration."
#endif

#if ENABLED(USB_FLASH_DRIVE_SUPPORT) && !PINS_EXIST(USB_CS, USB_INTR)
  #error "USB_CS_PIN and USB_INTR_PIN are required for USB_FLASH_DRIVE_SUPPORT."
#endif

#if ENABLED(SD_FIRMWARE_UPDATE) && !defined(__AVR_ATmega2560__)
  #error "SD_FIRMWARE_UPDATE requires an ATmega2560-based (Arduino Mega) board."
#endif

#if ENABLED(GCODE_MACROS) && !WITHIN(GCODE_MACROS_SLOTS, 1, 10)
  #error "GCODE_MACROS_SLOTS must be a number from 1 to 10."
#endif

#if ENABLED(CUSTOM_USER_MENUS)
  #ifdef USER_GCODE_1
    constexpr char _chr1 = USER_GCODE_1[strlen(USER_GCODE_1) - 1];
    static_assert(_chr1 != '\n' && _chr1 != '\r', "USER_GCODE_1 cannot have a newline at the end. Please remove it.");
  #endif
  #ifdef USER_GCODE_2
    constexpr char _chr2 = USER_GCODE_2[strlen(USER_GCODE_2) - 1];
    static_assert(_chr2 != '\n' && _chr2 != '\r', "USER_GCODE_2 cannot have a newline at the end. Please remove it.");
  #endif
  #ifdef USER_GCODE_3
    constexpr char _chr3 = USER_GCODE_3[strlen(USER_GCODE_3) - 1];
    static_assert(_chr3 != '\n' && _chr3 != '\r', "USER_GCODE_3 cannot have a newline at the end. Please remove it.");
  #endif
  #ifdef USER_GCODE_4
    constexpr char _chr4 = USER_GCODE_4[strlen(USER_GCODE_4) - 1];
    static_assert(_chr4 != '\n' && _chr4 != '\r', "USER_GCODE_4 cannot have a newline at the end. Please remove it.");
  #endif
  #ifdef USER_GCODE_5
    constexpr char _chr5 = USER_GCODE_5[strlen(USER_GCODE_5) - 1];
    static_assert(_chr5 != '\n' && _chr5 != '\r', "USER_GCODE_5 cannot have a newline at the end. Please remove it.");
  #endif
  #ifdef USER_GCODE_6
    constexpr char _chr6 = USER_GCODE_6[strlen(USER_GCODE_6) - 1];
    static_assert(_chr6 != '\n' && _chr6 != '\r', "USER_GCODE_6 cannot have a newline at the end. Please remove it.");
  #endif
  #ifdef USER_GCODE_7
    constexpr char _chr7 = USER_GCODE_7[strlen(USER_GCODE_7) - 1];
    static_assert(_chr7 != '\n' && _chr7 != '\r', "USER_GCODE_7 cannot have a newline at the end. Please remove it.");
  #endif
  #ifdef USER_GCODE_8
    constexpr char _chr8 = USER_GCODE_8[strlen(USER_GCODE_8) - 1];
    static_assert(_chr8 != '\n' && _chr8 != '\r', "USER_GCODE_8 cannot have a newline at the end. Please remove it.");
  #endif
  #ifdef USER_GCODE_9
    constexpr char _chr9 = USER_GCODE_9[strlen(USER_GCODE_9) - 1];
    static_assert(_chr9 != '\n' && _chr9 != '\r', "USER_GCODE_9 cannot have a newline at the end. Please remove it.");
  #endif
  #ifdef USER_GCODE_10
    constexpr char _chr10 = USER_GCODE_10[strlen(USER_GCODE_10) - 1];
    static_assert(_chr10 != '\n' && _chr10 != '\r', "USER_GCODE_10 cannot have a newline at the end. Please remove it.");
  #endif
  #ifdef USER_GCODE_11
    constexpr char _chr11 = USER_GCODE_11[strlen(USER_GCODE_11) - 1];
    static_assert(_chr11 != '\n' && _chr11 != '\r', "USER_GCODE_11 cannot have a newline at the end. Please remove it.");
  #endif
  #ifdef USER_GCODE_12
    constexpr char _chr12 = USER_GCODE_12[strlen(USER_GCODE_12) - 1];
    static_assert(_chr12 != '\n' && _chr12 != '\r', "USER_GCODE_12 cannot have a newline at the end. Please remove it.");
  #endif
  #ifdef USER_GCODE_13
    constexpr char _chr13 = USER_GCODE_13[strlen(USER_GCODE_13) - 1];
    static_assert(_chr13 != '\n' && _chr13 != '\r', "USER_GCODE_13 cannot have a newline at the end. Please remove it.");
  #endif
  #ifdef USER_GCODE_14
    constexpr char _chr14 = USER_GCODE_14[strlen(USER_GCODE_14) - 1];
    static_assert(_chr14 != '\n' && _chr14 != '\r', "USER_GCODE_14 cannot have a newline at the end. Please remove it.");
  #endif
  #ifdef USER_GCODE_15
    constexpr char _chr15 = USER_GCODE_15[strlen(USER_GCODE_15) - 1];
    static_assert(_chr15 != '\n' && _chr15 != '\r', "USER_GCODE_15 cannot have a newline at the end. Please remove it.");
  #endif
  #ifdef USER_GCODE_16
    constexpr char _chr16 = USER_GCODE_16[strlen(USER_GCODE_16) - 1];
    static_assert(_chr16 != '\n' && _chr16 != '\r', "USER_GCODE_16 cannot have a newline at the end. Please remove it.");
  #endif
  #ifdef USER_GCODE_17
    constexpr char _chr17 = USER_GCODE_17[strlen(USER_GCODE_17) - 1];
    static_assert(_chr17 != '\n' && _chr17 != '\r', "USER_GCODE_17 cannot have a newline at the end. Please remove it.");
  #endif
  #ifdef USER_GCODE_18
    constexpr char _chr18 = USER_GCODE_18[strlen(USER_GCODE_18) - 1];
    static_assert(_chr18 != '\n' && _chr18 != '\r', "USER_GCODE_18 cannot have a newline at the end. Please remove it.");
  #endif
  #ifdef USER_GCODE_19
    constexpr char _chr19 = USER_GCODE_19[strlen(USER_GCODE_19) - 1];
    static_assert(_chr19 != '\n' && _chr19 != '\r', "USER_GCODE_19 cannot have a newline at the end. Please remove it.");
  #endif
  #ifdef USER_GCODE_20
    constexpr char _chr20 = USER_GCODE_20[strlen(USER_GCODE_20) - 1];
    static_assert(_chr20 != '\n' && _chr20 != '\r', "USER_GCODE_20 cannot have a newline at the end. Please remove it.");
  #endif
  #ifdef USER_GCODE_21
    constexpr char _chr21 = USER_GCODE_21[strlen(USER_GCODE_21) - 1];
    static_assert(_chr21 != '\n' && _chr21 != '\r', "USER_GCODE_21 cannot have a newline at the end. Please remove it.");
  #endif
  #ifdef USER_GCODE_22
    constexpr char _chr22 = USER_GCODE_22[strlen(USER_GCODE_22) - 1];
    static_assert(_chr22 != '\n' && _chr22 != '\r', "USER_GCODE_22 cannot have a newline at the end. Please remove it.");
  #endif
  #ifdef USER_GCODE_23
    constexpr char _chr23 = USER_GCODE_23[strlen(USER_GCODE_23) - 1];
    static_assert(_chr23 != '\n' && _chr23 != '\r', "USER_GCODE_23 cannot have a newline at the end. Please remove it.");
  #endif
  #ifdef USER_GCODE_24
    constexpr char _chr24 = USER_GCODE_24[strlen(USER_GCODE_24) - 1];
    static_assert(_chr24 != '\n' && _chr24 != '\r', "USER_GCODE_24 cannot have a newline at the end. Please remove it.");
  #endif
  #ifdef USER_GCODE_25
    constexpr char _chr25 = USER_GCODE_25[strlen(USER_GCODE_25) - 1];
    static_assert(_chr25 != '\n' && _chr25 != '\r', "USER_GCODE_25 cannot have a newline at the end. Please remove it.");
  #endif
#endif

#if ENABLED(BACKLASH_COMPENSATION)
  #ifndef BACKLASH_DISTANCE_MM
    #error "BACKLASH_COMPENSATION requires BACKLASH_DISTANCE_MM."
  #elif !defined(BACKLASH_CORRECTION)
    #error "BACKLASH_COMPENSATION requires BACKLASH_CORRECTION."
  #elif IS_CORE
    constexpr float backlash_arr[] = BACKLASH_DISTANCE_MM;
    static_assert(!backlash_arr[CORE_AXIS_1] && !backlash_arr[CORE_AXIS_2],
                  "BACKLASH_COMPENSATION can only apply to " STRINGIFY(NORMAL_AXIS) " with your CORE system.");
  #endif
#endif

#if ENABLED(GRADIENT_MIX) && MIXING_VIRTUAL_TOOLS < 2
  #error "GRADIENT_MIX requires 2 or more MIXING_VIRTUAL_TOOLS."
#endif

/**
 * Photo G-code requirements
 */
#if ENABLED(PHOTO_GCODE)
  #if (PIN_EXISTS(CHDK) + PIN_EXISTS(PHOTOGRAPH) + defined(PHOTO_SWITCH_POSITION)) > 1
    #error "Please define only one of CHDK_PIN, PHOTOGRAPH_PIN, or PHOTO_SWITCH_POSITION."
  #elif defined(PHOTO_SWITCH_POSITION) && !defined(PHOTO_POSITION)
    #error "PHOTO_SWITCH_POSITION requires PHOTO_POSITION. Please update your Configuration_adv.h."
  #elif PIN_EXISTS(CHDK) && defined(CHDK_DELAY)
    #error "CHDK_DELAY has been replaced by PHOTO_SWITCH_MS. Please update your Configuration_adv.h."
  #elif PIN_EXISTS(CHDK) && !defined(PHOTO_SWITCH_MS)
    #error "PHOTO_SWITCH_MS is required with CHDK_PIN. Please update your Configuration_adv.h."
  #elif defined(PHOTO_RETRACT_MM)
    static_assert(PHOTO_RETRACT_MM + 0 >= 0, "PHOTO_RETRACT_MM must be >= 0.");
  #endif
#endif

/**
 * Prusa MMU2 requirements
 */
#if ENABLED(PRUSA_MMU2)
  #if EXTRUDERS != 5
    #error "PRUSA_MMU2 requires EXTRUDERS = 5."
  #elif DISABLED(NOZZLE_PARK_FEATURE)
    #error "PRUSA_MMU2 requires NOZZLE_PARK_FEATURE. Enable it to continue."
  #elif EITHER(PRUSA_MMU2_S_MODE, MMU_EXTRUDER_SENSOR) && DISABLED(FILAMENT_RUNOUT_SENSOR)
    #error "PRUSA_MMU2_S_MODE or MMU_EXTRUDER_SENSOR requires FILAMENT_RUNOUT_SENSOR. Enable it to continue."
  #elif BOTH(PRUSA_MMU2_S_MODE, MMU_EXTRUDER_SENSOR)
    #error "Enable only one of PRUSA_MMU2_S_MODE or MMU_EXTRUDER_SENSOR."
  #elif DISABLED(ADVANCED_PAUSE_FEATURE)
    static_assert(nullptr == strstr(MMU2_FILAMENT_RUNOUT_SCRIPT, "M600"), "ADVANCED_PAUSE_FEATURE is required to use M600 with PRUSA_MMU2.");
  #endif
#endif

/**
 * Advanced PRINTCOUNTER settings
 */
#if ENABLED(PRINTCOUNTER)
  #if defined(SERVICE_INTERVAL_1) != defined(SERVICE_NAME_1)
    #error "Both SERVICE_NAME_1 and SERVICE_INTERVAL_1 are required."
  #elif defined(SERVICE_INTERVAL_2) != defined(SERVICE_NAME_2)
    #error "Both SERVICE_NAME_2 and SERVICE_INTERVAL_2 are required."
  #elif defined(SERVICE_INTERVAL_3) != defined(SERVICE_NAME_3)
    #error "Both SERVICE_NAME_3 and SERVICE_INTERVAL_3 are required."
  #endif
#endif

/**
 * Require soft endstops for certain setups
 */
#if !BOTH(MIN_SOFTWARE_ENDSTOPS, MAX_SOFTWARE_ENDSTOPS)
  #if ENABLED(DUAL_X_CARRIAGE)
    #error "DUAL_X_CARRIAGE requires both MIN_ and MAX_SOFTWARE_ENDSTOPS."
  #elif HAS_HOTEND_OFFSET
    #error "MIN_ and MAX_SOFTWARE_ENDSTOPS are both required with offset hotends."
  #endif
#endif

/**
 * Ensure this option is set intentionally
 */
#if ENABLED(PSU_CONTROL)
  #ifndef PSU_ACTIVE_HIGH
    #error "PSU_CONTROL requires PSU_ACTIVE_HIGH to be defined as 'true' or 'false'."
  #elif !PIN_EXISTS(PS_ON)
    #error "PSU_CONTROL requires PS_ON_PIN."
  #endif
#elif ENABLED(AUTO_POWER_CONTROL)
  #error "AUTO_POWER_CONTROL requires PSU_CONTROL."
#endif

#if HAS_CUTTER
  #ifndef CUTTER_POWER_UNIT
    #error "CUTTER_POWER_UNIT is required with a spindle or laser. Please update your Configuration_adv.h."
  #elif !CUTTER_UNIT_IS(PWM255) && !CUTTER_UNIT_IS(PERCENT) && !CUTTER_UNIT_IS(RPM)
    #error "CUTTER_POWER_UNIT must be PWM255, PERCENT, or RPM. Please update your Configuration_adv.h."
  #endif

  #if ENABLED(LASER_POWER_INLINE)
    #if ENABLED(SPINDLE_CHANGE_DIR)
      #error "SPINDLE_CHANGE_DIR and LASER_POWER_INLINE are incompatible."
    #elif ENABLED(LASER_MOVE_G0_OFF) && DISABLED(LASER_MOVE_POWER)
      #error "LASER_MOVE_G0_OFF requires LASER_MOVE_POWER. Please update your Configuration_adv.h."
    #endif
    #if ENABLED(LASER_POWER_INLINE_TRAPEZOID)
      #if DISABLED(SPINDLE_LASER_PWM)
        #error "LASER_POWER_INLINE_TRAPEZOID requires SPINDLE_LASER_PWM to function."
      #elif ENABLED(S_CURVE_ACCELERATION)
        //#ifndef LASER_POWER_INLINE_S_CURVE_ACCELERATION_WARN
        //  #define LASER_POWER_INLINE_S_CURVE_ACCELERATION_WARN
        //  #warning "Combining LASER_POWER_INLINE_TRAPEZOID with S_CURVE_ACCELERATION may result in unintended behavior."
        //#endif
      #endif
    #endif
    #if ENABLED(LASER_POWER_INLINE_INVERT)
      //#ifndef LASER_POWER_INLINE_INVERT_WARN
      //  #define LASER_POWER_INLINE_INVERT_WARN
      //  #warning "Enabling LASER_POWER_INLINE_INVERT means that `M5` won't kill the laser immediately; use `M5 I` instead."
      //#endif
    #endif
  #else
    #if SPINDLE_LASER_POWERUP_DELAY < 1
      #error "SPINDLE_LASER_POWERUP_DELAY must be greater than 0."
    #elif SPINDLE_LASER_POWERDOWN_DELAY < 1
      #error "SPINDLE_LASER_POWERDOWN_DELAY must be greater than 0."
    #elif ENABLED(LASER_MOVE_POWER)
      #error "LASER_MOVE_POWER requires LASER_POWER_INLINE."
    #elif ANY(LASER_POWER_INLINE_TRAPEZOID, LASER_POWER_INLINE_INVERT, LASER_MOVE_G0_OFF, LASER_MOVE_POWER)
      #error "Enabled an inline laser feature without inline laser power being enabled."
    #endif
  #endif
  #define _PIN_CONFLICT(P) (PIN_EXISTS(P) && P##_PIN == SPINDLE_LASER_PWM_PIN)
  #if BOTH(SPINDLE_FEATURE, LASER_FEATURE)
    #error "Enable only one of SPINDLE_FEATURE or LASER_FEATURE."
  #elif !PIN_EXISTS(SPINDLE_LASER_ENA)
    #error "(SPINDLE|LASER)_FEATURE requires SPINDLE_LASER_ENA_PIN."
  #elif ENABLED(SPINDLE_CHANGE_DIR) && !PIN_EXISTS(SPINDLE_DIR)
    #error "SPINDLE_DIR_PIN is required for SPINDLE_CHANGE_DIR."
  #elif ENABLED(SPINDLE_LASER_PWM)
    #if !defined(SPINDLE_LASER_PWM_PIN) || SPINDLE_LASER_PWM_PIN < 0
      #error "SPINDLE_LASER_PWM_PIN is required for SPINDLE_LASER_PWM."
    #elif !_TEST_PWM(SPINDLE_LASER_PWM_PIN)
      #error "SPINDLE_LASER_PWM_PIN not assigned to a PWM pin."
    #elif !defined(SPINDLE_LASER_PWM_INVERT)
      #error "SPINDLE_LASER_PWM_INVERT is required for (SPINDLE|LASER)_FEATURE."
    #elif !(defined(SPEED_POWER_INTERCEPT) && defined(SPEED_POWER_MIN) && defined(SPEED_POWER_MAX) && defined(SPEED_POWER_STARTUP))
      #error "SPINDLE_LASER_PWM equation constant(s) missing."
    #elif _PIN_CONFLICT(X_MIN)
      #error "SPINDLE_LASER_PWM pin conflicts with X_MIN_PIN."
    #elif _PIN_CONFLICT(X_MAX)
      #error "SPINDLE_LASER_PWM pin conflicts with X_MAX_PIN."
    #elif _PIN_CONFLICT(Z_STEP)
      #error "SPINDLE_LASER_PWM pin conflicts with Z_STEP_PIN."
    #elif _PIN_CONFLICT(CASE_LIGHT)
      #error "SPINDLE_LASER_PWM_PIN conflicts with CASE_LIGHT_PIN."
    #elif _PIN_CONFLICT(E0_AUTO_FAN)
      #error "SPINDLE_LASER_PWM_PIN conflicts with E0_AUTO_FAN_PIN."
    #elif _PIN_CONFLICT(E1_AUTO_FAN)
      #error "SPINDLE_LASER_PWM_PIN conflicts with E1_AUTO_FAN_PIN."
    #elif _PIN_CONFLICT(E2_AUTO_FAN)
      #error "SPINDLE_LASER_PWM_PIN conflicts with E2_AUTO_FAN_PIN."
    #elif _PIN_CONFLICT(E3_AUTO_FAN)
      #error "SPINDLE_LASER_PWM_PIN conflicts with E3_AUTO_FAN_PIN."
    #elif _PIN_CONFLICT(E4_AUTO_FAN)
      #error "SPINDLE_LASER_PWM_PIN conflicts with E4_AUTO_FAN_PIN."
    #elif _PIN_CONFLICT(E5_AUTO_FAN)
      #error "SPINDLE_LASER_PWM_PIN conflicts with E5_AUTO_FAN_PIN."
    #elif _PIN_CONFLICT(E6_AUTO_FAN)
      #error "SPINDLE_LASER_PWM_PIN conflicts with E6_AUTO_FAN_PIN."
    #elif _PIN_CONFLICT(E7_AUTO_FAN)
      #error "SPINDLE_LASER_PWM_PIN conflicts with E7_AUTO_FAN_PIN."
    #elif _PIN_CONFLICT(FAN)
      #error "SPINDLE_LASER_PWM_PIN conflicts with FAN_PIN."
    #elif _PIN_CONFLICT(FAN1)
      #error "SPINDLE_LASER_PWM_PIN conflicts with FAN1_PIN."
    #elif _PIN_CONFLICT(FAN2)
      #error "SPINDLE_LASER_PWM_PIN conflicts with FAN2_PIN."
    #elif _PIN_CONFLICT(FAN3)
      #error "SPINDLE_LASER_PWM_PIN conflicts with FAN3_PIN."
    #elif _PIN_CONFLICT(FAN4)
      #error "SPINDLE_LASER_PWM_PIN conflicts with FAN4_PIN."
    #elif _PIN_CONFLICT(FAN5)
      #error "SPINDLE_LASER_PWM_PIN conflicts with FAN5_PIN."
    #elif _PIN_CONFLICT(FAN6)
      #error "SPINDLE_LASER_PWM_PIN conflicts with FAN6_PIN."
    #elif _PIN_CONFLICT(FAN7)
      #error "SPINDLE_LASER_PWM_PIN conflicts with FAN7_PIN."
    #elif _PIN_CONFLICT(CONTROLLERFAN)
      #error "SPINDLE_LASER_PWM_PIN conflicts with CONTROLLERFAN_PIN."
    #elif _PIN_CONFLICT(MOTOR_CURRENT_PWM_XY)
      #error "SPINDLE_LASER_PWM_PIN conflicts with MOTOR_CURRENT_PWM_XY."
    #elif _PIN_CONFLICT(MOTOR_CURRENT_PWM_Z)
      #error "SPINDLE_LASER_PWM_PIN conflicts with MOTOR_CURRENT_PWM_Z."
    #elif _PIN_CONFLICT(MOTOR_CURRENT_PWM_E)
      #error "SPINDLE_LASER_PWM_PIN conflicts with MOTOR_CURRENT_PWM_E."
    #endif
  #endif
  #undef _PIN_CONFLICT
#endif

#if !HAS_GRAPHICAL_LCD
  #if ENABLED(PRINT_PROGRESS_SHOW_DECIMALS)
    #error "PRINT_PROGRESS_SHOW_DECIMALS currently requires a Graphical LCD."
  #elif ENABLED(SHOW_REMAINING_TIME)
    #error "SHOW_REMAINING_TIME currently requires a Graphical LCD."
  #endif
#endif

#if HAS_ADC_BUTTONS && defined(ADC_BUTTON_DEBOUNCE_DELAY) && ADC_BUTTON_DEBOUNCE_DELAY < 16
  #error "ADC_BUTTON_DEBOUNCE_DELAY must be greater than 16."
#endif

/**
 * Check to make sure MONITOR_DRIVER_STATUS isn't enabled
 * on boards where TMC drivers share the SPI bus with SD.
 */
#if HAS_TMC_SPI && ALL(MONITOR_DRIVER_STATUS, SDSUPPORT, USES_SHARED_SPI)
  #error "MONITOR_DRIVER_STATUS and SDSUPPORT cannot be used together on boards with shared SPI."
#endif

// G60/G61 Position Save
#if SAVED_POSITIONS > 256
  #error "SAVED_POSITIONS must be an integer from 0 to 256."
#endif

/**
 * Stepper Chunk support
 */
#if BOTH(DIRECT_STEPPING, LIN_ADVANCE)
  #error "DIRECT_STEPPING is incompatible with LIN_ADVANCE. Enable in external planner if possible."
#endif

/**
 * Touch Buttons
 */
#if ENABLED(TOUCH_SCREEN)
  #ifndef XPT2046_X_CALIBRATION
    #error "XPT2046_X_CALIBRATION must be defined with TOUCH_SCREEN."
  #endif
  #ifndef XPT2046_Y_CALIBRATION
    #error "XPT2046_Y_CALIBRATION must be defined with TOUCH_SCREEN."
  #endif
  #ifndef XPT2046_X_OFFSET
    #error "XPT2046_X_OFFSET must be defined with TOUCH_SCREEN."
  #endif
  #ifndef XPT2046_Y_OFFSET
    #error "XPT2046_Y_OFFSET must be defined with TOUCH_SCREEN."
  #endif
#endif

/**
 * Sanity check for WIFI
 */
#if EITHER(ESP3D_WIFISUPPORT, WIFISUPPORT) && DISABLED(ARDUINO_ARCH_ESP32)
  #error "ESP3D_WIFISUPPORT or WIFISUPPORT requires an ESP32 controller."
#endif

// Misc. Cleanup
#undef _TEST_PWM
