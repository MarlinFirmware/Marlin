/**
 * For directions on how to use this firmware visit http://uf2.th3dstudio.com and click on your printer/board link
 * NO IMPLIED SUPPORT OR WARRANTY IS PROVIDED WITH THIS FIRMWARE AND IS PROVIDED AS-IS
 */
#pragma once
#define CONFIGURATION_H_VERSION 02000900

//===========================================================================
//============================ TH3D Configuration ===========================
//===========================================================================

// ONLY UNCOMMENT THINGS IN ONE PRINTER SECTION!!! IF YOU HAVE MULTIPLE MACHINES FLASH THEM ONE AT A TIME.
// UNCOMMENT MEANS REMOVING THE // IN FRONT OF A #define XXXXXX LINE.

//===========================================================================
// ******************   CREALITY PRINTERS W/V4.X BOARD    *******************
//===========================================================================
//------------------------------ V4.2.2 Board -------------------------------
//#define ENDER3_V422_BOARD
//#define ENDER3_MAX_V422_BOARD
//#define ENDER3_V2_V422_BOARD
//#define ENDER5_V422_BOARD

// V4.2.2 TMC Driver Settings - Uncomment if you have TMC drivers on a 4.2.2 Board to set driver timings
//#define V422_TMC220X_DRIVERS //"A" or "B" Code on SD Slot

//------------------------------ V4.2.7 Board --------------------------------
//#define ENDER3_V427_BOARD
//#define ENDER3_MAX_V427_BOARD
//#define ENDER3_V2_V427_BOARD
//#define ENDER5_V427_BOARD

//#define CR10_V427_BOARD
//#define CR10MINI_V427_BOARD
//#define CR10S4_V427_BOARD
//#define CR10S5_V427_BOARD
// NOTE: It is HIGHLY recommended to use an external bed MOSFET with the CR-10 series machines due to the high load the beds have.
// While these boards work on 12V machines, they are designed for 24V printers that pull less current (specifically on the bed).
// If you need a MOSFET, we carry one here: https://www.th3dstudio.com/product/high-amp-12v-24v-mosfet-heated-bed-or-hotend/

//------------------------------ V4.3.1 Board -------------------------------
//#define ENDER6_V431_BOARD

// If you are using our EZOut V1/V2 (connected to LCD header) filament sensor kit please follow the install guide
// and then uncomment the #define EZOUT_ENABLE line below.
// Do NOT ever connect our filament sensor without the supplied adapter board.
// Ender 3 V2 needs LCD converted to use an EZOut: https://support.th3dstudio.com/hc/guides/upgrades-printer-information/ender-3-v2-swapping-to-the-12864-creality-lcd-for-more-features/
//#define EZOUT_ENABLE

// Creality CR-10S Series Filament Sensor
// Connect the stock sensor to the "J1" port and uncomment the below line to enable the filament sensor.
//#define CR10S_STOCKFILAMENTSENSOR

// EZABL Probe Mounts
//#define CR10_OEM
//#define ENDER3_OEM
//#define ENDER3_V2_OEM
//#define ENDER3_MAX_OEM
//#define ENDER5_OEM
//#define ENDER6_OEM
//#define CUSTOM_PROBE

// Ender 3 Specific Options

// Ender Xtender Kit Options
//#define ENDER_XTENDER_300
//#define ENDER_XTENDER_400
//#define ENDER_XTENDER_400XL
//#define ENDER_XTENDER_XL

// Ender 5 - Leadscrew Setting
// If you have the new Ender 5/5 Pro Model that has the new 800steps/mm Z leadscrew uncomment the below option to set the correct steps/mm
//#define ENDER5_NEW_LEADSCREW

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

// EXTRUDER SETTINGS -------------------------------
// Use to set custom esteps and/or reverse your E Motor direction if you are installing an extruder that needs the direction reversed.
// If you reversed the wiring on your E motor already (like the Bondtech Guide says to do) then you do not need to reverse it in the firmware here.

