/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 * Copyright (c) 2016 Bob Cousins bobcousins42@googlemail.com
 * Copyright (c) 2015-2016 Nico Tonnhofer wurstnase.reprap@gmail.com
 * Copyright (c) 2017 Victor Perez
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
 * HAL for stm32duino.com based on Libmaple and compatible (STM32F1)
 */

#ifndef _HAL_STM32F1_H
#define _HAL_STM32F1_H

#undef DEBUG_NONE

#ifndef vsnprintf_P
  #define vsnprintf_P vsnprintf
#endif

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------

#include <stdint.h>

#include "Arduino.h"

// --------------------------------------------------------------------------
// Undefine DEBUG_ settings
// --------------------------------------------------------------------------


#undef DEBUG_NONE
#undef DEBUG_FAULT
#undef DEBUG_ALL

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------

#include "fastio_Stm32f1.h"
#include "watchdog_Stm32f1.h"

#include "HAL_timers_Stm32f1.h"


// --------------------------------------------------------------------------
// Defines
// --------------------------------------------------------------------------

#if SERIAL_PORT == -1
  #define MYSERIAL SerialUSB
#elif SERIAL_PORT == 0
  #define MYSERIAL Serial
#elif SERIAL_PORT == 1
  #define MYSERIAL Serial1
#elif SERIAL_PORT == 2
  #define MYSERIAL Serial2
#elif SERIAL_PORT == 3
  #define MYSERIAL Serial3
#endif

#define _BV(bit) 	(1 << (bit))

/**
 * TODO: review this to return 1 for pins that are not analog input
 */
#ifndef analogInputToDigitalPin
  #define analogInputToDigitalPin(p) (p)
#endif

#define CRITICAL_SECTION_START	noInterrupts();
#define CRITICAL_SECTION_END    interrupts();

// On AVR this is in math.h?
#define square(x) ((x)*(x))

#ifndef strncpy_P
  #define strncpy_P(dest, src, num) strncpy((dest), (src), (num))
#endif

// Fix bug in pgm_read_ptr
#undef pgm_read_ptr
#define pgm_read_ptr(addr) (*(addr))

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

// --------------------------------------------------------------------------
// Public Variables
// --------------------------------------------------------------------------

/** result of last ADC conversion */
extern uint16_t HAL_adc_result;

// --------------------------------------------------------------------------
// Public functions
// --------------------------------------------------------------------------

// Disable interrupts
#define cli() noInterrupts()

// Enable interrupts
#define sei() interrupts()

// Memory related
#define __bss_end __bss_end__

/** clear reset reason */
void HAL_clear_reset_source (void);

/** reset reason */
uint8_t HAL_get_reset_source (void);

void _delay_ms(const int delay);

/*
extern "C" {
  int freeMemory(void);
}
*/

extern "C" char* _sbrk(int incr);
/*
static int freeMemory() {
  volatile int top;
  top = (int)((char*)&top - reinterpret_cast<char*>(_sbrk(0)));
  return top;
}
*/
static int freeMemory() {
  volatile char top;
  return &top - reinterpret_cast<char*>(_sbrk(0));
}

// SPI: Extended functions which take a channel number (hardware SPI only)
/** Write single byte to specified SPI channel */
void spiSend(uint32_t chan, byte b);
/** Write buffer to specified SPI channel */
void spiSend(uint32_t chan, const uint8_t* buf, size_t n);
/** Read single byte from specified SPI channel */
uint8_t spiRec(uint32_t chan);


// EEPROM

/**
 * TODO: Write all this eeprom stuff. Can emulate eeprom in flash as last resort.
 * Wire library should work for i2c eeproms.
 */
void eeprom_write_byte(unsigned char *pos, unsigned char value);
unsigned char eeprom_read_byte(unsigned char *pos);
void eeprom_read_block (void *__dst, const void *__src, size_t __n);
void eeprom_update_block (const void *__src, void *__dst, size_t __n);

// ADC

#define HAL_ANALOG_SELECT(pin) pinMode(pin, INPUT_ANALOG);

inline void HAL_adc_init(void) {}

#define HAL_START_ADC(pin)  HAL_adc_start_conversion(pin)
#define HAL_READ_ADC        HAL_adc_result

void HAL_adc_start_conversion(const uint8_t adc_pin);

uint16_t HAL_adc_get_result(void);

/* Todo: Confirm none of this is needed.
uint16_t HAL_getAdcReading(uint8_t chan);

void HAL_startAdcConversion(uint8_t chan);
uint8_t HAL_pinToAdcChannel(int pin);

uint16_t HAL_getAdcFreerun(uint8_t chan, bool wait_for_conversion = false);
//uint16_t HAL_getAdcSuperSample(uint8_t chan);

void HAL_enable_AdcFreerun(void);
//void HAL_disable_AdcFreerun(uint8_t chan);

*/

#define GET_PIN_MAP_PIN(index) index
#define GET_PIN_MAP_INDEX(pin) pin
#define PARSED_PIN_INDEX(code, dval) parser.intval(code, dval)

#endif // _HAL_STM32F1_H
