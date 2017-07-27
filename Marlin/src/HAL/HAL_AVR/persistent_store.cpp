#include "../persistent_store_api.h"

#include "../../../types.h"
#include "../../../language.h"
#include "../../../serial.h"
#include "../../../utility.h"

#if defined(ARDUINO_ARCH_AVR)
#if ENABLED(EEPROM_SETTINGS)

namespace HAL {
namespace PersistentStore {

bool access_start() {
  return true;
}

bool access_finish(){
  return true;
}

bool write_data(int &pos, const uint8_t *value, uint16_t size, uint16_t *crc) {
  while (size--) {
    uint8_t * const p = (uint8_t * const)pos;
    uint8_t v = *value;
    // EEPROM has only ~100,000 write cycles,
    // so only write bytes that have changed!
    if (v != eeprom_read_byte(p)) {
      eeprom_write_byte(p, v);
      if (eeprom_read_byte(p) != v) {
        SERIAL_ECHO_START();
        SERIAL_ECHOLNPGM(MSG_ERR_EEPROM_WRITE);
        return false;
      }
    }
    crc16(crc, &v, 1);
    pos++;
    value++;
  };
  return true;
}

void read_data(int &pos, uint8_t* value, uint16_t size, uint16_t *crc) {
  do {
    uint8_t c = eeprom_read_byte((unsigned char*)pos);
    *value = c;
    crc16(crc, &c, 1);
    pos++;
    value++;
  } while (--size);
}

}
}

#endif // #if ENABLED(EEPROM_SETTINGS)
#endif // #if defined(ARDUINO_ARCH_AVR)
