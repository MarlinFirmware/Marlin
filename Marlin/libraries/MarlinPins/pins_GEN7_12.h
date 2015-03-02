/**
 * Gen7 v1.1, v1.2, v1.3 pin assignments
 */

#if !defined(__AVR_ATmega644P__) && !defined(__AVR_ATmega644__) && !defined(__AVR_ATmega1284P__)
  #error Oops!  Make sure you have 'Gen7' selected from the 'Tools -> Boards' menu.
#endif

#ifndef GEN7_VERSION
  #define GEN7_VERSION 12 // v1.x
#endif

//x axis pins
#define X_STEP_PIN 19
#define X_DIR_PIN 18
#define X_ENABLE_PIN 24
#define X_STOP_PIN 7

//y axis pins
#define Y_STEP_PIN 23
#define Y_DIR_PIN 22
#define Y_ENABLE_PIN 24
#define Y_STOP_PIN 5

//z axis pins
#define Z_STEP_PIN 26
#define Z_DIR_PIN 25
#define Z_ENABLE_PIN 24
#define Z_MIN_PIN 1
#define Z_MAX_PIN 0

//extruder pins
#define E0_STEP_PIN 28
#define E0_DIR_PIN 27
#define E0_ENABLE_PIN 24

#define TEMP_0_PIN 1
#define TEMP_1_PIN -1
#define TEMP_2_PIN -1
#define TEMP_BED_PIN 2

#define HEATER_0_PIN 4
#define HEATER_1_PIN -1
#define HEATER_2_PIN -1
#define HEATER_BED_PIN 3

#define KILL_PIN -1

#define SDPOWER -1
#define SDSS -1 // SCL pin of I2C header
#define LED_PIN -1

#if (GEN7_VERSION >= 13)
  // Gen7 v1.3 removed the fan pin
  #define FAN_PIN -1
#else
  #define FAN_PIN 31
#endif

#define PS_ON_PIN 15

//All these generations of Gen7 supply thermistor power
//via PS_ON, so ignore bad thermistor readings
#define BOGUS_TEMPERATURE_FAILSAFE_OVERRIDE

//our pin for debugging.
#define DEBUG_PIN 0

//our RS485 pins
#define TX_ENABLE_PIN 12
#define RX_ENABLE_PIN 13

