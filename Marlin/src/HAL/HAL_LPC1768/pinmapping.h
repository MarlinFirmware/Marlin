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

typedef int16_t pin_t;

const uint8_t PIN_FEATURE_INTERRUPT = 1 << 0;
const uint8_t PIN_FEATURE_PWM = 1 << 1;
constexpr uint8_t PIN_FEATURE_ADC(const int8_t chan) { return (((chan + 1) & 0b1111) << 2); }

constexpr pin_t LPC1768_PIN(const uint8_t port, const uint8_t pin, const uint8_t feat = 0) {
  return (((pin_t)feat << 8) | (((pin_t)port & 0x7) << 5) | ((pin_t)pin & 0x1F));
}

constexpr uint8_t LPC1768_PIN_PORT(const pin_t pin) { return ((uint8_t)((pin >> 5) & 0b111)); }
constexpr uint8_t LPC1768_PIN_PIN(const pin_t pin) { return ((uint8_t)(pin & 0b11111)); }
constexpr bool LPC1768_PIN_INTERRUPT(const pin_t pin) { return (((pin >> 8) & PIN_FEATURE_INTERRUPT) != 0); }
constexpr bool LPC1768_PIN_PWM(const pin_t pin) { return (((pin >> 8) & PIN_FEATURE_PWM) != 0); }
constexpr int8_t LPC1768_PIN_ADC(const pin_t pin) { return (int8_t)((pin >> 8) & 0b1111) - 1; }

