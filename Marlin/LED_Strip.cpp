// RGB Led Strip Control
 
#include "Marlin.h"
#include "temperature.h" 
#include "Configuration.h"
#include "ultralcd.h"
#include "LED_Strip.h"

#if ENABLED(LED_STRIP)

// Set color by RGB values
void rgb_to_led_strip(int red, int green, int blue) {    
  analogWrite(REDPIN, red);
  analogWrite(GREENPIN, green);
  analogWrite(BLUEPIN, blue);
}

// Print finished - wait for user click
void rgb_print_finished() {
  LCD_MESSAGEPGM(MSG_INFO_COMPLETED_PRINTS);
  lcd_update();
  rgb_to_led_strip(0, 255, 0);  // Turn LED strip to Green
  while (!lcd_clicked()) {        
      idle();
  }
  rgb_to_led_strip(0, 0, 0);  // Turn LED strip off
  
  LCD_MESSAGEPGM(WELCOME_MSG);
  lcd_update();
}

#endif
