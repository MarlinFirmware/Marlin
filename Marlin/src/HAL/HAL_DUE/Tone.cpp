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

/**
 * Description: Tone function for Arduino Due and compatible (SAM3X8E)
 * Derived from http://forum.arduino.cc/index.php?topic=136500.msg2903012#msg2903012
 */

#ifdef ARDUINO_ARCH_SAM

#include "HAL_Due.h"
#include "HAL_timers_Due.h"

static pin_t tone_pin;
volatile static int32_t toggles;

void toneInit() {
  HAL_timer_start(TONE_TIMER_NUM, 1); // Lowest frequency possible
}

void tone(const pin_t _pin, const unsigned int frequency, const unsigned long duration) {
  tone_pin = _pin;
  toggles = 2 * frequency * duration / 1000;
  HAL_timer_set_compare(TONE_TIMER_NUM, VARIANT_MCK / 2 / frequency); // 84MHz / 2 prescaler / Hz
  HAL_timer_enable_interrupt(TONE_TIMER_NUM);
}

void noTone(const pin_t _pin) {
  HAL_timer_disable_interrupt(TONE_TIMER_NUM);
  digitalWrite(_pin, LOW);
}

HAL_TONE_TIMER_ISR {
  static uint8_t pin_state = 0;
  HAL_timer_isr_prologue(TONE_TIMER_NUM);
  if (toggles) {
    toggles--;
    digitalWrite(tone_pin, (pin_state ^= 1));
  }
  else noTone(tone_pin);                                  // seems superfluous ?
}

#endif // ARDUINO_ARCH_SAM
