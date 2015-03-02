/**
 * Ultimaker pin assignments (Old electronics)
 */

#if !defined(__AVR_ATmega1280__) && !defined(__AVR_ATmega2560__)
  #error Oops!  Make sure you have 'Arduino Mega' selected from the 'Tools -> Boards' menu.
#endif

#define LARGE_FLASH true

#define X_STEP_PIN 25
#define X_DIR_PIN 23
#define X_MIN_PIN 15
#define X_MAX_PIN 14
#define X_ENABLE_PIN 27

#define Y_STEP_PIN 31
#define Y_DIR_PIN 33
#define Y_MIN_PIN 17
#define Y_MAX_PIN 16
#define Y_ENABLE_PIN 29

#define Z_STEP_PIN 37
#define Z_DIR_PIN 39
#define Z_MIN_PIN 19
#define Z_MAX_PIN 18
#define Z_ENABLE_PIN 35

#define HEATER_BED_PIN -1
#define TEMP_BED_PIN -1

#define HEATER_0_PIN  2
#define TEMP_0_PIN 8

#define HEATER_1_PIN 1
#define TEMP_1_PIN 1

#define HEATER_2_PIN -1
#define TEMP_2_PIN -1

#define E0_STEP_PIN         43
#define E0_DIR_PIN          45
#define E0_ENABLE_PIN       41

#define E1_STEP_PIN         -1
#define E1_DIR_PIN          -1
#define E1_ENABLE_PIN       -1

#define SDPOWER            -1
#define SDSS               -1
#define LED_PIN            -1
#define FAN_PIN            -1
#define PS_ON_PIN          -1
#define KILL_PIN           -1
#define SUICIDE_PIN        -1  //PIN that has to be turned on right after start, to keep power flowing.

#define LCD_PINS_RS 24
#define LCD_PINS_ENABLE 22
#define LCD_PINS_D4 36
#define LCD_PINS_D5 34
#define LCD_PINS_D6 32
#define LCD_PINS_D7 30
