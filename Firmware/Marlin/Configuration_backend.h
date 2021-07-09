#pragma once

#define CONFIGURATION_BACKEND_H_VERSION 02000901

//===========================================================================
//======================= DO NOT MODIFY THIS FILE ===========================
//===========================================================================

#define UNIFIED_VERSION "TH3D UFW 2.30"

/**
 * ABL Probe Settings
 */

#if ENABLED(CUSTOM_PROBE)
  #define ABL_ENABLE
#endif
#if ENABLED(ENDER6_OEM)
  #define ABL_ENABLE
  #define NOZZLE_TO_PROBE_OFFSET { -40, -14, 0 }
#endif
#if ENABLED(ENDER3_MAX_OEM)
  #define ABL_ENABLE
  #define NOZZLE_TO_PROBE_OFFSET { 57, -9, 0 }
#endif
#if ENABLED(KP3S_OEM_MOUNT)
  #define ABL_ENABLE
  #define NOZZLE_TO_PROBE_OFFSET { 37, -3, 0 }
#endif
#if ENABLED(ANET_A2_OEM_MOUNT)
  #define ABL_ENABLE
  #define NOZZLE_TO_PROBE_OFFSET { -33, -22, 0 }
#endif
#if ENABLED(ANET_A6_OEM_MOUNT)
  #define ABL_ENABLE
  #define NOZZLE_TO_PROBE_OFFSET { -1, -54, 0 }
#endif
#if ENABLED(ANET_A8_OEM_MOUNT)
  #define ABL_ENABLE
  #define NOZZLE_TO_PROBE_OFFSET { -26, -40, 0 }
#endif
#if ENABLED(ANET_A8_PLUS_OEM_MOUNT)
  #define ABL_ENABLE
  #define NOZZLE_TO_PROBE_OFFSET { -24, -45, 0 }
#endif
#if ENABLED(ANET_E10_OEM_MOUNT)
  #define ABL_ENABLE
  #define NOZZLE_TO_PROBE_OFFSET { -38, -12, 0 }
#endif
#if ENABLED(ANET_E12_OEM_MOUNT)
  #define ABL_ENABLE
  #define NOZZLE_TO_PROBE_OFFSET { -38, -12, 0 }
#endif
#if ENABLED(ANET_E16_OEM_MOUNT)
  #define ABL_ENABLE
  #define NOZZLE_TO_PROBE_OFFSET { -38, -12, 0 }
#endif
#if ENABLED(SUNLU_S8_5015_EZABL)
  #define ABL_ENABLE
  #define NOZZLE_TO_PROBE_OFFSET { -47, -11, 0 }
#endif
#if ENABLED(AC_MEGA_ZERO_OEM)
  #define ABL_ENABLE
  #define NOZZLE_TO_PROBE_OFFSET { -52, -19, 0 }
#endif
#if ENABLED(SUNLU_S8_OEM)
  #define ABL_ENABLE
  #define NOZZLE_TO_PROBE_OFFSET { -53, -12, 0 }
#endif
#if ENABLED(KP3_OEM_MOUNT)
  #define NOZZLE_TO_PROBE_OFFSET { -46, -15, 0 }
  #define ABL_ENABLE
#endif
#if ENABLED(ENDER3_OEM)
  #define ABL_ENABLE
  #define NOZZLE_TO_PROBE_OFFSET { -44, -10, 0 }
#endif
#if ENABLED(ENDER5_OEM)
  #define ABL_ENABLE
  #define NOZZLE_TO_PROBE_OFFSET { -44, -10, 0 }
#endif
#if ENABLED(CR10_OEM)
  #define ABL_ENABLE
  #define NOZZLE_TO_PROBE_OFFSET { -44, -10, 0 }
#endif
#if ENABLED(CR10S_OEM)
  #define ABL_ENABLE
  #define NOZZLE_TO_PROBE_OFFSET { -44, -10, 0 }
#endif
#if ENABLED(ENDER5_PLUS_OEM)
  #define ABL_ENABLE
  #define NOZZLE_TO_PROBE_OFFSET { -44, -10, 0 }
#endif
#if ENABLED(CR20_OEM)
  #define ABL_ENABLE
  #define NOZZLE_TO_PROBE_OFFSET { -44, -10, 0 }
#endif
#if ENABLED(ENDER3_V2_OEM)
  #define NOZZLE_TO_PROBE_OFFSET { -48, -15, 0 }
  #define ABL_ENABLE
