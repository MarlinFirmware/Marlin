/*
 *******************************************************************************
 * Copyright (c) 2017, STMicroelectronics
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
//
/*----------------------------------------------------------------------------
 *        Pins
 *----------------------------------------------------------------------------*/
                      // | DIGITAL | ANALOG IN  | ANALOG OUT | UART/USART            | TWI                  | SPI                               | SPECIAL   |
                      // |---------|------------|------------|-----------------------|----------------------|-----------------------------------|-----------|
#define PA0  PIN_A0   // | 0       | A0 (ADC1)  |            | UART4_TX              |                      |                                   |           |
#define PA1  PIN_A1   // | 1       | A1 (ADC1)  |            | UART4_RX              |                      |                                   |           |
#define PA2  PIN_A2   // | 2       | A2 (ADC1)  |            | USART2_TX             |                      |                                   |           |
#define PA3  PIN_A3   // | 3       | A3 (ADC1)  |            | USART2_RX             |                      |                                   |           |
#define PA4  PIN_A4   // | 4       | A4 (ADC1)  | DAC_OUT1   |                       |                      | SPI1_SS, (SPI3_SS)                |           |
#define PA5  PIN_A5   // | 5       | A5 (ADC1)  | DAC_OUT2   |                       |                      | SPI1_SCK                          |           |
#define PA6  PIN_A6   // | 6       | A6 (ADC1)  |            |                       |                      | SPI1_MISO                         |           |
#define PA7  PIN_A7   // | 7       | A7 (ADC1)  |            |                       |                      | SPI1_MOSI                         |           |
#define PA8  8        // | 8       |            |            |                       | TWI3_SCL             |                                   |           |
#define PA9  9        // | 9       |            |            | USART1_TX             |                      |                                   |           |
#define PA10 10       // | 10      |            |            | USART1_RX             |                      |                                   |           |
#define PA11 11       // | 11      |            |            |                       |                      |                                   |           |
#define PA12 12       // | 12      |            |            |                       |                      |                                   |           |
#define PA13 13       // | 13      |            |            |                       |                      |                                   | SWD_SWDIO |
#define PA14 14       // | 14      |            |            |                       |                      |                                   | SWD_SWCLK |
#define PA15 15       // | 15      |            |            |                       |                      | SPI3_SS, (SPI1_SS)                |           |
                      // |---------|------------|------------|-----------------------|----------------------|-----------------------------------|-----------|
#define PB0  PIN_A8   // | 16      | A8 (ADC1)  |            |                       |                      |                                   |           |
#define PB1  PIN_A9   // | 17      | A9 (ADC1)  |            |                       |                      |                                   |           |
#define PB2  18       // | 18      |            |            |                       |                      |                                   | BOOT1     |
#define PB3  19       // | 19      |            |            |                       |                      | SPI3_SCK,  (SPI1_SCK)             |           |
#define PB4  20       // | 20      |            |            |                       |                      | SPI3_MISO, (SPI1_MISO)            |           |
#define PB5  21       // | 21      |            |            |                       |                      | SPI3_MOSI, (SPI1_MOSI)            |           |
#define PB6  22       // | 22      |            |            | USART1_TX             | TWI1_SCL             |                                   |           |
#define PB7  23       // | 23      |            |            | USART1_RX             | TWI1_SDA             |                                   |           |
#define PB8  24       // | 24      |            |            |                       | TWI1_SCL             |                                   |           |
#define PB9  25       // | 25      |            |            |                       | TWI1_SDA             | SPI2_SS                           |           |
#define PB10 26       // | 26      |            |            | USART3_TX, (UART4_TX) | TWI2_SCL             | SPI2_SCK                          |           |
#define PB11 27       // | 27      |            |            | USART3_RX             | TWI2_SDA             |                                   |           |
#define PB12 28       // | 28      |            |            |                       |                      | SPI2_SS                           |           |
#define PB13 29       // | 29      |            |            |                       |                      | SPI2_SCK                          |           |
#define PB14 30       // | 30      |            |            |                       |                      | SPI2_MISO                         |           |
#define PB15 31       // | 31      |            |            |                       |                      | SPI2_MOSI                         |           |
                      // |---------|------------|------------|-----------------------|----------------------|-----------------------------------|-----------|
