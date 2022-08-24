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

// Strings for sanity check messages
#define _NUM_AXES_STR NUM_AXIS_GANG("X ", "Y ", "Z ", "I ", "J ", "K ", "U ", "V ", "W ")
#define _LOGICAL_AXES_STR LOGICAL_AXIS_GANG("E ", "X ", "Y ", "Z ", "I ", "J ", "K ", "U ", "V ", "W ")

// Make sure macros aren't borked
#define TEST1
#define TEST2 1
#define TEST3 0
#define TEST4 true
#if ENABLED(TEST0) || !ENABLED(TEST2) || ENABLED(TEST3) || !ENABLED(TEST1, TEST2, TEST4)
  #error "ENABLED is borked!"
#endif
#if BOTH(TEST0, TEST1)
  #error "BOTH is borked!"
#endif
#if DISABLED(TEST1) || !DISABLED(TEST3) || DISABLED(TEST4) || DISABLED(TEST0, TEST1, TEST2, TEST4) || !DISABLED(TEST0, TEST3)
  #error "DISABLED is borked!"
#endif
#if !ANY(TEST1, TEST2, TEST3, TEST4) || ANY(TEST0, TEST3)
  #error "ANY is borked!"
#endif
#if NONE(TEST0, TEST1, TEST2, TEST4) || !NONE(TEST0, TEST3)
  #error "NONE is borked!"
#endif
#undef TEST1
#undef TEST2
#undef TEST3
#undef TEST4

/**
 * This is to alert you about non-matching versions of config files.
 *
 * You can edit the version tag in your old config files and try the build again.
 * The checks below will alert you about options that need to be changed, but they won't
 * tell you about new options that you might find useful. So it's recommended to transfer
 * your settings to new Configuration files matching your Marlin version as soon as possible.
 */
#define HEXIFY(H) _CAT(0x,H)
#if !defined(CONFIGURATION_H_VERSION) || HEXIFY(CONFIGURATION_H_VERSION) < HEXIFY(REQUIRED_CONFIGURATION_H_VERSION)
  #error "Your Configuration.h file is for an old version of Marlin. Downgrade Marlin or upgrade your Configuration.h."
#elif HEXIFY(CONFIGURATION_H_VERSION) > HEXIFY(REQUIRED_CONFIGURATION_H_VERSION)
  #error "Your Configuration.h file is for a newer version of Marlin. Upgrade Marlin or downgrade your Configuration.h."
#endif
#if !defined(CONFIGURATION_ADV_H_VERSION) || HEXIFY(CONFIGURATION_ADV_H_VERSION) < HEXIFY(REQUIRED_CONFIGURATION_ADV_H_VERSION)
  #error "Your Configuration_adv.h file is for an old version of Marlin. Downgrade Marlin or upgrade your Configuration_adv.h."
#elif HEXIFY(CONFIGURATION_ADV_H_VERSION) > HEXIFY(REQUIRED_CONFIGURATION_ADV_H_VERSION)
  #error "Your Configuration_adv.h file is for a newer version of Marlin. Upgrade Marlin or downgrade your Configuration_adv.h."
#endif
#undef HEXIFY

/**
 * Warnings for old configurations
 */
#ifndef MOTHERBOARD
  #error "MOTHERBOARD is required."
#elif !defined(X_BED_SIZE) || !defined(Y_BED_SIZE)
  #error "X_BED_SIZE and Y_BED_SIZE are now required!"
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
  #error "SDCARDDETECTINVERTED is now SD_DETECT_STATE (HIGH)."
#elif defined(SD_DETECT_INVERTED)
  #error "SD_DETECT_INVERTED is now SD_DETECT_STATE (HIGH)."
#elif defined(BTENABLED)
  #error "BTENABLED is now BLUETOOTH."
#elif defined(CUSTOM_MENDEL_NAME)
  #error "CUSTOM_MENDEL_NAME is now CUSTOM_MACHINE_NAME."
#elif defined(HAS_AUTOMATIC_VERSIONING)
  #error "HAS_AUTOMATIC_VERSIONING is now CUSTOM_VERSION_FILE."
#elif defined(USE_AUTOMATIC_VERSIONING)
  #error "USE_AUTOMATIC_VERSIONING is now CUSTOM_VERSION_FILE."
#elif defined(SDSLOW)
  #error "SDSLOW deprecated. Set SD_SPI_SPEED to SPI_HALF_SPEED instead."
#elif defined(SDEXTRASLOW)
  #error "SDEXTRASLOW deprecated. Set SD_SPI_SPEED to SPI_QUARTER_SPEED instead."
#elif defined(FILAMENT_SENSOR)
  #error "FILAMENT_SENSOR is now FILAMENT_WIDTH_SENSOR."
#elif defined(ENDSTOPPULLUP_FIL_RUNOUT)
  #error "ENDSTOPPULLUP_FIL_RUNOUT is now FIL_RUNOUT_PULLUP."
#elif defined(DISABLE_MAX_ENDSTOPS) || defined(DISABLE_MIN_ENDSTOPS)
  #error "DISABLE_MAX_ENDSTOPS and DISABLE_MIN_ENDSTOPS deprecated. Use individual USE_*_PLUG options instead."
#elif defined(LANGUAGE_INCLUDE)
  #error "LANGUAGE_INCLUDE has been replaced by LCD_LANGUAGE."
#elif defined(EXTRUDER_OFFSET_X) || defined(EXTRUDER_OFFSET_Y)
  #error "EXTRUDER_OFFSET_[XY] is deprecated. Use HOTEND_OFFSET_[XY] instead."
#elif defined(PID_PARAMS_PER_EXTRUDER)
  #error "PID_PARAMS_PER_EXTRUDER is deprecated. Use PID_PARAMS_PER_HOTEND instead."
#elif defined(EXTRUDER_WATTS) || defined(BED_WATTS)
  #error "EXTRUDER_WATTS and BED_WATTS are deprecated and should be removed."
#elif defined(SERVO_ENDSTOP_ANGLES)
  #error "SERVO_ENDSTOP_ANGLES is deprecated. Use Z_SERVO_ANGLES instead."
#elif defined(X_ENDSTOP_SERVO_NR) || defined(Y_ENDSTOP_SERVO_NR)
  #error "X_ENDSTOP_SERVO_NR and Y_ENDSTOP_SERVO_NR are deprecated and should be removed."
#elif defined(Z_ENDSTOP_SERVO_NR)
  #error "Z_ENDSTOP_SERVO_NR is now Z_PROBE_SERVO_NR."
#elif defined(DEFAULT_XYJERK)
  #error "DEFAULT_XYJERK is deprecated. Use DEFAULT_XJERK and DEFAULT_YJERK instead."
#elif defined(XY_TRAVEL_SPEED)
  #error "XY_TRAVEL_SPEED is now XY_PROBE_FEEDRATE."
#elif defined(XY_PROBE_SPEED)
  #error "XY_PROBE_SPEED is now XY_PROBE_FEEDRATE."
#elif defined(Z_PROBE_SPEED_FAST)
  #error "Z_PROBE_SPEED_FAST is now Z_PROBE_FEEDRATE_FAST."
#elif defined(Z_PROBE_SPEED_SLOW)
  #error "Z_PROBE_SPEED_SLOW is now Z_PROBE_FEEDRATE_SLOW."
#elif defined(PROBE_SERVO_DEACTIVATION_DELAY)
  #error "PROBE_SERVO_DEACTIVATION_DELAY is deprecated. Use DEACTIVATE_SERVOS_AFTER_MOVE instead."
#elif defined(SERVO_DEACTIVATION_DELAY)
  #error "SERVO_DEACTIVATION_DELAY is now SERVO_DELAY."
#elif ENABLED(FILAMENTCHANGEENABLE)
  #error "FILAMENTCHANGEENABLE is now ADVANCED_PAUSE_FEATURE."
#elif ENABLED(FILAMENT_CHANGE_FEATURE)
  #error "FILAMENT_CHANGE_FEATURE is now ADVANCED_PAUSE_FEATURE."
#elif defined(FILAMENT_CHANGE_X_POS) || defined(FILAMENT_CHANGE_Y_POS)
  #error "FILAMENT_CHANGE_[XY]_POS is now set with NOZZLE_PARK_POINT."
#elif defined(FILAMENT_CHANGE_Z_ADD)
  #error "FILAMENT_CHANGE_Z_ADD is now set with NOZZLE_PARK_POINT."
#elif defined(FILAMENT_CHANGE_XY_FEEDRATE)
  #error "FILAMENT_CHANGE_XY_FEEDRATE is now NOZZLE_PARK_XY_FEEDRATE."
#elif defined(FILAMENT_CHANGE_Z_FEEDRATE)
  #error "FILAMENT_CHANGE_Z_FEEDRATE is now NOZZLE_PARK_Z_FEEDRATE."
#elif defined(PAUSE_PARK_X_POS) || defined(PAUSE_PARK_Y_POS)
  #error "PAUSE_PARK_[XY]_POS is now set with NOZZLE_PARK_POINT."
#elif defined(PAUSE_PARK_Z_ADD)
  #error "PAUSE_PARK_Z_ADD is now set with NOZZLE_PARK_POINT."
#elif defined(PAUSE_PARK_XY_FEEDRATE)
  #error "PAUSE_PARK_XY_FEEDRATE is now NOZZLE_PARK_XY_FEEDRATE."
#elif defined(PAUSE_PARK_Z_FEEDRATE)
  #error "PAUSE_PARK_Z_FEEDRATE is now NOZZLE_PARK_Z_FEEDRATE."
#elif defined(FILAMENT_CHANGE_RETRACT_FEEDRATE)
  #error "FILAMENT_CHANGE_RETRACT_FEEDRATE is now PAUSE_PARK_RETRACT_FEEDRATE."
#elif defined(FILAMENT_CHANGE_RETRACT_LENGTH)
  #error "FILAMENT_CHANGE_RETRACT_LENGTH is now PAUSE_PARK_RETRACT_LENGTH."
#elif defined(FILAMENT_CHANGE_EXTRUDE_FEEDRATE)
  #error "FILAMENT_CHANGE_EXTRUDE_FEEDRATE is now ADVANCED_PAUSE_PURGE_FEEDRATE."
#elif defined(ADVANCED_PAUSE_EXTRUDE_FEEDRATE)
  #error "ADVANCED_PAUSE_EXTRUDE_FEEDRATE is now ADVANCED_PAUSE_PURGE_FEEDRATE."
#elif defined(FILAMENT_CHANGE_EXTRUDE_LENGTH)
  #error "FILAMENT_CHANGE_EXTRUDE_LENGTH is now ADVANCED_PAUSE_PURGE_LENGTH."
#elif defined(ADVANCED_PAUSE_EXTRUDE_LENGTH)
  #error "ADVANCED_PAUSE_EXTRUDE_LENGTH is now ADVANCED_PAUSE_PURGE_LENGTH."
#elif defined(FILAMENT_CHANGE_NOZZLE_TIMEOUT)
  #error "FILAMENT_CHANGE_NOZZLE_TIMEOUT is now PAUSE_PARK_NOZZLE_TIMEOUT."
#elif defined(FILAMENT_CHANGE_NUMBER_OF_ALERT_BEEPS)
  #error "FILAMENT_CHANGE_NUMBER_OF_ALERT_BEEPS is now FILAMENT_CHANGE_ALERT_BEEPS."
#elif defined(FILAMENT_CHANGE_NO_STEPPER_TIMEOUT)
  #error "FILAMENT_CHANGE_NO_STEPPER_TIMEOUT is now PAUSE_PARK_NO_STEPPER_TIMEOUT."
#elif defined(PLA_PREHEAT_HOTEND_TEMP)
  #error "PLA_PREHEAT_HOTEND_TEMP is now PREHEAT_1_TEMP_HOTEND."
#elif defined(PLA_PREHEAT_HPB_TEMP)
  #error "PLA_PREHEAT_HPB_TEMP is now PREHEAT_1_TEMP_BED."
#elif defined(PLA_PREHEAT_FAN_SPEED)
  #error "PLA_PREHEAT_FAN_SPEED is now PREHEAT_1_FAN_SPEED."
#elif defined(ABS_PREHEAT_HOTEND_TEMP)
  #error "ABS_PREHEAT_HOTEND_TEMP is now PREHEAT_2_TEMP_HOTEND."
#elif defined(ABS_PREHEAT_HPB_TEMP)
  #error "ABS_PREHEAT_HPB_TEMP is now PREHEAT_2_TEMP_BED."
#elif defined(ABS_PREHEAT_FAN_SPEED)
  #error "ABS_PREHEAT_FAN_SPEED is now PREHEAT_2_FAN_SPEED."
#elif defined(ENDSTOPS_ONLY_FOR_HOMING)
  #error "ENDSTOPS_ONLY_FOR_HOMING is deprecated. Use (disable) ENDSTOPS_ALWAYS_ON_DEFAULT instead."
#elif defined(HOMING_FEEDRATE)
  #error "HOMING_FEEDRATE is now set using the HOMING_FEEDRATE_MM_M array instead."
#elif (defined(HOMING_FEEDRATE_XY) || defined(HOMING_FEEDRATE_Z)) && !defined(HOMING_FEEDRATE_MM_M)
  #error "HOMING_FEEDRATE_XY and HOMING_FEEDRATE_Z are now set using the HOMING_FEEDRATE_MM_M array instead."
#elif defined(MANUAL_HOME_POSITIONS)
  #error "MANUAL_HOME_POSITIONS is deprecated. Set MANUAL_[XYZ]_HOME_POS as-needed instead."
#elif defined(PID_ADD_EXTRUSION_RATE)
  #error "PID_ADD_EXTRUSION_RATE is now PID_EXTRUSION_SCALING and is DISABLED by default."
#elif defined(Z_RAISE_BEFORE_HOMING)
  #error "Z_RAISE_BEFORE_HOMING is now Z_HOMING_HEIGHT."
#elif defined(MIN_Z_HEIGHT_FOR_HOMING)
  #error "MIN_Z_HEIGHT_FOR_HOMING is now Z_HOMING_HEIGHT."
#elif defined(Z_RAISE_BEFORE_PROBING) || defined(Z_RAISE_AFTER_PROBING)
  #error "Z_RAISE_(BEFORE|AFTER)_PROBING are deprecated. Use Z_CLEARANCE_DEPLOY_PROBE and Z_AFTER_PROBING instead."
#elif defined(Z_RAISE_PROBE_DEPLOY_STOW) || defined(Z_RAISE_BETWEEN_PROBINGS)
  #error "Z_RAISE_PROBE_DEPLOY_STOW and Z_RAISE_BETWEEN_PROBINGS are now Z_CLEARANCE_DEPLOY_PROBE and Z_CLEARANCE_BETWEEN_PROBES."
#elif defined(Z_PROBE_DEPLOY_HEIGHT) || defined(Z_PROBE_TRAVEL_HEIGHT)
  #error "Z_PROBE_DEPLOY_HEIGHT and Z_PROBE_TRAVEL_HEIGHT are now Z_CLEARANCE_DEPLOY_PROBE and Z_CLEARANCE_BETWEEN_PROBES."
#elif defined(MANUAL_BED_LEVELING)
  #error "MANUAL_BED_LEVELING is now LCD_BED_LEVELING."
#elif defined(MESH_HOME_SEARCH_Z)
  #error "MESH_HOME_SEARCH_Z is now LCD_PROBE_Z_RANGE."
#elif defined(MANUAL_PROBE_Z_RANGE)
  #error "MANUAL_PROBE_Z_RANGE is now LCD_PROBE_Z_RANGE."
#elif !defined(MIN_STEPS_PER_SEGMENT)
  #error "Please replace 'const int dropsegments' with '#define MIN_STEPS_PER_SEGMENT' (and increase by 1)."
#elif MIN_STEPS_PER_SEGMENT <= 0
  #error "MIN_STEPS_PER_SEGMENT must be at least 1."
#elif defined(PREVENT_DANGEROUS_EXTRUDE)
  #error "PREVENT_DANGEROUS_EXTRUDE is now PREVENT_COLD_EXTRUSION."
#elif defined(SCARA)
  #error "SCARA is now MORGAN_SCARA."
#elif defined(ENABLE_AUTO_BED_LEVELING)
  #error "ENABLE_AUTO_BED_LEVELING is deprecated. Specify AUTO_BED_LEVELING_LINEAR, AUTO_BED_LEVELING_BILINEAR, or AUTO_BED_LEVELING_3POINT."
#elif defined(AUTO_BED_LEVELING_FEATURE)
  #error "AUTO_BED_LEVELING_FEATURE is deprecated. Specify AUTO_BED_LEVELING_LINEAR, AUTO_BED_LEVELING_BILINEAR, or AUTO_BED_LEVELING_3POINT."
#elif defined(ABL_GRID_POINTS)
  #error "ABL_GRID_POINTS is now GRID_MAX_POINTS_X and GRID_MAX_POINTS_Y."
#elif defined(ABL_GRID_POINTS_X) || defined(ABL_GRID_POINTS_Y)
  #error "ABL_GRID_POINTS_[XY] is now GRID_MAX_POINTS_[XY]."
#elif defined(ABL_GRID_MAX_POINTS_X) || defined(ABL_GRID_MAX_POINTS_Y)
  #error "ABL_GRID_MAX_POINTS_[XY] is now GRID_MAX_POINTS_[XY]."
#elif defined(MESH_NUM_X_POINTS) || defined(MESH_NUM_Y_POINTS)
  #error "MESH_NUM_[XY]_POINTS is now GRID_MAX_POINTS_[XY]."
#elif defined(UBL_MESH_NUM_X_POINTS) || defined(UBL_MESH_NUM_Y_POINTS)
  #error "UBL_MESH_NUM_[XY]_POINTS is now GRID_MAX_POINTS_[XY]."
#elif defined(UBL_G26_MESH_VALIDATION)
  #error "UBL_G26_MESH_VALIDATION is now G26_MESH_VALIDATION."
#elif defined(UBL_MESH_EDIT_ENABLED)
  #error "UBL_MESH_EDIT_ENABLED is now G26_MESH_VALIDATION."
#elif defined(UBL_MESH_EDITING)
  #error "UBL_MESH_EDITING is now G26_MESH_VALIDATION."
#elif defined(BLTOUCH_HEATERS_OFF)
  #error "BLTOUCH_HEATERS_OFF is now PROBING_HEATERS_OFF."
#elif defined(BLTOUCH_V3)
  #error "BLTOUCH_V3 is obsolete."
#elif defined(BLTOUCH_FORCE_OPEN_DRAIN_MODE)
  #error "BLTOUCH_FORCE_OPEN_DRAIN_MODE is obsolete."
#elif defined(BEEPER)
  #error "BEEPER is now BEEPER_PIN."
#elif defined(SDCARDDETECT)
  #error "SDCARDDETECT is now SD_DETECT_PIN."
#elif defined(STAT_LED_RED) || defined(STAT_LED_BLUE)
  #error "STAT_LED_RED/STAT_LED_BLUE are now STAT_LED_RED_PIN/STAT_LED_BLUE_PIN."
#elif defined(LCD_PIN_BL)
  #error "LCD_PIN_BL is now LCD_BACKLIGHT_PIN."
#elif defined(LCD_PIN_RESET)
  #error "LCD_PIN_RESET is now LCD_RESET_PIN."
#elif defined(EXTRUDER_0_AUTO_FAN_PIN) || defined(EXTRUDER_1_AUTO_FAN_PIN) || defined(EXTRUDER_2_AUTO_FAN_PIN) || defined(EXTRUDER_3_AUTO_FAN_PIN)
  #error "EXTRUDER_[0123]_AUTO_FAN_PIN is now E[0123]_AUTO_FAN_PIN."
#elif defined(PID_FAN_SCALING) && !HAS_FAN
  #error "PID_FAN_SCALING needs at least one fan enabled."
#elif defined(min_software_endstops) || defined(max_software_endstops)
  #error "(min|max)_software_endstops are now (MIN|MAX)_SOFTWARE_ENDSTOPS."
#elif ENABLED(Z_PROBE_SLED) && defined(SLED_PIN)
  #error "Replace SLED_PIN with SOL1_PIN (applies to both Z_PROBE_SLED and SOLENOID_PROBE)."
#elif defined(CONTROLLERFAN_PIN)
  #error "CONTROLLERFAN_PIN is now CONTROLLER_FAN_PIN, enabled with USE_CONTROLLER_FAN."
#elif defined(CONTROLLERFAN_SPEED)
  #error "CONTROLLERFAN_SPEED is now CONTROLLERFAN_SPEED_ACTIVE."
#elif defined(CONTROLLERFAN_SECS)
  #error "CONTROLLERFAN_SECS is now CONTROLLERFAN_IDLE_TIME."
#elif defined(MIN_RETRACT)
  #error "MIN_RETRACT is now MIN_AUTORETRACT and MAX_AUTORETRACT."
#elif defined(ADVANCE)
  #error "ADVANCE is now LIN_ADVANCE."
#elif defined(LIN_ADVANCE_E_D_RATIO)
  #error "LIN_ADVANCE (1.5) no longer uses LIN_ADVANCE_E_D_RATIO."
#elif defined(NEOPIXEL_RGBW_LED)
  #error "NEOPIXEL_RGBW_LED is now NEOPIXEL_LED."
#elif ENABLED(DELTA) && defined(DELTA_PROBEABLE_RADIUS)
  #error "Remove DELTA_PROBEABLE_RADIUS and use PROBING_MARGIN to inset the probe area instead."
#elif ENABLED(DELTA) && defined(DELTA_CALIBRATION_RADIUS)
  #error "Remove DELTA_CALIBRATION_RADIUS and use PROBING_MARGIN to inset the probe area instead."
#elif defined(UBL_MESH_INSET)
  #error "UBL_MESH_INSET is now just MESH_INSET."
#elif defined(UBL_MESH_MIN_X) || defined(UBL_MESH_MIN_Y) || defined(UBL_MESH_MAX_X) || defined(UBL_MESH_MAX_Y)
  #error "UBL_MESH_(MIN|MAX)_[XY] is now just MESH_(MIN|MAX)_[XY]."
#elif defined(ABL_PROBE_PT_1_X) || defined(ABL_PROBE_PT_1_Y) || defined(ABL_PROBE_PT_2_X) || defined(ABL_PROBE_PT_2_Y) || defined(ABL_PROBE_PT_3_X) || defined(ABL_PROBE_PT_3_Y)
  #error "ABL_PROBE_PT_[123]_[XY] is no longer required. Please remove it."
#elif defined(UBL_PROBE_PT_1_X) || defined(UBL_PROBE_PT_1_Y) || defined(UBL_PROBE_PT_2_X) || defined(UBL_PROBE_PT_2_Y) || defined(UBL_PROBE_PT_3_X) || defined(UBL_PROBE_PT_3_Y)
  #error "UBL_PROBE_PT_[123]_[XY] is no longer required. Please remove it."
#elif defined(MIN_PROBE_EDGE)
  #error "MIN_PROBE_EDGE is now called PROBING_MARGIN."
#elif defined(MIN_PROBE_EDGE_LEFT)
  #error "MIN_PROBE_EDGE_LEFT is now called PROBING_MARGIN_LEFT."
#elif defined(MIN_PROBE_EDGE_RIGHT)
  #error "MIN_PROBE_EDGE_RIGHT is now called PROBING_MARGIN_RIGHT."
#elif defined(MIN_PROBE_EDGE_FRONT)
  #error "MIN_PROBE_EDGE_FRONT is now called PROBING_MARGIN_FRONT."
#elif defined(MIN_PROBE_EDGE_BACK)
  #error "MIN_PROBE_EDGE_BACK is now called PROBING_MARGIN_BACK."
#elif defined(LEFT_PROBE_BED_POSITION)
  #error "LEFT_PROBE_BED_POSITION is obsolete. Set a margin with PROBING_MARGIN or PROBING_MARGIN_LEFT instead."
#elif defined(RIGHT_PROBE_BED_POSITION)
  #error "RIGHT_PROBE_BED_POSITION is obsolete. Set a margin with PROBING_MARGIN or PROBING_MARGIN_RIGHT instead."
#elif defined(FRONT_PROBE_BED_POSITION)
  #error "FRONT_PROBE_BED_POSITION is obsolete. Set a margin with PROBING_MARGIN or PROBING_MARGIN_FRONT instead."
#elif defined(BACK_PROBE_BED_POSITION)
  #error "BACK_PROBE_BED_POSITION is obsolete. Set a margin with PROBING_MARGIN or PROBING_MARGIN_BACK instead."
#elif defined(ENABLE_MESH_EDIT_GFX_OVERLAY)
  #error "ENABLE_MESH_EDIT_GFX_OVERLAY is now MESH_EDIT_GFX_OVERLAY."
#elif defined(BABYSTEP_ZPROBE_GFX_REVERSE)
  #error "BABYSTEP_ZPROBE_GFX_REVERSE is now set by OVERLAY_GFX_REVERSE."
#elif defined(UBL_GRANULAR_SEGMENTATION_FOR_CARTESIAN)
  #error "UBL_GRANULAR_SEGMENTATION_FOR_CARTESIAN is now SEGMENT_LEVELED_MOVES."
#elif HAS_PID_HEATING && (defined(K1) || !defined(PID_K1))
  #error "K1 is now PID_K1."
#elif defined(PROBE_DOUBLE_TOUCH)
  #error "PROBE_DOUBLE_TOUCH is now MULTIPLE_PROBING."
#elif defined(ANET_KEYPAD_LCD)
  #error "ANET_KEYPAD_LCD is now ZONESTAR_LCD."
#elif defined(LCD_I2C_SAINSMART_YWROBOT)
  #error "LCD_I2C_SAINSMART_YWROBOT is now LCD_SAINSMART_I2C_(1602|2004)."
#elif defined(MEASURED_LOWER_LIMIT) || defined(MEASURED_UPPER_LIMIT)
  #error "MEASURED_(UPPER|LOWER)_LIMIT is now FILWIDTH_ERROR_MARGIN."
#elif defined(HAVE_TMCDRIVER)
  #error "HAVE_TMCDRIVER is now [AXIS]_DRIVER_TYPE TMC26X."
#elif defined(STEALTHCHOP)
  #error "STEALTHCHOP is now STEALTHCHOP_(XY|Z|E)."
#elif defined(HAVE_TMC26X)
  #error "HAVE_TMC26X is now [AXIS]_DRIVER_TYPE TMC26X."
#elif defined(HAVE_TMC2130)
  #error "HAVE_TMC2130 is now [AXIS]_DRIVER_TYPE TMC2130."
#elif defined(HAVE_TMC2208)
  #error "HAVE_TMC2208 is now [AXIS]_DRIVER_TYPE TMC2208."
#elif defined(HAVE_L6470DRIVER)
  #error "HAVE_L6470DRIVER is obsolete. L64xx stepper drivers are no longer supported in Marlin."
#elif defined(X_IS_TMC) || defined(X2_IS_TMC) || defined(Y_IS_TMC) || defined(Y2_IS_TMC) || defined(Z_IS_TMC) || defined(Z2_IS_TMC) || defined(Z3_IS_TMC) \
   || defined(E0_IS_TMC) || defined(E1_IS_TMC) || defined(E2_IS_TMC) || defined(E3_IS_TMC) || defined(E4_IS_TMC) || defined(E5_IS_TMC) || defined(E6_IS_TMC) || defined(E7_IS_TMC)
  #error "[AXIS]_IS_TMC is now [AXIS]_DRIVER_TYPE TMC26X."
#elif defined(X_IS_TMC26X) || defined(X2_IS_TMC26X) || defined(Y_IS_TMC26X) || defined(Y2_IS_TMC26X) || defined(Z_IS_TMC26X) || defined(Z2_IS_TMC26X) || defined(Z3_IS_TMC26X) \
   || defined(E0_IS_TMC26X) || defined(E1_IS_TMC26X) || defined(E2_IS_TMC26X) || defined(E3_IS_TMC26X) || defined(E4_IS_TMC26X) || defined(E5_IS_TMC26X) || defined(E6_IS_TMC26X) || defined(E7_IS_TMC26X)
  #error "[AXIS]_IS_TMC26X is now [AXIS]_DRIVER_TYPE TMC26X."
#elif defined(X_IS_TMC2130) || defined(X2_IS_TMC2130) || defined(Y_IS_TMC2130) || defined(Y2_IS_TMC2130) || defined(Z_IS_TMC2130) || defined(Z2_IS_TMC2130) || defined(Z3_IS_TMC2130) \
   || defined(E0_IS_TMC2130) || defined(E1_IS_TMC2130) || defined(E2_IS_TMC2130) || defined(E3_IS_TMC2130) || defined(E4_IS_TMC2130) || defined(E5_IS_TMC2130) || defined(E6_IS_TMC2130) || defined(E7_IS_TMC2130)
  #error "[AXIS]_IS_TMC2130 is now [AXIS]_DRIVER_TYPE TMC2130."
#elif defined(X_IS_TMC2208) || defined(X2_IS_TMC2208) || defined(Y_IS_TMC2208) || defined(Y2_IS_TMC2208) || defined(Z_IS_TMC2208) || defined(Z2_IS_TMC2208) || defined(Z3_IS_TMC2208) \
   || defined(E0_IS_TMC2208) || defined(E1_IS_TMC2208) || defined(E2_IS_TMC2208) || defined(E3_IS_TMC2208) || defined(E4_IS_TMC2208) || defined(E5_IS_TMC2208) || defined(E6_IS_TMC2208) || defined(E7_IS_TMC2208)
  #error "[AXIS]_IS_TMC2208 is now [AXIS]_DRIVER_TYPE TMC2208."
