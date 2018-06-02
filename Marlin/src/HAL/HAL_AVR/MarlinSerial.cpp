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
 * MarlinSerial.cpp - Hardware serial library for Wiring
 * Copyright (c) 2006 Nicholas Zambetti.  All right reserved.
 *
 * Modified 23 November 2006 by David A. Mellis
 * Modified 28 September 2010 by Mark Sproul
 * Modified 14 February 2016 by Andreas Hardtung (added tx buffer)
 * Modified 01 October 2017 by Eduardo José Tagle (added XON/XOFF)
 */
#ifdef __AVR__

// Disable HardwareSerial.cpp to support chips without a UART (Attiny, etc.)

#include "../../inc/MarlinConfig.h"

#if !defined(USBCON) && (defined(UBRRH) || defined(UBRR0H) || defined(UBRR1H) || defined(UBRR2H) || defined(UBRR3H))

  #include "MarlinSerial.h"
  #include "../../Marlin.h"

  struct ring_buffer_r {
    unsigned char buffer[RX_BUFFER_SIZE];
    volatile ring_buffer_pos_t head, tail;
  };

  #if TX_BUFFER_SIZE > 0
    struct ring_buffer_t {
      unsigned char buffer[TX_BUFFER_SIZE];
      volatile uint8_t head, tail;
    };
  #endif

  #if UART_PRESENT(SERIAL_PORT)
    ring_buffer_r rx_buffer = { { 0 }, 0, 0 };
    #if TX_BUFFER_SIZE > 0
      ring_buffer_t tx_buffer = { { 0 }, 0, 0 };
      static bool _written;
    #endif
  #endif

  #if ENABLED(SERIAL_XON_XOFF)
    constexpr uint8_t XON_XOFF_CHAR_SENT = 0x80;  // XON / XOFF Character was sent
    constexpr uint8_t XON_XOFF_CHAR_MASK = 0x1F;  // XON / XOFF character to send
    // XON / XOFF character definitions
    constexpr uint8_t XON_CHAR  = 17;
    constexpr uint8_t XOFF_CHAR = 19;
    uint8_t xon_xoff_state = XON_XOFF_CHAR_SENT | XON_CHAR;
  #endif

  #if ENABLED(SERIAL_STATS_DROPPED_RX)
    uint8_t rx_dropped_bytes = 0;
  #endif

  #if ENABLED(SERIAL_STATS_MAX_RX_QUEUED)
    ring_buffer_pos_t rx_max_enqueued = 0;
  #endif

  // A SW memory barrier, to ensure GCC does not overoptimize loops
  #define sw_barrier() asm volatile("": : :"memory");

  #if ENABLED(EMERGENCY_PARSER)
    #include "../../feature/emergency_parser.h"
  #endif

  // (called with RX interrupts disabled)
  FORCE_INLINE void store_rxd_char() {

    #if ENABLED(EMERGENCY_PARSER)
      static EmergencyParser::State emergency_state; // = EP_RESET
    #endif

    const ring_buffer_pos_t h = rx_buffer.head,
                            i = (ring_buffer_pos_t)(h + 1) & (ring_buffer_pos_t)(RX_BUFFER_SIZE - 1);

    // Read the character
    const uint8_t c = M_UDRx;

    // If the character is to be stored at the index just before the tail
    // (such that the head would advance to the current tail), the buffer is
    // critical, so don't write the character or advance the head.
    if (i != rx_buffer.tail) {
      rx_buffer.buffer[h] = c;
      rx_buffer.head = i;
    }
    else {
      #if ENABLED(SERIAL_STATS_DROPPED_RX)
        if (!++rx_dropped_bytes) ++rx_dropped_bytes;
      #endif
    }

    #if ENABLED(SERIAL_STATS_MAX_RX_QUEUED)
      // calculate count of bytes stored into the RX buffer
      ring_buffer_pos_t rx_count = (ring_buffer_pos_t)(rx_buffer.head - rx_buffer.tail) & (ring_buffer_pos_t)(RX_BUFFER_SIZE - 1);
      // Keep track of the maximum count of enqueued bytes
      NOLESS(rx_max_enqueued, rx_count);
    #endif

    #if ENABLED(SERIAL_XON_XOFF)

      // for high speed transfers, we can use XON/XOFF protocol to do
      // software handshake and avoid overruns.
      if ((xon_xoff_state & XON_XOFF_CHAR_MASK) == XON_CHAR) {

        // calculate count of bytes stored into the RX buffer
        ring_buffer_pos_t rx_count = (ring_buffer_pos_t)(rx_buffer.head - rx_buffer.tail) & (ring_buffer_pos_t)(RX_BUFFER_SIZE - 1);

        // if we are above 12.5% of RX buffer capacity, send XOFF before
        // we run out of RX buffer space .. We need 325 bytes @ 250kbits/s to
        // let the host react and stop sending bytes. This translates to 13mS
        // propagation time.
        if (rx_count >= (RX_BUFFER_SIZE) / 8) {

          // If TX interrupts are disabled and data register is empty,
          // just write the byte to the data register and be done. This
          // shortcut helps significantly improve the effective datarate
          // at high (>500kbit/s) bitrates, where interrupt overhead
          // becomes a slowdown.
          if (!TEST(M_UCSRxB, M_UDRIEx) && TEST(M_UCSRxA, M_UDREx)) {

            // Send an XOFF character
            M_UDRx = XOFF_CHAR;

            // clear the TXC bit -- "can be cleared by writing a one to its bit
            // location". This makes sure flush() won't return until the bytes
            // actually got written
            SBI(M_UCSRxA, M_TXCx);

            // And remember it was sent
            xon_xoff_state = XOFF_CHAR | XON_XOFF_CHAR_SENT;
          }
          else {
            // TX interrupts disabled, but buffer still not empty ... or
            // TX interrupts enabled. Reenable TX ints and schedule XOFF
            // character to be sent
            #if TX_BUFFER_SIZE > 0
              SBI(M_UCSRxB, M_UDRIEx);
              xon_xoff_state = XOFF_CHAR;
            #else
              // We are not using TX interrupts, we will have to send this manually
              while (!TEST(M_UCSRxA, M_UDREx)) sw_barrier();
              M_UDRx = XOFF_CHAR;

              // clear the TXC bit -- "can be cleared by writing a one to its bit
              // location". This makes sure flush() won't return until the bytes
              // actually got written
              SBI(M_UCSRxA, M_TXCx);

              // And remember we already sent it
              xon_xoff_state = XOFF_CHAR | XON_XOFF_CHAR_SENT;
            #endif
          }
        }
      }
    #endif // SERIAL_XON_XOFF

    #if ENABLED(EMERGENCY_PARSER)
      emergency_parser.update(emergency_state, c);
    #endif
  }

  #if TX_BUFFER_SIZE > 0

    // (called with TX irqs disabled)
    FORCE_INLINE void _tx_udr_empty_irq(void) {
      // If interrupts are enabled, there must be more data in the output
      // buffer.

      #if ENABLED(SERIAL_XON_XOFF)
        // Do a priority insertion of an XON/XOFF char, if needed.
        const uint8_t state = xon_xoff_state;
        if (!(state & XON_XOFF_CHAR_SENT)) {
          M_UDRx = state & XON_XOFF_CHAR_MASK;
          xon_xoff_state = state | XON_XOFF_CHAR_SENT;
        }
        else
      #endif
      { // Send the next byte
        const uint8_t t = tx_buffer.tail, c = tx_buffer.buffer[t];
        tx_buffer.tail = (t + 1) & (TX_BUFFER_SIZE - 1);
        M_UDRx = c;
      }

      // clear the TXC bit -- "can be cleared by writing a one to its bit
      // location". This makes sure flush() won't return until the bytes
      // actually got written
      SBI(M_UCSRxA, M_TXCx);

      // Disable interrupts if the buffer is empty
      if (tx_buffer.head == tx_buffer.tail)
        CBI(M_UCSRxB, M_UDRIEx);
    }

    #ifdef M_USARTx_UDRE_vect
      ISR(M_USARTx_UDRE_vect) { _tx_udr_empty_irq(); }
    #endif

  #endif // TX_BUFFER_SIZE

  #ifdef M_USARTx_RX_vect
    ISR(M_USARTx_RX_vect) { store_rxd_char(); }
  #endif

  // Public Methods

  void MarlinSerial::begin(const long baud) {
    uint16_t baud_setting;
    bool useU2X = true;

    #if F_CPU == 16000000UL && SERIAL_PORT == 0
      // Hard-coded exception for compatibility with the bootloader shipped
      // with the Duemilanove and previous boards, and the firmware on the
      // 8U2 on the Uno and Mega 2560.
      if (baud == 57600) useU2X = false;
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
    #if TX_BUFFER_SIZE > 0
      CBI(M_UCSRxB, M_UDRIEx);
      _written = false;
    #endif
  }

  void MarlinSerial::end() {
    CBI(M_UCSRxB, M_RXENx);
    CBI(M_UCSRxB, M_TXENx);
    CBI(M_UCSRxB, M_RXCIEx);
    CBI(M_UCSRxB, M_UDRIEx);
  }

  int MarlinSerial::peek(void) {
    #if RX_BUFFER_SIZE > 256
      // Disable RX interrupts, but only if non atomic reads
      const bool isr_enabled = TEST(M_UCSRxB, M_RXCIEx);
      CBI(M_UCSRxB, M_RXCIEx);
    #endif
      const int v = rx_buffer.head == rx_buffer.tail ? -1 : rx_buffer.buffer[rx_buffer.tail];
    #if RX_BUFFER_SIZE > 256
      // Reenable RX interrupts if they were enabled
      if (isr_enabled) SBI(M_UCSRxB, M_RXCIEx);
    #endif
    return v;
  }

  int MarlinSerial::read(void) {
    int v;

    #if RX_BUFFER_SIZE > 256
      // Disable RX interrupts to ensure atomic reads
      const bool isr_enabled = TEST(M_UCSRxB, M_RXCIEx);
      CBI(M_UCSRxB, M_RXCIEx);
    #endif

    const ring_buffer_pos_t h = rx_buffer.head;

    #if RX_BUFFER_SIZE > 256
      // End critical section
      if (isr_enabled) SBI(M_UCSRxB, M_RXCIEx);
    #endif

    ring_buffer_pos_t t = rx_buffer.tail;

    if (h == t)
      v = -1;
    else {
      v = rx_buffer.buffer[t];
      t = (ring_buffer_pos_t)(t + 1) & (RX_BUFFER_SIZE - 1);

      #if RX_BUFFER_SIZE > 256
        // Disable RX interrupts to ensure atomic write to tail, so
        // the RX isr can't read partially updated values
        const bool isr_enabled = TEST(M_UCSRxB, M_RXCIEx);
        CBI(M_UCSRxB, M_RXCIEx);
      #endif

      // Advance tail
      rx_buffer.tail = t;

      #if RX_BUFFER_SIZE > 256
        // End critical section
        if (isr_enabled) SBI(M_UCSRxB, M_RXCIEx);
      #endif

      #if ENABLED(SERIAL_XON_XOFF)
        if ((xon_xoff_state & XON_XOFF_CHAR_MASK) == XOFF_CHAR) {

          // Get count of bytes in the RX buffer
          ring_buffer_pos_t rx_count = (ring_buffer_pos_t)(h - t) & (ring_buffer_pos_t)(RX_BUFFER_SIZE - 1);

          // When below 10% of RX buffer capacity, send XON before
          // running out of RX buffer bytes
          if (rx_count < (RX_BUFFER_SIZE) / 10) {
            xon_xoff_state = XON_CHAR | XON_XOFF_CHAR_SENT;
            write(XON_CHAR);
            return v;
          }
        }
      #endif
    }

    return v;
  }

  ring_buffer_pos_t MarlinSerial::available(void) {
    #if RX_BUFFER_SIZE > 256
      const bool isr_enabled = TEST(M_UCSRxB, M_RXCIEx);
      CBI(M_UCSRxB, M_RXCIEx);
    #endif

      const ring_buffer_pos_t h = rx_buffer.head, t = rx_buffer.tail;

    #if RX_BUFFER_SIZE > 256
      if (isr_enabled) SBI(M_UCSRxB, M_RXCIEx);
    #endif

    return (ring_buffer_pos_t)(RX_BUFFER_SIZE + h - t) & (RX_BUFFER_SIZE - 1);
  }

  void MarlinSerial::flush(void) {
    #if RX_BUFFER_SIZE > 256
      const bool isr_enabled = TEST(M_UCSRxB, M_RXCIEx);
      CBI(M_UCSRxB, M_RXCIEx);
    #endif

      rx_buffer.tail = rx_buffer.head;

    #if RX_BUFFER_SIZE > 256
      if (isr_enabled) SBI(M_UCSRxB, M_RXCIEx);
    #endif

    #if ENABLED(SERIAL_XON_XOFF)
      if ((xon_xoff_state & XON_XOFF_CHAR_MASK) == XOFF_CHAR) {
        xon_xoff_state = XON_CHAR | XON_XOFF_CHAR_SENT;
        write(XON_CHAR);
      }
    #endif
  }

  #if TX_BUFFER_SIZE > 0
    void MarlinSerial::write(const uint8_t c) {
      _written = true;

      // If the TX interrupts are disabled and the data register
      // is empty, just write the byte to the data register and
      // be done. This shortcut helps significantly improve the
      // effective datarate at high (>500kbit/s) bitrates, where
      // interrupt overhead becomes a slowdown.
      if (!TEST(M_UCSRxB, M_UDRIEx) && TEST(M_UCSRxA, M_UDREx)) {
        M_UDRx = c;

        // clear the TXC bit -- "can be cleared by writing a one to its bit
        // location". This makes sure flush() won't return until the bytes
        // actually got written
        SBI(M_UCSRxA, M_TXCx);
        return;
      }

      const uint8_t i = (tx_buffer.head + 1) & (TX_BUFFER_SIZE - 1);

      // If the output buffer is full, there's nothing for it other than to
      // wait for the interrupt handler to empty it a bit
      while (i == tx_buffer.tail) {
        if (!ISRS_ENABLED()) {
          // Interrupts are disabled, so we'll have to poll the data
          // register empty flag ourselves. If it is set, pretend an
          // interrupt has happened and call the handler to free up
          // space for us.
          if (TEST(M_UCSRxA, M_UDREx))
            _tx_udr_empty_irq();
        }
        // (else , the interrupt handler will free up space for us)

        // Make sure compiler rereads tx_buffer.tail
        sw_barrier();
      }

      // Store new char. head is always safe to move
      tx_buffer.buffer[tx_buffer.head] = c;
      tx_buffer.head = i;

      // Enable TX isr
      SBI(M_UCSRxB, M_UDRIEx);
      return;
    }

    void MarlinSerial::flushTX(void) {
      // TX
      // If we have never written a byte, no need to flush. This special
      // case is needed since there is no way to force the TXC (transmit
      // complete) bit to 1 during initialization
      if (!_written)
        return;

      while (TEST(M_UCSRxB, M_UDRIEx) || !TEST(M_UCSRxA, M_TXCx)) {
        if (!ISRS_ENABLED()) {
          // Interrupts are globally disabled, but the DR empty
          // interrupt should be enabled, so poll the DR empty flag to
          // prevent deadlock
          if (TEST(M_UCSRxA, M_UDREx))
            _tx_udr_empty_irq();
        }
        sw_barrier();
      }
      // If we get here, nothing is queued anymore (DRIE is disabled) and
      // the hardware finished transmission (TXC is set).
    }

  #else // TX_BUFFER_SIZE == 0

    void MarlinSerial::write(const uint8_t c) {
      while (!TEST(M_UCSRxA, M_UDREx)) sw_barrier();
      M_UDRx = c;
    }

  #endif // TX_BUFFER_SIZE == 0

  /**
   * Imports from print.h
   */

  void MarlinSerial::print(char c, int base) {
    print((long)c, base);
  }

  void MarlinSerial::print(unsigned char b, int base) {
    print((unsigned long)b, base);
  }

  void MarlinSerial::print(int n, int base) {
    print((long)n, base);
  }

  void MarlinSerial::print(unsigned int n, int base) {
    print((unsigned long)n, base);
  }

  void MarlinSerial::print(long n, int base) {
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

  // Private Methods

  void MarlinSerial::printNumber(unsigned long n, uint8_t base) {
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

  void MarlinSerial::printFloat(double number, uint8_t digits) {
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
  MarlinSerial customizedSerial;

#endif // !USBCON && (UBRRH || UBRR0H || UBRR1H || UBRR2H || UBRR3H)

// For AT90USB targets use the UART for BT interfacing
#if defined(USBCON) && ENABLED(BLUETOOTH)
  HardwareSerial bluetoothSerial;
#endif

#endif // __AVR__
