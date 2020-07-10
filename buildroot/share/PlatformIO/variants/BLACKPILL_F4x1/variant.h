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

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/*----------------------------------------------------------------------------
 *        Pins
 *----------------------------------------------------------------------------*/
#ifdef ARDUINO_BLACKPILL


/*----------------------------------------------------------------------------
 *        Pins
 *----------------------------------------------------------------------------*/
// UART USER
#define PA9  0 // TX
#define PA10 1 // RX

// I2C USER
#define PB3  2 // SDA
#define PB10 3 // SCL

// SPI
#define PA3  4 // DOGLCD_CS
#define PA4  5 // SDSS
#define PA5  6 // SPI_SCK
#define PA6  7 // SPI_MISO
#define PA7  8 // SPI_MOSI

// SDCARD
#define PB15 9 // SD_CARD_DETECT

// OTG
#define PA11 10  //OTG_DM
#define PA12 11 //OTG_DP

// ENDSTOPS
#define PB12 12 // X_STOP
#define PB13 13 // Y_STOP
#define PB14 14 // Z_STOP

// HEATERS
#define PA0  15 // E1_HEAT_PWM
#define PA1  16 // BED_HEAT_1

// FANS
#define PB7  17 // E1_FAN

// THERMISTOR
#define PB0  18 // E1_THERMISTOR
#define PB1  19 // BED_THERMISTOR_1

// X_MOTOR
#define PC13 20 // X_Y_Z_E1_RESET
#define PB9  21 // X_PWM
#define PC15 22 // X_DIR

// Y_MOTOR
#define PB8  23 // Y_PWM
#define PC14 24 // Y_DIR

// Z_MOTOR
#define PB5  25 // Z_PWM
#define PB6  26 // Z_DIR

// E1_MOTOR
#define PB4  27 // E1_PWM
#define PA15 28 // E1_DIR

// STATUS_LED
//#define PC13 29 // STATUS_LED

// FREE PINS
#define PA2  29
#define PA8  30
#define PB2  31

// This must be a literal
#define NUM_DIGITAL_PINS        32
// This must be a literal with a value less than or equal to to MAX_ANALOG_INPUTS
#define NUM_ANALOG_INPUTS       2
#define NUM_ANALOG_FIRST        18

// SPI Definitions
//#define PIN_SPI_MOSI            PA7
//#define PIN_SPI_MISO            PA6
//#define PIN_SPI_SCK             PA5
//#define SS_PIN                  PA4

// I2C Definitions
#define PIN_WIRE_SDA            PB3
#define PIN_WIRE_SCL            PB10

// On-board LED pin number
//#define LED_BUILTIN             PC13
//#define LED_BLUE                LED_BUILTIN

// UART Definitions
#define SERIAL_UART_INSTANCE    1 // Connected to ST-Link
//#define SERIAL_UART_INSTANCE    2 // Connected to WIFI

// Default pin used for 'Serial' instance (ex: ST-Link)
// Mandatory for Firmata
#if SERIAL_UART_INSTANCE == 1             // ST-Link & J23
  #define PIN_SERIAL_RX         PA10
  #define PIN_SERIAL_TX         PA9
#elif SERIAL_UART_INSTANCE == 2         // WIFI interface
  #define PIN_SERIAL2_RX        PD6
  #define PIN_SERIAL2_TX        PD5
#else
  #error "Invalid setting for SERIAL_UART_INSTANCE."
#endif

// Timer Definitions
#define TIMER_SERVO             TIM9  // TIMER_SERVO must be defined in this file
#define TIMER_TONE              TIM2  // TIMER_TONE must be defined in this file

/* SD detect signal */
#define SD_DETECT_PIN           PB15

/* HAL configuration */
#define HSE_VALUE               25000000U

/* Extra HAL modules */
#define HAL_SD_MODULE_ENABLED

#endif

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

#endif // _VARIANT_ARDUINO_STM32_

// PA0  54 // E1_THERMISTOR
// PA1  55 // E2_THERMISTOR
// PA2  56 // E3_THERMISTOR
// PA3  53 // BED_THERMISTOR_3
// PA4  16 // SPI_CS
// PA5  17 // SPI_SCK
// PA6  18 // SPI_MISO
// PA7  19 // SPI_MOSI
// PA8  43 // V_STOP
// PA9   0 //TX
// PA10  1 //RX
// PA11 30 //OTG_DM
// PA12 31 //OTG_DP
// PA13 20 // JTAG_TMS/SWDIO
// PA14 21 // JTAG_TCK/SWCLK
// PA15 25 // SD_CARD_DETECT
// PB0  49 // E2_HEAT_PWM
// PB1  50 // E3_HEAT_PWM
// PB3  22 // JTAG_TDO/SWO
// PB4  37 // USER4
// PB5   6 // WIFI_WAKEUP
// PB6  15 // SCL
// PB7  14 // SDA
// PB8  77 // E3_PWM
// PB9  35 // USER1
// PB10 64 // Y_PWM
// PB12 10 // SPI_CS
// PB13 13 // SPI_SCK
// PB14 12 // SPI_MISO
// PB15 11 // SPI_MOSI
// PC0  68 // Z_DIR
// PC1  33 //IR_ON
// PC2  51 // BED_THERMISTOR_1
// PC3  52 // BED_THERMISTOR_2
// PC4  57 // E1_FAN
// PC5  58 // E2_FAN
// PC6  67 // Z_PWM
// PC7  48 // E1_HEAT_PWM
// PC8  23 // SDIO_D0
// PC9  24 // SDIO_D1
// PC10 26 // SDIO_D2
// PC11 27 // SDIO_D3
// PC12 28 // SDIO_CK
// PC13 70 // E1_DIR
// PC14 69 // E1_RESET
// PC15 66 // Z_RESET
// PD0  44 // W_STOP
// PD1  32 //IR_OUT
// PD2  29 // SDIO_CMD
// PD3   2 // CTS
// PD4   3 // RTS
// PD5   4 // TX
// PD6   5 // RX
// PD7  34 // USER3
// PD8  39 // X_STOP
// PD9  40 // Y_STOP
// PD10 41 // Z_STOP
// PD11 42 // U_STOP
// PD12 71 // E1_PWM
// PD13 45 // BED_HEAT_2
// PD14 46 // BED_HEAT_1
// PD15 47 // BED_HEAT_3
// PE0  36 // USER2
// PE1   9 // STATUS_LED
// PE2  76 // E3_DIR
// PE3  75 // E3_RESET
// PE4  72 // E2_RESET
// PE5  73 // E2_PWM
// PE6  74 // E2_DIR
// PE7  38 // USER_BUTTON
// PE8  59 // E3_FAN
// PE9  65 // Y_DIR
// PE10 63 // Y_RESET
// PE11  7 // WIFI_RESET
// PE12  8 // WIFI_BOOT
// PE13 60 // X_RESET
// PE14 61 // X_PWM
// PE15 62 // X_DIR
