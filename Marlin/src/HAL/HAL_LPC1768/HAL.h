/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 * Copyright (c) 2016 Bob Cousins bobcousins42@googlemail.com
 * Copyright (c) 2015-2016 Nico Tonnhofer wurstnase.reprap@gmail.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * Description: HAL for Arduino Due and compatible (SAM3X8E)
 *
 * For ARDUINO_ARCH_SAM
 */

#ifndef _HAL_LPC1768_H
#define _HAL_LPC1768_H

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------

#include <stdint.h>
#include <stdarg.h>
#include <algorithm>

void _printf (const  char *format, ...);
void _putc(uint8_t c);
uint8_t _getc();

extern "C" volatile uint32_t _millis;

//arduino: Print.h
#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2
//arduino: binary.h (weird defines)
#define B01 1
#define B10 2

#include "arduino.h"
#include "pinmapping.h"
#include "fastio.h"
#include "watchdog.h"
#include "serial.h"
#include "HAL_timers.h"
#include "HardwareSerial.h"

#define ST7920_DELAY_1 DELAY_5_NOP;DELAY_5_NOP;DELAY_5_NOP;DELAY_5_NOP;DELAY_5_NOP;DELAY_5_NOP;DELAY_5_NOP;DELAY_5_NOP;DELAY_5_NOP;DELAY_5_NOP;DELAY_5_NOP;DELAY_5_NOP
#define ST7920_DELAY_2 DELAY_5_NOP;DELAY_5_NOP;DELAY_5_NOP;DELAY_5_NOP;DELAY_5_NOP;DELAY_5_NOP;DELAY_5_NOP;DELAY_5_NOP;DELAY_5_NOP;DELAY_5_NOP;DELAY_5_NOP;DELAY_5_NOP;DELAY_5_NOP;DELAY_5_NOP;DELAY_5_NOP
#define ST7920_DELAY_3 DELAY_5_NOP;DELAY_5_NOP;DELAY_5_NOP;DELAY_5_NOP;DELAY_5_NOP;DELAY_5_NOP;DELAY_5_NOP;DELAY_5_NOP;DELAY_5_NOP;DELAY_5_NOP;DELAY_5_NOP;DELAY_5_NOP;DELAY_5_NOP;DELAY_5_NOP;DELAY_5_NOP

//Serial override
extern HalSerial usb_serial;

#if SERIAL_PORT == -1
  #define MYSERIAL usb_serial
#elif SERIAL_PORT == 0
  #define MYSERIAL Serial
#elif SERIAL_PORT == 1
  #define MYSERIAL Serial1
#elif SERIAL_PORT == 2
  #define MYSERIAL Serial2
#elif SERIAL_PORT == 3
  #define MYSERIAL Serial3
#endif

#define CRITICAL_SECTION_START  uint32_t primask = __get_PRIMASK(); __disable_irq();
#define CRITICAL_SECTION_END    if (!primask) __enable_irq();

//Utility functions
int freeMemory(void);

// SPI: Extended functions which take a channel number (hardware SPI only)
/** Write single byte to specified SPI channel */
void spiSend(uint32_t chan, byte b);
/** Write buffer to specified SPI channel */
void spiSend(uint32_t chan, const uint8_t* buf, size_t n);
/** Read single byte from specified SPI channel */
uint8_t spiRec(uint32_t chan);

// ADC
#define HAL_ANALOG_SELECT(pin) HAL_adc_enable_channel(pin)
#define HAL_START_ADC(pin)     HAL_adc_start_conversion(pin)
#define HAL_READ_ADC           HAL_adc_get_result()

void HAL_adc_init(void);
void HAL_adc_enable_channel(int pin);
void HAL_adc_start_conversion(const uint8_t adc_pin);
uint16_t HAL_adc_get_result(void);

#endif // _HAL_LPC1768_H
