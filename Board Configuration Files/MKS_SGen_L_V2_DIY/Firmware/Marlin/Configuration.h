/**
 * For directions on how to use this firmware visit http://uf2.th3dstudio.com and click on your printer/board link
 * NO IMPLIED SUPPORT OR WARRANTY IS PROVIDED WITH THIS FIRMWARE AND IS PROVIDED AS-IS
 * 
 * THIS FIRMWARE VERSION IS FOR DIY USE ONLY BY ADVANCED USERS AND IS NOT COVERED UNDER INCLUDED TECHNICAL SUPPORT
 */
#pragma once
#define CONFIGURATION_H_VERSION 02000901

//#@CONFIGURATION_START_FLAG

//===========================================================================
//============================ TH3D Configuration ===========================
//===========================================================================

// UNCOMMENT MEANS REMOVING THE // IN FRONT OF A #define XXXXXX LINE.

//===========================================================================
// ******************   MKS SGEN L V2 BOARD DIY CONFIG   ********************
//===========================================================================

/**
 * !!!Important Information!!!
 * This firmware is being put out as a courtesy for people who want to use the MKS SGen L V2 in their custom printer.
 * Setup and usage of this firmware is *not* covered under the technical support included with the MKS SGen L V2.
 * Using the MKS SGen L V2 in a custom printer is an advanced setup for users who know what they are doing.
 * 
 * Stepper Drivers - This firmware is setup to work with the MKS TMC2209 V2 or Watterott TMC2209 drivers in UART Mode.
 * Jumper Config
 * O O O  M0
 * O O O  M1
 * O=O O  M2
 * O O  
 * 
 * Hotends - This firmware currently only supports using one hotend and extruder at this time.
 * 
 * Z Homing Direction - This firmware only supports homing Z to the MIN position (not MAX) at this time.
 */

/**
 * LCD Selection
 * Select the LCD you are using and if you want to use the SD slot on the board
 * or the SD slot on your LCD (if it has one)
 */

// RepRap 12864 Type LCD/TH3D 12864 LCD
//#define REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER

// MKS Mini 12864 LCD
//#define MKS_MINI_12864

// Single Cable Creality 12864 LCD - Put in EXP1 and rotate 180 Degrees
//#define CR10_STOCKDISPLAY

// SD Slot Selection - LCD for LCD Slot, ONBOARD for slot on MKS board - Only select one
//#define SDCARD_CONNECTION LCD
#define SDCARD_CONNECTION ONBOARD

/**
 * Build Volume Settings
 * Set your printer build area sizes below
 * 
 * X_BED_SIZE - The size in MM of your X Axis printable area
 * Y_BED_SIZE - The size in MM of your Y Axis printable area
 * Z_MAX_POS - The tallest your machine can print without hitting the frame
 */

#define X_BED_SIZE 300
#define Y_BED_SIZE 300
#define Z_MAX_POS 400

/**
 * Motor Direction Settings
 * Set the motor direction below. Changing a false to a true will change the motor direction
 * Flash the firmware, put your print head and Z to the middle and tell each to move +10mm
 * If an axis moves the wrong direction then change the false to a true below and flash the board
 * 
 * INVERT_X_DIR - Changing from false to true changes the direction of the X axis
 * INVERT_Y_DIR - Changing from false to true changes the direction of the Y axis
 * INVERT_Z_DIR - Changing from false to true changes the direction of the Z axis
 * INVERT_E0_DIR - Changing from false to true changes the direction of the 1st Extruder motor <- Hotend must be over 170C for the motor to move
 * INVERT_E1_DIR - Changing from false to true changes the direction of the 2nd Extruder motor <- Hotend must be over 170C for the motor to move
 */

#define INVERT_X_DIR false
#define INVERT_Y_DIR false
#define INVERT_Z_DIR false
#define INVERT_E0_DIR false
#define INVERT_E1_DIR false

