/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#if defined(STM32GENERIC) && (defined(STM32F4) || defined(STM32F4xx))

/**
 * Description: functions for I2C connected external EEPROM.
 * Not platform dependent.
 */

#include "../../inc/MarlinConfig.h"

#if ENABLED(EEPROM_SETTINGS) && DISABLED(I2C_EEPROM) && DISABLED(SPI_EEPROM)

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------

#include "HAL.h"
#include "EEPROM_Emul/eeprom_emul.h"


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

// --------------------------------------------------------------------------
// Private Variables
// --------------------------------------------------------------------------
static bool eeprom_initialized = false;
// --------------------------------------------------------------------------
// Function prototypes
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Private functions
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Public functions
// --------------------------------------------------------------------------

// FLASH_FLAG_PGSERR (Programming Sequence Error) was renamed to
// FLASH_FLAG_ERSERR (Erasing Sequence Error) in STM32F4
// #define FLASH_FLAG_PGSERR FLASH_FLAG_ERSERR

// --------------------------------------------------------------------------
// EEPROM
// --------------------------------------------------------------------------


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
  uint16_t eeprom_address = (unsigned) pos;

  eeprom_init();

  HAL_FLASH_Unlock();
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);

  if (EE_WriteVariable(eeprom_address, (uint16_t) value) != EE_OK)
      for (;;) HAL_Delay(1); // Spin forever until watchdog reset

  HAL_FLASH_Lock();
}

uint8_t eeprom_read_byte(uint8_t *pos) {
  uint16_t data = 0xFF;
  uint16_t eeprom_address = (unsigned)pos;

  eeprom_init();

  if (EE_ReadVariable(eeprom_address, &data) != EE_OK) {
    return (unsigned char)data;
  }
  return (unsigned char)data;
}

void eeprom_read_block(void *__dst, const void *__src, size_t __n) {
  uint16_t data = 0xFF;
  uint16_t eeprom_address = (unsigned) __src;

  eeprom_init();

  for (uint8_t c = 0; c < __n; c++) {
    EE_ReadVariable(eeprom_address+c, &data);
    *((uint8_t*)__dst + c) = data;
  }
}

void eeprom_update_block(const void *__src, void *__dst, size_t __n) {

}

#endif // ENABLED(EEPROM_SETTINGS) && DISABLED(I2C_EEPROM) && DISABLED(SPI_EEPROM)
#endif // STM32F4 || STM32F4xx

