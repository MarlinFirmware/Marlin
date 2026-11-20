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

/**
 * Based on u8g_com_st7920_hw_spi.c
 *
 * Universal 8bit Graphics Library
 *
 * Copyright (c) 2011, olikraus@gmail.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice, this list
 *    of conditions and the following disclaimer.
 *
 *  * Redistributions in binary form must reproduce the above copyright notice, this
 *    list of conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef TARGET_LPC5528

#include "../../../inc/MarlinConfigPre.h"

#if IS_U8GLIB_ST7920

#include <U8glib-HAL.h>
#include "../../shared/Delay.h"
#include "../../shared/HAL_SPI.h"

#include <Arduino.h>
#include <algorithm>
#include <LPC55S28.h>
#include <gpio.h>

#ifndef LCD_SPI_SPEED
  #define LCD_SPI_SPEED SPI_EIGHTH_SPEED  // About 1 MHz
#endif

static pin_t SCK_pin_ST7920_HAL, MOSI_pin_ST7920_HAL_HAL;
static uint8_t SPI_speed = 0;

static uint8_t swSpiTransfer_mode_0(uint8_t b, const uint8_t spi_speed, const pin_t sck_pin, const pin_t miso_pin, const pin_t mosi_pin ) {

  for (uint8_t i = 0; i < 8; ++i) {
    if (spi_speed == 0) {
      LPC5528::gpio_set(mosi_pin, !!(b & 0x80));
      LPC5528::gpio_set(sck_pin, HIGH);
      b <<= 1;
      if (miso_pin >= 0 && LPC5528::gpio_get(miso_pin)) b |= 1;
      LPC5528::gpio_set(sck_pin, LOW);
    }
    else {
      const uint8_t state = (b & 0x80) ? HIGH : LOW;
      for (uint8_t j = 0; j < spi_speed; ++j)
        LPC5528::gpio_set(mosi_pin, state);

      for (uint8_t j = 0; j < spi_speed + (miso_pin >= 0 ? 0 : 1); ++j)
        LPC5528::gpio_set(sck_pin, HIGH);

      b <<= 1;
      if (miso_pin >= 0 && LPC5528::gpio_get(miso_pin)) b |= 1;

      for (uint8_t j = 0; j < spi_speed; ++j)
        LPC5528::gpio_set(sck_pin, LOW);
    }
  }

  return b;
}

static uint8_t swSpiTransfer_mode_3(uint8_t b, const uint8_t spi_speed, const pin_t sck_pin, const pin_t miso_pin, const pin_t mosi_pin ) {

  for (uint8_t i = 0; i < 8; ++i) {
    const uint8_t state = (b & 0x80) ? HIGH : LOW;
    if (spi_speed == 0) {
      LPC5528::gpio_set(sck_pin, LOW);
      LPC5528::gpio_set(mosi_pin, state);
      LPC5528::gpio_set(mosi_pin, state);  // need some setup time
      LPC5528::gpio_set(sck_pin, HIGH);
    }
    else {
      for (uint8_t j = 0; j < spi_speed + (miso_pin >= 0 ? 0 : 1); ++j)
        LPC5528::gpio_set(sck_pin, LOW);

      for (uint8_t j = 0; j < spi_speed; ++j)
        LPC5528::gpio_set(mosi_pin, state);

      for (uint8_t j = 0; j < spi_speed; ++j)
        LPC5528::gpio_set(sck_pin, HIGH);
    }
    b <<= 1;
    if (miso_pin >= 0 && LPC5528::gpio_get(miso_pin)) b |= 1;
  }

  return b;
}

static inline uint8_t swSpiTransfer(uint8_t b, const uint8_t spi_speed, const pin_t sck_pin, const pin_t miso_pin, const pin_t mosi_pin) {
  #if U8G_SPI_USE_MODE_3
    return swSpiTransfer_mode_3(b, spi_speed, sck_pin, miso_pin, mosi_pin);
  #else
    return swSpiTransfer_mode_0(b, spi_speed, sck_pin, miso_pin, mosi_pin);
  #endif
}

static uint8_t swSpiInit(const uint8_t spi_speed, const pin_t sck_pin, const pin_t mosi_pin) {
  UNUSED(sck_pin);
  UNUSED(mosi_pin);
  return spi_speed;
}

static void u8g_com_LPC5528_st7920_write_byte_sw_spi(uint8_t rs, uint8_t val) {
  static uint8_t rs_last_state = 255;
  if (rs != rs_last_state) {
    // Transfer Data (FA) or Command (F8)
    swSpiTransfer(rs ? 0x0FA : 0x0F8, SPI_speed, SCK_pin_ST7920_HAL, -1, MOSI_pin_ST7920_HAL_HAL);
    rs_last_state = rs;
    DELAY_US(40); // Give the controller time to process the data: 20 is bad, 30 is OK, 40 is safe
  }
  swSpiTransfer(val & 0x0F0, SPI_speed, SCK_pin_ST7920_HAL, -1, MOSI_pin_ST7920_HAL_HAL);
  swSpiTransfer(val << 4, SPI_speed, SCK_pin_ST7920_HAL, -1, MOSI_pin_ST7920_HAL_HAL);
}

uint8_t u8g_com_HAL_LPC5528_ST7920_sw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr) {
  switch (msg) {
    case U8G_COM_MSG_INIT:
      SCK_pin_ST7920_HAL = u8g->pin_list[U8G_PI_SCK];
      MOSI_pin_ST7920_HAL_HAL = u8g->pin_list[U8G_PI_MOSI];

      u8g_SetPIOutput(u8g, U8G_PI_CS);
      u8g_SetPIOutput(u8g, U8G_PI_SCK);
      u8g_SetPIOutput(u8g, U8G_PI_MOSI);
      u8g_Delay(5);

      SPI_speed = swSpiInit(LCD_SPI_SPEED, SCK_pin_ST7920_HAL, MOSI_pin_ST7920_HAL_HAL);

      u8g_SetPILevel(u8g, U8G_PI_CS, 0);
      u8g_SetPILevel(u8g, U8G_PI_SCK, 0);
      u8g_SetPILevel(u8g, U8G_PI_MOSI, 0);

      u8g->pin_list[U8G_PI_A0_STATE] = 0;       /* initial RS state: command mode */
      break;

    case U8G_COM_MSG_STOP:
      break;

    case U8G_COM_MSG_RESET:
       if (U8G_PIN_NONE != u8g->pin_list[U8G_PI_RESET]) u8g_SetPILevel(u8g, U8G_PI_RESET, arg_val);
      break;

    case U8G_COM_MSG_ADDRESS:                     /* define cmd (arg_val = 0) or data mode (arg_val = 1) */
      u8g->pin_list[U8G_PI_A0_STATE] = arg_val;
      break;

    case U8G_COM_MSG_CHIP_SELECT:
      if (U8G_PIN_NONE != u8g->pin_list[U8G_PI_CS]) u8g_SetPILevel(u8g, U8G_PI_CS, arg_val);  //note: the st7920 has an active high chip select
      break;

    case U8G_COM_MSG_WRITE_BYTE:
      u8g_com_LPC5528_st7920_write_byte_sw_spi(u8g->pin_list[U8G_PI_A0_STATE], arg_val);
      break;

    case U8G_COM_MSG_WRITE_SEQ: {
        uint8_t *ptr = (uint8_t*) arg_ptr;
        while (arg_val > 0) {
          u8g_com_LPC5528_st7920_write_byte_sw_spi(u8g->pin_list[U8G_PI_A0_STATE], *ptr++);
          arg_val--;
        }
      }
      break;

      case U8G_COM_MSG_WRITE_SEQ_P: {
        uint8_t *ptr = (uint8_t*) arg_ptr;
        while (arg_val > 0) {
          u8g_com_LPC5528_st7920_write_byte_sw_spi(u8g->pin_list[U8G_PI_A0_STATE], *ptr++);
          arg_val--;
        }
      }
      break;
  }
  return 1;
}

#endif // IS_U8GLIB_ST7920
#endif // TARGET_LPC5528