#elif defined(AUTOMATIC_CURRENT_CONTROL)
  #error "AUTOMATIC_CURRENT_CONTROL is now MONITOR_DRIVER_STATUS."
#elif defined(FILAMENT_CHANGE_LOAD_LENGTH)
  #error "FILAMENT_CHANGE_LOAD_LENGTH is now FILAMENT_CHANGE_FAST_LOAD_LENGTH."
#elif defined(LEVEL_CORNERS_INSET)
  #error "LEVEL_CORNERS_INSET is now BED_TRAMMING_INSET_LFRB."
#elif defined(BEZIER_JERK_CONTROL)
  #error "BEZIER_JERK_CONTROL is now S_CURVE_ACCELERATION."
#elif HAS_JUNCTION_DEVIATION && defined(JUNCTION_DEVIATION_FACTOR)
  #error "JUNCTION_DEVIATION_FACTOR is now JUNCTION_DEVIATION_MM."
#elif defined(JUNCTION_ACCELERATION_FACTOR)
  #error "JUNCTION_ACCELERATION_FACTOR is obsolete. Delete it from Configuration_adv.h."
#elif defined(JUNCTION_ACCELERATION)
  #error "JUNCTION_ACCELERATION is obsolete. Delete it from Configuration_adv.h."
#elif defined(MAX7219_DEBUG_STEPPER_HEAD)
  #error "MAX7219_DEBUG_STEPPER_HEAD is now MAX7219_DEBUG_PLANNER_HEAD."
#elif defined(MAX7219_DEBUG_STEPPER_TAIL)
  #error "MAX7219_DEBUG_STEPPER_TAIL is now MAX7219_DEBUG_PLANNER_TAIL."
#elif defined(MAX7219_DEBUG_STEPPER_QUEUE)
  #error "MAX7219_DEBUG_STEPPER_QUEUE is now MAX7219_DEBUG_PLANNER_QUEUE."
#elif defined(ENDSTOP_NOISE_FILTER)
  #error "ENDSTOP_NOISE_FILTER is now ENDSTOP_NOISE_THRESHOLD [2-7]."
#elif defined(RETRACT_ZLIFT)
  #error "RETRACT_ZLIFT is now RETRACT_ZRAISE."
#elif defined(TOOLCHANGE_FS_INIT_BEFORE_SWAP)
  #error "TOOLCHANGE_FS_INIT_BEFORE_SWAP is now TOOLCHANGE_FS_SLOW_FIRST_PRIME."
#elif defined(TOOLCHANGE_PARK_ZLIFT) || defined(TOOLCHANGE_UNPARK_ZLIFT)
  #error "TOOLCHANGE_PARK_ZLIFT and TOOLCHANGE_UNPARK_ZLIFT are now TOOLCHANGE_ZRAISE."
#elif defined(SINGLENOZZLE_TOOLCHANGE_ZRAISE)
  #error "SINGLENOZZLE_TOOLCHANGE_ZRAISE is now TOOLCHANGE_ZRAISE."
#elif defined(SINGLENOZZLE_SWAP_LENGTH)
  #error "SINGLENOZZLE_SWAP_LENGTH is now TOOLCHANGE_FIL_SWAP_LENGTH."
#elif defined(SINGLENOZZLE_SWAP_RETRACT_SPEED)
  #error "SINGLENOZZLE_SWAP_RETRACT_SPEED is now TOOLCHANGE_FIL_SWAP_RETRACT_SPEED."
#elif defined(SINGLENOZZLE_SWAP_PRIME_SPEED)
  #error "SINGLENOZZLE_SWAP_PRIME_SPEED is now TOOLCHANGE_FIL_SWAP_PRIME_SPEED."
#elif defined(SINGLENOZZLE_SWAP_PARK)
  #error "SINGLENOZZLE_SWAP_PARK is now TOOLCHANGE_PARK."
#elif defined(SINGLENOZZLE_TOOLCHANGE_XY)
  #error "SINGLENOZZLE_TOOLCHANGE_XY is now TOOLCHANGE_PARK_XY."
#elif defined(SINGLENOZZLE_PARK_XY_FEEDRATE)
  #error "SINGLENOZZLE_PARK_XY_FEEDRATE is now TOOLCHANGE_PARK_XY_FEEDRATE."
#elif defined(PARKING_EXTRUDER_SECURITY_RAISE)
  #error "PARKING_EXTRUDER_SECURITY_RAISE is now TOOLCHANGE_ZRAISE."
#elif defined(SWITCHING_TOOLHEAD_SECURITY_RAISE)
  #error "SWITCHING_TOOLHEAD_SECURITY_RAISE is now TOOLCHANGE_ZRAISE."
#elif defined(G0_FEEDRATE) && G0_FEEDRATE == 0
  #error "G0_FEEDRATE is now used to set the G0 feedrate."
#elif defined(MBL_Z_STEP)
  #error "MBL_Z_STEP is now MESH_EDIT_Z_STEP."
#elif defined(CHDK)
  #error "CHDK is now CHDK_PIN."
#elif ANY_PIN( \
        MAX6675_SS, MAX6675_SS2, MAX6675_CS, MAX6675_CS2, \
        MAX31855_SS, MAX31855_SS2, MAX31855_CS, MAX31855_CS2, \
        MAX31865_SS, MAX31865_SS2, MAX31865_CS, MAX31865_CS2)
  #warning "MAX*_SS_PIN, MAX*_SS2_PIN, MAX*_CS_PIN, and MAX*_CS2_PIN are deprecated and will be removed in a future version. Please use TEMP_0_CS_PIN/TEMP_1_CS_PIN instead."
#elif ANY_PIN(MAX6675_SCK, MAX31855_SCK, MAX31865_SCK)
  #warning "MAX*_SCK_PIN is deprecated and will be removed in a future version. Please use TEMP_0_SCK_PIN/TEMP_1_SCK_PIN instead."
#elif ANY_PIN(MAX6675_MISO, MAX6675_DO, MAX31855_MISO, MAX31855_DO, MAX31865_MISO, MAX31865_DO)
  #warning "MAX*_MISO_PIN and MAX*_DO_PIN are deprecated and will be removed in a future version. Please use TEMP_0_MISO_PIN/TEMP_1_MISO_PIN instead."
#elif PIN_EXISTS(MAX31865_MOSI)
  #warning "MAX31865_MOSI_PIN is deprecated and will be removed in a future version. Please use TEMP_0_MOSI_PIN/TEMP_1_MOSI_PIN instead."
#elif ANY_PIN(THERMO_CS1_PIN, THERMO_CS2_PIN, THERMO_DO_PIN, THERMO_SCK_PIN)
  #error "THERMO_*_PIN is now TEMP_n_CS_PIN, TEMP_n_SCK_PIN, TEMP_n_MOSI_PIN, TEMP_n_MISO_PIN."
#elif defined(MAX31865_SENSOR_OHMS)
  #error "MAX31865_SENSOR_OHMS is now MAX31865_SENSOR_OHMS_0."
#elif defined(MAX31865_CALIBRATION_OHMS)
  #error "MAX31865_CALIBRATION_OHMS is now MAX31865_CALIBRATION_OHMS_0."
#elif defined(SPINDLE_LASER_ENABLE)
  #error "SPINDLE_LASER_ENABLE is now SPINDLE_FEATURE or LASER_FEATURE."
#elif defined(SPINDLE_LASER_ENABLE_PIN)
  #error "SPINDLE_LASER_ENABLE_PIN is now SPINDLE_LASER_ENA_PIN."
#elif defined(SPINDLE_DIR_CHANGE)
  #error "SPINDLE_DIR_CHANGE is now SPINDLE_CHANGE_DIR."
#elif defined(SPINDLE_STOP_ON_DIR_CHANGE)
  #error "SPINDLE_STOP_ON_DIR_CHANGE is now SPINDLE_CHANGE_DIR_STOP."
#elif defined(SPINDLE_LASER_ACTIVE_HIGH)
  #error "SPINDLE_LASER_ACTIVE_HIGH is now SPINDLE_LASER_ACTIVE_STATE."
#elif defined(SPINDLE_LASER_ENABLE_INVERT)
  #error "SPINDLE_LASER_ENABLE_INVERT is now SPINDLE_LASER_ACTIVE_STATE."
#elif defined(LASER_POWER_INLINE)
  #error "LASER_POWER_INLINE is not required, inline mode is enabled with 'M3 I' and disabled with 'M5 I'."
#elif defined(LASER_POWER_INLINE_TRAPEZOID)
  #error "LASER_POWER_INLINE_TRAPEZOID is now LASER_POWER_TRAP."
#elif defined(LASER_POWER_INLINE_TRAPEZOID_CONT)
  #error "LASER_POWER_INLINE_TRAPEZOID_CONT is replaced with LASER_POWER_TRAP."
#elif defined(LASER_POWER_INLINE_TRAPEZOID_PER)
  #error "LASER_POWER_INLINE_TRAPEZOID_CONT_PER  replaced with LASER_POWER_TRAP."
#elif defined(LASER_POWER_INLINE_CONTINUOUS)
  #error "LASER_POWER_INLINE_CONTINUOUS is not required, inline mode is enabled with 'M3 I' and disabled with 'M5 I'."
#elif defined(CUTTER_POWER_DISPLAY)
  #error "CUTTER_POWER_DISPLAY is now CUTTER_POWER_UNIT."
#elif defined(CHAMBER_HEATER_PIN)
  #error "CHAMBER_HEATER_PIN is now HEATER_CHAMBER_PIN."
#elif defined(TMC_Z_CALIBRATION)
  #error "TMC_Z_CALIBRATION has been deprecated in favor of MECHANICAL_GANTRY_CALIBRATION."
#elif defined(Z_MIN_PROBE_ENDSTOP)
  #error "Z_MIN_PROBE_ENDSTOP is no longer required. Please remove it."
#elif defined(DUAL_NOZZLE_DUPLICATION_MODE)
  #error "DUAL_NOZZLE_DUPLICATION_MODE is now MULTI_NOZZLE_DUPLICATION."
#elif defined(MENU_ITEM_CASE_LIGHT)
  #error "MENU_ITEM_CASE_LIGHT is now CASE_LIGHT_MENU."
#elif defined(CASE_LIGHT_NEOPIXEL_COLOR)
  #error "CASE_LIGHT_NEOPIXEL_COLOR is now CASE_LIGHT_DEFAULT_COLOR."
#elif defined(ABORT_ON_ENDSTOP_HIT_FEATURE_ENABLED)
  #error "ABORT_ON_ENDSTOP_HIT_FEATURE_ENABLED is now SD_ABORT_ON_ENDSTOP_HIT."
#elif defined(LPC_SD_LCD) || defined(LPC_SD_ONBOARD) || defined(LPC_SD_CUSTOM_CABLE)
  #error "LPC_SD_(LCD|ONBOARD|CUSTOM_CABLE) are now SDCARD_CONNECTION."
#elif defined(USB_SD_DISABLED)
  #error "USB_SD_DISABLED is now NO_SD_HOST_DRIVE."
#elif defined(USB_SD_ONBOARD)
  #error "USB_SD_ONBOARD is obsolete. Disable NO_SD_HOST_DRIVE instead."
#elif defined(PSU_ACTIVE_HIGH)
  #error "PSU_ACTIVE_HIGH is now PSU_ACTIVE_STATE."
#elif POWER_SUPPLY == 1
  #error "Replace POWER_SUPPLY 1 by enabling PSU_CONTROL and setting PSU_ACTIVE_STATE to 'LOW'."
#elif POWER_SUPPLY == 2
  #error "Replace POWER_SUPPLY 2 by enabling PSU_CONTROL and setting PSU_ACTIVE_STATE to 'HIGH'."
#elif defined(POWER_SUPPLY)
  #error "POWER_SUPPLY is now obsolete. Please remove it."
#elif defined(MKS_ROBIN_TFT)
  #error "MKS_ROBIN_TFT is now FSMC_GRAPHICAL_TFT."
#elif defined(SDPOWER)
  #error "SDPOWER is now SDPOWER_PIN."
#elif defined(STRING_SPLASH_LINE1) || defined(STRING_SPLASH_LINE2)
  #error "STRING_SPLASH_LINE[12] are now obsolete. Please remove them."
#elif defined(Z_PROBE_ALLEN_KEY_DEPLOY_1_X) || defined(Z_PROBE_ALLEN_KEY_STOW_1_X)
  #error "Z_PROBE_ALLEN_KEY_(DEPLOY|STOW) coordinates are now a single setting."
#elif defined(X_PROBE_OFFSET_FROM_EXTRUDER) || defined(Y_PROBE_OFFSET_FROM_EXTRUDER) || defined(Z_PROBE_OFFSET_FROM_EXTRUDER)
  #error "[XYZ]_PROBE_OFFSET_FROM_EXTRUDER is now NOZZLE_TO_PROBE_OFFSET."
#elif defined(MIN_PROBE_X) || defined(MIN_PROBE_Y) || defined(MAX_PROBE_X) || defined(MAX_PROBE_Y)
  #error "(MIN|MAX)_PROBE_[XY] are now calculated at runtime. Please remove them."
#elif defined(Z_STEPPER_ALIGN_X) || defined(Z_STEPPER_ALIGN_X)
  #error "Z_STEPPER_ALIGN_X and Z_STEPPER_ALIGN_Y are now combined as Z_STEPPER_ALIGN_XY."
#elif defined(JUNCTION_DEVIATION)
  #error "JUNCTION_DEVIATION is no longer required. (See CLASSIC_JERK). Please remove it."
#elif defined(BABYSTEP_MULTIPLICATOR)
  #error "BABYSTEP_MULTIPLICATOR is now BABYSTEP_MULTIPLICATOR_[XY|Z]."
#elif defined(LULZBOT_TOUCH_UI)
  #error "LULZBOT_TOUCH_UI is now TOUCH_UI_FTDI_EVE."
#elif defined(PS_DEFAULT_OFF)
  #error "PS_DEFAULT_OFF is now PSU_DEFAULT_OFF."
#elif defined(FILAMENT_UNLOAD_RETRACT_LENGTH)
  #error "FILAMENT_UNLOAD_RETRACT_LENGTH is now FILAMENT_UNLOAD_PURGE_RETRACT."
#elif defined(FILAMENT_UNLOAD_DELAY)
  #error "FILAMENT_UNLOAD_DELAY is now FILAMENT_UNLOAD_PURGE_DELAY."
#elif defined(HOME_USING_SPREADCYCLE)
  #error "HOME_USING_SPREADCYCLE is now obsolete. Please remove it."
#elif defined(DGUS_LCD)
  #error "DGUS_LCD is now DGUS_LCD_UI_(ORIGIN|FYSETC|HIPRECY)."
#elif defined(DGUS_SERIAL_PORT)
  #error "DGUS_SERIAL_PORT is now LCD_SERIAL_PORT."
#elif defined(DGUS_BAUDRATE)
  #error "DGUS_BAUDRATE is now LCD_BAUDRATE."
#elif defined(DGUS_STATS_RX_BUFFER_OVERRUNS)
  #error "DGUS_STATS_RX_BUFFER_OVERRUNS is now STATS_RX_BUFFER_OVERRUNS."
#elif defined(ANYCUBIC_LCD_SERIAL_PORT)
  #error "ANYCUBIC_LCD_SERIAL_PORT is now LCD_SERIAL_PORT."
#elif defined(INTERNAL_SERIAL_PORT)
  #error "INTERNAL_SERIAL_PORT is now MMU2_SERIAL_PORT."
#elif defined(X_DUAL_ENDSTOPS_ADJUSTMENT) || defined(Y_DUAL_ENDSTOPS_ADJUSTMENT) || defined(Z_DUAL_ENDSTOPS_ADJUSTMENT)
  #error "[XYZ]_DUAL_ENDSTOPS_ADJUSTMENT is now [XYZ]2_ENDSTOP_ADJUSTMENT."
#elif defined(Z_TRIPLE_ENDSTOPS_ADJUSTMENT2) || defined(Z_TRIPLE_ENDSTOPS_ADJUSTMENT3)
  #error "Z_TRIPLE_ENDSTOPS_ADJUSTMENT[23] is now Z[23]_ENDSTOP_ADJUSTMENT."
#elif defined(Z_QUAD_ENDSTOPS_ADJUSTMENT2) || defined(Z_QUAD_ENDSTOPS_ADJUSTMENT3) || defined(Z_QUAD_ENDSTOPS_ADJUSTMENT4)
  #error "Z_QUAD_ENDSTOPS_ADJUSTMENT[234] is now Z[234]_ENDSTOP_ADJUSTMENT."
#elif defined(Z_DUAL_STEPPER_DRIVERS)
  #error "Z_DUAL_STEPPER_DRIVERS is no longer needed and should be removed."
#elif defined(Z_TRIPLE_STEPPER_DRIVERS)
  #error "Z_TRIPLE_STEPPER_DRIVERS is no longer needed and should be removed."
#elif defined(Z_QUAD_STEPPER_DRIVERS)
  #error "Z_QUAD_STEPPER_DRIVERS is no longer needed and should be removed."
#elif defined(Z_DUAL_ENDSTOPS) || defined(Z_TRIPLE_ENDSTOPS) || defined(Z_QUAD_ENDSTOPS)
  #error "Z_(DUAL|TRIPLE|QUAD)_ENDSTOPS is now Z_MULTI_ENDSTOPS."
#elif defined(DUGS_UI_MOVE_DIS_OPTION)
  #error "DUGS_UI_MOVE_DIS_OPTION is spelled DGUS_UI_MOVE_DIS_OPTION."
#elif defined(ORIG_E0_AUTO_FAN_PIN) || defined(ORIG_E1_AUTO_FAN_PIN) || defined(ORIG_E2_AUTO_FAN_PIN) || defined(ORIG_E3_AUTO_FAN_PIN) || defined(ORIG_E4_AUTO_FAN_PIN) || defined(ORIG_E5_AUTO_FAN_PIN) || defined(ORIG_E6_AUTO_FAN_PIN) || defined(ORIG_E7_AUTO_FAN_PIN)
  #error "ORIG_Ex_AUTO_FAN_PIN is now just Ex_AUTO_FAN_PIN."
#elif defined(ORIG_CHAMBER_AUTO_FAN_PIN)
  #error "ORIG_CHAMBER_AUTO_FAN_PIN is now just CHAMBER_AUTO_FAN_PIN."
#elif defined(HOMING_BACKOFF_MM)
  #error "HOMING_BACKOFF_MM is now HOMING_BACKOFF_POST_MM."
#elif defined(X_HOME_BUMP_MM) || defined(Y_HOME_BUMP_MM) || defined(Z_HOME_BUMP_MM)
  #error "[XYZ]_HOME_BUMP_MM is now HOMING_BUMP_MM."
#elif defined(DIGIPOT_I2C)
  #error "DIGIPOT_I2C is now DIGIPOT_MCP4451 (or DIGIPOT_MCP4018)."
#elif defined(TOUCH_BUTTONS)
  #error "TOUCH_BUTTONS is now TOUCH_SCREEN."
#elif defined(LCD_FULL_PIXEL_HEIGHT) || defined(LCD_FULL_PIXEL_WIDTH)
  #error "LCD_FULL_PIXEL_(WIDTH|HEIGHT) is deprecated and should be removed."
#elif defined(FSMC_UPSCALE)
  #error "FSMC_UPSCALE is now GRAPHICAL_TFT_UPSCALE."
#elif defined(ANYCUBIC_TFT_MODEL)
  #error "ANYCUBIC_TFT_MODEL is now ANYCUBIC_LCD_I3MEGA."
#elif defined(EVENT_GCODE_SD_STOP)
  #error "EVENT_GCODE_SD_STOP is now EVENT_GCODE_SD_ABORT."
#elif defined(GRAPHICAL_TFT_ROTATE_180)
  #error "GRAPHICAL_TFT_ROTATE_180 is now TFT_ROTATION set to TFT_ROTATE_180."
#elif defined(PROBE_OFFSET_START)
  #error "PROBE_OFFSET_START is now PROBE_OFFSET_WIZARD_START_Z."
#elif defined(POWER_LOSS_PULL)
  #error "POWER_LOSS_PULL is now specifically POWER_LOSS_PULL(UP|DOWN)."
#elif defined(SHORT_MANUAL_Z_MOVE)
  #error "SHORT_MANUAL_Z_MOVE is now FINE_MANUAL_MOVE, applying to Z on most printers."
#elif defined(FIL_RUNOUT_INVERTING)
  #if FIL_RUNOUT_INVERTING
    #error "FIL_RUNOUT_INVERTING true is now FIL_RUNOUT_STATE HIGH."
  #else
    #error "FIL_RUNOUT_INVERTING false is now FIL_RUNOUT_STATE LOW."
  #endif
#elif defined(ASSISTED_TRAMMING_MENU_ITEM)
  #error "ASSISTED_TRAMMING_MENU_ITEM is deprecated and should be removed."
#elif defined(UNKNOWN_Z_NO_RAISE)
  #error "UNKNOWN_Z_NO_RAISE is replaced by setting Z_IDLE_HEIGHT to Z_MAX_POS."
#elif defined(Z_AFTER_DEACTIVATE)
  #error "Z_AFTER_DEACTIVATE is replaced by Z_IDLE_HEIGHT."
#elif defined(MEATPACK)
  #error "MEATPACK is now enabled with MEATPACK_ON_SERIAL_PORT_1, MEATPACK_ON_SERIAL_PORT_2, etc."
#elif defined(CUSTOM_USER_MENUS)
  #error "CUSTOM_USER_MENUS has been replaced by CUSTOM_MENU_MAIN and CUSTOM_MENU_CONFIG."
#elif defined(MKS_LCD12864)
  #error "MKS_LCD12864 is now MKS_LCD12864A or MKS_LCD12864B."
#elif defined(DOGM_SD_PERCENT)
  #error "DOGM_SD_PERCENT is now SHOW_SD_PERCENT."
#elif defined(NEOPIXEL_BKGD_LED_INDEX)
  #error "NEOPIXEL_BKGD_LED_INDEX is now NEOPIXEL_BKGD_INDEX_FIRST."
#elif defined(TEMP_SENSOR_1_AS_REDUNDANT)
  #error "TEMP_SENSOR_1_AS_REDUNDANT is now TEMP_SENSOR_REDUNDANT, with associated TEMP_SENSOR_REDUNDANT_* config."
#elif defined(MAX_REDUNDANT_TEMP_SENSOR_DIFF)
  #error "MAX_REDUNDANT_TEMP_SENSOR_DIFF is now TEMP_SENSOR_REDUNDANT_MAX_DIFF"
#elif defined(LCD_ALEPHOBJECTS_CLCD_UI)
  #error "LCD_ALEPHOBJECTS_CLCD_UI is now LCD_LULZBOT_CLCD_UI."
#elif defined(MIN_ARC_SEGMENTS)
  #error "MIN_ARC_SEGMENTS is now MIN_CIRCLE_SEGMENTS."
#elif defined(ARC_SEGMENTS_PER_R)
  #error "ARC_SUPPORT no longer uses ARC_SEGMENTS_PER_R."
#elif ENABLED(ARC_SUPPORT) && (!defined(MIN_ARC_SEGMENT_MM) || !defined(MAX_ARC_SEGMENT_MM))
  #error "ARC_SUPPORT now requires MIN_ARC_SEGMENT_MM and MAX_ARC_SEGMENT_MM."
#elif defined(LASER_POWER_INLINE)
  #error "LASER_POWER_INLINE is obsolete."
#elif defined(SPINDLE_LASER_PWM)
  #error "SPINDLE_LASER_PWM (true) is now set with SPINDLE_LASER_USE_PWM (enabled)."
#elif ANY(IS_RAMPS_EEB, IS_RAMPS_EEF, IS_RAMPS_EFB, IS_RAMPS_EFF, IS_RAMPS_SF)
  #error "The IS_RAMPS_* conditionals (for heater/fan/bed pins) are now called FET_ORDER_*."
#elif defined(PROBE_TEMP_COMPENSATION)
  #error "PROBE_TEMP_COMPENSATION is now set using the PTC_PROBE, PTC_BED, PTC_HOTEND options."
#elif defined(BTC_PROBE_TEMP)
  #error "BTC_PROBE_TEMP is now PTC_PROBE_TEMP."
#elif defined(LCD_SCREEN_ROT_90)
  #error "LCD_SCREEN_ROT_90 is now LCD_SCREEN_ROTATE with a value of 90."
#elif defined(LCD_SCREEN_ROT_180)
  #error "LCD_SCREEN_ROT_180 is now LCD_SCREEN_ROTATE with a value of 180."
#elif defined(LCD_SCREEN_ROT_270)
  #error "LCD_SCREEN_ROT_270 is now LCD_SCREEN_ROTATE with a value of 270."
#elif defined(DEFAULT_LCD_BRIGHTNESS)
  #error "DEFAULT_LCD_BRIGHTNESS is now LCD_BRIGHTNESS_DEFAULT."
#elif defined(NOZZLE_PARK_X_ONLY)
  #error "NOZZLE_PARK_X_ONLY is now NOZZLE_PARK_MOVE 1."
#elif defined(NOZZLE_PARK_Y_ONLY)
  #error "NOZZLE_PARK_Y_ONLY is now NOZZLE_PARK_MOVE 2."
#elif defined(Z_STEPPER_ALIGN_KNOWN_STEPPER_POSITIONS)
  #error "Z_STEPPER_ALIGN_KNOWN_STEPPER_POSITIONS is now just Z_STEPPER_ALIGN_STEPPER_XY."
#elif defined(DWIN_CREALITY_LCD_ENHANCED)
  #error "DWIN_CREALITY_LCD_ENHANCED is now DWIN_LCD_PROUI."
#elif defined(LINEAR_AXES)
  #error "LINEAR_AXES is now NUM_AXES (to account for rotational axes)."
#elif defined(X_DUAL_STEPPER_DRIVERS)
  #error "X_DUAL_STEPPER_DRIVERS is no longer needed and should be removed."
#elif defined(Y_DUAL_STEPPER_DRIVERS)
  #error "Y_DUAL_STEPPER_DRIVERS is no longer needed and should be removed."
#elif defined(NUM_Z_STEPPER_DRIVERS)
  #error "NUM_Z_STEPPER_DRIVERS is no longer needed and should be removed."
#elif defined(LEVEL_BED_CORNERS)
  #error "LEVEL_BED_CORNERS is now LCD_BED_TRAMMING."
#elif defined(LEVEL_CORNERS_INSET_LFRB) || defined(LEVEL_CORNERS_HEIGHT) || defined(LEVEL_CORNERS_Z_HOP) || defined(LEVEL_CORNERS_USE_PROBE) || defined(LEVEL_CORNERS_PROBE_TOLERANCE) || defined(LEVEL_CORNERS_VERIFY_RAISED) || defined(LEVEL_CORNERS_AUDIO_FEEDBACK)
  #error "LEVEL_CORNERS_* settings have been renamed BED_TRAMMING_*."
#elif defined(LEVEL_CENTER_TOO)
  #error "LEVEL_CENTER_TOO is now BED_TRAMMING_INCLUDE_CENTER."
#endif

// L64xx stepper drivers have been removed
#define _L6470              0x6470
#define _L6474              0x6474
#define _L6480              0x6480
#define _POWERSTEP01        0xF00D
#if HAS_DRIVER(L6470)
  #error "L6470 stepper drivers are no longer supported in Marlin."
#elif HAS_DRIVER(L6474)
  #error "L6474 stepper drivers are no longer supported in Marlin."
#elif HAS_DRIVER(L6480)
  #error "L6480 stepper drivers are no longer supported in Marlin."
#elif HAS_DRIVER(POWERSTEP01)
  #error "POWERSTEP01 stepper drivers are no longer supported in Marlin."
#endif
#undef _L6470
#undef _L6474
#undef _L6480
#undef _POWERSTEP01

// Check AXIS_RELATIVE_MODES
constexpr float arm[] = AXIS_RELATIVE_MODES;
static_assert(COUNT(arm) == LOGICAL_AXES, "AXIS_RELATIVE_MODES must contain " _LOGICAL_AXES_STR "elements.");

/**
 * Probe temp compensation requirements
 */
