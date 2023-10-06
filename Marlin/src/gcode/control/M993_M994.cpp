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

#include "../../inc/MarlinConfig.h"

#if ALL(SPI_FLASH, SDSUPPORT, MARLIN_DEV_MODE)

#include "../gcode.h"
#include "../../sd/cardreader.h"
#include "../../libs/W25Qxx.h"

/**
 * M993: Backup SPI Flash to SD
 */
void GcodeSuite::M993() {
  if (!card.isMounted()) card.mount();

  char fname[] = "spiflash.bin";
  card.openFileWrite(fname);
  if (!card.isFileOpen()) {
    SERIAL_ECHOLNPGM("Failed to open ", fname, " to write.");
    return;
  }

  uint8_t buf[1024];
  uint32_t addr = 0;
  W25QXX.init(SPI_QUARTER_SPEED);
  SERIAL_ECHOPGM("Save SPI Flash");
  while (addr < SPI_FLASH_SIZE) {
    W25QXX.SPI_FLASH_BufferRead(buf, addr, COUNT(buf));
    addr += COUNT(buf);
    card.write(buf, COUNT(buf));
    if (addr % (COUNT(buf) * 10) == 0) SERIAL_CHAR('.');
  }
  SERIAL_ECHOLNPGM(" done");

  card.closefile();
}

/**
 * M994: Load a backup from SD to SPI Flash
 */
void GcodeSuite::M994() {
  if (!card.isMounted()) card.mount();

  char fname[] = "spiflash.bin";
  card.openFileRead(fname);
  if (!card.isFileOpen()) {
    SERIAL_ECHOLNPGM("Failed to open ", fname, " to read.");
    return;
  }

  uint8_t buf[1024];
  uint32_t addr = 0;
  W25QXX.init(SPI_QUARTER_SPEED);
  W25QXX.SPI_FLASH_BulkErase();
  SERIAL_ECHOPGM("Load SPI Flash");
  while (addr < SPI_FLASH_SIZE) {
    card.read(buf, COUNT(buf));
    W25QXX.SPI_FLASH_BufferWrite(buf, addr, COUNT(buf));
    addr += COUNT(buf);
    if (addr % (COUNT(buf) * 10) == 0) SERIAL_CHAR('.');
  }
  SERIAL_ECHOLNPGM(" done");

  card.closefile();
}

#endif // SPI_FLASH && SDSUPPORT && MARLIN_DEV_MODE
