/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 * SAMD51 HAL developed by Giuliano Zaro (AKA GMagician)
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

#define CPU_32_BIT

#include "../shared/Marduino.h"
#include "../shared/math_32bit.h"
#include "../shared/HAL_SPI.h"
#include "fastio.h"
#include "watchdog.h"

#ifdef ADAFRUIT_GRAND_CENTRAL_M4
  #include "MarlinSerial_AGCM4.h"

  // Serial ports

  // MYSERIAL0 required before MarlinSerial includes!

  #define __MSERIAL(X) Serial##X
  #define _MSERIAL(X) __MSERIAL(X)
  #define MSERIAL(X) _MSERIAL(INCREMENT(X))

  #if SERIAL_PORT == -1
    #define MYSERIAL0 Serial
  #elif WITHIN(SERIAL_PORT, 0, 3)
    #define MYSERIAL0 MSERIAL(SERIAL_PORT)
  #else
    #error "SERIAL_PORT must be from -1 to 3. Please update your configuration."
  #endif

  #ifdef SERIAL_PORT_2
    #if SERIAL_PORT_2 == -1
      #define MYSERIAL1 Serial
    #elif WITHIN(SERIAL_PORT_2, 0, 3)
      #define MYSERIAL1 MSERIAL(SERIAL_PORT_2)
    #else
      #error "SERIAL_PORT_2 must be from -1 to 3. Please update your configuration."
    #endif
  #endif

  #ifdef MMU2_SERIAL_PORT
    #if MMU2_SERIAL_PORT == -1
      #define MMU2_SERIAL Serial
    #elif WITHIN(MMU2_SERIAL_PORT, 0, 3)
      #define MMU2_SERIAL MSERIAL(MMU2_SERIAL_PORT)
    #else
      #error "MMU2_SERIAL_PORT must be from -1 to 3. Please update your configuration."
    #endif
  #endif

  #ifdef LCD_SERIAL_PORT
    #if LCD_SERIAL_PORT == -1
      #define LCD_SERIAL Serial
    #elif WITHIN(LCD_SERIAL_PORT, 0, 3)
      #define LCD_SERIAL MSERIAL(LCD_SERIAL_PORT)
    #else
      #error "LCD_SERIAL_PORT must be from -1 to 3. Please update your configuration."
    #endif
  #endif

#endif // ADAFRUIT_GRAND_CENTRAL_M4

typedef int8_t pin_t;

#define SHARED_SERVOS HAS_SERVOS
#define HAL_SERVO_LIB Servo

//
// Interrupts
//
#define CRITICAL_SECTION_START()  uint32_t primask = __get_PRIMASK(); __disable_irq()
#define CRITICAL_SECTION_END()    if (!primask) __enable_irq()
#define ISRS_ENABLED() (!__get_PRIMASK())
#define ENABLE_ISRS()  __enable_irq()
#define DISABLE_ISRS() __disable_irq()

#define cli() __disable_irq()       // Disable interrupts
#define sei() __enable_irq()        // Enable interrupts

void HAL_clear_reset_source();  // clear reset reason
uint8_t HAL_get_reset_source(); // get reset reason

inline void HAL_reboot() {}  // reboot the board or restart the bootloader

//
// ADC
//
extern uint16_t HAL_adc_result;     // Most recent ADC conversion

#define HAL_ANALOG_SELECT(pin)

void HAL_adc_init();

//#define HAL_ADC_FILTERED          // Disable Marlin's oversampling. The HAL filters ADC values.
#define HAL_ADC_VREF         3.3
#define HAL_ADC_RESOLUTION  10      // ... 12
#define HAL_START_ADC(pin)  HAL_adc_start_conversion(pin)
#define HAL_READ_ADC()      HAL_adc_result
#define HAL_ADC_READY()     true

void HAL_adc_start_conversion(const uint8_t adc_pin);

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
void HAL_init();
/*
#define HAL_IDLETASK 1
void HAL_idletask();
*/

//
// Utility functions
//
FORCE_INLINE void _delay_ms(const int delay_ms) { delay(delay_ms); }

#if GCC_VERSION <= 50000
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wunused-function"
#endif

int freeMemory();

#if GCC_VERSION <= 50000
  #pragma GCC diagnostic pop
#endif

#ifdef __cplusplus
  extern "C" {
#endif
char *dtostrf(double __val, signed char __width, unsigned char __prec, char *__s);
#ifdef __cplusplus
  }
#endif
