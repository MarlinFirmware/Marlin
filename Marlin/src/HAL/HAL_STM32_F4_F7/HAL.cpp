/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#if defined(STM32GENERIC) && (defined(STM32F4) || defined(STM32F7))

#include "HAL.h"

//#include <Wire.h>

// ------------------------
// Public Variables
// ------------------------

uint16_t HAL_adc_result;

// ------------------------
// Public functions
// ------------------------

/* VGPV Done with defines
// disable interrupts
void cli() { noInterrupts(); }

// enable interrupts
void sei() { interrupts(); }
*/

void HAL_clear_reset_source() { __HAL_RCC_CLEAR_RESET_FLAGS(); }

uint8_t HAL_get_reset_source() {
  if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST) != RESET) return RST_WATCHDOG;
  if (__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST)  != RESET) return RST_SOFTWARE;
  if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST)  != RESET) return RST_EXTERNAL;
  if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST)  != RESET) return RST_POWER_ON;
  return 0;
}

void _delay_ms(const int delay_ms) { delay(delay_ms); }

extern "C" {
  extern unsigned int _ebss; // end of bss section
}

// return free memory between end of heap (or end bss) and whatever is current

/*
#include "wirish/syscalls.c"
//extern caddr_t _sbrk(int incr);
#ifndef CONFIG_HEAP_END
extern char _lm_heap_end;
#define CONFIG_HEAP_END ((caddr_t)&_lm_heap_end)
#endif

extern "C" {
  static int freeMemory() {
    char top = 't';
    return &top - reinterpret_cast<char*>(sbrk(0));
  }
  int freeMemory() {
    int free_memory;
    int heap_end = (int)_sbrk(0);
    free_memory = ((int)&free_memory) - ((int)heap_end);
    return free_memory;
  }
}
*/

// ------------------------
// ADC
// ------------------------

void HAL_adc_start_conversion(const uint8_t adc_pin) { HAL_adc_result = analogRead(adc_pin); }

uint16_t HAL_adc_get_result() { return HAL_adc_result; }

#endif // STM32GENERIC && (STM32F4 || STM32F7)
