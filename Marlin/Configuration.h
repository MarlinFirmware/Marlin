/**
 * For directions on how to use this firmware visit http://uf2.th3dstudio.com
 * THIS VERSION IS NOT FOR PRODUCTION USE AT THIS TIME AND ONLY AVAILABLE FOR TESTING PURPOSES
 * NO IMPLIED SUPPORT OR WARRANTY IS PROVIDED WITH THIS FIRMWARE RELEASE
 */
#pragma once
#define CONFIGURATION_H_VERSION 020006

//===========================================================================
//============================ TH3D Configuration ===========================
//===========================================================================

// ONLY UNCOMMENT THINGS IN ONE PRINTER SECTION!!! IF YOU HAVE MULTIPLE MACHINES FLASH THEM ONE AT A TIME.

//===========================================================================
// *************************   CREALITY PRINTERS    *************************
//===========================================================================

//===========================================================================
// Creality Ender 3 Options
// NOTE: If you have the MicroUSB port use the ENDER3_V4_BOARD option below
//===========================================================================
//#define ENDER3
//#define ENDER3_V4_BOARD

// EZABL Probe Mounts
//#define ENDER3_OEM
//#define CUSTOM_PROBE

//===========================================================================
// Creality Ender 3 V2 Options
//===========================================================================
//#define ENDER3_V2

// EZABL Probe Mounts
//#define ENDER3_V2_OEM
//#define CUSTOM_PROBE

//===========================================================================
// *************************  END PRINTER SECTION   *************************
//===========================================================================

//===========================================================================
// EZABL Advanced Settings
//===========================================================================

// If you want more or less EZABL probe points change the number below (only used if EZABL enabled)
// Default is 3 which gives you 3x3 grid for a total of 9 points. STICK WITH ODD NUMBERS
#define EZABL_POINTS 3

// If you want to change how far in or out the probe senses change EZABL_PROBE_EDGE value below
// Most Machines - 35
// Binder Clips? - 50
#define EZABL_PROBE_EDGE 35

// If you have issues with your machine running the faster probe setting disable the #define EZABL_FASTPROBE below.
// NOTE: Most machines will work with the fast probe enabled. Use M48 to verify accuracy.
#define EZABL_FASTPROBE

// Superfast probing - Only works with the EZABL Pro Sensors
//#define EZABL_SUPERFASTPROBE

// Heaters will stay on during probing - only use if directed to by support. Do not use on AC beds.
//#define HEATERS_ON_DURING_PROBING

// Does your machine make weird noises/vibrations when it is probing the mesh? Enable this to slow down the speed between probe points.
//#define SLOWER_PROBE_MOVES

//================================================================================
// IF YOU HAVE A CUSTOM PROBE MOUNT OR ONE THAT IS NOT PRE-SUPPORTED UNCOMMENT THE
// CUSTOM_PROBE OPTION IN YOUR PRINTER SECTION AND ENTER YOUR PROBE LOCATION BELOW
//================================================================================
#if ENABLED(CUSTOM_PROBE)
  /**
  * Z Probe to nozzle (X,Y) offset, relative to (0, 0).
  *
  * In the following example the X and Y offsets are both positive:
  *
  *   #define NOZZLE_TO_PROBE_OFFSET { 10, 10, 0 }
  *
  *     +-- BACK ---+
  *     |           |
  *   L |    (+) P  | R <-- probe (20,20)
  *   E |           | I
  *   F | (-) N (+) | G <-- nozzle (10,10)
  *   T |           | H
  *     |    (-)    | T
  *     |           |
  *     O-- FRONT --+
  *   (0,0)
  *
  * Specify a Probe position as { X, Y, Z }
  * Do NOT enter an number for the Z position in here. Store your offset in EEPROM.
  */
  #define NOZZLE_TO_PROBE_OFFSET { 10, 10, 0 }
#endif


//===========================================================================
//******************** EXTRA FEATURES AND TWEAKS ****************************
//===========================================================================

// EXTRUDER SETTINGS -------------------------------
// Use to set custom esteps and/or reverse your E Motor direction if you are installing an extruder that needs the direction reversed.
// If you reversed the wiring on your E motor already (like the Bondtech Guide says to do) then you do not need to reverse it in the firmware here.

