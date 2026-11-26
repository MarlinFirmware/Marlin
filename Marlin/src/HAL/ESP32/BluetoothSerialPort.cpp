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
#ifdef ARDUINO_ARCH_ESP32

#include "../../inc/MarlinConfig.h"

#if HAS_BLUETOOTH_SERIAL

#include "BluetoothSerialPort.h"
#include <BluetoothSerial.h>
#include <freertos/ringbuf.h>

#ifndef BLUETOOTH_NAME
  #define BLUETOOTH_NAME MACHINE_NAME
#endif
#ifndef BLUETOOTH_TX_BUFFER_SIZE
  #define BLUETOOTH_TX_BUFFER_SIZE 2048
#endif

Serial1Class<BluetoothSerialPort> btSerial(false);

static BluetoothSerial bt;
static RingbufHandle_t tx_ring;

// Send buffered output. BluetoothSerial::write can wait when the host isn't reading.
static void bt_tx_task(void*) {
  for (;;) {
    size_t len;
    uint8_t * const data = (uint8_t*)xRingbufferReceiveUpTo(tx_ring, &len, portMAX_DELAY, 256);
    if (data) {
      bt.write(data, len);
      vRingbufferReturnItem(tx_ring, data);
    }
  }
}

void BluetoothSerialPort::begin(const long) {
  if (tx_ring) return;
  tx_ring = xRingbufferCreate(BLUETOOTH_TX_BUFFER_SIZE, RINGBUF_TYPE_BYTEBUF);
  bt.begin(BLUETOOTH_NAME);
  xTaskCreatePinnedToCore(bt_tx_task, "BT TX", 2048, nullptr, 1, nullptr, 0);
}

int BluetoothSerialPort::available() { return bt.available(); }
int BluetoothSerialPort::peek() { return bt.peek(); }
int BluetoothSerialPort::read() { return bt.read(); }

size_t BluetoothSerialPort::write(const uint8_t *buffer, size_t size) {
  // Without a host, or with the buffer full, the output is dropped
  if (tx_ring && bt.hasClient()) xRingbufferSend(tx_ring, buffer, size, 0);
  return size;
}

#endif // HAS_BLUETOOTH_SERIAL
#endif // ARDUINO_ARCH_ESP32