#if HAS_PTC
  #if TEMP_SENSOR_PROBE && TEMP_SENSOR_BED
    #if defined(PTC_PARK_POS_X) || defined(PTC_PARK_POS_Y) || defined(PTC_PARK_POS_Z)
      #error "PTC_PARK_POS_[XYZ] is now PTC_PARK_POS (array)."
    #elif !defined(PTC_PARK_POS)
      #error "PTC_PARK_POS is required for Probe Temperature Compensation."
    #elif defined(PTC_PROBE_POS_X) || defined(PTC_PROBE_POS_Y)
      #error "PTC_PROBE_POS_[XY] is now PTC_PROBE_POS (array)."
    #elif !defined(PTC_PROBE_POS)
      #error "PTC_PROBE_POS is required for Probe Temperature Compensation."
    #endif
  #endif

  #if ENABLED(PTC_PROBE)
    #if !TEMP_SENSOR_PROBE
      #error "PTC_PROBE requires a probe with a thermistor."
    #endif
    #ifdef PTC_PROBE_START
      constexpr auto _ptc_sample_start = PTC_PROBE_START;
      constexpr decltype(_ptc_sample_start) _test_ptc_sample_start = 12.3f;
      static_assert(_test_ptc_sample_start != 12.3f, "PTC_PROBE_START must be a whole number.");
    #endif
    #ifdef PTC_PROBE_RES
      constexpr auto _ptc_sample_res = PTC_PROBE_RES;
      constexpr decltype(_ptc_sample_res) _test_ptc_sample_res = 12.3f;
      static_assert(_test_ptc_sample_res != 12.3f, "PTC_PROBE_RES must be a whole number.");
    #endif
    #if ENABLED(PTC_BED) && defined(PTC_PROBE_TEMP)
      constexpr auto _btc_probe_temp = PTC_PROBE_TEMP;
      constexpr decltype(_btc_probe_temp) _test_btc_probe_temp = 12.3f;
      static_assert(_test_btc_probe_temp != 12.3f, "PTC_PROBE_TEMP must be a whole number.");
    #endif
  #endif

  #if ENABLED(PTC_BED)
    #if !TEMP_SENSOR_BED
      #error "PTC_BED requires a bed with a thermistor."
    #endif
    #ifdef PTC_BED_START
      constexpr auto _btc_sample_start = PTC_BED_START;
      constexpr decltype(_btc_sample_start) _test_btc_sample_start = 12.3f;
      static_assert(_test_btc_sample_start != 12.3f, "PTC_BED_START must be a whole number.");
    #endif
    #ifdef PTC_BED_RES
      constexpr auto _btc_sample_res = PTC_BED_RES;
      constexpr decltype(_btc_sample_res) _test_btc_sample_res = 12.3f;
      static_assert(_test_btc_sample_res != 12.3f, "PTC_BED_RES must be a whole number.");
    #endif
  #endif

  #if ENABLED(PTC_HOTEND)
    #if EXTRUDERS != 1
      #error "PTC_HOTEND requires a single extruder."
    #endif
    #ifdef PTC_HOTEND_START
      constexpr auto _etc_sample_start = PTC_HOTEND_START;
      constexpr decltype(_etc_sample_start) _test_etc_sample_start = 12.3f;
      static_assert(_test_etc_sample_start != 12.3f, "PTC_HOTEND_START must be a whole number.");
    #endif
    #ifdef PTC_HOTEND_RES
      constexpr auto _etc_sample_res = PTC_HOTEND_RES;
      constexpr decltype(_etc_sample_res) _test_etc_sample_res = 12.3f;
      static_assert(_test_etc_sample_res != 12.3f, "PTC_HOTEND_RES must be a whole number.");
    #endif
  #endif
#endif // HAS_PTC

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
#ifndef SERIAL_PORT
  #error "SERIAL_PORT must be defined."
#elif defined(SERIAL_PORT_2) && SERIAL_PORT_2 == SERIAL_PORT
  #error "SERIAL_PORT_2 cannot be the same as SERIAL_PORT."
#elif defined(SERIAL_PORT_3)
  #ifndef SERIAL_PORT_2
    #error "Use SERIAL_PORT_2 before using SERIAL_PORT_3"
  #elif SERIAL_PORT_3 == SERIAL_PORT
    #error "SERIAL_PORT_3 cannot be the same as SERIAL_PORT."
  #elif SERIAL_PORT_3 == SERIAL_PORT_2
    #error "SERIAL_PORT_3 cannot be the same as SERIAL_PORT_2."
  #endif
#endif
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

/**
 * Multiple Stepper Drivers Per Axis
 */
#define GOOD_AXIS_PINS(A) (HAS_##A##_ENABLE && HAS_##A##_STEP && HAS_##A##_DIR)
#if HAS_X2_STEPPER && !GOOD_AXIS_PINS(X)
  #error "If X2_DRIVER_TYPE is defined, then X2 ENABLE/STEP/DIR pins are also needed."
#endif

#if HAS_DUAL_Y_STEPPERS && !GOOD_AXIS_PINS(Y)
  #error "If Y2_DRIVER_TYPE is defined, then Y2 ENABLE/STEP/DIR pins are also needed."
#endif

#if HAS_Z_AXIS
  #if NUM_Z_STEPPERS >= 2 && !GOOD_AXIS_PINS(Z2)
    #error "If Z2_DRIVER_TYPE is defined, then Z2 ENABLE/STEP/DIR pins are also needed."
  #elif NUM_Z_STEPPERS >= 3 && !GOOD_AXIS_PINS(Z3)
    #error "If Z3_DRIVER_TYPE is defined, then Z3 ENABLE/STEP/DIR pins are also needed."
  #elif NUM_Z_STEPPERS >= 4 && !GOOD_AXIS_PINS(Z4)
    #error "If Z4_DRIVER_TYPE is defined, then Z4 ENABLE/STEP/DIR pins are also needed."
  #endif
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
#elif BOTH(ENDSTOPPULLUP_IMAX, ENDSTOPPULLDOWN_IMAX)
  #error "Enable only one of ENDSTOPPULLUP_I_MAX or ENDSTOPPULLDOWN_I_MAX."
#elif BOTH(ENDSTOPPULLUP_JMAX, ENDSTOPPULLDOWN_JMAX)
  #error "Enable only one of ENDSTOPPULLUP_J_MAX or ENDSTOPPULLDOWN_J_MAX."
#elif BOTH(ENDSTOPPULLUP_KMAX, ENDSTOPPULLDOWN_KMAX)
  #error "Enable only one of ENDSTOPPULLUP_K_MAX or ENDSTOPPULLDOWN_K_MAX."
#elif BOTH(ENDSTOPPULLUP_UMAX, ENDSTOPPULLDOWN_UMAX)
  #error "Enable only one of ENDSTOPPULLUP_U_MAX or ENDSTOPPULLDOWN_U_MAX."
#elif BOTH(ENDSTOPPULLUP_VMAX, ENDSTOPPULLDOWN_VMAX)
  #error "Enable only one of ENDSTOPPULLUP_V_MAX or ENDSTOPPULLDOWN_V_MAX."
#elif BOTH(ENDSTOPPULLUP_WMAX, ENDSTOPPULLDOWN_WMAX)
  #error "Enable only one of ENDSTOPPULLUP_W_MAX or ENDSTOPPULLDOWN_W_MAX."
#elif BOTH(ENDSTOPPULLUP_XMIN, ENDSTOPPULLDOWN_XMIN)
  #error "Enable only one of ENDSTOPPULLUP_X_MIN or ENDSTOPPULLDOWN_X_MIN."
#elif BOTH(ENDSTOPPULLUP_YMIN, ENDSTOPPULLDOWN_YMIN)
  #error "Enable only one of ENDSTOPPULLUP_Y_MIN or ENDSTOPPULLDOWN_Y_MIN."
#elif BOTH(ENDSTOPPULLUP_ZMIN, ENDSTOPPULLDOWN_ZMIN)
  #error "Enable only one of ENDSTOPPULLUP_Z_MIN or ENDSTOPPULLDOWN_Z_MIN."
#elif BOTH(ENDSTOPPULLUP_IMIN, ENDSTOPPULLDOWN_IMIN)
  #error "Enable only one of ENDSTOPPULLUP_I_MIN or ENDSTOPPULLDOWN_I_MIN."
#elif BOTH(ENDSTOPPULLUP_JMIN, ENDSTOPPULLDOWN_JMIN)
  #error "Enable only one of ENDSTOPPULLUP_J_MIN or ENDSTOPPULLDOWN_J_MIN."
#elif BOTH(ENDSTOPPULLUP_KMIN, ENDSTOPPULLDOWN_KMIN)
  #error "Enable only one of ENDSTOPPULLUP_K_MIN or ENDSTOPPULLDOWN_K_MIN."
#elif BOTH(ENDSTOPPULLUP_UMIN, ENDSTOPPULLDOWN_UMIN)
  #error "Enable only one of ENDSTOPPULLUP_U_MIN or ENDSTOPPULLDOWN_U_MIN."
#elif BOTH(ENDSTOPPULLUP_VMIN, ENDSTOPPULLDOWN_VMIN)
  #error "Enable only one of ENDSTOPPULLUP_V_MIN or ENDSTOPPULLDOWN_V_MIN."
#elif BOTH(ENDSTOPPULLUP_WMIN, ENDSTOPPULLDOWN_WMIN)
  #error "Enable only one of ENDSTOPPULLUP_W_MIN or ENDSTOPPULLDOWN_W_MIN."
#endif

/**
 * LCD Info Screen Style
 */
#if LCD_INFO_SCREEN_STYLE > 0
  #if HAS_MARLINUI_U8GLIB || LCD_WIDTH < 20 || LCD_HEIGHT < 4
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
  #elif NONE(HAS_MARLINUI_HD44780, IS_TFTGLCD_PANEL)
    #error "LCD_PROGRESS_BAR only applies to HD44780 character LCD and TFTGLCD_PANEL_(SPI|I2C)."
  #elif HAS_MARLINUI_U8GLIB || IS_DWIN_MARLINUI
    #error "LCD_PROGRESS_BAR does not apply to graphical displays."
  #elif ENABLED(FILAMENT_LCD_DISPLAY)
    #error "LCD_PROGRESS_BAR and FILAMENT_LCD_DISPLAY are not fully compatible. Comment out this line to use both."
  #elif PROGRESS_MSG_EXPIRE < 0
    #error "PROGRESS_MSG_EXPIRE must be greater than or equal to 0."
  #endif
#elif ENABLED(LCD_SET_PROGRESS_MANUALLY) && NONE(HAS_MARLINUI_U8GLIB, HAS_GRAPHICAL_TFT, HAS_MARLINUI_HD44780, EXTENSIBLE_UI, HAS_DWIN_E3V2, IS_DWIN_MARLINUI)
  #error "LCD_SET_PROGRESS_MANUALLY requires LCD_PROGRESS_BAR, Character LCD, Graphical LCD, TFT, DWIN_CREALITY_LCD, DWIN_LCD_PROUI, DWIN_CREALITY_LCD_JYERSUI, DWIN_MARLINUI_*, OR EXTENSIBLE_UI."
#endif

#if ENABLED(USE_M73_REMAINING_TIME) && DISABLED(LCD_SET_PROGRESS_MANUALLY)
  #error "USE_M73_REMAINING_TIME requires LCD_SET_PROGRESS_MANUALLY"
#endif

#if !HAS_MARLINUI_MENU && ENABLED(SD_REPRINT_LAST_SELECTED_FILE)
  #error "SD_REPRINT_LAST_SELECTED_FILE currently requires a Marlin-native LCD menu."
#endif

#if ANY(HAS_MARLINUI_MENU, TOUCH_UI_FTDI_EVE, EXTENSIBLE_UI) && !defined(MANUAL_FEEDRATE)
  #error "MANUAL_FEEDRATE is required for MarlinUI, ExtUI, or FTDI EVE Touch UI."
#endif

/**
 * Custom Boot and Status screens
 */
#if ENABLED(SHOW_CUSTOM_BOOTSCREEN) && NONE(HAS_MARLINUI_U8GLIB, TOUCH_UI_FTDI_EVE, IS_DWIN_MARLINUI)
  #error "SHOW_CUSTOM_BOOTSCREEN requires Graphical LCD or TOUCH_UI_FTDI_EVE."
#elif ENABLED(SHOW_CUSTOM_BOOTSCREEN) && DISABLED(SHOW_BOOTSCREEN)
  #error "SHOW_CUSTOM_BOOTSCREEN requires SHOW_BOOTSCREEN."
#elif ENABLED(CUSTOM_STATUS_SCREEN_IMAGE) && !HAS_MARLINUI_U8GLIB
  #error "CUSTOM_STATUS_SCREEN_IMAGE requires a 128x64 DOGM B/W Graphical LCD."
#endif

/**
 * LCD Lightweight Screen Style
 */
#if ENABLED(LIGHTWEIGHT_UI) && !IS_U8GLIB_ST7920
  #error "LIGHTWEIGHT_UI requires a U8GLIB_ST7920-based display."
#endif

/**
 * SD Card Settings
 */
#if ALL(SDSUPPORT, HAS_SD_DETECT, SD_CONNECTION_TYPICAL, ELB_FULL_GRAPHIC_CONTROLLER, HAS_MARLINUI_MENU) && SD_DETECT_STATE == LOW
  #error "SD_DETECT_STATE must be set HIGH for SD on the ELB_FULL_GRAPHIC_CONTROLLER."
#endif
#undef SD_CONNECTION_TYPICAL

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

#if defined(EVENT_GCODE_SD_ABORT) && DISABLED(NOZZLE_PARK_FEATURE)
  static_assert(nullptr == strstr(EVENT_GCODE_SD_ABORT, "G27"), "NOZZLE_PARK_FEATURE is required to use G27 in EVENT_GCODE_SD_ABORT.");
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
  #elif ENABLED(BABYSTEP_XY) && EITHER(MARKFORGED_XY, MARKFORGED_YX)
    #error "BABYSTEPPING only implemented for Z axis on MarkForged."
  #elif BOTH(DELTA, BABYSTEP_XY)
    #error "BABYSTEPPING only implemented for Z axis on deltabots."
  #elif BOTH(BABYSTEP_ZPROBE_OFFSET, MESH_BED_LEVELING)
    #error "MESH_BED_LEVELING and BABYSTEP_ZPROBE_OFFSET is not a valid combination"
  #elif ENABLED(BABYSTEP_ZPROBE_OFFSET) && !HAS_BED_PROBE
    #error "BABYSTEP_ZPROBE_OFFSET requires a probe."
  #elif ENABLED(BABYSTEP_ZPROBE_GFX_OVERLAY) && NONE(HAS_MARLINUI_U8GLIB, IS_DWIN_MARLINUI)
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
  #elif HAS_PRUSA_MMU2 && NUM_RUNOUT_SENSORS != 1
      #error "NUM_RUNOUT_SENSORS must be 1 with MMU2 / MMU2S."
  #elif NUM_RUNOUT_SENSORS != 1 && NUM_RUNOUT_SENSORS != E_STEPPERS
    #error "NUM_RUNOUT_SENSORS must be either 1 or number of E steppers."
  #elif NUM_RUNOUT_SENSORS >= 8 && !PIN_EXISTS(FIL_RUNOUT8)
    #error "FIL_RUNOUT8_PIN is required with NUM_RUNOUT_SENSORS >= 8."
  #elif NUM_RUNOUT_SENSORS >= 7 && !PIN_EXISTS(FIL_RUNOUT7)
    #error "FIL_RUNOUT7_PIN is required with NUM_RUNOUT_SENSORS >= 7."
  #elif NUM_RUNOUT_SENSORS >= 6 && !PIN_EXISTS(FIL_RUNOUT6)
    #error "FIL_RUNOUT6_PIN is required with NUM_RUNOUT_SENSORS >= 6."
  #elif NUM_RUNOUT_SENSORS >= 5 && !PIN_EXISTS(FIL_RUNOUT5)
    #error "FIL_RUNOUT5_PIN is required with NUM_RUNOUT_SENSORS >= 5."
  #elif NUM_RUNOUT_SENSORS >= 4 && !PIN_EXISTS(FIL_RUNOUT4)
    #error "FIL_RUNOUT4_PIN is required with NUM_RUNOUT_SENSORS >= 4."
  #elif NUM_RUNOUT_SENSORS >= 3 && !PIN_EXISTS(FIL_RUNOUT3)
    #error "FIL_RUNOUT3_PIN is required with NUM_RUNOUT_SENSORS >= 3."
  #elif NUM_RUNOUT_SENSORS >= 2 && !PIN_EXISTS(FIL_RUNOUT2)
    #error "FIL_RUNOUT2_PIN is required with NUM_RUNOUT_SENSORS >= 2."
  #elif BOTH(FIL_RUNOUT1_PULLUP, FIL_RUNOUT1_PULLDOWN)
    #error "You can't enable FIL_RUNOUT1_PULLUP and FIL_RUNOUT1_PULLDOWN at the same time."
  #elif BOTH(FIL_RUNOUT2_PULLUP, FIL_RUNOUT2_PULLDOWN)
    #error "You can't enable FIL_RUNOUT2_PULLUP and FIL_RUNOUT2_PULLDOWN at the same time."
  #elif BOTH(FIL_RUNOUT3_PULLUP, FIL_RUNOUT3_PULLDOWN)
    #error "You can't enable FIL_RUNOUT3_PULLUP and FIL_RUNOUT3_PULLDOWN at the same time."
  #elif BOTH(FIL_RUNOUT4_PULLUP, FIL_RUNOUT4_PULLDOWN)
    #error "You can't enable FIL_RUNOUT4_PULLUP and FIL_RUNOUT4_PULLDOWN at the same time."
  #elif BOTH(FIL_RUNOUT5_PULLUP, FIL_RUNOUT5_PULLDOWN)
    #error "You can't enable FIL_RUNOUT5_PULLUP and FIL_RUNOUT5_PULLDOWN at the same time."
  #elif BOTH(FIL_RUNOUT6_PULLUP, FIL_RUNOUT6_PULLDOWN)
    #error "You can't enable FIL_RUNOUT6_PULLUP and FIL_RUNOUT6_PULLDOWN at the same time."
  #elif BOTH(FIL_RUNOUT7_PULLUP, FIL_RUNOUT7_PULLDOWN)
    #error "You can't enable FIL_RUNOUT7_PULLUP and FIL_RUNOUT7_PULLDOWN at the same time."
  #elif BOTH(FIL_RUNOUT8_PULLUP, FIL_RUNOUT8_PULLDOWN)
    #error "You can't enable FIL_RUNOUT8_PULLUP and FIL_RUNOUT8_PULLDOWN at the same time."
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
    #error "ADVANCED_PAUSE_FEATURE requires a supported LCD controller (or EMERGENCY_PARSER)."
  #elif DISABLED(NOZZLE_PARK_FEATURE)
    #error "ADVANCED_PAUSE_FEATURE requires NOZZLE_PARK_FEATURE."
  #elif !defined(FILAMENT_UNLOAD_PURGE_FEEDRATE)
    #error "ADVANCED_PAUSE_FEATURE requires FILAMENT_UNLOAD_PURGE_FEEDRATE."
  #elif ENABLED(EXTRUDER_RUNOUT_PREVENT)
    #error "EXTRUDER_RUNOUT_PREVENT is incompatible with ADVANCED_PAUSE_FEATURE."
  #elif ENABLED(PARK_HEAD_ON_PAUSE) && NONE(SDSUPPORT, IS_NEWPANEL, EMERGENCY_PARSER)
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
 * Instant Freeze
 */
#if ENABLED(FREEZE_FEATURE) && !(PIN_EXISTS(FREEZE) && defined(FREEZE_STATE))
  #error "FREEZE_FEATURE requires both FREEZE_PIN and FREEZE_STATE."
#endif

/**
 * Individual axis homing is useless for DELTAS
 */
#if BOTH(INDIVIDUAL_AXIS_HOMING_MENU, DELTA)
  #error "INDIVIDUAL_AXIS_HOMING_MENU is incompatible with DELTA kinematics."
#endif

/**
 * Sanity checking for all Průša MMU
 */
#ifdef SNMM
  #error "SNMM is obsolete. Define MMU_MODEL as PRUSA_MMU1 instead."
#elif ENABLED(MK2_MULTIPLEXER)
  #error "MK2_MULTIPLEXER is obsolete. Define MMU_MODEL as PRUSA_MMU1 instead."
#elif ENABLED(PRUSA_MMU2)
  #error "PRUSA_MMU2 is obsolete. Define MMU_MODEL as PRUSA_MMU2 instead."
#elif ENABLED(PRUSA_MMU2_S_MODE)
  #error "PRUSA_MMU2_S_MODE is obsolete. Define MMU_MODEL as PRUSA_MMU2S instead."
#elif ENABLED(SMUFF_EMU_MMU2)
  #error "SMUFF_EMU_MMU2 is obsolete. Define MMU_MODEL as EXTENDABLE_EMU_MMU2 instead."
#elif ENABLED(SMUFF_EMU_MMU2S)
  #error "SMUFF_EMU_MMU2S is obsolete. Define MMU_MODEL as EXTENDABLE_EMU_MMU2S instead."
#endif

/**
 * Multi-Material-Unit 2 / EXTENDABLE_EMU_MMU2 requirements
 */
#if HAS_PRUSA_MMU2
  #if !HAS_EXTENDABLE_MMU && EXTRUDERS != 5
    #undef SINGLENOZZLE
    #error "PRUSA_MMU2(S) requires exactly 5 EXTRUDERS. Please update your Configuration."
  #elif HAS_EXTENDABLE_MMU && EXTRUDERS > 15
    #error "EXTRUDERS is too large for MMU(S) emulation mode. The maximum value is 15."
  #elif DISABLED(NOZZLE_PARK_FEATURE)
    #error "PRUSA_MMU2(S) requires NOZZLE_PARK_FEATURE. Enable it to continue."
  #elif HAS_PRUSA_MMU2S && DISABLED(FILAMENT_RUNOUT_SENSOR)
    #error "PRUSA_MMU2S requires FILAMENT_RUNOUT_SENSOR. Enable it to continue."
  #elif ENABLED(MMU_EXTRUDER_SENSOR) && DISABLED(FILAMENT_RUNOUT_SENSOR)
    #error "MMU_EXTRUDER_SENSOR requires FILAMENT_RUNOUT_SENSOR. Enable it to continue."
  #elif ENABLED(MMU_EXTRUDER_SENSOR) && !HAS_MARLINUI_MENU
    #error "MMU_EXTRUDER_SENSOR requires an LCD supporting MarlinUI."
  #elif ENABLED(MMU2_MENUS) && !HAS_MARLINUI_MENU
    #error "MMU2_MENUS requires an LCD supporting MarlinUI."
  #elif DISABLED(ADVANCED_PAUSE_FEATURE)
    static_assert(nullptr == strstr(MMU2_FILAMENT_RUNOUT_SCRIPT, "M600"), "ADVANCED_PAUSE_FEATURE is required to use M600 with PRUSA_MMU2(S) / HAS_EXTENDABLE_MMU(S).");
  #endif
#endif

/**
 * Options only for EXTRUDERS > 1
 */
#if HAS_MULTI_EXTRUDER

  #if HAS_EXTENDABLE_MMU
    #define MAX_EXTRUDERS 15
  #else
    #define MAX_EXTRUDERS  8
  #endif
  static_assert(EXTRUDERS <= MAX_EXTRUDERS, "Marlin supports a maximum of " STRINGIFY(MAX_EXTRUDERS) " EXTRUDERS.");
  #undef MAX_EXTRUDERS

  #if ENABLED(HEATERS_PARALLEL)
    #error "EXTRUDERS must be 1 with HEATERS_PARALLEL."
  #endif

  #if ENABLED(STATUS_HOTEND_INVERTED) && NONE(STATUS_HOTEND_NUMBERLESS, STATUS_HOTEND_ANIM)
    #error "With multiple hotends STATUS_HOTEND_INVERTED requires STATUS_HOTEND_ANIM or STATUS_HOTEND_NUMBERLESS."
  #endif

  #if ENABLED(TOOLCHANGE_FILAMENT_SWAP)
    #ifndef TOOLCHANGE_FS_LENGTH
      #error "TOOLCHANGE_FILAMENT_SWAP requires TOOLCHANGE_FS_LENGTH."
    #elif !defined(TOOLCHANGE_FS_RETRACT_SPEED)
      #error "TOOLCHANGE_FILAMENT_SWAP requires TOOLCHANGE_FS_RETRACT_SPEED."
    #elif !defined(TOOLCHANGE_FS_PRIME_SPEED)
      #error "TOOLCHANGE_FILAMENT_SWAP requires TOOLCHANGE_FS_PRIME_SPEED."
    #endif
  #endif

  #if ENABLED(TOOLCHANGE_PARK)
    #ifndef TOOLCHANGE_PARK_XY
      #error "TOOLCHANGE_PARK requires TOOLCHANGE_PARK_XY."
    #elif !defined(TOOLCHANGE_PARK_XY_FEEDRATE)
      #error "TOOLCHANGE_PARK requires TOOLCHANGE_PARK_XY_FEEDRATE."
    #endif
  #endif

  #ifndef TOOLCHANGE_ZRAISE
    #error "TOOLCHANGE_ZRAISE required for EXTRUDERS > 1."
  #endif

#elif HAS_PRUSA_MMU1 || HAS_EXTENDABLE_MMU

  #error "Multi-Material-Unit requires 2 or more EXTRUDERS."

#elif ENABLED(SINGLENOZZLE)

  #error "SINGLENOZZLE requires 2 or more EXTRUDERS."
  #if ENABLED(PID_PARAMS_PER_HOTEND)
    #error "PID_PARAMS_PER_HOTEND must be disabled when using any SINGLENOZZLE extruder."
  #endif

#endif

/**
 * A Dual Nozzle carriage with switching servo
 */
#if ENABLED(SWITCHING_NOZZLE)
  #if ENABLED(DUAL_X_CARRIAGE)
    #error "SWITCHING_NOZZLE and DUAL_X_CARRIAGE are incompatible."
  #elif ENABLED(SINGLENOZZLE)
    #error "SWITCHING_NOZZLE and SINGLENOZZLE are incompatible."
  #elif HAS_PRUSA_MMU2
    #error "SWITCHING_NOZZLE and PRUSA_MMU2(S) are incompatible."
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
  #if HAS_MULTI_EXTRUDER
    #error "For MIXING_EXTRUDER set MIXING_STEPPERS > 1 instead of EXTRUDERS > 1."
  #elif MIXING_STEPPERS < 2
    #error "You must set MIXING_STEPPERS >= 2 for a mixing extruder."
  #elif ENABLED(FILAMENT_WIDTH_SENSOR)
    #error "MIXING_EXTRUDER is incompatible with FILAMENT_WIDTH_SENSOR. Comment out this line to use it anyway."
  #elif ENABLED(SWITCHING_EXTRUDER)
    #error "Please select either MIXING_EXTRUDER or SWITCHING_EXTRUDER, not both."
  #elif ENABLED(SINGLENOZZLE)
    #error "MIXING_EXTRUDER is incompatible with SINGLENOZZLE."
  #elif ENABLED(DISABLE_INACTIVE_EXTRUDER)
    #error "MIXING_EXTRUDER is incompatible with DISABLE_INACTIVE_EXTRUDER."
  #elif HAS_FILAMENT_RUNOUT_DISTANCE
    #error "MIXING_EXTRUDER is incompatible with FILAMENT_RUNOUT_DISTANCE_MM."
  #endif
#endif

/**
 * Dual E Steppers requirements
 */
#if ENABLED(E_DUAL_STEPPER_DRIVERS)
  #if EXTRUDERS > 1
    #error "E_DUAL_STEPPER_DRIVERS can only be used with EXTRUDERS set to 1."
  #elif ENABLED(MIXING_EXTRUDER)
    #error "E_DUAL_STEPPER_DRIVERS is incompatible with MIXING_EXTRUDER."
  #elif ENABLED(SWITCHING_EXTRUDER)
    #error "E_DUAL_STEPPER_DRIVERS is incompatible with SWITCHING_EXTRUDER."
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
  #elif ENABLED(DIRECT_STEPPING)
    #error "DIRECT_STEPPING is incompatible with LIN_ADVANCE. Enable in external planner if possible."
  #elif NONE(HAS_JUNCTION_DEVIATION, ALLOW_LOW_EJERK) && defined(DEFAULT_EJERK)
    static_assert(DEFAULT_EJERK >= 10, "It is strongly recommended to set DEFAULT_EJERK >= 10 when using LIN_ADVANCE. Enable ALLOW_LOW_EJERK to bypass this alert (e.g., for direct drive).");
  #endif
#endif

/**
 * Special tool-changing options
 */
#if MANY(SINGLENOZZLE, DUAL_X_CARRIAGE, PARKING_EXTRUDER, MAGNETIC_PARKING_EXTRUDER, SWITCHING_TOOLHEAD, MAGNETIC_SWITCHING_TOOLHEAD, ELECTROMAGNETIC_SWITCHING_TOOLHEAD)
  #error "Please select only one of SINGLENOZZLE, DUAL_X_CARRIAGE, PARKING_EXTRUDER, MAGNETIC_PARKING_EXTRUDER, SWITCHING_TOOLHEAD, MAGNETIC_SWITCHING_TOOLHEAD, or ELECTROMAGNETIC_SWITCHING_TOOLHEAD."
#endif

/**
 * (Magnetic) Parking Extruder requirements
 */
#if EITHER(PARKING_EXTRUDER, MAGNETIC_PARKING_EXTRUDER)
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
 * Generic Switching Toolhead requirements
 */
#if ANY(SWITCHING_TOOLHEAD, MAGNETIC_SWITCHING_TOOLHEAD, ELECTROMAGNETIC_SWITCHING_TOOLHEAD)
  constexpr float thpx[] = SWITCHING_TOOLHEAD_X_POS;
  static_assert(COUNT(thpx) == EXTRUDERS, "SWITCHING_TOOLHEAD_X_POS must be an array EXTRUDERS long.");
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
 * Magnetic / Electromagnetic Switching Toolhead requirements
 */
#if EITHER(MAGNETIC_SWITCHING_TOOLHEAD, ELECTROMAGNETIC_SWITCHING_TOOLHEAD)
  #ifndef SWITCHING_TOOLHEAD_Y_POS
    #error "(ELECTRO)?MAGNETIC_SWITCHING_TOOLHEAD requires SWITCHING_TOOLHEAD_Y_POS"
  #elif !defined(SWITCHING_TOOLHEAD_X_POS)
    #error "(ELECTRO)?MAGNETIC_SWITCHING_TOOLHEAD requires SWITCHING_TOOLHEAD_X_POS"
  #elif !defined(SWITCHING_TOOLHEAD_Y_CLEAR)
    #error "(ELECTRO)?MAGNETIC_SWITCHING_TOOLHEAD requires SWITCHING_TOOLHEAD_Y_CLEAR."
  #endif
