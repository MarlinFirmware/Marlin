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

#ifndef _VARIANT_ARDUINO_STM32_
#define _VARIANT_ARDUINO_STM32_

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/
#include "PeripheralPins.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/*----------------------------------------------------------------------------
 *        Pins
 *----------------------------------------------------------------------------*/
extern const PinName digitalPin[];
// Right Side
#define PE1  0
#define PE0  1
#define PB9  2
#define PB8  3
#define PB7  4
#define PB6  5
#define PB5  6
#define PB4  7
#define PB3  8
#define PG15 9
#define PG14 10
#define PG13 11
#define PG12 12
#define PG11 13
#define PG10 14
#define PG9  15
#define PD7  16
#define PD6  17
#define PD5  18
#define PD4  19
#define PD3  20
#define PD2  21
#define PD1  22
#define PD0  23
#define PC12 24
#define PC11 25
#define PC10 26
#define PA15 27
#define PA14 28

#define PA13 29
#define PA12 30 // USB_DP
#define PA11 31 // USB_DM
#define PA10 32
#define PA9  33
#define PA8  34
#define PC9  35
#define PC8  36
#define PC7  37
#define PC6  38
#define PG8  39
#define PG7  40
#define PG6  41
#define PG5  42
#define PG4  43
#define PG3  44
#define PG2  45
#define PD15 46
#define PD14 47
#define PD13 48
#define PD12 49
#define PD11 50
#define PD10 51
#define PD9  52
#define PD8  53
#define PB15 54
#define PB14 55
#define PB13 56
#define PB12 57

#define PB11 58
#define PB10 59
#define PE15 60
#define PE14 61
#define PE13 62
#define PE12 63
#define PE11 64
#define PE10 65
#define PE9  66
#define PE8  67
#define PE7  68
#define PG1  69
#define PG0  70
#define PF15 71
#define PF14 72
#define PF13 73
#define PF12 74
#define PF11 75
#define PB2  76
#define PB1  77
#define PB0  78
#define PC5  79
#define PC4  80
#define PA7  81
#define PA6  82
#define PA5  83
#define PA4  84
#define PA3  85

#define PA2  86
#define PA1  87
#define PA0  88
#define PC3  89
#define PC2  90
#define PC1  91
#define PC0  92
#define PH1  93
#define PH0  94
#define PF10 95
#define PF9  96
#define PF8  97
#define PF7  98
#define PF6  99
#define PF5  100
#define PF4  101
#define PF3  102
#define PF2  103
#define PF1  104
#define PF0  105
#define PC15 106
#define PC14 107
#define PC13 108
#define PE6  109
#define PE5  110
#define PE4  111
#define PE3  112
#define PE2  113


// This must be a literal
#define NUM_DIGITAL_PINS        114
// This must be a literal with a value less than or equal to MAX_ANALOG_INPUTS
#define NUM_ANALOG_INPUTS       4
#define NUM_ANALOG_FIRST        44

// Below ADC, DAC and PWM definitions already done in the core
// Could be redefined here if needed
// ADC resolution is 12bits
//#define ADC_RESOLUTION          12
//#define DACC_RESOLUTION         12

// PWM resolution
#define PWM_RESOLUTION          8
#define PWM_FREQUENCY           20000
#define PWM_MAX_DUTY_CYCLE      255

// On-board LED pin number
#define LED_D2                  PA6
#define LED_D3                  PA7

// Board specific button
#define BTN_K_UP                PA0

#define LED_BUILTIN             LED_D2
#define LED_GREEN               LED_D2

// On-board user button
#define BTN_K0                  PE4
#define BTN_K1                  PE3
#define USER_BTN                BTN_K0

// Below SPI and I2C definitions already done in the core
// Could be redefined here if differs from the default one
// SPI Definitions
#define PIN_SPI_MOSI            PB15
#define PIN_SPI_MISO            PB14
#define PIN_SPI_SCK             PB13
#define PIN_SPI_SS              PB12
//#define PIN_SPI_SS1             PB0 // W25Q16 (on board flash)

// I2C Definitions
#define PIN_WIRE_SDA            PB7
#define PIN_WIRE_SCL            PB6

// Timer Definitions
//Do not use timer used by PWM pins when possible. See PinMap_PWM in PeripheralPins.c
#define TIMER_TONE              TIM6

// Do not use basic timer: OC is required
#define TIMER_SERVO             TIM2  //TODO: advanced-control timers don't work

// UART Definitions
// Define here Serial instance number to map on Serial generic name
#define SERIAL_UART_INSTANCE    1 //ex: 2 for Serial2 (USART2)
// DEBUG_UART could be redefined to print on another instance than 'Serial'
//#define DEBUG_UART              ((USART_TypeDef *) U(S)ARTX) // ex: USART3
// DEBUG_UART baudrate, default: 9600 if not defined
//#define DEBUG_UART_BAUDRATE     x
// DEBUG_UART Tx pin name, default: the first one found in PinMap_UART_TX for DEBUG_UART
//#define DEBUG_PINNAME_TX        PX_n // PinName used for TX

// Default pin used for 'Serial' instance (ex: ST-Link)
// Mandatory for Firmata
#define PIN_SERIAL_RX           PA10
#define PIN_SERIAL_TX           PA9

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
#define SERIAL_PORT_MONITOR     Serial
#define SERIAL_PORT_HARDWARE    Serial1
#endif

#endif /* _VARIANT_ARDUINO_STM32_ */
