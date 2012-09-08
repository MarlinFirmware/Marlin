/*
  wiring_serial.c - serial functions.
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2005-2006 David A. Mellis
  Modified 29 January 2009, Marius Kintel for Sanguino - http://www.sanguino.cc/

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  $Id: wiring.c 248 2007-02-03 15:36:30Z mellis $
*/


#include "wiring_private.h"

// Define constants and variables for buffering incoming serial data.  We're
// using a ring buffer (I think), in which rx_buffer_head is the index of the
// location to which to write the next incoming character and rx_buffer_tail
// is the index of the location from which to read.
#define RX_BUFFER_SIZE 128

#if defined(__AVR_ATmega644P__)
unsigned char rx_buffer[2][RX_BUFFER_SIZE];
int rx_buffer_head[2] = {0, 0};
int rx_buffer_tail[2] = {0, 0};
#else
unsigned char rx_buffer[1][RX_BUFFER_SIZE];
int rx_buffer_head[1] = {0};
int rx_buffer_tail[1] = {0};
#endif


#define BEGIN_SERIAL(uart_, baud_) \
{ \
    UBRR##uart_##H = ((F_CPU / 16 + baud / 2) / baud - 1) >> 8; \
    UBRR##uart_##L = ((F_CPU / 16 + baud / 2) / baud - 1); \
    \
    /* reset config for UART */ \
    UCSR##uart_##A = 0; \
    UCSR##uart_##B = 0; \
    UCSR##uart_##C = 0; \
    \
    /* enable rx and tx */ \
    sbi(UCSR##uart_##B, RXEN##uart_);\
    sbi(UCSR##uart_##B, TXEN##uart_);\
    \
    /* enable interrupt on complete reception of a byte */ \
    sbi(UCSR##uart_##B, RXCIE##uart_); \
    UCSR##uart_##C = _BV(UCSZ##uart_##1)|_BV(UCSZ##uart_##0); \
    /* defaults to 8-bit, no parity, 1 stop bit */ \
}

void beginSerial(uint8_t uart, long baud)
{
  if (uart == 0) BEGIN_SERIAL(0, baud)
#if defined(__AVR_ATmega644P__)
  else BEGIN_SERIAL(1, baud)
#endif
}

#define SERIAL_WRITE(uart_, c_) \
    while (!(UCSR##uart_##A & (1 << UDRE##uart_))) \
      ; \
    UDR##uart_ = c

void serialWrite(uint8_t uart, unsigned char c)
{
  if (uart == 0) {
    SERIAL_WRITE(0, c);
  }
#if defined(__AVR_ATmega644P__)
  else {
    SERIAL_WRITE(1, c);
  }
#endif
}

int serialAvailable(uint8_t uart)
{
  return (RX_BUFFER_SIZE + rx_buffer_head[uart] - rx_buffer_tail[uart]) % RX_BUFFER_SIZE;
}

int serialRead(uint8_t uart)
{
  // if the head isn't ahead of the tail, we don't have any characters
  if (rx_buffer_head[uart] == rx_buffer_tail[uart]) {
    return -1;
  } else {
    unsigned char c = rx_buffer[uart][rx_buffer_tail[uart]];
    rx_buffer_tail[uart] = (rx_buffer_tail[uart] + 1) % RX_BUFFER_SIZE;
    return c;
  }
}

void serialFlush(uint8_t uart)
{
  // don't reverse this or there may be problems if the RX interrupt
  // occurs after reading the value of rx_buffer_head but before writing
  // the value to rx_buffer_tail; the previous value of rx_buffer_head
  // may be written to rx_buffer_tail, making it appear as if the buffer
  // were full, not empty.
  rx_buffer_head[uart] = rx_buffer_tail[uart];
}

#define UART_ISR(uart_) \
ISR(USART##uart_##_RX_vect) \
{ \
  unsigned char c = UDR##uart_; \
  \
  int i = (rx_buffer_head[uart_] + 1) % RX_BUFFER_SIZE; \
  \  
  /* if we should be storing the received character into the location \
     just before the tail (meaning that the head would advance to the \
     current location of the tail), we're about to overflow the buffer \
     and so we don't write the character or advance the head. */ \
  if (i != rx_buffer_tail[uart_]) { \
    rx_buffer[uart_][rx_buffer_head[uart_]] = c; \
    rx_buffer_head[uart_] = i; \
  } \
}

UART_ISR(0)
#if defined(__AVR_ATmega644P__) 
UART_ISR(1)
#endif