#endif

/**
 * Electromagnetic Switching Toolhead requirements
 */
#if ENABLED(ELECTROMAGNETIC_SWITCHING_TOOLHEAD)
  #if ENABLED(EXT_SOLENOID)
    #error "ELECTROMAGNETIC_SWITCHING_TOOLHEAD and EXT_SOLENOID are incompatible. (Pins are used twice.)"
  #elif !PIN_EXISTS(SOL0)
    #error "ELECTROMAGNETIC_SWITCHING_TOOLHEAD requires SOL0_PIN."
  #elif !defined(SWITCHING_TOOLHEAD_Z_HOP)
    #error "ELECTROMAGNETIC_SWITCHING_TOOLHEAD requires SWITCHING_TOOLHEAD_Z_HOP."
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
 * Limited user-controlled fans
 */
#if NUM_M106_FANS > FAN_COUNT
  #error "The selected board doesn't support enough user-controlled fans. Reduce NUM_M106_FANS."
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
#if ENABLED(DEACTIVATE_SERVOS_AFTER_MOVE) && NONE(HAS_Z_SERVO_PROBE, POLARGRAPH) && !defined(SWITCHING_NOZZLE_SERVO_NR) && !defined(SWITCHING_EXTRUDER_SERVO_NR) && !defined(SWITCHING_TOOLHEAD_SERVO_NR)
  #error "Z_PROBE_SERVO_NR, switching nozzle, switching toolhead, switching extruder, or POLARGRAPH is required for DEACTIVATE_SERVOS_AFTER_MOVE."
#endif

/**
 * Required LCD language
 */
#if HAS_MARLINUI_HD44780 && !defined(DISPLAY_CHARSET_HD44780)
  #error "You must set DISPLAY_CHARSET_HD44780 to JAPANESE, WESTERN or CYRILLIC for your LCD controller."
#endif

/**
 * Extruder temperature control algorithm - There can be only one!
 */
#if BOTH(PIDTEMP, MPCTEMP)
  #error "Only enable PIDTEMP or MPCTEMP, but not both."
#endif

#if ENABLED(MPC_INCLUDE_FAN)
  #if FAN_COUNT < 1
    #error "MPC_INCLUDE_FAN requires at least one fan."
  #endif
  #if FAN_COUNT < HOTENDS
    #if COUNT_ENABLED(MPC_FAN_0_ALL_HOTENDS, MPC_FAN_0_ACTIVE_HOTEND) > 1
      #error "Enable either MPC_FAN_0_ALL_HOTENDS or MPC_FAN_0_ACTIVE_HOTEND, not both."
    #elif NONE(MPC_FAN_0_ALL_HOTENDS, MPC_FAN_0_ACTIVE_HOTEND)
      #error "MPC_INCLUDE_FAN requires MPC_FAN_0_ALL_HOTENDS or MPC_FAN_0_ACTIVE_HOTEND for one fan with multiple hotends."
    #endif
  #endif
#endif

/**
 * Bed Heating Options - PID vs Limit Switching
 */
#if BOTH(PIDTEMPBED, BED_LIMIT_SWITCHING)
  #error "To use BED_LIMIT_SWITCHING you must disable PIDTEMPBED."
#endif

/**
 * Synchronous M106/M107 checks
 */
#if ENABLED(LASER_SYNCHRONOUS_M106_M107)
  #if FAN_KICKSTART_TIME
    #error "FAN_KICKSTART_TIME must be 0 with LASER_SYNCHRONOUS_M106_M107 (because the laser will always come on at FULL power)."
  #elif FAN_MIN_PWM
    #error "FAN_MIN_PWM must be 0 with LASER_SYNCHRONOUS_M106_M107 (otherwise the laser will never turn OFF)."
  #endif
#endif

/**
 * Chamber Heating Options - PID vs Limit Switching
 */
#if BOTH(PIDTEMPCHAMBER, CHAMBER_LIMIT_SWITCHING)
  #error "To use CHAMBER_LIMIT_SWITCHING you must disable PIDTEMPCHAMBER."
#endif

/**
 * Features that require a min/max/specific NUM_AXES
 */
#if HAS_LEVELING && !HAS_Z_AXIS
  #error "Leveling in Marlin requires three or more axes, with Z as the vertical axis."
#elif ENABLED(CNC_WORKSPACE_PLANES) && !HAS_Z_AXIS
  #error "CNC_WORKSPACE_PLANES currently requires NUM_AXES >= 3"
#elif ENABLED(DIRECT_STEPPING) && NUM_AXES > XYZ
  #error "DIRECT_STEPPING currently requires NUM_AXES 3"
#elif ENABLED(FOAMCUTTER_XYUV) && NUM_AXES < 5
  #error "FOAMCUTTER_XYUV requires NUM_AXES >= 5."
#elif ENABLED(LINEAR_ADVANCE) && HAS_I_AXIS
  #error "LINEAR_ADVANCE currently requires NUM_AXES <= 3."
#endif

/**
 * Allow only extra axis codes that do not conflict with G-code parameter names
 */
#if HAS_I_AXIS
  #if !defined(I_MIN_POS) || !defined(I_MAX_POS)
    #error "I_MIN_POS and I_MAX_POS are required with NUM_AXES >= 4."
  #elif !defined(I_HOME_DIR)
    #error "I_HOME_DIR is required with NUM_AXES >= 4."
  #elif HAS_I_ENABLE && !defined(I_ENABLE_ON)
    #error "I_ENABLE_ON is required for your I driver with NUM_AXES >= 4."
  #endif
#endif
#if HAS_J_AXIS
  #if AXIS5_NAME == AXIS4_NAME
    #error "AXIS5_NAME must be unique."
  #elif ENABLED(AXIS5_ROTATES) && DISABLED(AXIS4_ROTATES)
    #error "AXIS5_ROTATES requires AXIS4_ROTATES."
  #elif !defined(J_MIN_POS) || !defined(J_MAX_POS)
    #error "J_MIN_POS and J_MAX_POS are required with NUM_AXES >= 5."
  #elif !defined(J_HOME_DIR)
    #error "J_HOME_DIR is required with NUM_AXES >= 5."
  #elif HAS_J_ENABLE && !defined(J_ENABLE_ON)
    #error "J_ENABLE_ON is required for your J driver with NUM_AXES >= 5."
  #endif
#endif
#if HAS_K_AXIS
  #if AXIS6_NAME == AXIS5_NAME || AXIS6_NAME == AXIS4_NAME
    #error "AXIS6_NAME must be unique."
  #elif ENABLED(AXIS6_ROTATES) && DISABLED(AXIS5_ROTATES)
    #error "AXIS6_ROTATES requires AXIS5_ROTATES."
  #elif !defined(K_MIN_POS) || !defined(K_MAX_POS)
    #error "K_MIN_POS and K_MAX_POS are required with NUM_AXES >= 6."
  #elif !defined(K_HOME_DIR)
    #error "K_HOME_DIR is required with NUM_AXES >= 6."
  #elif HAS_K_ENABLE && !defined(K_ENABLE_ON)
    #error "K_ENABLE_ON is required for your K driver with NUM_AXES >= 6."
  #endif
#endif
#if HAS_U_AXIS
  #if AXIS7_NAME == AXIS6_NAME || AXIS7_NAME == AXIS5_NAME || AXIS7_NAME == AXIS4_NAME
    #error "AXIS7_NAME must be unique."
  #elif ENABLED(AXIS7_ROTATES) && DISABLED(AXIS6_ROTATES)
    #error "AXIS7_ROTATES requires AXIS6_ROTATES."
  #elif !defined(U_MIN_POS) || !defined(U_MAX_POS)
    #error "U_MIN_POS and U_MAX_POS are required with NUM_AXES >= 7."
  #elif !defined(U_HOME_DIR)
    #error "U_HOME_DIR is required with NUM_AXES >= 7."
  #elif HAS_U_ENABLE && !defined(U_ENABLE_ON)
    #error "U_ENABLE_ON is required for your U driver with NUM_AXES >= 7."
  #endif
#endif
#if HAS_V_AXIS
  #if AXIS8_NAME == AXIS7_NAME || AXIS8_NAME == AXIS6_NAME || AXIS8_NAME == AXIS5_NAME || AXIS8_NAME == AXIS4_NAME
    #error "AXIS8_NAME must be unique."
  #elif ENABLED(AXIS8_ROTATES) && DISABLED(AXIS7_ROTATES)
    #error "AXIS8_ROTATES requires AXIS7_ROTATES."
  #elif !defined(V_MIN_POS) || !defined(V_MAX_POS)
    #error "V_MIN_POS and V_MAX_POS are required with NUM_AXES >= 8."
  #elif !defined(V_HOME_DIR)
    #error "V_HOME_DIR is required with NUM_AXES >= 8."
  #elif HAS_V_ENABLE && !defined(V_ENABLE_ON)
    #error "V_ENABLE_ON is required for your V driver with NUM_AXES >= 8."
  #endif
#endif
#if HAS_W_AXIS
  #if AXIS9_NAME == AXIS8_NAME || AXIS9_NAME == AXIS7_NAME || AXIS9_NAME == AXIS6_NAME || AXIS9_NAME == AXIS5_NAME || AXIS9_NAME == AXIS4_NAME
    #error "AXIS9_NAME must be unique."
  #elif ENABLED(AXIS9_ROTATES) && DISABLED(AXIS8_ROTATES)
    #error "AXIS9_ROTATES requires AXIS8_ROTATES."
  #elif !defined(W_MIN_POS) || !defined(W_MAX_POS)
    #error "W_MIN_POS and W_MAX_POS are required with NUM_AXES >= 9."
  #elif !defined(W_HOME_DIR)
    #error "W_HOME_DIR is required with NUM_AXES >= 9."
  #elif HAS_W_ENABLE && !defined(W_ENABLE_ON)
    #error "W_ENABLE_ON is required for your W driver with NUM_AXES >= 9."
  #endif
#endif

/**
 * Kinematics
 */

/**
 * Allow only one kinematic type to be defined
 */
#if MANY(DELTA, MORGAN_SCARA, MP_SCARA, AXEL_TPARA, COREXY, COREXZ, COREYZ, COREYX, COREZX, COREZY, MARKFORGED_XY, MARKFORGED_YX, ARTICULATED_ROBOT_ARM, FOAMCUTTER_XYUV)
  #error "Please enable only one of DELTA, MORGAN_SCARA, MP_SCARA, AXEL_TPARA, COREXY, COREXZ, COREYZ, COREYX, COREZX, COREZY, MARKFORGED_XY, MARKFORGED_YX, ARTICULATED_ROBOT_ARM, or FOAMCUTTER_XYUV."
#endif

/**
 * Delta requirements
 */
#if ENABLED(DELTA)
  #if NONE(USE_XMAX_PLUG, USE_YMAX_PLUG, USE_ZMAX_PLUG)
    #error "You probably want to use Max Endstops for DELTA!"
  #elif ENABLED(ENABLE_LEVELING_FADE_HEIGHT) && DISABLED(AUTO_BED_LEVELING_BILINEAR) && !UBL_SEGMENTED
    #error "ENABLE_LEVELING_FADE_HEIGHT on DELTA requires AUTO_BED_LEVELING_BILINEAR or AUTO_BED_LEVELING_UBL."
  #elif ENABLED(DELTA_AUTO_CALIBRATION) && !(HAS_BED_PROBE || HAS_MARLINUI_MENU)
    #error "DELTA_AUTO_CALIBRATION requires a probe or LCD Controller."
  #elif ENABLED(DELTA_CALIBRATION_MENU) && !HAS_MARLINUI_MENU
    #error "DELTA_CALIBRATION_MENU requires an LCD Controller."
  #elif ABL_USES_GRID
    #if ((GRID_MAX_POINTS_X) & 1) == 0 || ((GRID_MAX_POINTS_Y) & 1) == 0
      #error "DELTA requires GRID_MAX_POINTS_X and GRID_MAX_POINTS_Y to be odd numbers."
    #elif (GRID_MAX_POINTS_X) < 3
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
 * Some things should not be used on Belt Printers
 */
#if BOTH(BELTPRINTER, HAS_LEVELING)
  #error "Bed Leveling is not compatible with BELTPRINTER."
#endif

/**
 * Probes
 */

/**
 * Allow only one probe option to be defined
 */
#if 1 < 0 \
  + (DISABLED(BLTOUCH) && HAS_Z_SERVO_PROBE) \
  + COUNT_ENABLED(PROBE_MANUALLY, BLTOUCH, BD_SENSOR, FIX_MOUNTED_PROBE, NOZZLE_AS_PROBE, TOUCH_MI_PROBE, SOLENOID_PROBE, Z_PROBE_ALLEN_KEY, Z_PROBE_SLED, RACK_AND_PINION_PROBE, SENSORLESS_PROBING, MAGLEV4, MAG_MOUNTED_PROBE)
  #error "Please enable only one probe option: PROBE_MANUALLY, SENSORLESS_PROBING, BLTOUCH, BD_SENSOR, FIX_MOUNTED_PROBE, NOZZLE_AS_PROBE, TOUCH_MI_PROBE, SOLENOID_PROBE, Z_PROBE_ALLEN_KEY, Z_PROBE_SLED, MAGLEV4, MAG_MOUNTED_PROBE or Z Servo."
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
      #error "SOLENOID_PROBE requires SOL1_PIN."
    #endif
  #endif

  /**
   * NUM_SERVOS is required for a Z servo probe
   */
  #if HAS_Z_SERVO_PROBE
    #if !NUM_SERVOS
      #error "NUM_SERVOS is required for a Z servo probe (Z_PROBE_SERVO_NR)."
    #elif Z_PROBE_SERVO_NR >= NUM_SERVOS
      #error "Z_PROBE_SERVO_NR must be smaller than NUM_SERVOS."
    #elif Z_PROBE_SERVO_NR == 0 && !PIN_EXISTS(SERVO0)
      #error "SERVO0_PIN must be defined for your servo or BLTOUCH probe."
    #elif Z_PROBE_SERVO_NR == 1 && !PIN_EXISTS(SERVO1)
      #error "SERVO1_PIN must be defined for your servo or BLTOUCH probe."
    #elif Z_PROBE_SERVO_NR == 2 && !PIN_EXISTS(SERVO2)
      #error "SERVO2_PIN must be defined for your servo or BLTOUCH probe."
    #elif Z_PROBE_SERVO_NR == 3 && !PIN_EXISTS(SERVO3)
      #error "SERVO3_PIN must be defined for your servo or BLTOUCH probe."
    #endif
  #endif

  #if ENABLED(BLTOUCH)

    // BLTouch can't run in 5V mode with a 3.3V probe pin
    #if ENABLED(BLTOUCH_SET_5V_MODE)
      #define _5V(P,A,B) WITHIN(P,A,B)
      #ifdef STM32F1            // STM32F103 5V-tolerant pins
        #define _IS_5V_TOLERANT(P) (_5V(P,PA8,PA15) || _5V(P,PB2,PB15) || _5V(P,PC6,PC12) || _5V(P,PD0,PD15) || _5V(P,PE0,PE15) || _5V(P,PF0,PF5) || _5V(P,PF11,PF15))
      #elif defined(ARDUINO_ARCH_SAM)
        #define _IS_5V_TOLERANT(P) 0 // Assume no 5V tolerance
      #else
        #define _IS_5V_TOLERANT(P) 1 // Assume 5V tolerance
      #endif
      #if USES_Z_MIN_PROBE_PIN
        #if !_IS_5V_TOLERANT(Z_MIN_PROBE_PIN)
          #error "BLTOUCH_SET_5V_MODE is not compatible with the Z_MIN_PROBE_PIN."
        #endif
      #elif !_IS_5V_TOLERANT(Z_MIN_PIN)
        #if !MB(CHITU3D_V6)
          #error "BLTOUCH_SET_5V_MODE is not compatible with the Z_MIN_PIN."
        #endif
      #endif
      #undef _IS_5V_TOLERANT
      #undef _5V
    #elif NONE(ONBOARD_ENDSTOPPULLUPS, ENDSTOPPULLUPS, ENDSTOPPULLUP_ZMIN, ENDSTOPPULLUP_ZMIN_PROBE)
      #if USES_Z_MIN_PROBE_PIN
        #error "BLTOUCH on Z_MIN_PROBE_PIN requires ENDSTOPPULLUP_ZMIN_PROBE, ENDSTOPPULLUPS, or BLTOUCH_SET_5V_MODE."
      #else
        #error "BLTOUCH on Z_MIN_PIN requires ENDSTOPPULLUP_ZMIN, ENDSTOPPULLUPS, or BLTOUCH_SET_5V_MODE."
      #endif
    #endif

    #if ENABLED(BLTOUCH_HS_MODE) && BLTOUCH_HS_MODE == 0
      #error "BLTOUCH_HS_MODE must now be defined as true or false, indicating the default state."
    #endif
    #if BLTOUCH_DELAY < 200
      #error "BLTOUCH_DELAY less than 200 is unsafe and is not supported."
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
   * Mag mounted probe requirements
   */
  #if BOTH(MAG_MOUNTED_PROBE, USE_PROBE_FOR_Z_HOMING) && DISABLED(Z_SAFE_HOMING)
    #error "MAG_MOUNTED_PROBE requires Z_SAFE_HOMING if it's being used to home Z."
  #endif

  /**
   * MagLev V4 probe requirements
   */
  #if ENABLED(MAGLEV4)
    #if !PIN_EXISTS(MAGLEV_TRIGGER)
      #error "MAGLEV4 requires MAGLEV_TRIGGER_PIN to be defined."
    #elif ENABLED(HOMING_Z_WITH_PROBE) && DISABLED(Z_SAFE_HOMING)
      #error "MAGLEV4 requires Z_SAFE_HOMING."
    #elif MAGLEV_TRIGGER_DELAY != 15
      #error "MAGLEV_TRIGGER_DELAY should not be changed. Comment out this line to continue."
    #endif
  #endif

  /**
   * Require pin options and pins to be defined
   */
  #if ENABLED(SENSORLESS_PROBING)
    #if ENABLED(DELTA) && !(X_SENSORLESS && Y_SENSORLESS && Z_SENSORLESS)
      #error "SENSORLESS_PROBING requires TMC2130/2160/2209/5130/5160 drivers on X, Y, and Z."
    #elif !Z_SENSORLESS
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

  /**
   * Check for improper NOZZLE_TO_PROBE_OFFSET
   */
  constexpr xyz_pos_t sanity_nozzle_to_probe_offset = NOZZLE_TO_PROBE_OFFSET;
  #if ENABLED(NOZZLE_AS_PROBE)
    static_assert(sanity_nozzle_to_probe_offset.x == 0 && sanity_nozzle_to_probe_offset.y == 0,
                  "NOZZLE_AS_PROBE requires the XY offsets in NOZZLE_TO_PROBE_OFFSET to both be 0.");
  #elif !IS_KINEMATIC
    static_assert(PROBING_MARGIN       >= 0, "PROBING_MARGIN must be >= 0.");
    static_assert(PROBING_MARGIN_BACK  >= 0, "PROBING_MARGIN_BACK must be >= 0.");
    static_assert(PROBING_MARGIN_FRONT >= 0, "PROBING_MARGIN_FRONT must be >= 0.");
    static_assert(PROBING_MARGIN_LEFT  >= 0, "PROBING_MARGIN_LEFT must be >= 0.");
    static_assert(PROBING_MARGIN_RIGHT >= 0, "PROBING_MARGIN_RIGHT must be >= 0.");
  #endif

  #define _MARGIN(A) TERN(IS_SCARA, SCARA_PRINTABLE_RADIUS, TERN(DELTA, DELTA_PRINTABLE_RADIUS, ((A##_BED_SIZE) / 2)))
  static_assert(PROBING_MARGIN       < _MARGIN(X), "PROBING_MARGIN is too large.");
  static_assert(PROBING_MARGIN_BACK  < _MARGIN(Y), "PROBING_MARGIN_BACK is too large.");
  static_assert(PROBING_MARGIN_FRONT < _MARGIN(Y), "PROBING_MARGIN_FRONT is too large.");
  static_assert(PROBING_MARGIN_LEFT  < _MARGIN(X), "PROBING_MARGIN_LEFT is too large.");
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

  #if ENABLED(PROBE_ACTIVATION_SWITCH)
    #ifndef PROBE_ACTIVATION_SWITCH_STATE
      #error "PROBE_ACTIVATION_SWITCH_STATE is required for PROBE_ACTIVATION_SWITCH."
    #elif !PIN_EXISTS(PROBE_ACTIVATION_SWITCH)
      #error "A PROBE_ACTIVATION_SWITCH_PIN is required for PROBE_ACTIVATION_SWITCH."
    #endif
  #endif

#else

  /**
   * Require some kind of probe for bed leveling and probe testing
   */
  #if HAS_ABL_NOT_UBL && !PROBE_SELECTED
    #error "Auto Bed Leveling requires either PROBE_MANUALLY, SENSORLESS_PROBING, or a real probe."
  #endif

  #if ENABLED(Z_MIN_PROBE_REPEATABILITY_TEST)
    #error "Z_MIN_PROBE_REPEATABILITY_TEST requires a real probe."
  #endif

#endif

#if ENABLED(LCD_BED_TRAMMING)
  #ifndef BED_TRAMMING_INSET_LFRB
    #error "LCD_BED_TRAMMING requires BED_TRAMMING_INSET_LFRB values."
  #elif ENABLED(BED_TRAMMING_USE_PROBE)
    #if !HAS_BED_PROBE
      #error "BED_TRAMMING_USE_PROBE requires a real probe."
    #elif ENABLED(SENSORLESS_PROBING)
      #error "BED_TRAMMING_USE_PROBE is incompatible with SENSORLESS_PROBING."
    #endif
  #endif
#endif

/**
 * Allow only one bed leveling option to be defined
 */
#if MANY(AUTO_BED_LEVELING_LINEAR, AUTO_BED_LEVELING_3POINT, AUTO_BED_LEVELING_BILINEAR, AUTO_BED_LEVELING_UBL, MESH_BED_LEVELING)
  #error "Select only one of: MESH_BED_LEVELING, AUTO_BED_LEVELING_LINEAR, AUTO_BED_LEVELING_3POINT, AUTO_BED_LEVELING_BILINEAR or AUTO_BED_LEVELING_UBL."
#endif

/**
 * Bed Leveling Requirements
 */

#if ENABLED(AUTO_BED_LEVELING_UBL)

  /**
   * Unified Bed Leveling
   */

  #if IS_SCARA
    #error "AUTO_BED_LEVELING_UBL does not yet support SCARA printers."
  #elif DISABLED(EEPROM_SETTINGS)
    #error "AUTO_BED_LEVELING_UBL requires EEPROM_SETTINGS."
  #elif !WITHIN(GRID_MAX_POINTS_X, 3, 15) || !WITHIN(GRID_MAX_POINTS_Y, 3, 15)
    #error "GRID_MAX_POINTS_[XY] must be a whole number between 3 and 15."
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

  // Mesh Bed Leveling
  #if ENABLED(DELTA)
    #error "MESH_BED_LEVELING is not compatible with DELTA printers."
  #elif (GRID_MAX_POINTS_X) > 9 || (GRID_MAX_POINTS_Y) > 9
    #error "GRID_MAX_POINTS_X and GRID_MAX_POINTS_Y must be less than 10 for MBL."
  #endif

#endif

#if ALL(HAS_LEVELING, RESTORE_LEVELING_AFTER_G28, ENABLE_LEVELING_AFTER_G28)
  #error "Only enable RESTORE_LEVELING_AFTER_G28 or ENABLE_LEVELING_AFTER_G28, but not both."
#endif

#if HAS_MESH && HAS_CLASSIC_JERK
  static_assert(DEFAULT_ZJERK > 0.1, "Low DEFAULT_ZJERK values are incompatible with mesh-based leveling.");
#endif

#if ENABLED(G26_MESH_VALIDATION)
  #if !HAS_EXTRUDERS
    #error "G26_MESH_VALIDATION requires at least one extruder."
  #elif !HAS_MESH
    #error "G26_MESH_VALIDATION requires MESH_BED_LEVELING, AUTO_BED_LEVELING_BILINEAR, or AUTO_BED_LEVELING_UBL."
  #endif
#endif

#if ENABLED(MESH_EDIT_GFX_OVERLAY)
  #if DISABLED(AUTO_BED_LEVELING_UBL)
    #error "MESH_EDIT_GFX_OVERLAY requires AUTO_BED_LEVELING_UBL."
  #elif NONE(HAS_MARLINUI_U8GLIB, IS_DWIN_MARLINUI)
    #error "MESH_EDIT_GFX_OVERLAY requires a Graphical LCD."
  #endif
#endif

#if ENABLED(G29_RETRY_AND_RECOVER) && NONE(AUTO_BED_LEVELING_3POINT, AUTO_BED_LEVELING_LINEAR, AUTO_BED_LEVELING_BILINEAR)
  #error "G29_RETRY_AND_RECOVER requires AUTO_BED_LEVELING_3POINT, LINEAR, or BILINEAR."
#endif

/**
 * LCD_BED_LEVELING requirements
 */
#if ENABLED(LCD_BED_LEVELING)
  #if !HAS_MARLINUI_MENU
    #error "LCD_BED_LEVELING is not supported by the selected LCD controller."
  #elif !(ENABLED(MESH_BED_LEVELING) || HAS_ABL_NOT_UBL)
    #error "LCD_BED_LEVELING requires MESH_BED_LEVELING or AUTO_BED_LEVELING."
  #elif ENABLED(MESH_EDIT_MENU) && !HAS_MESH
    #error "MESH_EDIT_MENU requires MESH_BED_LEVELING, AUTO_BED_LEVELING_BILINEAR or AUTO_BED_LEVELING_UBL."
  #endif
#endif

#if BOTH(PREHEAT_BEFORE_PROBING, PREHEAT_BEFORE_LEVELING)
  #error "Disable PREHEAT_BEFORE_LEVELING when using PREHEAT_BEFORE_PROBING."
#endif

/**
 * Homing checks
 */
#ifndef HOMING_BUMP_MM
  #error "Required setting HOMING_BUMP_MM is missing!"
#elif !defined(HOMING_BUMP_DIVISOR)
  #error "Required setting HOMING_BUMP_DIVISOR is missing!"
#else
  constexpr float hbm[] = HOMING_BUMP_MM, hbd[] = HOMING_BUMP_DIVISOR;
  static_assert(COUNT(hbm) == NUM_AXES, "HOMING_BUMP_MM must have " _NUM_AXES_STR "elements (and no others).");
  NUM_AXIS_CODE(
    static_assert(hbm[X_AXIS] >= 0, "HOMING_BUMP_MM.X must be greater than or equal to 0."),
    static_assert(hbm[Y_AXIS] >= 0, "HOMING_BUMP_MM.Y must be greater than or equal to 0."),
    static_assert(hbm[Z_AXIS] >= 0, "HOMING_BUMP_MM.Z must be greater than or equal to 0."),
    static_assert(hbm[I_AXIS] >= 0, "HOMING_BUMP_MM.I must be greater than or equal to 0."),
    static_assert(hbm[J_AXIS] >= 0, "HOMING_BUMP_MM.J must be greater than or equal to 0."),
    static_assert(hbm[K_AXIS] >= 0, "HOMING_BUMP_MM.K must be greater than or equal to 0."),
    static_assert(hbm[U_AXIS] >= 0, "HOMING_BUMP_MM.U must be greater than or equal to 0."),
    static_assert(hbm[V_AXIS] >= 0, "HOMING_BUMP_MM.V must be greater than or equal to 0."),
    static_assert(hbm[W_AXIS] >= 0, "HOMING_BUMP_MM.W must be greater than or equal to 0.")
  );
  static_assert(COUNT(hbd) == NUM_AXES, "HOMING_BUMP_DIVISOR must have " _NUM_AXES_STR "elements (and no others).");
  NUM_AXIS_CODE(
    static_assert(hbd[X_AXIS] >= 1, "HOMING_BUMP_DIVISOR.X must be greater than or equal to 1."),
    static_assert(hbd[Y_AXIS] >= 1, "HOMING_BUMP_DIVISOR.Y must be greater than or equal to 1."),
    static_assert(hbd[Z_AXIS] >= 1, "HOMING_BUMP_DIVISOR.Z must be greater than or equal to 1."),
    static_assert(hbd[I_AXIS] >= 1, "HOMING_BUMP_DIVISOR.I must be greater than or equal to 1."),
    static_assert(hbd[J_AXIS] >= 1, "HOMING_BUMP_DIVISOR.J must be greater than or equal to 1."),
    static_assert(hbd[K_AXIS] >= 1, "HOMING_BUMP_DIVISOR.K must be greater than or equal to 1."),
    static_assert(hbd[U_AXIS] >= 1, "HOMING_BUMP_DIVISOR.U must be greater than or equal to 1."),
    static_assert(hbd[V_AXIS] >= 1, "HOMING_BUMP_DIVISOR.V must be greater than or equal to 1."),
    static_assert(hbd[W_AXIS] >= 1, "HOMING_BUMP_DIVISOR.W must be greater than or equal to 1.")
  );
