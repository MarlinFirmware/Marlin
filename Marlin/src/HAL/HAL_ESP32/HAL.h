/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

/**
 * Description: HAL for Espressif ESP32 WiFi
 */

#define CPU_32_BIT

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------

#include <stdint.h>

#undef DISABLED
#undef M_PI

#include <Arduino.h>

#undef DISABLED
#define DISABLED(V...) DO(DIS,&&,V)

#include "../shared/math_32bit.h"
#include "../shared/HAL_SPI.h"

#include "fastio_ESP32.h"
#include "watchdog_ESP32.h"
#include "i2s.h"

#include "HAL_timers_ESP32.h"

#include "WebSocketSerial.h"

// --------------------------------------------------------------------------
// Defines
// --------------------------------------------------------------------------

extern portMUX_TYPE spinlock;

#define MYSERIAL0 Serial

#if ENABLED(WIFISUPPORT)
  #define NUM_SERIAL 2
  #define MYSERIAL1 webSocketSerial
#else
  #define NUM_SERIAL 1
#endif

#define CRITICAL_SECTION_START portENTER_CRITICAL(&spinlock)
#define CRITICAL_SECTION_END   portEXIT_CRITICAL(&spinlock)
#define ISRS_ENABLED() (spinlock.owner == portMUX_FREE_VAL)
#define ENABLE_ISRS()  if (spinlock.owner != portMUX_FREE_VAL) portEXIT_CRITICAL(&spinlock)
#define DISABLE_ISRS() portENTER_CRITICAL(&spinlock)


// Fix bug in pgm_read_ptr
#undef pgm_read_ptr
#define pgm_read_ptr(addr) (*(addr))

// --------------------------------------------------------------------------
// Types
// --------------------------------------------------------------------------

typedef int16_t pin_t;

// --------------------------------------------------------------------------
// Public Variables
// --------------------------------------------------------------------------

/** result of last ADC conversion */
extern uint16_t HAL_adc_result;

// --------------------------------------------------------------------------
// Public functions
// --------------------------------------------------------------------------

// clear reset reason
void HAL_clear_reset_source (void);

// reset reason
uint8_t HAL_get_reset_source (void);

void _delay_ms(int delay);

int freeMemory(void);

void analogWrite(int pin, int value);

// EEPROM
void eeprom_write_byte(uint8_t *pos, unsigned char value);
uint8_t eeprom_read_byte(uint8_t *pos);
void eeprom_read_block (void *__dst, const void *__src, size_t __n);
void eeprom_update_block (const void *__src, void *__dst, size_t __n);

// ADC
#define HAL_ANALOG_SELECT(pin)

void HAL_adc_init(void);

#define HAL_START_ADC(pin)  HAL_adc_start_conversion(pin)
#define HAL_READ_ADC()      HAL_adc_result
#define HAL_ADC_READY()     true

void HAL_adc_start_conversion (uint8_t adc_pin);

#define GET_PIN_MAP_PIN(index) index
#define GET_PIN_MAP_INDEX(pin) pin
#define PARSED_PIN_INDEX(code, dval) parser.intval(code, dval)

// Enable hooks into idle and setup for HAL
#define HAL_IDLETASK 1
#define HAL_INIT 1
void HAL_idletask(void);
void HAL_init(void);
