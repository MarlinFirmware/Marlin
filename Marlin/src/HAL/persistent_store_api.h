#ifndef _PERSISTENT_STORE_H_
#define _PERSISTENT_STORE_H_

#include <stddef.h>
#include <stdint.h>

namespace HAL {
namespace PersistentStore {

bool access_start();
bool access_finish();
bool write_data(int &pos, const uint8_t *value, uint16_t size, uint16_t *crc);
void read_data(int &pos, uint8_t* value, uint16_t size, uint16_t *crc) ;

}
}


#endif /* _PERSISTANT_STORE_H_ */