// If you want to change the Esteps for your printer you can uncomment the below line and set CUSTOM_ESTEPS_VALUE to what you want - USE WHOLE NUMBERS ONLY
// This option sets the esteps from the CUSTOM_ESTEPS_VALUE line below.
// If you need to reverse the e motor direction also enabled the REVERSE_E_MOTOR_DIRECTION option.
// Example EStep Values: TH3D Aluminum Extruder - 95 ESteps, TH3D Tough Extruder - 463 ESteps, BMG Extruder - 415 ESteps
// When installing a Tough Extruder or E3D Titan or Bondtech that is Geared you likely need to enable the REVERSE_E_MOTOR_DIRECTION option
//#define CUSTOM_ESTEPS
#define CUSTOM_ESTEPS_VALUE 463
//#define REVERSE_E_MOTOR_DIRECTION

// THERMISTOR SETTINGS -----------------------------

// If you are using an E3D V6 Hotend (or Hemera) with their cartridge thermistor (not glass version) uncomment the below line.
//#define V6_HOTEND

// If you are using a Tough Hotend from TH3D or any thermistors TH3D sells for your hotend uncomment the below line.
//#define TH3D_HOTEND_THERMISTOR

// If you are using a known hotend thermistor value uncomment the below 2 lines and enter the thermistor number replacing the X after the #define KNOWN_HOTEND_THERMISTOR_VALUE
//#define KNOWN_HOTEND_THERMISTOR
//#define KNOWN_HOTEND_THERMISTOR_VALUE X

// If you have a hotend and thermistor capable of over 290C you can set the max temp value below.
// Setting this higher than 290C on a stock or traditional thermistor will damage it. Refer to your thermistor documentation to see what max temp is.
//#define HIGH_TEMP_THERMISTOR
#define HIGH_TEMP_THERMISTOR_TEMP 350

// BED THERMISTOR SETTINGS -------------------------

// If you are using a thermistor TH3D sells for your bed uncomment the below line.
//#define TH3D_BED_THERMISTOR

// If you are using a Keenovo with SSR and the Keenovo temperature sensor uncomment the below line.
//#define KEENOVO_TEMPSENSOR

// If you are using a known bed thermistor value uncomment the below 2 lines and enter the thermistor number replacing the X after the #define KNOWN_BED_THERMISTOR_VALUE
//#define KNOWN_BED_THERMISTOR
//#define KNOWN_BED_THERMISTOR_VALUE X

// If you are using an AC bed with a standalone controller (Keenovo) uncomment the below line to disable the heated bed in the firmware
//#define AC_BED

// MISC --------------------------------------------

// If you have a 5015 fan that whines when under 100% speed uncomment the below line.
//#define FAN_FIX

// Use your own printer name
//#define USER_PRINTER_NAME "CHANGE ME"

// If your printer is homing to the endstops hard uncomment this to change the homing speed/divisor to make it less aggressive.
//#define SLOWER_HOMING

//===========================================================================
// **********************  END CONFIGURATION SETTINGS   *********************
//===========================================================================

/**
 * ****************************DO NOT TOUCH ANYTHING BELOW THIS COMMENT**************************
 * Core machine settings are below. Do NOT modify these unless you understand what you are doing.
 */

/**
 * Machine Configuration Settings
 */
 
