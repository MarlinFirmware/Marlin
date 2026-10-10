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
 * Marlin serial port over native USB (CDC) for Atmel SAM3U (AT91SAM3U4E)
 *
 * Selected by setting any of SERIAL_PORT, SERIAL_PORT_2 or SERIAL_PORT_3 to
 * -1. The device itself lives in usb/usb_cdc.cpp; this is only the thin
 * adapter onto Marlin's serial interface. There is one USB device, so all
 * slots set to -1 share it.
 */

#include "../../inc/MarlinConfig.h"
#include "../../core/serial_hook.h"

struct MarlinSerialUSB {
  void begin(const long);
  void end();
  int peek();
  int read();
  void flush();
  int available();
  size_t write(const uint8_t c);

  #if ENABLED(SERIAL_STATS_DROPPED_RX)
    FORCE_INLINE uint32_t dropped() { return 0; }
  #endif

  #if ENABLED(SERIAL_STATS_MAX_RX_QUEUED)
    FORCE_INLINE int rxMaxEnqueued() { return 0; }
  #endif
};

// USB may be assigned to any of the three slots; serial_ports.h resolves
// SERIAL_PORT_n == -1 to customizedSerial<n> via USB_SERIAL_PORT().
#if SERIAL_PORT == -1
  typedef Serial1Class<MarlinSerialUSB> MSerialUSB1;
  extern MSerialUSB1 customizedSerial1;
#endif
#if defined(SERIAL_PORT_2) && SERIAL_PORT_2 == -1
  typedef Serial1Class<MarlinSerialUSB> MSerialUSB2;
  extern MSerialUSB2 customizedSerial2;
#endif
#if defined(SERIAL_PORT_3) && SERIAL_PORT_3 == -1
  typedef Serial1Class<MarlinSerialUSB> MSerialUSB3;
  extern MSerialUSB3 customizedSerial3;
#endif
