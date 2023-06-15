/*
 *******************************************************************************
 * Copyright (c) 2020, STMicroelectronics
 * All rights reserved.
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 *******************************************************************************
 */
#pragma once

/*----------------------------------------------------------------------------
 *        STM32 pins number
 *----------------------------------------------------------------------------*/
#define PA0                     0
#define PA1                     1
#define PA2                     2
#define PA3                     3
#define PA4                     4
#define PA5                     5
#define PA6                     6
#define PA7                     7
//#define PA0                     PIN_A0
//#define PA1                     PIN_A1
//#define PA2                     PIN_A2
//#define PA3                     PIN_A3
//#define PA4                     PIN_A4
//#define PA5                     PIN_A5
//#define PA6                     PIN_A6
//#define PA7                     PIN_A7
#define PA8                     8
#define PA9                     9
#define PA10                    10
#define PA11                    11
#define PA12                    12
#define PA13                    13
#define PA14                    14
#define PA15                    15
#define PB0                     16
#define PB1                     17
//#define PB0                     PIN_A8
//#define PB1                     PIN_A9
#define PB2                     18
#define PB3                     19
#define PB4                     20
#define PB5                     21
#define PB6                     22
#define PB7                     23
#define PB8                     24
#define PB9                     25
#define PB10                    26
#define PB11                    27
#define PB12                    28
#define PB13                    29
#define PB14                    30
#define PB15                    31
#define PC0                     32
#define PC1                     33
#define PC2                     34
#define PC3                     35
#define PC4                     36
#define PC5                     37
//#define PC0                     PIN_A10
//#define PC1                     PIN_A11
//#define PC2                     PIN_A12
//#define PC3                     PIN_A13
//#define PC4                     PIN_A14
//#define PC5                     PIN_A15
#define PC6                     38
#define PC7                     39
#define PC8                     40
#define PC9                     41
#define PC10                    42
#define PC11                    43
#define PC12                    44
#define PC13                    45
#define PC14                    46
#define PC15                    47
#define PD0                     48
#define PD1                     49
#define PD2                     50
#define PD3                     51
#define PD4                     52
#define PD5                     53
#define PD6                     54
#define PD7                     55
#define PD8                     56
#define PD9                     57
#define PD10                    58
#define PD11                    59
#define PD12                    60
#define PD13                    61
#define PD14                    62
#define PD15                    63
#define PE0                     64
#define PE1                     65
#define PE2                     66
#define PE3                     67
#define PE4                     68
#define PE5                     69
#define PE6                     70
#define PE7                     71
#define PE8                     72
#define PE9                     73
#define PE10                    74
#define PE11                    75
#define PE12                    76
#define PE13                    77
#define PE14                    78
#define PE15                    79
#define PF0                     80
#define PF1                     81
#define PF2                     82
//#define PF3                     PIN_A16
//#define PF4                     PIN_A17
//#define PF5                     PIN_A18
//#define PF6                     PIN_A19
//#define PF7                     PIN_A20
//#define PF8                     PIN_A21
//#define PF9                     PIN_A22
//#define PF10                    PIN_A23
#define PF3                     83
#define PF4                     84
#define PF5                     85
#define PF6                     86
#define PF7                     87
#define PF8                     88
#define PF9                     89
#define PF10                    90
#define PF11                    91
#define PF12                    92
#define PF13                    93
#define PF14                    94
#define PF15                    95
#define PG0                     96
#define PG1                     97
#define PG2                     98
#define PG3                     99
#define PG4                     100
#define PG5                     101
#define PG6                     102
#define PG7                     103
#define PG8                     104
#define PG9                     105
#define PG10                    106
#define PG11                    107
#define PG12                    108
#define PG13                    109
#define PG14                    110
#define PG15                    111
#define PH0                     112
#define PH1                     113
#define PH2                     114
#define PH3                     115
#define PH4                     116
#define PH5                     117
#define PH6                     118
#define PH7                     119
#define PH8                     120
#define PH9                     121
#define PH10                    122
#define PH11                    123
#define PH12                    124
#define PH13                    125
#define PH14                    126
#define PH15                    127
#define PI0                     128
#define PI1                     129
#define PI2                     130
#define PI3                     131
#define PI4                     132
#define PI5                     133
#define PI6                     134
#define PI7                     135
#define PI8                     136
#define PI9                     137
#define PI10                    138
#define PI11                    139
#define PI12                    140
#define PI13                    141
#define PI14                    142
#define PI15                    143
#define PJ0                     144
#define PJ1                     145
#define PJ2                     146
#define PJ3                     147
#define PJ4                     148
#define PJ5                     149
#define PJ6                     150
#define PJ7                     151
#define PJ8                     152
#define PJ9                     153
#define PJ10                    154
#define PJ11                    155
#define PJ12                    156
#define PJ13                    157
#define PJ14                    158
#define PJ15                    159
#define PK0                     160
#define PK1                     161
#define PK2                     162
#define PK3                     163
#define PK4                     164
#define PK5                     165
#define PK6                     166
#define PK7                     167

