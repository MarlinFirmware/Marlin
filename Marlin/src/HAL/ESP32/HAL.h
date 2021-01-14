/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 * Copyright (c) 2016 Bob Cousins bobcousins42@googlemail.com
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * HAL for Espressif ESP32 WiFi
 */

#define CPU_32_BIT

#include <stdint.h>

#include "../shared/Marduino.h"
#include "../shared/math_32bit.h"
#include "../shared/HAL_SPI.h"

#include "fastio.h"
#include "watchdog.h"
#include "i2s.h"

#if ENABLED(WIFISUPPORT)
  #include "WebSocketSerial.h"
#endif

#if ENABLED(ESP3D_WIFISUPPORT)
  #include "esp3dlib.h"
#endif

#include "FlushableHardwareSerial.h"

// ------------------------
// Defines
// ------------------------

extern portMUX_TYPE spinlock;

#define MYSERIAL0 flushableSerial

#if EITHER(WIFISUPPORT, ESP3D_WIFISUPPORT)
  #if ENABLED(ESP3D_WIFISUPPORT)
    #define MYSERIAL1 Serial2Socket
  #else
    #define MYSERIAL1 webSocketSerial
  #endif
#endif

#define CRITICAL_SECTION_START() portENTER_CRITICAL(&spinlock)
#define CRITICAL_SECTION_END()   portEXIT_CRITICAL(&spinlock)
#define ISRS_ENABLED() (spinlock.owner == portMUX_FREE_VAL)
#define ENABLE_ISRS()  if (spinlock.owner != portMUX_FREE_VAL) portEXIT_CRITICAL(&spinlock)
#define DISABLE_ISRS() portENTER_CRITICAL(&spinlock)

// Fix bug in pgm_read_ptr
#undef pgm_read_ptr
#define pgm_read_ptr(addr) (*(addr))

// ------------------------
// Types
// ------------------------

typedef int16_t pin_t;

#define HAL_SERVO_LIB Servo

// ------------------------
// Public Variables
// ------------------------

/** result of last ADC conversion */
extern uint16_t HAL_adc_result;

// ------------------------
// Public functions
// ------------------------

//
// Tone
//
void toneInit();
void tone(const pin_t _pin, const unsigned int frequency, const unsigned long duration=0);
void noTone(const pin_t _pin);

// clear reset reason
void HAL_clear_reset_source();

// reset reason
uint8_t HAL_get_reset_source();

inline void HAL_reboot() {}  // reboot the board or restart the bootloader

void _delay_ms(int delay);

#if GCC_VERSION <= 50000
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wunused-function"
#endif

int freeMemory();

#if GCC_VERSION <= 50000
  #pragma GCC diagnostic pop
#endif

void analogWrite(pin_t pin, int value);

// ADC
#define HAL_ANALOG_SELECT(pin)

void HAL_adc_init();

#define HAL_ADC_VREF         3.3
#define HAL_ADC_RESOLUTION  10
#define HAL_START_ADC(pin)  HAL_adc_start_conversion(pin)
#define HAL_READ_ADC()      HAL_adc_result
#define HAL_ADC_READY()     true

void HAL_adc_start_conversion(const uint8_t adc_pin);

#define GET_PIN_MAP_PIN(index) index
#define GET_PIN_MAP_INDEX(pin) pin
#define PARSED_PIN_INDEX(code, dval) parser.intval(code, dval)

// Enable hooks into idle and setup for HAL
#define HAL_IDLETASK 1
#define BOARD_INIT() HAL_init_board();
void HAL_idletask();
void HAL_init();
void HAL_init_board();

//
// Delay in cycles (used by DELAY_NS / DELAY_US)
//
FORCE_INLINE static void DELAY_CYCLES(uint32_t x) {
  unsigned long start, ccount, stop;

  /**
   * It's important to care for race conditions (and overflows) here.
   * Race condition example: If `stop` calculates to being close to the upper boundary of
   * `uint32_t` and if at the same time a longer loop interruption kicks in (e.g. due to other
   * FreeRTOS tasks or interrupts), `ccount` might overflow (and therefore be below `stop` again)
   * without the loop ever being able to notice that `ccount` had already been above `stop` once
   * (and that therefore the number of cycles to delay has already passed).
   * As DELAY_CYCLES (through DELAY_NS / DELAY_US) is used by software SPI bit banging to drive
   * LCDs and therefore might be called very, very often, this seemingly improbable situation did
   * actually happen in reality. It resulted in apparently random print pauses of ~17.9 seconds
   * (0x100000000 / 240 MHz) or multiples thereof, essentially ruining the current print by causing
   * large blobs of filament.
   */

  __asm__ __volatile__ ( "rsr     %0, ccount" : "=a" (start) );
  stop = start + x;
  ccount = start;

  if (stop >= start) {
    // no overflow, so only loop while in between start and stop:
    // 0x00000000 -----------------start****stop-- 0xFFFFFFFF
    while (ccount >= start && ccount < stop) {
      __asm__ __volatile__ ( "rsr     %0, ccount" : "=a" (ccount) );
    }
  }
  else {
    // stop did overflow, so only loop while outside of stop and start:
    // 0x00000000 **stop-------------------start** 0xFFFFFFFF
    while (ccount >= start || ccount < stop) {
      __asm__ __volatile__ ( "rsr     %0, ccount" : "=a" (ccount) );
    }
  }

}
