/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2018 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * UART support
  */

#ifndef _HAL_UART_H_
#define _HAL_UART_H_


#include "../../inc/MarlinConfigPre.h"

#ifdef USE_UART

#include "../../inc/MarlinConfigPre.h"

#include <WString.h>

// The presence of the UBRRH register is used to detect a UART.
#define MUART_PRESENT(port) ((port == 0 && (defined(UBRRH) || defined(UBRR0H))) || \
                            (port == 1 && defined(UBRR1H)) || (port == 2 && defined(UBRR2H)) || \
                            (port == 3 && defined(UBRR3H)))

// These are macros to build serial port register names for the selected USE_UART (C preprocessor
// requires two levels of indirection to expand macro values properly)
#define MSERIAL_REGNAME(registerbase,number,suffix) MSERIAL_REGNAME_INTERNAL(registerbase,number,suffix)
#if USE_UART == 0 && (!defined(UBRR0H) || !defined(UDR0)) // use un-numbered registers if necessary
  #define MSERIAL_REGNAME_INTERNAL(registerbase,number,suffix) registerbase##suffix
#else
  #define MSERIAL_REGNAME_INTERNAL(registerbase,number,suffix) registerbase##number##suffix
#endif

// Registers used by MarlinSerial class (expanded depending on selected serial port)
#define MM_UCSRxA           MSERIAL_REGNAME(UCSR,USE_UART,A) // defines MM_UCSRxA to be UCSRnA where n is the serial port number
#define MM_UCSRxB           MSERIAL_REGNAME(UCSR,USE_UART,B)
#define MM_RXENx            MSERIAL_REGNAME(RXEN,USE_UART,)
#define MM_TXENx            MSERIAL_REGNAME(TXEN,USE_UART,)
#define MM_TXCx             MSERIAL_REGNAME(TXC,USE_UART,)
#define MM_RXCIEx           MSERIAL_REGNAME(RXCIE,USE_UART,)
#define MM_UDREx            MSERIAL_REGNAME(UDRE,USE_UART,)
#define MM_FEx              MSERIAL_REGNAME(FE,USE_UART,)
#define MM_DORx             MSERIAL_REGNAME(DOR,USE_UART,)
#define MM_UPEx             MSERIAL_REGNAME(UPE,USE_UART,)
#define MM_UDRIEx           MSERIAL_REGNAME(UDRIE,USE_UART,)
#define MM_UDRx             MSERIAL_REGNAME(UDR,USE_UART,)
#define MM_UBRRxH           MSERIAL_REGNAME(UBRR,USE_UART,H)
#define MM_UBRRxL           MSERIAL_REGNAME(UBRR,USE_UART,L)
#define MM_RXCx             MSERIAL_REGNAME(RXC,USE_UART,)
#define MM_USARTx_RX_vect   MSERIAL_REGNAME(USART,USE_UART,_RX_vect)
#define MM_U2Xx             MSERIAL_REGNAME(U2X,USE_UART,)
#define MM_USARTx_UDRE_vect MSERIAL_REGNAME(USART,USE_UART,_UDRE_vect)

#define M_DEC 10
#define M_HEX 16
#define M_OCT 8
#define M_BIN 2
#define M_BYTE 0

  // We're using a ring buffer (I think), in which rx_buffer_head is the index of the
  // location to which to write the next incoming character and rx_buffer_tail is the
  // index of the location from which to read.
  #if RX_BUFFER_SIZE > 256
    typedef uint16_t uart_ring_buffer_pos_t;
  #else
    typedef uint8_t uart_ring_buffer_pos_t;
  #endif

  class MarlinUART {

    public:
      MarlinUART() {};
      static void begin(const long);
      static void end();
      static int peek(void);
      static int read(void);
      static void flush(void);
      static uart_ring_buffer_pos_t available(void);
      static void write(const uint8_t c);
      static void flushTX(void);

      FORCE_INLINE static void write(const char* str) { while (*str) write(*str++); }
      FORCE_INLINE static void write(const uint8_t* buffer, size_t size) { while (size--) write(*buffer++); }
      FORCE_INLINE static void print(const String& s) { for (int i = 0; i < (int)s.length(); i++) write(s[i]); }
      FORCE_INLINE static void print(const char* str) { write(str); }

      operator bool() { return true; }

  };


extern MarlinUART marlinUART;



#endif // USE_UART

#endif // _HAL_UART_H_