//Ender 3 Settings
#if ENABLED(ENDER3) || ENABLED(ENDER3_V4_BOARD)
  #define SERIAL_PORT 0

  #define BAUDRATE 115200
  
  #define REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER

  #if ENABLED(ENDER3_V4_BOARD)
    #ifndef MOTHERBOARD
      #define MOTHERBOARD BOARD_CREALITY_V4
    #endif
  #else
    #ifndef MOTHERBOARD
      #define MOTHERBOARD BOARD_MELZI_CREALITY
    #endif
  #endif

  #if ENABLED(CUSTOM_ESTEPS)
    #define DEFAULT_AXIS_STEPS_PER_UNIT   { 80, 80, 400, CUSTOM_ESTEPS_VALUE }
  #else
    #define DEFAULT_AXIS_STEPS_PER_UNIT   { 80, 80, 400, 95 }
  #endif
  #define DEFAULT_MAX_FEEDRATE          { 500, 500, 15, 25 }
  #define DEFAULT_MAX_ACCELERATION      { 500, 500, 100, 1000 }

  #define DEFAULT_ACCELERATION          500
  #define DEFAULT_RETRACT_ACCELERATION  500
  #define DEFAULT_TRAVEL_ACCELERATION   1000

  #define CLASSIC_JERK
  #if ENABLED(CLASSIC_JERK)
    #define DEFAULT_XJERK 10.0
    #define DEFAULT_YJERK 10.0
    #define DEFAULT_ZJERK  0.3
  #endif

  #define DEFAULT_EJERK    5.0

  #define SHOW_BOOTSCREEN

  #define EXTRUDERS 1

  #define X_BED_SIZE 220
  #define Y_BED_SIZE 220
  #define Z_MAX_POS 250
  #define X_MIN_POS 0
  #define Y_MIN_POS 0

  #define USE_XMIN_PLUG
  #define USE_YMIN_PLUG
  #define USE_ZMIN_PLUG

  #define X_HOME_DIR -1
  #define Y_HOME_DIR -1
  #define Z_HOME_DIR -1

  #if DISABLED(V6_HOTEND) && DISABLED(TH3D_HOTEND_THERMISTOR) && DISABLED(KNOWN_HOTEND_THERMISTOR)
    #define TEMP_SENSOR_0 1
  #else
    #if ENABLED(EZBOARD_PT100)
      #define TEMP_SENSOR_0 20
    #elif ENABLED(V6_HOTEND)
      #define TEMP_SENSOR_0 5
    #elif ENABLED(KNOWN_HOTEND_THERMISTOR)
      #define TEMP_SENSOR_0 KNOWN_HOTEND_THERMISTOR_VALUE
    #elif ENABLED(TH3D_HOTEND_THERMISTOR)
      #define TEMP_SENSOR_0 1
    #endif
  #endif
  
  #define TEMP_SENSOR_1 0 
  #define TEMP_SENSOR_2 0
  #define TEMP_SENSOR_3 0
  #define TEMP_SENSOR_4 0
  #define TEMP_SENSOR_5 0
  #define TEMP_SENSOR_6 0
  #define TEMP_SENSOR_7 0
  
  #if DISABLED(TH3D_BED_THERMISTOR) && DISABLED(KEENOVO_TEMPSENSOR) && DISABLED(KNOWN_BED_THERMISTOR) && DISABLED(AC_BED)
    #define TEMP_SENSOR_BED 1
  #else
    #if ENABLED(AC_BED)
      #define TEMP_SENSOR_BED 0
    #elif ENABLED(KNOWN_BED_THERMISTOR)
      #define TEMP_SENSOR_BED KNOWN_BED_THERMISTOR_VALUE
    #elif ENABLED(TH3D_BED_THERMISTOR)
      #define TEMP_SENSOR_BED 1
    #elif ENABLED(KEENOVO_TEMPSENSOR)
      #define TEMP_SENSOR_BED 11
    #endif
  #endif
  
  #define TEMP_SENSOR_PROBE 0
  #define TEMP_SENSOR_CHAMBER 0

  #define DEFAULT_Kp 28.72
  #define DEFAULT_Ki 2.62
  #define DEFAULT_Kd 78.81
  
  #define DEFAULT_bedKp 462.10
  #define DEFAULT_bedKi 85.47
  #define DEFAULT_bedKd 624.59

  #define ENDSTOPPULLUPS

  #define X_MIN_ENDSTOP_INVERTING false
  #define Y_MIN_ENDSTOP_INVERTING false
  #define Z_MIN_ENDSTOP_INVERTING false
  #define X_MAX_ENDSTOP_INVERTING false
  #define Y_MAX_ENDSTOP_INVERTING false
  #define Z_MAX_ENDSTOP_INVERTING false
  #define Z_MIN_PROBE_ENDSTOP_INVERTING false
  #define Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN

  #if ENABLED(ENDER3_V4_BOARD)
    #define X_DRIVER_TYPE TMC2208_STANDALONE
    #define Y_DRIVER_TYPE TMC2208_STANDALONE
    #define Z_DRIVER_TYPE TMC2208_STANDALONE
    #define E0_DRIVER_TYPE TMC2208_STANDALONE
  #else
    #define X_DRIVER_TYPE A4988
    #define Y_DRIVER_TYPE A4988
    #define Z_DRIVER_TYPE A4988
    #define E0_DRIVER_TYPE A4988
  #endif
  
  #define ENDSTOP_INTERRUPTS_FEATURE

  #define X_ENABLE_ON 0
  #define Y_ENABLE_ON 0
  #define Z_ENABLE_ON 0
  #define E_ENABLE_ON 0

  #define INVERT_X_DIR false
  #define INVERT_Y_DIR false
  #define INVERT_Z_DIR true

  #if ENABLED(REVERSE_E_MOTOR_DIRECTION)
    #define INVERT_E0_DIR true
  #else
    #define INVERT_E0_DIR false
  #endif
  
  #define INVERT_E1_DIR false
  #define INVERT_E2_DIR false
  #define INVERT_E3_DIR false
  #define INVERT_E4_DIR false
  #define INVERT_E5_DIR false
  #define INVERT_E6_DIR false
  #define INVERT_E7_DIR false

  #define ENCODER_PULSES_PER_STEP 4
  #define ENCODER_STEPS_PER_MENU_ITEM 1

  #define Z_PROBE_OFFSET_RANGE_MIN -10
  #define Z_PROBE_OFFSET_RANGE_MAX 10

  #if ENABLED(ENDER3_V2_OEM)
    #define ABL_ENABLE
    #define NOZZLE_TO_PROBE_OFFSET { -48, -15, 0 }
  #endif
