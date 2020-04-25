#pragma once
#define SHORT_BUILD_VERSION "Bugfix 2+ Build 494"
#define CUSTOM_MACHINE_NAME "3D Printer"

// Ctrl+click to open links
// Youtube             "https://youtube.com/verta"
// Discord             "https://discord.gg/brq79WQ"
// Forum               "http://www.vertshobbies.com/" 
// GitHub              "https://github.com/Vertabreak/Marlin"
// Thingivese          "https://www.thingiverse.com/Vertabreaker"
// Patreon             "https://www.patreon.com/vertabreaker" 
// Buy me a coffee     "https://ko-fi.com/vertabreaker"

//-------------------------------------------------------------------------------------------------------------------------
// Remove // to enable - add // to disable                                                                                |
// Complete steps below > build > flash, its that simple there are hundreds of combinations & more being added.           |
// Post flash initilize the eeprom - M502+M500 or menu option load failsafe+store settings.                               |
// Guides on my youtube, when you watch/like/subscribe/share it helps support the channel and helps it grow.              | 
// Looking for help or have a question? join us on the discord or post on the forums.                                     |
// Confirmed to compile with vscode and the platformio extension                                                          |
// Sometimes Vscode gets weird click the trashcan clean button and restart vscode it often fixs the issue.                |
// If you get a succeeded compile all "problems" should be ignored only a real error means anything.                      |  
//-------------------------------------------------------------------------------------------------------------------------

//(Step 1) enable 1 base model/frame
//GT2560 Boards - vscode: default_envs = mega2560 in platformio.ini
#define GTA10       // A10 & Variants
//#define GTA20       // A20 & Variants
//#define MECREATOR2  // Mecreator2 & Variants     
//#define I3PROA      // I3ProA & Variants
//#define I3PROB      // I3PROB & Variants
//#define I3PROC      // I3PROC & Variants
//#define I3PROW      // I3PROW & Variants
//#define I3PROX      // I3PROX & Variants

//GTM32 Boards - vscode: default_envs = STM32F103VE_GTM32 in platformio.ini
//#define GTA30       // A30 & Variants  - testing
//#define GTE180      // E180 & Variants - testing
//#define GTM201      // M201 & Variants - testing
//#define GTD200      // D200 & Variants - testing

//Melzi Boards - vscode: default_envs = melzi in platformio.ini
//#define ENDER3      // ENDER3 & Variants - testing 

//----------------------------------------------------------------------------------------------------
//Based on https://github.com/codiac2600/SKR-MK3s-V1.4-Beta
//SKR 1.4 Boards - vscode: default_envs = #default_envs = LPC1768
//Select 1 board
//#define BEAR        // Bear MK3/MK3s & Variants - tesing

//vscode: default_envs = #default_envs = LPC1769
//#define BEAR_TURBO  // Bear MK3/MK3s Turbo & Variants - testing

//(Bear & Bear_Turbo)Switch from Bear MK3 to MK2.5 
//#define MK25        // Enable to set 12V for Bear MK2.5

//(Bear & Bear_Turbo)Z mod pick only 1 or none for stock
//#define Z320        // Enable to change Zmax to 320  
//#define Z420        // Enable to change Zmax to 420 

//(Bear & Bear_Turbo)Extruder mod pick only 1 or none for stock
//#define BMG18       // BMG E 1.8 stepper
//#define BMG9        // BMG E 0.9 stepper

//(Bear & Bear_Turbo)XY mod pick only 1 or none for stock
//#define GREYBEAR    // XY 0.9 stepper

//-----------------------------------------------------------------------------------------------------

//(Step 2) enable 1 if you have mixing or multi extruder (Variant)
//#define MIX      // Enable Mixing    2 in 1 - 1 Virtual Stepper (M)
//#define MIXT     // Enable Mixing    3 in 1 - 1 Virtual Stepper (T)
//#define CYCLOPS  // Enable Cyclops   2 in 1 - 2 Physical Stepper (C) 
//#define CYCLOPST // Enable Cyclops   3 in 1 - 3 Physical Stepper (CT)
//#define DUALEX   // 2 Extruders      2 in 2 - 2 Physical Stepper (D) 
//#define TRIEX    // 3 Extruders      3 in 3 - 3 Physical Stepper (E3)

//---------------
//Hardware Mods |
//---------------

