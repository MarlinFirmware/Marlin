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

#ifdef ARDUINO_ARCH_ESP32

#include "../../inc/MarlinConfig.h"
#include "FlushableHardwareSerial.h"

#if MB(MKS_TINYBEE)
  // Emitted once per TinyBee build so the log shows which path was compiled in.
  #ifndef MARLIN_ESP32_RX_QUEUE_LEN
    #pragma message "TinyBee: UART0 RX buffer NOT resized (RX_BUFFER_SIZE <= 256 or MARLIN_ESP32_KEEP_STOCK_RX_BUFFER)"
  #elif MARLIN_ESP32_CORE_MAJOR == 1
    #pragma message "TinyBee: UART0 RX queue sized from RX_BUFFER_SIZE (arduino-esp32 1.0.x path, uartBegin)"
  #else
    #pragma message "TinyBee: UART0 RX buffer sized from RX_BUFFER_SIZE (arduino-esp32 2.0.x path, setRxBufferSize)"
  #endif
#endif

Serial1Class<FlushableHardwareSerial> flushableSerial(false, 0);
Serial1Class<FlushableHardwareSerial> flushableSerial2(false, 2);

#endif
