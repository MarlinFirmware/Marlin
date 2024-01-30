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

/**
 * Assign custom or standard U8G device drivers
 */

#include <U8glib-HAL.h>

#include HAL_PATH(../.., u8g/LCD_defines.h)

#if HAS_FSMC_GRAPHICAL_TFT || HAS_SPI_GRAPHICAL_TFT || HAS_LTDC_GRAPHICAL_TFT
  uint8_t u8g_com_hal_tft_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);
  #define U8G_COM_HAL_TFT_FN u8g_com_hal_tft_fn
#endif

// U8G_HAL_LINKS is defined for LPC1768/9 and Native envs by -DU8G_HAL_LINKS in platform.ini
#ifndef U8G_COM_HAL_SW_SPI_FN
  #define U8G_COM_HAL_SW_SPI_FN     TERN(U8G_HAL_LINKS, u8g_com_null_fn, u8g_com_arduino_std_sw_spi_fn)
#endif
#ifndef U8G_COM_HAL_HW_SPI_FN
  #define U8G_COM_HAL_HW_SPI_FN     TERN(U8G_HAL_LINKS, u8g_com_null_fn, u8g_com_arduino_hw_spi_fn)
#endif
#ifndef U8G_COM_ST7920_HAL_SW_SPI
  #define U8G_COM_ST7920_HAL_SW_SPI TERN(U8G_HAL_LINKS, u8g_com_null_fn, u8g_com_arduino_st7920_spi_fn)
#endif
#ifndef U8G_COM_ST7920_HAL_HW_SPI
  #define U8G_COM_ST7920_HAL_HW_SPI TERN(U8G_HAL_LINKS, u8g_com_null_fn, u8g_com_arduino_st7920_hw_spi_fn)
#endif
#ifndef U8G_COM_SSD_I2C_HAL
  #define U8G_COM_SSD_I2C_HAL       TERN(U8G_HAL_LINKS, u8g_com_null_fn, u8g_com_arduino_ssd_i2c_fn)
#endif
#ifndef U8G_COM_HAL_TFT_FN
  #define U8G_COM_HAL_TFT_FN        u8g_com_null_fn
#endif
