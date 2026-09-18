/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

// The AT32 Arduino core's HardwareSerial is a complete class (declared in
// libcore/HardwareSerial.h) but it does NOT inherit Arduino's Print/Stream.
// Marlin's serial hooks need a type that provides begin()/read()/write()/
// available()/end()/flush()/operator bool(), so we wrap it with an adapter
// rather than subclassing (the core already instantiates Serial1..Serial5 as
// globals, so we wrap those by reference).
#include "../../inc/MarlinConfig.h"
#include "../../core/serial_hook.h"
#include <libcore/HardwareSerial.h>

struct AT32SerialAdapter {
  HardwareSerial* const _uart;

  AT32SerialAdapter(HardwareSerial* uart) : _uart(uart) {}

  void begin(uint32_t baud) { _uart->begin(baud); }
  void end() {} // Not provided by AT32 core; no-op

  int     available()        { return _uart->available(); }
  int     read()             { return _uart->read(); }
  size_t  write(uint8_t c)   { _uart->write(c); return 1; }
  size_t  write(const uint8_t* buf, size_t len) { _uart->write(buf, (uint16_t)len); return len; }
  void    flush() {}         // Not provided by AT32 core; no-op

  operator bool() { return true; }
};

typedef ForwardSerial1Class<AT32SerialAdapter> MSerialT;

// Serial port index range (AT32 core provides Serial1–Serial5 and Serial7)
#define SERIAL_INDEX_MIN 1
#define SERIAL_INDEX_MAX 5

// Declare MSerial1–MSerial5 as ForwardSerial instances
#define DECLARE_SERIAL(X) extern MSerialT MSerial##X

#include "../shared/serial_ports.h"

extern MSerialT MSerial1;
extern MSerialT MSerial2;
extern MSerialT MSerial3;
extern MSerialT MSerial4;
extern MSerialT MSerial5;

#if defined(LCD_SERIAL_PORT) && ANY(HAS_DGUS_LCD, EXTENSIBLE_UI)
  #define LCD_SERIAL_TX_BUFFER_FREE() LCD_SERIAL.availableForWrite()
#endif
