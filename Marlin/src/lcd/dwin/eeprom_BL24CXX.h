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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

/********************************************************************************
 * @file     eeprom_BL24CXX.h
 * @brief    i2c EEPROM for Ender 3 v2 board (4.2.2)
 ********************************************************************************/

#include <libmaple/gpio.h>

/******************** IIC ********************/

//IO direction setting
#define SDA_IN()  do{ PIN_MAP[IIC_EEPROM_SDA].gpio_device->regs->CRH &= 0XFFFF0FFF; PIN_MAP[IIC_EEPROM_SDA].gpio_device->regs->CRH |= 8 << 12; }while(0)
#define SDA_OUT() do{ PIN_MAP[IIC_EEPROM_SDA].gpio_device->regs->CRH &= 0XFFFF0FFF; PIN_MAP[IIC_EEPROM_SDA].gpio_device->regs->CRH |= 3 << 12; }while(0)

//IO operation function
#define IIC_SCL_0()   WRITE(IIC_EEPROM_SCL, LOW)
#define IIC_SCL_1()   WRITE(IIC_EEPROM_SCL, HIGH)
#define IIC_SDA_0()   WRITE(IIC_EEPROM_SDA, LOW)
#define IIC_SDA_1()   WRITE(IIC_EEPROM_SDA, HIGH)
#define READ_SDA()    READ(IIC_EEPROM_SDA)

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

  static void write_one_byte(uint8_t daddr, uint8_t addr, uint8_t data);
  static uint8_t read_one_byte(uint8_t daddr, uint8_t addr);
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
public:
  static void init(); // Initialize IIC
  static uint8_t check();  // Check the device
  static uint8_t readOneByte(uint16_t ReadAddr);                       // Read a byte at the specified address
  static void writeOneByte(uint16_t WriteAddr, uint8_t DataToWrite);   // Write a byte at the specified address
  static void writeLenByte(uint16_t WriteAddr, uint32_t DataToWrite, uint8_t Len);// The specified address begins to write the data of the specified length
  static uint32_t readLenByte(uint16_t ReadAddr, uint8_t Len);         // The specified address starts to read the data of the specified length
  static void write(uint16_t WriteAddr, uint8_t *pBuffer, uint16_t NumToWrite);  // Write the specified length of data from the specified address
  static void read(uint16_t ReadAddr, uint8_t *pBuffer, uint16_t NumToRead);     // Read the data of the specified length from the specified address
};
