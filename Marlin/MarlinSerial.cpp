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

/**
  HardwareSerial.cpp - Hardware serial library for Wiring
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  Modified 23 November 2006 by David A. Mellis
  Modified 28 September 2010 by Mark Sproul
*/

#include "Marlin.h"
#include "MarlinSerial.h"

#ifndef USBCON
// this next line disables the entire HardwareSerial.cpp,
// this is so I can support Attiny series and any other chip without a UART
#if defined(UBRRH) || defined(UBRR0H) || defined(UBRR1H) || defined(UBRR2H) || defined(UBRR3H)

#if UART_PRESENT(SERIAL_PORT)
  ring_buffer rx_buffer  =  { { 0 }, 0, 0 };
#endif

FORCE_INLINE void store_char(unsigned char c) {
  CRITICAL_SECTION_START;
    uint8_t h = rx_buffer.head;
    uint8_t i = (uint8_t)(h + 1)  & (RX_BUFFER_SIZE - 1);

    // if we should be storing the received character into the location
    // just before the tail (meaning that the head would advance to the
    // current location of the tail), we're about to overflow the buffer
    // and so we don't write the character or advance the head.
    if (i != rx_buffer.tail) {
      rx_buffer.buffer[h] = c;
      rx_buffer.head = i;
    }
  CRITICAL_SECTION_END;
}


//#elif defined(SIG_USART_RECV)
#if defined(M_USARTx_RX_vect)
  // fixed by Mark Sproul this is on the 644/644p
  //SIGNAL(SIG_USART_RECV)
  SIGNAL(M_USARTx_RX_vect) {
    unsigned char c  =  M_UDRx;
    store_char(c);
  }
#endif

// Constructors ////////////////////////////////////////////////////////////////

MarlinSerial::MarlinSerial() { }

// Public Methods //////////////////////////////////////////////////////////////

void MarlinSerial::begin(long baud) {
  uint16_t baud_setting;
  bool useU2X = true;

  #if F_CPU == 16000000UL && SERIAL_PORT == 0
    // hard-coded exception for compatibility with the bootloader shipped
    // with the Duemilanove and previous boards and the firmware on the 8U2
    // on the Uno and Mega 2560.
    if (baud == 57600) {
      useU2X = false;
    }
  #endif

  if (useU2X) {
    M_UCSRxA = _BV(M_U2Xx);
    baud_setting = (F_CPU / 4 / baud - 1) / 2;
  }
  else {
    M_UCSRxA = 0;
    baud_setting = (F_CPU / 8 / baud - 1) / 2;
  }

  // assign the baud_setting, a.k.a. ubbr (USART Baud Rate Register)
  M_UBRRxH = baud_setting >> 8;
  M_UBRRxL = baud_setting;

  SBI(M_UCSRxB, M_RXENx);
  SBI(M_UCSRxB, M_TXENx);
  SBI(M_UCSRxB, M_RXCIEx);
}

void MarlinSerial::end() {
  CBI(M_UCSRxB, M_RXENx);
  CBI(M_UCSRxB, M_TXENx);
  CBI(M_UCSRxB, M_RXCIEx);
}


int MarlinSerial::peek(void) {
  int v;
  CRITICAL_SECTION_START;
  uint8_t t = rx_buffer.tail;
  if (rx_buffer.head == t) {
    v = -1;
  }
  else {
    v = rx_buffer.buffer[t];
  }
  CRITICAL_SECTION_END;
  return v;
}

int MarlinSerial::read(void) {
  int v;
  CRITICAL_SECTION_START;
  uint8_t t = rx_buffer.tail;
  if (rx_buffer.head == t) {
    v = -1;
  }
  else {
    v = rx_buffer.buffer[t];
    rx_buffer.tail = (uint8_t)(t + 1) & (RX_BUFFER_SIZE - 1);
  }
  CRITICAL_SECTION_END;
  return v;
}

void MarlinSerial::flush() {
  // don't reverse this or there may be problems if the RX interrupt
  // occurs after reading the value of rx_buffer_head but before writing
  // the value to rx_buffer_tail; the previous value of rx_buffer_head
  // may be written to rx_buffer_tail, making it appear as if the buffer
  // were full, not empty.
  CRITICAL_SECTION_START;
    rx_buffer.head = rx_buffer.tail;
  CRITICAL_SECTION_END;
}


/// imports from print.h


void MarlinSerial::print(char c, int base) {
  print((long) c, base);
}

void MarlinSerial::print(unsigned char b, int base) {
  print((unsigned long) b, base);
}

void MarlinSerial::print(int n, int base) {
  print((long) n, base);
}

void MarlinSerial::print(unsigned int n, int base) {
  print((unsigned long) n, base);
}

void MarlinSerial::print(long n, int base) {
  if (base == 0) {
    write(n);
  }
  else if (base == 10) {
    if (n < 0) {
      print('-');
      n = -n;
    }
    printNumber(n, 10);
  }
  else {
    printNumber(n, base);
  }
}

void MarlinSerial::print(unsigned long n, int base) {
  if (base == 0) write(n);
  else printNumber(n, base);
}

void MarlinSerial::print(double n, int digits) {
  printFloat(n, digits);
}

void MarlinSerial::println(void) {
  print('\r');
  print('\n');
}

void MarlinSerial::println(const String& s) {
  print(s);
  println();
}

void MarlinSerial::println(const char c[]) {
  print(c);
  println();
}

void MarlinSerial::println(char c, int base) {
  print(c, base);
  println();
}

void MarlinSerial::println(unsigned char b, int base) {
  print(b, base);
  println();
}

void MarlinSerial::println(int n, int base) {
  print(n, base);
  println();
}

void MarlinSerial::println(unsigned int n, int base) {
  print(n, base);
  println();
}

void MarlinSerial::println(long n, int base) {
  print(n, base);
  println();
}

void MarlinSerial::println(unsigned long n, int base) {
  print(n, base);
  println();
}

void MarlinSerial::println(double n, int digits) {
  print(n, digits);
  println();
}

// Private Methods /////////////////////////////////////////////////////////////

void MarlinSerial::printNumber(unsigned long n, uint8_t base) {
  unsigned char buf[8 * sizeof(long)]; // Assumes 8-bit chars.
  unsigned long i = 0;

  if (n == 0) {
    print('0');
    return;
  }

  while (n > 0) {
    buf[i++] = n % base;
    n /= base;
  }

  for (; i > 0; i--)
    print((char)(buf[i - 1] < 10 ?
                 '0' + buf[i - 1] :
                 'A' + buf[i - 1] - 10));
}

void MarlinSerial::printFloat(double number, uint8_t digits) {
  // Handle negative numbers
  if (number < 0.0) {
    print('-');
    number = -number;
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"
  double rounding = 0.5;
  for (uint8_t i = 0; i < digits; ++i)
    rounding /= 10.0;

  number += rounding;

  // Extract the integer part of the number and print it
  unsigned long int_part = (unsigned long)number;
  double remainder = number - (double)int_part;
  print(int_part);

  // Print the decimal point, but only if there are digits beyond
  if (digits > 0) print('.');

  // Extract digits from the remainder one at a time
  while (digits-- > 0) {
    remainder *= 10.0;
    int toPrint = int(remainder);
    print(toPrint);
    remainder -= toPrint;
  }
}
// Preinstantiate Objects //////////////////////////////////////////////////////


MarlinSerial customizedSerial;

#endif // whole file
#endif // !USBCON

// For AT90USB targets use the UART for BT interfacing
#if defined(USBCON) && ENABLED(BLUETOOTH)
  HardwareSerial bluetoothSerial;
#endif
