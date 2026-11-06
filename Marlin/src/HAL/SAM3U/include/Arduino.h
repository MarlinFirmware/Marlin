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
 * Arduino API compatibility shim for Atmel SAM3U (AT91SAM3U4E).
 *
 * There is no Arduino core for the SAM3U, so this HAL is bare-metal on top of
 * CMSIS. Marlin's shared layer (`HAL/shared/Marduino.h`) unconditionally does
 * `#include <Arduino.h>`, so this file stands in for it and provides only the
 * slice of the Arduino API that Marlin actually calls. It is reached by adding
 * `-IMarlin/src/HAL/SAM3U/include` to `build_src_flags` (see `ini/sam3u.ini`).
 *
 * Implementations live in `HAL/SAM3U/arduino.cpp`.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "pinmapping.h"

// ------------------------
// Constants
// ------------------------

#define HIGH           0x01
#define LOW            0x00

#define INPUT          0x00
#define OUTPUT         0x01
#define INPUT_PULLUP   0x02
#define INPUT_PULLDOWN 0x03

#define LSBFIRST       0
#define MSBFIRST       1

#define LOW_EDGE       0x00
#define HIGH_EDGE      0x01
#define CHANGE         0x02
#define FALLING        0x03
#define RISING         0x04

typedef uint8_t byte;
typedef bool boolean;

// ------------------------
// Math helpers
// ------------------------

#undef min
#undef max
#define min(a,b)     ((a) < (b) ? (a) : (b))
#define max(a,b)     ((a) > (b) ? (a) : (b))
#define constrain(v,lo,hi) ((v) < (lo) ? (lo) : ((v) > (hi) ? (hi) : (v)))
#define radians(d)   ((d) * DEG_TO_RAD)
#define degrees(r)   ((r) * RAD_TO_DEG)

#ifndef DEG_TO_RAD
  #define DEG_TO_RAD 0.017453292519943295769236907684886
#endif
#ifndef RAD_TO_DEG
  #define RAD_TO_DEG 57.295779513082320876798154814105
#endif

long map(long x, long in_min, long in_max, long out_min, long out_max);

/**
 * Arduino's random() takes bounds, while newlib's stdlib.h already declares a
 * zero-argument long random(void). These are overloads of it, not replacements
 * - without them a call like random(0, 24) fails to compile as "too many
 * arguments" rather than as a missing symbol.
 */
long random(long howbig);
long random(long howsmall, long howbig);
void randomSeed(unsigned long seed);

// ------------------------
// Interrupts
// ------------------------

// NOTE: Marduino.h defines FORCE_INLINE only *after* including this file,
// so spell the attribute out rather than relying on the macro.
__attribute__((always_inline)) inline void interrupts()   { __enable_irq(); }
__attribute__((always_inline)) inline void noInterrupts() { __disable_irq(); }

void attachInterrupt(const pin_t pin, void (*callback)(), const uint8_t mode);
void detachInterrupt(const pin_t pin);
#define digitalPinToInterrupt(P) (P)

// ------------------------
// Time
// ------------------------

#ifdef __cplusplus
  extern "C" {
#endif

uint32_t millis();
uint32_t micros();
void delay(const uint32_t ms);
void delayMicroseconds(const uint32_t us);

#ifdef __cplusplus
  }
#endif

// ------------------------
// Digital / analog I/O
// ------------------------

void pinMode(const pin_t pin, const uint8_t mode);
void digitalWrite(const pin_t pin, const uint8_t value);
bool digitalRead(const pin_t pin);

uint16_t analogRead(const pin_t pin);
void analogWrite(const pin_t pin, const int value);

// NOTE: no analogReadResolution(). This HAL fixes the ADC at 12 bits
// (HAL_ADC_RESOLUTION), and declaring a setter nothing implements would only
// turn a caller's mistake into a link error at the far end of a build.

// ------------------------
// Misc
// ------------------------

char* dtostrf(double value, signed char width, unsigned char prec, char *out);

__attribute__((always_inline)) inline void yield() {}
