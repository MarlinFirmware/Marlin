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

#ifndef _PINMAPPING_H_
#define _PINMAPPING_H_

#include "../../inc/MarlinConfigPre.h"

#include <stdint.h>

typedef int16_t pin_t;

#define PORT_0  000
#define PORT_1  001
#define PORT_2  010
#define PORT_3  011
#define PORT_4  100

#define PORT_(p)  PORT_##p
#define PORT(p)   PORT_(p)

#define PIN_0  00000
#define PIN_1  00001
#define PIN_2  00010
#define PIN_3  00011
#define PIN_4  00100
#define PIN_5  00101
#define PIN_6  00110
#define PIN_7  00111
#define PIN_8  01000
#define PIN_9  01001
#define PIN_10 01010
#define PIN_11 01011
#define PIN_12 01100
#define PIN_13 01101
#define PIN_14 01110
#define PIN_15 01111
#define PIN_16 10000
#define PIN_17 10001
#define PIN_18 10010
#define PIN_19 10011
#define PIN_20 10100
#define PIN_21 10101
#define PIN_22 10110
#define PIN_23 10111
#define PIN_24 11000
#define PIN_25 11001
#define PIN_26 11010
#define PIN_27 11011
#define PIN_28 11100
#define PIN_29 11101
#define PIN_30 11110
#define PIN_31 11111

#define PIN_(p) PIN_##p
#define PIN(p)  PIN_(p)

#define ADC_NONE    0000
#define ADC_CHAN_0  0001
#define ADC_CHAN_1  0010
#define ADC_CHAN_2  0011
#define ADC_CHAN_3  0100
#define ADC_CHAN_4  0101
#define ADC_CHAN_5  0110
#define ADC_CHAN_6  0111
#define ADC_CHAN_7  1000

#define ADC_CHAN_(c)  ADC_CHAN_##c
#define ADC_CHAN(p)   ADC_CHAN_(p)

#define BOOL_0 0
#define BOOL_1 1
#define BOOL_(b)      BOOL_##b

#define INTERRUPT(b)  BOOL_(b)
#define PWM(b)        BOOL_(b)

// Combine elements into pin bits: 0b00AAAAWIPPPNNNNN
#define LPC1768_PIN_(port, pin, int, pwm, adc)  0b00##adc##pwm##int##port##pin
#define LPC1768_PIN(port, pin, int, pwm, adc)   LPC1768_PIN_(port, pin, int, pwm, adc)

constexpr uint8_t LPC1768_PIN_PORT(const pin_t pin) { return ((uint8_t)((pin >> 5) & 0b111)); }
constexpr uint8_t LPC1768_PIN_PIN(const pin_t pin) { return ((uint8_t)(pin & 0b11111)); }
constexpr bool LPC1768_PIN_INTERRUPT(const pin_t pin) { return (((pin >> 8) & 0b1) != 0); }
constexpr bool LPC1768_PIN_PWM(const pin_t pin) { return (((pin >> 9) & 0b1) != 0); }
constexpr int8_t LPC1768_PIN_ADC(const pin_t pin) { return (int8_t)((pin >> 10) & 0b1111) - 1; }

// ******************
// Runtime pinmapping
// ******************
#define P_NC -1

#if SERIAL_PORT != 3 && SERIAL_PORT_2 != 3
  #define P0_00 LPC1768_PIN(PORT(0), PIN( 0), INTERRUPT(1), PWM(0), ADC_NONE)
  #define P0_01 LPC1768_PIN(PORT(0), PIN( 1), INTERRUPT(1), PWM(0), ADC_NONE)
#endif
#if SERIAL_PORT != 0 && SERIAL_PORT_2 != 0
  #define P0_02 LPC1768_PIN(PORT(0), PIN( 2), INTERRUPT(1), PWM(0), ADC_CHAN(7))
  #define P0_03 LPC1768_PIN(PORT(0), PIN( 3), INTERRUPT(1), PWM(0), ADC_CHAN(6))
