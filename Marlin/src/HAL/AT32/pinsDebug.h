/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 */
#pragma once

/**
 * Pins Debugging for AT32
 */

#include "../../inc/MarlinConfig.h"
#include "fastio.h"

// AT32 pin debugging uses the Arduino PIN_MAP infrastructure
#define NUM_DIGITAL_PINS        PIN_MAX
#define NUMBER_PINS_TOTAL       PIN_MAX
#define isValidPin(P)           IS_PIN(P)

#define getPinByIndex(x)        pin_t(pin_array[x].pin)
#define digitalRead_mod(P)      extDigitalRead(P)

#define printPinNumber(P)       do{ sprintf_P(buffer, PSTR("%3hd "), int16_t(P)); SERIAL_ECHO(buffer); }while(0)
#define printPinAnalog(P)       do{ sprintf_P(buffer, PSTR(" (A%2d)  "), digitalPinToAnalogIndex(P)); SERIAL_ECHO(buffer); }while(0)

#define printPinNameByIndex(x)  do{ sprintf_P(buffer, PSTR("%-" STRINGIFY(MAX_NAME_LENGTH) "s"), pin_array[x].name); SERIAL_ECHO(buffer); }while(0)

#define MULTI_NAME_PAD 21

//
// Pins that will cause a hang / reset / disconnect in M43 Toggle and Watch utils
//
#ifndef M43_NEVER_TOUCH
  #define IS_HOST_USART_PIN(Q) (Q == BOARD_USART1_TX_PIN || Q == BOARD_USART1_RX_PIN)
  #define IS_OSC_PIN(Q) (Q == PH0 || Q == PH1 || Q == PC14 || Q == PC15)

  #define M43_NEVER_TOUCH(Q) (IS_HOST_USART_PIN(Q) || IS_OSC_PIN(Q))
#endif

#ifndef ADC_PIN_INVALID
  #define ADC_PIN_INVALID 0xFF
#endif

int8_t digitalPinToAnalogIndex(const pin_t pin) {
  if (!isValidPin(pin)) return -1;
  if (PIN_MAP[pin].ADCx == NULL || PIN_MAP[pin].ADC_Channel == ADC_PIN_INVALID) return -1;
  return int8_t(PIN_MAP[pin].ADC_Channel);
}

bool isAnalogPin(pin_t pin) {
  if (!isValidPin(pin)) return false;
  if (PIN_MAP[pin].ADCx == NULL || PIN_MAP[pin].ADC_Channel == ADC_PIN_INVALID) return false;
  return _GET_MODE(pin) == INPUT_ANALOG && !M43_NEVER_TOUCH(pin);
}

bool getValidPinMode(const pin_t pin) {
  return isValidPin(pin) && !IS_INPUT(pin);
}

bool getPinIsDigitalByIndex(const int16_t index) {
  const pin_t pin = getPinByIndex(index);
  return !isAnalogPin(pin);
}

bool pwm_status(const pin_t pin) {
  return isValidPin(pin) && IS_PWM_PIN(pin);
}

void printPinPWM(const pin_t pin) {
  if (!pwm_status(pin)) return;
  SERIAL_ECHOPGM("PWM");
}

void printPinPort(pin_t pin) {
  const char port = 'A' + char(pin >> 4);
  const uint8_t gbit = pin & 0x0F;
  char buffer[8];
  sprintf_P(buffer, PSTR("P%c%hd "), port, gbit);
  if (gbit < 10) SERIAL_CHAR(' ');
  SERIAL_ECHO(buffer);
}
