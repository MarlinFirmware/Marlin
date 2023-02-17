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

/**
 * HAL for stm32duino.com based on Libmaple and compatible (HC32F46x based on STM32F1)
 */

#define CPU_32_BIT

#include "../../core/macros.h"
#include "../shared/Marduino.h"
#include "../shared/math_32bit.h"
#include "../shared/HAL_SPI.h"

#include "fastio.h"
#include "timers.h"

#include <stdint.h>

#include "../../inc/MarlinConfigPre.h"
#include "../inc/MarlinConfig.h"
#include "MarlinSerial.h"

//
// Serial Ports
//

#define _MSERIAL(X) MSerial##X
#define MSERIAL(X) _MSERIAL(X)

#define MYSERIAL1 MSERIAL(2)
#define LCD_SERIAL MSERIAL(4)

#if !WITHIN(SERIAL_PORT, -1, 5)
#error "SERIAL_PORT must be from -1 to 5"
#endif

#if !WITHIN(SERIAL_PORT_2, -1, 5)
#error "SERIAL_PORT_2 must be from -1 to 5"
#elif SERIAL_PORT_2 == SERIAL_PORT
#error "SERIAL_PORT_2 must be different than SERIAL_PORT"
#endif

//
// Emergency Parser
//

#if ENABLED(EMERGENCY_PARSER)
void usart_rx_irq_hook(uint8_t ch, uint8_t usart);
#endif

//
// Misc. Defines
//

#define STM32_FLASH_SIZE 256
#define square(x) ((x) * (x))

#ifndef strncpy_P
#define strncpy_P(dest, src, num) strncpy((dest), (src), (num))
#endif

//
// Misc. Functions
//

/**
 * TODO: review this to return 1 for pins that are not analog input
 */
#ifndef analogInputToDigitalPin
#define analogInputToDigitalPin(p) (p)
#endif

#ifndef digitalPinHasPWM
#define digitalPinHasPWM(P) (P) //(PIN_MAP[P].timer_device != nullptr)
#endif

#define CRITICAL_SECTION_START        \
  uint32_t primask = __get_PRIMASK(); \
  (void)__iCliRetVal()

#define CRITICAL_SECTION_END \
  if (!primask)              \
  (void)__iSeiRetVal()

// Fix bug in pgm_read_ptr
#undef pgm_read_ptr
#define pgm_read_ptr(addr) (*(addr))

// Disable interrupts
#define cli() noInterrupts()

// Enable interrupts
#define sei() interrupts()

// Memory related
#define __bss_end __bss_end__

//
// EEPROM
//

/**
 * TODO: Write all this EEPROM stuff. Can emulate EEPROM in flash as last resort.
 * Wire library should work for i2c EEPROMs.
 */
void eeprom_write_byte(uint8_t *pos, unsigned char value);
uint8_t eeprom_read_byte(uint8_t *pos);
void eeprom_read_block(void *__dst, const void *__src, size_t __n);
void eeprom_update_block(const void *__src, void *__dst, size_t __n);

//
// ADC
//

#define HAL_ADC_VREF 3.3
#define HAL_ADC_RESOLUTION 10

#define GET_PIN_MAP_PIN(index) index
#define GET_PIN_MAP_INDEX(pin) pin
#define PARSED_PIN_INDEX(code, dval) parser.intval(code, dval)

#define JTAG_DISABLE()    // afio_cfg_debug_ports(AFIO_DEBUG_SW_ONLY)
#define JTAGSWD_DISABLE() // afio_cfg_debug_ports(AFIO_DEBUG_NONE)

//
// MarlinHAL implementation
//

#include "MarlinHAL.h"
