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

/**
 * Implementation of EEPROM settings in SD Card
 */

#ifdef TARGET_LPC1768

#include "../../inc/MarlinConfig.h"

#if ENABLED(SDCARD_EEPROM_EMULATION)

//#define DEBUG_SD_EEPROM_EMULATION

#include "../shared/eeprom_api.h"

#include <chanfs/diskio.h>
#include <chanfs/ff.h>

extern uint32_t MSC_Aquire_Lock();
extern uint32_t MSC_Release_Lock();

FATFS fat_fs;
FIL eeprom_file;
bool eeprom_file_open = false;

#define EEPROM_FILENAME "eeprom.dat"
#ifndef MARLIN_EEPROM_SIZE
  #define MARLIN_EEPROM_SIZE size_t(0x1000) // 4KiB of Emulated EEPROM
#endif

size_t PersistentStore::capacity() { return MARLIN_EEPROM_SIZE; }

bool PersistentStore::access_start() {
  const char eeprom_erase_value = 0xFF;
  MSC_Aquire_Lock();
  if (f_mount(&fat_fs, "", 1)) {
    MSC_Release_Lock();
    return false;
  }
  FRESULT res = f_open(&eeprom_file, EEPROM_FILENAME, FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
  if (res) MSC_Release_Lock();

  if (res == FR_OK) {
    UINT bytes_written;
    FSIZE_t file_size = f_size(&eeprom_file);
    f_lseek(&eeprom_file, file_size);
    while (file_size < capacity() && res == FR_OK) {
      res = f_write(&eeprom_file, &eeprom_erase_value, 1, &bytes_written);
      file_size++;
    }
  }
  if (res == FR_OK) {
    f_lseek(&eeprom_file, 0);
    f_sync(&eeprom_file);
    eeprom_file_open = true;
  }
  return res == FR_OK;
}

bool PersistentStore::access_finish() {
  f_close(&eeprom_file);
  f_unmount("");
  MSC_Release_Lock();
  eeprom_file_open = false;
  return true;
}

// This extra chit-chat goes away soon, but is helpful for now
// to see errors that are happening in read_data / write_data
static void debug_rw(const bool write, int &pos, const uint8_t *value, const size_t size, const FRESULT s, const size_t total=0) {
  #if ENABLED(DEBUG_SD_EEPROM_EMULATION)
    PGM_P const rw_str = write ? PSTR("write") : PSTR("read");
    SERIAL_CHAR(' ');
    SERIAL_ECHOPGM_P(rw_str);
    SERIAL_ECHOLNPGM("_data(", pos, ",", *value, ",", size, ", ...)");
    if (total) {
      SERIAL_ECHOPGM(" f_");
      SERIAL_ECHOPGM_P(rw_str);
      SERIAL_ECHOPGM("()=", s, "\n size=", size, "\n bytes_");
      SERIAL_ECHOLNPGM_P(write ? PSTR("written=") : PSTR("read="), total);
    }
    else
      SERIAL_ECHOLNPGM(" f_lseek()=", s);
  #endif
}

// File function return codes for type FRESULT. This goes away soon, but
// is helpful right now to see any errors in read_data and write_data.
//
//  typedef enum {
//    FR_OK = 0,               /* (0) Succeeded */
//    FR_DISK_ERR,             /* (1) A hard error occurred in the low level disk I/O layer */
//    FR_INT_ERR,              /* (2) Assertion failed */
//    FR_NOT_READY,            /* (3) The physical drive cannot work */
//    FR_NO_FILE,              /* (4) Could not find the file */
//    FR_NO_PATH,              /* (5) Could not find the path */
//    FR_INVALID_NAME,         /* (6) The path name format is invalid */
//    FR_DENIED,               /* (7) Access denied due to prohibited access or directory full */
//    FR_EXIST,                /* (8) Access denied due to prohibited access */
//    FR_INVALID_OBJECT,       /* (9) The file/directory object is invalid */
//    FR_WRITE_PROTECTED,      /* (10) The physical drive is write protected */
//    FR_INVALID_DRIVE,        /* (11) The logical drive number is invalid */
//    FR_NOT_ENABLED,          /* (12) The volume has no work area */
//    FR_NO_FILESYSTEM,        /* (13) There is no valid FAT volume */
//    FR_MKFS_ABORTED,         /* (14) The f_mkfs() aborted due to any problem */
//    FR_TIMEOUT,              /* (15) Could not get a grant to access the volume within defined period */
//    FR_LOCKED,               /* (16) The operation is rejected according to the file sharing policy */
//    FR_NOT_ENOUGH_CORE,      /* (17) LFN working buffer could not be allocated */
//    FR_TOO_MANY_OPEN_FILES,  /* (18) Number of open files > FF_FS_LOCK */
//    FR_INVALID_PARAMETER     /* (19) Given parameter is invalid */
//  } FRESULT;

bool PersistentStore::write_data(int &pos, const uint8_t *value, size_t size, uint16_t *crc) {
  if (!eeprom_file_open) return true;
  FRESULT s;
  UINT bytes_written = 0;

  s = f_lseek(&eeprom_file, pos);
  if (s) {
    debug_rw(true, pos, value, size, s);
    return s;
  }

  s = f_write(&eeprom_file, (void*)value, size, &bytes_written);
  if (s) {
    debug_rw(true, pos, value, size, s, bytes_written);
    return s;
  }
  crc16(crc, value, size);
  pos += size;
  return bytes_written != size;  // return true for any error
}

bool PersistentStore::read_data(int &pos, uint8_t *value, const size_t size, uint16_t *crc, const bool writing/*=true*/) {
  if (!eeprom_file_open) return true;
  UINT bytes_read = 0;
  FRESULT s;
  s = f_lseek(&eeprom_file, pos);

  if (s) {
    debug_rw(false, pos, value, size, s);
    return true;
  }

  if (writing) {
    s = f_read(&eeprom_file, (void*)value, size, &bytes_read);
    crc16(crc, value, size);
  }
  else {
    uint8_t temp[size];
    s = f_read(&eeprom_file, (void*)temp, size, &bytes_read);
    crc16(crc, temp, size);
  }

  if (s) {
    debug_rw(false, pos, value, size, s, bytes_read);
    return true;
  }

  pos += size;
  return bytes_read != size;  // return true for any error
}

#endif // SDCARD_EEPROM_EMULATION
#endif // TARGET_LPC1768
