#pragma once

// =  disabled removing // enables it

//Step 1) enable 1 model.
//GT2560 Boards - Mega2560 8bit - Arduino Librarys: U8glib Library required for mixing variants.
#define GTA10       // A10 & Variants - tested fully functional
//#define GTA20       // A20 & Variants - tested fully functional
//#define MECREATOR2  // Mecreator2     - tested fully functional
//#define I3PROA      // PI3A Pro       - testing
//#define I3PROB      // I3PROB         - testing
//#define I3PROC      // I3PROC         - testing
//#define I3PROW      // I3PROW         - testing
//#define I3PROX      // I3PROX         - testing

//GTM32 Boards - STM32F103VE 32bit - Arduino Librarys : https://github.com/rogerclarkmelbourne/Arduino_STM32 & https://github.com/stm32duino/Arduino_Core_STM32 
//Delete folder /Marlin-bugfix-2.0.x/Marlin/src/HAL/HAL_STM32 & Delete All but SPI.h in Folder Arduino_STM32-master/STM32F1/libraries/SPI/src/
//#define GTA30       // A30  - experimental, set jumpers base on your board for flashing and use serial to upload.
//#define GTE180      // E180 - experimental, set jumpers base on your board for flashing and use serial to upload.
//#define GTM201      // M201 - experimental, set jumpers base on your board for flashing and use serial to upload.
//#define GTD200      // D200 - experimental, set jumpers base on your board for flashing and use serial to upload.

//Step 2) enable 1 probe type else disable both for manual(no probe stock),
#define Probe    // Enable Bltouch Type Probe.
//#define FMP      // Enable Fixed Mount Type Probe.

//If a probe is enabled
#define NOZZLE_TO_PROBE_OFFSET { -37, -1, 0 } // Probe offset XYZ A10/A20 calibrating is suggested
//#define NOZZLE_TO_PROBE_OFFSET { -40, -1, 0 }  // Probe offset XYZ A10M/A20M calibrating is suggested.
//#define NOZZLE_TO_PROBE_OFFSET { 0, 0, 0 }     // Blank custom XYZ probe offsets
#define MIN_PROBE_EDGE 0  // #mm to stay away from edges of bed for leveling suggest 0-10mm if you have issues with reaching max travel or clips. 

//Step 3) enable 1 driver timing set 
#define T2208    // Enable TMC2208 Standalone on all drivers
//#define T2209    // Enable TMC2209 Standalone on all drivers
//#define T2130    // Enable TMC2130 Standalone on all drivers
//#define T2160    // Enable TMC2160 Standalone on all drivers
//#define T26X     // Enable TMC226X Standalone on all drivers
//#define T2660    // Enable TMC2660 Standalone on all drivers
//#define T5130    // Enable TMC5130 Standalone on all drivers
//#define T5160    // Enable TMC5160 Standalone on all drivers
//#define A498     // Enable A4988   on all drivers (stock drivers)
//#define A598     // Enable A5984   on all drivers
//#define D8825     // Enable DRV8825 on all drivers
//#define L8729     // Enable LV8729  on all drivers
//#define L647     // Enable L6470   on all drivers
//#define T6560     // Enable TB6560  on all drivers
//#define T6600     // Enable TB6600  on all drivers
//#define CUSTOM   // Fill in #elif ENABLED (CUSTOM) in configuration.h with a custom set

//Step 4) enable if needed to invert motors direction used with TMC drivers & geared extruders
#define INVERTE     // invert E direction disabe if moves wrong direction - the geared extruder on M variants are inverted
#define INVERTXYZ   // inverts XYZ direction disable if moves wrong direction

//Step 5) enable 1 XYZE step set or define your own, suggest you run your own calibrations these are perfect on my machines.
#define DEFAULT_AXIS_STEPS_PER_UNIT   { 81, 81, 410, 98 }  // ungeared like found on a10/a20/a30/i3pro
//#define DEFAULT_AXIS_STEPS_PER_UNIT   { 81, 81, 410, 430 } // geared extruder like found stock on mix variants
//#define DEFAULT_AXIS_STEPS_PER_UNIT   { 81, 81, 410, 415 } // bmg extruder
//#define DEFAULT_AXIS_STEPS_PER_UNIT   { 81, 81, 2560, 98 } // M8 Z rod steps 2560
//#define DEFAULT_AXIS_STEPS_PER_UNIT   { 0, 0, 0, 0 }       // Blank custom steps

//Step 6) enable 1 if you have mixing or multi extruder.
//#define MIX      // Enable Mixing, Geeetech 2 in 1 - Virtual Motor Control
//#define MIXT     // Enable Mixing, Geeetech 3 in 1 - Virtual Motor Control
//#define CYCLOPS  // Alternative to mixing   2 in 1 - Physical Motor Control 
//#define CYCLOPST // Alternative to mixing   3 in 1 - Physical Motor Control 
//#define DUALEX   // Dual extruder 2 in 2           - Physical Motor Control 
//#define TRIEX    // Dual extruder 3 in 3           - Physical Motor Control 

//Optional Features
//#define HAC         // Enable host action commands for octoprint
//#define RUNOUT      // Enable filament runout feature
//#define PLR         // Enable power loss resume feature only works from sdcard - memory use ~260bytes
#define SDCARD      // Enabled SD card slot - memory use ~1800bytes 

//Adjustable Features
#define MAXHOTENDTEMP 260    // Max hotend temp 260
#define MAXBEDTEMP    135    // Max bed temp 135
