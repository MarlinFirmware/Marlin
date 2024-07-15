/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2024 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm / Ryan Power
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

#ifdef HAL_STM32

#include "../../../inc/MarlinConfig.h"

#if ALL(HAS_MARLINUI_U8GLIB, FORCE_SOFT_SPI)

#include <U8glib-HAL.h>
#include "../../shared/HAL_SPI.h"

#define nop asm volatile ("\tnop\n")

static inline uint8_t swSpiTransfer_mode_0(uint8_t b) {
  for (uint8_t i = 0; i < 8; ++i) {
    const uint8_t state = (b & 0x80) ? HIGH : LOW;
    WRITE(DOGLCD_SCK, HIGH);
    WRITE(DOGLCD_MOSI, state);
    b <<= 1;
    WRITE(DOGLCD_SCK, LOW);
  }
  return b;
}

static inline uint8_t swSpiTransfer_mode_3(uint8_t b) {
  for (uint8_t i = 0; i < 8; ++i) {
    const uint8_t state = (b & 0x80) ? HIGH : LOW;
    WRITE(DOGLCD_SCK, LOW);
    WRITE(DOGLCD_MOSI, state);
    b <<= 1;
    WRITE(DOGLCD_SCK, HIGH);
  }
  return b;
}

static void u8g_sw_spi_shift_out(uint8_t val) {
  #if U8G_SPI_USE_MODE_3
    swSpiTransfer_mode_3(val);
  #else
    swSpiTransfer_mode_0(val);
  #endif
}

static void swSpiInit() {
  #if PIN_EXISTS(LCD_RESET)
    SET_OUTPUT(LCD_RESET_PIN);
  #endif
  SET_OUTPUT(DOGLCD_A0);
  OUT_WRITE(DOGLCD_SCK, LOW);
  OUT_WRITE(DOGLCD_MOSI, LOW);
  OUT_WRITE(DOGLCD_CS, HIGH);
}

uint8_t u8g_com_HAL_STM32_sw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr) {
  switch (msg) {
    case U8G_COM_MSG_INIT:
      swSpiInit();
      break;

    case U8G_COM_MSG_STOP:
      break;

    case U8G_COM_MSG_RESET:
      #if PIN_EXISTS(LCD_RESET)
        WRITE(LCD_RESET_PIN, arg_val);
      #endif
      break;

    case U8G_COM_MSG_CHIP_SELECT:
      #if U8G_SPI_USE_MODE_3        // This LCD SPI is running mode 3 while SD card is running mode 0
        if (arg_val) {              // SCK idle state needs to be set to the proper idle state before
                                    //  the next chip select goes active
          WRITE(DOGLCD_SCK, HIGH);  // Set SCK to mode 3 idle state before CS goes active
          WRITE(DOGLCD_CS, LOW);
          nop; // hold SCK high for a few ns
          nop;
         }
        else {
          WRITE(DOGLCD_CS, HIGH);
          WRITE(DOGLCD_SCK, LOW);   // Set SCK to mode 0 idle state after CS goes inactive
        }
      #else
        WRITE(DOGLCD_CS, !arg_val);
      #endif
      break;

    case U8G_COM_MSG_WRITE_BYTE:
      u8g_sw_spi_shift_out(arg_val);
      break;

    case U8G_COM_MSG_WRITE_SEQ: {
      uint8_t *ptr = (uint8_t *)arg_ptr;
      while (arg_val > 0) {
        u8g_sw_spi_shift_out(*ptr++);
        arg_val--;
      }
    } break;

    case U8G_COM_MSG_WRITE_SEQ_P: {
      uint8_t *ptr = (uint8_t *)arg_ptr;
      while (arg_val > 0) {
        u8g_sw_spi_shift_out(u8g_pgm_read(ptr));
        ptr++;
        arg_val--;
      }
    } break;

    case U8G_COM_MSG_ADDRESS: /* define cmd (arg_val = 0) or data mode (arg_val = 1) */
      WRITE(DOGLCD_A0, arg_val);
      break;
  }
  return 1;
}

#endif // HAS_MARLINUI_U8GLIB && FORCE_SOFT_SPI
#endif // HAL_STM32
