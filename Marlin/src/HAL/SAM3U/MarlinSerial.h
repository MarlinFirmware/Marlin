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
#pragma once

/**
 * Hardware serial for Atmel SAM3U (AT91SAM3U4E)
 *
 * The 4pi brings out exactly one asynchronous port: the chip's dedicated UART
 * on PA11 (URXD, peripheral A) and PA12 (UTXD, peripheral A), wired to pins 13
 * and 14 of the board's 14-pin expansion header. The four USARTs are not
 * broken out, so this HAL implements the single UART rather than the DUE's
 * five-port template.
 *
 * The UART is fixed at 8 data bits and no parity in hardware; only the baud
 * rate and stop bits are programmable.
 */

#include "../../inc/MarlinConfigPre.h"
#include "../../core/types.h"
#include "../../core/serial_hook.h"

// This HAL has exactly one port, instantiated as customizedSerial1, so every
// valid index maps onto it rather than onto a MSerial<N> family.
#define _MSERIAL(X) customizedSerial1
#define SERIAL_INDEX_MIN 0
#define SERIAL_INDEX_MAX 0
#include "../shared/serial_ports.h"

// Ring buffer sizes. Both must be powers of two so the index wrap is a mask.
#ifndef RX_BUFFER_SIZE
  #define RX_BUFFER_SIZE 128
#endif
#ifndef TX_BUFFER_SIZE
  #define TX_BUFFER_SIZE 32
#endif

#if RX_BUFFER_SIZE < 2 || !IS_POWER_OF_2(RX_BUFFER_SIZE)
  #error "RX_BUFFER_SIZE must be a power of 2 greater than 1."
#endif
#if TX_BUFFER_SIZE && (TX_BUFFER_SIZE < 2 || TX_BUFFER_SIZE > 256 || !IS_POWER_OF_2(TX_BUFFER_SIZE))
  #error "TX_BUFFER_SIZE must be 0 (unbuffered), or a power of 2 between 2 and 256."
#endif

template<typename Cfg>
class MarlinSerial {
protected:
  typedef uvalue_t(Cfg::RX_SIZE - 1) ring_buffer_pos_t;

  struct ring_buffer_r {
    volatile ring_buffer_pos_t head, tail;
    unsigned char buffer[Cfg::RX_SIZE];
  };

  // TX_SIZE may be 0, meaning "write straight to the hardware". A zero-length
  // array is not valid, so keep at least one byte in that case; write() never
  // touches it.
  struct ring_buffer_t {
    volatile uint8_t head, tail;
    unsigned char buffer[Cfg::TX_SIZE ? Cfg::TX_SIZE : 1];
  };

  static ring_buffer_r rx_buffer;
  static ring_buffer_t tx_buffer;
  static bool _written;

  static constexpr uint8_t XON_XOFF_CHAR_SENT = 0x80,  // XON / XOFF character was sent
                           XON_XOFF_CHAR_MASK = 0x1F;  // XON / XOFF character to send

  static constexpr uint8_t XON_CHAR = 17, XOFF_CHAR = 19;

  static uint8_t xon_xoff_state,
                 rx_dropped_bytes,
                 rx_buffer_overruns,
                 rx_framing_errors;
  static ring_buffer_pos_t rx_max_enqueued;

  static void store_rxd_char();
  static void _tx_thr_empty_irq();

public:
  MarlinSerial() {}

  static void begin(const long baud);
  static void end();
  static int peek();
  static int read();
  static void flush();
  static ring_buffer_pos_t available();
  static size_t write(const uint8_t c);
  static void flushTX();

  // Called from the UART interrupt vector
  static void UART_ISR();

  static bool emergency_parser_enabled() { return Cfg::EMERGENCYPARSER; }

  FORCE_INLINE static uint8_t dropped()          { return Cfg::DROPPED_RX ? rx_dropped_bytes : 0; }
  FORCE_INLINE static uint8_t buffer_overruns()  { return Cfg::RX_OVERRUNS ? rx_buffer_overruns : 0; }
  FORCE_INLINE static uint8_t framing_errors()   { return Cfg::RX_FRAMING_ERRORS ? rx_framing_errors : 0; }
  FORCE_INLINE static ring_buffer_pos_t rxMaxEnqueued() { return Cfg::MAX_RX_QUEUED ? rx_max_enqueued : 0; }
};

// Serial port configuration
template <uint8_t serial>
struct MarlinSerialCfg {
  static constexpr int PORT               = serial;
  static constexpr unsigned int RX_SIZE   = RX_BUFFER_SIZE;
  static constexpr unsigned int TX_SIZE   = TX_BUFFER_SIZE;
  static constexpr bool XONOFF            = ENABLED(SERIAL_XON_XOFF);
  static constexpr bool EMERGENCYPARSER   = ENABLED(EMERGENCY_PARSER);
  static constexpr bool DROPPED_RX        = ENABLED(SERIAL_STATS_DROPPED_RX);
  static constexpr bool RX_OVERRUNS       = ENABLED(SERIAL_STATS_RX_BUFFER_OVERRUNS);
  static constexpr bool RX_FRAMING_ERRORS = ENABLED(SERIAL_STATS_RX_FRAMING_ERRORS);
  static constexpr bool MAX_RX_QUEUED     = ENABLED(SERIAL_STATS_MAX_RX_QUEUED);
};

#if defined(SERIAL_PORT) && SERIAL_PORT >= 0
  typedef Serial1Class< MarlinSerial< MarlinSerialCfg<SERIAL_PORT> > > MSerialT1;
  extern MSerialT1 customizedSerial1;
#endif

#if defined(SERIAL_PORT_2) && SERIAL_PORT_2 >= 0
  #error "The SAM3U/4pi HAL only exposes one hardware serial port (SERIAL_PORT 0)."
#endif
