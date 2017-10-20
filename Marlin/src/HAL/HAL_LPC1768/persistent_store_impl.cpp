#ifdef TARGET_LPC1768

#include "../../inc/MarlinConfig.h"

#if ENABLED(EEPROM_SETTINGS)

#include "../persistent_store_api.h"

#include "chanfs/diskio.h"
#include "chanfs/ff.h"

extern uint32_t MSC_Aquire_Lock();
extern uint32_t MSC_Release_Lock();

namespace HAL {
namespace PersistentStore {

FATFS fat_fs;
FIL eeprom_file;

bool access_start() {
  UINT file_size = 0,
       bytes_written = 0;
  const char eeprom_erase_value = 0xFF;
  MSC_Aquire_Lock();
  if (f_mount(&fat_fs, "", 1)) {
    MSC_Release_Lock();
    return false;
  }
  FRESULT res = f_open(&eeprom_file, "eeprom.dat", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
  if (res) MSC_Release_Lock();

  if (res == FR_OK) file_size = f_size(&eeprom_file);

  if (res == FR_OK) {
    f_lseek(&eeprom_file, file_size);
    while (file_size <= E2END && res == FR_OK) {
      res = f_write(&eeprom_file, &eeprom_erase_value, 1, &bytes_written);
      file_size++;
    }
  }
  if (res == FR_OK) {
    f_lseek(&eeprom_file, 0);
    f_sync(&eeprom_file);
  }
  return res == FR_OK;
}

bool access_finish() {
  f_close(&eeprom_file);
  f_unmount("");
  MSC_Release_Lock();
  return true;
}

// File function return codes for type FRESULT   This goes away soon.   But it is helpful right now to see
// the different errors the read_data() and write_data() functions are seeing.
//
//typedef enum {
//      FR_OK = 0,               /* (0) Succeeded */
//      FR_DISK_ERR,             /* (1) A hard error occurred in the low level disk I/O layer */
//      FR_INT_ERR,              /* (2) Assertion failed */
//      FR_NOT_READY,            /* (3) The physical drive cannot work */
//      FR_NO_FILE,              /* (4) Could not find the file */
//      FR_NO_PATH,              /* (5) Could not find the path */
//      FR_INVALID_NAME,         /* (6) The path name format is invalid */
//      FR_DENIED,               /* (7) Access denied due to prohibited access or directory full */
//      FR_EXIST,                /* (8) Access denied due to prohibited access */
//      FR_INVALID_OBJECT,       /* (9) The file/directory object is invalid */
//      FR_WRITE_PROTECTED,      /* (10) The physical drive is write protected */
//      FR_INVALID_DRIVE,        /* (11) The logical drive number is invalid */
//      FR_NOT_ENABLED,          /* (12) The volume has no work area */
//      FR_NO_FILESYSTEM,        /* (13) There is no valid FAT volume */
//      FR_MKFS_ABORTED,         /* (14) The f_mkfs() aborted due to any problem */
//      FR_TIMEOUT,              /* (15) Could not get a grant to access the volume within defined period */
//      FR_LOCKED,               /* (16) The operation is rejected according to the file sharing policy */
//      FR_NOT_ENOUGH_CORE,      /* (17) LFN working buffer could not be allocated */
//      FR_TOO_MANY_OPEN_FILES,  /* (18) Number of open files > FF_FS_LOCK */
//      FR_INVALID_PARAMETER     /* (19) Given parameter is invalid */
//} FRESULT;

bool write_data(int &pos, const uint8_t *value, uint16_t size, uint16_t *crc) {
  FRESULT s;
  UINT bytes_written = 0;
  s = f_lseek(&eeprom_file, pos);
  if ( s ) {
   SERIAL_PROTOCOLPAIR(" write_data(", pos);         // This extra chit-chat goes away soon.  But it is helpful
   SERIAL_PROTOCOLPAIR(",", (int) value);            // right now to see errors that are happening in the 
   SERIAL_PROTOCOLPAIR(",", (int) size);             // read_data() and write_data() functions
   SERIAL_PROTOCOL("...)\n");
   SERIAL_PROTOCOLPAIR(" f_lseek()=", (int) s);
   SERIAL_PROTOCOL("\n");
   return s;
  }
  s = f_write(&eeprom_file, (void *)value, size, &bytes_written);
  if ( s ) {
   SERIAL_PROTOCOLPAIR(" write_data(", pos);         // This extra chit-chat goes away soon.  But it is helpful
   SERIAL_PROTOCOLPAIR(",", (int) value);            // right now to see errors that are happening in the 
   SERIAL_PROTOCOLPAIR(",", (int) size);             // read_data() and write_data() functions
   SERIAL_PROTOCOL("...)\n");
   SERIAL_PROTOCOLPAIR(" f_write()=", (int) s);
   SERIAL_PROTOCOL("\n");
   SERIAL_PROTOCOLPAIR(" size=",  (int) size);
   SERIAL_PROTOCOLPAIR("\n bytes_written=",  (int) bytes_written);
   SERIAL_PROTOCOL("\n");
   return s;
  }
  crc16(crc, value, size);
  pos = pos + size;
  return (bytes_written != size);  // return true for any error
}

bool read_data(int &pos, uint8_t* value, uint16_t size, uint16_t *crc) {
  UINT bytes_read = 0;
  FRESULT s;
  s = f_lseek(&eeprom_file, pos);
  if ( s ) {
   SERIAL_PROTOCOLPAIR(" read_data(", pos);          // This extra chit-chat goes away soon.  But it is helpful
   SERIAL_PROTOCOLPAIR(",", (int) value);            // right now to see errors that are happening in the 
   SERIAL_PROTOCOLPAIR(",", (int) size);             // read_data() and write_data() functions
   SERIAL_PROTOCOL("...)\n");
   SERIAL_PROTOCOLPAIR(" f_lseek()=", (int) s);
   SERIAL_PROTOCOL("\n");
   return true;
  }
  s = f_read(&eeprom_file, (void *)value, size, &bytes_read);
  if ( s ) {
   SERIAL_PROTOCOLPAIR(" read_data(", pos);         // This extra chit-chat goes away soon.  But it is helpful
   SERIAL_PROTOCOLPAIR(",", (int) value);           // right now to see errors that are happening in the 
   SERIAL_PROTOCOLPAIR(",", (int) size);            // read_data() and write_data() functions
   SERIAL_PROTOCOL("...)\n");
   SERIAL_PROTOCOLPAIR(" f_write()=", (int) s);
   SERIAL_PROTOCOL("\n");
   SERIAL_PROTOCOLPAIR(" size=",  (int) size);
   SERIAL_PROTOCOLPAIR("\n bytes_read=",  (int) bytes_read);
   SERIAL_PROTOCOL("\n");
   return true;
  }
  crc16(crc, value, size);
  pos = pos + size;
  return bytes_read != size;  // return true for any error
}

} // PersistentStore
} // HAL

#endif // EEPROM_SETTINGS
#endif // TARGET_LPC1768
