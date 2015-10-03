/**
 * MegaTronics v3.0 pin assignments
 */

#ifndef __AVR_ATmega2560__
  #error Oops!  Make sure you have 'Arduino Mega' selected from the 'Tools -> Boards' menu.
#endif

#define LARGE_FLASH        true


#if ENABLED(Z_PROBE_SLED)
  #define SLED_PIN         -1
#endif

// Servo support
#define SERVO0_PIN         46 //AUX3-6
#define SERVO1_PIN         47 //AUX3-5
#define SERVO2_PIN         48 //AUX3-4
#define SERVO2_PIN         49 //AUX3-3

#define X_STEP_PIN         58
#define X_DIR_PIN          57
#define X_ENABLE_PIN       59
#define X_MIN_PIN          37
#define X_MAX_PIN          40 // put to -1 to disable

#define Y_STEP_PIN         5
#define Y_DIR_PIN          17
#define Y_ENABLE_PIN       4
#define Y_MIN_PIN          41
#define Y_MAX_PIN          38 // put to -1 to disable

#define Z_STEP_PIN         16
#define Z_DIR_PIN          11
#define Z_ENABLE_PIN       3
#define Z_MIN_PIN          18
#define Z_MAX_PIN          19 // put to -1 to disable

#define E0_STEP_PIN        28
#define E0_DIR_PIN         27
#define E0_ENABLE_PIN      29

#define E1_STEP_PIN        25
#define E1_DIR_PIN         24
#define E1_ENABLE_PIN      26

#define E2_STEP_PIN        22
#define E2_DIR_PIN         60
#define E2_ENABLE_PIN      23

#define SDPOWER -1
#define SDSS 53
#define LED_PIN 13

#define PS_ON_PIN 12
#define KILL_PIN -1

#define HEATER_0_PIN 2
#define HEATER_1_PIN 9
#define HEATER_2_PIN 8
#define HEATER_BED_PIN 10
#define FAN_PIN 6
#define FAN2_PIN 7

#define TEMP_0_PIN   (TEMP_SENSOR_0 == -1 ?  11 : 15) // ANALOG NUMBERING
#define TEMP_1_PIN   (TEMP_SENSOR_1 == -1 ?  10 : 13) // ANALOG NUMBERING
#define TEMP_2_PIN   (TEMP_SENSOR_2 == -1 ?   9 : 12) // ANALOG NUMBERING
#define TEMP_BED_PIN (TEMP_SENSOR_BED == -1 ? 8 : 14) // ANALOG NUMBERING

#define BEEPER_PIN 61

#define LCD_PINS_RS 32
#define LCD_PINS_ENABLE 31
#define LCD_PINS_D4 14
#define LCD_PINS_D5 30
#define LCD_PINS_D6 39
#define LCD_PINS_D7 15

#define SHIFT_CLK 43
#define SHIFT_LD 35
#define SHIFT_OUT 34
#define SHIFT_EN 44

// Buttons are directly attached using keypad
#define BTN_EN1 44
#define BTN_EN2 45
#define BTN_ENC 33 // the click

#define BLEN_C 2
#define BLEN_B 1
#define BLEN_A 0

#define SD_DETECT_PIN -1 // Megatronics doesn't use this