#endif

#ifdef HOMING_BACKOFF_POST_MM
  constexpr float hbp[] = HOMING_BACKOFF_POST_MM;
  static_assert(COUNT(hbp) == NUM_AXES, "HOMING_BACKOFF_POST_MM must have " _NUM_AXES_STR "elements (and no others).");
  NUM_AXIS_CODE(
    static_assert(hbp[X_AXIS] >= 0, "HOMING_BACKOFF_POST_MM.X must be greater than or equal to 0."),
    static_assert(hbp[Y_AXIS] >= 0, "HOMING_BACKOFF_POST_MM.Y must be greater than or equal to 0."),
    static_assert(hbp[Z_AXIS] >= 0, "HOMING_BACKOFF_POST_MM.Z must be greater than or equal to 0."),
    static_assert(hbp[I_AXIS] >= 0, "HOMING_BACKOFF_POST_MM.I must be greater than or equal to 0."),
    static_assert(hbp[J_AXIS] >= 0, "HOMING_BACKOFF_POST_MM.J must be greater than or equal to 0."),
    static_assert(hbp[K_AXIS] >= 0, "HOMING_BACKOFF_POST_MM.K must be greater than or equal to 0."),
    static_assert(hbp[U_AXIS] >= 0, "HOMING_BACKOFF_POST_MM.U must be greater than or equal to 0."),
    static_assert(hbp[V_AXIS] >= 0, "HOMING_BACKOFF_POST_MM.V must be greater than or equal to 0."),
    static_assert(hbp[W_AXIS] >= 0, "HOMING_BACKOFF_POST_MM.W must be greater than or equal to 0.")
  );
#endif

#ifdef SENSORLESS_BACKOFF_MM
  constexpr float sbm[] = SENSORLESS_BACKOFF_MM;
  static_assert(COUNT(sbm) == NUM_AXES, "SENSORLESS_BACKOFF_MM must have " _NUM_AXES_STR "elements (and no others).");
  NUM_AXIS_CODE(
    static_assert(sbm[X_AXIS] >= 0, "SENSORLESS_BACKOFF_MM.X must be greater than or equal to 0."),
    static_assert(sbm[Y_AXIS] >= 0, "SENSORLESS_BACKOFF_MM.Y must be greater than or equal to 0."),
    static_assert(sbm[Z_AXIS] >= 0, "SENSORLESS_BACKOFF_MM.Z must be greater than or equal to 0."),
    static_assert(sbm[I_AXIS] >= 0, "SENSORLESS_BACKOFF_MM.I must be greater than or equal to 0."),
    static_assert(sbm[J_AXIS] >= 0, "SENSORLESS_BACKOFF_MM.J must be greater than or equal to 0."),
    static_assert(sbm[K_AXIS] >= 0, "SENSORLESS_BACKOFF_MM.K must be greater than or equal to 0."),
    static_assert(sbm[U_AXIS] >= 0, "SENSORLESS_BACKOFF_MM.U must be greater than or equal to 0."),
    static_assert(sbm[V_AXIS] >= 0, "SENSORLESS_BACKOFF_MM.V must be greater than or equal to 0."),
    static_assert(sbm[W_AXIS] >= 0, "SENSORLESS_BACKOFF_MM.W must be greater than or equal to 0.")
  );
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
  static_assert(WITHIN(Z_SAFE_HOMING_X_POINT, X_MIN_POS, X_MAX_POS), "Z_SAFE_HOMING_X_POINT can't be reached by the nozzle.");
  static_assert(WITHIN(Z_SAFE_HOMING_Y_POINT, Y_MIN_POS, Y_MAX_POS), "Z_SAFE_HOMING_Y_POINT can't be reached by the nozzle.");
#endif

/**
 * Make sure DISABLE_[XYZ] compatible with selected homing options
 */
#if ANY(DISABLE_X, DISABLE_Y, DISABLE_Z, DISABLE_I, DISABLE_J, DISABLE_K, DISABLE_U, DISABLE_V, DISABLE_W)
  #if EITHER(HOME_AFTER_DEACTIVATE, Z_SAFE_HOMING)
    #error "DISABLE_[XYZIJKUVW] is not compatible with HOME_AFTER_DEACTIVATE or Z_SAFE_HOMING."
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
#if IS_ULTIPANEL && NONE(IS_NEWPANEL, SR_LCD_2W_NL) && !PIN_EXISTS(SHIFT_CLK)
  #error "ULTIPANEL controllers require some kind of encoder."
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
#if ALL(Z_HOME_TO_MIN, Z_PROBE_ALLEN_KEY, Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN)
  #error "You can't home to a Z min endstop with a Z_PROBE_ALLEN_KEY."
#endif

/**
 * Dual X Carriage requirements
 */
#if ENABLED(DUAL_X_CARRIAGE)
  #if EXTRUDERS < 2
    #error "DUAL_X_CARRIAGE requires 2 (or more) extruders."
  #elif ANY(CORE_IS_XY, CORE_IS_XZ, MARKFORGED_XY, MARKFORGED_YX)
    #error "DUAL_X_CARRIAGE cannot be used with COREXY, COREYX, COREXZ, COREZX, MARKFORGED_YX, or MARKFORGED_XY."
  #elif !GOOD_AXIS_PINS(X2)
    #error "DUAL_X_CARRIAGE requires X2 stepper pins to be defined."
  #elif !HAS_X_MAX
    #error "DUAL_X_CARRIAGE requires USE_XMAX_PLUG and an X Max Endstop."
  #elif !defined(X2_HOME_POS) || !defined(X2_MIN_POS) || !defined(X2_MAX_POS)
    #error "DUAL_X_CARRIAGE requires X2_HOME_POS, X2_MIN_POS, and X2_MAX_POS."
  #elif X_HOME_TO_MAX || X2_HOME_TO_MIN
    #error "DUAL_X_CARRIAGE requires X_HOME_DIR -1 and X2_HOME_DIR 1."
  #endif
#endif

#undef GOOD_AXIS_PINS

/**
 * Make sure auto fan pins don't conflict with the fan pin
 */
#if HAS_AUTO_FAN
  #if HAS_FAN0
    #if PIN_EXISTS(E0_AUTO_FAN) && E0_AUTO_FAN_PIN == FAN_PIN
      #error "You cannot set E0_AUTO_FAN_PIN equal to FAN_PIN."
    #elif PIN_EXISTS(E1_AUTO_FAN) && E1_AUTO_FAN_PIN == FAN_PIN
      #error "You cannot set E1_AUTO_FAN_PIN equal to FAN_PIN."
    #elif PIN_EXISTS(E2_AUTO_FAN) && E2_AUTO_FAN_PIN == FAN_PIN
      #error "You cannot set E2_AUTO_FAN_PIN equal to FAN_PIN."
    #elif PIN_EXISTS(E3_AUTO_FAN) &&  E3_AUTO_FAN_PIN == FAN_PIN
      #error "You cannot set E3_AUTO_FAN_PIN equal to FAN_PIN."
    #endif
  #endif
#endif

#if HAS_FAN0
  #if CONTROLLER_FAN_PIN == FAN_PIN
    #error "You cannot set CONTROLLER_FAN_PIN equal to FAN_PIN."
  #elif ENABLED(FAN_SOFT_PWM_REQUIRED) && DISABLED(FAN_SOFT_PWM)
    #error "FAN_SOFT_PWM is required for your board. Enable it to continue."
  #endif
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

#ifdef REDUNDANT_PART_COOLING_FAN
  #if FAN_COUNT < 2
    #error "REDUNDANT_PART_COOLING_FAN requires a board with at least two PWM fans."
  #else
    static_assert(WITHIN(REDUNDANT_PART_COOLING_FAN, 1, FAN_COUNT - 1), "REDUNDANT_PART_COOLING_FAN must be between 1 and " STRINGIFY(DECREMENT(FAN_COUNT)) ".");
  #endif
#endif

/**
 * Case Light requirements
 */
#if NEED_CASE_LIGHT_PIN
  #if !PIN_EXISTS(CASE_LIGHT)
    #error "CASE_LIGHT_ENABLE requires CASE_LIGHT_PIN, CASE_LIGHT_USE_NEOPIXEL, or CASE_LIGHT_USE_RGB_LED."
  #elif CASE_LIGHT_PIN == FAN_PIN
    #error "CASE_LIGHT_PIN conflicts with FAN_PIN. Resolve before continuing."
  #endif
#endif

/**
 * Required custom thermistor settings
 */
#if   TEMP_SENSOR_0_IS_CUSTOM && !(defined(HOTEND0_PULLUP_RESISTOR_OHMS) && defined(HOTEND0_RESISTANCE_25C_OHMS) && defined(HOTEND0_BETA))
  #error "TEMP_SENSOR_0 1000 requires HOTEND0_PULLUP_RESISTOR_OHMS, HOTEND0_RESISTANCE_25C_OHMS and HOTEND0_BETA in Configuration_adv.h."
#elif TEMP_SENSOR_1_IS_CUSTOM && !(defined(HOTEND1_PULLUP_RESISTOR_OHMS) && defined(HOTEND1_RESISTANCE_25C_OHMS) && defined(HOTEND1_BETA))
  #error "TEMP_SENSOR_1 1000 requires HOTEND1_PULLUP_RESISTOR_OHMS, HOTEND1_RESISTANCE_25C_OHMS and HOTEND1_BETA in Configuration_adv.h."
#elif TEMP_SENSOR_2_IS_CUSTOM && !(defined(HOTEND2_PULLUP_RESISTOR_OHMS) && defined(HOTEND2_RESISTANCE_25C_OHMS) && defined(HOTEND2_BETA))
  #error "TEMP_SENSOR_2 1000 requires HOTEND2_PULLUP_RESISTOR_OHMS, HOTEND2_RESISTANCE_25C_OHMS and HOTEND2_BETA in Configuration_adv.h."
#elif TEMP_SENSOR_3_IS_CUSTOM && !(defined(HOTEND3_PULLUP_RESISTOR_OHMS) && defined(HOTEND3_RESISTANCE_25C_OHMS) && defined(HOTEND3_BETA))
  #error "TEMP_SENSOR_3 1000 requires HOTEND3_PULLUP_RESISTOR_OHMS, HOTEND3_RESISTANCE_25C_OHMS and HOTEND3_BETA in Configuration_adv.h."
#elif TEMP_SENSOR_4_IS_CUSTOM && !(defined(HOTEND4_PULLUP_RESISTOR_OHMS) && defined(HOTEND4_RESISTANCE_25C_OHMS) && defined(HOTEND4_BETA))
  #error "TEMP_SENSOR_4 1000 requires HOTEND4_PULLUP_RESISTOR_OHMS, HOTEND4_RESISTANCE_25C_OHMS and HOTEND4_BETA in Configuration_adv.h."
#elif TEMP_SENSOR_5_IS_CUSTOM && !(defined(HOTEND5_PULLUP_RESISTOR_OHMS) && defined(HOTEND5_RESISTANCE_25C_OHMS) && defined(HOTEND5_BETA))
  #error "TEMP_SENSOR_5 1000 requires HOTEND5_PULLUP_RESISTOR_OHMS, HOTEND5_RESISTANCE_25C_OHMS and HOTEND5_BETA in Configuration_adv.h."
#elif TEMP_SENSOR_6_IS_CUSTOM && !(defined(HOTEND6_PULLUP_RESISTOR_OHMS) && defined(HOTEND6_RESISTANCE_25C_OHMS) && defined(HOTEND6_BETA))
  #error "TEMP_SENSOR_6 1000 requires HOTEND6_PULLUP_RESISTOR_OHMS, HOTEND6_RESISTANCE_25C_OHMS and HOTEND6_BETA in Configuration_adv.h."
#elif TEMP_SENSOR_7_IS_CUSTOM && !(defined(HOTEND7_PULLUP_RESISTOR_OHMS) && defined(HOTEND7_RESISTANCE_25C_OHMS) && defined(HOTEND7_BETA))
  #error "TEMP_SENSOR_7 1000 requires HOTEND7_PULLUP_RESISTOR_OHMS, HOTEND7_RESISTANCE_25C_OHMS and HOTEND7_BETA in Configuration_adv.h."
#elif TEMP_SENSOR_BED_IS_CUSTOM && !(defined(BED_PULLUP_RESISTOR_OHMS) && defined(BED_RESISTANCE_25C_OHMS) && defined(BED_BETA))
  #error "TEMP_SENSOR_BED 1000 requires BED_PULLUP_RESISTOR_OHMS, BED_RESISTANCE_25C_OHMS and BED_BETA in Configuration_adv.h."
#elif TEMP_SENSOR_CHAMBER_IS_CUSTOM && !(defined(CHAMBER_PULLUP_RESISTOR_OHMS) && defined(CHAMBER_RESISTANCE_25C_OHMS) && defined(CHAMBER_BETA))
  #error "TEMP_SENSOR_CHAMBER 1000 requires CHAMBER_PULLUP_RESISTOR_OHMS, CHAMBER_RESISTANCE_25C_OHMS and CHAMBER_BETA in Configuration_adv.h."
#elif TEMP_SENSOR_PROBE_IS_CUSTOM && !(defined(PROBE_PULLUP_RESISTOR_OHMS) && defined(PROBE_RESISTANCE_25C_OHMS) && defined(PROBE_BETA))
  #error "TEMP_SENSOR_PROBE 1000 requires PROBE_PULLUP_RESISTOR_OHMS, PROBE_RESISTANCE_25C_OHMS and PROBE_BETA in Configuration_adv.h."
#elif TEMP_SENSOR_BOARD_IS_CUSTOM && !(defined(BOARD_PULLUP_RESISTOR_OHMS) && defined(BOARD_RESISTANCE_25C_OHMS) && defined(BOARD_BETA))
  #error "TEMP_SENSOR_BOARD 1000 requires BOARD_PULLUP_RESISTOR_OHMS, BOARD_RESISTANCE_25C_OHMS and BOARD_BETA in Configuration_adv.h."
#elif TEMP_SENSOR_REDUNDANT_IS_CUSTOM && !(defined(REDUNDANT_PULLUP_RESISTOR_OHMS) && defined(REDUNDANT_RESISTANCE_25C_OHMS) && defined(REDUNDANT_BETA))
  #error "TEMP_SENSOR_REDUNDANT 1000 requires REDUNDANT_PULLUP_RESISTOR_OHMS, REDUNDANT_RESISTANCE_25C_OHMS and REDUNDANT_BETA in Configuration_adv.h."
#endif

/**
 * Required MAX31865 settings
 */
#if TEMP_SENSOR_0_IS_MAX31865 || (TEMP_SENSOR_REDUNDANT_IS_MAX31865 && REDUNDANT_TEMP_MATCH(SOURCE, E0))
  #if !defined(MAX31865_SENSOR_WIRES_0) || !WITHIN(MAX31865_SENSOR_WIRES_0, 2, 4)
    #error "MAX31865_SENSOR_WIRES_0 must be defined as an integer between 2 and 4."
  #elif !defined(MAX31865_SENSOR_OHMS_0) || !defined(MAX31865_CALIBRATION_OHMS_0)
    #error "MAX31865_SENSOR_OHMS_0 and MAX31865_CALIBRATION_OHMS_0 must be set if TEMP_SENSOR_0/TEMP_SENSOR_REDUNDANT is MAX31865."
  #endif
#endif
#if TEMP_SENSOR_1_IS_MAX31865 || (TEMP_SENSOR_REDUNDANT_IS_MAX31865 && REDUNDANT_TEMP_MATCH(SOURCE, E1))
  #if !defined(MAX31865_SENSOR_WIRES_1) || !WITHIN(MAX31865_SENSOR_WIRES_1, 2, 4)
    #error "MAX31865_SENSOR_WIRES_1 must be defined as an integer between 2 and 4."
  #elif !defined(MAX31865_SENSOR_OHMS_1) || !defined(MAX31865_CALIBRATION_OHMS_1)
    #error "MAX31865_SENSOR_OHMS_1 and MAX31865_CALIBRATION_OHMS_1 must be set if TEMP_SENSOR_1/TEMP_SENSOR_REDUNDANT is MAX31865."
  #endif
#endif

/**
 * Redundant temperature sensor config
 */
#if HAS_TEMP_REDUNDANT
  #ifndef TEMP_SENSOR_REDUNDANT_SOURCE
    #error "TEMP_SENSOR_REDUNDANT requires TEMP_SENSOR_REDUNDANT_SOURCE."
  #elif !defined(TEMP_SENSOR_REDUNDANT_TARGET)
    #error "TEMP_SENSOR_REDUNDANT requires TEMP_SENSOR_REDUNDANT_TARGET."
  #elif REDUNDANT_TEMP_MATCH(SOURCE, TEMP_SENSOR_REDUNDANT_TARGET)
    #error "TEMP_SENSOR_REDUNDANT_SOURCE can't be the same as TEMP_SENSOR_REDUNDANT_TARGET."
  #elif HAS_MULTI_HOTEND && TEMP_SENSOR_REDUNDANT_SOURCE < HOTENDS
    #error "TEMP_SENSOR_REDUNDANT_SOURCE must be after the last used hotend TEMP_SENSOR."
  #endif

  #if REDUNDANT_TEMP_MATCH(SOURCE, E0) && HAS_HOTEND
    #error "TEMP_SENSOR_REDUNDANT_SOURCE can't be E0 if a hotend is used. E0 always uses TEMP_SENSOR_0."
  #elif REDUNDANT_TEMP_MATCH(SOURCE, COOLER) && HAS_TEMP_COOLER
    #error "TEMP_SENSOR_REDUNDANT_SOURCE can't be COOLER. TEMP_SENSOR_COOLER is in use."
  #elif REDUNDANT_TEMP_MATCH(SOURCE, PROBE) && HAS_TEMP_PROBE
    #error "TEMP_SENSOR_REDUNDANT_SOURCE can't be PROBE. TEMP_SENSOR_PROBE is in use."
  #elif REDUNDANT_TEMP_MATCH(SOURCE, BOARD) && HAS_TEMP_BOARD
    #error "TEMP_SENSOR_REDUNDANT_SOURCE can't be BOARD. TEMP_SENSOR_BOARD is in use."
  #elif REDUNDANT_TEMP_MATCH(SOURCE, CHAMBER) && HAS_TEMP_CHAMBER
    #error "TEMP_SENSOR_REDUNDANT_SOURCE can't be CHAMBER. TEMP_SENSOR_CHAMBER is in use."
  #elif REDUNDANT_TEMP_MATCH(SOURCE, BED) && HAS_TEMP_BED
    #error "TEMP_SENSOR_REDUNDANT_SOURCE can't be BED. TEMP_SENSOR_BED is in use."
  #endif

  #if REDUNDANT_TEMP_MATCH(TARGET, E0) && !PIN_EXISTS(TEMP_0)
    #error "TEMP_SENSOR_REDUNDANT_TARGET can't be E0 without TEMP_0_PIN defined."
  #elif REDUNDANT_TEMP_MATCH(TARGET, E1) && !PIN_EXISTS(TEMP_1)
    #error "TEMP_SENSOR_REDUNDANT_TARGET can't be E1 without TEMP_1_PIN defined."
  #elif REDUNDANT_TEMP_MATCH(TARGET, E2) && !PIN_EXISTS(TEMP_2)
    #error "TEMP_SENSOR_REDUNDANT_TARGET can't be E2 without TEMP_2_PIN defined."
  #elif REDUNDANT_TEMP_MATCH(TARGET, E3) && !PIN_EXISTS(TEMP_3)
    #error "TEMP_SENSOR_REDUNDANT_TARGET can't be E3 without TEMP_3_PIN defined."
  #elif REDUNDANT_TEMP_MATCH(TARGET, E4) && !PIN_EXISTS(TEMP_4)
    #error "TEMP_SENSOR_REDUNDANT_TARGET can't be E4 without TEMP_4_PIN defined."
  #elif REDUNDANT_TEMP_MATCH(TARGET, E5) && !PIN_EXISTS(TEMP_5)
    #error "TEMP_SENSOR_REDUNDANT_TARGET can't be E5 without TEMP_5_PIN defined."
  #elif REDUNDANT_TEMP_MATCH(TARGET, E6) && !PIN_EXISTS(TEMP_6)
    #error "TEMP_SENSOR_REDUNDANT_TARGET can't be E6 without TEMP_6_PIN defined."
  #elif REDUNDANT_TEMP_MATCH(TARGET, E7) && !PIN_EXISTS(TEMP_7)
    #error "TEMP_SENSOR_REDUNDANT_TARGET can't be E7 without TEMP_7_PIN defined."
  #elif REDUNDANT_TEMP_MATCH(TARGET, BED) && !PIN_EXISTS(TEMP_BED)
    #error "TEMP_SENSOR_REDUNDANT_TARGET can't be BED without TEMP_BED_PIN defined."
  #elif REDUNDANT_TEMP_MATCH(TARGET, CHAMBER) && !PIN_EXISTS(TEMP_CHAMBER)
    #error "TEMP_SENSOR_REDUNDANT_TARGET can't be CHAMBER without TEMP_CHAMBER_PIN defined."
  #elif REDUNDANT_TEMP_MATCH(TARGET, BOARD) && !PIN_EXISTS(TEMP_BOARD)
    #error "TEMP_SENSOR_REDUNDANT_TARGET can't be BOARD without TEMP_BOARD_PIN defined."
  #elif REDUNDANT_TEMP_MATCH(TARGET, PROBE) && !PIN_EXISTS(TEMP_PROBE)
    #error "TEMP_SENSOR_REDUNDANT_TARGET can't be PROBE without TEMP_PROBE_PIN defined."
  #elif REDUNDANT_TEMP_MATCH(TARGET, COOLER) && !PIN_EXISTS(TEMP_COOLER)
    #error "TEMP_SENSOR_REDUNDANT_TARGET can't be COOLER without TEMP_COOLER_PIN defined."
  #endif

  #if TEMP_SENSOR_IS_MAX_TC(REDUNDANT) && REDUNDANT_TEMP_MATCH(SOURCE, E0) && !PIN_EXISTS(TEMP_0_CS)
    #error "TEMP_SENSOR_REDUNDANT MAX Thermocouple with TEMP_SENSOR_REDUNDANT_SOURCE E0 requires TEMP_0_CS_PIN."
  #elif TEMP_SENSOR_IS_MAX_TC(REDUNDANT) && REDUNDANT_TEMP_MATCH(SOURCE, E1) && !PIN_EXISTS(TEMP_1_CS)
    #error "TEMP_SENSOR_REDUNDANT MAX Thermocouple with TEMP_SENSOR_REDUNDANT_SOURCE E1 requires TEMP_1_CS_PIN."
  #endif
#endif

/**
 * Test Sensor & Heater pin combos.
 * Pins and Sensor IDs must be set for each heater
 */
#if HAS_EXTRUDERS && !ANY_PIN(TEMP_0, TEMP_0_CS)
  #error "TEMP_0_PIN or TEMP_0_CS_PIN not defined for this board."
#elif HAS_EXTRUDERS && !HAS_HEATER_0
  #error "HEATER_0_PIN not defined for this board."
#elif TEMP_SENSOR_IS_MAX_TC(0) && !PIN_EXISTS(TEMP_0_CS)
  #error "TEMP_SENSOR_0 MAX thermocouple requires TEMP_0_CS_PIN."
#elif HAS_HOTEND && !HAS_TEMP_HOTEND && !TEMP_SENSOR_0_IS_DUMMY
  #error "TEMP_0_PIN (required for TEMP_SENSOR_0) not defined for this board."
#elif EITHER(HAS_MULTI_HOTEND, HEATERS_PARALLEL) && !HAS_HEATER_1
  #error "HEATER_1_PIN is not defined. TEMP_SENSOR_1 might not be set, or the board (not EEB / EEF?) doesn't define a pin."
#endif

#if HAS_MULTI_HOTEND
  #if TEMP_SENSOR_IS_MAX_TC(1) && !PIN_EXISTS(TEMP_1_CS)
    #error "TEMP_SENSOR_1 MAX thermocouple requires TEMP_1_CS_PIN."
  #elif TEMP_SENSOR_1 == 0
    #error "TEMP_SENSOR_1 is required with 2 or more HOTENDS."
  #elif !ANY_PIN(TEMP_1, TEMP_1_CS) && !TEMP_SENSOR_1_IS_DUMMY
    #error "TEMP_1_PIN or TEMP_1_CS_PIN not defined for this board."
  #endif
  #if HOTENDS > 2
    #if TEMP_SENSOR_2 == 0
      #error "TEMP_SENSOR_2 is required with 3 or more HOTENDS."
    #elif !HAS_HEATER_2
      #error "HEATER_2_PIN not defined for this board."
    #elif !PIN_EXISTS(TEMP_2) && !TEMP_SENSOR_2_IS_DUMMY
      #error "TEMP_2_PIN not defined for this board."
    #endif
    #if HOTENDS > 3
      #if TEMP_SENSOR_3 == 0
        #error "TEMP_SENSOR_3 is required with 4 or more HOTENDS."
      #elif !HAS_HEATER_3
        #error "HEATER_3_PIN not defined for this board."
      #elif !PIN_EXISTS(TEMP_3) && !TEMP_SENSOR_3_IS_DUMMY
        #error "TEMP_3_PIN not defined for this board."
      #endif
      #if HOTENDS > 4
        #if TEMP_SENSOR_4 == 0
          #error "TEMP_SENSOR_4 is required with 5 or more HOTENDS."
        #elif !HAS_HEATER_4
          #error "HEATER_4_PIN not defined for this board."
        #elif !PIN_EXISTS(TEMP_4) && !TEMP_SENSOR_4_IS_DUMMY
          #error "TEMP_4_PIN not defined for this board."
        #endif
        #if HOTENDS > 5
          #if TEMP_SENSOR_5 == 0
            #error "TEMP_SENSOR_5 is required with 6 HOTENDS."
          #elif !HAS_HEATER_5
            #error "HEATER_5_PIN not defined for this board."
          #elif !PIN_EXISTS(TEMP_5) && !TEMP_SENSOR_5_IS_DUMMY
            #error "TEMP_5_PIN not defined for this board."
          #endif
          #if HOTENDS > 6
            #if TEMP_SENSOR_6 == 0
              #error "TEMP_SENSOR_6 is required with 6 HOTENDS."
            #elif !HAS_HEATER_6
              #error "HEATER_6_PIN not defined for this board."
            #elif !PIN_EXISTS(TEMP_6) && !TEMP_SENSOR_6_IS_DUMMY
              #error "TEMP_6_PIN not defined for this board."
            #endif
            #if HOTENDS > 7
              #if TEMP_SENSOR_7 == 0
                #error "TEMP_SENSOR_7 is required with 7 HOTENDS."
              #elif !HAS_HEATER_7
                #error "HEATER_7_PIN not defined for this board."
              #elif !PIN_EXISTS(TEMP_7) && !TEMP_SENSOR_7_IS_DUMMY
                #error "TEMP_7_PIN not defined for this board."
              #endif
            #endif // HOTENDS > 7
          #endif // HOTENDS > 6
        #endif // HOTENDS > 5
      #endif // HOTENDS > 4
    #endif // HOTENDS > 3
  #endif // HOTENDS > 2
#endif // HAS_MULTI_HOTEND

/**
 * Pins must be set for temp sensors, with some other feature requirements.
 */
#if TEMP_SENSOR_CHAMBER && !PIN_EXISTS(TEMP_CHAMBER)
  #error "TEMP_SENSOR_CHAMBER requires TEMP_CHAMBER_PIN."
#endif

#if TEMP_SENSOR_COOLER
  #if !PIN_EXISTS(TEMP_COOLER)
    #error "TEMP_SENSOR_COOLER requires TEMP_COOLER_PIN."
  #elif DISABLED(LASER_FEATURE)
    #error "TEMP_SENSOR_COOLER requires LASER_FEATURE."
  #endif
#endif

#if TEMP_SENSOR_PROBE
  #if !PIN_EXISTS(TEMP_PROBE)
    #error "TEMP_SENSOR_PROBE requires TEMP_PROBE_PIN."
  #elif DISABLED(FIX_MOUNTED_PROBE)
    #error "TEMP_SENSOR_PROBE shouldn't be set without FIX_MOUNTED_PROBE."
  #endif
#endif

#if TEMP_SENSOR_BOARD
  #if !PIN_EXISTS(TEMP_BOARD)
    #error "TEMP_SENSOR_BOARD requires TEMP_BOARD_PIN."
  #elif ENABLED(THERMAL_PROTECTION_BOARD) && (!defined(BOARD_MINTEMP) || !defined(BOARD_MAXTEMP))
    #error "THERMAL_PROTECTION_BOARD requires BOARD_MINTEMP and BOARD_MAXTEMP."
  #endif
#elif CONTROLLER_FAN_MIN_BOARD_TEMP
  #error "CONTROLLER_FAN_MIN_BOARD_TEMP requires TEMP_SENSOR_BOARD."
#endif

#if TEMP_SENSOR_BOARD && !PIN_EXISTS(TEMP_BOARD)
  #error "TEMP_SENSOR_BOARD requires TEMP_BOARD_PIN."
#endif

#if ENABLED(LASER_COOLANT_FLOW_METER) && !(PIN_EXISTS(FLOWMETER) && ENABLED(LASER_FEATURE))
  #error "LASER_COOLANT_FLOW_METER requires FLOWMETER_PIN and LASER_FEATURE."
#endif