#endif
#if ENABLED(SV01_OEM_MOUNT)
  #define NOZZLE_TO_PROBE_OFFSET { 22, -50, 0 }
  #define ABL_ENABLE
#endif
#if ENABLED(CR10_VOLCANO)
  #define NOZZLE_TO_PROBE_OFFSET { 30, 12, 0 }
  #define ABL_ENABLE
#endif
#if ENABLED(TORNADO_VOLCANO)
  #define NOZZLE_TO_PROBE_OFFSET { 30, 12, 0 }
  #define ABL_ENABLE
#endif
#if ENABLED(CR10_V6HEAVYDUTY)
  #define NOZZLE_TO_PROBE_OFFSET { 63, 0, 0 }
  #define ABL_ENABLE
#endif
#if ENABLED(TORNADO_V6HEAVYDUTY)
  #define NOZZLE_TO_PROBE_OFFSET { 63, 0, 0 }
  #define ABL_ENABLE
#endif
#if ENABLED(TM3DAERO)
  #define NOZZLE_TO_PROBE_OFFSET { -51, -7, 0 }
  #define ABL_ENABLE
#endif
#if ENABLED(TM3DAERO_EXTENDED)
  #define NOZZLE_TO_PROBE_OFFSET { -55, -7, 0 }
  #define ABL_ENABLE
#endif
#if ENABLED(PETSFANG)
  #define NOZZLE_TO_PROBE_OFFSET { 48, -2, 0 }
  #define ABL_ENABLE
#endif
#if ENABLED(ENDER2_OEM)
  #define NOZZLE_TO_PROBE_OFFSET { -33, -10, 0 }
  #define ABL_ENABLE
#endif
#if ENABLED(ENDER2_V6)
  #define NOZZLE_TO_PROBE_OFFSET { -35, -2, 0 }
  #define ABL_ENABLE
#endif
#if ENABLED(CR10V2_OEM)
  #define NOZZLE_TO_PROBE_OFFSET { 52, 5, 0 }
  #define ABL_ENABLE
#endif
#if ENABLED(SIDEWINDER_X1_OEM)
  #define NOZZLE_TO_PROBE_OFFSET { 33, -39, 0 }
  #define ABL_ENABLE
#endif
#if ENABLED(CR10S_PRO_OEM)
  #define NOZZLE_TO_PROBE_OFFSET { -27, 0, 0 }
  #define ABL_ENABLE
#endif
#if ENABLED(CRX_OEM)
  #define NOZZLE_TO_PROBE_OFFSET { -5, -48, 0 }
  #define ABL_ENABLE
#endif
#if ENABLED(ZONESTAR_Z5F_STOCK_ABL)
  #define NOZZLE_TO_PROBE_OFFSET { 35, 0, 0 }
  #define ABL_ENABLE
#endif
#if ENABLED(TARANTULA_PRO_OEM)
  #define NOZZLE_TO_PROBE_OFFSET { -65, 0, 0 }
  #define ABL_ENABLE
#endif
#if ENABLED(TORNADO_OEM)
  #define NOZZLE_TO_PROBE_OFFSET { -37, -10, 0 }
  #define ABL_ENABLE
#endif
#if ENABLED(TAZ5_OEM)
  #define NOZZLE_TO_PROBE_OFFSET { -52, 15, 0 }
  #define ABL_ENABLE
#endif
#if ENABLED(WANHAO_I3_OEM)
  #define NOZZLE_TO_PROBE_OFFSET { -25, -38, 0 }
  #define ABL_ENABLE
#endif
#if ENABLED(WANHAO_D6_OEM)
  #define NOZZLE_TO_PROBE_OFFSET { 24, -16, 0 }
  #define ABL_ENABLE
#endif
#if ENABLED(WANHAO_I3_DIIICOOLER)
  #define NOZZLE_TO_PROBE_OFFSET { -36, -37, 0 }
  #define ABL_ENABLE
#endif
#if ENABLED(WANHAO_I3MINI_OEM_EZABLMINI)
  #define NOZZLE_TO_PROBE_OFFSET { -32, 0, 0 }
  #define ABL_ENABLE
#endif
#if ENABLED(WANHAO_I3MINI_OEM)
  #define NOZZLE_TO_PROBE_OFFSET { -36, 0, 0 }
  #define ABL_ENABLE
