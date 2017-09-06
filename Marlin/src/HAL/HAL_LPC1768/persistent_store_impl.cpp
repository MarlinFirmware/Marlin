#ifdef TARGET_LPC1768

#include "../../inc/MarlinConfig.h"

#if ENABLED(EEPROM_SETTINGS)

#include "../persistent_store_api.h"

#include "../../core/serial.h"

#include "chanfs/diskio.h"
#include "chanfs/ff.h"

namespace HAL {
namespace PersistentStore {

FATFS fat_fs;
FIL eeprom_file;

bool access_start() {
  f_mount(&fat_fs, "", 1);
  FRESULT res = f_open(&eeprom_file, "eeprom.dat", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
  return (res == FR_OK);
}

bool access_finish() {
  f_close(&eeprom_file);
  f_unmount("");
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
