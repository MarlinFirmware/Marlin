/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
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
 * HAL_LPC5528/HAL.h
 * Hardware Abstraction Layer for NXP LPC5528
 */

#define CPU_32_BIT

#include "../../core/macros.h"
#include "../shared/Marduino.h"
#include "../shared/math_32bit.h"
#include "../shared/HAL_SPI.h"
#include "fastio.h"
//#include "Servo.h"
//#include "watchdog.h"
//#include "MarlinSerial.h"

//#include "../../inc/MarlinConfigPre.h"
//#include "../../pins/lpc5528/pins_MKS_OWL.h"
#include "MarlinSerial.h"
#include <stdint.h>

//
// Default graphical display delays
//
#define CPU_ST7920_DELAY_1 300
#define CPU_ST7920_DELAY_2  40
#define CPU_ST7920_DELAY_3 340

#define F_CPU 150000000L

//
// Serial Ports --- if use USB CDC
//
#ifdef USBCON
  //#include <USBSerial.h>
  #include "../../core/serial_hook.h"
  typedef ForwardSerial1Class< decltype(UsbSerial) > DefaultSerial1;
  extern DefaultSerial1 MSerial0;
#endif

// ------------------------
// Serial ports
// ------------------------

// redefine for LPC5528 Library
#ifndef byte
  typedef uint8_t byte;
#endif

// define for serial
#define _MSERIAL(X) MSerial##X
#define MSERIAL(X) _MSERIAL(X)

#if SERIAL_PORT == -1
  #define MYSERIAL1 MSerial0
#elif WITHIN(SERIAL_PORT, 1, 8)
  #define MYSERIAL1 MSERIAL(SERIAL_PORT)
#else
  //#error "SERIAL_PORT must be from 1 to 8. You can also use -1 if the board supports Native USB."
#endif

#ifdef SERIAL_PORT_2
  #if SERIAL_PORT_2 == -1
    #define MYSERIAL2 MSerial0
  #elif WITHIN(SERIAL_PORT_2, 1, 8)
    #define MYSERIAL2 MSERIAL(SERIAL_PORT_2)
  #else
    #error "SERIAL_PORT_2 must be from 1 to 8. You can also use -1 if the board supports Native USB."
  #endif
#endif

#ifdef SERIAL_PORT_3
  #if SERIAL_PORT_3 == -1
    #define MYSERIAL3 MSerial0
  #elif WITHIN(SERIAL_PORT_3, 1, 8)
    #define MYSERIAL3 MSERIAL(SERIAL_PORT_3)
  #else
    #error "SERIAL_PORT_3 must be from 1 to 8. You can also use -1 if the board supports Native USB."
  #endif
#endif

#ifdef LCD_SERIAL_PORT
  #if LCD_SERIAL_PORT == -1
    #define LCD_SERIAL MSerial0
  #elif WITHIN(LCD_SERIAL_PORT, 1, 8)
    #define LCD_SERIAL MSERIAL(LCD_SERIAL_PORT)
  #else
    #error "LCD_SERIAL_PORT must be from 1 to 8. You can also use -1 if the board supports Native USB."
  #endif
  #if HAS_DGUS_LCD
    #define SERIAL_GET_TX_BUFFER_FREE() LCD_SERIAL.availableForWrite()
  #endif
#endif

//
// Interrupts
//

#define CRITICAL_SECTION_START()  uint32_t irqon = __get_PRIMASK(); __disable_irq()
#define CRITICAL_SECTION_END()    if (!irqon) __enable_irq()
#define ISRS_ENABLED() (!__get_PRIMASK())
#define ENABLE_ISRS()  __enable_irq()
#define DISABLE_ISRS() __disable_irq()
#define cli() __disable_irq()
#define sei() __enable_irq()

typedef int16_t pin_t;

// Maple Compatibility
typedef void (*systickCallback_t)(void);
void systick_attach_callback(systickCallback_t cb);
void SYSTICK_Callback();

extern volatile uint32_t systick_uptime_millis;