#endif
#if ENABLED(ALFAWISEU10_OEM)
  #define NOZZLE_TO_PROBE_OFFSET { 39, -47, 0 }
  #define ABL_ENABLE
#endif
#if ENABLED(ALFAWISEU10_PETSFANG)
  #define NOZZLE_TO_PROBE_OFFSET { -48, -2, 0 }
  #define ABL_ENABLE
#endif
#if ENABLED(ENDER4_OEM)
  #define NOZZLE_TO_PROBE_OFFSET { -53, -19, 0 }
  #define ABL_ENABLE
#endif
#if ENABLED(ADIM_I3P_OEM)
  #define NOZZLE_TO_PROBE_OFFSET { 33, -60, 0 }
  #define ABL_ENABLE
#endif
#if ENABLED(GEE_A10_V1_OEM)
  #define NOZZLE_TO_PROBE_OFFSET { -54, 0, 0 }
  #define ABL_ENABLE
#endif
#if ENABLED(GEE_A10_V2_OEM)
  #define NOZZLE_TO_PROBE_OFFSET { -52, 0, 0 }
  #define ABL_ENABLE
#endif
#if ENABLED(EZ300_OEM_MOUNT)
  #define NOZZLE_TO_PROBE_OFFSET { -32, -8, 0 }
  #define ABL_ENABLE
#endif

#if ENABLED(ABL_ENABLE)
  // **************NEW FEATURES FROM 2.0.8.1 UPDATE - NOT SURE IF USING YET OR NOT*******
  
  // Require minimum nozzle and/or bed temperature for probing
  //#define PREHEAT_BEFORE_PROBING
  #if ENABLED(PREHEAT_BEFORE_PROBING)
    #define PROBING_NOZZLE_TEMP 120   // (°C) Only applies to E0 at this time
    #define PROBING_BED_TEMP     50
  #endif
  
  /**
   * Normally G28 leaves leveling disabled on completion. Enable one of
   * these options to restore the prior leveling state or to always enable
   * leveling immediately after G28.
   */
  //#define RESTORE_LEVELING_AFTER_G28
  //#define ENABLE_LEVELING_AFTER_G28

  /**
   * Auto-leveling needs preheating
   */
  //#define PREHEAT_BEFORE_LEVELING
  #if ENABLED(PREHEAT_BEFORE_LEVELING)
    #define LEVELING_NOZZLE_TEMP 120   // (°C) Only applies to E0 at this time
    #define LEVELING_BED_TEMP     60
  #endif
  
  // Gradually reduce leveling correction until a set height is reached,
  // at which point movement will be level to the machine's XY plane.
  // The height can be set with M420 Z<height>
  #define ENABLE_LEVELING_FADE_HEIGHT
  #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
    #define DEFAULT_LEVELING_FADE_HEIGHT 10.0 // (mm) Default fade height.
  #endif
  
  // Require minimum nozzle and/or bed temperature for probing
  //#define PREHEAT_BEFORE_PROBING
  #if ENABLED(PREHEAT_BEFORE_PROBING)
    #define PROBING_NOZZLE_TEMP 120   // (°C) Only applies to E0 at this time
    #define PROBING_BED_TEMP     60
  #endif
  
  // **************************END NEW FEATURES FROM 2.0.8.1 UPDATE**********************
  
  #define SEGMENT_LEVELED_MOVES
  #define LEVELED_SEGMENT_LENGTH 5.0
  
  #undef Z_PROBE_OFFSET_RANGE_MIN
  #define Z_PROBE_OFFSET_RANGE_MIN    -10
  #undef Z_PROBE_OFFSET_RANGE_MAX
  #define Z_PROBE_OFFSET_RANGE_MAX     1
  
  #define Z_MIN_PROBE_REPEATABILITY_TEST
  #define Z_AFTER_HOMING               5
  #define Z_PROBE_LOW_POINT           -10
  
  #if DISABLED(BLTOUCH)
    #define FIX_MOUNTED_PROBE
  #endif
  
  #if ENABLED(SLOWER_PROBE_MOVES) || ENABLED(PROBING_STEPPERS_OFF)
    #define XY_PROBE_FEEDRATE (133*60)
  #else
    #if ENABLED(EZABL_SUPERFASTPROBE)
      #define XY_PROBE_FEEDRATE (266*60)
    #else
      #define XY_PROBE_FEEDRATE (200*60)
    #endif
  #endif
  
  #define PROBING_MARGIN EZABL_PROBE_EDGE
  
  #if ENABLED(FIX_MOUNTED_PROBE) && DISABLED(HEATERS_ON_DURING_PROBING)
    #define PROBING_HEATERS_OFF   
  #endif
  
  #define MULTIPLE_PROBING 2
  #define AUTO_BED_LEVELING_BILINEAR
  
  #define GRID_MAX_POINTS_X EZABL_POINTS
  #define GRID_MAX_POINTS_Y GRID_MAX_POINTS_X
  #define Z_SAFE_HOMING
  
  #if ENABLED(Z_SAFE_HOMING)
    #define Z_SAFE_HOMING_X_POINT X_CENTER  // X point for Z homing
    #define Z_SAFE_HOMING_Y_POINT Y_CENTER  // Y point for Z homing
  #endif
  
  // ABL Probe Logic Settings
  #if ENABLED(BLTOUCH) // BLTouch uses false
    #undef Z_MIN_PROBE_ENDSTOP_INVERTING
    #define Z_MIN_PROBE_ENDSTOP_INVERTING false
    #undef Z_MIN_ENDSTOP_INVERTING
    #define Z_MIN_ENDSTOP_INVERTING false
  #elif ENABLED(CR10S_PRO_STOCK_ABL) && ENABLED(CR10S_PRO)
    #undef Z_MIN_PROBE_ENDSTOP_INVERTING
    #define Z_MIN_PROBE_ENDSTOP_INVERTING false
    #undef Z_MIN_ENDSTOP_INVERTING
    #define Z_MIN_ENDSTOP_INVERTING false
  #else                // EZABL uses true
    #undef Z_MIN_PROBE_ENDSTOP_INVERTING
    #define Z_MIN_PROBE_ENDSTOP_INVERTING true
    #undef Z_MIN_ENDSTOP_INVERTING
    #define Z_MIN_ENDSTOP_INVERTING true
  #endif