// If you want to change the Esteps for your printer you can uncomment the below line and set CUSTOM_ESTEPS_VALUE to what you want - USE WHOLE NUMBERS ONLY
// This option sets the esteps from the CUSTOM_ESTEPS_VALUE line below.
// If you need to reverse the e motor direction also enabled the REVERSE_E_MOTOR_DIRECTION option.
// Example EStep Values: TH3D Aluminum Extruder - 95 ESteps, TH3D Tough Extruder - 410 ESteps, BMG Extruder - 415 ESteps
// When installing a Tough Extruder or E3D Titan or Bondtech that is Geared you likely need to enable the REVERSE_E_MOTOR_DIRECTION option
//#define CUSTOM_ESTEPS
#define CUSTOM_ESTEPS_VALUE 410
//#define REVERSE_E_MOTOR_DIRECTION

// FILAMENT SENSOR UNLOAD SETTINGS -----------------
// If you have a filament sensor that is physically mounted to the machine you can enable MOUNTED_FILAMENT_SENSOR to set the unload length to 0 to prevent filament from backing up in the sensor by uncommenting MOUNTED_FILAMENT_SENSOR 
//#define MOUNTED_FILAMENT_SENSOR

// If you have a direct drive machine with a filament sensor uncomment DIRECT_DRIVE_PRINTER to decrease the unload length from 100mm to 20mm
//#define DIRECT_DRIVE_PRINTER

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
// There are 2 ways to connect the BL Touch to the V4.2.X boards - All on the 5 pin header or using 3 pins on the 5 pin header + Z Endstop port
// For details on these 2 types of connections refer to our help center article here: https://support.th3dstudio.com/hc/product-information/3rd-party-control-boards/creality-boards/creality-v4-2-2-v4-2-7-board-bl-touch-wiring-options/
// If you want to use the BL-Touch uncomment the BLTOUCH line below and uncomment #define CUSTOM_PROBE above and then enter in your offsets above in the CUSTOM PROBE section.
//#define BLTOUCH
// If you are using the 5 pin header for all the BL Touch connections, uncomment the below line
//#define CREALITY_V42X_BLTOUCH_ON_5PIN

// MANUAL MESH LEVELING ----------------------------
// If you want to use manual mesh leveling you can enable the below option. This is for generating a MANUAL mesh WITHOUT a probe.
// Mesh Bed Leveling Documentation: http://marlinfw.org/docs/gcode/G029-mbl.html 
// NOTE: If you want to automate the leveling process our EZABL kits do this for you. Check them out here: http://EZABL.TH3DStudio.com
//#define MANUAL_MESH_LEVELING

// POWER LOSS RECOVERY -----------------------------
// Continue after Power-Loss feature will store the current state to the SD Card at the start of each layer
// during SD printing. If this is found at bootup it will ask you if you want to resume the print.
//
// NOTE: This feature causes excessive wear on your SD card.
//#define POWER_LOSS_RECOVERY

// ARC Support Override ----------------------------
// Arc support is enabled by default on all builds but this takes up extra space. If you get compile errors due to the size being too large when enabling other options, then disable ARC_SUPPORT
// by uncommenting the DISABLE_ARC_SUPPORT line below.
//#define DISABLE_ARC_SUPPORT

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
 
