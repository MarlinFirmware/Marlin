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
#ifndef __GPIO_H
#define __GPIO_H

#include "mcu_type.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NULL
#  define NULL ((void*)0)
#endif

#define ADC_CHANNEL_X ((uint8_t)0xFF)

#define IS_PIN(Pin)     (Pin < PIN_MAX)
#define IS_ADC_PIN(Pin) (IS_PIN(Pin) && PIN_MAP[Pin].ADCx != NULL && PIN_MAP[Pin].ADC_Channel  != ADC_CHANNEL_X)
#define IS_PWM_PIN(Pin) (IS_PIN(Pin) && PIN_MAP[Pin].TIMx != NULL && PIN_MAP[Pin].TimerChannel != 0)

#define GPIO_HIGH(GPIOX,GPIO_PIN_X)    ((GPIOX)->scr    = (GPIO_PIN_X))
#define GPIO_LOW(GPIOX,GPIO_PIN_X)     ((GPIOX)->clr     = (GPIO_PIN_X))
#define GPIO_READ(GPIOX,GPIO_PIN_X)   (((GPIOX)->idt   & (GPIO_PIN_X))!=0)
#define GPIO_TOGGLE(GPIOX,GPIO_PIN_X)  ((GPIOX)->odt  ^= (GPIO_PIN_X))

#define portInputRegister(Port)     (&(Port->idt))
#define portOutputRegister(Port)    (&(Port->odt))

#define analogInPinToBit(Pin)       (Pin)
#define digitalPinToInterrupt(Pin)  (Pin)
#define digitalPinToPort(Pin)       (PIN_MAP[Pin].GPIOx)
#define digitalPinToBitMask(Pin)    (PIN_MAP[Pin].GPIO_Pin_x)

#define digitalWrite_HIGH(Pin)      GPIO_HIGH  (PIN_MAP[Pin].GPIOx, PIN_MAP[Pin].GPIO_Pin_x)
#define digitalWrite_LOW(Pin)       GPIO_LOW   (PIN_MAP[Pin].GPIOx, PIN_MAP[Pin].GPIO_Pin_x)
#define digitalRead_FAST(Pin)       GPIO_READ  (PIN_MAP[Pin].GPIOx, PIN_MAP[Pin].GPIO_Pin_x)
#define togglePin(Pin)              GPIO_TOGGLE(PIN_MAP[Pin].GPIOx, PIN_MAP[Pin].GPIO_Pin_x)

#define LED_BUILTIN PC13

typedef enum
{
    PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7, PA8, PA9, PA10, PA11, PA12, PA13, PA14, PA15,
    PB0, PB1, PB2, PB3, PB4, PB5, PB6, PB7, PB8, PB9, PB10, PB11, PB12, PB13, PB14, PB15,
    PC0, PC1, PC2, PC3, PC4, PC5, PC6, PC7, PC8, PC9, PC10, PC11, PC12, PC13, PC14, PC15,
    PD0, PD1, PD2, PD3, PD4, PD5, PD6, PD7, PD8, PD9, PD10, PD11, PD12, PD13, PD14, PD15,
#ifdef GPIOE
    PE0, PE1, PE2, PE3, PE4, PE5, PE6, PE7, PE8, PE9, PE10, PE11, PE12, PE13, PE14, PE15,
#endif
#ifdef GPIOF
    PF0, PF1, PF2, PF3, PF4, PF5, PF6, PF7, PF8, PF9, PF10, PF11, PF12, PF13, PF14, PF15,
#endif
#ifdef GPIOG
    PG0, PG1, PG2, PG3, PG4, PG5, PG6, PG7, PG8, PG9, PG10, PG11, PG12, PG13, PG14, PG15,
#endif
    PIN_MAX
} Pin_TypeDef;




typedef struct
{
    gpio_type* GPIOx;
    tmr_type* TIMx;
    adc_type* ADCx;
    uint16_t GPIO_Pin_x;
    uint8_t TimerChannel;
    uint8_t ADC_Channel;
} PinInfo_TypeDef;

typedef enum
{
    INPUT,
    INPUT_PULLUP,
    INPUT_PULLDOWN,
    INPUT_ANALOG,
    INPUT_ANALOG_DMA,
    OUTPUT,
    OUTPUT_OPEN_DRAIN,
    OUTPUT_AF_OD,
    OUTPUT_AF_PP,
    PWM
} PinMode_TypeDef;

extern const PinInfo_TypeDef PIN_MAP[PIN_MAX];


void GPIOx_Init(
    gpio_type* GPIOx,
    uint16_t GPIO_Pin_x,
    PinMode_TypeDef Mode,
    gpio_drive_type GPIO_Drive_x
);
void GPIO_JTAG_Disable(void);
uint8_t GPIO_GetPortNum(uint8_t Pin);
uint8_t GPIO_GetPinNum(uint8_t Pin);
uint8_t GPIO_GetPinSource(uint16_t GPIO_Pin_x);

#ifdef __cplusplus
}// extern "C"
#endif

#endif