#endif

/**
 * TH3D Sanity Checks
 */

#if ENABLED(ABL_ENABLE) && ENABLED(S_CURVE_ACCELERATION)
  #error "S_CURVE_ACCELERATION is not compatible with ABL systems. Disable this and re-compile."
#endif

#if ENABLED(BLTOUCH) && DISABLED(CUSTOM_PROBE)
  #error "You must uncomment the CUSTOM_PROBE option in the EZABL probe mount section and then enter your mount offsets into the Custom Probe section."
#endif

/**
 * Temp Settings
 */

#define PREVENT_COLD_EXTRUSION
#define EXTRUDE_MINTEMP 170

#define DUMMY_THERMISTOR_998_VALUE 25
#define DUMMY_THERMISTOR_999_VALUE 100
#define TEMP_SENSOR_REDUNDANT_MAX_DIFF 10

#define HEATER_0_MINTEMP   0
#define HEATER_1_MINTEMP   HEATER_0_MINTEMP
#define HEATER_2_MINTEMP   HEATER_0_MINTEMP
#define HEATER_3_MINTEMP   HEATER_0_MINTEMP
#define HEATER_4_MINTEMP   HEATER_0_MINTEMP
#define HEATER_5_MINTEMP   HEATER_0_MINTEMP
#define HEATER_6_MINTEMP   HEATER_0_MINTEMP
#define HEATER_7_MINTEMP   HEATER_0_MINTEMP
#define BED_MINTEMP        HEATER_0_MINTEMP

#if ENABLED(HIGH_TEMP_THERMISTOR)
  #define HEATER_0_MAXTEMP HIGH_TEMP_THERMISTOR_TEMP
  #define HEATER_1_MAXTEMP HIGH_TEMP_THERMISTOR_TEMP
  #define HEATER_2_MAXTEMP HIGH_TEMP_THERMISTOR_TEMP
  #define HEATER_3_MAXTEMP HIGH_TEMP_THERMISTOR_TEMP
  #define HEATER_4_MAXTEMP HIGH_TEMP_THERMISTOR_TEMP
  #define HEATER_5_MAXTEMP HIGH_TEMP_THERMISTOR_TEMP
  #define HEATER_6_MAXTEMP HIGH_TEMP_THERMISTOR_TEMP
  #define HEATER_7_MAXTEMP HIGH_TEMP_THERMISTOR_TEMP
