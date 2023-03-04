#pragma once

#include "libmaple_types.h"

//
// pin modes
//
#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2
#define INPUT_FLOATING INPUT
#define INPUT_PULLDOWN INPUT
#define INPUT_ANALOG 4

//
// digitalWrite
//
#define HIGH 1
#define LOW 0

//
// public api
//

void pinMode(uint8 pin, uint8_t mode);

void digitalWrite(uint8 pin, uint8 val);
uint32 digitalRead(uint8 pin);

void pwmWrite(uint8 pin, uint16 duty_cycle);
void analogWrite(uint8 pin, int duty_cycle);
uint16 analogRead(uint8 pin);
void gpio_set_mode(uint8 pin, uint8_t mode);
uint8_t gpio_get_mode(uint8 pin);
