/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2011 LeafLabs, LLC.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************/

/**
 * @file   wirish/boards/maple/board.cpp
 * @author Marti Bolivar <mbolivar@leaflabs.com>
 * @brief  Maple board file.
 */

#include <board/board.h>         // For this board's header file


/* Roger Clark. Added next to includes for changes to Serial */
#include <libmaple/usart.h>
#include <HardwareSerial.h>

#include <wirish_types.h> // For stm32_pin_info and its contents
                                 // (these go into PIN_MAP).

#include "boards_private.h"      // For PMAP_ROW(), which makes
                                 // PIN_MAP easier to read.

// boardInit(): nothing special to do for Maple.
//
// When defining your own board.cpp, you can put extra code in this
// function if you have anything you want done on reset, before main()
// or setup() are called.
//
// If there's nothing special you need done, feel free to leave this
// function out, as we do here.

void boardInit(void) {
    // afio_remap(AFIO_REMAP_I2C1);
}


// Pin map: this lets the basic I/O functions (digitalWrite(),
// analogRead(), pwmWrite()) translate from pin numbers to STM32
// peripherals.
//
// PMAP_ROW() lets us specify a row (really a struct stm32_pin_info)
// in the pin map. Its arguments are:
//
// - GPIO device for the pin (&gpioa, etc.)
// - GPIO bit for the pin (0 through 15)
// - Timer device, or NULL if none
// - Timer channel (1 to 4, for PWM), or 0 if none
// - ADC device, or NULL if none
// - ADC channel, or ADCx if none

