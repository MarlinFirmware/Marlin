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
#pragma once

/**
 * Description: HAL for Arduino Due and compatible (SAM3X8E)
 *
 * For ARDUINO_ARCH_SAM
 */

#define CPU_32_BIT

#include <stdint.h>

#include <Arduino.h>

#include "../shared/math_32bit.h"
#include "../shared/HAL_SPI.h"
#include "fastio_Due.h"
#include "watchdog_Due.h"
#include "HAL_timers_Due.h"

// Serial ports
#if !WITHIN(SERIAL_PORT, -1, 3)
  #error "SERIAL_PORT must be from -1 to 3"
#endif

// MYSERIAL0 required before MarlinSerial includes!
#define MYSERIAL0 customizedSerial1

#ifdef SERIAL_PORT_2
  #if !WITHIN(SERIAL_PORT_2, -1, 3)
    #error "SERIAL_PORT_2 must be from -1 to 3"
  #elif SERIAL_PORT_2 == SERIAL_PORT
    #error "SERIAL_PORT_2 must be different than SERIAL_PORT"
  #endif
  #define NUM_SERIAL 2
  #define MYSERIAL1 customizedSerial2
#else
  #define NUM_SERIAL 1
#endif

#include "MarlinSerial_Due.h"
#include "MarlinSerialUSB_Due.h"

// On AVR this is in math.h?
#define square(x) ((x)*(x))

#ifndef strncpy_P
  #define strncpy_P(dest, src, num) strncpy((dest), (src), (num))
#endif

#ifndef vsnprintf_P
  #define vsnprintf_P vsnprintf
#endif

// Fix bug in pgm_read_ptr
#undef pgm_read_ptr
#define pgm_read_ptr(addr) (*((void**)(addr)))
#undef pgm_read_word
#define pgm_read_word(addr) (*((uint16_t*)(addr)))

#define RST_POWER_ON   1
#define RST_EXTERNAL   2
#define RST_BROWN_OUT  4
#define RST_WATCHDOG   8
#define RST_JTAG       16
#define RST_SOFTWARE   32
#define RST_BACKUP     64

typedef int8_t pin_t;

#define HAL_SERVO_LIB Servo

//
// Interrupts
//
#define CRITICAL_SECTION_START  uint32_t primask = __get_PRIMASK(); __disable_irq()
#define CRITICAL_SECTION_END    if (!primask) __enable_irq()
#define ISRS_ENABLED() (!__get_PRIMASK())
#define ENABLE_ISRS()  __enable_irq()
#define DISABLE_ISRS() __disable_irq()

void cli(void);                     // Disable interrupts
void sei(void);                     // Enable interrupts

void HAL_clear_reset_source(void);  // clear reset reason
uint8_t HAL_get_reset_source(void); // get reset reason

//
// SPI: Extended functions taking a channel number (Hardware SPI only)
//

// Write single byte to specified SPI channel
void spiSend(uint32_t chan, byte b);

// Write buffer to specified SPI channel
void spiSend(uint32_t chan, const uint8_t* buf, size_t n);

// Read single byte from specified SPI channel
uint8_t spiRec(uint32_t chan);

//
// EEPROM
//
void eeprom_write_byte(uint8_t *pos, unsigned char value);
uint8_t eeprom_read_byte(uint8_t *pos);
void eeprom_read_block (void *__dst, const void *__src, size_t __n);
void eeprom_update_block (const void *__src, void *__dst, size_t __n);

//
// ADC
//
extern uint16_t HAL_adc_result;     // result of last ADC conversion

#ifndef analogInputToDigitalPin
  #define analogInputToDigitalPin(p) ((p < 12u) ? (p) + 54u : -1)
#endif

#define HAL_ANALOG_SELECT(pin)

inline void HAL_adc_init(void) {}//todo

#define HAL_START_ADC(pin)  HAL_adc_start_conversion(pin)
#define HAL_READ_ADC()      HAL_adc_result
#define HAL_ADC_READY()     true

void HAL_adc_start_conversion(const uint8_t adc_pin);
uint16_t HAL_adc_get_result(void);
uint16_t HAL_getAdcReading(uint8_t chan);
void HAL_startAdcConversion(uint8_t chan);
uint8_t HAL_pinToAdcChannel(int pin);
uint16_t HAL_getAdcFreerun(uint8_t chan, bool wait_for_conversion = false);
//uint16_t HAL_getAdcSuperSample(uint8_t chan);
void HAL_enable_AdcFreerun(void);
//void HAL_disable_AdcFreerun(uint8_t chan);

//
// Pin Map
//
#define GET_PIN_MAP_PIN(index) index
#define GET_PIN_MAP_INDEX(pin) pin
#define PARSED_PIN_INDEX(code, dval) parser.intval(code, dval)

//
// Tone
//
void toneInit();
void tone(const pin_t _pin, const unsigned int frequency, const unsigned long duration=0);
void noTone(const pin_t _pin);

// Enable hooks into idle and setup for HAL
#define HAL_IDLETASK 1
#define HAL_INIT 1
void HAL_idletask(void);
void HAL_init(void);

//
// Utility functions
//
void _delay_ms(const int delay);
int freeMemory(void);

#ifdef __cplusplus
  extern "C" {
#endif
char *dtostrf (double __val, signed char __width, unsigned char __prec, char *__s);
#ifdef __cplusplus
  }
#endif
