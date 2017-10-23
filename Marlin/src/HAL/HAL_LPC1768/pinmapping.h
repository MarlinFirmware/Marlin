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

#ifndef __HAL_PINMAPPING_H__
#define __HAL_PINMAPPING_H__
#include "../../core/macros.h"

struct pin_data { uint8_t port, pin; };
struct adc_pin_data { uint8_t port, pin, adc; };

#define LPC1768_PIN(port, pin) (((port & 0x7) << 5) | (pin & 0x1F))
#define LPC1768_PIN_PORT(pin) ((uint8_t)((pin >> 5) & 0x7))
#define LPC1768_PIN_PIN(pin) ((uint8_t)(pin & 0x1F))

// ******************
// Runtime pinmapping
// ******************

#if SERIAL_PORT == 0
  #define NUM_ANALOG_INPUTS 6
#else
  #define NUM_ANALOG_INPUTS 8
#endif

const adc_pin_data adc_pin_map[] = {
  {0, 23, 0},         //A0 (T0) - D67 - TEMP_0_PIN
  {0, 24, 1},         //A1 (T1) - D68 - TEMP_BED_PIN
  {0, 25, 2},         //A2 (T2) - D69 - TEMP_1_PIN
  {0, 26, 3},         //A3 - D63
  {1, 30, 4},         //A4 - D37 - BUZZER_PIN
  {1, 31, 5},         //A5 - D49 - SD_DETECT_PIN
  #if SERIAL_PORT != 0
    {0,  3, 6},       //A6 - D0  - RXD0
    {0,  2, 7}        //A7 - D1  - TXD0
  #endif
};

constexpr FORCE_INLINE int8_t analogInputToDigitalPin(int8_t p) {
  return (p == 0 ? LPC1768_PIN(0, 23):
          p == 1 ? LPC1768_PIN(0, 24):
          p == 2 ? LPC1768_PIN(0, 25):
          p == 3 ? LPC1768_PIN(0, 26):
          p == 4 ? LPC1768_PIN(1, 30):
          p == 5 ? LPC1768_PIN(1, 31):
          #if SERIAL_PORT != 0
            p == 6 ?  LPC1768_PIN(0,  3):
            p == 7 ?  LPC1768_PIN(0,  2):
          #endif
          -1);
}

constexpr FORCE_INLINE int8_t DIGITAL_PIN_TO_ANALOG_PIN(int8_t p) {
  return (p == LPC1768_PIN(0, 23) ? 0:
          p == LPC1768_PIN(0, 24) ? 1:
          p == LPC1768_PIN(0, 25) ? 2:
          p == LPC1768_PIN(0, 26) ? 3:
          p == LPC1768_PIN(1, 30) ? 4:
          p == LPC1768_PIN(1, 31) ? 5:
          #if SERIAL_PORT != 0
            p == LPC1768_PIN(0,  3)  ? 6:
            p == LPC1768_PIN(0,  2)  ? 7:
          #endif
          -1);
}

// This is large enough to encompass from 0.0 through 4.29
#define NUM_DIGITAL_PINS 158

constexpr FORCE_INLINE uint8_t VALID_PIN(int8_t r) {
  return (    
    #if SERIAL_PORT == 0
      (LPC1768_PIN_PORT(r) == 0 && (LPC1768_PIN_PIN(r) == 2 || LPC1768_PIN_PIN(r) == 3)) ? 0 :
    #endif
    (LPC1768_PIN_PORT(r) == 0 && LPC1768_PIN_PIN(r) <= 30) ? 1 :
    (LPC1768_PIN_PORT(r) == 1 && LPC1768_PIN_PIN(r) <= 31) ? 1 :
    (LPC1768_PIN_PORT(r) == 2 && LPC1768_PIN_PIN(r) <= 13) ? 1 :
    (LPC1768_PIN_PORT(r) == 3 && LPC1768_PIN_PIN(r) >= 25 && LPC1768_PIN_PIN(r) <= 26) ? 1 :
    (LPC1768_PIN_PORT(r) == 4 && LPC1768_PIN_PIN(r) >= 28 && LPC1768_PIN_PIN(r) <= 29) ? 1 :
    0);
}

