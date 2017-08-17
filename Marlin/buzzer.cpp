/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "Marlin.h"
#if HAS_BUZZER
  #include "buzzer.h"
  #include "ultralcd.h"

  void buzz(long duration, uint16_t freq) {
    if (freq > 0) {
      #if ENABLED(LCD_USE_I2C_BUZZER)
        lcd_buzz(duration, freq);
      #elif PIN_EXISTS(BEEPER) // on-board buzzers have no further condition
        SET_OUTPUT(BEEPER_PIN);
        #if ENABLED(SPEAKER) // a speaker needs a AC ore a pulsed DC
          //tone(BEEPER_PIN, freq, duration); // needs a PWMable pin
          unsigned int delay = 1000000 / freq / 2;
          int i = duration * freq / 1000;
          while (i--) {
            WRITE(BEEPER_PIN, HIGH);
            delayMicroseconds(delay);
            WRITE(BEEPER_PIN, LOW);
            delayMicroseconds(delay);
           }
        #else // buzzer has its own resonator - needs a DC
          WRITE(BEEPER_PIN, HIGH);
          delay(duration);
          WRITE(BEEPER_PIN, LOW);
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