#else
  #define HEATER_0_MAXTEMP 275
  #define HEATER_1_MAXTEMP HEATER_0_MAXTEMP
  #define HEATER_2_MAXTEMP HEATER_0_MAXTEMP
  #define HEATER_3_MAXTEMP HEATER_0_MAXTEMP
  #define HEATER_4_MAXTEMP HEATER_0_MAXTEMP
  #define HEATER_5_MAXTEMP HEATER_0_MAXTEMP
  #define HEATER_6_MAXTEMP HEATER_0_MAXTEMP
  #define HEATER_7_MAXTEMP HEATER_0_MAXTEMP
#endif

#if ENABLED(BED_HIGHTEMP)
  #define BED_MAXTEMP 150
#else
  #define BED_MAXTEMP 120
#endif

/**
 * Thermal Overshoot
 * During heatup (and printing) the temperature can often "overshoot" the target by many degrees
 * (especially before PID tuning). Setting the target temperature too close to MAXTEMP guarantees
 * a MAXTEMP shutdown! Use these values to forbid temperatures being set too close to MAXTEMP.
 */
#define HOTEND_OVERSHOOT 15   // (°C) Forbid temperatures over MAXTEMP - OVERSHOOT
#define BED_OVERSHOOT    10   // (°C) Forbid temperatures over MAXTEMP - OVERSHOOT
#define COOLER_OVERSHOOT  2   // (°C) Forbid temperatures closer than OVERSHOOT

#define TEMP_RESIDENCY_TIME      3
#define TEMP_WINDOW              1
#define TEMP_HYSTERESIS          3

#define TEMP_BED_RESIDENCY_TIME  3
#define TEMP_BED_WINDOW          1
#define TEMP_BED_HYSTERESIS      3

// PID Settings
#define PIDTEMP
#define BANG_MAX 255
#define PID_MAX BANG_MAX
#define PID_K1 0.95

#define PID_AUTOTUNE_MENU

#if DISABLED(SPACE_SAVER) && DISABLED(SKR_E3_MINI_BOARD)
  #define PID_EDIT_MENU
#endif

#ifndef DEFAULT_Kp
  #define DEFAULT_Kp 22.2 // Define Marlin default PID if no machine specific PID is defined.
  #define DEFAULT_Ki 1.08
  #define DEFAULT_Kd 114
#endif

#if ENABLED(ENABLE_PIDBED) // PID Bed is not needed. Bang Bang loop is set to 500ms
  #define PIDTEMPBED
  #define MAX_BED_POWER 255
  #if ENABLED(PIDTEMPBED)
    #ifndef DEFAULT_bedKp
      #define  DEFAULT_bedKp 113.36
      #define  DEFAULT_bedKi 21.62
      #define  DEFAULT_bedKd 148.59
    #endif
  #endif
#endif

#if EITHER(PIDTEMP, PIDTEMPBED)
  #define PID_FUNCTIONAL_RANGE 10
#endif
// End PID Settings

// Enable Thermal Protection for Bed and Hotends
#define THERMAL_PROTECTION_HOTENDS
#define THERMAL_PROTECTION_BED

/**
 * General Firmware Settings
 */

#define STRING_CONFIG_H_AUTHOR "TH3D Studio"
#define CUSTOM_VERSION_FILE Version.h

#if ENABLED(CUSTOM_PRINTER_NAME)
  #define CUSTOM_MACHINE_NAME USER_PRINTER_NAME
#else
  #if ENABLED(EZBOARD_DIY)
    #define CUSTOM_MACHINE_NAME "EZBoard DIY"
  #elif ENABLED(BLTOUCH)
    #define CUSTOM_MACHINE_NAME "TH3D BLTouch"
  #elif ENABLED(ABL_ENABLE) && DISABLED(BLTOUCH)
    #define CUSTOM_MACHINE_NAME "TH3D EZABL"
  #elif ENABLED(EZOUT_ENABLE)
    #define CUSTOM_MACHINE_NAME "TH3D EZOut"
  #elif ENABLED(CR10S_PRO_STOCK_ABL) && ENABLED(CR10S_PRO)
    #define CUSTOM_MACHINE_NAME "CR-10S Pro"
  #else
    #define CUSTOM_MACHINE_NAME SHORT_BUILD_VERSION
  #endif
#endif

#define DEFAULT_NOMINAL_FILAMENT_DIA 1.75

// Homing Speed Settings XYZ
#if ENABLED(SLOWER_HOMING)
  #define HOMING_FEEDRATE_MM_M { (20*60), (20*60), (4*60) }
  #define Z_PROBE_FEEDRATE_FAST (4*60)
