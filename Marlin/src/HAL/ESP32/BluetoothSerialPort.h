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

#include <Stream.h>

#include "../shared/Marduino.h"
#include "../../core/serial_hook.h"

/**
 * Bluetooth Classic (SPP) serial port, using the ESP32's built-in Bluetooth.
 *
 * Output goes into a buffer and a separate task sends it, so a host that stops
 * reading can't stall Marlin. Output that doesn't fit in the buffer is dropped.
 */
class BluetoothSerialPort : public Stream {
public:
  void begin(const long);
  void end() {}
  int available();
  int peek();
  int read();
  void flush() {}
  size_t write(const uint8_t c) { return write(&c, 1); }
  size_t write(const uint8_t *buffer, size_t size);
  bool connected() { return true; }
};

extern Serial1Class<BluetoothSerialPort> btSerial;
