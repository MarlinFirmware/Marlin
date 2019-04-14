/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#pragma once

#include "../../inc/MarlinConfig.h"

#include "Stream.h"

#ifndef RX_BUFFER_SIZE
  #define RX_BUFFER_SIZE 128
#endif
#ifndef TX_BUFFER_SIZE
  #define TX_BUFFER_SIZE 32
#endif
#if TX_BUFFER_SIZE <= 0
  #error "TX_BUFFER_SIZE is required for the WebSocket."
#endif

typedef uint16_t ring_buffer_pos_t;

class RingBuffer {
  uint8_t *data;
  ring_buffer_pos_t size;
  ring_buffer_pos_t head, tail;

public:
  RingBuffer(ring_buffer_pos_t size);
  ~RingBuffer();
  void write(const uint8_t *buffer, ring_buffer_pos_t size);
  void write(uint8_t c);
  int available(void);
  int peek(void);
  int read(void);
  int read(uint8_t *buffer, ring_buffer_pos_t *size);
  void flush(void);
};

class WebSocketSerial: public Stream {
  RingBuffer rx_buffer;
  RingBuffer tx_buffer;

public:
  WebSocketSerial();
  void begin(const long);
  void end();
  int available(void);
  int peek(void);
  int read(void);
  void flush(void);
  void flushTX(void);
  size_t write(const uint8_t c);

  operator bool() { return true; }

  FORCE_INLINE size_t write(const uint8_t* buffer, size_t size) {
    for(size_t i = 0; i < size; i++) {
      write(buffer[i]);
    }
    return size;
  }

  #if ENABLED(SERIAL_STATS_DROPPED_RX)
    FORCE_INLINE uint32_t dropped() { return 0; }
  #endif

  #if ENABLED(SERIAL_STATS_MAX_RX_QUEUED)
    FORCE_INLINE int rxMaxEnqueued() { return 0; }
  #endif
};

extern WebSocketSerial webSocketSerial;
