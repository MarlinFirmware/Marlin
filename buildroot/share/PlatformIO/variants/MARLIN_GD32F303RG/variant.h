/*
    Copyright (c) 2025 Arduino LLC.  All rights reserved.

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
    See the GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#pragma once

#include <stdint.h>

// GPIOA
#define PA0     0
#define PA1     1
#define PA2     2
#define PA3     3
#define PA4     4
#define PA5     5
#define PA6     6
#define PA7     7
#define PA8     8
#define PA9     9
#define PA10    10
#define PA11    11
#define PA12    12
#define PA13    13
#define PA14    14
#define PA15    15

// GPIOB
#define PB0     16
#define PB1     17
#define PB2     18
#define PB3     19
#define PB4     20
#define PB5     21
#define PB6     22
#define PB7     23
#define PB8     24
#define PB9     25
#define PB10    26
#define PB11    27
#define PB12    28
#define PB13    29
#define PB14    30
#define PB15    31

// GPIOC
#define PC0     32
#define PC1     33
#define PC2     34
#define PC3     35
#define PC4     36
#define PC5     37
#define PC6     38
#define PC7     39
#define PC8     40
#define PC9     41
#define PC10    42
#define PC11    43
#define PC12    44
#define PC13    45
#define PC14    46
#define PC15    47

// GPIOD
#define PD0     48
#define PD1     49
#define PD2     50

// Pin numbers
#define DIGITAL_PIN_COUNT   51
#define ANALOG_PIN_COUNT    16
#define TOTAL_PIN_COUNT     DIGITAL_PIN_COUNT

#define ADC_TEMP            (TOTAL_PIN_COUNT + 1)
#define ADC_VREF            (ADC_TEMP + 1)
#define MAX_PIN_NUM         (ADC_VREF)
#define INVALID_PIN_NUMBER  (ADC_VREF + 1)

// On-board LED pin number
#ifndef LED_BUILTIN
    #define LED_BUILTIN PB11
#endif

#define LED_GREEN   LED_BUILTIN

// On board user button
#ifndef USER_BTN
    #define USER_BTN    PC13
#endif

#ifndef ADC_CHANNEL_TEMPSENSOR
    #define ADC_CHANNEL_TEMPSENSOR  16
#endif
#ifndef ADC_CHANNEL_VREFINT
    #define ADC_CHANNEL_VREFINT     17
#endif

//
// SPI
//
// Each define will create a global instance
// Leave undefined to call your own
//
#define BOARD_USE_SPI0
//#define BOARD_USE_SPI1
//#define BOARD_USE_SPI2

// SPI0
#define PIN_SPI_SS      PA4
#define PIN_SPI_MOSI    PA7
#define PIN_SPI_MISO    PA6
#define PIN_SPI_SCK     PA5

// SPI1
#define PIN_SPI1_SS     PB12
#define PIN_SPI1_MOSI   PB15
#define PIN_SPI1_MISO   PB14
#define PIN_SPI1_SCK    PB13

// SPI2
#define PIN_SPI2_SS     PA15
#define PIN_SPI2_MOSI   PB5
#define PIN_SPI2_MISO   PB4
#define PIN_SPI2_SCK    PB3

//
// I2C
//
// Each define will create a global instance
// Leave undefined to call your own
//
#define BOARD_USE_I2C0
//#define BOARD_USE_I2C1

// I2C0
#define PIN_WIRE_SDA    PB7
#define PIN_WIRE_SCL    PB6

// I2C1
#define PIN_WIRE1_SDA   PB11
#define PIN_WIRE1_SCL   PB10

// Timer
#ifndef TIMER_TONE
    #define TIMER_TONE  2  // TIMER_TONE must be defined in this file
#endif
#ifndef TIMER_SERVO
    #define TIMER_SERVO 1  // TIMER_SERVO must be defined in this file
#endif

// Default U(S)ARTx Serial number (0-4)
#define DEFAULT_HARDWARE_SERIAL 0

// Serial
#define PIN_SERIAL_RX   PA10
#define PIN_SERIAL_TX   PA9

#ifndef PWM_FREQUENCY
    #define PWM_FREQUENCY       1000
#endif
#ifndef PWM_MAX_DUTY_CYCLE
    #define PWM_MAX_DUTY_CYCLE  4095
#endif

// C++ only
#ifdef __cplusplus

    #include <array>
    #include "GPIO.hpp"

    // These serial port names are intended to allow libraries and architecture-neutral
    // sketches to automatically default to the correct port name for a particular type
    // of use.  For example, a GPS module would normally use SERIAL_PORT_HARDWARE_OPEN,
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
    #ifndef SERIAL_PORT_MONITOR
        #define SERIAL_PORT_MONITOR   Serial
    #endif
    #ifndef SERIAL_PORT_HARDWARE
        #define SERIAL_PORT_HARDWARE  Serial0
    #endif

    // Ultra-compact port/pin pair structure
    struct PortPinPair {
      gpio::GPIO_Base port : 3;
      gpio::Pin_Number pin : 5;
    };

    // Ports
    inline constexpr gpio::GPIO_Base GPIO_A = gpio::GPIO_Base::GPIOA_BASE;
    inline constexpr gpio::GPIO_Base GPIO_B = gpio::GPIO_Base::GPIOB_BASE;
    inline constexpr gpio::GPIO_Base GPIO_C = gpio::GPIO_Base::GPIOC_BASE;
    inline constexpr gpio::GPIO_Base GPIO_D = gpio::GPIO_Base::GPIOD_BASE;

    // Pins
    inline constexpr gpio::Pin_Number PN0 = gpio::Pin_Number::PIN_0;
    inline constexpr gpio::Pin_Number PN1 = gpio::Pin_Number::PIN_1;
    inline constexpr gpio::Pin_Number PN2 = gpio::Pin_Number::PIN_2;
    inline constexpr gpio::Pin_Number PN3 = gpio::Pin_Number::PIN_3;
    inline constexpr gpio::Pin_Number PN4 = gpio::Pin_Number::PIN_4;
    inline constexpr gpio::Pin_Number PN5 = gpio::Pin_Number::PIN_5;
    inline constexpr gpio::Pin_Number PN6 = gpio::Pin_Number::PIN_6;
    inline constexpr gpio::Pin_Number PN7 = gpio::Pin_Number::PIN_7;
    inline constexpr gpio::Pin_Number PN8 = gpio::Pin_Number::PIN_8;
    inline constexpr gpio::Pin_Number PN9 = gpio::Pin_Number::PIN_9;
    inline constexpr gpio::Pin_Number PN10 = gpio::Pin_Number::PIN_10;
    inline constexpr gpio::Pin_Number PN11 = gpio::Pin_Number::PIN_11;
    inline constexpr gpio::Pin_Number PN12 = gpio::Pin_Number::PIN_12;
    inline constexpr gpio::Pin_Number PN13 = gpio::Pin_Number::PIN_13;
    inline constexpr gpio::Pin_Number PN14 = gpio::Pin_Number::PIN_14;
    inline constexpr gpio::Pin_Number PN15 = gpio::Pin_Number::PIN_15;

    // Pre-computed port/pin map for all possible pins
    // Maximum possible pins: TOTAL_PIN_COUNT
    inline constexpr std::array<PortPinPair, TOTAL_PIN_COUNT> port_pin_map = {{
      // Port A pins (0-15)
      {GPIO_A, PN0},  {GPIO_A, PN1},  {GPIO_A, PN2},  {GPIO_A, PN3},
      {GPIO_A, PN4},  {GPIO_A, PN5},  {GPIO_A, PN6},  {GPIO_A, PN7},
      {GPIO_A, PN8},  {GPIO_A, PN9},  {GPIO_A, PN10}, {GPIO_A, PN11},
      {GPIO_A, PN12}, {GPIO_A, PN13}, {GPIO_A, PN14}, {GPIO_A, PN15},
      // Port B pins (16-31)
      {GPIO_B, PN0},  {GPIO_B, PN1},  {GPIO_B, PN2},  {GPIO_B, PN3},
      {GPIO_B, PN4},  {GPIO_B, PN5},  {GPIO_B, PN6},  {GPIO_B, PN7},
      {GPIO_B, PN8},  {GPIO_B, PN9},  {GPIO_B, PN10}, {GPIO_B, PN11},
      {GPIO_B, PN12}, {GPIO_B, PN13}, {GPIO_B, PN14}, {GPIO_B, PN15},
      // Port C pins (32-47)
      {GPIO_C, PN0},  {GPIO_C, PN1},  {GPIO_C, PN2},  {GPIO_C, PN3},
      {GPIO_C, PN4},  {GPIO_C, PN5},  {GPIO_C, PN6},  {GPIO_C, PN7},
      {GPIO_C, PN8},  {GPIO_C, PN9},  {GPIO_C, PN10}, {GPIO_C, PN11},
      {GPIO_C, PN12}, {GPIO_C, PN13}, {GPIO_C, PN14}, {GPIO_C, PN15},
      // Port D pins (48-50)
      {GPIO_D, PN0},  {GPIO_D, PN1},  {GPIO_D, PN2}
    }};

#endif  // __cplusplus