/**
 * Endstop Logic Settings
 * Test your endstops with the M119 command by sending it to the printer over USB
 * When a switch is pressed it should show "TRIGGERED" and when not pressed it will show "open".
 * If a switch is showing the opposite of what it should then change the false to a true below for that switch
 * 
 * ALL SETTINGS - Changing from false to true changes when a switch shows "TRIGGERED" based on its signal (NC vs NO)
 */

#define X_ENDSTOP_LOGIC false
#define Y_ENDSTOP_LOGIC false
#define Z_ENDSTOP_LOGIC false

/**
 * XY Homing Direction Settings
 * Most printers home to the "MIN" position on the XY. 
 * If your machine homes to "MAX" then change the -1 to a 1 for the axis that needs to home to the "MAX" position
 * 
 * A value of -1 homes to "MIN"
 * A value of 1 homes to "MAX"
 */

#define X_HOME_DIR -1
#define Y_HOME_DIR -1

/**
 * Motion Settings
 * Below are settings for steps per mm, acceleration, and jerk.
 * Make sure when setting the acceleration values that the DEFAULT_ACCELERATION is not higher than the MAX_X/Y_ACCEL
 */

// Steps per MM -------------------------------------------------------------
#define X_STEPS_MM  80
#define Y_STEPS_MM  80
#define Z_STEPS_MM  400
#define E0_STEPS_MM 95

// Acceleration Settings ----------------------------------------------------
#define DEFAULT_ACCELERATION          500
#define DEFAULT_RETRACT_ACCELERATION  500
#define DEFAULT_TRAVEL_ACCELERATION   1000
#define MAX_X_ACCEL 1000
#define MAX_Y_ACCEL 1000

// Jerk Settings ------------------------------------------------------------
#define DEFAULT_XJERK 5.0
#define DEFAULT_YJERK 5.0
#define DEFAULT_ZJERK 0.3
#define DEFAULT_EJERK 5.0

/**
 * Thermistor Settings
 * 
 * This firmware defaults to Marlin thermistor #1 if no other option is set below for the hotend and bed.
 * Most printers use Marlin thermistor #1 for their stock thermistor setting
 */

// Hotend Thermistor Settings -----------------------------------------------

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

// Bed Thermistor Settings --------------------------------------------------

// If you are using a thermistor TH3D sells for your bed uncomment the below line.
//#define TH3D_BED_THERMISTOR

// If you are using a Keenovo with SSR and the Keenovo temperature sensor uncomment the below line.
//#define KEENOVO_TEMPSENSOR

// If you are using a known bed thermistor value uncomment the below 2 lines and enter the thermistor number replacing the X after the #define KNOWN_BED_THERMISTOR_VALUE
//#define KNOWN_BED_THERMISTOR
//#define KNOWN_BED_THERMISTOR_VALUE X

// If you are using an AC bed with a standalone controller (Keenovo) uncomment the below line to disable the heated bed in the firmware
//#define AC_BED

/**
 * Other Settings
 */

// Baud Rate - Valid settings are 115200 and 250000 -------------------------
#define BAUDRATE 250000

// Printer Voltage - Uncomment your machine voltage -------------------------
//#define PRINTER_VOLTAGE_12
//#define PRINTER_VOLTAGE_24

// Motor Currents - Raise current if steps are being lost -------------------
#define X_MOTOR_CURRENT 600
#define Y_MOTOR_CURRENT 600
#define Z_MOTOR_CURRENT 800
#define E0_MOTOR_CURRENT 800
#define E1_MOTOR_CURRENT 800

// Motor Mode - If you want more torque on an axis change to spreadcycle ----
//#define XY_SPREADCYCLE
//#define Z_SPREADCYCLE

// Filament Sensor Options --------------------------------------------------
// If you are using our EZOut Sensor connect to the X+ header with the RED wire lined up with the "5V" marking by the header and uncomment the below line.
// If you have 2, uncomment the EZOUTV2_DUAL_ENABLE instead of EZOUTV2_ENABLE. The 2nd sensor connects like the 1st, but to the Y+ header.
//#define EZOUTV2_ENABLE
//#define EZOUTV2_DUAL_ENABLE

