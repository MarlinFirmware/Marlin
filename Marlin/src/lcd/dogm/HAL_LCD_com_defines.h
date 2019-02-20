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

// Use this file to select the com driver for device drivers that are NOT in the U8G library

#ifndef U8G_HAL_LINKS

  uint8_t u8g_com_arduino_sw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);
  #define U8G_COM_HAL_SW_SPI_FN  u8g_com_arduino_sw_spi_fn

  #ifdef __SAM3X8E__
    uint8_t u8g_com_HAL_DUE_shared_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);
    #define U8G_COM_HAL_HW_SPI_FN u8g_com_HAL_DUE_shared_hw_spi_fn

    uint8_t u8g_com_HAL_DUE_ST7920_sw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);
    #define U8G_COM_ST7920_HAL_SW_SPI u8g_com_HAL_DUE_ST7920_sw_spi_fn
  #else
    uint8_t u8g_com_arduino_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);
    #define U8G_COM_HAL_HW_SPI_FN u8g_com_arduino_hw_spi_fn

    uint8_t u8g_com_arduino_st7920_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);
    #define U8G_COM_ST7920_HAL_SW_SPI u8g_com_arduino_st7920_spi_fn
  #endif

  uint8_t u8g_com_arduino_st7920_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);
  #define U8G_COM_ST7920_HAL_HW_SPI u8g_com_arduino_st7920_hw_spi_fn

  uint8_t u8g_com_HAL_LPC1768_ssd_hw_i2c_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);
  #define U8G_COM_SSD_I2C_HAL u8g_com_arduino_ssd_i2c_fn

  #if defined(STM32F1) || defined(STM32F1xx)
    uint8_t u8g_com_stm32duino_fsmc_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);
    #define U8G_COM_HAL_FSMC_FN u8g_com_stm32duino_fsmc_fn
  #else
    #define U8G_COM_HAL_FSMC_FN u8g_com_null_fn
  #endif
#elif TARGET_LPC1768
  uint8_t u8g_com_HAL_LPC1768_sw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);
  #define U8G_COM_HAL_SW_SPI_FN u8g_com_HAL_LPC1768_sw_spi_fn

  uint8_t u8g_com_HAL_LPC1768_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);
  #define U8G_COM_HAL_HW_SPI_FN u8g_com_HAL_LPC1768_hw_spi_fn

  uint8_t u8g_com_HAL_LPC1768_ST7920_sw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);
  #define U8G_COM_ST7920_HAL_SW_SPI u8g_com_HAL_LPC1768_ST7920_sw_spi_fn

  uint8_t u8g_com_HAL_LPC1768_ST7920_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);
  #define U8G_COM_ST7920_HAL_HW_SPI u8g_com_HAL_LPC1768_ST7920_hw_spi_fn

  uint8_t u8g_com_HAL_LPC1768_ssd_hw_i2c_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);
  #define U8G_COM_SSD_I2C_HAL u8g_com_HAL_LPC1768_ssd_hw_i2c_fn

  #define U8G_COM_HAL_FSMC_FN u8g_com_null_fn

#else  // need to give them some definition or else get compiler errors
  uint8_t u8g_com_null_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);
  #define U8G_COM_HAL_SW_SPI_FN u8g_com_null_fn
  #define U8G_COM_HAL_HW_SPI_FN u8g_com_null_fn
  #define U8G_COM_ST7920_HAL_SW_SPI u8g_com_null_fn
  #define U8G_COM_ST7920_HAL_HW_SPI u8g_com_null_fn
  #define U8G_COM_SSD_I2C_HAL u8g_com_null_fn
  #define U8G_COM_HAL_FSMC_FN u8g_com_null_fn
#endif