#endif
#define P0_04   LPC1768_PIN(PORT(0), PIN( 4), INTERRUPT(1), PWM(0), ADC_NONE)
#define P0_05   LPC1768_PIN(PORT(0), PIN( 5), INTERRUPT(1), PWM(0), ADC_NONE)
#define P0_06   LPC1768_PIN(PORT(0), PIN( 6), INTERRUPT(1), PWM(0), ADC_NONE)
#define P0_07   LPC1768_PIN(PORT(0), PIN( 7), INTERRUPT(1), PWM(0), ADC_NONE)
#define P0_08   LPC1768_PIN(PORT(0), PIN( 8), INTERRUPT(1), PWM(0), ADC_NONE)
#define P0_09   LPC1768_PIN(PORT(0), PIN( 9), INTERRUPT(1), PWM(0), ADC_NONE)
#if SERIAL_PORT != 2 && SERIAL_PORT_2 != 2
  #define P0_10 LPC1768_PIN(PORT(0), PIN(10), INTERRUPT(1), PWM(0), ADC_NONE)
  #define P0_11 LPC1768_PIN(PORT(0), PIN(11), INTERRUPT(1), PWM(0), ADC_NONE)
#endif
#if SERIAL_PORT != 1 && SERIAL_PORT_2 != 1
  #define P0_15 LPC1768_PIN(PORT(0), PIN(15), INTERRUPT(1), PWM(0), ADC_NONE)
  #define P0_16 LPC1768_PIN(PORT(0), PIN(16), INTERRUPT(1), PWM(0), ADC_NONE)
#endif
#define P0_17   LPC1768_PIN(PORT(0), PIN(17), INTERRUPT(1), PWM(0), ADC_NONE)
#define P0_18   LPC1768_PIN(PORT(0), PIN(18), INTERRUPT(1), PWM(0), ADC_NONE)
#define P0_19   LPC1768_PIN(PORT(0), PIN(19), INTERRUPT(1), PWM(0), ADC_NONE)
#define P0_20   LPC1768_PIN(PORT(0), PIN(20), INTERRUPT(1), PWM(0), ADC_NONE)
#define P0_21   LPC1768_PIN(PORT(0), PIN(21), INTERRUPT(1), PWM(0), ADC_NONE)
#define P0_22   LPC1768_PIN(PORT(0), PIN(22), INTERRUPT(1), PWM(0), ADC_NONE)
#define P0_23   LPC1768_PIN(PORT(0), PIN(23), INTERRUPT(1), PWM(0), ADC_CHAN(0))
#define P0_24   LPC1768_PIN(PORT(0), PIN(24), INTERRUPT(1), PWM(0), ADC_CHAN(1))
#define P0_25   LPC1768_PIN(PORT(0), PIN(25), INTERRUPT(1), PWM(0), ADC_CHAN(2))
#define P0_26   LPC1768_PIN(PORT(0), PIN(26), INTERRUPT(1), PWM(0), ADC_CHAN(3))
#define P0_27   LPC1768_PIN(PORT(0), PIN(27), INTERRUPT(1), PWM(0), ADC_NONE)
#define P0_28   LPC1768_PIN(PORT(0), PIN(28), INTERRUPT(1), PWM(0), ADC_NONE)
#if SERIAL_PORT != -1 && SERIAL_PORT_2 != -1
  #define P0_29 LPC1768_PIN(PORT(0), PIN(29), INTERRUPT(1), PWM(0), ADC_NONE)
  #define P0_30 LPC1768_PIN(PORT(0), PIN(30), INTERRUPT(1), PWM(0), ADC_NONE)
