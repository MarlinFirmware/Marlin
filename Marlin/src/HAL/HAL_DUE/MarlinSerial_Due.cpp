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
    static bool _written;
  #endif

  #if ENABLED(SERIAL_XON_XOFF)
    constexpr uint8_t XON_XOFF_CHAR_SENT = 0x80;  // XON / XOFF Character was sent
    constexpr uint8_t XON_XOFF_CHAR_MASK = 0x1F;  // XON / XOFF character to send
    // XON / XOFF character definitions
    constexpr uint8_t XON_CHAR  = 17;
    constexpr uint8_t XOFF_CHAR = 19;
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

  #if ENABLED(SERIAL_STATS_MAX_RX_QUEUED)
    ring_buffer_pos_t rx_max_enqueued = 0;
  #endif

  // A SW memory barrier, to ensure GCC does not overoptimize loops
  #define sw_barrier() asm volatile("": : :"memory");

  #if ENABLED(EMERGENCY_PARSER)
    #include "../../feature/emergency_parser.h"
  #endif

  FORCE_INLINE void store_rxd_char() {

    const ring_buffer_pos_t h = rx_buffer.head,
                            i = (ring_buffer_pos_t)(h + 1) & (ring_buffer_pos_t)(RX_BUFFER_SIZE - 1);

      // Read the character
    const uint8_t c = HWUART->UART_RHR;

    // If the character is to be stored at the index just before the tail
    // (such that the head would advance to the current tail), the buffer is
    // critical, so don't write the character or advance the head.
    if (i != rx_buffer.tail) {
      rx_buffer.buffer[h] = c;
      rx_buffer.head = i;
    }
    #if ENABLED(SERIAL_STATS_DROPPED_RX)
      else if (!++rx_dropped_bytes) ++rx_dropped_bytes;
    #endif

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
          if (!(HWUART->UART_IMR & UART_IMR_TXRDY) && (HWUART->UART_SR & UART_SR_TXRDY)) {
            // Send an XOFF character
            HWUART->UART_THR = XOFF_CHAR;

            // And remember it was sent
            xon_xoff_state = XOFF_CHAR | XON_XOFF_CHAR_SENT;
          }
          else {
            // TX interrupts disabled, but buffer still not empty ... or
            // TX interrupts enabled. Reenable TX ints and schedule XOFF
            // character to be sent
            #if TX_BUFFER_SIZE > 0
              HWUART->UART_IER = UART_IER_TXRDY;
              xon_xoff_state = XOFF_CHAR;
            #else
              // We are not using TX interrupts, we will have to send this manually
              while (!(HWUART->UART_SR & UART_SR_TXRDY)) { sw_barrier(); };
              HWUART->UART_THR = XOFF_CHAR;
              // And remember we already sent it
              xon_xoff_state = XOFF_CHAR | XON_XOFF_CHAR_SENT;
            #endif
          }
        }
      }
    #endif // SERIAL_XON_XOFF

    #if ENABLED(EMERGENCY_PARSER)
      emergency_parser.update(c);
    #endif
  }

  #if TX_BUFFER_SIZE > 0

    FORCE_INLINE void _tx_thr_empty_irq(void) {
      // If interrupts are enabled, there must be more data in the output
      // buffer.

      #if ENABLED(SERIAL_XON_XOFF)
        // Do a priority insertion of an XON/XOFF char, if needed.
        const uint8_t state = xon_xoff_state;
        if (!(state & XON_XOFF_CHAR_SENT)) {
          HWUART->UART_THR = state & XON_XOFF_CHAR_MASK;
          xon_xoff_state = state | XON_XOFF_CHAR_SENT;
        }
        else
      #endif
        { // Send the next byte
          const uint8_t t = tx_buffer.tail, c = tx_buffer.buffer[t];
          tx_buffer.tail = (t + 1) & (TX_BUFFER_SIZE - 1);
          HWUART->UART_THR = c;
        }

      // Disable interrupts if the buffer is empty
      if (tx_buffer.head == tx_buffer.tail)
        HWUART->UART_IDR = UART_IDR_TXRDY;
    }

  #endif // TX_BUFFER_SIZE > 0

  static void UART_ISR(void) {
    uint32_t status = HWUART->UART_SR;

    // Did we receive data?
    if (status & UART_SR_RXRDY)
      store_rxd_char();

    #if TX_BUFFER_SIZE > 0
      // Do we have something to send, and TX interrupts are enabled (meaning something to send) ?
      if ((status & UART_SR_TXRDY) && (HWUART->UART_IMR & UART_IMR_TXRDY))
        _tx_thr_empty_irq();
    #endif

    // Acknowledge errors
    if ((status & UART_SR_OVRE) || (status & UART_SR_FRAME)) {
      // TODO: error reporting outside ISR
      HWUART->UART_CR = UART_CR_RSTSTA;
    }
  }

  // Public Methods

  void MarlinSerial::begin(const long baud_setting) {

    // Disable UART interrupt in NVIC
    NVIC_DisableIRQ( HWUART_IRQ );

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

    pmc_disable_periph_clk( HWUART_IRQ_ID );
  }

  void MarlinSerial::checkRx(void) {
    if (HWUART->UART_SR & UART_SR_RXRDY) {
      CRITICAL_SECTION_START;
      store_rxd_char();
      CRITICAL_SECTION_END;
    }
  }

  int MarlinSerial::peek(void) {
    CRITICAL_SECTION_START;
    const int v = rx_buffer.head == rx_buffer.tail ? -1 : rx_buffer.buffer[rx_buffer.tail];
    CRITICAL_SECTION_END;
    return v;
  }

  int MarlinSerial::read(void) {
    int v;
    CRITICAL_SECTION_START;
    const ring_buffer_pos_t t = rx_buffer.tail;
    if (rx_buffer.head == t)
      v = -1;
    else {
      v = rx_buffer.buffer[t];
      rx_buffer.tail = (ring_buffer_pos_t)(t + 1) & (RX_BUFFER_SIZE - 1);

      #if ENABLED(SERIAL_XON_XOFF)
        if ((xon_xoff_state & XON_XOFF_CHAR_MASK) == XOFF_CHAR) {
          // Get count of bytes in the RX buffer
          ring_buffer_pos_t rx_count = (ring_buffer_pos_t)(rx_buffer.head - rx_buffer.tail) & (ring_buffer_pos_t)(RX_BUFFER_SIZE - 1);
          // When below 10% of RX buffer capacity, send XON before
          // running out of RX buffer bytes
          if (rx_count < (RX_BUFFER_SIZE) / 10) {
            xon_xoff_state = XON_CHAR | XON_XOFF_CHAR_SENT;
            CRITICAL_SECTION_END;       // End critical section before returning!
            writeNoHandshake(XON_CHAR);
            return v;
          }
        }
      #endif
    }
    CRITICAL_SECTION_END;
    return v;
  }

  ring_buffer_pos_t MarlinSerial::available(void) {
    CRITICAL_SECTION_START;
    const ring_buffer_pos_t h = rx_buffer.head, t = rx_buffer.tail;
    CRITICAL_SECTION_END;
    return (ring_buffer_pos_t)(RX_BUFFER_SIZE + h - t) & (RX_BUFFER_SIZE - 1);
  }

  void MarlinSerial::flush(void) {
    // Don't change this order of operations. If the RX interrupt occurs between
    // reading rx_buffer_head and updating rx_buffer_tail, the previous rx_buffer_head
    // may be written to rx_buffer_tail, making the buffer appear full rather than empty.
    CRITICAL_SECTION_START;
    rx_buffer.head = rx_buffer.tail;
    CRITICAL_SECTION_END;

    #if ENABLED(SERIAL_XON_XOFF)
      if ((xon_xoff_state & XON_XOFF_CHAR_MASK) == XOFF_CHAR) {
        xon_xoff_state = XON_CHAR | XON_XOFF_CHAR_SENT;
        writeNoHandshake(XON_CHAR);
      }
    #endif
  }

  #if TX_BUFFER_SIZE > 0

    uint8_t MarlinSerial::availableForWrite(void) {
      CRITICAL_SECTION_START;
      const uint8_t h = tx_buffer.head, t = tx_buffer.tail;
      CRITICAL_SECTION_END;
      return (uint8_t)(TX_BUFFER_SIZE + h - t) & (TX_BUFFER_SIZE - 1);
    }

    void MarlinSerial::write(const uint8_t c) {
      #if ENABLED(SERIAL_XON_XOFF)
        const uint8_t state = xon_xoff_state;
        if (!(state & XON_XOFF_CHAR_SENT)) {
          // Send 2 chars: XON/XOFF, then a user-specified char
          writeNoHandshake(state & XON_XOFF_CHAR_MASK);
          xon_xoff_state = state | XON_XOFF_CHAR_SENT;
        }
      #endif
      writeNoHandshake(c);
    }

    void MarlinSerial::writeNoHandshake(const uint8_t c) {
      _written = true;
      CRITICAL_SECTION_START;
      bool emty = (tx_buffer.head == tx_buffer.tail);
      CRITICAL_SECTION_END;
      // If the buffer and the data register is empty, just write the byte
      // to the data register and be done. This shortcut helps
      // significantly improve the effective datarate at high (>
      // 500kbit/s) bitrates, where interrupt overhead becomes a slowdown.
      if (emty && (HWUART->UART_SR & UART_SR_TXRDY)) {
        CRITICAL_SECTION_START;
          HWUART->UART_THR = c;
          HWUART->UART_IER = UART_IER_TXRDY;
        CRITICAL_SECTION_END;
        return;
      }
      const uint8_t i = (tx_buffer.head + 1) & (TX_BUFFER_SIZE - 1);

      // If the output buffer is full, there's nothing for it other than to
      // wait for the interrupt handler to empty it a bit
      while (i == tx_buffer.tail) {
        if (__get_PRIMASK()) {
          // Interrupts are disabled, so we'll have to poll the data
          // register empty flag ourselves. If it is set, pretend an
          // interrupt has happened and call the handler to free up
          // space for us.
          if (HWUART->UART_SR & UART_SR_TXRDY)
            _tx_thr_empty_irq();
        }
        else {
          // nop, the interrupt handler will free up space for us
        }
        sw_barrier();
      }

      tx_buffer.buffer[tx_buffer.head] = c;
      { CRITICAL_SECTION_START;
          tx_buffer.head = i;
          HWUART->UART_IER = UART_IER_TXRDY;
        CRITICAL_SECTION_END;
      }
      return;
    }

    void MarlinSerial::flushTX(void) {
      // TX
      // If we have never written a byte, no need to flush.
      if (!_written)
        return;

      while ((HWUART->UART_IMR & UART_IMR_TXRDY) || !(HWUART->UART_SR & UART_SR_TXEMPTY)) {
        if (__get_PRIMASK())
          if ((HWUART->UART_SR & UART_SR_TXRDY))
            _tx_thr_empty_irq();
        sw_barrier();
      }
      // If we get here, nothing is queued anymore (TX interrupts are disabled) and
      // the hardware finished tranmission (TXEMPTY is set).
    }

  #else // TX_BUFFER_SIZE == 0

    void MarlinSerial::write(const uint8_t c) {
      #if ENABLED(SERIAL_XON_XOFF)
        // Do a priority insertion of an XON/XOFF char, if needed.
        const uint8_t state = xon_xoff_state;
        if (!(state & XON_XOFF_CHAR_SENT)) {
          writeNoHandshake(state & XON_XOFF_CHAR_MASK);
          xon_xoff_state = state | XON_XOFF_CHAR_SENT;
        }
      #endif
      writeNoHandshake(c);
    }

    void MarlinSerial::writeNoHandshake(const uint8_t c) {
      while (!(HWUART->UART_SR & UART_SR_TXRDY)) { sw_barrier(); };
      HWUART->UART_THR = c;
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
#endif

#endif // ARDUINO_ARCH_SAM
