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
 * The SAM3U has five asynchronous ports, numbered here the way Marlin's
 * SERIAL_PORT settings expect:
 *
 *   0  UART    URXD  PA11  UTXD  PA12   (peripheral A)
 *   1  USART0  RXD0  PA19  TXD0  PA18   (peripheral A)
 *   2  USART1  RXD1  PA21  TXD1  PA20   (peripheral A)
 *   3  USART2  RXD2  PA23  TXD2  PA22   (peripheral A)
 *   4  USART3  RXD3  PC13  TXD3  PC12   (peripheral B)
 *
 * Any of them may be assigned to SERIAL_PORT, SERIAL_PORT_2 or SERIAL_PORT_3,
 * in any combination, alongside -1 for native USB (see MarlinSerialUSB.h).
 * Marlin's USING_HW_SERIALn conditionals decide which get compiled in.
 *
 * IMPORTANT, on the 4pi specifically: only port 0's pins are broken out. Every
 * USART lands on pins the board already uses - two of them on heater outputs -
 * so pins_4PI.h and inc/SanityCheck.h reject those. The generic support here
 * is for the SAM3U family; the board file is what constrains it.
 *
 * The UART and the USARTs share register offsets and bit positions for
 * everything this driver touches (CR, IER/IDR/IMR, SR/CSR, RHR, THR, BRGR), so
 * all five are driven through a single Uart* view. Only the mode register
 * differs, and that is handled per port type in begin().
 */

#include "../../inc/MarlinConfigPre.h"
#include "../../core/types.h"
#include "../../core/serial_hook.h"
#include "include/pinmapping.h"

#include "MarlinSerialUSB.h"

// Marlin's default _MSERIAL(X) names instances MSerial<port>, which is what
// this file declares. USB instances are slot-named; see MarlinSerialUSB.h.
#define USB_SERIAL_PORT(N) customizedSerial##N
#define SERIAL_INDEX_MIN 0
#define SERIAL_INDEX_MAX 4
#include "../shared/serial_ports.h"

/**
 * Which ports are in use.
 *
 * Marlin's own USING_HW_SERIALn flags would be the natural thing to test, but
 * they are set in Conditionals-5-post.h *after* the HAL header is pulled into
 * the cascade, so they are not visible here. Derive the same thing from the
 * port assignments directly, which are available this early.
 */
#define SAM3U_SERIAL_IN_USE(N) (   (defined(SERIAL_PORT)       && (N) == SERIAL_PORT) \
                                || (defined(SERIAL_PORT_2)     && (N) == SERIAL_PORT_2) \
                                || (defined(SERIAL_PORT_3)     && (N) == SERIAL_PORT_3) \
                                || (defined(MMU_SERIAL_PORT)   && (N) == MMU_SERIAL_PORT) \
                                || (defined(LCD_SERIAL_PORT)   && (N) == LCD_SERIAL_PORT) \
                                || (defined(RS485_SERIAL_PORT) && (N) == RS485_SERIAL_PORT) )

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

// ------------------------
// Port descriptions
// ------------------------

struct SerialPortDesc {
  uint32_t  base;       // Peripheral base, viewed as Uart*
  IRQn_Type irq;
  uint8_t   periph_id;
  int8_t    rx_pin, tx_pin;
  bool      periph_b;   // false: PIO peripheral A, true: peripheral B
  bool      is_usart;   // USARTs need their own mode register layout
};

// constexpr so a port's description folds away entirely for a constant index
constexpr SerialPortDesc serial_port_desc(const uint8_t port) {
  return port == 0 ? SerialPortDesc{ 0x400E0600, UART_IRQn,   ID_UART,   PA11, PA12, false, false }
       : port == 1 ? SerialPortDesc{ 0x40090000, USART0_IRQn, ID_USART0, PA19, PA18, false, true  }
       : port == 2 ? SerialPortDesc{ 0x40094000, USART1_IRQn, ID_USART1, PA21, PA20, false, true  }
       : port == 3 ? SerialPortDesc{ 0x40098000, USART2_IRQn, ID_USART2, PA23, PA22, false, true  }
       :             SerialPortDesc{ 0x4009C000, USART3_IRQn, ID_USART3, PC13, PC12, true,  true  };
}

// ------------------------
// Driver
// ------------------------

template<typename Cfg>
class MarlinSerial {
protected:
  static constexpr SerialPortDesc DESC = serial_port_desc(Cfg::PORT);

  static Uart* regs() { return (Uart *)DESC.base; }

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

  // Called from this port's interrupt vector
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
  static constexpr uint8_t PORT           = serial;
  static constexpr unsigned int RX_SIZE   = RX_BUFFER_SIZE;
  static constexpr unsigned int TX_SIZE   = TX_BUFFER_SIZE;
  static constexpr bool XONOFF            = ENABLED(SERIAL_XON_XOFF);
  static constexpr bool EMERGENCYPARSER   = ENABLED(EMERGENCY_PARSER);
  static constexpr bool DROPPED_RX        = ENABLED(SERIAL_STATS_DROPPED_RX);
  static constexpr bool RX_OVERRUNS       = ENABLED(SERIAL_STATS_RX_BUFFER_OVERRUNS);
  static constexpr bool RX_FRAMING_ERRORS = ENABLED(SERIAL_STATS_RX_FRAMING_ERRORS);
  static constexpr bool MAX_RX_QUEUED     = ENABLED(SERIAL_STATS_MAX_RX_QUEUED);
};

// One instance per port actually in use, named the way _MSERIAL() expects
#if SAM3U_SERIAL_IN_USE(0)
  typedef Serial1Class< MarlinSerial< MarlinSerialCfg<0> > > MSerialT0;
  extern MSerialT0 MSerial0;
#endif
#if SAM3U_SERIAL_IN_USE(1)
  typedef Serial1Class< MarlinSerial< MarlinSerialCfg<1> > > MSerialT1x;
  extern MSerialT1x MSerial1;
#endif
#if SAM3U_SERIAL_IN_USE(2)
  typedef Serial1Class< MarlinSerial< MarlinSerialCfg<2> > > MSerialT2x;
  extern MSerialT2x MSerial2;
#endif
#if SAM3U_SERIAL_IN_USE(3)
  typedef Serial1Class< MarlinSerial< MarlinSerialCfg<3> > > MSerialT3x;
  extern MSerialT3x MSerial3;
#endif
#if SAM3U_SERIAL_IN_USE(4)
  typedef Serial1Class< MarlinSerial< MarlinSerialCfg<4> > > MSerialT4x;
  extern MSerialT4x MSerial4;
#endif
