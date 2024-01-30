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
#pragma once

/********************************************************************************
 * @file     BL24CXX.h
 * @brief    i2c EEPROM for Ender-3 v2 board (4.2.2)
 ********************************************************************************/

/******************** IIC ********************/

class BL24CXX;

// All operation functions of IIC
class IIC {
friend class BL24CXX;
protected:
  static void init();                // Initialize the IO port of IIC
  static void start();               // Send IIC start signal
  static void stop();                // Send IIC stop signal
  static void send_byte(uint8_t txd); // IIC sends a byte
  static uint8_t read_byte(unsigned char ack); // IIC reads a byte
  static uint8_t wait_ack();         // IIC waits for ACK signal
  static void ack();                 // IIC sends ACK signal
  static void nAck();                // IIC does not send ACK signal
};

/******************** EEPROM ********************/

#define BL24C01     127
#define BL24C02     255
#define BL24C04     511
#define BL24C08    1023
#define BL24C16    2047
#define BL24C32    4095
#define BL24C64    8191
#define BL24C128  16383
#define BL24C256  32767
#define EE_TYPE BL24C16

class BL24CXX {
private:
  static bool _check();                                                             // Check the device
public:
  static void init();                                                               // Initialize IIC
  static bool check();                                                              // Check / recheck the device
  static uint8_t readOneByte(uint16_t ReadAddr);                                    // Read a byte at the specified address
  static void writeOneByte(uint16_t WriteAddr, uint8_t DataToWrite);                // Write a byte at the specified address
  static void writeLenByte(uint16_t WriteAddr, uint32_t DataToWrite, uint8_t Len);  // The specified address begins to write the data of the specified length
  static uint32_t readLenByte(uint16_t ReadAddr, uint8_t Len);                      // The specified address starts to read the data of the specified length
  static void write(uint16_t WriteAddr, uint8_t *pBuffer, uint16_t NumToWrite);     // Write the specified length of data from the specified address
  static void read(uint16_t ReadAddr, uint8_t *pBuffer, uint16_t NumToRead);        // Read the data of the specified length from the specified address
};
