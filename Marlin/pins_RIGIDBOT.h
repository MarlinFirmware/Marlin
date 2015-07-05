/**
 * RIGIDBOT (Arduino Mega) pin assignments
 */

#include "pins_RAMPS_13.h"



#define CONTROLLERFAN_PIN         4 //Pin used for the fan to cool motherboard (-1 to disable) //Fans/Water Pump to cool the hotend cool side.
//#define EXTRUDER_0_AUTO_FAN_PIN   4
//#define EXTRUDER_1_AUTO_FAN_PIN   5
//#define EXTRUDER_2_AUTO_FAN_PIN   -1
//#define EXTRUDER_3_AUTO_FAN_PIN   -1

#ifdef Z_PROBE_ENDSTOP
  #define Z_PROBE_PIN      19
#endif

#undef HEATER_0_PIN
#undef HEATER_1_PIN
//#undef HEATER_2_PIN
//#undef HEATER_3_PIN

#define HEATER_0_PIN        9    // EXTRUDER 1
#define HEATER_1_PIN       12    // EXTRUDER 2 (FAN On Sprinter)
//#define HEATER_2_PIN       -1
//#define HEATER_3_PIN       -1

#undef TEMP_0_PIN
#undef TEMP_1_PIN
//#undef TEMP_2_PIN

#define TEMP_0_PIN         14   // ANALOG NUMBERING
#define TEMP_1_PIN         13   // ANALOG NUMBERING
//#define TEMP_2_PIN         -1   // ANALOG NUMBERING

#undef SERVO0_PIN
#undef SERVO1_PIN
#undef SERVO2_PIN
#undef SERVO3_PIN

//
//These Servo pins are for when they are defined. Tested for usage with bed leveling
//on a Delta with 1 servo. Running through the Z servo endstop in code. 
//Physical wire attachment was done on EXT1 on the GND, 5V, and D47 pins.
//
#ifdef NUM_SERVOS
  #define SERVO0_PIN         11
  #if NUM_SERVOS > 1
    #define SERVO1_PIN        6
  #endif
  #if NUM_SERVOS > 2
    #define SERVO2_PIN        5
  #endif
  #if NUM_SERVOS > 3
    #define SERVO3_PIN        4
  #endif
#endif


// LCD Panel options for the RigidBoard
#undef  BEEPER

#undef LCD_PINS_RS
#undef LCD_PINS_ENABLE
#undef LCD_PINS_D4
#undef LCD_PINS_D5
#undef LCD_PINS_D6
#undef LCD_PINS_D7

#define LCD_PINS_RS          16
#define LCD_PINS_ENABLE      17
#define LCD_PINS_D4          23
#define LCD_PINS_D5          25
#define LCD_PINS_D6          27
#define LCD_PINS_D7          29

#ifdef REPRAP_DISCOUNT_SMART_CONTROLLER
  #define BEEPER -1

  #undef  BTN_EN1
  #undef  BTN_EN2
  #undef  BTN_ENC
  #define BTN_EN1 33
  #define BTN_EN2 31
  #define BTN_ENC 35

  #undef  SDCARDDETECT
  #define SDCARDDETECT 22

  #undef  KILL_PIN
  #define KILL_PIN 32

#elif defined(RIGIDPANEL)
  #define BEEPER -1

  #define BTN_UP  37
  #define BTN_DWN 35
  #define BTN_LFT 33
  #define BTN_RT  32

  #undef  BTN_EN1
  #undef  BTN_EN2
  #undef  BTN_ENC
  #define BTN_EN1 -1
  #define BTN_EN2 -1
  #define BTN_ENC 31

  #undef  SDCARDDETECT
  #define SDCARDDETECT 22
  
  #undef  KILL_PIN
  #define KILL_PIN -1

#else
  #define BEEPER 33  // Beeper on AUX-4
  #define KILL_PIN -1

  //buttons are directly attached using AUX-2
  #ifdef REPRAPWORLD_KEYPAD
    #define BTN_EN1 64 // encoder
    #define BTN_EN2 59 // encoder
    #define BTN_ENC 63 // enter button
    #define SHIFT_OUT 40 // shift register
    #define SHIFT_CLK 44 // shift register
    #define SHIFT_LD 42 // shift register
  #else
    #define BTN_UP  37
    #define BTN_DWN 35
    #define BTN_LFT 33
    #define BTN_RT  32
    #define BTN_ENT 31
    //#define BTN_EN1 37
    //#define BTN_EN2 35
    //#define BTN_ENC 31  //the click
  #endif

  #ifdef G3D_PANEL
    #define SDCARDDETECT 49
  #else
    #define SDCARDDETECT -1  // Ramps does not use this port
  #endif

#endif


// SPI for Max6675 Thermocouple
#ifndef SDSUPPORT
  #define MAX6675_SS       53 // Do not use pin 53 if there is even the remote possibility of using Display/SD card
#else
  #define MAX6675_SS       49 // Do not use pin 49 as this is tied to the switch inside the SD card socket to detect if there is an SD card present
#endif

#ifndef SDSUPPORT
  // these pins are defined in the SD library if building with SD support
  #define SCK_PIN          52
  #define MISO_PIN         50
  #define MOSI_PIN         51
#endif



/****************************************************************************************
* RigidBot Arduino Mega pin assignment
*
****************************************************************************************/
//#define X_STEP_PIN         54
//#define X_DIR_PIN          55
//#define X_ENABLE_PIN       38
//#define X_MIN_PIN           3
//#define X_MAX_PIN           2

//#define Y_STEP_PIN         60
//#define Y_DIR_PIN          61
//#define Y_ENABLE_PIN       56
//#define Y_MIN_PIN          14
//#define Y_MAX_PIN          15
 
//#define Z_STEP_PIN         46
//#define Z_DIR_PIN          48
//#define Z_ENABLE_PIN       62
//#define Z_MIN_PIN          18
//#define Z_MAX_PIN          19
//#define Z_PROBE_PIN        -1

//#define Y2_STEP_PIN        36  // Not supported by Rigidbot
//#define Y2_DIR_PIN         34  // Not supported by Rigidbot
//#define Y2_ENABLE_PIN      30  // Not supported by Rigidbot

//#define Z2_STEP_PIN        36
//#define Z2_DIR_PIN         34
//#define Z2_ENABLE_PIN      30

#undef E0_STEP_PIN
#undef E0_DIR_PIN
#undef E0_ENABLE_PIN

#define E0_STEP_PIN        36
#define E0_DIR_PIN         34
#define E0_ENABLE_PIN      30

#undef E1_STEP_PIN
#undef E1_DIR_PIN
#undef E1_ENABLE_PIN

#define E1_STEP_PIN        26
#define E1_DIR_PIN         28
#define E1_ENABLE_PIN      24

//#define SDPOWER            -1
//#define SDSS               53
//#define LED_PIN            13

#undef  FAN_PIN
#define FAN_PIN             8 //11 // IO pin. Buffer needed

#undef  PS_ON_PIN
#define PS_ON_PIN          -1

#undef  HEATER_BED_PIN
#define HEATER_BED_PIN     10   // BED

#undef  TEMP_BED_PIN
#define TEMP_BED_PIN       15   // ANALOG NUMBERING
 

#define STEPPER_RESET_PIN 41 // #define STEPPER_RESET_FIX in Configuration.h to use this during boot
