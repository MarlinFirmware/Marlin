/**
 * bq ZUM Mega 3D board definition
 */

#ifndef __AVR_ATmega2560__
  #error Oops! Make sure you have 'Arduino Mega 2560' selected from the 'Tools -> Boards' menu.
#endif

#include "pins_RAMPS_13.h"

#undef X_MAX_PIN
#define X_MAX_PIN         79 //  2

#undef Z_ENABLE_PIN
#define Z_ENABLE_PIN      77 // 62

#undef FAN_PIN
#define FAN_PIN           12 //  4

#undef HEATER_0_PIN
#define HEATER_0_PIN       9 // 10

#undef HEATER_1_PIN
#define HEATER_1_PIN      10 //  9

#undef TEMP_1_PIN
#define TEMP_1_PIN        14 // 15

#undef TEMP_BED_PIN
#define TEMP_BED_PIN      15 // 14

#define DIGIPOTSS_PIN     22
#define DIGIPOT_CHANNELS  { 4, 5, 3, 0, 1 }

#define FAN1_PIN          7

#undef PS_ON_PIN             // 12
#define PS_ON_PIN         81 // External Power Supply

#if ENABLED(BQ_LCD_SMART_CONTROLLER) // Most similar to REPRAP_DISCOUNT_SMART_CONTROLLER
  #define KILL_PIN        41

  #define BEEPER_PIN      37

  #define BTN_EN1         31
  #define BTN_EN2         33
  #define BTN_ENC         35

  #define LCD_PIN_BL      39
  #define LCD_PINS_RS     16
  #define LCD_PINS_ENABLE 17
  #define LCD_PINS_D4     23

  #define SD_DETECT_PIN   49
#endif

#if ENABLED(AUTO_BED_LEVELING_FEATURE)
  #undef Z_MIN_PIN
  #undef Z_MAX_PIN
  #define Z_MIN_PIN       19 // IND_S_5V
  #define Z_MAX_PIN       18 // Z-MIN Label
#endif
