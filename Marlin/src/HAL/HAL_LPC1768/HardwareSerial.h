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
  #include <lpc17xx_uart.h>
  #include "lpc17xx_pinsel.h"
}

class HardwareSerial : public Stream {
private:
  LPC_UART_TypeDef *UARTx;

  uint32_t Status;
  uint8_t RxBuffer[RX_BUFFER_SIZE];
  uint32_t RxQueueWritePos;
  uint32_t RxQueueReadPos;
  #if TX_BUFFER_SIZE > 0
    uint8_t TxBuffer[TX_BUFFER_SIZE];
    uint32_t TxQueueWritePos;
    uint32_t TxQueueReadPos;
  #endif

public:
  HardwareSerial(LPC_UART_TypeDef *UARTx)
    : UARTx(UARTx)
    , RxQueueWritePos(0)
    , RxQueueReadPos(0)
    #if TX_BUFFER_SIZE > 0
      , TxQueueWritePos(0)
      , TxQueueReadPos(0)
    #endif
  {
  }

  void begin(uint32_t baudrate);
  int peek();
  int read();
  size_t write(uint8_t send);
  #if TX_BUFFER_SIZE > 0
    void flushTX();
  #endif
  int available();
  void flush();
  void printf(const char *format, ...);

  operator bool() { return true; }

  void IRQHandler();

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

extern HardwareSerial Serial;
extern HardwareSerial Serial1;
extern HardwareSerial Serial2;
extern HardwareSerial Serial3;

#endif // MARLIN_SRC_HAL_HAL_SERIAL_H_
