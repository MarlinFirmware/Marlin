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
 * Fast I/O for extended pins
 */

#ifdef __AVR__

#include "fastio.h"

#ifdef FASTIO_EXT_START

#include "../shared/Marduino.h"

#define _IS_EXT(P) WITHIN(P, FASTIO_EXT_START, FASTIO_EXT_END)

void extDigitalWrite(const int8_t pin, const uint8_t state) {
  #define _WCASE(N) case N: WRITE(N, state); break
  switch (pin) {
    default: digitalWrite(pin, state);
    #if _IS_EXT(70)
      _WCASE(70);
    #endif
    #if _IS_EXT(71)
      _WCASE(71);
    #endif
    #if _IS_EXT(72)
      _WCASE(72);
    #endif
    #if _IS_EXT(73)
      _WCASE(73);
    #endif
    #if _IS_EXT(74)
      _WCASE(74);
    #endif
    #if _IS_EXT(75)
      _WCASE(75);
    #endif
    #if _IS_EXT(76)
      _WCASE(76);
    #endif
    #if _IS_EXT(77)
      _WCASE(77);
    #endif
    #if _IS_EXT(78)
      _WCASE(78);
    #endif
    #if _IS_EXT(79)
      _WCASE(79);
    #endif
    #if _IS_EXT(80)
      _WCASE(80);
    #endif
    #if _IS_EXT(81)
      _WCASE(81);
    #endif
    #if _IS_EXT(82)
      _WCASE(82);
    #endif
    #if _IS_EXT(83)
      _WCASE(83);
    #endif
    #if _IS_EXT(84)
      _WCASE(84);
    #endif
    #if _IS_EXT(85)
      _WCASE(85);
    #endif
    #if _IS_EXT(86)
      _WCASE(86);
    #endif
    #if _IS_EXT(87)
      _WCASE(87);
    #endif
    #if _IS_EXT(88)
      _WCASE(88);
    #endif
    #if _IS_EXT(89)
      _WCASE(89);
    #endif
    #if _IS_EXT(90)
      _WCASE(90);
    #endif
    #if _IS_EXT(91)
      _WCASE(91);
    #endif
    #if _IS_EXT(92)
      _WCASE(92);
    #endif
    #if _IS_EXT(93)
      _WCASE(93);
    #endif
    #if _IS_EXT(94)
      _WCASE(94);
    #endif
    #if _IS_EXT(95)
      _WCASE(95);
    #endif
    #if _IS_EXT(96)
      _WCASE(96);
    #endif
    #if _IS_EXT(97)
      _WCASE(97);
    #endif
    #if _IS_EXT(98)
      _WCASE(98);
    #endif
    #if _IS_EXT(99)
      _WCASE(99);
    #endif
    #if _IS_EXT(100)
      _WCASE(100);
    #endif
  }
}

uint8_t extDigitalRead(const int8_t pin) {
  #define _RCASE(N) case N: return READ(N)
  switch (pin) {
    default: return digitalRead(pin);
    #if _IS_EXT(70)
      _RCASE(70);
    #endif
    #if _IS_EXT(71)
      _RCASE(71);
    #endif
    #if _IS_EXT(72)
      _RCASE(72);
    #endif
    #if _IS_EXT(73)
      _RCASE(73);
    #endif
    #if _IS_EXT(74)
      _RCASE(74);
    #endif
    #if _IS_EXT(75)
      _RCASE(75);
    #endif
    #if _IS_EXT(76)
      _RCASE(76);
    #endif
    #if _IS_EXT(77)
      _RCASE(77);
    #endif
    #if _IS_EXT(78)
      _RCASE(78);
    #endif
    #if _IS_EXT(79)
      _RCASE(79);
    #endif
    #if _IS_EXT(80)
      _RCASE(80);
    #endif
    #if _IS_EXT(81)
      _RCASE(81);
    #endif
    #if _IS_EXT(82)
      _RCASE(82);
    #endif
    #if _IS_EXT(83)
      _RCASE(83);
    #endif
    #if _IS_EXT(84)
      _RCASE(84);
    #endif
    #if _IS_EXT(85)
      _RCASE(85);
    #endif
    #if _IS_EXT(86)
      _RCASE(86);
    #endif
    #if _IS_EXT(87)
      _RCASE(87);
    #endif
    #if _IS_EXT(88)
      _RCASE(88);
    #endif
    #if _IS_EXT(89)
      _RCASE(89);
    #endif
    #if _IS_EXT(90)
      _RCASE(90);
    #endif
    #if _IS_EXT(91)
      _RCASE(91);
    #endif
    #if _IS_EXT(92)
      _RCASE(92);
    #endif
    #if _IS_EXT(93)
      _RCASE(93);
    #endif
    #if _IS_EXT(94)
      _RCASE(94);
    #endif
    #if _IS_EXT(95)
      _RCASE(95);
    #endif
    #if _IS_EXT(96)
      _RCASE(96);
    #endif
    #if _IS_EXT(97)
      _RCASE(97);
    #endif
    #if _IS_EXT(98)
      _RCASE(98);
    #endif
    #if _IS_EXT(99)
      _RCASE(99);
    #endif
    #if _IS_EXT(100)
      _RCASE(100);
    #endif
  }
}

