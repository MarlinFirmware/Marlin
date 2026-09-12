/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

/**
 * Hardware serial for Atmel SAM3U (AT91SAM3U4E). See MarlinSerial.h.
 */

#ifdef __SAM3U4E__

#include "../../inc/MarlinConfig.h"

#if SAM3U_SERIAL_IN_USE(0) || SAM3U_SERIAL_IN_USE(1) || SAM3U_SERIAL_IN_USE(2) \
 || SAM3U_SERIAL_IN_USE(3) || SAM3U_SERIAL_IN_USE(4)

#include "MarlinSerial.h"
#include "clock.h"

#if ENABLED(EMERGENCY_PARSER)
  #include "../../feature/e_parser.h"
#endif

// ------------------------
// Static member storage
// ------------------------

template<typename Cfg> typename MarlinSerial<Cfg>::ring_buffer_r MarlinSerial<Cfg>::rx_buffer = { };
template<typename Cfg> constexpr SerialPortDesc MarlinSerial<Cfg>::DESC;
template<typename Cfg> typename MarlinSerial<Cfg>::ring_buffer_t MarlinSerial<Cfg>::tx_buffer = { };
template<typename Cfg> bool     MarlinSerial<Cfg>::_written = false;
template<typename Cfg> uint8_t  MarlinSerial<Cfg>::xon_xoff_state = MarlinSerial<Cfg>::XON_XOFF_CHAR_SENT | MarlinSerial<Cfg>::XON_CHAR;
template<typename Cfg> uint8_t  MarlinSerial<Cfg>::rx_dropped_bytes = 0;
template<typename Cfg> uint8_t  MarlinSerial<Cfg>::rx_buffer_overruns = 0;
template<typename Cfg> uint8_t  MarlinSerial<Cfg>::rx_framing_errors = 0;
template<typename Cfg> typename MarlinSerial<Cfg>::ring_buffer_pos_t MarlinSerial<Cfg>::rx_max_enqueued = 0;

// ------------------------
// Receive
// ------------------------

template<typename Cfg>
void MarlinSerial<Cfg>::store_rxd_char() {

  const uint32_t status = regs()->UART_SR;

  // A framing or parity error means the byte in RHR is garbage. Read it to
  // clear RXRDY, reset the error latch, and drop it.
  if (status & (UART_SR_FRAME | UART_SR_PARE)) {
    if (Cfg::RX_FRAMING_ERRORS && !++rx_framing_errors) rx_framing_errors--; // Track, but don't wrap to zero
    (void)regs()->UART_RHR;
    regs()->UART_CR = UART_CR_RSTSTA;
    return;
  }

  // An overrun means the hardware dropped at least one byte before we got here
  if (status & UART_SR_OVRE) {
    if (Cfg::RX_OVERRUNS && !++rx_buffer_overruns) rx_buffer_overruns--;
    regs()->UART_CR = UART_CR_RSTSTA;
  }

  const uint8_t c = uint8_t(regs()->UART_RHR & 0xFF);

  #if ENABLED(EMERGENCY_PARSER)
    static EmergencyParser::State emergency_state;  // = EP_RESET
    if (Cfg::EMERGENCYPARSER) emergency_parser.update(emergency_state, c);
  #endif

  const ring_buffer_pos_t h = rx_buffer.head,
                          i = ring_buffer_pos_t(h + 1) & ring_buffer_pos_t(Cfg::RX_SIZE - 1);

  // Silently drop the byte if the buffer is full rather than corrupting it
  if (i == rx_buffer.tail) {
    if (Cfg::DROPPED_RX && !++rx_dropped_bytes) rx_dropped_bytes--;
    return;
  }

  rx_buffer.buffer[h] = c;
  rx_buffer.head = i;

  if (Cfg::MAX_RX_QUEUED) {
    const ring_buffer_pos_t rx_count = ring_buffer_pos_t(rx_buffer.head - rx_buffer.tail) & ring_buffer_pos_t(Cfg::RX_SIZE - 1);
    NOLESS(rx_max_enqueued, rx_count);
  }

  if (Cfg::XONOFF) {
    // Ask the host to pause once the buffer is more than half full
    const ring_buffer_pos_t rx_count = ring_buffer_pos_t(rx_buffer.head - rx_buffer.tail) & ring_buffer_pos_t(Cfg::RX_SIZE - 1);
    if (rx_count >= (Cfg::RX_SIZE) / 2 && (xon_xoff_state & XON_XOFF_CHAR_MASK) == XON_CHAR) {
      xon_xoff_state = XOFF_CHAR;              // Pending, not yet sent
      if (regs()->UART_SR & UART_SR_TXRDY) {
        regs()->UART_THR = XOFF_CHAR;
        xon_xoff_state = XOFF_CHAR | XON_XOFF_CHAR_SENT;
      }
      else
        regs()->UART_IER = UART_IER_TXRDY;       // Send it from the TX interrupt
    }
  }
}

