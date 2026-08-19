/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../inc/MarlinConfig.h"

#if ENABLED(CREALITY_BOOTLOADER_INFO_DUMP)

#include "../gcode.h"
#include "../../libs/hex_print.h"

// Layout of the stock Creality v4.2.x bootloader's flash-info page, reverse-
// engineered from disassembly. Read-only: nothing here is ever written.
#define CREALITY_BL_INFO_ADDR 0x0807F800UL // Start of the reserved bootloader info page
#define CREALITY_BL_INFO_SIZE 0x0800UL     // Page is 2K (ends at 0x0807FFFF)
#define CREALITY_BL_NAME_ADDR CREALITY_BL_INFO_ADDR // Last-flashed filename (128 bytes, NUL-terminated)
#define CREALITY_BL_NAME_SIZE 0x80UL
#define CREALITY_BL_MAGIC_ADDR (CREALITY_BL_INFO_ADDR + CREALITY_BL_NAME_SIZE) // 0x0807F880
#define CREALITY_BL_MAGIC_BYTE0 0xAA // Stored byte order: AA then 55 (not a little-endian 0xAA55 word)
#define CREALITY_BL_MAGIC_BYTE1 0x55

#if ENABLED(MARLIN_DEV_MODE)
  static void dump_flash_region(const uint32_t start, const uint32_t size) {
    const uint8_t *p = (const uint8_t*)start;
    for (uint32_t off = 0; off < size; off += 16) {
      print_hex_address((const void*)(start + off));
      SERIAL_CHAR(' ');
      const uint32_t linelen = _MIN(16UL, size - off);
      for (uint32_t i = 0; i < 16; ++i) {
        if (i < linelen) { SERIAL_CHAR(' '); print_hex_byte(p[off + i]); }
        else SERIAL_ECHOPGM("   ");
        if (i == 7) SERIAL_CHAR(' ');
      }
      SERIAL_ECHOPGM("  |");
      for (uint32_t i = 0; i < linelen; ++i) {
        const uint8_t c = p[off + i];
        SERIAL_CHAR((c >= 0x20 && c < 0x7F) ? (char)c : '.');
      }
      SERIAL_ECHOLNPGM("|");
      hal.watchdog_refresh();
    }
  }
#endif // MARLIN_DEV_MODE

static void decode_firmware_info() {
  const uint8_t *magic = (const uint8_t*)CREALITY_BL_MAGIC_ADDR;
  SERIAL_ECHOPGM("Magic header @");
  print_hex_address((const void*)CREALITY_BL_MAGIC_ADDR);
  SERIAL_ECHOPGM(": ");
  print_hex_byte(magic[0]);
  print_hex_byte(magic[1]);
  const bool magic_ok = magic[0] == CREALITY_BL_MAGIC_BYTE0 && magic[1] == CREALITY_BL_MAGIC_BYTE1;
  if (!magic_ok) {
    SERIAL_ECHOLNPGM(" (unexpected - not decoding)");
    return;
  }
  SERIAL_ECHOLNPGM(" (OK)");

  SERIAL_ECHOPGM("Last flashed: ");
  SERIAL_ECHOLN((const char*)CREALITY_BL_NAME_ADDR);
}

/**
 * M7422: Dump the Creality v4.2.x bootloader info page (read-only diagnostic)
 *
 *  With no parameters, Decode the last filename if the 0xAA55 magic header is present.
 *  D             : hex+ASCII dump the whole 2K bootloader info region. (MARLIN_DEV_MODE only)
 *  B             : hex+ASCII dump the bootloader. (MARLIN_DEV_MODE only)
 */
void GcodeSuite::M7422() {
  #if ENABLED(MARLIN_DEV_MODE)
    if (parser.seen_test('D'))
      dump_flash_region(CREALITY_BL_INFO_ADDR, CREALITY_BL_INFO_SIZE);
    else if (parser.seen_test('B'))
      dump_flash_region(0x08000000UL, 0x7000UL);
    else
  #endif // MARLIN_DEV_MODE
    decode_firmware_info();
}

#endif // CREALITY_BOOTLOADER_INFO_DUMP