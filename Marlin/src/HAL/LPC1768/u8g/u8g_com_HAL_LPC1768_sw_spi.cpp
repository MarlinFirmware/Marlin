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
 * Based on u8g_com_std_sw_spi.c
 *
 * Universal 8bit Graphics Library
 *
 * Copyright (c) 2015, olikraus@gmail.com
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

#ifdef TARGET_LPC1768

#include "../../../inc/MarlinConfigPre.h"

#if HAS_GRAPHICAL_LCD && DISABLED(U8GLIB_ST7920)

#include <SoftwareSPI.h>

#undef SPI_SPEED
#define SPI_SPEED 2  // About 2 MHz

#include <Arduino.h>
#include <algorithm>
#include <LPC17xx.h>
#include <gpio.h>

#include <U8glib.h>

uint8_t swSpiTransfer_mode_0(uint8_t b, const uint8_t spi_speed, const pin_t sck_pin, const pin_t miso_pin, const pin_t mosi_pin ) {

  LOOP_L_N(i, 8) {
    if (spi_speed == 0) {
      LPC176x::gpio_set(mosi_pin, !!(b & 0x80));
      LPC176x::gpio_set(sck_pin, HIGH);
      b <<= 1;
      if (miso_pin >= 0 && LPC176x::gpio_get(miso_pin)) b |= 1;
      LPC176x::gpio_set(sck_pin, LOW);
    }
    else {
      const uint8_t state = (b & 0x80) ? HIGH : LOW;
      LOOP_L_N(j, spi_speed)
        LPC176x::gpio_set(mosi_pin, state);

      LOOP_L_N(j, spi_speed + (miso_pin >= 0 ? 0 : 1))
        LPC176x::gpio_set(sck_pin, HIGH);

      b <<= 1;
      if (miso_pin >= 0 && LPC176x::gpio_get(miso_pin)) b |= 1;

      LOOP_L_N(j, spi_speed)
        LPC176x::gpio_set(sck_pin, LOW);
    }
  }

  return b;
}

uint8_t swSpiTransfer_mode_3(uint8_t b, const uint8_t spi_speed, const pin_t sck_pin, const pin_t miso_pin, const pin_t mosi_pin ) {

  LOOP_L_N(i, 8) {
    const uint8_t state = (b & 0x80) ? HIGH : LOW;
    if (spi_speed == 0) {
      LPC176x::gpio_set(sck_pin, LOW);
      LPC176x::gpio_set(mosi_pin, state);
      LPC176x::gpio_set(mosi_pin, state);  // need some setup time
      LPC176x::gpio_set(sck_pin, HIGH);
    }
    else {
      LOOP_L_N(j, spi_speed + (miso_pin >= 0 ? 0 : 1))
        LPC176x::gpio_set(sck_pin, LOW);

      LOOP_L_N(j, spi_speed)
        LPC176x::gpio_set(mosi_pin, state);

      LOOP_L_N(j, spi_speed)
        LPC176x::gpio_set(sck_pin, HIGH);
    }
    b <<= 1;
    if (miso_pin >= 0 && LPC176x::gpio_get(miso_pin)) b |= 1;
  }

  return b;
}

static uint8_t SPI_speed = 0;

static void u8g_sw_spi_HAL_LPC1768_shift_out(uint8_t dataPin, uint8_t clockPin, uint8_t val) {
  #if EITHER(FYSETC_MINI_12864, MKS_MINI_12864)
    swSpiTransfer_mode_3(val, SPI_speed, clockPin, -1, dataPin);
  #else
    swSpiTransfer_mode_0(val, SPI_speed, clockPin, -1, dataPin);
  #endif
}

uint8_t u8g_com_HAL_LPC1768_sw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr) {
  switch (msg) {
    case U8G_COM_MSG_INIT:
      u8g_SetPIOutput(u8g, U8G_PI_SCK);
      u8g_SetPIOutput(u8g, U8G_PI_MOSI);
      u8g_SetPIOutput(u8g, U8G_PI_CS);
      u8g_SetPIOutput(u8g, U8G_PI_A0);
      if (U8G_PIN_NONE != u8g->pin_list[U8G_PI_RESET]) u8g_SetPIOutput(u8g, U8G_PI_RESET);
      SPI_speed = swSpiInit(SPI_SPEED, u8g->pin_list[U8G_PI_SCK], u8g->pin_list[U8G_PI_MOSI]);
      u8g_SetPILevel(u8g, U8G_PI_SCK, 0);
      u8g_SetPILevel(u8g, U8G_PI_MOSI, 0);
      break;

    case U8G_COM_MSG_STOP:
      break;

    case U8G_COM_MSG_RESET:
      if (U8G_PIN_NONE != u8g->pin_list[U8G_PI_RESET]) u8g_SetPILevel(u8g, U8G_PI_RESET, arg_val);
      break;

    case U8G_COM_MSG_CHIP_SELECT:
      #if EITHER(FYSETC_MINI_12864, MKS_MINI_12864)  // LCD SPI is running mode 3 while SD card is running mode 0
        if (arg_val) {                               //   SCK idle state needs to be set to the proper idle state before
                                                     //   the next chip select goes active
          u8g_SetPILevel(u8g, U8G_PI_SCK, 1);        // Set SCK to mode 3 idle state before CS goes active
          u8g_SetPILevel(u8g, U8G_PI_CS, LOW);
        }
        else {
          u8g_SetPILevel(u8g, U8G_PI_CS, HIGH);
          u8g_SetPILevel(u8g, U8G_PI_SCK, 0);  // Set SCK to mode 0 idle state after CS goes inactive
        }
      #else
        u8g_SetPILevel(u8g, U8G_PI_CS, !arg_val);
      #endif
      break;

    case U8G_COM_MSG_WRITE_BYTE:
      u8g_sw_spi_HAL_LPC1768_shift_out(u8g->pin_list[U8G_PI_MOSI], u8g->pin_list[U8G_PI_SCK], arg_val);
      break;

    case U8G_COM_MSG_WRITE_SEQ: {
        uint8_t *ptr = (uint8_t *)arg_ptr;
        while (arg_val > 0) {
          u8g_sw_spi_HAL_LPC1768_shift_out(u8g->pin_list[U8G_PI_MOSI], u8g->pin_list[U8G_PI_SCK], *ptr++);
          arg_val--;
        }
      }
      break;

      case U8G_COM_MSG_WRITE_SEQ_P: {
        uint8_t *ptr = (uint8_t *)arg_ptr;
        while (arg_val > 0) {
          u8g_sw_spi_HAL_LPC1768_shift_out(u8g->pin_list[U8G_PI_MOSI], u8g->pin_list[U8G_PI_SCK], u8g_pgm_read(ptr));
          ptr++;
          arg_val--;
        }
      }
      break;

    case U8G_COM_MSG_ADDRESS:                     /* define cmd (arg_val = 0) or data mode (arg_val = 1) */
      u8g_SetPILevel(u8g, U8G_PI_A0, arg_val);
      break;
  }
  return 1;
}

#endif // HAS_GRAPHICAL_LCD && !U8GLIB_ST7920
#endif // TARGET_LPC1768
