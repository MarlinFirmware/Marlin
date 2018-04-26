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

#include "../../inc/MarlinConfigPre.h"
#if ENABLED(EMERGENCY_PARSER)
  #include "../../feature/emergency_parser.h"
#endif

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

  uint32_t Baudrate;
  uint32_t Status;
  uint8_t RxBuffer[RX_BUFFER_SIZE];
  uint32_t RxQueueWritePos;
  uint32_t RxQueueReadPos;
  #if TX_BUFFER_SIZE > 0
    uint8_t TxBuffer[TX_BUFFER_SIZE];
    uint32_t TxQueueWritePos;
    uint32_t TxQueueReadPos;
  #endif
  #if ENABLED(EMERGENCY_PARSER)
    EmergencyParser::State emergency_state;
  #endif

public:
  HardwareSerial(LPC_UART_TypeDef *UARTx)
    : UARTx(UARTx)
    , Baudrate(0)
    , RxQueueWritePos(0)
    , RxQueueReadPos(0)
    #if TX_BUFFER_SIZE > 0
      , TxQueueWritePos(0)
      , TxQueueReadPos(0)
    #endif
    #if ENABLED(EMERGENCY_PARSER)
      , emergency_state(EmergencyParser::State::EP_RESET)
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

  #define DEC 10
  #define HEX 16
  #define OCT 8
  #define BIN 2

  void print_bin(uint32_t value, uint8_t num_digits) {
    uint32_t mask = 1 << (num_digits -1);
    for (uint8_t i = 0; i < num_digits; i++) {
      if (!(i % 4) && i)    printf(" ");
      if (!(i % 16)  && i)  printf(" ");
      if (value & mask)     printf("1");
      else                  printf("0");
      value <<= 1;
    }
  }

  void print(const char value[]) {
    printf("%s" , value);
  }
  void print(char value, int nbase = 0) {
    if (nbase == BIN) print_bin(value,8);
    else if (nbase == OCT) printf("%3o", value);
    else if (nbase == HEX) printf("%2X", value);
    else if (nbase == DEC ) printf("%d", value);
    else printf("%c" , value);
  }
  void print(unsigned char value, int nbase = 0) {
    if (nbase == BIN) print_bin(value,8);
    else if (nbase == OCT) printf("%3o", value);
    else if (nbase == HEX) printf("%2X", value);
    else printf("%u" , value);
  }
  void print(int value, int nbase = 0) {
    if (nbase == BIN) print_bin(value,16);
    else if (nbase == OCT) printf("%6o", value);
    else if (nbase == HEX) printf("%4X", value);
    else printf("%d", value);
  }
  void print(unsigned int value, int nbase = 0) {
    if (nbase == BIN) print_bin(value,16);
    else if (nbase == OCT) printf("%6o", value);
    else if (nbase == HEX) printf("%4X", value);
    else printf("%u" , value);
  }
  void print(long value, int nbase = 0) {
    if (nbase == BIN) print_bin(value,32);
    else if (nbase == OCT) printf("%11o", value);
    else if (nbase == HEX) printf("%8X", value);
    else printf("%ld" , value);
  }
  void print(unsigned long value, int nbase = 0) {
    if (nbase == BIN) print_bin(value,32);
    else if (nbase == OCT) printf("%11o", value);
    else if (nbase == HEX) printf("%8X", value);
    else printf("%lu" , value);
  }
  void print(float value, int round = 6) {
    printf("%f" , value);
  }
  void print(double value, int round = 6) {
    printf("%f" , value );
  }

  void println(const char value[]) {
    printf("%s\n" , value);
  }
  void println(char value, int nbase = 0) {
    print(value, nbase);
    println();
  }
  void println(unsigned char value, int nbase = 0) {
    print(value, nbase);
    println();
  }
  void println(int value, int nbase = 0) {
    print(value, nbase);
    println();
  }
  void println(unsigned int value, int nbase = 0) {
    print(value, nbase);
    println();
  }
  void println(long value, int nbase = 0) {
    print(value, nbase);
    println();
  }
  void println(unsigned long value, int nbase = 0) {
    print(value, nbase);
    println();
  }
  void println(float value, int round = 6) {
    printf("%f\n" , value );
  }
  void println(double value, int round = 6) {
    printf("%f\n" , value );
  }
  void println(void) {
    print('\n');
  }

};

#endif // MARLIN_SRC_HAL_HAL_SERIAL_H_
