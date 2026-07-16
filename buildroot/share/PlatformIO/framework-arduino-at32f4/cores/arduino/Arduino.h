/*
 * MIT License
 * Copyright (c) 2017 - 2022 _VIFEXTech
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef __ARDUINO_H
#define __ARDUINO_H

#include <stdlib.h>
#include <stdbool.h>
#include "libcore/mcu_core.h"
#include "binary.h"
#include "avr/pgmspace.h"



#ifdef __cplusplus
extern "C" {
#endif

#define KEILDUINO_VERSION       100

#define PI                      3.1415926535897932384626433832795f
#define HALF_PI                 1.5707963267948966192313216916398f
#define TWO_PI                  6.283185307179586476925286766559f
#define DEG_TO_RAD              0.017453292519943295769236907684886f
#define RAD_TO_DEG              57.295779513082320876798154814105f
#define EULER                   2.718281828459045235360287471352f

#define SERIAL                  0x0
#define DISPLAY                 0x1

#define LSBFIRST                0x0
#define MSBFIRST                0x1

#define LOW                     0x0
#define HIGH                    0x1

#define min(a,b)                ((a)<(b)?(a):(b))
#define max(a,b)                ((a)>(b)?(a):(b))
#define ABS(x)                  (((x)>0)?(x):-(x)) //abs(x) is define in stdlib.h
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
//#define round(x)                ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg)            ((deg)*DEG_TO_RAD)
#define degrees(rad)            ((rad)*RAD_TO_DEG)
#define sq(x)                   ((x)*(x))

#define lowByte(w)              ((uint8_t) ((w) & 0xff))
#define highByte(w)             ((uint8_t) ((w) >> 8))

#define bitRead(value, bit)     (((value) >> (bit)) & 0x01)
#define bitSet(value, bit)      ((value) |= (1UL << (bit)))
#define bitClear(value, bit)    ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

#ifndef _BV
#  define _BV(bit)              (1 << (bit))
#endif

#define clockCyclesPerMicrosecond()  (F_CPU / 1000000L)
#define clockCyclesToMicroseconds(a) (((a) * 1000L) / (F_CPU / 1000L))
#define microsecondsToClockCycles(a) ((a) * (F_CPU / 1000000L))

#define delay(ms)                    delay_ms(ms)
#define delayMicroseconds(us)        delay_us(us)

#define interrupts()                 sei()
#define noInterrupts()               cli()

#define NOT_A_PIN                    0xFF
#define NOT_A_PORT                   0xFF
#define NOT_AN_INTERRUPT             -1

#define boolean bool
typedef unsigned char byte;

void     pinMode(uint8_t pin, PinMode_TypeDef mode);
void     digitalWrite(uint8_t pin, uint8_t value);
uint8_t  digitalRead(uint8_t pin);
void     analogWrite(uint8_t pin, uint16_t value);
uint16_t analogRead(uint8_t pin);
uint16_t analogRead_DMA(uint8_t pin);
void     shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t value);
uint32_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint32_t bitOrder);
uint32_t pulseIn(uint32_t pin, uint32_t state, uint32_t timeout);

long     map(long x, long in_min, long in_max, long out_min, long out_max);
float    fmap(float x, float in_min, float in_max, float out_min, float out_max);
void     yield(void);



#ifdef __cplusplus
}// extern "C"
#endif

#ifdef __cplusplus
#  include "WCharacter.h"
#  include "WString.h"
#  include "WMath.h"
#  include <libcore/Tone.h>
#  include <libcore/HardwareSerial.h>
#  include <libcore/usb_vcp_wrapper.h>
#endif

#endif
