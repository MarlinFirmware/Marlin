/*
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

#include "Marlin.h"

#if ENABLED(EXPERIMENTAL_I2CBUS)

#include "twibus.h"

#include <Wire.h>

TWIBus::TWIBus() {
  #if I2C_SLAVE_ADDRESS == 0
    Wire.begin();                  // No address joins the BUS as the master
  #else
    Wire.begin(I2C_SLAVE_ADDRESS); // Join the bus as a slave
  #endif
  this->reset();
}

void TWIBus::reset() {
  this->buffer_s = 0;
  this->buffer[0] = 0x00;
}

void TWIBus::address(const uint8_t addr) {
  this->addr = addr;

  #if ENABLED(DEBUG_TWIBUS)
    debug(PSTR("address"), this->addr);
  #endif
}

void TWIBus::addbyte(const char c) {
  if (buffer_s >= sizeof(this->buffer)) return;
  this->buffer[this->buffer_s++] = c;

  #if ENABLED(DEBUG_TWIBUS)
    debug(PSTR("addbyte"), this->buffer[this->buffer_s - 1]);
  #endif
}

void TWIBus::send() {
  if (!this->addr) return;

  #if ENABLED(DEBUG_TWIBUS)
    debug(PSTR("send()"));
  #endif

  Wire.beginTransmission(this->addr);
  Wire.write(this->buffer, this->buffer_s);
  Wire.endTransmission();

  // Reset the buffer after sending the data
  this->reset();
}

void TWIBus::reqbytes(const uint8_t bytes) {
  if (!this->addr) return;

  #if ENABLED(DEBUG_TWIBUS)
    debug(PSTR("reqbytes"), bytes);
  #endif

  // requestFrom() is a blocking function
  millis_t t = millis() + this->timeout;
  Wire.requestFrom(this->addr, bytes);
  while (Wire.available() < bytes && PENDING(millis(), t)) { /*nada*/ }

  this->relaydata(bytes);

  // Reset the buffer after sending the data
  this->reset();
}

void TWIBus::relaydata(uint8_t bytes) {
  SERIAL_ECHO_START;
  SERIAL_ECHOPAIR("i2c-reply: from:", this->addr);
  SERIAL_ECHOPAIR(" bytes:", bytes);
  SERIAL_ECHOPGM (" data:");
  while (bytes-- && Wire.available()) SERIAL_CHAR(Wire.read());
  SERIAL_EOL;
}

#if ENABLED(DEBUG_TWIBUS)

  void TWIBus::debug(const char func[], int32_t val/*=-1*/) {
    if (DEBUGGING(INFO)) {
      SERIAL_ECHOPGM("TWIBus::");
      serialprintPGM(func);
      if (val >= 0) SERIAL_ECHOPAIR(": ", val);
      SERIAL_EOL;
    }
  }

#endif

#endif //EXPERIMENTAL_I2CBUS