// ------------------------
// Transmit
// ------------------------

template<typename Cfg>
void MarlinSerial<Cfg>::_tx_thr_empty_irq() {

  // An XON/XOFF that could not be sent immediately takes priority over data
  if (Cfg::XONOFF && (xon_xoff_state & XON_XOFF_CHAR_SENT) == 0) {
    regs()->UART_THR = xon_xoff_state & XON_XOFF_CHAR_MASK;
    xon_xoff_state |= XON_XOFF_CHAR_SENT;
    return;
  }

  const uint8_t t = tx_buffer.tail, h = tx_buffer.head;

  if (h == t) {                                // Nothing left to send
    regs()->UART_IDR = UART_IDR_TXRDY;
    return;
  }

  regs()->UART_THR = tx_buffer.buffer[t];
  tx_buffer.tail = (t + 1) & (Cfg::TX_SIZE - 1);

  if (tx_buffer.head == tx_buffer.tail) regs()->UART_IDR = UART_IDR_TXRDY;
}

template<typename Cfg>
void MarlinSerial<Cfg>::UART_ISR() {
  const uint32_t status = regs()->UART_SR;

  if (status & UART_SR_RXRDY) store_rxd_char();

  if ((status & UART_SR_TXRDY) && (regs()->UART_IMR & UART_IMR_TXRDY)) _tx_thr_empty_irq();

  // Clear any error latch we did not handle above
  if (status & (UART_SR_OVRE | UART_SR_FRAME | UART_SR_PARE)) regs()->UART_CR = UART_CR_RSTSTA;
}

// ------------------------
// Public interface
// ------------------------

