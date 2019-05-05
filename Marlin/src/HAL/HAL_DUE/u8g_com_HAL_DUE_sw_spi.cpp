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

#ifdef ARDUINO_ARCH_SAM

#include "../../inc/MarlinConfigPre.h"

#if HAS_GRAPHICAL_LCD && !ENABLED(U8GLIB_ST7920)

#undef SPI_SPEED
#define SPI_SPEED 2  // About 2 MHz

#include "../shared/Marduino.h"
#include "../shared/Delay.h"

#include <U8glib.h>

void u8g_SetPIOutput_DUE(u8g_t *u8g, uint8_t pin_index);
void u8g_SetPILevel_DUE(u8g_t *u8g, uint8_t pin_index, uint8_t level);
void U8G_spiSend_sw_DUE_mode_0(uint8_t val);
void U8G_spiSend_sw_DUE_mode_3(uint8_t val);

Pio *SCK_pPio, *MOSI_pPio;
uint32_t SCK_dwMask, MOSI_dwMask;

#if ENABLED(FYSETC_MINI_12864)
  #define U8G_spiSend_sw_DUE U8G_spiSend_sw_DUE_mode_3
#else
  #define U8G_spiSend_sw_DUE U8G_spiSend_sw_DUE_mode_0
#endif

uint8_t u8g_com_HAL_DUE_sw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr) {
  switch (msg) {
    case U8G_COM_MSG_INIT:
      SCK_pPio = g_APinDescription[u8g->pin_list[U8G_PI_SCK]].pPort;
      SCK_dwMask = g_APinDescription[u8g->pin_list[U8G_PI_SCK]].ulPin;
      MOSI_pPio = g_APinDescription[u8g->pin_list[U8G_PI_MOSI]].pPort;
      MOSI_dwMask = g_APinDescription[u8g->pin_list[U8G_PI_MOSI]].ulPin;
      u8g_SetPIOutput_DUE(u8g, U8G_PI_SCK);
      u8g_SetPIOutput_DUE(u8g, U8G_PI_MOSI);
      u8g_SetPIOutput_DUE(u8g, U8G_PI_CS);
      u8g_SetPIOutput_DUE(u8g, U8G_PI_A0);
      if (U8G_PIN_NONE != u8g->pin_list[U8G_PI_RESET]) u8g_SetPIOutput_DUE(u8g, U8G_PI_RESET);
      u8g_SetPILevel_DUE(u8g, U8G_PI_SCK, 0);
      u8g_SetPILevel_DUE(u8g, U8G_PI_MOSI, 0);
      break;

    case U8G_COM_MSG_STOP:
      break;

    case U8G_COM_MSG_RESET:
      if (U8G_PIN_NONE != u8g->pin_list[U8G_PI_RESET]) u8g_SetPILevel_DUE(u8g, U8G_PI_RESET, arg_val);
      break;

    case U8G_COM_MSG_CHIP_SELECT:
      #if ENABLED(FYSETC_MINI_12864)           // LCD SPI is running mode 3 while SD card is running mode 0
        if (arg_val) {                        //   SCK idle state needs to be set to the proper idle state before
                                               //   the next chip select goes active
          u8g_SetPILevel_DUE(u8g, U8G_PI_SCK, 1);  //set SCK to mode 3 idle state before CS goes active
          u8g_SetPILevel_DUE(u8g, U8G_PI_CS, LOW);
        }
        else {
          u8g_SetPILevel_DUE(u8g, U8G_PI_CS, HIGH);
          u8g_SetPILevel_DUE(u8g, U8G_PI_SCK, 0); //set SCK to mode 0 idle state after CS goes inactive
        }
      #else
        u8g_SetPILevel_DUE(u8g, U8G_PI_CS, !arg_val);
      #endif
      break;

    case U8G_COM_MSG_WRITE_BYTE:
      U8G_spiSend_sw_DUE(arg_val);
      break;

    case U8G_COM_MSG_WRITE_SEQ: {
        uint8_t *ptr = (uint8_t *)arg_ptr;
        while (arg_val > 0) {
          U8G_spiSend_sw_DUE(*ptr++);
          arg_val--;
        }
      }
      break;

      case U8G_COM_MSG_WRITE_SEQ_P: {
        uint8_t *ptr = (uint8_t *)arg_ptr;
        while (arg_val > 0) {
          U8G_spiSend_sw_DUE(u8g_pgm_read(ptr));
          ptr++;
          arg_val--;
        }
      }
      break;

    case U8G_COM_MSG_ADDRESS:                     /* define cmd (arg_val = 0) or data mode (arg_val = 1) */
      u8g_SetPILevel_DUE(u8g, U8G_PI_A0, arg_val);
      break;
  }
  return 1;
}

#endif // HAS_GRAPHICAL_LCD
#endif // ARDUINO_ARCH_SAM
