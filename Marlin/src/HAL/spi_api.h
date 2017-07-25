#ifndef _SPI_API_H_
#define _SPI_API_H_

#include <stdint.h>
#include "HAL_spi_pins.h"

namespace HAL {
namespace SPI {

enum SPI_CHANNELS {
  CHANNEL_0 = 0,
  CHANNEL_1,
  CHANNEL_2,
  CHANNEL_3,
  CHANNEL_4,
  CHANNEL_5
};

/*
 * Initialise the hardware layer (pins and peripheral)
 */
bool initialise(uint8_t channel);

/*
 * Allow override of automatic Chip Select
 */
bool enable_cs(uint8_t channel);
void disable_cs(uint8_t channel);


void set_frequency(uint8_t channel, uint32_t frequency);

void read(uint8_t channel, uint8_t *buffer, uint32_t length);
uint8_t read(uint8_t channel);

void write(uint8_t channel, const uint8_t *buffer, uint32_t length);
void write(uint8_t channel, uint8_t value);

void transfer(uint8_t channel, const uint8_t *buffer_write, uint8_t *buffer_read, uint32_t length);
uint8_t transfer(uint8_t channel, uint8_t value);

}
}


#endif /* _SPI_API_H_ */
