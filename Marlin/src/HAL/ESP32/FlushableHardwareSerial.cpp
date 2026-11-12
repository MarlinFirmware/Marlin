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

#include "FlushableHardwareSerial.h"

typedef Serial1Class<FlushableHardwareSerial> MSerialT;

MSerialT flushableSerial(ENABLED(EMERGENCY_PARSER), 0);
#if SERIAL_PORT_2 == 2
  MSerialT flushableSerial2(ENABLED(EMERGENCY_PARSER), 2);
#else
  MSerialT flushableSerial2(false, 2); // Not a host port
#endif

#if ENABLED(EMERGENCY_PARSER)

  #include "../../feature/e_parser.h"

  #define RX_NEXT(i) (((i) + 1) & (RX_BUFFER_SIZE - 1))

  void FlushableHardwareSerial::rx_start() {
    MSerialT * const s = static_cast<MSerialT*>(this);
    if (!s->emergency_parser_enabled()) return;
    rx_head = rx_tail = 0;
    s->emergency_state = EmergencyParser::State::EP_RESET;
    rx_active = true;
    // Higher priority than the Marlin loop, so it runs like a receive interrupt does on other HALs
    if (!rx_task) xTaskCreatePinnedToCore(rx_task_loop, "SerialRX", 2048, this, 2, &rx_task, CONFIG_ARDUINO_RUNNING_CORE);
  }

  // Leave bytes in the core's buffer when rx_buffer is full
  void FlushableHardwareSerial::rx_poll() {
    while (rx_active && RX_NEXT(rx_head) != rx_tail && HardwareSerial::available() > 0) {
      const int c = HardwareSerial::read();
      if (c < 0) break;
      emergency_parser.update(static_cast<MSerialT*>(this)->emergency_state, uint8_t(c));
      rx_buffer[rx_head] = uint8_t(c);
      rx_head = RX_NEXT(rx_head);
    }
  }

  void FlushableHardwareSerial::rx_task_loop(void *arg) {
    FlushableHardwareSerial * const s = static_cast<FlushableHardwareSerial*>(arg);
    for (;;) {
      s->rx_poll();
      vTaskDelay(1);
    }
  }

  int FlushableHardwareSerial::available() {
    if (!rx_task) return HardwareSerial::available();
    return (rx_head - rx_tail) & (RX_BUFFER_SIZE - 1);
  }

  int FlushableHardwareSerial::peek() {
    if (!rx_task) return HardwareSerial::peek();
    return rx_head == rx_tail ? -1 : rx_buffer[rx_tail];
  }

  int FlushableHardwareSerial::read() {
    if (!rx_task) return HardwareSerial::read();
    if (rx_head == rx_tail) return -1;
    const uint8_t c = rx_buffer[rx_tail];
    rx_tail = RX_NEXT(rx_tail);
    return c;
  }

#endif // EMERGENCY_PARSER

#endif