#endif
// End Ender 3 Settings

//Ender 3 V2 Settings
#if ENABLED(ENDER3_V2)
  #define SERIAL_PORT 1
  #define SERIAL_PORT_2 3

  #define BAUDRATE 115200

  #ifndef MOTHERBOARD
    #define MOTHERBOARD BOARD_CREALITY_V4
  #endif

  #if ENABLED(CUSTOM_ESTEPS)
    #define DEFAULT_AXIS_STEPS_PER_UNIT   { 80, 80, 400, CUSTOM_ESTEPS_VALUE }
  #else
    #define DEFAULT_AXIS_STEPS_PER_UNIT   { 80, 80, 400, 95 }
  #endif
  #define DEFAULT_MAX_FEEDRATE          { 500, 500, 15, 25 }
  #define DEFAULT_MAX_ACCELERATION      { 500, 500, 100, 1000 }

  #define DEFAULT_ACCELERATION          500
  #define DEFAULT_RETRACT_ACCELERATION  500
  #define DEFAULT_TRAVEL_ACCELERATION   1000

  #define CLASSIC_JERK
  #if ENABLED(CLASSIC_JERK)
    #define DEFAULT_XJERK 10.0
    #define DEFAULT_YJERK 10.0
    #define DEFAULT_ZJERK  0.3
  #endif

  #define DEFAULT_EJERK    5.0

  #define SHOW_BOOTSCREEN

  #define EXTRUDERS 1

  #define X_BED_SIZE 220
  #define Y_BED_SIZE 220
  #define Z_MAX_POS 250
  #define X_MIN_POS 0
  #define Y_MIN_POS 0

  #define USE_XMIN_PLUG
  #define USE_YMIN_PLUG
  #define USE_ZMIN_PLUG

  #define X_HOME_DIR -1
  #define Y_HOME_DIR -1
  #define Z_HOME_DIR -1

  #if DISABLED(V6_HOTEND) && DISABLED(TH3D_HOTEND_THERMISTOR) && DISABLED(KNOWN_HOTEND_THERMISTOR)
    #define TEMP_SENSOR_0 1
  #else
    #if ENABLED(EZBOARD_PT100)
      #define TEMP_SENSOR_0 20
    #elif ENABLED(V6_HOTEND)
      #define TEMP_SENSOR_0 5
    #elif ENABLED(KNOWN_HOTEND_THERMISTOR)
      #define TEMP_SENSOR_0 KNOWN_HOTEND_THERMISTOR_VALUE
    #elif ENABLED(TH3D_HOTEND_THERMISTOR)
      #define TEMP_SENSOR_0 1
    #endif
  #endif
  
  #define TEMP_SENSOR_1 0 
  #define TEMP_SENSOR_2 0
  #define TEMP_SENSOR_3 0
  #define TEMP_SENSOR_4 0
  #define TEMP_SENSOR_5 0
  #define TEMP_SENSOR_6 0
  #define TEMP_SENSOR_7 0
  
  #if DISABLED(TH3D_BED_THERMISTOR) && DISABLED(KEENOVO_TEMPSENSOR) && DISABLED(KNOWN_BED_THERMISTOR) && DISABLED(AC_BED)
    #define TEMP_SENSOR_BED 1
  #else
    #if ENABLED(AC_BED)
      #define TEMP_SENSOR_BED 0
    #elif ENABLED(KNOWN_BED_THERMISTOR)
      #define TEMP_SENSOR_BED KNOWN_BED_THERMISTOR_VALUE
    #elif ENABLED(TH3D_BED_THERMISTOR)
      #define TEMP_SENSOR_BED 1
    #elif ENABLED(KEENOVO_TEMPSENSOR)
      #define TEMP_SENSOR_BED 11
    #endif
  #endif
  
  #define TEMP_SENSOR_PROBE 0
  #define TEMP_SENSOR_CHAMBER 0

  #define DEFAULT_Kp 28.72
  #define DEFAULT_Ki 2.62
  #define DEFAULT_Kd 78.81
  
  #define DEFAULT_bedKp 462.10
  #define DEFAULT_bedKi 85.47
  #define DEFAULT_bedKd 624.59

  #define ENDSTOPPULLUPS

  #define X_MIN_ENDSTOP_INVERTING false
  #define Y_MIN_ENDSTOP_INVERTING false
  #define Z_MIN_ENDSTOP_INVERTING false
  #define X_MAX_ENDSTOP_INVERTING false
  #define Y_MAX_ENDSTOP_INVERTING false
  #define Z_MAX_ENDSTOP_INVERTING false
  #define Z_MIN_PROBE_ENDSTOP_INVERTING false
  #define Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN

  #define X_DRIVER_TYPE TMC2208_STANDALONE
  #define Y_DRIVER_TYPE TMC2208_STANDALONE
  #define Z_DRIVER_TYPE TMC2208_STANDALONE
  #define E0_DRIVER_TYPE TMC2208_STANDALONE

  #define ENDSTOP_INTERRUPTS_FEATURE

  #define X_ENABLE_ON 0
  #define Y_ENABLE_ON 0
  #define Z_ENABLE_ON 0
  #define E_ENABLE_ON 0

  #define INVERT_X_DIR false
  #define INVERT_Y_DIR false
  #define INVERT_Z_DIR true

  #if ENABLED(REVERSE_E_MOTOR_DIRECTION)
    #define INVERT_E0_DIR true
  #else
    #define INVERT_E0_DIR false
  #endif
  
  #define INVERT_E1_DIR false
  #define INVERT_E2_DIR false
  #define INVERT_E3_DIR false
  #define INVERT_E4_DIR false
  #define INVERT_E5_DIR false
  #define INVERT_E6_DIR false
  #define INVERT_E7_DIR false

  #define DWIN_CREALITY_LCD

  #define ENCODER_PULSES_PER_STEP 4
  #define ENCODER_STEPS_PER_MENU_ITEM 1

  #define Z_PROBE_OFFSET_RANGE_MIN -10
  #define Z_PROBE_OFFSET_RANGE_MAX 10

  #define POWER_LOSS_RECOVERY

  #if ENABLED(ENDER3_OEM)
    #define NOZZLE_TO_PROBE_OFFSET { -44, -10, 0 }
    #define ABL_ENABLE
  #endif
  
