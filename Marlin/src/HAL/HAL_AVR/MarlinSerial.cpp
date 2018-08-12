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
 * Modified 10 June 2018 by Eduardo José Tagle (See #10991)
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
    #endif
    static bool _written;
  #endif

  #if ENABLED(SERIAL_XON_XOFF)
    constexpr uint8_t XON_XOFF_CHAR_SENT = 0x80,  // XON / XOFF Character was sent
                      XON_XOFF_CHAR_MASK = 0x1F;  // XON / XOFF character to send
    // XON / XOFF character definitions
    constexpr uint8_t XON_CHAR  = 17, XOFF_CHAR = 19;
    uint8_t xon_xoff_state = XON_XOFF_CHAR_SENT | XON_CHAR;
  #endif

  #if ENABLED(SERIAL_STATS_DROPPED_RX)
    uint8_t rx_dropped_bytes = 0;
  #endif

  #if ENABLED(SERIAL_STATS_RX_BUFFER_OVERRUNS)
    uint8_t rx_buffer_overruns = 0;
  #endif

  #if ENABLED(SERIAL_STATS_RX_FRAMING_ERRORS)
    uint8_t rx_framing_errors = 0;
  #endif

  #if ENABLED(SERIAL_STATS_MAX_RX_QUEUED)
    ring_buffer_pos_t rx_max_enqueued = 0;
  #endif

  // A SW memory barrier, to ensure GCC does not overoptimize loops
  #define sw_barrier() asm volatile("": : :"memory");

  #if ENABLED(EMERGENCY_PARSER)
    #include "../../feature/emergency_parser.h"
  #endif

  // "Atomically" read the RX head index value without disabling interrupts:
  // This MUST be called with RX interrupts enabled, and CAN'T be called
  // from the RX ISR itself!
  FORCE_INLINE ring_buffer_pos_t atomic_read_rx_head() {
    #if RX_BUFFER_SIZE > 256
      // Keep reading until 2 consecutive reads return the same value,
      // meaning there was no update in-between caused by an interrupt.
      // This works because serial RX interrupts happen at a slower rate
      // than successive reads of a variable, so 2 consecutive reads with
      // the same value means no interrupt updated it.
      ring_buffer_pos_t vold, vnew = rx_buffer.head;
      sw_barrier();
      do {
        vold = vnew;
        vnew = rx_buffer.head;
        sw_barrier();
      } while (vold != vnew);
      return vnew;
    #else
      // With an 8bit index, reads are always atomic. No need for special handling
      return rx_buffer.head;
    #endif
  }

  #if RX_BUFFER_SIZE > 256
    static volatile bool rx_tail_value_not_stable = false;
    static volatile uint16_t rx_tail_value_backup = 0;
  #endif

  // Set RX tail index, taking into account the RX ISR could interrupt
  //  the write to this variable in the middle - So a backup strategy
  //  is used to ensure reads of the correct values.
  //    -Must NOT be called from the RX ISR -
  FORCE_INLINE void atomic_set_rx_tail(ring_buffer_pos_t value) {
    #if RX_BUFFER_SIZE > 256
      // Store the new value in the backup
      rx_tail_value_backup = value;
      sw_barrier();
      // Flag we are about to change the true value
      rx_tail_value_not_stable = true;
      sw_barrier();
      // Store the new value
      rx_buffer.tail = value;
      sw_barrier();
      // Signal the new value is completely stored into the value
      rx_tail_value_not_stable = false;
      sw_barrier();
    #else
      rx_buffer.tail = value;
    #endif
  }

  // Get the RX tail index, taking into account the read could be
  //  interrupting in the middle of the update of that index value
  //    -Called from the RX ISR -
  FORCE_INLINE ring_buffer_pos_t atomic_read_rx_tail() {
    #if RX_BUFFER_SIZE > 256
      // If the true index is being modified, return the backup value
      if (rx_tail_value_not_stable) return rx_tail_value_backup;
    #endif
    // The true index is stable, return it
    return rx_buffer.tail;
  }

  // (called with RX interrupts disabled)
  FORCE_INLINE void store_rxd_char() {

    #if ENABLED(EMERGENCY_PARSER)
      static EmergencyParser::State emergency_state; // = EP_RESET
    #endif

    // Get the tail - Nothing can alter its value while this ISR is executing, but there's
    // a chance that this ISR interrupted the main process while it was updating the index.
    // The backup mechanism ensures the correct value is always returned.
    const ring_buffer_pos_t t = atomic_read_rx_tail();

    // Get the head pointer - This ISR is the only one that modifies its value, so it's safe to read here
    ring_buffer_pos_t h = rx_buffer.head;

    // Get the next element
    ring_buffer_pos_t i = (ring_buffer_pos_t)(h + 1) & (ring_buffer_pos_t)(RX_BUFFER_SIZE - 1);

    // This must read the M_UCSRxA register before reading the received byte to detect error causes
    #if ENABLED(SERIAL_STATS_DROPPED_RX)
      if (TEST(M_UCSRxA, M_DORx) && !++rx_dropped_bytes) --rx_dropped_bytes;
    #endif

    #if ENABLED(SERIAL_STATS_RX_BUFFER_OVERRUNS)
      if (TEST(M_UCSRxA, M_DORx) && !++rx_buffer_overruns) --rx_buffer_overruns;
    #endif

    #if ENABLED(SERIAL_STATS_RX_FRAMING_ERRORS)
      if (TEST(M_UCSRxA, M_FEx) && !++rx_framing_errors) --rx_framing_errors;
    #endif

    // Read the character from the USART
    uint8_t c = M_UDRx;

    #if ENABLED(EMERGENCY_PARSER)
      emergency_parser.update(emergency_state, c);
    #endif

    // If the character is to be stored at the index just before the tail
    // (such that the head would advance to the current tail), the RX FIFO is
    // full, so don't write the character or advance the head.
    if (i != t) {
      rx_buffer.buffer[h] = c;
      h = i;
    }
    #if ENABLED(SERIAL_STATS_DROPPED_RX)
      else if (!++rx_dropped_bytes) --rx_dropped_bytes;
    #endif

    #if ENABLED(SERIAL_STATS_MAX_RX_QUEUED)
      // Calculate count of bytes stored into the RX buffer
      const ring_buffer_pos_t rx_count = (ring_buffer_pos_t)(h - t) & (ring_buffer_pos_t)(RX_BUFFER_SIZE - 1);

      // Keep track of the maximum count of enqueued bytes
      NOLESS(rx_max_enqueued, rx_count);
    #endif

    #if ENABLED(SERIAL_XON_XOFF)
      // If the last char that was sent was an XON
      if ((xon_xoff_state & XON_XOFF_CHAR_MASK) == XON_CHAR) {

        // Bytes stored into the RX buffer
        const ring_buffer_pos_t rx_count = (ring_buffer_pos_t)(h - t) & (ring_buffer_pos_t)(RX_BUFFER_SIZE - 1);

        // If over 12.5% of RX buffer capacity, send XOFF before running out of
        // RX buffer space .. 325 bytes @ 250kbits/s needed to let the host react
        // and stop sending bytes. This translates to 13mS propagation time.
        if (rx_count >= (RX_BUFFER_SIZE) / 8) {

          // At this point, definitely no TX interrupt was executing, since the TX ISR can't be preempted.
          // Don't enable the TX interrupt here as a means to trigger the XOFF char, because if it happens
          // to be in the middle of trying to disable the RX interrupt in the main program, eventually the
          // enabling of the TX interrupt could be undone. The ONLY reliable thing this can do to ensure
          // the sending of the XOFF char is to send it HERE AND NOW.

          // About to send the XOFF char
          xon_xoff_state = XOFF_CHAR | XON_XOFF_CHAR_SENT;

          // Wait until the TX register becomes empty and send it - Here there could be a problem
          // - While waiting for the TX register to empty, the RX register could receive a new
          //   character. This must also handle that situation!
          while (!TEST(M_UCSRxA, M_UDREx)) {

            if (TEST(M_UCSRxA,M_RXCx)) {
              // A char arrived while waiting for the TX buffer to be empty - Receive and process it!

              i = (ring_buffer_pos_t)(h + 1) & (ring_buffer_pos_t)(RX_BUFFER_SIZE - 1);

              // Read the character from the USART
              c = M_UDRx;

              #if ENABLED(EMERGENCY_PARSER)
                emergency_parser.update(emergency_state, c);
              #endif

              // If the character is to be stored at the index just before the tail
              // (such that the head would advance to the current tail), the FIFO is
              // full, so don't write the character or advance the head.
              if (i != t) {
                rx_buffer.buffer[h] = c;
                h = i;
              }
              #if ENABLED(SERIAL_STATS_DROPPED_RX)
                else if (!++rx_dropped_bytes) --rx_dropped_bytes;
              #endif
            }
            sw_barrier();
          }

          M_UDRx = XOFF_CHAR;

          // Clear the TXC bit -- "can be cleared by writing a one to its bit
          // location". This makes sure flush() won't return until the bytes
          // actually got written
          SBI(M_UCSRxA, M_TXCx);

          // At this point there could be a race condition between the write() function
          // and this sending of the XOFF char. This interrupt could happen between the
          // wait to be empty TX buffer loop and the actual write of the character. Since
          // the TX buffer is full because it's sending the XOFF char, the only way to be
          // sure the write() function will succeed is to wait for the XOFF char to be
          // completely sent. Since an extra character could be received during the wait
          // it must also be handled!
          while (!TEST(M_UCSRxA, M_UDREx)) {

            if (TEST(M_UCSRxA,M_RXCx)) {
              // A char arrived while waiting for the TX buffer to be empty - Receive and process it!

              i = (ring_buffer_pos_t)(h + 1) & (ring_buffer_pos_t)(RX_BUFFER_SIZE - 1);

              // Read the character from the USART
              c = M_UDRx;

              #if ENABLED(EMERGENCY_PARSER)
                emergency_parser.update(emergency_state, c);
              #endif

              // If the character is to be stored at the index just before the tail
              // (such that the head would advance to the current tail), the FIFO is
              // full, so don't write the character or advance the head.
              if (i != t) {
                rx_buffer.buffer[h] = c;
                h = i;
              }
              #if ENABLED(SERIAL_STATS_DROPPED_RX)
                else if (!++rx_dropped_bytes) --rx_dropped_bytes;
              #endif
            }
            sw_barrier();
          }

          // At this point everything is ready. The write() function won't
          // have any issues writing to the UART TX register if it needs to!
        }
      }
    #endif // SERIAL_XON_XOFF

    // Store the new head value - The main loop will retry until the value is stable
    rx_buffer.head = h;
  }

  #if TX_BUFFER_SIZE > 0

    // (called with TX irqs disabled)
    FORCE_INLINE void _tx_udr_empty_irq(void) {

      // Read positions
      uint8_t t = tx_buffer.tail;
      const uint8_t h = tx_buffer.head;

      #if ENABLED(SERIAL_XON_XOFF)
        // If an XON char is pending to be sent, do it now
        if (xon_xoff_state == XON_CHAR) {

          // Send the character
          M_UDRx = XON_CHAR;

          // clear the TXC bit -- "can be cleared by writing a one to its bit
          // location". This makes sure flush() won't return until the bytes
          // actually got written
          SBI(M_UCSRxA, M_TXCx);

          // Remember we sent it.
          xon_xoff_state = XON_CHAR | XON_XOFF_CHAR_SENT;

          // If nothing else to transmit, just disable TX interrupts.
          if (h == t) CBI(M_UCSRxB, M_UDRIEx); // (Non-atomic, could be reenabled by the main program, but eventually this will succeed)

          return;
        }
      #endif

      // If nothing to transmit, just disable TX interrupts. This could
      // happen as the result of the non atomicity of the disabling of RX
      // interrupts that could end reenabling TX interrupts as a side effect.
      if (h == t) {
        CBI(M_UCSRxB, M_UDRIEx); // (Non-atomic, could be reenabled by the main program, but eventually this will succeed)
        return;
      }

      // There is something to TX, Send the next byte
      const uint8_t c = tx_buffer.buffer[t];
      t = (t + 1) & (TX_BUFFER_SIZE - 1);
      M_UDRx = c;
      tx_buffer.tail = t;

      // Clear the TXC bit (by writing a one to its bit location).
      // Ensures flush() won't return until the bytes are actually written/
      SBI(M_UCSRxA, M_TXCx);

      // Disable interrupts if there is nothing to transmit following this byte
      if (h == t) CBI(M_UCSRxB, M_UDRIEx); // (Non-atomic, could be reenabled by the main program, but eventually this will succeed)
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
    #endif
    _written = false;
  }

  void MarlinSerial::end() {
    CBI(M_UCSRxB, M_RXENx);
    CBI(M_UCSRxB, M_TXENx);
    CBI(M_UCSRxB, M_RXCIEx);
    CBI(M_UCSRxB, M_UDRIEx);
  }

  int MarlinSerial::peek(void) {
    const ring_buffer_pos_t h = atomic_read_rx_head(), t = rx_buffer.tail;
    return h == t ? -1 : rx_buffer.buffer[t];
  }

  int MarlinSerial::read(void) {
    const ring_buffer_pos_t h = atomic_read_rx_head();

    // Read the tail. Main thread owns it, so it is safe to directly read it
    ring_buffer_pos_t t = rx_buffer.tail;

    // If nothing to read, return now
    if (h == t) return -1;

    // Get the next char
    const int v = rx_buffer.buffer[t];
    t = (ring_buffer_pos_t)(t + 1) & (RX_BUFFER_SIZE - 1);

    // Advance tail - Making sure the RX ISR will always get an stable value, even
    // if it interrupts the writing of the value of that variable in the middle.
    atomic_set_rx_tail(t);

    #if ENABLED(SERIAL_XON_XOFF)
      // If the XOFF char was sent, or about to be sent...
      if ((xon_xoff_state & XON_XOFF_CHAR_MASK) == XOFF_CHAR) {
        // Get count of bytes in the RX buffer
        const ring_buffer_pos_t rx_count = (ring_buffer_pos_t)(h - t) & (ring_buffer_pos_t)(RX_BUFFER_SIZE - 1);
        if (rx_count < (RX_BUFFER_SIZE) / 10) {
          #if TX_BUFFER_SIZE > 0
            // Signal we want an XON character to be sent.
            xon_xoff_state = XON_CHAR;
            // Enable TX ISR. Non atomic, but it will eventually enable them
            SBI(M_UCSRxB, M_UDRIEx);
          #else
            // If not using TX interrupts, we must send the XON char now
            xon_xoff_state = XON_CHAR | XON_XOFF_CHAR_SENT;
            while (!TEST(M_UCSRxA, M_UDREx)) sw_barrier();
            M_UDRx = XON_CHAR;
          #endif
        }
      }
    #endif

    return v;
  }

  ring_buffer_pos_t MarlinSerial::available(void) {
    const ring_buffer_pos_t h = atomic_read_rx_head(), t = rx_buffer.tail;
    return (ring_buffer_pos_t)(RX_BUFFER_SIZE + h - t) & (RX_BUFFER_SIZE - 1);
  }

  void MarlinSerial::flush(void) {

    // Set the tail to the head:
    //  - Read the RX head index in a safe way. (See atomic_read_rx_head.)
    //  - Set the tail, making sure the RX ISR will always get a stable value, even
    //    if it interrupts the writing of the value of that variable in the middle.
    atomic_set_rx_tail(atomic_read_rx_head());

    #if ENABLED(SERIAL_XON_XOFF)
      // If the XOFF char was sent, or about to be sent...
      if ((xon_xoff_state & XON_XOFF_CHAR_MASK) == XOFF_CHAR) {
        #if TX_BUFFER_SIZE > 0
          // Signal we want an XON character to be sent.
          xon_xoff_state = XON_CHAR;
          // Enable TX ISR. Non atomic, but it will eventually enable it.
          SBI(M_UCSRxB, M_UDRIEx);
        #else
          // If not using TX interrupts, we must send the XON char now
          xon_xoff_state = XON_CHAR | XON_XOFF_CHAR_SENT;
          while (!TEST(M_UCSRxA, M_UDREx)) sw_barrier();
          M_UDRx = XON_CHAR;
        #endif
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
      // Yes, there is a race condition between the sending of the
      // XOFF char at the RX ISR, but it is properly handled there
      if (!TEST(M_UCSRxB, M_UDRIEx) && TEST(M_UCSRxA, M_UDREx)) {
        M_UDRx = c;

        // clear the TXC bit -- "can be cleared by writing a one to its bit
        // location". This makes sure flush() won't return until the bytes
        // actually got written
        SBI(M_UCSRxA, M_TXCx);
        return;
      }

      const uint8_t i = (tx_buffer.head + 1) & (TX_BUFFER_SIZE - 1);

      // If global interrupts are disabled (as the result of being called from an ISR)...
      if (!ISRS_ENABLED()) {

        // Make room by polling if it is possible to transmit, and do so!
        while (i == tx_buffer.tail) {

          // If we can transmit another byte, do it.
          if (TEST(M_UCSRxA, M_UDREx)) _tx_udr_empty_irq();

          // Make sure compiler rereads tx_buffer.tail
          sw_barrier();
        }
      }
      else {
        // Interrupts are enabled, just wait until there is space
        while (i == tx_buffer.tail) { sw_barrier(); }
      }

      // Store new char. head is always safe to move
      tx_buffer.buffer[tx_buffer.head] = c;
      tx_buffer.head = i;

      // Enable TX ISR - Non atomic, but it will eventually enable TX ISR
      SBI(M_UCSRxB, M_UDRIEx);
    }

    void MarlinSerial::flushTX(void) {
      // No bytes written, no need to flush. This special case is needed since there's
      // no way to force the TXC (transmit complete) bit to 1 during initialization.
      if (!_written) return;

      // If global interrupts are disabled (as the result of being called from an ISR)...
      if (!ISRS_ENABLED()) {

        // Wait until everything was transmitted - We must do polling, as interrupts are disabled
        while (tx_buffer.head != tx_buffer.tail || !TEST(M_UCSRxA, M_TXCx)) {

          // If there is more space, send an extra character
          if (TEST(M_UCSRxA, M_UDREx))
            _tx_udr_empty_irq();

          sw_barrier();
        }

      }
      else {
        // Wait until everything was transmitted
        while (tx_buffer.head != tx_buffer.tail || !TEST(M_UCSRxA, M_TXCx)) sw_barrier();
      }

      // At this point nothing is queued anymore (DRIE is disabled) and
      // the hardware finished transmission (TXC is set).
    }

  #else // TX_BUFFER_SIZE == 0

    void MarlinSerial::write(const uint8_t c) {
      _written = true;
      while (!TEST(M_UCSRxA, M_UDREx)) sw_barrier();
      M_UDRx = c;
    }

    void MarlinSerial::flushTX(void) {
      // No bytes written, no need to flush. This special case is needed since there's
      // no way to force the TXC (transmit complete) bit to 1 during initialization.
      if (!_written) return;

      // Wait until everything was transmitted
      while (!TEST(M_UCSRxA, M_TXCx)) sw_barrier();

      // At this point nothing is queued anymore (DRIE is disabled) and
      // the hardware finished transmission (TXC is set).
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
    if (base == 0) write(n);
    else if (base == 10) {
      if (n < 0) { print('-'); n = -n; }
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
