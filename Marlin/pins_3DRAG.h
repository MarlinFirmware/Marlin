/**
 * 3DRAG (and K8200) Arduino Mega with RAMPS v1.3 pin assignments
 */

#include "pins_RAMPS_13.h"

#undef Z_ENABLE_PIN
#define Z_ENABLE_PIN       63

#undef X_MAX_PIN
#undef Y_MAX_PIN
#undef Z_MAX_PIN
#define X_MAX_PIN          2
#define Y_MAX_PIN          15
#define Z_MAX_PIN          -1

#undef SDSS
#define SDSS               25//53

#undef FAN_PIN
#define FAN_PIN            8

#undef HEATER_1_PIN
#undef HEATER_2_PIN
#undef HEATER_BED_PIN
#define HEATER_0_PIN       10
#define HEATER_1_PIN       12
#define HEATER_2_PIN       6

#define HEATER_BED_PIN     9    // BED

#if defined(ULTRA_LCD) && defined(NEWPANEL)
  #undef BEEPER
  #define BEEPER -1

  #undef LCD_PINS_RS
  #undef LCD_PINS_ENABLE
  #undef LCD_PINS_D4
  #undef LCD_PINS_D5
  #undef LCD_PINS_D6
  #undef LCD_PINS_D7
  #define LCD_PINS_RS 27
  #define LCD_PINS_ENABLE 29
  #define LCD_PINS_D4 37
  #define LCD_PINS_D5 35
  #define LCD_PINS_D6 33
  #define LCD_PINS_D7 31

  // Buttons
  #undef BTN_EN1
  #undef BTN_EN2
  #undef BTN_ENC
  #define BTN_EN1 16
  #define BTN_EN2 17
  #define BTN_ENC 23 //the click

#else

  #define BEEPER 33

#endif // ULTRA_LCD && NEWPANEL
