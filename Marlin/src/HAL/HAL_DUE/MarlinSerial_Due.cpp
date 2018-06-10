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
 * MarlinSerial_Due.cpp - Hardware serial library for Arduino DUE
 * Copyright (c) 2017 Eduardo José Tagle. All right reserved
 * Based on MarlinSerial for AVR, copyright (c) 2006 Nicholas Zambetti.  All right reserved.
 */
#ifdef ARDUINO_ARCH_SAM

#include "../../inc/MarlinConfig.h"

#include "MarlinSerial_Due.h"
#include "InterruptVectors_Due.h"
#include "../../Marlin.h"

// If not using the USB port as serial port
#if SERIAL_PORT >= 0

  // Based on selected port, use the proper configuration
  #if SERIAL_PORT == 0
    #define HWUART UART
    #define HWUART_IRQ UART_IRQn
    #define HWUART_IRQ_ID ID_UART
  #elif SERIAL_PORT == 1
    #define HWUART ((Uart*)USART0)
    #define HWUART_IRQ USART0_IRQn
    #define HWUART_IRQ_ID ID_USART0
  #elif SERIAL_PORT == 2
    #define HWUART ((Uart*)USART1)
    #define HWUART_IRQ USART1_IRQn
    #define HWUART_IRQ_ID ID_USART1
  #elif SERIAL_PORT == 3
    #define HWUART ((Uart*)USART2)
    #define HWUART_IRQ USART2_IRQn
    #define HWUART_IRQ_ID ID_USART2
  #elif SERIAL_PORT == 4
    #define HWUART ((Uart*)USART3)
    #define HWUART_IRQ USART3_IRQn
    #define HWUART_IRQ_ID ID_USART3
  #endif

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

  ring_buffer_r rx_buffer = { { 0 }, 0, 0 };
  #if TX_BUFFER_SIZE > 0
    ring_buffer_t tx_buffer = { { 0 }, 0, 0 };
  #endif
  static bool _written;

  #if ENABLED(SERIAL_XON_XOFF)
    constexpr uint8_t XON_XOFF_CHAR_SENT = 0x80,  // XON / XOFF Character was sent
                      XON_XOFF_CHAR_MASK = 0x1F;  // XON / XOFF character to send
    // XON / XOFF character definitions
    constexpr uint8_t XON_CHAR  = 17, XOFF_CHAR = 19;
    uint8_t xon_xoff_state = XON_XOFF_CHAR_SENT | XON_CHAR;

    // Validate that RX buffer size is at least 4096 bytes- According to several experiments, on
    // the original Arduino Due that uses a ATmega16U2 as USB to serial bridge, due to the introduced
    // latencies, at least 2959 bytes of RX buffering (when transmitting at 250kbits/s) are required
    // to avoid overflows.

    #if RX_BUFFER_SIZE < 4096
      #error Arduino DUE requires at least 4096 bytes of RX buffer to avoid buffer overflows when using XON/XOFF handshake
    #endif
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

  // (called with RX interrupts disabled)
  FORCE_INLINE void store_rxd_char() {

    #if ENABLED(EMERGENCY_PARSER)
      static EmergencyParser::State emergency_state; // = EP_RESET
    #endif

    // Get the tail - Nothing can alter its value while we are at this ISR
    const ring_buffer_pos_t t = rx_buffer.tail;

    // Get the head pointer
    ring_buffer_pos_t h = rx_buffer.head;

    // Get the next element
    ring_buffer_pos_t i = (ring_buffer_pos_t)(h + 1) & (ring_buffer_pos_t)(RX_BUFFER_SIZE - 1);

    // Read the character from the USART
    uint8_t c = HWUART->UART_RHR;

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
      const ring_buffer_pos_t rx_count = (ring_buffer_pos_t)(h - t) & (ring_buffer_pos_t)(RX_BUFFER_SIZE - 1);
      // Calculate count of bytes stored into the RX buffer

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

          // At this point, definitely no TX interrupt was executing, since the TX isr can't be preempted.
          // Don't enable the TX interrupt here as a means to trigger the XOFF char, because if it happens
          // to be in the middle of trying to disable the RX interrupt in the main program, eventually the
          // enabling of the TX interrupt could be undone. The ONLY reliable thing this can do to ensure
          // the sending of the XOFF char is to send it HERE AND NOW.

          // About to send the XOFF char
          xon_xoff_state = XOFF_CHAR | XON_XOFF_CHAR_SENT;

          // Wait until the TX register becomes empty and send it - Here there could be a problem
          // - While waiting for the TX register to empty, the RX register could receive a new
          //   character. This must also handle that situation!
          uint32_t status;
          while (!((status = HWUART->UART_SR) & UART_SR_TXRDY)) {

            if (status & UART_SR_RXRDY) {
              // We received a char while waiting for the TX buffer to be empty - Receive and process it!

              i = (ring_buffer_pos_t)(h + 1) & (ring_buffer_pos_t)(RX_BUFFER_SIZE - 1);

              // Read the character from the USART
              c = HWUART->UART_RHR;

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

          HWUART->UART_THR = XOFF_CHAR;

          // At this point there could be a race condition between the write() function
          // and this sending of the XOFF char. This interrupt could happen between the
          // wait to be empty TX buffer loop and the actual write of the character. Since
          // the TX buffer is full because it's sending the XOFF char, the only way to be
          // sure the write() function will succeed is to wait for the XOFF char to be
          // completely sent. Since an extra character could be received during the wait
          // it must also be handled!
          while (!((status = HWUART->UART_SR) & UART_SR_TXRDY)) {

            if (status & UART_SR_RXRDY) {
              // A char arrived while waiting for the TX buffer to be empty - Receive and process it!

              i = (ring_buffer_pos_t)(h + 1) & (ring_buffer_pos_t)(RX_BUFFER_SIZE - 1);

              // Read the character from the USART
              c = HWUART->UART_RHR;

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

    // Store the new head value
    rx_buffer.head = h;
  }

  #if TX_BUFFER_SIZE > 0

    FORCE_INLINE void _tx_thr_empty_irq(void) {
      // Read positions
      uint8_t t = tx_buffer.tail;
      const uint8_t h = tx_buffer.head;

      #if ENABLED(SERIAL_XON_XOFF)
        // If an XON char is pending to be sent, do it now
        if (xon_xoff_state == XON_CHAR) {

          // Send the character
          HWUART->UART_THR = XON_CHAR;

          // Remember we sent it.
          xon_xoff_state = XON_CHAR | XON_XOFF_CHAR_SENT;

          // If nothing else to transmit, just disable TX interrupts.
          if (h == t) HWUART->UART_IDR = UART_IDR_TXRDY;

          return;
        }
      #endif

      // If nothing to transmit, just disable TX interrupts. This could
      // happen as the result of the non atomicity of the disabling of RX
      // interrupts that could end reenabling TX interrupts as a side effect.
      if (h == t) {
        HWUART->UART_IDR = UART_IDR_TXRDY;
        return;
      }

      // There is something to TX, Send the next byte
      const uint8_t c = tx_buffer.buffer[t];
      t = (t + 1) & (TX_BUFFER_SIZE - 1);
      HWUART->UART_THR = c;
      tx_buffer.tail = t;

      // Disable interrupts if there is nothing to transmit following this byte
      if (h == t) HWUART->UART_IDR = UART_IDR_TXRDY;
    }

  #endif // TX_BUFFER_SIZE > 0

  static void UART_ISR(void) {
    const uint32_t status = HWUART->UART_SR;

    // Data received?
    if (status & UART_SR_RXRDY) store_rxd_char();

    #if TX_BUFFER_SIZE > 0
      // Something to send, and TX interrupts are enabled (meaning something to send)?
      if ((status & UART_SR_TXRDY) && (HWUART->UART_IMR & UART_IMR_TXRDY)) _tx_thr_empty_irq();
    #endif

    // Acknowledge errors
    if ((status & UART_SR_OVRE) || (status & UART_SR_FRAME)) {

      #if ENABLED(SERIAL_STATS_DROPPED_RX)
        if (status & UART_SR_OVRE && !++rx_dropped_bytes) --rx_dropped_bytes;
      #endif

      #if ENABLED(SERIAL_STATS_RX_BUFFER_OVERRUNS)
        if (status & UART_SR_OVRE && !++rx_buffer_overruns) --rx_buffer_overruns;
      #endif

      #if ENABLED(SERIAL_STATS_RX_FRAMING_ERRORS)
        if (status & UART_SR_FRAME && !++rx_framing_errors) --rx_framing_errors;
      #endif

      // TODO: error reporting outside ISR
      HWUART->UART_CR = UART_CR_RSTSTA;
    }
  }

  // Public Methods

  void MarlinSerial::begin(const long baud_setting) {

    // Disable UART interrupt in NVIC
    NVIC_DisableIRQ( HWUART_IRQ );

    // We NEED memory barriers to ensure Interrupts are actually disabled!
    // ( https://dzone.com/articles/nvic-disabling-interrupts-on-arm-cortex-m-and-the )
    __DSB();
    __ISB();

    // Disable clock
    pmc_disable_periph_clk( HWUART_IRQ_ID );

    // Configure PMC
    pmc_enable_periph_clk( HWUART_IRQ_ID );

    // Disable PDC channel
    HWUART->UART_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;

    // Reset and disable receiver and transmitter
    HWUART->UART_CR = UART_CR_RSTRX | UART_CR_RSTTX | UART_CR_RXDIS | UART_CR_TXDIS;

    // Configure mode: 8bit, No parity, 1 bit stop
    HWUART->UART_MR = UART_MR_CHMODE_NORMAL | US_MR_CHRL_8_BIT | US_MR_NBSTOP_1_BIT | UART_MR_PAR_NO;

    // Configure baudrate (asynchronous, no oversampling)
    HWUART->UART_BRGR = (SystemCoreClock / (baud_setting << 4));

    // Configure interrupts
    HWUART->UART_IDR = 0xFFFFFFFF;
    HWUART->UART_IER = UART_IER_RXRDY | UART_IER_OVRE | UART_IER_FRAME;

    // Install interrupt handler
    install_isr(HWUART_IRQ, UART_ISR);

    // Configure priority. We need a very high priority to avoid losing characters
    // and we need to be able to preempt the Stepper ISR and everything else!
    // (this could probably be fixed by using DMA with the Serial port)
    NVIC_SetPriority(HWUART_IRQ, 1);

    // Enable UART interrupt in NVIC
    NVIC_EnableIRQ(HWUART_IRQ);

    // Enable receiver and transmitter
    HWUART->UART_CR = UART_CR_RXEN | UART_CR_TXEN;

    #if TX_BUFFER_SIZE > 0
      _written = false;
    #endif
  }

  void MarlinSerial::end() {
    // Disable UART interrupt in NVIC
    NVIC_DisableIRQ( HWUART_IRQ );

    // We NEED memory barriers to ensure Interrupts are actually disabled!
    // ( https://dzone.com/articles/nvic-disabling-interrupts-on-arm-cortex-m-and-the )
    __DSB();
    __ISB();

    pmc_disable_periph_clk( HWUART_IRQ_ID );
  }

  int MarlinSerial::peek(void) {
    const int v = rx_buffer.head == rx_buffer.tail ? -1 : rx_buffer.buffer[rx_buffer.tail];
    return v;
  }

  int MarlinSerial::read(void) {

    const ring_buffer_pos_t h = rx_buffer.head;
    ring_buffer_pos_t t = rx_buffer.tail;

    if (h == t) return -1;

    int v = rx_buffer.buffer[t];
    t = (ring_buffer_pos_t)(t + 1) & (RX_BUFFER_SIZE - 1);

    // Advance tail
    rx_buffer.tail = t;

    #if ENABLED(SERIAL_XON_XOFF)
      // If the XOFF char was sent, or about to be sent...
      if ((xon_xoff_state & XON_XOFF_CHAR_MASK) == XOFF_CHAR) {
        // Get count of bytes in the RX buffer
        const ring_buffer_pos_t rx_count = (ring_buffer_pos_t)(h - t) & (ring_buffer_pos_t)(RX_BUFFER_SIZE - 1);
        // When below 10% of RX buffer capacity, send XON before running out of RX buffer bytes
        if (rx_count < (RX_BUFFER_SIZE) / 10) {
          #if TX_BUFFER_SIZE > 0
            // Signal we want an XON character to be sent.
            xon_xoff_state = XON_CHAR;
            // Enable TX isr.
            HWUART->UART_IER = UART_IER_TXRDY;
          #else
            // If not using TX interrupts, we must send the XON char now
            xon_xoff_state = XON_CHAR | XON_XOFF_CHAR_SENT;
            while (!(HWUART->UART_SR & UART_SR_TXRDY)) sw_barrier();
            HWUART->UART_THR = XON_CHAR;
          #endif
        }
      }
    #endif

    return v;
  }

  ring_buffer_pos_t MarlinSerial::available(void) {
    const ring_buffer_pos_t h = rx_buffer.head, t = rx_buffer.tail;
    return (ring_buffer_pos_t)(RX_BUFFER_SIZE + h - t) & (RX_BUFFER_SIZE - 1);
  }

  void MarlinSerial::flush(void) {
    rx_buffer.tail = rx_buffer.head;

    #if ENABLED(SERIAL_XON_XOFF)
      if ((xon_xoff_state & XON_XOFF_CHAR_MASK) == XOFF_CHAR) {
        #if TX_BUFFER_SIZE > 0
          // Signal we want an XON character to be sent.
          xon_xoff_state = XON_CHAR;
          // Enable TX isr.
          HWUART->UART_IER = UART_IER_TXRDY;
        #else
          // If not using TX interrupts, we must send the XON char now
          xon_xoff_state = XON_CHAR | XON_XOFF_CHAR_SENT;
          while (!(HWUART->UART_SR & UART_SR_TXRDY)) sw_barrier();
          HWUART->UART_THR = XON_CHAR;
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
      // XOFF char at the RX isr, but it is properly handled there
      if (!(HWUART->UART_IMR & UART_IMR_TXRDY) && (HWUART->UART_SR & UART_SR_TXRDY)) {
        HWUART->UART_THR = c;
        return;
      }

      const uint8_t i = (tx_buffer.head + 1) & (TX_BUFFER_SIZE - 1);

      // If global interrupts are disabled (as the result of being called from an ISR)...
      if (!ISRS_ENABLED()) {

        // Make room by polling if it is possible to transmit, and do so!
        while (i == tx_buffer.tail) {
          // If we can transmit another byte, do it.
          if (HWUART->UART_SR & UART_SR_TXRDY) _tx_thr_empty_irq();
          // Make sure compiler rereads tx_buffer.tail
          sw_barrier();
        }
      }
      else {
        // Interrupts are enabled, just wait until there is space
        while (i == tx_buffer.tail) sw_barrier();
      }

      // Store new char. head is always safe to move
      tx_buffer.buffer[tx_buffer.head] = c;
      tx_buffer.head = i;

      // Enable TX isr - Non atomic, but it will eventually enable TX isr
      HWUART->UART_IER = UART_IER_TXRDY;
    }

    void MarlinSerial::flushTX(void) {
      // TX

      // If we have never written a byte, no need to flush. This special
      // case is needed since there is no way to force the TXC (transmit
      // complete) bit to 1 during initialization
      if (!_written) return;

      // If global interrupts are disabled (as the result of being called from an ISR)...
      if (!ISRS_ENABLED()) {

        // Wait until everything was transmitted - We must do polling, as interrupts are disabled
        while (tx_buffer.head != tx_buffer.tail || !(HWUART->UART_SR & UART_SR_TXEMPTY)) {
          // If there is more space, send an extra character
          if (HWUART->UART_SR & UART_SR_TXRDY) _tx_thr_empty_irq();
          sw_barrier();
        }

      }
      else {
        // Wait until everything was transmitted
        while (tx_buffer.head != tx_buffer.tail || !(HWUART->UART_SR & UART_SR_TXEMPTY)) sw_barrier();
      }

      // At this point nothing is queued anymore (DRIE is disabled) and
      // the hardware finished transmission (TXC is set).
    }

  #else // TX_BUFFER_SIZE == 0

    void MarlinSerial::write(const uint8_t c) {
      _written = true;
      while (!(HWUART->UART_SR & UART_SR_TXRDY)) sw_barrier();
      HWUART->UART_THR = c;
    }

    void MarlinSerial::flushTX(void) {
      // TX

      // No bytes written, no need to flush. This special case is needed since there's
      // no way to force the TXC (transmit complete) bit to 1 during initialization.
      if (!_written) return;

      // Wait until everything was transmitted
      while (!(HWUART->UART_SR & UART_SR_TXEMPTY)) sw_barrier();

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
    for (uint8_t i = 0; i < digits; ++i) rounding *= 0.1;
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
#endif

#endif // ARDUINO_ARCH_SAM
