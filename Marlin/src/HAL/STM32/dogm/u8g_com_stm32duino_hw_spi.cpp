/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * Hardware SPI for a DOGM display on its own SPI bus.
 *
 * U8glib-HAL's u8g_com_stm32duino_hw_spi_fn drives the shared 'SPI' object.
 * Marlin's SD card code repoints that object at the card (HAL_SPI.cpp), so a
 * display on different pins gets its output sent to the SD card's pins as soon
 * as the card is initialized - the display draws its boot screen and then goes
 * quiet. That is why marlinui_DOGM.h falls back to bit-banging whenever the
 * DOGLCD pins differ from the SD pins.
 *
 * Where the board declares its display header's SPI pins (LCD_SPI_*), the
 * display can instead have a MarlinSPI instance of its own, on its own
 * peripheral, and keep hardware SPI. The same approach is used for the onboard
 * SPI flash (libs/W25Qxx.cpp).
 */

#ifdef HAL_STM32

#include "../../../inc/MarlinConfig.h"

#if HAS_DOGLCD_HW_SPI

#include <U8glib-HAL.h>
#include <SPI.h>

#ifndef DOGLCD_SPI_SPEED
  #define DOGLCD_SPI_SPEED 2500000    // (Hz) Same base clock as the U8glib-HAL driver
#endif

// MISO is unused by the display, but the STM32 core refuses to bring up an SPI
// peripheral whose MISO pin has no peripheral mapping (spi_com.c), so it has to
// be a real pin on the same peripheral. Boards supply it as LCD_HW_SPI_MISO_PIN.
static SPIClass lcdSPI(DOGLCD_MOSI, DOGLCD_MISO, DOGLCD_SCK);
static SPISettings spiConfig;

uint8_t u8g_com_HAL_STM32_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr) {
  switch (msg) {
    case U8G_COM_MSG_STOP:
      break;

    case U8G_COM_MSG_INIT:
      u8g_SetPIOutput(u8g, U8G_PI_CS);
      u8g_SetPIOutput(u8g, U8G_PI_A0);
      u8g_SetPIOutput(u8g, U8G_PI_RESET);
      u8g_SetPILevel(u8g, U8G_PI_CS, 1);

      spiConfig = SPISettings(DOGLCD_SPI_SPEED, MSBFIRST, TERN(U8G_SPI_USE_MODE_3, SPI_MODE3, SPI_MODE0));
      lcdSPI.begin();
      break;

    case U8G_COM_MSG_ADDRESS:                 // Command (arg_val = 0) or data (arg_val = 1)
      u8g_SetPILevel(u8g, U8G_PI_A0, arg_val);
      break;

    case U8G_COM_MSG_CHIP_SELECT:             // arg_val == 0 means CS high
      u8g_SetPILevel(u8g, U8G_PI_CS, arg_val ? LOW : HIGH);
      break;

    case U8G_COM_MSG_RESET:
      u8g_SetPILevel(u8g, U8G_PI_RESET, arg_val);
      break;

    case U8G_COM_MSG_WRITE_BYTE:
      lcdSPI.beginTransaction(spiConfig);
      lcdSPI.transfer(arg_val);
      lcdSPI.endTransaction();
      break;

    case U8G_COM_MSG_WRITE_SEQ:
      lcdSPI.beginTransaction(spiConfig);
      lcdSPI.transfer((uint8_t *)arg_ptr, arg_val);
      lcdSPI.endTransaction();
      break;
  }
  return 1;
}

#endif // HAS_DOGLCD_HW_SPI
#endif // HAL_STM32
