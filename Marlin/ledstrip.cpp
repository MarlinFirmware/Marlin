/*
  ledstrip.cpp - Library for controlling leds over a wire
  Created by Philippe Hervier Feb 2016.
*/
#include "Marlin.h"
#include "ultralcd.h"
#include "language.h"

#if ENABLED(LEDSTRIP)
   
#include "ledstrip.h"
#include <FastLED.h>

CRGB leds[LEDSTRIP_NLED];
CRGB colorSaved[LEDSTRIP_NSEGMENT+1];

boolean initialized = false;
byte r_bak, b_bak;

void SendColorsOnLedstrip (byte red, byte grn, byte blu, byte segment, byte power) {
  
   #if !defined(LEDSTRIP_PIN)
    return LEDSTRIP_NOLEDSPIN;
   #endif
   #if !defined(LEDSTRIP_NLED)
    return LEDSTRIP_NONLEDS;
   #endif
   #if !defined(LEDSTRIP_NSEGMENT)
    #define LEDSTRIP_NSEGMENT 1
   #endif
   if (segment > LEDSTRIP_NSEGMENT || segment < 0)
     return  LEDSTRIP_BADSEGMENT;

   if (!initialized){
    //memset8( leds, 0, LEDSTRIP_NLED * sizeof(CRGB));
    #if ENABLED(LEDSTRIP_EXCHANGE_RU)
      FastLED.addLeds<LEDSTRIP_TYPE, LEDSTRIP_PIN, GRB>(leds, LEDSTRIP_NLED);
    #else
      FastLED.addLeds<LEDSTRIP_TYPE, LEDSTRIP_PIN, RGB>(leds, LEDSTRIP_NLED);
    #endif
    FastLED.clear();
    colorSaved[0]=CRGB::Linen;
    for (int i = 1; i <= LEDSTRIP_NSEGMENT; i++)
      colorSaved[i]=CRGB::Seashell;

    initialized = true;
   }
   #if ENABLED(DEBUG_LEDSTRIP)
     SERIAL_ECHO_START;
     SERIAL_ECHOLN("--------------------------------");
     SERIAL_ECHOLN("LEDSTRIP.CPP BEFORE COLOR UPDATE");
     SERIAL_ECHOLNPAIR(" colorSaved.red Saved:", colorSaved[segment].red);
     SERIAL_ECHOLNPAIR(" colorSaved.green Saved:", colorSaved[segment].green);
     SERIAL_ECHOLNPAIR(" colorSaved.blue Saved:", colorSaved[segment].blue);
   #endif

   // update saved color
   if (red != colorSaved[segment].red)
     colorSaved[segment].red = red;
   if (grn != colorSaved[segment].green)
     colorSaved[segment].green = grn;
   if (blu != colorSaved[segment].blue)
     colorSaved[segment].blue = blu;

   #if ENABLED(DEBUG_LEDSTRIP)
     SERIAL_ECHO_START;
     SERIAL_ECHOLN("LEDSTRIP.CPP AFTER COLOR UPDATE");
     SERIAL_ECHOLNPAIR(" colorSaved.red Saved:", colorSaved[segment].red);
     SERIAL_ECHOLNPAIR(" colorSaved.green Saved:", colorSaved[segment].green);
     SERIAL_ECHOLNPAIR(" colorSaved.blue Saved:", colorSaved[segment].blue);
     SERIAL_ECHOLNPAIR(" power = ", power);
     SERIAL_ECHOLNPAIR(" segment = ", segment);
     SERIAL_ECHOLN("--------------------------------");
   #endif

   byte updtend = (segment > 0) ? LEDSTRIP_NLED/LEDSTRIP_NSEGMENT : LEDSTRIP_NLED ;
   byte updshift = (segment > 0) ? (segment-1) : 0;
   byte j;

   #if ENABLED(DEBUG_LEDSTRIP)
     SERIAL_ECHO_START;
     SERIAL_ECHOLN("LEDSTRIP.CPP");
     SERIAL_ECHOLNPAIR(" power = ", power);
     SERIAL_ECHOLNPAIR(" Total LED Value = ", (red + grn + blu));
     SERIAL_ECHOLNPAIR(" power = ", power);
     SERIAL_ECHOLNPAIR(" segment = ", segment);
     SERIAL_ECHOLN("--------------------------------");
   #endif

   if (red + grn + blu <= 3){  // no color change use the saved color or black
      switch(power) {
        case LED_POWEROFF:
          for(byte i = 0; i < updtend; i++) 
           leds[i + (updshift * updtend)] = CRGB::Black;
          #if ENABLED(DEBUG_LEDSTRIP)
            SERIAL_ECHO_START;
            SERIAL_ECHOPAIR("LED_POWEROFF:", LED_POWEROFF);
            SERIAL_ECHOLNPAIR(" | Segment Saved:", colorSaved[segment]);
          #endif
          break;
        case LED_POWERON:
          for(byte i = 0; i < updtend; i++) 
            leds[i + (updshift * updtend)] = colorSaved[segment];
          #if ENABLED(DEBUG_LEDSTRIP)
            SERIAL_ECHO_START;
            SERIAL_ECHOPAIR("LED_POWERON:", LED_POWERON);
            SERIAL_ECHOLNPAIR(" | Segment Saved:", colorSaved[segment]);
            SERIAL_ECHOLNPAIR(" Total LED Value = ", (red + grn + blu));
          #endif
          break;
        case LED_POWERHALF:
          for(byte i = 0; i < updtend; i++) 
           leds[i + (updshift * updtend)] = (i % 2)? colorSaved[segment]:CRGB::Black;
          #if ENABLED(DEBUG_LEDSTRIP)
            SERIAL_ECHO_START;
            SERIAL_ECHOPAIR("LED_POWERHALF:", LED_POWERHALF);
            SERIAL_ECHOLNPAIR(" | Segment Saved:", colorSaved[segment]);
          #endif
          break;
        case LED_POWERNOCHG:
          return LEDSTRIP_NOACTION;
          #if ENABLED(DEBUG_LEDSTRIP)
            SERIAL_ECHO_START;
            SERIAL_ECHOLN("No Action");
          #endif
          break;
      }
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
   #if ENABLED(DEBUG_LEDSTRIP)
     SERIAL_ECHO_START;
     SERIAL_ECHOLN("--------------------------------");
     SERIAL_ECHOLN("LEDSTRIP.CPP after FastLED.show");
     SERIAL_ECHOLNPAIR(" led.red Saved:", leds[j].red);
     SERIAL_ECHOLNPAIR(" leds.green Saved:", leds[j].green);
     SERIAL_ECHOLNPAIR(" leds.blue Saved:", leds[j].blue);
     SERIAL_ECHOLNPAIR(" power:", power);
     SERIAL_ECHOLN("--------------------------------");
   #endif
   return LEDSTRIP_OK;
}

  /*  Handle the various printer events
   *  
   *  0 - Green and timed or click to OFF.
   *  1 - White       - Used for main printing as a case light.
   *  2 - Yellow      - Used when homing.
   *  3 - Purple      - Not currently used.
   *  4 - Aqua        - Used for filament change.
   *  5 - Aqua dimmed - Used for filment change hotend timed out.
   *  6 - Aqua Half   - Used for filament change hotend heating.
   *  7 - Blacklight  - Used for bed leveling.
   *  9 - Off         - Used for after homing and after main printing.
   */

        /* OTHER COLORS BY NAME AND VALUE FOR REFERENCE
         *        COLOR NAME           R   G(U)  B
         * Candle                     255, 147, 41
         * 40W Tungsten               255, 197, 143
         * 100W Tungsten              255, 214, 170
         * Halogen                    255, 241, 224
         * Carbon Arc                 255, 250, 244
         * High Noon Sun              255, 255, 251
         * Direct Sunlight            255, 255, 255
         * Overcast Sky               201, 226, 255
         * Clear Blue Sky             64, 156, 255
         * Warm Fluorescent           255, 244, 229
         * Standard Fluorescent       244, 255, 250
         * Cool White Fluorescent     212, 235, 255
         * Full Spectrum Fluorescent  255, 244, 242
         * Grow Light Fluorescent     255, 239, 247
         * 
         *      OTHER COLORS
         * Black Light Fluorescent    167, 0, 255
         */

  void handle_led_print_event(int code) {
    switch(code) {
      case(0):        // Print Complete
        #if ENABLED(DEBUG_LEDSTRIP)
          SERIAL_ECHO_START;
          SERIAL_ECHOLNPAIR("PRINTER_EVENT:", 0);
        #endif

        LCD_MESSAGEPGM(MSG_INFO_COMPLETED_PRINTS);
        lcd_update();
        SendColorsOnLedstrip (0, 255, 0, 0, 1);  // Turn LEDs Green

        #if DISABLED(NO_PAUSE_OR_TIMEOUT)
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
        #endif  // NO_PAUSE_OR_TIMEOUT

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
      case(4):      // Turn RGB LEDs Aqua
        #if ENABLED(DEBUG_LEDSTRIP)
          SERIAL_ECHO_START;
          SERIAL_ECHOLNPAIR("PRINTER_EVENT:", 4);
        #endif
        SendColorsOnLedstrip (0, 255, 255, 0, 1);
        break;
      case(5):      // Turn RGB LEDs Aqua dimmed
        #if ENABLED(DEBUG_LEDSTRIP)
          SERIAL_ECHO_START;
          SERIAL_ECHOLNPAIR("PRINTER_EVENT:", 5);
        #endif
        SendColorsOnLedstrip (0, 50, 50, 0, 1);
        break;
      case(6):      // Turn RGB LEDs Aqua half
        #if ENABLED(DEBUG_LEDSTRIP)
          SERIAL_ECHO_START;
          SERIAL_ECHOLNPAIR("PRINTER_EVENT:", 6);
        #endif
        SendColorsOnLedstrip (0, 127, 127, 0, 1);
        break;
      case(7):      // Turn RGB LEDs Blacklight
        #if ENABLED(DEBUG_LEDSTRIP)
          SERIAL_ECHO_START;
          SERIAL_ECHOLNPAIR("PRINTER_EVENT:", 7);
        #endif
        SendColorsOnLedstrip (167, 0, 255, 0, 1);
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
