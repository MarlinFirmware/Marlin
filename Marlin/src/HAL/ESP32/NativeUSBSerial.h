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

#include "../shared/Marduino.h"
#include "../../core/serial_hook.h"

#if CONFIG_IDF_TARGET_ESP32S3
  #include <HWCDC.h>
  typedef HWCDC NativeUSBPort;
#else
  #include <USB.h>
  typedef USBCDC NativeUSBPort;
#endif

#if ENABLED(EMERGENCY_PARSER)
  #include "SerialRxTask.h"
  class NativeUSBSerial;
  typedef SerialRxTask<NativeUSBPort, NativeUSBSerial> NativeUSBBaseSerial;
#else
  typedef NativeUSBPort NativeUSBBaseSerial;
#endif

#if CONFIG_IDF_TARGET_ESP32S3

  /**
   * Native USB CDC serial port (USB Serial/JTAG) of the ESP32-S3
   */
  class NativeUSBSerial : public NativeUSBBaseSerial {
  public:
    void begin(const long baud) {
      setTxBufferSize(4096);
      NativeUSBBaseSerial::begin(baud);
      // A USB bus reset starts a new enumeration, so wait for the host to send again
      onEvent(ARDUINO_HW_CDC_BUS_RESET_EVENT, [](void *arg, esp_event_base_t, int32_t, void*) {
        static_cast<NativeUSBSerial*>(arg)->host_sent = false;
      });
    }

    int available() override {
      const int n = NativeUSBBaseSerial::available();
      if (n) host_sent = true;
      return n;
    }
    int read() override { host_sent = true; return NativeUSBBaseSerial::read(); }

    // The port gives no DTR, so there's no way to know whether a program has it open.
    // Output written while it's closed is held until a program opens it, and Linux echoes
    // it back before the program has set the port to raw, so Marlin would read its own
    // output (e.g., the boot messages) as G-code. So drop output until the host has sent
    // something: printer hosts send a command as soon as they open the port.
    //
    // HWCDC::write waits for buffer space while the host isn't reading, so don't let it.
    // Allow 1ms for a host that is reading to make room, then drop the output, and
    // keep dropping without waiting until there is room again.
    using HWCDC::write;
    size_t write(const uint8_t *buffer, size_t size) override {
      if (!host_sent) return size;
      if (availableForWrite() < int(size)) {
        if (!dropping) delay(1);
        if (availableForWrite() < int(size)) { dropping = true; return size; }
      }
      dropping = false;
      return HWCDC::write(buffer, size);
    }

    bool connected() { return true; } // Don't hold up startup waiting for a host

  private:
    volatile bool host_sent = false;
    bool dropping = false;
  };

#else

  /**
   * Native USB CDC serial port (USB OTG, TinyUSB) of the ESP32-S2
   */
  class NativeUSBSerial : public NativeUSBBaseSerial {
  public:
    void begin(const long baud) {
      NativeUSBBaseSerial::begin(baud);
      USB.begin();
    }

    // USBCDC::write drops output while no program has the port open (DTR off),
    // but waits for buffer space while one has it open and isn't reading, so
    // don't let it. Allow 1ms for a host that is reading to make room, then drop
    // the output, and keep dropping without waiting until there is room again.
    // The TX FIFO is only 64 bytes, so send longer output in pieces that fit.
    using USBCDC::write;
    size_t write(const uint8_t c) override { return write(&c, 1); }
    size_t write(const uint8_t *buffer, size_t size) override {
      if (!*this) return size;
      for (size_t sent = 0; sent < size;) {
        int room = availableForWrite();
        if (room <= 0 && !dropping) { delay(1); room = availableForWrite(); }
        if (room <= 0) { dropping = true; break; }
        dropping = false;
        const size_t n = _MIN(size - sent, size_t(room));
        USBCDC::write(buffer + sent, n);
        sent += n;
      }
      return size;
    }

    bool connected() { return true; } // Don't hold up startup waiting for a host

  private:
    bool dropping = false;
  };

#endif

extern Serial1Class<NativeUSBSerial> nativeUSBSerial;
