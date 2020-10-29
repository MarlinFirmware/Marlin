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
#ifdef TARGET_LPC1768

#include "../../inc/MarlinConfig.h"
#include "../shared/Delay.h"
#include "../../../gcode/parser.h"

#if ENABLED(USE_WATCHDOG)
  #include "watchdog.h"
#endif

uint32_t HAL_adc_reading = 0;

// U8glib required functions
extern "C" {
  void u8g_xMicroDelay(uint16_t val) { DELAY_US(val); }
  void u8g_MicroDelay()              { u8g_xMicroDelay(1); }
  void u8g_10MicroDelay()            { u8g_xMicroDelay(10); }
  void u8g_Delay(uint16_t val)       { delay(val); }
}

//************************//

// return free heap space
int freeMemory() {
  char stack_end;
  void *heap_start = malloc(sizeof(uint32_t));
  if (heap_start == 0) return 0;

  uint32_t result = (uint32_t)&stack_end - (uint32_t)heap_start;
  free(heap_start);
  return result;
}

// scan command line for code
//   return index into pin map array if found and the pin is valid.
//   return dval if not found or not a valid pin.
int16_t PARSED_PIN_INDEX(const char code, const int16_t dval) {
  const uint16_t val = (uint16_t)parser.intval(code, -1), port = val / 100, pin = val % 100;
  const  int16_t ind = (port < ((NUM_DIGITAL_PINS) >> 5) && pin < 32) ? ((port << 5) | pin) : -2;
  return ind > -1 ? ind : dval;
}

void flashFirmware(const int16_t) { NVIC_SystemReset(); }

void HAL_clear_reset_source(void) {
  TERN_(USE_WATCHDOG, watchdog_clear_timeout_flag());
}

uint8_t HAL_get_reset_source(void) {
  #if ENABLED(USE_WATCHDOG)
    if (watchdog_timed_out()) return RST_WATCHDOG;
  #endif
  return RST_POWER_ON;
}

// Overwrite DiskIO functions from LPC framework to use Marlin Sd2Card API.
// TODO: need a define to enable/disable it... maybe a file only for this code!

#include <chanfs/diskio.h>
#include "../../sd/cardreader.h"

DRESULT disk_read (
	BYTE drv,		/* Physical drive number (0) */
	BYTE *buff,		/* Pointer to the data buffer to store read data */
	DWORD sector,	/* Start sector number (LBA) */
	UINT count		/* Number of sectors to read (1..128) */
)
{
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

DSTATUS disk_status (
	BYTE drv		/* Physical drive number (0) */
)
{
  return 0;
}

DSTATUS disk_initialize (
	BYTE drv		/* Physical drive number (0) */
) {
  //if already mounted, its already initialized!
  if (card.isMounted()) {
    return RES_OK;
  }

  auto sd2card = card.getSd2Card();
  if (!sd2card.init(SPI_SPEED, SDSS)
    #if defined(LCD_SDSS) && (LCD_SDSS != SDSS)
      && !sd2card.init(SPI_SPEED, LCD_SDSS)
    #endif
  ) {
    return RES_ERROR;
  }

  return RES_OK;
}

#if _DISKIO_WRITE
  DRESULT disk_write (
    BYTE drv,			/* Physical drive number (0) */
    const BYTE *buff,	/* Ponter to the data to write */
    DWORD sector,		/* Start sector number (LBA) */
    UINT count			/* Number of sectors to write (1..128) */
  )
  {
    auto sd2card = card.getSd2Card();
    if (count == 1) {
      sd2card.writeBlock(sector, buff);
      return RES_OK;
    }

    sd2card.writeStart(sector, count);
    while (count--)
    {
      sd2card.writeData(buff);
      buff += 512;
    }
    sd2card.writeStop();
    return RES_OK;
  }
#endif // _DISKIO_WRITE

#if _DISKIO_IOCTL

DRESULT disk_ioctl (
	BYTE drv,		/* Physical drive number (0) */
	BYTE cmd,		/* Control command code */
	void *buff		/* Pointer to the conrtol data */
)
{
  DWORD *dp, st, ed;

  auto sd2card = card.getSd2Card();
  switch (cmd) {
	  case CTRL_SYNC:			/* Wait for end of internal write process of the drive */
    break;
    case GET_SECTOR_COUNT:	/* Get drive capacity in unit of sector (DWORD) */
      *(int32_t*)buff = sd2card.cardSize();
    break;
    case GET_BLOCK_SIZE:	/* Get erase block size in unit of sector (DWORD) */
    break;
    case CTRL_TRIM:		/* Erase a block of sectors (used when _USE_TRIM in ffconf.h is 1) */
      dp = (DWORD*)buff; st = dp[0]; ed = dp[1];				/* Load sector block */
      sd2card.erase(st, ed);
    break;
  }

  return RES_OK;
}

#endif // _DISKIO_IOCTL

#endif // TARGET_LPC1768
