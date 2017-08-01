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

bool access_start() {
  f_mount(&fat_fs, "", 1);
  FRESULT res = f_open(&eeprom_file, "eeprom.dat", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
  return (res == FR_OK);
}

bool access_finish(){
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

}
}

#endif // EEPROM_SETTINGS
#endif // ARDUINO_ARCH_AVR