#endif
#define P1_00   LPC1768_PIN(PORT(1), PIN( 0), INTERRUPT(0), PWM(0), ADC_NONE)
#define P1_01   LPC1768_PIN(PORT(1), PIN( 1), INTERRUPT(0), PWM(0), ADC_NONE)
#define P1_04   LPC1768_PIN(PORT(1), PIN( 4), INTERRUPT(0), PWM(0), ADC_NONE)
#define P1_08   LPC1768_PIN(PORT(1), PIN( 8), INTERRUPT(0), PWM(0), ADC_NONE)
#define P1_09   LPC1768_PIN(PORT(1), PIN( 9), INTERRUPT(0), PWM(0), ADC_NONE)
#define P1_10   LPC1768_PIN(PORT(1), PIN(10), INTERRUPT(0), PWM(0), ADC_NONE)
#define P1_14   LPC1768_PIN(PORT(1), PIN(14), INTERRUPT(0), PWM(0), ADC_NONE)
#define P1_15   LPC1768_PIN(PORT(1), PIN(15), INTERRUPT(0), PWM(0), ADC_NONE)
#define P1_16   LPC1768_PIN(PORT(1), PIN(16), INTERRUPT(0), PWM(0), ADC_NONE)
#define P1_17   LPC1768_PIN(PORT(1), PIN(17), INTERRUPT(0), PWM(0), ADC_NONE)
#define P1_18   LPC1768_PIN(PORT(1), PIN(18), INTERRUPT(0), PWM(1), ADC_NONE)
#define P1_19   LPC1768_PIN(PORT(1), PIN(19), INTERRUPT(0), PWM(0), ADC_NONE)
#define P1_20   LPC1768_PIN(PORT(1), PIN(20), INTERRUPT(0), PWM(1), ADC_NONE)
#define P1_21   LPC1768_PIN(PORT(1), PIN(21), INTERRUPT(0), PWM(1), ADC_NONE)
#define P1_22   LPC1768_PIN(PORT(1), PIN(22), INTERRUPT(0), PWM(0), ADC_NONE)
#define P1_23   LPC1768_PIN(PORT(1), PIN(23), INTERRUPT(0), PWM(1), ADC_NONE)
#define P1_24   LPC1768_PIN(PORT(1), PIN(24), INTERRUPT(0), PWM(1), ADC_NONE)
#define P1_25   LPC1768_PIN(PORT(1), PIN(25), INTERRUPT(0), PWM(0), ADC_NONE)
#define P1_26   LPC1768_PIN(PORT(1), PIN(26), INTERRUPT(0), PWM(1), ADC_NONE)
#define P1_27   LPC1768_PIN(PORT(1), PIN(27), INTERRUPT(0), PWM(0), ADC_NONE)
#define P1_28   LPC1768_PIN(PORT(1), PIN(28), INTERRUPT(0), PWM(0), ADC_NONE)
#define P1_29   LPC1768_PIN(PORT(1), PIN(29), INTERRUPT(0), PWM(0), ADC_NONE)
#define P1_30   LPC1768_PIN(PORT(1), PIN(30), INTERRUPT(0), PWM(0), ADC_CHAN(4))
#define P1_31   LPC1768_PIN(PORT(1), PIN(31), INTERRUPT(0), PWM(0), ADC_CHAN(5))
#define P2_00   LPC1768_PIN(PORT(2), PIN( 0), INTERRUPT(1), PWM(1), ADC_NONE)
#define P2_01   LPC1768_PIN(PORT(2), PIN( 1), INTERRUPT(1), PWM(1), ADC_NONE)
#define P2_02   LPC1768_PIN(PORT(2), PIN( 2), INTERRUPT(1), PWM(1), ADC_NONE)
#define P2_03   LPC1768_PIN(PORT(2), PIN( 3), INTERRUPT(1), PWM(1), ADC_NONE)
#define P2_04   LPC1768_PIN(PORT(2), PIN( 4), INTERRUPT(1), PWM(1), ADC_NONE)
#define P2_05   LPC1768_PIN(PORT(2), PIN( 5), INTERRUPT(1), PWM(1), ADC_NONE)
#define P2_06   LPC1768_PIN(PORT(2), PIN( 6), INTERRUPT(1), PWM(0), ADC_NONE)
#define P2_07   LPC1768_PIN(PORT(2), PIN( 7), INTERRUPT(1), PWM(0), ADC_NONE)
#define P2_08   LPC1768_PIN(PORT(2), PIN( 8), INTERRUPT(1), PWM(0), ADC_NONE)
#define P2_09   LPC1768_PIN(PORT(2), PIN( 9), INTERRUPT(1), PWM(0), ADC_NONE)
#define P2_10   LPC1768_PIN(PORT(2), PIN(10), INTERRUPT(1), PWM(0), ADC_NONE)
#define P2_11   LPC1768_PIN(PORT(2), PIN(11), INTERRUPT(1), PWM(0), ADC_NONE)
#define P2_12   LPC1768_PIN(PORT(2), PIN(12), INTERRUPT(1), PWM(0), ADC_NONE)
#define P2_13   LPC1768_PIN(PORT(2), PIN(13), INTERRUPT(1), PWM(0), ADC_NONE)
#define P3_25   LPC1768_PIN(PORT(3), PIN(25), INTERRUPT(0), PWM(1), ADC_NONE)
#define P3_26   LPC1768_PIN(PORT(3), PIN(26), INTERRUPT(0), PWM(1), ADC_NONE)
#define P4_28   LPC1768_PIN(PORT(4), PIN(28), INTERRUPT(0), PWM(0), ADC_NONE)
#define P4_29   LPC1768_PIN(PORT(4), PIN(29), INTERRUPT(0), PWM(0), ADC_NONE)

