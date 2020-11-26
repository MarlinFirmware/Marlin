/*==========================================================================
*=================== FLSun QQS - DELTA Printer =============================
*================= With pins_FLSUN_HiSPEED.h BOARD =========================
*===========================================================================
*
* For a Delta printer start with one of the configuration files in the
* config/examples/delta/FLSUN/ directory and customize for your machine.
*
* TIPS:
* -For reduce binary size : https://thborges.github.io/blog/marlin/2019/01/07/reducing-marlin-binary-size.html
* 
* -For NeoPixel use library : https://github.com/Foxies-CSTL/Nano-NeoPixel-Lib/archive/master.zip,
* and active it into the part of platformio.ini "env:flsun_hispeed",
* and commented error in SanityCheck.h for pass error check.
*
* -For 2209 change TMC2208 by TMC2209 at the bottom file.
*/

//========= Hardware ==========//
/*------Drivers-(1 CHOICE)-----*/
#define QQS                        //(S) For 4xA4988(green or red color)
//#define QQS_TMC                    //(8) For 4xTMC220x_STANDALONE For 2208(white color) or 2209(black color)

/* MODE UART XYZ */
//#define QQS_UARTx                  //(U8/U9) 4xTMC220x Note: For 2209 change TMC2208 by TMC2209 at the bottom file and remove on your printer the module WIFI.
//#define QQS_UART9                  //(UH) Mode special 2209 wiring with one I/O pin (Remove module ESP12)

/*------- Choice Other driver for EXTRUDER-------//
* Options: 
* LV8729/A4988/TMC2208_STANDALONE/TMC2209_STANDALONE/TMC2208/TMC2209 
*/
//#define DRIVER_EXT A4988

/* QQS Stock have a clone TITAN EXtruder,
* also if you have another try this.
* (T=397)/(B=417)/(b=141)
*/
//#define INV_EXT                    //(T) Uncommment to reverse direction.
//#define BMG                        //(B) Uncomment to change Extruder step(417).
//#define Mini                        //(b) Uncomment BMG&Mini to change Extruder step(141).

//#define FLYING                     //(Y) Uncomment to change Extruder flying.

/*-------Driver TFT Color--(1 CHOICE)-----*/
#define MKS_ROBIN_TFT32            //Mks_Robin_TFTV_2.0
//#define MKS_ROBIN_TFT28            //Mks_Robin_TFT
//#define MKS_ROBIN_TFT_V1_1R
//#define MKS_ROBIN_TFT24
//#define TFT_GENERIC

/*--- Choice UI TFT ----*/
//#define TFT_CLASSIC_UI             //(F) UI STANDARD 
#define TFT_COLOR_UI               //(C) UI MARLIN (too big with mode UART+UBL=ok with nanolib)

/*----  Modules -----*/
#define ESP_WIFI                   //(W) Module ESP8266/ESP12

/*For LedStrip which need an external power source on Vcc pin.*/
//#define NEOPIXEL_LED               //(N) Use port GPIO Wifi module (PA10/PA9/PA8/PC7)

//Many options for Modules: 
#define POWER_LOSS_RECOVERY        //NC LVGL pb SD
#define FILAMENT_RUNOUT_SENSOR     //NC LVGL
#define ADVANCED_PAUSE_FEATURE     //NC LVGL
#define LIN_ADVANCE                //(L) Possible Bug with BabyStep.For TMC_UART prefer mode spreadCycle         
#define ARC_SUPPORT                //(R)

//============= End_Hardware ===============//

//Choice add menu: (OPT)
//#define DELTA_CALIBRATION_MENU     //auto for CLASSIC and COLOR (NC LVGL)
#define SOFT_ENDSTOPS_MENU_ITEM    // for UI CLASSIC and UI COLOR
#define PID_EDIT_MENU              //
#define PID_AUTOTUNE_MENU          //
#define PAUSE_BEFORE_DEPLOY_STOW   //Message Stow/remove Probe.
#define LCD_INFO_MENU              // Informations printer.

//  Type Calibration (CAL)
#define AUTO_BED_LEVELING_BILINEAR //(A)
//#define AUTO_BED_LEVELING_UBL      //(U)
//#define LEVEL_BED_CORNERS
#define PROBE_OFFSET_WIZARD

// Option for Octoprint (OCTO)
//#define HOST_ACTION_COMMANDS       // Action Command Prompt support Message on Octoprint
//#define UTF_FILENAME_SUPPORT      // Bug at the reboot
//#define CANCEL_OBJECTS

/* OPTION no validate */
//#define USE_CONTROLLER_FAN         //BOARD FAN
//EXTRUDER_AUTO_FAN   //

//
//==================Part for Driver defintions=============//
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

//Set for A4988 
#if ANY(QQS, STOCK)
    #define DRIVER_AXES A4988
    #ifndef DRIVER_EXT
      #define DRIVER_EXT A4988
    #endif
#endif

//For set 2209 change TMC2208 by TMC2209 
#ifdef QQS_TMC
    #define DRIVER_AXES TMC2208_STANDALONE
    #ifndef DRIVER_EXT
      #define DRIVER_EXT TMC2208_STANDALONE
    #endif
#endif

// Software Serial UART for TMC 2208 / TMC 2209
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
// and proper jumper configuration. Uses I/O pins
// like PA10/PA9/PC7/PA8 only.
#ifdef QQS_UART9
    #define QQS_UART
    #define HARDWARE_SERIAL
    #define DRIVER_AXES TMC2209
    #ifndef DRIVER_EXT
      #define DRIVER_EXT TMC2209
    #endif
#endif