// If you are using the Creality CR-10S Sensor connect to the FIL SENSOR header with the RED wire lined up with the "5V" marking by the header and uncomment the below line.
//#define CR10S_STOCKFILAMENTSENSOR

// Sample EZABL Probe Mounts ------------------------------------------------
//#define CR10_OEM                 //OEM Mount for Creality Machines (Ender3/Ender5/CR-10/CR-10S/CR-20)
//#define ENDER2_OEM               //Ender 2 Specific OEM Mount
//#define ENDER2_V6                //Ender 2 Specific V6 Mount
//#define SV01_OEM_MOUNT           //Sovol SV01 OEM Mount
//#define CR10_VOLCANO             //TH3D CR-10 Volcano Mount 
//#define CR10_V6HEAVYDUTY         //V6 Heavy Duty Mount
//#define TM3DAERO                 //TM3D Aero Mount for V6
//#define TM3DAERO_EXTENDED        //TM3D Arto Mount for Volcano
//#define PETSFANG                 //This is the RIGHT mounted version
//#define CUSTOM_PROBE             //For any other probe mount (also used for BL Touch), Enter offsets below

// PID Settings -------------------------------------------------------------
// If you want to put your PID settings in at the firmware level uncomment the 3 lines for each PID value setting

// Hotend PID
//#define DEFAULT_Kp 22.2
//#define DEFAULT_Ki 1.08
//#define DEFAULT_Kd 114

// Bed PID - To use PID on enable ENABLE_PIDBED below
//#define DEFAULT_bedKp 10.00
//#define DEFAULT_bedKi .023
//#define DEFAULT_bedKd 305.4

// CoreXY Printer Setting ---------------------------------------------------
// If you are using the board in a CoreXY printer, uncomment the below line and make sure you have your motors connected and mounted to the correct locations
//#define COREXY

// Extra Fan Outputs --------------------------------------------------------
// If you want to use the 2nd hotend output (HE1) for your controller fan, uncomment the below line
// This fan will turn on based on if your stepper drivers are enabled or not - not available if using 2 hotends
#define MKS_SGENL_V2_HE1_FAN

// If you want to use the FAN2 output for your hotend fan, uncomment the below line.
// This fan will turn on when your hotend temperature is at 50C or higher
#define MKS_SGENL_V2_FAN2

// Dual Extrusion Settings --------------------------------------------------
// Set your 2nd E Motor steps and uncommend the below line
//#define E1_STEPS_MM 95

// For Cyclops or any "multi-extruder" that shares a single nozzle.
//#define SINGLENOZZLE

// 2nd Hotend Nozzle Offset
// The offset has to be X=0, Y=0 for the extruder 0 hotend (default extruder).
// For the other hotend it is the distance from the extruder 0 hotend.
//#define HOTEND_OFFSET_X { 0.0, 20.00 } // (mm) relative X-offset for each nozzle
//#define HOTEND_OFFSET_Y { 0.0, 5.00 }  // (mm) relative Y-offset for each nozzle

// EZNeo Settings -----------------------------------------------------------
// If you are using an EZNeo strip on your printer, uncomment the line for what strip you are using.
// Specify your IO pin below as well as this board does not have a dedicated NEOPIXEL header on it.
//#define EZNEO_220

// EZNeo Manual IO Pin Setting ----------------------------------------------
// If you have the EZNeo wired with your own 5V power provided, specify the pin used below.
//#define NEOPIXEL_PIN P1_18

//===========================================================================
// *************************  END PRINTER SECTION   *************************
//===========================================================================

//===========================================================================
// EZABL Advanced Settings - EZABL_POINTS & EZABL_PROBE_EDGE are also used for other probes
//===========================================================================

// Probing Grid Points - If you want more or less EZABL probe points change the number below, use odd numbers. Total points is # times #.
#define EZABL_POINTS 3

// Probe Edge - How far from the edge of the bed to probe from. Use 50 if using binder clips. This also sets the edge inset value for MANUAL_MESH_LEVELING.
#define EZABL_PROBE_EDGE 35

// Fast Probing - Works with most machines and all EZABL sensors (8mm/s)
#define EZABL_FASTPROBE

