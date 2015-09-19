/**
 * Brainwave 1.0 pin assignments (AT90USB646)
 *
 * Requires hardware bundle for Arduino:
 * https://github.com/unrepentantgeek/brainwave-arduino
 */

#ifndef __AVR_AT90USB646__
  #error Oops!  Make sure you have 'Brainwave' selected from the 'Tools -> Boards' menu.
#endif

#define X_STEP_PIN         27
#define X_DIR_PIN          29
#define X_ENABLE_PIN       28
#define X_STOP_PIN          7
#define X_ATT_PIN          26

#define Y_STEP_PIN         31
#define Y_DIR_PIN          33
#define Y_ENABLE_PIN       32
#define Y_STOP_PIN          6
#define Y_ATT_PIN          30

#define Z_STEP_PIN         17
#define Z_DIR_PIN          19
#define Z_ENABLE_PIN       18
#define Z_STOP_PIN          5
#define Z_ATT_PIN          16

#define E0_STEP_PIN        21
#define E0_DIR_PIN         23
#define E0_ENABLE_PIN      22
#define E0_ATT_PIN         20

#define HEATER_0_PIN        4  // Extruder
#define HEATER_1_PIN       -1
#define HEATER_2_PIN       -1
#define HEATER_BED_PIN     38  // Bed
#define FAN_PIN             3  // Fan

#define TEMP_0_PIN          7  // Extruder / Analog pin numbering
#define TEMP_1_PIN         -1
#define TEMP_2_PIN         -1
#define TEMP_BED_PIN        6  // Bed / Analog pin numbering

#define SDPOWER            -1
#define SDSS               -1
#define LED_PIN            39
#define PS_ON_PIN          -1
#define KILL_PIN           -1
#define ALARM_PIN          -1

#if DISABLED(SDSUPPORT)
  // these pins are defined in the SD library if building with SD support
  #define SCK_PIN           9
  #define MISO_PIN         11
  #define MOSI_PIN         10
#endif