// ******************
// Runtime pinmapping
// ******************
const pin_t P0_0  = LPC1768_PIN(0,  0, PIN_FEATURE_INTERRUPT);
const pin_t P0_1  = LPC1768_PIN(0,  1, PIN_FEATURE_INTERRUPT);
const pin_t P0_2  = LPC1768_PIN(0,  2, PIN_FEATURE_INTERRUPT | PIN_FEATURE_ADC(7));
const pin_t P0_3  = LPC1768_PIN(0,  3, PIN_FEATURE_INTERRUPT | PIN_FEATURE_ADC(6));
const pin_t P0_4  = LPC1768_PIN(0,  4, PIN_FEATURE_INTERRUPT);
const pin_t P0_5  = LPC1768_PIN(0,  5, PIN_FEATURE_INTERRUPT);
const pin_t P0_6  = LPC1768_PIN(0,  6);
const pin_t P0_7  = LPC1768_PIN(0,  7, PIN_FEATURE_INTERRUPT);
const pin_t P0_8  = LPC1768_PIN(0,  8, PIN_FEATURE_INTERRUPT);
const pin_t P0_9  = LPC1768_PIN(0,  9, PIN_FEATURE_INTERRUPT);
const pin_t P0_10 = LPC1768_PIN(0, 10, PIN_FEATURE_INTERRUPT);
const pin_t P0_11 = LPC1768_PIN(0, 11, PIN_FEATURE_INTERRUPT);
const pin_t P0_12 = LPC1768_PIN(0, 12, PIN_FEATURE_INTERRUPT);
const pin_t P0_13 = LPC1768_PIN(0, 13);
const pin_t P0_14 = LPC1768_PIN(0, 14, PIN_FEATURE_INTERRUPT);
const pin_t P0_15 = LPC1768_PIN(0, 15, PIN_FEATURE_INTERRUPT);
const pin_t P0_16 = LPC1768_PIN(0, 16, PIN_FEATURE_INTERRUPT);
const pin_t P0_17 = LPC1768_PIN(0, 17, PIN_FEATURE_INTERRUPT);
const pin_t P0_18 = LPC1768_PIN(0, 18, PIN_FEATURE_INTERRUPT);
const pin_t P0_19 = LPC1768_PIN(0, 19, PIN_FEATURE_INTERRUPT);
const pin_t P0_20 = LPC1768_PIN(0, 20, PIN_FEATURE_INTERRUPT);
const pin_t P0_21 = LPC1768_PIN(0, 21, PIN_FEATURE_INTERRUPT);
const pin_t P0_22 = LPC1768_PIN(0, 22, PIN_FEATURE_INTERRUPT);
const pin_t P0_23 = LPC1768_PIN(0, 23, PIN_FEATURE_INTERRUPT | PIN_FEATURE_ADC(0));
const pin_t P0_24 = LPC1768_PIN(0, 24, PIN_FEATURE_INTERRUPT | PIN_FEATURE_ADC(1));
const pin_t P0_25 = LPC1768_PIN(0, 25, PIN_FEATURE_INTERRUPT | PIN_FEATURE_ADC(2));
const pin_t P0_26 = LPC1768_PIN(0, 26, PIN_FEATURE_INTERRUPT | PIN_FEATURE_ADC(3));
const pin_t P0_27 = LPC1768_PIN(0, 27, PIN_FEATURE_INTERRUPT);
const pin_t P0_28 = LPC1768_PIN(0, 28, PIN_FEATURE_INTERRUPT);
const pin_t P0_29 = LPC1768_PIN(0, 29);
const pin_t P0_30 = LPC1768_PIN(0, 30);
const pin_t P1_0  = LPC1768_PIN(1,  0);
const pin_t P1_1  = LPC1768_PIN(1,  1);
const pin_t P1_2  = LPC1768_PIN(1,  2);
const pin_t P1_3  = LPC1768_PIN(1,  3);
const pin_t P1_4  = LPC1768_PIN(1,  4);
const pin_t P1_5  = LPC1768_PIN(1,  5);
const pin_t P1_6  = LPC1768_PIN(1,  6);
const pin_t P1_7  = LPC1768_PIN(1,  7);
const pin_t P1_8  = LPC1768_PIN(1,  8);
const pin_t P1_9  = LPC1768_PIN(1,  9);
const pin_t P1_10 = LPC1768_PIN(1, 10);
const pin_t P1_11 = LPC1768_PIN(1, 11);
const pin_t P1_12 = LPC1768_PIN(1, 12);
const pin_t P1_13 = LPC1768_PIN(1, 13);
const pin_t P1_14 = LPC1768_PIN(1, 14);
const pin_t P1_15 = LPC1768_PIN(1, 15);
const pin_t P1_16 = LPC1768_PIN(1, 16);
const pin_t P1_17 = LPC1768_PIN(1, 17);
const pin_t P1_18 = LPC1768_PIN(1, 18, PIN_FEATURE_PWM);
const pin_t P1_19 = LPC1768_PIN(1, 19);
const pin_t P1_20 = LPC1768_PIN(1, 20, PIN_FEATURE_PWM);
const pin_t P1_21 = LPC1768_PIN(1, 21, PIN_FEATURE_PWM);
const pin_t P1_22 = LPC1768_PIN(1, 22);
const pin_t P1_23 = LPC1768_PIN(1, 23, PIN_FEATURE_PWM);
const pin_t P1_24 = LPC1768_PIN(1, 24, PIN_FEATURE_PWM);
const pin_t P1_25 = LPC1768_PIN(1, 25);
const pin_t P1_26 = LPC1768_PIN(1, 26, PIN_FEATURE_PWM);
const pin_t P1_27 = LPC1768_PIN(1, 27);
const pin_t P1_28 = LPC1768_PIN(1, 28);
const pin_t P1_29 = LPC1768_PIN(1, 29);
const pin_t P1_30 = LPC1768_PIN(1, 30, PIN_FEATURE_ADC(4));
const pin_t P1_31 = LPC1768_PIN(1, 31, PIN_FEATURE_ADC(5));
const pin_t P2_0  = LPC1768_PIN(2,  0, PIN_FEATURE_INTERRUPT | PIN_FEATURE_PWM);
const pin_t P2_1  = LPC1768_PIN(2,  1, PIN_FEATURE_INTERRUPT | PIN_FEATURE_PWM);
const pin_t P2_2  = LPC1768_PIN(2,  2, PIN_FEATURE_INTERRUPT | PIN_FEATURE_PWM);
const pin_t P2_3  = LPC1768_PIN(2,  3, PIN_FEATURE_INTERRUPT | PIN_FEATURE_PWM);
const pin_t P2_4  = LPC1768_PIN(2,  4, PIN_FEATURE_INTERRUPT | PIN_FEATURE_PWM);
const pin_t P2_5  = LPC1768_PIN(2,  5, PIN_FEATURE_INTERRUPT | PIN_FEATURE_PWM);
const pin_t P2_6  = LPC1768_PIN(2,  6, PIN_FEATURE_INTERRUPT);
const pin_t P2_7  = LPC1768_PIN(2,  7, PIN_FEATURE_INTERRUPT);
const pin_t P2_8  = LPC1768_PIN(2,  8, PIN_FEATURE_INTERRUPT);
const pin_t P2_9  = LPC1768_PIN(2,  9);
const pin_t P2_10 = LPC1768_PIN(2, 10);
const pin_t P2_11 = LPC1768_PIN(2, 11, PIN_FEATURE_INTERRUPT);
const pin_t P2_12 = LPC1768_PIN(2, 12);
const pin_t P2_13 = LPC1768_PIN(2, 13, PIN_FEATURE_INTERRUPT);
const pin_t P3_25 = LPC1768_PIN(3, 25);
const pin_t P3_26 = LPC1768_PIN(3, 26);
const pin_t P4_28 = LPC1768_PIN(4, 28);
const pin_t P4_29 = LPC1768_PIN(4, 29);

