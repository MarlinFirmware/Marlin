#include <Wire.h>
#include "fastio_ESP.h"

uint8_t get_i2c_register(uint8_t reg) {
  Wire.beginTransmission(GPIOX_I2C_ADDR);
  Wire.write(reg);
  Wire.endTransmission();

  Wire.requestFrom(GPIOX_I2C_ADDR, 1);
  return Wire.read();
}

void set_i2c_register(uint8_t reg, uint8_t val) {
  Wire.beginTransmission(GPIOX_I2C_ADDR);
  Wire.write(reg);
  Wire.write(val);
  Wire.endTransmission();
}

void set_i2c_bit(uint8_t reg, uint8_t bit, uint8_t val) {
  uint8_t r = get_i2c_register(reg);
  r = val ? (r | (1 << bit)) : (r & ~(1 << bit));
  set_i2c_register(reg, r);
}

void set_i2c_pin_mode(uint8_t pin, uint8_t mode) {
  set_i2c_bit(GET_REGISTER(pin, IODIRA), GET_PIN(pin), mode & INPUT ? 1 : 0);
}

void set_i2c_pin_pullup(uint8_t pin, bool pullup) {
  set_i2c_bit(GET_REGISTER(pin, GPPUA), GET_PIN(pin), pullup);
}

uint8_t get_i2c_pin(uint8_t pin) {
  return ((get_i2c_register(GET_REGISTER(pin, GPIOA)) >> GET_PIN(pin)) & 0x1);
}

void set_i2c_pin(uint8_t pin, uint8_t val) {
  set_i2c_bit(GET_REGISTER(pin, OLATA), GET_PIN(pin), val);
}
