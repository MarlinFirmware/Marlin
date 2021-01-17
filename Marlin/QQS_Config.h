/*==========================================================================
*=================== FLSun QQS - DELTA Printer =============================
*================= With pins_FLSUN_HiSPEED.h BOARD =========================
*===========================================================================
*
* For a Delta printer start with one of the configuration files in the
* config/examples/delta/FLSUN/ directory and customize for your machine.
*
* TIPS:
* -For NeoPixel use special library(v1.4.2)and commented error in SanityCheck.h (src/HAL/STM32F1/inc) for pass error check.
* -For 2209 change TMC2208 by TMC2209 at the bottom file.
*/
//#define XP
//========= Hardware ==========//
/*-------Motherboard-----------*/
#define STOCK                       // env = hispeed (Default)

/*------Drivers-(1 CHOICE)-----*/
#define QQS                        //(S) For 4xA4988(green or red color) (Default)
//#define QQS_TMC                    //(8) For 4xTMC220x_STANDALONE For 2208(white color) or 2209(black color)

/* MODE UART XYZ */
//#define QQS_UARTx                  //(U8/U9) 4xTMC220x Note: For 2209 change TMC2208 by TMC2209 at the bottom file and remove on your printer the module WIFI.
//#define QQS_UART9                  //(UH) Mode special 2209 wiring with one I/O pin (Remove module ESP12)

/**===============================================================
 * -- If you have an other stepper driver for EXTRUDER----------//
 * Options: =====================================================
 * A4988/DRV8825/LV8729/...
 * TMC2208_STANDALONE/TMC2209_STANDALONE/TMC2208/TMC2209. 
 */
//#define DRIVER_EXT A4988

/*--- Choice UI TFT ----*/
#define TFT_COLOR_UI               //(C) UI MARLIN (Default)
//#define TFT_CLASSIC_UI             //(F) UI STANDARD 

/*----  Modules -----*/
#define ESP_WIFI                   //(W) Module ESP8266/ESP12 (Default)
/*For LedStrip which need an external power source on Vcc pin.*/
//#define NEOPIXEL_LED               //(N) Use port GPIO Wifi module (PC7)

/**=========================================================
 * =========================================================
 * = QQS Stock have a clone TITAN EXtruder(Default),========
 * = If you have another choose by uncomment your extruder.=
 * = Also you can change the direction.=====================
 * = Default eStep (T=397)/(B=417)/(b=141)==================
 * =========================================================
 */
//#define INV_EXT                    // Uncommment to reverse direction for BMG/BMGmini.

// BMG Extruder (B) Extruder step(417).
//#define EXTRUDER_STEPS 417         //(B) Uncommment for BMG Left/Right.

// BMG Mini (b) Extruder step(141).
//#define EXTRUDER_STEPS 141         //(b) Uncommment for BMG mini.

/** ==============================
 * ======= MODE LEVELING==========
 * == type Calibration UBL or ABL=
 * ========= 1 CHOICE)============
 * ===============================
 */
//#define DELTA_HOME_TO_SAFE_ZONE    // Option to move down after homing to a height where XYZ movement is unconstrained.
#define PREHEAT_BEFORE_LEVELING    //(P) Run a PreHeat bed at 50°C (Default)
#define AUTO_BED_LEVELING_UBL      //(U) (Default)
//#define AUTO_BED_LEVELING_BILINEAR //(A)

//Many options for Modules: 
#define LIN_ADVANCE                //(L) For TMC_UART prefer mode spreadCycle(by TFT menu) (Default)         
#define ARC_SUPPORT                //(R) (Default)

/** =============================
 * =====Type of TFT screen ======
 * = Driver TFT Color (1 CHOICE)=
 * ==============================
 */
#define MKS_ROBIN_TFT32            // Mks_Robin_TFT_V2.0 (Default)
//#define TFT_GENERIC                // For the user who haven't the same screen.

//#define FLYING                     //(Y) Uncomment to change Extruder flying (You must modified the parameters of DELTA part)

//============= End_Hardware ===============//


/** =====================================================
 *  ==== For users who dont have a terminal ============= 
 * = like (Prontoface/Octoprint/HostRepertier/Astoprint)=
 * ====== Choice add menu on TFT: (OPT) =================
 */
#define LCD_INFO_MENU              // Informations printer (Default).
#define DELTA_CALIBRATION_MENU     // auto for CLASSIC and COLOR (Default).
#define PID_EDIT_MENU              // tune PID Bed and Nozzle (Default).
#define PID_AUTOTUNE_MENU          // tune auto PID (Default).

// ---Expe tools
//#define LEVEL_BED_CORNERS

// Option for Octoprint (OCTO)
//#define HOST_ACTION_COMMANDS       // Action Command Prompt support Message on Octoprint
//#define CANCEL_OBJECTS             // Add menu "Cancel Objet"

/* OPTION no validate */
//#define USE_CONTROLLER_FAN         //BOARD FAN
//EXTRUDER_AUTO_FAN                 //

/**==================================
 * ===Part for Hardware defintions===
 * ==================================
 */
// Options for Modules Hardware
#ifdef ESP_WIFI
  #define BINARY_FILE_TRANSFER       // Bin transfert for host like ESP3D or others.
#endif
#ifdef NEOPIXEL_LED
  #define LED_CONTROL_MENU           // To control LedStrip.
#endif

//TFT Type For TFT_GENERIC
#if ENABLED(TFT_GENERIC)
  #define TFT_DRIVER AUTO
  #define TFT_INTERFACE_FSMC
  #define TFT_RES_320x240
#endif

// Variables to calculate steps and current
#ifndef EXTRUDER_STEPS
  #define EXTRUDER_STEPS 397  //TITAN(Default)
#endif
// TMC
#ifndef XYZ_CURRENT
  #define XYZ_CURRENT       900
#endif
#ifndef XYZ_CURRENT_HOME
  #define XYZ_CURRENT_HOME  800
#endif  
 #ifndef E_CURRENT
  #define E_CURRENT         850
#endif

// Set for A4988 
#ifdef QQS
    #define DRIVER_AXES A4988
    #ifndef DRIVER_EXT
      #define DRIVER_EXT A4988
    #endif
#endif

// For set 2209 change TMC2208 by TMC2209 
#ifdef QQS_TMC
    #define DRIVER_AXES TMC2208_STANDALONE
    #ifndef DRIVER_EXT
      #define DRIVER_EXT TMC2208_STANDALONE
    #endif
#endif

// Software Serial UART for TMC 2208/TMC 2209
#ifdef QQS_UARTx
    #define QQS_UART
    #define DRIVER_AXES TMC2208
    #ifndef DRIVER_EXT
      #define DRIVER_EXT TMC2208
    #endif
#endif

// Note:
// HardwareSerial with one pins for four drivers
// Compatible with TMC2209. Provides best performance.
// Requires SLAVE_ADDRESS definitions in Configuration_adv.h
// and proper jumper configuration. Uses I/O pins PA8(Default).
#ifdef QQS_UART9
    #define QQS_UART
    #define HARDWARE_SERIAL
    #define DRIVER_AXES TMC2209
    #ifndef DRIVER_EXT
      #define DRIVER_EXT TMC2209
    #endif
#endif
