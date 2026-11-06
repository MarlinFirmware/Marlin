/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2025 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * Direct ST7920 access for LIGHTWEIGHT_UI, sent through the U8g2 byte transport
 * so it works with whatever SPI callbacks the display was set up with.
 */

#include "../../../inc/MarlinConfigPre.h"

#if ALL(HAS_MARLINUI_U8G2, LIGHTWEIGHT_UI)

#include "../marlinui_DOGM.h"
#include "../../../HAL/shared/HAL_ST7920.h"

void ST7920_cs()      { u8x8_byte_StartTransfer(u8g.getU8x8()); }
void ST7920_ncs()     { u8x8_byte_EndTransfer(u8g.getU8x8()); }
void ST7920_set_cmd() { u8x8_byte_SendByte(u8g.getU8x8(), 0xF8); }
void ST7920_set_dat() { u8x8_byte_SendByte(u8g.getU8x8(), 0xFA); }

void ST7920_write_byte(const uint8_t val) {
  uint8_t b[2] = { uint8_t(val & 0xF0), uint8_t(val << 4) };
  u8x8_byte_SendBytes(u8g.getU8x8(), 2, b);
}

#endif // HAS_MARLINUI_U8G2 && LIGHTWEIGHT_UI
