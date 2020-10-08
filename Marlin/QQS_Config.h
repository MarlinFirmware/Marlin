//===========================================================================
//=================== FLSun QQS - DELTA Printer =============================
//================= With pins_FLSUN_HiSPEED.h BOARD =========================
//===========================================================================
// For a Delta printer start with one of the configuration files in the
// config/examples/delta directory and customize for your machine.
// TIPS:
// For reduce binary size : https://thborges.github.io/blog/marlin/2019/01/07/reducing-marlin-binary-size.html
// For NeoPixel use library : https://github.com/................/archive/master.zip
// and commented error in SanityCheck.h
//========= Hardware ==========
//#define STOCK

/*------Drivers-(1 CHOICE)-----*/
<<<<<<< Updated upstream
#define QQS               //(S) 4xA4988
//#define QQS_TMC           //(8) 4xTMC220x For 2208 or 2209

/* MODE UART XYZ*/
//#define QQS_UART8         //(U8) 4xTMC2208 (Remove module ESP12)
//#define QQS_UART9         //(U9) 4xTMC2209 (Remove module ESP12)

/*------- Choice Other driver for EXTRUDER-------*/
//#define DRIVER_EXT  TMC2208_STANDALONE

// Set Software Serial UART for TMC 2208 / TMC 2209
//#define TMC_SOFTWARE_SERIAL                             
=======
//#define QQS               //(S) 4xA4988
//#define QQS_TMC           //(8) 4xTMC220x For 2208 or 2209

/* MODE UART XYZ*/
#define QQS_UART8           //(U8) 4xTMC2208 (Remove module ESP12)
//#define QQS_UART9         //(U9) 4xTMC2209 (Remove module ESP12)

/*------- Choice Other driver for EXTRUDER-------*/
#define DRIVER_EXT  TMC2208_STANDALONE

// Set Software Serial UART for TMC 2208 / TMC 2209
>>>>>>> Stashed changes
#ifdef QQS_UART8
    #define QQS_UART
    #define SOFTWARE_SERIAL
    #define DRIVER_AXES TMC2208
    #ifndef DRIVER_EXT
      #define DRIVER_EXT TMC2208
    #endif
#endif

// Set Hardware Serial UART only TMC 2209
<<<<<<< Updated upstream
//#define TMC_HARDWARE_SERIAL                             
=======
>>>>>>> Stashed changes
#ifdef QQS_UART9
    #define QQS_UART
    #define HARDWARE_SERIAL
    #define DRIVER_AXES TMC2209
    #ifndef DRIVER_EXT
      #define DRIVER_EXT TMC2209
    #endif
#endif


/*----  Modules -----*/
<<<<<<< Updated upstream
#define ESP_WIFI               //(W) Module ESP8266/ESP12
//#define ESP3D_WIFISUPPORT      //(W)
//#define BMG                    //(B) Extruder
//#define NEOPIXEL_LED           //(N) Use port GPIO Wifi module (PA10/PA9/PA8/PC7)
=======
//#define ESP_WIFI               //(W) Module ESP8266/ESP12
//#define ESP3D_WIFISUPPORT      //(W)
//#define BMG                    //(B) Extruder
#define NEOPIXEL_LED             //(N) Use port GPIO Wifi module (PA10/PA9/PA8/PC7)
>>>>>>> Stashed changes


//Many options for Modules: 
#define POWER_LOSS_RECOVERY       //NC LVGL pb SD
#define FILAMENT_RUNOUT_SENSOR    //NC LVGL
#define ADVANCED_PAUSE_FEATURE    //NC LVGL
#define LIN_ADVANCE               //(L) Possible Bug with BabyStep.For TMC_UART prefer mode spreadCycle         

/*-------Screen Mks_Robin_TFT_v2---(FSMC)-----*/
//Choice UI TFT 
<<<<<<< Updated upstream
//#define FSMC_GRAPHICAL_TFT      //(F) UI STANDARD 
#define TFT_320x240             //(C) UI MARLIN (too big with mode UART+UBL=ok with nanolib)
//#define TFT_LVGL_UI_FSMC        //(I) UI MKS  => (Bug)
=======
//#define FSMC_GRAPHICAL_TFT    //(F) UI STANDARD 
#define TFT_320x240             //(C) UI MARLIN (too big with mode UART+UBL=ok with nanolib)
//#define TFT_LVGL_UI_FSMC      //(I) UI MKS  => (Bug)
>>>>>>> Stashed changes

//============= End_Hardware ===============//

//Choice menu: (OPT)
#define DELTA_CALIBRATION_MENU        //NC LVGL
<<<<<<< Updated upstream
#define PID_EDIT_MENU                 //
#define PID_AUTOTUNE_MENU             //
//#define PAUSE_BEFORE_DEPLOY_STOW      //Message Stow/remove Probe (bug Octoprint & UI Marlin)
=======
#define PID_EDIT_MENU              //
#define PID_AUTOTUNE_MENU          //
#define PAUSE_BEFORE_DEPLOY_STOW      //Message Stow/remove Probe (bug Octoprint & UI Marlin)
>>>>>>> Stashed changes
#define LCD_INFO_MENU                 //
//#define LED_CONTROL_MENU              // For LedStrip

//  Type Calibration (CAL)
//#define AUTO_BED_LEVELING_BILINEAR  //(A)
#define AUTO_BED_LEVELING_UBL         //(U) 

// Option for Octoprint (OCTO)
#define HOST_ACTION_COMMANDS        // Action Command Prompt support Message on Octoprint
//#define BINARY_FILE_TRANSFER        // Bin transfert

//#define USE_CONTROLLER_FAN          //BOARD FAN
//EXTRUDER_AUTO_FAN   //
//

