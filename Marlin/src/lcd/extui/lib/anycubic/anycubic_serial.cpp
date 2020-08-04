/*
  anycubic_serial.cpp  --- Support for Anycubic i3 Mega TFT serial connection
  Created by Christian Hopp on 09.12.17.

  Original file:
  HardwareSerial.cpp - Hardware serial library for Wiring
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

  Modified 23 November 2006 by David A. Mellis
  Modified 28 September 2010 by Mark Sproul
  Modified 14 August 2012 by Alarus
*/

#include "../../../../inc/MarlinConfig.h"

#if ENABLED(ANYCUBIC_TFT_MODEL)

#include <Arduino.h>

// This next line disables the entire anycubic_serial.cpp,
// to support AtTiny series and other chips without a UART
#ifdef UBRR3H

#include "anycubic_serial.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "wiring_private.h"

// Define constants and variables for buffering incoming serial data.  We're
// using a ring buffer (I think), in which head is the index of the location
// to which to write the next incoming character and tail is the index of the
// location from which to read.
#if (RAMEND < 1000)
  #define SERIAL_BUFFER_SIZE 64
#else
  #define SERIAL_BUFFER_SIZE 128
#endif

struct ring_buffer {
  unsigned char buffer[SERIAL_BUFFER_SIZE];
  volatile unsigned int head;
  volatile unsigned int tail;
};

ring_buffer rx_buffer_ajg  =  { { 0 }, 0, 0 };
ring_buffer tx_buffer_ajg  =  { { 0 }, 0, 0 };

inline void store_char(unsigned char c, ring_buffer *buffer) {
  int i = (unsigned int)(buffer->head + 1) % SERIAL_BUFFER_SIZE;

  // if we should be storing the received character into the location
  // just before the tail (meaning that the head would advance to the
  // current location of the tail), we're about to overflow the buffer
  // and so we don't write the character or advance the head.
  if (i != buffer->tail) {
    buffer->buffer[buffer->head] = c;
    buffer->head = i;
  }
}

#if defined(USART3_RX_vect) && defined(UDR3)
  void serialEvent3() __attribute__((weak));
  void serialEvent3() {}
  #define serialEvent3_implemented
  ISR(USART3_RX_vect) {
    if (bit_is_clear(UCSR3A, UPE3)) {
      unsigned char c = UDR3;
      store_char(c, &rx_buffer_ajg);
    }
    else {
      unsigned char c = UDR3;
    }
  }
#endif

#ifdef USART3_UDRE_vect

  ISR(USART3_UDRE_vect) {
    if (tx_buffer_ajg.head == tx_buffer_ajg.tail) {
  	// Buffer empty, so disable interrupts
      cbi(UCSR3B, UDRIE3);
    }
    else {
      // There is more data in the output buffer. Send the next byte
      unsigned char c = tx_buffer_ajg.buffer[tx_buffer_ajg.tail];
      tx_buffer_ajg.tail = (tx_buffer_ajg.tail + 1) % SERIAL_BUFFER_SIZE;

      UDR3 = c;
    }
  }

#endif

// Constructors ////////////////////////////////////////////////////////////////

AnycubicSerialClass::AnycubicSerialClass(ring_buffer *rx_buffer, ring_buffer *tx_buffer,
  volatile uint8_t *ubrrh, volatile uint8_t *ubrrl,
  volatile uint8_t *ucsra, volatile uint8_t *ucsrb,
  volatile uint8_t *ucsrc, volatile uint8_t *udr,
  uint8_t rxen, uint8_t txen, uint8_t rxcie, uint8_t udrie, uint8_t u2x
) {
  _rx_buffer = rx_buffer;
  _tx_buffer = tx_buffer;
  _ubrrh = ubrrh;
  _ubrrl = ubrrl;
  _ucsra = ucsra;
  _ucsrb = ucsrb;
  _ucsrc = ucsrc;
  _udr = udr;
  _rxen = rxen;
  _txen = txen;
  _rxcie = rxcie;
  _udrie = udrie;
  _u2x = u2x;
}

// Public Methods //////////////////////////////////////////////////////////////

void AnycubicSerialClass::begin(unsigned long baud) {
  uint16_t baud_setting;
  bool use_u2x = true;

  #if F_CPU == 16000000UL
    // hardcoded exception for compatibility with the bootloader shipped
    // with the Duemilanove and previous boards and the firmware on the 8U2
    // on the Uno and Mega 2560.
    if (baud == 57600) use_u2x = false;
  #endif

try_again:

  if (use_u2x) {
    *_ucsra = 1 << _u2x;
    baud_setting = (F_CPU / 4 / baud - 1) / 2;
  } else {
    *_ucsra = 0;
    baud_setting = (F_CPU / 8 / baud - 1) / 2;
  }

  if ((baud_setting > 4095) && use_u2x) {
    use_u2x = false;
    goto try_again;
  }

  // assign the baud_setting, a.k.a. ubbr (USART Baud Rate Register)
  *_ubrrh = baud_setting >> 8;
  *_ubrrl = baud_setting;

  transmitting = false;

  sbi(*_ucsrb, _rxen);
  sbi(*_ucsrb, _txen);
  sbi(*_ucsrb, _rxcie);
  cbi(*_ucsrb, _udrie);
}

