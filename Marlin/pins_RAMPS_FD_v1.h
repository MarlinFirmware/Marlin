
/****************************************************************************************
*
* pin assignment for Arduino Due with RAMPS-FD V1 
* 
****************************************************************************************/

//
#ifndef ARDUINO_ARCH_SAM
 #error Oops!  Make sure you have 'Arduino Due' selected from the 'Tools -> Boards' menu.
#endif


// v1
  #define RAMPS_FD_V1
  #define INVERTED_HEATER_PINS
  // No EEPROM
  // Use 4k7 thermistor tables
  
// TODO: v2
  //#define RAMPS_FD_V2
  // EEPROM supported
  // Use 1k thermistor tables


#define X_STEP_PIN         63
#define X_DIR_PIN          62
#define X_ENABLE_PIN       48
#define X_MIN_PIN          22
#define X_MAX_PIN           -1   //2 //Max endstops default to disabled "-1", set to commented value to enable.

#define Y_STEP_PIN         65
#define Y_DIR_PIN          64
#define Y_ENABLE_PIN       46
#define Y_MIN_PIN          24
#define Y_MAX_PIN          -1   //15

#define Z_STEP_PIN         67
#define Z_DIR_PIN          66
#define Z_ENABLE_PIN       44
#define Z_MIN_PIN          26
#define Z_MAX_PIN          -1

#define E0_STEP_PIN        36
#define E0_DIR_PIN         28
#define E0_ENABLE_PIN      42

#define E1_STEP_PIN        43
#define E1_DIR_PIN         41
#define E1_ENABLE_PIN      39

#define E2_STEP_PIN        32
#define E2_DIR_PIN         47
#define E2_ENABLE_PIN      45

#define SDPOWER            -1
#define SDSS               4
#define LED_PIN            13

#define BEEPER_PIN         -1

#define FAN_PIN            -1

#define CONTROLLERFAN_PIN  -1 //Pin used for the fan to cool controller

#define PS_ON_PIN          -1

#define KILL_PIN           -1


#define HEATER_BED_PIN     8    // BED

#define HEATER_0_PIN       9
#define HEATER_1_PIN       10
#define HEATER_2_PIN       11

#define TEMP_BED_PIN       0   // ANALOG NUMBERING

#define TEMP_0_PIN         1   // ANALOG NUMBERING
#define TEMP_1_PIN         -1  // 2    // ANALOG NUMBERING
#define TEMP_2_PIN         -1  // 3     // ANALOG NUMBERING

#define TEMP_3_PIN         -1   // ANALOG NUMBERING
#define TEMP_4_PIN         -1   // ANALOG NUMBERING



  #ifdef NUM_SERVOS
    #define SERVO0_PIN         11

    #if NUM_SERVOS > 1
      #define SERVO1_PIN         6
    #endif

    #if NUM_SERVOS > 2
      #define SERVO2_PIN         5
    #endif

    #if NUM_SERVOS > 3
      #define SERVO3_PIN         4
    #endif
  #endif


  #ifdef ULTRA_LCD

    #ifdef NEWPANEL
      // ramps-fd lcd adaptor
      #define LCD_PINS_RS 16
      #define LCD_PINS_ENABLE 17
      #define LCD_PINS_D4 23
      #define LCD_PINS_D5 25
      #define LCD_PINS_D6 27
      #define LCD_PINS_D7 29

      #ifdef REPRAP_DISCOUNT_SMART_CONTROLLER
        #define BEEPER_PIN 37

        #define BTN_EN1 33
        #define BTN_EN2 31
        #define BTN_ENC 35

        #define SDCARDDETECT 49
        #endif

      #endif

  #endif //ULTRA_LCD


// SPI for Max6675 Thermocouple

#ifndef SDSUPPORT
// these pins are defined in the SD library if building with SD support
  #define MAX_SCK_PIN          52
  #define MAX_MISO_PIN         50
  #define MAX_MOSI_PIN         51
  #define MAX6675_SS       53
#else
  #define MAX6675_SS       49
#endif

