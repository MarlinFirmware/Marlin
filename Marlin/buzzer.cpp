#include "Marlin.h"
#include "buzzer.h"
#include "ultralcd.h"

#if HAS_BUZZER
  void buzz(long duration, uint16_t freq) {
    if (freq > 0) {
      #ifdef LCD_USE_I2C_BUZZER
        lcd_buzz(duration, freq);
      #elif defined(BEEPER) && BEEPER >= 0 // on-board buzzers have no further condition
        SET_OUTPUT(BEEPER);
        #ifdef SPEAKER // a speaker needs a AC ore a pulsed DC
          //tone(BEEPER, freq, duration); // needs a PWMable pin
          unsigned int delay = 1000000 / freq / 2;
          int i = duration * freq / 1000;
          while (i--) {
            WRITE(BEEPER,HIGH);
            delayMicroseconds(delay);
            WRITE(BEEPER,LOW);
            delayMicroseconds(delay);
           }
        #else // buzzer has its own resonator - needs a DC
          WRITE(BEEPER, HIGH);
          delay(duration);
          WRITE(BEEPER, LOW);
        #endif
      #else
        delay(duration);
      #endif
    }
    else {
      delay(duration);
    }
  }
#endif
