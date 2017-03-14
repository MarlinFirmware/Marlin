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

void TWIBus::address(const uint8_t adr) {
  if (adr < 8 || adr > 127) {
    SERIAL_ECHO_START;
    SERIAL_ECHOLNPGM("Bad I2C address (8-127)");
  }

  this->addr = adr;

  #if ENABLED(DEBUG_TWIBUS)
    debug(PSTR("address"), adr);
  #endif
}

void TWIBus::addbyte(const char c) {
  if (this->buffer_s >= COUNT(this->buffer)) return;
  this->buffer[this->buffer_s++] = c;
  #if ENABLED(DEBUG_TWIBUS)
    debug(PSTR("addbyte"), c);
  #endif
}

void TWIBus::addbytes(char src[], uint8_t bytes) {
  #if ENABLED(DEBUG_TWIBUS)
    debug(PSTR("addbytes"), bytes);
  #endif
  while (bytes--) this->addbyte(*src++);
}

void TWIBus::addstring(char str[]) {
  #if ENABLED(DEBUG_TWIBUS)
    debug(PSTR("addstring"), str);
  #endif
  while (char c = *str++) this->addbyte(c);
}

void TWIBus::send() {
  if (!this->addr) return;

  #if ENABLED(DEBUG_TWIBUS)
    debug(PSTR("send"), this->addr);
  #endif

  Wire.beginTransmission(this->addr);
  Wire.write(this->buffer, this->buffer_s);
  Wire.endTransmission();

  this->reset();
}

// static
void TWIBus::echoprefix(uint8_t bytes, const char prefix[], uint8_t adr) {
  SERIAL_ECHO_START;
  serialprintPGM(prefix);
  SERIAL_ECHOPAIR(": from:", adr);
  SERIAL_ECHOPAIR(" bytes:", bytes);
  SERIAL_ECHOPGM (" data:");
}

// static
void TWIBus::echodata(uint8_t bytes, const char prefix[], uint8_t adr) {
  echoprefix(bytes, prefix, adr);
  while (bytes-- && Wire.available()) SERIAL_CHAR(Wire.read());
  SERIAL_EOL;
}

void TWIBus::echobuffer(const char prefix[], uint8_t adr) {
  echoprefix(this->buffer_s, prefix, adr);
  for (uint8_t i = 0; i < this->buffer_s; i++) SERIAL_CHAR(this->buffer[i]);
  SERIAL_EOL;
}

bool TWIBus::request(const uint8_t bytes) {
  if (!this->addr) return false;

  #if ENABLED(DEBUG_TWIBUS)
    debug(PSTR("request"), bytes);
  #endif

  // requestFrom() is a blocking function
  if (Wire.requestFrom(this->addr, bytes) == 0) {
    #if ENABLED(DEBUG_TWIBUS)
      debug("request fail", this->addr);
    #endif
    return false;
  }

  return true;
}

void TWIBus::relay(const uint8_t bytes) {
  #if ENABLED(DEBUG_TWIBUS)
    debug(PSTR("relay"), bytes);
  #endif

  if (this->request(bytes))
    echodata(bytes, PSTR("i2c-reply"), this->addr);
}

uint8_t TWIBus::capture(char *dst, const uint8_t bytes) {
  this->reset();
  uint8_t count = 0;
  while (count < bytes && Wire.available())
    dst[count++] = Wire.read();

  #if ENABLED(DEBUG_TWIBUS)
    debug(PSTR("capture"), count);
  #endif

  return count;
}

// static
void TWIBus::flush() {
  while (Wire.available()) Wire.read();
}

#if I2C_SLAVE_ADDRESS > 0

  void TWIBus::receive(uint8_t bytes) {
    #if ENABLED(DEBUG_TWIBUS)
      debug(PSTR("receive"), bytes);
    #endif
    echodata(bytes, PSTR("i2c-receive"), 0);
  }

  void TWIBus::reply(char str[]/*=NULL*/) {
    #if ENABLED(DEBUG_TWIBUS)
      debug(PSTR("reply"), str);
    #endif

    if (str) {
      this->reset();
      this->addstring(str);
    }

    Wire.write(this->buffer, this->buffer_s);

    this->reset();
  }

#endif

#if ENABLED(DEBUG_TWIBUS)

  // static
  void TWIBus::prefix(const char func[]) {
    SERIAL_ECHOPGM("TWIBus::");
    serialprintPGM(func);
    SERIAL_ECHOPGM(": ");
  }
  void TWIBus::debug(const char func[], uint32_t adr) {
    if (DEBUGGING(INFO)) { prefix(func); SERIAL_ECHOLN(adr); }
  }
  void TWIBus::debug(const char func[], char c) {
    if (DEBUGGING(INFO)) { prefix(func); SERIAL_ECHOLN(c); }
  }
  void TWIBus::debug(const char func[], char str[]) {
    if (DEBUGGING(INFO)) { prefix(func); SERIAL_ECHOLN(str); }
  }

#endif

#endif //EXPERIMENTAL_I2CBUS