#endif
// End Ender 3 V2 Settings

/**
 * Temp Settings
 */

#define DUMMY_THERMISTOR_998_VALUE 25
#define DUMMY_THERMISTOR_999_VALUE 100
#define MAX_REDUNDANT_TEMP_SENSOR_DIFF 10

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

#define BED_MAXTEMP      120

#define TEMP_RESIDENCY_TIME     10
#define TEMP_WINDOW              1
#define TEMP_HYSTERESIS          3

#define TEMP_BED_RESIDENCY_TIME 10
#define TEMP_BED_WINDOW          1
#define TEMP_BED_HYSTERESIS      3

// PID Settings
#define PIDTEMP
#define BANG_MAX 255
#define PID_MAX BANG_MAX
#define PID_K1 0.95

#ifndef DEFAULT_Kp
  #define DEFAULT_Kp 22.2 // Define Marlin default PID if no machine specific PID is defined.
  #define DEFAULT_Ki 1.08
  #define DEFAULT_Kd 114
#endif

#define PIDTEMPBED
#define MAX_BED_POWER 255
#if ENABLED(PIDTEMPBED)
  #ifndef DEFAULT_bedKp
    #define DEFAULT_bedKp 10.00 // Define Marlin default bed PID if no machine specific PID is defined.
    #define DEFAULT_bedKi .023
    #define DEFAULT_bedKd 305.4
  #endif
