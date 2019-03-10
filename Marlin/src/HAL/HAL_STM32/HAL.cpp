/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#if defined(ARDUINO_ARCH_STM32) && !defined(STM32GENERIC)


// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------

#include "HAL.h"

#include "../../inc/MarlinConfig.h"
#include "../shared/Delay.h"

#if ENABLED(EEPROM_EMULATED_WITH_SRAM)
  #if STM32F7xx
    #include "stm32f7xx_ll_pwr.h"
  #else
    #error "EEPROM_EMULATED_WITH_SRAM is currently only supported for STM32F7xx"
  #endif
#endif // EEPROM_EMULATED_WITH_SRAM

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


// Needed for DELAY_NS() / DELAY_US() on CORTEX-M7
#if (defined(__arm__) || defined(__thumb__)) && __CORTEX_M == 7
  // HAL pre-initialization task
  // Force the preinit function to run between the premain() and main() function
  // of the STM32 arduino core
  __attribute__((constructor (102)))
  void HAL_preinit() {
    enableCycleCounter();
  }
#endif

// HAL initialization task
void HAL_init(void) {
  FastIO_init();

  #if ENABLED(SDSUPPORT)
    OUT_WRITE(SDSS, HIGH); // Try to set SDSS inactive before any other SPI users start up
  #endif

  #if PIN_EXISTS(LED)
    OUT_WRITE(LED_PIN, LOW);
  #endif

  #if ENABLED(EEPROM_EMULATED_WITH_SRAM)
  // Enable access to backup SRAM
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_BKPSRAM_CLK_ENABLE();

  // Enable backup regulator
  LL_PWR_EnableBkUpRegulator();
  // Wait until backup regulator is initialized
  while (!LL_PWR_IsActiveFlag_BRR());
  #endif // EEPROM_EMULATED_SRAM
}

void HAL_clear_reset_source(void) { __HAL_RCC_CLEAR_RESET_FLAGS(); }

uint8_t HAL_get_reset_source (void) {
  if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST) != RESET) return RST_WATCHDOG;
  if (__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST) != RESET)  return RST_SOFTWARE;
  if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST) != RESET)  return RST_EXTERNAL;
  if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST) != RESET)  return RST_POWER_ON;
  return 0;
}

void _delay_ms(const int delay_ms) { delay(delay_ms); }

extern "C" {
  extern unsigned int _ebss; // end of bss section
}

// --------------------------------------------------------------------------
// ADC
// --------------------------------------------------------------------------

void HAL_adc_start_conversion(const uint8_t adc_pin) {
  HAL_adc_result = analogRead(adc_pin);
}

uint16_t HAL_adc_get_result(void) {
  return HAL_adc_result;
}

#endif // ARDUINO_ARCH_STM32
