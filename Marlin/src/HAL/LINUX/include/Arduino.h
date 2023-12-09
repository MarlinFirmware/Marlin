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

#include <stddef.h>
#include <stdint.h>
#include <math.h>
#include <cstring>

#include <pinmapping.h>

#define strlcpy(A, B, C)   strncpy(A, B, (C) - 1)
#define strlcpy_P(A, B, C) strncpy_P(A, B, (C) - 1)

#define HIGH         0x01
#define LOW          0x00

#define INPUT          0x00
#define OUTPUT         0x01
#define INPUT_PULLUP   0x02
#define INPUT_PULLDOWN 0x03

#define LSBFIRST     0
#define MSBFIRST     1

#define CHANGE       0x02
#define FALLING      0x03
#define RISING       0x04

typedef uint8_t byte;
#define PROGMEM
#define PSTR(v) (v)
#define PGM_P const char *

// Used for libraries, preprocessor, and constants
#define abs(x) ((x)>0?(x):-(x))

#ifndef isnan
  #define isnan std::isnan
#endif
#ifndef isinf
  #define isinf std::isinf
#endif

#define sq(v) ((v) * (v))
#define constrain(value, arg_min, arg_max) ((value) < (arg_min) ? (arg_min) :((value) > (arg_max) ? (arg_max) : (value)))

// Interrupts
void cli(); // Disable
void sei(); // Enable
void attachInterrupt(uint32_t pin, void (*callback)(), uint32_t mode);
void detachInterrupt(uint32_t pin);

extern "C" {
  void GpioEnableInt(uint32_t port, uint32_t pin, uint32_t mode);
  void GpioDisableInt(uint32_t port, uint32_t pin);
}

// Time functions
extern "C" void delay(const int ms);
void _delay_ms(const int ms);
void delayMicroseconds(unsigned long);
uint32_t millis();

//IO functions
void pinMode(const pin_t, const uint8_t);
void digitalWrite(pin_t, uint8_t);
bool digitalRead(pin_t);
void analogWrite(pin_t, int);
uint16_t analogRead(pin_t);

int32_t random(int32_t);
int32_t random(int32_t, int32_t);
void randomSeed(uint32_t);

char *dtostrf(double __val, signed char __width, unsigned char __prec, char *__s);

int map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max);
