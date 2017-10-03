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
#include <STM32ADC.h>

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
USBSerial SerialUSB;
STM32ADC adc(ADC1);

uint8 adc_pins[] = {
  #if HAS_TEMP_0
    TEMP_0_PIN,
  #endif
  #if HAS_TEMP_1
    TEMP_1_PIN
  #endif
  #if HAS_TEMP_2
    TEMP_2_PIN,
  #endif
  #if HAS_TEMP_3
    TEMP_3_PIN,
  #endif
  #if HAS_TEMP_4
    TEMP_4_PIN,
  #endif
  #if HAS_TEMP_BED
    TEMP_BED_PIN,
  #endif
  #if ENABLED(FILAMENT_WIDTH_SENSOR)
    FILWIDTH_PIN,
  #endif
};

enum TEMP_PINS
{
    TEMP_0,
    TEMP_1,
    TEMP_2,
    TEMP_3,
    TEMP_4,
    TEMP_BED,
    FILWIDTH
};

#define ADC_PIN_COUNT (sizeof(adc_pins)/sizeof(adc_pins[0]))
uint16_t HAL_adc_results[ADC_PIN_COUNT];


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
// Init the AD in continuous capture mode
void HAL_adc_init(void)
{
  // configure the ADC
  adc.calibrate();
  adc.setSampleRate(ADC_SMPR_1_5); // ?
  adc.setPins(adc_pins, ADC_PIN_COUNT);
  adc.setDMA(HAL_adc_results, ADC_PIN_COUNT, (DMA_MINC_MODE | DMA_CIRC_MODE), NULL);
  adc.setScanMode();
  adc.setContinuous();
  adc.startConversion();

  // HACK - Not needed once the pin mapping/fastio stuff is done
  for (int i = 0; i < ADC_PIN_COUNT; i++)
  {
    pinMode(adc_pins[i], INPUT_ANALOG);
  }
}

void HAL_adc_start_conversion (uint8_t adc_pin) {
    TEMP_PINS pin_index = TEMP_0;
    if (adc_pin == TEMP_0_PIN){
          pin_index = TEMP_0;
    }

    else if (adc_pin == TEMP_1_PIN) {
          pin_index = TEMP_1;
    }
    else if (adc_pin == TEMP_2_PIN) {
          pin_index = TEMP_2;
    }
    else if (adc_pin == TEMP_3_PIN) {
          pin_index = TEMP_3;
    }
    else if (adc_pin == TEMP_4_PIN) {
          pin_index = TEMP_4;
    }
    else if (adc_pin == TEMP_BED_PIN) {
          pin_index = TEMP_BED;
    }
#if ENABLED(FILAMENT_WIDTH_SENSOR)
    else if (adc_pin == FILWIDTH_PIN) {
          pin_index = FILWIDTH;
    }
#endif

    HAL_adc_result = (HAL_adc_results[(int)pin_index] >> 2)& 0x3ff; // shift to get 10 bits only.
}

uint16_t HAL_adc_get_result(void) {
    return HAL_adc_result;
}

#endif // __STM32F1__
