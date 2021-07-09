/**
 * For directions on how to use this firmware visit http://uf2.th3dstudio.com and click on your printer/board link
 * NO IMPLIED SUPPORT OR WARRANTY IS PROVIDED WITH THIS FIRMWARE AND IS PROVIDED AS-IS
 */
#pragma once
#define CONFIGURATION_H_VERSION 02000901

//#@CONFIGURATION_START_FLAG

//===========================================================================
//============================ TH3D Configuration ===========================
//===========================================================================

// ONLY UNCOMMENT THINGS IN ONE PRINTER SECTION!!! IF YOU HAVE MULTIPLE MACHINES FLASH THEM ONE AT A TIME.
// UNCOMMENT MEANS REMOVING THE // IN FRONT OF A #define XXXXXX LINE.

//===========================================================================
// ****************    EZBOARD LITE BOARD POWERED PRINTERS   ****************
//===========================================================================

// CR-10 Series -------------------------------------------------------------
#define CR10
//#define CR10_MINI
//#define CR10_S4
//#define CR10_S5

// CR-10S Series ------------------------------------------------------------
//#define CR10S
//#define CR10S_MINI
//#define CR10S_S4
//#define CR10S_S5

// Ender Series -------------------------------------------------------------
//#define ENDER2
//#define ENDER3
//#define ENDER3_MAX
//#define ENDER5
//#define ENDER5_PLUS

// Other Machines -----------------------------------------------------------
//#define CR20

// Sovol Machines -----------------------------------------------------------
//#define SOVOL_SV01

// Filament Sensor Options --------------------------------------------------
// If your machine came stock with a filament sensor it will be enabled automatically. If you replaced your stock sensor with our EZOut or you added an EZOut enabling the EZOUTV2_ENABLE will override the Creality sensor if your machine had one

// If you are using our EZOut Sensor connect to the FIL SENSOR header with the RED wire lined up with the "5V" marking by the header and uncomment the below line.
//#define EZOUTV2_ENABLE

// If you are using the Creality CR-10S Sensor connect to the FIL SENSOR header with the RED wire lined up with the "5V" marking by the header and uncomment the below line.
//#define CR10S_STOCKFILAMENTSENSOR

// EZABL Probe Mounts -------------------------------------------------------
//#define CR10_OEM                 //OEM Mount for Creality Machines (Ender3/Ender5/CR-10/CR-10S/CR-20)
//#define ENDER2_OEM               //Ender 2 Specific OEM Mount
//#define ENDER2_V6                //Ender 2 Specific V6 Mount
//#define ENDER3_MAX_OEM           //Ender 3 MAX Specific OEM Mount
//#define SV01_OEM_MOUNT           //Sovol SV01 OEM Mount
//#define CR10_VOLCANO             //TH3D CR-10 Volcano Mount 
//#define CR10_V6HEAVYDUTY         //V6 Heavy Duty Mount
//#define TM3DAERO                 //TM3D Aero Mount for V6
//#define TM3DAERO_EXTENDED        //TM3D Arto Mount for Volcano
//#define PETSFANG                 //This is the RIGHT mounted version
//#define CUSTOM_PROBE             //For any other probe mount (also used for BL Touch), Enter offsets below

// Ender 5 Plus ONLY ABL Settings -------------------------------------------
// By default the Ender 5 Plus comes with a BL Touch. Enabling the ENDER5_PLUS_EZABL or ENDER5_PLUS_NOABL will override the BL Touch setting
// If you are using the stock BL Touch with a non-stock mount enable the CUSTOM_PROBE line above and enter the offsets below for the new mount.
//#define ENDER5_PLUS_EZABL
//#define ENDER5_PLUS_NOABL

// EZNeo Settings -----------------------------------------------------------
// If you are using an EZNeo strip on your printer, uncomment the line for what strip you are using.
//#define EZNEO_220

// Advanced Settings --------------------------------------------------------
// These settings do not typically need to be adjusted except for machines that do not follow stock configs

// If you are using a modded machine with a higher Z height use the below option to change the height. This value is in mm.
//#define CUSTOM_ZHEIGHT 400

// If you have the new Ender 5 or Ender 5 Pro Model that has the new leadscrew uncomment the below option to set the correct stepping mode for the Z driver
//#define ENDER5_NEW_LEADSCREW

// Dual Z Motor Settings
// When running dual Z motors uncomment the below line. This will increase the Z motor driver current for 2x motors.
//#define DUAL_Z_MOTORS

// Axis Direction Settings
// If you need to reverse the direction of a motor uncomment the below option for that axis.
// E motor settings are below in the Extruder Settings Section
//#define REVERSE_X_MOTOR
//#define REVERSE_Y_MOTOR
//#define REVERSE_Z_MOTOR

// E Motor Torque Settings
// By default we have StealthChop2 Disabled for the E axis. This is due to the possibility of losing steps
// due to the low torque nature of StealthChop2. If you want to quiet down the E motor you can uncomment the
// below line. However, if you get lost steps and inconsistent extrusion then disable this option.
//#define STEALTHCHOP_E

// Fix for Older EZABL Kits
// If you are having issues with the EZABL not triggering when connected to the Z-Stop header you can use the servo header pins.
// See here for details: https://www.th3dstudio.com/knowledgebase/ezabl-ezboard-not-triggering-fix/
//#define V3_EZABL_ON_SERVO

// E3D PT100 Thermocouple Support
// Refer to the EZBoard installation documentation for connecting the PT100 to the EZBoard Expansion Header
//#define EZBOARD_PT100

// Ender Xtender Kit Options
//#define ENDER_XTENDER_300
//#define ENDER_XTENDER_400
//#define ENDER_XTENDER_400XL
//#define ENDER_XTENDER_XL

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
  * In the following examp