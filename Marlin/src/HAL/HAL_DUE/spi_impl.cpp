#ifdef ARDUINO_ARCH_SAM

#include <stdint.h>

namespace HAL {
namespace SPI {

bool initialise(uint8_t channel) {
  return false;
}

bool enable_cs(uint8_t channel) {
  return false;
}

void disable_cs(uint8_t channel) {

}

void set_frequency(uint8_t channel, uint32_t frequency) {

}

void read(uint8_t channel, uint8_t *buffer, uint32_t length) {

}

uint8_t read(uint8_t channel) {
  return '\0';
}

void write(uint8_t channel, const uint8_t *buffer, uint32_t length) {

}

void write(uint8_t channel, uint8_t value) {

}

void transfer(uint8_t channel, const uint8_t *buffer_write, uint8_t *buffer_read, uint32_t length) {

}

uint8_t transfer(uint8_t channel, uint8_t value) {
  return '\0';
}

} // namespace SPI
} // namespace HAL

#endif //#ifdef ARDUINO_ARCH_AVR
