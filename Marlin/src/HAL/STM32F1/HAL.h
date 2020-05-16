/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#pragma once

/**
 * HAL for stm32duino.com based on Libmaple and compatible (STM32F1)
 */

#define CPU_32_BIT

#include "../../core/macros.h"
#include "../shared/Marduino.h"
#include "../shared/math_32bit.h"
#include "../shared/HAL_SPI.h"

#include "fastio.h"
#include "watchdog.h"

#include "timers.h"

#include <stdint.h>
#include <util/atomic.h>

#include "../../inc/MarlinConfigPre.h"

#ifdef USE_USB_COMPOSITE
  #include "msc_sd.h"
#endif

// ------------------------
// Defines
// ------------------------

#ifndef STM32_FLASH_SIZE
  #if defined(MCU_STM32F103RE) || defined(MCU_STM32F103VE)
    #define STM32_FLASH_SIZE 512
  #else
    #define STM32_FLASH_SIZE 256
  #endif
#endif

#ifdef SERIAL_USB
  #ifndef USE_USB_COMPOSITE
    #define UsbSerial Serial
  #else
    #define UsbSerial MarlinCompositeSerial
  #endif
  #define MSerial1  Serial1
  #define MSerial2  Serial2
  #define MSerial3  Serial3
  #define MSerial4  Serial4
  #define MSerial5  Serial5
#else
  #define MSerial1  Serial
  #define MSerial2  Serial1
  #define MSerial3  Serial2
  #define MSerial4  Serial3
  #define MSerial5  Serial4
#endif

#if SERIAL_PORT == 0
  #error "SERIAL_PORT cannot be 0. (Port 0 does not exist.) Please update your configuration."
#elif SERIAL_PORT == -1
  #define MYSERIAL0 UsbSerial
#elif SERIAL_PORT == 1
  #define MYSERIAL0 MSerial1
#elif SERIAL_PORT == 2
  #define MYSERIAL0 MSerial2
#elif SERIAL_PORT == 3
  #define MYSERIAL0 MSerial3
#elif SERIAL_PORT == 4
  #define MYSERIAL0 MSerial4
#elif SERIAL_PORT == 5
  #define MYSERIAL0 MSerial5
#else
  #error "SERIAL_PORT must be from -1 to 5. Please update your configuration."
#endif

#ifdef SERIAL_PORT_2
  #if SERIAL_PORT_2 == 0
    #error "SERIAL_PORT_2 cannot be 0. (Port 0 does not exist.) Please update your configuration."
  #elif SERIAL_PORT_2 == SERIAL_PORT
    #error "SERIAL_PORT_2 must be different than SERIAL_PORT. Please update your configuration."
  #elif SERIAL_PORT_2 == -1
    #define MYSERIAL1 UsbSerial
  #elif SERIAL_PORT_2 == 1
    #define MYSERIAL1 MSerial1
  #elif SERIAL_PORT_2 == 2
    #define MYSERIAL1 MSerial2
  #elif SERIAL_PORT_2 == 3
    #define MYSERIAL1 MSerial3
  #elif SERIAL_PORT_2 == 4
    #define MYSERIAL1 MSerial4
  #elif SERIAL_PORT_2 == 5
    #define MYSERIAL1 MSerial5
  #else
    #error "SERIAL_PORT_2 must be from -1 to 5. Please update your configuration."
  #endif
  #define NUM_SERIAL 2
#else
  #define NUM_SERIAL 1
#endif

#ifdef DGUS_SERIAL
  #if DGUS_SERIAL_PORT == 0
    #error "DGUS_SERIAL_PORT cannot be 0. (Port 0 does not exist.) Please update your configuration."
  #elif DGUS_SERIAL_PORT == SERIAL_PORT
    #error "DGUS_SERIAL_PORT must be different than SERIAL_PORT. Please update your configuration."
  #elif defined(SERIAL_PORT_2) && DGUS_SERIAL_PORT == SERIAL_PORT_2
    #error "DGUS_SERIAL_PORT must be different than SERIAL_PORT_2. Please update your configuration."
  #elif DGUS_SERIAL_PORT == -1
    #define DGUS_SERIAL UsbSerial
  #elif DGUS_SERIAL_PORT == 1
    #define DGUS_SERIAL MSerial1
  #elif DGUS_SERIAL_PORT == 2
    #define DGUS_SERIAL MSerial2
  #elif DGUS_SERIAL_PORT == 3
    #define DGUS_SERIAL MSerial3
  #elif DGUS_SERIAL_PORT == 4
    #define DGUS_SERIAL MSerial4
  #elif DGUS_SERIAL_PORT == 5
    #define DGUS_SERIAL MSerial5
  #else
    #error "DGUS_SERIAL_PORT must be from -1 to 5. Please update your configuration."
  #endif
#endif


// Set interrupt grouping for this MCU
void HAL_init();
#define HAL_IDLETASK 1
void HAL_idletask();

/**
 * TODO: review this to return 1 for pins that are not analog input
 */
#ifndef analogInputToDigitalPin
  #define analogInputToDigitalPin(p) (p)
#endif

#ifndef digitalPinHasPWM
  #define digitalPinHasPWM(P) (PIN_MAP[P].timer_device != nullptr)
  #define NO_COMPILE_TIME_PWM
#endif

#define CRITICAL_SECTION_START()  uint32_t primask = __get_primask(); (void)__iCliRetVal()
#define CRITICAL_SECTION_END()    if (!primask) (void)__iSeiRetVal()
#define ISRS_ENABLED() (!__get_primask())
#define ENABLE_ISRS()  ((void)__iSeiRetVal())
#define DISABLE_ISRS() ((void)__iCliRetVal())

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

// ------------------------
// Types
// ------------------------

typedef int8_t pin_t;

// ------------------------
// Public Variables
// ------------------------

// Result of last ADC conversion
extern uint16_t HAL_adc_result;

// ------------------------
// Public functions
// ------------------------

// Disable interrupts
#define cli() noInterrupts()

// Enable interrupts
#define sei() interrupts()

// Memory related
#define __bss_end __bss_end__

// Clear reset reason
void HAL_clear_reset_source();

// Reset reason
uint8_t HAL_get_reset_source();

void _delay_ms(const int delay);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

/*
extern "C" {
  int freeMemory();
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

#pragma GCC diagnostic pop

//
// ADC
//

#define HAL_ANALOG_SELECT(pin) pinMode(pin, INPUT_ANALOG);

void HAL_adc_init();

#define HAL_START_ADC(pin)  HAL_adc_start_conversion(pin)
#define HAL_ADC_RESOLUTION  10
#define HAL_READ_ADC()      HAL_adc_result
#define HAL_ADC_READY()     true

void HAL_adc_start_conversion(const uint8_t adc_pin);
uint16_t HAL_adc_get_result();

uint16_t analogRead(pin_t pin); // need HAL_ANALOG_SELECT() first
void analogWrite(pin_t pin, int pwm_val8); // PWM only! mul by 257 in maple!?

#define GET_PIN_MAP_PIN(index) index
#define GET_PIN_MAP_INDEX(pin) pin
#define PARSED_PIN_INDEX(code, dval) parser.intval(code, dval)

#define JTAG_DISABLE() afio_cfg_debug_ports(AFIO_DEBUG_SW_ONLY)
#define JTAGSWD_DISABLE() afio_cfg_debug_ports(AFIO_DEBUG_NONE)

#define PLATFORM_M997_SUPPORT
void flashFirmware(const int16_t);