#define PC0  PIN_A10  // | 32      | A10 (ADC1) |            |                       |                      |                                   |           |
#define PC1  PIN_A11  // | 33      | A11 (ADC1) |            |                       |                      |                                   |           |
#define PC2  PIN_A12  // | 34      | A12 (ADC1) |            |                       |                      | SPI2_MISO                         |           |
#define PC3  PIN_A13  // | 35      | A13 (ADC1) |            |                       |                      | SPI2_MOSI                         |           |
#define PC4  PIN_A14  // | 36      | A14 (ADC1) |            |                       |                      |                                   |           |
#define PC5  PIN_A15  // | 37      | A15 (ADC1) |            | USART3_RX             |                      |                                   |           |
#define PC6  38       // | 38      |            |            | USART6_TX             |                      |                                   |           |
#define PC7  39       // | 39      |            |            | USART6_RX             |                      |                                   |           |
#define PC8  40       // | 40      |            |            |                       |                      |                                   |           |
#define PC9  41       // | 41      |            |            | USART3_TX             | TWI3_SDA             |                                   |           |
#define PC10 42       // | 42      |            |            |                       |                      | SPI3_SCK                          |           |
#define PC11 43       // | 43      |            |            | USART3_RX, (UART4_RX) |                      | SPI3_MISO                         |           |
#define PC12 44       // | 44      |            |            | UART5_TX              |                      | SPI3_MOSI                         |           |
#define PC13 45       // | 45      |            |            |                       |                      |                                   |           |
#define PC14 46       // | 46      |            |            |                       |                      |                                   | OSC32_IN  |
#define PC15 47       // | 47      |            |            |                       |                      |                                   | OSC32_OUT |
                      // |---------|------------|------------|-----------------------|----------------------|-----------------------------------|-----------|
#define PD0  48       // | 48      |            |            |                       |                      |                                   |           |
#define PD1  49       // | 49      |            |            |                       |                      |                                   |           |
#define PD2  50       // | 50      |            |            | UART5_RX              |                      |                                   |           |
#define PD3  51       // | 51      |            |            |                       |                      |                                   |           |
#define PD4  52       // | 52      |            |            |                       |                      |                                   |           |
#define PD5  53       // | 53      |            |            | USART2_TX             |                      |                                   |           |
#define PD6  54       // | 54      |            |            | USART2_RX             |                      |                                   |           |
#define PD7  55       // | 55      |            |            |                       |                      |                                   |           |
#define PD8  56       // | 56      |            |            | USART3_TX             |                      |                                   |           |
#define PD9  57       // | 57      |            |            | USART3_RX             |                      |                                   |           |
#define PD10 58       // | 58      |            |            |                       |                      |                                   |           |
#define PD11 59       // | 59      |            |            |                       |                      |                                   |           |
#define PD12 60       // | 60      |            |            |                       |                      |                                   |           |
#define PD13 61       // | 61      |            |            |                       |                      |                                   |           |
#define PD14 62       // | 62      |            |            |                       |                      |                                   |           |
#define PD15 63       // | 63      |            |            |                       |                      |                                   |           |
                      // |---------|------------|------------|-----------------------|----------------------|-----------------------------------|-----------|
