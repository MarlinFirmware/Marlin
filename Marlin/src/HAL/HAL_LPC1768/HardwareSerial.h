/**
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

#ifndef HARDWARE_SERIAL_H_
#define HARDWARE_SERIAL_H_

#include <stdarg.h>
#include <stdio.h>
#include <Stream.h>

extern "C" {
  #include <debug_frmwrk.h>
  //#include <lpc17xx_uart.h>
}

#define IER_RBR   0x01
#define IER_THRE  0x02
#define IER_RLS   0x04

#define IIR_PEND  0x01
#define IIR_RLS   0x03
#define IIR_RDA   0x02
#define IIR_CTI   0x06
#define IIR_THRE  0x01

#define LSR_RDR   0x01
#define LSR_OE    0x02
#define LSR_PE    0x04
#define LSR_FE    0x08
#define LSR_BI    0x10
#define LSR_THRE  0x20
#define LSR_TEMT  0x40
#define LSR_RXFE  0x80

#define UARTRXQUEUESIZE   0x10

class HardwareSerial : public Stream {
private:
uint8_t PortNum;
uint32_t baudrate;

public:
  HardwareSerial(uint32_t uart) :
    PortNum(uart)
    {
    }

  void begin(uint32_t baudrate);
  int read();
  size_t write(uint8_t send);
  int available();
  void flush();
  void printf(const char *format, ...);
  int peek() {
    return 0;
  };

  operator bool() { return true; }

  void print(const char value[])              { printf("%s" , value); }
  void print(char value, int = 0)             { printf("%c" , value); }
  void print(unsigned char value, int = 0)    { printf("%u" , value); }
  void print(int value, int = 0)              { printf("%d" , value); }
  void print(unsigned int value, int = 0)     { printf("%u" , value); }
  void print(long value, int = 0)             { printf("%ld" , value); }
  void print(unsigned long value, int = 0)    { printf("%lu" , value); }

  void print(float value, int round = 6)      { printf("%f" , value); }
  void print(double value, int round = 6)     { printf("%f" , value ); }

  void println(const char value[])            { printf("%s\n" , value); }
  void println(char value, int = 0)           { printf("%c\n" , value); }
  void println(unsigned char value, int = 0)  { printf("%u\r\n" , value); }
  void println(int value, int = 0)            { printf("%d\n" , value); }
  void println(unsigned int value, int = 0)   { printf("%u\n" , value); }
  void println(long value, int = 0)           { printf("%ld\n" , value); }
  void println(unsigned long value, int = 0)  { printf("%lu\n" , value); }
  void println(float value, int round = 6)    { printf("%f\n" , value ); }
  void println(double value, int round = 6)   { printf("%f\n" , value ); }
  void println(void)                          { print('\n'); }

};

//extern HardwareSerial Serial0;
//extern HardwareSerial Serial1;
//extern HardwareSerial Serial2;
extern HardwareSerial Serial3;

#endif // MARLIN_SRC_HAL_HAL_SERIAL_H_
