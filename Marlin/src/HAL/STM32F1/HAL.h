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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
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


#include <stdint.h>
#include <util/atomic.h>

#include "../../inc/MarlinConfigPre.h"

#if HAS_SD_HOST_DRIVE
  #include "msc_sd.h"
#endif

#include "MarlinSerial.h"

// ------------------------
// Defines
// ------------------------

#ifndef STM32_FLASH_SIZE
  #if EITHER(MCU_STM32F103RE, MCU_STM32F103VE)
    #define STM32_FLASH_SIZE 512
  #else
    #define STM32_FLASH_SIZE 256
  #endif
#endif

#ifdef SERIAL_USB
  typedef ForwardSerial1Class< USBSerial > DefaultSerial1;
  extern DefaultSerial1 MSerial0;

  #if !HAS_SD_HOST_DRIVE
    #define UsbSerial MSerial0
  #else
    #define UsbSerial MarlinCompositeSerial
  #endif
#endif

#define _MSERIAL(X) MSerial##X
#define MSERIAL(X) _MSERIAL(X)

#if EITHER(STM32_HIGH_DENSITY, STM32_XL_DENSITY)
  #define NUM_UARTS 5
#else
  #define NUM_UARTS 3
#endif

#if SERIAL_PORT == -1
  #define MYSERIAL1 UsbSerial
#elif WITHIN(SERIAL_PORT, 1, NUM_UARTS)
  #define MYSERIAL1 MSERIAL(SERIAL_PORT)
#else
  #define MYSERIAL1 MSERIAL(1) // dummy port
  #if NUM_UARTS == 5
    #error "SERIAL_PORT must be -1 or from 1 to 5. Please update your configuration."
  #else
    #error "SERIAL_PORT must be -1 or from 1 to 3. Please update your configuration."
  #endif
#endif

#ifdef SERIAL_PORT_2
  #if SERIAL_PORT_2 == -1
    #define MYSERIAL2 UsbSerial
  #elif WITHIN(SERIAL_PORT_2, 1, NUM_UARTS)
    #define MYSERIAL2 MSERIAL(SERIAL_PORT_2)
  #else
    #define MYSERIAL2 MSERIAL(1) // dummy port
    #if NUM_UARTS == 5
      #error "SERIAL_PORT_2 must be -1 or from 1 to 5. Please update your configuration."
    #else
      #error "SERIAL_PORT_2 must be -1 or from 1 to 3. Please update your configuration."
    #endif
  #endif
#endif

#ifdef MMU2_SERIAL_PORT
  #if MMU2_SERIAL_PORT == -1
    #define MMU2_SERIAL UsbSerial
  #elif WITHIN(MMU2_SERIAL_PORT, 1, NUM_UARTS)
    #define MMU2_SERIAL MSERIAL(MMU2_SERIAL_PORT)
  #else
    #define MMU2_SERIAL MSERIAL(1) // dummy port
    #if NUM_UARTS == 5
      #error "MMU2_SERIAL_PORT must be -1 or from 1 to 5. Please update your configuration."
    #else
      #error "MMU2_SERIAL_PORT must be -1 or from 1 to 3. Please update your configuration."
    #endif
  #endif
#endif

#ifdef LCD_SERIAL_PORT
  #if LCD_SERIAL_PORT == -1
    #define LCD_SERIAL UsbSerial
  #elif WITHIN(LCD_SERIAL_PORT, 1, NUM_UARTS)
    #define LCD_SERIAL MSERIAL(LCD_SERIAL_PORT)
  #else
    #define LCD_SERIAL MSERIAL(1) // dummy port
    #if NUM_UARTS == 5
      #error "LCD_SERIAL_PORT must be -1 or from 1 to 5. Please update your configuration."
    #else
      #error "LCD_SERIAL_PORT must be -1 or from 1 to 3. Please update your configuration."
    #endif
  #endif
  #if HAS_DGUS_LCD
    #define SERIAL_GET_TX_BUFFER_FREE() LCD_SERIAL.availableForWrite()
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
  #define digitalPinHasPWM(P) !!PIN_MAP[P].timer_device
  #define NO_COMPILE_TIME_PWM
#endif

#define CRITICAL_SECTION_START()  uint32_t primask = __get_primask(); (void)__iCliRetVal()
#define CRITICAL_SECTION_END()    if (!primask) (void)__iSeiRetVal()
#define ISRS_ENABLED() (!__get_primask())
#define ENABLE_ISRS()  ((void)__iSeiRetVal())
#define DISABLE_ISRS() ((void)__iCliRetVal())

// On AVR this is in math.h?
#define square(x) ((x)*(x))

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

inline void HAL_reboot() {}  // reboot the board or restart the bootloader

void _delay_ms(const int delay);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

/*
extern "C" {
  int freeMemory();
}
*/

extern "C" char* _sbrk(int incr);

static inline int freeMemory() {
  volatile char top;
  return &top - _sbrk(0);
}

#pragma GCC diagnostic pop

//
// ADC
//

#define HAL_ANALOG_SELECT(pin) pinMode(pin, INPUT_ANALOG);

void HAL_adc_init();

#define HAL_ADC_VREF         3.3
#define HAL_ADC_RESOLUTION  10
#define HAL_START_ADC(pin)  HAL_adc_start_conversion(pin)
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

#define HAL_CAN_SET_PWM_FREQ   // This HAL supports PWM Frequency adjustment

/**
 * set_pwm_frequency
 *  Set the frequency of the timer corresponding to the provided pin
 *  All Timer PWM pins run at the same frequency
 */
void set_pwm_frequency(const pin_t pin, int f_desired);

/**
 * set_pwm_duty
 *  Set the PWM duty cycle of the provided pin to the provided value
 *  Optionally allows inverting the duty cycle [default = false]
 *  Optionally allows changing the maximum size of the provided value to enable finer PWM duty control [default = 255]
 */
void set_pwm_duty(const pin_t pin, const uint16_t v, const uint16_t v_size=255, const bool invert=false);
