/**
 * Sanguinololu board pin assignments
 */

#if !defined(__AVR_ATmega644P__) && !defined(__AVR_ATmega1284P__)
  #error Oops!  Make sure you have 'Sanguino' selected from the 'Tools -> Boards' menu.
#endif

#define X_STEP_PIN         15
#define X_DIR_PIN          21
#define X_STOP_PIN         18

#define Y_STEP_PIN         22
#define Y_DIR_PIN          23
#define Y_STOP_PIN         19

#define Z_STEP_PIN         3
#define Z_DIR_PIN          2
#define Z_STOP_PIN         20

#define E0_STEP_PIN         1
#define E0_DIR_PIN          0

#define LED_PIN            -1

#define FAN_PIN            -1

// #if FAN_PIN == 12 || FAN_PIN ==13
//   #define FAN_SOFT_PWM
// #endif

#if MB(MELZI)
  #define LED_PIN            27 /* On some broken versions of the Sanguino libraries the pin definitions are wrong, which then needs LED_PIN as pin 28. But you better upgrade your Sanguino libraries! See #368. */
  #define FAN_PIN            4 // Works for Panelolu2 too
#endif

#if MB(STB_11)
  #define FAN_PIN            4
  //  Uncomment this if you have the first generation (V1.10) of STBs board
  #define LCD_PIN_BL         17 // LCD backlight LED
#endif

#if MB(AZTEEG_X1)
  #define FAN_PIN            4
#endif

#ifdef NUM_SERVOS
  #define SERVO0_PIN          -1

  #if NUM_SERVOS > 1
    #define SERVO1_PIN        -1
  #endif

  #if NUM_SERVOS > 2
    #define SERVO2_PIN        -1
  #endif

  #if NUM_SERVOS > 3
    #define SERVO3_PIN        -1
  #endif
#endif

#define PS_ON_PIN          -1
#define KILL_PIN           -1

#define HEATER_0_PIN       13 // (extruder)
#define HEATER_1_PIN       -1
#define HEATER_2_PIN       -1

#ifdef SANGUINOLOLU_V_1_2

  #define HEATER_BED_PIN     12 // (bed)
  #define X_ENABLE_PIN       14
  #define Y_ENABLE_PIN       14
  #define Z_ENABLE_PIN       26
  #define E0_ENABLE_PIN      14

  #ifdef LCD_I2C_PANELOLU2
    #define FAN_PIN          4 // Uses Transistor1 (PWM) on Panelolu2's Sanguino Adapter Board to drive the fan
  #endif

#else

  #define HEATER_BED_PIN      14  // (bed)
  #define X_ENABLE_PIN       -1
  #define Y_ENABLE_PIN       -1
  #define Z_ENABLE_PIN       -1
  #define E0_ENABLE_PIN      -1

#endif

#define TEMP_0_PIN          7   // MUST USE ANALOG INPUT NUMBERING NOT DIGITAL OUTPUT NUMBERING!!!!!!!!! (pin 33 extruder)
#define TEMP_1_PIN         -1
#define TEMP_2_PIN         -1
#define TEMP_BED_PIN        6   // MUST USE ANALOG INPUT NUMBERING NOT DIGITAL OUTPUT NUMBERING!!!!!!!!! (pin 34 bed)
#define SDPOWER            -1
#define SDSS               31

/* On some broken versions of the Sanguino libraries the pin definitions are wrong, which then needs SDSS as pin 24. But you better upgrade your Sanguino libraries! See #368. */
//#define SDSS               24

#ifdef ULTRA_LCD
 #ifdef NEWPANEL
   //we have no buzzer installed
   #define BEEPER -1
   //LCD Pins
   #ifdef DOGLCD
   #ifdef U8GLIB_ST7920 //SPI GLCD 12864 ST7920 ( like [www.digole.com] ) For Melzi V2.0
     #define LCD_PINS_RS 30 //CS chip select /SS chip slave select
     #define LCD_PINS_ENABLE 29 //SID (MOSI)
     #define LCD_PINS_D4 17 //SCK (CLK) clock
     #define BEEPER 27 // Pin 27 is take by LED_Pin, but Melzi LED do nothing with Marlin and I take this pin for BEEPER.... See here > [github.com] , If you want use this pin with Gcode M42 instead BEEPER
   #else
     // Pins for DOGM SPI LCD Support
     #define DOGLCD_A0 30
     #define DOGLCD_CS 29
     #define LCD_CONTRAST 1
   #endif
     // Uncomment screen orientation
     #define LCD_SCREEN_ROT_0
       // #define LCD_SCREEN_ROT_90
       // #define LCD_SCREEN_ROT_180
       // #define LCD_SCREEN_ROT_270
     #else // standard Hitachi LCD controller
     #define LCD_PINS_RS        4
     #define LCD_PINS_ENABLE    17
     #define LCD_PINS_D4        30
     #define LCD_PINS_D5        29
     #define LCD_PINS_D6        28
     #define LCD_PINS_D7        27
   #endif
   //The encoder and click button
   #define BTN_EN1 11
   #define BTN_EN2 10
   #ifdef LCD_I2C_PANELOLU2
     #if MB(MELZI)
       #define BTN_ENC 29 //the click switch
       #define LCD_SDSS 30 //to use the SD card reader on the Panelolu2 rather than the melzi board
     #else
       #define BTN_ENC 30 //the click switch
     #endif
   #else
     #define BTN_ENC 16  //the click switch
     #define LCD_SDSS 28 //to use the SD card reader on the smart controller rather than the melzi board
   #endif //Panelolu2
   //not connected to a pin
   #define SDCARDDETECT -1

 #endif //NEWPANEL
#endif //ULTRA_LCD

#ifdef MAKRPANEL
  #define BEEPER 29
  // Pins for DOGM SPI LCD Support
  #define DOGLCD_A0  30
  #define DOGLCD_CS  17
  #define LCD_PIN_BL 28  // backlight LED on PA3
  // GLCD features
  #define LCD_CONTRAST 1
  // Uncomment screen orientation
  #define LCD_SCREEN_ROT_0
  // #define LCD_SCREEN_ROT_90
  // #define LCD_SCREEN_ROT_180
  // #define LCD_SCREEN_ROT_270
  //The encoder and click button
  #define BTN_EN1 11
  #define BTN_EN2 10
  #define BTN_ENC 16  //the click switch
  //not connected to a pin
  #define SDCARDDETECT -1
#endif //Makrpanel