constexpr bool VALID_PIN(const pin_t p) {
  return ((LPC1768_PIN_PORT(p) == 0 && LPC1768_PIN_PIN(p) <= 30) ||
          (LPC1768_PIN_PORT(p) == 1 && LPC1768_PIN_PIN(p) <= 31) ||
          (LPC1768_PIN_PORT(p) == 2 && LPC1768_PIN_PIN(p) <= 13) ||
          (LPC1768_PIN_PORT(p) == 3 && WITHIN(LPC1768_PIN_PIN(p), 25, 26)) ||
          (LPC1768_PIN_PORT(p) == 4 && WITHIN(LPC1768_PIN_PIN(p), 28, 29)));
}

constexpr bool PWM_PIN(const pin_t p) {
  return (VALID_PIN(p) && LPC1768_PIN_PWM(p));
}

constexpr bool INTERRUPT_PIN(const pin_t p) {
  return (VALID_PIN(p) && LPC1768_PIN_INTERRUPT(p));
}

#define NUM_ANALOG_INPUTS 8

constexpr pin_t adc_pin_table[] = { P0_23, P0_24, P0_25, P0_26, P1_30, P1_31, P0_3, P0_2 };

constexpr pin_t analogInputToDigitalPin(const uint8_t p) {
  return (p < COUNT(adc_pin_table) ? adc_pin_table[p] : -1);
}

constexpr int8_t DIGITAL_PIN_TO_ANALOG_PIN(const pin_t p) {
  return (VALID_PIN(p) ? LPC1768_PIN_ADC(p) : -1);
}

// P0.6 thru P0.9 are for the onboard SD card
// P0.29 and P0.30 are for the USB port
#define HAL_SENSITIVE_PINS P0_6, P0_7, P0_8, P0_9, P0_29, P0_30

// Pin map for M43 and M226
const pin_t pin_map[] = {
  P0_0,  P0_1,  P0_2,  P0_3,  P0_4,  P0_5,  P0_6,  P0_7,  P0_8,  P0_9,
  P0_10, P0_11, P0_12, P0_13, P0_14, P0_15, P0_16, P0_17, P0_18, P0_19,
  P0_20, P0_21, P0_22, P0_23, P0_24, P0_25, P0_26, P0_27, P0_28, P0_29,
  P0_30,
  P1_0,  P1_1,  P1_2,  P1_3,  P1_4,  P1_5,  P1_6,  P1_7,  P1_8,  P1_9,
  P1_10, P1_11, P1_12, P1_13, P1_14, P1_15, P1_16, P1_17, P1_18, P1_19,
  P1_20, P1_21, P1_22, P1_23, P1_24, P1_25, P1_26, P1_27, P1_28, P1_29,
  P1_30, P1_31,
  P2_0,  P2_1,  P2_2,  P2_3,  P2_4,  P2_5,  P2_6,  P2_7,  P2_8,  P2_9,
  P2_10, P2_11, P2_12, P2_13,
  P3_25, P3_26,
  P4_28, P4_29
};

#define NUM_DIGITAL_PINS COUNT(pin_map)

#define GET_PIN_MAP_PIN(index) pin_map[index]
                           
#endif // __HAL_PINMAPPING_H__
