/**
 * Mega controller pin assignments
 */

#ifndef __AVR_ATmega2560__
  #error Oops!  Make sure you have 'Arduino Mega' selected from the 'Tools -> Boards' menu.
#endif

#if EXTRUDERS > 2
  #error Mega Controller supports up to 2 extruders. Comment this line to keep going.
#endif

#define SERVO0_PIN 30
#define SERVO1_PIN 31
#define SERVO2_PIN 32
#define SERVO3_PIN 33

#define X_STEP_PIN 62//A8
#define X_DIR_PIN 63//A9
#define X_ENABLE_PIN 61//A7
#define X_MIN_PIN 43
#define X_MAX_PIN 42 //Max endstops default to disabled "-1", set to commented value to enable.

#define Y_STEP_PIN 65 // A11
#define Y_DIR_PIN 66 // A12
#define Y_ENABLE_PIN 64//A10
#define Y_MIN_PIN 38
#define Y_MAX_PIN 41

#define Z_STEP_PIN 68 // A14
#define Z_DIR_PIN 69 // A15
#define Z_ENABLE_PIN 67 // A13
#define Z_MIN_PIN 40
#define Z_MAX_PIN 37

#define E0_STEP_PIN 23
#define E0_DIR_PIN 24
#define E0_ENABLE_PIN 22

#define E1_STEP_PIN 26
#define E1_DIR_PIN 27
#define E1_ENABLE_PIN 25

#define SDPOWER -1
#define SDSS 53
#define LED_PIN 13

#define FAN_PIN 39
#define FAN1_PIN 35
#define FAN2_PIN 36
#define FAN_SOFT_PWM
#define CONTROLLERFAN_PIN 36
#define PS_ON_PIN -1
#define KILL_PIN -1

#define HEATER_0_PIN 29 // EXTRUDER 1
#define HEATER_1_PIN 34 // EXTRUDER 2
#define HEATER_2_PIN -1

#if TEMP_SENSOR_0 == -1
  #define TEMP_0_PIN 4 // ANALOG NUMBERING
#else
  #define TEMP_0_PIN 0 // ANALOG NUMBERING
#endif


#if TEMP_SENSOR_1 == -1
  #define TEMP_1_PIN 5 // ANALOG NUMBERING
#else
  #define TEMP_1_PIN 2 // ANALOG NUMBERING
#endif

#define TEMP_2_PIN 3 // ANALOG NUMBERING

#define HEATER_BED_PIN 28 // BED

#if TEMP_SENSOR_BED == -1
  #define TEMP_BED_PIN 6 // ANALOG NUMBERING
#else
  #define TEMP_BED_PIN 1 // ANALOG NUMBERING
#endif

#if ENABLED(MINIPANEL)
  #define BEEPER_PIN 46
  // Pins for DOGM SPI LCD Support
  #define DOGLCD_A0  47
  #define DOGLCD_CS  45
  #define LCD_PIN_BL 44  // backlight LED on PA3

  #define KILL_PIN 12
  // GLCD features
  //#define LCD_CONTRAST 190
  // Uncomment screen orientation
  //#define LCD_SCREEN_ROT_90
  //#define LCD_SCREEN_ROT_180
  //#define LCD_SCREEN_ROT_270
  //The encoder and click button
  #define BTN_EN1 48
  #define BTN_EN2 11
  #define BTN_ENC 10  //the click switch
  //not connected to a pin
  #define SD_DETECT_PIN 49
#endif //Minipanel

