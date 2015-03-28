/**
 * AZTEEG_X3 Arduino Mega with RAMPS v1.3 pin assignments
 */

#include "pins_RAMPS_13.h"

#undef FAN_PIN
#define FAN_PIN            9 // (Sprinter config)

#undef HEATER_1_PIN
#define HEATER_1_PIN       -1

#ifdef TEMP_STAT_LEDS
  #define STAT_LED_RED       6
  #define STAT_LED_BLUE     11
#endif
