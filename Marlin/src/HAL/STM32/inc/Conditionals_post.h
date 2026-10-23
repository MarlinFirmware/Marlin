/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#pragma once

// If no real or emulated EEPROM selected, fall back to SD emulation
#if USE_FALLBACK_EEPROM
  #define SDCARD_EEPROM_EMULATION
#elif ANY(I2C_EEPROM, SPI_EEPROM)
  #define USE_SHARED_EEPROM 1
#endif

// Some STM32F4 boards may lose steps when saving to EEPROM during print (PR #17946)
#if ALL(STM32F4xx, FLASH_EEPROM_EMULATION) && PRINTCOUNTER_SAVE_INTERVAL > 0
  #define PRINTCOUNTER_SYNC
#endif

/**
 * A DOGM display on its own hardware SPI bus
 *
 * The u8g hardware-SPI driver uses the shared 'SPI' object, which the SD card
 * repoints to its own pins, so a display on other pins has to bit-bang instead.
 * Where a board declares its display header's SPI pins and the configured
 * DOGLCD pins match them, the display can be given a dedicated SPI instance
 * and keep hardware SPI.
 *
 * There is no portable way to ask whether a pin is SPI-capable - the pin map is
 * a runtime table - so this relies on the board saying so. Boards opt in by
 * defining LCD_HW_SPI_SCK_PIN / LCD_HW_SPI_MOSI_PIN / LCD_HW_SPI_MISO_PIN. e.g., MKS Robin Nano V3
 */
#if defined(DOGLCD_SCK) && defined(DOGLCD_MOSI)
  #if defined(LCD_HW_SPI_SCK_PIN) && defined(LCD_HW_SPI_MOSI_PIN) && defined(LCD_HW_SPI_MISO_PIN) \
      && DOGLCD_SCK == LCD_HW_SPI_SCK_PIN && DOGLCD_MOSI == LCD_HW_SPI_MOSI_PIN
    #define HAS_DOGLCD_HW_SPI 1
    #define DOGLCD_MISO LCD_HW_SPI_MISO_PIN
  #elif defined(PIN_SPI_SCK) && defined(PIN_SPI_MOSI) && defined(PIN_SPI_MISO) \
      && DOGLCD_SCK == PIN_SPI_SCK && DOGLCD_MOSI == PIN_SPI_MOSI
    #define HAS_DOGLCD_HW_SPI 1
    #define DOGLCD_MISO PIN_SPI_MISO
  #endif
#endif // DOGLCD_SCK && DOGLCD_MOSI
