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

#if MB(MKS_TINYBEE)

  #if __has_include(<esp_arduino_version.h>)
    #include <esp_arduino_version.h>   // Only exists from arduino-esp32 2.0.0
  #endif

  /**
   * MKS TinyBee: Marlin's RX_BUFFER_SIZE never reaches the ESP32 host port (UART0).
   *
   * MYSERIAL1 is flushableSerial (HAL/ESP32/HAL.h), and MarlinCore calls
   * MYSERIAL1.begin(BAUDRATE) without any buffer size, so whatever the Arduino
   * core picks as its default is what the printer actually gets. Both core
   * generations pick something too small and then drop the surplus silently:
   *
   *  - arduino-esp32 1.0.x: HardwareSerial::begin() hands a literal 256 to
   *    uartBegin(), which does xQueueCreate(256, 1). When that queue is full the
   *    RX ISR still drains the hardware FIFO and throws the byte away, with no
   *    error flag and no counter.
   *
   *  - arduino-esp32 2.0.x: begin() passes the _rxBufferSize member (256 by
   *    default) to uartBegin(), which becomes the IDF driver's ring buffer. The
   *    IDF driver at least raises UART_BUFFER_FULL / UART_FIFO_OVF events, but
   *    Marlin installs no onReceiveError() handler, so the loss is silent again.
   *
   * It can fill up because the I2S stepper task (HAL/ESP32/i2s.cpp) is pinned to
   * the same core and the same priority (1) as the Arduino loopTask that runs
   * GCodeQueue, and it spins through a whole DMA buffer (~4 ms) before it blocks
   * again. On the host this surfaces as "Line Number is not Last Line Number+1",
   * "checksum mismatch", "Resend:" and aborted prints.
   *
   * The two core generations need different fixes, see the class below.
   */

  // arduino-esp32 1.0.x has no esp_arduino_version.h, hence no version macros.
  #ifdef ESP_ARDUINO_VERSION_MAJOR
    #define MARLIN_ESP32_CORE_MAJOR ESP_ARDUINO_VERSION_MAJOR
  #else
    #define MARLIN_ESP32_CORE_MAJOR 1
  #endif

  #if defined(RX_BUFFER_SIZE) && RX_BUFFER_SIZE > 256 && DISABLED(MARLIN_ESP32_KEEP_STOCK_RX_BUFFER)
    #define MARLIN_ESP32_RX_QUEUE_LEN RX_BUFFER_SIZE
  #endif

  #ifdef MARLIN_ESP32_RX_QUEUE_LEN

    #if MARLIN_ESP32_CORE_MAJOR == 1

      #include <esp32-hal-uart.h>

      #if RX_BUFFER_SIZE > 65535
        #error "RX_BUFFER_SIZE must be <= 65535 on arduino-esp32 1.0.x (uartBegin() takes a uint16_t queue length)."
      #endif

    #elif MARLIN_ESP32_CORE_MAJOR == 2

      #include <soc/soc_caps.h>

      #if RX_BUFFER_SIZE <= SOC_UART_FIFO_LEN
        #error "RX_BUFFER_SIZE must be > SOC_UART_FIFO_LEN or setRxBufferSize() rejects it."
      #endif
      #if RX_BUFFER_SIZE > 65535
        #error "RX_BUFFER_SIZE must be <= 65535 (uartBegin() takes a uint16_t rx_buffer_size)."
      #endif

    #else

      // Never let this fail quietly: a silently ineffective RX_BUFFER_SIZE is the
      // exact bug this file exists to fix.
      #error "Untested arduino-esp32 major version. Verify how HardwareSerial::begin() sizes the RX buffer, then extend FlushableHardwareSerial. Define MARLIN_ESP32_KEEP_STOCK_RX_BUFFER to build without the fix."

    #endif

  #endif

#endif // MARLIN_ESP32_TINYBEE_FIXES

class FlushableHardwareSerial : public HardwareSerial {
public:
  FlushableHardwareSerial(int uart_nr) : HardwareSerial(uart_nr) {}

  #if MB(MKS_TINYBEE) && defined(MARLIN_ESP32_RX_QUEUE_LEN)
    #if MARLIN_ESP32_CORE_MAJOR == 1

      /**
       * 1.0.x: the 256 is a literal inside begin(), and setRxBufferSize() maps to
       * uartResizeRxBuffer(), which does vQueueDelete() + xQueueCreate() while the
       * RX ISR is live and would dereference the freed handle. So run the stock
       * begin() (it resolves the pins and, from 1.0.5 on, records _rx_pin/_tx_pin
       * for a later end()), then tear it down through end() and re-run uartBegin()
       * with the real size.
       */
      void begin(unsigned long baud, uint32_t config=SERIAL_8N1, int8_t rxPin=-1, int8_t txPin=-1, bool invert=false, unsigned long timeout_ms=20000UL) {
        HardwareSerial::begin(baud, config, rxPin, txPin, invert, timeout_ms);

        // Baud detection (baud == 0) and the remapped UART1/UART2 (TMC UART) keep
        // stock behavior. A failed begin() leaves _uart NULL; nothing to redo.
        if (_uart_nr != 0 || !baud || !_uart) return;

        if (rxPin < 0 && txPin < 0) { rxPin = 3; txPin = 1; }  // ESP32 UART0 default pads

        HardwareSerial::end();
        _uart = uartBegin(0, baud, config, rxPin, txPin, MARLIN_ESP32_RX_QUEUE_LEN, invert);
      }

    #else

      /**
       * 2.0.x: begin() forwards the _rxBufferSize member to uartBegin(), so the
       * size only has to be recorded first. setRxBufferSize() refuses to run once
       * the driver is installed (it returns 0 and logs), which is why it must come
       * before begin() and not after.
       */
      void begin(unsigned long baud, uint32_t config=SERIAL_8N1, int8_t rxPin=-1, int8_t txPin=-1, bool invert=false, unsigned long timeout_ms=20000UL, uint8_t rxfifo_full_thrhd=112) {
        if (_uart_nr == 0 && !_uart) setRxBufferSize(MARLIN_ESP32_RX_QUEUE_LEN);
        HardwareSerial::begin(baud, config, rxPin, txPin, invert, timeout_ms, rxfifo_full_thrhd);
      }

    #endif
  #endif
};

extern Serial1Class<FlushableHardwareSerial> flushableSerial;
extern Serial1Class<FlushableHardwareSerial> flushableSerial2;
