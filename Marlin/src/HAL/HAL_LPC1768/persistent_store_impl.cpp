#ifdef TARGET_LPC1768
#include "../persistent_store_api.h"

#include "../../../types.h"
#include "../../../language.h"
#include "../../../serial.h"
#include "../../../utility.h"

#include "chanfs/diskio.h"
#include "chanfs/ff.h"


#if ENABLED(EEPROM_SETTINGS)

namespace HAL {
namespace PersistentStore {


FATFS fat_fs;
FIL eeprom_file;
#define _EEPROM_SIZE 4096
char eeprom_content[_EEPROM_SIZE];

bool access_start() {
  UINT bytes_read = 0;
  f_mount(&fat_fs, "", 1);
  FRESULT res = f_open(&eeprom_file, "eeprom.dat", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);

  if (res == FR_OK)
    res = f_read(&eeprom_file, eeprom_content, _EEPROM_SIZE, &bytes_read);

  for (; bytes_read < _EEPROM_SIZE; bytes_read++)
    eeprom_content[bytes_read] = 0;

  return (res == FR_OK);
}

bool access_finish() {
  UINT bytes_written = 0;
  f_lseek(&eeprom_file, 0);
  f_write(&eeprom_file, eeprom_content, _EEPROM_SIZE, &bytes_written);
  f_close(&eeprom_file);
  f_unmount("");
  return (bytes_written == _EEPROM_SIZE);
}

bool write_data(int &pos, const uint8_t *value, uint16_t size, uint16_t *crc) {
  for (uint16_t i = 0; i < size; i++) eeprom_content[pos + i] = value[i];
  crc16(crc, value, size);
  pos += size;
  return true;
}

void read_data(int &pos, uint8_t* value, uint16_t size, uint16_t *crc) {
  for (uint16_t i = 0; i < size; i++) value[i] = eeprom_content[pos + i];
  crc16(crc, value, size);
  pos += size;
}

} // PersistentStore
} // HAL

#endif // EEPROM_SETTINGS
#endif // ARDUINO_ARCH_AVR