// Superfast Probing - Only works with the EZABL Pro Sensors (15mm/s)
//#define EZABL_SUPERFASTPROBE

// Heaters on During Probing - Heaters will stay on during probing - May reduce accuracy do not use unless told to by support
//#define HEATERS_ON_DURING_PROBING

// Probing Steppers Off - This will cycle the XYE stepper motors during probing to reduce interference from them. When using this do NOT touch the X or Y during probing as they will not be locked.
//#define PROBING_STEPPERS_OFF

// Slow Down Moves - Does your machine make weird noises/vibrations when it is probing the mesh? Enable this to slow down the speed between probe points.
//#define SLOWER_PROBE_MOVES

// Grid Extrapolation - This will use the mesh data to make assumptions of the bed outside the probe area. Disable if you are getting incorrect results on the edges of the bed.
#define EXTRAPOLATE_BEYOND_GRID

//================================================================================
// CUSTOM PROBE SETTINGS - FOR EZABL OR BL TOUCH
// If you have a probe mount that is not pre-setup in the firmware then uncomment
// the CUSTOM_PROBE line above and enter your probe offsets below
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
  *   L |    (+) P  | R <-- probe (10,10)
  *   E |           | I
  *   F | (-) N (+) | G <-- nozzle (0,0)
  *   T |           | H
  *     |    (-)    | T
  *     |           |
  *     O-- FRONT --+
  *   (0,0)
  *
  * Specify a Probe position as { X, Y, Z }
  * Do NOT enter an number for the Z position in here. Store your offset in EEPROM.
  *
  * When is the offset POSITIVE?
  * If the probe is right of the nozzle the offset on X is POSITIVE
  * If the probe is behind of the nozzle the offset on Y is POSITIVE
  * 
  * When is the offset NEGATIVE?
  * If the probe is left of the nozzle the offset on X is NEGATIVE
  * If the probe is in front of the nozzle the offset on Y is NEGATIVE
  */
  #define NOZZLE_TO_PROBE_OFFSET { 10, 10, 0 }
#endif


//===========================================================================
//******************** EXTRA FEATURES AND TWEAKS ****************************
//===========================================================================

// FILAMENT SENSOR UNLOAD SETTINGS -----------------
// If you have a filament sensor that is physically mounted to the machine you can enable MOUNTED_FILAMENT_SENSOR to set the unload length to 0 to prevent filament from backing up in the sensor by uncommenting MOUNTED_FILAMENT_SENSOR 
//#define MOUNTED_FILAMENT_SENSOR

// If you have a direct drive machine with a filament sensor uncomment DIRECT_DRIVE_PRINTER to decrease the unload length from 100mm to 20mm
//#define DIRECT_DRIVE_PRINTER

// MISC --------------------------------------------

// LCD Knob Direction
// Turning your LCD knob clockwise should move DOWN in the menus/make values increase and counter-clockwise should move UP in the menus/make values decrease
// If yours is behaving opposite then enable the REVERSE_KNOB_DIRECTION option below
//#define REVERSE_KNOB_DIRECTION

// If you have a 5015 fan that whines when under 100% speed uncomment the below line.
//#define FAN_FIX

// Use your own printer name - Uncomment both lines
//#define CUSTOM_PRINTER_NAME
//#define USER_PRINTER_NAME "CHANGE ME"

// If your printer is homing to the endstops hard uncomment this to change the homing speed/divisor to make it less aggressive.
//#define SLOWER_HOMING

//===========================================================================
//****************** COMMUNITY REQUESTED FEATURES ***************************
//===========================================================================

// HOME OFFSET ADJUSTMENT --------------------------
// If you need to adjust your XY home offsets from defaults then you can uncomment the HOME_ADJUST line below and enter your
// custom XY offsets. This is provided for convenience and is unsupported with included product support.
// How to use - measure (home XY then jog using the LCD 1mm at a time) the X and Y distance the nozzle is off
// the build plate and then put those as NEGATIVE values below, positive values will NOT work (move your endstops to fix a positve offset).
//#define HOME_ADJUST
#define X_HOME_LOCATION -10
#define Y_HOME_LOCATION -10

