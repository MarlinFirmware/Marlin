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

#ifndef _HAL_DUE_H
#define _HAL_DUE_H

#define CPU_32_BIT

#include <stdint.h>

#include <Arduino.h>

#include "../math_32bit.h"
#include "../HAL_SPI.h"
#include "fastio_Due.h"
#include "watchdog_Due.h"
#include "HAL_timers_Due.h"

//
// Defines
//
#define NUM_SERIAL 1
#define MYSERIAL0 customizedSerial

// We need the previous define before the include, or compilation bombs...
#include "MarlinSerial_Due.h"
#include "MarlinSerialUSB_Due.h"

#ifndef analogInputToDigitalPin
  #define analogInputToDigitalPin(p) ((p < 12u) ? (p) + 54u : -1)
#endif

#define CRITICAL_SECTION_START  uint32_t primask = __get_PRIMASK(); __disable_irq();
#define CRITICAL_SECTION_END    if (!primask) __enable_irq();

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

// --------------------------------------------------------------------------
// Types
// --------------------------------------------------------------------------

typedef int8_t pin_t;

#define HAL_SERVO_LIB Servo

// --------------------------------------------------------------------------
// Public Variables
// --------------------------------------------------------------------------

extern uint16_t HAL_adc_result;     // result of last ADC conversion

void cli(void);                     // Disable interrupts
void sei(void);                     // Enable interrupts

void HAL_clear_reset_source(void);  // clear reset reason
uint8_t HAL_get_reset_source(void); // get reset reason

void _delay_ms(const int delay);

int freeMemory(void);

/**
 * SPI: Extended functions taking a channel number (hardware SPI only)
 */

// Write single byte to specified SPI channel
void spiSend(uint32_t chan, byte b);

// Write buffer to specified SPI channel
void spiSend(uint32_t chan, const uint8_t* buf, size_t n);

// Read single byte from specified SPI channel
uint8_t spiRec(uint32_t chan);

/**
 * EEPROM
 */

void eeprom_write_byte(unsigned char *pos, unsigned char value);
unsigned char eeprom_read_byte(unsigned char *pos);
void eeprom_read_block (void *__dst, const void *__src, size_t __n);
void eeprom_update_block (const void *__src, void *__dst, size_t __n);

/**
 * ADC
 */

#define HAL_ANALOG_SELECT(pin)

inline void HAL_adc_init(void) {}//todo

#define HAL_START_ADC(pin)  HAL_adc_start_conversion(pin)
#define HAL_READ_ADC        HAL_adc_result

void HAL_adc_start_conversion(const uint8_t adc_pin);
uint16_t HAL_adc_get_result(void);
uint16_t HAL_getAdcReading(uint8_t chan);
void HAL_startAdcConversion(uint8_t chan);
uint8_t HAL_pinToAdcChannel(int pin);
uint16_t HAL_getAdcFreerun(uint8_t chan, bool wait_for_conversion = false);
//uint16_t HAL_getAdcSuperSample(uint8_t chan);
void HAL_enable_AdcFreerun(void);
//void HAL_disable_AdcFreerun(uint8_t chan);

/**
 * Pin Map
 */
#define GET_PIN_MAP_PIN(index) index
#define GET_PIN_MAP_INDEX(pin) pin
#define PARSED_PIN_INDEX(code, dval) parser.intval(code, dval)

/**
 * Tone
 */
void toneInit();
void tone(const pin_t _pin, const unsigned int frequency, const unsigned long duration=0);
void noTone(const pin_t _pin);

// Enable hooks into idle and setup for HAL
#define HAL_IDLETASK 1
#define HAL_INIT 1
void HAL_idletask(void);
void HAL_init(void);

#ifdef __cplusplus
  extern "C" {
#endif
char *dtostrf (double __val, signed char __width, unsigned char __prec, char *__s);
#ifdef __cplusplus
  }
#endif

#endif // _HAL_DUE_H
