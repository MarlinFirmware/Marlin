/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2022 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#ifdef TARGET_LPC1768

#include "../../inc/MarlinConfig.h"

#if BOTH(SDSUPPORT, DISKIO_HOST_DRIVE)

// Overwrite DiskIO functions from LPC framework to use Marlin Sd2Card API.
#include <chanfs/diskio.h>
#include "../../sd/cardreader.h"

DRESULT disk_read (
  BYTE drv,     // Physical drive number (0)
  BYTE *buff,   // Pointer to the data buffer to store read data
  DWORD sector, // Start sector number (LBA)
  UINT count    // Number of sectors to read (1..128)
) {
  auto sd2card = card.getSd2Card();
  if (count == 1) {
    sd2card.readBlock(sector, buff);
    return RES_OK;
  }

  sd2card.readStart(sector);
  while (count--) {
    sd2card.readData(buff);
    buff += 512;
  }
  sd2card.readStop();
  return RES_OK;
}

DSTATUS disk_status(BYTE drv) {    // Physical drive number (0)
  return 0;
}

DSTATUS disk_initialize(BYTE drv) {    // Physical drive number (0)
  // If already mounted, it's already initialized!
  if (!card.isMounted()) {
    auto sd2card = card.getSd2Card();
    if (!sd2card.init(SPI_SPEED, SDSS)
      #if defined(LCD_SDSS) && (LCD_SDSS != SDSS)
        && !sd2card.init(SPI_SPEED, LCD_SDSS)
      #endif
    ) {
      return RES_ERROR;
    }
  }

  return RES_OK;
}

#if _DISKIO_WRITE

  DRESULT disk_write(
    BYTE drv,         // Physical drive number (0)
    const BYTE *buff, // Ponter to the data to write
    DWORD sector,     // Start sector number (LBA)
    UINT count        // Number of sectors to write (1..128)
  ) {
    auto sd2card = card.getSd2Card();
    if (count == 1) {
      sd2card.writeBlock(sector, buff);
      return RES_OK;
    }

    sd2card.writeStart(sector, count);
    while (count--) {
      sd2card.writeData(buff);
      buff += 512;
    }
    sd2card.writeStop();
    return RES_OK;
  }

#endif // _DISKIO_WRITE

#if _DISKIO_IOCTL

  DRESULT disk_ioctl(
    BYTE drv,   // Physical drive number (0)
    BYTE cmd,   // Control command code
    void *buff  // Pointer to the conrtol data
  ) {
    DWORD *dp, st, ed;

    auto sd2card = card.getSd2Card();
    switch (cmd) {
      case CTRL_SYNC:         // Wait for end of internal write process of the drive
      break;
      case GET_SECTOR_COUNT:  // Get drive capacity in unit of sector (DWORD)
        *(int32_t*)buff = sd2card.cardSize();
      break;
      case GET_BLOCK_SIZE:    // Get erase block size in unit of sector (DWORD)
      break;
      case CTRL_TRIM:         // Erase a block of sectors (used when _USE_TRIM in ffconf.h is 1)
        dp = (DWORD*)buff; st = dp[0]; ed = dp[1];        // Load sector block
        sd2card.erase(st, ed);
      break;
    }

    return RES_OK;
  }

#endif // _DISKIO_IOCTL

#endif // SDSUPPORT && DISKIO_HOST_DRIVE

#endif // TARGET_LPC1768