#define PWM_PIN(r) (r == LPC1768_PIN(1, 18) ? 1 :\
                  r == LPC1768_PIN(1, 20) ? 1 :\
                  r == LPC1768_PIN(1, 21) ? 1 :\
                  r == LPC1768_PIN(1, 23) ? 1 :\
                  r == LPC1768_PIN(1, 24) ? 1 :\
                  r == LPC1768_PIN(1, 26) ? 1 :\
                  r == LPC1768_PIN(2,  0) ? 1 :\
                  r == LPC1768_PIN(2,  1) ? 1 :\
                  r == LPC1768_PIN(2,  2) ? 1 :\
                  r == LPC1768_PIN(2,  3) ? 1 :\
                  r == LPC1768_PIN(2,  4) ? 1 :\
                  r == LPC1768_PIN(2,  5) ? 1 : 0)

#define NUM_INTERRUPT_PINS 34

#define INTERRUPT_PIN(r) (r == LPC1768_PIN(0,  0) ? 1 :\
                        r == LPC1768_PIN(0,  1) ? 1 :\
                        r == LPC1768_PIN(0,  2) ? 1 :\
                        r == LPC1768_PIN(0,  3) ? 1 :\
                        r == LPC1768_PIN(0,  4) ? 1 :\
                        r == LPC1768_PIN(0,  5) ? 1 :\
                        r == LPC1768_PIN(0, 10) ? 1 :\
                        r == LPC1768_PIN(0, 11) ? 1 :\
                        r == LPC1768_PIN(2, 12) ? 1 :\
                        r == LPC1768_PIN(0, 15) ? 1 :\
                        r == LPC1768_PIN(0, 16) ? 1 :\
                        r == LPC1768_PIN(0, 17) ? 1 :\
                        r == LPC1768_PIN(0, 18) ? 1 :\
                        r == LPC1768_PIN(0, 19) ? 1 :\
                        r == LPC1768_PIN(0, 20) ? 1 :\
                        r == LPC1768_PIN(0, 21) ? 1 :\
                        r == LPC1768_PIN(0, 22) ? 1 :\
                        r == LPC1768_PIN(0, 23) ? 1 :\
                        r == LPC1768_PIN(0, 24) ? 1 :\
                        r == LPC1768_PIN(0, 25) ? 1 :\
                        r == LPC1768_PIN(0, 26) ? 1 :\
                        r == LPC1768_PIN(0, 27) ? 1 :\
                        r == LPC1768_PIN(0, 28) ? 1 :\
                        r == LPC1768_PIN(2,  0) ? 1 :\
                        r == LPC1768_PIN(2,  1) ? 1 :\
                        r == LPC1768_PIN(2,  2) ? 1 :\
                        r == LPC1768_PIN(2,  3) ? 1 :\
                        r == LPC1768_PIN(2,  4) ? 1 :\
                        r == LPC1768_PIN(2,  5) ? 1 :\
                        r == LPC1768_PIN(2,  6) ? 1 :\
                        r == LPC1768_PIN(2,  7) ? 1 :\
                        r == LPC1768_PIN(2,  8) ? 1 :\
                        r == LPC1768_PIN(2, 11) ? 1 :\
                        r == LPC1768_PIN(2, 13) ? 1 : 0)
                        /*Internal SD Card */
                        /* r == LPC1768_PIN(0, 14) ? 1 :\
                           r == LPC1768_PIN(0,  7) ? 1 :\
                           r == LPC1768_PIN(0,  8) ? 1 :\
                           r == LPC1768_PIN(0,  9) ? 1 :\ */

#endif // __HAL_PINMAPPING_H__
