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
#pragma once

/**
 * Pins Debugging for Atmel SAM3U (AT91SAM3U4E)
 *
 * Provides the interface M43 expects:
 *   - NUMBER_PINS_TOTAL
 *   - MULTI_NAME_PAD
 *   - getPinByIndex(index)
 *   - printPinNameByIndex(index)
 *   - getPinIsDigitalByIndex(index)
 *   - digitalPinToAnalogIndex(pin)
 *   - getValidPinMode(pin)
 *   - isValidPin(pin)
 *   - isAnalogPin(pin)
 *   - digitalRead_mod(pin)
 *   - pwm_status(pin)
 *   - printPinPWM(pin)
 *   - printPinPort(pin)
 *   - printPinNumber(pin)
 *   - printPinAnalog(pin)
 *
 * Pins here are (port * 32 + bit), so the port letter and bit number fall
 * straight out of the pin number.
 */

#include "include/pinmapping.h"
#include "adc.h"

#define NUMBER_PINS_TOTAL NUM_DIGITAL_PINS

#define digitalRead_mod(P)  extDigitalRead(P)
#define isValidPin(P)       VALID_PIN(P)
#define getPinByIndex(x)    pin_t(x)
#define getPinIsDigitalByIndex(x) true

// The SAM3U's four PWM channels do not serve any 4pi output, and this HAL
// drives everything with software PWM, so no pin reports as hardware PWM.
#define pwm_status(P)       false
#define printPinPWM(P)      NOOP

#define MULTI_NAME_PAD 21   // Space needed to be pretty if not the first name assigned to a pin

#define digitalPinToAnalogIndex(P) sam3u_adc_channel(P)
#define isAnalogPin(P)      (digitalPinToAnalogIndex(P) >= 0)

#define printPinNumber(P)   do{ sprintf_P(buffer, PSTR("%3d "), P); SERIAL_ECHO(buffer); }while(0)
#define printPinAnalog(P)   do{ sprintf_P(buffer, PSTR(" (A%2d)  "), digitalPinToAnalogIndex(P)); SERIAL_ECHO(buffer); }while(0)
#define printPinNameByIndex(x) do{ sprintf_P(buffer, PSTR("%-" STRINGIFY(MAX_NAME_LENGTH) "s"), pin_array[x].name); SERIAL_ECHO(buffer); }while(0)

// Print the native name of the pin, e.g. "PC28"
inline void printPinPort(const pin_t pin) {
  char buffer[8];
  sprintf_P(buffer, PSTR("P%c%02u "), char('A' + SAM3U_PORT(pin)), unsigned(SAM3U_BIT(pin)));
  SERIAL_ECHO(buffer);
}

// 1: output, 0: input
inline bool getValidPinMode(const pin_t pin) {
  return isValidPin(pin) && (SAM3U_PIO(pin)->PIO_OSR & SAM3U_MASK(pin));
}

/**
 * Pins that must not be toggled or watched by M43. PA11/PA12 are the console
 * UART, and PA3..PA8 are the SD card's HSMCI bus - driving either by hand
 * disconnects the host or corrupts a transfer in progress.
 */
#define M43_NEVER_TOUCH(Q) (WITHIN(Q, PA3, PA8) || (Q) == PA11 || (Q) == PA12)
