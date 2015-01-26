/**
 * 3DRAG (and K8200) Arduino Mega with RAMPS v1.3 pin assignments
 */

#include "pins_RAMPS_13.h"

#define X_MAX_PIN          2
#define Y_MAX_PIN          15
#define Z_MAX_PIN          -1

#define SDSS               25//53

#define BEEPER             33

#define FAN_PIN            8

#define HEATER_0_PIN       10
#define HEATER_1_PIN       12
#define HEATER_2_PIN       6

#define HEATER_BED_PIN     9    // BED

#if defined(ULTRA_LCD) && defined(NEWPANEL)
  #define BEEPER -1

  #define LCD_PINS_RS 27
  #define LCD_PINS_ENABLE 29
  #define LCD_PINS_D4 37
  #define LCD_PINS_D5 35
  #define LCD_PINS_D6 33
  #define LCD_PINS_D7 31

  // Buttons
  #define BTN_EN1 16
  #define BTN_EN2 17
  #define BTN_ENC 23 //the click
#endif // ULTRA_LCD && NEWPANEL