#endif

#if EITHER(PIDTEMP, PIDTEMPBED)
  #define PID_FUNCTIONAL_RANGE 10
#endif
// End PID Settings

// Misc User Protections
#define PREVENT_COLD_EXTRUSION // Enable for release. If you want to disable cold extrusion prevention, disable or remove this line.
#define EXTRUDE_MINTEMP 170

#define PREVENT_LENGTHY_EXTRUDE // Enable for release. If you want to disable lengthy extrude prevention, disable or remove this line.
#define EXTRUDE_MAXLENGTH 1000

// Enable Thermal Protection for Bed and Hotends
#define THERMAL_PROTECTION_HOTENDS
#define THERMAL_PROTECTION_BED

/**
 * ABL Probe Settings
 */

#if ENABLED(CUSTOM_PROBE)
  #define ABL_ENABLE
#endif

#if ENABLED(ABL_ENABLE)
  //#define ENABLE_LEVELING_FADE_HEIGHT // Disable for release. Can cause issues.
  #define SEGMENT_LEVELED_MOVES
  #define LEVELED_SEGMENT_LENGTH 5.0
  
  #undef Z_PROBE_OFFSET_RANGE_MIN //Some machines define these even with no probe. Override when a probe is used to ensure correct settings.
  #define Z_PROBE_OFFSET_RANGE_MIN    -5
  #undef Z_PROBE_OFFSET_RANGE_MAX //Some machines define these even with no probe. Override when a probe is used to ensure correct settings.
  #define Z_PROBE_OFFSET_RANGE_MAX     1
  
  #define Z_MIN_PROBE_REPEATABILITY_TEST
  #define Z_AFTER_PROBING              5
  #define Z_PROBE_LOW_POINT           -2
  #if DISABLED(BLTOUCH)
    #define FIX_MOUNTED_PROBE
  #endif
  #if ENABLED(SLOWER_PROBE_MOVES)
    #define XY_PROBE_SPEED 8000
  #else
    #if ENABLED(EZABL_SUPERFASTPROBE)
      #define XY_PROBE_SPEED 16000
    #else
      #define XY_PROBE_SPEED 12000
    #endif
  #endif
  #define PROBING_MARGIN EZABL_PROBE_EDGE
  #if ENABLED(FIX_MOUNTED_PROBE) && DISABLED(HEATERS_ON_DURING_PROBING)
    #define PROBING_HEATERS_OFF   
  #endif
  #define MULTIPLE_PROBING 2
  #define AUTO_BED_LEVELING_BILINEAR
  #define EXTRAPOLATE_BEYOND_GRID
  #define GRID_MAX_POINTS_X EZABL_POINTS
  #define GRID_MAX_POINTS_Y GRID_MAX_POINTS_X
  #define Z_SAFE_HOMING
  #if ENABLED(Z_SAFE_HOMING)
    #define Z_SAFE_HOMING_X_POINT ((X_BED_SIZE) / 2)    // X point for Z homing
    #define Z_SAFE_HOMING_Y_POINT ((Y_BED_SIZE) / 2)    // Y point for Z homing
  #endif
  
  // ABL Probe Logic Settings
  #if ENABLED(BLTOUCH) // BLTouch uses false
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
 * General Firmware Settings
 */

