/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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
#ifdef STM32H7

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------

#include "HAL.h"
//#include <stm32h7xx_hal_rcc.h>
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

void HAL_clear_reset_source(void) { __HAL_RCC_CLEAR_RESET_FLAGS(); }

uint8_t HAL_get_reset_source(void) {

  if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDG1RST) != RESET)
    return RST_WATCHDOG;

  if (__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST) != RESET)
    return RST_SOFTWARE;

  if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST) != RESET)
    return RST_EXTERNAL;

  if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST) != RESET)
    return RST_POWER_ON;
  return 0;
}

void _delay_ms(const int delay_ms) { delay(delay_ms); }

extern "C" {
  extern unsigned int _ebss; // end of bss section
}

// return free memory between end of heap (or end bss) and whatever is current

// --------------------------------------------------------------------------
// ADC
// --------------------------------------------------------------------------

void HAL_adc_start_conversion(const uint8_t adc_pin) {
  HAL_adc_result = analogRead(adc_pin);
}

uint16_t HAL_adc_get_result(void) {
  return HAL_adc_result;
}

#endif // STM32H7
