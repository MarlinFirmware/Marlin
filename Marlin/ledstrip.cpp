/*
  ledstrip.cpp - Library for controlling leds over a wire
  Created by Philippe Hervier Feb 2016.
*/
#include "Marlin.h"
#include "temperature.h" 
#include "Configuration.h"
#include "ultralcd.h"
#include "language.h"

#if ENABLED(LEDSTRIP)

#include "ledstrip.h"
#include <FastLED.h>


CRGB leds[LEDSTRIP_NLED];
CRGB colorSaved[LEDSTRIP_NSEGMENT+1];

boolean initialized = false;

#if ENABLED(LEDSTRIP_EXCHANGE_RU)
byte SendColorsOnLedstrip (int grn, int red, int blu, byte segment, byte power) {
#else
byte SendColorsOnLedstrip (int red, int grn, int blu, byte segment, byte power) {
#endif
   #if ! defined(LEDSTRIP_PIN)
    return LEDSTRIP_NOLEDSPIN;
   #endif
   #if ! defined(LEDSTRIP_NLED)
    return LEDSTRIP_NONLEDS;
   #endif
   #if ! defined(LEDSTRIP_NSEGMENT)
    #define LEDSTRIP_NSEGMENT 1
   #endif
   if (segment > LEDSTRIP_NSEGMENT || segment < 0)
     return  LEDSTRIP_BADSEGMENT;

   if (!initialized){
    //memset8( leds, 0, LEDSTRIP_NLED * sizeof(CRGB));
    FastLED.addLeds<LEDSTRIP_TYPE, LEDSTRIP_PIN, RGB>(leds, LEDSTRIP_NLED);
    FastLED.clear();
    colorSaved[0]=CRGB::Linen;
    for (int i = 1; i <= LEDSTRIP_NSEGMENT; i++)
      colorSaved[i]=CRGB::Seashell;
    initialized = true;
   }

   // update saved color
     if (red >= 0)
       colorSaved[segment].red  = red;
     if (grn >= 0)
       colorSaved[segment].green= grn;
     if (blu >= 0)
       colorSaved[segment].blue = blu;

   byte updtend = (segment > 0) ? LEDSTRIP_NLED/LEDSTRIP_NSEGMENT : LEDSTRIP_NLED ;
   byte updshift = (segment > 0) ? (segment-1) : 0;
   byte j;

   if (red + grn + blu <= 3){  // no color change use the saved color or black
     if (power == LED_POWERON){
      for(byte i = 0; i < updtend; i++) 
       leds[i + (updshift * updtend)] = colorSaved[segment];
     }
     else if (power == LED_POWEROFF){
      for(byte i = 0; i < updtend; i++) 
       leds[i + (updshift * updtend)] = CRGB::Black;

     }
     else if (power == LED_POWERHALF){
      for(byte i = 0; i < updtend; i++) 
       leds[i + (updshift * updtend)] = (i % 2)? colorSaved[segment]:CRGB::Black;
     }
     else
       return LEDSTRIP_NOACTION;
   }
   else {
     for(byte i = 0; i < updtend; i++) { 
      j = i + (updshift * updtend);
       if (red >= 0)
         leds[j].red = red;
       if (grn >= 0)
         leds[j].green = grn;
       if (blu >= 0)
         leds[j].blue = blu;
     }
   }
   FastLED.show();
   return LEDSTRIP_OK;
}

  // Handle the various printer events
  void handle_led_print_event(int code) {
    char ledstripgcode[20];
    int ledstrip_segment = 0;
    byte pi = 1;
    switch(code) {
      case(0):        // Print Complete
        #if ENABLED(DEBUG_LEDSTRIP)
          SERIAL_ECHO_START;
          SERIAL_ECHOLNPAIR("PRINTER_EVENT:", 0);
        #endif

        LCD_MESSAGEPGM(MSG_INFO_COMPLETED_PRINTS);
        lcd_update();
        SendColorsOnLedstrip (0, 255, 0, 0, 1);  // Turn LEDs Green
        wait_for_user = true;
        int wait_for_user_timeout;

        #if ENABLED(ULTRA_LCD)
          wait_for_user_timeout = 0;
          do {
           idle();
           safe_delay(100);
           if (wait_for_user_timeout == LEDSTRIP_RESET_TIME / 2)
           SERIAL_ECHOLNPGM(MSG_BUSY_PAUSED_FOR_USER_OR_TIMEOUT);
           wait_for_user_timeout ++;
           if (wait_for_user_timeout >= LEDSTRIP_RESET_TIME) break;
          } while (wait_for_user);
        #endif // ULTRA_LCD

        #if ENABLED(DOGLCD)
          wait_for_user_timeout = 0;
          do {
           idle();
           safe_delay(100);
           if (wait_for_user_timeout == LEDSTRIP_RESET_TIME / 2)
           SERIAL_ECHOLNPGM(MSG_BUSY_PAUSED_FOR_USER_OR_TIMEOUT);
           wait_for_user_timeout ++;
           if (wait_for_user_timeout >= LEDSTRIP_RESET_TIME) break;
          } while (wait_for_user);
        #endif // DOGLCD

        wait_for_user = false;
        SendColorsOnLedstrip (0, 0, 0, 0, 0);  // Turn RGB LEDs off
        LCD_MESSAGEPGM(WELCOME_MSG);
        idle();
        break;
      case(1):      // Turn RGB LEDs White
        #if ENABLED(DEBUG_LEDSTRIP)
          SERIAL_ECHO_START;
          SERIAL_ECHOLNPAIR("PRINTER_EVENT:", 1);
        #endif
        SendColorsOnLedstrip (255, 255, 255, 0, 1);
        break;
      case(2):      // Turn RGB LEDs Yellow
        #if ENABLED(DEBUG_LEDSTRIP)
          SERIAL_ECHO_START;
          SERIAL_ECHOLNPAIR("PRINTER_EVENT:", 2);
        #endif
        SendColorsOnLedstrip (255, 255, 0, 0, 1);
        break;
      case(3):      // Turn RGB LEDs Purple
        #if ENABLED(DEBUG_LEDSTRIP)
          SERIAL_ECHO_START;
          SERIAL_ECHOLNPAIR("PRINTER_EVENT:", 3);
        #endif
        SendColorsOnLedstrip (255, 0, 255, 0, 1);
        break;
      case(4):      // Turn RGB LEDs Teal
        #if ENABLED(DEBUG_LEDSTRIP)
          SERIAL_ECHO_START;
          SERIAL_ECHOLNPAIR("PRINTER_EVENT:", 4);
        #endif
        SendColorsOnLedstrip (0, 255, 255, 0, 1);
        break;
      case(5):      // Turn RGB LEDs Teal dimmed
        #if ENABLED(DEBUG_LEDSTRIP)
          SERIAL_ECHO_START;
          SERIAL_ECHOLNPAIR("PRINTER_EVENT:", 5);
        #endif
        SendColorsOnLedstrip (0, 50, 50, 0, 1);
        break;
      case(6):      // Turn RGB LEDs Teal half
        #if ENABLED(DEBUG_LEDSTRIP)
          SERIAL_ECHO_START;
          SERIAL_ECHOLNPAIR("PRINTER_EVENT:", 6);
        #endif
        SendColorsOnLedstrip (0, 127, 127, 0, 1);
        break;
      case(9):      // Turn RGB LEDs off
        #if ENABLED(DEBUG_LEDSTRIP)
          SERIAL_ECHO_START;
          SERIAL_ECHOLNPAIR("PRINTER_EVENT:", 9);
        #endif
        SendColorsOnLedstrip (0, 0, 0, 0, 0);
        break;
    } // switch(code)
}
#endif //LEDSTRIP