//CR-10 Series V427 Settings
#if ENABLED(CR10_V427_BOARD) || ENABLED(CR10MINI_V427_BOARD) || ENABLED(CR10S4_V427_BOARD) || ENABLED(CR10S5_V427_BOARD)
  //V42X with TMC Driver Sanity Checks
  #if ENABLED(LINEAR_ADVANCE)
    #error "Linear Advance does NOT work on the V4.2.7 boards with the TMC drivers due to how Creality has them setup. Disable Linear Advance to continue."
  #endif

  #define SERIAL_PORT 1

  #define BAUDRATE 115200
  
  #define CR10_STOCKDISPLAY
  #define RET6_12864_LCD
  
  #if ENABLED(REVERSE_KNOB_DIRECTION)
    #define REVERSE_ENCODER_DIRECTION
  #endif
  
  #define MOTHERBOARD BOARD_CREALITY_V427

  #if ENABLED(CUSTOM_ESTEPS)
    #define DEFAULT_AXIS_STEPS_PER_UNIT   { 80, 80, 400, CUSTOM_ESTEPS_VALUE }
  #else
    #define DEFAULT_AXIS_STEPS_PER_UNIT   { 80, 80, 400, 95 }
  #endif
  
  #define DEFAULT_MAX_FEEDRATE          { 200, 200, 15, 50 }
  #define DEFAULT_MAX_ACCELERATION      { 1000, 1000, 500, 5000 }

  #define DEFAULT_ACCELERATION          500
  #define DEFAULT_RETRACT_ACCELERATION  500
  #define DEFAULT_TRAVEL_ACCELERATION   1000

  #define CLASSIC_JERK
  #if ENABLED(CLASSIC_JERK)
    #if ENABLED(CR10S4_V427_BOARD) || ENABLED(CR10S5_V427_BOARD)
      #define DEFAULT_XJERK                 5.0
      #define DEFAULT_YJERK                 5.0
    #else
      #define DEFAULT_XJERK                 7.0
      #define DEFAULT_YJERK                 7.0
    #endif
    #define DEFAULT_ZJERK  0.3
  #endif

  #define DEFAULT_EJERK    5.0

  #define SHOW_BOOTSCREEN

  #define EXTRUDERS 1

  #if ENABLED(CR10_V427_BOARD)
    #define X_BED_SIZE 300
    #define Y_BED_SIZE 300
    #define Z_MAX_POS 400
    #define PRINTER_VOLTAGE_12
  #endif

  #if ENABLED(CR10MINI_V427_BOARD)
    #define X_BED_SIZE 300
    #define Y_BED_SIZE 220
    #define Z_MAX_POS 300
    #define PRINTER_VOLTAGE_12
  #endif

  #if ENABLED(CR10S4_V427_BOARD)
    #define X_BED_SIZE 400
    #define Y_BED_SIZE 400
    #define Z_MAX_POS 400
    #define PRINTER_VOLTAGE_12
    #define SLOWER_PROBE_MOVES
  #endif

  #if ENABLED(CR10S5_V427_BOARD)
    #define X_BED_SIZE 500
    #define Y_BED_SIZE 500
    #define Z_MAX_POS 500
    #define PRINTER_VOLTAGE_12
    #define SLOWER_PROBE_MOVES
  #endif
  
  #if ENABLED(HOME_ADJUST)
    #define X_MIN_POS X_HOME_LOCATION
    #define Y_MIN_POS Y_HOME_LOCATION
  #else
    #define X_MIN_POS 0
    #define Y_MIN_POS 0
  #endif

  #define USE_XMIN_PLUG
  #define USE_YMIN_PLUG
  #define USE_ZMIN_PLUG

  #define X_HOME_DIR -1
  #define Y_HOME_DIR -1
  #define Z_HOME_DIR -1
  
  #if NONE(V6_HOTEND, TH3D_HOTEND_THERMISTOR, KNOWN_HOTEND_THERMISTOR)
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

  #define ENDSTOPPULLUPS

  #define X_MIN_ENDSTOP_INVERTING false
  #define Y_MIN_ENDSTOP_INVERTING false
  #define Z_MIN_ENDSTOP_INVERTING false
  #define X_MAX_ENDSTOP_INVERTING false
  #define Y_MAX_ENDSTOP_INVERTING false
  #define Z_MAX_ENDSTOP_INVERTING false
  #define Z_MIN_PROBE_ENDSTOP_INVERTING false

  #if ENABLED(CREALITY_V42X_BLTOUCH_ON_5PIN)
    #define USE_PROBE_FOR_Z_HOMING
  #else
    #define Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN
  #endif

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

  #define ENCODER_PULSES_PER_STEP 4
  #define ENCODER_STEPS_PER_MENU_ITEM 1
  
  #if ENABLED(EZOUT_ENABLE)
    #define FILAMENT_RUNOUT_SENSOR
    #define SPEAKER_KILL
  #endif

  #if ENABLED(EZOUT_ENABLE) || ENABLED(CR10S_STOCKFILAMENTSENSOR)
    #define FILAMENT_RUNOUT_SENSOR
  #endif

  #if ENABLED(FILAMENT_RUNOUT_SENSOR)
    #define FIL_RUNOUT_ENABLED_DEFAULT true // Enable the sensor on startup. Override with M412 followed by M500.
    #define NUM_RUNOUT_SENSORS   1          // Number of sensors, up to one per extruder. Define a FIL_RUNOUT#_PIN for each.
    #if ENABLED(EZOUT_ENABLE)
      #define FIL_RUNOUT_STATE     LOW       // Pin state indicating that filament is NOT present.
    #else
      #define FIL_RUNOUT_STATE     HIGH       // Pin state indicating that filament is NOT present.
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
#endif
// End CR-10 Series V427 Settings
 
