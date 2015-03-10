/**
 * Minitronics v1.0 pin assignments
 */

#ifndef __AVR_ATmega1281__
  #error Oops!  Make sure you have 'Minitronics' selected from the 'Tools -> Boards' menu.
#endif

#define LARGE_FLASH        true

#define X_STEP_PIN 48
#define X_DIR_PIN 47
#define X_ENABLE_PIN 49
#define X_MIN_PIN 5
#define X_MAX_PIN 2

#define Y_STEP_PIN 39 // A6
#define Y_DIR_PIN 40 // A0
#define Y_ENABLE_PIN 38
#define Y_MIN_PIN 2
#define Y_MAX_PIN 15

#define Z_STEP_PIN 42 // A2
#define Z_DIR_PIN 43 // A6
#define Z_ENABLE_PIN 41 // A1
#define Z_MIN_PIN 6
#define Z_MAX_PIN -1

#define E0_STEP_PIN 45
#define E0_DIR_PIN 44
#define E0_ENABLE_PIN 27

#define E1_STEP_PIN 36
#define E1_DIR_PIN 35
#define E1_ENABLE_PIN 37

#define E2_STEP_PIN -1
#define E2_DIR_PIN -1
#define E2_ENABLE_PIN -1

#define SDPOWER -1
#define SDSS 16
#define LED_PIN 46

#define FAN_PIN 9
#define FAN2_PIN -1
#define PS_ON_PIN -1
#define KILL_PIN -1

#define HEATER_0_PIN 7 // EXTRUDER 1
#define HEATER_1_PIN 8 // EXTRUDER 2
#define HEATER_2_PIN -1


#define TEMP_0_PIN 7 // ANALOG NUMBERING
#define TEMP_1_PIN 6 // ANALOG NUMBERING
#define TEMP_2_PIN -1 // ANALOG NUMBERING

#define HEATER_BED_PIN 3 // BED
#define TEMP_BED_PIN 6 // ANALOG NUMBERING

#define BEEPER -1


#define LCD_PINS_RS -1
#define LCD_PINS_ENABLE -1
#define LCD_PINS_D4 -1
#define LCD_PINS_D5 -1
#define LCD_PINS_D6 -1
#define LCD_PINS_D7 -1


// Buttons are directly attached using keypad
#define BTN_EN1 -1
#define BTN_EN2 -1
#define BTN_ENC -1 // the click

#define BLEN_C 2
#define BLEN_B 1
#define BLEN_A 0

#define SDCARDDETECT -1  // Megatronics does not use this port
