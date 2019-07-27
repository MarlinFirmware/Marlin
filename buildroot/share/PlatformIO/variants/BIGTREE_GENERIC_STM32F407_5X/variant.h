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

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/
#include <PeripheralPins.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/*----------------------------------------------------------------------------
 *        Pins
 *----------------------------------------------------------------------------*/
extern const PinName digitalPin[];

#ifdef STM32F405RX
  #define STM32F4X_PIN_NUM  64  //64 pins mcu, 51 gpio
  #define STM32F4X_GPIO_NUM 51
  #define STM32F4X_ADC_NUM  16
#elif defined STM32F407_5VX
  #define STM32F4X_PIN_NUM  100  //100 pins mcu, 82 gpio
  #define STM32F4X_GPIO_NUM 82
  #define STM32F4X_ADC_NUM  16
#elif defined STM32F407_5ZX
  #define STM32F4X_PIN_NUM  144  //144 pins mcu, 114 gpio
  #define STM32F4X_GPIO_NUM 114
  #define STM32F4X_ADC_NUM  24
#elif defined STM32F407IX
  #define STM32F4X_PIN_NUM  176  //176 pins mcu, 140 gpio
  #define STM32F4X_GPIO_NUM 140
  #define STM32F4X_ADC_NUM  24
#else
  #error "no match MCU defined"
#endif

#if STM32F4X_PIN_NUM >= 64  //64 pins mcu, 51 gpio
  #define PC13  0
  #define PC14  1  //OSC32_IN
  #define PC15  2  //OSC32_OUT
  #define PH0   3  //OSC_IN
  #define PH1   4  //OSC_OUT
  #define PC0   5  //1:  2:ADC123_IN10
  #define PC1   6  //1:  2:ADC123_IN11
  #define PC2   7  //1:SPI2_MISO  2:ADC123_IN12
  #define PC3   8  //1:SPI2_MOSI  2:ADC123_IN13
  #define PA0   9  //1:UART4_TX / TIM5_CH1  2:ADC123_IN0
  #define PA1   10 //1:UART4_RX / TIM5_CH2 / TIM2_CH2  2:ADC123_IN1
  #define PA2   11 //1:USART2_TX /TIM5_CH3 / TIM9_CH1 / TIM2_CH3  2:ADC123_IN2
  #define PA3   12 //1:USART2_RX /TIM5_CH4 / TIM9_CH2 / TIM2_CH4  2:ADC123_IN3
  #define PA4   13 //NOT FT 1:SPI1_NSS / SPI3_NSS / USART2_CK  2:ADC12_IN4 / DAC_OUT1
  #define PA5   14 //NOT FT 1:SPI1_SCK  2:ADC12_IN5 / DAC_OUT2
  #define PA6   15 //1:SPI1_MISO / TIM13_CH1 / TIM3_CH1  2:ADC12_IN6
  #define PA7   16 //1:SPI1_MOSI / TIM14_CH1 / TIM3_CH2  2:ADC12_IN7
  #define PC4   17 //1:  2:ADC12_IN14
  #define PC5   18 //1:  2:ADC12_IN15
  #define PB0   19 //1:TIM3_CH3  2:ADC12_IN8
  #define PB1   20 //1:TIM3_CH4  2:ADC12_IN9
  #define PB2   21 //BOOT1
  #define PB10  22 //1:SPI2_SCK / I2C2_SCL / USART3_TX / TIM2_CH3
  #define PB11  23 //1:I2C2_SDA / USART3_RX / TIM2_CH4
  #define PB12  24 //1:SPI2_NSS / OTG_HS_ID
  #define PB13  25 //1:SPI2_SCK  2:OTG_HS_VBUS
  #define PB14  26 //1:SPI2_MISO / TIM12_CH1 / OTG_HS_DM
  #define PB15  27 //SPI2_MOSI / TIM12_CH2 / OTG_HS_DP
  #define PC6   28 //1:TIM8_CH1 / SDIO_D6 / USART6_TX / TIM3_CH1
  #define PC7   29 //1:TIM8_CH2 / SDIO_D7 / USART6_RX / TIM3_CH2
  #define PC8   30 //1:TIM8_CH3 / SDIO_D0 / TIM3_CH3
  #define PC9   31 //1:TIM8_CH4 / SDIO_D1 / TIM3_CH4
  #define PA8   32 //1:TIM1_CH1 / I2C3_SCL / OTG_FS_SOF
  #define PA9   33 //1:USART1_TX / TIM1_CH2  2:OTG_FS_VBUS
  #define PA10  34 //1:USART1_RX / TIM1_CH3 / OTG_FS_ID
  #define PA11  35 //1:TIM1_CH4 / OTG_FS_DM
  #define PA12  36 //1:OTG_FS_DP
  #define PA13  37 //0:JTMS-SWDIO
  #define PA14  38 //0:JTCK-SWCLK
  #define PA15  39 //0:JTDI  1:SPI3_NSS / SPI1_NSS
  #define PC10  40 //1:UART4_TX / SPI3_SCK / SDIO_D2 / USART3_TX
  #define PC11  41 //1:UART4_RX / SPI3_MISO / SDIO_D3 / USART3_RX
  #define PC12  42 //1:UART5_TX / SPI3_MOSI / SDIO_CK
  #define PD2   43 //1:UART5_RX / SDIO_CMD
  #define PB3   44 //0:JTDO  1:SPI3_SCK / TIM2_CH2 / SPI1_SCK
  #define PB4   45 //0:NJTRST  1:SPI3_MISO / TIM3_CH1 / SPI1_MISO
  #define PB5   45 //1:TIM3_CH2 / SPI1_MOSI / SPI3_MOSI
  #define PB6   47 //1:I2C1_SCL / TIM4_CH1 / USART1_TX
  #define PB7   48 //1:I2C1_SDA / TIM4_CH2 / USART1_RX
  #define PB8   49 //1:I2C1_SCL / TIM4_CH3 / SDIO_D4 / TIM10_CH1
  #define PB9   50 //1:I2C1_SDA / TIM4_CH4 / SDIO_D5 / TIM11_CH1 / SPI2_NSS