// Alternate pins number
#define PA_0_ALT1                (PA0  | ALT1)
#define PA_0_ALT2                (PA0  | ALT2)
#define PA_1_ALT1                (PA1  | ALT1)
#define PA_1_ALT2                (PA1  | ALT2)
#define PA_2_ALT1                (PA2  | ALT1)
#define PA_2_ALT2                (PA2  | ALT2)
#define PA_3_ALT1                (PA3  | ALT1)
#define PA_3_ALT2                (PA3  | ALT2)
#define PA_4_ALT1                (PA4  | ALT1)
#define PA_5_ALT1                (PA5  | ALT1)
#define PA_6_ALT1                (PA6  | ALT1)
#define PA_7_ALT1                (PA7  | ALT1)
#define PA_7_ALT2                (PA7  | ALT2)
#define PA_7_ALT3                (PA7  | ALT3)
#define PA_15_ALT1               (PA15 | ALT1)
#define PB_0_ALT1                (PB0  | ALT1)
#define PB_0_ALT2                (PB0  | ALT2)
#define PB_1_ALT1                (PB1  | ALT1)
#define PB_1_ALT2                (PB1  | ALT2)
#define PB_3_ALT1                (PB3  | ALT1)
#define PB_4_ALT1                (PB4  | ALT1)
#define PB_5_ALT1                (PB5  | ALT1)
#define PB_8_ALT1                (PB8  | ALT1)
#define PB_9_ALT1                (PB9  | ALT1)
#define PB_14_ALT1               (PB14 | ALT1)
#define PB_14_ALT2               (PB14 | ALT2)
#define PB_15_ALT1               (PB15 | ALT1)
#define PB_15_ALT2               (PB15 | ALT2)
#define PC_0_ALT1                (PC0  | ALT1)
#define PC_0_ALT2                (PC0  | ALT2)
#define PC_1_ALT1                (PC1  | ALT1)
#define PC_1_ALT2                (PC1  | ALT2)
#define PC_2_ALT1                (PC2  | ALT1)
#define PC_2_ALT2                (PC2  | ALT2)
#define PC_3_ALT1                (PC3  | ALT1)
#define PC_3_ALT2                (PC3  | ALT2)
#define PC_4_ALT1                (PC4  | ALT1)
#define PC_5_ALT1                (PC5  | ALT1)
#define PC_6_ALT1                (PC6  | ALT1)
#define PC_7_ALT1                (PC7  | ALT1)
#define PC_8_ALT1                (PC8  | ALT1)
#define PC_9_ALT1                (PC9  | ALT1)
#define PC_10_ALT1               (PC10 | ALT1)
#define PC_11_ALT1               (PC11 | ALT1)