//
// ADC
//

#ifdef ADC_RESOLUTION
  #define HAL_ADC_RESOLUTION ADC_RESOLUTION
#else
  #define HAL_ADC_RESOLUTION   12   // 15 bit maximum, raw temperature is stored as int16_t
#endif

#define HAL_ADC_VREF            3.3 // ADC voltage reference
#define HAL_ADC_FILTERED

// ------------------------
// Defines
// ------------------------

//#ifndef PLATFORM_M997_SUPPORT
//  #define PLATFORM_M997_SUPPORT
//#endif
//void flashFirmware(const int16_t);

#define HAL_CAN_SET_PWM_FREQ   // This HAL supports PWM Frequency adjustment

//#define HAL_SERVO_LIB libServo
//#define PAUSE_SERVO_OUTPUT() libServo::pause_all_servos()
//#define RESUME_SERVO_OUTPUT() libServo::resume_all_servos()

// ------------------------
// Public functions
// ------------------------

// Memory related
#define __bss_end __bss_end__

extern "C" char* _sbrk(int incr);

#pragma GCC diagnostic push
#if GCC_VERSION <= 50000
  #pragma GCC diagnostic ignored "-Wunused-function"
#endif

int freeMemory();

#pragma GCC diagnostic pop

// ------------------------
// MarlinHAL Class
// ------------------------

class MarlinHAL {
public:

  // Earliest possible init, before setup()
  MarlinHAL() {}

  static void init();          // Called early in setup()
  static void init_board() {}  // Called less early in setup()
  static void reboot();        // Restart the firmware from 0x0

  // Interrupts
  //static bool isr_state() { return !__get_PRIMASK(); }
  static void isr_on()  { __enable_irq(); }
  static void isr_off() { __disable_irq(); }

  static void delay_ms(const int ms) { delay(ms); }

  // Watchdog
  static void watchdog_init() {}
  static void watchdog_refresh() IF_DISABLED(USE_WATCHDOG, {});
  static bool watchdog_timed_out() IF_DISABLED(USE_WATCHDOG, { return false; });
  static void watchdog_clear_timeout_flag() IF_DISABLED(USE_WATCHDOG, {});

  // Tasks, called from idle()
  static void idletask();

  // Reset
  static uint8_t get_reset_source();
  static void clear_reset_source();

  // Free SRAM
  static int freeMemory() { return ::freeMemory(); }

  //
  // ADC Methods
  //

  #ifndef analogInputToDigitalPin
    #define analogInputToDigitalPin(p) (p)
  #endif

  //#define HAL_ANALOG_SELECT(pin) adc_init(pin)

  // ------------------------
  // Public Variables
  // ------------------------

  // result of last ADC conversion
  static uint16_t adc_result;

  static pin_t adc_pin;

  //
  // ADC Methods
  //

  static void adc_init();

  static void adc_start(const pin_t pin);

  // Is the ADC ready for reading?
  static bool adc_ready() { return true; }

  // The current value of the ADC register
  static uint16_t adc_value() { return adc_result; }

  // Called by Temperature::init for each sensor at startup
  static void adc_enable(const pin_t pin) {}

  #define GET_PIN_MAP_PIN(index) index
  #define GET_PIN_MAP_INDEX(pin) pin
  #define PARSED_PIN_INDEX(code, dval) parser.intval(code, dval)

  //extern volatile uint32_t systick_uptime_millis;

  /**
   * Set the PWM duty cycle for the pin to the given value.
   * Optionally invert the duty cycle [default = false]
   * Optionally change the scale of the provided value to enable finer PWM duty control [default = 255]
   */
  static void set_pwm_duty(const pin_t pin, const uint16_t v, const uint16_t v_size=255, const bool invert=false);

  /**
   * Set the frequency of the timer corresponding to the provided pin
   * All Hardware PWM pins will run at the same frequency and
   * All Software PWM pins will run at the same frequency
   */
  static void set_pwm_frequency(const pin_t pin, const uint16_t f_desired);
};