template<typename Cfg>
void MarlinSerial<Cfg>::begin(const long baud) {

  // Hand the RX/TX pads to the peripheral. Which PIO and which of the two
  // peripheral functions depends on the port; see serial_port_desc().
  Pio * const rx_port = SAM3U_PIO(DESC.rx_pin);
  Pio * const tx_port = SAM3U_PIO(DESC.tx_pin);
  const uint32_t rx_mask = SAM3U_MASK(DESC.rx_pin), tx_mask = SAM3U_MASK(DESC.tx_pin);

  pmc_enable_periph_clk(SAM3U_PIO_ID(DESC.rx_pin));
  pmc_enable_periph_clk(SAM3U_PIO_ID(DESC.tx_pin));

  rx_port->PIO_IDR = rx_mask;  tx_port->PIO_IDR = tx_mask;
  rx_port->PIO_PUDR = rx_mask; tx_port->PIO_PUDR = tx_mask;   // Lines are driven

  if (DESC.periph_b) {
    rx_port->PIO_ABSR |= rx_mask;
    tx_port->PIO_ABSR |= tx_mask;
  }
  else {
    rx_port->PIO_ABSR &= ~rx_mask;
    tx_port->PIO_ABSR &= ~tx_mask;
  }

  rx_port->PIO_PDR = rx_mask;  tx_port->PIO_PDR = tx_mask;    // Pads to the peripheral

  pmc_enable_periph_clk(DESC.periph_id);

  // Stop the PDC in case a previous user left it running
  regs()->UART_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;

  // Reset and disable both directions before reconfiguring
  regs()->UART_CR = UART_CR_RSTRX | UART_CR_RSTTX | UART_CR_RXDIS | UART_CR_TXDIS | UART_CR_RSTSTA;

  /**
   * 8N1. This is the one place the UART and the USARTs genuinely differ: the
   * UART's word length and stop bits are fixed in hardware and only parity and
   * channel mode are programmable, while a USART has to be told all of it and
   * uses a different field layout.
   */
  if (DESC.is_usart) {
    ((Usart *)DESC.base)->US_MR = US_MR_USART_MODE_NORMAL
                                | US_MR_USCLKS_MCK
                                | US_MR_CHRL_8_BIT
                                | US_MR_PAR_NO
                                | US_MR_NBSTOP_1_BIT
                                | US_MR_CHMODE_NORMAL;
  }
  else {
    regs()->UART_MR = UART_MR_CHMODE_NORMAL | UART_MR_PAR_NO;
  }

  // Baud rate: CD = MCK / (16 * baud)
  regs()->UART_BRGR = (F_CPU) / (baud * 16);

  regs()->UART_IDR = 0xFFFFFFFF;                 // Mask every source, then add RX back
  regs()->UART_IER = UART_IER_RXRDY | UART_IER_OVRE | UART_IER_FRAME;

  NVIC_SetPriority(DESC.irq, 1);                 // Just below the stepper timer
  NVIC_EnableIRQ(DESC.irq);

  regs()->UART_CR = UART_CR_RXEN | UART_CR_TXEN;

  rx_buffer.head = rx_buffer.tail = 0;
  tx_buffer.head = tx_buffer.tail = 0;
  _written = false;

  if (Cfg::XONOFF) xon_xoff_state = XON_XOFF_CHAR_SENT | XON_CHAR;
}

template<typename Cfg>
void MarlinSerial<Cfg>::end() {
  NVIC_DisableIRQ(DESC.irq);
  __DSB();
  __ISB();
  regs()->UART_IDR = 0xFFFFFFFF;
  regs()->UART_CR = UART_CR_RXDIS | UART_CR_TXDIS;
  pmc_disable_periph_clk(DESC.periph_id);
}

template<typename Cfg>
int MarlinSerial<Cfg>::peek() {
  return rx_buffer.head == rx_buffer.tail ? -1 : rx_buffer.buffer[rx_buffer.tail];
}

template<typename Cfg>
int MarlinSerial<Cfg>::read() {
  const ring_buffer_pos_t t = rx_buffer.tail;
  if (rx_buffer.head == t) return -1;

  const int v = rx_buffer.buffer[t];
  rx_buffer.tail = ring_buffer_pos_t(t + 1) & ring_buffer_pos_t(Cfg::RX_SIZE - 1);

  if (Cfg::XONOFF) {
    // Enough room again - let the host resume
    const ring_buffer_pos_t rx_count = ring_buffer_pos_t(rx_buffer.head - rx_buffer.tail) & ring_buffer_pos_t(Cfg::RX_SIZE - 1);
    if (rx_count < (Cfg::RX_SIZE) / 10 && (xon_xoff_state & XON_XOFF_CHAR_MASK) == XOFF_CHAR) {
      xon_xoff_state = XON_CHAR;
      if (regs()->UART_SR & UART_SR_TXRDY) {
        regs()->UART_THR = XON_CHAR;
        xon_xoff_state = XON_CHAR | XON_XOFF_CHAR_SENT;
      }
      else
        regs()->UART_IER = UART_IER_TXRDY;
    }
  }

  return v;
}

template<typename Cfg>
typename MarlinSerial<Cfg>::ring_buffer_pos_t MarlinSerial<Cfg>::available() {
  const ring_buffer_pos_t h = rx_buffer.head, t = rx_buffer.tail;
  return ring_buffer_pos_t(h - t) & ring_buffer_pos_t(Cfg::RX_SIZE - 1);
}