#endif
#if STM32F4X_PIN_NUM >= 100  //100 pins mcu, 82 gpio
  #define PE2   51 //1:FSMC_A23
  #define PE3   52 //1:FSMC_A19
  #define PE4   53 //1:FSMC_A20
  #define PE5   54 //1:FSMC_A21
  #define PE6   55 //1:FSMC_A22
  #define PE7   56 //1:FSMC_D4
  #define PE8   57 //1:FSMC_D5
  #define PE9   58 //1:FSMC_D6 / TIM1_CH1
  #define PE10  59 //1:FSMC_D7
  #define PE11  60 //1:FSMC_D8 / TIM1_CH2
  #define PE12  61 //1:FSMC_D9
  #define PE13  62 //1:FSMC_D10 / TIM1_CH3
  #define PE14  63 //1:FSMC_D11 / TIM1_CH4
  #define PE15  64 //1:FSMC_D12
  #define PD8   65 //1:FSMC_D13 / USART3_TX
  #define PD9   66 //1:FSMC_D14 / USART3_RX
  #define PD10  67 //1:FSMC_D15
  #define PD11  68 //1:FSMC_A16
  #define PD12  69 //1:FSMC_A17 / TIM4_CH1
  #define PD13  70 //1:FSMC_A18 / TIM4_CH2
  #define PD14  71 //1:FSMC_D0 / TIM4_CH3
  #define PD15  72 //1:FSMC_D1 / TIM4_CH4
  #define PD0   73 //1:FSMC_D2
  #define PD1   74 //1:FSMC_D3
  #define PD3   75 //1:FSMC_CLK
  #define PD4   76 //1:FSMC_NOE
  #define PD5   77 //1:USART2_TX
  #define PD6   78 //1:USART2_RX
  #define PD7   79
  #define PE0   80
  #define PE1   81
