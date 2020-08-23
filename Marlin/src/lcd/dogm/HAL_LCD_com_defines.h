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

// Use this file to select the com driver for device drivers that are NOT in the U8G library

#include <U8glib.h>

#ifndef U8G_HAL_LINKS   // Defined by LPC1768/9 environments in platform.ini

  #ifdef __SAM3X8E__

    uint8_t u8g_com_HAL_DUE_sw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);
    uint8_t u8g_com_HAL_DUE_shared_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);
    uint8_t u8g_com_HAL_DUE_ST7920_sw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);
    #define U8G_COM_HAL_SW_SPI_FN     u8g_com_HAL_DUE_sw_spi_fn
    #define U8G_COM_HAL_HW_SPI_FN     u8g_com_HAL_DUE_shared_hw_spi_fn
    #define U8G_COM_ST7920_HAL_SW_SPI u8g_com_HAL_DUE_ST7920_sw_spi_fn

  #elif defined(__SAMD51__)

    #define U8G_COM_HAL_HW_SPI_FN     u8g_com_samd51_hw_spi_fn
    #define U8G_COM_ST7920_HAL_HW_SPI u8g_com_samd51_st7920_hw_spi_fn

  #elif defined(__STM32F1__)

    uint8_t u8g_com_HAL_STM32F1_sw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);
    uint8_t u8g_com_stm32duino_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);
    #define U8G_COM_HAL_SW_SPI_FN     u8g_com_HAL_STM32F1_sw_spi_fn
    #define U8G_COM_HAL_HW_SPI_FN     u8g_com_stm32duino_hw_spi_fn
    #define U8G_COM_ST7920_HAL_SW_SPI u8g_com_std_sw_spi_fn
    #define U8G_COM_ST7920_HAL_HW_SPI u8g_com_stm32duino_hw_spi_fn

  #elif defined(ARDUINO_ARCH_STM32)

    uint8_t u8g_com_stm32duino_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);
    #define U8G_COM_HAL_HW_SPI_FN     u8g_com_stm32duino_hw_spi_fn

  #elif defined(__AVR__)

    uint8_t u8g_com_HAL_AVR_sw_sp_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);
    #define U8G_COM_HAL_SW_SPI_FN     u8g_com_HAL_AVR_sw_sp_fn

  #endif

  #ifndef U8G_COM_HAL_SW_SPI_FN
    #define U8G_COM_HAL_SW_SPI_FN     u8g_com_arduino_std_sw_spi_fn
  #endif
  #ifndef U8G_COM_HAL_HW_SPI_FN
    #define U8G_COM_HAL_HW_SPI_FN     u8g_com_arduino_hw_spi_fn
  #endif
  #ifndef U8G_COM_ST7920_HAL_SW_SPI
    #define U8G_COM_ST7920_HAL_SW_SPI u8g_com_arduino_st7920_spi_fn
  #endif
  #ifndef U8G_COM_ST7920_HAL_HW_SPI
    #define U8G_COM_ST7920_HAL_HW_SPI u8g_com_arduino_st7920_hw_spi_fn
  #endif

  // This can't be invoked from the current platformio.ini
  #ifdef TARGET_LPC1768
    uint8_t u8g_com_HAL_LPC1768_ssd_hw_i2c_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);
  #endif

  #define U8G_COM_SSD_I2C_HAL         u8g_com_arduino_ssd_i2c_fn

  #if EITHER(FSMC_GRAPHICAL_TFT, SPI_GRAPHICAL_TFT)
    uint8_t u8g_com_stm32duino_tft_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);
    #define U8G_COM_HAL_TFT_FN       u8g_com_stm32duino_tft_fn
  #endif

#elif defined(TARGET_LPC1768)

  uint8_t u8g_com_HAL_LPC1768_sw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);
  uint8_t u8g_com_HAL_LPC1768_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);
  uint8_t u8g_com_HAL_LPC1768_ST7920_sw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);
  uint8_t u8g_com_HAL_LPC1768_ST7920_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);
  uint8_t u8g_com_HAL_LPC1768_ssd_hw_i2c_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);
  #define U8G_COM_HAL_SW_SPI_FN       u8g_com_HAL_LPC1768_sw_spi_fn
  #define U8G_COM_HAL_HW_SPI_FN       u8g_com_HAL_LPC1768_hw_spi_fn
  #define U8G_COM_ST7920_HAL_SW_SPI   u8g_com_HAL_LPC1768_ST7920_sw_spi_fn
  #define U8G_COM_ST7920_HAL_HW_SPI   u8g_com_HAL_LPC1768_ST7920_hw_spi_fn
  #define U8G_COM_SSD_I2C_HAL         u8g_com_HAL_LPC1768_ssd_hw_i2c_fn

#endif

#ifndef U8G_COM_HAL_SW_SPI_FN
  #define U8G_COM_HAL_SW_SPI_FN     u8g_com_null_fn
#endif
#ifndef U8G_COM_HAL_HW_SPI_FN
  #define U8G_COM_HAL_HW_SPI_FN     u8g_com_null_fn
#endif
#ifndef U8G_COM_ST7920_HAL_SW_SPI
  #define U8G_COM_ST7920_HAL_SW_SPI u8g_com_null_fn
#endif
#ifndef U8G_COM_ST7920_HAL_HW_SPI
  #define U8G_COM_ST7920_HAL_HW_SPI u8g_com_null_fn
#endif
#ifndef U8G_COM_SSD_I2C_HAL
  #define U8G_COM_SSD_I2C_HAL       u8g_com_null_fn
#endif
#ifndef U8G_COM_HAL_TFT_FN
  #define U8G_COM_HAL_TFT_FN       u8g_com_null_fn
#endif