template<typename Cfg>
void MarlinSerial<Cfg>::flush() {
  rx_buffer.tail = rx_buffer.head;

  if (Cfg::XONOFF && (xon_xoff_state & XON_XOFF_CHAR_MASK) == XOFF_CHAR) {
    xon_xoff_state = XON_CHAR;
    if (regs()->UART_SR & UART_SR_TXRDY) {
      regs()->UART_THR = XON_CHAR;
      xon_xoff_state = XON_CHAR | XON_XOFF_CHAR_SENT;
    }
    else
      regs()->UART_IER = UART_IER_TXRDY;
  }
}

template<typename Cfg>
size_t MarlinSerial<Cfg>::write(const uint8_t c) {
  _written = true;

  // Unbuffered mode (TX_BUFFER_SIZE 0), or interrupts masked so the ISR can
  // never drain the buffer: block on the hardware directly. The latter keeps
  // panic/abort output working.
  if (Cfg::TX_SIZE == 0 || __get_PRIMASK()) {
    while (!(regs()->UART_SR & UART_SR_TXRDY)) { /* wait */ }
    regs()->UART_THR = c;
    return 1;
  }

  const uint8_t i = (tx_buffer.head + 1) & (Cfg::TX_SIZE - 1);

  // Block until the ISR frees a slot
  while (i == tx_buffer.tail) { /* wait */ }

  tx_buffer.buffer[tx_buffer.head] = c;
  tx_buffer.head = i;

  regs()->UART_IER = UART_IER_TXRDY;
  return 1;
}

template<typename Cfg>
void MarlinSerial<Cfg>::flushTX() {
  if (!_written) return;
  while (tx_buffer.head != tx_buffer.tail || !(regs()->UART_SR & UART_SR_TXEMPTY)) { /* wait */ }
}

// ------------------------
// Instantiation
// ------------------------

/**
 * One instance and one interrupt vector per port actually in use. Marlin's
 * USING_HW_SERIALn flags are set from SERIAL_PORT, SERIAL_PORT_2 and
 * SERIAL_PORT_3 together, so any combination pulls in exactly what it needs.
 */

#if SAM3U_SERIAL_IN_USE(0)
  template class MarlinSerial< MarlinSerialCfg<0> >;
  MSerialT0 MSerial0(MarlinSerialCfg<0>::EMERGENCYPARSER);
  extern "C" void UART_Handler()   { MarlinSerial< MarlinSerialCfg<0> >::UART_ISR(); }
#endif

#if SAM3U_SERIAL_IN_USE(1)
  template class MarlinSerial< MarlinSerialCfg<1> >;
  MSerialT1x MSerial1(MarlinSerialCfg<1>::EMERGENCYPARSER);
  extern "C" void USART0_Handler() { MarlinSerial< MarlinSerialCfg<1> >::UART_ISR(); }
#endif

#if SAM3U_SERIAL_IN_USE(2)
  template class MarlinSerial< MarlinSerialCfg<2> >;
  MSerialT2x MSerial2(MarlinSerialCfg<2>::EMERGENCYPARSER);
  extern "C" void USART1_Handler() { MarlinSerial< MarlinSerialCfg<2> >::UART_ISR(); }
#endif

#if SAM3U_SERIAL_IN_USE(3)
  template class MarlinSerial< MarlinSerialCfg<3> >;
  MSerialT3x MSerial3(MarlinSerialCfg<3>::EMERGENCYPARSER);
  extern "C" void USART2_Handler() { MarlinSerial< MarlinSerialCfg<3> >::UART_ISR(); }
#endif

#if SAM3U_SERIAL_IN_USE(4)
  template class MarlinSerial< MarlinSerialCfg<4> >;
  MSerialT4x MSerial4(MarlinSerialCfg<4>::EMERGENCYPARSER);
  extern "C" void USART3_Handler() { MarlinSerial< MarlinSerialCfg<4> >::UART_ISR(); }
#endif

#endif // SAM3U_SERIAL_IN_USE(n)
#endif // __SAM3U4E__