// PID BED TEMPERATURE CONTROL ---------------------
// If you want PID Bed Temperature control enable the below line. You will need to tune it for your machine.
// See the PID Bed setup guide here: https://support.th3dstudio.com/hc/guides/diy-guides/p-i-d-bed-calibration-guide/
//#define ENABLE_PIDBED

// Z PROBE OFFSET WIZARD ---------------------------
// Marlin has a Z Probe Offset Wizard now. If you want to enable this, uncomment the below line.
//#define PROBE_OFFSET_WIZARD

// FINE BABYSTEPPING -------------------------------
// Enabling the below line will set the babystep resolution from 0.025mm to 0.010mm for finer control.
//#define FINE_BABYSTEPPING

// LINEAR ADVANCE ----------------------------------
// See here on how to use Linear Advance: http://marlinfw.org/docs/features/lin_advance.html
//#define LINEAR_ADVANCE
// Change the K Value here or use M900 KX.XX in your starting code (recommended).
#define LINEAR_ADVANCE_K 0

// BL TOUCH ----------------------------------------
// If you want to use the BL-Touch uncomment the 2 lines below and refer to the installation guide for the EZBoard to connect the probe.
// You also need to uncomment #define CUSTOM_PROBE above and then enter in your offsets above in the CUSTOM PROBE section.
//#define BLTOUCH
// Here is where you set your servo pin. For EZBoard use P2_04
//#define SERVO0_PIN P2_04

// MANUAL MESH LEVELING ----------------------------
// If you want to use manual mesh leveling you can enable the below option. This is for generating a MANUAL mesh WITHOUT a probe. To change the mesh inset value change the EZABL_PROBE_EDGE setting above.
// Mesh Bed Leveling Documentation: http://marlinfw.org/docs/gcode/G029-mbl.html 
// NOTE: If you want to automate the leveling process our EZABL kits do this for you. Check them out here: http://EZABL.TH3DStudio.com
//#define MANUAL_MESH_LEVELING

// POWER LOSS RECOVERY -----------------------------
// Continue after Power-Loss feature will store the current state to the SD Card at the start of each layer
// during SD printing. If this is found at bootup it will ask you if you want to resume the print.
//
// NOTE: This feature causes excessive wear on your SD card.
//#define POWER_LOSS_RECOVERY

//===========================================================================
// **********************  END CONFIGURATION SETTINGS   *********************
//===========================================================================

//#@CONFIGURATION_END_FLAG

/**
 * ****************************DO NOT TOUCH ANYTHING BELOW THIS COMMENT**************************
 * Core machine settings are below. Do NOT modify these unless you understand what you are doing.
 */

/**
 * Machine Configuration Settings
 */

/**
 * DIY Config Sanity Checks
 */

#if DEFAULT_ACCELERATION > MAX_X_ACCEL
  #error "DEFAULT_ACCELERATION cannot be higher than MAX_X_ACCEL. Set to the same number or lower."
#endif

#if DEFAULT_ACCELERATION > MAX_Y_ACCEL
  #error "DEFAULT_ACCELERATION cannot be higher than MAX_Y_ACCEL. Set to the same number or lower."
#endif

#if DISABLED(PRINTER_VOLTAGE_12) && DISABLED(PRINTER_VOLTAGE_24)
  #error "Uncomment the PRINTER_VOLTAGE_## line for the DC (Power supply output) voltage that your printer runs on."
#endif

#if BAUDRATE == 115200
  //Do Nothing
#elif BAUDRATE == 250000
  //Do Nothing
#else
  #error "Printer BAUDRATE is not correct. Valid settings are 115200 and 250000. Change to a valid setting (115200 is recommended)"
#endif

//MKS SGEN L V2 DIY based Machine Settings
#define SERIAL_PORT -1
#define SERIAL_PORT_2 0

#ifndef MOTHERBOARD
  #define MOTHERBOARD BOARD_MKS_SGEN_L_V2
#endif

#define DIY_TMCBOARD
  
