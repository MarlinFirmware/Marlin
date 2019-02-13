/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * MarlinSerial_Due.cpp - Hardware serial library for Arduino DUE
 * Copyright (c) 2017 Eduardo José Tagle. All right reserved
 * Based on MarlinSerial for AVR, copyright (c) 2006 Nicholas Zambetti.  All right reserved.
 */
#ifdef ARDUINO_ARCH_SAM

#include "../../inc/MarlinConfig.h"

#if SERIAL_PORT == -1

#include "MarlinSerialUSB_Due.h"

#if ENABLED(EMERGENCY_PARSER)
  #include "../../feature/emergency_parser.h"
#endif

// Imports from Atmel USB Stack/CDC implementation
extern "C" {
  bool usb_task_cdc_isenabled(void);
  bool usb_task_cdc_dtr_active(void);
  bool udi_cdc_is_rx_ready(void);
  int udi_cdc_getc(void);
  bool udi_cdc_is_tx_ready(void);
  int udi_cdc_putc(int value);
};

// Pending character
static int pending_char = -1;

#if ENABLED(EMERGENCY_PARSER)
  static EmergencyParser::State emergency_state; // = EP_RESET
#endif

// Public Methods
void MarlinSerialUSB::begin(const long baud_setting) {
}

void MarlinSerialUSB::end() {
}

int MarlinSerialUSB::peek(void) {
  if (pending_char >= 0)
    return pending_char;

  // If USB CDC not enumerated or not configured on the PC side
  if (!usb_task_cdc_isenabled())
    return -1;

  // If no bytes sent from the PC
  if (!udi_cdc_is_rx_ready())
    return -1;

  pending_char = udi_cdc_getc();

  #if ENABLED(EMERGENCY_PARSER)
    emergency_parser.update(emergency_state, (char)pending_char);
  #endif

  return pending_char;
}

int MarlinSerialUSB::read(void) {
  if (pending_char >= 0) {
    int ret = pending_char;
    pending_char = -1;
    return ret;
  }

  // If USB CDC not enumerated or not configured on the PC side
  if (!usb_task_cdc_isenabled())
    return -1;

  // If no bytes sent from the PC
  if (!udi_cdc_is_rx_ready())
    return -1;

  int c = udi_cdc_getc();

  #if ENABLED(EMERGENCY_PARSER)
    emergency_parser.update(emergency_state, (char)c);
  #endif

  return c;
}

bool MarlinSerialUSB::available(void) {
    /* If Pending chars */
  return pending_char >= 0 ||
    /* or USB CDC enumerated and configured on the PC side and some
       bytes where sent to us */
      (usb_task_cdc_isenabled() && udi_cdc_is_rx_ready());
}

void MarlinSerialUSB::flush(void) {
}

void MarlinSerialUSB::write(const uint8_t c) {

  /* Do not even bother sending anything if USB CDC is not enumerated
     or not configured on the PC side or there is no program on the PC
     listening to our messages */
  if (!usb_task_cdc_isenabled() || !usb_task_cdc_dtr_active())
    return;

  /* Wait until the PC has read the pending to be sent data */
  while (usb_task_cdc_isenabled() &&
         usb_task_cdc_dtr_active() &&
        !udi_cdc_is_tx_ready()) {
  };

  /* Do not even bother sending anything if USB CDC is not enumerated
     or not configured on the PC side or there is no program on the PC
     listening to our messages at this point */
  if (!usb_task_cdc_isenabled() || !usb_task_cdc_dtr_active())
    return;

  // Fifo full
  //  udi_cdc_signal_overrun();
  udi_cdc_putc(c);
}

/**
 * Imports from print.h
 */

void MarlinSerialUSB::print(char c, int base) {
  print((long)c, base);
}

void MarlinSerialUSB::print(unsigned char b, int base) {
  print((unsigned long)b, base);
}

void MarlinSerialUSB::print(int n, int base) {
  print((long)n, base);
}

void MarlinSerialUSB::print(unsigned int n, int base) {
  print((unsigned long)n, base);
}

void MarlinSerialUSB::print(long n, int base) {
  if (base == 0)
    write(n);
  else if (base == 10) {
    if (n < 0) {
      print('-');
      n = -n;
    }
    printNumber(n, 10);
  }
  else
    printNumber(n, base);
}

void MarlinSerialUSB::print(unsigned long n, int base) {
  if (base == 0) write(n);
  else printNumber(n, base);
}

void MarlinSerialUSB::print(double n, int digits) {
  printFloat(n, digits);
}

void MarlinSerialUSB::println(void) {
  print('\r');
  print('\n');
}

void MarlinSerialUSB::println(const String& s) {
  print(s);
  println();
}

void MarlinSerialUSB::println(const char c[]) {
  print(c);
  println();
}

void MarlinSerialUSB::println(char c, int base) {
  print(c, base);
  println();
}

void MarlinSerialUSB::println(unsigned char b, int base) {
  print(b, base);
  println();
}

void MarlinSerialUSB::println(int n, int base) {
  print(n, base);
  println();
}

void MarlinSerialUSB::println(unsigned int n, int base) {
  print(n, base);
  println();
}

void MarlinSerialUSB::println(long n, int base) {
  print(n, base);
  println();
}

void MarlinSerialUSB::println(unsigned long n, int base) {
  print(n, base);
  println();
}

void MarlinSerialUSB::println(double n, int digits) {
  print(n, digits);
  println();
}

// Private Methods

void MarlinSerialUSB::printNumber(unsigned long n, uint8_t base) {
  if (n) {
    unsigned char buf[8 * sizeof(long)]; // Enough space for base 2
    int8_t i = 0;
    while (n) {
      buf[i++] = n % base;
      n /= base;
    }
    while (i--)
      print((char)(buf[i] + (buf[i] < 10 ? '0' : 'A' - 10)));
  }
  else
    print('0');
}

void MarlinSerialUSB::printFloat(double number, uint8_t digits) {
  // Handle negative numbers
  if (number < 0.0) {
    print('-');
    number = -number;
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"
  double rounding = 0.5;
  for (uint8_t i = 0; i < digits; ++i)
    rounding *= 0.1;

  number += rounding;

  // Extract the integer part of the number and print it
  unsigned long int_part = (unsigned long)number;
  double remainder = number - (double)int_part;
  print(int_part);

  // Print the decimal point, but only if there are digits beyond
  if (digits) {
    print('.');
    // Extract digits from the remainder one at a time
    while (digits--) {
      remainder *= 10.0;
      int toPrint = int(remainder);
      print(toPrint);
      remainder -= toPrint;
    }
  }
}

// Preinstantiate
MarlinSerialUSB customizedSerial1;

#endif // SERIAL_PORT == -1

#endif // ARDUINO_ARCH_SAM
