/**
 * Gen3+ pin assignments
 */

#if !defined(__AVR_ATmega644P__) && !defined(__AVR_ATmega1284P__)
  #error Oops!  Make sure you have 'Sanguino' selected from the 'Tools -> Boards' menu.
#endif

#define X_STEP_PIN         15
#define X_DIR_PIN          18
#define X_STOP_PIN         20

#define Y_STEP_PIN         23
#define Y_DIR_PIN          22
#define Y_STOP_PIN         25

#define Z_STEP_PIN         27
#define Z_DIR_PIN          28
#define Z_STOP_PIN         30

#define E0_STEP_PIN        17
#define E0_DIR_PIN         21

#define LED_PIN            -1

#define FAN_PIN            -1

#define PS_ON_PIN         14
#define KILL_PIN           -1

#define HEATER_0_PIN       12 // (extruder)

#define HEATER_BED_PIN     16 // (bed)
#define X_ENABLE_PIN       19
#define Y_ENABLE_PIN       24
#define Z_ENABLE_PIN       29
#define E0_ENABLE_PIN      13

#define TEMP_0_PIN          0   // MUST USE ANALOG INPUT NUMBERING NOT DIGITAL OUTPUT NUMBERING!!!!!!!!! (pin 33 extruder)
#define TEMP_1_PIN         -1
#define TEMP_2_PIN         -1
#define TEMP_BED_PIN        5   // MUST USE ANALOG INPUT NUMBERING NOT DIGITAL OUTPUT NUMBERING!!!!!!!!! (pin 34 bed)
#define SDPOWER            -1
#define SDSS               4
#define HEATER_2_PIN       -1
