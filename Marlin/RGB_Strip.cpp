// RGB Led Strip Control
 
#include "Marlin.h"
#include "temperature.h" 
#include "Configuration.h"
#include "ultralcd.h"
#include "RGB_Strip.h"
#include "language.h"

#if ENABLED(BLINKM)
  #include "blinkm.h"
#endif

#if ENABLED(BLINKM) || ENABLED(RGB_LED) || ENABLED(RGB_STRIP)
 
  void set_rgb_color(const uint8_t r, const uint8_t g, const uint8_t b) {

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

    #endif
  }

  // Handle the various printer events
  void handle_led_print_event(int code) {
        
    switch(code) {
      case(0):        // Print Complete
        LCD_MESSAGEPGM(MSG_INFO_COMPLETED_PRINTS);
        lcd_update();
        set_rgb_color(0, 255, 0);  // Turn RGB LEDs to GREEN
        wait_for_user = true;
        int wait_for_user_timeout;

        #if ENABLED(ULTRA_LCD)
          wait_for_user_timeout = 0;
          do {
           idle();
           safe_delay(100);
           SERIAL_ECHOLNPGM(MSG_BUSY_PAUSED_FOR_USER_OR_TIMEOUT);
           wait_for_user_timeout ++;
           if (wait_for_user_timeout >= RGB_reset_time) break;
          } while (wait_for_user);
        #endif // ULTRA_LCD

        #if ENABLED(DOGLCD)
          wait_for_user_timeout = 0;
          do {
           lcd_update();
           safe_delay(100);
           wait_for_user_timeout ++;
           if (wait_for_user_timeout >= RGB_reset_time) break;
          } while (wait_for_user);
        #endif // DOGLCD

        wait_for_user = false;
        set_rgb_color(0, 0, 0);  // Turn RGB LEDs off

        LCD_MESSAGEPGM(WELCOME_MSG);
        lcd_update();
        break;

      case(1):      // Turn RGB LEDs White
        set_rgb_color(255, 255, 255);
        break;

      case(2):      // Turn RGB LEDs Yellow
        set_rgb_color(255, 255, 0);
        break;

      case(3):      // Turn RGB LEDs Purple
        set_rgb_color(255, 0, 255);
        break;

      case(4):      // Turn RGB LEDs Teal
        set_rgb_color(0, 255, 255);
        break;

      case(9):      // Turn RGB LEDs off
        set_rgb_color(0, 0, 0);
        break;

    } // switch(code)
}

#endif  // #if ENABLED(BLINKM) || ENABLED(RGB_LED) || ENABLED(RGB_STRIP)
