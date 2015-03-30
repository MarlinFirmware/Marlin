/**
 * AZTEEG_X3 Arduino Mega with RAMPS v1.3 pin assignments
 */

#include "pins_RAMPS_13.h"

#define FAN_PIN            9 // (Sprinter config)
#define HEATER_1_PIN       -1

//LCD Pins//

#if defined(VIKI2) || defined(miniVIKI)
 #define BEEPER 33
 // Pins for DOGM SPI LCD Support
 #define DOGLCD_A0  31 
 #define DOGLCD_CS  32 
 #define LCD_SCREEN_ROT_180
 
 //The encoder and click button 
 #define BTN_EN1 22 
 #define BTN_EN2 7
 #define BTN_ENC 12  //the click switch
 
 #define SDSS 53
 #define SDCARDDETECT -1 // Pin 49 if using display sd interface   
 
  #ifdef TEMP_STAT_LEDS
   #define STAT_LED_RED 64
   #define STAT_LED_BLUE 63
  #endif
#endif

#elif define TEMP_STAT_LEDS
  #define STAT_LED_RED       6
  #define STAT_LED_BLUE     11
#endif
