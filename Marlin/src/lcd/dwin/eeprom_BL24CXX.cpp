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

/********************************************************************************
 * @file     eeprom_BL24CXX.cpp
 * @brief    i2c EEPROM for Ender 3 v2 board (4.2.2)
 ********************************************************************************/

#include "../../inc/MarlinConfig.h"

#if ENABLED(IIC_BL24CXX_EEPROM)

#include "eeprom_BL24CXX.h"
#include "../../MarlinCore.h"

#include <stdlib.h>

/******************** IIC ********************/

// 初始化IIC
void IIC::init() {
  SET_OUTPUT(IIC_EEPROM_SDA);
  SET_OUTPUT(IIC_EEPROM_SCL);

  IIC_SCL_1();
  IIC_SDA_1();
}

// 产生IIC起始信号
void IIC::start() {
  SDA_OUT();      // sda线输出
  IIC_SDA_1();
  IIC_SCL_1();
  delay_us(4);
  IIC_SDA_0(); // START:when CLK is high, DATA change form high to low
  delay_us(4);
  IIC_SCL_0(); // 钳住I2C总线，准备发送或接收数据
}

// 产生IIC停止信号
void IIC::stop() {
  SDA_OUT(); // sda线输出
  IIC_SCL_0();
  IIC_SDA_0(); // STOP:when CLK is high DATA change form low to high
  delay_us(4);
  IIC_SCL_1();
  IIC_SDA_1(); // 发送I2C总线结束信号
  delay_us(4);
}

// 等待应答信号到来
// 返回值：1，接收应答失败
//         0，接收应答成功
uint8_t IIC::wait_ack() {
  uint8_t ucErrTime=0;
  SDA_IN();      // SDA设置为输入
  IIC_SDA_1();delay_us(1);
  IIC_SCL_1();delay_us(1);
  while (READ_SDA()) {
    ucErrTime++;
    if (ucErrTime>250) {
      stop();
      return 1;
    }
  }
  IIC_SCL_0(); // 时钟输出0
  return 0;
}

// 产生ACK应答
void IIC::ack() {
  IIC_SCL_0();
  SDA_OUT();
  IIC_SDA_0();
  delay_us(2);
  IIC_SCL_1();
  delay_us(2);
  IIC_SCL_0();
}

// 不产生ACK应答
void IIC::nAck() {
  IIC_SCL_0();
  SDA_OUT();
  IIC_SDA_1();
  delay_us(2);
  IIC_SCL_1();
  delay_us(2);
  IIC_SCL_0();
}

// IIC发送一个字节
// 返回从机有无应答
// 1，有应答
// 0，无应答
void IIC::send_byte(uint8_t txd) {
  SDA_OUT();
  IIC_SCL_0(); // 拉低时钟开始数据传输
  LOOP_L_N(t, 8) {
    // IIC_SDA = (txd & 0x80) >> 7;
    if (txd & 0x80) IIC_SDA_1(); else IIC_SDA_0();
    txd <<= 1;
    delay_us(2);   // 对TEA5767这三个延时都是必须的
    IIC_SCL_1();
    delay_us(2);
    IIC_SCL_0();
    delay_us(2);
  }
}

// 读1个字节，ack=1时，发送ACK，ack=0，发送nACK
uint8_t IIC::read_byte(unsigned char ack_chr) {
  unsigned char receive = 0;
  SDA_IN(); // SDA设置为输入
  LOOP_L_N(i, 8) {
    IIC_SCL_0();
    delay_us(2);
    IIC_SCL_1();
    receive <<= 1;
    if (READ_SDA()) receive++;
    delay_us(1);
  }
  ack_chr ? ack() : nAck(); // 发送ACK / 发送nACK
  return receive;
}

/******************** EEPROM ********************/

// 初始化IIC接口
void BL24CXX::init() { IIC::init(); }