//(Driver Mods) enable 1 (MOD) driver type or none for (Stock)
//#define A5984      // Enable A5984   all drivers
//#define DRV8825    // Enable DRV8825 all drivers
//#define LV8729     // Enable LV8729  all drivers
//#define L6470      // Enable L6470   all drivers
//#define TB6560     // Enable TB6560  all drivers
//#define TB6600     // Enable TB6600  all drivers

//#define TMC2208S   // Enable TMC2208 Standalone all drivers
//#define TMC2209S   // Enable TMC2209 Standalone all drivers
//#define TMC2130S   // Enable TMC2130 Standalone all drivers
//#define TMC2160S   // Enable TMC2160 Standalone all drivers
//#define TMC26XS    // Enable TMC226X Standalone all drivers
//#define TMC2660S   // Enable TMC2660 Standalone all drivers
//#define TMC5130S   // Enable TMC5130 Standalone all drivers
//#define TMC5160S   // Enable TMC5160 Standalone all drivers

//#define TMC2208U   // Enable TMC2208 UART/SPI all drivers
//#define TMC2209U   // Enable TMC2209 UART/SPI all drivers
//#define TMC2130U   // Enable TMC2130 UART/SPI all drivers
//#define TMC2160U   // Enable TMC2160 UART/SPI all drivers
//#define TMC26XU    // Enable TMC226X UART/SPI all drivers
//#define TMC2660U   // Enable TMC2660 UART/SPI all drivers
//#define TMC5130U   // Enable TMC5130 UART/SPI all drivers
//#define TMC5160U   // Enable TMC5160 UART/SPI all drivers

//Custom driver set if none selected above
//#define CUSTOMDRIVERS     // Define Custom drivers 
#if ENABLED (CUSTOMDRIVERS)
   //'A4988', 'A5984', 'DRV8825', 'LV8729', 'L6470', 'L6474', 'POWERSTEP01', 'TB6560', 'TB6600', 'TMC2100', 'TMC2130', 'TMC2130_STANDALONE', 'TMC2160'
   //'TMC2160_STANDALONE', 'TMC2208', 'TMC2208_STANDALONE', 'TMC2209', 'TMC2209_STANDALONE', 'TMC26X', 'TMC26X_STANDALONE', 'TMC2660', 'TMC2660_STANDALONE'
   //'TMC5130', 'TMC5130_STANDALONE', 'TMC5160', 'TMC5160_STANDALONE'
   
 // Timings
   #define X_DRIVER_TYPE  TMC2208_STANDALONE
   #define Y_DRIVER_TYPE  TMC2208_STANDALONE
   #define Z_DRIVER_TYPE  TMC2208_STANDALONE
   #define E0_DRIVER_TYPE A4988
   #define E1_DRIVER_TYPE A4988
   #define E2_DRIVER_TYPE A4988

// Motor directions
   #define CUSTOMX true // true/false to change direction
   #define CUSTOMY true // true/false to change direction
   #define CUSTOMZ true // true/false to change direction
   #define INVERTE      // enable false / disabe true to change direction

#endif

//(Probe Mod) enable 1 (Mod) probe type none = manual (stock) - No GTM32 probe support yet
//#define TOUCHPROBE  // Enable Touch Type Probe (Bltouch / 3Dtouch)
//#define FMP         // Enable Fixed Mounted Type Probe (Capacitive / Inductive)
//#define PINDA       // Enable Pinda Type Probe

//Probe settings
//#define HEATERACCURACY   // Disable heaters while probing - May effect accuracy +-
//#define HALFSPEED        // Reduce probing speed by 50% = 120 - May effect accuracy +-
//#define DOUBLESPEED      // Raise probing speed by 100% = 480 - May effect accuracy +-

//(LCD Mod) enable 1 (Mod) to override default LCD as defined by step 1
//#define FULLGFXLCD      // Enable repreap full gfx lcd
//#define CR10DISPLAY     // Enable cr10 style lcd

//(Fan Mod) enable 1 (Mod) to override default FAN PWM
//#define MECHFAN     // Enable Mechatronics fan 80 pwm
//#define RADIALFAN   // Enable Radial fan 50 pwm
//#define BEAR_FAN    // Enable fan 20 pwm on when not a bear model 

//------------------------------
//Optional settings & features |
//------------------------------
//Note 1000bytes of ram should remain for system stability.

//Optional features
//#define PLR              // Enabled power loss resume - Only functions from SDcard
//#define RUNOUT           // Enable filament runout sensor - Only If you have them and want to use them
//#define BEDCLIPS         // Enable to avoid bed clips (manual or probe) - Only If you have them and want to use them
//#define CASELIGHT        // Enable case light menu if board has led header.

