/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

/**
 * Platform-independent Arduino functions for I2C EEPROM.
 * Enable USE_SHARED_EEPROM if not supplied by the framework.
 */

#include "../../inc/MarlinConfig.h"

#if ENABLED(I2C_EEPROM)

#include "eeprom_if.h"

#if ENABLED(SOFT_I2C_EEPROM)
  #include <SlowSoftWire.h>
  SlowSoftWire Wire = SlowSoftWire(I2C_SDA_PIN, I2C_SCL_PIN, true);
#else
  #include <Wire.h>
#endif

void eeprom_init() {
  Wire.begin(
    #if PINS_EXIST(I2C_SCL, I2C_SDA) && DISABLED(SOFT_I2C_EEPROM)
      uint8_t(I2C_SDA_PIN), uint8_t(I2C_SCL_PIN)
    #endif
  );
}

#if ENABLED(USE_SHARED_EEPROM)

#ifndef EEPROM_WRITE_DELAY
  #define EEPROM_WRITE_DELAY    5
#endif
#ifndef EEPROM_DEVICE_ADDRESS
  #define EEPROM_DEVICE_ADDRESS  0x50
#endif

static constexpr uint8_t eeprom_device_address = I2C_ADDRESS(EEPROM_DEVICE_ADDRESS);

// ------------------------
// Public functions
// ------------------------

#define SMALL_EEPROM (MARLIN_EEPROM_SIZE <= 2048)

// Combine Address high bits into the device address on <=16Kbit (2K) and >512Kbit (64K) EEPROMs.
// Note: MARLIN_EEPROM_SIZE is specified in bytes, whereas EEPROM model numbers refer to bits.
//       e.g., The "16" in BL24C16 indicates a 16Kbit (2KB) size.
static uint8_t _eeprom_calc_device_address(uint8_t * const pos) {
  const unsigned eeprom_address = (unsigned)pos;
  return (SMALL_EEPROM || MARLIN_EEPROM_SIZE > 65536)
    ? uint8_t(eeprom_device_address | ((eeprom_address >> (SMALL_EEPROM ? 8 : 16)) & 0x07))
    : eeprom_device_address;
}

static void _eeprom_begin(uint8_t * const pos) {
  const unsigned eeprom_address = (unsigned)pos;
  Wire.beginTransmission(_eeprom_calc_device_address(pos));
  if (!SMALL_EEPROM)
    Wire.write(uint8_t((eeprom_address >> 8) & 0xFF));  // Address High, if needed
  Wire.write(uint8_t(eeprom_address & 0xFF));           // Address Low
}

void eeprom_write_byte(uint8_t *pos, uint8_t value) {
  _eeprom_begin(pos);
  Wire.write(value);
  Wire.endTransmission();

  // wait for write cycle to complete
  // this could be done more efficiently with "acknowledge polling"
  delay(EEPROM_WRITE_DELAY);
}

uint8_t eeprom_read_byte(uint8_t *pos) {
  _eeprom_begin(pos);
  Wire.endTransmission();
  Wire.requestFrom(_eeprom_calc_device_address(pos), (byte)1);
  return Wire.available() ? Wire.read() : 0xFF;
}

#endif // USE_SHARED_EEPROM
#endif // I2C_EEPROM
