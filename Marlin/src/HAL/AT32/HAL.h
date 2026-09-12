/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * HAL for AT32F403 based boards
 *
 * Note: MarlinHAL class is in MarlinHAL.h/cpp
 */

#define CPU_32_BIT

#include "../../inc/MarlinConfig.h"

#include "../shared/Marduino.h"
#include "../shared/math_32bit.h"
#include "../shared/HAL_SPI.h"
#include "./pins_arduino.h"

#include "fastio.h"
#include "timers.h"
#include "MarlinHAL.h"

//
// Serial Ports
//
#include "MarlinSerial.h"

//
// Emergency Parser
//
#if ENABLED(EMERGENCY_PARSER)
  extern "C" void usart_rx_irq_hook(uint8_t ch, uint8_t usart);
#endif

//
// Misc. Defines
//
#define square(x) ((x) * (x))

#ifndef strncpy_P
  #define strncpy_P(dest, src, num) strncpy((dest), (src), (num))
#endif

#ifndef analogInputToDigitalPin
  #define analogInputToDigitalPin(p) pin_t(p)
#endif

#define CRITICAL_SECTION_START()        \
  const bool irqon = !__get_PRIMASK();  \
  __disable_irq();                      \
  __DSB();
#define CRITICAL_SECTION_END()          \
  __DSB();                              \
  if (irqon) __enable_irq();

#ifndef cli
  #define cli() __disable_irq() // __set_PRIMASK(1)
#endif
#ifndef sei
  #define sei() __enable_irq()  // __set_PRIMASK(0)
#endif

#define __bss_end __bss_end__

// Servo
class libServo;
typedef libServo hal_servo_t;
#define PAUSE_SERVO_OUTPUT()  libServo::pause_all_servos()
#define RESUME_SERVO_OUTPUT() libServo::resume_all_servos()

#define JTAG_DISABLE()
#define JTAGSWD_DISABLE()
#define JTAGSWD_RESET()

#ifdef ADC_RESOLUTION
  #define HAL_ADC_RESOLUTION ADC_RESOLUTION
#else
  #define HAL_ADC_RESOLUTION 12
#endif
#define HAL_ADC_VREF_MV 3300

#define GET_PIN_MAP_PIN(index)       index
#define GET_PIN_MAP_INDEX(pin)       pin
#define PARSED_PIN_INDEX(code, dval) parser.intval(code, dval)

#define HAL_CAN_SET_PWM_FREQ

extern "C" char* _sbrk(int incr);
extern "C" char* dtostrf(double val, signed char width, unsigned char prec, char* sout);
