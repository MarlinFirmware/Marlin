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
  const char eeprom_zero = 0xFF;
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
    while (file_size < E2END && res == FR_OK) {
      res = f_write(&eeprom_file, &eeprom_zero, 1, &bytes_written);
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

bool write_data(int &pos, const uint8_t *value, uint16_t size, uint16_t *crc) {
  UINT bytes_written = 0;
  f_lseek(&eeprom_file, pos);
  f_write(&eeprom_file, (void *)value, size, &bytes_written);
  crc16(crc, value, size);
  pos = pos + size;
  return (bytes_written == size);
}

void read_data(int &pos, uint8_t* value, uint16_t size, uint16_t *crc) {
  UINT bytes_read = 0;
  f_lseek(&eeprom_file, pos);
  f_read(&eeprom_file, (void *)value, size, &bytes_read);
  crc16(crc, value, size);
  pos = pos + size;
}

} // PersistentStore
} // HAL

#endif // EEPROM_SETTINGS
#endif // TARGET_LPC1768
