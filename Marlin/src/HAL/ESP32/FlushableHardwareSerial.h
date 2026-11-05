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

#include <HardwareSerial.h>

#include "../shared/Marduino.h"
#include "../../core/serial_hook.h"

class FlushableHardwareSerial : public HardwareSerial {
public:
  FlushableHardwareSerial(int uart_nr) : HardwareSerial(uart_nr) {}

  #if ENABLED(EMERGENCY_PARSER)
    // The core gives no access to received bytes from its UART interrupt, so a task
    // on Marlin's core moves them into rx_buffer, passing each to the emergency parser.
    template <typename... Args>
    void begin(Args... args) { rx_active = false; HardwareSerial::begin(args...); rx_start(); }
    template <typename... Args>
    void end(Args... args) { rx_active = false; HardwareSerial::end(args...); }
    int available();
    int peek();
    int read();

  private:
    uint8_t rx_buffer[RX_BUFFER_SIZE];
    volatile uint16_t rx_head = 0, rx_tail = 0;
    volatile bool rx_active = false;
    TaskHandle_t rx_task = nullptr;
    void rx_start();
    void rx_poll();
    static void rx_task_loop(void *arg);
  #endif
};

extern Serial1Class<FlushableHardwareSerial> flushableSerial;
extern Serial1Class<FlushableHardwareSerial> flushableSerial2;
