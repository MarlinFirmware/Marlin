/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * For two wire I2C interfaces.
 *
 * Three and four wire I2C interfaces have an A0 line. That line is
 * used to switch between command and data modes.
 *
 * The two wire LCDs use an instruction byte to signal if data or
 * command info is to follow. The command stream needs the instruction
 * byte between eack command byte. The data stream needs one at the
 * beginning.
 */

#include "../../../inc/MarlinConfigPre.h"

#if ANY(U8GLIB_SH1106, U8GLIB_SSD1306)

#include "HAL_LCD_com_defines.h"

// This routine adds the instruction byte between the command
// bytes to make the init sequences easier to read.

#define I2C_CMD_MODE 0x80

uint8_t u8g_WriteEscSeqP_2_wire(u8g_t *u8g, u8g_dev_t *dev, const uint8_t *esc_seq) {
  bool is_escape = false;
  for (;;) {
    uint8_t value = u8g_pgm_read(esc_seq);
    if (!is_escape) {
      if (value != 255) {
        if (u8g_WriteByte(u8g, dev, value) == 0) return 0;
        if (u8g_WriteByte(u8g, dev, I2C_CMD_MODE) == 0) return 0;
      }
      else {
        is_escape = true;
      }
    }
    else {
      if (value == 255) {
        if (u8g_WriteByte(u8g, dev, value) == 0) return 0;
        if (u8g_WriteByte(u8g, dev, I2C_CMD_MODE) == 0) return 0;
      }
      else if (value == 0xFE) {
        break;
      }
      else if (value >= 0xF0) {
        // Not yet used, do nothing
      }
      else if (value >= 0xE0) {
        u8g_SetAddress(u8g, dev, value & 0x0F);
      }
      else if (value >= 0xD0) {
        u8g_SetChipSelect(u8g, dev, value & 0x0F);
      }
      else if (value >= 0xC0) {
        u8g_SetResetLow(u8g, dev);
        value &= 0x0F;
        value <<= 4;
        value+=2;
        u8g_Delay(value);
        u8g_SetResetHigh(u8g, dev);
        u8g_Delay(value);
      }
      else if (value >= 0xBE) {                       // not yet implemented
        //u8g_SetVCC(u8g, dev, value & 0x01);
      }
      else if (value <= 0x7F) {
        u8g_Delay(value);
      }
      is_escape = false;
    }
    esc_seq++;
  }
  return 1;
}

uint8_t u8g_Write_Init_Sequence_2_wire(u8g_t *u8g, u8g_dev_t *dev, uint32_t length, const uint8_t *init_seq) {
  u8g_SetAddress(u8g, dev, 0);  // Instruction mode
  u8g_WriteSequence(u8g, dev, length, (uint8_t*)init_seq);
  return 1;
}

#endif // U8GLIB_SH1106 || U8GLIB_SSD1306
