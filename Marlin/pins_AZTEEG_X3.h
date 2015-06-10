/**
 * AZTEEG_X3 Arduino Mega with RAMPS v1.3 pin assignments
 */

#include "pins_RAMPS_13.h"

#define FAN_PIN            9 // (Sprinter config)
#define HEATER_1_PIN       -1

#ifdef TEMP_STAT_LEDS
  #define STAT_LED_RED       6
  #define STAT_LED_BLUE     11
#endif