// Pin index for M43 and M226
constexpr pin_t pin_map[] = {
  #if SERIAL_PORT != 3 && SERIAL_PORT_2 != 3
    P0_00, P0_01,
  #else
    P_NC,  P_NC,
  #endif
  #if SERIAL_PORT != 0 && SERIAL_PORT_2 != 0
                  P0_02, P0_03,
  #else
                  P_NC,  P_NC,
  #endif
                                P0_04, P0_05, P0_06, P0_07,
    P0_08, P0_09,
  #if SERIAL_PORT != 2 && SERIAL_PORT_2 != 2
                  P0_10, P0_11,
  #else
                  P_NC,  P_NC,
  #endif
                                P_NC,  P_NC,  P_NC,
  #if SERIAL_PORT != 1 && SERIAL_PORT_2 != 1
                                                     P0_15,
    P0_16,
  #else
                                                     P_NC,
    P_NC,
  #endif
           P0_17, P0_18, P0_19, P0_20, P0_21, P0_22, P0_23,
    P0_24, P0_25, P0_26, P0_27, P0_28,
  #if SERIAL_PORT != -1 && SERIAL_PORT_2 != -1
                                       P0_29, P0_30,
  #else
                                       P_NC,  P_NC,
  #endif
                                                   P_NC,

  P1_00, P1_01, P_NC,  P_NC,  P1_04, P_NC,  P_NC,  P_NC,
  P1_08, P1_09, P1_10, P_NC,  P_NC,  P_NC,  P1_14, P1_15,
  P1_16, P1_17, P1_18, P1_19, P1_20, P1_21, P1_22, P1_23,
  P1_24, P1_25, P1_26, P1_27, P1_28, P1_29, P1_30, P1_31,

  P2_00, P2_01, P2_02, P2_03, P2_04, P2_05, P2_06, P2_07,
  P2_08, P2_09, P2_10, P2_11, P2_12, P2_13, P_NC,  P_NC,
  P_NC,  P_NC,  P_NC,  P_NC,  P_NC,  P_NC,  P_NC,  P_NC,
  P_NC,  P_NC,  P_NC,  P_NC,  P_NC,  P_NC,  P_NC,  P_NC,

  P_NC,  P_NC,  P_NC,  P_NC,  P_NC,  P_NC,  P_NC,  P_NC,
  P_NC,  P_NC,  P_NC,  P_NC,  P_NC,  P_NC,  P_NC,  P_NC,
  P_NC,  P_NC,  P_NC,  P_NC,  P_NC,  P_NC,  P_NC,  P_NC,
  P_NC,  P3_25, P3_26, P_NC,  P_NC,  P_NC,  P_NC,  P_NC,

  P_NC,  P_NC,  P_NC,  P_NC,  P_NC,  P_NC,  P_NC,  P_NC,
  P_NC,  P_NC,  P_NC,  P_NC,  P_NC,  P_NC,  P_NC,  P_NC,
  P_NC,  P_NC,  P_NC,  P_NC,  P_NC,  P_NC,  P_NC,  P_NC,
  P_NC,  P_NC,  P_NC,  P_NC,  P4_28, P4_29, P_NC,  P_NC
};

constexpr uint8_t NUM_DIGITAL_PINS = COUNT(pin_map);

constexpr pin_t adc_pin_table[] = {
  P0_23, P0_24, P0_25, P0_26, P1_30, P1_31,
  #if SERIAL_PORT != 0 && SERIAL_PORT_2 != 0
    P0_03, P0_02
  #endif
};

#if SERIAL_PORT != 0 && SERIAL_PORT_2 != 0
  #define NUM_ANALOG_INPUTS 8
#else
  #define NUM_ANALOG_INPUTS 6
#endif

// P0.6 thru P0.9 are for the onboard SD card
#define HAL_SENSITIVE_PINS P0_06, P0_07, P0_08, P0_09

// Get the digital pin for an analog index
pin_t analogInputToDigitalPin(const int8_t p);

// Return the index of a pin number
// The pin number given here is in the form ppp:nnnnn
int16_t GET_PIN_MAP_INDEX(const pin_t pin);

// Test whether the pin is valid
bool VALID_PIN(const pin_t p);

// Get the analog index for a digital pin
int8_t DIGITAL_PIN_TO_ANALOG_PIN(const pin_t p);

// Test whether the pin is PWM
bool PWM_PIN(const pin_t p);

// Test whether the pin is interruptable
bool INTERRUPT_PIN(const pin_t p);

// Get the pin number at the given index
pin_t GET_PIN_MAP_PIN(const int16_t ind);

// Parse a G-code word into a pin index
int16_t PARSED_PIN_INDEX(const char code, const int16_t dval);

#endif // _PINMAPPING_H_
