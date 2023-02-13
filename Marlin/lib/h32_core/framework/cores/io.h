
#ifndef _WIRISH_IO_H_
#define _WIRISH_IO_H_

#include "../cores/libmaple_types.h"
#include "../board/startup.h"

typedef enum WiringPinMode
{
    OUTPUT,
    OUTPUT_OPEN_DRAIN,
    INPUT,
    INPUT_ANALOG,
    INPUT_PULLUP,
    INPUT_PULLDOWN,
    INPUT_FLOATING,
    PWM,
    PWM_OPEN_DRAIN,
} WiringPinMode;

void pinMode(uint8 pin, WiringPinMode mode);

#define HIGH 0x1
#define LOW 0x0

void digitalWrite(uint8 pin, uint8 val);
uint32 digitalRead(uint8 pin);

void pwmWrite(uint8 pin, uint16 duty_cycle16);
void analogWrite(uint8 pin, int duty_cycle8);
uint16 analogRead(uint8 pin);
void gpio_set_mode(uint8 pin, WiringPinMode mode);
WiringPinMode gpio_get_mode(uint8 PinNum);

#endif
