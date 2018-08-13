#ifndef _PERSISTENT_STORE_H_
#define _PERSISTENT_STORE_H_

#include <stddef.h>
#include <stdint.h>

class PersistentStore {
public:
  static bool access_start();
  static bool access_finish();
  static bool write_data(int &pos, const uint8_t *value, size_t size, uint16_t *crc);
  static bool read_data(int &pos, uint8_t* value, size_t size, uint16_t *crc, const bool writing=true);
  static bool write_data(const int pos, uint8_t* value, size_t size);
  static bool read_data(const int pos, uint8_t* value, size_t size);
  static const size_t capacity();
};

extern PersistentStore persistentStore;

#endif // _PERSISTENT_STORE_H_
