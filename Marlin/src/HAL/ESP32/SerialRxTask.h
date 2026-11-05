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

#include "../../core/serial_hook.h"

/**
 * For EMERGENCY_PARSER on a port from the Arduino-ESP32 core. The core gives no access
 * to received bytes from its interrupt, so a task on Marlin's core moves them into
 * rx_buffer, passing each to the emergency parser. Marlin reads from rx_buffer.
 *
 * PortT is the core's serial class. FinalT is the class derived from this one, which
 * is wrapped in a Serial1Class holding the parser state.
 */
template <class PortT, class FinalT>
class SerialRxTask : public PortT {
public:
  using PortT::PortT;

  template <typename... Args>
  void begin(Args... args) { rx_active = false; PortT::begin(args...); rx_start(); }
  template <typename... Args>
  void end(Args... args) { rx_active = false; PortT::end(args...); }

  int available() {
    if (!rx_task) return PortT::available();
    return (rx_head - rx_tail) & (RX_BUFFER_SIZE - 1);
  }

  int peek() {
    if (!rx_task) return PortT::peek();
    return rx_head == rx_tail ? -1 : rx_buffer[rx_tail];
  }

  int read() {
    if (!rx_task) return PortT::read();
    if (rx_head == rx_tail) return -1;
    const uint8_t c = rx_buffer[rx_tail];
    rx_tail = rx_next(rx_tail);
    return c;
  }

private:
  uint8_t rx_buffer[RX_BUFFER_SIZE];
  volatile uint16_t rx_head = 0, rx_tail = 0;
  volatile bool rx_active = false;
  TaskHandle_t rx_task = nullptr;

  static uint16_t rx_next(const uint16_t i) { return (i + 1) & (RX_BUFFER_SIZE - 1); }

  Serial1Class<FinalT>* wrapper() { return static_cast<Serial1Class<FinalT>*>(static_cast<FinalT*>(this)); }

  void rx_start() {
    if (!wrapper()->emergency_parser_enabled()) return;
    rx_head = rx_tail = 0;
    wrapper()->emergency_state = EmergencyParser::State::EP_RESET;
    rx_active = true;
    // Higher priority than the Marlin loop, so it runs like a receive interrupt does on other HALs
    if (!rx_task) xTaskCreatePinnedToCore(rx_task_loop, "SerialRX", 2048, this, 2, &rx_task, CONFIG_ARDUINO_RUNNING_CORE);
  }

  // Leave bytes in the core's buffer when rx_buffer is full
  void rx_poll() {
    while (rx_active && rx_next(rx_head) != rx_tail && PortT::available() > 0) {
      const int c = PortT::read();
      if (c < 0) break;
      emergency_parser.update(wrapper()->emergency_state, uint8_t(c));
      rx_buffer[rx_head] = uint8_t(c);
      rx_head = rx_next(rx_head);
    }
  }

  static void rx_task_loop(void *arg) {
    SerialRxTask * const s = static_cast<SerialRxTask*>(arg);
    for (;;) {
      s->rx_poll();
      vTaskDelay(1);
    }
  }
};