#if ENABLED(CHAMBER_FAN) && !(defined(CHAMBER_FAN_MODE) && WITHIN(CHAMBER_FAN_MODE, 0, 3))
  #error "CHAMBER_FAN_MODE must be between 0 and 3."
#endif

#if ENABLED(CHAMBER_VENT)
  #ifndef CHAMBER_VENT_SERVO_NR
    #error "CHAMBER_VENT_SERVO_NR is required for CHAMBER SERVO."
  #elif !NUM_SERVOS
    #error "NUM_SERVOS is required for a Heated Chamber vent servo (CHAMBER_VENT_SERVO_NR)."
  #elif CHAMBER_VENT_SERVO_NR >= NUM_SERVOS
    #error "CHAMBER_VENT_SERVO_NR must be smaller than NUM_SERVOS."
  #elif HAS_Z_SERVO_PROBE && CHAMBER_VENT_SERVO_NR == Z_PROBE_SERVO_NR
    #error "CHAMBER SERVO is already used by BLTOUCH."
  #elif CHAMBER_VENT_SERVO_NR == 0 && !PIN_EXISTS(SERVO0)
    #error "SERVO0_PIN must be defined for your Heated Chamber vent servo."
  #elif CHAMBER_VENT_SERVO_NR == 1 && !PIN_EXISTS(SERVO1)
    #error "SERVO1_PIN must be defined for your Heated Chamber vent servo."
  #elif CHAMBER_VENT_SERVO_NR == 2 && !PIN_EXISTS(SERVO2)
    #error "SERVO2_PIN must be defined for your Heated Chamber vent servo."
  #elif CHAMBER_VENT_SERVO_NR == 3 && !PIN_EXISTS(SERVO3)
    #error "SERVO3_PIN must be defined for your Heated Chamber vent servo."
  #endif
#endif

/**
 * Temperature status LEDs
 */
#if ENABLED(TEMP_STAT_LEDS) && !ANY_PIN(STAT_LED_RED, STAT_LED_BLUE)
  #error "TEMP_STAT_LEDS requires STAT_LED_RED_PIN or STAT_LED_BLUE_PIN, preferably both."
#endif

/**
 * FYSETC/MKS/BTT Mini Panel Requirements
 */
#if EITHER(FYSETC_242_OLED_12864, FYSETC_MINI_12864_2_1)
  #ifndef NEO_RGB
    #define NEO_RGB 123
    #define FAUX_RGB 1
  #endif
  #if defined(NEOPIXEL_TYPE) && NEOPIXEL_TYPE != NEO_RGB
    #error "Your FYSETC/MKS/BTT Mini Panel requires NEOPIXEL_TYPE to be NEO_RGB."
  #elif defined(NEOPIXEL_PIXELS) && NEOPIXEL_PIXELS < 3
    #error "Your FYSETC/MKS/BTT Mini Panel requires NEOPIXEL_PIXELS >= 3."
  #endif
  #if FAUX_RGB
    #undef NEO_RGB
    #undef FAUX_RGB
  #endif
#elif EITHER(FYSETC_MINI_12864_1_2, FYSETC_MINI_12864_2_0) && DISABLED(RGB_LED)
  #error "Your FYSETC Mini Panel requires RGB_LED."
#endif

/**
 * LED Control Menu requirements
 */
#if ENABLED(LED_CONTROL_MENU) && !HAS_COLOR_LEDS
  #error "LED_CONTROL_MENU requires BLINKM, RGB_LED, RGBW_LED, PCA9533, PCA9632, or NEOPIXEL_LED."
#endif

/**
 * Basic multi hotend duplication mode
 */
#if ENABLED(MULTI_NOZZLE_DUPLICATION)
  #if ENABLED(SINGLENOZZLE)
    #error "MULTI_NOZZLE_DUPLICATION is incompatible with SINGLENOZZLE."
  #elif ENABLED(DUAL_X_CARRIAGE)
    #error "MULTI_NOZZLE_DUPLICATION is incompatible with DUAL_X_CARRIAGE."
  #elif ENABLED(MIXING_EXTRUDER)
    #error "MULTI_NOZZLE_DUPLICATION is incompatible with MIXING_EXTRUDER."
  #elif ENABLED(SWITCHING_EXTRUDER)
    #error "MULTI_NOZZLE_DUPLICATION is incompatible with SWITCHING_EXTRUDER."
  #elif HOTENDS < 2
    #error "MULTI_NOZZLE_DUPLICATION requires 2 or more hotends."
  #endif
#endif

/**
 * Test Extruder Stepper Pins
 */
#if HAS_EXTRUDERS
  #if ((defined(__AVR_ATmega644P__) || defined(__AVR_ATmega1284P__)) && !PINS_EXIST(E0_STEP, E0_DIR))
    #error "E0_STEP_PIN or E0_DIR_PIN not defined for this board."
  #elif ( !(defined(__AVR_ATmega644P__) || defined(__AVR_ATmega1284P__)) && (!PINS_EXIST(E0_STEP, E0_DIR) || !HAS_E0_ENABLE))
    #error "E0_STEP_PIN, E0_DIR_PIN, or E0_ENABLE_PIN not defined for this board."
  #elif EXTRUDERS && TEMP_SENSOR_0 == 0
    #error "TEMP_SENSOR_0 is required if there are any extruders."
  #endif
#endif

#if E_STEPPERS > 0 && !(PINS_EXIST(E0_STEP, E0_DIR) && HAS_E0_ENABLE)
  #error "E0_STEP_PIN, E0_DIR_PIN, or E0_ENABLE_PIN not defined for this board."
#endif
#if E_STEPPERS > 1 && !(PINS_EXIST(E1_STEP, E1_DIR) && HAS_E1_ENABLE)
  #error "E1_STEP_PIN, E1_DIR_PIN, or E1_ENABLE_PIN not defined for this board."
#endif
#if E_STEPPERS > 2 && !(PINS_EXIST(E2_STEP, E2_DIR) && HAS_E2_ENABLE)
  #error "E2_STEP_PIN, E2_DIR_PIN, or E2_ENABLE_PIN not defined for this board."
#endif
#if E_STEPPERS > 3 && !(PINS_EXIST(E3_STEP, E3_DIR) && HAS_E3_ENABLE)
  #error "E3_STEP_PIN, E3_DIR_PIN, or E3_ENABLE_PIN not defined for this board."
#endif
#if E_STEPPERS > 4 && !(PINS_EXIST(E4_STEP, E4_DIR) && HAS_E4_ENABLE)
  #error "E4_STEP_PIN, E4_DIR_PIN, or E4_ENABLE_PIN not defined for this board."
#endif
#if E_STEPPERS > 5 && !(PINS_EXIST(E5_STEP, E5_DIR) && HAS_E5_ENABLE)
  #error "E5_STEP_PIN, E5_DIR_PIN, or E5_ENABLE_PIN not defined for this board."
#endif
#if E_STEPPERS > 6 && !(PINS_EXIST(E6_STEP, E6_DIR) && HAS_E6_ENABLE)
  #error "E6_STEP_PIN, E6_DIR_PIN, or E6_ENABLE_PIN not defined for this board."
#endif
#if E_STEPPERS > 7 && !(PINS_EXIST(E7_STEP, E7_DIR) && HAS_E7_ENABLE)
  #error "E7_STEP_PIN, E7_DIR_PIN, or E7_ENABLE_PIN not defined for this board."
#endif

/**
 * Endstop Tests
 */