void AnycubicSerialClass::begin(unsigned long baud, byte config) {
  uint16_t baud_setting;
  uint8_t current_config;
  bool use_u2x = true;

  #if F_CPU == 16000000UL
    // hardcoded exception for compatibility with the bootloader shipped
    // with the Duemilanove and previous boards and the firmware on the 8U2
    // on the Uno and Mega 2560.
    if (baud == 57600) use_u2x = false;
  #endif

try_again:

  if (use_u2x) {
    *_ucsra = 1 << _u2x;
    baud_setting = (F_CPU / 4 / baud - 1) / 2;
  }
  else {
    *_ucsra = 0;
    baud_setting = (F_CPU / 8 / baud - 1) / 2;
  }

  if ((baud_setting > 4095) && use_u2x) {
    use_u2x = false;
    goto try_again;
  }

  // assign the baud_setting, a.k.a. ubbr (USART Baud Rate Register)
  *_ubrrh = baud_setting >> 8;
  *_ubrrl = baud_setting;

  //set the data bits, parity, and stop bits
  #ifdef __AVR_ATmega8__
    config |= 0x80; // select UCSRC register (shared with UBRRH)
  #endif
  *_ucsrc = config;

  sbi(*_ucsrb, _rxen);
  sbi(*_ucsrb, _txen);
  sbi(*_ucsrb, _rxcie);
  cbi(*_ucsrb, _udrie);
}

void AnycubicSerialClass::end() {
  // wait for transmission of outgoing data
  while (_tx_buffer->head != _tx_buffer->tail)
    ;

  cbi(*_ucsrb, _rxen);
  cbi(*_ucsrb, _txen);
  cbi(*_ucsrb, _rxcie);
  cbi(*_ucsrb, _udrie);

  // clear any received data
  _rx_buffer->head = _rx_buffer->tail;
}

int AnycubicSerialClass::available(void) {
  return (int)(SERIAL_BUFFER_SIZE + _rx_buffer->head - _rx_buffer->tail) % SERIAL_BUFFER_SIZE;
}

int AnycubicSerialClass::peek(void) {
  if (_rx_buffer->head == _rx_buffer->tail) {
    return -1;
  } else {
    return _rx_buffer->buffer[_rx_buffer->tail];
  }
}

int AnycubicSerialClass::read(void) {
  // if the head isn't ahead of the tail, we don't have any characters
  if (_rx_buffer->head == _rx_buffer->tail) {
    return -1;
  } else {
    unsigned char c = _rx_buffer->buffer[_rx_buffer->tail];
    _rx_buffer->tail = (unsigned int)(_rx_buffer->tail + 1) % SERIAL_BUFFER_SIZE;
    return c;
  }
}

void AnycubicSerialClass::flush() {
  // UDR is kept full while the buffer is not empty, so TXC triggers when EMPTY && SENT
  while (transmitting && ! (*_ucsra & _BV(TXC0)));
  transmitting = false;
}

size_t AnycubicSerialClass::write(uint8_t c) {
  int i = (_tx_buffer->head + 1) % SERIAL_BUFFER_SIZE;

  // If the output buffer is full, there's nothing for it other than to
  // wait for the interrupt handler to empty it a bit
  // ???: return 0 here instead?
  while (i == _tx_buffer->tail)
    ;

  _tx_buffer->buffer[_tx_buffer->head] = c;
  _tx_buffer->head = i;

  sbi(*_ucsrb, _udrie);
  // clear the TXC bit -- "can be cleared by writing a one to its bit location"
  transmitting = true;
  sbi(*_ucsra, TXC0);

  return 1;
}

AnycubicSerialClass::operator bool() {
	return true;
}

// Preinstantiate Objects //////////////////////////////////////////////////////

AnycubicSerialClass AnycubicSerial(&rx_buffer_ajg, &tx_buffer_ajg, &UBRR3H, &UBRR3L, &UCSR3A, &UCSR3B, &UCSR3C, &UDR3, RXEN3, TXEN3, RXCIE3, UDRIE3, U2X3);

#endif // UBRR3H
#endif // ANYCUBIC_TFT_MODEL
