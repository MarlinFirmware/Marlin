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

#include "../inc/MarlinConfig.h"

#if ENABLED(EXPERIMENTAL_I2CBUS)

#include "twibus.h"

#include <Wire.h>

TWIBus i2c;

TWIBus::TWIBus() {
  #if I2C_SLAVE_ADDRESS == 0
    Wire.begin();                  // No address joins the BUS as the master
  #else
    Wire.begin(I2C_SLAVE_ADDRESS); // Join the bus as a slave
  #endif
  reset();
}

void TWIBus::reset() {
  buffer_s = 0;
  buffer[0] = 0x00;
}

void TWIBus::address(const uint8_t adr) {
  if (!WITHIN(adr, 8, 127)) {
    SERIAL_ECHO_MSG("Bad I2C address (8-127)");
  }

  addr = adr;

  debug(PSTR("address"), adr);
}

void TWIBus::addbyte(const char c) {
  if (buffer_s >= COUNT(buffer)) return;
  buffer[buffer_s++] = c;
  debug(PSTR("addbyte"), c);
}

void TWIBus::addbytes(char src[], uint8_t bytes) {
  debug(PSTR("addbytes"), bytes);
  while (bytes--) addbyte(*src++);
}

void TWIBus::addstring(char str[]) {
  debug(PSTR("addstring"), str);
  while (char c = *str++) addbyte(c);
}

void TWIBus::send() {
  debug(PSTR("send"), addr);

  Wire.beginTransmission(I2C_ADDRESS(addr));
  Wire.write(buffer, buffer_s);
  Wire.endTransmission();

  reset();
}

// static
void TWIBus::echoprefix(uint8_t bytes, const char pref[], uint8_t adr) {
  SERIAL_ECHO_START();
  SERIAL_ECHOPGM_P(pref);
  SERIAL_ECHOPAIR(": from:", adr, " bytes:", bytes, " data:");
}

// static
void TWIBus::echodata(uint8_t bytes, const char pref[], uint8_t adr) {
  echoprefix(bytes, pref, adr);
  while (bytes-- && Wire.available()) SERIAL_CHAR(Wire.read());
  SERIAL_EOL();
}

void TWIBus::echobuffer(const char pref[], uint8_t adr) {
  echoprefix(buffer_s, pref, adr);
  LOOP_L_N(i, buffer_s) SERIAL_CHAR(buffer[i]);
  SERIAL_EOL();
}

bool TWIBus::request(const uint8_t bytes) {
  if (!addr) return false;

  debug(PSTR("request"), bytes);

  // requestFrom() is a blocking function
  if (Wire.requestFrom(I2C_ADDRESS(addr), bytes) == 0) {
    debug("request fail", I2C_ADDRESS(addr));
    return false;
  }

  return true;
}

void TWIBus::relay(const uint8_t bytes) {
  debug(PSTR("relay"), bytes);

  if (request(bytes))
    echodata(bytes, PSTR("i2c-reply"), addr);
}

uint8_t TWIBus::capture(char *dst, const uint8_t bytes) {
  reset();
  uint8_t count = 0;
  while (count < bytes && Wire.available())
    dst[count++] = Wire.read();

  debug(PSTR("capture"), count);

  return count;
}

// static
void TWIBus::flush() {
  while (Wire.available()) Wire.read();
}

#if I2C_SLAVE_ADDRESS > 0

  void TWIBus::receive(uint8_t bytes) {
    debug(PSTR("receive"), bytes);
    echodata(bytes, PSTR("i2c-receive"), 0);
  }

  void TWIBus::reply(char str[]/*=nullptr*/) {
    debug(PSTR("reply"), str);

    if (str) {
      reset();
      addstring(str);
    }

    Wire.write(buffer, buffer_s);

    reset();
  }

  void i2c_on_receive(int bytes) { // just echo all bytes received to serial
    i2c.receive(bytes);
  }

  void i2c_on_request() {          // just send dummy data for now
    i2c.reply("Hello World!\n");
  }

#endif

#if ENABLED(DEBUG_TWIBUS)

  // static
  void TWIBus::prefix(const char func[]) {
    SERIAL_ECHOPGM("TWIBus::");
    SERIAL_ECHOPGM_P(func);
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

#endif // EXPERIMENTAL_I2CBUS
