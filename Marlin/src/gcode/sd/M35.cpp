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

#if ENABLED(SD_FILE_CHECKSUM)

#include "../gcode.h"
#include "../../MarlinCore.h"
#include "../../sd/cardreader.h"

/**
 * Update a CRC-32 as specified for the POSIX cksum utility.
 *
 * The final bytewise complement is applied after the caller has appended the
 * file length, least-significant byte first. This makes M35 directly
 * comparable with the host command: `cksum file.gcode`.
 */
static uint32_t cksum_crc32(uint32_t crc, const uint8_t *data, uint16_t count) {
  while (count--) {
    crc ^= uint32_t(*data++) << 24;
    for (uint8_t bit = 0; bit < 8; ++bit)
      crc = (crc << 1) ^ (crc & 0x80000000UL ? 0x04C11DB7UL : 0);
  }
  return crc;
}

/**
 * M35 <filename>: Report the POSIX cksum CRC and size of an SD-card file.
 *
 * The pathname has exactly the same syntax and resolution rules as M23. In
 * particular, a filename usable by M23 / M24 is usable by M35; with long
 * filename support enabled, that includes long names accepted by M23. M35
 * uses the same FAT namespace, so it introduces no separate short/long-name
 * lookup or conflict behavior.
 *
 * The command is intentionally unavailable during an SD print. It opens the
 * requested file independently and reads it in small blocks, so it neither
 * changes the selected print file nor needs a large RAM buffer.
 */
void GcodeSuite::M35() {
  if (!card.isMounted()) {
    SERIAL_ECHO_MSG(STR_NO_MEDIA);
    return;
  }
  if (card.isPrinting()) {
    SERIAL_ERROR_MSG("M35 unavailable while SD printing");
    return;
  }
  if (!parser.string_arg[0]) {
    SERIAL_ERROR_MSG("M35 requires a filename");
    return;
  }

  // Match M23: Simplify3D may append a space and the file size.
  for (char *fn = parser.string_arg; *fn; ++fn)
    if (*fn == ' ') *fn = '\0';

  MediaFile *dir;
  const char * const fname = card.diveToFile(false, dir, parser.string_arg);
  if (!fname) {
    SERIAL_ECHOLNPGM(STR_SD_OPEN_FILE_FAIL, parser.string_arg, ".");
    return;
  }

  MediaFile file;
  if (!file.open(dir, fname, O_READ) || !file.isFile()) {
    file.close();
    SERIAL_ECHOLNPGM(STR_SD_OPEN_FILE_FAIL, parser.string_arg, ".");
    return;
  }

  const uint32_t file_size = file.fileSize();
  uint32_t crc = 0;
  uint8_t buffer[32];
  bool read_error = false;

  for (;;) {
    const int16_t bytes_read = file.read(buffer, sizeof(buffer));
    if (bytes_read < 0) {
      read_error = true;
      break;
    }
    if (!bytes_read) break;
    crc = cksum_crc32(crc, buffer, bytes_read);
    idle();
  }
  file.close();

  if (read_error) {
    SERIAL_ERROR_MSG("M35 read failed: ", parser.string_arg);
    return;
  }

  for (uint32_t length = file_size; length; length >>= 8) {
    const uint8_t byte = length;
    crc = cksum_crc32(crc, &byte, 1);
  }

  SERIAL_ECHOLNPGM("File: ", parser.string_arg);
  SERIAL_ECHOLNPGM("Size: ", file_size);
  SERIAL_ECHOLNPGM("cksum: ", ~crc);
}

#endif // SD_FILE_CHECKSUM