#define PE0  64       // | 64      |            |            |                       |                      |                                   |           |
#define PE1  65       // | 65      |            |            |                       |                      |                                   |           |
#define PE2  66       // | 66      |            |            |                       |                      |                                   |           |
#define PE3  67       // | 67      |            |            |                       |                      |                                   |           |
#define PE4  68       // | 68      |            |            |                       |                      |                                   |           |
#define PE5  69       // | 69      |            |            |                       |                      |                                   |           |
#define PE6  70       // | 70      |            |            |                       |                      |                                   |           |
#define PE7  71       // | 71      |            |            |                       |                      |                                   |           |
#define PE8  72       // | 72      |            |            |                       |                      |                                   |           |
#define PE9  73       // | 73      |            |            |                       |                      |                                   |           |
#define PE10 74       // | 74      |            |            |                       |                      |                                   |           |
#define PE11 75       // | 75      |            |            |                       |                      |                                   |           |
#define PE12 76       // | 76      |            |            |                       |                      |                                   |           |
#define PE13 77       // | 77      |            |            |                       |                      |                                   |           |
#define PE14 78       // | 78      |            |            |                       |                      |                                   |           |
#define PE15 79       // | 79      |            |            |                       |                      |                                   |           |
                      // |---------|------------|------------|-----------------------|----------------------|-----------------------------------|-----------|
#define PH0  80       // | 80      |            |            |                       |                      |                                   | OSC_IN    |
#define PH1  81       // | 81      |            |            |                       |                      |                                   | OSC_OUT   |
                      // |---------|------------|------------|-----------------------|----------------------|-----------------------------------|-----------|

// This must be a literal
#define NUM_DIGITAL_PINS        82
#define NUM_ANALOG_INPUTS       16

// Below SPI and I2C definitions already done in the core
// Could be redefined here if differs from the default one
// SPI Definitions
#define PIN_SPI_SS              PC9
#define PIN_SPI_SCK             PC10
#define PIN_SPI_MISO            PC11
#define PIN_SPI_MOSI            PC12

// I2C Definitions
#define PIN_WIRE_SCL            PB8
#define PIN_WIRE_SDA            PB9

// Timer Definitions
// Do not use timer used by PWM pins when possible. See PinMap_PWM in PeripheralPins.c
// TIM1 = HEATER0, HEATER1, [SERVO]
// TIM2 = FAN1, FAN2, [BEEPER]
// TIM4 = HEATER_BED
// TIM5 = HEATER2, FAN0
// Uses default for STM32F4xx STEP_TIMER 6 and TEMP_TIMER 14
#define TIMER_SERVO             TIM1  // TIMER_SERVO must be defined in this file
#define TIMER_TONE              TIM2  // TIMER_TONE must be defined in this file
#define TIMER_SERIAL            TIM3  // TIMER_SERIAL must be defined in this file

// USART1 (direct to RK3328 SoC)
#define ENABLE_HWSERIAL1
#define PIN_SERIAL1_TX           PA9
#define PIN_SERIAL1_RX          PA10

// USART3 connector
#define ENABLE_HWSERIAL3
#define PIN_SERIAL3_TX          PB10
#define PIN_SERIAL3_RX          PB11

#ifdef __cplusplus
} // extern "C"
#endif

/*----------------------------------------------------------------------------
 *        Arduino objects - C++ only
 *----------------------------------------------------------------------------*/

#ifdef __cplusplus
// These serial port names are intended to allow libraries and architecture-neutral
// sketches to automatically default to the correct port name for a particular type
// of use.  For example, a GPS module would normally connect to SERIAL_PORT_HARDWARE_OPEN,
// the first hardware serial port whose RX/TX pins are not dedicated to another use.
//
// SERIAL_PORT_MONITOR        Port which normally prints to the Arduino Serial Monitor
//
// SERIAL_PORT_USBVIRTUAL     Port which is USB virtual serial
//
// SERIAL_PORT_LINUXBRIDGE    Port which connects to a Linux system via Bridge library
//
// SERIAL_PORT_HARDWARE       Hardware serial port, physical RX & TX pins.
//
// SERIAL_PORT_HARDWARE_OPEN  Hardware serial ports which are open for use.  Their RX & TX
//                            pins are NOT connected to anything by default.
#define SERIAL_PORT_MONITOR         Serial1
#define SERIAL_PORT_USBVIRTUAL      SerialUSB
#define SERIAL_PORT_HARDWARE        Serial1
#define SERIAL_PORT_HARDWARE1       Serial3
#define SERIAL_PORT_HARDWARE_OPEN   Serial1
#define SERIAL_PORT_HARDWARE_OPEN1  Serial3
#endif