#if 0
/**
 * Set Timer 5 PWM frequency in Hz, from 3.8Hz up to ~16MHz
 * with a minimum resolution of 100 steps.
 *
 * DC values -1.0 to 1.0. Negative duty cycle inverts the pulse.
 */
uint16_t set_pwm_frequency_hz(const_float_t hz, const float dca, const float dcb, const float dcc) {
  float count = 0;
  if (hz > 0 && (dca || dcb || dcc)) {
    count = float(F_CPU) / hz;            // 1x prescaler, TOP for 16MHz base freq.
    uint16_t prescaler;                   // Range of 30.5Hz (65535) 64.5KHz (>31)

         if (count >= 255. * 256.) { prescaler = 1024; SET_CS(5, PRESCALER_1024); }
    else if (count >= 255. * 64.)  { prescaler = 256;  SET_CS(5,  PRESCALER_256); }
    else if (count >= 255. * 8.)   { prescaler = 64;   SET_CS(5,   PRESCALER_64); }
    else if (count >= 255.)        { prescaler = 8;    SET_CS(5,    PRESCALER_8); }
    else                           { prescaler = 1;    SET_CS(5,    PRESCALER_1); }

    count /= float(prescaler);
    const float pwm_top = round(count);   // Get the rounded count

    ICR5 = (uint16_t)pwm_top - 1;         // Subtract 1 for TOP
    OCR5A = pwm_top * ABS(dca);          // Update and scale DCs
    OCR5B = pwm_top * ABS(dcb);
    OCR5C = pwm_top * ABS(dcc);
    _SET_COM(5, A, dca ? (dca < 0 ? COM_SET_CLEAR : COM_CLEAR_SET) : COM_NORMAL); // Set compare modes
    _SET_COM(5, B, dcb ? (dcb < 0 ? COM_SET_CLEAR : COM_CLEAR_SET) : COM_NORMAL);
    _SET_COM(5, C, dcc ? (dcc < 0 ? COM_SET_CLEAR : COM_CLEAR_SET) : COM_NORMAL);

    SET_WGM(5, FAST_PWM_ICRn);            // Fast PWM with ICR5 as TOP

    //SERIAL_ECHOLNPGM("Timer 5 Settings:");
    //SERIAL_ECHOLNPAIR("  Prescaler=", prescaler);
    //SERIAL_ECHOLNPAIR("        TOP=", ICR5);
    //SERIAL_ECHOLNPAIR("      OCR5A=", OCR5A);
    //SERIAL_ECHOLNPAIR("      OCR5B=", OCR5B);
    //SERIAL_ECHOLNPAIR("      OCR5C=", OCR5C);
  }
  else {
    // Restore the default for Timer 5
    SET_WGM(5, PWM_PC_8);                 // PWM 8-bit (Phase Correct)
    SET_COMS(5, NORMAL, NORMAL, NORMAL);  // Do nothing
    SET_CS(5, PRESCALER_64);              // 16MHz / 64 = 250KHz
    OCR5A = OCR5B = OCR5C = 0;
  }
  return round(count);
}
#endif

#endif // FASTIO_EXT_START
#endif // __AVR__
