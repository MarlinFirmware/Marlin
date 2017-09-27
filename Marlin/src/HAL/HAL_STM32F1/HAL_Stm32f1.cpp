/* **************************************************************************

Marlin 3D Printer Firmware
Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
Copyright (c) 2016 Bob Cousins bobcousins42@googlemail.com
Copyright (c) 2017 Victor Perez 

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
****************************************************************************/


/**
* Description: HAL for Arduino Stm32f1 (libmaple based stm32duino.com)
*
* For Libmaple based STM32F1
*/


#ifdef __STM32F1__

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------

#include "../HAL.h"

//#include <Wire.h>

// --------------------------------------------------------------------------
// Externals
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Local defines
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Types
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Variables
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Public Variables
// --------------------------------------------------------------------------

uint16_t HAL_adc_result;

// --------------------------------------------------------------------------
// Private Variables
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Function prototypes
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Private functions
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Public functions
// --------------------------------------------------------------------------

/* VGPV Done with defines
// disable interrupts
void cli(void) { noInterrupts(); }

// enable interrupts
void sei(void) { interrupts(); }
*/

void HAL_clear_reset_source(void) { }
/* Todo: Check this and change or remove.
* currently returns 1 that's equal to poweron reset.
*/
uint8_t HAL_get_reset_source (void) {
    return 1;
}

void _delay_ms(int delay_ms) {
    delay (delay_ms);
}

extern "C" {
    extern unsigned int _ebss; // end of bss section
}

/* Todo: Change this to correct it for libmaple
*/

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
// --------------------------------------------------------------------------
// ADC
// --------------------------------------------------------------------------

void HAL_adc_start_conversion (uint8_t adc_pin) {
    HAL_adc_result = (analogRead(adc_pin) >> 2)& 0x3ff; // shift to get 10 bits only.
}

uint16_t HAL_adc_get_result(void) {
    return HAL_adc_result;
}

#endif // __STM32F1__
