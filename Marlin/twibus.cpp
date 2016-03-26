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
#include "twibus.h"

#include <Wire.h>

/**
 * @brief Class constructor
 * @details Initialized the TWI bus and clears the buffer
 */
twibus::twibus() {
    Wire.begin(); // We use no address so we will join the BUS as the master
    this->reset();
}

/**
 * @brief Reset the buffer
 * @details Brings the internal buffer to a known-empty state
 */
void twibus::reset() {
    this->addr = 0;
    this->buffer_s = 0;
    this->buffer[0] = 0x00;
}

/**
 * @brief Sets the target slave address
 * @details The target slave address is stored so it can be later used when
 * the complete packet needs to be sent over the bus.
 *
 * @param addr 7-bit integer address
 */
void twibus::address(uint8_t addr) {
    this->addr = addr;

    if (marlin_debug_flags & DEBUG_INFO) {
        SERIAL_ECHOPAIR("twibus::sendto: ", this->addr);
        SERIAL_EOL;
    }
}

/**
 * @brief Add one byte to the buffer
 * @details Adds the byte to the buffer in a sequential way, if buffer is full
 * the request is silently ignored.
 *
 * @param c a data byte
 */
void twibus::addbyte(char c) {
    if (buffer_s >= sizeof(this->buffer)) return;
    this->buffer[this->buffer_s++] = c;

    if (marlin_debug_flags & DEBUG_INFO) {
        SERIAL_ECHOPAIR("twibus::addbyte: ", this->buffer[this->buffer_s -1]);
        SERIAL_EOL;
    }
}

/**
 * @brief Send the buffer data to the bus
 * @details Flushed the buffer into the bus targeting the cached slave device
 * address.
 */
void twibus::send() {
    if (!this->addr) return;
    if (marlin_debug_flags & DEBUG_INFO)
        SERIAL_ECHOLNPGM("twibus::send()");

    Wire.beginTransmission(this->addr);
    Wire.write(this->buffer, this->buffer_s);
    Wire.endTransmission();

    // Reset the buffer after sending the data
    this->reset();
}

/**
 * @brief Request data from slave device
 * @details Requests data from a slave device, when the data is received it will
 * be relayed to the serial line using a parser-friendly formatting.
 *
 * @param bytes the number of bytes to request
 */
void twibus::reqbytes(uint8_t bytes) {
    if (!this->addr) return;
    if (marlin_debug_flags & DEBUG_INFO) {
        SERIAL_ECHOPAIR("twibus::reqbytes(): ", bytes);
        SERIAL_EOL;
    }

    millis_t t = millis();
    Wire.requestFrom(this->addr, bytes);

    // requestFrom() is a blocking function
    while (Wire.available() < bytes) {
        if (millis() - t >= this->timeout) break;
        else continue;
    }

    SERIAL_ECHO_START;
    SERIAL_ECHOPAIR("I2C-reply: from:", this->addr);
    SERIAL_ECHOPAIR(" bytes:", Wire.available());
    SERIAL_ECHOPGM (" data:");

    // Protect against buffer overflows if the number of received bytes
    // is less than the number of requested bytes
    uint8_t wba = Wire.available();
    for (int i = 0; i < wba; i++) SERIAL_CHAR(Wire.read());
    SERIAL_EOL;

    // Reset the buffer after sending the data
    this->reset();
}
