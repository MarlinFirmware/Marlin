/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2025 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#define CPU_32_BIT

#include "../../core/macros.h"
#include "../shared/Marduino.h"
#include "../shared/math_32bit.h"
#include "../shared/HAL_SPI.h"

#include "temp_soc.h"
#include "fastio.h"
#include "Servo.h"

#include "../../inc/MarlinConfigPre.h"

#include <stdint.h>
#include <GPIO.hpp>
#include <AFIO.hpp>

// Default graphical display delays
#define CPU_ST7920_DELAY_1 300
#define CPU_ST7920_DELAY_2  40
#define CPU_ST7920_DELAY_3 340

// Serial Ports
#include "MarlinSerial.h"

// Interrupts
#define CRITICAL_SECTION_START()  const bool irqon = !__get_PRIMASK(); __disable_irq()
#define CRITICAL_SECTION_END()    if (irqon) __enable_irq()

#define cli() __disable_irq()
#define sei() __enable_irq()

// Alias of __bss_end__
#define __bss_end __bss_end__

// Types
typedef double isr_float_t; // FPU ops are used for single-precision, so use double for ISRs.
typedef uint8_t pin_t;      // Parity with mfl platform

// Servo
class libServo;
typedef libServo hal_servo_t;
#define PAUSE_SERVO_OUTPUT()  libServo::pause_all_servos()
#define RESUME_SERVO_OUTPUT() libServo::resume_all_servos()

// Debugging
#define JTAG_DISABLE()    AFIO_I.set_remap(gpio::Pin_Remap_Select::SWJ_DP_ONLY_REMAP)
#define JTAGSWD_DISABLE() AFIO_I.set_remap(gpio::Pin_Remap_Select::SWJ_ALL_DISABLED_REMAP)
#define JTAGSWD_RESET()   AFIO_I.set_remap(gpio::Pin_Remap_Select::FULL_SWJ_REMAP)

// ADC
#ifdef ADC_RESOLUTION
  #define HAL_ADC_RESOLUTION  ADC_RESOLUTION
#else
  #define HAL_ADC_RESOLUTION  12
#endif

#define HAL_ADC_VREF_MV   3300

// Disable Marlin's software oversampling.
// The MFL framework uses 16x hardware oversampling by default
#define HAL_ADC_FILTERED

#define GET_PIN_MAP_PIN(index)        index
#define GET_PIN_MAP_INDEX(pin)        pin
#define PARSED_PIN_INDEX(code, dval)  parser.intval(code, dval)

#ifndef PLATFORM_M997_SUPPORT
  #define PLATFORM_M997_SUPPORT
#endif

void flashFirmware(const int16_t);

#define HAL_CAN_SET_PWM_FREQ    // This HAL supports PWM Frequency adjustment

extern "C" char* _sbrk(int incr);
extern "C" char* dtostrf(double val, signed char width, unsigned char prec, char* sout);

// MarlinHAL Class
class MarlinHAL {
public:
  // Before setup()
  MarlinHAL() {}

  // Watchdog
  static void watchdog_init()    IF_DISABLED(USE_WATCHDOG, {});
  static void watchdog_refresh() IF_DISABLED(USE_WATCHDOG, {});

  static void init();                             // called early in setup()
  static void init_board() {}                     // called less early in setup()
  static void reboot() { NVIC_SystemReset(); }    // restart the firmware from 0x0

  // Interrupts
  static bool isr_state() { return !__get_PRIMASK(); }
  static void isr_on() { sei(); }
  static void isr_off() { cli(); }
  static void delay_ms(const int ms) { delay(ms); }

  // Tasks called from idle()
  static void idletask() {}

  // Reset
  static uint8_t get_reset_source();
  static void clear_reset_source() { RCU_I.clear_all_reset_flags(); }

  // Free SRAM
  static int freeMemory();

  // ADC methods
  static uint16_t adc_result;

  // Called by Temperature::init once at startup
  static void adc_init() { analogReadResolution(HAL_ADC_RESOLUTION); }

  // Called by Temperature::init for each sensor at startup
  static void adc_enable(const pin_t pin) { pinMode(pin, INPUT); }

  // Called from Temperature::isr to start ADC sampling on the given pin
  static void adc_start(const pin_t pin) { adc_result = static_cast<uint16_t>(analogRead(pin)); }

  // Check if ADC is ready for reading
  static bool adc_ready() { return true; }

  // Current value of the ADC register
  static uint16_t adc_value() { return adc_result; }

  // Set the PWM duty cycle for the pin to the given value.
  // Optionally invert the duty cycle [default = false]
  // Optionally change the maximum size of the provided value to enable finer PWM duty control [default = 255]
  static void set_pwm_duty(const pin_t pin, const uint16_t value, const uint16_t scale = 255U, const bool invert = false);

  // Set the frequency of the timer for the given pin.
  // All Timer PWM pins run at the same frequency.
  static void set_pwm_frequency(const pin_t pin, const uint16_t f_desired);
};
