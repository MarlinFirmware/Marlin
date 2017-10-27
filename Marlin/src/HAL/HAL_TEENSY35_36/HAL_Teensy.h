/* **************************************************************************

 Marlin 3D Printer Firmware
 Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 Copyright (c) 2016 Bob Cousins bobcousins42@googlemail.com
 Copyright (c) 2015-2016 Nico Tonnhofer wurstnase.reprap@gmail.com

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
****************************************************************************/

/**
 * Description: HAL for Teensy 3.5 and Teensy 3.6
 */

#ifndef _HAL_TEENSY_H
#define _HAL_TEENSY_H

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------

#include "Arduino.h"

#include "fastio_Teensy.h"
#include "watchdog_Teensy.h"

#include "HAL_timers_Teensy.h"

#include <stdint.h>

// --------------------------------------------------------------------------
// Defines
// --------------------------------------------------------------------------

#undef MOTHERBOARD
#define MOTHERBOARD BOARD_TEENSY35_36

#define IS_32BIT_TEENSY (defined(__MK64FX512__) || defined(__MK66FX1M0__))
#define IS_TEENSY35 defined(__MK64FX512__)
#define IS_TEENSY36 defined(__MK66FX1M0__)

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

#define HAL_SERVO_LIB libServo

typedef int8_t pin_t;

#ifndef analogInputToDigitalPin
  #define analogInputToDigitalPin(p) ((p < 12u) ? (p) + 54u : -1)
#endif

#define CRITICAL_SECTION_START  unsigned char _sreg = SREG; cli();
#define CRITICAL_SECTION_END    SREG = _sreg;

// On AVR this is in math.h?
#define square(x) ((x)*(x))

#ifndef strncpy_P
  #define strncpy_P(dest, src, num) strncpy((dest), (src), (num))
#endif

// Fix bug in pgm_read_ptr
#undef pgm_read_ptr
#define pgm_read_ptr(addr) (*((void**)(addr)))
// Add type-checking to pgm_read_word
#undef pgm_read_word
#define pgm_read_word(addr) (*((uint16_t*)(addr)))

#define RST_POWER_ON   1
#define RST_EXTERNAL   2
#define RST_BROWN_OUT  4
#define RST_WATCHDOG   8
#define RST_JTAG       16
#define RST_SOFTWARE   32
#define RST_BACKUP     64

/** clear reset reason */
void HAL_clear_reset_source(void);

/** reset reason */
uint8_t HAL_get_reset_source(void);

FORCE_INLINE void _delay_ms(const int delay_ms) { delay(delay_ms); }

extern "C" {
  int freeMemory(void);
}

// SPI: Extended functions which take a channel number (hardware SPI only)
/** Write single byte to specified SPI channel */
void spiSend(uint32_t chan, byte b);
/** Write buffer to specified SPI channel */
void spiSend(uint32_t chan, const uint8_t* buf, size_t n);
/** Read single byte from specified SPI channel */
uint8_t spiRec(uint32_t chan);

// ADC

void HAL_adc_init();

#define HAL_START_ADC(pin)  HAL_adc_start_conversion(pin)
#define HAL_READ_ADC        HAL_adc_get_result()

#define HAL_ANALOG_SELECT(pin) NOOP;

void HAL_adc_start_conversion(const uint8_t adc_pin);

uint16_t HAL_adc_get_result(void);

/*
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

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

#endif // _HAL_TEENSY_H
