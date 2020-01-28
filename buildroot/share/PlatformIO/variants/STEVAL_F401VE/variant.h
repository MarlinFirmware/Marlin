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
#if defined(ARDUINO_STEVAL)


/*----------------------------------------------------------------------------
 *        Pins
 *----------------------------------------------------------------------------*/
// USART1_MAIN
#define PA9  0 //TX
#define PA10 1 //RX

// WIFI (USART2)
#define PD3   2 // CTS
#define PD4   3 // RTS
#define PD5   4 // TX
#define PD6   5 // RX
#define PB5   6 // WIFI_WAKEUP
#define PE11  7 // WIFI_RESET
#define PE12  8 // WIFI_BOOT

// STATUS_LED
#define PE1   9 // STATUS_LED

// SPI USER
#define PB12 10 // SPI_CS
#define PB15 11 // SPI_MOSI
#define PB14 12 // SPI_MISO
#define PB13 13 // SPI_SCK

// I2C USER
#define PB7  14 // SDA
#define PB6  15 // SCL

// SPI
#define PA4  16 // SPI_CS
#define PA5  17 // SPI_SCK
#define PA6  18 // SPI_MISO
#define PA7  19 // SPI_MOSI

// JTAG
#define PA13 20 // JTAG_TMS/SWDIO
#define PA14 21 // JTAG_TCK/SWCLK
#define PB3  22 // JTAG_TDO/SWO

// SDCARD
#define PC8  23 // SDIO_D0
#define PC9  24 // SDIO_D1
#define PA15 25 // SD_CARD_DETECT
#define PC10 26 // SDIO_D2
#define PC11 27 // SDIO_D3
#define PC12 28 // SDIO_CK
#define PD2  29 // SDIO_CMD

// OTG
#define PA11 30 //OTG_DM
#define PA12 31 //OTG_DP

// IR/PROBE
#define PD1  32 //IR_OUT
#define PC1  33 //IR_ON

// USER_PINS
#define PD7  34 // USER3
#define PB9  35 // USER1
#define PE0  36 // USER2
#define PB4  37 // USER4

// USERKET
#define PE7  38 // USER_BUTTON

// ENDSTOPS
#define PD8  39 // X_STOP
#define PD9  40 // Y_STOP
#define PD10 41 // Z_STOP
#define PD11 42 // U_STOP
#define PA8  43 // V_STOP
#define PD0  44 // W_STOP

// HEATERS
#define PD13 45 // BED_HEAT_2
#define PD14 46 // BED_HEAT_1
#define PD15 47 // BED_HEAT_3
#define PC7  48 // E1_HEAT_PWM
#define PB0  49 // E2_HEAT_PWM
#define PB1  50 // E3_HEAT_PWM

// THERMISTOR
#define PC2  51 // BED_THERMISTOR_1
#define PC3  52 // BED_THERMISTOR_2
#define PA3  53 // BED_THERMISTOR_3
#define PA0  54 // E1_THERMISTOR
#define PA1  55 // E2_THERMISTOR
#define PA2  56 // E3_THERMISTOR

// FANS
#define PC4  57 // E1_FAN
#define PC5  58 // E2_FAN
#define PE8  59 // E3_FAN

// X_MOTOR
#define PE13 60 // X_RESET
#define PE14 61 // X_PWM
#define PE15 62 // X_DIR

// Y_MOTOR
#define PE10 63 // Y_RESET
#define PB10 64 // Y_PWM
#define PE9  65 // Y_DIR

// Z_MOTOR
#define PC15 66 // Z_RESET
#define PC6  67 // Z_PWM
#define PC0  68 // Z_DIR

// E1_MOTOR
#define PC14 69 // E1_RESET
#define PC13 70 // E1_DIR
#define PD12 71 // E1_PWM

// E2_MOTOR
#define PE4  72 // E2_RESET
#define PE5  73 // E2_PWM
#define PE6  74 // E2_DIR

// E3_MOTOR
#define PE3  75 // E3_RESET
#define PE2  76 // E3_DIR
#define PB8  77 // E3_PWM

// This must be a literal
#define NUM_DIGITAL_PINS        78
// This must be a literal with a value less than or equal to to MAX_ANALOG_INPUTS
#define NUM_ANALOG_INPUTS       6
#define NUM_ANALOG_FIRST        51

// On-board LED pin number
#define LED_BUILTIN             PE1
#define LED_GREEN               LED_BUILTIN

// On-board user button
#define USER_BTN                PE7

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
  #error'Invaqlid setting for SERIAL_UART_INSTANCE'
#endif

// Timer Definitions
#define TIMER_SERVO             TIM4  // TIMER_SERVO must be defined in this file
#define TIMER_TONE              TIM5  // TIMER_TONE must be defined in this file

/* SD detect signal */
/*
 * By default, R67 is not provided, so SD card detect is not used.
 * Note: SD CD (pin 16 of expansion connector J23) can be connected
 * to GND in order to be able to use SD_DETECT_PIN
 */
/*#define SD_DETECT_PIN           PA15*/

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
