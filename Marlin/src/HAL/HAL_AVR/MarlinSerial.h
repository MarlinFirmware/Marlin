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
 * MarlinSerial.h - Hardware serial library for Wiring
 * Copyright (c) 2006 Nicholas Zambetti.  All right reserved.
 *
 * Modified 28 September 2010 by Mark Sproul
 * Modified 14 February 2016 by Andreas Hardtung (added tx buffer)
 * Modified 01 October 2017 by Eduardo José Tagle (added XON/XOFF)
 * Templatized 01 October 2018 by Eduardo José Tagle to allow multiple instances
 */

#ifndef _MARLINSERIAL_H_
#define _MARLINSERIAL_H_

#include "../shared/MarlinSerial.h"

#include <WString.h>

#ifndef SERIAL_PORT
  #define SERIAL_PORT 0
#endif

#ifndef USBCON

  // The presence of the UBRRH register is used to detect a UART.
  #define UART_PRESENT(port) ((port == 0 && (defined(UBRRH) || defined(UBRR0H))) || \
                              (port == 1 && defined(UBRR1H)) || (port == 2 && defined(UBRR2H)) || \
                              (port == 3 && defined(UBRR3H)))

  // These are macros to build serial port register names for the selected SERIAL_PORT (C preprocessor
  // requires two levels of indirection to expand macro values properly)
  #define SERIAL_REGNAME(registerbase,number,suffix) SERIAL_REGNAME_INTERNAL(registerbase,number,suffix)
  #if SERIAL_PORT == 0 && (!defined(UBRR0H) || !defined(UDR0)) // use un-numbered registers if necessary
    #define SERIAL_REGNAME_INTERNAL(registerbase,number,suffix) registerbase##suffix
  #else
    #define SERIAL_REGNAME_INTERNAL(registerbase,number,suffix) registerbase##number##suffix
  #endif

  // Registers used by MarlinSerial class (expanded depending on selected serial port)

  // Templated 8bit register (generic)
  #define UART_REGISTER_DECL_BASE(registerbase, suffix) \
    template<int portNr> struct R_##registerbase##x##suffix {}

  // Templated 8bit register (specialization for each port)
  #define UART_REGISTER_DECL(port, registerbase, suffix) \
    template<> struct R_##registerbase##x##suffix<port> { \
      constexpr R_##registerbase##x##suffix(int) {} \
      FORCE_INLINE void operator=(uint8_t newVal) const { SERIAL_REGNAME(registerbase,port,suffix) = newVal; } \
      FORCE_INLINE operator uint8_t() const { return SERIAL_REGNAME(registerbase,port,suffix); } \
    }

  // Templated 1bit register (generic)
  #define UART_BIT_DECL_BASE(registerbase, suffix, bit) \
    template<int portNr>struct B_##bit##x {}

  // Templated 1bit register (specialization for each port)
  #define UART_BIT_DECL(port, registerbase, suffix, bit) \
    template<> struct B_##bit##x<port> { \
      constexpr B_##bit##x(int) {} \
      FORCE_INLINE void operator=(int newVal) const { \
        if (newVal) \
          SBI(SERIAL_REGNAME(registerbase,port,suffix),SERIAL_REGNAME(bit,port,)); \
        else \
          CBI(SERIAL_REGNAME(registerbase,port,suffix),SERIAL_REGNAME(bit,port,)); \
      } \
      FORCE_INLINE operator bool() const { return TEST(SERIAL_REGNAME(registerbase,port,suffix),SERIAL_REGNAME(bit,port,)); } \
    }

  #define UART_DECL_BASE() \
    UART_REGISTER_DECL_BASE(UCSR,A);\
    UART_REGISTER_DECL_BASE(UDR,);\
    UART_REGISTER_DECL_BASE(UBRR,H);\
    UART_REGISTER_DECL_BASE(UBRR,L);\
    UART_BIT_DECL_BASE(UCSR,B,RXEN);\
    UART_BIT_DECL_BASE(UCSR,B,TXEN);\
    UART_BIT_DECL_BASE(UCSR,A,TXC);\
    UART_BIT_DECL_BASE(UCSR,B,RXCIE);\
    UART_BIT_DECL_BASE(UCSR,A,UDRE);\
    UART_BIT_DECL_BASE(UCSR,A,FE);\
    UART_BIT_DECL_BASE(UCSR,A,DOR);\
    UART_BIT_DECL_BASE(UCSR,B,UDRIE);\
    UART_BIT_DECL_BASE(UCSR,A,RXC);\
    UART_BIT_DECL_BASE(UCSR,A,U2X)

  #define UART_DECL(port) \
    UART_REGISTER_DECL(port,UCSR,A);\
    UART_REGISTER_DECL(port,UDR,);\
    UART_REGISTER_DECL(port,UBRR,H);\
    UART_REGISTER_DECL(port,UBRR,L);\
    UART_BIT_DECL(port,UCSR,B,RXEN);\
    UART_BIT_DECL(port,UCSR,B,TXEN);\
    UART_BIT_DECL(port,UCSR,A,TXC);\
    UART_BIT_DECL(port,UCSR,B,RXCIE);\
    UART_BIT_DECL(port,UCSR,A,UDRE);\
    UART_BIT_DECL(port,UCSR,A,FE);\
    UART_BIT_DECL(port,UCSR,A,DOR);\
    UART_BIT_DECL(port,UCSR,B,UDRIE);\
    UART_BIT_DECL(port,UCSR,A,RXC);\
    UART_BIT_DECL(port,UCSR,A,U2X)

  // Declare empty templates
  UART_DECL_BASE();

  // And all the specializations for each possible serial port
  #if UART_PRESENT(0)
    UART_DECL(0);
  #endif
  #if UART_PRESENT(1)
    UART_DECL(1);
  #endif
  #if UART_PRESENT(2)
    UART_DECL(2);
  #endif
  #if UART_PRESENT(3)
    UART_DECL(3);
  #endif

  #define DEC 10
  #define HEX 16
  #define OCT 8
  #define BIN 2
  #define BYTE 0

  // Templated type selector
  template<bool b, typename T, typename F> struct TypeSelector { typedef T type;} ;
  template<typename T, typename F> struct TypeSelector<false, T, F> { typedef F type; };

  template<typename Cfg>
  class MarlinSerial {
  protected:
    // Registers
    static constexpr R_UCSRxA<Cfg::PORT> R_UCSRA = 0;
    static constexpr R_UDRx<Cfg::PORT>   R_UDR   = 0;
    static constexpr R_UBRRxH<Cfg::PORT> R_UBRRH = 0;
    static constexpr R_UBRRxL<Cfg::PORT> R_UBRRL = 0;

    // Bits
    static constexpr B_RXENx<Cfg::PORT>  B_RXEN  = 0;
    static constexpr B_TXENx<Cfg::PORT>  B_TXEN  = 0;
    static constexpr B_TXCx<Cfg::PORT>   B_TXC   = 0;
    static constexpr B_RXCIEx<Cfg::PORT> B_RXCIE = 0;
    static constexpr B_UDREx<Cfg::PORT>  B_UDRE  = 0;
    static constexpr B_FEx<Cfg::PORT>    B_FE    = 0;
    static constexpr B_DORx<Cfg::PORT>   B_DOR   = 0;
    static constexpr B_UDRIEx<Cfg::PORT> B_UDRIE = 0;
    static constexpr B_RXCx<Cfg::PORT>   B_RXC   = 0;
    static constexpr B_U2Xx<Cfg::PORT>   B_U2X   = 0;

    // Base size of type on buffer size
    typedef typename TypeSelector<(Cfg::RX_SIZE>256), uint16_t, uint8_t>::type ring_buffer_pos_t;

    struct ring_buffer_r {
      volatile ring_buffer_pos_t head, tail;
      unsigned char buffer[Cfg::RX_SIZE];
    };

    struct ring_buffer_t {
      volatile uint8_t head, tail;
      unsigned char buffer[Cfg::TX_SIZE];
    };

    static ring_buffer_r rx_buffer;
    static ring_buffer_t tx_buffer;
    static bool _written;

    static constexpr uint8_t XON_XOFF_CHAR_SENT = 0x80,  // XON / XOFF Character was sent
                             XON_XOFF_CHAR_MASK = 0x1F;  // XON / XOFF character to send

    // XON / XOFF character definitions
    static constexpr uint8_t XON_CHAR  = 17, XOFF_CHAR = 19;
    static uint8_t xon_xoff_state,
                   rx_dropped_bytes,
                   rx_buffer_overruns,
                   rx_framing_errors;
    static ring_buffer_pos_t rx_max_enqueued;

    static FORCE_INLINE ring_buffer_pos_t atomic_read_rx_head();

    static volatile bool rx_tail_value_not_stable;
    static volatile uint16_t rx_tail_value_backup;

    static FORCE_INLINE void atomic_set_rx_tail(ring_buffer_pos_t value);
    static FORCE_INLINE ring_buffer_pos_t atomic_read_rx_tail();

    public:

    FORCE_INLINE static void store_rxd_char();
    FORCE_INLINE static void _tx_udr_empty_irq(void);

    public:
      MarlinSerial() {};
      static void begin(const long);
      static void end();
      static int peek(void);
      static int read(void);
      static void flush(void);
      static ring_buffer_pos_t available(void);
      static void write(const uint8_t c);
      static void flushTX(void);

      FORCE_INLINE static uint8_t dropped() { return Cfg::DROPPED_RX ? rx_dropped_bytes : 0; }
      FORCE_INLINE static uint8_t buffer_overruns() { return Cfg::RX_OVERRUNS ? rx_buffer_overruns : 0; }
      FORCE_INLINE static uint8_t framing_errors() { return Cfg::RX_FRAMING_ERRORS ? rx_framing_errors : 0; }
      FORCE_INLINE static ring_buffer_pos_t rxMaxEnqueued() { return Cfg::MAX_RX_QUEUED ? rx_max_enqueued : 0; }

      FORCE_INLINE static void write(const char* str) { while (*str) write(*str++); }
      FORCE_INLINE static void write(const uint8_t* buffer, size_t size) { while (size--) write(*buffer++); }
      FORCE_INLINE static void print(const String& s) { for (int i = 0; i < (int)s.length(); i++) write(s[i]); }
      FORCE_INLINE static void print(const char* str) { write(str); }

      static void print(char, int = BYTE);
      static void print(unsigned char, int = BYTE);
      static void print(int, int = DEC);
      static void print(unsigned int, int = DEC);
      static void print(long, int = DEC);
      static void print(unsigned long, int = DEC);
      static void print(double, int = 2);

      static void println(const String& s);
      static void println(const char[]);
      static void println(char, int = BYTE);
      static void println(unsigned char, int = BYTE);
      static void println(int, int = DEC);
      static void println(unsigned int, int = DEC);
      static void println(long, int = DEC);
      static void println(unsigned long, int = DEC);
      static void println(double, int = 2);
      static void println(void);
      operator bool() { return true; }

    private:
      static void printNumber(unsigned long, const uint8_t);
      static void printFloat(double, uint8_t);
  };

  template <uint8_t serial>
  struct MarlinSerialCfg {
    static constexpr int PORT               = serial;
    static constexpr unsigned int RX_SIZE   = RX_BUFFER_SIZE;
    static constexpr unsigned int TX_SIZE   = TX_BUFFER_SIZE;
    static constexpr bool XONOFF            = bSERIAL_XON_XOFF;
    static constexpr bool EMERGENCYPARSER   = bEMERGENCY_PARSER;
    static constexpr bool DROPPED_RX        = bSERIAL_STATS_DROPPED_RX;
    static constexpr bool RX_OVERRUNS       = bSERIAL_STATS_RX_BUFFER_OVERRUNS;
    static constexpr bool RX_FRAMING_ERRORS = bSERIAL_STATS_RX_FRAMING_ERRORS;
    static constexpr bool MAX_RX_QUEUED     = bSERIAL_STATS_MAX_RX_QUEUED;
  };
  extern MarlinSerial<MarlinSerialCfg<SERIAL_PORT>> customizedSerial1;

  #ifdef SERIAL_PORT_2

    extern MarlinSerial<MarlinSerialCfg<SERIAL_PORT_2>> customizedSerial2;

  #endif

#endif // !USBCON

// Use the UART for Bluetooth in AT90USB configurations
#if defined(USBCON) && ENABLED(BLUETOOTH)
  extern HardwareSerial bluetoothSerial;
#endif

#endif // _MARLINSERIAL_H_