#define _PLUG_UNUSED_TEST(A,P) (DISABLED(USE_##P##MIN_PLUG, USE_##P##MAX_PLUG) \
  && !(ENABLED(A##_DUAL_ENDSTOPS) && WITHIN(A##2_USE_ENDSTOP, _##P##MAX_, _##P##MIN_)) \
  && !(ENABLED(A##_MULTI_ENDSTOPS) && WITHIN(A##2_USE_ENDSTOP, _##P##MAX_, _##P##MIN_)) )
#define _AXIS_PLUG_UNUSED_TEST(A) (1 NUM_AXIS_GANG(&& _PLUG_UNUSED_TEST(A,X), && _PLUG_UNUSED_TEST(A,Y), && _PLUG_UNUSED_TEST(A,Z), \
                                                      && _PLUG_UNUSED_TEST(A,I), && _PLUG_UNUSED_TEST(A,J), && _PLUG_UNUSED_TEST(A,K), \
                                                      && _PLUG_UNUSED_TEST(A,U), && _PLUG_UNUSED_TEST(A,V), && _PLUG_UNUSED_TEST(A,W) ) )

// A machine with endstops must have a minimum of 3
#if HAS_ENDSTOPS
  #if _AXIS_PLUG_UNUSED_TEST(X)
    #error "You must enable USE_XMIN_PLUG or USE_XMAX_PLUG."
  #endif
  #if _AXIS_PLUG_UNUSED_TEST(Y)
    #error "You must enable USE_YMIN_PLUG or USE_YMAX_PLUG."
  #endif
  #if _AXIS_PLUG_UNUSED_TEST(Z)
    #error "You must enable USE_ZMIN_PLUG or USE_ZMAX_PLUG."
  #endif
  #if HAS_I_AXIS && _AXIS_PLUG_UNUSED_TEST(I)
    #error "You must enable USE_IMIN_PLUG or USE_IMAX_PLUG."
  #endif
  #if HAS_J_AXIS && _AXIS_PLUG_UNUSED_TEST(J)
    #error "You must enable USE_JMIN_PLUG or USE_JMAX_PLUG."
  #endif
  #if HAS_K_AXIS && _AXIS_PLUG_UNUSED_TEST(K)
    #error "You must enable USE_KMIN_PLUG or USE_KMAX_PLUG."
  #endif
  #if HAS_U_AXIS && _AXIS_PLUG_UNUSED_TEST(U)
    #error "You must enable USE_UMIN_PLUG or USE_UMAX_PLUG."
  #endif
  #if HAS_V_AXIS && _AXIS_PLUG_UNUSED_TEST(V)
    #error "You must enable USE_VMIN_PLUG or USE_VMAX_PLUG."
  #endif
  #if HAS_W_AXIS && _AXIS_PLUG_UNUSED_TEST(W)
    #error "You must enable USE_WMIN_PLUG or USE_WMAX_PLUG."
  #endif

  // Delta and Cartesian use 3 homing endstops
  #if NONE(IS_SCARA, SPI_ENDSTOPS)
    #if X_HOME_TO_MIN && DISABLED(USE_XMIN_PLUG)
      #error "Enable USE_XMIN_PLUG when homing X to MIN."
    #elif X_HOME_TO_MAX && DISABLED(USE_XMAX_PLUG)
      #error "Enable USE_XMAX_PLUG when homing X to MAX."
    #elif Y_HOME_TO_MIN && DISABLED(USE_YMIN_PLUG)
      #error "Enable USE_YMIN_PLUG when homing Y to MIN."
    #elif Y_HOME_TO_MAX && DISABLED(USE_YMAX_PLUG)
      #error "Enable USE_YMAX_PLUG when homing Y to MAX."
    #elif HAS_I_AXIS && I_HOME_TO_MIN && DISABLED(USE_IMIN_PLUG)
      #error "Enable USE_IMIN_PLUG when homing I to MIN."
    #elif HAS_I_AXIS && I_HOME_TO_MAX && DISABLED(USE_IMAX_PLUG)
      #error "Enable USE_IMAX_PLUG when homing I to MAX."
    #elif HAS_J_AXIS && J_HOME_TO_MIN && DISABLED(USE_JMIN_PLUG)
      #error "Enable USE_JMIN_PLUG when homing J to MIN."
    #elif HAS_J_AXIS && J_HOME_TO_MAX && DISABLED(USE_JMAX_PLUG)
      #error "Enable USE_JMAX_PLUG when homing J to MAX."
    #elif HAS_K_AXIS && K_HOME_TO_MIN && DISABLED(USE_KMIN_PLUG)
      #error "Enable USE_KMIN_PLUG when homing K to MIN."
    #elif HAS_K_AXIS && K_HOME_TO_MAX && DISABLED(USE_KMAX_PLUG)
      #error "Enable USE_KMAX_PLUG when homing K to MAX."
    #elif HAS_U_AXIS && U_HOME_TO_MIN && DISABLED(USE_UMIN_PLUG)
      #error "Enable USE_UMIN_PLUG when homing U to MIN."
    #elif HAS_U_AXIS && U_HOME_TO_MAX && DISABLED(USE_UMAX_PLUG)
      #error "Enable USE_UMAX_PLUG when homing U to MAX."
    #elif HAS_V_AXIS && V_HOME_TO_MIN && DISABLED(USE_VMIN_PLUG)
      #error "Enable USE_VMIN_PLUG when homing V to MIN."
    #elif HAS_V_AXIS && V_HOME_TO_MAX && DISABLED(USE_VMAX_PLUG)
      #error "Enable USE_VMAX_PLUG when homing V to MAX."
    #elif HAS_W_AXIS && W_HOME_TO_MIN && DISABLED(USE_WMIN_PLUG)
      #error "Enable USE_WMIN_PLUG when homing W to MIN."
    #elif HAS_W_AXIS && W_HOME_TO_MAX && DISABLED(USE_WMAX_PLUG)
      #error "Enable USE_WMAX_PLUG when homing W to MAX."
    #endif
  #endif

  // Z homing direction and plug usage flags
  #if Z_HOME_TO_MIN && NONE(USE_ZMIN_PLUG, HOMING_Z_WITH_PROBE)
    #error "Enable USE_ZMIN_PLUG when homing Z to MIN."
  #elif Z_HOME_TO_MAX && ENABLED(USE_PROBE_FOR_Z_HOMING)
    #error "Z_HOME_DIR must be -1 when homing Z with the probe."
  #elif BOTH(HOMING_Z_WITH_PROBE, Z_MULTI_ENDSTOPS)
    #error "Z_MULTI_ENDSTOPS is incompatible with USE_PROBE_FOR_Z_HOMING."
  #elif Z_HOME_TO_MAX && DISABLED(USE_ZMAX_PLUG)
    #error "Enable USE_ZMAX_PLUG when homing Z to MAX."
  #endif
#endif

#if BOTH(HOME_Z_FIRST, USE_PROBE_FOR_Z_HOMING)
  #error "HOME_Z_FIRST can't be used when homing Z with a probe."
#endif

// Dual/multiple endstops requirements
#if ENABLED(X_DUAL_ENDSTOPS)
  #if ENABLED(DELTA)
    #error "X_DUAL_ENDSTOPS is not compatible with DELTA."
  #elif !X2_USE_ENDSTOP
    #error "X2_USE_ENDSTOP must be set with X_DUAL_ENDSTOPS."
  #endif
#endif
#if ENABLED(Y_DUAL_ENDSTOPS)
  #if ENABLED(DELTA)
    #error "Y_DUAL_ENDSTOPS is not compatible with DELTA."
  #elif !Y2_USE_ENDSTOP
    #error "Y2_USE_ENDSTOP must be set with Y_DUAL_ENDSTOPS."
  #endif
#endif
#if ENABLED(Z_MULTI_ENDSTOPS)
  #if ENABLED(DELTA)
    #error "Z_MULTI_ENDSTOPS is not compatible with DELTA."
  #elif !Z2_USE_ENDSTOP
    #error "Z2_USE_ENDSTOP must be set with Z_MULTI_ENDSTOPS."
  #elif !Z3_USE_ENDSTOP && NUM_Z_STEPPERS >= 3
    #error "Z3_USE_ENDSTOP must be set with Z_MULTI_ENDSTOPS and Z3_DRIVER_TYPE."
  #elif !Z4_USE_ENDSTOP && NUM_Z_STEPPERS >= 4
    #error "Z4_USE_ENDSTOP must be set with Z_MULTI_ENDSTOPS and Z4_DRIVER_TYPE."
  #endif
#endif

#if defined(ENDSTOP_NOISE_THRESHOLD) && !WITHIN(ENDSTOP_NOISE_THRESHOLD, 2, 7)
  #error "ENDSTOP_NOISE_THRESHOLD must be an integer from 2 to 7."
#endif

/**
 * Emergency Command Parser
 */
#if ENABLED(EMERGENCY_PARSER) && defined(__AVR__) && defined(USBCON)
  #error "EMERGENCY_PARSER does not work on boards with AT90USB processors (USBCON)."
#endif

/**
 * Software Reset options
 */
#if ENABLED(SOFT_RESET_VIA_SERIAL) && DISABLED(EMERGENCY_PARSER)
  #error "EMERGENCY_PARSER is required to activate SOFT_RESET_VIA_SERIAL."
#endif
#if ENABLED(SOFT_RESET_ON_KILL) && !BUTTON_EXISTS(ENC)
  #error "An encoder button is required or SOFT_RESET_ON_KILL will reset the printer without notice!"
#endif

// Reset reason for AVR
#if ENABLED(OPTIBOOT_RESET_REASON) && !defined(__AVR__)
  #error "OPTIBOOT_RESET_REASON only applies to AVR."
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
 * G35 Assisted Tramming
 */
#if ENABLED(ASSISTED_TRAMMING) && !HAS_BED_PROBE
  #error "ASSISTED_TRAMMING requires a bed probe."
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
#endif
#undef _RGB_TEST

// NeoPixel requirements
#if ENABLED(NEOPIXEL_LED)
  #if !PIN_EXISTS(NEOPIXEL) || NEOPIXEL_PIXELS == 0
    #error "NEOPIXEL_LED requires NEOPIXEL_PIN and NEOPIXEL_PIXELS."
  #elif ENABLED(NEOPIXEL2_SEPARATE) && !(defined(NEOPIXEL2_TYPE) && PIN_EXISTS(NEOPIXEL2) && NEOPIXEL2_PIXELS > 0)
    #error "NEOPIXEL2_SEPARATE requires NEOPIXEL2_TYPE, NEOPIXEL2_PIN and NEOPIXEL2_PIXELS."
  #elif ENABLED(NEO2_COLOR_PRESETS) && DISABLED(NEOPIXEL2_SEPARATE)
    #error "NEO2_COLOR_PRESETS requires NEOPIXEL2_SEPARATE to be enabled."
  #endif
#endif

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
 * Fan check
 */
#if HAS_FANCHECK
  #if BOTH(E0_FAN_TACHO_PULLUP, E0_FAN_TACHO_PULLDOWN)
    #error "Enable only one of E0_FAN_TACHO_PULLUP or E0_FAN_TACHO_PULLDOWN."
  #elif BOTH(E1_FAN_TACHO_PULLUP, E1_FAN_TACHO_PULLDOWN)
    #error "Enable only one of E1_FAN_TACHO_PULLUP or E1_FAN_TACHO_PULLDOWN."
  #elif BOTH(E2_FAN_TACHO_PULLUP, E2_FAN_TACHO_PULLDOWN)
    #error "Enable only one of E2_FAN_TACHO_PULLUP or E2_FAN_TACHO_PULLDOWN."
  #elif BOTH(E3_FAN_TACHO_PULLUP, E3_FAN_TACHO_PULLDOWN)
    #error "Enable only one of E3_FAN_TACHO_PULLUP or E3_FAN_TACHO_PULLDOWN."
  #elif BOTH(E4_FAN_TACHO_PULLUP, E4_FAN_TACHO_PULLDOWN)
    #error "Enable only one of E4_FAN_TACHO_PULLUP or E4_FAN_TACHO_PULLDOWN."
  #elif BOTH(E5_FAN_TACHO_PULLUP, E5_FAN_TACHO_PULLDOWN)
    #error "Enable only one of E5_FAN_TACHO_PULLUP or E5_FAN_TACHO_PULLDOWN."
  #elif BOTH(E6_FAN_TACHO_PULLUP, E6_FAN_TACHO_PULLDOWN)
    #error "Enable only one of E6_FAN_TACHO_PULLUP or E6_FAN_TACHO_PULLDOWN."
  #elif BOTH(E7_FAN_TACHO_PULLUP, E7_FAN_TACHO_PULLDOWN)
    #error "Enable only one of E7_FAN_TACHO_PULLUP or E7_FAN_TACHO_PULLDOWN."
  #endif
#elif ENABLED(AUTO_REPORT_FANS)
  #error "AUTO_REPORT_FANS requires one or more fans with a tachometer pin."
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
 * Make sure features that need to write to the SD card can
 */
#if ENABLED(SDCARD_READONLY) && ANY(POWER_LOSS_RECOVERY, BINARY_FILE_TRANSFER, SDCARD_EEPROM_EMULATION)
  #undef SDCARD_READONLY
  #if ENABLED(POWER_LOSS_RECOVERY)
    #warning "Either disable SDCARD_READONLY or disable POWER_LOSS_RECOVERY."
  #elif ENABLED(BINARY_FILE_TRANSFER)
    #warning "Either disable SDCARD_READONLY or disable BINARY_FILE_TRANSFER."
  #elif ENABLED(SDCARD_EEPROM_EMULATION)
    #warning "Either disable SDCARD_READONLY or disable SDCARD_EEPROM_EMULATION."
  #endif
#endif

#if ENABLED(SD_IGNORE_AT_STARTUP)
  #if ENABLED(POWER_LOSS_RECOVERY)
    #error "SD_IGNORE_AT_STARTUP is incompatible with POWER_LOSS_RECOVERY."
  #elif ENABLED(SDCARD_EEPROM_EMULATION)
    #error "SD_IGNORE_AT_STARTUP is incompatible with SDCARD_EEPROM_EMULATION."
  #endif
#endif

/**
 * Make sure only one display is enabled
 */
#if 1 < 0 \
  + ENABLED(REPRAP_DISCOUNT_SMART_CONTROLLER) \
  + ENABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER) \
  + (ENABLED(U8GLIB_SSD1306) && DISABLED(IS_U8GLIB_SSD1306)) \
  + (ENABLED(MINIPANEL) && NONE(MKS_MINI_12864, ENDER2_STOCKDISPLAY)) \
  + (ENABLED(MKS_MINI_12864) && NONE(MKS_LCD12864A, MKS_LCD12864B)) \
  + (ENABLED(FYSETC_MINI_12864_2_1) && NONE(MKS_MINI_12864_V3, BTT_MINI_12864_V1)) \
  + COUNT_ENABLED(MKS_MINI_12864_V3, BTT_MINI_12864_V1) \
  + (ENABLED(EXTENSIBLE_UI) && DISABLED(IS_EXTUI)) \
  + (DISABLED(IS_LEGACY_TFT) && ENABLED(TFT_GENERIC)) \
  + (ENABLED(IS_LEGACY_TFT) && COUNT_ENABLED(TFT_320x240, TFT_320x240_SPI, TFT_480x320, TFT_480x320_SPI)) \
  + COUNT_ENABLED(ANYCUBIC_LCD_I3MEGA, ANYCUBIC_LCD_CHIRON, ANYCUBIC_TFT35) \
  + COUNT_ENABLED(DGUS_LCD_UI_ORIGIN, DGUS_LCD_UI_FYSETC, DGUS_LCD_UI_HIPRECY, DGUS_LCD_UI_MKS, DGUS_LCD_UI_RELOADED) \
  + COUNT_ENABLED(ENDER2_STOCKDISPLAY, CR10_STOCKDISPLAY) \
  + COUNT_ENABLED(DWIN_CREALITY_LCD, DWIN_LCD_PROUI, DWIN_CREALITY_LCD_JYERSUI, DWIN_MARLINUI_PORTRAIT, DWIN_MARLINUI_LANDSCAPE) \
  + COUNT_ENABLED(FYSETC_MINI_12864_X_X, FYSETC_MINI_12864_1_2, FYSETC_MINI_12864_2_0, FYSETC_GENERIC_12864_1_1) \
  + COUNT_ENABLED(LCD_SAINSMART_I2C_1602, LCD_SAINSMART_I2C_2004) \
  + COUNT_ENABLED(MKS_12864OLED, MKS_12864OLED_SSD1306) \
  + COUNT_ENABLED(MKS_TS35_V2_0, MKS_ROBIN_TFT24, MKS_ROBIN_TFT28, MKS_ROBIN_TFT32, MKS_ROBIN_TFT35, MKS_ROBIN_TFT43, MKS_ROBIN_TFT_V1_1R, ANET_ET4_TFT28, ANET_ET5_TFT35, BIQU_BX_TFT70, BTT_TFT35_SPI_V1_0) \
  + COUNT_ENABLED(TFTGLCD_PANEL_SPI, TFTGLCD_PANEL_I2C) \
  + COUNT_ENABLED(VIKI2, miniVIKI) \
  + ENABLED(WYH_L12864) \
  + COUNT_ENABLED(ZONESTAR_12864LCD, ZONESTAR_12864OLED, ZONESTAR_12864OLED_SSD1306) \
  + COUNT_ENABLED(ANET_FULL_GRAPHICS_LCD, ANET_FULL_GRAPHICS_LCD_ALT_WIRING) \
  + ENABLED(AZSMZ_12864) \
  + ENABLED(BQ_LCD_SMART_CONTROLLER) \
  + ENABLED(CARTESIO_UI) \
  + ENABLED(ELB_FULL_GRAPHIC_CONTROLLER) \
  + ENABLED(FF_INTERFACEBOARD) \
  + ENABLED(FYSETC_242_OLED_12864) \
  + ENABLED(G3D_PANEL) \
  + ENABLED(LCD_FOR_MELZI) \
  + ENABLED(LCD_I2C_PANELOLU2) \
  + ENABLED(LCD_I2C_VIKI) \
  + ENABLED(LCM1602) \
  + ENABLED(LONGER_LK_TFT28) \
  + ENABLED(MAKEBOARD_MINI_2_LINE_DISPLAY_1602) \
  + ENABLED(MAKRPANEL) \
  + ENABLED(MALYAN_LCD) \
  + ENABLED(NEXTION_TFT) \
  + ENABLED(MKS_LCD12864A) \
  + ENABLED(MKS_LCD12864B) \
  + ENABLED(OLED_PANEL_TINYBOY2) \
  + ENABLED(OVERLORD_OLED) \
  + ENABLED(PANEL_ONE) \
  + ENABLED(RA_CONTROL_PANEL) \
  + ENABLED(RADDS_DISPLAY) \
  + ENABLED(REPRAPWORLD_GRAPHICAL_LCD) \
  + ENABLED(RIGIDBOT_PANEL) \
  + ENABLED(SAV_3DGLCD) \
  + ENABLED(SAV_3DLCD) \
  + ENABLED(SILVER_GATE_GLCD_CONTROLLER) \
  + ENABLED(TFT_TRONXY_X5SA) \
  + ENABLED(TOUCH_UI_FTDI_EVE) \
  + ENABLED(U8GLIB_SH1106_EINSTART) \
  + ENABLED(ULTI_CONTROLLER) \
  + ENABLED(ULTIMAKERCONTROLLER) \
  + ENABLED(ULTIPANEL) \
  + ENABLED(ULTRA_LCD) \
  + ENABLED(YHCB2004) \
  + ENABLED(ZONESTAR_LCD) \
  + ENABLED(K3D_FULL_GRAPHIC_SMART_CONTROLLER) \
  + ENABLED(K3D_242_OLED_CONTROLLER)
  #error "Please select only one LCD controller option."
#endif

#undef IS_U8GLIB_SSD1306
#undef IS_EXTUI

#if ANY(TFT_GENERIC, MKS_TS35_V2_0, MKS_ROBIN_TFT24, MKS_ROBIN_TFT28, MKS_ROBIN_TFT32, MKS_ROBIN_TFT35, MKS_ROBIN_TFT43, MKS_ROBIN_TFT_V1_1R, TFT_TRONXY_X5SA, ANYCUBIC_TFT35, ANYCUBIC_TFT35, LONGER_LK_TFT28, ANET_ET4_TFT28, ANET_ET5_TFT35, BIQU_BX_TFT70, BTT_TFT35_SPI_V1_0)
  #if NONE(TFT_COLOR_UI, TFT_CLASSIC_UI, TFT_LVGL_UI)
    #error "TFT_COLOR_UI, TFT_CLASSIC_UI, TFT_LVGL_UI is required for your TFT. Please enable one."
  #elif MANY(TFT_COLOR_UI, TFT_CLASSIC_UI, TFT_LVGL_UI)
    #error "Please select only one of TFT_COLOR_UI, TFT_CLASSIC_UI, or TFT_LVGL_UI."
  #endif
#elif ANY(TFT_COLOR_UI, TFT_CLASSIC_UI, TFT_LVGL_UI)
  #error "TFT_(COLOR|CLASSIC|LVGL)_UI requires a TFT display to be enabled."
#endif

#if ENABLED(TFT_GENERIC) && NONE(TFT_INTERFACE_FSMC, TFT_INTERFACE_SPI)
  #error "TFT_GENERIC requires either TFT_INTERFACE_FSMC or TFT_INTERFACE_SPI interface."
#endif

#if BOTH(TFT_INTERFACE_FSMC, TFT_INTERFACE_SPI)
  #error "Please enable only one of TFT_INTERFACE_SPI or TFT_INTERFACE_SPI."
#endif

#if defined(LCD_SCREEN_ROTATE) && LCD_SCREEN_ROTATE != 0 && LCD_SCREEN_ROTATE != 90 && LCD_SCREEN_ROTATE != 180 && LCD_SCREEN_ROTATE != 270
  #error "LCD_SCREEN_ROTATE must be 0, 90, 180, or 270."
#endif

#if MANY(TFT_RES_320x240, TFT_RES_480x272, TFT_RES_480x320, TFT_RES_1024x600)
  #error "Please select only one of TFT_RES_320x240, TFT_RES_480x272, TFT_RES_480x320, or TFT_RES_1024x600."
#endif

#if ENABLED(TFT_LVGL_UI)
  #if DISABLED(TFT_RES_480x320)
    #error "TFT_LVGL_UI requires TFT_RES_480x320."
  #elif DISABLED(SDSUPPORT)
    #error "TFT_LVGL_UI requires SDSUPPORT."
  #endif
#endif

#if defined(GRAPHICAL_TFT_UPSCALE) && !WITHIN(GRAPHICAL_TFT_UPSCALE, 2, 8)
  #error "GRAPHICAL_TFT_UPSCALE must be between 2 and 8."
#endif

#if BOTH(CHIRON_TFT_STANDARD, CHIRON_TFT_NEW)
  #error "Please select only one of CHIRON_TFT_STANDARD or CHIRON_TFT_NEW."
#endif

#if ENABLED(ANYCUBIC_LCD_CHIRON)
  #ifndef BEEPER_PIN
    #error "ANYCUBIC_LCD_CHIRON requires BEEPER_PIN"
  #elif DISABLED(SDSUPPORT)
    #error "ANYCUBIC_LCD_CHIRON requires SDSUPPORT"
  #elif TEMP_SENSOR_BED == 0
    #error "ANYCUBIC_LCD_CHIRON requires heatbed (TEMP_SENSOR_BED)"
  #elif NONE(AUTO_BED_LEVELING_BILINEAR, AUTO_BED_LEVELING_UBL, MESH_BED_LEVELING)
    #error "ANYCUBIC_LCD_CHIRON requires one of: AUTO_BED_LEVELING_BILINEAR, AUTO_BED_LEVELING_UBL or MESH_BED_LEVELING"
  #elif DISABLED(BABYSTEPPING)
    #error "ANYCUBIC_LCD_CHIRON requires BABYSTEPPING"
  #endif
#endif

#if EITHER(MKS_TS35_V2_0, BTT_TFT35_SPI_V1_0) && SD_CONNECTION_IS(LCD)
  #error "SDCARD_CONNECTION cannot be set to LCD for the enabled TFT. No available SD card reader."
#endif

/**
 * Ender 3 V2 controller has some limitations
 */
#if ENABLED(DWIN_CREALITY_LCD)
  #if DISABLED(SDSUPPORT)
    #error "DWIN_CREALITY_LCD requires SDSUPPORT to be enabled."
  #elif EITHER(PID_EDIT_MENU, PID_AUTOTUNE_MENU)
    #error "DWIN_CREALITY_LCD does not support PID_EDIT_MENU or PID_AUTOTUNE_MENU."
  #elif EITHER(MPC_EDIT_MENU, MPC_AUTOTUNE_MENU)
    #error "DWIN_CREALITY_LCD does not support MPC_EDIT_MENU or MPC_AUTOTUNE_MENU."
  #elif ENABLED(LCD_BED_TRAMMING)
    #error "DWIN_CREALITY_LCD does not support LCD_BED_TRAMMING."
  #elif BOTH(LCD_BED_LEVELING, PROBE_MANUALLY)
    #error "DWIN_CREALITY_LCD does not support LCD_BED_LEVELING with PROBE_MANUALLY."
  #endif
#elif ENABLED(DWIN_LCD_PROUI)
  #if DISABLED(SDSUPPORT)
    #error "DWIN_LCD_PROUI requires SDSUPPORT to be enabled."
  #elif EITHER(PID_EDIT_MENU, PID_AUTOTUNE_MENU)
    #error "DWIN_LCD_PROUI does not support PID_EDIT_MENU or PID_AUTOTUNE_MENU."
  #elif EITHER(MPC_EDIT_MENU, MPC_AUTOTUNE_MENU)
    #error "DWIN_LCD_PROUI does not support MPC_EDIT_MENU or MPC_AUTOTUNE_MENU."
  #elif ENABLED(LCD_BED_TRAMMING)
    #error "DWIN_LCD_PROUI does not support LCD_BED_TRAMMING."
  #elif BOTH(LCD_BED_LEVELING, PROBE_MANUALLY)
    #error "DWIN_LCD_PROUI does not support LCD_BED_LEVELING with PROBE_MANUALLY."
  #endif
#endif

#if LCD_BACKLIGHT_TIMEOUT
  #if !HAS_ENCODER_ACTION
    #error "LCD_BACKLIGHT_TIMEOUT requires an LCD with encoder or keypad."
  #elif !PIN_EXISTS(LCD_BACKLIGHT)
    #error "LCD_BACKLIGHT_TIMEOUT requires LCD_BACKLIGHT_PIN."
  #endif
#endif

/**
 * Display Sleep is not supported by these common displays
 */
#if HAS_DISPLAY_SLEEP
  #if ANY(IS_U8GLIB_LM6059_AF, IS_U8GLIB_ST7565_64128, REPRAPWORLD_GRAPHICAL_LCD, FYSETC_MINI, ENDER2_STOCKDISPLAY, MINIPANEL)
    #error "DISPLAY_SLEEP_MINUTES is not supported by your display."
  #elif !WITHIN(DISPLAY_SLEEP_MINUTES, 0, 255)
    #error "DISPLAY_SLEEP_MINUTES must be between 0 and 255."
  #endif
#endif

/**
 * Some boards forbid the use of -1 Native USB
 */
#if ENABLED(BOARD_NO_NATIVE_USB)
  #undef BOARD_NO_NATIVE_USB
  #if SERIAL_PORT == -1
    #error "SERIAL_PORT is set to -1, but the MOTHERBOARD has no native USB support. Set SERIAL_PORT to a valid value for your board."
  #elif SERIAL_PORT_2 == -1
    #error "SERIAL_PORT_2 is set to -1, but the MOTHERBOARD has no native USB support. Set SERIAL_PORT_2 to a valid value for your board."
  #elif MMU2_SERIAL_PORT == -1
    #error "MMU2_SERIAL_PORT is set to -1, but the MOTHERBOARD has no native USB support. Set MMU2_SERIAL_PORT to a valid value for your board."
  #elif LCD_SERIAL_PORT == -1
    #error "LCD_SERIAL_PORT is set to -1, but the MOTHERBOARD has no native USB support. Set LCD_SERIAL_PORT to a valid value for your board."
  #endif
#endif

/**
 * MMU2 require a dedicated serial port
 */
#ifdef MMU2_SERIAL_PORT
  #if MMU2_SERIAL_PORT == SERIAL_PORT
    #error "MMU2_SERIAL_PORT cannot be the same as SERIAL_PORT."
  #elif defined(SERIAL_PORT_2) && MMU2_SERIAL_PORT == SERIAL_PORT_2
    #error "MMU2_SERIAL_PORT cannot be the same as SERIAL_PORT_2."
  #elif defined(LCD_SERIAL_PORT) && MMU2_SERIAL_PORT == LCD_SERIAL_PORT
    #error "MMU2_SERIAL_PORT cannot be the same as LCD_SERIAL_PORT."
  #endif
#endif

/**
 * Serial displays require a dedicated serial port
 */
#ifdef LCD_SERIAL_PORT
  #if LCD_SERIAL_PORT == SERIAL_PORT
    #error "LCD_SERIAL_PORT cannot be the same as SERIAL_PORT."
  #elif defined(SERIAL_PORT_2) && LCD_SERIAL_PORT == SERIAL_PORT_2
    #error "LCD_SERIAL_PORT cannot be the same as SERIAL_PORT_2."
  #endif
#else
  #if HAS_DGUS_LCD
    #error "The DGUS LCD requires LCD_SERIAL_PORT to be defined."
  #elif EITHER(ANYCUBIC_LCD_I3MEGA, ANYCUBIC_LCD_CHIRON)
    #error "The ANYCUBIC LCD requires LCD_SERIAL_PORT to be defined."
  #elif ENABLED(MALYAN_LCD)
    #error "MALYAN_LCD requires LCD_SERIAL_PORT to be defined."
  #elif ENABLED(NEXTION_LCD)
    #error "NEXTION_LCD requires LCD_SERIAL_PORT to be defined."
  #endif
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
#elif INVALID_TMC_SPI(I)
  #error "An SPI driven TMC on I requires I_CS_PIN."
#elif INVALID_TMC_SPI(J)
  #error "An SPI driven TMC on J requires J_CS_PIN."
#elif INVALID_TMC_SPI(K)
  #error "An SPI driven TMC on K requires K_CS_PIN."
#elif INVALID_TMC_SPI(U)
  #error "An SPI driven TMC on U requires U_CS_PIN."
#elif INVALID_TMC_SPI(V)
  #error "An SPI driven TMC on V requires V_CS_PIN."
#elif INVALID_TMC_SPI(W)
  #error "An SPI driven TMC on W requires W_CS_PIN."
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
#elif HAS_I_AXIS && INVALID_TMC_UART(I)
  #error "TMC2208 or TMC2209 on I requires I_HARDWARE_SERIAL or I_SERIAL_(RX|TX)_PIN."
#elif HAS_J_AXIS && INVALID_TMC_UART(J)
  #error "TMC2208 or TMC2209 on J requires J_HARDWARE_SERIAL or J_SERIAL_(RX|TX)_PIN."
#elif HAS_K_AXIS && INVALID_TMC_UART(K)
  #error "TMC2208 or TMC2209 on K requires K_HARDWARE_SERIAL or K_SERIAL_(RX|TX)_PIN."
#elif HAS_U_AXIS && INVALID_TMC_UART(U)
  #error "TMC2208 or TMC2209 on U requires U_HARDWARE_SERIAL or U_SERIAL_(RX|TX)_PIN."
#elif HAS_V_AXIS && INVALID_TMC_UART(V)
  #error "TMC2208 or TMC2209 on V requires V_HARDWARE_SERIAL or V_SERIAL_(RX|TX)_PIN."
#elif HAS_W_AXIS && INVALID_TMC_UART(W)
  #error "TMC2208 or TMC2209 on W requires W_HARDWARE_SERIAL or W_SERIAL_(RX|TX)_PIN."

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
#elif AXIS_DRIVER_TYPE_I(TMC2209)
  INVALID_TMC_ADDRESS(I);
#elif AXIS_DRIVER_TYPE_J(TMC2209)
  INVALID_TMC_ADDRESS(J);
#elif AXIS_DRIVER_TYPE_K(TMC2209)
  INVALID_TMC_ADDRESS(K);
#elif AXIS_DRIVER_TYPE_U(TMC2209)
  INVALID_TMC_ADDRESS(U);
#elif AXIS_DRIVER_TYPE_V(TMC2209)
  INVALID_TMC_ADDRESS(V);
#elif AXIS_DRIVER_TYPE_W(TMC2209)
  INVALID_TMC_ADDRESS(W);
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
#elif HAS_I_AXIS && !TMC_MICROSTEP_IS_VALID(I)
  INVALID_TMC_MS(I)
#elif HAS_J_AXIS && !TMC_MICROSTEP_IS_VALID(J)
  INVALID_TMC_MS(J)
#elif HAS_K_AXIS && !TMC_MICROSTEP_IS_VALID(K)
  INVALID_TMC_MS(K)
#elif HAS_U_AXIS && !TMC_MICROSTEP_IS_VALID(U)
  INVALID_TMC_MS(U)
#elif HAS_V_AXIS && !TMC_MICROSTEP_IS_VALID(V)
  INVALID_TMC_MS(V)
#elif HAS_W_AXIS && !TMC_MICROSTEP_IS_VALID(W)
  INVALID_TMC_MS(W)
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
  #if HAS_I_AXIS
    #define I_ENDSTOP_INVERTING !AXIS_DRIVER_TYPE(I,TMC2209)
  #endif
  #if HAS_J_AXIS
    #define J_ENDSTOP_INVERTING !AXIS_DRIVER_TYPE(J,TMC2209)
  #endif
  #if HAS_K_AXIS
    #define K_ENDSTOP_INVERTING !AXIS_DRIVER_TYPE(K,TMC2209)
  #endif
  #if HAS_U_AXIS
    #define U_ENDSTOP_INVERTING !AXIS_DRIVER_TYPE(U,TMC2209)
  #endif
  #if HAS_V_AXIS
    #define V_ENDSTOP_INVERTING !AXIS_DRIVER_TYPE(V,TMC2209)
  #endif
  #if HAS_W_AXIS
    #define W_ENDSTOP_INVERTING !AXIS_DRIVER_TYPE(W,TMC2209)
  #endif

  #if NONE(SPI_ENDSTOPS, ONBOARD_ENDSTOPPULLUPS, ENDSTOPPULLUPS)
    #if   X_SENSORLESS && X_HOME_TO_MIN && DISABLED(ENDSTOPPULLUP_XMIN)
      #error "SENSORLESS_HOMING requires ENDSTOPPULLUP_XMIN (or ENDSTOPPULLUPS) when homing to X_MIN."
    #elif X_SENSORLESS && X_HOME_TO_MAX && DISABLED(ENDSTOPPULLUP_XMAX)
      #error "SENSORLESS_HOMING requires ENDSTOPPULLUP_XMAX (or ENDSTOPPULLUPS) when homing to X_MAX."
    #elif Y_SENSORLESS && Y_HOME_TO_MIN && DISABLED(ENDSTOPPULLUP_YMIN)
      #error "SENSORLESS_HOMING requires ENDSTOPPULLUP_YMIN (or ENDSTOPPULLUPS) when homing to Y_MIN."
    #elif Y_SENSORLESS && Y_HOME_TO_MAX && DISABLED(ENDSTOPPULLUP_YMAX)
      #error "SENSORLESS_HOMING requires ENDSTOPPULLUP_YMAX (or ENDSTOPPULLUPS) when homing to Y_MAX."
    #elif Z_SENSORLESS && Z_HOME_TO_MIN && DISABLED(ENDSTOPPULLUP_ZMIN)
      #error "SENSORLESS_HOMING requires ENDSTOPPULLUP_ZMIN (or ENDSTOPPULLUPS) when homing to Z_MIN."
    #elif Z_SENSORLESS && Z_HOME_TO_MAX && DISABLED(ENDSTOPPULLUP_ZMAX)
      #error "SENSORLESS_HOMING requires ENDSTOPPULLUP_ZMAX (or ENDSTOPPULLUPS) when homing to Z_MAX."
    #elif ALL(HAS_I_AXIS, I_SENSORLESS, I_HOME_TO_MIN) && DISABLED(ENDSTOPPULLUP_IMIN)
      #error "SENSORLESS_HOMING requires ENDSTOPPULLUP_IMIN (or ENDSTOPPULLUPS) when homing to I_MIN."
    #elif ALL(HAS_I_AXIS, I_SENSORLESS, I_HOME_TO_MAX) && DISABLED(ENDSTOPPULLUP_IMAX)
      #error "SENSORLESS_HOMING requires ENDSTOPPULLUP_IMAX (or ENDSTOPPULLUPS) when homing to I_MAX."
    #elif ALL(HAS_J_AXIS, J_SENSORLESS, J_HOME_TO_MIN) && DISABLED(ENDSTOPPULLUP_JMIN)
      #error "SENSORLESS_HOMING requires ENDSTOPPULLUP_JMIN (or ENDSTOPPULLUPS) when homing to J_MIN."
    #elif ALL(HAS_J_AXIS, J_SENSORLESS, J_HOME_TO_MAX) && DISABLED(ENDSTOPPULLUP_JMAX)
      #error "SENSORLESS_HOMING requires ENDSTOPPULLUP_JMAX (or ENDSTOPPULLUPS) when homing to J_MAX."
    #elif ALL(HAS_K_AXIS, K_SENSORLESS, K_HOME_TO_MIN) && DISABLED(ENDSTOPPULLUP_KMIN)
      #error "SENSORLESS_HOMING requires ENDSTOPPULLUP_KMIN (or ENDSTOPPULLUPS) when homing to K_MIN."
    #elif ALL(HAS_K_AXIS, K_SENSORLESS, K_HOME_TO_MAX) && DISABLED(ENDSTOPPULLUP_KMAX)
      #error "SENSORLESS_HOMING requires ENDSTOPPULLUP_KMAX (or ENDSTOPPULLUPS) when homing to K_MAX."
    #elif HAS_U_AXIS && U_SENSORLESS && U_HOME_TO_MIN && DISABLED(ENDSTOPPULLUP_UMIN)
      #error "SENSORLESS_HOMING requires ENDSTOPPULLUP_UMIN (or ENDSTOPPULLUPS) when homing to U_MIN."
    #elif HAS_U_AXIS && U_SENSORLESS && U_HOME_TO_MAX && DISABLED(ENDSTOPPULLUP_UMAX)
      #error "SENSORLESS_HOMING requires ENDSTOPPULLUP_UMAX (or ENDSTOPPULLUPS) when homing to U_MAX."
    #elif HAS_V_AXIS && V_SENSORLESS && V_HOME_TO_MIN && DISABLED(ENDSTOPPULLUP_VMIN)
      #error "SENSORLESS_HOMING requires ENDSTOPPULLUP_VMIN (or ENDSTOPPULLUPS) when homing to V_MIN."
    #elif HAS_V_AXIS && V_SENSORLESS && V_HOME_TO_MAX && DISABLED(ENDSTOPPULLUP_VMAX)
      #error "SENSORLESS_HOMING requires ENDSTOPPULLUP_VMAX (or ENDSTOPPULLUPS) when homing to V_MAX."
    #elif HAS_W_AXIS && W_SENSORLESS && W_HOME_TO_MIN && DISABLED(ENDSTOPPULLUP_WMIN)
      #error "SENSORLESS_HOMING requires ENDSTOPPULLUP_WMIN (or ENDSTOPPULLUPS) when homing to W_MIN."
    #elif HAS_W_AXIS && W_SENSORLESS && W_HOME_TO_MAX && DISABLED(ENDSTOPPULLUP_WMAX)
      #error "SENSORLESS_HOMING requires ENDSTOPPULLUP_WMAX (or ENDSTOPPULLUPS) when homing to W_MAX."

    #endif
  #endif

  #if ENABLED(SPI_ENDSTOPS)
    #if ENABLED(QUICK_HOME)
      #warning "SPI_ENDSTOPS may be unreliable with QUICK_HOME. Adjust back-offs for better results."
    #endif
  #else
    #if X_SENSORLESS && X_HOME_TO_MIN && X_MIN_ENDSTOP_INVERTING != X_ENDSTOP_INVERTING
      #if X_ENDSTOP_INVERTING
        #error "SENSORLESS_HOMING requires X_MIN_ENDSTOP_INVERTING = true when homing to X_MIN."
      #else
        #error "SENSORLESS_HOMING requires X_MIN_ENDSTOP_INVERTING = false when homing TMC2209 to X_MIN."
      #endif
    #elif X_SENSORLESS && X_HOME_TO_MAX && X_MAX_ENDSTOP_INVERTING != X_ENDSTOP_INVERTING
      #if X_ENDSTOP_INVERTING
        #error "SENSORLESS_HOMING requires X_MAX_ENDSTOP_INVERTING = true when homing to X_MAX."
      #else
        #error "SENSORLESS_HOMING requires X_MAX_ENDSTOP_INVERTING = false when homing TMC2209 to X_MAX."
      #endif
    #elif Y_SENSORLESS && Y_HOME_TO_MIN && Y_MIN_ENDSTOP_INVERTING != Y_ENDSTOP_INVERTING
      #if Y_ENDSTOP_INVERTING
        #error "SENSORLESS_HOMING requires Y_MIN_ENDSTOP_INVERTING = true when homing to Y_MIN."
      #else
        #error "SENSORLESS_HOMING requires Y_MIN_ENDSTOP_INVERTING = false when homing TMC2209 to Y_MIN."
      #endif
    #elif Y_SENSORLESS && Y_HOME_TO_MAX && Y_MAX_ENDSTOP_INVERTING != Y_ENDSTOP_INVERTING
      #if Y_ENDSTOP_INVERTING
        #error "SENSORLESS_HOMING requires Y_MAX_ENDSTOP_INVERTING = true when homing to Y_MAX."
      #else
        #error "SENSORLESS_HOMING requires Y_MAX_ENDSTOP_INVERTING = false when homing TMC2209 to Y_MAX."
      #endif
    #elif Z_SENSORLESS && Z_HOME_TO_MIN && Z_MIN_ENDSTOP_INVERTING != Z_ENDSTOP_INVERTING
      #if Z_ENDSTOP_INVERTING
        #error "SENSORLESS_HOMING requires Z_MIN_ENDSTOP_INVERTING = true when homing to Z_MIN."
      #else
        #error "SENSORLESS_HOMING requires Z_MIN_ENDSTOP_INVERTING = false when homing TMC2209 to Z_MIN."
      #endif
    #elif Z_SENSORLESS && Z_HOME_TO_MAX && Z_MAX_ENDSTOP_INVERTING != Z_ENDSTOP_INVERTING
      #if Z_ENDSTOP_INVERTING
        #error "SENSORLESS_HOMING requires Z_MAX_ENDSTOP_INVERTING = true when homing to Z_MAX."
      #else
        #error "SENSORLESS_HOMING requires Z_MAX_ENDSTOP_INVERTING = false when homing TMC2209 to Z_MAX."
      #endif
    #elif ALL(HAS_I_AXIS, I_SENSORLESS, I_HOME_TO_MIN) && I_MIN_ENDSTOP_INVERTING != I_ENDSTOP_INVERTING
      #if I_ENDSTOP_INVERTING
        #error "SENSORLESS_HOMING requires I_MIN_ENDSTOP_INVERTING = true when homing to I_MIN."
      #else
        #error "SENSORLESS_HOMING requires I_MIN_ENDSTOP_INVERTING = false when homing TMC2209 to I_MIN."
      #endif
    #elif ALL(HAS_I_AXIS, I_SENSORLESS, I_HOME_TO_MAX) && I_MAX_ENDSTOP_INVERTING != I_ENDSTOP_INVERTING
      #if I_ENDSTOP_INVERTING
        #error "SENSORLESS_HOMING requires I_MAX_ENDSTOP_INVERTING = true when homing to I_MAX."
      #else
        #error "SENSORLESS_HOMING requires I_MAX_ENDSTOP_INVERTING = false when homing TMC2209 to I_MAX."
      #endif
    #elif ALL(HAS_J_AXIS, J_SENSORLESS, J_HOME_TO_MIN) && J_MIN_ENDSTOP_INVERTING != J_ENDSTOP_INVERTING
      #if J_ENDSTOP_INVERTING
        #error "SENSORLESS_HOMING requires J_MIN_ENDSTOP_INVERTING = true when homing to J_MIN."
      #else
        #error "SENSORLESS_HOMING requires J_MIN_ENDSTOP_INVERTING = false when homing TMC2209 to J_MIN."
      #endif
    #elif ALL(HAS_J_AXIS, J_SENSORLESS, J_HOME_TO_MAX) && J_MAX_ENDSTOP_INVERTING != J_ENDSTOP_INVERTING
      #if J_ENDSTOP_INVERTING
        #error "SENSORLESS_HOMING requires J_MAX_ENDSTOP_INVERTING = true when homing to J_MAX."
      #else
        #error "SENSORLESS_HOMING requires J_MAX_ENDSTOP_INVERTING = false when homing TMC2209 to J_MAX."
      #endif
    #elif ALL(HAS_K_AXIS, K_SENSORLESS, K_HOME_TO_MIN) && K_MIN_ENDSTOP_INVERTING != K_ENDSTOP_INVERTING
      #if K_ENDSTOP_INVERTING
        #error "SENSORLESS_HOMING requires K_MIN_ENDSTOP_INVERTING = true when homing to K_MIN."
      #else
        #error "SENSORLESS_HOMING requires K_MIN_ENDSTOP_INVERTING = false when homing TMC2209 to K_MIN."
      #endif
    #elif ALL(HAS_K_AXIS, K_SENSORLESS, K_HOME_TO_MAX) && K_MAX_ENDSTOP_INVERTING != K_ENDSTOP_INVERTING
      #if K_ENDSTOP_INVERTING
        #error "SENSORLESS_HOMING requires K_MAX_ENDSTOP_INVERTING = true when homing to K_MAX."
      #else
        #error "SENSORLESS_HOMING requires K_MAX_ENDSTOP_INVERTING = false when homing TMC2209 to K_MAX."
      #endif
    #elif ALL(HAS_U_AXIS, U_SENSORLESS, U_HOME_TO_MIN) && U_MIN_ENDSTOP_INVERTING != U_ENDSTOP_INVERTING
      #if U_ENDSTOP_INVERTING
        #error "SENSORLESS_HOMING requires U_MIN_ENDSTOP_INVERTING = true when homing to U_MIN."
      #else
        #error "SENSORLESS_HOMING requires U_MIN_ENDSTOP_INVERTING = false when homing TMC2209 to U_MIN."
      #endif
    #elif ALL(HAS_U_AXIS, U_SENSORLESS, U_HOME_TO_MAX) && U_MAX_ENDSTOP_INVERTING != U_ENDSTOP_INVERTING
      #if U_ENDSTOP_INVERTING
        #error "SENSORLESS_HOMING requires U_MAX_ENDSTOP_INVERTING = true when homing to U_MAX."
      #else
        #error "SENSORLESS_HOMING requires U_MAX_ENDSTOP_INVERTING = false when homing TMC2209 to U_MAX."
      #endif
    #elif ALL(HAS_V_AXIS, V_SENSORLESS, V_HOME_TO_MIN) && V_MIN_ENDSTOP_INVERTING != V_ENDSTOP_INVERTING
      #if V_ENDSTOP_INVERTING
        #error "SENSORLESS_HOMING requires V_MIN_ENDSTOP_INVERTING = true when homing to V_MIN."
      #else
        #error "SENSORLESS_HOMING requires V_MIN_ENDSTOP_INVERTING = false when homing TMC2209 to V_MIN."
      #endif
    #elif ALL(HAS_V_AXIS, V_SENSORLESS, V_HOME_TO_MAX) && V_MAX_ENDSTOP_INVERTING != V_ENDSTOP_INVERTING
      #if V_ENDSTOP_INVERTING
        #error "SENSORLESS_HOMING requires V_MAX_ENDSTOP_INVERTING = true when homing to V_MAX."
      #else
        #error "SENSORLESS_HOMING requires V_MAX_ENDSTOP_INVERTING = false when homing TMC2209 to V_MAX."
      #endif
    #elif ALL(HAS_W_AXIS, W_SENSORLESS, W_HOME_TO_MIN) && W_MIN_ENDSTOP_INVERTING != W_ENDSTOP_INVERTING
      #if W_ENDSTOP_INVERTING
        #error "SENSORLESS_HOMING requires W_MIN_ENDSTOP_INVERTING = true when homing to W_MIN."
      #else
        #error "SENSORLESS_HOMING requires W_MIN_ENDSTOP_INVERTING = false when homing TMC2209 to W_MIN."
      #endif
    #elif ALL(HAS_W_AXIS, W_SENSORLESS, W_HOME_TO_MAX0) && W_MAX_ENDSTOP_INVERTING != W_ENDSTOP_INVERTING
      #if W_ENDSTOP_INVERTING
        #error "SENSORLESS_HOMING requires W_MAX_ENDSTOP_INVERTING = true when homing to W_MAX."
      #else
        #error "SENSORLESS_HOMING requires W_MAX_ENDSTOP_INVERTING = false when homing TMC2209 to W_MAX."
      #endif
    #endif
  #endif

  #if ENABLED(DELTA) && !BOTH(STEALTHCHOP_XY, STEALTHCHOP_Z)
    #error "SENSORLESS_HOMING on DELTA currently requires STEALTHCHOP_XY and STEALTHCHOP_Z."
  #elif ENDSTOP_NOISE_THRESHOLD
    #error "SENSORLESS_HOMING is incompatible with ENDSTOP_NOISE_THRESHOLD."
  #elif !(X_SENSORLESS || Y_SENSORLESS || Z_SENSORLESS)
    #error "SENSORLESS_HOMING requires a TMC stepper driver with StallGuard on X, Y, or Z axes."
  #endif

  #undef X_ENDSTOP_INVERTING
  #undef Y_ENDSTOP_INVERTING
  #undef Z_ENDSTOP_INVERTING
  #undef I_ENDSTOP_INVERTING
  #undef J_ENDSTOP_INVERTING
  #undef K_ENDSTOP_INVERTING
  #undef U_ENDSTOP_INVERTING
  #undef V_ENDSTOP_INVERTING
  #undef W_ENDSTOP_INVERTING
#endif

// Sensorless probing requirements
#if ENABLED(SENSORLESS_PROBING)
  #if ENABLED(DELTA) && !(X_SENSORLESS && Y_SENSORLESS && Z_SENSORLESS)
    #error "SENSORLESS_PROBING for DELTA requires TMC stepper drivers with StallGuard on X, Y, and Z axes."
  #elif ENABLED(Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN)
    #error "SENSORLESS_PROBING cannot be used with Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN."
  #elif ENABLED(USE_PROBE_FOR_Z_HOMING)
    #error "SENSORLESS_PROBING cannot be used with USE_PROBE_FOR_Z_HOMING."
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
#elif EITHER(MARKFORGED_XY, MARKFORGED_YX) && X_SENSORLESS != Y_SENSORLESS
  #error "MARKFORGED requires both X and Y to use sensorless homing if either one does."
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
#define IN_CHAIN(A) A##_CHAIN_POS > 0
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
    #elif IN_CHAIN(I)
      #define CS_COMPARE I_CS_PIN
    #elif IN_CHAIN(J)
      #define CS_COMPARE J_CS_PIN
    #elif IN_CHAIN(K)
      #define CS_COMPARE K_CS_PIN
    #elif IN_CHAIN(U)
      #define CS_COMPARE U_CS_PIN
    #elif IN_CHAIN(V)
      #define CS_COMPARE V_CS_PIN
    #elif IN_CHAIN(W)
      #define CS_COMPARE W_CS_PIN
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
      || BAD_CS_PIN(I) || BAD_CS_PIN(J) || BAD_CS_PIN(K) \
      || BAD_CS_PIN(U) || BAD_CS_PIN(V) || BAD_CS_PIN(W) \
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
#if HAS_MOTOR_CURRENT_I2C
  #if BOTH(DIGIPOT_MCP4018, DIGIPOT_MCP4451)
    #error "Enable only one of DIGIPOT_MCP4018 or DIGIPOT_MCP4451."
  #elif !MB(MKS_SBASE, AZTEEG_X5_GT, AZTEEG_X5_MINI, AZTEEG_X5_MINI_WIFI) \
    && (!defined(DIGIPOTS_I2C_SDA_X) || !defined(DIGIPOTS_I2C_SDA_Y) || !defined(DIGIPOTS_I2C_SDA_Z) || !defined(DIGIPOTS_I2C_SDA_E0) || !defined(DIGIPOTS_I2C_SDA_E1))
      #error "DIGIPOT_MCP4018/4451 requires DIGIPOTS_I2C_SDA_* pins to be defined."
  #endif
#endif

/**
 * Check per-axis initializers for errors
 */

#define __PLUS_TEST(I,A) && (sanity_arr_##A[_MIN(I,signed(COUNT(sanity_arr_##A)-1))] > 0)
#define _PLUS_TEST(A) (1 REPEAT2(14,__PLUS_TEST,A))
#if HAS_MULTI_EXTRUDER
  #define _EXTRA_NOTE " (Did you forget to enable DISTINCT_E_FACTORS?)"
#else
  #define _EXTRA_NOTE " (Should be " STRINGIFY(NUM_AXES) "+" STRINGIFY(E_STEPPERS) ")"
#endif

constexpr float sanity_arr_1[] = DEFAULT_AXIS_STEPS_PER_UNIT;
static_assert(COUNT(sanity_arr_1) >= LOGICAL_AXES,  "DEFAULT_AXIS_STEPS_PER_UNIT requires " _LOGICAL_AXES_STR "elements.");
static_assert(COUNT(sanity_arr_1) <= DISTINCT_AXES, "DEFAULT_AXIS_STEPS_PER_UNIT has too many elements." _EXTRA_NOTE);
static_assert(_PLUS_TEST(1), "DEFAULT_AXIS_STEPS_PER_UNIT values must be positive.");

constexpr float sanity_arr_2[] = DEFAULT_MAX_FEEDRATE;
static_assert(COUNT(sanity_arr_2) >= LOGICAL_AXES,  "DEFAULT_MAX_FEEDRATE requires " _LOGICAL_AXES_STR "elements.");
static_assert(COUNT(sanity_arr_2) <= DISTINCT_AXES, "DEFAULT_MAX_FEEDRATE has too many elements." _EXTRA_NOTE);
static_assert(_PLUS_TEST(2), "DEFAULT_MAX_FEEDRATE values must be positive.");

constexpr float sanity_arr_3[] = DEFAULT_MAX_ACCELERATION;
static_assert(COUNT(sanity_arr_3) >= LOGICAL_AXES,  "DEFAULT_MAX_ACCELERATION requires " _LOGICAL_AXES_STR "elements.");
static_assert(COUNT(sanity_arr_3) <= DISTINCT_AXES, "DEFAULT_MAX_ACCELERATION has too many elements." _EXTRA_NOTE);
static_assert(_PLUS_TEST(3), "DEFAULT_MAX_ACCELERATION values must be positive.");

constexpr float sanity_arr_4[] = HOMING_FEEDRATE_MM_M;
static_assert(COUNT(sanity_arr_4) == NUM_AXES,  "HOMING_FEEDRATE_MM_M requires " _NUM_AXES_STR "elements (and no others).");
static_assert(_PLUS_TEST(4), "HOMING_FEEDRATE_MM_M values must be positive.");

#ifdef MAX_ACCEL_EDIT_VALUES
  constexpr float sanity_arr_5[] = MAX_ACCEL_EDIT_VALUES;
  static_assert(COUNT(sanity_arr_5) >= LOGICAL_AXES, "MAX_ACCEL_EDIT_VALUES requires " _LOGICAL_AXES_STR "elements.");
  static_assert(COUNT(sanity_arr_5) <= LOGICAL_AXES, "MAX_ACCEL_EDIT_VALUES has too many elements. " _LOGICAL_AXES_STR "elements only.");
  static_assert(_PLUS_TEST(5), "MAX_ACCEL_EDIT_VALUES values must be positive.");
#endif

#ifdef MAX_FEEDRATE_EDIT_VALUES
  constexpr float sanity_arr_6[] = MAX_FEEDRATE_EDIT_VALUES;
  static_assert(COUNT(sanity_arr_6) >= LOGICAL_AXES, "MAX_FEEDRATE_EDIT_VALUES requires " _LOGICAL_AXES_STR "elements.");
  static_assert(COUNT(sanity_arr_6) <= LOGICAL_AXES, "MAX_FEEDRATE_EDIT_VALUES has too many elements. " _LOGICAL_AXES_STR "elements only.");
  static_assert(_PLUS_TEST(6), "MAX_FEEDRATE_EDIT_VALUES values must be positive.");
#endif

#ifdef MAX_JERK_EDIT_VALUES
  constexpr float sanity_arr_7[] = MAX_JERK_EDIT_VALUES;
  static_assert(COUNT(sanity_arr_7) >= LOGICAL_AXES, "MAX_JERK_EDIT_VALUES requires " _LOGICAL_AXES_STR "elements.");
  static_assert(COUNT(sanity_arr_7) <= LOGICAL_AXES, "MAX_JERK_EDIT_VALUES has too many elements. " _LOGICAL_AXES_STR "elements only.");
  static_assert(_PLUS_TEST(7), "MAX_JERK_EDIT_VALUES values must be positive.");
#endif

#ifdef MANUAL_FEEDRATE
  constexpr float sanity_arr_8[] = MANUAL_FEEDRATE;
  static_assert(COUNT(sanity_arr_8) >= LOGICAL_AXES, "MANUAL_FEEDRATE requires " _LOGICAL_AXES_STR "elements.");
  static_assert(COUNT(sanity_arr_8) <= LOGICAL_AXES, "MANUAL_FEEDRATE has too many elements. " _LOGICAL_AXES_STR "elements only.");
  static_assert(_PLUS_TEST(8), "MANUAL_FEEDRATE values must be positive.");
#endif

#undef __PLUS_TEST
#undef _PLUS_TEST
#undef _EXTRA_NOTE

#if BOTH(CNC_COORDINATE_SYSTEMS, NO_WORKSPACE_OFFSETS)
  #error "CNC_COORDINATE_SYSTEMS is incompatible with NO_WORKSPACE_OFFSETS."
#endif

#if !BLOCK_BUFFER_SIZE || !IS_POWER_OF_2(BLOCK_BUFFER_SIZE)
  #error "BLOCK_BUFFER_SIZE must be a power of 2."
#elif BLOCK_BUFFER_SIZE > 64
  #error "A very large BLOCK_BUFFER_SIZE is not needed and takes longer to drain the buffer on pause / cancel."
#endif

#if ENABLED(LED_CONTROL_MENU) && NONE(HAS_MARLINUI_MENU, DWIN_LCD_PROUI)
  #error "LED_CONTROL_MENU requires an LCD controller that implements the menu."
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

#if BOTH(X_AXIS_TWIST_COMPENSATION, NOZZLE_AS_PROBE)
  #error "X_AXIS_TWIST_COMPENSATION is incompatible with NOZZLE_AS_PROBE."
#endif

#if ENABLED(POWER_LOSS_RECOVERY)
  #if ENABLED(BACKUP_POWER_SUPPLY) && !PIN_EXISTS(POWER_LOSS)
    #error "BACKUP_POWER_SUPPLY requires a POWER_LOSS_PIN."
  #elif BOTH(POWER_LOSS_PULLUP, POWER_LOSS_PULLDOWN)
    #error "You can't enable POWER_LOSS_PULLUP and POWER_LOSS_PULLDOWN at the same time."
  #elif ENABLED(POWER_LOSS_RECOVER_ZHOME) && Z_HOME_TO_MAX
    #error "POWER_LOSS_RECOVER_ZHOME is not needed on a machine that homes to ZMAX."
  #elif BOTH(IS_CARTESIAN, POWER_LOSS_RECOVER_ZHOME) && Z_HOME_TO_MIN && !defined(POWER_LOSS_ZHOME_POS)
    #error "POWER_LOSS_RECOVER_ZHOME requires POWER_LOSS_ZHOME_POS for a Cartesian that homes to ZMIN."
  #endif
#endif

#if ENABLED(Z_STEPPER_AUTO_ALIGN)
  #if NUM_Z_STEPPERS <= 1
    #error "Z_STEPPER_AUTO_ALIGN requires more than one Z stepper."
  #elif !HAS_BED_PROBE
    #error "Z_STEPPER_AUTO_ALIGN requires a Z-bed probe."
  #elif HAS_Z_STEPPER_ALIGN_STEPPER_XY
    static_assert(WITHIN(Z_STEPPER_ALIGN_AMP, 0.5, 2.0), "Z_STEPPER_ALIGN_AMP must be between 0.5 and 2.0.");
    #if NUM_Z_STEPPERS < 3
      #error "Z_STEPPER_ALIGN_STEPPER_XY requires 3 or 4 Z steppers."
    #endif
  #endif
#endif

#if ENABLED(MECHANICAL_GANTRY_CALIBRATION)
  #if NONE(HAS_MOTOR_CURRENT_DAC, HAS_MOTOR_CURRENT_SPI, HAS_MOTOR_CURRENT_DAC, HAS_TRINAMIC_CONFIG, HAS_MOTOR_CURRENT_PWM)
    #error "Adjustable current drivers are highly recommended to prevent damage. Comment out this line to continue anyway."
  #elif !defined(GANTRY_CALIBRATION_CURRENT)
    #error "MECHANICAL_GANTRY_CALIBRATION Requires GANTRY_CALIBRATION_CURRENT to be set."
  #elif !defined(GANTRY_CALIBRATION_EXTRA_HEIGHT)
    #error "MECHANICAL_GANTRY_CALIBRATION Requires GANTRY_CALIBRATION_EXTRA_HEIGHT to be set."
  #elif !defined(GANTRY_CALIBRATION_FEEDRATE)
    #error "MECHANICAL_GANTRY_CALIBRATION Requires GANTRY_CALIBRATION_FEEDRATE to be set."
  #elif ENABLED(Z_MULTI_ENDSTOPS)
    #error "Sorry! MECHANICAL_GANTRY_CALIBRATION cannot be used with Z_MULTI_ENDSTOPS."
  #elif ENABLED(Z_STEPPER_AUTO_ALIGN)
    #error "Sorry! MECHANICAL_GANTRY_CALIBRATION cannot be used with Z_STEPPER_AUTO_ALIGN."
  #endif
  #if defined(GANTRY_CALIBRATION_SAFE_POSITION) && !defined(GANTRY_CALIBRATION_XY_PARK_FEEDRATE)
    #error "GANTRY_CALIBRATION_SAFE_POSITION Requires GANTRY_CALIBRATION_XY_PARK_FEEDRATE to be set."
  #endif
#endif

#if ENABLED(PRINTCOUNTER) && DISABLED(EEPROM_SETTINGS)
  #error "PRINTCOUNTER requires EEPROM_SETTINGS."
#endif

#if ENABLED(USB_FLASH_DRIVE_SUPPORT) && !PINS_EXIST(USB_CS, USB_INTR) && DISABLED(USE_OTG_USB_HOST)
  #error "USB_CS_PIN and USB_INTR_PIN are required for USB_FLASH_DRIVE_SUPPORT."
#endif

#if ENABLED(USE_OTG_USB_HOST) && !defined(HAS_OTG_USB_HOST_SUPPORT)
  #error "The current board does not support USE_OTG_USB_HOST."
#endif

#if ENABLED(SD_FIRMWARE_UPDATE) && !defined(__AVR_ATmega2560__)
  #error "SD_FIRMWARE_UPDATE requires an ATmega2560-based (Arduino Mega) board."
#endif

#if ENABLED(GCODE_MACROS) && !WITHIN(GCODE_MACROS_SLOTS, 1, 10)
  #error "GCODE_MACROS_SLOTS must be a number from 1 to 10."
#endif

#if ENABLED(BACKLASH_COMPENSATION)
  #ifndef BACKLASH_DISTANCE_MM
    #error "BACKLASH_COMPENSATION requires BACKLASH_DISTANCE_MM."
  #elif !defined(BACKLASH_CORRECTION)
    #error "BACKLASH_COMPENSATION requires BACKLASH_CORRECTION."
  #elif EITHER(MARKFORGED_XY, MARKFORGED_YX)
    constexpr float backlash_arr[] = BACKLASH_DISTANCE_MM;
    static_assert(!backlash_arr[CORE_AXIS_1] && !backlash_arr[CORE_AXIS_2],
                  "BACKLASH_COMPENSATION can only apply to " STRINGIFY(NORMAL_AXIS) " on a MarkForged system.");
  #elif IS_CORE
    constexpr float backlash_arr[] = BACKLASH_DISTANCE_MM;
    #ifndef CORE_BACKLASH
      static_assert(!backlash_arr[CORE_AXIS_1] && !backlash_arr[CORE_AXIS_2],
                  "BACKLASH_COMPENSATION can only apply to " STRINGIFY(NORMAL_AXIS) " with your CORE system.");
    #endif
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
    #error "PHOTO_SWITCH_POSITION requires PHOTO_POSITION."
  #elif PIN_EXISTS(CHDK) && defined(CHDK_DELAY)
    #error "CHDK_DELAY has been replaced by PHOTO_SWITCH_MS."
  #elif PIN_EXISTS(CHDK) && !defined(PHOTO_SWITCH_MS)
    #error "PHOTO_SWITCH_MS is required with CHDK_PIN."
  #elif defined(PHOTO_RETRACT_MM)
    static_assert(PHOTO_RETRACT_MM + 0 >= 0, "PHOTO_RETRACT_MM must be >= 0.");
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
 * Validate MKS_PWC
 */
#if ENABLED(MKS_PWC) && PSU_ACTIVE_STATE != HIGH
  #error "MKS_PWC requires PSU_ACTIVE_STATE to be set HIGH."
#endif

/**
 * Ensure this option is set intentionally
 */
#if ENABLED(PSU_CONTROL)
  #ifndef PSU_ACTIVE_STATE
    #error "PSU_CONTROL requires PSU_ACTIVE_STATE to be defined as 'HIGH' or 'LOW'."
  #elif !PIN_EXISTS(PS_ON)
    #error "PSU_CONTROL requires PS_ON_PIN."
  #elif POWER_OFF_DELAY < 0
    #error "POWER_OFF_DELAY must be a positive value."
  #elif ENABLED(POWER_OFF_WAIT_FOR_COOLDOWN) && !(defined(AUTO_POWER_E_TEMP) || defined(AUTO_POWER_CHAMBER_TEMP) || defined(AUTO_POWER_COOLER_TEMP))
    #error "POWER_OFF_WAIT_FOR_COOLDOWN requires AUTO_POWER_E_TEMP, AUTO_POWER_CHAMBER_TEMP, and/or AUTO_POWER_COOLER_TEMP."
  #endif
#endif

#if HAS_CUTTER
  #ifndef CUTTER_POWER_UNIT
    #error "CUTTER_POWER_UNIT is required with a spindle or laser."
  #elif !CUTTER_UNIT_IS(PWM255) && !CUTTER_UNIT_IS(PERCENT) && !CUTTER_UNIT_IS(RPM) && !CUTTER_UNIT_IS(SERVO)
    #error "CUTTER_POWER_UNIT must be PWM255, PERCENT, RPM, or SERVO."
  #endif

  #if ENABLED(LASER_FEATURE)
    #if ENABLED(SPINDLE_CHANGE_DIR)
      #error "SPINDLE_CHANGE_DIR and LASER_FEATURE are incompatible."
    #elif ENABLED(LASER_MOVE_G0_OFF)
      #error "LASER_MOVE_G0_OFF is no longer required, G0 and G28 cannot apply power."
    #elif ENABLED(LASER_MOVE_G28_OFF)
      #error "LASER_MOVE_G0_OFF is no longer required, G0 and G28 cannot apply power."
    #elif ENABLED(LASER_MOVE_POWER)
      #error "LASER_MOVE_POWER is no longer applicable."
    #endif
    #if ENABLED(LASER_POWER_TRAP)
      #if DISABLED(SPINDLE_LASER_USE_PWM)
        #error "LASER_POWER_TRAP requires SPINDLE_LASER_USE_PWM to function."
      #endif
    #endif
  #else
    #if SPINDLE_LASER_POWERUP_DELAY < 1
      #error "SPINDLE_LASER_POWERUP_DELAY must be greater than 0."
    #elif SPINDLE_LASER_POWERDOWN_DELAY < 1
      #error "SPINDLE_LASER_POWERDOWN_DELAY must be greater than 0."
    #endif
  #endif

  #define _PIN_CONFLICT(P) (PIN_EXISTS(P) && P##_PIN == SPINDLE_LASER_PWM_PIN)
  #if BOTH(SPINDLE_FEATURE, LASER_FEATURE)
    #error "Enable only one of SPINDLE_FEATURE or LASER_FEATURE."
  #elif NONE(SPINDLE_SERVO, SPINDLE_LASER_USE_PWM) && !PIN_EXISTS(SPINDLE_LASER_ENA)
    #error "(SPINDLE|LASER)_FEATURE requires SPINDLE_LASER_ENA_PIN, SPINDLE_LASER_USE_PWM, or SPINDLE_SERVO to control the power."
  #elif ENABLED(SPINDLE_CHANGE_DIR) && !PIN_EXISTS(SPINDLE_DIR)
    #error "SPINDLE_DIR_PIN is required for SPINDLE_CHANGE_DIR."
  #elif ENABLED(SPINDLE_LASER_USE_PWM)
    #if !defined(SPINDLE_LASER_PWM_PIN) || SPINDLE_LASER_PWM_PIN < 0
      #error "SPINDLE_LASER_PWM_PIN is required for SPINDLE_LASER_USE_PWM."
    #elif !_TEST_PWM(SPINDLE_LASER_PWM_PIN)
      #error "SPINDLE_LASER_PWM_PIN not assigned to a PWM pin."
    #elif !defined(SPINDLE_LASER_PWM_INVERT)
      #error "SPINDLE_LASER_PWM_INVERT is required for (SPINDLE|LASER)_FEATURE."
    #elif !(defined(SPEED_POWER_MIN) && defined(SPEED_POWER_MAX) && defined(SPEED_POWER_STARTUP))
      #error "SPINDLE_LASER_USE_PWM equation constant(s) missing."
    #elif _PIN_CONFLICT(X_MIN)
      #error "SPINDLE_LASER_USE_PWM pin conflicts with X_MIN_PIN."
    #elif _PIN_CONFLICT(X_MAX)
      #error "SPINDLE_LASER_USE_PWM pin conflicts with X_MAX_PIN."
    #elif _PIN_CONFLICT(Z_STEP)
      #error "SPINDLE_LASER_USE_PWM pin conflicts with Z_STEP_PIN."
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

  #ifdef LASER_SAFETY_TIMEOUT_MS
    static_assert(LASER_SAFETY_TIMEOUT_MS < (DEFAULT_STEPPER_DEACTIVE_TIME) * 1000UL, "LASER_SAFETY_TIMEOUT_MS must be less than DEFAULT_STEPPER_DEACTIVE_TIME (" STRINGIFY(DEFAULT_STEPPER_DEACTIVE_TIME) " seconds)");
  #endif

#endif

#if ENABLED(COOLANT_MIST) && !PIN_EXISTS(COOLANT_MIST)
  #error "COOLANT_MIST requires COOLANT_MIST_PIN to be defined."
#elif ENABLED(COOLANT_FLOOD) && !PIN_EXISTS(COOLANT_FLOOD)
  #error "COOLANT_FLOOD requires COOLANT_FLOOD_PIN to be defined."
#endif

#if NONE(HAS_MARLINUI_U8GLIB, EXTENSIBLE_UI, IS_DWIN_MARLINUI) && ENABLED(PRINT_PROGRESS_SHOW_DECIMALS)
  #error "PRINT_PROGRESS_SHOW_DECIMALS currently requires a Graphical LCD."
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
 * Touch Screen Calibration
 */
#if !MB(LINUX_RAMPS) && ENABLED(TFT_TOUCH_DEVICE_XPT2046) && DISABLED(TOUCH_SCREEN_CALIBRATION) \
    && !(defined(TOUCH_CALIBRATION_X) && defined(TOUCH_CALIBRATION_Y) && defined(TOUCH_OFFSET_X) && defined(TOUCH_OFFSET_Y))
  #error "TOUCH_CALIBRATION_[XY] and TOUCH_OFFSET_[XY] are required for resistive touch screens with TOUCH_SCREEN_CALIBRATION disabled."
#endif

/**
 * Sanity check for WIFI
 */
#if EITHER(ESP3D_WIFISUPPORT, WIFISUPPORT) && DISABLED(ARDUINO_ARCH_ESP32)
  #error "ESP3D_WIFISUPPORT or WIFISUPPORT requires an ESP32 MOTHERBOARD."
#endif

/**
 * Sanity Check for Password Feature
 */
#if ENABLED(PASSWORD_FEATURE)
  #if NONE(HAS_MARLINUI_MENU, PASSWORD_UNLOCK_GCODE, PASSWORD_CHANGE_GCODE)
    #error "Without PASSWORD_UNLOCK_GCODE, PASSWORD_CHANGE_GCODE, or a supported LCD there's no way to unlock the printer or set a password."
  #elif DISABLED(EEPROM_SETTINGS)
    #warning "PASSWORD_FEATURE settings will be lost on power-off without EEPROM_SETTINGS."
  #endif
#endif

/**
 * Sanity Check for MEATPACK and BINARY_FILE_TRANSFER Features
 */
#if BOTH(HAS_MEATPACK, BINARY_FILE_TRANSFER)
  #error "Either enable MEATPACK_ON_SERIAL_PORT_* or BINARY_FILE_TRANSFER, not both."
#endif

/**
 * Sanity Check for Slim LCD Menus and Probe Offset Wizard
 */
#if BOTH(SLIM_LCD_MENUS, PROBE_OFFSET_WIZARD)
  #error "SLIM_LCD_MENUS disables \"Advanced Settings > Probe Offsets > PROBE_OFFSET_WIZARD.\""
#endif

/**
 * Sanity check for unique start and stop values in NOZZLE_CLEAN_FEATURE
 */
#if ENABLED(NOZZLE_CLEAN_FEATURE)
  constexpr xyz_pos_t start_xyz[8] = NOZZLE_CLEAN_START_POINT,
                        end_xyz[8] = NOZZLE_CLEAN_END_POINT;
  #define _CLEAN_ASSERT(N) static_assert(N >= HOTENDS || end_xyz[N].x != start_xyz[N].x || TERN(NOZZLE_CLEAN_NO_Y, false, end_xyz[N].y != start_xyz[N].y), \
                        "NOZZLE_CLEAN Start and End must be made different on HOTEND " STRINGIFY(N))
  _CLEAN_ASSERT(0); _CLEAN_ASSERT(1);
  _CLEAN_ASSERT(2); _CLEAN_ASSERT(3);
  _CLEAN_ASSERT(4); _CLEAN_ASSERT(5);
  _CLEAN_ASSERT(6); _CLEAN_ASSERT(7);
  #undef _CLEAN_ASSERT
