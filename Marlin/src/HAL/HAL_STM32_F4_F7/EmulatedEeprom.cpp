/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * Description: Functions for a Flash emulated EEPROM
 * Not platform dependent.
 */

// Include configs and pins to get all EEPROM flags
#include "../../inc/MarlinConfig.h"

#ifdef STM32F7
  #define HAS_EMULATED_EEPROM 1
#else
  #define HAS_EMULATED_EEPROM NONE(I2C_EEPROM, SPI_EEPROM)
#endif

#if HAS_EMULATED_EEPROM && ENABLED(EEPROM_SETTINGS)

// ------------------------
// Includes
// ------------------------

#include "HAL.h"
#include "eeprom_emul.h"

// ------------------------
// Local defines
// ------------------------

// FLASH_FLAG_PGSERR (Programming Sequence Error) was renamed to
// FLASH_FLAG_ERSERR (Erasing Sequence Error) in STM32F4/7

#ifdef STM32F7
  #define FLASH_FLAG_PGSERR FLASH_FLAG_ERSERR
#else
  //#define FLASH_FLAG_PGSERR FLASH_FLAG_ERSERR
#endif

// ------------------------
// Private Variables
// ------------------------

static bool eeprom_initialized = false;

// ------------------------
// Public functions
// ------------------------

void eeprom_init() {
  if (!eeprom_initialized) {
    HAL_FLASH_Unlock();

    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);

    /* EEPROM Init */
    if (EE_Initialize() != EE_OK)
      for (;;) HAL_Delay(1); // Spin forever until watchdog reset

    HAL_FLASH_Lock();
    eeprom_initialized = true;
  }
}

void eeprom_write_byte(uint8_t *pos, unsigned char value) {
  eeprom_init();

  HAL_FLASH_Unlock();
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);

  uint16_t eeprom_address = unsigned(pos);
  if (EE_WriteVariable(eeprom_address, uint16_t(value)) != EE_OK)
    for (;;) HAL_Delay(1); // Spin forever until watchdog reset

  HAL_FLASH_Lock();
}

uint8_t eeprom_read_byte(uint8_t *pos) {
  eeprom_init();

  uint16_t data = 0xFF;
  uint16_t eeprom_address = unsigned(pos);
  (void)EE_ReadVariable(eeprom_address, &data); // Data unchanged on error

  return uint8_t(data);
}

void eeprom_read_block(void *__dst, const void *__src, size_t __n) {
  eeprom_init();

  uint16_t data = 0xFF;
  uint16_t eeprom_address = unsigned(__src);
  for (uint8_t c = 0; c < __n; c++) {
    EE_ReadVariable(eeprom_address+c, &data);
    *((uint8_t*)__dst + c) = data;
  }
}

void eeprom_update_block(const void *__src, void *__dst, size_t __n) {

}

#endif // EEPROM_SETTINGS
#endif // STM32GENERIC && (STM32F4 || STM32F7)