//Ender 6 V431 Board Settings
#if ENABLED(ENDER6_V431_BOARD)
  //V431 with TMC Driver Sanity Checks
  #if ENABLED(LINEAR_ADVANCE)
    #error "Linear Advance does NOT work on the V4.3.1 boards with the TMC drivers due to how Creality has them setup. Disable Linear Advance to continue."
  #endif

  #define SERIAL_PORT 1

  #define BAUDRATE 115200
  
  #define CR10_STOCKDISPLAY
  #define RET6_12864_LCD
  
  #if ENABLED(REVERSE_KNOB_DIRECTION)
    #define REVERSE_ENCODER_DIRECTION
  #endif

  #ifndef MOTHERBOARD
    #define MOTHERBOARD BOARD_CREALITY_V431
  #endif

  #if ENABLED(CUSTOM_ESTEPS)
    #define DEFAULT_AXIS_STEPS_PER_UNIT   { 80, 80, 400, CUSTOM_ESTEPS_VALUE }
  #else
    #define DEFAULT_AXIS_STEPS_PER_UNIT   { 80, 80, 400, 140 }
  #endif
  
  #define DEFAULT_MAX_FEEDRATE          { 200, 200, 15, 50 }
  #define DEFAULT_MAX_ACCELERATION      { 3000, 3000, 500, 5000 }

  #define DEFAULT_ACCELERATION          1500
  #define DEFAULT_RETRACT_ACCELERATION  500
  #define DEFAULT_TRAVEL_ACCELERATION   1500

  #define CLASSIC_JERK
  #if ENABLED(CLASSIC_JERK)
    #define DEFAULT_XJERK  10.0
    #define DEFAULT_YJERK  10.0
    #define DEFAULT_ZJERK  0.3
  #endif

  #define DEFAULT_EJERK    5.0

  #define SHOW_BOOTSCREEN

  #define EXTRUDERS 1

  #define X_BED_SIZE 250
  #define Y_BED_SIZE 250
  #define Z_MAX_POS 400
  
  #if ENABLED(HOME_ADJUST)
    #define X_MIN_POS X_HOME_LOCATION
    #define Y_MIN_POS Y_HOME_LOCATION
  #else
    #define X_MIN_POS 0
    #define Y_MIN_POS 0
  #endif

  #define USE_XMAX_PLUG
  #define USE_YMAX_PLUG
  #define USE_ZMIN_PLUG

  #define X_HOME_DIR 1
  #define Y_HOME_DIR 1
  #define Z_HOME_DIR -1

  #if NONE(V6_HOTEND, TH3D_HOTEND_THERMISTOR, KNOWN_HOTEND_THERMISTOR)
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

  #if ENABLED(BLTOUCH)
    #define USE_PROBE_FOR_Z_HOMING
  #else
    #define Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN
  #endif

  #define X_DRIVER_TYPE TMC2208_STANDALONE
  #define Y_DRIVER_TYPE TMC2208_STANDALONE
  #define Z_DRIVER_TYPE TMC2208_STANDALONE
  #define E0_DRIVER_TYPE TMC2208_STANDALONE

  #define ENDSTOP_INTERRUPTS_FEATURE

  #define X_ENABLE_ON 0
  #define Y_ENABLE_ON 0
  #define Z_ENABLE_ON 0
  #define E_ENABLE_ON 0

  #define COREYX

  #define INVERT_X_DIR false
  #define INVERT_Y_DIR false 
  #define INVERT_Z_DIR false

  #if ENABLED(REVERSE_E_MOTOR_DIRECTION)
    #define INVERT_E0_DIR false
  #else
    #define INVERT_E0_DIR true
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

  #define FILAMENT_RUNOUT_SENSOR

  #if ENABLED(FILAMENT_RUNOUT_SENSOR)
    #define FIL_RUNOUT_ENABLED_DEFAULT true // Enable the sensor on startup. Override with M412 followed by M500.
    #define NUM_RUNOUT_SENSORS   1          // Number of sensors, up to one per extruder. Define a FIL_RUNOUT#_PIN for each.
    #define FIL_RUNOUT_STATE     LOW       // Pin state indicating that filament is NOT present.
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