//Disable to save resources on hardware you dont use
//#define NOSCREEN         // Disable the screen - Save alot of resources good for octoprint users
//#define NOSDCARD         // Disable the sdcard slot - Save alot of resources good for octoprint users 

//Used to switch the default board of the model selected in step 1
//#define CUSTOMBOARD // Enable Custom Board
#if ENABLED (CUSTOMBOARD)
  #define MOTHERBOARD BOARD_BTT_SKR_V1_4  // CTRL+Click to jump to board list & also set the correct default_env in platfomio.ini
#endif

//Framework for adding a new printer to this config
//#define NEWMODEL // New model

//-----------------------------
//logic to reduce setup steps | 
//-----------------------------

//Multiextruder 
#if ANY(MIX, MIXT, CYCLOPS, CYCLOPST, DUALEX, TRIEX)
  #define MULTIEXTRUDER 
#endif

//TMC drivers
#if ANY(TMC2208S, TMC2209S, TMC2130S, TMC2160S, TMC26XS, TMC2660S, TMC5130S, TMC5160S) || ANY(TMC2208U, TMC2209U, TMC2130U, TMC2160U, TMC26XU, TMC2660U, TMC5130U, TMC5160U)
  #define TMCCHIPS
#endif

//Models with direct drive
#if ANY(MECREATOR2, I3PROA, I3PROB, I3PROC, I3PROW, I3PROX)
  #define DIRECTDRIVE
#endif

//Board types----------------------------------------------------------------------
//32bit boards models
#if ANY(GTA30, GTE180, GTM201, GTD200, BEAR, BREAR_TURBO, CUSTOMBOARD)
  #define MCU32
#endif

//256kb boards models
#if ANY(GTA10, GTA20, MECREATOR2, I3PROA, I3PROB, I3PROC, I3PROW, I3PROX)
  #define AT2560
#endif

//128kb or smaller boards models only if disabled
#if ENABLED (ENDER3) && DISABLED (CUSTOMBOARD)
  #define AT1280
#endif
//----------------------------------------------------------------------------------

#if DISABLED (AT1280)
 #define LINADV           // Enable linear advance.
 #define FANSCALING       // Enabled PID FAN SCALING
 #define EXTRUSIONSCALING // Enabled PID EXTRUSION SCALING
 #define ACTIONCOMMANDS   // Enable ACTION COMMANDS for use with octoprint
 #define MESHVALIDATE     // Enable G26 mesh validation does not work well in my testing
 #define PROGRESSINFO     // Enable print progress info display
#endif

//Bed clip logic - use mesh inset or min probe edge to avoid clips not both
#if ENABLED (BEDCLIPS)
  #define MESH_INSET 10   // Move mesh in #mm from edge
  //#define MESH_MIN_X MESH_INSET
  //#define MESH_MIN_Y MESH_INSET
  //#define MESH_MAX_X X_BED_SIZE - (MESH_INSET) - 40 //PROBE OFFSET X - Workaround for preventing exceeding Max X in some cases
  //#define MESH_MAX_Y Y_BED_SIZE - (MESH_INSET)
#else
  #define MESH_INSET 0    // Move mesh in #mm from edge
  //#define MESH_MIN_X MESH_INSET
  //#define MESH_MIN_Y MESH_INSET
  //#define MESH_MAX_X X_BED_SIZE - (MESH_INSET) - 40 //PROBE OFFSET X - Workaround for preventing exceeding Max X in some cases
  //#define MESH_MAX_Y Y_BED_SIZE - (MESH_INSET)
#endif

//Probe offset logic - suggest you mesure yours and adjust as needed. 
#if DISABLED (MULTIEXTRUDER) && ANY(TOUCHPROBE, FMP) && ANY (GTA10, GTA20)
  #define NOZZLE_TO_PROBE_OFFSET { -38, 5, 0 } // Nozzle To Probe offset XYZ A10/A20 - this is what it is on my test machines yours could differ 
#elif ENABLED (MULTIEXTRUDER) && ANY(TOUCHPROBE, FMP) && ANY (GTA10, GTA20)
  #define NOZZLE_TO_PROBE_OFFSET { -40, 0, 0 }  // Nozzle To Probe offset XYZ A10M+T/A20M+T - this is what it is on my test machines yours could differ
#elif ANY (BEAR, BEAR_TURBO) && ENABLED (TOUCHPROBE)
  #define NOZZLE_TO_PROBE_OFFSET { 26, 10, 0 } 
