/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <Wire.h>
#include "fastio_ESP32.h"

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