#else
  #if ENABLED(EZABL_SUPERFASTPROBE) && ENABLED(ABL_ENABLE) && DISABLED(BLTOUCH)
    #define HOMING_FEEDRATE_MM_M { (50*60), (50*60), (15*60) }
    #define Z_PROBE_FEEDRATE_FAST (15*60)
  #elif ENABLED(EZABL_FASTPROBE) && ENABLED(ABL_ENABLE)
    #define HOMING_FEEDRATE_MM_M { (50*60), (50*60), (8*60) }
    #define Z_PROBE_FEEDRATE_FAST (8*60)
  #else
    #define HOMING_FEEDRATE_MM_M { (50*60), (50*60), (5*60) }
    #define Z_PROBE_FEEDRATE_FAST (5*60)
  #endif
#endif

#define Z_MIN_POS 0
#define X_MAX_POS X_BED_SIZE
#define Y_MAX_POS Y_BED_SIZE
#define VALIDATE_HOMING_ENDSTOPS
#define PRINTJOB_TIMER_AUTOSTART

#if DISABLED(NO_SDCARD)
  #define SDSUPPORT
#endif

#if DISABLED(SPEAKER_KILL)
  #define SPEAKER
#endif

#if ENABLED(FAN_FIX)
  #define FAN_SOFT_PWM
  #define SOFT_PWM_SCALE 1
#else
  #define SOFT_PWM_SCALE 0
#endif

#if ENABLED(BLTOUCH)
  #define Z_CLEARANCE_DEPLOY_PROBE   8
  #define Z_CLEARANCE_BETWEEN_PROBES 5
  #define Z_CLEARANCE_MULTI_PROBE    5
  #define Z_PROBE_FEEDRATE_SLOW (Z_PROBE_FEEDRATE_FAST / 2)
  #define ENDSTOPPULLUP_ZMIN
  #define ENDSTOPPULLUP_ZMIN_PROBE
#elif ENABLED(EZABL_SUPERFASTPROBE) && ENABLED(ABL_ENABLE)
  #define Z_CLEARANCE_DEPLOY_PROBE   2
  #define Z_CLEARANCE_BETWEEN_PROBES 2
  #define Z_CLEARANCE_MULTI_PROBE    2
  #define Z_PROBE_FEEDRATE_SLOW (Z_PROBE_FEEDRATE_FAST / 1.5)
#else
  #define Z_CLEARANCE_DEPLOY_PROBE   5
  #define Z_CLEARANCE_BETWEEN_PROBES 3
  #define Z_CLEARANCE_MULTI_PROBE    3
  #define Z_PROBE_FEEDRATE_SLOW (Z_PROBE_FEEDRATE_FAST / 2)
#endif

#define HOST_KEEPALIVE_FEATURE
#define DEFAULT_KEEPALIVE_INTERVAL 2

#if DISABLED(SPACE_SAVER)
  #define BUSY_WHILE_HEATING
#endif

#define EEPROM_SETTINGS

#if ENABLED(SPACE_SAVER)
  //#define DISABLE_M503
#endif

#define EEPROM_CHITCHAT
#define EEPROM_BOOT_SILENT

#if DISABLED(SPACE_SAVER) && DISABLED(SPACE_SAVER_2560)
  #define EEPROM_AUTO_INIT
#endif

#define MIN_SOFTWARE_ENDSTOPS
#if ENABLED(MIN_SOFTWARE_ENDSTOPS)
  #define MIN_SOFTWARE_ENDSTOP_X
  #define MIN_SOFTWARE_ENDSTOP_Y
  #if DISABLED(ABL_ENABLE)
    #define MIN_SOFTWARE_ENDSTOP_Z
  #endif
#endif

#define MAX_SOFTWARE_ENDSTOPS
#if ENABLED(MAX_SOFTWARE_ENDSTOPS)
  #define MAX_SOFTWARE_ENDSTOP_X
  #define MAX_SOFTWARE_ENDSTOP_Y
  #define MAX_SOFTWARE_ENDSTOP_Z
#endif

#define PREHEAT_1_LABEL       "PLA"
#define PREHEAT_1_TEMP_HOTEND 200
#define PREHEAT_1_TEMP_BED     60
#define PREHEAT_1_FAN_SPEED     0