extern const stm32_pin_info PIN_MAP[BOARD_NR_GPIO_PINS] = {
/*
    gpio_dev *gpio_device;      GPIO device
    timer_dev *timer_device;    Pin's timer device, if any.
    const adc_dev *adc_device;  ADC device, if any.
    uint8 gpio_bit;             Pin's GPIO port bit.
    uint8 timer_channel;        Timer channel, or 0 if none.
    uint8 adc_channel;          Pin ADC channel, or ADCx if none.
*/

    {&gpioa, &timer2, &adc1,  0, 1,    0}, /* PA0 */
    {&gpioa, &timer2, &adc1,  1, 2,    1}, /* PA1 */
	{&gpioa, &timer2, &adc1,  2, 3,    2}, /* PA2 */
    {&gpioa, &timer2, &adc1,  3, 4,    3}, /* PA3 */
	{&gpioa,   NULL, &adc1,  4, 0,    4}, /* PA4 */
    {&gpioa,   NULL, &adc1,  5, 0,    5}, /* PA5 */
    {&gpioa, &timer3, &adc1,  6, 1,    6}, /* PA6 */
    {&gpioa, &timer3, &adc1,  7, 2,    7}, /* PA7 */
    {&gpioa, &timer1, NULL,  8, 1, ADCx}, /* PA8 */
    {&gpioa, &timer1, NULL,  9, 2, ADCx}, /* PA9 */
    {&gpioa, &timer1, NULL, 10, 3, ADCx}, /* PA10 */
    {&gpioa,   NULL, NULL, 11, 0, ADCx}, /* PA11 */
    {&gpioa,   NULL, NULL, 12, 0, ADCx}, /* PA12 */
    {&gpioa,   NULL, NULL, 13, 0, ADCx}, /* PA13 */
    {&gpioa,   NULL, NULL, 14, 0, ADCx}, /* PA14 */
    {&gpioa,   NULL, NULL, 15, 0, ADCx}, /* PA15 */

	{&gpiob, &timer3, &adc1,  0, 3,    8}, /* PB0 */
	{&gpiob, &timer3, &adc1,  1, 4,    9}, /* PB1 */
	{&gpiob,   NULL, NULL,  2, 0, ADCx}, /* PB2  */
	{&gpiob,   NULL, NULL,  3, 0, ADCx}, /* PB3  */
    {&gpiob,   NULL, NULL,  4, 0, ADCx}, /* PB4  */
    {&gpiob,   NULL, NULL,  5, 0, ADCx}, /* PB5 */
    {&gpiob, &timer4, NULL,  6, 1, ADCx}, /* PB6 */
    {&gpiob, &timer4, NULL,  7, 2, ADCx}, /* PB7 */
    {&gpiob, &timer4, NULL,  8, 3, ADCx}, /* PB8 */
	{&gpiob,   NULL, NULL,  9, 0, ADCx}, /* PB9 */
    {&gpiob,   NULL, NULL, 10, 0, ADCx}, /* PB10 */
    {&gpiob,   NULL, NULL, 11, 0, ADCx}, /* PB11 */
    {&gpiob,   NULL, NULL, 12, 0, ADCx}, /* PB12 */
    {&gpiob,   NULL, NULL, 13, 0, ADCx}, /* PB13 */
    {&gpiob,   NULL, NULL, 14, 0, ADCx}, /* PB14 */
    {&gpiob,   NULL, NULL, 15, 0, ADCx}, /* PB15 */


    {&gpioc,   NULL, &adc1,  0, 0,   10}, /* PC0 */
    {&gpioc,   NULL, &adc1,  1, 0,   11}, /* PC1 */
    {&gpioc,   NULL, &adc1,  2, 0,   12}, /* PC2 */
    {&gpioc,   NULL, &adc1,  3, 0,   13}, /* PC3 */
    {&gpioc,   NULL, &adc1,  4, 0,   14}, /* PC4 */
    {&gpioc,   NULL, &adc1,  5, 0,   15}, /* PC5 */
    {&gpioc, &timer8, NULL,  6, 1, ADCx}, /* PC6 */
	{&gpioc, &timer8, NULL,  7, 2, ADCx}, /* PC7 */
    {&gpioc, &timer8, NULL,  8, 3, ADCx}, /* PC8 */
    {&gpioc, &timer8, NULL,  9, 4, ADCx}, /* PC9 */
    {&gpioc,   NULL, NULL, 10, 0, ADCx}, /* PC10 UART4_TX/SDIO_D2 */
    {&gpioc,   NULL, NULL, 11, 0, ADCx}, /* PC11 UART4_RX/SDIO_D3 */
    {&gpioc,   NULL, NULL, 12, 0, ADCx}, /* PC12 UART5_TX/SDIO_CK */
    {&gpioc,   NULL, NULL, 13, 0, ADCx}, /* PC13 TAMPER-RTC */
    {&gpioc,   NULL, NULL, 14, 0, ADCx}, /* PC14 OSC32_IN */
    {&gpioc,   NULL, NULL, 15, 0, ADCx}, /* PC15 OSC32_OUT */

	{&gpiod,   NULL, NULL,   0, 0, ADCx} , /* PD0 OSC_IN */
	{&gpiod,   NULL, NULL,   1, 0, ADCx} , /* PD1  OSC_OUT */
	{&gpiod,   NULL, NULL,   2, 0, ADCx} , /* PD2  TIM3_ETR/UART5_RX SDIO_CMD */
};

/*  Basically everything that is defined as having a timer us PWM */
extern const uint8 boardPWMPins[BOARD_NR_PWM_PINS] __FLASH__ = {
    PA0,PA1,PA2,PA3,PA6,PA7,PA8,PA9,PA10,PB0,PB1,PB6,PB7,PB8,PB9,PC6,PC7,PC8,PC9
};

/*  Basically everything that is defined having ADC */
extern const uint8 boardADCPins[BOARD_NR_ADC_PINS] __FLASH__ = {
    PA0,PA1,PA2,PA3,PA4,PA5,PA6,PA7,PB0,PB1,PC0,PC1,PC2,PC3,PC4,PC5
};

/* not sure what this us used for */
extern const uint8 boardUsedPins[BOARD_NR_USED_PINS] __FLASH__ = {
    BOARD_JTMS_SWDIO_PIN,
    BOARD_JTCK_SWCLK_PIN, BOARD_JTDI_PIN, BOARD_JTDO_PIN, BOARD_NJTRST_PIN
};

DEFINE_HWSERIAL(Serial1, 1);
DEFINE_HWSERIAL(Serial2, 2);
DEFINE_HWSERIAL(Serial3, 3);
DEFINE_HWSERIAL_UART(Serial4, 4);
DEFINE_HWSERIAL_UART(Serial5, 5);