#ifdef E1_STEPS_MM
  #define Z_PROBE_LOW_POINT -10
  #define DISTINCT_E_FACTORS
  #define EXTRUDERS 2
#else
  #define EXTRUDERS 1
#endif
  
#ifdef E1_STEPS_MM
  #define DEFAULT_AXIS_STEPS_PER_UNIT   { X_STEPS_MM, Y_STEPS_MM, Z_STEPS_MM, E0_STEPS_MM, E1_STEPS_MM }
#else
  #define DEFAULT_AXIS_STEPS_PER_UNIT   { X_STEPS_MM, Y_STEPS_MM, Z_STEPS_MM, E0_STEPS_MM }
#endif
  
#define DEFAULT_MAX_FEEDRATE          { 200, 200, 15, 50 }
#define DEFAULT_MAX_ACCELERATION      { MAX_X_ACCEL, MAX_Y_ACCEL, 500, 5000 }

#define CLASSIC_JERK

#define SHOW_BOOTSCREEN
  
#if ENABLED(HOME_ADJUST)
  #define X_MIN_POS X_HOME_LOCATION
  #define Y_MIN_POS Y_HOME_LOCATION
#else
  #define X_MIN_POS 0
  #define Y_MIN_POS 0
#endif
  
#define USE_ZMIN_PLUG
 
#if NONE(V6_HOTEND, TH3D_HOTEND_THERMISTOR, KNOWN_HOTEND_THERMISTOR, EZBOARD_PT100)
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
  
#ifdef E1_STEPS_MM
  #if DISABLED(SINGLENOZZLE)
    #define TEMP_SENSOR_1 TEMP_SENSOR_0
  #else
    #define TEMP_SENSOR_1 0
  #endif
#else
  #define TEMP_SENSOR_1 0 
#endif
#define TEMP_SENSOR_2 0
#define TEMP_SENSOR_3 0
#define TEMP_SENSOR_4 0
#define TEMP_SENSOR_5 0
#define TEMP_SENSOR_6 0
#define TEMP_SENSOR_7 0
  
#if NONE(TH3D_BED_THERMISTOR, KEENOVO_TEMPSENSOR, KNOWN_BED_THERMISTOR, AC_BED)
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

#define X_MIN_ENDSTOP_INVERTING X_ENDSTOP_LOGIC
#define Y_MIN_ENDSTOP_INVERTING Y_ENDSTOP_LOGIC
#define Z_MIN_ENDSTOP_INVERTING Z_ENDSTOP_LOGIC
#define X_MAX_ENDSTOP_INVERTING X_ENDSTOP_LOGIC
#define Y_MAX_ENDSTOP_INVERTING Y_ENDSTOP_LOGIC
#define Z_MAX_ENDSTOP_INVERTING Z_ENDSTOP_LOGIC
#define Z_MIN_PROBE_ENDSTOP_INVERTING Z_ENDSTOP_LOGIC
#define Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN

#define X_DRIVER_TYPE  TMC2209
#define Y_DRIVER_TYPE  TMC2209
#define Z_DRIVER_TYPE  TMC2209
#define E0_DRIVER_TYPE TMC2209
#ifdef E1_STEPS_MM
  #define E1_DRIVER_TYPE TMC2209
#endif

#define X_ENABLE_ON 0
#define Y_ENABLE_ON 0
#define Z_ENABLE_ON 0
#define E_ENABLE_ON 0

#define INVERT_E2_DIR false
#define INVERT_E3_DIR false
#define INVERT_E4_DIR false
#define INVERT_E5_DIR false

#if X_HOME_DIR == 1
  #define USE_XMAX_PLUG
#else
  #define USE_XMIN_PLUG
#endif

#if Y_HOME_DIR == 1
  #define USE_YMAX_PLUG
#else
  #define USE_YMIN_PLUG
#endif

#define Z_HOME_DIR -1

#if ENABLED(REVERSE_KNOB_DIRECTION)
  #define REVERSE_ENCODER_DIRECTION
#endif

#define ENCODER_PULSES_PER_STEP 4
#define ENCODER_STEPS_PER_MENU_ITEM 1

