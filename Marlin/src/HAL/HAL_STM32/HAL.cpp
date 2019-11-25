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

#if defined(ARDUINO_ARCH_STM32) && !defined(STM32GENERIC)

#include "HAL.h"

#include "../../inc/MarlinConfig.h"
#include "../shared/Delay.h"

#if (__cplusplus == 201703L) && defined(__has_include)
  #define HAS_SWSERIAL __has_include(<SoftwareSerial.h>)
#else
  #define HAS_SWSERIAL HAS_TMC220x
#endif

#if HAS_SWSERIAL
  #include "SoftwareSerial.h"
#endif

#if ENABLED(SRAM_EEPROM_EMULATION)
  #if STM32F7xx
    #include <stm32f7xx_ll_pwr.h>
  #elif STM32F4xx
    #include <stm32f4xx_ll_pwr.h>
  #else
    #error "SRAM_EEPROM_EMULATION is currently only supported for STM32F4xx and STM32F7xx"
  #endif
#endif

// ------------------------
// Public Variables
// ------------------------

uint16_t HAL_adc_result;

// ------------------------
// Public functions
// ------------------------

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
void HAL_init() {
  FastIO_init();

  #if ENABLED(SDSUPPORT)
    //this is done temporarily (to avoid that some device on the bus tries to become the master)
    //when the spi device is initialized it's done again.
    for (uint8_t dev = 0; dev < NUM_SPI_DEVICES; dev++) {
      OUT_WRITE(CS_OF_DEV(dev), HIGH); //Set ChipSelect PIN high (inactive) before any other SPI users start up
      if (IS_DEV_SD(dev) && SW_OF_SD(dev) != NC) //if it's an SD card and has a real switch set the pin as input
          _SET_MODE(SW_OF_SD(dev), DLV_OF_SD(dev) == LOW ? INPUT_PULLUP : INPUT_PULLDOWN); //with the appropriate pull
    }
  #endif

  #if PIN_EXISTS(LED)
    OUT_WRITE(LED_PIN, LOW);
  #endif

  #if ENABLED(SRAM_EEPROM_EMULATION)
  // Enable access to backup SRAM
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_BKPSRAM_CLK_ENABLE();

  // Enable backup regulator
  LL_PWR_EnableBkUpRegulator();
  // Wait until backup regulator is initialized
  while (!LL_PWR_IsActiveFlag_BRR());
  #endif // EEPROM_EMULATED_SRAM

  #if HAS_SWSERIAL
    SoftwareSerial::setInterruptPriority(SWSERIAL_TIMER_IRQ_PRIO, 0);
  #endif
}

void HAL_clear_reset_source() { __HAL_RCC_CLEAR_RESET_FLAGS(); }

uint8_t HAL_get_reset_source() {
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

// ------------------------
// ADC
// ------------------------

// TODO: Make sure this doesn't cause any delay
void HAL_adc_start_conversion(const uint8_t adc_pin) { HAL_adc_result = analogRead(adc_pin); }

uint16_t HAL_adc_get_result() { return HAL_adc_result; }

void flashFirmware(int16_t) { NVIC_SystemReset(); }

#endif // ARDUINO_ARCH_STM32 && !STM32GENERIC