// 在BL24CXX指定地址读出一个数据
// ReadAddr:开始读数的地址
// 返回值  :读到的数据
uint8_t BL24CXX::readOneByte(uint16_t ReadAddr) {
  uint8_t temp = 0;
  IIC::start();
  if (EE_TYPE > BL24C16) {
    IIC::send_byte(0xA0);     // 发送写命令
    IIC::wait_ack();
    IIC::send_byte(ReadAddr >> 8); // 发送高地址
    IIC::wait_ack();
  }
  else
    IIC::send_byte(0xA0 + ((ReadAddr >> 8) << 1));   // 发送器件地址0xA0,写数据

  IIC::wait_ack();
  IIC::send_byte(ReadAddr & 0xFF); // 发送低地址
  IIC::wait_ack();
  IIC::start();
  IIC::send_byte(0xA1);           // 进入接收模式
  IIC::wait_ack();
  temp = IIC::read_byte(0);
  IIC::stop(); // 产生一个停止条件
  return temp;
}

// 在BL24CXX指定地址写入一个数据
// WriteAddr  :写入数据的目的地址
// DataToWrite:要写入的数据
void BL24CXX::writeOneByte(uint16_t WriteAddr, uint8_t DataToWrite) {
  IIC::start();
  if (EE_TYPE > BL24C16) {
    IIC::send_byte(0xA0);      // 发送写命令
    IIC::wait_ack();
    IIC::send_byte(WriteAddr >> 8); // 发送高地址
  }
  else {
    IIC::send_byte(0xA0 + ((WriteAddr >> 8) << 1));   // 发送器件地址0xA0,写数据
  }
  IIC::wait_ack();
  IIC::send_byte(WriteAddr & 0xFF); // 发送低地址
  IIC::wait_ack();
  IIC::send_byte(DataToWrite);     // 发送字节
  IIC::wait_ack();
  IIC::stop(); // 产生一个停止条件
  delay(10);
}

// 在BL24CXX里面的指定地址开始写入长度为Len的数据
// 该函数用于写入16bit或者32bit的数据.
// WriteAddr  :开始写入的地址
// DataToWrite:数据数组首地址
// Len        :要写入数据的长度2,4
void BL24CXX::writeLenByte(uint16_t WriteAddr, uint32_t DataToWrite, uint8_t Len) {
  LOOP_L_N(t, Len)
    writeOneByte(WriteAddr + t, (DataToWrite >> (8 * t)) & 0xFF);
}

// 在BL24CXX里面的指定地址开始读出长度为Len的数据
// 该函数用于读出16bit或者32bit的数据.
// ReadAddr   :开始读出的地址
// 返回值     :数据
// Len        :要读出数据的长度2,4
uint32_t BL24CXX::readLenByte(uint16_t ReadAddr, uint8_t Len) {
  uint32_t temp = 0;
  LOOP_L_N(t, Len) {
    temp <<= 8;
    temp += readOneByte(ReadAddr + Len - t - 1);
  }
  return temp;
}

// 检查BL24CXX是否正常
// 这里用了24XX的最后一个地址(255)来存储标志字.
// 如果用其他24C系列,这个地址要修改
// 返回1:检测失败
// 返回0:检测成功
uint8_t BL24CXX::check() {
  uint8_t temp;
  temp = readOneByte(255); // 避免每次开机都写BL24CXX
  if (temp == 'U') return 0;
  else { // 排除第一次初始化的情况
    writeOneByte(255, 'U');
    temp = readOneByte(255);
    if (temp == 'U') return 0;
  }
  return 1;
}

// 在BL24CXX里面的指定地址开始读出指定个数的数据
// ReadAddr :开始读出的地址 对24c02为0~255
// pBuffer  :数据数组首地址
// NumToRead:要读出数据的个数
void BL24CXX::read(uint16_t ReadAddr, uint8_t *pBuffer, uint16_t NumToRead) {
  while (NumToRead) {
    *pBuffer++ = readOneByte(ReadAddr++);
    NumToRead--;
  }
}

// 在BL24CXX里面的指定地址开始写入指定个数的数据
// WriteAddr :开始写入的地址 对24c02为0~255
// pBuffer   :数据数组首地址
// NumToWrite:要写入数据的个数
void BL24CXX::write(uint16_t WriteAddr, uint8_t *pBuffer, uint16_t NumToWrite) {
  while (NumToWrite--) {
    writeOneByte(WriteAddr, *pBuffer);
    WriteAddr++;
    pBuffer++;
  }
}

#endif // IIC_BL24CXX_EEPROM