#if ENABLED(EZOUTV2_DUAL_ENABLE)
  #define EZOUTV2_ENABLE
#endif

#if ENABLED(EZOUTV2_ENABLE) || ENABLED(CR10S_STOCKFILAMENTSENSOR)
  #define FILAMENT_RUNOUT_SENSOR
#endif

#if ENABLED(FILAMENT_RUNOUT_SENSOR)
  
  #define FIL_RUNOUT_ENABLED_DEFAULT true // Enable the sensor on startup. Override with M412 followed by M500.
  #if ENABLED(EZOUTV2_DUAL_ENABLE)
    #define NUM_RUNOUT_SENSORS   2          // Number of sensors, up to one per extruder. Define a FIL_RUNOUT#_PIN for each.
    #define FIL_RUNOUT_PIN P1_28
    #define FIL_RUNOUT2_PIN P1_26
  #else
    #define NUM_RUNOUT_SENSORS   1          // Number of sensors, up to one per extruder. Define a FIL_RUNOUT#_PIN for each.
    #define FIL_RUNOUT_PIN P1_28
  #endif
  
  #if ENABLED(EZOUTV2_ENABLE)
    #define FIL_RUNOUT_STATE LOW  // Pin state indicating that filament is NOT present.
  #else
    #define FIL_RUNOUT_STATE HIGH // Pin state indicating that filament is NOT present.
  #endif
  
  #define FIL_RUNOUT_PULLUP               // Use internal pullup for filament runout pins.
  //#define FIL_RUNOUT_PULLDOWN           // Use internal pulldown for filament runout pins.

  // Set one or more commands to execute on filament runout.
  // (After 'M412 H' Marlin will ask the host to handle the process.)
  #define FILAMENT_RUNOUT_SCRIPT "M600"

  // After a runout is detected, continue printing this length of filament
  // before executing the runout script. Useful for a sensor at the end of
  // a feed tube. Requires 4 bytes SRAM per sensor, plus 4 bytes overhead.
  //#define FILAMENT_RUNOUT_DISTANCE_MM 25

  #ifdef FILAMENT_RUNOUT_DISTANCE_MM
    // Enable this option to use an encoder disc that toggles the runout pin
    // as the filament moves. (Be sure to set FILAMENT_RUNOUT_DISTANCE_MM
    // large enough to avoid false positives.)
    //#define FILAMENT_MOTION_SENSOR
  #endif
#endif

#if ENABLED(EZNEO_220)
  #define RGB_LIGHTS
  #define NEOPIXEL_LED
  #if ENABLED(NEOPIXEL_LED)
    #define NEOPIXEL_TYPE   NEO_GRB // NEO_GRBW / NEO_GRB - four/three channel driver type (defined in Adafruit_NeoPixel.h)
    #define NEOPIXEL_PIXELS 15       // Number of LEDs in the strip. (Longest strip when NEOPIXEL2_SEPARATE is disabled.)
    #define NEOPIXEL_IS_SEQUENTIAL   // Sequential display for temperature change - LED by LED. Disable to change all LEDs at once.
    #define NEOPIXEL_BRIGHTNESS 127  // Initial brightness (0-255)
    #define NEOPIXEL_STARTUP_TEST  // Cycle through colors at startup
  #endif

  /**
   * Printer Event LEDs
   *
   * During printing, the LEDs will reflect the printer status:
   *
   *  - Gradually change from blue to violet as the heated bed gets to target temp
   *  - Gradually change from violet to red as the hotend gets to temperature
   *  - Change to white to illuminate work surface
   *  - Change to green once print has finished
   *  - Turn off after the print has finished and the user has pushed a button
   */
  #if ANY(BLINKM, RGB_LED, RGBW_LED, PCA9632, PCA9533, NEOPIXEL_LED)
    #define PRINTER_EVENT_LEDS
  #endif
#endif

//End MKS SGEN L V2 DIY Config

/*
 * All other settings are stored in the Configuration_backend.h file. Do not change unless you know what you are doing.
 */
 
#include "Configuration_backend.h"
