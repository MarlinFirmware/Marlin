/*
  anycubic_serial.h  --- Support for Anycubic i3 Mega TFT serial connection
  Created by Christian Hopp on 09.12.17.

  Original file:

  HardwareSerial.h - Hardware serial library for Wiring
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Modified 28 September 2010 by Mark Sproul
  Modified 14 August 2012 by Alarus
*/
#pragma once

#include <inttypes.h>
#include <avr/pgmspace.h>

#include "Stream.h"

#define FORCE_INLINE __attribute__((always_inline)) inline

struct ring_buffer;

class AnycubicSerialClass : public Stream {
  private:
    ring_buffer *_rx_buffer;
    ring_buffer *_tx_buffer;
    volatile uint8_t *_ubrrh;
    volatile uint8_t *_ubrrl;
    volatile uint8_t *_ucsra;
    volatile uint8_t *_ucsrb;
    volatile uint8_t *_ucsrc;
    volatile uint8_t *_udr;
    uint8_t _rxen;
    uint8_t _txen;
    uint8_t _rxcie;
    uint8_t _udrie;
    uint8_t _u2x;
    bool transmitting;
  public:
    AnycubicSerialClass(ring_buffer *rx_buffer, ring_buffer *tx_buffer,
      volatile uint8_t *ubrrh, volatile uint8_t *ubrrl,
      volatile uint8_t *ucsra, volatile uint8_t *ucsrb,
      volatile uint8_t *ucsrc, volatile uint8_t *udr,
      uint8_t rxen, uint8_t txen, uint8_t rxcie, uint8_t udrie, uint8_t u2x
    );
    void begin(unsigned long);
    void begin(unsigned long, uint8_t);
    void end();
    virtual int available(void);
    virtual int peek(void);
    virtual int read(void);
    virtual void flush(void);
    virtual size_t write(uint8_t);
    inline size_t write(unsigned long n) { return write((uint8_t)n); }
    inline size_t write(long n) { return write((uint8_t)n); }
    inline size_t write(unsigned int n) { return write((uint8_t)n); }
    inline size_t write(int n) { return write((uint8_t)n); }
    using Print::write; // pull in write(str) and write(buf, size) from Print
    operator bool();
};

// Define config for Serial.begin(baud, config);
#define SERIAL_5N1 0x00
#define SERIAL_6N1 0x02
#define SERIAL_7N1 0x04
#define SERIAL_8N1 0x06
#define SERIAL_5N2 0x08
#define SERIAL_6N2 0x0A
#define SERIAL_7N2 0x0C
#define SERIAL_8N2 0x0E
#define SERIAL_5E1 0x20
#define SERIAL_6E1 0x22
#define SERIAL_7E1 0x24
#define SERIAL_8E1 0x26
#define SERIAL_5E2 0x28
#define SERIAL_6E2 0x2A
#define SERIAL_7E2 0x2C
#define SERIAL_8E2 0x2E
#define SERIAL_5O1 0x30
#define SERIAL_6O1 0x32
#define SERIAL_7O1 0x34
#define SERIAL_8O1 0x36
#define SERIAL_5O2 0x38
#define SERIAL_6O2 0x3A
#define SERIAL_7O2 0x3C
#define SERIAL_8O2 0x3E

extern void serialEventRun(void) __attribute__((weak));

#define ANYCUBIC_SERIAL_PROTOCOL(x) (AnycubicSerial.print(x))
#define ANYCUBIC_SERIAL_PROTOCOL_F(x,y) (AnycubicSerial.print(x,y))
#define ANYCUBIC_SERIAL_PROTOCOLPGM(x) (AnycubicSerialprintPGM(PSTR(x)))
#define ANYCUBIC_SERIAL_(x) (AnycubicSerial.print(x),AnycubicSerial.write('\n'))
#define ANYCUBIC_SERIAL_PROTOCOLLN(x) (AnycubicSerial.print(x),AnycubicSerial.write('\r'),AnycubicSerial.write('\n'))
#define ANYCUBIC_SERIAL_PROTOCOLLNPGM(x) (AnycubicSerialprintPGM(PSTR(x)),AnycubicSerial.write('\r'),AnycubicSerial.write('\n'))

#define ANYCUBIC_SERIAL_START() (AnycubicSerial.write('\r'),AnycubicSerial.write('\n'))
#define ANYCUBIC_SERIAL_CMD_SEND(x) (AnycubicSerialprintPGM(PSTR(x)),AnycubicSerial.write('\r'),AnycubicSerial.write('\n'))
#define ANYCUBIC_SERIAL_ENTER() (AnycubicSerial.write('\r'),AnycubicSerial.write('\n'))
#define ANYCUBIC_SERIAL_SPACE() (AnycubicSerial.write(' '))

const char newErr[] PROGMEM = "ERR ";
const char newSucc[] PROGMEM = "OK";

#define ANYCUBIC_SERIAL_ERROR_START (AnycubicSerialprintPGM(newErr))
#define ANYCUBIC_SERIAL_ERROR(x) ANYCUBIC_SERIAL_PROTOCOL(x)
#define ANYCUBIC_SERIAL_ERRORPGM(x) ANYCUBIC_SERIAL_PROTOCOLPGM(x)
#define ANYCUBIC_SERIAL_ERRORLN(x) ANYCUBIC_SERIAL_PROTOCOLLN(x)
#define ANYCUBIC_SERIAL_ERRORLNPGM(x) ANYCUBIC_SERIAL_PROTOCOLLNPGM(x)

//##define ANYCUBIC_SERIAL_ECHO_START (AnycubicSerialprintPGM(newSucc))
#define ANYCUBIC_SERIAL_ECHOLN(x) ANYCUBIC_SERIAL_PROTOCOLLN(x)
#define ANYCUBIC_SERIAL_SUCC_START (AnycubicSerialprintPGM(newSucc))
#define ANYCUBIC_SERIAL_ECHOPAIR(name,value) (serial_echopair_P(PSTR(name),(value)))
#define ANYCUBIC_SERIAL_ECHOPGM(x) ANYCUBIC_SERIAL_PROTOCOLPGM(x)
#define ANYCUBIC_SERIAL_ECHO(x) ANYCUBIC_SERIAL_PROTOCOL(x)

FORCE_INLINE void AnycubicSerialprintPGM(const char *str) {
  char ch=pgm_read_byte(str);
  while (ch) {
    AnycubicSerial.write(ch);
    ch=pgm_read_byte(++str);
  }
}

#ifdef UBRR3H
  extern AnycubicSerialClass AnycubicSerial;
#endif
