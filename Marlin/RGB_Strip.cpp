// RGB Led Strip Control
 
#include "Marlin.h"
#include "temperature.h" 
#include "Configuration_adv.h"
#include "ultralcd.h"
#include "RGB_Strip.h"
#include "language.h"

#if ENABLED(BLINKM)
  #include "blinkm.h"
#endif

#if ENABLED(BLINKM) || ENABLED(RGB_LED) || ENABLED(RGB_STRIP) || ENABLED(RGBW_STRIP)
 
  void set_rgb_color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t w) {

    #if ENABLED(BLINKM)
      // This variant uses i2c to send the RGB components to the device.
      SendColors(r, g, b);
    #else

      // This variant uses 3 separate pins for the RGB components.
      // If the pins can do PWM then their intensity will be set.
      digitalWrite(RGB_LED_R_PIN, r ? HIGH : LOW);
      digitalWrite(RGB_LED_G_PIN, g ? HIGH : LOW);
      digitalWrite(RGB_LED_B_PIN, b ? HIGH : LOW);
      analogWrite(RGB_LED_R_PIN, r);
      analogWrite(RGB_LED_G_PIN, g);
      analogWrite(RGB_LED_B_PIN, b);
      #if ENABLED(RGBW_STRIP)
        digitalWrite(RGB_LED_W_PIN, w ? HIGH : LOW);
        analogWrite(RGB_LED_W_PIN, w);
      #endif
    #endif
  }

  /* OTHER COLORS BY NAME AND VALUE FOR REFERENCE
   *        COLOR NAME           R   G(U)  B   (W)
   * Candle                     255, 147,  41,   0
   * 40W Tungsten               255, 197, 143,   0
   * 100W Tungsten              255, 214, 170,   0
   * Halogen                    255, 241, 224,   0
   * Carbon Arc                 255, 250, 244,   0
   * High Noon Sun              255, 255, 251,   0
   * Direct Sunlight            255, 255, 255,   0
   * Direct Sunlight(RGBW)        0,   0,   0, 255
   * Overcast Sky               201, 226, 255,   0
   * Clear Blue Sky              64, 156, 255,   0
   * Warm Fluorescent           255, 244, 229,   0
   * Standard Fluorescent       244, 255, 250,   0
   * Cool White Fluorescent     212, 235, 255,   0
   * Full Spectrum Fluorescent  255, 244, 242,   0
   * Grow Light Fluorescent     255, 239, 247,   0
   * 
   *      OTHER COLORS
   * Black Light Fluorescent    167, 0, 255
   */

  /*  Handle the various printer events
   *  
   *  0 - Green       - Timed or click to OFF.
   *  1 - White       - Used for main printing as a case light.
   *  2 - Yellow      - Used when homing.
   *  3 - Purple      - Not currently used.
   *  4 - Aqua        - Used for filament change.
   *  5 - Aqua dimmed - Used for filment change hotend timed out.
   *  6 - Aqua Half   - Used for filament change hotend heating.
   *  7 - Red         - Not currently used.
   *  9 - Off         - Used for after homing and leveling.
   */

  // Handle the various printer events
  void handle_led_print_event(byte code) {
        
    switch(code) {
      case(0):        // Print Complete
        LCD_MESSAGEPGM(MSG_INFO_COMPLETED_PRINTS);
        lcd_update();
        set_rgb_color(0, 255, 0, 0);  // Turn RGB LEDs to GREEN

        #if DISABLED(NO_PAUSE_OR_TIMEOUT)
          wait_for_user = true;
          int wait_for_user_timeout;
  
          #if ENABLED(ULTRA_LCD)
            wait_for_user_timeout = 0;
            do {
             idle();
             safe_delay(100);
             if (wait_for_user_timeout == RGB_reset_time / 2)
             SERIAL_ECHOLNPGM(MSG_BUSY_PAUSED_FOR_USER_OR_TIMEOUT);
             wait_for_user_timeout ++;
             if (wait_for_user_timeout >= RGB_reset_time) break;
            } while (wait_for_user);
          #endif // ULTRA_LCD
  
          #if ENABLED(DOGLCD)
            wait_for_user_timeout = 0;
            do {
             idle();
             safe_delay(100);
             if (wait_for_user_timeout == RGB_reset_time / 2)
             SERIAL_ECHOLNPGM(MSG_BUSY_PAUSED_FOR_USER_OR_TIMEOUT);
             wait_for_user_timeout ++;
             if (wait_for_user_timeout >= RGB_reset_time) break;
            } while (wait_for_user);
          #endif // DOGLCD
  
          wait_for_user = false;
          set_rgb_color(0, 0, 0, 0);  // Turn RGB LEDs off
        #endif  // NO_PAUSE_OR_TIMEOUT

        LCD_MESSAGEPGM(WELCOME_MSG);
        idle();
        break;
      case(1):      // Turn RGB LEDs White
        if (RGBW_STRIP)
          set_rgb_color(0, 0, 0, 255);
        else        
          set_rgb_color(255, 255, 255);
        break;
      case(2):      // Turn RGB LEDs Yellow
        set_rgb_color(255, 255, 0, 0);
        break;
      case(3):      // Turn RGB LEDs Purple
        set_rgb_color(255, 0, 255, 0);
        break;
      case(4):      // Turn RGB LEDs Aqua
        set_rgb_color(0, 255, 255, 0);
        break;
      case(5):      // Turn RGB LEDs Aqua dimmed
        set_rgb_color(0, 50, 50, 0);
        break;
      case(6):      // Turn RGB LEDs Aqua half
        set_rgb_color(0, 127, 127, 0);
        break;
      case(7):      // Turn RGB LEDs Blacklight
        set_rgb_color(167, 0, 255, 0);
        break;
      case(9):      // Turn RGB LEDs off
        set_rgb_color(0, 0, 0, 0);
        break;
    } // switch(code)
}
#endif  // BLINKM || RGB_LED || RGB_STRIP || RGBW_STRIP
