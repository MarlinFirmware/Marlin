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

//IO方向设置
#define SDA_IN()  do{ PIN_MAP[IIC_EEPROM_SDA].gpio_device->regs->CRH &= 0XFFFF0FFF; PIN_MAP[IIC_EEPROM_SDA].gpio_device->regs->CRH |= 8 << 12; }while(0)
#define SDA_OUT() do{ PIN_MAP[IIC_EEPROM_SDA].gpio_device->regs->CRH &= 0XFFFF0FFF; PIN_MAP[IIC_EEPROM_SDA].gpio_device->regs->CRH |= 3 << 12; }while(0)

//IO操作函数
#define IIC_SCL_0()   WRITE(IIC_EEPROM_SCL, LOW)
#define IIC_SCL_1()   WRITE(IIC_EEPROM_SCL, HIGH)
#define IIC_SDA_0()   WRITE(IIC_EEPROM_SDA, LOW)
#define IIC_SDA_1()   WRITE(IIC_EEPROM_SDA, HIGH)
#define READ_SDA()    READ(IIC_EEPROM_SDA)

class BL24CXX;

// IIC所有操作函数
class IIC {
friend class BL24CXX;
protected:
  static void init();                // 初始化IIC的IO口
  static void start();               // 发送IIC开始信号
  static void stop();                // 发送IIC停止信号
  static void send_byte(uint8_t txd); // IIC发送一个字节
  static uint8_t read_byte(unsigned char ack); // IIC读取一个字节
  static uint8_t wait_ack();         // IIC等待ACK信号
  static void ack();                 // IIC发送ACK信号
  static void nAck();                // IIC不发送ACK信号

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
  static void init(); //初始化IIC
  static uint8_t check();  //检查器件
  static uint8_t readOneByte(uint16_t ReadAddr);                       //指定地址读取一个字节
  static void writeOneByte(uint16_t WriteAddr, uint8_t DataToWrite);   //指定地址写入一个字节
  static void writeLenByte(uint16_t WriteAddr, uint32_t DataToWrite, uint8_t Len);//指定地址开始写入指定长度的数据
  static uint32_t readLenByte(uint16_t ReadAddr, uint8_t Len);         //指定地址开始读取指定长度数据
  static void write(uint16_t WriteAddr, uint8_t *pBuffer, uint16_t NumToWrite);  //从指定地址开始写入指定长度的数据
  static void read(uint16_t ReadAddr, uint8_t *pBuffer, uint16_t NumToRead);     //从指定地址开始读出指定长度的数据
};