#endif

/**
 * Sanity check for MIXING_EXTRUDER & DISTINCT_E_FACTORS these are not compatible
 */
#if BOTH(MIXING_EXTRUDER, DISTINCT_E_FACTORS)
  #error "MIXING_EXTRUDER can't be used with DISTINCT_E_FACTORS. But you may use SINGLENOZZLE with DISTINCT_E_FACTORS."
#endif

/**
 * Sanity check for valid stepper driver types
 */
#define _BAD_DRIVER(A) (defined(A##_DRIVER_TYPE) && !_DRIVER_ID(A##_DRIVER_TYPE))
#if _BAD_DRIVER(X)
  #error "X_DRIVER_TYPE is not recognized."
#endif
#if _BAD_DRIVER(Y)
  #error "Y_DRIVER_TYPE is not recognized."
#endif
#if _BAD_DRIVER(Z)
  #error "Z_DRIVER_TYPE is not recognized."
#endif
#if _BAD_DRIVER(I)
  #error "I_DRIVER_TYPE is not recognized."
#endif
#if _BAD_DRIVER(J)
  #error "J_DRIVER_TYPE is not recognized."
#endif
#if _BAD_DRIVER(K)
  #error "K_DRIVER_TYPE is not recognized."
#endif
#if _BAD_DRIVER(U)
  #error "U_DRIVER_TYPE is not recognized."
#endif
#if _BAD_DRIVER(V)
  #error "V_DRIVER_TYPE is not recognized."
#endif
#if _BAD_DRIVER(W)
  #error "W_DRIVER_TYPE is not recognized."
#endif
#if _BAD_DRIVER(X2)
  #error "X2_DRIVER_TYPE is not recognized."
#endif
#if _BAD_DRIVER(Y2)
  #error "Y2_DRIVER_TYPE is not recognized."
#endif
#if _BAD_DRIVER(Z2)
  #error "Z2_DRIVER_TYPE is not recognized."
#endif
#if _BAD_DRIVER(Z3)
  #error "Z3_DRIVER_TYPE is not recognized."
#endif
#if _BAD_DRIVER(Z4)
  #error "Z4_DRIVER_TYPE is not recognized."
#endif
#if _BAD_DRIVER(E0)
  #error "E0_DRIVER_TYPE is not recognized."
#endif
#if _BAD_DRIVER(E1)
  #error "E1_DRIVER_TYPE is not recognized."
#endif
#if _BAD_DRIVER(E2)
  #error "E2_DRIVER_TYPE is not recognized."
#endif
#if _BAD_DRIVER(E3)
  #error "E3_DRIVER_TYPE is not recognized."
#endif
#if _BAD_DRIVER(E4)
  #error "E4_DRIVER_TYPE is not recognized."
#endif
#if _BAD_DRIVER(E5)
  #error "E5_DRIVER_TYPE is not recognized."
#endif
#if _BAD_DRIVER(E6)
  #error "E6_DRIVER_TYPE is not recognized."
#endif
#if _BAD_DRIVER(E7)
  #error "E7_DRIVER_TYPE is not recognized."
#endif
#undef _BAD_DRIVER

/**
 * Require certain features for DGUS_LCD_UI_RELOADED.
 */
#if ENABLED(DGUS_LCD_UI_RELOADED)
  #if BUFSIZE < 4
    #error "DGUS_LCD_UI_RELOADED requires a BUFSIZE of at least 4."
  #elif HOTENDS < 1
    #error "DGUS_LCD_UI_RELOADED requires at least 1 hotend."
  #elif EXTRUDERS < 1
    #error "DGUS_LCD_UI_RELOADED requires at least 1 extruder."
  #elif !HAS_HEATED_BED
    #error "DGUS_LCD_UI_RELOADED requires a heated bed."
  #elif FAN_COUNT < 1
    #error "DGUS_LCD_UI_RELOADED requires a fan."
  #elif !HAS_BED_PROBE
    #error "DGUS_LCD_UI_RELOADED requires a bed probe."
  #elif !HAS_MESH
    #error "DGUS_LCD_UI_RELOADED requires mesh leveling."
  #elif DISABLED(LCD_BED_TRAMMING)
    #error "DGUS_LCD_UI_RELOADED requires LCD_BED_TRAMMING."
  #elif DISABLED(BABYSTEP_ALWAYS_AVAILABLE)
    #error "DGUS_LCD_UI_RELOADED requires BABYSTEP_ALWAYS_AVAILABLE."
  #elif DISABLED(BABYSTEP_ZPROBE_OFFSET)
    #error "DGUS_LCD_UI_RELOADED requires BABYSTEP_ZPROBE_OFFSET."
  #elif ENABLED(AUTO_BED_LEVELING_UBL) && DISABLED(UBL_SAVE_ACTIVE_ON_M500)
    #warning "Without UBL_SAVE_ACTIVE_ON_M500, your mesh will not be saved when using the touchscreen."
  #endif
#endif

// Misc. Cleanup
#undef _TEST_PWM
#undef _NUM_AXES_STR
#undef _LOGICAL_AXES_STR

// JTAG support in the HAL
#if ENABLED(DISABLE_DEBUG) && !defined(JTAGSWD_DISABLE)
  #error "DISABLE_DEBUG is not supported for the selected MCU/Board."
#elif ENABLED(DISABLE_JTAG) && !defined(JTAG_DISABLE)
  #error "DISABLE_JTAG is not supported for the selected MCU/Board."
#endif

// Check requirements for upload.py
#if ENABLED(XFER_BUILD) && !BOTH(BINARY_FILE_TRANSFER, CUSTOM_FIRMWARE_UPLOAD)
  #error "BINARY_FILE_TRANSFER and CUSTOM_FIRMWARE_UPLOAD are required for custom upload."
#endif