#endif
#if STM32F4X_PIN_NUM >= 144  //144 pins mcu, 114 gpio
  #define PF0   82 //1:FSMC_A0 / I2C2_SDA
  #define PF1   83 //1:FSMC_A1 / I2C2_SCL
  #define PF2   84 //1:FSMC_A2
  #define PF3   85 //1:FSMC_A3  2:ADC3_IN9
  #define PF4   86 //1:FSMC_A4  2:ADC3_IN14
  #define PF5   87 //1:FSMC_A5  2:ADC3_IN15
  #define PF6   88 //1:TIM10_CH1  2:ADC3_IN4
  #define PF7   89 //1:TIM11_CH1  2:ADC3_IN5
  #define PF8   90 //1:TIM13_CH1  2:ADC3_IN6
  #define PF9   91 //1;TIM14_CH1  2:ADC3_IN7
  #define PF10  92 //2:ADC3_IN8
  #define PF11  93
  #define PF12  94 //1:FSMC_A6
  #define PF13  95 //1:FSMC_A7
  #define PF14  96 //1:FSMC_A8
  #define PF15  97 //1:FSMC_A9
  #define PG0   98 //1:FSMC_A10
  #define PG1   99 //1:FSMC_A11
  #define PG2   100 //1:FSMC_A12
  #define PG3   101 //1:FSMC_A13
  #define PG4   102 //1:FSMC_A14
  #define PG5   103 //1:FSMC_A15
  #define PG6   104
  #define PG7   105
  #define PG8   106
  #define PG9   107 //1:USART6_RX
  #define PG10  108 //1:FSMC_NE3
  #define PG11  109
  #define PG12  110 //1:FSMC_NE4
  #define PG13  111 //1:FSMC_A24
  #define PG14  112 //1:FSMC_A25 / USART6_TX
  #define PG15  113
#endif
#if STM32F4X_PIN_NUM >= 176  //176 pins mcu, 140 gpio
  #define PI8   114
  #define PI9   115
  #define PI10  116
  #define PI11  117
  #define PH2   118
  #define PH3   119
  #define PH4   120 //1:I2C2_SCL
  #define PH5   121 //1:I2C2_SDA
  #define PH6   122 //1:TIM12_CH1
  #define PH7   123 //1:I2C3_SCL
  #define PH8   124 //1:I2C3_SDA
  #define PH9   125 //1:TIM12_CH2
  #define PH10  126 //1:TIM5_CH1
  #define PH11  127 //1:TIM5_CH2
  #define PH12  128 //1:TIM5_CH3
  #define PH13  129
  #define PH14  130
  #define PH15  131
  #define PI0   132 //1:TIM5_CH4 / SPI2_NSS
  #define PI1   133 //1:SPI2_SCK
  #define PI2   134 //1:TIM8_CH4 /SPI2_MISO
  #define PI3   135 //1:SPI2_MOS
  #define PI4   136
  #define PI5   137 //1:TIM8_CH1
  #define PI6   138 //1:TIM8_CH2
  #define PI7   139 //1:TIM8_CH3
#endif


// This must be a literal
#define NUM_DIGITAL_PINS        (STM32F4X_GPIO_NUM + STM32F4X_ADC_NUM)
// This must be a literal with a value less than or equal to MAX_ANALOG_INPUTS
#define NUM_ANALOG_INPUTS       (STM32F4X_ADC_NUM)
#define NUM_ANALOG_FIRST        (STM32F4X_GPIO_NUM)

// Below ADC, DAC and PWM definitions already done in the core
// Could be redefined here if needed
// ADC resolution is 12bits
//#define ADC_RESOLUTION          12
//#define DACC_RESOLUTION         12

// PWM resolution
#define PWM_RESOLUTION          8
#define PWM_FREQUENCY           20000
#define PWM_MAX_DUTY_CYCLE      255

// Below SPI and I2C definitions already done in the core
// Could be redefined here if differs from the default one
// SPI Definitions
#define PIN_SPI_MOSI            PB15
#define PIN_SPI_MISO            PB14
#define PIN_SPI_SCK             PB13
#define PIN_SPI_SS              PB12

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