#endif
// End Ender 6 Settings
 
//Ender 3/3 MAX/5 V42X Board Settings
#if ENABLED(ENDER3_V422_BOARD) || ENABLED(ENDER5_V422_BOARD) || ENABLED(ENDER3_V427_BOARD) || ENABLED(ENDER5_V427_BOARD) || ENABLED(ENDER3_MAX_V422_BOARD) || ENABLED(ENDER3_MAX_V427_BOARD)
  //V42X with TMC Driver Sanity Checks
  #if (ENABLED(V422_TMC220X_DRIVERS) || ENABLED(ENDER3_V427_BOARD) || ENABLED(ENDER5_V427_BOARD) || ENABLED(ENDER3_MAX_V427_BOARD)) && ENABLED(LINEAR_ADVANCE)
    #error "Linear Advance does NOT work on the V4.2.X boards with the TMC drivers due to how Creality has them setup. Disable Linear Advance to continue."
  #endif

  #define SERIAL_PORT 1

  #define BAUDRATE 115200
  
  #define CR10_STOCKDISPLAY
  #define RET6_12864_LCD
  
  #if ENABLED(REVERSE_KNOB_DIRECTION)
    #define REVERSE_ENCODER_DIRECTION
  #endif
  
  #if ENABLED(ENDER3_V422_BOARD) || ENABLED(ENDER5_V422_BOARD) || ENABLED(ENDER3_MAX_V422_BOARD)
    #ifndef MOTHERBOARD
      #define MOTHERBOARD BOARD_CREALITY_V4
    #endif
  #else
    #ifndef MOTHERBOARD
      #define MOTHERBOARD BOARD_CREALITY_V427
    #endif
  #endif
  
  #if ENABLED(ENDER5_NEW_LEADSCREW)
    #define CREALITY_Z_STEPS 800
  #else
    #define CREALITY_Z_STEPS 400
  #endif

  #if ENABLED(CUSTOM_ESTEPS)
    #define DEFAULT_AXIS_STEPS_PER_UNIT   { 80, 80, CREALITY_Z_STEPS, CUSTOM_ESTEPS_VALUE }
  #else
    #define DEFAULT_AXIS_STEPS_PER_UNIT   { 80, 80, CREALITY_Z_STEPS, 95 }
  #endif
  
  #define DEFAULT_MAX_FEEDRATE          { 200, 200, 15, 50 }
  #define DEFAULT_MAX_ACCELERATION      { 1000, 1000, 500, 5000 }

  #define DEFAULT_ACCELERATION          500
  #define DEFAULT_RETRACT_ACCELERATION  500
  #define DEFAULT_TRAVEL_ACCELERATION   1000

  #define CLASSIC_JERK
  #if ENABLED(CLASSIC_JERK)
    #define DEFAULT_XJERK  7.0
    #define DEFAULT_YJERK  7.0
    #define DEFAULT_ZJERK  0.3
  #endif

  #define DEFAULT_EJERK    5.0

  #define SHOW_BOOTSCREEN

  #define EXTRUDERS 1

  #if ENABLED(ENDER5_V422_BOARD) || ENABLED(ENDER5_V427_BOARD)
    #define X_BED_SIZE 220
    #define Y_BED_SIZE 220
    #define Z_MAX_POS 300
  #elif ENABLED(ENDER3_MAX_V422_BOARD) || ENABLED(ENDER3_MAX_V427_BOARD)
    #define X_BED_SIZE 300
    #define Y_BED_SIZE 300
    #define Z_MAX_POS 340
  #else
    #if ENABLED(ENDER_XTENDER_400)
      #define X_BED_SIZE 400
      #define Y_BED_SIZE 400
      #define Z_MAX_POS 250
    #elif ENABLED(ENDER_XTENDER_300)
      #define X_BED_SIZE 300
      #define Y_BED_SIZE 300
      #define Z_MAX_POS 250
    #elif ENABLED(ENDER_XTENDER_400XL)
      #define X_BED_SIZE 400
      #define Y_BED_SIZE 400
      #define Z_MAX_POS 500
    #elif ENABLED(ENDER_XTENDER_XL)
      #define X_BED_SIZE 235
      #define Y_BED_SIZE 235
      #define Z_MAX_POS 500
    #else
      #define X_BED_SIZE 235
      #define Y_BED_SIZE 235
      #define Z_MAX_POS 250
    #endif
  #endif
  
  #if ENABLED(HOME_ADJUST)
    #define X_MIN_POS X_HOME_LOCATION
    #define Y_MIN_POS Y_HOME_LOCATION
  #else
    #define X_MIN_POS 0
    #define Y_MIN_POS 0
  #endif

  #if ENABLED(ENDER5_V422_BOARD) || ENABLED(ENDER5_V427_BOARD)
    #define USE_XMAX_PLUG
    #define USE_YMAX_PLUG
    #define USE_ZMIN_PLUG
  #else
    #define USE_XMIN_PLUG
    #define USE_YMIN_PLUG
    #define USE_ZMIN_PLUG
  #endif

  #if ENABLED(ENDER5_V422_BOARD) || ENABLED(ENDER5_V427_BOARD)
    #define X_HOME_DIR 1
    #define Y_HOME_DIR 1
    #define Z_HOME_DIR -1
  #else
    #define X_HOME_DIR -1
    #define Y_HOME_DIR -1
    #define Z_HOME_DIR -1
  #endif

  #if NONE(V6_HOTEND, TH3D_HOTEND_THERMISTOR, KNOWN_HOTEND_THERMISTOR)
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

  #if ENABLED(CREALITY_V42X_BLTOUCH_ON_5PIN)
    #define USE_PROBE_FOR_Z_HOMING
  #else
    #define Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN
  #endif

  #if ENABLED(ENDER3_V427_BOARD) || ENABLED(ENDER5_V427_BOARD) || ENABLED(V422_TMC220X_DRIVERS) || ENABLED(ENDER3_MAX_V427_BOARD) 
    #define X_DRIVER_TYPE TMC2208_STANDALONE
    #define Y_DRIVER_TYPE TMC2208_STANDALONE
    #define Z_DRIVER_TYPE TMC2208_STANDALONE
    #define E0_DRIVER_TYPE TMC2208_STANDALONE
  #elif ENABLED(ENDER3_MAX_V422_BOARD)
    #define X_DRIVER_TYPE TMC2208_STANDALONE
    #define Y_DRIVER_TYPE TMC2208_STANDALONE
    #define Z_DRIVER_TYPE A4988
    #define E0_DRIVER_TYPE A4988  
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
  
  #if ENABLED(ENDER5_V422_BOARD) || ENABLED(ENDER5_V427_BOARD)
    #define INVERT_Z_DIR false
  #else  
    #define INVERT_Z_DIR true
  #endif

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
  
  #if ENABLED(EZOUT_ENABLE)
    #define FILAMENT_RUNOUT_SENSOR
    #define SPEAKER_KILL
  #endif

  #if ENABLED(ENDER3_MAX_V422_BOARD) || ENABLED(ENDER3_MAX_V427_BOARD)
    #define FILAMENT_RUNOUT_SENSOR
  #endif

  #if ENABLED(FILAMENT_RUNOUT_SENSOR)
    #define FIL_RUNOUT_ENABLED_DEFAULT true // Enable the sensor on startup. Override with M412 followed by M500.
    #define NUM_RUNOUT_SENSORS   1          // Number of sensors, up to one per extruder. Define a FIL_RUNOUT#_PIN for each.
    #define FIL_RUNOUT_STATE     LOW       // Pin state indicating that filament is NOT present.
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
#endif
// End Ender 3/3 MAX/5 V42X Board Settings
 