#define PREHEAT_2_LABEL       "ABS"
#define PREHEAT_2_TEMP_HOTEND 240
#define PREHEAT_2_TEMP_BED    100
#define PREHEAT_2_FAN_SPEED     0

#define NOZZLE_PARK_FEATURE
#if ENABLED(NOZZLE_PARK_FEATURE)
  #define NOZZLE_PARK_POINT { 10, 10, 10 }
  #define NOZZLE_PARK_XY_FEEDRATE 100  
  #define NOZZLE_PARK_Z_FEEDRATE 5
#endif

#define DISABLE_X false
#define DISABLE_Y false
#define DISABLE_Z false
#define DISABLE_E false
#define DISABLE_INACTIVE_EXTRUDER

#define ENDSTOP_NOISE_THRESHOLD 2

#define SERVO_DELAY { 300 }

/**
 * LCD General Settings
 */

#define LCD_LANGUAGE en
#define DISPLAY_CHARSET_HD44780 JAPANESE
#define LCD_INFO_SCREEN_STYLE 0
#define DISABLE_REDUCED_ACCURACY_WARNING
#if DISABLED(DWIN_CREALITY_LCD) && DISABLED(LCD2004) && DISABLED(TFT_COLOR_UI)
  #define SHOW_BOOTSCREEN
  #define SHOW_CUSTOM_BOOTSCREEN
#endif

#if ENABLED(TH3D_STATUS_SCREEN_LOGO)
  #define CUSTOM_STATUS_SCREEN_IMAGE
#endif

#if DISABLED(DWIN_CREALITY_LCD) && DISABLED(SPACE_SAVER)
  #define INDIVIDUAL_AXIS_HOMING_MENU
#endif

// NEW OPTIONS WITH 2.0.8.1 UPDATE
// Add a menu item to move between bed corners for manual bed adjustment
#define LEVEL_BED_CORNERS

#if ENABLED(LEVEL_BED_CORNERS)
  #define LEVEL_CORNERS_INSET_LFRB { 30, 30, 30, 30 } // (mm) Left, Front, Right, Back insets
  #define LEVEL_CORNERS_HEIGHT      0.0   // (mm) Z height of nozzle at leveling points
  #define LEVEL_CORNERS_Z_HOP       5.0   // (mm) Z height of nozzle between leveling points
  #define LEVEL_CENTER_TOO              // Move to the center after the last corner
  //#define LEVEL_CORNERS_USE_PROBE
  #if ENABLED(LEVEL_CORNERS_USE_PROBE)
    #define LEVEL_CORNERS_PROBE_TOLERANCE 0.1
    #define LEVEL_CORNERS_VERIFY_RAISED   // After adjustment triggers the probe, re-probe to verify
    //#define LEVEL_CORNERS_AUDIO_FEEDBACK
  #endif

  /**
   * Corner Leveling Order
   *
   * Set 2 or 4 points. When 2 points are given, the 3rd is the center of the opposite edge.
   *
   *  LF  Left-Front    RF  Right-Front
   *  LB  Left-Back     RB  Right-Back
   *
   * Examples:
   *
   *      Default        {LF,RB,LB,RF}         {LF,RF}           {LB,LF}
   *  LB --------- RB   LB --------- RB    LB --------- RB   LB --------- RB
   *  |  4       3  |   | 3         2 |    |     <3>     |   | 1           |
   *  |             |   |             |    |             |   |          <3>|
   *  |  1       2  |   | 1         4 |    | 1         2 |   | 2           |
   *  LF --------- RF   LF --------- RF    LF --------- RF   LF --------- RF
   */
  #define LEVEL_CORNERS_LEVELING_ORDER { LF, RF, RB, LB }
#endif

#if ENABLED(MANUAL_MESH_LEVELING) && DISABLED(ABL_ENABLE)
  #define LCD_BED_LEVELING
  #define MESH_BED_LEVELING
  #define RESTORE_LEVELING_AFTER_G28
  #define MESH_EDIT_Z_STEP  0.025
  #define LCD_PROBE_Z_RANGE 4
  #define MESH_INSET EZABL_PROBE_EDGE
  
  #define SEGMENT_LEVELED_MOVES
  #define LEVELED_SEGMENT_LENGTH 5.0

  #define GRID_MAX_POINTS_X 3
  #define GRID_MAX_POINTS_Y GRID_MAX_POINTS_X
  
  #if DISABLED(SPACE_SAVER)
    #define MESH_EDIT_MENU
  #endif
#endif