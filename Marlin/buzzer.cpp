#include "Marlin.h"
#include "buzzer.h"
#include "ultralcd.h"

#if HAS_BUZZER
  void buzz(long duration, uint16_t freq) {
    if (freq > 0) {
      #if ENABLED(LCD_USE_I2C_BUZZER)
        lcd_buzz(duration, freq);
      #elif HAS_BUZZER // on-board buzzers have no further condition
        SET_OUTPUT(BEEPER);
        tone(BEEPER, freq, duration);
        delay(duration);
      #else
        delay(duration);
      #endif
    }
    else {
      delay(duration);
    }
  }
#endif
