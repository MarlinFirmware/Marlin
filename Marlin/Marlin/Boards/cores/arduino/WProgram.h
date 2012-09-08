#ifndef WProgram_h
#define WProgram_h

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <avr/interrupt.h>

#include "wiring.h"

#ifdef __cplusplus
#include "WCharacter.h"
#include "WString.h"
#include "HardwareSerial.h"

uint16_t makeWord(uint16_t w);
uint16_t makeWord(byte h, byte l);

#define word(...) makeWord(__VA_ARGS__)

unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout = 1000000L);

void tone(uint8_t _pin, unsigned int frequency, unsigned long duration = 0);
void noTone(uint8_t _pin);

// WMath prototypes
long random(long);
long random(long, long);
void randomSeed(unsigned int);
long map(long, long, long, long, long);

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
const static uint8_t A0 = 54;
const static uint8_t A1 = 55;
const static uint8_t A2 = 56;
const static uint8_t A3 = 57;
const static uint8_t A4 = 58;
const static uint8_t A5 = 59;
const static uint8_t A6 = 60;
const static uint8_t A7 = 61;
const static uint8_t A8 = 62;
const static uint8_t A9 = 63;
const static uint8_t A10 = 64;
const static uint8_t A11 = 65;
const static uint8_t A12 = 66;
const static uint8_t A13 = 67;
const static uint8_t A14 = 68;
const static uint8_t A15 = 69;
#else
const static uint8_t A0 = 14;
const static uint8_t A1 = 15;
const static uint8_t A2 = 16;
const static uint8_t A3 = 17;
const static uint8_t A4 = 18;
const static uint8_t A5 = 19;
const static uint8_t A6 = 20;
const static uint8_t A7 = 21;
#endif

#endif

#endif