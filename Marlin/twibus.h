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

#ifndef TWIBUS_H
#define TWIBUS_H

#include "macros.h"

// Print debug messages with M111 S2 (Uses 236 bytes of PROGMEM)
//#define DEBUG_TWIBUS

/**
 * TWIBUS class
 *
 * This class implements a wrapper around the two wire (I2C) bus, it allows
 * Marlin to send and request data from any slave device on the bus. This is
 * an experimental feature and it's inner workings as well as public facing
 * interface are prune to change in the future.
 *
 * The two main consumers of this class are M155 and M156, where M155 allows
 * Marlin to send a I2C packet to a device (please be aware that no repeated
 * starts are possible), this can be done in caching method by calling multiple
 * times M155 B<byte-1 value in base 10> or a one liner M155, have a look at
 * the gcode_M155() function for more information. M156 allows Marlin to
 * request data from a device, the received data is then relayed into the serial
 * line for host interpretation.
 *
 */
class TWIBus {
  private:
    /**
     * @brief Timeout value in milliseconds
     * @details For blocking operations this constant value will set the max
     * amount of time Marlin will keep waiting for a reply. Useful is something
     * goes wrong on the bus and the SDA/SCL lines are held up by another device.
     */
    const int timeout = 5;

    /**
     * @brief Target device address
     * @description This stores, until the buffer is flushed, the target device
     * address, take not we do follow Arduino 7bit addressing.
     */
    uint8_t addr = 0;

    /**
     * @brief Number of bytes on buffer
     * @description This var holds the total number of bytes on our buffer
     * waiting to be flushed to the bus.
     */
    uint8_t buffer_s = 0;

    /**
     * @brief Internal buffer
     * @details This is a fixed buffer, TWI command cannot be longer than this
     */
    char buffer[30];


  public:
    /**
     * @brief Class constructor
     * @details Initialized the TWI bus and clears the buffer
     */
    TWIBus();

    /**
     * @brief Reset the buffer
     * @details Brings the internal buffer to a known-empty state
     */
    void reset();

    /**
     * @brief Send the buffer data to the bus
     * @details Flushed the buffer into the bus targeting the cached slave device
     * address.
     */
    void send();

    /**
     * @brief Add one byte to the buffer
     * @details Adds the byte to the buffer in a sequential way, if buffer is full
     * the request is silently ignored.
     *
     * @param c a data byte
     */
    void addbyte(char c);

    /**
     * @brief Sets the target slave address
     * @details The target slave address is stored so it can be later used when
     * the complete packet needs to be sent over the bus.
     *
     * @param addr 7-bit integer address
     */
    void address(uint8_t addr);

    /**
     * @brief Request data from slave device
     * @details Requests data from a slave device, when the data is received it will
     * be relayed to the serial line using a parser-friendly formatting.
     *
     * @param bytes the number of bytes to request
     */
    void reqbytes(uint8_t bytes);

    #if ENABLED(DEBUG_TWIBUS)

      /**
       * @brief Prints a debug message
       * @details Prints a simple debug message "TWIBus::function: value"
       */
      static void debug(const char func[], int32_t val = -1);

    #endif
};

#endif //TWIBUS_H