#define STRING_CONFIG_H_AUTHOR "TH3D Studio"
#define UNIFIED_VERSION "TH3D UF2.00"
#if ENABLED(USER_PRINTER_NAME)
  #define CUSTOM_MACHINE_NAME USER_PRINTER_NAME
#else
  #define CUSTOM_MACHINE_NAME "TH3D UFW2"
#endif
#define S_CURVE_ACCELERATION
#define DEFAULT_NOMINAL_FILAMENT_DIA 1.75
#define HOMING_FEEDRATE_XY (50*60)
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

#if ENABLED(EZABL_SUPERFASTPROBE) && ENABLED(ABL_ENABLE) && DISABLED(BLTOUCH)
  #define HOMING_FEEDRATE_Z  (15*60)
#elif ENABLED(EZABL_FASTPROBE) && ENABLED(ABL_ENABLE) && DISABLED(BLTOUCH)
  #define HOMING_FEEDRATE_Z  (8*60)
#else
  #define HOMING_FEEDRATE_Z  (4*60)
#endif

#define Z_PROBE_SPEED_FAST HOMING_FEEDRATE_Z
#define Z_PROBE_SPEED_SLOW (Z_PROBE_SPEED_FAST / 2)

#if ENABLED(BLTOUCH)
  #define Z_CLEARANCE_DEPLOY_PROBE   12
  #define Z_CLEARANCE_BETWEEN_PROBES 8
  #define Z_CLEARANCE_MULTI_PROBE    8
  #define ENDSTOPPULLUP_ZMIN
  #define ENDSTOPPULLUP_ZMIN_PROBE
#elif ENABLED(EZABL_SUPERFASTPROBE) && ENABLED(ABL_ENABLE) && DISABLED(BLTOUCH)
  #define Z_CLEARANCE_DEPLOY_PROBE   1
  #define Z_CLEARANCE_BETWEEN_PROBES 2
  #define Z_CLEARANCE_MULTI_PROBE    1
#else
  #define Z_CLEARANCE_DEPLOY_PROBE   5
  #define Z_CLEARANCE_BETWEEN_PROBES 3
  #define Z_CLEARANCE_MULTI_PROBE    3
#endif

#define HOST_KEEPALIVE_FEATURE
#define DEFAULT_KEEPALIVE_INTERVAL 2
#if DISABLED(SPACE_SAVER)
  #define BUSY_WHILE_HEATING
#endif

#define EEPROM_SETTINGS
#if ENABLED(SPACE_SAVER)
  #define DISABLE_M503
#endif
#define EEPROM_CHITCHAT
#define EEPROM_BOOT_SILENT
#if DISABLED(SPACE_SAVER)
  #define EEPROM_AUTO_INIT
#endif

#define MIN_SOFTWARE_ENDSTOPS
#if ENABLED(MIN_SOFTWARE_ENDSTOPS)
  #define MIN_SOFTWARE_ENDSTOP_X
  #define MIN_SOFTWARE_ENDSTOP_Y
  #if DISABLED(ABL_ENABLE)
    #define MIN_SOFTWARE_ENDSTOP_Z // Disable when any ABL is being used.
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

/**
 * LCD General Settings
 */

#define LCD_LANGUAGE en
#define DISPLAY_CHARSET_HD44780 JAPANESE
#define LCD_INFO_SCREEN_STYLE 0
#define DISABLE_REDUCED_ACCURACY_WARNING
#define LCD_FEEDBACK_FREQUENCY_DURATION_MS 2
#define LCD_FEEDBACK_FREQUENCY_HZ 5000

#if DISABLED(DWIN_CREALITY_LCD) && DISABLED(SPACE_SAVER)
  #define INDIVIDUAL_AXIS_HOMING_MENU
#endif

#if DISABLED(SPACE_SAVER)
  #define LEVEL_BED_CORNERS
  #if ENABLED(LEVEL_BED_CORNERS)
    #define LEVEL_CORNERS_INSET_LFRB { 30, 30, 30, 30 }
    #define LEVEL_CORNERS_HEIGHT      0.0
    #define LEVEL_CORNERS_Z_HOP       4.0
    #define LEVEL_CENTER_TOO
  #endif
#endif