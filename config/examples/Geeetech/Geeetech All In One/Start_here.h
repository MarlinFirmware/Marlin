#pragma once

// = commented out or disabled removing // enables it

//Step 1 uncomment a model.
//GT2560 Boards- Mega2560 8bit---------------------------------------------------------
#define GTA10       // A10 & Variants - tested fully functional
//#define GTA20       // A20 & Variants - tested fully functional
//#define MECREATOR2  // Mecreator2     - tested fully functional
//#define I3PROA      // PI3A Pro - testing
//#define I3PROB      // I3PROB   - testing
//#define I3PROC      // I3PROC   - testing
//#define I3PROW      // I3PROW   - testing
//#define I3PROX      // I3PROX   - testing
//GTM32 Boards - STM32F103 VE 32bit----------------------------------------------------
//#define GTA30       // A30  - experimental
//#define GTE180      // E180 - experimental
//#define GTM201      // M201 - experimental

//Step 2 uncomment 1 probe type else disable both for manual,
#define Probe    // Enable 3dtouch/bltouch Probe.
//#define FMP      // Enable Fixed Mounted Probe.

//Step 3 uncomment 1 driver timing set 
#define TMC2208    // Enable TMC2208 standard inverted on all drivers
//#define TMC2209    // Enable TMC2209 on all drivers
//#define TMC2130    // Enable TMC2130 on all drivers
//#define TMC2160    // Enable TMC2160 on all drivers
//#define TMC26X     // Enable TMC226X on all drivers
//#define TMC2660    // Enable TMC2660 on all drivers
//#define TMC5130    // Enable TMC5130 on all drivers
//#define TMC5160    // Enable TMC5160 on all drivers
//#define A4988      // Enable A4988   on all drivers (stock drivers)
//#define A5984      // Enable A5984   on all drivers
//#define DRV8825    // Enable DRV8825 on all drivers
//#define LV8729     // Enable LV8729  on all drivers
//#define L6470      // Enable L6470   on all drivers
//#define TB6560     // Enable TB6560  on all drivers
//#define TB6600     // Enable TB6600  on all drivers

//Step 4 uncomment as needed to invert motors direction generally used with TMC drivers
#define INVERTE // invert E direction disabe if moves wrong direction // the geared extruder on M variants also use inverted
#define INVERTXYZ // inverts XYZ direction disable if moves wrong direction

//Step 5  uncomment 1 XYZE step set suggest you run your own calibrations these are base line.
//#define DEFAULT_AXIS_STEPS_PER_UNIT   { 80, 80, 400, 93 }  // ungeared like found on a10/a20/a30/i3pro
//#define DEFAULT_AXIS_STEPS_PER_UNIT   { 80, 80, 400, 430 } // geared extruder like found stock on mix variants
//#define DEFAULT_AXIS_STEPS_PER_UNIT   { 80, 80, 400, 415 } // bmg extruder
//#define DEFAULT_AXIS_STEPS_PER_UNIT   { 80, 80, 2560, 93 } // M8 Z rod steps 2560
//----------------------------------------------------------------------------------------------------------------------------------
#define DEFAULT_AXIS_STEPS_PER_UNIT   { 81, 81, 405, 98 } // Verts calibrated steps results in perfect sized prints on A10/A20
//#define DEFAULT_AXIS_STEPS_PER_UNIT   { 81, 81, 405, 430 } // Verts calibrated steps results in perfect sized prints on A10M/A20M


//Step 6 uncomment 1 if you have mix/multi extruder.
//#define MIX      // Enable Mixing, Geeetech 2 in 1 control system - Virtual Motor Control (E0+M163-M164-M165-M166) 
//#define MIXT     // Enable Mixing, Geeetech 3 in 1 control system - Virtual Motor Control (E0+M163-M164-M165-M166) 
//#define CYCLOPS  // alternative to mix for 2 in 1 - Physical control (E0+E1) - this is how most people expect 2 in 1 to work 
//#define DUALEX   // dual extruder 2 in 2 out
//---------------------------------------------------------------------------------------------------------------------------------
//#define CYCLOPST // alternative to mix for 3 in 1 - Physical control (E0+E1+E2) - experimental missing pins defines for E2
//#define TRIEX    // tripple extruder  3 in 3 out - experimental missing pins defines for E2

//Adjustable Features
#define MAXHOTENDTEMP 260 // default max hotend temp 260
#define MAXBEDTEMP 135    // default max bed temp 135
#define MIN_PROBE_EDGE 0  // #mm to stay away from edges of bed for leveling suggest 5-10mm if you have issues with reaching max travel
#define NOZZLE_TO_PROBE_OFFSET { -37, -1, 0 } // Probe offset XYZ set for A10/A20+M variants
//#define PLR    // Enable power loss resume feature only works from sdcard - high memory cost 
//#define RUNOUT // Enable filament runout feature