#elif ANY (BEAR, BEAR_TURBO)
  #define NOZZLE_TO_PROBE_OFFSET { 23, 5, 0 }  
#else
  #define NOZZLE_TO_PROBE_OFFSET { 0, 0, 0 }
#endif

//Bed offset logic - distance from endstop to bed, nozzle on front left bed edge should = X0 Y0
#if  ANY (GTA10, GTA20, GTA30) && ANY(MIXT, CYCLOPST, CYCLOPST)
  #define X_MIN_POS -1   //- this is what it is on my test machines yours could differ
  #define Y_MIN_POS -7   //- this is what it is on my test machines yours could differ
#elif ANY (GTA10, GTA20, GTA30)
  #define X_MIN_POS -10  //- this is what it is on my test machines yours could differ
  #define Y_MIN_POS -5   //- this is what it is on my test machines yours could differ
#elif ANY (BEAR, BEAR_TURBO)
  #define X_MIN_POS 0
  #define Y_MIN_POS -4  
#elif ENABLED (NEWMODEL) 
  #define X_MIN_POS 0        
  #define Y_MIN_POS 0  
#else
  #define X_MIN_POS 0        
  #define Y_MIN_POS 0      
#endif

//Steps selection logic
#if DISABLED (NEWMODEL)
#if DISABLED (MULTIEXTRUDER) && DISABLED (BEAR) && DISABLED (BEAR_TURBO)
  #define DEFAULT_AXIS_STEPS_PER_UNIT  { 80, 80, 400, 95 }  // ungeared extruder found on a10/a20/a30/i3pro
  //#define DEFAULT_AXIS_STEPS_PER_UNIT   { 80, 80, 800, 95 } 
  //#define DEFAULT_AXIS_STEPS_PER_UNIT   { 80, 80, 2560, 95 } // M8 Z rod steps 2560 found on old I3pro
#elif ENABLED (MULTIEXTRUDER) && DISABLED (BEAR) && DISABLED (BEAR_TURBO)
  #define DEFAULT_AXIS_STEPS_PER_UNIT  { 80, 80, 400, 430 } // geared extruder found on M & T variants
  //#define DEFAULT_AXIS_STEPS_PER_UNIT  { 80, 80, 800, 430 } 
  //#define DEFAULT_AXIS_STEPS_PER_UNIT  { 80, 80, 2560, 430 } // M8 Z rod steps 2560 found on old I3pro 
#endif

#if ENABLED (GREYBEAR) && ENABLED (BMG18) && ANY (BEAR, BEAR_TURBO) 
    #define DEFAULT_AXIS_STEPS_PER_UNIT   { 200, 200, 400, 830 }  // BMG1.8 + grey
#elif ENABLED (BMG18) && ANY (BEAR, BEAR_TURBO)  
    #define DEFAULT_AXIS_STEPS_PER_UNIT   { 100, 100, 400, 830 }  // BMG1.8
#elif ENABLED (GREYBEAR) && ENABLED (BMG9) && ANY (BEAR, BEAR_TURBO)
    #define DEFAULT_AXIS_STEPS_PER_UNIT   { 200, 200, 400, 1660 }  // BMG 0.9 +grey
#elif ENABLED (BMG9) && ANY (BEAR, BEAR_TURBO) 
    #define DEFAULT_AXIS_STEPS_PER_UNIT   { 100, 100, 400, 1660 }  // BMG 0.9
#elif ENABLED (GREYBEAR) && ANY (BEAR, BEAR_TURBO) 
    #define DEFAULT_AXIS_STEPS_PER_UNIT   { 200, 200, 400, 280 }  // stock + grey  
#elif ANY (BEAR, BEAR_TURBO) 
    #define DEFAULT_AXIS_STEPS_PER_UNIT   { 100, 100, 400, 280 }  //stock
#endif
#endif

#if ENABLED (NEWMODEL) 
  #define DEFAULT_AXIS_STEPS_PER_UNIT  { 80, 80, 400, 95 }
#endif

//Motor direction logic
#if ENABLED (TMCCHIPS) && DISABLED (MULTIEXTRUDER) || DISABLED (TMCCHIPS) && ENABLED (MULTIEXTRUDER)
  #define INVERTE     // Invert E direction disabe if wrong direction - Geared exturders invert E (stock)
#else
  //#define INVERTE  // Enable to force on
#endif 

#if ENABLED (TMCCHIPS)
  #define INVERTXYZ   // Invert XYZ direction disable if wrong direction.
#else
  //#define INVERTXYZ // Enable to force on 
#endif
