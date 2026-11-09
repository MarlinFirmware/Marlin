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
 * U8glib hardware SPI for the simulator. Bytes go through the HAL SPI functions to the
 * simulated SPI bus, where the simulator's display models receive them with CS and A0.
 */

#ifdef __PLAT_NATIVE_SIM__

#include "../../../inc/MarlinConfigPre.h"

#if HAS_MARLINUI_U8GLIB && !HAS_MARLINUI_U8G2

#include <U8glib-HAL.h>
#include "../../shared/HAL_SPI.h"

#ifndef LCD_SPI_SPEED
  #ifdef SD_SPI_SPEED
    #define LCD_SPI_SPEED SD_SPI_SPEED    // Assume SPI speed shared with SD
  #else
    #define LCD_SPI_SPEED SPI_FULL_SPEED  // Use full speed if SD speed is not supplied
  #endif
#endif

uint8_t u8g_com_HAL_NATIVE_SIM_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr) {
  switch (msg) {
    case U8G_COM_MSG_STOP:
      break;

    case U8G_COM_MSG_INIT:
      u8g_SetPILevel(u8g, U8G_PI_CS, 1);
      u8g_SetPILevel(u8g, U8G_PI_A0, 1);
      u8g_SetPILevel(u8g, U8G_PI_RESET, 1);
      u8g_SetPIOutput(u8g, U8G_PI_CS);
      u8g_SetPIOutput(u8g, U8G_PI_A0);
      u8g_SetPIOutput(u8g, U8G_PI_RESET);
      u8g_Delay(5);
      spiBegin();
      spiInit(LCD_SPI_SPEED);
      break;

    case U8G_COM_MSG_ADDRESS:                     // Command (arg_val = 0) or data mode (arg_val = 1)
      u8g_SetPILevel(u8g, U8G_PI_A0, arg_val);
      break;

    case U8G_COM_MSG_CHIP_SELECT:
      u8g_SetPILevel(u8g, U8G_PI_CS, (arg_val ? 0 : 1));
      break;

    case U8G_COM_MSG_RESET:
      u8g_SetPILevel(u8g, U8G_PI_RESET, arg_val);
      break;

    case U8G_COM_MSG_WRITE_BYTE:
      spiSend((uint8_t)arg_val);
      break;

    case U8G_COM_MSG_WRITE_SEQ:
    case U8G_COM_MSG_WRITE_SEQ_P: {
      uint8_t *ptr = (uint8_t*)arg_ptr;
      while (arg_val > 0) {
        spiSend(*ptr++);
        arg_val--;
      }
    } break;
  }
  return 1;
}

#endif // HAS_MARLINUI_U8GLIB && !HAS_MARLINUI_U8G2
#endif // __PLAT_NATIVE_SIM__
