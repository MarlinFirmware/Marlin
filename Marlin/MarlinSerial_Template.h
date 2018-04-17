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
 * Modified 15 April 2018 by Stephan Veigl (multi instance version)
 */

/*
 * This file should NEVER be included directly, but only via MarlinSerial.h!
 * So it is ensured, that the required macros are set correctly.
 */

#ifdef T_PORT
  #undef MarlinSerialX
  #define MarlinSerialX SERIAL_REGNAME(MarlinSerial,T_PORT,)
  class MarlinSerialX : public Stream {

    public:
      void begin(const long);
      void end();
      int peek(void);
      int read(void);
      void flush(void);
      int available(void);
      void checkRx(void);
      size_t write(uint8_t c);
      void flushTX();
      void writeNoHandshake(const uint8_t c);

      #if ENABLED(SERIAL_STATS_DROPPED_RX)
        FORCE_INLINE uint32_t dropped() { return rx_dropped_bytes; }
      #endif

      #if ENABLED(SERIAL_STATS_MAX_RX_QUEUED)
        FORCE_INLINE ring_buffer_pos_t rxMaxEnqueued() { return rx_max_enqueued; }
      #endif

    private:
      static ring_buffer_r rx_buffer;
      #if ENABLED(SERIAL_STATS_DROPPED_RX)
        static uint8_t rx_dropped_bytes;
      #endif
      #if ENABLED(SERIAL_STATS_MAX_RX_QUEUED)
        static ring_buffer_pos_t rx_max_enqueued;
      #endif
      friend FORCE_INLINE void store_rxd_char(void);

      #if TX_BUFFER_SIZE > 0
        static ring_buffer_t tx_buffer;
        static bool _written;
        friend FORCE_INLINE void _tx_udr_empty_irq(void);
        int availableForWrite();
      #endif

      #if ENABLED(SERIAL_XON_XOFF)
        static uint8_t xon_xoff_state;
      #endif
      
    public:
      size_t write(const uint8_t *buffer, size_t size) { size_t count = size; while (count--) write(*buffer++); return size; }

      // interrupt functions
      static FORCE_INLINE void store_rxd_char(void);
      static FORCE_INLINE void tx_udr_empty_irq(void);
  };

  extern MarlinSerialX SERIAL_REGNAME(customizedSerial,T_PORT,);

#endif // T_PORT
