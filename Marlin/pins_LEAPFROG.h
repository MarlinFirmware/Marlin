/**
 * Leapfrog Driver board pin assignments
 */

#if !defined(__AVR_ATmega1280__) && !defined(__AVR_ATmega2560__)
  #error Oops!  Make sure you have 'Arduino Mega' selected from the 'Tools -> Boards' menu.
#endif

#define X_STEP_PIN         28
#define X_DIR_PIN          63
#define X_ENABLE_PIN       29
#define X_MIN_PIN          47
#define X_MAX_PIN          2 //Max endstops default to disabled "-1", set to commented value to enable.

#define Y_STEP_PIN         14 // A6
#define Y_DIR_PIN          15 // A0
#define Y_ENABLE_PIN       39
#define Y_MIN_PIN          48
#define Y_MAX_PIN          15

#define Z_STEP_PIN         31 // A2
#define Z_DIR_PIN          32 // A6
#define Z_ENABLE_PIN       30 // A1
#define Z_MIN_PIN          49
#define Z_MAX_PIN          -1

#define E0_STEP_PIN         34  //34
#define E0_DIR_PIN          35 //35
#define E0_ENABLE_PIN       33 //33

#define E1_STEP_PIN         37 //37
#define E1_DIR_PIN          40 //40
#define E1_ENABLE_PIN       36 //36

#define SDPOWER            -1
#define SDSS               11
#define SD_DETECT_PIN      -1 // 10 optional also used as mode pin
#define LED_PIN            13
#define FAN_PIN            7
#define PS_ON_PIN          -1
#define KILL_PIN           -1
#define SOL1_PIN   16
#define SOL2_PIN    17

#define HEATER_0_PIN       9
#define HEATER_1_PIN       8 // 12
#define HEATER_2_PIN       11 //-1 // 13
#define TEMP_0_PIN         13 //D27   // MUST USE ANALOG INPUT NUMBERING NOT DIGITAL OUTPUT NUMBERING!!!!!!!!!
#define TEMP_1_PIN         15 // 1
#define TEMP_2_PIN         -1 // 2
#define HEATER_BED_PIN     10 // 14/15
#define TEMP_BED_PIN       14 // 1,2 or I2C
/*  Unused (1) (2) (3) 4 5 6 7 8 9 10 11 12 13 (14) (15) (16) 17 (18) (19) (20) (21) (22) (23) 24 (25) (26) (27) 28 (29) (30) (31)  */

