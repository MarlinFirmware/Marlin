/**
 * Brainwave Pro pin assignments (AT90USB1286)
 *
 * Requires hardware bundle for Arduino:
 * https://github.com/unrepentantgeek/brainwave-arduino
 */

#ifndef __AVR_AT90USB1286__
  #error Oops!  Make sure you have 'Brainwave Pro' selected from the 'Tools -> Boards' menu.
#endif

#include "fastio.h"

#ifndef AT90USBxx_TEENSYPP_ASSIGNMENTS  // use Teensyduino Teensy++2.0 pin assignments instead of Marlin alphabetical.
  #error  Uncomment #define AT90USBxx_TEENSYPP_ASSIGNMENTS in fastio.h for this config
#endif

#define LARGE_FLASH        true

#define X_STEP_PIN         33
#define X_DIR_PIN          32
#define X_ENABLE_PIN       11
#define X_STOP_PIN         47

#define Y_STEP_PIN         31
#define Y_DIR_PIN          30
#define Y_ENABLE_PIN        8
#define Y_STOP_PIN         18

#define Z_STEP_PIN         29
#define Z_DIR_PIN          28
#define Z_ENABLE_PIN       37
#define Z_MAX_PIN          36
#define Z_MIN_PIN          17  // Bed Z probe

#define E0_STEP_PIN        35
#define E0_DIR_PIN         34
#define E0_ENABLE_PIN      13

#define HEATER_0_PIN       15
#define HEATER_1_PIN       -1
#define HEATER_2_PIN       -1
#define HEATER_BED_PIN     14  // Bed
#define FAN_PIN            16  // Fan, PWM

#define TEMP_0_PIN          2  // Extruder / Analog pin numbering
#define TEMP_1_PIN          1  // Spare / Analog pin numbering
#define TEMP_2_PIN         -1
#define TEMP_BED_PIN        0  // Bed / Analog pin numbering

#define SDPOWER            -1
#define SDSS               20
#define LED_PIN            19
#define PS_ON_PIN          -1
#define KILL_PIN           -1
#define ALARM_PIN          -1
#define SD_DETECT_PIN      12

#if DISABLED(SDSUPPORT)
  // these pins are defined in the SD library if building with SD support
  #define SCK_PIN          21
  #define MISO_PIN         23
  #define MOSI_PIN         22
#endif