#define STM32F746Vxx (defined(STM32F746VE) || defined(STM32F746VG))
#define STM32F746Zxx (defined(STM32F746ZE) || defined(STM32F746ZG))
#define STM32F746Ixx (defined(STM32F746IE) || defined(STM32F746IG))
#define STM32F746Bxx (defined(STM32F746BE) || defined(STM32F746BG))
#define STM32F746Nxx (defined(STM32F746NE) || defined(STM32F746NG))


#ifdef NUM_DIGITAL_PINS
  #undef NUM_DIGITAL_PINS
#endif
#ifdef NUM_ANALOG_INPUTS
  #undef NUM_ANALOG_INPUTS
#endif



#if STM32F746Vxx
  #define NUM_DIGITAL_PINS        80
  #define NUM_ANALOG_INPUTS       16
#elif STM32F746Zxx
  #define NUM_DIGITAL_PINS        114
  #define NUM_ANALOG_INPUTS       24
#elif STM32F746Ixx
  #define NUM_DIGITAL_PINS        140
  #define NUM_ANALOG_INPUTS       24
#elif (STM32F746Bxx || STM32F746Nxx)
  #define NUM_DIGITAL_PINS        168
  #define NUM_ANALOG_INPUTS       24
#endif


// On-board LED pin number
#ifndef LED_BUILTIN
  #define LED_BUILTIN           PNUM_NOT_DEFINED
#endif

// On-board user button
#ifndef USER_BTN
  #define USER_BTN              PNUM_NOT_DEFINED
#endif

// SPI definitions
#ifndef PIN_SPI_SS
  #define PIN_SPI_SS            PA4
#endif
#ifndef PIN_SPI_SS1
  #define PIN_SPI_SS1           PA15
#endif
#ifndef PIN_SPI_SS2
  #define PIN_SPI_SS2           PNUM_NOT_DEFINED
#endif
#ifndef PIN_SPI_SS3
  #define PIN_SPI_SS3           PNUM_NOT_DEFINED
#endif
#ifndef PIN_SPI_MOSI
  #define PIN_SPI_MOSI          PA7
#endif
#ifndef PIN_SPI_MISO
  #define PIN_SPI_MISO          PA6
#endif
#ifndef PIN_SPI_SCK
  #define PIN_SPI_SCK           PA5
#endif

// I2C definitions
#ifndef PIN_WIRE_SDA
  #define PIN_WIRE_SDA          PB7
#endif
#ifndef PIN_WIRE_SCL
  #define PIN_WIRE_SCL          PB6
#endif

// Timer Definitions
// Use TIM6/TIM7 when possible as servo and tone don't need GPIO output pin
#ifndef TIMER_TONE
  #define TIMER_TONE            TIM6
#endif
#ifndef TIMER_SERVO
  #define TIMER_SERVO           TIM7
#endif

// UART Definitions
#ifndef SERIAL_UART_INSTANCE
  #define SERIAL_UART_INSTANCE  3
#endif

// Default pin used for generic 'Serial' instance
//   point at USART3 for ST Link virtual com port
// Mandatory for Firmata
#ifndef PIN_SERIAL_RX
  #define PIN_SERIAL_RX         PD9
#endif
#ifndef PIN_SERIAL_TX
  #define PIN_SERIAL_TX         PD8
#endif

// Extra HAL modules
#if !defined(HAL_DAC_MODULE_DISABLED)
  #define HAL_DAC_MODULE_ENABLED
#endif
#if !defined(HAL_ETH_MODULE_DISABLED)
  #define HAL_ETH_MODULE_ENABLED
#endif
#if !defined(HAL_QSPI_MODULE_DISABLED)
  #define HAL_QSPI_MODULE_ENABLED
#endif
#if !defined(HAL_SD_MODULE_DISABLED)
  #define HAL_SD_MODULE_ENABLED
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
  #define SERIAL_PORT_HARDWARE_OPEN  Serial
#endif