// Ender 3 V2 Settings
#if ENABLED(ENDER3_V2_V422_BOARD) || ENABLED(ENDER3_V2_V427_BOARD)
  #define SERIAL_PORT 1
  #define SERIAL_PORT_2 3
  
  #if ENABLED(MANUAL_MESH_LEVELING)
    #error "Due to closed source LCD firmware, Manual Mesh Leveling is not available on the Ender 3 V2."
  #endif

  #define BAUDRATE 115200

  #if ENABLED(ENDER3_V2_V422_BOARD) || ENABLED(ENDER3_V2_V422_BOARD)
    #ifndef MOTHERBOARD
      #define MOTHERBOARD BOARD_CREALITY_V4
    #endif
  #else
    #ifndef MOTHERBOARD
      #define MOTHERBOARD BOARD_CREALITY_V427
    #endif
  #endif

  #if ENABLED(CUSTOM_ESTEPS)
    #define DEFAULT_AXIS_STEPS_PER_UNIT   { 80, 80, 400, CUSTOM_ESTEPS_VALUE }
  #else
    #define DEFAULT_AXIS_STEPS_PER_UNIT   { 80, 80, 400, 95 }
  #endif
  
  #define DEFAULT_MAX_FEEDRATE          { 200, 200, 15, 50 }
  #define DEFAULT_MAX_ACCELERATION      { 1000, 1000, 500, 5000 }

  #define DEFAULT_ACCELERATION          500
  #define DEFAULT_RETRACT_ACCELERATION  500
  #define DEFAULT_TRAVEL_ACCELERATION   1000

  #define CLASSIC_JERK
  #if ENABLED(CLASSIC_JERK)
    #define DEFAULT_XJERK  7.0
    #define DEFAULT_YJERK  7.0
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

  #if NONE(V6_HOTEND, TH3D_HOTEND_THERMISTOR, KNOWN_HOTEND_THERMISTOR)
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

  #if ENABLED(CREALITY_V42X_BLTOUCH_ON_5PIN)
    #define USE_PROBE_FOR_Z_HOMING
  #else
    #define Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN
  #endif

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
  #define EXTRUDE_MAXLENGTH 1000

  #define POWER_LOSS_RECOVERY
  
  #if ENABLED(EZOUT_ENABLE)
    #define FILAMENT_RUNOUT_SENSOR
    #define SPEAKER_KILL
  #endif

  #if ENABLED(FILAMENT_RUNOUT_SENSOR)
    #define FIL_RUNOUT_ENABLED_DEFAULT true // Enable the sensor on startup. Override with M412 followed by M500.
    #define NUM_RUNOUT_SENSORS   1          // Number of sensors, up to one per extruder. Define a FIL_RUNOUT#_PIN for each.
    #define FIL_RUNOUT_STATE     LOW       // Pin state indicating that filament is NOT present.
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
  
#endif
// End Ender 3 V2 Settings

/*
 * All other settings are stored in the Configuration_backend.h file. Do not change unless you know what you are doing.
 */
 
#include "Configuration_backend.h"