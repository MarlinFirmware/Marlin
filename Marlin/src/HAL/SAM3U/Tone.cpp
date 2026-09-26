/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

/**
 * Tone for Atmel SAM3U (AT91SAM3U4E), driven from TC0 channel 2.
 */

#ifdef __SAM3U4E__

#include "../../inc/MarlinConfig.h"
#include "HAL.h"

#if HAS_SOUND

static pin_t tone_pin;
volatile static int32_t toggles;

void tone(const pin_t _pin, const unsigned int frequency, const unsigned long duration/*=0*/) {
  if (!frequency) { noTone(_pin); return; }
  tone_pin = _pin;
  toggles = 2 * frequency * duration / 1000;
  HAL_timer_start(MF_TIMER_TONE, 2 * frequency);
  HAL_timer_enable_interrupt(MF_TIMER_TONE);
}

void noTone(const pin_t _pin) {
  HAL_timer_disable_interrupt(MF_TIMER_TONE);
  extDigitalWrite(_pin, LOW);
}

HAL_TONE_TIMER_ISR() {
  static uint8_t pin_state = 0;
  HAL_timer_isr_prologue(MF_TIMER_TONE);

  if (toggles) {
    toggles--;
    extDigitalWrite(tone_pin, (pin_state ^= 1));
  }
  else
    noTone(tone_pin);   // Stop the interrupt
}

#endif // HAS_SOUND
#endif // __SAM3U4E__
