/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2022 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * Copypaste of SAMD51 HAL developed by Giuliano Zaro (AKA GMagician)
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
#ifdef ARDUINO_ARCH_ESP32

#include "../../inc/MarlinConfigPre.h"

#if EITHER(MKS_MINI_12864, FYSETC_MINI_12864_2_1)

#include <U8glib-HAL.h>
#include "Arduino.h"
#include "../shared/HAL_SPI.h"
#include "HAL.h"
#include "SPI.h"

static SPISettings spiConfig;

#define MDOGLCD_MOSI    23
#define MDOGLCD_SCK     18
#define MLCD_RESET_PIN   0
#define MLCD_PINS_DC     4
#define MDOGLCD_CS      21
#define MDOGLCD_A0       4

#ifndef LCD_SPI_SPEED
  #ifdef SD_SPI_SPEED
    #define LCD_SPI_SPEED SD_SPI_SPEED    // Assume SPI speed shared with SD
  #else
    #define LCD_SPI_SPEED SPI_FULL_SPEED  // Use full speed if SD speed is not supplied
  #endif
#endif

uint8_t u8g_eps_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr) {
  static uint8_t msgInitCount = 2; // Ignore all messages until 2nd U8G_COM_MSG_INIT
  if (msgInitCount) {
    if (msg == U8G_COM_MSG_INIT) msgInitCount--;
    if (msgInitCount) return -1;
  }

  switch (msg) {
    case U8G_COM_MSG_STOP: break;

    case U8G_COM_MSG_INIT:
      OUT_WRITE(MDOGLCD_CS, HIGH);
      OUT_WRITE(MDOGLCD_A0, HIGH);
      OUT_WRITE(MLCD_RESET_PIN, HIGH);
      u8g_Delay(5);
      spiBegin();
      spiInit(LCD_SPI_SPEED);
      break;

    case U8G_COM_MSG_ADDRESS:           /* define cmd (arg_val = 0) or data mode (arg_val = 1) */
      WRITE(MDOGLCD_A0, arg_val ? HIGH : LOW);
      break;

    case U8G_COM_MSG_CHIP_SELECT:       /* arg_val == 0 means HIGH level of U8G_PI_CS */
      WRITE(MDOGLCD_CS, arg_val ? LOW : HIGH);
      break;

    case U8G_COM_MSG_RESET:
      WRITE(MLCD_RESET_PIN, arg_val);
      break;

    case U8G_COM_MSG_WRITE_BYTE:
      spiSend((uint8_t)arg_val);
      break;

    case U8G_COM_MSG_WRITE_SEQ:
      uint8_t *ptr = (uint8_t*) arg_ptr;
      while (arg_val > 0) {
        spiSend(*ptr++);
        arg_val--;
      }
      break;
  }
  return 1;
}

#endif // EITHER(MKS_MINI_12864, FYSETC_MINI_12864_2_1)

#endif // ARDUINO_ARCH_ESP32
